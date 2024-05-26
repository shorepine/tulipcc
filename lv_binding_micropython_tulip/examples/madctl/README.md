# Determining the `colormode`, `invert`, and `rot` parameters

The [madctl_helper.py](madctl_helper.py) program can help determine the `colormode`, `invert`, and
 `rot` parameters for a display. Set the `mosi`, `clk`, `dc`, `cs`, `backlight`, `width`, and
 `height` parameters for your display. Start with `rot=0`, `colormode=COLOR_MODE_RGB`, and `rot=0`.
 Run the program with the display in the orientation you would like to configure.

##  Setting the `colormode` and `invert` parameter

The program will draw a large red full-screen button with an 'F' character centered in the button.
You can determine the `colormode` and `invert` parameters by observing the button's color.

  - If the button is RED, the `colormode` and `invert` parameters are correct.
  - If the button is BLUE, `colormode` should be `COLOR_MODE_BGR`.
  - If the button is YELLOW, `invert` should be `True`.
  - If the button is CYAN, `colormode` should be `COLOR_MODE_BGR` and `invert` should be `True`.

## Setting the `rot` parameter

The `rot` parameter is used to set the MADCTL register of the display. The MADCTL register controls
the order that pixels are written to the framebuffer. This sets the Orientation or Rotation of the
display. Match the orientation and direction of the 'F' character as drawn on the display by the
[madctl_helper.py](madctl_helper.py) program with the examples in the table below to determine the
`rot` parameter value.

Image | `rot` parameter
----- | ---------------
![MADCTL_0](images/madctl_0.png) | rot=0
![MADCTL_MY](images/madctl_y.png) | rot=MADCTL_MY
![MADCTL_MX](images/madctl_x.png) | rot=MADCTL_MX
![MADCTL_MX &#124; MADCTL_MY](images/madctl_xy.png) | rot=MADCTL_MX &#124; MADCTL_MY
![MADCTL_MV](images/madctl_v.png) | rot=MADCTL_MV
![MADCTL_MV &#124; MADCTL_MY](images/madctl_vy.png) | rot=MADCTL_MV &#124; MADCTL_MY
![MADCTL_MV &#124; MADCTL_MX](images/madctl_vx.png) | rot=MADCTL_MV &#124; MADCTL_MX
![MADCTL_MV &#124; MADCTL_MX &#124; MADCTL_MY](images/madctl_vxy.png) | rot=MADCTL_MV &#124; MADCTL_MX &#124; MADCTL_MX

## MADCTL Constants

The following table shows the MADCTL bit flags and their effects.

  Constant       | Value | Description
  -------------- | ----- | ----------------------
  MADCTL_MY      | 0x80  | Page Address Order
  MADCTL_MX      | 0x40  | Column Address Order
  MADCTL_MV      | 0x20  | Page/Column Order
  MADCTL_ML      | 0x10  | Line Address Order
  MADCTL_MH      | 0x04  | Display Data Latch Order
  COLOR_MODE_RGB | 0x00  | RGB color order
  COLOR_MODE_BGR | 0x08  | BGR color order
