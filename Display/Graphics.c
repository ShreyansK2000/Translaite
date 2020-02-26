#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Graphics.h"
#include "Fonts.h"
#include "../Hardware/HardwareRegisters.h"
#include "Colours.h"
#include "../Hardware/Touch.h"
#include "../Global.h"

// graphics register addresses
#define GraphicsCommandRegOffset   				(0x00010000)
#define GraphicsStatusRegOffset   				(0x00010000)
#define GraphicsX1RegOffset						(0x00010002)
#define GraphicsY1RegOffset   					(0x00010004)
#define GraphicsX2RegOffset   					(0x00010006)
#define GraphicsY2RegOffset   					(0x00010008)
#define GraphicsColourRegOffset   				(0x0001000E)
#define GraphicsBackGroundColourRegOffset   	(0x00010010)

// #defined constants representing values we write to the graphics 'command' register to get
// it to draw something. You will add more values as you add hardware to the graphics chip
// Note DrawHLine, DrawVLine and DrawLine at the moment do nothing - you will modify these

#define DrawHLine		1
#define DrawVLine		2
#define DrawLine		3
#define DrawCircle		0x11
#define	PutAPixel		0xA
#define	GetAPixel		0xB
#define	ProgramPaletteColour    0x10

volatile unsigned short int *GraphicsCommandReg = NULL;
volatile unsigned short int *GraphicsStatusReg = NULL;
volatile unsigned short int *GraphicsX1Reg = NULL;
volatile unsigned short int *GraphicsY1Reg = NULL;
volatile unsigned short int *GraphicsX2Reg = NULL;
volatile unsigned short int *GraphicsY2Reg = NULL;
volatile unsigned short int *GraphicsColourReg = NULL;
volatile unsigned short int *GraphicsBackGroundColourReg = NULL;

void wait_for_graphics() {
	while ((*GraphicsStatusReg & 0x0001) != 0x0001);
}

int init_graphics() {
	GraphicsCommandReg = (unsigned short int *) (virtual_base
			+ (( GraphicsCommandRegOffset) & (HW_REGS_MASK)));
	if (GraphicsCommandReg == NULL) {
		printf(
				"ERROR, GraphicsCommandReg was not correctly defined. Try again");
		return 0;
	}

	GraphicsStatusReg = (unsigned short int *) (virtual_base
			+ (( GraphicsStatusRegOffset) & (HW_REGS_MASK)));
	if (GraphicsStatusReg == NULL) {
		printf("ERROR, GraphicsStatusReg was not correctly defined. Try again");
		return 0;
	}

	GraphicsX1Reg = (unsigned short int *) (virtual_base
			+ (( GraphicsX1RegOffset) & (HW_REGS_MASK)));
	if (GraphicsX1Reg == NULL) {
		printf("ERROR, GraphicsX1Reg was not correctly defined. Try again");
		return 0;
	}

	GraphicsY1Reg = (unsigned short int *) (virtual_base
			+ (( GraphicsY1RegOffset) & (HW_REGS_MASK)));
	if (GraphicsY1Reg == NULL) {
		printf("ERROR, GraphicsY1Reg was not correctly defined. Try again");
		return 0;
	}

	GraphicsX2Reg = (unsigned short int *) (virtual_base
			+ (( GraphicsX2RegOffset) & (HW_REGS_MASK)));
	if (GraphicsX2Reg == NULL) {
		printf("ERROR, GraphicsX2Reg was not correctly defined. Try again");
		return 0;
	}

	GraphicsY2Reg = (unsigned short int *) (virtual_base
			+ (( GraphicsY2RegOffset) & (HW_REGS_MASK)));
	if (GraphicsY2Reg == NULL) {
		printf("ERROR, GraphicsY2Reg was not correctly defined. Try again");
		return 0;
	}

	GraphicsColourReg = (unsigned short int *) (virtual_base
			+ (( GraphicsColourRegOffset) & (HW_REGS_MASK)));
	if (GraphicsColourReg == NULL) {
		printf("ERROR, GraphicsY2Reg was not correctly defined. Try again");
		return 0;
	}

	GraphicsBackGroundColourReg = (unsigned short int *) (virtual_base
			+ (( GraphicsBackGroundColourRegOffset) & (HW_REGS_MASK)));
	if (GraphicsBackGroundColourReg == NULL) {
		printf(
				"ERROR, GraphicsBackGroundColourReg was not correctly defined. Try again");
		return 0;
	}

	// Program the palette for the GUI
	load_original_colour_palette();

	return 1;
}

