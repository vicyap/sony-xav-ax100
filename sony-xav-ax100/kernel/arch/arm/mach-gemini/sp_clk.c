/*
 * Info:
 * http://psweb.sunplus.com/mantis_PD2/view.php?id=7730
 * http://wiki2.sunplus.com/dvdsa/images/3/38/Q610_Clock_Arch_V0.92.pdf
 * http://wiki2.sunplus.com/dvdsa/images/2/25/letb8388_regfile_x081_system_part.pdf
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <mach/hal/regmap/reg_system.h>
#include <mach/sp_clk.h>
#include <mach/module.h>
/* #define FOR_FPGA */

#if defined(FOR_FPGA)
u32 sp_clk_get(u32 which)
{
	u32 freq;

	switch (which) {
	case SP_CLK_SYSSLOW:
		freq = 54000000;
		break;
	case SP_CLK_CA9:
		freq = 30000000;
		break;
	default:
		freq = 0;
		break;
	}

	return freq;
}
EXPORT_SYMBOL(sp_clk_get);
#else   /* !defined(FOR_FPGA) */

static u32 sp_pllsys(void)
{
	moonReg_t *pmoon0Reg  = (moonReg_t *)LOGI_ADDR_MOON_REG;
	u32 freq;

	freq = ((pmoon0Reg->pllsys_cfg_sel & 0x3f) + 1) * 13500000;
	if (pmoon0Reg->pllsys_cfg_sel & (1 << 11))	/* MO_PLLSYS_DIV2 */
		freq /= 2;

	return freq;
}

static u32 sp_pllsys_plus_div(void)
{
	moonReg_t *pmoon0Reg = (moonReg_t *)LOGI_ADDR_MOON_REG;

	/* Grp 0.5[3:0], MO_SYSFAST_DIV_SEL_ENC */
	return (sp_pllsys() >> (pmoon0Reg->clk_sel[1] & 0xf));
}

/*
 * PLLC1:
 *          (27 MHz) * TableA[{FBKDIVA, DIV15}] * FBKDIVB
 *  Fout =  ---------------------------------------------------
 *          (1 << {RSV[1], PREDIV}) * (1 << (1 + {BP, PSTDIV}))
 *
 *          (27 MHz) * TableA({FBKDIVA, DIV15}) * FBKDIVB
 *       =  ---------------------------------------------------
 *          1 << ({RSV[1], PREDIV} + 1 + {BP, PSTDIV})
 *
 *       Where TableA[] = {N/A, 3, 5, 15}
 *
 *          (27 MHz) * TableA[{G(1, 28)[7], G(1, 28)[25]}] * (G(1, 28)[12:8] + 1)
 *       =  -------------------------------------------------------------------
 *          1 << ({G(1, 28)[19], G(1, 28)[3]} + 1 + {G(1, 28)[1], G(1, 28)[6]})
 */
static u32 sp_pllc1(void)
{
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
	u32 value, fbkdiva_div15, bfkdivb, rsv1_prediv, bp_pstdiv, freq_out;

	value = pmoon1Reg->sft_cfg_28;

	fbkdiva_div15 = ((value & (1 <<  7)) >>  6) | ((value & (1 << 25)) >> 25);
	bfkdivb       = ((value >> 8) & 0x1f) + 1;
	rsv1_prediv   = ((value & (1 << 19)) >> 18) | ((value & (1 <<  3)) >>  3);
	bp_pstdiv     = ((value & (1 <<  1)) >>  0) | ((value & (1 <<  6)) >>  6);

	freq_out = 27000000 >> (rsv1_prediv + 1 + bp_pstdiv);
	if (fbkdiva_div15 & 0x02)
		freq_out *= 5;
	if (fbkdiva_div15 & 0x01)
		freq_out *= 3;
	freq_out *= bfkdivb;

	return freq_out;
}

/*
 * PLLC2:
 *          (27 MHz) * TableA[{FBKDIVA, DIV15}] * FBKDIVB
 *  Fout =  ---------------------------------------------------
 *          (1 << {RSV[1], PREDIV}) * (1 << (1 + {BP, PSTDIV}))
 *
 *          (27 MHz) * TableA({FBKDIVA, DIV15}) * FBKDIVB
 *       =  ---------------------------------------------------
 *          1 << ({RSV[1], PREDIV} + 1 + {BP, PSTDIV})
 *
 *       Where TableA[] = {N/A, 3, 5, 15}
 *
 *          (27 MHz) * TableA[{G(1, 25)[7], G(1, 25)[25]}] * (G(1, 25)[12:8] + 1)
 *       =  -------------------------------------------------------------------
 *          1 << ({G(1, 25)[19], G(1, 25)[3]} + 1 + {G(1, 25)[1], G(1, 25)[6]})
 */
