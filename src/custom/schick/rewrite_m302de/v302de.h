/*
 *	transitional functions for version 3.02_de
 *
 *	Here are function, which should make the transition from
 *	DOXBox emulation to native easier.
 *	These functions will only work with the version mentioned above.
 */

#if !defined V302DE_H
#define V302DE_H

#include <stdlib.h>

#if !defined(__BORLANDC__)
#include "schick.h"
#endif


#define UNICORN_MET	(0x2464)
#define UNICORN_GET_MAP	(0x2465)
#define UNICORN_TIMER	(0x2466)
#define TWO_FIELDED_SPRITE_ID (0x25f9)	/* char[5] array */
#define EMS_ENABLED	(0x26ab)
#define TIMERS_DISABLED	(0x2c99)
#define CURRENT_ANI	(0x2ccf)
#define IN_FIGHT	(0x2cd5)
#define CURRENT_GROUP	(0x2d35)
#define DIRECTION	(0x2d3d)
#define X_TARGET	(0x2d44)
#define Y_TARGET	(0x2d46)
#define LOCATION	(0x2d60)
#define CURRENT_TOWN	(0x2d67)
#define DUNGEON_INDEX	(0x2d6e)
#define DUNGEON_LEVEL	(0x2d75)
#define DAY_TIMER	(0x2dbb)
#define DAY_OF_WEEK	(0x2dbf)
#define DAY_OF_MONTH	(0x2dc0)
#define MONTH		(0x2dc1)
#define YEAR		(0x2dc2)
#define SPECIAL_DAY	(0x2dc3)
#define TREASURE_MAPS	(0x3332)	/* array char[9] */
#define GOT_MAIN_QUEST	(0x334c)
#define QUESTED_MONTHS	(0x334e)
#define NPC_MONTHS	(0x3470)
#define CURRENT_INFORMER	(0x3612)
#define TRAVELING	(0x3614)
#define SPHERE_TIMER	(0x3c97)
#define SPHERE_ACTIVE	(0x3c98)
#define MAGE_POISON	(0x3cb6)
#define FOOD_MESSAGE	(0x4219)
#define CITYINDEX	(0x4222)
#define TYPEINDEX	(0x4224)
#define CHECK_PARTY	(0x4495)
#define FOOD_MOD	(0x4496)
#define CONSUME_QUIET	(0x4648)
#define DELAY_FACTOR	(0x4b66)
#define FNAMES		(0x4c8c)
#define AUTOFIGHT_MAGIC (0x5f31)	/* ?8 */
#define AF_SPELL_LIST	(0x5fac)	/* char[11] */
#define LIGHT_TYPE	(0xaee8)	/* ?16 0 = none, 1 = torch, 2 = lantern */
#define CD_DRIVE_NR	(0xbc52)	/* ?16 */
#define SPLASH_AE	(0xbcc7)
#define SPLASH_LE	(0xbccb)
#define SCENARIO_BUF	(0xbd2c)
#define PTR_FIGHT_LST	(0xbd28)	/* pointer to FIGHT.LST */
#define HEROS		(0xbd34)
#define DIALOG_TEXT	(0xc3b1)
#define CITY_LTX	(0xc3ad)
#define TEXT_LTX	(0xc3b5)
#define ACTION		(0xc3d9)	/* ? short */
#define ANI_MAIN_PTR	(0xce35)
#define TEXTCOLOR_BG	(0xd2c7)
#define TEXTCOLOR_FG	(0xd2c9)
#define DTP2		(0xd2f3)
#define ENEMY_SHEETS	(0xd34b)	/* struct enemy[20] */
#define CHESSBOARD	(0xd852)
#define NR_OF_ENEMIES	(0xd872)	/* ? short */
#define WALLCLOCK_Y	(0xe10f)
#define WALLCLOCK_X	(0xe111)
#define MEM_SLOTS_MON	(0xe115)
#define MEM_SLOTS_WFIG	(0xe119)
#define MEM_SLOTS_MFIG	(0xe11d)
#define MONNAMES	(0xe129)
#define ITEMSDAT	(0xe22b)
#define ITEMSNAME	(0xe22f)
#define DELAY_TIMER	(0xe2d0)
#define CURRENT_FIG_NR	(0xe316)
#define AUTOFIGHT	(0xe318)	/* ?16 */
#define BUFFER_WEAPANIDAT	(0xe374)	/* pointer to WEAPANI.DAT */
#define BUFFER_ANIDAT	(0xe378)	/* pointer to ANI.DAT buffer */
#define GRAMMAR_STRING	(0xe4e3)	/* char grammar_string[201] */
#define SPELLTARGET	(0xe5b8)
#define SPELLUSER	(0xe5bc)