void write_pixel(int x, int y, int Colour) {
	wait_for_graphics();

	*GraphicsX1Reg = x;				// write coords to x1, y1
	*GraphicsY1Reg = y;
	*GraphicsColourReg = Colour;			// set pixel colour
	*GraphicsCommandReg = PutAPixel;	// give graphics "write pixel" command
}

int read_pixel(int x, int y) {
	wait_for_graphics();

	*GraphicsX1Reg = x;			// write coords to x1, y1
	*GraphicsY1Reg = y;
	*GraphicsCommandReg = GetAPixel;	// give graphics a "get pixel" command

	wait_for_graphics();
	return (int) (GraphicsColourReg);	// return the palette number (colour)
}

void program_palette(int PaletteNumber, int RGB) {
	wait_for_graphics();

	*GraphicsColourReg = PaletteNumber;
	*GraphicsX1Reg = RGB >> 16;       // program red value in ls.8 bit of X1 reg
	*GraphicsY1Reg = RGB;      // program LIME and blue into ls 16 bit of Y1 reg
	*GraphicsCommandReg = ProgramPaletteColour; // issue command
}

void horizontal_line(int x1, int y1, int length, int Colour) {
	wait_for_graphics();

	*GraphicsColourReg = Colour;
	*GraphicsX1Reg = x1;
	*GraphicsY1Reg = y1;
	*GraphicsX2Reg = x1 + length;
	*GraphicsCommandReg = DrawHLine;
}

void vertical_line(int x1, int y1, int length, int Colour) {
	wait_for_graphics();

	*GraphicsColourReg = Colour;
	*GraphicsX1Reg = x1;
	*GraphicsY1Reg = y1;
	*GraphicsY2Reg = y1 + length;
	*GraphicsCommandReg = DrawVLine;
}

void line(int x1, int y1, int x2, int y2, int Colour)
{
    wait_for_graphics();              // is graphics ready for new command

    *GraphicsX1Reg = x1;              // write coords to x1, y1, and x2, y2
    *GraphicsY1Reg = y1;
    *GraphicsX2Reg = x2;
    *GraphicsY2Reg = y2;
    *GraphicsColourReg = Colour;         // set pixel colour
    *GraphicsCommandReg = DrawLine;         // give graphics "draw line" command
}

void clear_screen() {
	int i;
	for (i = 0; i < YRES; i++) {
		horizontal_line(0, i, XRES, BLACK);
	}
}

void FillScreen(int Colour)
{
    draw_filled_box(0, XRES - 1, 0, YRES - 1, Colour);
}

void draw_hollow_box(int x1, int x2, int y1, int y2, int colour) {
	horizontal_line(x1, y1, x2 - x1, colour);
	horizontal_line(x1, y2, x2 - x1, colour);
	vertical_line(x1, y1, y2 - y1, colour);
	vertical_line(x2, y1, y2 - y1, colour);
}

void draw_filled_box(int x1, int x2, int y1, int y2, int colour) {
	int i;
	for (i = y1; i <= y2; i++) {
		horizontal_line(x1, i, x2 - x1, colour);
	}
}

void draw_bordered_box(int x1, int x2, int y1, int y2, int borderWidth, int fillColour, int borderColour)
{
	draw_filled_box(x1, x2, y1, y1 + borderWidth, borderColour); //top
	draw_filled_box(x1, x2, y2 - borderWidth, y2, borderColour); //bottom
	draw_filled_box(x1, x1 + borderWidth, y1, y2, borderColour); //left
	draw_filled_box(x2 - borderWidth, x2, y1, y2, borderColour); //right

	draw_filled_box(x1 + borderWidth, x2 - borderWidth, y1 + borderWidth, y2 - borderWidth, fillColour);
}

