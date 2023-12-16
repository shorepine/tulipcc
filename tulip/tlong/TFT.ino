#include "AXS15231B.h"
#include <Arduino.h>

extern uint8_t test1_180640_map[];
extern uint8_t test2_180640_map[];
extern uint8_t test3_180640_map[];
extern uint8_t test4_180640_map[];

bool result = false;
void setup() {
    Serial.begin(115200);
    Serial.println("sta\n");

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    axs15231_init();

    lcd_PushColors(0, 0, 180, 640, (uint16_t *)test1_180640_map);

    Serial.println("end\n");
}

extern uint32_t transfer_num;
extern size_t lcd_PushColors_len;
int cont = 0;
void loop() {
    delay(1);
    cont++;

    if(cont >= 12000)
        cont = 0;

    if (transfer_num <= 0 && lcd_PushColors_len <= 0)
    {
        #ifdef LCD_SPI_DMA
            char i = 0;
            while (get_lcd_spi_dma_write())
            {
                i = i >> 1;
                lcd_PushColors(0, 0, 0, 0, NULL);
            }
        #endif

        if(cont == 0)
            lcd_PushColors(0, 0, 180, 640, (uint16_t *)test1_180640_map);
        else if(cont == 4000)
            lcd_PushColors(0, 0, 180, 640, (uint16_t *)test2_180640_map);
        else if(cont == 8000)
            lcd_PushColors(0, 0, 180, 640, (uint16_t *)test3_180640_map);
    }

    if (transfer_num <= 1 && lcd_PushColors_len > 0) {
        lcd_PushColors(0, 0, 0, 0, NULL);
    }
}
