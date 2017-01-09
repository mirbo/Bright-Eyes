/*
 *	Rewrite of DSA1 v3.02_de functions of seg029 (playmask)
 *	Functions rewritten: 10/10 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg029.cpp
*/

#include "v302de.h"

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg096.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if 0
struct coord {
	unsigned short x,y;
};

/* GUI_BUTTONS_POS */
/* Positions of the icons */
static const struct coord icon_pos[9] = {
	{241, 57}, {268, 57}, {295, 57},
	{241, 84}, {268, 84}, {295, 84},
	{241, 111}, {268, 111}, {295, 111}
};

/* DS:LOADED_MENU_ICONS - DS:0x5ed4 */
static signed char icon_array[9] = {
	-2, -2, -2,
	-2, -2, -2,
	-2, -2, -2
};
#endif

/**
 *	draw_playmask() - loads and draws the playmask to the screen
 */
//static
void draw_playmask(void)
{
	ds_writew(UPDATE_STATUSLINE, 0);

	/* load the desired playmask */
	if (ds_readb(PLAYMASK_US) != 0)
		load_pp20(ARCHIVE_FILE_PLAYM_US);
	else
		load_pp20(ARCHIVE_FILE_PLAYM_UK);

	ds_writeb(PP20_INDEX, ARCHIVE_FILE_PLAYM_UK);

	wait_for_vsync();

	set_palette(p_datseg + 0x26c3, 0xe0, 0x20);

	update_mouse_cursor();

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 319);
	ds_writew(PIC_COPY_Y2, 199);
	ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));

	do_pic_copy(0);

	wait_for_vsync();

	set_color(p_datseg + COLOR_BLACK, 0);

	set_palette(p_datseg + 0x27e3, 0xe0, 0x20);

	ds_writew(ANI_POSX, 16);
	ds_writew(ANI_POSY, 2);

	set_textcolor(0x1f, 0x1b);

	ds_writew(TEXTLINE_POSX, 196);
	ds_writew(TEXTLINE_POSY, 12);
	ds_writew(TEXTLINE_UNKNOWN, 103);
	ds_writew(TEXTLINE_MAXLEN, 113);

	ds_writew(TXT_TABPOS1, 205);
#if !defined(__BORLANDC__)
	ds_writed(ACTION_TABLE_PRIMARY, (long)RealMake(datseg, ACTION_TABLE_PLAYMASK));
#else
	ds_writed(ACTION_TABLE_PRIMARY, (long)MK_FP(datseg, ACTION_TABLE_PLAYMASK));
#endif
	ds_writed(ACTION_TABLE_SECONDARY, 0);

	ds_writew(UPDATE_STATUSLINE, 1);

	refresh_screen_size();

}

/**
 * copy_forename - copys the forename from a name
 * @dst:	wheres the forename should be stored
 * @name:	the full name
 *
 * A forename has a maximum length of 7 characters.
 */
void copy_forename(Bit8u *dst, Bit8u *name)
{

	int i;

	for (i = 0; i < 7; i++) {
		if (host_readb(name + i) == 0x20) {
			host_writeb(dst + i, 0);
			break;
		} else {
			host_writeb(dst + i, host_readb(name + i));
		}
	}

	host_writeb(dst + 7, 0);
}

/**
 *	draw_status_line() - draws the status line (pictures and names)
 */
