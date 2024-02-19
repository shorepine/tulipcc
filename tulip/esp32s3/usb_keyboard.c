// usb_keyboard.c
#include "usb_keyboard.h"
#include "midi.h"  // from extmod/tulip/
#include "lvgl.h"
#define KEYBOARD_BUFFER_SIZE 32

uint16_t keyboard_bytes = KEYBOARD_BYTES;

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

//#define DEBUG_USB
#ifdef DEBUG_USB
// from show_desc.hpp
// from https://github.com/touchgadget/esp32-usb-host-demos

#define DBGPRINTF(s) printf(s)
#define DBGPRINTF1(s, a) printf(s, a)
#define DBGPRINTF2(s, a, b) printf(s, a, b)
#define DBGPRINTF3(s, a, b, c) printf(s, a, b, c)
#define DBGPRINTF4(s, a, b, c, d) printf(s, a, b, c, d)

void show_config_desc(const void *p, int indent)
{
    char prefix[8];
    for (int i=0; i < indent; ++i) prefix[i] = '*';
    prefix[indent] = '\0';
  const usb_config_desc_t *config_desc = (const usb_config_desc_t *)p;
  fprintf(stderr, "%s CONFIG_DESC(size=%d)\n", prefix, config_desc->bLength);
  //fprintf(stderr, "bDescriptorType(config): %d\n", config_desc->bDescriptorType);
  fprintf(stderr, "%s wTotalLength: %d\n", prefix, config_desc->wTotalLength);
  fprintf(stderr, "%s bNumInterfaces: %d\n", prefix, config_desc->bNumInterfaces);
  fprintf(stderr, "%s bConfigurationValue: %d\n", prefix, config_desc->bConfigurationValue);
  fprintf(stderr, "%s iConfiguration: %d\n", prefix, config_desc->iConfiguration);
  fprintf(stderr, "%s bmAttributes: 0x%02x (%s%s%s)\n", prefix, 
          config_desc->bmAttributes,
          (config_desc->bmAttributes & USB_BM_ATTRIBUTES_SELFPOWER)?"Self Powered, ":"",
          (config_desc->bmAttributes & USB_BM_ATTRIBUTES_WAKEUP)?"Remote Wakeup, ":"",
          (config_desc->bmAttributes & USB_BM_ATTRIBUTES_BATTERY)?"Battery Powered":"");
  fprintf(stderr,"%s bMaxPower: %d (%d mA)\n", prefix, config_desc->bMaxPower, config_desc->bMaxPower*2);
}

uint8_t show_interface_desc(const void *p, int indent)
{
    char prefix[8];
    for (int i=0; i < indent; ++i) prefix[i] = '*';
    prefix[indent] = '\0';
  const usb_intf_desc_t *intf = (const usb_intf_desc_t *)p;

  fprintf(stderr, "%s INTERFACE_DESC(size=%d)\n", prefix, intf->bLength);
  //fprintf(stderr, "%s bDescriptorType (interface): %d\n", intf->bDescriptorType);
  fprintf(stderr, "%s bInterfaceNumber: %d\n", prefix, intf->bInterfaceNumber);
  fprintf(stderr, "%s bAlternateSetting: %d\n", prefix, intf->bAlternateSetting);
  fprintf(stderr, "%s bNumEndpoints: %d\n", prefix, intf->bNumEndpoints);
  fprintf(stderr, "%s bInterfaceClass: 0x%02x\n", prefix, intf->bInterfaceClass);
  fprintf(stderr, "%s bInterfaceSubClass: 0x%02x\n", prefix, intf->bInterfaceSubClass);
  fprintf(stderr, "%s bInterfaceProtocol: 0x%02x\n", prefix, intf->bInterfaceProtocol);
  fprintf(stderr, "%s iInterface: %d\n", prefix, intf->iInterface);
  return intf->bInterfaceClass;
}

