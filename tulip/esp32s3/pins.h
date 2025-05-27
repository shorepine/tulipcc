// pins.h
// pins (and other MCU constants) for all Tulip variants

#define BK_LIGHT_ON_LEVEL  1
#ifndef DEFAULT_BRIGHTNESS
#define DEFAULT_BRIGHTNESS 9 // 9 is max, 1 is min
#endif
#define BK_LIGHT_OFF_LEVEL !BK_LIGHT_ON_LEVEL
#define SPI_LL_DATA_MAX_BIT_LEN (1 << 18)

#ifdef TDECK
#define TDECK_PERI_GPIO 10
#define TDECK_LCD_BK_LIGHT_GPIO  42
#define TDECK_LCD_RST_GPIO       -1
#define TDECK_LCD_CS_GPIO        12
#define TDECK_LCD_DC_GPIO        11
#define TDECK_LCD_PCLK_GPIO      40
#define TDECK_LCD_MOSI_GPIO     41 // MOSI
#define TDECK_LCD_MISO_GPIO     38 // MISO
#define TDECK_LCD_PIXEL_CLOCK_HZ (80 * 1000 * 1000)
#define TDECK_SPI_HOST_ID SPI2_HOST
#define TDECK_POWERON       10
#define TDECK_I2C_SDA       18
#define TDECK_I2C_SCL       8
#define LILYGO_KB_SLAVE_ADDRESS     0x55
#define TDECK_TOUCH_ADDR_0 0x14
#define TDECK_TOUCH_ADDR_1 0x5D
#define TDECK_TRACKBALL_UP 3
#define TDECK_TRACKBALL_DOWN 15
#define TDECK_TRACKBALL_LEFT 1
#define TDECK_TRACKBALL_RIGHT 2
#define TDECK_TRACKBALL_CLICK 0
#define TOUCH_INT 16
#define I2C_SCL 8
#define I2C_SDA 18
#define I2C_NUM I2C_NUM_0
#define I2C_CLK_FREQ 400000
#define TOUCH_RST -1
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_I2S_LRCLK 5
#define CONFIG_I2S_BCLK 7
#define CONFIG_I2S_DOUT 6
#define CONFIG_I2S_DIN -1
#define CONFIG_I2S_NUM 0
#define MIDI_OUT_PIN UART_PIN_NO_CHANGE
#define MIDI_IN_PIN UART_PIN_NO_CHANGE 
#endif

// Tulip4R10, V0, MaTouch7
#ifdef MAKERFABS 
#define I2C_SDA 17
#define I2C_SCL 18
#define TOUCH_INT -1
#define I2C_NUM I2C_NUM_0
#define I2C_CLK_FREQ 100000
#define DEFAULT_BRIGHTNESS 9 // 9 is max, 1 is min
#define HSYNC_BACK_PORCH 16
#define HSYNC_FRONT_PORCH 210
#define HSYNC_PULSE_WIDTH 30
#define VSYNC_BACK_PORCH 10
#define VSYNC_FRONT_PORCH 22
#define VSYNC_PULSE_WIDTH 13
#define PIN_NUM_HSYNC          39
#define PIN_NUM_VSYNC          41
#define PIN_NUM_DE             40
#define PIN_NUM_PCLK           42 

#ifdef MATOUCH7
#define TOUCH_RST 38
#define PIN_NUM_BK_PWM 10
#define PIN_R7 14
#define PIN_R6 21
#define PIN_R5 47
#define PIN_R4 48
#define PIN_R3 45
#define PIN_G7 4
#define PIN_G6 16
#define PIN_G5 15
#define PIN_G4 7
#define PIN_G3 6
#define PIN_G2 5
#define PIN_B7 1
#define PIN_B6 9
#define PIN_B5 46
#define PIN_B4 3
#define PIN_B3 8
#define ESP_INTR_FLAG_DEFAULT 0

// This assumes you've used a SD card adapter to get I2s audio 
#define CONFIG_I2S_LRCLK 13 //2
#define CONFIG_I2S_BCLK 12 //20 
#define CONFIG_I2S_DOUT 11 //19
#define CONFIG_I2S_DIN -1
#define CONFIG_I2S_NUM 0 

// No MIDI on Matouch7
#define MIDI_OUT_PIN UART_PIN_NO_CHANGE
#define MIDI_IN_PIN UART_PIN_NO_CHANGE

#elif defined TULIP4_R11
#define TOUCH_RST 48 
#define PIN_NUM_BK_PWM 47
#define PIN_R7 14
#define PIN_R6 21
#define PIN_R5 10
#define PIN_R4 13
#define PIN_R3 45
#define PIN_G7 4
#define PIN_G6 16
#define PIN_G5 15
#define PIN_G4 7
#define PIN_G3 6
#define PIN_G2 -1
#define PIN_B7 1
#define PIN_B6 9
#define PIN_B5 46
#define PIN_B4 38  
#define PIN_B3 -1 
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_I2S_LRCLK 2
#define CONFIG_I2S_BCLK 8 
#define CONFIG_I2S_DOUT 5 
#define CONFIG_I2S_DIN -1 
#define CONFIG_I2S_NUM 0 
#define MIDI_OUT_PIN 11
#define MIDI_IN_PIN 12

#define PIN_NUM_DISP_EN        -1
#endif
#endif // MAKERFABS



#ifdef TULIP_DIY
#define I2C_SCL                     17
#define I2C_SDA                     18
#define TOUCH_INT                   (5)
#define TOUCH_INT_IO_MUX            (PERIPHS_IO_MUX_GPIO5_U)
#define I2C_NUM                     I2C_NUM_0
#define TOUCH_RST                   -1
#define I2C_CLK_FREQ 400000
#define HSYNC_BACK_PORCH 139
#define HSYNC_FRONT_PORCH 140
#define HSYNC_PULSE_WIDTH 20
#define VSYNC_BACK_PORCH 20
#define VSYNC_FRONT_PORCH 12
#define VSYNC_PULSE_WIDTH 20
#define PIN_NUM_BK_LIGHT       39 // was 19, was 14 white
#define PIN_NUM_HSYNC          40
#define PIN_NUM_VSYNC          41
#define PIN_NUM_DE             42
#define PIN_NUM_PCLK           14 // was 38, was 20, was 13 black
#define PIN_NUM_BK_PWM         16

#define PIN_R7 15  
#define PIN_R6 7   
#define PIN_R5 6   
#define PIN_R4 -1 // NC
#define PIN_R3 -1 // NC

#define PIN_G7 46  
#define PIN_G6 3  
#define PIN_G5 8  
#define PIN_G4 -1 // NC
#define PIN_G3 -1 // NC
#define PIN_G2 -1 // NC

#define PIN_B7 21 
#define PIN_B6 12 
#define PIN_B5 -1 // NC
#define PIN_B4 -1 // NC
#define PIN_B3 -1 // NC 

#define PIN_NUM_DISP_EN        -1
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_I2S_LRCLK 4
#define CONFIG_I2S_BCLK 1
#define CONFIG_I2S_DOUT 2
#define CONFIG_I2S_DIN -1
#define CONFIG_I2S_NUM 0 
#define MIDI_OUT_PIN 11
#define MIDI_IN_PIN 47
#endif