void centre_button_text(int x1, int x2, int y1, int y2, const char *title, int buttonColour, int fontColour)
{
	int titleSize = strlen(title);
	int width = x2 - x1;
	int height = y2 - y1;
	int textStartingX;
	int textStartingY;

	int charWidth = width/titleSize;

	// Pick the largest font that can fit in the button box
	if (charWidth < (FONT2_XPIXELS+2)) { // + 2 is for space
		textStartingY = y1 + height/2 - FONT1_YPIXELS/2; // Centre the y coordinate of the font
		textStartingX = x1 + (width-((FONT1_XPIXELS+2)*titleSize))/2; // Centre the x coordinate of the font
		write_word(textStartingX, textStartingY, title, buttonColour, fontColour, SMALL); // Draw the word in the box
	}
	else if (charWidth < (FONT3_XPIXELS+2)) {
		textStartingY = y1 + height/2 - FONT2_YPIXELS/2;
		textStartingX = x1 + (width-((FONT2_XPIXELS+2)*titleSize))/2;
		write_word(textStartingX, textStartingY, title, buttonColour, fontColour, MEDIUM);
	} else {
		textStartingY = y1 + height/2 - FONT3_YPIXELS/2;
		textStartingX = x1 + (width-((FONT3_XPIXELS+2)*titleSize))/2;
		write_word(textStartingX, textStartingY, title, buttonColour, fontColour, LARGE);
	}

	// TODO: Fix bug for larger fonts and uncomment this
	/*else if (charWidth < (FONT4_XPIXELS+2)) {
		textStartingY = y1 + height/2 - FONT3_YPIXELS/2;
		textStartingX = x1 + (width-((FONT3_XPIXELS+2)*titleSize))/2;
		writeWord(textStartingX, textStartingY, title, buttonColour, fontColour, LARGE);
	}
	else if (charWidth < (FONT5_XPIXELS+2)) {
		textStartingY = y1 + height/2 - FONT4_YPIXELS/2;
		textStartingX = x1 + (width-((FONT4_XPIXELS+2)*titleSize))/2;
		writeWord(textStartingX, textStartingY, title, buttonColour, fontColour, GIANT);
	}
	else {
		textStartingY = y1 + height/2 - FONT5_YPIXELS/2;
		textStartingX = x1 + (width-((FONT5_XPIXELS+2)*titleSize))/2;
		writeWord(textStartingX, textStartingY, title, buttonColour, fontColour, MASSIVE);
	}*/
}

void draw_hollow_button(int x1, int x2, int y1, int y2, const char *title, int bgColour, int buttonColour, int fontColour)
{
	draw_hollow_box(x1, x2, y1, y2, buttonColour);
	centre_button_text(x1, x2, y1, y2, title, bgColour, fontColour);
}

void draw_filled_button(int x1, int x2, int y1, int y2, const char *title, int buttonColour, int fontColour)
{
	draw_filled_box(x1, x2, y1, y2, buttonColour);
	centre_button_text(x1, x2, y1, y2, title, buttonColour, fontColour);
}

void draw_hollow_circle(int centreX, int centreY, int radius, int Colour)
{
	wait_for_graphics();              // is graphics ready for new command

    *GraphicsX1Reg = centreX;              // write coords to x1, y1
    *GraphicsY1Reg = centreY;
    *GraphicsX2Reg = radius;             // write radius
    *GraphicsColourReg = Colour;         // set pixel colour
    *GraphicsCommandReg = DrawCircle;         // give graphics "draw line" command
}

void draw_filled_circle(int centreX, int centreY, int radius, int Colour)
{
    int i;
    for(i = 1; i <= radius; i++) {
        draw_hollow_circle(centreX, centreY, i, Colour);
    }
}

void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int Colour)
{
    line(x1, y1, x2, y2, Colour);
    line(x2, y2, x3, y3, Colour);
    line(x3, y3, x1, y1, Colour);
}

void draw_bitmap_image(char* bmp_filename, char* palette_filename) {
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char *bitmapData;

	load_bitmap_palette(palette_filename);

	bitmapData =  load_bitmap_file(bmp_filename,&bitmapInfoHeader);

	int x;
	int y;
	int i = 0;

	for(y = bitmapInfoHeader.biHeight-1; y >= 0 ; y--) {
		for(x = 0; x < bitmapInfoHeader.biWidth; x++) {
				write_pixel(x,y, bitmapData[i++]);
			}
	}

	free(bitmapData);
}

// This code is from: https://stackoverflow.com/questions/14279242/read-bitmap-file-into-structure
unsigned char * load_bitmap_file(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    unsigned char *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    unsigned char tempRGB;  //our swap variable

    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return NULL;

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(filePtr);
        return NULL;
    }

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr);

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,bitmapInfoHeader->biSizeImage,1,filePtr);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(filePtr);
        return NULL;
    }

    //swap the r and b values to get RGB (bitmap is BGR)
    for (imageIdx = 0;imageIdx < bitmapInfoHeader->biSizeImage;imageIdx+=3) // fixed semicolon
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    //close file and return bitmap iamge data
    fclose(filePtr);
    return bitmapImage;
}

void load_original_colour_palette() {
	int c = 0;
	for(c = 0; c < 8; c++) { // 8 = number of colours
		program_palette(c, OriginalColourPaletteData[c]);
	}
}

void load_bitmap_palette(char* filename) {
    FILE* file = fopen(filename, "r");
    char line[256];

    // Read the text file and write each RGB hex value to memory
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
    	int rgb_number = (int)strtol(&line[1], &line[6], 16);

    	program_palette(i++, rgb_number);
    }

    fclose(file);
}
