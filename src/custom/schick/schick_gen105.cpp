#include <stdlib.h>

#include "regs.h"

#include "schick.h"

#include "seg008.h"
#include "seg009.h"

static int seg004(unsigned short offs) {

	switch (offs) {
	case 0xc: {
		RealPt dst = CPU_Pop32();
		RealPt src = CPU_Pop32();
		unsigned int len = CPU_Pop32();
		CPU_Push32(len);
		CPU_Push32(src);
		CPU_Push32(dst);

		D1_INFO("decomp_pp20(dst=0x%04x:0x%04x, src=0x%04x:0x%04x, %u)\n",
			RealSeg(dst), RealOff(dst),
			RealSeg(src), RealOff(src), len);

		decomp_pp20(MemBase + Real2Phys(src),
			MemBase + Real2Phys(dst),
			MemBase + Real2Phys(src) + 8, len);

		return 1;
	}
	default:
		D1_ERR("Uncatched call to Segment %s:0x%04x\n", __func__, offs);
		exit(1);
	}
}

/* Rasterlib */
static int seg005(unsigned short offs) {

	switch (offs) {
	case 0x008: {
		unsigned short mode = CPU_Pop16();
		CPU_Push16(mode);

		D1_GFX("set_video_mode(mode=0x%x);\n", mode);
		set_video_mode(mode);

		return 1;
	}
	case 0x1e: {
		unsigned short page = CPU_Pop16();
		CPU_Push16(page);

		D1_GFX("set_video_page(page=0x%x);\n", page);
		set_video_page(page);

		return 1;
        }
	case 0x34: {
		RealPt addr = CPU_Pop32();
		CPU_Push32(addr);

		D1_GFX("SaveDisplayStat(dstat=0x%x:0x%x);\n",
			RealSeg(addr), RealOff(addr));

		return 0;
        }
	case 0xde: {
		RealPt ptr = CPU_Pop32();
		unsigned short color = CPU_Pop16();
		CPU_Push16(color);
		CPU_Push32(ptr);

		D1_GFX("set_color(rgb=0x%x:0x%x, color=0x%x);\n",
			RealSeg(ptr), RealOff(ptr), color);

		set_color(MemBase + Real2Phys(ptr), color);

		D1_GFX("RGB=(0x%x, 0x%x, 0x%x);\n",
		real_readb(RealSeg(ptr), RealOff(ptr)),
		real_readb(RealSeg(ptr), RealOff(ptr) + 1),
		real_readb(RealSeg(ptr), RealOff(ptr) + 2));

		return 1;
	}
	case 0x100: {
		RealPt ptr = CPU_Pop32();
		unsigned short first_color = CPU_Pop16();
		unsigned short colors = CPU_Pop16();
		CPU_Push16(colors);
		CPU_Push16(first_color);
		CPU_Push32(ptr);

		unsigned short i;

		D1_GFX("set_palette(rgb=0x%x:0x%x, first_color=0x%x, colors=0x%x);\n",
			RealSeg(ptr), RealOff(ptr), first_color, colors);

		set_palette(MemBase + Real2Phys(ptr), first_color, colors);

		if (RealSeg(ptr) == datseg)
			D1_LOG("Palette at DS:0x%x\n", RealSeg(ptr));
		for (i=0; i<colors; i++)
			D1_GFX("\"\\%02d\\%02d\\%02d\"..\n",
				host_readb(MemBase + Real2Phys(ptr)+i*3),
				host_readb(MemBase + Real2Phys(ptr)+i*3+1),
				host_readb(MemBase + Real2Phys(ptr)+i*3+2));
		return 1;
	}
	case 0x127: {
		unsigned short ptr = CPU_Pop16();
		unsigned short cnt = CPU_Pop16();
		unsigned short color = CPU_Pop16();
		CPU_Push16(color);
		CPU_Push16(cnt);
		CPU_Push16(ptr);

		draw_h_line(PhysMake(0xa000, ptr), cnt, color);

			D1_GFX("HLine(X=%03d,Y=%03d,len=%u,color=0x%02x);\n",
				ptr % 320, ptr / 320, cnt, color);
		return 1;
        }
	case 0x184: {
		unsigned short ptr = CPU_Pop16();
		unsigned short cnt = CPU_Pop16();
		unsigned short color = CPU_Pop16();
		unsigned short space = CPU_Pop16();
		CPU_Push16(space);
		CPU_Push16(color);
		CPU_Push16(cnt);
		CPU_Push16(ptr);

		draw_h_spaced_dots(PhysMake(0xa000, ptr), cnt, color, space);

		D1_GFX("HSpacedDots(X=%03d,Y=%03u,%03u,0x%02x,%u);\n",
			ptr % 320, ptr / 320, cnt, color, space);

		return 1;
	}
	case 0x386: {
		unsigned short val = CPU_Pop16();
		CPU_Push16(val);

		reg_ax = swap_u16(val);
		D1_GFX("swap_u16(val=0x%04x); = 0x%04x\n", val, reg_ax);

		return 1;
	}

	}

	if (offs == 0x1f3)
	{
		D1_GFX("DrawPic(Dest=0x%x:0x%x, X=%d, Y=%d, ..., Breite=%d, Höhe=%d, Src=0x%x:0x%x, Mode=%d);\n",
			real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
			real_readw(SegValue(ss), reg_sp+4), real_readw(SegValue(ss), reg_sp+6),
			real_readw(SegValue(ss), reg_sp+20), real_readw(SegValue(ss), reg_sp+22),
			real_readw(SegValue(ss), reg_sp+26), real_readw(SegValue(ss), reg_sp+24),
			real_readw(SegValue(ss), reg_sp+28));
		return 0;
	}
	if (offs == 0x2e3) {
		unsigned short off=real_readw(SegValue(ss), reg_sp+2);
		unsigned short seg=real_readw(SegValue(ss), reg_sp);

		D1_GFX("FillRect(segment=0x%x, offset=0x%x, color=0x%x, width=%d, height=%d);",
			seg, off, real_readw(SegValue(ss), reg_sp+4),
			real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+8));

		if (seg == 0xa000)
			D1_GFX("\t/*X=%d Y=%d*/\n",
				off%320, off/320);
		else
			D1_GFX("\n");

		return 0;
	}

	if (offs == 0x39f) {
		D1_GFX("_39F(src=0x%x:0x%x, dst=0x%x:0x%x, a=%d, b=%d, c=%d);\n",
			real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
			real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4),
			real_readw(SegValue(ss), reg_sp+8), real_readw(SegValue(ss), reg_sp+10),
			real_readw(SegValue(ss), reg_sp+12));
		return 0;
	}
	D1_GFX("Rasterlib:0x%x\n", offs);
	return 0;
}

