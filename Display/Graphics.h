#ifndef GRAPHICS_H__
#define GRAPHICS_H__

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef long LONG;

#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;  //specifies the file type
    DWORD bfSize;  //specifies the size in bytes of the bitmap file
    WORD bfReserved1;  //reserved; must be 0
    WORD bfReserved2;  //reserved; must be 0
    DWORD bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;  //specifies the number of bytes required by the struct
    LONG biWidth;  //specifies width in pixels
    LONG biHeight;  //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//spcifies the type of compression
    DWORD biSizeImage;  //size of image in bytes
    LONG biXPelsPerMeter;  //number of pixels per meter in x axis
    LONG biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;  //number of colors used by th ebitmap
    DWORD biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;

#pragma pack(pop)

/**
 * This function pauses until the graphics chip status register indicates that it is idle
 */
void wait_for_graphics(void);

/**
 * Initializes all the registers related to VGA on the De1, and loads the colour palette
 */
int init_graphics(void);

/**
 * This function writes a single pixel to the x,y coords specified using the specified colour
 * Note colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
 */
void write_pixel(int x, int y, int Colour);

/**
 * This function read a single pixel from the x,y coords specified and returns its colour
 * Note returned colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
 */
int read_pixel(int x, int y);

/**
 * Subroutine to program a hardware (graphics chip) palette number with an RGB value
 * e.g. ProgramPalette(RED, 0x00FF0000) ;
 *
 */
void program_palette(int PaletteNumber, int RGB);

/**
 * Draws a horizontal line from (x1,y1) going length units to the right and
 * of colour Colour.
 */
void horizontal_line(int x1, int y1, int length, int Colour);

/**
 * Draws a vertical line from (x1,y1) going length units downwards and of
 * colour Colour.
 */
void vertical_line(int x1, int y1, int length, int Colour);

/**
 * Draws a line from (x1,y1) to (x2,y2) of colour Colour.
 */
void line(int x1, int y1, int x2, int y2, int Colour);

/**
 * Makes the screen black.
 */
void clear_screen(void);

/**
 * Draws a box from (x1,y1) to (x2,y2) of colour colour.
 */
void draw_box(int x1, int x2, int y1, int y2, int colour);

/**
 * Draws a hollow box from (x1,y1) to (x2,y2) of colour colour.
 */
void draw_hollow_box(int x1, int x2, int y1, int y2, int colour);

/**
 * Draws a filled box from (x1,y1) to (x2,y2) of colour colour.
 */
void draw_filled_box(int x1, int x2, int y1, int y2, int colour);

/**
 * Draws a bordered box from (x1,y1) to (x2,y2) of colour colour. Has a border width of
 * borderWidth and of colour borderColour. The inside of the box is coloured fillColour.
 */
void draw_bordered_box(int x1, int x2, int y1, int y2, int borderWidth, int fillColour, int borderColour);

/**
 * Given the coordinates of a rectangle from (x1,y1) to (x2,y2), center the string pointed to by title,
 * having font colour fontColour and filling in the non-text region of the font with buttonColour.
 */
void centre_button_text(int x1, int x2, int y1, int y2, const char *title, int buttonColour, int fontColour);

/**
 * Draws a hollow button from (x1,y1) to (x2,y2). The text in the button is the string pointed to by title, having font
 * colour fontColour. The colour behind the hollow button is set  by bgColour. The colour of the button box outline is buttonColour.
 */
void draw_hollow_button(int x1, int x2, int y1, int y2, const char *title, int bgColour, int buttonColour, int fontColour);

/**
 * Draws a filled button from (x1,y1) to (x2,y2). The text in the button is the string pointed to by title, having font
 * colour fontColour. The colour of the button is buttonColour.
 */
void draw_filled_button(int x1, int x2, int y1, int y2, const char *title, int buttonColour, int fontColour);

/**
 * Draws a hollow circle centered at (centerX, centerY) with radius radius. The colour of the radius of the circle
 * is of colour Colour.
 */
void draw_hollow_circle(int centreX, int centreY, int radius, int Colour);

/**
 * Draws a (mostly) filled circle centered at (centerX, centerY) with radius radius. The colour of the circle
 * is of colour Colour.
 */
void draw_filled_circle(int centreX, int centreY, int radius, int Colour);

/**
 * Draws a triangle connecting vertices (x1,y1), (x2,y2), and (x3,y3). The perimeter of the triangle
 * is of colour Colour.
 */
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int Colour);

/**
 * Given a path to the bmp file (bmp_filename) and a path to the colour paletter text file
 * (palette_filename), this function will re-program the colour palette and draw the image
 * over anything on the De1 screen.
 */
void draw_bitmap_image(char* bmp_filename, char* palette_filename);

/**
 * Given a path to the bmp file (filename) and a pointer to a bitmapInfoHeader struct,
 * this function will populate the struct and return a pointer to the start of the bitmap
 * image file pixel data.
 */
unsigned char * load_bitmap_file(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);

/**
 * Given a path to the palette file (filename) for a bitmap file, this function will program the
 * colour palette in the De1 memory.
 */
void load_bitmap_palette(char* filename);

/**
 * This function will reset the colour palette in the De1 memory with the original colours needed for
 * the GUI. This should be called after displaying an image, so that the GUI can be drawn properly.
 */
void load_original_colour_palette(void);

#endif /* GRAPHICS_H__ */