static u32 sp_pllc2(void)
{
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
	u32 value, fbkdiva_div15, bfkdivb, rsv1_prediv, bp_pstdiv, freq_out;

	value = pmoon1Reg->sft_cfg_25;

	fbkdiva_div15 = ((value & (1 <<  7)) >>  6) | ((value & (1 << 25)) >> 25);
	bfkdivb       = ((value >> 8) & 0x1f) + 1;
	rsv1_prediv   = ((value & (1 << 19)) >> 18) | ((value & (1 <<  3)) >>  3);
	bp_pstdiv     = ((value & (1 <<  1)) >>  0) | ((value & (1 <<  6)) >>  6);

	freq_out = 27000000 >> (rsv1_prediv + 1 + bp_pstdiv);
	if (fbkdiva_div15 & 0x02)
		freq_out *= 5;
	if (fbkdiva_div15 & 0x01)
		freq_out *= 3;
	freq_out *= bfkdivb;

	return freq_out;
}

static u32 sp_pllc1_plus_dfs(void)
{
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
	u32 value, divider, num, deno;

	value = pmoon1Reg->sft_cfg_12;
	divider = value & 0xff;
	num     = (value >>  8) & 0xf;
	deno    = (value >> 12) & 0xf;

	return (sp_pllc1() / divider) * (num + 1) / (deno + 1);
}

/*
 * PLLDIS:
 * Integer Mode (G(1, 26)[5] == 1'b0)
 *
 *          (27 MHz) * NS
 *  Fout = --------------
 *             S * R
 *
 *       where NS = (G(1, 27)[11:4] + 2), (3 <= NS < 66) required.
 *             S  =  G(1, 26)[15:11],       if (G(1, 26)[15:11] > 3)
 *                   N/A??,                 if (G(1, 26)[15:11] == 5'h2 or 5'h3)
 *                   G(1, 26)[15:11] + 1    if (G(1, 26)[15:11] == 5'h0 or 5'h1)
 *             R  = (1 << (G(1, 27)[1:0]))
 *
 * Fractional Mode (G(1, 26)[5] == 1'b1)
 *
 *          (27 MHz) * N * f
 *  Fout = ------------------
 *             S * R
 *
 *       where N  = (G(1, 27)[31:27] + 1)
 *             f  = G(1, 27)[26:20] / 91
 *             S, R: same as integer mode
 */
static u32 sp_plldis(void)
{
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
	u32 value26, value27, ns, s, r, n;
	u32 freq_out;

	value26 = pmoon1Reg->sft_cfg_26;
	value27 = pmoon1Reg->sft_cfg_27;

	s = (value26 >> 11) & 0x1F;
	if (s < 2)
		s++;
	else if (s <= 3)
		printk("Warning: %s, (S == 2 or 3)\n", __func__);

	r = 1 << (value27 & 0x03);

	if ((value26 & (1 << 5)) == 0) {
		ns = ((value27 >> 4) & 0x0F) + 2;
		if ((ns < 3) || (ns >= 66))
			printk("Warning: %s, (3 <= NS < 66) required.\n", __func__);

		freq_out = 27000000 * ns / (s * r);
	} else {
		n = ((value27 >> 27) & 0x1F) + 1;
		freq_out = 27000000 * n * ((value27 >> 20) * 0x7F) / (s * r * 91);
	}

	return freq_out;
}

/*
 * HW doc: CKVCO = CK2160M = (27 MHz) * ( CK1620_EN ? 60 : 80) / (MO_PLLH_BWS ? 1 : 2)
 *         => CK2160M doesn't mean it's 2160 MHz, it may be either 2160 MHz or 1260 MHz.
 */
static u32 sp_pllh_ckvco(void)
{
	moonReg_t *pmoon0Reg = (moonReg_t *)LOGI_ADDR_MOON_REG;
	u32 freq_out;

	/* Grp 0.6[23], MO_PLLH_CK1620_EN */
	freq_out = 27000000 * ((pmoon0Reg->pllsys_cfg_sel & (1 << 23)) ? 60 : 80);
	/* Grp 0.6[27], MO_PLLH_BWS */
	if ((pmoon0Reg->pllsys_cfg_sel & (1 << 27)) == 0)
		freq_out /= 2;

	return freq_out;
}

static u32 sp_pllh_clk270_clk308p5(void)
{
	moonReg_t *pmoon0Reg = (moonReg_t *)LOGI_ADDR_MOON_REG;
	u32 div;

	/* Grp 0.6[23], MO_PLLH_CK1620_EN */
	if (pmoon0Reg->pllsys_cfg_sel & (1 << 23))
		return sp_pllh_ckvco() / 6;	/* always 270 MHz, Grp 0.6[30], MO_PLLH_SELCLK270_308P5 ignored */

	div = (pmoon0Reg->pllsys_cfg_sel & (1 << 30)) ? 7 : 8;	/* Grp 0.6[30], MO_PLLH_SELCLK270_308P5 */
	return sp_pllh_ckvco()/div;
}