void draw_status_line(void)
{
	signed short fg_bak, bg_bak;
	Bit8u *src, *dst;
	Bit16s head_bak;
	signed short i, j;

	ds_writew(UPDATE_STATUSLINE, 0);

	get_textcolor(&fg_bak, &bg_bak);

	for (i = 0; i < 7; i++) {
		/* Clear name field */
		do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR),
			ds_readw(HERO_PIC_POSX + i * 2), 190,
			ds_readw(HERO_PIC_POSX + i * 2) + 41, 197, 0);

		if (host_readb(get_hero(i) + HERO_TYPE) != HERO_TYPE_NONE) {

			copy_forename(Real2Host(ds_readd(DTP2)),
				get_hero(i) + HERO_NAME2);

			set_textcolor(0xff, 0);

			/* Gray the names of heros in another group */
			if (host_readb(get_hero(i) + HERO_GROUP_NO) != ds_readb(CURRENT_GROUP))
				set_textcolor(0x6f, 0);

			/* print the name */
			GUI_print_string(Real2Host(ds_readd(DTP2)),
				GUI_get_first_pos_centered(Real2Host(ds_readd(DTP2)),	ds_readw(HERO_PIC_POSX + i * 2), 43, 0), 190);
		}

		wait_for_vsync();
		update_mouse_cursor();

		if (!host_readbs(get_hero(i) + HERO_TYPE)) {
			clear_hero_icon(i);
		} else {
			if (host_readb(get_hero(i) + HERO_GROUP_NO) == ds_readb(CURRENT_GROUP)) {
				ds_writew(PIC_COPY_X1, ds_readw(HERO_PIC_POSX + 2 * i));
				ds_writew(PIC_COPY_Y1, 157);
				ds_writew(PIC_COPY_X2, ds_readw(HERO_PIC_POSX + 2 * i) + 31);
				ds_writew(PIC_COPY_Y2, 188);

				head_bak = -1;

				/* load skull if hero is dead */
				if (hero_dead(get_hero(i))) {
					head_bak = ds_readw(LOADED_HEAD_ID);
					load_in_head(41);
				}

				/* set the src pointer of the head */
				ds_writed(PIC_COPY_SRC, (hero_dead(get_hero(i)) ? ds_readd(DTP2) :
					(Bit32u)((RealPt)ds_readd(HEROS) + i * SIZEOF_HERO + HERO_PORTRAIT)));

				do_pic_copy(0);

				if (head_bak != -1)
					load_in_head(head_bak);
			} else {

				dst = Real2Host(ds_readd(RENDERBUF_PTR));
				head_bak = -1;

				/* load skull if hero is dead */
				if (hero_dead(get_hero(i))) {
					head_bak = ds_readw(LOADED_HEAD_ID);
					load_in_head(41);
				}

				/* set the src pointer of the head */
				/* TODO: expression to complicated ? */
				src = (hero_dead(get_hero(i))) ? Real2Host(ds_readd(DTP2)) : (get_hero(i) + HERO_PORTRAIT);

				/* Gray out picture */
				for (j = 0; j < 1024; src++, dst++, j++)
					*dst = *src + 0x40;

				ds_writew(PIC_COPY_X1, ds_readw(HERO_PIC_POSX + 2 * i));
				ds_writew(PIC_COPY_Y1, 157);
				ds_writew(PIC_COPY_X2, ds_readw(HERO_PIC_POSX + 2 * i) + 31);
				ds_writew(PIC_COPY_Y2, 188);
				ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));

				do_pic_copy(0);

				if (head_bak != -1)
					load_in_head(head_bak);
			}

			for (j = 0; j < 6; j++)
				ds_writew(CHAR_STATUS_BARS + i * 8 + j * 2, 0xffff);
		}

		refresh_screen_size();
	}

	set_textcolor(fg_bak, bg_bak);

	ds_writew(UPDATE_STATUSLINE, 1);
}

/**
 * clear_hero_icon - fills hero icon and bars with black color
 * @pos:	position of the hero
 */
void clear_hero_icon(unsigned short pos)
{

	/* fill icon area black */
	do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), ds_readw(HERO_PIC_POSX + pos * 2), 157,
		ds_readw(HERO_PIC_POSX + pos * 2) + 31, 188, 0);

	if (!host_readbs(get_hero(pos) + HERO_TYPE))
		/* fill bars area black */
		do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), ds_readw(HERO_PIC_POSX + pos * 2) + 33, 157,
			ds_readw(HERO_PIC_POSX + pos * 2) + 39, 188, 0);
}

/**
 * load_icon - loads an icon to the icoc_buffer
 * @fileindex:	index of the icon file (ICONS or BICONS)
 * @icon:	index if the icon in the file
 * @pos:	position of the icon (0-8)
 */
//static
void load_icon(Bit16u fileindex, Bit16s icon, Bit16s pos)
{
	Bit16u fd;

	fd = load_archive_file(fileindex);

	seek_archive_file(fd, icon * 576L, 0);

	read_archive_file(fd, Real2Host(ds_readd(BUF_ICON)) + pos * 576, 576);

	bc_close(fd);

	/* set a real or blank icon */
	ds_writeb(LOADED_MENU_ICONS + pos, (fileindex == ARCHIVE_FILE_ICONS) ? icon : -1);
}

/**
 *	draw_icons() - draws all icons
 */
void draw_icons(void)
{
	signed short i;

	if (ds_readb(PP20_INDEX) != ARCHIVE_FILE_PLAYM_UK)
		return;

	update_mouse_cursor();

	for (i = 0; i < 9; i++) {

		ds_writew(PIC_COPY_X1, ds_readw(GUI_BUTTONS_POS + i * 4));
		ds_writew(PIC_COPY_Y1, ds_readw(GUI_BUTTONS_POS + i * 4 + 2));
		ds_writew(PIC_COPY_X2, ds_readw(GUI_BUTTONS_POS + i * 4) + 23);
		ds_writew(PIC_COPY_Y2, ds_readw(GUI_BUTTONS_POS + i * 4 + 2) + 23);
		ds_writed(PIC_COPY_SRC, (Bit32u)((RealPt)ds_readd(BUF_ICON) + i * 576));

		if (ds_readbs(NEW_MENU_ICONS + i) != -1) {
			if (ds_readbs(LOADED_MENU_ICONS + i) != ds_readbs(NEW_MENU_ICONS + i))
				load_icon(ARCHIVE_FILE_ICONS, ds_readbs(NEW_MENU_ICONS + i), i);
		} else {
			if (ds_readbs(LOADED_MENU_ICONS + i) != -1)
				load_icon(ARCHIVE_FILE_BICONS, i, i);
		}

		do_pic_copy(0);
	}

	refresh_screen_size();
}

