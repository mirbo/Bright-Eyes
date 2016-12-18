/**
 *	Rewrite of DSA1 v3.02_de functions of seg084 (dungeon: dungeon)
 *	Functions rewritten: 10/10 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg084.cpp
 */

#include "v302de.h"

#include <stdio.h>
#include <string.h>

#include "seg000.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg027.h"
#include "seg032.h"
#include "seg047.h"
#include "seg049.h"
#include "seg075.h"
#include "seg081.h"
#include "seg082.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG09_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short l3;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;

	amap_ptr = p_datseg + 0xbd95;
	tw_bak = ds_readws(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 7);

	target_pos = 4096 * ds_readbs(DUNGEON_LEVEL) + 256 * ds_readws(X_TARGET) + ds_readws(Y_TARGET);

	/* TODO: not the leader ? */
	hero = get_hero(0);

	if (ds_readb(0x40fd) != 0 && !(ds_readds(DAY_TIMER) / 90 * 20))
	{
		GUI_output(get_dtp(0x80));
	}

	if (target_pos == 0xb0e && target_pos != ds_readws(0x330e) && ds_readbs(DIRECTION) == 1)
	{
		GUI_output(get_dtp(0x04));

	} else if (target_pos == 0xa08 && target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x08));

	} else if ((target_pos == 0xe0e || target_pos == 0xe08) && target_pos != ds_readws(0x330e))
	{
		GUI_output(get_dtp(0x0c));

	} else if ((target_pos == 0x30e || target_pos == 0x50e ||
			target_pos == 0x10e ||	target_pos == 0x101 ||
			target_pos == 0x103 ||	target_pos == 0x601 ||
			target_pos == 0x901 ||	target_pos == 0x604 ||
			target_pos == 0x904 ||	target_pos == 0xc06 ||
			target_pos == 0x1c07 ||	target_pos == 0x1e04 ||
			target_pos == 0x1e06 ||	target_pos == 0x1d0e ||
			target_pos == 0x1a0e ||	target_pos == 0x110e ||
			target_pos == 0x1408 ||	target_pos == 0x1b01 ||
			target_pos == 0x1601 ||	target_pos == 0x1801 ||
			target_pos == 0x1603 ||	target_pos == 0x1803) &&
			target_pos != ds_readws(0x330e))
	{
		DNG09_statues(10, 20);

	} else if (target_pos == 0x40b && target_pos != ds_readws(0x330e) &&
			ds_readb(0x40f6) != 0 && !ds_readb(0x40f5))
	{
		GUI_output(get_dtp(0x2c));

		dec_ds_bs_post(0x40f6);

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_attrib(hero, 4, 4) <= 0)
			{
				/* failed FF+4 test */
				sub_hero_le(hero, dice_roll(2, 6, 4));

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x30),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}

	} else if (target_pos == 0x108 && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x38)))
		{
			GUI_output(get_dtp(0x3c));

			add_ds_ws(X_TARGET, 2);
			ds_writebs(DIRECTION, 1);
			DNG_inc_level();
		}

	} else if (target_pos == 0x405 && target_pos != ds_readws(0x330e) &&
			ds_readb(0x40f7) != 0 && ds_readb(0x40f5) != 0)
	{
		dec_ds_bs_post(0x40f7);

		GUI_output(get_dtp(0x2c));

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_attrib(hero, 4, 4) <= 0)
			{
				/* failed FF+4 test */
				sub_hero_le(hero, dice_roll(2, 6, 4));

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x30),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}

	} else if (target_pos == 0x202 && target_pos != ds_readws(0x330e) &&
			ds_readb(0x40f8) != 0)
	{
		dec_ds_bs_post(0x40f8);

		GUI_output(get_dtp(0x2c));

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_attrib(hero, 4, 4) <= 0)
			{
				/* failed FF+4 test */
				sub_hero_le(hero, dice_roll(2, 6, 4));

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x30),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}

	} else if (target_pos == 0x403 && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x40)))
		{
			GUI_output(get_dtp(0x44));
			xor_ds_bs(0x40f5, 1);
		}

	} else if (target_pos == 0x806 && target_pos != ds_readws(0x330e))
	{
		for (i = l3 = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_skill(hero, 50, 2) > 0)
			{
				l3 = 1;
			}
		}

		if (l3 != 0)
		{
			GUI_output(get_dtp(0x48));
		} else {
			GUI_output(get_dtp(0x4c));

			dec_ds_ws_post(X_TARGET);
			add_ds_ws(Y_TARGET, 2);
			DNG_inc_level();
		}

	} else if (target_pos == 0x1d06 && target_pos != ds_readws(0x330e) &&
			ds_readb(0x40f9) != 0 && !ds_readb(0x40fa))
	{
		GUI_output(get_dtp(0x2c));

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_attrib(hero, 4, 4) <= 0)
			{
				/* failed FF+4 test */
				sub_hero_le(hero, dice_roll(2, 6, 4));

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x30),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}

	} else if (target_pos == 0x1e09 && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x40)))
		{
			GUI_output(get_dtp(0x70));
			xor_ds_bs(0x40fa, 1);
		}

	} else if (target_pos == 0x190b && target_pos != ds_readws(0x330e) &&
			ds_readb(0x40fb) != 0 && ds_readb(0x4101) == 2)
	{
		GUI_output(get_dtp(0x2c));

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != 0 &&
				host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
				!hero_dead(hero) &&
				test_attrib(hero, 4, 4) <= 0)
			{
				/* failed FF+4 test */
				sub_hero_le(hero, dice_roll(2, 6, 4));

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x30),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));
			}
		}

	} else if ((target_pos == 0x1607 || target_pos == 0x1401 ||
			target_pos == 0x1403) &&
			target_pos != ds_readws(0x330e))
	{
		DNG09_statues(20, 50);

	} else if (target_pos == 0x130c && target_pos != ds_readws(0x330e))
	{
		DNG09_pitfall();

	} else if (target_pos == 0x1208 && target_pos != ds_readws(0x330e))
	{
		if (GUI_bool(get_dtp(0x98)))
		{
			dec_ds_ws_post(X_TARGET);
			inc_ds_ws_post(Y_TARGET);
			ds_writeb(DIRECTION, 2);
			DNG_dec_level();
		}

	} else if (target_pos == 0x170d && target_pos != ds_readws(0x330e) &&
			!ds_readb(0x43a1))
	{
		do {
			i = GUI_radio(get_dtp(0x9c), 3,
					get_dtp(0xa0),
					get_dtp(0xa4),
					get_dtp(0xa8));
		} while (i == -1);

		if (i == 2)
		{
			GUI_output(get_dtp(0xac));

		} else if (i == 3)
		{
			if ((i = select_hero_ok(get_dtp(0xb0))) != -1)
			{
				hero = get_hero(i);

				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0xb4),
					(char*)hero + HERO_NAME2);

				GUI_output(Real2Host(ds_readd(DTP2)));

				and_ptr_bs(amap_ptr + 0x54, 0x0f);
				or_ptr_bs(amap_ptr + 0x53, 0xf0);

				if (ds_readbs(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP)) > 1)
				{
					ds_writeb(0x2d7c, ds_readbs(DIRECTION));
					l3 = 0;
					while (ds_readb(GROUP_MEMBER_COUNTS + l3) != 0)
					{
						l3++;
					}

					host_writeb(hero + 0x87, (signed char)l3);
					inc_ds_bs_post(GROUP_MEMBER_COUNTS + l3);
					dec_ds_bs_post(GROUP_MEMBER_COUNTS + ds_readbs(CURRENT_GROUP));
					GRP_save_pos(l3);
					ds_writeb(0x43a1, 1);
				}
			} else
			{
				ds_writeb(0x2d7c, ds_readbs(DIRECTION));
			}
		} else
		{
			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}

	} else if ((target_pos == 0x180d || target_pos == 0x170e) &&
			target_pos != ds_readws(0x330e))
	{
		for (i = l3 = 0; i < 6; i++)
		{
			if (ds_readws(GROUPS_X_TARGET + 2 * i) == 7 &&
				ds_readws(GROUPS_Y_TARGET + 2 * i) == 13 &&
				ds_readbs(GROUPS_DNG_LEVEL + i) == 1 &&
				ds_readbs(CURRENT_GROUP) != i)
			{
				l3 = 1;
			}
		}

		if (l3 == 0)
		{
			or_ptr_bs(amap_ptr + 0x54, 0xf0);
			and_ptr_bs(amap_ptr + 0x53, 0x0f);
			ds_writeb(0x43a1, 0);
		}

	} else if (target_pos == 0x1502 && target_pos != ds_readws(0x330e) && !ds_readb(0x40ff))
	{
		load_ani(26);
		init_ani(0);
		delay_or_keypress(100);

		if (GUI_bool(get_dtp(0xb8)))
		{
			ds_writeb(0x40ff, 1);

			add_hero_ap_all(60);

			for (i = 0; i < 14; i++)
			{
				add_ds_ds(GODS_ESTIMATION + 4 * i, 100L);
			}

			/* mark quest as done */
			ds_writeb(QUEST_NAMELESS_DONE, 1);


			do {
				i = GUI_radio(get_dtp(0xbc), 2,
						get_dtp(0xc0),
						get_dtp(0xc4));
			} while (i == -1);

			if (i == 1)
			{
				get_item(248, 1, 1);

			} else {
				GUI_output(get_dtp(0xc8));
			}
		}

		set_var_to_zero();
		ds_writews(0x2ccb, -1);

	} else if (target_pos == 0x1402 &&
			(target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)) &&
			ds_readbs(DIRECTION) == 3 && ds_readbs(0x4100) != 2)
	{
		if (ds_readb(0x4100) != 0 || test_skill((hero = Real2Host(get_first_hero_available_in_group())), 51, 6) > 0)
		{
			ds_writeb(0x4100, 1);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0xcc),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)(((l3 = test_skill(hero, 48, 4)) > 0) ? get_dtp(0xd0) : get_dtp(0xd8)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

			strcat((char*)Real2Host(ds_readd(DTP2)),
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			if (l3 > 0)
			{
				and_ptr_bs(amap_ptr + 0x23, 0x0f);

				ds_writeb(0x4100, 2);

				DNG_update_pos();

			} else {
				sub_hero_le(hero, dice_roll(2, 6, 0));
			}

			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}
	} else if (target_pos == 0x150b  &&
			(target_pos != ds_readws(0x330e) || ds_readbs(DIRECTION) != ds_readbs(0x2d7c)) &&
			ds_readbs(DIRECTION) == 1 && ds_readbs(0x4101) != 2)
	{
		if (ds_readb(0x4101) != 0 || test_skill((hero = Real2Host(get_first_hero_available_in_group())), 51, 8) > 0)
		{
			ds_writeb(0x4101, 1);

			sprintf((char*)Real2Host(ds_readd(DTP2)),
				(char*)get_dtp(0xcc),
				(char*)hero + HERO_NAME2);

			sprintf((char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)),
				(char*)(((l3 = test_skill(hero, 48, 6)) > 0) ? get_dtp(0xd0) : get_dtp(0xd4)),
				(char*)Real2Host(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			strcat((char*)Real2Host(ds_readd(DTP2)),
				(char*)Real2Host(ds_readd(TEXT_OUTPUT_BUF)));

			GUI_output(Real2Host(ds_readd(DTP2)));

			if (l3 > 0)
			{
				and_ptr_bs(amap_ptr + 0xb6, 0x0f);

				ds_writeb(0x4101, 2);

				DNG_update_pos();
			}

			ds_writeb(0x2d7c, ds_readbs(DIRECTION));
		}

	} else if (target_pos == 0x70f && target_pos != ds_readws(0x330e))
	{
		leave_dungeon();
		ds_writebs(CURRENT_TOWN, (signed char)ds_readws(0x4338));
		ds_writews(X_TARGET, ds_readws(0x433a));
		ds_writews(Y_TARGET, ds_readws(0x433c));
		ds_writeb(LOCATION, 0);
		ds_writeb(DIRECTION, (ds_readws(0x433e) + 2) & 3);

		sprintf((char*)Real2Host(ds_readd(DTP2)),
			(char*)get_dtp(0xdc),
			(char*)get_ltx(4 * (ds_readws(0x434a) + 0xeb)));

		GUI_output(Real2Host(ds_readd(DTP2)));

		timewarp(HOURS(3));

		ds_writeb(0x4475, 3);
	}

	ds_writew(TEXTBOX_WIDTH, tw_bak);
	ds_writew(0x330e, target_pos);

	return 0;
}