static u32 sp_pllh_dsp(void)
{
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
	u32 freq_out, value;
	const u32 div[] = {8, 10, 9, 7};	/* (2 * {4, 5, 4.5, 3.5}) / 2 */

	value = (pmoon1Reg->sft_cfg_29 >> 5) & 0x03;	/* Grp 1.29[6:5], MO_PLLH_DSPCLKDIV */
	freq_out  = sp_pllh_ckvco() / div[value];
	freq_out *= 2;
	return freq_out;
}

static u32 sp_pllh_dcg(u32 dcg_n)
{
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
	u32 freq_out;
	const u32 div[] = {3, 4, 5, 6, 7, 8, 9, 10, 12};
	u32 dcg_n_clk_sel;

	freq_out = 0;

	if (dcg_n > 4) {
		printk("Error: %s, %d\n", __FILE__, __LINE__);
		return freq_out;
	}

	/* Grp 1.30[19:0], MO_PLLH_DCG4_CLK_SEL, ..., MO_PLLH_DCG0_CLK_SEL */
	dcg_n_clk_sel = ((pmoon1Reg->sft_cfg_30) >> (dcg_n * 4)) & 0x0F;
	if (dcg_n_clk_sel >= (sizeof(div) / sizeof(div[0]))) {
		printk("Error: %s, %d\n", __FILE__, __LINE__);
		return freq_out;
	}
	freq_out = sp_pllh_ckvco() / div[dcg_n_clk_sel];

	return freq_out;
}

static void sp_pllh_dcg_clk_sel(u32 dcg_n, u32 clk_sel)
{
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
	u32 tmp_u32;

	if (dcg_n > 4) {
		printk("Error: %s, %d\n", __FILE__, __LINE__);
		return;
	}

	tmp_u32 = pmoon1Reg->sft_cfg_30;	/* Grp 1.30[19:0], MO_PLLH_DCG4_CLK_SEL, ..., MO_PLLH_DCG0_CLK_SEL */
	tmp_u32 &= ~(0x0F << (4 * dcg_n));
	tmp_u32 |= (clk_sel & 0x0F) << (4 * dcg_n);
	pmoon1Reg->sft_cfg_30 = tmp_u32;
}

void sp_dpll_set(dpllReg_t *reg_ptr,	/* (dpllReg_t *)(LOGI_ADDR_DPLLx) */
		 u32 remainder, u32 denominator, u32 divider, u32 spread_num, u32 ctrl)
{
	/* Refer to sp_dpll_get_freq() for formula */
	reg_ptr->dpll_remainder   = remainder;
	reg_ptr->dpll_denominator = denominator;
	reg_ptr->dpll_divider     = divider;
	reg_ptr->dpll_sprd_num    = spread_num;
	reg_ptr->dpll_ctrl        = ctrl;
}

/*
 * DPLLx:
 * Page 11/12 of Q610_Clock_Arch_V0.92.pdf:
 *   Fout = Fin/(8*(DIV+(REMAINDER/DENOMINATOR))), DIV>=1, REMAINDER<DENOMINATOR
 *        = (Fin/8)*DENOMINATOR/(DIV*DENOMINATOR+REMAINDER)
 */
static u32 sp_dpll_get_freq(dpllReg_t *reg_ptr,		/* (dpllReg_t *)(LOGI_ADDR_DPLLx) */
			    u32 freq_in)
{
	u32 remainder, denominator, divider, ctrl;
	u32 freq_out;

	ctrl        = reg_ptr->dpll_ctrl;
	remainder   = reg_ptr->dpll_remainder;
	denominator = reg_ptr->dpll_denominator;
	divider     = reg_ptr->dpll_divider;

	freq_out  = freq_in / 8;

	if ((ctrl & 0x01) == 0)	/* disabled */
		return freq_out;

	if (divider == 0) {
		printk("Warning: %s, (Div == 0) at %p\n", __func__, reg_ptr);
		return freq_out;
	}

	if (remainder >= denominator) {
		printk("Warning: %s, (remainder >= denominator) at %p\n", __func__, reg_ptr);
		return freq_out;
	}

	freq_out *= denominator;
	freq_out /= (divider * denominator + remainder);

	return freq_out;
}

