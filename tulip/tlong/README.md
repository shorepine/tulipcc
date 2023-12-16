# Tulip for the T-Long-Display

![T-Long](../../docs/pics/tlong_editor.jpg)


The [T-Long-Display]() 

I've ported [Tulip CC](../../README.md) to it and most things work just fine. The screen is 640x180, with a TFB of 108x22, using a AXS15231B controller. We're able to use our existing Tulip CC display code as a framebuffer and we blit over DMA each 12-row line of a screen every frame, to reach a very stable 30FPS when everything on the screen changes at once. There's no onboard audio.

You can get [batteries with the smaller JST pins already on](https://www.amazon.com/dp/B08T6GT7DV?ref=ppx_yo2ov_dt_b_product_details&th=1) that fit right in. 

To flash your T-Long with Tulip, follow the usual [Tulip CC flashing instructions](../../docs/tulip_flashing.md) except be in this directory (`tulipcc/tulip/tlong`) instead of `tulipcc/tulip/esp32s3`, and don't pass any `MICROPY_BOARD` parameter. 

We default the T-Long to a 6x8 REPL font, to get 108x22 REPL / editor screen. If this is too small, comment out the `idf_build_set_property(COMPILE_OPTIONS "-DTULIP_REPL_FONT_8X6" APPEND)` line at the end of `CMakeLists.txt` in this folder. 


Notes:

 * USB for MIDI and real keyboard *should* work, but it's annoying to test as the UART for monitoring goes over the same USB connection. There's a header on back for serial monitoring, so I'll eventually move to that and try to get USB working. Alternatively we could use MIDI over the exposed UART pins, but that would require a separate breakout board. 

Still todo 

 * Touchscreen support
 * ~~Serial console - you can read but not write to it~~ 
 * USB host support for keyboard and MIDI
 * Battery voltage reporting
 * Try 120MHz SPI 

