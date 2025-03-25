// usb_host.c
#include "usb_host.h"



// Turn this on if you want more USB debug info
#define DEBUG_USB


typedef union {
    struct {
        uint8_t bLength;                                        /**< Size of the descriptor in bytes */
        uint8_t bDescriptorType;                        /**< Constant name specifying type of HID descriptor. */
        uint16_t bcdHID;                                        /**< USB HID Specification Release Number in Binary-Coded Decimal (i.e., 2.10 is 210H) */
        uint8_t bCountryCode;                             /**< Numeric expression identifying country code of the localized hardware. */
        uint8_t bNumDescriptor;                         /**< Numeric expression specifying the number of class descriptors. */
        uint8_t bHIDDescriptorType;                 /**< Constant name identifying type of class descriptor. See Section 7.1.2: Set_Descriptor Request for a table of class descriptor constants. */
        uint16_t wHIDDescriptorLength;            /**< Numeric expression that is the total size of the Report descriptor. */
        uint8_t bHIDDescriptorTypeOpt;            /**< Optional constant name identifying type of class descriptor. See Section 7.1.2: Set_Descriptor Request for a table of class descriptor constants. */
        uint16_t wHIDDescriptorLengthOpt;     /**< Optional numeric expression that is the total size of the Report descriptor. */
    } USB_DESC_ATTR;
    uint8_t val[9];
} usb_hid_desc_t;



#ifdef DEBUG_USB
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
    fprintf(stderr, "%s bDescriptorType (interface): %d\n", prefix, intf->bDescriptorType);
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
    fprintf(stderr, "%s bDescriptorType (endpoint): %d\n", prefix, endpoint->bDescriptorType);
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
#else    // !DEBUG_USB

#define DBGPRINTF(s)    /* nothing */
#define DBGPRINTF1(s, a)    /* nothing */
#define DBGPRINTF2(s, a, b)    /* nothing */
#define DBGPRINTF3(s, a, b, c)    /* nothing */
#define DBGPRINTF4(s, a, b, c, d)    /* nothing */

#define show_config_desc(p, i)    /* nothing */
#define show_interface_desc(p, i)    /* nothing */
#define show_endpoint_desc(p, i)    /* nothing */

#endif // DEBUG_USB

#define KEYBOARD_BUFFER_SIZE 64
#define KEYBOARD_BYTES 8
#define MOUSE_BYTES 8
#define DEFAULT_TIMEOUT_MS  (5000)
uint16_t keyboard_bytes = KEYBOARD_BYTES;
uint16_t mouse_bytes = MOUSE_BYTES;

// How long you hold down a key before it starts repeating
#define KEY_REPEAT_TRIGGER_MS 500
// How often (in ms) to repeat a key once held
#define KEY_REPEAT_INTER_MS 90


const TickType_t HOST_EVENT_TIMEOUT = 1;
const TickType_t CLIENT_EVENT_TIMEOUT = 1;

const size_t USB_HID_DESC_SIZE = 9;

usb_host_client_handle_t Client_Handle;
usb_device_handle_t Device_Handle;

uint8_t Interface_Number_kb;
uint8_t Interface_Number_midi;
uint8_t Interface_Number_mouse;

bool keyboard_claimed = false;
bool keyboard_ready = false;

bool midi_claimed = false;
bool midi_has_out = false;
bool midi_has_in = false;
bool midi_ready = false;

bool mouse_claimed = false;
bool mouse_ready = false;

bool isKeyboardPolling = false;
int64_t KeyboardTimer=0;
uint8_t KeyboardInterval;
usb_transfer_t *KeyboardIn = NULL;

bool isMousePolling = false;
int64_t MouseTimer=0;
uint8_t MouseInterval;
usb_transfer_t *MouseIn = NULL;

uint8_t boot_protocol_requested = 0;

// Keep track of which keys / scan codes are being held
int64_t KeyRepeatTimer=0;
uint16_t current_held = 0;
int64_t current_held_ms = 0;
int64_t last_inter_trigger_ms = 0;