// Hooks for tracing far calls for GEN.EXE(de/V1.05)
int schick_farcall_gen105(unsigned segm, unsigned offs)
{
	/* _decomp() */
	if (segm == 0xb39)
		return seg004(offs);
	if (segm == 0xb6b)
		return seg005(offs);


	if (segm == 0x0) {
		if (offs == 0x0438) {
			D1_TRAC("_dos_getvect(intnr=0x%x)\n", real_readw(SegValue(ss),reg_sp));
			return 0;
		}
		if (offs == 0x0447) {
			D1_TRAC("_dos_setvect(intnr=0x%x, *isr=0x%x:0x%x)\n",
				real_readw(SegValue(ss), reg_sp), real_readw(SegValue(ss), reg_sp+4),
				real_readw(SegValue(ss), reg_sp+2));
			return 0;
		}
		if (offs == 0x07c5) {
			D1_LOG(
			"__read(Handle=0x%x, Buffer=0x%x:0x%x, Length=%d)\n",
			real_readw(SegValue(ss), reg_sp), real_readw(SegValue(ss), reg_sp+4),
			real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp+6));
			return 0;
		}
		if (offs == 0x20bc) {
			D1_LOG("close(Handle=0x%x)\n", real_readw(SegValue(ss), reg_sp));
			return 0;
		}
		if (offs == 0x254e) {
			D1_LOG(
			"memcpy(__dest=0x%x:0x%x, __src=0x%x:0x%x, __n=0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4),
				real_readw(SegValue(ss), reg_sp+8));
			return 0;
		}
		if (offs == 0x2596) {
			D1_LOG(
			"memset(__dest=0x%x:0x%x, __c=0x%x, __n=0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+4), real_readw(SegValue(ss), reg_sp+6));
			return 0;
		}
		if (offs == 0x2607) {
			D1_LOG(
			"memmove(__dest=0x%x:0x%x, __src=0x%x:0x%x, __n=0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4),
				real_readw(SegValue(ss), reg_sp+8));
			return 0;
		}
		if (offs == 0x2655) {
			D1_LOG("open()\n");
			return 0;
		}
		if (offs == 0x2dd5) {
			D1_LOG(
			"strcpy(__s1=0x%x:0x%x, __s2=0x%x:0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4));
			return 0;
		}
		if (offs == 0x2e1d) {
			D1_LOG(
			"strncmp(__s1=0x%x:0x%x, __s2=0x%x:0x%x, __maxlen=0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4),
				real_readw(SegValue(ss), reg_sp+8));
			return 0;
		}
		if (offs == 0x2e55) {
			D1_LOG(
			"strncpy(__s1=0x%x:0x%x, __s2=0x%x:0x%x, __maxlen=0x%x)\n",
				real_readw(SegValue(ss), reg_sp+2), real_readw(SegValue(ss), reg_sp),
				real_readw(SegValue(ss), reg_sp+6), real_readw(SegValue(ss), reg_sp+4),
				real_readw(SegValue(ss), reg_sp+8));
			return 0;
		}
	}
	return 0;
}

int schick_nearcall_gen105(unsigned offs) {
	return 0;
}