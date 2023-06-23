// usb_keyboard.c
#include "usb_keyboard.h"
#include "midi.h"  // from extmod/tulip/


typedef union {
    struct {
        uint8_t bLength;                    /**< Size of the descriptor in bytes */
        uint8_t bDescriptorType;            /**< Constant name specifying type of HID descriptor. */
        uint16_t bcdHID;                    /**< USB HID Specification Release Number in Binary-Coded Decimal (i.e., 2.10 is 210H) */
        uint8_t bCountryCode;               /**< Numeric expression identifying country code of the localized hardware. */
        uint8_t bNumDescriptor;             /**< Numeric expression specifying the number of class descriptors. */
        uint8_t bHIDDescriptorType;         /**< Constant name identifying type of class descriptor. See Section 7.1.2: Set_Descriptor Request for a table of class descriptor constants. */
        uint16_t wHIDDescriptorLength;      /**< Numeric expression that is the total size of the Report descriptor. */
        uint8_t bHIDDescriptorTypeOpt;      /**< Optional constant name identifying type of class descriptor. See Section 7.1.2: Set_Descriptor Request for a table of class descriptor constants. */
        uint16_t wHIDDescriptorLengthOpt;   /**< Optional numeric expression that is the total size of the Report descriptor. */
    } USB_DESC_ATTR;
    uint8_t val[9];
} usb_hid_desc_t;

#ifdef DEBUG_USB
// from show_desc.hpp
// from https://github.com/touchgadget/esp32-usb-host-demos

void show_config_desc(const void *p)
{
  printf("*** show_config_desc:\n");
  const usb_config_desc_t *config_desc = (const usb_config_desc_t *)p;

  printf("bLength: %d\n", config_desc->bLength);
  printf("bDescriptorType(config): %d\n", config_desc->bDescriptorType);
  printf("wTotalLength: %d\n", config_desc->wTotalLength);
  printf("bNumInterfaces: %d\n", config_desc->bNumInterfaces);
  printf("bConfigurationValue: %d\n", config_desc->bConfigurationValue);
  printf("iConfiguration: %d\n", config_desc->iConfiguration);
  printf("bmAttributes(%s%s%s): 0x%02x\n",
      (config_desc->bmAttributes & USB_BM_ATTRIBUTES_SELFPOWER)?"Self Powered":"",
      (config_desc->bmAttributes & USB_BM_ATTRIBUTES_WAKEUP)?", Remote Wakeup":"",
      (config_desc->bmAttributes & USB_BM_ATTRIBUTES_BATTERY)?", Battery Powered":"",
      config_desc->bmAttributes);
  printf("bMaxPower: %d = %d mA\n", config_desc->bMaxPower, config_desc->bMaxPower*2);
  printf("***\n");
}

uint8_t show_interface_desc(const void *p)
{
  printf("*** show_interface_desc:\n");
  const usb_intf_desc_t *intf = (const usb_intf_desc_t *)p;

  printf("bLength: %d\n", intf->bLength);
  printf("bDescriptorType (interface): %d\n", intf->bDescriptorType);
  printf("bInterfaceNumber: %d\n", intf->bInterfaceNumber);
  printf("bAlternateSetting: %d\n", intf->bAlternateSetting);
  printf("bNumEndpoints: %d\n", intf->bNumEndpoints);
  printf("bInterfaceClass: 0x%02x\n", intf->bInterfaceClass);
  printf("bInterfaceSubClass: 0x%02x\n", intf->bInterfaceSubClass);
  printf("bInterfaceProtocol: 0x%02x\n", intf->bInterfaceProtocol);
  printf("iInterface: %d\n", intf->iInterface);
  printf("***\n");
  return intf->bInterfaceClass;
}

void show_endpoint_desc(const void *p)
{
  printf("*** show_endpoint_desc:\n");
  const usb_ep_desc_t *endpoint = (const usb_ep_desc_t *)p;
  const char *XFER_TYPE_NAMES[] = {
    "Control", "Isochronous", "Bulk", "Interrupt"
  };
  printf("bLength: %d\n", endpoint->bLength);
  printf("bDescriptorType (endpoint): %d\n", endpoint->bDescriptorType);
  printf("bEndpointAddress(%s): 0x%02x\n",
    (endpoint->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK)?"In":"Out",
    endpoint->bEndpointAddress);
  printf("bmAttributes(%s): 0x%02x\n",
      XFER_TYPE_NAMES[endpoint->bmAttributes & USB_BM_ATTRIBUTES_XFERTYPE_MASK],
      endpoint->bmAttributes);
  printf("wMaxPacketSize: %d\n", endpoint->wMaxPacketSize);
  printf("bInterval: %d\n", endpoint->bInterval);
  printf("***\n");
}

