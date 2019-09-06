# Example: e-paper dispalys

### This test code shows how to use ESP32 driver e-Paper displays
* Support for **Waveshare 2.7inch e-Paper HAT** based ePaper modules in 4-wire SPI mode
 
#### e-paper functions:

 * **Graphics drawing functions**:
  * **Paint_Clear**  Clear all screen memery
  * **Paint_DrawAbsolutePixel**  This draws a pixel by absolute coordinates
  * **Paint_GetImage**  Getters and Setters
  * **Paint_SetRotate**  Setup displays rotate
  * **Paint_DrawPixel**  Draws a pixel by the coordinates
  * **Paint_DrawCharAt**  Draw a charactor on the frame buffer but not refresh
  * **Paint_DrawStringAt**  Displays a string on the frame buffer but not refresh
  * **Paint_DrawLine**  Draws a line on the frame buffer
  * **Paint_DrawHorizontalLine**  Draw a horizontal line on the frame buffer
  * **Paint_DrawVerticalLine**  Draw a vertical line on the frame buffer
  * **Paint_DrawRectangle**  Draws a rectangle
  * **Paint_DrawFilledRectangle**  Draws a filled rectangle
  * **Paint_DrawCircle**  Draws a circle
  * **Paint_DrawFilledCircle**  Draws a filled circle

* **Fonts**:
  * Support for **font8**,**font12**,**font16**,**font20**,**font24**, fonts embeded
  
* **String write function**:
  * **EPD_print**  Write text to display.
* **Images**:
  * Can display the image form C array 

#### Connecting the display
  * mosi: 23 (display DIN)
  * sck: 18
  * CS:  5 (display CS)
  * DC: 26 (display DC)
  * RST: 27 (display RESET)
  * BUSY: 32 (display BUSY output)	
  
#### How to build
	
  `make menuconfig`

  Make and flash the example.

  `make all && make flash`
  
#### Test:
 * Dispaly espressif logo for 2s 
 * display the random value on e-paper, refresh 5s 

 



