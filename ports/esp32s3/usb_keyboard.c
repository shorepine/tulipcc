// usb_keyboard.c

#include "esp_rom_gpio.h"
#include "driver/gpio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "usb/usb_host.h"
#include "keyscan.h"
#include "tulip_helpers.h"

const size_t USB_HID_DESC_SIZE = 9;

// How long you hold down a key before it starts repeating
#define KEY_REPEAT_TRIGGER_MS 500
// How often (in ms) to repeat a key once held
#define KEY_REPEAT_INTER_MS 90

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




const TickType_t HOST_EVENT_TIMEOUT = 1;
const TickType_t CLIENT_EVENT_TIMEOUT = 1;

usb_host_client_handle_t Client_Handle;
usb_device_handle_t Device_Handle;
typedef void (*usb_host_enum_cb_t)(const usb_config_desc_t *config_desc);
static usb_host_enum_cb_t _USB_host_enumerate;
bool isKeyboard = false;
bool isKeyboardReady = false;
uint8_t KeyboardInterval;
bool isKeyboardPolling = false;
int64_t KeyboardTimer=0;


// vortex 8
// keychron 16
#define KEYBOARD_BYTES 16

const size_t KEYBOARD_IN_BUFFER_SIZE = KEYBOARD_BYTES; 
usb_transfer_t *KeyboardIn = NULL;


void _client_event_callback(const usb_host_client_event_msg_t *event_msg, void *arg)
{
  esp_err_t err;
  switch (event_msg->event)
  {
    /**< A new device has been enumerated and added to the USB Host Library */
    case USB_HOST_CLIENT_EVENT_NEW_DEV:
      printf("New device address: %d\n", event_msg->new_dev.address);
      err = usb_host_device_open(Client_Handle, event_msg->new_dev.address, &Device_Handle);
      if (err != ESP_OK) printf("usb_host_device_open: %x\n", err);

      usb_device_info_t dev_info;
      err = usb_host_device_info(Device_Handle, &dev_info);
      if (err != ESP_OK) printf("usb_host_device_info: %x\n", err);
      printf("speed: %d dev_addr %d vMaxPacketSize0 %d bConfigurationValue %d\n",
          dev_info.speed, dev_info.dev_addr, dev_info.bMaxPacketSize0,
          dev_info.bConfigurationValue);

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
      printf("Device Gone handle: %x\n", (uint32_t)event_msg->dev_gone.dev_hdl);
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
      printf("usb_host_lib_handle_events: %x flags: %x\n", err, event_flags);
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
    // Second byte, reserved
    // next 6 bytes, scan codes (for rollover)
    //printf("decode report %d %d %d %d %d %d\n", p[2],p[3],p[4],p[5],p[6],p[7]);
    for(uint8_t i=2;i<8;i++) {
		if(p[i]!=0) {
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
                    send_key_to_micropython(c);
				}
			}
			
		} 
    }
    if(!new_key) {
        // we got a message but no new keys. so is a release
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

void check_interface_desc_boot_keyboard(const void *p)
{
  const usb_intf_desc_t *intf = (const usb_intf_desc_t *)p;

  if ((intf->bInterfaceClass == USB_CLASS_HID) &&
      (intf->bInterfaceSubClass == 1) &&
      (intf->bInterfaceProtocol == 1)) {
    isKeyboard = true;
    esp_err_t err = usb_host_interface_claim(Client_Handle, Device_Handle,
        intf->bInterfaceNumber, intf->bAlternateSetting);
    if (err != ESP_OK) printf("usb_host_interface_claim failed: %x\n", err);
  }
}

void prepare_endpoint(const void *p)
{
  const usb_ep_desc_t *endpoint = (const usb_ep_desc_t *)p;
  esp_err_t err;

  // must be interrupt for HID
  if ((endpoint->bmAttributes & USB_BM_ATTRIBUTES_XFERTYPE_MASK) != USB_BM_ATTRIBUTES_XFER_INT) {
    printf("Not interrupt endpoint: 0x%02x\n", endpoint->bmAttributes);
    return;
  }
  if (endpoint->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK) {
    err = usb_host_transfer_alloc(KEYBOARD_IN_BUFFER_SIZE, 0, &KeyboardIn);
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
    printf("USB boot keyboard ready\n");
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
          break;
        case USB_B_DESCRIPTOR_TYPE_STRING:
          printf("USB string desc TBD\n");
          break;
        case USB_B_DESCRIPTOR_TYPE_INTERFACE:
          check_interface_desc_boot_keyboard(p);
          break;
        case USB_B_DESCRIPTOR_TYPE_ENDPOINT:
          if (isKeyboard && KeyboardIn == NULL) prepare_endpoint(p);
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

      // Handle key repeat
      if(current_held > 0) {
        if(KeyboardTimer - current_held_ms > KEY_REPEAT_TRIGGER_MS) {
            if(KeyboardTimer - last_inter_trigger_ms > KEY_REPEAT_INTER_MS) {
                send_key_to_micropython(current_held);
                last_inter_trigger_ms = KeyboardTimer;
            }
        }
      }
      if (isKeyboardReady && !isKeyboardPolling && (KeyboardTimer > KeyboardInterval)) {
        // vortex is 8
        // keychron is 16
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
