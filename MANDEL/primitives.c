/*
** BitGraph primitive routines
*/
#include <stdio.h>

BG_cursor_pos(r,c)
int r, c;
{
	if ( r==0 && c==0 )
		printf("\033[H");
	else
		printf("\033[%d;%dH",r,c);
}
BG_load_font_char(op,chr,font,rwidth,rheight,xadjust,yadjust,xupdate,yupdate,x,y)
int op,chr,font,rwidth,rheight,xadjust,yadjust,xupdate,yupdate,x,y;
{
	switch ( op ) {
	case 0:
		printf("\033P:%d;%d;%d;%d;%d;%d;%d;%d;%dL",
		op,chr,font,rwidth,rheight,xadjust,yadjust,xupdate,yupdate);
		break;
	case 1:
		printf("\033P:%d;%d;%d;%d;%d;%d;%d;%d;%d;%dL",
		op,chr,font,rwidth,rheight,xadjust,yadjust,xupdate,yupdate,x,y);
		break;
	}
}
BG_set_drawing_op(ps)
{
	printf("\033:%dv",ps);
}
BG_move_graphic(x,y)
int x,y;
{
	printf("\033:%d;%dm",x,y);
}
BG_draw_graphic(x,y)
int x,y;
{
	printf("\033:%d;%dd",x,y);
}
BG_draw_point(x,y)
{
	printf("\033:%d;%dp",x,y);
}
/*
** Pop Context
*/
BG_pop_context()
{
	printf("\033:]");
}
/*
** Push Context
*/
BG_push_context()
{
	printf("\033:[");
}
/*
** Select Emulation Mode
*/
BG_select_em_mode(ps)
int ps;
{
	printf("\033:%dc",ps);
}
BG_set_mode(ps)
int ps;
{
	printf("\033[?%dh",ps);
}
BG_reset_mode(ps)
int ps;
{
	printf("\033[?%dl",ps);
}
/*
** Set Font Parameters
*/
BG_set_font(chr,font,rwidth,rheigth,xadjust,yadjust,xupdate,yupdate,cwidth,cheight)
int chr,font,rwidth,rheigth,xadjust,yadjust,xupdate,yupdate,cwidth,cheight;
{
	if ( chr==0 )
		printf("\033:1;;%d;%d;%d;%d;%d;%d;%d;%d;%dL",
		font,rwidth,rheigth,xadjust,yadjust,
		xupdate,yupdate,cwidth,cheight);
	else
		printf("\033:1;%d;%d;%d;%d;%d;%d;%d;%d;%d;%dL",
		chr,font,rwidth,rheigth,xadjust,yadjust,
		xupdate,yupdate,cwidth,cheight);
}
BG_reset_font(chr,font)
{
	printf("\033:2;");
	if ( chr )
		printf("%d",chr);
	printf(";");
	if ( font )
		printf("%d",font);
	printf("L");
}
BG_select_text_cursor(chr,font)
{
	printf("\033:4;%d;%dL",chr,font);
}
BG_black()
{
	printf("\033[?5l");
}
BG_erase_display(ps)
{
	printf("\033[");
	if ( ps )
		printf("%d",ps);
	printf("J");
}
BG_select_g0(font)
{
	printf("\033(%c",font+'0');
}
BG_select_g1(font)
{
	printf("\033)%c",font+'0');
}
BG_shift_in()
{
	printf("\017");
}
BG_shift_out()
{
	printf("\016");
}
BG_rast_op(opcode,xsrc,ysrc,xdest,ydest,width,height)
{
	printf("\033:%d;%d;%d;%d;%d;%d;%dr",
		opcode,xsrc,ysrc,xdest,ydest,width,height);
}

BG_fill_rectangle(opcode,xdest,ydest,width,height,chr,font)
{
	printf("\033:%d;%d;%d;%d;%d;%d;%df",
		opcode,xdest,ydest,width,height,chr,font);
}

/*
** Mouse routines
*/