void show_endpoint_desc(const void *p, int indent)
{
    char prefix[8];
    for (int i=0; i < indent; ++i) prefix[i] = '*';
    prefix[indent] = '\0';
  const usb_ep_desc_t *endpoint = (const usb_ep_desc_t *)p;
  const char *XFER_TYPE_NAMES[] = {
    "Control", "Isochronous", "Bulk", "Interrupt"
  };
  fprintf(stderr, "%s ENDPOINT_DESC(size=%d)\n", prefix, endpoint->bLength);
  //fprintf(stderr, "%s bDescriptorType (endpoint): %d\n", prefix, endpoint->bDescriptorType);
  fprintf(stderr, "%s bEndpointAddress: 0x%02x (%s)\n", prefix,
          endpoint->bEndpointAddress,
          (endpoint->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK)?"In":"Out");
  fprintf(stderr, "%s bmAttributes: 0x%02x (%s)\n", prefix,
          endpoint->bmAttributes,
          XFER_TYPE_NAMES[endpoint->bmAttributes & USB_BM_ATTRIBUTES_XFERTYPE_MASK]);
  fprintf(stderr, "%s wMaxPacketSize: %d\n", prefix, endpoint->wMaxPacketSize);
  fprintf(stderr, "%s bInterval: %d\n", prefix, endpoint->bInterval);
}

// end of show_desc.hpp
#else  // !DEBUG_USB

#define DBGPRINTF(s)  /* nothing */
#define DBGPRINTF1(s, a)  /* nothing */
#define DBGPRINTF2(s, a, b)  /* nothing */
#define DBGPRINTF3(s, a, b, c)  /* nothing */
#define DBGPRINTF4(s, a, b, c, d)  /* nothing */

#define show_config_desc(p, i)  /* nothing */
#define show_interface_desc(p, i)  /* nothing */
#define show_endpoint_desc(p, i)  /* nothing */

#endif // DEBUG_USB

const TickType_t HOST_EVENT_TIMEOUT = 1;
const TickType_t CLIENT_EVENT_TIMEOUT = 1;

const size_t USB_HID_DESC_SIZE = 9;

usb_host_client_handle_t Client_Handle;
usb_device_handle_t Device_Handle;
uint8_t Interface_Number;

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
bool haveMIDIin = false;
bool haveMIDIout = false;
bool isMIDIReady = false;

//const size_t MIDI_IN_BUFFERS = 8;
//const size_t MIDI_OUT_BUFFERS = 8;
#define MIDI_IN_BUFFERS 8
//#define MIDI_OUT_BUFFERS 8
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
        DBGPRINTF4("midi: %02x %02x %02x %02x\n",
                   p[i], p[i + 1], p[i + 2], p[i + 3]);
        // Strip the first byte which is the USB-MIDI virtual wire ID,
        // rest is MIDI message (at least for 3-byte messages).
        convert_midi_bytes_to_messages(p + i + 1, 3);
      }
      esp_err_t err = usb_host_transfer_submit(transfer);
      if (err != ESP_OK) {
        printf("midi usb_host_transfer_submit err: 0x%x\n", err);
      }
    } else {
      printf("midi transfer->status %d\n", transfer->status);
    }
  }
}

bool check_interface_desc_MIDI(const void *p) {
  const usb_intf_desc_t *intf = (const usb_intf_desc_t *)p;

  // USB MIDI
  if ((intf->bInterfaceClass == USB_CLASS_AUDIO) &&
      (intf->bInterfaceSubClass == 3) &&
      (intf->bInterfaceProtocol == 0)) {
    isMIDI = true;
    printf("Claiming a MIDI device!\n");
    Interface_Number = intf->bInterfaceNumber;
    esp_err_t err = usb_host_interface_claim(Client_Handle, Device_Handle,
                                             Interface_Number, intf->bAlternateSetting);
    if (err != ESP_OK) printf("midi usb_host_interface_claim failed: %x\n", err);
    return true;
  }
  return false;
}