// end of show_desc.hpp
#else  // !DEBUG_USB
#define show_config_desc(p)  /* nothing */
#define show_interface_desc(p)  /* nothing */
#define show_endpoint_desc(p)  /* nothing */

#endif // DEBUG_USB

const TickType_t HOST_EVENT_TIMEOUT = 1;
const TickType_t CLIENT_EVENT_TIMEOUT = 1;

const size_t USB_HID_DESC_SIZE = 9;

usb_host_client_handle_t Client_Handle;
usb_device_handle_t Device_Handle;
static usb_host_enum_cb_t _USB_host_enumerate;
bool isKeyboard = false;
bool isKeyboardReady = false;
uint8_t KeyboardInterval;
bool isKeyboardPolling = false;
int64_t KeyboardTimer=0;
int64_t KeyRepeatTimer=0;



//const size_t KEYBOARD_IN_BUFFER_SIZE = KEYBOARD_BYTES; 
usb_transfer_t *KeyboardIn = NULL;

// ---------------------------------
// from usbhmidi.ino
bool isMIDI = false;
bool isMIDIReady = false;

//const size_t MIDI_IN_BUFFERS = 8;
//const size_t MIDI_OUT_BUFFERS = 8;
#define MIDI_IN_BUFFERS 8
#define MIDI_OUT_BUFFERS 8
usb_transfer_t *MIDIOut = NULL;
usb_transfer_t *MIDIIn[MIDI_IN_BUFFERS] = { NULL };

// USB MIDI Event Packet Format (always 4 bytes)
//
// Byte 0 |Byte 1 |Byte 2 |Byte 3
// -------|-------|-------|------
// CN+CIN |MIDI_0 |MIDI_1 |MIDI_2
//
// CN = Cable Number ((0x0..0xf)<<4) specifies virtual MIDI jack/cable
// CIN = Code Index Number (0x0..0xf) classifies the 3 MIDI bytes.
// See Table 4-1 in the MIDI 1.0 spec at usb.org.
//

static void midi_transfer_cb(usb_transfer_t *transfer) {
  //printf("**midi_transfer_cb context: %p\n", transfer->context);
  if (Device_Handle == transfer->device_handle) {
    int in_xfer = transfer->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK;
    if ((transfer->status == 0) && in_xfer) {
      uint8_t *const p = transfer->data_buffer;
      for (int i = 0; i < transfer->actual_num_bytes; i += 4) {
        if ((p[i] + p[i + 1] + p[i + 2] + p[i + 3]) == 0) break;
        printf("midi: %02x %02x %02x %02x\n",
                 p[i], p[i + 1], p[i + 2], p[i + 3]);
        // Strip the first byte which is the USB-MIDI virtual wire ID,
        // rest is MIDI message (at least for 3-byte messages).
        callback_midi_message_received(p + i + 1, 3);
      }
      esp_err_t err = usb_host_transfer_submit(transfer);
      if (err != ESP_OK) {
        printf("usb_host_transfer_submit In fail: %x\n", err);
      }
    } else {
      printf("transfer->status %d\n", transfer->status);
    }
  }
}

bool check_interface_desc_MIDI(const void *p) {
  const usb_intf_desc_t *intf = (const usb_intf_desc_t *)p;

  // USB MIDI
  if ((intf->bInterfaceClass == USB_CLASS_AUDIO) && (intf->bInterfaceSubClass == 3) && (intf->bInterfaceProtocol == 0)) {
    isMIDI = true;
    printf("Claiming a MIDI device!\n");
    esp_err_t err = usb_host_interface_claim(Client_Handle, Device_Handle,
                                             intf->bInterfaceNumber, intf->bAlternateSetting);
    if (err != ESP_OK) printf("usb_host_interface_claim failed: %x\n", err);
    return true;
  }
  return false;
}