#define MIDI_IN_BUFFERS 8
#define MIDI_OUT_BUFFERS 8

usb_transfer_t *MIDIOut = NULL;
usb_transfer_t *MIDIIn[MIDI_IN_BUFFERS] = { NULL };


// This identifies a mouse HID report packet for a standard "boot" mouse. 
typedef struct {
    union {
        struct {
            uint8_t button1:    1;
            uint8_t button2:    1;
            uint8_t button3:    1;
            uint8_t reserved:   5;
        };
        uint8_t val;
    } buttons;
    int8_t x_displacement;
    int8_t y_displacement;
} __attribute__((packed)) hid_mouse_input_report_boot_t;

int16_t mouse_x_pos = 0;
int16_t mouse_y_pos = 0;
uint8_t mouse_buttons[3];



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
    #ifdef DEBUG_USB
    #endif
    //fprintf(stderr, "**midi_transfer_cb context: %p, num_bytes %d\n", transfer->context, transfer->actual_num_bytes);
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
                convert_midi_bytes_to_messages(p + i + 1, 3,1);
            }
            esp_err_t err = usb_host_transfer_submit(transfer);
            if (err != ESP_OK) {
                fprintf(stderr, "midi usb_host_transfer_submit err: 0x%x\n", err);
            }
        } else {
            //fprintf(stderr, "midi transfer->status %d\n", transfer->status);
        }
    }
}

uint8_t out_mtx = 0;
static void midi_out_transfer_cb(usb_transfer_t *transfer) {
    //fprintf(stderr, "**midi_out_transfer_cb context: %p, num_bytes %d\n", transfer->context, transfer->actual_num_bytes);
    xTaskNotifyGive(tulip_mp_handle);
}


bool check_interface_desc_MIDI(const void *p) {
    const usb_intf_desc_t *intf = (const usb_intf_desc_t *)p;

    // USB MIDI
    if ((intf->bInterfaceClass == USB_CLASS_AUDIO) &&
            (intf->bInterfaceSubClass == 3) &&
            (intf->bInterfaceProtocol == 0)) {
        midi_claimed = true;
        fprintf(stderr, "Claiming a MIDI device!\n");
        Interface_Number_midi = intf->bInterfaceNumber;
        esp_err_t err = usb_host_interface_claim(Client_Handle, Device_Handle,
                Interface_Number_midi, intf->bAlternateSetting);
        if (err != ESP_OK) fprintf(stderr, "midi usb_host_interface_claim failed: %x\n", err);
        return true;
    }
    return false;
}

void midi_out_transfer() {
    //fprintf(stderr, "sending %d bytes to midi\n", MIDIOut->num_bytes);
    esp_err_t err = usb_host_transfer_submit(MIDIOut);
    // Don't exit until you get the callback
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    if (err != ESP_OK) {
        fprintf(stderr, "midi OUT usb_host_transfer_submit err: 0x%x\n", err);
    }
}

void send_single_midi_out_packet(uint8_t * data) { // 4 bytes
    MIDIOut->data_buffer[0] = data[0];
    MIDIOut->data_buffer[1] = data[1];
    MIDIOut->data_buffer[2] = data[2];
    MIDIOut->data_buffer[3] = data[3];
    MIDIOut->num_bytes = 4;
    //fprintf(stderr, "sending midi out packet %02x %02x %02x %02x\n", data[0], data[1], data[2], data[3]);
    midi_out_transfer();
}

uint8_t usb_sysex_flag=0;
#define MAX_USB_SYSEX_BYTES (MAX_MESSAGE_LEN+3)
uint8_t usb_sysex_buffer[MAX_USB_SYSEX_BYTES];
uint16_t usb_sysex_len = 0;

