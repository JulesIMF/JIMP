### JIMP - Jules Image Manipulation Program

<img src="docs/images/JIMP alpha.png">

## What is that?
JIMP is very simple (and yet useless) graphics editor - you even can`t save somthing now :( 
But it already has a canvas, a palette, many layers and two instruments - brush and eraser.

## How to use?
When you open the app, you see two panels - canvas panel and palette panel. You can drag them, but cannot resize.

To apply instrument to the image, just click on canvas and begin to move the mouse. Note that the instrument is applied to the current layer!

To switch current layer, use Ctrl+PgUp and Ctrl+PgDown. To switch instrument, use PgUp and PgDown.

To open a BMP file, type ```./jimp "<filepath>"```.

## How to build?
Enter main JIMP folder, type ```make``` in the terminal. Note that must run under Linux with installed SFML!