void prepare_endpoint_midi(const void *p) {
  const usb_ep_desc_t *endpoint = (const usb_ep_desc_t *)p;
  esp_err_t err;

  // must be bulk for MIDI
  if ((endpoint->bmAttributes & USB_BM_ATTRIBUTES_XFERTYPE_MASK) != USB_BM_ATTRIBUTES_XFER_BULK) {
    printf("Not bulk endpoint: 0x%02x\n", endpoint->bmAttributes);
    return;
  }
  if (endpoint->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK) {
    for (int i = 0; i < MIDI_IN_BUFFERS; i++) {
      err = usb_host_transfer_alloc(endpoint->wMaxPacketSize, 0, &MIDIIn[i]);
      if (err != ESP_OK) {
        MIDIIn[i] = NULL;
        printf("usb_host_transfer_alloc In fail: %x\n", err);
      } else {
        MIDIIn[i]->device_handle = Device_Handle;
        MIDIIn[i]->bEndpointAddress = endpoint->bEndpointAddress;
        MIDIIn[i]->callback = midi_transfer_cb;
        MIDIIn[i]->context = (void *)i;
        MIDIIn[i]->num_bytes = endpoint->wMaxPacketSize;
        esp_err_t err = usb_host_transfer_submit(MIDIIn[i]);
        if (err != ESP_OK) {
          printf("usb_host_transfer_submit In fail: %x\n", err);
        }
      }
    }
  } else {
    err = usb_host_transfer_alloc(endpoint->wMaxPacketSize, 0, &MIDIOut);
    if (err != ESP_OK) {
      MIDIOut = NULL;
      printf("usb_host_transfer_alloc Out fail: %x\n", err);
      return;
    }
    printf("Out data_buffer_size: %d\n", MIDIOut->data_buffer_size);
    MIDIOut->device_handle = Device_Handle;
    MIDIOut->bEndpointAddress = endpoint->bEndpointAddress;
    MIDIOut->callback = midi_transfer_cb;
    MIDIOut->context = NULL;
    //    MIDIOut->flags |= USB_TRANSFER_FLAG_ZERO_PACK;
  }
  isMIDIReady = ((MIDIOut != NULL) && (MIDIIn[0] != NULL));
}

// end of usbhmidi.ino
// ---------------------------------


void _client_event_callback(const usb_host_client_event_msg_t *event_msg, void *arg)
{
  esp_err_t err;
  switch (event_msg->event)
  {
    /**< A new device has been enumerated and added to the USB Host Library */
    case USB_HOST_CLIENT_EVENT_NEW_DEV:
      //printf("New device address: %d\n", event_msg->new_dev.address);
      err = usb_host_device_open(Client_Handle, event_msg->new_dev.address, &Device_Handle);
      if (err != ESP_OK) printf("usb_host_device_open: %x\n", err);

      usb_device_info_t dev_info;
      err = usb_host_device_info(Device_Handle, &dev_info);
      if (err != ESP_OK) printf("usb_host_device_info: %x\n", err);
      //printf("speed: %d dev_addr %d vMaxPacketSize0 %d bConfigurationValue %d\n",
      //    dev_info.speed, dev_info.dev_addr, dev_info.bMaxPacketSize0,
      //    dev_info.bConfigurationValue);

      const usb_device_desc_t *dev_desc;
      err = usb_host_get_device_descriptor(Device_Handle, &dev_desc);
      if (err != ESP_OK) printf("usb_host_get_device_desc: %x\n", err);

      const usb_config_desc_t *config_desc;
      err = usb_host_get_active_config_descriptor(Device_Handle, &config_desc);
      if (err != ESP_OK) printf("usb_host_get_config_desc: %x\n", err);
      (*_USB_host_enumerate)(config_desc);
      break;
    /**< A device opened by the client is now gone */
    case USB_HOST_CLIENT_EVENT_DEV_GONE:
      printf("Device Gone handle: %lx\n", (uint32_t)event_msg->dev_gone.dev_hdl);
      break;
    default:
      printf("Unknown value %d\n", event_msg->event);
      break;
  }
}

// Reference: esp-idf/examples/peripherals/usb/host/usb_host_lib/main/usb_host_lib_main.c

void usbh_setup(usb_host_enum_cb_t enumeration_cb)
{
  const usb_host_config_t config = {
    .intr_flags = ESP_INTR_FLAG_LEVEL1,
  };
  esp_err_t err = usb_host_install(&config);
  printf("usb_host_install: %x\n", err);

  const usb_host_client_config_t client_config = {
    .is_synchronous = false,
    .max_num_event_msg = 5,
    .async = {
        .client_event_callback = _client_event_callback,
        .callback_arg = Client_Handle
    }
  };
  err = usb_host_client_register(&client_config, &Client_Handle);
  printf("usb_host_client_register: %x\n", err);

  _USB_host_enumerate = enumeration_cb;
}