static u32 sp_dpll(u32 dpll_n)
{
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
	u32 freq_in, freq_out;
	const u32 logi_addr_dpll[] = {LOGI_ADDR_DPLL0, LOGI_ADDR_DPLL1, LOGI_ADDR_DPLL2, LOGI_ADDR_DPLL3};
	const int bit_n[] = {12, 13, 14, 15};	/* Bit of MO_DPLL0_CLK2160D2_SEL, .., MO_DPLL3_CLK2160D2_SEL, Grp 1.10 */

	freq_in = sp_pllh_ckvco();
	if ((pmoon1Reg->sft_cfg_10) & (1 << bit_n[dpll_n]))	/* Grp 1.10[??], MO_DPLL??_CLK2160D2_SEL */
		freq_in /= 2;

	freq_out = sp_dpll_get_freq((dpllReg_t *)(logi_addr_dpll[dpll_n]), freq_in);

	return freq_out;
}

static u32 sp_dpllspdif(void)
{
	moon2Reg_t *pmoon2Reg = (moon2Reg_t *)LOGI_ADDR_MOON2_REG;
	u32 value_2_4, value_2_7;
	const u32 fbdiv[] = {12, 16, 24, 32, 48, 64, 72, 96, 144};	/* for Grp 2.4[7:4], MO3_PLLSP_FBDIV */
	const u32 sr[] = {32000, 48000, 96000, 192000, 44100, 88200, 176400, 176400};	/* for Grp 2.4[26:24], MO3_PLLSP_SR */
	const u32 adcbck_sp[] = {
		 [0] =  1024000,
		 [1] =  1536000,
		 [2] =  2048000,
		 [3] =  2048000,
		 [4] =  1536000,
		 [5] =  2304000,
		 [6] =  3072000,
		 [7] =  3072000,
		 [8] =  3072000,
		 [9] =  4608000,
		[10] =  6144000,
		[11] =  6144000,
		[12] =  6144000,
		[13] =  9216000,
		[14] = 12288000,
		[15] = 12288000,
		[16] =  1411200,
		[17] =  2116800,
		[18] =  2822400,
		[19] =  2822400,
		[20] =  2822400,
		[21] =  4233600,
		[22] =  5644800,
		[23] =  5644800,
		[24] =  5644800,
		[25] =  8467200,
		[26] = 11289600,
		[27] = 11289600,
		[28] =  5644800,
		[29] =  8467200,
		[30] = 11289600,
		[31] = 11289600};

	u32 tmp_u32;
	u32 freq_out = 0;

	value_2_4 = pmoon2Reg->pllsp_ctl_0;
	value_2_7 = pmoon2Reg->pllsp_ctl_3;

	if (value_2_4 & (1 <<  2)) {	/* Grp 2.4[2], MO3_PLLSP_BP */
		printk("Warning: %s, %d: Bypass mode, don't know freq.\n", __FILE__, __LINE__);
	} else if ((value_2_7 & (1 << 17)) == 0) {	/* Grp 2.7[17], MO3_PLLSP_SRC_SEL */
		if ( ((value_2_4 & (1 <<  2)) == 0) &&	/* Grp 2.4[2], MO3_PLLSP_BP */
		     ((value_2_7 & (1 << 16)) != 0) ) { /* Grp 2.7[16], MO3_PLLSP_PDN */
			/*
			 * FCKOUT = SPDIFIN_SP * FBDIV
			 *        = 64 * sr[Grp 2.4[26:24]] * fbdiv[Grp 2.4[7:4]]
			 */
			tmp_u32 = (value_2_4 >> 4) & 0x0F;	/* Grp 2.4[7:4], MO3_PLLSP_FBDIV */
			if (tmp_u32 > 8) {
				tmp_u32 = 8;
				printk("Error: %s, %d\n", __FILE__, __LINE__);
			}
			freq_out = 64 * sr[(value_2_4 >> 24) & 0x07] * fbdiv[tmp_u32];
		}
	} else { /* (Grp 2.7[17], MO3_PLLSP_SRC_SEL) == 1'b1 */
		if ( ((value_2_4 & (1 << 30)) == 0) &&		/* Grp 2.4[30], MO3_PLLSP_FBDIVEN */
		     ((value_2_4 & (1 <<  2)) == 0) &&		/* Grp 2.4[2], MO3_PLLSP_BP */
		     ((value_2_7 & (1 << 16)) != 0) ) {		/* Grp 2.7[16], MO3_PLLSP_PDN */
			/*
			 * FCKOUT = ADCBCK_SP * FBDIV
			 *        = adcbck_sp[{Grp 2.4[26:24], Grp 2.4[29:28]}] * fbdiv[Grp 2.4[7:4]]
			 */
			tmp_u32 = (((value_2_4 >> 24) & 0x7) << 2) |
				  (((value_2_4 >> 28) & 0x3) << 0);
			freq_out = adcbck_sp[tmp_u32];
			tmp_u32 = (value_2_4 >> 4) & 0x0F;	/* Grp 2.4[7:4], MO3_PLLSP_FBDIV */
			if (tmp_u32 > 8) {
				tmp_u32 = 8;
				printk("Error: %s, %d\n", __FILE__, __LINE__);
			}
			freq_out *= fbdiv[tmp_u32];
		} else if ( ((value_2_4 & (1 << 30)) != 0) &&		/* Grp 2.4[30], MO3_PLLSP_FBDIVEN */
			    ((value_2_4 & (1 <<  2)) == 0) &&		/* Grp 2.4[2], MO3_PLLSP_BP */
			    ((value_2_7 & (1 << 16)) != 0) ) {		/* Grp 2.7[16], MO3_PLLSP_PDN */
			printk("Warning: %s, %d: Don't know input freq., return FBDIV instead.\n", __FILE__, __LINE__);
			tmp_u32 = (value_2_4 >> 4) & 0x0F;	/* Grp 2.4[7:4], MO3_PLLSP_FBDIV */
			if (tmp_u32 > 8) {
				tmp_u32 = 8;
				printk("Error: %s, %d\n", __FILE__, __LINE__);
			}
			freq_out = fbdiv[tmp_u32];
		}
	}

	return freq_out;
}

