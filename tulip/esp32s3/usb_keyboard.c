// usb_keyboard.c
#include "usb_keyboard.h"

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

#define DEBUG_USB
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


void lvgl_keyboard_read(lv_indev_t * indev_drv, lv_indev_data_t * data) {
    (void) indev_drv;     // unused

    static bool dummy_read = false;
    const size_t len = strlen(lvgl_kb_buf);

    // Send a release manually
    if (dummy_read) {
        dummy_read = false;
        data->state = LV_INDEV_STATE_RELEASED;
        data->continue_reading = len > 0;
    }
        // Send the pressed character 
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
      //if(transfer->actual_num_bytes > 7 && transfer->actual_num_bytes < 17) {
      if (transfer->actual_num_bytes == 8 || transfer->actual_num_bytes == 16) {
        uint8_t *const p = transfer->data_buffer;
        decode_report(p);
      } else if (transfer->actual_num_bytes == 10) {
        uint8_t *const p = transfer->data_buffer;
        decode_report(p+1);
      } else {
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
#if 0
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
#endif
#if 0

/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "esp_log.h"
#include "usb/usb_host.h"
#include "errno.h"
#include "driver/gpio.h"

#include "usb/hid_host.h"
#include "usb/hid_usage_keyboard.h"
#include "usb/hid_usage_mouse.h"

/* GPIO Pin number for quit from example logic */
#define APP_QUIT_PIN                GPIO_NUM_0

static const char *TAG = "example";

QueueHandle_t app_event_queue = NULL;

/**
 * @brief APP event group
 *
 * Application logic can be different. There is a one among other ways to distingiush the
 * event by application event group.
 * In this example we have two event groups:
 * APP_EVENT            - General event, which is APP_QUIT_PIN press event (Generally, it is IO0).
 * APP_EVENT_HID_HOST   - HID Host Driver event, such as device connection/disconnection or input report.
 */
typedef enum {
    APP_EVENT = 0,
    APP_EVENT_HID_HOST
} app_event_group_t;

/**
 * @brief APP event queue
 *
 * This event is used for delivering the HID Host event from callback to a task.
 */
typedef struct {
    app_event_group_t event_group;
    /* HID Host - Device related info */
    struct {
        hid_host_device_handle_t handle;
        hid_host_driver_event_t event;
        void *arg;
    } hid_host_device;
} app_event_queue_t;

/**
 * @brief HID Protocol string names
 */
static const char *hid_proto_name_str[] = {
    "NONE",
    "KEYBOARD",
    "MOUSE"
};

/**
 * @brief Key event
 */
typedef struct {
    enum key_state {
        KEY_STATE_PRESSED = 0x00,
        KEY_STATE_RELEASED = 0x01
    } state;
    uint8_t modifier;
    uint8_t key_code;
} key_event_t;

/* Main char symbol for ENTER key */
#define KEYBOARD_ENTER_MAIN_CHAR    '\r'
/* When set to 1 pressing ENTER will be extending with LineFeed during serial debug output */
#define KEYBOARD_ENTER_LF_EXTEND    1

/**
 * @brief Scancode to ascii table
 */
const uint8_t keycode2ascii [57][2] = {
    {0, 0}, /* HID_KEY_NO_PRESS        */
    {0, 0}, /* HID_KEY_ROLLOVER        */
    {0, 0}, /* HID_KEY_POST_FAIL       */
    {0, 0}, /* HID_KEY_ERROR_UNDEFINED */
    {'a', 'A'}, /* HID_KEY_A               */
    {'b', 'B'}, /* HID_KEY_B               */
    {'c', 'C'}, /* HID_KEY_C               */
    {'d', 'D'}, /* HID_KEY_D               */
    {'e', 'E'}, /* HID_KEY_E               */
    {'f', 'F'}, /* HID_KEY_F               */
    {'g', 'G'}, /* HID_KEY_G               */
    {'h', 'H'}, /* HID_KEY_H               */
    {'i', 'I'}, /* HID_KEY_I               */
    {'j', 'J'}, /* HID_KEY_J               */
    {'k', 'K'}, /* HID_KEY_K               */
    {'l', 'L'}, /* HID_KEY_L               */
    {'m', 'M'}, /* HID_KEY_M               */
    {'n', 'N'}, /* HID_KEY_N               */
    {'o', 'O'}, /* HID_KEY_O               */
    {'p', 'P'}, /* HID_KEY_P               */
    {'q', 'Q'}, /* HID_KEY_Q               */
    {'r', 'R'}, /* HID_KEY_R               */
    {'s', 'S'}, /* HID_KEY_S               */
    {'t', 'T'}, /* HID_KEY_T               */
    {'u', 'U'}, /* HID_KEY_U               */
    {'v', 'V'}, /* HID_KEY_V               */
    {'w', 'W'}, /* HID_KEY_W               */
    {'x', 'X'}, /* HID_KEY_X               */
    {'y', 'Y'}, /* HID_KEY_Y               */
    {'z', 'Z'}, /* HID_KEY_Z               */
    {'1', '!'}, /* HID_KEY_1               */
    {'2', '@'}, /* HID_KEY_2               */
    {'3', '#'}, /* HID_KEY_3               */
    {'4', '$'}, /* HID_KEY_4               */
    {'5', '%'}, /* HID_KEY_5               */
    {'6', '^'}, /* HID_KEY_6               */
    {'7', '&'}, /* HID_KEY_7               */
    {'8', '*'}, /* HID_KEY_8               */
    {'9', '('}, /* HID_KEY_9               */
    {'0', ')'}, /* HID_KEY_0               */
    {KEYBOARD_ENTER_MAIN_CHAR, KEYBOARD_ENTER_MAIN_CHAR}, /* HID_KEY_ENTER           */
    {0, 0}, /* HID_KEY_ESC             */
    {'\b', 0}, /* HID_KEY_DEL             */
    {0, 0}, /* HID_KEY_TAB             */
    {' ', ' '}, /* HID_KEY_SPACE           */
    {'-', '_'}, /* HID_KEY_MINUS           */
    {'=', '+'}, /* HID_KEY_EQUAL           */
    {'[', '{'}, /* HID_KEY_OPEN_BRACKET    */
    {']', '}'}, /* HID_KEY_CLOSE_BRACKET   */
    {'\\', '|'}, /* HID_KEY_BACK_SLASH      */
    {'\\', '|'}, /* HID_KEY_SHARP           */  // HOTFIX: for NonUS Keyboards repeat HID_KEY_BACK_SLASH
    {';', ':'}, /* HID_KEY_COLON           */
    {'\'', '"'}, /* HID_KEY_QUOTE           */
    {'`', '~'}, /* HID_KEY_TILDE           */
    {',', '<'}, /* HID_KEY_LESS            */
    {'.', '>'}, /* HID_KEY_GREATER         */
    {'/', '?'} /* HID_KEY_SLASH           */
};

/**
 * @brief Makes new line depending on report output protocol type
 *
 * @param[in] proto Current protocol to output
 */
static void hid_print_new_device_report_header(hid_protocol_t proto)
{
    static hid_protocol_t prev_proto_output = -1;

    if (prev_proto_output != proto) {
        prev_proto_output = proto;
        printf("\r\n");
        if (proto == HID_PROTOCOL_MOUSE) {
            printf("Mouse\r\n");
        } else if (proto == HID_PROTOCOL_KEYBOARD) {
            printf("Keyboard\r\n");
        } else {
            printf("Generic\r\n");
        }
        fflush(stdout);
    }
}

/**
 * @brief HID Keyboard modifier verification for capitalization application (right or left shift)
 *
 * @param[in] modifier
 * @return true  Modifier was pressed (left or right shift)
 * @return false Modifier was not pressed (left or right shift)
 *
 */
static inline bool hid_keyboard_is_modifier_shift(uint8_t modifier)
{
    if (((modifier & HID_LEFT_SHIFT) == HID_LEFT_SHIFT) ||
            ((modifier & HID_RIGHT_SHIFT) == HID_RIGHT_SHIFT)) {
        return true;
    }
    return false;
}

/**
 * @brief HID Keyboard get char symbol from key code
 *
 * @param[in] modifier  Keyboard modifier data
 * @param[in] key_code  Keyboard key code
 * @param[in] key_char  Pointer to key char data
 *
 * @return true  Key scancode converted successfully
 * @return false Key scancode unknown
 */
static inline bool hid_keyboard_get_char(uint8_t modifier,
                                         uint8_t key_code,
                                         unsigned char *key_char)
{
    uint8_t mod = (hid_keyboard_is_modifier_shift(modifier)) ? 1 : 0;

    if ((key_code >= HID_KEY_A) && (key_code <= HID_KEY_SLASH)) {
        *key_char = keycode2ascii[key_code][mod];
    } else {
        // All other key pressed
        return false;
    }

    return true;
}

/**
 * @brief HID Keyboard print char symbol
 *
 * @param[in] key_char  Keyboard char to stdout
 */
static inline void hid_keyboard_print_char(unsigned int key_char)
{
    if (!!key_char) {
        putchar(key_char);
#if (KEYBOARD_ENTER_LF_EXTEND)
        if (KEYBOARD_ENTER_MAIN_CHAR == key_char) {
            putchar('\n');
        }
#endif // KEYBOARD_ENTER_LF_EXTEND
        fflush(stdout);
    }
}

/**
 * @brief Key Event. Key event with the key code, state and modifier.
 *
 * @param[in] key_event Pointer to Key Event structure
 *
 */
static void key_event_callback(key_event_t *key_event)
{
    unsigned char key_char;

    hid_print_new_device_report_header(HID_PROTOCOL_KEYBOARD);

    if (KEY_STATE_PRESSED == key_event->state) {
        if (hid_keyboard_get_char(key_event->modifier,
                                  key_event->key_code, &key_char)) {

            hid_keyboard_print_char(key_char);

        }
    }
}

/**
 * @brief Key buffer scan code search.
 *
 * @param[in] src       Pointer to source buffer where to search
 * @param[in] key       Key scancode to search
 * @param[in] length    Size of the source buffer
 */
static inline bool key_found(const uint8_t *const src,
                             uint8_t key,
                             unsigned int length)
{
    for (unsigned int i = 0; i < length; i++) {
        if (src[i] == key) {
            return true;
        }
    }
    return false;
}

/**
 * @brief USB HID Host Keyboard Interface report callback handler
 *
 * @param[in] data    Pointer to input report data buffer
 * @param[in] length  Length of input report data buffer
 */
static void hid_host_keyboard_report_callback(const uint8_t *const data, const int length)
{
    hid_keyboard_input_report_boot_t *kb_report = (hid_keyboard_input_report_boot_t *)data;

    if (length < sizeof(hid_keyboard_input_report_boot_t)) {
        return;
    }

    static uint8_t prev_keys[HID_KEYBOARD_KEY_MAX] = { 0 };
    key_event_t key_event;

    for (int i = 0; i < HID_KEYBOARD_KEY_MAX; i++) {

        // key has been released verification
        if (prev_keys[i] > HID_KEY_ERROR_UNDEFINED &&
                !key_found(kb_report->key, prev_keys[i], HID_KEYBOARD_KEY_MAX)) {
            key_event.key_code = prev_keys[i];
            key_event.modifier = 0;
            key_event.state = KEY_STATE_RELEASED;
            key_event_callback(&key_event);
        }

        // key has been pressed verification
        if (kb_report->key[i] > HID_KEY_ERROR_UNDEFINED &&
                !key_found(prev_keys, kb_report->key[i], HID_KEYBOARD_KEY_MAX)) {
            key_event.key_code = kb_report->key[i];
            key_event.modifier = kb_report->modifier.val;
            key_event.state = KEY_STATE_PRESSED;
            key_event_callback(&key_event);
        }
    }

    memcpy(prev_keys, &kb_report->key, HID_KEYBOARD_KEY_MAX);
}

/**
 * @brief USB HID Host Mouse Interface report callback handler
 *
 * @param[in] data    Pointer to input report data buffer
 * @param[in] length  Length of input report data buffer
 */
static void hid_host_mouse_report_callback(const uint8_t *const data, const int length)
{
    hid_mouse_input_report_boot_t *mouse_report = (hid_mouse_input_report_boot_t *)data;

    if (length < sizeof(hid_mouse_input_report_boot_t)) {
        return;
    }

    static int x_pos = 0;
    static int y_pos = 0;

    // Calculate absolute position from displacement
    x_pos += mouse_report->x_displacement;
    y_pos += mouse_report->y_displacement;

    hid_print_new_device_report_header(HID_PROTOCOL_MOUSE);

    printf("X: %06d\tY: %06d\t|%c|%c|\r",
           x_pos, y_pos,
           (mouse_report->buttons.button1 ? 'o' : ' '),
           (mouse_report->buttons.button2 ? 'o' : ' '));
    fflush(stdout);
}

/**
 * @brief USB HID Host Generic Interface report callback handler
 *
 * 'generic' means anything else than mouse or keyboard
 *
 * @param[in] data    Pointer to input report data buffer
 * @param[in] length  Length of input report data buffer
 */
static void hid_host_generic_report_callback(const uint8_t *const data, const int length)
{
    hid_print_new_device_report_header(HID_PROTOCOL_NONE);
    for (int i = 0; i < length; i++) {
        printf("%02X", data[i]);
    }
    putchar('\r');
}

/**
 * @brief USB HID Host interface callback
 *
 * @param[in] hid_device_handle  HID Device handle
 * @param[in] event              HID Host interface event
 * @param[in] arg                Pointer to arguments, does not used
 */
void hid_host_interface_callback(hid_host_device_handle_t hid_device_handle,
                                 const hid_host_interface_event_t event,
                                 void *arg)
{
    uint8_t data[64] = { 0 };
    size_t data_length = 0;
    hid_host_dev_params_t dev_params;
    ESP_ERROR_CHECK(hid_host_device_get_params(hid_device_handle, &dev_params));

    switch (event) {
    case HID_HOST_INTERFACE_EVENT_INPUT_REPORT:
        ESP_ERROR_CHECK(hid_host_device_get_raw_input_report_data(hid_device_handle,
                                                                  data,
                                                                  64,
                                                                  &data_length));

        if (HID_SUBCLASS_BOOT_INTERFACE == dev_params.sub_class) {
            if (HID_PROTOCOL_KEYBOARD == dev_params.proto) {
                hid_host_keyboard_report_callback(data, data_length);
            } else if (HID_PROTOCOL_MOUSE == dev_params.proto) {
                hid_host_mouse_report_callback(data, data_length);
            }
        } else {
            hid_host_generic_report_callback(data, data_length);
        }

        break;
    case HID_HOST_INTERFACE_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HID Device, protocol '%s' DISCONNECTED",
                 hid_proto_name_str[dev_params.proto]);
        ESP_ERROR_CHECK(hid_host_device_close(hid_device_handle));
        break;
    case HID_HOST_INTERFACE_EVENT_TRANSFER_ERROR:
        ESP_LOGI(TAG, "HID Device, protocol '%s' TRANSFER_ERROR",
                 hid_proto_name_str[dev_params.proto]);
        break;
    default:
        ESP_LOGE(TAG, "HID Device, protocol '%s' Unhandled event",
                 hid_proto_name_str[dev_params.proto]);
        break;
    }
}

/**
 * @brief USB HID Host Device event
 *
 * @param[in] hid_device_handle  HID Device handle
 * @param[in] event              HID Host Device event
 * @param[in] arg                Pointer to arguments, does not used
 */
void hid_host_device_event(hid_host_device_handle_t hid_device_handle,
                           const hid_host_driver_event_t event,
                           void *arg)
{
    hid_host_dev_params_t dev_params;
    ESP_ERROR_CHECK(hid_host_device_get_params(hid_device_handle, &dev_params));

    switch (event) {
    case HID_HOST_DRIVER_EVENT_CONNECTED:
        ESP_LOGI(TAG, "HID Device, protocol '%s' CONNECTED",
                 hid_proto_name_str[dev_params.proto]);

        const hid_host_device_config_t dev_config = {
            .callback = hid_host_interface_callback,
            .callback_arg = NULL
        };

        ESP_ERROR_CHECK(hid_host_device_open(hid_device_handle, &dev_config));
        if (HID_SUBCLASS_BOOT_INTERFACE == dev_params.sub_class) {
            ESP_ERROR_CHECK(hid_class_request_set_protocol(hid_device_handle, HID_REPORT_PROTOCOL_BOOT));
            if (HID_PROTOCOL_KEYBOARD == dev_params.proto) {
                ESP_ERROR_CHECK(hid_class_request_set_idle(hid_device_handle, 0, 0));
            }
        }
        ESP_ERROR_CHECK(hid_host_device_start(hid_device_handle));
        break;
    default:
        break;
    }
}

/**
 * @brief Start USB Host install and handle common USB host library events while app pin not low
 *
 * @param[in] arg  Not used
 */
static void usb_lib_task(void *arg)
{
    const usb_host_config_t host_config = {
        .skip_phy_setup = false,
        .intr_flags = ESP_INTR_FLAG_LEVEL1,
    };

    ESP_ERROR_CHECK(usb_host_install(&host_config));
    xTaskNotifyGive(arg);

    while (true) {
        uint32_t event_flags;
        usb_host_lib_handle_events(portMAX_DELAY, &event_flags);
        // In this example, there is only one client registered
        // So, once we deregister the client, this call must succeed with ESP_OK
        if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS) {
            ESP_ERROR_CHECK(usb_host_device_free_all());
            break;
        }
    }

    ESP_LOGI(TAG, "USB shutdown");
    // Clean up USB Host
    vTaskDelay(10); // Short delay to allow clients clean-up
    ESP_ERROR_CHECK(usb_host_uninstall());
    vTaskDelete(NULL);
}