/**
 *	draw_main_screen() - draws the main screen
 */
void draw_main_screen(void)
{
	ds_writew(WALLCLOCK_X, 0xf1);
	ds_writew(WALLCLOCK_Y, 0x1f);
	ds_writew(WALLCLOCK_UPDATE, 0);

	set_var_to_zero();

	if (ds_readb(PP20_INDEX))
		draw_playmask();

	clear_loc_line();

	draw_status_line();

	draw_icons();

	draw_compass();

	ds_writew(WALLCLOCK_UPDATE, ds_writew(WALLCLOCK_REDRAW, 1));

	set_textcolor(0x1f, 0x1b);
}

void clear_loc_line(void)
{
	update_mouse_cursor();
	do_fill_rect((RealPt)ds_readd(FRAMEBUF_PTR), 3, 140, 316, 153, 0);
	refresh_screen_size();
}

/**
 * select_hero_icon - marks a heros icon in the playmask
 * @pos:	position of the hero
 *
 * This function is only used from sell- and repair screens.
 */
void select_hero_icon(unsigned short pos) {

	signed short fg_bak, bg_bak;

	/* paint a blue border for the pic and bars */
	do_border((RealPt)ds_readd(FRAMEBUF_PTR),
		ds_readw(HERO_PIC_POSX + pos * 2) - 1, 156,
		ds_readw(HERO_PIC_POSX + pos * 2) + 42, 189, (signed char)0xfc);

	/* paint a blue border for the name */
	do_border((RealPt)ds_readd(FRAMEBUF_PTR),
		ds_readw(HERO_PIC_POSX + pos * 2) - 1, 189,
		ds_readw(HERO_PIC_POSX + pos * 2) + 42, 198, (signed char)0xfc);

	/* save the textcolors */
	get_textcolor(&fg_bak, &bg_bak);

	/* copy the heros forename */
	copy_forename(Real2Host(ds_readd(DTP2)), get_hero(pos) + HERO_NAME2);

	/* set the textcolors */
	set_textcolor(0xfc, 0);

	/* print forename */
	GUI_print_string(Real2Host(ds_readd(DTP2)),
		GUI_get_first_pos_centered(Real2Host(ds_readd(DTP2)),
			ds_readw(HERO_PIC_POSX + pos * 2), 43, 0), 190);


	/* restore textcolors */
	set_textcolor(fg_bak, bg_bak);
}

/**
 * deselect_hero_icon - deselect a heros icon in the playmask
 * @pos:	position of the hero
 *
 * This function is only used from sell- and repair screens.
 */
/* TODO: this function an select_hero_icon() can be merged into one. */
void deselect_hero_icon(unsigned short pos) {

	signed short fg_bak, bg_bak;

	/* paint a gray border for the pic and bars */
	do_border((RealPt)ds_readd(FRAMEBUF_PTR),
		ds_readw(HERO_PIC_POSX + pos * 2) - 1, 156,
		ds_readw(HERO_PIC_POSX + pos * 2) + 42, 189, (signed char)0xe6);

	/* paint a gray border for the name */
	do_border((RealPt)ds_readd(FRAMEBUF_PTR),
		ds_readw(HERO_PIC_POSX + pos * 2) - 1, 189,
		ds_readw(HERO_PIC_POSX + pos * 2) + 42, 198, (signed char)0xe6);

	/* save the textcolors */
	get_textcolor(&fg_bak, &bg_bak);

	/* copy the heros forename */
	copy_forename(Real2Host(ds_readd(DTP2)), get_hero(pos) + HERO_NAME2);

	/* set the textcolors */
	set_textcolor(0xff, 0);

	/* print forename */
	GUI_print_string(Real2Host(ds_readd(DTP2)),
		GUI_get_first_pos_centered(Real2Host(ds_readd(DTP2)),
			ds_readw(HERO_PIC_POSX + pos * 2), 43, 0), 190);


	/* restore textcolors */
	set_textcolor(fg_bak, bg_bak);
}

#if !defined(__BORLANDC__)
}
#endif