static u32 sp_dram_clk(void)
{
	u32 *ptr;

	ptr = (u32 *)(IO0_ADDRESS(50 * 32 * 4 + 12 * 4));	/* dpcu_aphy_mpll1 */
	return (*ptr & 0x0000007F) * 13500000;			/* DPCU_RI_MPLL_DIV */
}

u32 sp_clk_get(u32 which)
{
	moonReg_t  *pmoon0Reg = (moonReg_t *)LOGI_ADDR_MOON_REG;
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;

	u32 freq;

	switch (which) {
	case SP_CLK_SYSSLOW:
		freq = sp_pllh_clk270_clk308p5() >> ((pmoon0Reg->clk_sel[1] >> 4) & 0x0F);	/* Grp 0.5[7:4], MO_SYSSLOW_DIV_SEL_ENC */
		break;
	case SP_CLK_SYSFAST:
		freq = (sp_pllsys_plus_div() >> 1);
		break;
	case SP_CLK_CA9:
		if (pmoon1Reg->sft_cfg_12 & (1 << 16))	/* Grp 1.12[16], MO_CA9_DIVCLK_SEL */
			freq = sp_pllc1_plus_dfs();
		else
			freq = sp_pllsys_plus_div();
		break;
	case SP_CLK_ARM926:
		if (pmoon1Reg->sft_cfg_12 & (1 << 24))	/* Grp 1.12[24], MO_CLKA926CORE_SEL */
			freq = sp_pllc2();
		else
			freq = (sp_pllsys_plus_div() >> 1);
		break;
	case SP_CLK_SPI_NAND:
		if (pmoon0Reg->clk_sel[1] & (1 << 13))	/* Grp 0.5[13], MO_CLK_SPI_NAND */
			freq = sp_dpll(3);
		else
			freq = (sp_pllsys_plus_div() >> 1);
		break;
	case SP_CLK_SPI_NOR:
		if (pmoon0Reg->clk_sel[1] & (1 << 11))	/* Grp 0.5[11], MO_CLK_SPI_NOR */
			freq = sp_dpll(0);
		else
			freq = (sp_pllsys_plus_div() >> 1);
		break;
	case SP_CLK_SDCARD:
		if (pmoon0Reg->clk_sel[0] & (1 << 16))	/* Grp 0.4[16], MO_DSP_CLK_SEL[1] */
			freq = sp_pllh_dcg(2);
		else
			freq = sp_dpll(1);
		break;
	case SP_CLK_SDIO:
		if (pmoon0Reg->clk_sel[0] & (1 << 17))	/* Grp 0.4[17], MO_DSP_CLK_SEL[1] */
			freq = sp_pllh_dcg(3);
		else
			freq = sp_dpll(1);
		break;
	case SP_CLK_MFD:
		freq = sp_pllh_dcg(0);
		break;
	case SP_CLK_CA9_TRACER:
		freq = sp_pllh_dcg(4);
		if (pmoon1Reg->sft_cfg_22 & (1 << 19))	/* Grp 1.22[19], MO_TRACER_CLK_SEL */
			freq /= 2;
		break;
	case SP_CLK_DRAM:
		freq = sp_dram_clk();
		break;
	case SP_CLK_AUDIO_DSP:
		if (pmoon0Reg->clk_sel[0] & (1 << 19))	/* Grp 0.4[19], MO_DSP_CLK_SEL[1] */
			freq = (sp_pllsys_plus_div() >> 1);
		else if (pmoon0Reg->clk_sel[0] & (1 << 18))	/* Grp 0.4[18], MO_DSP_CLK_SEL[0] */
			freq = sp_pllh_dcg(1);
		else
			freq = sp_pllh_dsp();
		break;
	case SP_CLK_AUDIO_DPLL2:
		freq = sp_dpll(2);
		break;
	case SP_CLK_AUDIO_PLLA:
		freq = (pmoon1Reg->sft_cfg_24 & (1 << 31)) ? 135475200 : 147456000;	/* Grp 1.24[31], MO_PLLA_MODE */
		break;
	case SP_CLK_AUDIO_PLLSPDIF:
		freq = sp_dpllspdif();
		break;
	case SP_CLK_DISPLAY:
		freq = sp_plldis();
		break;
	default:
		freq = ~0;	/* Unknown */
		break;
	}

	return freq;
}
EXPORT_SYMBOL(sp_clk_get);

