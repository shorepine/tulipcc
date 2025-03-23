// pins.h
// pins (and other MCU constants) 

#define SPI_LL_DATA_MAX_BIT_LEN (1 << 18)

#define I2C_NUM I2C_NUM_0
#define I2C_CLK_FREQ 400000
#define ESP_INTR_FLAG_DEFAULT 0

// stuff in the eagle
#define I2S_MCLK  3
#define I2S_BCLK  8
#define I2S_LRCLK 2
#define AMYOUT 6 // data going to the codec, eg DAC data
#define I2C_FOLLOWER_SCL 5
#define I2C_FOLLOWER_SDA 4 
#define I2C_MASTER_SCL 18
#define I2C_MASTER_SDA 17
#define AMYIN 9 // data coming from the codec, eg ADC  data
#define MIDI_OUT_PIN 14
#define MIDI_IN_PIN 21
#define MPIO_C0 7

#define SPI0_CS0 10
#define SPI0_MOSI 11
#define SPI0_SCK 12
#define SPI0_MISO 13

#define CV_IN1 16
#define CV_IN2 15