void usb_emit_sysex() {
    // send any sysex out
    uint8_t data[64];
    uint8_t packet_count = 0;

    if(usb_sysex_len) {
        uint16_t i=0;
        while(i!=usb_sysex_len) {
            if (usb_sysex_len-i>3){
                data[(packet_count*4)+0] = 0x04;
                data[(packet_count*4)+1] = usb_sysex_buffer[i++];
                data[(packet_count*4)+2] = usb_sysex_buffer[i++];
                data[(packet_count*4)+3] = usb_sysex_buffer[i++];
            }else{
                switch(usb_sysex_len-i) {
                    case 1:
                        data[(packet_count*4)+0] = 0x05;
                        data[(packet_count*4)+1] = usb_sysex_buffer[i++];
                        break;
                    case 2:
                        data[(packet_count*4)+0] = 0x06;
                        data[(packet_count*4)+1] = usb_sysex_buffer[i++];
                        data[(packet_count*4)+2] = usb_sysex_buffer[i++];
                        break;
                    case 3:
                        data[(packet_count*4)+0] = 0x07;
                        data[(packet_count*4)+1] = usb_sysex_buffer[i++];
                        data[(packet_count*4)+2] = usb_sysex_buffer[i++];
                        data[(packet_count*4)+3] = usb_sysex_buffer[i++];
                        break;
                }
            }
            packet_count++;
            if(packet_count==16) {
                for(uint16_t j=0;j<64;j++) { MIDIOut->data_buffer[j] = data[j]; data[j]=0; }
                MIDIOut->num_bytes = 64;
                midi_out_transfer();
                packet_count = 0;
            }
        }
        for(uint16_t j=0;j<packet_count*4;j++) { MIDIOut->data_buffer[j] = data[j]; }
        MIDIOut->num_bytes = packet_count*4;
        midi_out_transfer();
    }
}

void send_usb_midi_out(uint8_t * data, uint16_t len) {
    // we have to discern code_index from data. basically a reverse version of our MIDI input parser.
    uint8_t usb_midi_packet[4] = {0,0,0,0};
    uint8_t usb_midi_message_slot = 0;
    for(size_t i=0;i<len;i++) {
        uint8_t byte = data[i];
        if(usb_sysex_flag) {
            usb_sysex_buffer[usb_sysex_len++] = byte;
            if(byte == 0xF7) {
                usb_sysex_flag = 0;
                usb_emit_sysex();
                usb_sysex_len = 0;
            }
        } else {
            if(byte & 0x80) { // new status byte 
                // Single byte message?
                usb_midi_packet[1] = byte;
                if(byte == 0xF4 || byte == 0xF5 || byte == 0xF6 || byte == 0xF9 ||  byte == 0xF8 ||
                    byte == 0xFA || byte == 0xFB || byte == 0xFC || byte == 0xFD || byte == 0xFE || byte == 0xFF) {
                    usb_midi_packet[0] = 0x05; // single byte system common message
                    send_single_midi_out_packet(usb_midi_packet);
                    i = len+1;
                }  else if(byte == 0xF0) {
                    usb_sysex_buffer[usb_sysex_len++] = 0xF0;
                    usb_sysex_flag = 1;
                } else { // a new status message that expects at least one byte of message after
                    // do nothing yet
                }
            } else { // data byte of some kind
                uint8_t status = usb_midi_packet[1] & 0xF0;
                // a 2 bytes of data message
                if(status == 0x80 || status == 0x90 || status == 0xA0 || status == 0xB0 || status == 0xE0 || usb_midi_packet[1] == 0xF2) {
                    usb_midi_packet[0] = (status >> 4); // status is code_index for these guys 
                    if(usb_midi_packet[1]==0xF2) usb_midi_packet[0] = 0x03; // special 
                    if(usb_midi_message_slot == 0) {
                        usb_midi_packet[2] = byte;
                        usb_midi_message_slot = 1;
                    } else {
                        usb_midi_packet[3] = byte;
                        usb_midi_message_slot = 0;
                        send_single_midi_out_packet(usb_midi_packet);
                    }
                // a 1 byte data message
                } else if (status == 0xC0 || status == 0xD0) {
                    usb_midi_packet[0] = (status >> 4);
                    usb_midi_packet[2] = byte;
                    send_single_midi_out_packet(usb_midi_packet);
                    i = len+1;
                } else if (usb_midi_packet[1] == 0xF3 || usb_midi_packet[1] == 0xF1) {
                    usb_midi_packet[0] = 0x02; // special
                    send_single_midi_out_packet(usb_midi_packet);
                    i = len+1;
                }
            }
        }
    }
    
}