void usbh_task(void)
{
  uint32_t event_flags;

  esp_err_t err = usb_host_lib_handle_events(HOST_EVENT_TIMEOUT, &event_flags);
  if (err == ESP_OK) {
    if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS) {
      printf("No more clients\n");
    }
    if (event_flags & USB_HOST_LIB_EVENT_FLAGS_ALL_FREE) {
      printf("No more devices\n");
    }
  }
  else {
    if (err != ESP_ERR_TIMEOUT) {
      printf("usb_host_lib_handle_events: %x flags: %lx\n", err, event_flags);
    }
  }

  err = usb_host_client_handle_events(Client_Handle, CLIENT_EVENT_TIMEOUT);
  if ((err != ESP_OK) && (err != ESP_ERR_TIMEOUT)) {
    printf("usb_host_client_handle_events: %x\n", err);
  }
}

// Keep track of which keys / scan codes are being held
uint16_t current_held = 0;
int64_t current_held_ms = 0;
int64_t last_inter_trigger_ms = 0;



void decode_report(uint8_t *p) {
    // First byte, modifier mask
    uint8_t modifier = p[0];
    uint8_t new_key = 0;
    uint8_t key_held_this_session = 0;
    // Second byte, reserved
    // next 6 bytes, scan codes (for rollover)
    //fprintf(stderr,"decode report %d %d %d %d %d %d\n", p[2],p[3],p[4],p[5],p[6],p[7]);
    for(uint8_t i=2;i<8;i++) {
        if(p[i]!=0) {
            key_held_this_session = 1;
            uint8_t skip = 0;
            for(uint8_t j=2;j<8;j++) {
        		if(last_scan[j] == p[i]) skip = 1;
  			}
	  		if(!skip) { // only process new keys
		        uint16_t c = scan_ascii(p[i], modifier);
		        if(c) {
                    new_key = 1;
                    current_held_ms = esp_timer_get_time()/1000;
                    current_held = c; 
                    //fprintf(stderr, "sending new key %d to MP at time %lld\n", c, current_held_ms);
                    send_key_to_micropython(c);
                }
            }	
		} 
    }
    if(!new_key && !key_held_this_session) {
        // we got a message but no new keys. so is a release
        //fprintf(stderr, "turning off key\n");
        current_held_ms = 0;
        current_held = 0;
        last_inter_trigger_ms = 0;
    }
    for(uint8_t i=0;i<8;i++) last_scan[i] = p[i];
}

void keyboard_transfer_cb(usb_transfer_t *transfer)
{
  if (Device_Handle == transfer->device_handle) {
    isKeyboardPolling = false;
    if (transfer->status == 0) {
        //fprintf(stderr, "nb is %d\n", transfer->actual_num_bytes);
      if (transfer->actual_num_bytes == 8 || transfer->actual_num_bytes == 16) {
        uint8_t *const p = transfer->data_buffer;
        decode_report(p);
      }
      else {
        printf("Keyboard boot hid transfer too short or long\n");
      }
    }
    else {
      printf("transfer->status %d\n", transfer->status);
    }
  }
}

bool check_interface_desc_boot_keyboard(const void *p)
{
  const usb_intf_desc_t *intf = (const usb_intf_desc_t *)p;

  if ((intf->bInterfaceClass == USB_CLASS_HID) &&
      (intf->bInterfaceSubClass == 1) &&
      (intf->bInterfaceProtocol == 1)) {
    isKeyboard = true;
    esp_err_t err = usb_host_interface_claim(Client_Handle, Device_Handle,
        intf->bInterfaceNumber, intf->bAlternateSetting);
    if (err != ESP_OK) printf("usb_host_interface_claim failed: %x\n", err);
    return true;
  }
  return false;
}