/**
 * @brief BOOT button pressed callback
 *
 * Signal application to exit the HID Host task
 *
 * @param[in] arg Unused
 */
static void gpio_isr_cb(void *arg)
{
    BaseType_t xTaskWoken = pdFALSE;
    const app_event_queue_t evt_queue = {
        .event_group = APP_EVENT,
    };

    if (app_event_queue) {
        xQueueSendFromISR(app_event_queue, &evt_queue, &xTaskWoken);
    }

    if (xTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

/**
 * @brief HID Host Device callback
 *
 * Puts new HID Device event to the queue
 *
 * @param[in] hid_device_handle HID Device handle
 * @param[in] event             HID Device event
 * @param[in] arg               Not used
 */
void hid_host_device_callback(hid_host_device_handle_t hid_device_handle,
                              const hid_host_driver_event_t event,
                              void *arg)
{
    const app_event_queue_t evt_queue = {
        .event_group = APP_EVENT_HID_HOST,
        // HID Host Device related info
        .hid_host_device.handle = hid_device_handle,
        .hid_host_device.event = event,
        .hid_host_device.arg = arg
    };

    if (app_event_queue) {
        xQueueSend(app_event_queue, &evt_queue, 0);
    }
}

void run_usb(void)
{
    BaseType_t task_created;
    app_event_queue_t evt_queue;
    ESP_LOGI(TAG, "HID Host example");

    /*
    * Create usb_lib_task to:
    * - initialize USB Host library
    * - Handle USB Host events while APP pin in in HIGH state
    */
    task_created = xTaskCreatePinnedToCore(usb_lib_task,
                                           "usb_events",
                                           4096,
                                           xTaskGetCurrentTaskHandle(),
                                           2, NULL, 0);
    assert(task_created == pdTRUE);

    // Wait for notification from usb_lib_task to proceed
    ulTaskNotifyTake(false, 1000);

    /*
    * HID host driver configuration
    * - create background task for handling low level event inside the HID driver
    * - provide the device callback to get new HID Device connection event
    */
    const hid_host_driver_config_t hid_host_driver_config = {
        .create_background_task = true,
        .task_priority = 5,
        .stack_size = 4096,
        .core_id = 0,
        .callback = hid_host_device_callback,
        .callback_arg = NULL
    };

    ESP_ERROR_CHECK(hid_host_install(&hid_host_driver_config));

    // Create queue
    app_event_queue = xQueueCreate(10, sizeof(app_event_queue_t));

    ESP_LOGI(TAG, "Waiting for HID Device to be connected");

    while (1) {
        // Wait queue
        if (xQueueReceive(app_event_queue, &evt_queue, portMAX_DELAY)) {
            if (APP_EVENT == evt_queue.event_group) {
                // User pressed button
                usb_host_lib_info_t lib_info;
                ESP_ERROR_CHECK(usb_host_lib_info(&lib_info));
                if (lib_info.num_devices == 0) {
                    // End while cycle
                    break;
                } else {
                    ESP_LOGW(TAG, "To shutdown example, remove all USB devices and press button again.");
                    // Keep polling
                }
            }

            if (APP_EVENT_HID_HOST ==  evt_queue.event_group) {
                hid_host_device_event(evt_queue.hid_host_device.handle,
                                      evt_queue.hid_host_device.event,
                                      evt_queue.hid_host_device.arg);
            }
        }
    }

    ESP_LOGI(TAG, "HID Driver uninstall");
    ESP_ERROR_CHECK(hid_host_uninstall());
    xQueueReset(app_event_queue);
    vQueueDelete(app_event_queue);
}
#endif


/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "usb/usb_host.h"

#define CLIENT_NUM_EVENT_MSG        5

typedef enum {
    ACTION_OPEN_DEV = 0x01,
    ACTION_GET_DEV_INFO = 0x02,
    ACTION_GET_DEV_DESC = 0x04,
    ACTION_GET_CONFIG_DESC = 0x08,
    ACTION_GET_STR_DESC = 0x10,
    ACTION_CLOSE_DEV = 0x20,
    ACTION_EXIT = 0x40,
    ACTION_RECONNECT = 0x80,
} action_t;

typedef struct {
    usb_host_client_handle_t client_hdl;
    uint8_t dev_addr;
    usb_device_handle_t dev_hdl;
    uint32_t actions;
} class_driver_t;

static const char *TAG = "CLASS";
static class_driver_t *s_driver_obj;

static void client_event_cb(const usb_host_client_event_msg_t *event_msg, void *arg)
{
    class_driver_t *driver_obj = (class_driver_t *)arg;
    switch (event_msg->event) {
    case USB_HOST_CLIENT_EVENT_NEW_DEV:
        if (driver_obj->dev_addr == 0) {
            driver_obj->dev_addr = event_msg->new_dev.address;
            //Open the device next
            driver_obj->actions |= ACTION_OPEN_DEV;
        }
        break;
    case USB_HOST_CLIENT_EVENT_DEV_GONE:
        if (driver_obj->dev_hdl != NULL) {
            //Cancel any other actions and close the device next
            driver_obj->actions = ACTION_CLOSE_DEV;
        }
        break;
    default:
        //Should never occur
        abort();
    }
}

static void action_open_dev(class_driver_t *driver_obj)
{
    assert(driver_obj->dev_addr != 0);
    ESP_LOGI(TAG, "Opening device at address %d", driver_obj->dev_addr);
    ESP_ERROR_CHECK(usb_host_device_open(driver_obj->client_hdl, driver_obj->dev_addr, &driver_obj->dev_hdl));
    //Get the device's information next
    driver_obj->actions &= ~ACTION_OPEN_DEV;
    driver_obj->actions |= ACTION_GET_DEV_INFO;
}

static void action_get_info(class_driver_t *driver_obj)
{
    assert(driver_obj->dev_hdl != NULL);
    ESP_LOGI(TAG, "Getting device information");
    usb_device_info_t dev_info;
    ESP_ERROR_CHECK(usb_host_device_info(driver_obj->dev_hdl, &dev_info));
    ESP_LOGI(TAG, "\t%s speed", (char *[]) {
        "Low", "Full", "High"
    }[dev_info.speed]);
    ESP_LOGI(TAG, "\tbConfigurationValue %d", dev_info.bConfigurationValue);

    //Get the device descriptor next
    driver_obj->actions &= ~ACTION_GET_DEV_INFO;
    driver_obj->actions |= ACTION_GET_DEV_DESC;
}

static void action_get_dev_desc(class_driver_t *driver_obj)
{
    assert(driver_obj->dev_hdl != NULL);
    ESP_LOGI(TAG, "Getting device descriptor");
    const usb_device_desc_t *dev_desc;
    ESP_ERROR_CHECK(usb_host_get_device_descriptor(driver_obj->dev_hdl, &dev_desc));
    usb_print_device_descriptor(dev_desc);
    //Get the device's config descriptor next
    driver_obj->actions &= ~ACTION_GET_DEV_DESC;
    driver_obj->actions |= ACTION_GET_CONFIG_DESC;
}

static void action_get_config_desc(class_driver_t *driver_obj)
{
    assert(driver_obj->dev_hdl != NULL);
    ESP_LOGI(TAG, "Getting config descriptor");
    const usb_config_desc_t *config_desc;
    ESP_ERROR_CHECK(usb_host_get_active_config_descriptor(driver_obj->dev_hdl, &config_desc));
    usb_print_config_descriptor(config_desc, NULL);
    //Get the device's string descriptors next
    driver_obj->actions &= ~ACTION_GET_CONFIG_DESC;
    driver_obj->actions |= ACTION_GET_STR_DESC;
}

static void action_get_str_desc(class_driver_t *driver_obj)
{
    assert(driver_obj->dev_hdl != NULL);
    usb_device_info_t dev_info;
    ESP_ERROR_CHECK(usb_host_device_info(driver_obj->dev_hdl, &dev_info));
    if (dev_info.str_desc_manufacturer) {
        ESP_LOGI(TAG, "Getting Manufacturer string descriptor");
        usb_print_string_descriptor(dev_info.str_desc_manufacturer);
    }
    if (dev_info.str_desc_product) {
        ESP_LOGI(TAG, "Getting Product string descriptor");
        usb_print_string_descriptor(dev_info.str_desc_product);
    }
    if (dev_info.str_desc_serial_num) {
        ESP_LOGI(TAG, "Getting Serial Number string descriptor");
        usb_print_string_descriptor(dev_info.str_desc_serial_num);
    }
    //Nothing to do until the device disconnects
    driver_obj->actions &= ~ACTION_GET_STR_DESC;
}

static void action_close_dev(class_driver_t *driver_obj)
{
    ESP_ERROR_CHECK(usb_host_device_close(driver_obj->client_hdl, driver_obj->dev_hdl));
    driver_obj->dev_hdl = NULL;
    driver_obj->dev_addr = 0;
    //We need to connect a new device
    driver_obj->actions &= ~ACTION_CLOSE_DEV;
    driver_obj->actions |= ACTION_RECONNECT;
}

void class_driver_task(void *arg)
{
    class_driver_t driver_obj = {0};

    ESP_LOGI(TAG, "Registering Client");
    usb_host_client_config_t client_config = {
        .is_synchronous = false,    //Synchronous clients currently not supported. Set this to false
        .max_num_event_msg = CLIENT_NUM_EVENT_MSG,
        .async = {
            .client_event_callback = client_event_cb,
            .callback_arg = (void *) &driver_obj,
        },
    };
    ESP_ERROR_CHECK(usb_host_client_register(&client_config, &driver_obj.client_hdl));
    s_driver_obj = &driver_obj;

    while (1) {
        if (driver_obj.actions == 0) {
            usb_host_client_handle_events(driver_obj.client_hdl, portMAX_DELAY);
        } else {
            if (driver_obj.actions & ACTION_OPEN_DEV) {
                action_open_dev(&driver_obj);
            }
            if (driver_obj.actions & ACTION_GET_DEV_INFO) {
                action_get_info(&driver_obj);
            }
            if (driver_obj.actions & ACTION_GET_DEV_DESC) {
                action_get_dev_desc(&driver_obj);
            }
            if (driver_obj.actions & ACTION_GET_CONFIG_DESC) {
                action_get_config_desc(&driver_obj);
            }
            if (driver_obj.actions & ACTION_GET_STR_DESC) {
                action_get_str_desc(&driver_obj);
            }
            if (driver_obj.actions & ACTION_CLOSE_DEV) {
                action_close_dev(&driver_obj);
            }
            if (driver_obj.actions & ACTION_EXIT) {
                break;
            }
            if (driver_obj.actions & ACTION_RECONNECT) {
                driver_obj.actions = 0;
            }
        }
    }

    ESP_LOGI(TAG, "Deregistering Client");
    ESP_ERROR_CHECK(usb_host_client_deregister(driver_obj.client_hdl));
    vTaskSuspend(NULL);
}

void class_driver_client_deregister(void)
{
    if (s_driver_obj->dev_hdl != NULL) {
        s_driver_obj->actions = ACTION_CLOSE_DEV;
    }
    s_driver_obj->actions |= ACTION_EXIT;

    // Unblock, exit the loop and proceed to deregister client
    ESP_ERROR_CHECK(usb_host_client_unblock(s_driver_obj->client_hdl));
}


/*
 * SPDX-FileCopyrightText: 2021-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_intr_alloc.h"
#include "usb/usb_host.h"
#include "driver/gpio.h"

#define HOST_LIB_TASK_PRIORITY    2
#define CLASS_TASK_PRIORITY     3
#define APP_QUIT_PIN                CONFIG_APP_QUIT_PIN

#ifdef CONFIG_USB_HOST_ENABLE_ENUM_FILTER_CALLBACK
#define ENABLE_ENUM_FILTER_CALLBACK
#endif // CONFIG_USB_HOST_ENABLE_ENUM_FILTER_CALLBACK

extern void class_driver_task(void *arg);
extern void class_driver_client_deregister(void);

//static const char *TAG = "USB host lib";

QueueHandle_t app_event_queue = NULL;

/**
 * @brief APP event group
 *
 * APP_EVENT            - General event, which is APP_QUIT_PIN press event in this example.
 */
typedef enum {
    APP_EVENT = 0,
} app_event_group_t;

/**
 * @brief APP event queue
 *
 * This event is used for delivering events from callback to a task.
 */
typedef struct {
    app_event_group_t event_group;
} app_event_queue_t;


/**
 * @brief Set configuration callback
 *
 * Set the USB device configuration during the enumeration process, must be enabled in the menuconfig

 * @note bConfigurationValue starts at index 1
 *
 * @param[in] dev_desc device descriptor of the USB device currently being enumerated
 * @param[out] bConfigurationValue configuration descriptor index, that will be user for enumeration
 *
 * @return bool
 * - true:  USB device will be enumerated
 * - false: USB device will not be enumerated
 */
#ifdef ENABLE_ENUM_FILTER_CALLBACK
static bool set_config_cb(const usb_device_desc_t *dev_desc, uint8_t *bConfigurationValue)
{
    // If the USB device has more than one configuration, set the second configuration
    if (dev_desc->bNumConfigurations > 1) {
        *bConfigurationValue = 2;
    } else {
        *bConfigurationValue = 1;
    }

    // Return true to enumerate the USB device
    return true;
}
#endif // ENABLE_ENUM_FILTER_CALLBACK

/**
 * @brief Start USB Host install and handle common USB host library events while app pin not low
 *
 * @param[in] arg  Not used
 */
static void usb_host_lib_task(void *arg)
{
    ESP_LOGI(TAG, "Installing USB Host Library");
    usb_host_config_t host_config = {
        .skip_phy_setup = false,
        .intr_flags = ESP_INTR_FLAG_LEVEL1,
# ifdef ENABLE_ENUM_FILTER_CALLBACK
        .enum_filter_cb = set_config_cb,
# endif // ENABLE_ENUM_FILTER_CALLBACK
    };
    ESP_ERROR_CHECK(usb_host_install(&host_config));

    //Signalize the app_main, the USB host library has been installed
    xTaskNotifyGive(arg);

    bool has_clients = true;
    bool has_devices = true;
    while (has_clients || has_devices) {
        uint32_t event_flags;
        ESP_ERROR_CHECK(usb_host_lib_handle_events(portMAX_DELAY, &event_flags));
        if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS) {
            ESP_LOGI(TAG, "No more clients");
            has_clients = false;
            if (ESP_OK == usb_host_device_free_all()) {
                ESP_LOGI(TAG, "All devices marked as free");
            } else {
                ESP_LOGI(TAG, "Wait for the ALL FREE EVENT");
            }
        }
        if (event_flags & USB_HOST_LIB_EVENT_FLAGS_ALL_FREE) {
            ESP_LOGI(TAG, "No more devices");
            has_devices = false;
        }

    }
    ESP_LOGI(TAG, "No more clients and devices");

    //Uninstall the USB Host Library
    ESP_ERROR_CHECK(usb_host_uninstall());
    vTaskSuspend(NULL);
}

void run_usb(void)
{
    ESP_LOGI(TAG, "USB host library example");

    // Init BOOT button: Pressing the button simulates app request to exit
    // It will uninstall the class driver and USB Host Lib

    app_event_queue = xQueueCreate(10, sizeof(app_event_queue_t));
    app_event_queue_t evt_queue;

    TaskHandle_t host_lib_task_hdl, class_driver_task_hdl;

    //Create usb host lib task
    BaseType_t task_created;
    task_created = xTaskCreatePinnedToCore(usb_host_lib_task,
                                           "usb_host",
                                           4096,
                                           xTaskGetCurrentTaskHandle(),
                                           HOST_LIB_TASK_PRIORITY,
                                           &host_lib_task_hdl,
                                           0);
    assert(task_created == pdTRUE);

    //Wait unit the USB host library is installed
    ulTaskNotifyTake(false, 1000);

    //Create class driver task
    task_created = xTaskCreatePinnedToCore(class_driver_task,
                                           "class",
                                           4096,
                                           NULL,
                                           CLASS_TASK_PRIORITY,
                                           &class_driver_task_hdl,
                                           0);
    assert(task_created == pdTRUE);
    vTaskDelay(10);     //Add a short delay to let the tasks run

    while (1) {
        if (xQueueReceive(app_event_queue, &evt_queue, portMAX_DELAY)) {
            if (APP_EVENT == evt_queue.event_group) {
                // User pressed button
                usb_host_lib_info_t lib_info;
                ESP_ERROR_CHECK(usb_host_lib_info(&lib_info));
                if (lib_info.num_devices == 0) {
                    // End while cycle
                    break;
                } else {
                    ESP_LOGW(TAG, "To shutdown example, remove all USB devices and press button again.");
                    // Keep polling
                }
            }
        }
    }

    //Deregister client
    class_driver_client_deregister();
    vTaskDelay(10);

    //Delete the tasks
    vTaskDelete(class_driver_task_hdl);
    vTaskDelete(host_lib_task_hdl);

    xQueueReset(app_event_queue);
    vQueueDelete(app_event_queue);
    ESP_LOGI(TAG, "End of the example");
}