#if !defined(__BORLANDC__)

#include "common.h"

static inline Bit8s host_readbs(Bit8u* p)
{
	return (Bit8s)host_readb(p);
}

static inline Bit16s host_readws(Bit8u* p)
{
	return (Bit16s)host_readw(p);
}

static inline Bit32s host_readds(Bit8u* p)
{
	return (Bit32s)host_readd(p);
}

static inline Bit8s host_writebs(Bit8u* p, Bit8s val)
{
	host_writeb(p, val);
	return val;
}

static inline Bit16s host_writews(Bit8u* p, Bit16s val)
{
	host_writew(p, val);
	return val;
}

static inline Bit32s host_writeds(Bit8u* p, Bit32s val)
{
	host_writed(p, val);
	return val;
}

static inline int NOT_NULL(Bit8u* p)
{
	return (p != NULL && p != MemBase);
}

/**
 *	ds_writeb_z() -	write only if target is 0
 *	@addr:	address in datasegment
 *	@val:	value which should be written
 *
 *	A often occuring Original-Bug resets some informants
 *	to older states. This helper writes only that value
 *	if the informer is unknown (0).
 */
static inline void ds_writeb_z(Bitu addr, char val) {
	if (ds_readb(addr) == 0)
		ds_writeb(addr, val);
}

static inline Bit8u *get_hero(unsigned short index) {
	return Real2Host(ds_readd(HEROS)) + index * 0x6da;
}

static inline void inc_ds_bs(Bit16u off)
{
	ds_writeb(off, ds_readb(off) + 1);
}

static inline void dec_ds_bs(Bit16u off)
{
	ds_writeb(off, ds_readb(off) - 1);
}

static inline void inc_ptr_bs(Bit8u *p)
{
	host_writebs(p, host_readbs(p) + 1);
}

static inline void inc_ptr_ws(Bit8u *p)
{
	host_writews(p, host_readws(p) + 1);
}

static inline void dec_ptr_bs(Bit8u *p)
{
	host_writebs(p, host_readbs(p) - 1);
}

static inline void or_ptr_bs(Bit8u *p, const signed char val)
{
	host_writebs(p, host_readbs(p) | val);
}

static inline void and_ptr_bs(Bit8u *p, const signed char val)
{
	host_writebs(p, host_readbs(p) & val);
}

static inline void add_ptr_bs(Bit8u *p, const signed char val)
{
	host_writebs(p, host_readbs(p) + val);
}

static inline void sub_ptr_bs(Bit8u *p, const signed char val)
{
	host_writebs(p, host_readbs(p) - val);
}

static inline void add_ptr_ws(Bit8u *p, Bit16s val)
{
	host_writews(p, host_readws(p) + val);
}

static inline void sub_ptr_ws(Bit8u *p, Bit16s val)
{
	host_writews(p, host_readws(p) - val);
}

static inline int __abs__(int j)
{
	return abs(j);
}

/**
 * hero_dead() -	check if hero is dead
 * @hero:	ptr to hero
 *
 * 0 = alive / 1 = dead
 */
static inline unsigned short hero_dead(Bit8u *hero) {
	if ((host_readb(hero + 0xaa) & 1))
		return 1;
	else
		return 0;
}

/**
 * hero_sleeps() -	check if hero is sleeping
 * @hero:	ptr to hero
 *
 * 0 = awake / 1 = sleeps
 */
static inline unsigned short hero_sleeps(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 1) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * hero_stoned() -	check if hero is stoned
 * @hero:	ptr to hero
 *
 * 0 = non-stoned / 1 = stoned
 */
