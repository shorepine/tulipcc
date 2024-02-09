// pins.h
// pins (and other MCU constants) for all Tulip variants

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
#define BK_LIGHT_ON_LEVEL  1
#define DEFAULT_BRIGHTNESS 5 // 9 is max, 1 is min
#define BK_LIGHT_OFF_LEVEL !BK_LIGHT_ON_LEVEL
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
#define TDECK_TOUCH_INT 16
#define I2C_SCL 8
#define I2C_SDA 18
#endif

#ifdef MAKERFABS
#define I2C_SDA 17
#define I2C_SCL 18
#define TOUCH_INT -1
#define TOUCH_RST 38
#define I2C_NUM I2C_NUM_0
#define I2C_CLK_FREQ 400000
#elif defined TULIP_DIY
#define I2C_SCL                     17
#define I2C_SDA                     18
#define TOUCH_INT                   (5)
#define TOUCH_INT_IO_MUX            (PERIPHS_IO_MUX_GPIO5_U)
#define I2C_NUM                     I2C_NUM_0
#define TOUCH_RST                   -1
#define I2C_CLK_FREQ 400000
#endif

#ifdef TULIP_DIY
#define HSYNC_BACK_PORCH 139
#define HSYNC_FRONT_PORCH 140
#define HSYNC_PULSE_WIDTH 20
#define VSYNC_BACK_PORCH 20
#define VSYNC_FRONT_PORCH 12
#define VSYNC_PULSE_WIDTH 20
#elif defined MAKERFABS
#define HSYNC_BACK_PORCH 16
#define HSYNC_FRONT_PORCH 210
#define HSYNC_PULSE_WIDTH 30
#define VSYNC_BACK_PORCH 10
#define VSYNC_FRONT_PORCH 22
#define VSYNC_PULSE_WIDTH 13
#endif


#define BK_LIGHT_ON_LEVEL  1
#define DEFAULT_BRIGHTNESS 5 // 9 is max, 1 is min
#define BK_LIGHT_OFF_LEVEL !BK_LIGHT_ON_LEVEL

#ifdef TULIP_DIY
#define PIN_NUM_BK_LIGHT       39 // was 19, was 14 white
#define PIN_NUM_HSYNC          40
#define PIN_NUM_VSYNC          41
#define PIN_NUM_DE             42
#define PIN_NUM_PCLK           14 // was 38, was 20, was 13 black
#define PIN_NUM_BK_PWM         16
#define PIN_NUM_DATA0          12 //  B6, 14
#define PIN_NUM_DATA1          21 //  B7, 13
#define PIN_NUM_DATA2          8  // G5, 23
#define PIN_NUM_DATA3          3  // G6, 22
#define PIN_NUM_DATA4          46 // G7, 21 
#define PIN_NUM_DATA5          6   // R5, 31
#define PIN_NUM_DATA6          7   // R6, 30
#define PIN_NUM_DATA7          15  // R7, 29
//... We keep the rest as we have to drive them low 
#define PIN_NUM_DATA8          13 // B2
#define PIN_NUM_DATA9          13 // B1
#define PIN_NUM_DATA10         13 // B0
#define PIN_NUM_DATA11         13 // G2
#define PIN_NUM_DATA12         13 // G1
#define PIN_NUM_DATA13         13 // G0
#define PIN_NUM_DATA14         13 // R1
#define PIN_NUM_DATA15         13 // R0
#define PIN_NUM_DISP_EN        -1
#elif defined MAKERFABS
#define PIN_NUM_HSYNC          39
#define PIN_NUM_VSYNC          41
#define PIN_NUM_DE             40
#define PIN_NUM_PCLK           42 
#define PIN_NUM_BK_PWM         10
#define PIN_NUM_DATA0          9 //  B6, 14
#define PIN_NUM_DATA1          1 //  B7, 13
#define PIN_NUM_DATA2          15  // G5, 23
#define PIN_NUM_DATA3          16  // G6, 22
#define PIN_NUM_DATA4          4 // G7, 21 
#define PIN_NUM_DATA5          47   // R5, 31
#define PIN_NUM_DATA6          21   // R6, 30
#define PIN_NUM_DATA7          14  // R7, 29
#define PIN_NUM_DATA8          46 // B5
#define PIN_NUM_DATA9          3 // B4
#define PIN_NUM_DISP_EN        -1


// Two color pins are not hooked up on R10 but are in MaTouch 7
#ifdef MATOUCH7
#define PIN_NUM_DATA10         8 // B3
#else
#define PIN_NUM_DATA10         3 // B4, repeated
#endif

#define PIN_NUM_DATA11         7 // G4
#define PIN_NUM_DATA12         6 // G3

#ifdef MATOUCH7
#define PIN_NUM_DATA13         5 // G2
#else
#define PIN_NUM_DATA13         6 // G3, repeated
#endif

#define PIN_NUM_DATA14         48 // R4
#define PIN_NUM_DATA15         45 // R3
#endif

#define SPI_LL_DATA_MAX_BIT_LEN (1 << 18)


#if defined MATOUCH7
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_I2S_LRCLK 13 //2
#define CONFIG_I2S_BCLK 12 //20 
#define CONFIG_I2S_DIN 11 //19
#define CONFIG_I2S_NUM 0 
#elif defined TULIP4_R10
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_I2S_LRCLK 2
#define CONFIG_I2S_BCLK 8
#define CONFIG_I2S_DIN 5
#define CONFIG_I2S_NUM 0 
#elif defined TDECK
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_I2S_LRCLK 5
#define CONFIG_I2S_BCLK 7
#define CONFIG_I2S_DIN 6
#define CONFIG_I2S_NUM 0 
#elif defined TULIP_DIY
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_I2S_LRCLK 4
#define CONFIG_I2S_BCLK 1
#define CONFIG_I2S_DIN 2
#define CONFIG_I2S_NUM 0 
#endif

