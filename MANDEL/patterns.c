/*
** define some standard raster patterns
*/

#include <stdio.h>

#define FONT 6

static struct {
	char row[3]
} grey[10] = {
	0x00, 0x00, 0x00,
	0x00, 0x40, 0x00,
	0x00, 0xc0, 0x00,
	0x00, 0xc0, 0x40,
	0x00, 0xe0, 0x40,
	0x20, 0xe0, 0x40,
	0x20, 0xe0, 0xc0,
	0xa0, 0xe0, 0xc0,
	0xa0, 0xe0, 0xe0,
	0xe0, 0xe0, 0xe0
};

init_patterns()
{
	int i;
	BG_reset_font(0,FONT);
	BG_set_font(0,FONT,3,3,0,0,3,0,3,3);
	for ( i=0 ; i<10 ; i++ )
		BG_fontbg(FONT,'0'+i,3,3,0,0,3,0,&grey[i]);
	BG_select_g1(FONT);
}
test_patterns()
{
	int i,j,k;
	BG_shift_out();
	for ( i=0 ; i<10 ; i++ ) {
		for ( j=0 ; j<10 ; j++ ) {
			for ( k=0 ; k<100 ; k++ )
				putchar(i+'0');
			putchar('\n');
		}
	}
	BG_shift_in();
}