static void sp_clk_set_spi_nand(u32 freq)
{
/*
 * TODO:
 * * Allowed to change:
 *   * DPLL3, sp_dpll_set((dpllReg_t *)(LOGI_ADDR_DPLL3), ...)
 *   * Grp 1.10[15]
 *     * Input frequency of DPLL3 = (Grp 1.10[15]) ? sp_pllh_ckvco()/2 : sp_pllh_ckvco()
 *   * Grp 0.5[13]
 *
 */

	moonReg_t  *pmoon0Reg = (moonReg_t *)LOGI_ADDR_MOON_REG;
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;

	pmoon1Reg->sft_cfg_10 |= (1<<15);	// Grp 1.10[15] from PLLH  2160M HZ/2

	if(	85000000 <= freq && freq <= 95000000) {
		sp_dpll_set((dpllReg_t *)(LOGI_ADDR_DPLL3), 5, 10, 1, 0, 3);	//90MHz
	}
	else if(65000000 <= freq && freq <= 75000000) {
		sp_dpll_set((dpllReg_t *)(LOGI_ADDR_DPLL3), 9, 10, 1, 0, 3);	//71MHz
	}
	else if(50000000 <= freq && freq <= 60000000) {
		sp_dpll_set((dpllReg_t *)(LOGI_ADDR_DPLL3), 5, 10, 2, 0, 3);	//54MHz
	}
	else if(30000000 <= freq && freq <= 40000000) {
		sp_dpll_set((dpllReg_t *)(LOGI_ADDR_DPLL3), 5, 10, 3, 0, 3);	//38MHz
	}
	else if(15000000 <= freq && freq <= 25000000) {
		sp_dpll_set((dpllReg_t *)(LOGI_ADDR_DPLL3), 9, 10, 5, 0, 3);	//22MHz
	} else {
		printk("Error: %s, %d\n", __FILE__, __LINE__);
	}
	pmoon0Reg->clk_sel[1] |= 1 << 13;	//from DPLL3
}

static void sp_clk_set_spi_nor(u32 freq)
{
/*
 * TODO:
 * * Allowed to change:
 *   * DPLL0, sp_dpll_set((dpllReg_t *)(LOGI_ADDR_DPLL0), ...)
 *   * Grp 1.10[12]
 *     * Input frequency of DPLL0 = (Grp 1.10[12]) ? sp_pllh_ckvco()/2 : sp_pllh_ckvco()
 *   * Grp 0.5[11]
 *
 * moonReg_t  *pmoon0Reg = (moonReg_t *)LOGI_ADDR_MOON_REG;
 * moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
 */
}

static void sp_clk_set_sdcard(u32 freq)
{
/*
 * TODO:
 * * Allowed to change:
 *   * sp_pllh_dcg_clk_sel(2, ...), and use sp_pllh_dcg(2) to check if DCG2 frequency is expected.
 *   * Grp 0.4[16]
 *
 * moonReg_t  *pmoon0Reg = (moonReg_t *)LOGI_ADDR_MOON_REG;
 */
	/* Used to set MO_CLKCARD_SEL, G0.4[16]*/
	moonReg_t *pmoon0Reg = (moonReg_t *)LOGI_ADDR_MOON_REG;

	/*
	 * For DCG2, 270MHz was used as the target frequency during "static timing analysis (STA)"
	 * Setting it to higher frequencies may cause SD host to behave abnormally.
	 */
	switch (freq) {
	case 270000000:
		pmoon0Reg->clk_sel[0] |= (1 << 16);
		sp_pllh_dcg_clk_sel(2, 5);
		break;
	case 240000000:
		pmoon0Reg->clk_sel[0] |= (1 << 16);
		sp_pllh_dcg_clk_sel(2, 6);
		break;
	case 216000000:
		pmoon0Reg->clk_sel[0] |= (1 << 16);
		sp_pllh_dcg_clk_sel(2, 7);
		break;
	case 180000000:
		pmoon0Reg->clk_sel[0] |= (1 << 16);
		sp_pllh_dcg_clk_sel(2, 8);
		break;
	default:
		printk("Error: Invalid SD card host controller clk source setting %s, #%d\n", __FILE__, __LINE__);
	}
}