void prepare_endpoint_midi(const void *p) {
  const usb_ep_desc_t *endpoint = (const usb_ep_desc_t *)p;
  esp_err_t err;

  // must be bulk for MIDI
  if ((endpoint->bmAttributes & USB_BM_ATTRIBUTES_XFERTYPE_MASK) != USB_BM_ATTRIBUTES_XFER_BULK) {
    printf("MIDI: Not bulk endpoint: 0x%02x\n", endpoint->bmAttributes);
    return;
  }
  if (endpoint->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK) {
      // MIDI-IN endpoint.
      for (int i = 0; i < MIDI_IN_BUFFERS; i++) {
          if (MIDIIn[i] == NULL) {
              err = usb_host_transfer_alloc(endpoint->wMaxPacketSize, 0, &MIDIIn[i]);
              if (err != ESP_OK) {
                  MIDIIn[i] = NULL;
                  printf("midi usb_host_transfer_alloc/In err: 0x%x\n", err);
                  haveMIDIout = false;
                  return;
              }
        }
        if (MIDIIn[i] != NULL) {
            MIDIIn[i]->device_handle = Device_Handle;
            MIDIIn[i]->bEndpointAddress = endpoint->bEndpointAddress;
            MIDIIn[i]->callback = midi_transfer_cb;
            MIDIIn[i]->context = (void *)i;
            MIDIIn[i]->num_bytes = endpoint->wMaxPacketSize;
            esp_err_t err = usb_host_transfer_submit(MIDIIn[i]);
            if (err != ESP_OK) {
                printf("midi usb_host_transfer_submit/In err: 0x%x\n", err);
            }
            haveMIDIout = true;
        }
      }
  } else {
      // MIDI-OUT endpoint
      if (MIDIOut == NULL) {
          err = usb_host_transfer_alloc(endpoint->wMaxPacketSize, 0, &MIDIOut);
          if (err != ESP_OK) {
              MIDIOut = NULL;
              printf("midi usb_host_transfer_alloc/Out err: 0x%x\n", err);
              haveMIDIin = false;
              return;
          }
      }
      if (MIDIOut != NULL) {
          DBGPRINTF1("Out data_buffer_size: %d\n", MIDIOut->data_buffer_size);
          MIDIOut->device_handle = Device_Handle;
          MIDIOut->bEndpointAddress = endpoint->bEndpointAddress;
          MIDIOut->callback = midi_transfer_cb;
          MIDIOut->context = NULL;
          //    MIDIOut->flags |= USB_TRANSFER_FLAG_ZERO_PACK;
          haveMIDIin = true;
      }
  }
  // MIDI is ready when both input and output endpoints are initialized.
  //isMIDIReady = ((MIDIOut != NULL) && (MIDIIn[0] != NULL));
  isMIDIReady = haveMIDIin && haveMIDIout;
}

// end of usbhmidi.ino
// ---------------------------------

void new_enumeration_config_fn(const usb_config_desc_t *config_desc);


