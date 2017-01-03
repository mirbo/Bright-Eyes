/**
 *	Rewrite of DSA1 v3.02_de functions of seg069 (special buildings: Thorwal 2/2)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg069.cpp
 */

#include "v302de.h"

#include "seg004.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg032.h"
#include "seg054.h"
#include "seg059.h"
#include "seg097.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void THO_hetmann(void)
{
	signed short answer;
	signed short i;
	signed short map_parts;

	load_in_head(66);

	if (!ds_readb(GOT_LETTER_HET) &&
		ds_readw(GOT_MAIN_QUEST) != 0 &&
		(ds_readb(JURGE_AWAITS_LETTER) != 0 || ds_readb(NEED_LETTER) != 0))
	{

		/* count already collected parts of the map */
		for (i = map_parts = 0; i < 9; i++) {
			if (ds_readb(TREASURE_MAPS + i) != 0) {
				map_parts++;
			}
		}

		load_in_head(61);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				get_dtp(0x148), 0);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				get_dtp(0x14c), 0);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				ds_readws(MIN_MAP_PARTS) <= map_parts ? get_dtp(0x150) : get_dtp(0x154), 0);

		answer = ds_readws(MIN_MAP_PARTS) <= map_parts ? 10 : 50;

		for (i = 0; i < 14; i++) {
			add_ds_ds(GODS_ESTIMATION + 4 * i, answer);
		}

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				get_dtp(0x158), 0);

		ds_writeb(GOT_LETTER_HET, 1);
		get_item(235, 1, 1);

	} else if (ds_readw(HEARD_ANNOUNCE) == 0 || ds_readw(GOT_MAIN_QUEST) != 0) {
		GUI_output(get_city(0x00));

	} else {
		ds_writew(SUBVENTION, 0);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x04), 3,
					get_city(0x08),
					get_city(0x0c),
					get_city(0x10));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x14), 0);

		} else if (answer == 2) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x18), 0);

			add_ds_ws(SUBVENTION, 2);

		} else {

			GUI_dialogbox((RealPt)ds_readd(DTP2), (RealPt)0,
					get_city(0x1c), 0);

			inc_ds_ws(SUBVENTION);
		}

		load_in_head(61);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				get_city(0x20), 0);

		ds_writew(HEARD_ANNOUNCE, 3);

		GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
				get_city(0x24), 0);

		do {
			answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
					get_city(0x28), 3,
					get_city(0x2c),
					get_city(0x30),
					get_city(0x34));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
					get_city(0x38), 0);

			ds_writeb(INFORMER_FLAGS + INFORMER_ISLEIF, 1);
			inc_ds_ws(SUBVENTION);
			ds_writew(GOT_MAIN_QUEST, 1);
			ds_writew(QUESTED_MONTHS, 0);

		} else if (answer == 2) {

			GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
					get_city(0x3c), 0);

			ds_writeb(INFORMER_FLAGS + INFORMER_ISLEIF, 1);
			ds_writew(GOT_MAIN_QUEST, 1);
			ds_writew(QUESTED_MONTHS, 0);
			ds_writeb(GOT_LETTER_HET, 1);
			get_item(235, 1, 1);

		} else if (answer == 3) {

			add_ds_ws(SUBVENTION, 2);

			do {
				answer = GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
						get_city(0x40), 2,
						get_city(0x44),
						get_city(0x48));
			} while (answer == -1);

			if (answer == 1) {
				GUI_dialogbox((RealPt)ds_readd(DTP2), get_city(0x154),
						get_city(0x4c), 0);

				ds_writew(GOT_MAIN_QUEST, 1);
				ds_writew(QUESTED_MONTHS, 0);
			}
		}
	}
}

void THO_windriders(void)
{
	signed short answer;

	answer = GUI_radio(get_city(0x50), 3,
				get_city(0x54),
				get_city(0x58),
				get_city(0x5c));
	if (answer != -1) {

		if (answer == 1) {

			GUI_output(get_city(0x60));

		} else if (answer == 2 || answer == 3) {

			if (ds_readw(OTTA_WINDRIDERS) == 0) {

				GUI_output(answer == 2 ? get_city(0x64): get_city(0x68));

			} else if (ds_readw(OTTA_WINDRIDERS) == 1) {

				GUI_output(get_city(0x6c));

			} else {

				GUI_output(get_city(0x70));

				ds_writew(FIG_DISCARD, 1);
				do_fight(FIGHTS_THOR8);
			}

			inc_ds_ws(OTTA_WINDRIDERS);
		}
	}
}

