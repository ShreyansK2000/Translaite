#ifndef FONTS_H__
#define FONTS_H__

//Font size macros
#define SMALL	0
#define MEDIUM 1
#define LARGE 2
#define GIANT 3
#define MASSIVE 4

// Macros for font-specific sizes
#define FONT1_XPIXELS 5
#define FONT1_YPIXELS 7
#define FONT2_XPIXELS	10
#define FONT2_YPIXELS	14
#define FONT3_XPIXELS 16
#define FONT3_YPIXELS 27
#define FONT4_XPIXELS 22
#define FONT4_YPIXELS 40
#define FONT5_XPIXELS 38
#define FONT5_YPIXELS 59

/*
 * Prints the small size font at a given x and y coordinate. The font colour is the colour of the text.
 * The background colour is the colour of the non-text space in the font. C is the ascii code of the character.
 * Erase is a flag that sets the background to black. If you set Erase = 0, this makes the background transparent.
 */
void print_character_small_font(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase);

/*
 * Prints the medium size font at a given x and y coordinate. The font colour is the colour of the text.
 * The background colour is the colour of the non-text space in the font. C is the ascii code of the character.
 * Erase is a flag that sets the background to black. If you set Erase = 0, this makes the background transparent.
 */
void print_character_medium_font(int x, int y, int colour, int backgroundcolour, int c, int Erase);

/*
 * Prints the large size font at a given x and y coordinate. The font colour is the colour of the text.
 * The background colour is the colour of the non-text space in the font. C is the ascii code of the character.
 * Erase is a flag that sets the background to black. If you set Erase = 0, this makes the background transparent.
 */
void print_character_large_font(int x, int y, int colour, int backgroundcolour, int c, int Erase);

/*
 * Prints the giant size font at a given x and y coordinate. The font colour is the colour of the text.
 * The background colour is the colour of the non-text space in the font. C is the ascii code of the character.
 * Erase is a flag that sets the background to black. If you set Erase = 0, this makes the background transparent.
 */
void print_character_giant_font(int x, int y, int colour, int backgroundcolour, int c, int Erase);

/*
 * Prints the massive size font at a given x and y coordinate. The font colour is the colour of the text.
 * The background colour is the colour of the non-text space in the font. C is the ascii code of the character.
 * Erase is a flag that sets the background to black. If you set Erase = 0, this makes the background transparent.
 */
void print_character_massive_font(int x, int y, int colour, int backgroundcolour, int c, int Erase);

/*
 * Prints a word at the x and y coordinates specified by x_offset and y_offset. word is a pointer to the string
 * you want to display. Background colour is the colour of the space that is non-text in each character. Colour is
 * the colour of the font. Size if the character size.
 */
void write_word(int x_offset, int y_offset, const char *word, int backgroundColour, int colour, int size);

extern const unsigned char Font5x7[95][7];
extern const unsigned short int Font10x14[][14];
extern const unsigned char Font16x27[];
extern const unsigned char Font22x40[];
extern const unsigned char Font38x59[];

#endif