void prepare_endpoint_midi(const void *p) {
    const usb_ep_desc_t *endpoint = (const usb_ep_desc_t *)p;
    esp_err_t err;

    // must be bulk for MIDI
    if ((endpoint->bmAttributes & USB_BM_ATTRIBUTES_XFERTYPE_MASK) != USB_BM_ATTRIBUTES_XFER_BULK) {
        fprintf(stderr, "USB MIDI: Not bulk endpoint: 0x%02x\n", endpoint->bmAttributes);
        return;
    }
    if (endpoint->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK) {
        // MIDI-IN endpoint.
        for (int i = 0; i < MIDI_IN_BUFFERS; i++) {
            if (MIDIIn[i] == NULL) {
                err = usb_host_transfer_alloc(endpoint->wMaxPacketSize, 0, &MIDIIn[i]);
                if (err != ESP_OK) {
                    MIDIIn[i] = NULL;
                    fprintf(stderr, "midi usb_host_transfer_alloc/In err: 0x%x\n", err);
                    midi_has_out = false;
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
                    fprintf(stderr, "midi usb_host_transfer_submit/In err: 0x%x\n", err);
                }
                midi_has_in = true;
            }
        }
    } else {
        // MIDI-OUT endpoint
        if (MIDIOut == NULL) {
            err = usb_host_transfer_alloc(endpoint->wMaxPacketSize, 0, &MIDIOut);
            if (err != ESP_OK) {
                MIDIOut = NULL;
                fprintf(stderr, "midi usb_host_transfer_alloc/Out err: 0x%x\n", err);
                midi_has_out = false;
                return;
            }
        }
        if (MIDIOut != NULL) {
            fprintf(stderr, "MIDI USB Out data_buffer_size: %d\n", MIDIOut->data_buffer_size);
            MIDIOut->device_handle = Device_Handle;
            MIDIOut->bEndpointAddress = endpoint->bEndpointAddress;
            MIDIOut->callback = midi_out_transfer_cb;
            MIDIOut->context = NULL;
            MIDIOut->flags |= USB_TRANSFER_FLAG_ZERO_PACK;
            midi_has_out = true;
        }
    }
    // MIDI is ready when both input and output endpoints are initialized.
    midi_ready = midi_has_in && midi_has_out;
}

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
            if (err != ESP_OK) fprintf(stderr, "usb_host_device_open: 0x%x\n", err);

            usb_device_info_t dev_info;
            err = usb_host_device_info(Device_Handle, &dev_info);
            if (err != ESP_OK) fprintf(stderr, "usb_host_device_info: 0x%x\n", err);
            fprintf(stderr,"speed: %d dev_addr %d vMaxPacketSize0 %d bConfigurationValue %d\n",
                    dev_info.speed, dev_info.dev_addr, dev_info.bMaxPacketSize0,
                    dev_info.bConfigurationValue);

            const usb_device_desc_t *dev_desc;
            err = usb_host_get_device_descriptor(Device_Handle, &dev_desc);
            if (err != ESP_OK) fprintf(stderr, "usb_host_get_device_desc: 0x%x\n", err);

            const usb_config_desc_t *config_desc;
            err = usb_host_get_active_config_descriptor(Device_Handle, &config_desc);
            if (err != ESP_OK) fprintf(stderr,"usb_host_get_config_desc: 0x%x\n", err);
            // Finally, we get to inspect the new device and maybe connect to it.
            new_enumeration_config_fn(config_desc);
            break;

        /**< A device opened by the client is now gone */
        case USB_HOST_CLIENT_EVENT_DEV_GONE:
            fprintf(stderr,"Device Gone handle: 0x%lx\n", (uint32_t)event_msg->dev_gone.dev_hdl);
            // Mark everything de-initialized so it will re-initialized on another connect.
            esp_err_t err;
            if (midi_claimed && (uint32_t)Device_Handle == (uint32_t)event_msg->dev_gone.dev_hdl) {
                err = usb_host_interface_release(Client_Handle, Device_Handle, Interface_Number_midi);
                if (err != ESP_OK) fprintf(stderr,"usb_host_interface_release err: 0x%x\n", err);
                midi_claimed = false;
                midi_ready = false;
                midi_has_in = false;
                midi_has_out = false;
            }
            if (keyboard_claimed && (uint32_t)Device_Handle == (uint32_t)event_msg->dev_gone.dev_hdl) {
                err = usb_host_interface_release(Client_Handle, Device_Handle, Interface_Number_kb);
                if (err != ESP_OK) fprintf(stderr,"usb_host_interface_release err: 0x%x\n", err);
                keyboard_claimed = false;
                keyboard_ready = false;
            }
            if (mouse_claimed && (uint32_t)Device_Handle == (uint32_t)event_msg->dev_gone.dev_hdl) {
                err = usb_host_interface_release(Client_Handle, Device_Handle, Interface_Number_mouse);
                if (err != ESP_OK) fprintf(stderr,"usb_host_interface_release err: 0x%x\n", err);
                mouse_claimed = false;
                mouse_ready = false;
                disable_mouse_pointer();
            }
            err = usb_host_device_close(Client_Handle, Device_Handle);
            if (err != ESP_OK) fprintf(stderr,"usb_host_device_close err: 0x%x\n", err);
            break;
        default:
            fprintf(stderr,"Unknown USB event: %d\n", event_msg->event);
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
            fprintf(stderr,"usb_host_lib_handle_events: 0x%x flags: %lx\n", err, event_flags);
        }
    }

    err = usb_host_client_handle_events(Client_Handle, CLIENT_EVENT_TIMEOUT);
    if ((err != ESP_OK) && (err != ESP_ERR_TIMEOUT)) {
        fprintf(stderr,"usb_host_client_handle_events: 0x%x\n", err);
    }
}