static void sp_clk_set_sdio(u32 freq)
{
/*
 * TODO:
 * * Allowed to change:
 *   * sp_pllh_dcg_clk_sel(3, ...), and use sp_pllh_dcg(3) to check if DCG3 frequency is expected.
 *   * Grp 0.4[17]
 *
 */
	moonReg_t  *pmoon0Reg = (moonReg_t *)LOGI_ADDR_MOON_REG;
	const u32 freq_tb[] = {
		[0] = 720000000,
		[1] =  540000000,
		[2] =  432000000,
		[3] =  360000000,
		[4] =  308000000,
		[5] =  270000000,
		[6] =  240000000,
		[7] =  216000000,
		[8] =  180000000,
		[9] = 0
	};
	u32 i;

	for (i = 0; freq_tb[i]; ++i) {
		if (freq_tb[i] == freq) {
			sp_pllh_dcg_clk_sel(3, i);
			if (freq != sp_pllh_dcg(3)) {
				printk("Error: %s, %d\n", __FILE__, __LINE__);
			} else {
				pmoon0Reg->clk_sel[0] |= BIT(17);
			}
		}
	}
}

static void sp_clk_set_audio_dsp(u32 freq)
{
	u32 value, tmp_1_29;
	moonReg_t  *pmoon0Reg = (moonReg_t *)LOGI_ADDR_MOON_REG;
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
		
	switch (freq) {
	case 540:
		value = 0;
		break;
	case 432:
		value = 1;
		break;
	case 617:
		value = 3;
		break;
	case 480:
	default:
		value = 2;
		break;
	}
	tmp_1_29 = pmoon1Reg->sft_cfg_29;
	tmp_1_29 &= ~((0x3) << 5);
	tmp_1_29 |= (value << 5);
	pmoon1Reg->sft_cfg_29 = tmp_1_29;		// Grp 1.29[6:5], PLLH DSPCLK output control
	
	pmoon0Reg->clk_sel[0] &= ~((0x3) << 18);	// Grp 0.4[19:18], DSPCLK source select PLLH
}

static void sp_clk_set_audio_dpll2(u32 freq)
{
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
	
	pmoon1Reg->sft_cfg_10 |= (1<<14);    // DPLL2 source clock always select CLK2160M/2

	switch (freq) {
	case 44100:
	case 88200:
	case 176400:
		sp_dpll_set((dpllReg_t *)(LOGI_ADDR_DPLL2), 3103, 3136, 2, 0, 3);
		break;
	/*
	case 32000:
	case 48000:
	case 96000:
	case 192000:
	*/
	default:
		sp_dpll_set((dpllReg_t *)(LOGI_ADDR_DPLL2), 1529, 2048, 2, 0, 3);
		break;
	}
}

static void sp_clk_set_audio_plla(u32 freq)
{
	u32 value, tmp_1_24;
	moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
	
	switch (freq) {
	case 135475200:
		value = 1;
		break;
	case 147456000:
	default:
		value = 0;
		break;
	}
	tmp_1_24 = pmoon1Reg->sft_cfg_24;
	tmp_1_24 |= 0x1 << 16;    // power on
	tmp_1_24 &= ~(0x1 << 31);
	tmp_1_24 |= value;        // Grp 1.24[31], PLLA output frequence control
	pmoon1Reg->sft_cfg_24 = tmp_1_24;
}

static void sp_clk_set_audio_pllspdif(u32 freq)
{
	//Grp 2.4 ~ Grp 2.7
	u32 value, tmp_2_4;
	moon2Reg_t *pmoon2Reg = (moon2Reg_t *)LOGI_ADDR_MOON2_REG;
	
	switch (freq) {
	case 32000:
		value = 0;
		break;
	case 96000:
		value = 2;
		break;
	case 192000:
		value = 3;
		break;
	case 44100:
		value = 4;
		break;
	case 88200:
		value = 5;
		break;
	case 176400:
		value = 6;
		break;
	case 48000:
	default:
		value = 1;
		break;
	}
	tmp_2_4 = pmoon2Reg->pllsp_ctl_0 & ~(0x7 << 24);
	pmoon2Reg->pllsp_ctl_0 = tmp_2_4 | (value << 24);
	pmoon2Reg->pllsp_ctl_3 = (1<<16);    // Grp 2.7, power up
}