void _client_event_callback(const usb_host_client_event_msg_t *event_msg, void *arg)
{
  esp_err_t err;
  switch (event_msg->event)
  {
    /**< A new device has been enumerated and added to the USB Host Library */
    case USB_HOST_CLIENT_EVENT_NEW_DEV:
      DBGPRINTF1("New device address: %d\n", event_msg->new_dev.address);
      err = usb_host_device_open(Client_Handle, event_msg->new_dev.address, &Device_Handle);
      if (err != ESP_OK) printf("usb_host_device_open: 0x%x\n", err);

      usb_device_info_t dev_info;
      err = usb_host_device_info(Device_Handle, &dev_info);
      if (err != ESP_OK) printf("usb_host_device_info: 0x%x\n", err);
      //printf("speed: %d dev_addr %d vMaxPacketSize0 %d bConfigurationValue %d\n",
      //    dev_info.speed, dev_info.dev_addr, dev_info.bMaxPacketSize0,
      //    dev_info.bConfigurationValue);

      const usb_device_desc_t *dev_desc;
      err = usb_host_get_device_descriptor(Device_Handle, &dev_desc);
      if (err != ESP_OK) printf("usb_host_get_device_desc: 0x%x\n", err);

      const usb_config_desc_t *config_desc;
      err = usb_host_get_active_config_descriptor(Device_Handle, &config_desc);
      if (err != ESP_OK) printf("usb_host_get_config_desc: 0x%x\n", err);
      // Finally, we get to inspect the new device and maybe connect to it.
      new_enumeration_config_fn(config_desc);
      break;
    /**< A device opened by the client is now gone */
    case USB_HOST_CLIENT_EVENT_DEV_GONE:
      printf("Device Gone handle: 0x%lx\n", (uint32_t)event_msg->dev_gone.dev_hdl);
      // Mark everything de-initialized so it will re-initialized on another connect.
      esp_err_t err;
      if (isMIDI || isKeyboard) {
          err = usb_host_interface_release(Client_Handle, Device_Handle, Interface_Number);
          if (err != ESP_OK) printf("usb_host_interface_release err: 0x%x\n", err);
          isMIDI = false;
          isKeyboard = false;
      }
      err = usb_host_device_close(Client_Handle, Device_Handle);
      if (err != ESP_OK) printf("usb_host_device_close err: 0x%x\n", err);
      isMIDIReady = false;
      haveMIDIin = false;
      haveMIDIout = false;
      isKeyboardReady = false;
      break;
    default:
      printf("Unknown USB event: %d\n", event_msg->event);
      break;
  }
}

// Reference: esp-idf/examples/peripherals/usb/host/usb_host_lib/main/usb_host_lib_main.c

void usbh_setup()
{
  const usb_host_config_t config = {
    .intr_flags = ESP_INTR_FLAG_LEVEL1,
  };
  esp_err_t err = usb_host_install(&config);
  (void)err;
  DBGPRINTF1("usb_host_install: 0x%x\n", err);

  const usb_host_client_config_t client_config = {
    .is_synchronous = false,
    .max_num_event_msg = 5,
    .async = {
        .client_event_callback = _client_event_callback,
        .callback_arg = Client_Handle
    }
  };
  err = usb_host_client_register(&client_config, &Client_Handle);
  DBGPRINTF1("usb_host_client_register: 0x%x\n", err);
}

void usbh_task(void)
{
  uint32_t event_flags;

  esp_err_t err = usb_host_lib_handle_events(HOST_EVENT_TIMEOUT, &event_flags);
  if (err == ESP_OK) {
    if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS) {
      DBGPRINTF("No more clients\n");
    }
    if (event_flags & USB_HOST_LIB_EVENT_FLAGS_ALL_FREE) {
      DBGPRINTF("No more devices\n");
    }
  } else {
    if (err != ESP_ERR_TIMEOUT) {
      printf("usb_host_lib_handle_events: 0x%x flags: %lx\n", err, event_flags);
    }
  }

  err = usb_host_client_handle_events(Client_Handle, CLIENT_EVENT_TIMEOUT);
  if ((err != ESP_OK) && (err != ESP_ERR_TIMEOUT)) {
    printf("usb_host_client_handle_events: 0x%x\n", err);
  }
}

// Keep track of which keys / scan codes are being held
uint16_t current_held = 0;
int64_t current_held_ms = 0;
int64_t last_inter_trigger_ms = 0;



uint32_t keycode_to_ctrl_key(uint16_t key)
{
    switch(key) {
        case 261:
            return LV_KEY_RIGHT;

        case 260:
            return LV_KEY_LEFT;

        case 259:
            return LV_KEY_UP;

        case 258:
            return LV_KEY_DOWN;

        case 27:
            return LV_KEY_ESC;

        case 8:
            return LV_KEY_BACKSPACE;

        //case SDLK_DELETE:
        //    return LV_KEY_DEL;

        case 13:
            return LV_KEY_ENTER;

        // We could feed in shift here, TODO
        case 9:
            return LV_KEY_NEXT;
            
        case 22:
            return LV_KEY_NEXT;

        case 25:
            return LV_KEY_PREV;

        default:
            return '\0';
    }
}