/**
 * \brief print introductions of dungeon #9 fights
 *
 * \param fight_id	ID of the fight
 */
void DNG09_fight_intro(signed short fight_id)
{
	if (fight_id == 94)
	{
		GUI_output(get_dtp(0x34));
	}
}

void DNG09_chest0_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x4102));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG09_chest1_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x410e));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG09_chest2_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x4118));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG09_chest3_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x4120));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG09_chest4_x1(RealPt chest)
{
	signed short answer;
	Bit8u *hero;

	if (!ds_readb(0x40fc))
	{
		if (GUI_bool(get_dtp(0x50)))
		{
			do {
				answer = GUI_radio(get_dtp(0x54), 3,
							get_dtp(0x58),
							get_dtp(0x5c),
							get_dtp(0x60));
			} while (answer == -1);

			hero = Real2Host(get_first_hero_available_in_group());

			if (answer == 1)
			{
				sprintf((char*)Real2Host(ds_readd(DTP2)),
					(char*)get_dtp(0x64),
					(char*)hero + HERO_NAME2);
				GUI_output(Real2Host(ds_readd(DTP2)));

				sub_hero_le(hero, dice_roll(2, 20, 0));
			} else if (answer == 2)
			{
				hero = get_hero(0) + select_hero_ok_forced(get_ltx(0x4f4)) * SIZEOF_HERO;
				if (test_spell(hero, 2, 0) > 0)
				{
					sub_ae_splash(hero, 20);

					sub_ptr_ws(hero + HERO_AE_ORIG, 3);

					GUI_output(get_dtp(0x6c));

					add_hero_ap_all(50);

					ds_writeb(0x40fc, 1);
				} else {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x68),
						(char*)hero + HERO_NAME2);
					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_ae_splash(hero, 10);
				}

			} else if (answer == 3)
			{
				hero = get_hero(0) + select_hero_ok_forced(get_ltx(0x4f4)) * SIZEOF_HERO;
				if (test_spell(hero, 53, 0) > 0)
				{
					sub_ae_splash(hero, 35);

					sub_ptr_ws(hero + HERO_AE_ORIG, 3);

					GUI_output(get_dtp(0x6c));

					add_hero_ap_all(50);

					ds_writeb(0x40fc, 1);
				} else {
					sprintf((char*)Real2Host(ds_readd(DTP2)),
						(char*)get_dtp(0x68),
						(char*)hero + HERO_NAME2);
					GUI_output(Real2Host(ds_readd(DTP2)));

					sub_ae_splash(hero, 20);
				}
			}
		}
	} else {
		GUI_output(get_ltx(0x828));
	}
}

void DNG09_chest5_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x4135));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG09_chest6_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd(Real2Host(chest) + 11);
	host_writed(Real2Host(chest) + 11, (Bit32u)RealMake(datseg, 0x4139));
	loot_simple_chest(Real2Host(chest));
	host_writed(Real2Host(chest) + 11, (Bit32u)ptr_bak);
}

void DNG09_chest4_x2(RealPt chest)
{
	ds_writew(0xd325, ds_writew(0xd327, ds_writew(0xd329, ds_writew(0xd32b, 0x1a2b))));

	do_fight(102);
}

#if !defined(__BORLANDC__)
}
#endif
