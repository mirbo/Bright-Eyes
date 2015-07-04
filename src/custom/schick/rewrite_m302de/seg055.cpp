/**
 *	Rewrite of DSA1 v3.02_de functions of seg055 (merchant: main)
 *	Functions rewritten: 2/6
 */
#include <string.h>

#include "v302de.h"

#include "seg002.h"
#include "seg004.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg055.h"
#include "seg056.h"
#include "seg057.h"
#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief		add an item to the shop
 * \param shop_ptr	pointer to the shop descrition
 * \param item_id	item_id
 * \param pos		position in the shop
 */
/* Borlandified and identical */
void add_item_to_shop(Bit8u *shop_ptr, signed short item_id, signed short pos)
{
	host_writews(Real2Host(ds_readd(0xc009)) + 7 * pos, item_id);

	host_writews(Real2Host(ds_readd(0xc009)) + 7 * pos + 2,
		host_readws(get_itemsdat(item_id) + 8) + host_readws(get_itemsdat(item_id) + 8) * host_readbs(shop_ptr) / 100);

	host_writews(Real2Host(ds_readd(0xc009)) + 7 * pos + 4,
			host_readbs(get_itemsdat(item_id) + 7));
}

struct dummy7 {
	char a[7];
};

/* Borlandified and identical */
void do_merchant(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else
	signed short l_si;
	signed short item_pos;
	signed short done;
	signed short answer;
	signed short refresh;
	signed short armor_pos;
	Bit32s party_money;
	Bit8u *shop_p;

	done = 0;

	if ((ds_readds(DAY_TIMER) < HOURS(8) || ds_readds(DAY_TIMER) > HOURS(19)) && ds_readb(LOCATION) != 9) {

		GUI_output(get_ltx(0x788));
		turnaround();
		return;
	}

	if (ds_readb(0x34d6 + ds_readws(TYPEINDEX)) != 0) {
		if (ds_readbs(0x6871 + 9 * ds_readws(TYPEINDEX)) != 3) {
			talk_merchant();
			turnaround();
			return;
		}
	} else if (ds_readb(0x3592 + ds_readws(TYPEINDEX)) != 0) {
		GUI_output(get_ltx(0x7ec));
		turnaround();
		return;
	}

	load_ggsts_nvf();
	refresh = ds_writews(0x2846, 1);

	ds_writed(0xc009, ds_readd(0xd2df));
	memset(Real2Host(ds_readd(0xc009)), 0, 3500);
	ds_writew(0xe3f6, 4);
	shop_p = p_datseg + 0x6870 + 9 * ds_readws(TYPEINDEX);

	for (l_si = 0; l_si < 100; l_si++) {
		host_writews(Real2Host(ds_readd(0xc009)) + 7 * l_si, 0);
	}

	l_si = 1;
	item_pos = 0;
	armor_pos = 70;

	while (host_readws(get_itemsdat(l_si)) != -1) {

		if (host_readbs(shop_p + 2) <= host_readbs(get_itemsdat(l_si) + 0xa)) {

			if (item_armor(get_itemsdat(l_si)) || item_weapon(get_itemsdat(l_si))) {

				if (host_readbs(shop_p + 1) == 1) {

					add_item_to_shop(shop_p, l_si,
						item_weapon(get_itemsdat(l_si)) ? item_pos++ : armor_pos++);
				}

			} else if (item_herb_potion(get_itemsdat(l_si))) {

				if (host_readbs(shop_p + 1) == 2) {

					add_item_to_shop(shop_p, l_si, item_pos++);
				}

			} else {

				if (host_readbs(shop_p + 1) == 3) {

					add_item_to_shop(shop_p, l_si, item_pos++);
				}
			}
		}

		l_si ++;
	}

	for (l_si = 0; l_si < 3; l_si++) {
		if (host_readws(shop_p + 2 * l_si + 3) != 0) {
			add_item_to_shop(shop_p, host_readws(shop_p + 2 * l_si + 3), item_pos++);
		}
	}

	if (host_readbs(shop_p + 1) == 1) {

		qsort(Real2Host(ds_readd(0xc009)), item_pos, 7, shop_compar);
		qsort(Real2Host(ds_readd(0xc009)) + 7 * 70, armor_pos - 70, 7, shop_compar);

		/* copy the rest */
		for (l_si = 0; armor_pos - 70 > l_si; l_si++) {

			*(struct dummy7*)(Real2Host(ds_readd(0xc009)) + 7 * (item_pos + l_si)) =
				*(struct dummy7*)(Real2Host(ds_readd(0xc009)) + 7 * (l_si + 70));
		}
		/* cleanup */
		for (l_si = item_pos + armor_pos - 70; l_si < 100; l_si++) {
			host_writews(Real2Host(ds_readd(0xc009)) + 7 * l_si, 0);
		}

	} else {
		qsort(Real2Host(ds_readd(0xc009)), item_pos, 7, shop_compar);
	}

	while (done == 0 && !ds_readb(0x3592 + ds_readws(TYPEINDEX))) {

		if (ds_readws(0x2846) != 0) {

			draw_loc_icons(4, 22, 24, 21, 8);

			draw_main_screen();

			set_var_to_zero();

			load_ani(host_readbs(shop_p + 1) == 1 ? 15 : (host_readbs(shop_p + 1) == 2 ? 22 : 14));

			init_ani(0);

			set_audio_track(145);

			GUI_print_loc_line(ds_readbs(LOCATION) == 9 ? get_ltx(0xa9c) : (ds_readws(TYPEINDEX) == 93 ?  get_ltx(0xb8) : get_dtp(4 * ds_readws(CITYINDEX))));

			ds_writew(0x2846, refresh = 0);

		}

		if (refresh != 0) {
			GUI_print_loc_line(get_dtp(4 * ds_readws(CITYINDEX)));
			refresh = 0;
		}

		handle_gui_input();

		if (ds_readb(CURRENT_TOWN) == 1 ||	/* THORWAL */
			ds_readb(CURRENT_TOWN) == 39 || /* PREM */
			ds_readb(CURRENT_TOWN) == 18 ||	/* PHEXCAER */
			ds_readb(CURRENT_TOWN) == 17)	/* OBERORKEN */
		{

			/* debt logic in shops */

			if (ds_readws(BANK_DEPOSIT) <= -1000 && ds_readws(DEBT_DAYS) == 0) {

				GUI_output(get_ltx(0xbe4));
				ds_writews(DEBT_DAYS, 7);
			}

			if (ds_readws(DEBT_DAYS) == -1) {

				if (GUI_bool(get_ltx(0xbe8))) {

					party_money = get_party_money();

					if (party_money < 5000) {

						GUI_output(get_ltx(0xbec));
						ds_writews(0xc3d3, ds_writews(ACTION, 0));
						done = 1;

					} else {
						add_ds_ws(BANK_DEPOSIT, 500);
						if (ds_readws(BANK_DEPOSIT) > -1000) {
							ds_writews(DEBT_DAYS, 0);
						}

						party_money -= 5000;
						set_party_money(party_money);
					}
				} else {

					GUI_output(get_ltx(0xbec));
					ds_writews(0xc3d3, ds_writews(ACTION, 0));
					done = 1;
				}
			}
		}

		if (ds_readws(0xc3d3) != 0 || ds_readws(ACTION) == 73) {

			answer = GUI_radio(get_ltx(0x6b8), 4,
						get_ltx(0x6bc), get_ltx(0x6c0),
						get_ltx(0x55c), get_ltx(0x6c8)) - 1;

			if (answer != -2) {
				ds_writews(ACTION, answer + 129);
			}
		}

		if (ds_readws(ACTION) == 132) {

			done = 1;

		} else if (ds_readws(ACTION) == 129) {


			buy_screen();

		} else if (ds_readws(ACTION) == 130) {

			sell_screen(shop_p);

		} else if (ds_readws(ACTION) == 131) {

			talk_merchant();

			if (ds_readb(0x34d6 + ds_readws(TYPEINDEX)) != 0) {
				done = 1;
			}
		}
	}

	copy_palette();

	turnaround();
#endif
}

void talk_merchant(void)
{
#if !defined(__BORLANDC__)
	DUMMY_WARNING();
#else
#endif
}

void TLK_ghandel(signed short state)
{
	DUMMY_WARNING();
}

void TLK_khandel(signed short state)
{
	DUMMY_WARNING();
}

void TLK_whandel(signed short state)
{
	DUMMY_WARNING();
}

#if !defined(__BORLANDC__)
}
#endif