void prepare_endpoint_hid(const void *p)
{
  const usb_ep_desc_t *endpoint = (const usb_ep_desc_t *)p;
  esp_err_t err;

  // must be interrupt for HID
  if ((endpoint->bmAttributes & USB_BM_ATTRIBUTES_XFERTYPE_MASK) != USB_BM_ATTRIBUTES_XFER_INT) {
    printf("Not interrupt endpoint: 0x%02x\n", endpoint->bmAttributes);
    return;
  }
  if (endpoint->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK) {
    err = usb_host_transfer_alloc(KEYBOARD_BYTES, 0, &KeyboardIn);
    if (err != ESP_OK) {
      KeyboardIn = NULL;
      printf("usb_host_transfer_alloc In fail: %x\n", err);
      return;
    }
    KeyboardIn->device_handle = Device_Handle;
    KeyboardIn->bEndpointAddress = endpoint->bEndpointAddress;
    KeyboardIn->callback = keyboard_transfer_cb;
    KeyboardIn->context = NULL;
    isKeyboardReady = true;
    KeyboardInterval = endpoint->bInterval;
    //printf("USB boot keyboard ready\n");
  }
  else {
    printf("Ignoring interrupt Out endpoint\n");
  }
}

void show_config_desc_full(const usb_config_desc_t *config_desc)
{
  // Full decode of config desc.
  const uint8_t *p = &config_desc->val[0];
  uint8_t bLength;
  for (int i = 0; i < config_desc->wTotalLength; i+=bLength, p+=bLength) {
    bLength = *p;
    if ((i + bLength) <= config_desc->wTotalLength) {
      const uint8_t bDescriptorType = *(p + 1);
      switch (bDescriptorType) {
        case USB_B_DESCRIPTOR_TYPE_DEVICE:
          printf("USB Device Descriptor should not appear in config\n");
          break;
        case USB_B_DESCRIPTOR_TYPE_CONFIGURATION:
          show_config_desc(p);
          break;
        case USB_B_DESCRIPTOR_TYPE_STRING:
          printf("USB string desc TBD\n");
          break;
        case USB_B_DESCRIPTOR_TYPE_INTERFACE:
          show_interface_desc(p);
          if (!isMIDI && !isKeyboard) {
              if (!check_interface_desc_MIDI(p)) {
                  check_interface_desc_boot_keyboard(p);
              }
          }
          if (!isMIDI && !isKeyboard) {
              printf("Interface was neither keyboard nor midi.\n");
          }
          break;
        case USB_B_DESCRIPTOR_TYPE_ENDPOINT:
          show_endpoint_desc(p);
          if (isKeyboard && KeyboardIn == NULL) {
              prepare_endpoint_hid(p);
          } else if (isMIDI && !isMIDIReady) {
            prepare_endpoint_midi(p);
          }
          break;
        case USB_B_DESCRIPTOR_TYPE_DEVICE_QUALIFIER:
          printf("USB device qual desc TBD\n");
          break;
        case USB_B_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION:
          printf("USB Other Speed TBD\n");
          break;
        case USB_B_DESCRIPTOR_TYPE_INTERFACE_POWER:
          printf("USB Interface Power TBD\n");
          break;
        case 0x21:
          // HID 
          break;
        case 0x24:
          // UAS PIPE mode - see this for MIDI keyboard.
          break;
        case 0x25:
          // Also see this for MIDI keyboard.
          break;
        default:
          printf("Unknown USB Descriptor Type: 0x%x\n", bDescriptorType);
          break;
      }
    }
    else {
      printf("USB Descriptor invalid\n");
      return;
    }
  }
}

void run_usb()
{
  usbh_setup(show_config_desc_full);
  while(1) {
      usbh_task();
      KeyboardTimer = esp_timer_get_time() / 1000;
      KeyRepeatTimer = esp_timer_get_time() / 1000;

      // Handle key repeat
      if(current_held > 0) {
        if(KeyRepeatTimer - current_held_ms > KEY_REPEAT_TRIGGER_MS) {
            if(KeyRepeatTimer - last_inter_trigger_ms > KEY_REPEAT_INTER_MS) {
                send_key_to_micropython(current_held);
                last_inter_trigger_ms = KeyRepeatTimer;
            }
        }
      }
      if (isKeyboardReady && !isKeyboardPolling && (KeyboardTimer > KeyboardInterval)) {
        // vortex is 8
        // nuphy is 8 
        // keychron is 16
        // We need to discern this from the descriptor instead of hardcoding it
        KeyboardIn->num_bytes = KEYBOARD_BYTES; 
        esp_err_t err = usb_host_transfer_submit(KeyboardIn);
        if (err != ESP_OK) {
          printf("usb_host_transfer_submit In fail: %x\n", err);
        }
        isKeyboardPolling = true;
        KeyboardTimer = 0;
      }
    }
}