void THO_tav_inn_combi(void)
{
	signed short answer;
	signed short type_bak;

	set_var_to_zero();
	load_ani(4);
	init_ani(0);

	answer = GUI_radio(get_ltx(0xa84), 3,
				get_ltx(0xa88),
				get_ltx(0xa8c),
				get_ltx(0x56c));

	/* save the combo typeindex */
	type_bak = ds_readw(TYPEINDEX);

	do {

		/* restore the combo typeindex */
		ds_writew(TYPEINDEX, type_bak);

		if (answer == 1) {

			/* enter TAVERN */

			/* set combo_mode active */
			ds_writew(COMBO_MODE, 1);

			/* set the typeindex of the corresponding tavern */
			answer = ds_readw(TYPEINDEX);
			ds_writew(TYPEINDEX, answer == 11 ? 0 : (
					answer == 14 ? 1 : (
					answer == 17 ? 2 : 6)));

			ds_writew(TEXTBOX_WIDTH, 3);

			ds_writeb(LOCATION, 3);
			do_tavern();

			/* leave the loop or enter the inn in the next iteration */
			answer = 0;
			if (ds_readw(COMBO_MODE) == 2) {
				answer = 2;
			}

		} else if (answer == 2) {

			/* enter INN */

			/* set combo_mode active */
			ds_writew(COMBO_MODE, 1);

			/* set the typeindex of the corresponding inn */
			answer = ds_readw(TYPEINDEX);
			ds_writew(TYPEINDEX, answer == 11 ? 70 : (
					answer == 14 ? 71 : (
					answer == 17 ? 72 : 73)));

			ds_writew(TEXTBOX_WIDTH, 3);

			ds_writeb(LOCATION, 7);
			do_inn();

			/* leave the loop or enter the tavern in the next iteration */
			answer = 0;
			if (ds_readw(COMBO_MODE) == 2) {
				answer = 1;
			}

		} else {
			set_var_to_zero();
			return;
		}

	} while (answer != 0);

	set_var_to_zero();
	turnaround();
}


void THO_stormchildren(void)
{
	signed short answer;

	answer = GUI_radio(get_city(0x74), 3,
				get_city(0x78),
				get_city(0x7c),
				get_city(0x80));
	if (answer != -1) {

		if (answer == 1) {

			GUI_output(get_city(0x84));

		} else if (answer == 2 || answer == 3) {

			if (ds_readw(OTTA_STORMCHILDREN) == 0) {

				GUI_output(answer == 2 ? get_city(0x88): get_city(0x8c));

			} else if (ds_readw(OTTA_STORMCHILDREN) == 1) {

				GUI_output(get_city(0x90));

			} else {

				GUI_output(get_city(0x94));

				ds_writew(FIG_DISCARD, 1);
				do_fight(FIGHTS_THOR8);
			}

			inc_ds_ws(OTTA_STORMCHILDREN);
		}
	}
}

void THO_garaldsson(void)
{
	signed short answer;

	answer = GUI_radio(get_city(0x98), 3,
				get_city(0x9c),
				get_city(0xa0),
				get_city(0xa4));
	if (answer != -1) {

		if (answer == 1) {

			GUI_output(get_city(0xa8));

		} else if (answer == 2 || answer == 3) {

			if (ds_readw(OTTA_GARALDSSON) == 0) {

				GUI_output(answer == 2 ? get_city(0xac): get_city(0xb0));

			} else if (ds_readw(OTTA_GARALDSSON) == 1) {

				GUI_output(get_city(0xb4));

			} else {

				GUI_output(get_city(0xb8));

				ds_writew(FIG_DISCARD, 1);
				do_fight(FIGHTS_THOR8);
			}

			inc_ds_ws(OTTA_GARALDSSON);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