static char lvgl_kb_buf[KEYBOARD_BUFFER_SIZE];

void lvgl_keyboard_read(lv_indev_t * indev_drv, lv_indev_data_t * data) {
    (void) indev_drv;         // unused
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


void decode_keyboard_report(uint8_t *p) {
    // First byte, modifier mask
    uint8_t modifier = p[0];
    uint8_t new_key = 0;
    uint8_t key_held_this_session = 0;
    // Second byte, reserved
    // next 6 bytes, scan codes (for rollover)
    
   
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
                    send_key_to_micropython(c);
                }
            }     
        } 
    }
    if(!new_key && !key_held_this_session) {
        // we got a message but no new keys. so is a release
        current_held_ms = 0;
        current_held = 0;
        last_inter_trigger_ms = 0;
    }
    for(uint8_t i=0;i<8;i++) last_scan[i] = p[i];
}

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
void keyboard_transfer_cb(usb_transfer_t *transfer)
{
    if (Device_Handle == transfer->device_handle) {
        isKeyboardPolling = false;
        if (transfer->status == 0) {
            #ifdef DEBUG_USB 
            //fprintf(stderr, "KB nb is %d\n", transfer->actual_num_bytes);
            #endif
            if (transfer->actual_num_bytes == 8 || transfer->actual_num_bytes == 16 || transfer->actual_num_bytes==18) {
                uint8_t *const p = transfer->data_buffer;
                decode_keyboard_report(p);
            } else if (transfer->actual_num_bytes == 10) {
                uint8_t *const p = transfer->data_buffer;
                decode_keyboard_report(p+1);
            } else {
                // This is a weird keyboard (8bitdo retro, custom 40 key ortho, custom 44 key) that uses USB FS HID and sends keys as a bitmask. 
                // I found people talking about this here but no code, so here i am https://stackoverflow.com/questions/57793525/unusual-usb-hid-reports
                // I'm still not sure how to determine between "normal" scan code buffers or these types other than looking at actual_num_bytes
                uint8_t *const p = transfer->data_buffer;
                // We treat each bit on position as a index into a scan code, and will add it to a new buffer for decode_report to deal with
                uint8_t new_decode[8];
                for(uint8_t i=0;i<8;i++) new_decode[i] = 0;
                new_decode[0] = p[1]; // modifier goes here
                uint8_t bit_count = 0;
                uint8_t rollover_count = 2; // start writing as pos 2
                // p[0] has the length (starting from the modifier)
                for(uint8_t i=2;i<p[0]-1;i++) {
                    for(uint8_t bit = 0; bit < 8; bit++) {
                        if(CHECK_BIT(p[i], bit)) {
                            new_decode[rollover_count++] = bit_count;
                            if(rollover_count == 8) rollover_count = 2;
                        }
                        bit_count++;
                    }
                }
                decode_keyboard_report(new_decode);
            }
        }
        else {
            fprintf(stderr, "kbd transfer->status %d\n", transfer->status);
        }
    }
}