static void sp_clk_set_mfd(u32 freq)
{
/*
 * TODO:
 * * Allowed to change:
 *   * sp_pllh_dcg_clk_sel(0, ...), and use sp_pllh_dcg(0) to check if DCG0 frequency is expected.
 */
}

u32 sp_clk_set(u32 which, u32 freq)
{
	switch (which) {
	case SP_CLK_SPI_NAND:
		sp_clk_set_spi_nand(freq);
		break;
	case SP_CLK_SPI_NOR:
		sp_clk_set_spi_nor(freq);
		break;
	case SP_CLK_SDCARD:
		sp_clk_set_sdcard(freq);
		break;
	case SP_CLK_SDIO:
		sp_clk_set_sdio(freq);
		break;
	case SP_CLK_MFD:
		sp_clk_set_mfd(freq);
		break;
	case SP_CLK_AUDIO_DSP:
		sp_clk_set_audio_dsp(freq);
		break;
	case SP_CLK_AUDIO_DPLL2:
		sp_clk_set_audio_dpll2(freq);
		break;
	case SP_CLK_AUDIO_PLLA:
		sp_clk_set_audio_plla(freq);
		break;
	case SP_CLK_AUDIO_PLLSPDIF:
		sp_clk_set_audio_pllspdif(freq);
		break;
	/*
	 * case SP_CLK_SYSSLOW:
	 * case SP_CLK_SYSFAST:
	 * case SP_CLK_CA9:
	 * case SP_CLK_ARM926:
	 * case SP_CLK_CA9_TRACER:
	 * case SP_CLK_DRAM:
	 * case SP_CLK_DISPLAY:
	*/
	default:
		printk("Error: %s, %d\n", __FILE__, __LINE__);
		break;
	}

	return sp_clk_get(which);
}
EXPORT_SYMBOL(sp_clk_set);

static int __init sp_clk_init_common_reg(void)
{
/*
 * TODO:
 * * Init common DPLLx, MUX. But it might be done in XBoot/UBoot already.
 *   * e.g., DPLL1 and Grp 1.10[13], which are used by both SDCard and SDIO.
 *     * Input frequency of DPLL1 = (Grp 1.10[13]) ? sp_pllh_ckvco()/2 : sp_pllh_ckvco()
 * * Init components which are not expected to change anymore.
 *   * e.g., clock for CA9_TRACER.
 *
 * moonReg_t  *pmoon0Reg = (moonReg_t *)LOGI_ADDR_MOON_REG;
 * moon1Reg_t *pmoon1Reg = (moon1Reg_t *)LOGI_ADDR_MOON1_REG;
 */
	return 0;
}

static int sp_clk_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	const char *string_ptr[] = {
		[SP_CLK_SYSSLOW]        =  "SYSSLOW",
		[SP_CLK_SYSFAST]        =  "SYSFAST",
		[SP_CLK_CA9]            =  "CA9",
		[SP_CLK_ARM926]         =  "ARM926",
		[SP_CLK_SPI_NAND]       =  "SPI_NAND",
		[SP_CLK_SPI_NOR]        =  "SPI_NOR",
		[SP_CLK_SDCARD]         =  "SDCARD",
		[SP_CLK_SDIO]           =  "SDIO",
		[SP_CLK_MFD]            =  "MFD",
		[SP_CLK_CA9_TRACER]     =  "CA9_TRACER",
		[SP_CLK_DRAM]           =  "DRAM",
		[SP_CLK_AUDIO_DSP]      =  "AUDIO_DSP",
		[SP_CLK_AUDIO_DPLL2]    =  "AUDIO_DPLL2",
		[SP_CLK_AUDIO_PLLA]     =  "AUDIO_PLLA",
		[SP_CLK_AUDIO_PLLSPDIF] =  "AUDIO_PLLSPDIF",
		[SP_CLK_DISPLAY]        =  "DISPLAY" };

	char *buffer_ptr;
	int  read_size;
	u32 freq, i;

	buffer_ptr = page;
	read_size = 0;
	for (i = 0; ; i++) {
		freq = sp_clk_get(i);
		if (freq == ~0)		/* Unknown */
			break;

		read_size += sprintf(buffer_ptr+read_size, "%-15s %10u\n", string_ptr[i], freq);
	}

	return read_size;
}

static int __init sp_clk_init(void)
{
	struct proc_dir_entry *ent;

	sp_clk_init_common_reg();

	ent = create_proc_entry("sp_clk", 0666, NULL);
	if (!ent) {
		printk("Error: %s, %d\n", __FILE__, __LINE__);
	} else {
		ent->read_proc = sp_clk_read;
	}
	return 0;
}
early_initcall(sp_clk_init);

#endif
MODULE_LICENSE("GPL v2");

