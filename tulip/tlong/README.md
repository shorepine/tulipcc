# Tulip for the T-Display-S3-Long

![T-Long](../../docs/pics/tlong_editor.jpg)


The [T-Display-S3-Long aka `T-Long`](https://www.lilygo.cc/en-ca/products/t-display-s3-long) is a $29 ESP32S3 with a wide 640x180 3.4" touchscreen display. 

I've ported [Tulip CC](../../README.md) to it and most things work just fine. The editor and REPL is 108x22, using their AXS15231B controller. We're able to use our existing Tulip CC display code as a framebuffer to reach a very stable 40FPS when everything on the screen changes at once. There's no onboard audio, so to use the synth you'll need to set up an [Alles speaker](https://github.com/bwhitman/alles). 

You can get [batteries with the smaller JST pins already on](https://www.amazon.com/dp/B08T6GT7DV?ref=ppx_yo2ov_dt_b_product_details&th=1) that fit right in. 

To flash your T-Long with Tulip, follow the usual [Tulip CC flashing instructions](../../docs/tulip_flashing.md) except be in this directory (`tulipcc/tulip/tlong`) instead of `tulipcc/tulip/esp32s3`, and don't pass any `MICROPY_BOARD` parameter. 

We default the T-Long to a 6x8 REPL font, to get 108x22 REPL / editor screen. If this is too small, comment out the `idf_build_set_property(COMPILE_OPTIONS "-DTULIP_REPL_FONT_8X6" APPEND)` line at the end of `CMakeLists.txt` in this folder. 

Notes:
 * If you're having trouble flashing the T-Long, hold down BOOT, then press RST briefly, then let go of BOOT. You may have to then hit RST to start up Tulip after the flash.
 * We had to use their screen driver as there's no pure C driver for the AXS15231B. Theirs is in Arduino, so we have to include a cut down version of Arduino in this repository. A great TODO is to port their driver from Arduino to pore C / ESP-IDF. 
 * Something about the QSPI screen driver conflicts with WiFi. I'm not able to connect to a WiFi network yet. 

Still todo 

 * Wifi doesn't work 
 * Touchscreen support
 * USB host support for keyboard and MIDI
 * Battery voltage reporting
 * Try 120MHz SPI 