void mouse_transfer_cb(usb_transfer_t *transfer)
{
    //fprintf(stderr, "mouse nb is %d\n", transfer->actual_num_bytes);
    //uint8_t *p = transfer->data_buffer;
    //fprintf(stderr,"mouse decode report %d %d %d %d %d %d %d %d\n", p[0], p[1], p[2],p[3],p[4],p[5],p[6],p[7]);

   
    if (Device_Handle == transfer->device_handle) {
        isMousePolling = false;
        if (transfer->status == 0) {
            hid_mouse_input_report_boot_t *mouse_report = (hid_mouse_input_report_boot_t *)(transfer->data_buffer+1);
            mouse_x_pos += mouse_report->x_displacement;
            mouse_y_pos += mouse_report->y_displacement;
            if(mouse_x_pos >= H_RES) mouse_x_pos = H_RES-1;
            if(mouse_y_pos >= V_RES) mouse_y_pos = V_RES-1;
            if(mouse_x_pos < 0) mouse_x_pos = 0;
            if(mouse_y_pos < 0) mouse_y_pos = 0;
            mouse_buttons[0] = mouse_report->buttons.button1;
            mouse_buttons[1] = mouse_report->buttons.button2;
            mouse_buttons[2] = mouse_report->buttons.button3;
            last_touch_x[0] = mouse_x_pos;
            last_touch_y[0] = mouse_y_pos;
            enable_mouse_pointer();// this is a no-op if already installed
            if(mouse_buttons[0]) {
                send_touch_to_micropython(mouse_x_pos, mouse_y_pos, 0);
            } else {
                send_touch_to_micropython(mouse_x_pos, mouse_y_pos, 1);                
            }
        }
        else {
            fprintf(stderr, "mouse transfer->status %d\n", transfer->status);
        }
    }
}

bool check_interface_desc_boot_keyboard(const void *p) {
    const usb_intf_desc_t *intf = (const usb_intf_desc_t *)p;
    if ((intf->bInterfaceClass == USB_CLASS_HID) &&
            (intf->bInterfaceSubClass == 1) &&
            (intf->bInterfaceProtocol == 1)) {
        keyboard_claimed = true;
        Interface_Number_kb = intf->bInterfaceNumber;
        //fprintf(stderr, "claiming for kb %d\n", Interface_Number_kb);
        esp_err_t err = usb_host_interface_claim(Client_Handle, Device_Handle,
                                                Interface_Number_kb, intf->bAlternateSetting);
        if (err != ESP_OK) fprintf(stderr, "usb_host_interface_claim failed: 0x%x\n", err);
        return true;
    }
    return false;
}