static char lvgl_kb_buf[KEYBOARD_BUFFER_SIZE];


void usb_keyboard_read(lv_indev_t * indev_drv, lv_indev_data_t * data) {
    (void) indev_drv;      /*Unused*/

    static bool dummy_read = false;
    const size_t len = strlen(lvgl_kb_buf);

    /*Send a release manually*/
    if (dummy_read) {
        dummy_read = false;
        data->state = LV_INDEV_STATE_RELEASED;
        data->continue_reading = len > 0;
    }
        /*Send the pressed character*/
    else if (len > 0) {
        dummy_read = true;
        data->state = LV_INDEV_STATE_PRESSED;
        data->key = lvgl_kb_buf[0];
        memmove(lvgl_kb_buf, lvgl_kb_buf + 1, len);
        data->continue_reading = true;
    }
}



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
                    if(keycode_to_ctrl_key(c) != '\0') {
                        const size_t len = strlen(lvgl_kb_buf);
                        if (len < KEYBOARD_BUFFER_SIZE - 1) {
                            lvgl_kb_buf[len] = keycode_to_ctrl_key(c);
                            lvgl_kb_buf[len + 1] = '\0';
                        }
                    } else {
                        // put it in lvgl_kb_buf for lvgl
                        const size_t len = strlen(lvgl_kb_buf);
                        if (len < KEYBOARD_BUFFER_SIZE - 1) {
                            lvgl_kb_buf[len] = c;
                            lvgl_kb_buf[len+1] = '\0';
                        }
                    }
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
          printf("Keyboard boot hid transfer (%d bytes) too short or long\n",
                 transfer->actual_num_bytes);
      }
    }
    else {
      printf("kbd transfer->status %d\n", transfer->status);
    }
  }
}

bool check_interface_desc_boot_keyboard(const void *p) {
  const usb_intf_desc_t *intf = (const usb_intf_desc_t *)p;

  // HID Keyboard
  if ((intf->bInterfaceClass == USB_CLASS_HID) &&
      (intf->bInterfaceSubClass == 1) &&
      (intf->bInterfaceProtocol == 1)) {
    isKeyboard = true;
    Interface_Number = intf->bInterfaceNumber;
    esp_err_t err = usb_host_interface_claim(Client_Handle, Device_Handle,
                                             Interface_Number, intf->bAlternateSetting);
    if (err != ESP_OK) printf("usb_host_interface_claim failed: 0x%x\n", err);
    return true;
  }
  return false;
}

void prepare_endpoint_hid(const void *p)
{
  const usb_ep_desc_t *endpoint = (const usb_ep_desc_t *)p;
  esp_err_t err;
  keyboard_bytes = usb_round_up_to_mps(KEYBOARD_BYTES, endpoint->wMaxPacketSize);
  DBGPRINTF2("Setting KB to %d from MPS %d\n", keyboard_bytes, endpoint->wMaxPacketSize);

  // must be interrupt for HID
  if ((endpoint->bmAttributes & USB_BM_ATTRIBUTES_XFERTYPE_MASK) != USB_BM_ATTRIBUTES_XFER_INT) {
    printf("Kbd: Not interrupt endpoint: 0x%02x\n", endpoint->bmAttributes);
    return;
  }
  if (endpoint->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK) {
      if (KeyboardIn == NULL) {
          err = usb_host_transfer_alloc(keyboard_bytes, 0, &KeyboardIn);
          if (err != ESP_OK) {
              KeyboardIn = NULL;
              printf("kbd usb_host_transfer_alloc/In err: 0x%x\n", err);
              return;
          }
      }
      if (KeyboardIn != NULL) {
          KeyboardIn->device_handle = Device_Handle;
          KeyboardIn->bEndpointAddress = endpoint->bEndpointAddress;
          KeyboardIn->callback = keyboard_transfer_cb;
          KeyboardIn->context = NULL;
          isKeyboardReady = true;
          KeyboardInterval = endpoint->bInterval;
          DBGPRINTF("USB boot keyboard ready\n");
      }
  } else {
      DBGPRINTF("Ignoring kbd interrupt Out endpoint\n");
  }
}

