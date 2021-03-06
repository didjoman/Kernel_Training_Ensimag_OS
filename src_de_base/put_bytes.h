#include <string.h>
#include <stdbool.h>                                           
#include <inttypes.h>

#ifndef PUT_BYTES_H
#define PUT_BYTES_H

// The screen contains 80 cols, 25 lines, each pixel = 2 Bytes 
#define SCREEN_NB_COLS 80
#define SCREEN_NB_LINES 25
#define SCREEN_SIZE SCREEN_NB_COLS * SCREEN_NB_LINES
#define SCREEN_MEM_SIZE 2 * SCREEN_SIZE
 
#define PORT_CURSOR_CMD 0x3D4
#define PORT_CURSOR_DATA 0x3D5



struct char_format{
	bool blink;
	uint8_t bg_color;
	uint8_t fg_color;
};

struct coordinate{
	uint8_t x;
	uint8_t y;
};

uint16_t *ptr_mem(uint32_t lig, uint32_t col);
void ecrit_car(uint32_t lig, uint32_t col, char c, struct char_format *format);
void place_curseur(uint32_t lig, uint32_t col);
void get_curseur(uint32_t* lig, uint32_t* col);
void set_text_format(bool blink, uint8_t bg_color, uint8_t fg_color);
void get_text_format(struct char_format * f);
void efface_ecran(void);
void traite_car(char c);
void defilement(void);
void console_putbytes(char *chaine, int32_t taille);


#endif