BG_pointer_enable(flags,btime,dtime,x,y,scalex,scaley,incrx,incry,dx,dy,region,buttons)
{
	printf("\033:0;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;c",
		flags,btime,dtime,x,y,scalex,scaley,incrx,incry,dx,dy,region,buttons);
}

BG_set_pointer_flags(flags)
{
	printf("\033:0;%dc", flags);
}
BG_set_pointer_btime(btime)
{
	printf("\033:0;;%dc",btime);
}
BG_set_pointer_dtime(dtime)     
{
	printf("\033:0;;;%dc",dtime);
}
BG_set_pointer_x(x)             
{
	printf("\033:0;;;;%dc",x);
}
BG_set_pointer_y(y)             
{
	printf("\033:0;;;;;%dc",y);
}

BG_set_pointer_scalex(scalex)   
{
	printf("\033:0;;;;;;%dc",scalex);
}

BG_set_pointer_scaley(scaley)   
{
	printf("\033:0;;;;;;;%dc",scaley);
}

BG_set_pointer_incrx(incrx)     
{
	printf("\033:0;;;;;;;;%dc",incrx);
}

BG_set_pointer_incry(incry)     
{
	printf("\033:0;;;;;;;;;%dc",incry);
}

BG_set_pointer_dx(dx)           
{
	printf("\033:0;;;;;;;;;;%dc",dx);
}

BG_set_pointer_dy(dy)           
{
	printf("\033:0;;;;;;;;;;;%dc",dy);
}

BG_set_pointer_region(region)   
{
	printf("\033:0;;;;;;;;;;;;%dc",region);
}
BG_select_pointer_cursor(chr,font)
{
	printf("\033:5;%d;%dL",chr,font);
}
/*
** Region routines
*/

BG_create_region(region,x,y,width,height)
{
	if (region <= 0 )
		printf("\033:2;;%d;%d;%d;%dC",x,y,width,height);
	else
		printf("\033:2;%d;%d;%d;%d;%dC",region,x,y,width,height);
}

BG_region_report()
{
	char buf[1024], *p;
	int region;
	fflush(stdout);
	while ( getchar() != '\033' ) /* nil */ ;
	p = buf;
	while ( (*p++ = getchar()) != 'C' ) /* nil */ ;
	*p = 0;
	sscanf(buf,":2;%dC",&region);
	return region;
}

BG_region_margins(region,x,y,width,height)
{
	printf("\033:4;%d;%d;%d;%d;%dC",region,x,y,width,height);
}

BG_region_origin(region,x,y)
{
	printf("\033:5;%d;%d;%dC",region,x,y);
}

BG_region_switch(region)
{
	printf("\033:0;%dC",region);
}

BG_save_rectangle(llx, lly, width, height, font, chr)
{
	printf("\033P:1;%d;%d;%d;%d;0;0;0;0;%d;%dL", chr, font, width, height,
		llx,lly);
}

BG_string_terminator()
{
	printf("\033\\");
}

BG_fontbg(font,chrnum,rwidth,rheight,xadjust,yadjust,xupdate,yupdate,bits)
int font,chrnum,rwidth,rheight,xadjust,yadjust,xupdate,yupdate;
char *bits;
{
	int i, j, rowbytes;
	short shc;

	rowbytes = (rwidth+7)/8;
	BG_load_font_char(0,chrnum,font,rwidth,rheight,xadjust,yadjust,xupdate,yupdate);
	for ( i=0 ; i<rheight ; i++ ) {
		for ( j=0 ; j<rowbytes ; j+=2 ) {
			shc = *(short *)(bits+i*rowbytes+j);
			shc = swab(shc);
			BG_send(shc);
		}
	}
	printf("\033\\");
}
BG_send(shc)
long shc;
{
	int piece1, piece2;
	piece1 = ((shc>>10)&0x3f);
	piece2 = ((shc>>4 )&0x3f);
	if ( piece1 ) {
		putchar('@'+piece1);
		putchar('@'+piece2);
	} else if ( piece2 )
		putchar('@'+piece2);
	putchar('0'+(shc&0xf));

}
swab(s)
{
	return ((s>>8)&0xff) | (s<<8);
}