void new_enumeration_config_fn(const usb_config_desc_t *config_desc) {
  // We just retrieved the config of a newly-connected device.
  // Read through it and see if we can claim a recognized device.
  //
  // &config_desc->val[0] is the same as config_desc
  // so the first "descriptor type" found is actually TYPE_CONFIGURATION
  // and the call to show_config_desc(p) is equivalent to doing
  // show_config_desc(config_desc) here.
  const uint8_t *p = &config_desc->val[0];
  uint8_t bLength;
  int indent = 1;
  int last_descriptor = -1;
  for (int i = 0; i < config_desc->wTotalLength; i+=bLength, p+=bLength) {
    bLength = *p;
    if ((i + bLength) <= config_desc->wTotalLength) {
      const uint8_t bDescriptorType = *(p + 1);
      DBGPRINTF3("config_desc->val[%d]: type=%d length=%d\n", i, bDescriptorType, bLength);
      switch (bDescriptorType) {
        case USB_B_DESCRIPTOR_TYPE_CONFIGURATION:
          // The first record in val[] is by definition the config description.
          show_config_desc(p, indent++);
          last_descriptor = bDescriptorType;
          break;
        case USB_B_DESCRIPTOR_TYPE_DEVICE:
          DBGPRINTF("USB Device Descriptor should not appear in config\n");
          break;
        case USB_B_DESCRIPTOR_TYPE_STRING:
          DBGPRINTF("USB string desc TBD\n");
          break;
        case USB_B_DESCRIPTOR_TYPE_INTERFACE:
          if ((last_descriptor == USB_B_DESCRIPTOR_TYPE_INTERFACE)
              || (last_descriptor == USB_B_DESCRIPTOR_TYPE_ENDPOINT)) --indent;
          show_interface_desc(p, indent++);
          if (!isMIDI && !isKeyboard) {
              if (!check_interface_desc_MIDI(p)) {
                  check_interface_desc_boot_keyboard(p);
              }
          }
          if (!isMIDI && !isKeyboard) {
              DBGPRINTF("Interface was neither keyboard nor midi.\n");
          }
          last_descriptor = bDescriptorType;
          break;
        case USB_B_DESCRIPTOR_TYPE_ENDPOINT:
          show_endpoint_desc(p, indent);
          if (isKeyboard && !isKeyboardReady) {
              prepare_endpoint_hid(p);
          } else if (isMIDI && !isMIDIReady) {
            prepare_endpoint_midi(p);
          }
          last_descriptor = bDescriptorType;
          break;
        case USB_B_DESCRIPTOR_TYPE_DEVICE_QUALIFIER:
          DBGPRINTF("USB device qual desc TBD\n");
          break;
        case USB_B_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION:
          DBGPRINTF("USB Other Speed TBD\n");
          break;
        case USB_B_DESCRIPTOR_TYPE_INTERFACE_POWER:
          DBGPRINTF("USB Interface Power TBD\n");
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
    // Reset key maps
    for(uint8_t i=0;i<MAX_KEY_REMAPS;i++) {
        key_remaps[i].scan = 0;
        key_remaps[i].mod = 0;
        key_remaps[i].code = 0;
    }

  usbh_setup();
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
        KeyboardIn->num_bytes = keyboard_bytes; 
        esp_err_t err = usb_host_transfer_submit(KeyboardIn);
        if (err != ESP_OK) {
          printf("kbd usb_host_transfer_submit/In err: 0x%x\n", err);
        }
        isKeyboardPolling = true;
        KeyboardTimer = 0;
      }
    }
}