bool check_interface_desc_boot_mouse(const void *p) {
    const usb_intf_desc_t *intf = (const usb_intf_desc_t *)p;
    if ((intf->bInterfaceClass == USB_CLASS_HID) &&
            (intf->bInterfaceSubClass == 1) &&
            (intf->bInterfaceProtocol == 2)) {
        mouse_claimed = true;
        Interface_Number_mouse = intf->bInterfaceNumber;
        esp_err_t err = usb_host_interface_claim(Client_Handle, Device_Handle,
                                                Interface_Number_mouse, intf->bAlternateSetting);
        if (err != ESP_OK) fprintf(stderr, "usb_host_interface_claim failed: 0x%x\n", err);
        return true;
    } 
    else if ((intf->bInterfaceClass == USB_CLASS_HID) &&
            (intf->bInterfaceSubClass == 0) &&
            (intf->bInterfaceProtocol == 0) && keyboard_claimed) {
        fprintf(stderr, "0x03 0x0 0x0 mouse claim\n");
        mouse_claimed = true;
        Interface_Number_mouse = intf->bInterfaceNumber;
        esp_err_t err = usb_host_interface_claim(Client_Handle, Device_Handle,
                                                Interface_Number_mouse, intf->bAlternateSetting);
        if (err != ESP_OK) fprintf(stderr, "usb_host_interface_claim failed: 0x%x\n", err);
        return true;
    }
    return false;
}

static void boot_protocol_cb(usb_transfer_t *transfer) {
    // nothing needed here
}

void request_boot_protocol() {
    // SETUP packet that requests boot protocol
    uint8_t setup[8];
    setup[0]= 0x21; // SETUP
    setup[1] = 0x0B; // setprotocol
    setup[2] = 0;
    setup[3] = 0; //boot protocol
    setup[4] = 0;
    setup[5] = 0;
    setup[6] = 0;
    setup[7] = 0;

    // Initialize transfer
    usb_transfer_t *ctrl_transfer;
    usb_host_transfer_alloc(8, 0, &ctrl_transfer);
    ctrl_transfer->device_handle = Device_Handle;
    ctrl_transfer->bEndpointAddress = 0; // default control endpoint address for the device
    ctrl_transfer->callback = boot_protocol_cb;
    ctrl_transfer->context = NULL;
    
    for(uint8_t i=0;i<sizeof(usb_setup_packet_t);i++) {
        ctrl_transfer->data_buffer[i] = setup[i];
    }
    ctrl_transfer->num_bytes = 8;
    esp_err_t err = usb_host_transfer_submit_control(Client_Handle, ctrl_transfer);
    fprintf(stderr, "boot protocol submit err is %d\n", err);
    boot_protocol_requested = 1;
}