static inline unsigned short hero_stoned(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 2) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * hero_cursed() -	check if hero is cursed
 * @hero:	ptr to hero
 *
 * 0 = not cursed / 1 = cursed
 */
static inline unsigned short hero_cursed(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 5) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * hero_unc() -	check if hero is unconscious
 * @hero:	ptr to hero
 *
 * 0 = awake / 1 = unconscious
 */
static inline unsigned short hero_unc(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 6) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * ks_empty() -	check if a item in the knapsack is empty
 * @item:	ptr to item
 *
 * 0 = filled / 1 = empty
 */
static inline unsigned short ks_empty(Bit8u *ks) {
	if (((host_readb(ks + 0x04) >> 2) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * ks_half_empty() -	check if a item in the knapsack is half empty
 * @item:	ptr to item
 *
 * 0 = filled / 1 = half empty
 */
static inline unsigned short ks_half_empty(Bit8u *ks) {
	if (((host_readb(ks + 0x04) >> 1) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_armor() -	check if a item is an armor
 * @item:	ptr to item
 *
 * 0 = non armor / 1 = armor
 */
static inline unsigned short item_armor(Bit8u *item) {
	if (((host_readb(item + 0x02) >> 0) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_weapon() -	check if a item is a weapon
 * @item:	ptr to item
 *
 * 0 = non weapon / 1 = weapon
 */
static inline unsigned short item_weapon(Bit8u *item) {
	if (((host_readb(item + 0x02) >> 1) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_bit2() -	check if a item is bit2
 * @item:	ptr to item
 *
 * 0 = non set / 1 = set
 */
static inline unsigned short item_bit2(Bit8u *item) {
	if (((host_readb(item + 0x02) >> 2) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_food() -	check if a item is food
 * @item:	ptr to item
 *
 * 0 = non food / 1 = food
 */
static inline unsigned short item_food(Bit8u *item) {
	if (((host_readb(item + 0x02) >> 3) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_stackable() -	check if a item is stackable
 * @item:	ptr to item
 *
 * 0 = non stackable / 1 = stackable
 */
static inline unsigned short item_stackable(Bit8u *item) {
	if (((host_readb(item + 0x02) >> 4) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_herb_potion() -	check if a item is a herb or potion
 * @item:	ptr to item
 *
 * 0 = non / 1 = herb or potion
 */
static inline unsigned short item_herb_potion(Bit8u *item) {
	if (((host_readb(item + 0x02) >> 5) & 1) == 0)
		return 0;
	else
		return 1;
}

/**
 * item_herb_undropable() -	check if a item is undropable
 * @item:	ptr to item
 *
 * 0 = dropable / 1 = undropable
 */
static inline unsigned short item_undropable(Bit8u *item) {
	if (((host_readb(item + 0x02) >> 6) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline Bit8u *get_spelluser() {
	return Real2Host(ds_readd(SPELLUSER));
}

static inline Bit8u *get_spelltarget() {
	return Real2Host(ds_readd(SPELLTARGET));
}

static inline Bit8u *get_fname(unsigned short off) {
	return Real2Host(ds_readd(FNAMES + off * 4));
}

static inline Bit8u *get_monname(unsigned short off)
{
	return Real2Host(host_readd(Real2Host(ds_readd(MONNAMES) + off * 4)));
}

static inline Bit8u *get_city(unsigned short off) {
	return Real2Host(host_readd(Real2Host(ds_readd(CITY_LTX) + off)));
}

static inline Bit8u *get_ltx(unsigned short off) {
	return Real2Host(host_readd(Real2Host(ds_readd(TEXT_LTX) + off)));
}

static inline Bit8u *get_dtp(unsigned short off) {
	return Real2Host(host_readd(Real2Host(ds_readd(DIALOG_TEXT) + off)));
}

static inline signed char get_cb_val(unsigned short x, unsigned short y) {
	return host_readb(Real2Host(ds_readd(CHESSBOARD)) + y * 25 + x);
}

static inline void set_cb_val(unsigned short x, unsigned short y, signed char val) {
	host_writeb(Real2Host(ds_readd(CHESSBOARD)) + y * 25 + x, val);
}

static inline void dump_cb(void)
{
	FILE *fd;
	int y, x;
	signed char val;

	fd = fopen("cb_dump.txt", "w");

	if (fd == NULL)
		return;
	fprintf(fd, "\n");
	for (y = 24; y >= 0; y--) {
		for (x = 0; x < 25; x++) {
			val = get_cb_val(x, y);
			if (val != 0)
				fprintf(fd, "%3d ", val);
			else
				fprintf(fd, "    ");
		}
		fprintf(fd, "\n");
	}
	fclose(fd);
}

static inline Bit8u *get_itemsdat(unsigned short item) {
	return Real2Host(ds_readd(ITEMSDAT)) + item * 12;
}

static inline char* get_itemname(unsigned short item)
{
	return (char*)Real2Host(host_readd(Real2Host(ds_readd(ITEMSNAME)) + item * 4));
}

#else

#include <IO.H>
#include <DOS.H>
#include <BIOS.H>

#undef M302de_ORIGINAL_BUGFIX

typedef unsigned char Bit8u;
typedef signed char Bit8s;
typedef unsigned short Bit16u;
typedef signed short Bit16s;
typedef unsigned long Bit32u;
typedef signed long Bit32s;

typedef Bit8u* RealPt;
typedef Bit8u* PhysPt;

#include "common.h"

#define NOT_NULL(p) (p != NULL)

/* poring functions for Borland C++ */
struct hero_struct {
	char name[16];
	char alias[16];
	char dummy;
	char typus;
};

extern char ds[0xffff];
#define p_datseg (&ds[0x0000])
#define datseg (FP_SEG(&ds[0x0000]))

#define ds_readb(p) (*(Bit8u*)(ds + p))
#define ds_readw(p) (*(Bit16u*)(ds + p))
#define ds_readd(p) (*(Bit32u*)(ds + p))

#define ds_readbs(p) *(Bit8s*)(ds + p)
#define ds_readws(p) *(Bit16s*)(ds + p)
#define ds_readds(p) *(Bit32s*)(ds + p)

#define ds_writeb(p, d) *(Bit8u*)(ds + p) = d
#define ds_writew(p, d) *(Bit16u*)(ds + p) = d
#define ds_writed(p, d) *(Bit32u*)(ds + p) = d

#define inc_ds_bs(o) (*(Bit8s*)(ds + o))++
#define dec_ds_bs(o) (*(Bit8s*)(ds + o))--

#define inc_ptr_bs(p)	(*(Bit8s*)(p))++
#define inc_ptr_ws(p)	(*(Bit16s*)(p))++
#define dec_ptr_bs(p)	(*(Bit8s*)(p))--
#define dec_ptr_ws(p)	(*(Bit16s*)(p))--

#define or_ptr_bs(p, val) *(Bit8s*)(p) |= val;
#define and_ptr_bs(p, val) *(Bit8s*)(p) &= val;
#define add_ptr_bs(p, v)  *(Bit8s*)(p)+=v
#define sub_ptr_bs(p, v)  *(Bit8s*)(p)-=v


#define add_ptr_ws(p, v)  *(Bit16s*)(p)+=v
#define sub_ptr_ws(p, v)  *(Bit16s*)(p)-=v

#define mem_readb(p) *(Bit8u*)(p)
#define mem_readw(p) *(Bit16u*)(p)
#define mem_readd(p) *(Bit32u*)(p)

#define mem_readbs(p) *(Bit8s*)(p)
#define mem_readws(p) *(Bit16s*)(p)
#define mem_readds(p) *(Bit32s*)(p)

#define mem_writeb(p, d) *(Bit8u*)(p) = d
#define mem_writew(p, d) *(Bit16u*)(p) = d
#define mem_writed(p, d) *(Bit32u*)(p) = d

#define get_hero(nr) ((Bit8u*)ds_readd(HEROS) + 0x6da * (nr))

#ifdef M302de_ORIGINAL_BUGFIX
#define ds_writeb_z(addr, val) (if (ds_readb(addr) == 0) ds_writeb(addr, val))
#else
#define ds_writeb_z(addr, val) (ds_writeb(addr, val))
#endif

extern Bit8u* text_ltx[];
extern Bit8u* dialog_text[];
extern Bit8u* city_ltx[];

#define RealMake(s, o) MK_FP(s, o)
#define PhysMake(s, o) MK_FP(s, o)

#define Real2Phys(p) (Bit8u*)(p)
#define Real2Host(p) (Bit8u*)(p)
#define getString(p) (char*)(p)

#define host_readb(p) *(Bit8u*)(p)
#define host_readw(p) *(Bit16u*)(p)
#define host_readd(p) *(Bit32u*)(p)

#define host_readbs(p) *(Bit8s*)(p)
#define host_readws(p) *(Bit16s*)(p)
#define host_readds(p) *(Bit32s*)(p)

#define host_writeb(p, d)	(*(Bit8u*)(p) = d)
#define host_writew(p, d)	(*(Bit16u*)(p) = d)
#define host_writed(p, d)	(*(Bit32u*)(p) = d)

#define host_writebs(p, d)	(*(Bit8s*)(p) = d)
#define host_writews(p, d)	(*(Bit16s*)(p) = d)
#define host_writeds(p, d)	(*(Bit32s*)(p) = d)

#define mem_memcpy memcpy

#define hero_dead(hero)  ((*(struct hero_status*)(hero + 0xaa)).dead)
#define hero_sleeps(hero)  ((*(struct hero_status*)(hero + 0xaa)).sleeps)
#define hero_stoned(hero)  ((*(struct hero_status*)(hero + 0xaa)).stoned)
#define hero_cham(hero)  ((*(struct hero_status*)(hero + 0xaa)).cham)
#define hero_cursed(hero)  ((*(struct hero_status*)(hero + 0xaa)).cursed)
#define hero_unc(hero)  ((*(struct hero_status*)(hero + 0xaa)).uncon)
#define hero_dup(hero)  ((*(struct hero_status*)(hero + 0xaa)).dup)

#define ks_half_empty(ks)  ((*(struct knapsack_status*)(ks + 0x4)).half_empty)
#define ks_empty(ks)  ((*(struct knapsack_status*)(ks + 0x4)).empty)

#define item_armor(item)  ((*(struct item_status*)(item + 0x2)).armor)
#define item_weapon(item)  ((*(struct item_status*)(item + 0x2)).weapon)
#define item_bit2(item)  ((*(struct item_status*)(item + 0x2)).bit2)
#define item_food(item)  ((*(struct item_status*)(item + 0x2)).food)
#define item_stackable(item)  ((*(struct item_status*)(item + 0x2)).stackable)
#define item_herb_potion(item)  ((*(struct item_status*)(item + 0x2)).herb_potion)
#define item_undropable(item)  ((*(struct item_status*)(item + 0x2)).undropable)

#define get_spelluser() (Bit8u*)ds_readd(SPELLUSER)
#define get_spelltarget() (Bit8u*)ds_readd(SPELLTARGET)

#define get_ltx(nr) (char*)(host_readd((RealPt)ds_readd(TEXT_LTX) + nr))
#define get_dtp(nr) (char*)(host_readd((RealPt)ds_readd(DIALOG_TEXT) + nr))
#define get_city(nr) (char*)(host_readd((RealPt)ds_readd(CITY_LTX) + nr))
#define get_monname(nr) (char*)(host_readd((RealPt)ds_readd(MONNAMES) + nr * 4))
#define get_itemsdat(nr) (char*)((RealPt)ds_readd(ITEMSDAT) + nr * 12)
#define get_itemname(nr) (char*)((RealPt)(host_readd((RealPt)(ds_readd(ITEMSNAME)) + nr * 4)))

#define struct_copy memcpy
#define bc__creat _creat
#define bc_close close
#define bc_memmove memmove


#define BORLAND_FAR __far

#define get_cb_val(x, y) (host_readbs((RealPt)ds_readd(CHESSBOARD) + y * 25 + x))
#define set_cb_val(x, y, val) (host_writeb(((RealPt)ds_readd(CHESSBOARD)) + y * 25 + x, val))

#endif
#endif