void prepare_endpoint_hid_kb(const void *p)
{
    const usb_ep_desc_t *endpoint = (const usb_ep_desc_t *)p;
    esp_err_t err;
    keyboard_bytes = usb_round_up_to_mps(KEYBOARD_BYTES, endpoint->wMaxPacketSize);
    DBGPRINTF2("Setting KB to %d from MPS %d\n", keyboard_bytes, endpoint->wMaxPacketSize);

    // must be interrupt for HID
    if ((endpoint->bmAttributes & USB_BM_ATTRIBUTES_XFERTYPE_MASK) != USB_BM_ATTRIBUTES_XFER_INT) {
        fprintf(stderr, "Kbd: Not interrupt endpoint: 0x%02x\n", endpoint->bmAttributes);
        return;
    }
    if (endpoint->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK) {
        if (KeyboardIn == NULL) {
            //fprintf(stderr, "creating KeyboardIn\n");
            err = usb_host_transfer_alloc(keyboard_bytes, 0, &KeyboardIn);
            if (err != ESP_OK) {
                KeyboardIn = NULL;
                fprintf(stderr, "kbd usb_host_transfer_alloc/In err: 0x%x\n", err);
                return;
            }
        }
        if (KeyboardIn != NULL) {
            KeyboardIn->device_handle = Device_Handle;
            KeyboardIn->timeout_ms = DEFAULT_TIMEOUT_MS;
            KeyboardIn->num_bytes = keyboard_bytes;
            KeyboardIn->bEndpointAddress = endpoint->bEndpointAddress;
            KeyboardIn->callback = keyboard_transfer_cb;
            KeyboardIn->context = NULL;
            keyboard_ready = true;
            KeyboardInterval = endpoint->bInterval;
            if(!boot_protocol_requested) {
                fprintf(stderr, "requesting boot protocol\n");
                request_boot_protocol();
            }

            DBGPRINTF("USB boot keyboard ready\n");
        }
    } else {
            DBGPRINTF("Ignoring kbd interrupt Out endpoint\n");
    }
}
void prepare_endpoint_hid_mouse(const void *p)
{
    const usb_ep_desc_t *endpoint = (const usb_ep_desc_t *)p;
    esp_err_t err;
    mouse_bytes = usb_round_up_to_mps(MOUSE_BYTES, endpoint->wMaxPacketSize);
    DBGPRINTF2("Setting mouse to %d from MPS %d\n", mouse_bytes, endpoint->wMaxPacketSize);
    
    // must be interrupt for HID
    if ((endpoint->bmAttributes & USB_BM_ATTRIBUTES_XFERTYPE_MASK) != USB_BM_ATTRIBUTES_XFER_INT) {
        fprintf(stderr, "Mouse: Not interrupt endpoint: 0x%02x\n", endpoint->bmAttributes);
        return;
    }
    if (endpoint->bEndpointAddress & USB_B_ENDPOINT_ADDRESS_EP_DIR_MASK) {
        if (MouseIn == NULL) {
            err = usb_host_transfer_alloc(mouse_bytes, 0, &MouseIn);
            if (err != ESP_OK) {
                MouseIn = NULL;
                fprintf(stderr, "mouse usb_host_transfer_alloc/In err: 0x%x\n", err);
                return;
            }
        }
        if (MouseIn != NULL) {
            MouseIn->device_handle = Device_Handle;
            MouseIn->bEndpointAddress = endpoint->bEndpointAddress;
            MouseIn->callback = mouse_transfer_cb;
            MouseIn->context = NULL;
            mouse_ready = true;
            mouse_x_pos = H_RES/2;
            mouse_y_pos = V_RES/2;
            MouseInterval = endpoint->bInterval;
            if(!boot_protocol_requested) {
                fprintf(stderr, "requesting boot protocol\n");
                request_boot_protocol();
            }

            DBGPRINTF("USB boot mouse ready\n");
        }
    } else {
        DBGPRINTF("Ignoring mouse interrupt Out endpoint\n");
    }
    //}
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
                    if(!midi_claimed) { check_interface_desc_MIDI(p); }
                    if(!keyboard_claimed) { check_interface_desc_boot_keyboard(p); }
                    if(!mouse_claimed) { check_interface_desc_boot_mouse(p); }
                    last_descriptor = bDescriptorType;
                    break;
                case USB_B_DESCRIPTOR_TYPE_ENDPOINT:
                    show_endpoint_desc(p, indent);
                    if (keyboard_claimed && !keyboard_ready) {
                        prepare_endpoint_hid_kb(p);
                    }
                    if (mouse_claimed && !mouse_ready) {
                        prepare_endpoint_hid_mouse(p);
                    }
                    if (midi_claimed && !midi_ready) {
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
                    fprintf(stderr, "Unknown USB Descriptor Type: 0x%x\n", bDescriptorType);
                    break;
            }
        }
        else {
            fprintf(stderr, "USB Descriptor invalid\n");
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
        MouseTimer = esp_timer_get_time() / 1000;
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
        if (keyboard_ready && !isKeyboardPolling && (KeyboardTimer > KeyboardInterval)) {
            KeyboardIn->num_bytes = keyboard_bytes; 
            esp_err_t err = usb_host_transfer_submit(KeyboardIn);
            if (err != ESP_OK) {
                fprintf(stderr,"kbd usb_host_transfer_submit/In err: 0x%x\n", err);
            }
            isKeyboardPolling = true;
            KeyboardTimer = 0;
        }
        if (mouse_ready && !isMousePolling && (MouseTimer > MouseInterval)) {
            MouseIn->num_bytes = mouse_bytes; 
            esp_err_t err = usb_host_transfer_submit(MouseIn);
            if (err != ESP_OK) {
                fprintf(stderr, "mouse usb_host_transfer_submit/In err: 0x%x\n", err);
            }
            isMousePolling = true;
            MouseTimer = 0;
        }
    }
}
