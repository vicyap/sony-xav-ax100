#ifndef __REGMAP_8600_H__
#define __REGMAP_8600_H__

/*
 * TYPE: RegisterFile0
 */
typedef struct
{
    // GROUP 000 : Chip Information
    UINT32  stamp                                 ; // 00  (ADDR : 0x9C00_0000)
    UINT32  emulation                             ; // 01  (ADDR : 0x9C00_0004)
    UINT32  G000_reserved_2                       ; // 02  (ADDR : 0x9C00_0008)
    UINT32  G000_reserved_3                       ; // 03  (ADDR : 0x9C00_000C)
    UINT32  clk_sel0                              ; // 04  (ADDR : 0x9C00_0010)
    UINT32  clk_sel1                              ; // 05  (ADDR : 0x9C00_0014)
    UINT32  sspll_cfg                             ; // 06  (ADDR : 0x9C00_0018)
    UINT32  clken0                                ; // 07  (ADDR : 0x9C00_0020)
    UINT32  clken1                                ; // 08  (ADDR : 0x9C00_0024)
    UINT32  clken2                                ; // 09  (ADDR : 0x9C00_0028)
    UINT32  clken3                                ; // 10  (ADDR : 0x9C00_002C)
    UINT32  clken4                                ; // 11  (ADDR : 0x9C00_002C)
    UINT32  gclken0                               ; // 12  (ADDR : 0x9C00_0030)
    UINT32  gclken1                               ; // 13  (ADDR : 0x9C00_0034)
    UINT32  gclken2                               ; // 14  (ADDR : 0x9C00_0038)
    UINT32  gclken3                               ; // 15  (ADDR : 0x9C00_003C)
    UINT32  gclken4                               ; // 16  (ADDR : 0x9C00_003C)
    UINT32  reset0                                ; // 17  (ADDR : 0x9C00_0040)
    UINT32  reset1                                ; // 18  (ADDR : 0x9C00_0044)
    UINT32  reset2                                ; // 19  (ADDR : 0x9C00_0048)
    UINT32  reset3                                ; // 20  (ADDR : 0x9C00_004C)
    UINT32  reset4                                ; // 21  (ADDR : 0x9C00_004C)
    UINT32  pwr_iso                               ; // 22  (ADDR : 0x9C00_0058)
    UINT32  pwr_ctl                               ; // 23  (ADDR : 0x9C00_005C)
    UINT32  hw_bo0                                ; // 24  (ADDR : 0x9C00_0060)
    UINT32  hw_bo1                                ; // 25  (ADDR : 0x9C00_0064)
    UINT32  hw_bo2                                ; // 26  (ADDR : 0x9C00_0068)
    UINT32  hw_bo3                                ; // 27  (ADDR : 0x9C00_006C)
    UINT32  hw_cfg                                ; // 28  (ADDR : 0x9C00_0070)
    UINT32  hw_cfg_chg                            ; // 29  (ADDR : 0x9C00_0074)
    UINT32  G000_reserved_30                      ; // 30  (ADDR : 0x9C00_0078)
    UINT32  show_bo_stamp                         ; // 31  (ADDR : 0x9C00_007C)

    // Group 001 : MOON1
    UINT32  rf_sft_cfg0                           ; // 00  (ADDR : 0x9C00_0080)
    UINT32  rf_sft_cfg1                           ; // 01  (ADDR : 0x9C00_0084)
    UINT32  rf_sft_cfg2                           ; // 02  (ADDR : 0x9C00_0088)
    UINT32  rf_sft_cfg3                           ; // 03  (ADDR : 0x9C00_008C)
    UINT32  rf_sft_cfg4                           ; // 04  (ADDR : 0x9C00_0090)
    UINT32  rf_sft_cfg5                           ; // 05  (ADDR : 0x9C00_0094)
    UINT32  rf_sft_cfg6                           ; // 06  (ADDR : 0x9C00_0098)
    UINT32  rf_sft_cfg7                           ; // 07  (ADDR : 0x9C00_009C)
    UINT32  rf_sft_cfg8                           ; // 08  (ADDR : 0x9C00_00A0)
    UINT32  rf_sft_cfg9                           ; // 09  (ADDR : 0x9C00_00A4)
    UINT32  rf_sft_cfg10                          ; // 10  (ADDR : 0x9C00_00A8)
    UINT32  rf_sft_cfg11                          ; // 11  (ADDR : 0x9C00_00AC)
    UINT32  rf_sft_cfg12                          ; // 12  (ADDR : 0x9C00_00B0)
    UINT32  rf_sft_cfg13                          ; // 13  (ADDR : 0x9C00_00B4)
    UINT32  rf_sft_cfg14                          ; // 14  (ADDR : 0x9C00_00B8)
    UINT32  rf_sft_cfg15                          ; // 15  (ADDR : 0x9C00_00BC)
    UINT32  rf_sft_cfg16                          ; // 16  (ADDR : 0x9C00_00C0)
    UINT32  rf_sft_cfg17                          ; // 17  (ADDR : 0x9C00_00C4)
    UINT32  rf_sft_cfg18                          ; // 18  (ADDR : 0x9C00_00C8)
    UINT32  rf_sft_cfg19                          ; // 19  (ADDR : 0x9C00_00CC)
    UINT32  rf_sft_cfg20                          ; // 20  (ADDR : 0x9C00_00D0)
    UINT32  rf_sft_cfg21                          ; // 21  (ADDR : 0x9C00_00D4)
    UINT32  rf_sft_cfg22                          ; // 22  (ADDR : 0x9C00_00D8)
    UINT32  rf_sft_cfg23                          ; // 23  (ADDR : 0x9C00_00DC)
    UINT32  rf_sft_cfg24                          ; // 24  (ADDR : 0x9C00_00E0)
    UINT32  rf_sft_cfg25                          ; // 25  (ADDR : 0x9C00_00E4)
    UINT32  rf_sft_cfg26                          ; // 26  (ADDR : 0x9C00_00E8)
    UINT32  rf_sft_cfg27                          ; // 27  (ADDR : 0x9C00_00EC)
    UINT32  rf_sft_cfg28                          ; // 28  (ADDR : 0x9C00_00F0)
    UINT32  rf_sft_cfg29                          ; // 29  (ADDR : 0x9C00_00F4)
    UINT32  rf_sft_cfg30                          ; // 30  (ADDR : 0x9C00_00F8)
    UINT32  rf_sft_cfg31                          ; // 31  (ADDR : 0x9C00_00FC)

    // Group 002 : Reserved
    UINT32  G002_RESERVED[32]                     ; //     (ADDR : 0x9C00_0100) ~ (ADDR : 0x9C00_017C)

    // Group 003 : Reserved
    UINT32  G003_RESERVED[32]                     ; //     (ADDR : 0x9C00_0180) ~ (ADDR : 0x9C00_01FC)

    // Group 004 : PAD_CTL
    UINT32  rf_pad_ctl0                           ; // 00  (ADDR : 0x9C00_0200)
    UINT32  rf_pad_ctl1                           ; // 01  (ADDR : 0x9C00_0204)
    UINT32  rf_pad_ctl2                           ; // 02  (ADDR : 0x9C00_0208)
    UINT32  rf_pad_ctl3                           ; // 03  (ADDR : 0x9C00_020C)
    UINT32  rf_pad_ctl4                           ; // 04  (ADDR : 0x9C00_0210)
    UINT32  rf_pad_ctl5                           ; // 05  (ADDR : 0x9C00_0214)
    UINT32  rf_pad_ctl6                           ; // 06  (ADDR : 0x9C00_0218)
    UINT32  rf_pad_ctl7                           ; // 07  (ADDR : 0x9C00_021C)
    UINT32  rf_pad_ctl8                           ; // 08  (ADDR : 0x9C00_0220)
    UINT32  rf_pad_ctl9                           ; // 09  (ADDR : 0x9C00_0224)
    UINT32  rf_pad_ctl10                          ; // 10  (ADDR : 0x9C00_0228)
    UINT32  rf_pad_ctl11                          ; // 11  (ADDR : 0x9C00_022C)
    UINT32  rf_pad_ctl12                          ; // 12  (ADDR : 0x9C00_0230)
    UINT32  rf_pad_ctl13                          ; // 13  (ADDR : 0x9C00_0234)
    UINT32  rf_pad_ctl14                          ; // 14  (ADDR : 0x9C00_0238)
    UINT32  rf_pad_ctl15                          ; // 15  (ADDR : 0x9C00_023C)
    UINT32  rf_pad_ctl16                          ; // 16  (ADDR : 0x9C00_0240)
    UINT32  rf_pad_ctl17                          ; // 17  (ADDR : 0x9C00_0244)
    UINT32  rf_pad_ctl18                          ; // 18  (ADDR : 0x9C00_0248)
    UINT32  rf_pad_ctl19                          ; // 19  (ADDR : 0x9C00_024C)
    UINT32  rf_pad_ctl20                          ; // 20  (ADDR : 0x9C00_0250)
    UINT32  rf_pad_ctl21                          ; // 21  (ADDR : 0x9C00_0254)
    UINT32  rf_pad_ctl22                          ; // 22  (ADDR : 0x9C00_0258)
    UINT32  rf_pad_ctl23                          ; // 23  (ADDR : 0x9C00_025C)
    UINT32  rf_pad_ctl24                          ; // 24  (ADDR : 0x9C00_0260)
    UINT32  rf_pad_ctl25                          ; // 25  (ADDR : 0x9C00_0264)
    UINT32  rf_pad_ctl26                          ; // 26  (ADDR : 0x9C00_0268)
    UINT32  rf_pad_ctl27                          ; // 27  (ADDR : 0x9C00_026C)
    UINT32  rf_pad_ctl28                          ; // 28  (ADDR : 0x9C00_0270)
    UINT32  rf_pad_ctl29                          ; // 29  (ADDR : 0x9C00_0274)
    UINT32  rf_pad_ctl30                          ; // 30  (ADDR : 0x9C00_0278)
    UINT32  rf_pad_ctl31                          ; // 31  (ADDR : 0x9C00_027C)

    // Group 005 : GPIOXT
    UINT32  G005_GPIOXT[32]                       ; //     (ADDR : 0x9C00_0280) ~ (ADDR : 0x9C00_02FC)

    // Group 006 : Reserved
    UINT32  G006_RESERVED[32]                     ; //     (ADDR : 0x9C00_0300) ~ (ADDR : 0x9C00_037C)

    // Group 007 : PAD_CTRL1
    UINT32  G007_PAD_CTRL1[32]                       ; //     (ADDR : 0x9C00_0380) ~ (ADDR : 0x9C00_03FC)

    // Group 008 : IOP
    UINT32  iop_control                           ; //
    UINT32  iop_status                            ; //
    UINT32  iop_bp                                ; //
    UINT32  iop_regsel                            ; //
    UINT32  iop_regout                            ; //
    UINT32  iop_memlimit                          ; //
//  UINT32  reserved[2]                           ; //
    UINT32  iop_resume_pcl                        ; //
    UINT32  iop_resume_pch                        ; //
    UINT32  iop_data[12]                          ; //
    UINT32  iop_lbus_offset_ext                   ; //
    UINT32  iop_lbus_offset                       ; //
    UINT32  iop_lbus_control                      ; //
    UINT32  iop_lbus_offset2_lsb                  ; //
    UINT32  iop_lbus_offset2                      ; //
    UINT32  iop_direct_adr                        ; //
    UINT32  g8_reserved[6]                        ; //

    // Group 009 : Interrupt
    UINT32  intr_type[7]                          ;
    UINT32  intr_polarity[7]                      ;
    UINT32  a926_priority[7]                      ;
    UINT32  a926_intr_mask[7]                     ;
    UINT32  dsp_intr_clr0                         ;
    UINT32  dsp_intr_clr1                         ;
    UINT32  dsp_intr_clr2                         ;
    UINT32  dsp_intr_clr3                         ;

    // Group 010 : interrupt
    UINT32  a926_intr_clr[7]                      ;
    UINT32  masked_a926_fiqs[7]                   ;
    UINT32  masked_a926_irqs[7]                   ;
    UINT32  dsp_intr_mask[7]                      ;
    UINT32  dsp_intr_clr4                         ;
    UINT32  dsp_intr_clr5                         ;
    UINT32  dsp_intr_clr6                         ;
    UINT32  g10_reserved                          ;

    // Group 011 : TSET
    UINT32  G011_TSET[32]                         ;

    // Group 012 : STC
    UINT32  stc_15_0                              ; // 00  384
    UINT32  stc_31_16                             ; // 01  385
    UINT32  stc_64                                ; // 02  386
    UINT32  stc_divisor                           ; // 03  387
    UINT32  rtc_15_0                              ; // 04  388
    UINT32  rtc_31_16                             ; // 05  389
    UINT32  rtc_divisor                           ; // 06  390
    UINT32  stc_config                            ; // 07  391
    UINT32  timer0_ctrl                           ; // 08  392
    UINT32  timer0_cnt                            ; // 09  393
    UINT32  timer1_ctrl                           ; // 0a  394
    UINT32  timer1_cnt                            ; // 0b  395
    UINT32  timerw_ctrl                           ; // 0c  396
    UINT32  timerw_cnt                            ; // 0d  397
    UINT32  stc_47_32                             ; // 0e  385
    UINT32  stc_63_48                             ; // 0f  385
    UINT32  timer2_ctrl                           ; // 10  400
    UINT32  timer2_divisor                        ; // 11  401
    UINT32  timer2_reload                         ; // 12  402
    UINT32  timer2_cnt                            ; // 13  403
    UINT32  timer3_ctrl                           ; // 14  404
    UINT32  timer3_divisor                        ; // 15  405
    UINT32  timer3_reload                         ; // 16  406
    UINT32  timer3_cnt                            ; // 17  407
    UINT32  stcl_0                                ; // 18  408
    UINT32  stcl_1                                ; // 19  409
    UINT32  stcl_2                                ; // 1a  410
    UINT32  atc_0                                 ; // 1b  411
    UINT32  atc_1                                 ; // 1c  412
    UINT32  atc_2                                 ; // 1d  413
    UINT32  g12_reserved[2]                       ; // 1e  414

    // Group 013 : BR_WRAPPER
    UINT32  G013_BR_WRAPPER[32]                   ;

    // Group 014 : DUMMY_MASTER
    UINT32  dummy0_op_mode                        ; // 00  (ADDR : 0x9C00_0700)
    UINT32  dummy0_addr_base                      ; // 01  (ADDR : 0x9C00_0704)
    UINT32  dummy0_addr_offset                    ; // 02  (ADDR : 0x9C00_0708)
    UINT32  dummy0_control                        ; // 03  (ADDR : 0x9C00_070C)
    UINT32  dummy0_urgent                         ; // 04  (ADDR : 0x9C00_0710)
    UINT32  dummy0_request_period                 ; // 05  (ADDR : 0x9C00_0714)
    UINT32  G14_reserved_6                        ; // 06  (ADDR : 0x9C00_0718)
    UINT32  dummy0_error                          ; // 07  (ADDR : 0x9C00_071C)
    UINT32  dummy0_runtime_l                      ; // 08  (ADDR : 0x9C00_0720)
    UINT32  dummy0_runtime_h                      ; // 09  (ADDR : 0x9C00_0724)
    UINT32  dummy0_data_count_l                   ; // 10  (ADDR : 0x9C00_0728)
    UINT32  dummy0_data_count_h                   ; // 11  (ADDR : 0x9C00_072C)
    UINT32  dummy0_mdata_count_l                  ; // 12  (ADDR : 0x9C00_0730)
    UINT32  dummy0_mdata_count_h                  ; // 13  (ADDR : 0x9C00_0734)
    UINT32  dummy0_sdata_count_l                  ; // 14  (ADDR : 0x9C00_0738)
    UINT32  dummy0_sdata_count_h                  ; // 15  (ADDR : 0x9C00_073C)
    UINT32  dummy1_op_mode                        ; // 16  (ADDR : 0x9C00_0740)
    UINT32  dummy1_addr_base                      ; // 17  (ADDR : 0x9C00_0744)
    UINT32  dummy1_addr_offset                    ; // 18  (ADDR : 0x9C00_0748)
    UINT32  dummy1_control                        ; // 19  (ADDR : 0x9C00_074C)
    UINT32  dummy1_urgent                         ; // 20  (ADDR : 0x9C00_0750)
    UINT32  dummy1_request_period                 ; // 21  (ADDR : 0x9C00_0754)
    UINT32  G14_reserved_16                       ; // 22  (ADDR : 0x9C00_0758)
    UINT32  dummy1_error                          ; // 23  (ADDR : 0x9C00_075C)
    UINT32  dummy1_runtime_l                      ; // 24  (ADDR : 0x9C00_0760)
    UINT32  dummy1_runtime_h                      ; // 25  (ADDR : 0x9C00_0764)
    UINT32  dummy1_data_count_l                   ; // 26  (ADDR : 0x9C00_0768)
    UINT32  dummy1_data_count_h                   ; // 27  (ADDR : 0x9C00_076C)
    UINT32  dummy1_mdata_count_l                  ; // 28  (ADDR : 0x9C00_0770)
    UINT32  dummy1_mdata_count_h                  ; // 29  (ADDR : 0x9C00_0774)
    UINT32  dummy1_sdata_count_l                  ; // 30  (ADDR : 0x9C00_0778)
    UINT32  dummy1_sdata_count_h                  ; // 31  (ADDR : 0x9C00_077C)

    // Group 015 : Reserved
    UINT32  G015_RESERVED[32]                     ;

    // Group 016 : UART2
    UINT32  uart2_data                            ; // 00  (ADDR : 0x9C00_0900)
    UINT32  uart2_lsr                             ; // 01  (ADDR : 0x9C00_0904)
    UINT32  uart2_msr                             ; // 02  (ADDR : 0x9C00_0908)
    UINT32  uart2_lcr                             ; // 03  (ADDR : 0x9C00_090C)
    UINT32  uart2_mcr                             ; // 04  (ADDR : 0x9C00_0910)
    UINT32  uart2_div_l                           ; // 05  (ADDR : 0x9C00_0914)
    UINT32  uart2_div_h                           ; // 06  (ADDR : 0x9C00_0918)
    UINT32  uart2_isc                             ; // 07  (ADDR : 0x9C00_091C)
    UINT32  uart2_tx_residue                      ; // 08  (ADDR : 0x9C00_0920)
    UINT32  uart2_rx_residue                      ; // 09  (ADDR : 0x9C00_0924)
    UINT32  uart2_rxfifo_thr                      ; // 10  (ADDR : 0x9C00_0928)
    UINT32  G016_reserved[21]                     ;

    // Group 017 : UART3
    UINT32  uart3_data                            ; // 00  (ADDR : 0x9C00_0880)
    UINT32  uart3_lsr                             ; // 01  (ADDR : 0x9C00_0884)
    UINT32  uart3_msr                             ; // 02  (ADDR : 0x9C00_0888)
    UINT32  uart3_lcr                             ; // 03  (ADDR : 0x9C00_088C)
    UINT32  uart3_mcr                             ; // 04  (ADDR : 0x9C00_0890)
    UINT32  uart3_div_l                           ; // 05  (ADDR : 0x9C00_0894)
    UINT32  uart3_div_h                           ; // 06  (ADDR : 0x9C00_0898)
    UINT32  uart3_isc                             ; // 07  (ADDR : 0x9C00_089C)
    UINT32  uart3_tx_residue                      ; // 08  (ADDR : 0x9C00_08A0)
    UINT32  uart3_rx_residue                      ; // 09  (ADDR : 0x9C00_08A4)
    UINT32  uart3_rxfifo_thr                      ; // 10  (ADDR : 0x9C00_08A8)
    UINT32  G017_reserved[21]                     ; //     (ADDR : 0x9C00_08AC)

    // Group 018 : UART0
    UINT32  uart0_data                            ; // 00  (ADDR : 0x9C00_0900)
    UINT32  uart0_lsr                             ; // 01  (ADDR : 0x9C00_0904)
    UINT32  uart0_msr                             ; // 02  (ADDR : 0x9C00_0908)
    UINT32  uart0_lcr                             ; // 03  (ADDR : 0x9C00_090C)
    UINT32  uart0_mcr                             ; // 04  (ADDR : 0x9C00_0910)
    UINT32  uart0_div_l                           ; // 05  (ADDR : 0x9C00_0914)
    UINT32  uart0_div_h                           ; // 06  (ADDR : 0x9C00_0918)
    UINT32  uart0_isc                             ; // 07  (ADDR : 0x9C00_091C)
    UINT32  uart0_tx_residue                      ; // 08  (ADDR : 0x9C00_0920)
    UINT32  uart0_rx_residue                      ; // 09  (ADDR : 0x9C00_0924)
    UINT32  uart0_rxfifo_thr                      ; // 10  (ADDR : 0x9C00_0928)
    UINT32  G018_reserved[21]                     ;

    // Group 019 : UART1
    UINT32  uart1_data                            ; // 00  (ADDR : 0x9C00_0900)
    UINT32  uart1_lsr                             ; // 01  (ADDR : 0x9C00_0904)
    UINT32  uart1_msr                             ; // 02  (ADDR : 0x9C00_0908)
    UINT32  uart1_lcr                             ; // 03  (ADDR : 0x9C00_090C)
    UINT32  uart1_mcr                             ; // 04  (ADDR : 0x9C00_0910)
    UINT32  uart1_div_l                           ; // 05  (ADDR : 0x9C00_0914)
    UINT32  uart1_div_h                           ; // 06  (ADDR : 0x9C00_0918)
    UINT32  uart1_isc                             ; // 07  (ADDR : 0x9C00_091C)
    UINT32  uart1_tx_residue                      ; // 08  (ADDR : 0x9C00_0920)
    UINT32  uart1_rx_residue                      ; // 09  (ADDR : 0x9C00_0924)
    UINT32  uart1_rxfifo_thr                      ; // 10  (ADDR : 0x9C00_0928)
    UINT32  G019_reserved[21]                     ;

    // Group 020 : DPLL
    UINT32  G020_DPLL[32]                         ;

    // Group 021 : Reserved
    UINT32  G021_RESERVED[32]                     ;

    // Group 022 : SPI_FLASH
    UINT32  spi_ctrl                              ;
    UINT32  spi_wait                              ;
    UINT32  spi_cust_cmd                          ;
    UINT32  spi_addr_low                          ;
    UINT32  spi_addr_high                         ;
    UINT32  spi_data_low                          ;
    UINT32  spi_data_high                         ;
    UINT32  spi_status                            ;
    UINT32  spi_cfg0                              ;
    UINT32  spi_cfg1                              ;
    UINT32  spi_cfg2                              ;
    UINT32  spi_cfg3                              ;
    UINT32  spi_cfg4                              ;
    UINT32  spi_cfg5                              ;
    UINT32  spi_cfg6                              ;
    UINT32  spi_cfg7                              ;
    UINT32  spi_cfg8                              ;
    UINT32  spi_cust_cmd2                         ;
    UINT32  spi_data64                            ;
    UINT32  spi_buf_addr                          ;
    UINT32  spi_status2                           ;
    UINT32  spi_status3                           ;
    UINT32  g22_reserved_0[10]                    ;

    // Group 023 : SPI_FLASH
    UINT32  G023_SPI_FLASH[32]                    ;

    // Group 024 : CB_SWITCH
    UINT32  G024_CB_SWITCH[32]                    ; //     (ADDR : 0x9C00_0C00) ~ (ADDR : 0x9C00_0C7C)

    // Group 025 : PBUS
    UINT32  G025_PBUS[32]                         ; //     (ADDR : 0x9C00_0C80) ~ (ADDR : 0x9C00_0CFC)

    // Group 026 : CB_DMA
    UINT32  cbdma_ver                             ; // 00  (ADDR : 0x9C00_0D00)
    UINT32  cbdma_config                          ; // 01  (ADDR : 0x9C00_0D04)
    UINT32  cbdma_dma_length                      ; // 02  (ADDR : 0x9C00_0D08)
    UINT32  cbdma_src_adr                         ; // 03  (ADDR : 0x9C00_0D0c)
    UINT32  cbdma_des_adr                         ; // 04  (ADDR : 0x9C00_0D10)
    UINT32  cbdma_int_status                      ; // 05  (ADDR : 0x9C00_0D14)
    UINT32  cbdma_int_en                          ; // 06  (ADDR : 0x9C00_0D18)
    UINT32  cbdma_memset_value                    ; // 07  (ADDR : 0x9C00_0D1c)
    UINT32  cbdma_sdram_size                      ; // 08  (ADDR : 0x9C00_0D20)
    UINT32  G026_reserved_09                      ; // 09  (ADDR : 0x9C00_0D24)
    UINT32  cbdma_sg_index                        ; // 10  (ADDR : 0x9C00_0D28)
    UINT32  cbdma_sg_config                       ; // 11  (ADDR : 0x9C00_0D2C)
    UINT32  cbdma_sg_dma_length                   ; // 12  (ADDR : 0x9C00_0D30)
    UINT32  cbdma_sg_src_adr                      ; // 13  (ADDR : 0x9C00_0D34)
    UINT32  cbdma_sg_des_adr                      ; // 14  (ADDR : 0x9C00_0D38)
    UINT32  cbdma_sg_memset_value                 ; // 15  (ADDR : 0x9C00_0D3C)
    UINT32  cbdma_sg_setting                      ; // 16  (ADDR : 0x9C00_0D40)
    UINT32  G026_reserved_17                      ; // 17  (ADDR : 0x9C00_0D44)
    UINT32  G026_reserved_18                      ; // 18  (ADDR : 0x9C00_0D48)
    UINT32  G026_reserved_19                      ; // 19  (ADDR : 0x9C00_0D4C)
    UINT32  G026_reserved_20                      ; // 20  (ADDR : 0x9C00_0D50)
    UINT32  G026_reserved_21                      ; // 21  (ADDR : 0x9C00_0D54)
    UINT32  G026_reserved_22                      ; // 22  (ADDR : 0x9C00_0D58)
    UINT32  G026_reserved_23                      ; // 23  (ADDR : 0x9C00_0D5C)
    UINT32  G026_reserved_24                      ; // 24  (ADDR : 0x9C00_0D60)
    UINT32  G026_reserved_25                      ; // 25  (ADDR : 0x9C00_0D64)
    UINT32  G026_reserved_26                      ; // 26  (ADDR : 0x9C00_0D68)
    UINT32  G026_reserved_27                      ; // 27  (ADDR : 0x9C00_0D6C)
    UINT32  G026_reserved_28                      ; // 28  (ADDR : 0x9C00_0D70)
    UINT32  G026_reserved_29                      ; // 29  (ADDR : 0x9C00_0D74)
    UINT32  G026_reserved_30                      ; // 30  (ADDR : 0x9C00_0D78)
    UINT32  G026_reserved_31                      ; // 31  (ADDR : 0x9C00_0D7C)

    // Group 027 : CB_DMA1
    UINT32  cbdma1_ver                            ; // 00  (ADDR : 0x9C00_0D00)
    UINT32  cbdma1_config                         ; // 01  (ADDR : 0x9C00_0D04)
    UINT32  cbdma1_dma_length                     ; // 02  (ADDR : 0x9C00_0D08)
    UINT32  cbdma1_src_adr                        ; // 03  (ADDR : 0x9C00_0D0c)
    UINT32  cbdma1_des_adr                        ; // 04  (ADDR : 0x9C00_0D10)
    UINT32  cbdma1_int_status                     ; // 05  (ADDR : 0x9C00_0D14)
    UINT32  cbdma1_int_en                         ; // 06  (ADDR : 0x9C00_0D18)
    UINT32  cbdma1_memset_value                   ; // 07  (ADDR : 0x9C00_0D1c)
    UINT32  cbdma1_sdram_size                     ; // 08  (ADDR : 0x9C00_0D20)
    UINT32  G027_reserved_09                      ; // 09  (ADDR : 0x9C00_0D24)
    UINT32  cbdma1_sg_index                       ; // 10  (ADDR : 0x9C00_0D28)
    UINT32  cbdma1_sg_config                      ; // 11  (ADDR : 0x9C00_0D2C)
    UINT32  cbdma1_sg_dma_length                  ; // 12  (ADDR : 0x9C00_0D30)
    UINT32  cbdma1_sg_src_adr                     ; // 13  (ADDR : 0x9C00_0D34)
    UINT32  cbdma1_sg_des_adr                     ; // 14  (ADDR : 0x9C00_0D38)
    UINT32  cbdma1_sg_memset_value                ; // 15  (ADDR : 0x9C00_0D3C)
    UINT32  cbdma1_sg_setting                     ; // 16  (ADDR : 0x9C00_0D40)
    UINT32  G027_reserved_17                      ; // 17  (ADDR : 0x9C00_0D44)
    UINT32  G027_reserved_18                      ; // 18  (ADDR : 0x9C00_0D48)
    UINT32  G027_reserved_19                      ; // 19  (ADDR : 0x9C00_0D4C)
    UINT32  G027_reserved_20                      ; // 20  (ADDR : 0x9C00_0D50)
    UINT32  G027_reserved_21                      ; // 21  (ADDR : 0x9C00_0D54)
    UINT32  G027_reserved_22                      ; // 22  (ADDR : 0x9C00_0D58)
    UINT32  G027_reserved_23                      ; // 23  (ADDR : 0x9C00_0D5C)
    UINT32  G027_reserved_24                      ; // 24  (ADDR : 0x9C00_0D60)
    UINT32  G027_reserved_25                      ; // 25  (ADDR : 0x9C00_0D64)
    UINT32  G027_reserved_26                      ; // 26  (ADDR : 0x9C00_0D68)
    UINT32  G027_reserved_27                      ; // 27  (ADDR : 0x9C00_0D6C)
    UINT32  G027_reserved_28                      ; // 28  (ADDR : 0x9C00_0D70)
    UINT32  G027_reserved_29                      ; // 29  (ADDR : 0x9C00_0D74)
    UINT32  G027_reserved_30                      ; // 30  (ADDR : 0x9C00_0D78)
    UINT32  G027_reserved_31                      ; // 31  (ADDR : 0x9C00_0D7C)

    // Group 028 : Reserved
    UINT32  G028_RESERVED[32]                     ;

    // Group 029 : MBUS
    UINT32  mbus_setting                          ;
    UINT32  G029_reserved[15]                     ;
    UINT32  mbus_int_en                           ;
    UINT32  mbus_int_status                       ;
    UINT32  mbus_int_addr                         ;
    UINT32  mbus_mx_ab_sel                        ;
    UINT32  mbus_mx_ab_arb_type                   ;
    UINT32  mbus_mx_ab_priority                   ;
    UINT32  G029_reserved1[10]                    ;

    // Group 030 : Reserved
    UINT32  G030_RESERVED[32]                     ;

    // Group 031 : MBUS
    UINT32  mbar0_page_setting                    ;
    UINT32  mbar0_m1_setting                      ;
    UINT32  mbar0_m2_setting                      ;
    UINT32  mbar0_m3_setting                      ;
    UINT32  mbar0_m4_setting                      ;
    UINT32  mbar0_m5_setting                      ;
    UINT32  mbar0_m6_setting                      ;
    UINT32  mbar0_m7_setting                      ;
    UINT32  mbar0_m8_setting                      ;
    UINT32  mbar0_m9_setting                      ;
    UINT32  mbar0_m10_setting                     ;
    UINT32  mbar0_m11_setting                     ;
    UINT32  mbar0_m12_setting                     ;
    UINT32  mbar0_m13_setting                     ;
    UINT32  mbar0_m14_setting                     ;
    UINT32  mbar0_m15_setting                     ;
    UINT32  mbar0_m16_setting                     ;
    UINT32  mbar0_m17_setting                     ;
    UINT32  mbar0_m18_setting                     ;
    UINT32  mbar0_m19_setting                     ;
    UINT32  mbar0_m20_setting                     ;
    UINT32  mbar0_m21_setting                     ;
    UINT32  mbar0_m22_setting                     ;
    UINT32  mbar0_m23_setting                     ;
    UINT32  mbar0_m24_setting                     ;
    UINT32  mbar0_m25_setting                     ;
    UINT32  mbar0_m26_setting                     ;
    UINT32  mbar0_m27_setting                     ;
    UINT32  mbar0_m28_setting                     ;
    UINT32  mbar0_m29_setting                     ;
    UINT32  mbar0_m30_setting                     ;
    UINT32  mbar0_m31_setting                     ;

    // Group 032 : SDCTRL0
    UINT32  G032_SDCTRL0[32]                      ;

    // Group 033 : SDCTRL0
    UINT32  G033_SDCTRL0[32]                      ;

    // Group 034 : SDCTRL0
    UINT32  G034_SDCTRL0[32]                      ;

    // Group 035 : SDCTRL0
    UINT32  G035_SDCTRL0[32]                      ;

    // Group 036 : SDCTRL0 (2DBG)
    UINT32  G036_SDCTRL0[32]                     ;

    // Group 037 : SDCTRL0 (2DBG)
    UINT32  G037_SDCTRL0[32]                     ;

    // Group 038 : SDCTRL0 (2DBG)
    UINT32  G038_SDCTRL0[32]                     ;

    // Group 039 : SDCTRL0 (2DBG)
    UINT32  G039_SDCTRL0[32]                     ;

    // Group 040 : SDCTRL0
    UINT32  G040_SDCTRL0[32]                      ;

    // Group 041 : SDCTRL0
    UINT32  G041_SDCTRL0[32]                      ;

    // Group 042 : SDCTRL0
    UINT32  G042_SDCTRL0[32]                      ;

    // Group 043 : Reserved
    UINT32  G043_RESERVED[32]                     ;
    
    // Group 044 : Reserved
    UINT32  G044_RESERVED[32]                     ;
    
    // Group 045 : Reserved
    UINT32  G045_RESERVED[32]                     ;

    //Group 046 : CEVA DSP
    UINT32  dma0_sram_addr                        ; // 00  
    UINT32  dma0_dram_addr                        ; // 01  
    UINT32  dma0_txsize                           ; // 02  
    UINT32  dma0_control                          ; // 03  
    UINT32  dma0_status                           ; // 04  
    UINT32  dma1_sram_addr                        ; // 05  
    UINT32  dma1_dram_addr                        ; // 06  
    UINT32  dma1_txsize                           ; // 07  
    UINT32  dma1_control                          ; // 08  
    UINT32  dma1_status                           ; // 09  
    UINT32  dma2_sram_addr                        ; // 10  
    UINT32  dma2_dram_addr                        ; // 11  
    UINT32  dma2_txsize                           ; // 12  
    UINT32  dma2_control                          ; // 13  
    UINT32  dma2_status                           ; // 14  
    UINT32  g56_reserved15                        ; // 15  
    UINT32  dsp_inst_page_sw                      ; // 16  
    UINT32  dsp_inst_page0                        ; // 17  
    UINT32  dsp_inst_page1                        ; // 18  
    UINT32  dsp_inst_page2                        ; // 19  
    UINT32  dsp_inst_page3                        ; // 20  
    UINT32  dsp_inst_page4                        ; // 21  
    UINT32  dsp_inst_page5                        ; // 22  
    UINT32  dsp_inst_page6                        ; // 23  
    UINT32  dsp_inst_page7                        ; // 24  
    UINT32  dsp_data_page                         ; // 25  
    UINT32  dsp_rom_page                          ; // 26  
    UINT32  g56_reserved27                        ; // 27  
    UINT32  g56_reserved28                        ; // 28  
    UINT32  g56_reserved29                        ; // 29  
    UINT32  g56_reserved30                        ; // 30  
    UINT32  g56_reserved31                        ; // 31  

    //Group 047 : CEVA DSP
    UINT32  rdif_ctrl                             ; // 00  
    UINT32  rdif_int_ctrl                         ; // 01  
    UINT32  rdif_vint_vector                      ; // 02  
    UINT32  rdif_code_key                         ; // 03  
    UINT32  dsp_status                            ; // 04  
    UINT32  rdif_cfg                              ; // 05  
    UINT32  rdif_oh_ctrl                          ; // 06  
    UINT32  g57_reserved7                         ; // 07  
    UINT32  dsp_timer_ctrl                        ; // 08  
    UINT32  dsp_tcount                            ; // 09  
    UINT32  dsp_tperiod                           ; // 10  
    UINT32  dsp_timer_status                      ; // 11  
    UINT32  g57_reserved12                        ; // 12  
    UINT32  g57_reserved13                        ; // 13  
    UINT32  g57_reserved14                        ; // 14  
    UINT32  g57_reserved15                        ; // 15  
    UINT32  g57_reserved16                        ; // 16  
    UINT32  g57_reserved17                        ; // 17  
    UINT32  g57_reserved18                        ; // 18  
    UINT32  g57_reserved19                        ; // 19  
    UINT32  g57_reserved20                        ; // 20  
    UINT32  g57_reserved21                        ; // 21  
    UINT32  g57_reserved22                        ; // 22  
    UINT32  g57_reserved23                        ; // 23  
    UINT32  g57_reserved24                        ; // 24  
    UINT32  g57_reserved25                        ; // 25  
    UINT32  g57_reserved26                        ; // 26  
    UINT32  g57_reserved27                        ; // 27  
    UINT32  g57_reserved28                        ; // 28  
    UINT32  g57_reserved29                        ; // 29  
    UINT32  g57_reserved30                        ; // 30  
    UINT32  g57_reserved31                        ; // 31  

    //Group 048 : CEVA DSP
    UINT32  dsp_monitor_trigger                   ; // 00  
    UINT32  g58_reserved1                         ; // 01  
    UINT32  dsp_cycle                             ; // 02  
    UINT32  dsp_dma_cnt                           ; // 03  
    UINT32  dsp_cache_nseq_cnt                    ; // 04  
    UINT32  dsp_dma_cycle                         ; // 05  
    UINT32  dsp_max_dma_cycle                     ; // 06  
    UINT32  dsp_min_dma_cycle                     ; // 07  
    UINT32  dsp_cache_acc_cycle                   ; // 08  
    UINT32  dsp_dma_status                        ; // 09  
    UINT32  dsp_error                             ; // 10  
    UINT32  g58_reserved11                        ; // 11  
    UINT32  g58_reserved12                        ; // 12  
    UINT32  g58_reserved13                        ; // 13  
    UINT32  g58_reserved14                        ; // 14  
    UINT32  g58_reserved15                        ; // 15  
    UINT32  ocm_inst_reg                          ; // 16  
    UINT32  ocm_cntl_reg                          ; // 17  
    UINT32  ocm_stat_reg                          ; // 18  
    UINT32  g58_reserved19                        ; // 19  
    UINT32  ocm_data_reg0                         ; // 20  
    UINT32  ocm_data_reg1                         ; // 21  
    UINT32  ocm_data_reg2                         ; // 22  
    UINT32  ocm_data_reg3                         ; // 23  
    UINT32  ocm_data_reg4                         ; // 24  
    UINT32  ocm_data_reg5                         ; // 25  
    UINT32  ocm_data_reg6                         ; // 26  
    UINT32  ocm_data_reg7                         ; // 27  
    UINT32  g58_reserved28                        ; // 28  
    UINT32  g58_reserved29                        ; // 29  
    UINT32  g58_reserved30                        ; // 30  
    UINT32  g58_reserved31                        ; // 31  

    //Group 049 : CEVA DSP
    UINT32  dsp_port0                             ; // 00  
    UINT32  dsp_port1                             ; // 01  
    UINT32  dsp_port2                             ; // 02  
    UINT32  dsp_port3                             ; // 03  
    UINT32  dsp_port4                             ; // 04  
    UINT32  dsp_port5                             ; // 05  
    UINT32  dsp_port6                             ; // 06  
    UINT32  dsp_port7                             ; // 07  
    UINT32  dsp_port8                             ; // 08  
    UINT32  dsp_port9                             ; // 09  
    UINT32  dsp_port10                            ; // 10  
    UINT32  dsp_port11                            ; // 11  
    UINT32  dsp_port12                            ; // 12  
    UINT32  dsp_port13                            ; // 13  
    UINT32  dsp_port14                            ; // 14  
    UINT32  dsp_port15                            ; // 15  
    UINT32  dsp_port16                            ; // 16  
    UINT32  dsp_port17                            ; // 17  
    UINT32  dsp_port18                            ; // 18  
    UINT32  dsp_port19                            ; // 19  
    UINT32  dsp_port20                            ; // 20  
    UINT32  dsp_port21                            ; // 21  
    UINT32  dsp_port22                            ; // 22  
    UINT32  dsp_port23                            ; // 23  
    UINT32  dsp_port24                            ; // 24  
    UINT32  dsp_port25                            ; // 25  
    UINT32  dsp_port26                            ; // 26  
    UINT32  dsp_port27                            ; // 27  
    UINT32  dsp_port28                            ; // 28  
    UINT32  dsp_port29                            ; // 29  
    UINT32  dsp_port30                            ; // 30  
    UINT32  dsp_port31                            ; // 31  
    
    // Group 050 : DDR_PHY0
    UINT32  G050_DDR_PHY0[32]                     ;

    // Group 051 : DDR_PHY0
    UINT32  G051_DDR_PHY0[32]                     ;

    // Group 052 : DDR_PHY0
    UINT32  G052_DDR_PHY0[32]                     ;

    // Group 053 : DDR_PHY0
    UINT32  G053_DDR_PHY0[32]                      ;
    
    // Group 054 : Reserved
    UINT32  G054_RESERVED[32]                     ;
    
    // Group 055 : Reserved
    UINT32  G055_DDC0[32]                     ;
    
    // Group 056 : Reserved
    UINT32  G056_RESERVED[32]                     ;
    
    // Group 057 : Reserved
    UINT32  G057_I2CM0[32]                        ;

    // Group 058 : Reserved
    UINT32  G058_I2CM1[32]                        ;

    // Group 059 : Reserved
    UINT32  G059_RESERVED[32]                     ;

    // Group 060 : AUD
    UINT32  audif_ctrl                            ; // 00
    UINT32  aud_enable                            ; // 01
    UINT32  pcm_cfg                               ; // 02
    UINT32  i2s_mute_flag_ctrl                    ; // 03
    UINT32  ext_adc_cfg                           ; // 04
    UINT32  int_dac_ctrl0                         ; // 05
    UINT32  int_adc_ctrl                          ; // 06
    UINT32  adc_in_path_switch                    ; // 07
    UINT32  int_adc_dac_cfg                       ; // 08
    UINT32  G060_reserved_9                       ; // 09
    UINT32  iec_cfg                               ; // 10
    UINT32  iec0_valid_out                        ; // 11
    UINT32  iec0_par0_out                         ; // 12
    UINT32  iec0_par1_out                         ; // 13
    UINT32  iec1_valid_out                        ; // 14
    UINT32  iec1_par0_out                         ; // 15
    UINT32  iec1_par1_out                         ; // 16
    UINT32  iec0_rx_debug_info                    ; // 17
    UINT32  iec0_valid_in                         ; // 18
    UINT32  iec0_par0_in                          ; // 19
    UINT32  iec0_par1_in                          ; // 20
    UINT32  iec1_rx_debug_info                    ; // 21
    UINT32  iec1_valid_in                         ; // 22
    UINT32  iec1_par0_in                          ; // 23
    UINT32  iec1_par1_in                          ; // 24
    UINT32  iec2_rx_debug_info                    ; // 25
    UINT32  iec2_valid_in                         ; // 26
    UINT32  iec2_par0_in                          ; // 27
    UINT32  iec2_par1_in                          ; // 28
    UINT32  G060_reserved_29                      ; // 29
    UINT32  iec_tx_user_wdata                     ; // 30
    UINT32  iec_tx_user_ctrl                      ; // 31

    // Group 061 : AUD
    UINT32  adcp_ch_enable                        ; // 00, ADCPRC Configuration Group 1  
    UINT32  adcp_fubypass                         ; // 01, ADCPRC Configuration Group 2  
    UINT32  adcp_mode_ctrl                        ; // 02, ADCPRC Mode Control  
    UINT32  adcp_init_ctrl                        ; // 03, ADCP Initialization Control  
    UINT32  adcp_coeff_din                        ; // 04, Coefficient Data Input  
    UINT32  adcp_agc_cfg                          ; // 05, ADCPRC AGC Configuration of Ch0/1  
    UINT32  adcp_agc_cfg2                         ; // 06, ADCPRC AGC Configuration of Ch2/3  
    UINT32  adcp_gain_0                           ; // 07, ADCP System Gain1
    UINT32  adcp_gain_1                           ; // 08, ADCP System Gain2
    UINT32  adcp_gain_2                           ; // 09, ADCP System Gain3
    UINT32  adcp_gain_3                           ; // 10, ADCP System Gain4
    UINT32  adcp_risc_gain                        ; // 11, ADCP RISC Gain  
    UINT32  adcp_mic_l                            ; // 12, ADCPRC Microphone - in Left Channel Data  
    UINT32  adcp_mic_r                            ; // 13, ADCPRC Microphone - in Right Channel Data  
    UINT32  adcp_agc_gain                         ; // 14, ADCPRC AGC Gain  
    UINT32  G061_reserved_15                      ; // 15, Reserved  
    UINT32  aud_apt_mode                          ; // 16, Audio Playback Timer Mode  
    UINT32  aud_apt_data                          ; // 17, Audio Playback Timer  
    UINT32  aud_apt_parameter                     ; // 18, Audio Playback Timer Parameter  
    UINT32  G061_reserved_19                      ; // 19, Reserved  
    UINT32  aud_audhwya                           ; // 20, DRAM Base Address Offset  
    UINT32  aud_inc_0                             ; // 21, DMA Counter Increment/Decrement  
    UINT32  aud_delta_0                           ; // 22, Delta Value  
    UINT32  aud_fifo_enable                       ; // 23, Audio FIFO Enable  
    UINT32  aud_fifo_mode                         ; // 24, FIFO Mode Control  
    UINT32  aud_fifo_support                      ; // 25, Supported FIFOs ( Debug Function )  
    UINT32  aud_fifo_reset                        ; // 26, Host FIFO Reset  
    UINT32  aud_chk_ctrl                          ; // 27, Checksum Control ( Debug Function )  
    UINT32  aud_checksum_data                     ; // 28, Checksum Data ( Debug Function )  
    UINT32  aud_chk_tcnt                          ; // 29, Target Count of Checksum ( Debug Function )  
    UINT32  aud_embedded_input_ctrl               ; // 30, Embedded Input Control ( Debug Function )  
    UINT32  aud_misc_ctrl                         ; // 31, Miscellaneous Control  

    // Group 062 : AUD
    UINT32  aud_ext_dac_xck_cfg                   ; // 00
    UINT32  aud_ext_dac_bck_cfg                   ; // 01
    UINT32  aud_iec0_bclk_cfg                     ; // 02
    UINT32  aud_ext_adc_xck_cfg                   ; // 03
    UINT32  aud_ext_adc_bck_cfg                   ; // 04
    UINT32  aud_int_adc_xck_cfg                   ; // 05
    UINT32  G062_reserved_6                       ; // 06
    UINT32  aud_int_dac_xck_cfg                   ; // 07
    UINT32  aud_int_dac_bck_cfg                   ; // 08
    UINT32  aud_iec1_bclk_cfg                     ; // 09
    UINT32  G062_reserved_10                      ; // 10
    UINT32  aud_pcm_iec_bclk_cfg                  ; // 11
    UINT32  aud_xck_osr104_cfg                    ; // 12
    UINT32  aud_hdmi_tx_mclk_cfg                  ; // 13
    UINT32  aud_hdmi_tx_bck_cfg                   ; // 14
    UINT32  hdmi_tx_i2s_cfg                       ; // 15
    UINT32  hdmi_rx_i2s_cfg                       ; // 16
    UINT32  G062_reserved_17                      ; // 17
    UINT32  G062_reserved_18                      ; // 18
    UINT32  G062_reserved_19                      ; // 19
    UINT32  G062_reserved_20                      ; // 20
    UINT32  int_adc_ctrl3                      	  ; // 21
    UINT32  int_adc_ctrl2                         ; // 22
    UINT32  G062_reserved_23                      ; // 23
    UINT32  G062_reserved_24                      ; // 24
    UINT32  int_dac_ctrl1                         ; // 25
    UINT32  G062_reserved_26                      ; // 26
    UINT32  G062_reserved_27                      ; // 27
    UINT32  G062_reserved_28                      ; // 28
    UINT32  G062_reserved_29                      ; // 29
    UINT32  G062_reserved_30                      ; // 30
    UINT32  G062_reserved_31                      ; // 31

    // Group 063 : AUD
    UINT32  aud_bt_ifx_cfg                        ; // 00
    UINT32  aud_bt_i2s_cfg                        ; // 01
    UINT32  aud_bt_xck_cfg                        ; // 02
    UINT32  aud_bt_bck_cfg                        ; // 03
    UINT32  aud_bt_sync_cfg                       ; // 04
    UINT32  G063_reserved_5                       ; // 05
    UINT32  G063_reserved_6                       ; // 06
    UINT32  G063_reserved_7                       ; // 07
    UINT32  G063_reserved_8                       ; // 08
    UINT32  G063_reserved_9                       ; // 09
    UINT32  G063_reserved_10                      ; // 10
    UINT32  G063_reserved_11                      ; // 11
    UINT32  G063_reserved_12                      ; // 12
    UINT32  G063_reserved_13                      ; // 13
    UINT32  G063_reserved_14                      ; // 14
    UINT32  G063_reserved_15                      ; // 15
    UINT32  G063_reserved_16                      ; // 16
    UINT32  G063_reserved_17                      ; // 17
    UINT32  G063_reserved_18                      ; // 18
    UINT32  G063_reserved_19                      ; // 19
    UINT32  G063_reserved_20                      ; // 20
    UINT32  G063_reserved_21                      ; // 21
    UINT32  G063_reserved_22                      ; // 22
    UINT32  G063_reserved_23                      ; // 23
    UINT32  aud_opt_test_pat                      ; // 24
    UINT32  aud_sys_status0                       ; // 25
    UINT32  aud_sys_status1                       ; // 26
    UINT32  int_adc_ctrl1                         ; // 27
    UINT32  bt_mute_flag                          ; // 28
    UINT32  G063_reserved_29                      ; // 29
    UINT32  G063_reserved_30                      ; // 30
    UINT32  other_config                          ; // 31

    // Group 064 : AUD
    UINT32  aud_a0_base                           ; // 
    UINT32  aud_a0_length                         ; // 
    UINT32  aud_a0_ptr                            ; // 
    UINT32  aud_a0_cnt                            ; // 
    UINT32  aud_a1_base                           ; // 
    UINT32  aud_a1_length                         ; // 
    UINT32  aud_a1_ptr                            ; // 
    UINT32  aud_a1_cnt                            ; // 
    UINT32  aud_a2_base                           ; // 
    UINT32  aud_a2_length                         ; // 
    UINT32  aud_a2_ptr                            ; // 
    UINT32  aud_a2_cnt                            ; // 
    UINT32  aud_a3_base                           ; // 
    UINT32  aud_a3_length                         ; // 
    UINT32  aud_a3_ptr                            ; // 
    UINT32  aud_a3_cnt                            ; // 
    UINT32  aud_a4_base                           ; // 
    UINT32  aud_a4_length                         ; // 
    UINT32  aud_a4_ptr                            ; // 
    UINT32  aud_a4_cnt                            ; // 
    UINT32  aud_a5_base                           ; // 
    UINT32  aud_a5_length                         ; // 
    UINT32  aud_a5_ptr                            ; // 
    UINT32  aud_a5_cnt                            ; // 
    UINT32  aud_a6_base                           ; // 
    UINT32  aud_a6_length                         ; // 
    UINT32  aud_a6_ptr                            ; // 
    UINT32  aud_a6_cnt                            ; // 
    UINT32  aud_a7_base                           ; // 
    UINT32  aud_a7_length                         ; // 
    UINT32  aud_a7_ptr                            ; // 
    UINT32  aud_a7_cnt                            ; // 

    // Group 065 : AUD
    UINT32  aud_a8_base                           ; //
    UINT32  aud_a8_length                         ; //
    UINT32  aud_a8_ptr                            ; //
    UINT32  aud_a8_cnt                            ; //
    UINT32  aud_a9_base                           ; //
    UINT32  aud_a9_length                         ; //
    UINT32  aud_a9_ptr                            ; //
    UINT32  aud_a9_cnt                            ; //
    UINT32  aud_a10_base                          ; //
    UINT32  aud_a10_length                        ; //
    UINT32  aud_a10_ptr                           ; //
    UINT32  aud_a10_cnt                           ; //
    UINT32  aud_a11_base                          ; //
    UINT32  aud_a11_length                        ; //
    UINT32  aud_a11_ptr                           ; //
    UINT32  aud_a11_cnt                           ; //
    UINT32  aud_a12_base                          ; //
    UINT32  aud_a12_length                        ; //
    UINT32  aud_a12_ptr                           ; //
    UINT32  aud_a12_cnt                           ; //
    UINT32  aud_a13_base                          ; //
    UINT32  aud_a13_length                        ; //
    UINT32  aud_a13_ptr                           ; //
    UINT32  aud_a13_cnt                           ; //
    UINT32  aud_a14_base                          ; //
    UINT32  aud_a14_length                        ; //
    UINT32  aud_a14_ptr                           ; //
    UINT32  aud_a14_cnt                           ; //
    UINT32  aud_a15_base                          ; //
    UINT32  aud_a15_length                        ; //
    UINT32  aud_a15_ptr                           ; //
    UINT32  aud_a15_cnt                           ; //


    // Group 066 : AUD
    UINT32  aud_a16_base                          ; //
    UINT32  aud_a16_length                        ; //
    UINT32  aud_a16_ptr                           ; //
    UINT32  aud_a16_cnt                           ; //
    UINT32  aud_a17_base                          ; //
    UINT32  aud_a17_length                        ; //
    UINT32  aud_a17_ptr                           ; //
    UINT32  aud_a17_cnt                           ; //
    UINT32  aud_a18_base                          ; //
    UINT32  aud_a18_length                        ; //
    UINT32  aud_a18_ptr                           ; //
    UINT32  aud_a18_cnt                           ; //
    UINT32  aud_a19_base                          ; //
    UINT32  aud_a19_length                        ; //
    UINT32  aud_a19_ptr                           ; //
    UINT32  aud_a19_cnt                           ; //
    UINT32  G066_reserved_16                      ; //
    UINT32  G066_reserved_17                      ; //
    UINT32  G066_reserved_18                      ; //
    UINT32  G066_reserved_19                      ; //
    UINT32  G066_reserved_20                      ; //
    UINT32  G066_reserved_21                      ; //
    UINT32  G066_reserved_22                      ; //
    UINT32  G066_reserved_23                      ; //
    UINT32  G066_reserved_24                      ; //
    UINT32  G066_reserved_25                      ; //
    UINT32  G066_reserved_26                      ; //
    UINT32  G066_reserved_27                      ; //
    UINT32  G066_reserved_28                      ; //
    UINT32  G066_reserved_29                      ; //
    UINT32  G066_reserved_30                      ; //
    UINT32  G066_reserved_31                      ; //

    // Group 067 : AUD
    UINT32  aud_grm_master_gain                   ; // Gain Control  
    UINT32  aud_grm_gain_control_0                ; // Gain Control  
    UINT32  aud_grm_gain_control_1                ; // Gain Control  
    UINT32  aud_grm_gain_control_2                ; // Gain Control  
    UINT32  aud_grm_gain_control_3                ; // Gain Control  
    UINT32  aud_grm_gain_control_4                ; // Gain Control  
    UINT32  aud_grm_mix_control_0                 ; // Mixer Setting  
    UINT32  aud_grm_mix_control_1                 ; // Mixer Setting  
    UINT32  aud_grm_mix_control_2                 ; // Mixer Setting  
    UINT32  aud_grm_switch_0                      ; // Channel Switch  
    UINT32  aud_grm_switch_1                      ; // Channel Switch  
    UINT32  aud_grm_switch_int                    ; // Channel Switch  
    UINT32  aud_grm_delta_volume                  ; // Gain Update  
    UINT32  aud_grm_delta_ramp_pcm                ; // Gain Update  
    UINT32  aud_grm_delta_ramp_risc               ; // Gain Update  
    UINT32  aud_grm_delta_ramp_linein             ; // Gain Update  
    UINT32  aud_grm_other                         ; // Other Setting  
    UINT32  aud_grm_gain_control_5                ; // Gain Control  
    UINT32  aud_grm_gain_control_6                ; // Gain Control  
    UINT32  aud_grm_gain_control_7                ; // Gain Control  
    UINT32  aud_grm_gain_control_8                ; // Gain Control  
    UINT32  aud_grm_fifo_eflag                    ; // FIFO Error Flag  
    UINT32  G067_reserved_22                      ; // 
    UINT32  G067_reserved_23                      ; // 
    UINT32  aud_grm_switch_hdmi_tx                ; // AUD_GRM_SWITCH_HDMI_TX
    UINT32  G067_reserved_25                      ; // 
    UINT32  G067_reserved_26                      ; // 
    UINT32  G067_reserved_27                      ; // 
    UINT32  G067_reserved_28                      ; // 
    UINT32  G067_reserved_29                      ; // 
    UINT32  G067_reserved_30                      ; // 
    UINT32  G067_reserved_31                      ; // 

    // Group 068 : AUD
    UINT32  G068_AUD[32]                          ;

    // Group 069 : Reserved
    UINT32  G069_RESERVED[32]                     ;

    // Group 070 : Reserved
    UINT32  G070_RESERVED[32]                     ;

    // Group 071 : Reserved
    UINT32  G071_RESERVED[32]                     ;

    // Group 072 : Reserved
    UINT32  G072_RESERVED[32]                     ;

    // Group 073 : Reserved
    UINT32  G073_RESERVED[32]                     ;

    // Group 074 : Reserved
    UINT32  G074_RESERVED[32]                     ;

    // Group 075 : Reserved
    UINT32  G075_RESERVED[32]                     ;

    // Group 076 : Reserved
    UINT32  G076_RESERVED[32]                     ;

    // Group 077 : Reserved
    UINT32  G077_RESERVED[32]                     ;

    // Group 078 : Reserved
    UINT32  G078_RESERVED[32]                     ;

    // Group 079 : Reserved
    UINT32  G079_RESERVED[32]                     ;

    // Group 080 : Reserved
    UINT32  G080_RESERVED[32]                     ;

    // Group 081 : Reserved
    UINT32  G081_RESERVED[32]                     ;

    // Group 082 : Reserved
    UINT32  G082_RESERVED[32]                     ;

    // Group 083 : SECGRP1
    UINT32  G083_SECGRP1[32]                      ;

    // Group 084 : MB0_SD0_WP
    UINT32  G084_MB0_SD0_WP[32]                   ;

    // Group 085 : MB1_SD1_WP
    UINT32  G085_MB1_SD1_WP[32]                   ;

    // Group 086 : Reserved
    UINT32  G086_RESERVED[32]                     ;

    // Group 087 : Reserved
    UINT32  spi_nand_ctrl                         ;
    UINT32  spi_nand_wait                         ;
    UINT32  spi_nand_cust_cmd                     ;
    UINT32  spi_nand_addr_low                     ;
    UINT32  spi_nand_addr_high                    ;
    UINT32  spi_nand_data_low                     ;
    UINT32  spi_nand_data_high                    ;
    UINT32  spi_nand_status                       ;
    UINT32  spi_nand_cfg0                         ;
    UINT32  spi_nand_cfg1                         ;
    UINT32  spi_nand_cfg2                         ;
    UINT32  spi_nand_cfg3                         ;
    UINT32  spi_nand_cfg4                         ;
    UINT32  spi_nand_cfg5                         ;
    UINT32  spi_nand_cfg6                         ;
    UINT32  spi_nand_cfg7                         ;
    UINT32  spi_nand_cfg8                         ;
    UINT32  spi_nand_cust_cmd2                    ;
    UINT32  spi_nand_data64                       ;
    UINT32  spi_nand_buf_addr                     ;
    UINT32  spi_nand_status2                      ;
    UINT32  spi_nand_status3                      ;
    UINT32  G087_RESERVED[10]                     ;

    // Group 088 : MIPIRX
    //UINT32  G088_MIPIRX[32]                       ;
    UINT32  mipi_int_status                       ;//0x00 0x00 
    UINT32  mipi_probesel                         ;//0x01 0x04
    UINT32  mipi_lvds_wordcnt                     ;//0x02 0x08
    UINT32  mipi_lvds_test                        ;//0x03 0x0c
    UINT32  mipi_lvds_reset                       ;//0x04 0x10    
    UINT32  mipi_fsm_rsten                        ;//0x05 0x14  
    UINT32  G088_RESERVED6                        ;//0x06 0x18  
    UINT32  mipi_lvdsen                           ;//0x07 0x1c  
    UINT32  mipi_digital_logic_ctrl               ;//0x08 0x20  
    UINT32  mipi_lvds_timing_ctrl                 ;//0x09 0x24  
    UINT32  mipi_image_datanum                    ;//0x0a 0x28  
    UINT32  mipi_syncword                         ;//0x0b 0x2c  
    UINT32  mipi_lvds_ctrlword_start              ;//0x0c 0x30  
    UINT32  mipi_lvds_ctrlword_end                ;//0x0d 0x34  
    UINT32  mipi_sel_lvcmos                       ;//0x0e 0x38  
    UINT32  G088_RESERVED15                       ;//0x0f 0x3c  
    UINT32  mipi_ecc_evt                          ;//0x10 0x40  
    UINT32  mipi_crc_evt                          ;//0x11 0x44  
    UINT32  mipi_ecc_ctrl                         ;//0x12 0x48  
    UINT32  G088_RESERVED19                       ;//0x13 0x4c  
    UINT32  G088_RESERVED20                       ;//0x14 0x50  
    UINT32  G088_RESERVED21                       ;//0x15 0x54  
    UINT32  G088_RESERVED22                       ;//0x16 0x58  
    UINT32  G088_RESERVED23                       ;//0x17 0x5c  
    UINT32  G088_RESERVED24                       ;//0x18 0x60  
    UINT32  G088_RESERVED25                       ;//0x19 0x64  
    UINT32  G088_RESERVED26                       ;//0x1a 0x68  
    UINT32  G088_RESERVED27                       ;//0x1b 0x6c  
    UINT32  G088_RESERVED28                       ;//0x1c 0x70  
    UINT32  G088_RESERVED29                       ;//0x1d 0x74  
    UINT32  G088_RESERVED30                       ;//0x1e 0x78  
    UINT32  G088_RESERVED31                       ;//0x1f 0x7c  


    // Group 089 : Reserved
    UINT32  G089_RESERVED[32]                     ;

    // Group 090 : SPI_TOUCH
    UINT32  TOU_TX_DATA_2_1_0_ADDR                ; // 00  (ADDR : 0x9C00_2D00)
    UINT32  TOU_TX_DATA_6_5_4_3                   ; // 01  (ADDR : 0x9C00_2D04)
    UINT32  TOU_TX_DATA_10_9_8_7                  ; // 02  (ADDR : 0x9C00_2D08)
    UINT32  TOU_TX_DATA_14_13_12_11               ; // 03  (ADDR : 0x9C00_2D0C)
    UINT32  TOU_TX_DATA_15                        ; // 04  (ADDR : 0x9C00_2D10)
    UINT32  G090_RESERVED_0[4]                    ; //     (ADDR : 0x9C00_2D14) ~ (ADDR : 0x9C00_2D20)
    UINT32  TOU_RX_DATA_3_2_1_0                   ; // 09  (ADDR : 0x9C00_2D24)
    UINT32  TOU_RX_DATA_7_6_5_4                   ; // 10  (ADDR : 0x9C00_2D28)
    UINT32  TOU_RX_DATA_11_10_9_8                 ; // 11  (ADDR : 0x9C00_2D2C)
    UINT32  TOU_RX_DATA_15_14_13_12               ; // 12  (ADDR : 0x9C00_2D30)
    UINT32  G090_RESERVED_1[4]                    ; //     (ADDR : 0x9C00_2D34) ~ (ADDR : 0x9C00_2D40)
    UINT32  TOU_SPI_CTRL_CLKSEL                   ; // 17  (ADDR : 0x9C00_2D44)
    UINT32  TOU_BYTE_NO                           ; // 18  (ADDR : 0x9C00_2D48)
    UINT32  TOU_SPI_INT_BUSY                      ; // 19  (ADDR : 0x9C00_2D4C)  
    UINT32  G090_RESERVED_2[12]                   ; // reserved ~0x9C00_2D7C

    // Group 091 : SPI_MASTER
    UINT32  MST_TX_DATA_2_1_0_ADDR                ; // 00  (ADDR : 0x9C00_2D80)
    UINT32  MST_TX_DATA_6_5_4_3                   ; // 01  (ADDR : 0x9C00_2D84)
    UINT32  MST_TX_DATA_10_9_8_7                  ; // 02  (ADDR : 0x9C00_2D88)
    UINT32  MST_TX_DATA_14_13_12_11               ; // 03  (ADDR : 0x9C00_2D8C)
    UINT32  MST_TX_DATA_15                        ; // 04  (ADDR : 0x9C00_2D90)
    UINT32  G091_RESERVED_0[4]                    ; //     (ADDR : 0x9C00_2D94) ~ (ADDR : 0x9C00_2DA0)
    UINT32  MST_RX_DATA_3_2_1_0                   ; // 09  (ADDR : 0x9C00_2DA4)
    UINT32  MST_RX_DATA_7_6_5_4                   ; // 10  (ADDR : 0x9C00_2DA8)
    UINT32  MST_RX_DATA_11_10_9_8                 ; // 11  (ADDR : 0x9C00_2DAC)
    UINT32  MST_RX_DATA_15_14_13_12               ; // 12  (ADDR : 0x9C00_2DB0)
    UINT32  G091_RESERVED_1[4]                    ; //     (ADDR : 0x9C00_2DB4) ~ (ADDR : 0x9C00_2DC0)
    UINT32  SPI_CTRL_CLKSEL                       ; // 17  (ADDR : 0x9C00_2DC4)
    UINT32  BYTE_NO                               ; // 18  (ADDR : 0x9C00_2DC8)
    UINT32  SPI_INT_BUSY                          ; // 19  (ADDR : 0x9C00_2DCC)
    UINT32  DMA_CTRL                              ; // 20  (ADDR : 0x9C00_2DD0)
    UINT32  DMA_LENGTH                            ; // 21  (ADDR : 0x9C00_2DD4)
    UINT32  DMA_ADDR                              ; // 22  (ADDR : 0x9C00_2DD8)
    UINT32  G091_RESERVED_2[1]                    ; // 23  (ADDR : 0x9C00_2DDC)
    UINT32  DMA_ADDR_STAT                         ; // 24  (ADDR : 0x9C00_2DE0)
    UINT32  G091_RESERVED_3[1]                    ; // 25  (ADDR : 0x9C00_2DE4)
    UINT32  UART_DMA_CTRL                         ; // 26  (ADDR : 0x9C00_2DE8)
    UINT32  G091_RESERVED_4[1]                    ; // 27  (ADDR : 0x9C00_2DEC)
    UINT32  G091_RESERVED_5[2]                    ; //     (ADDR : 0x9C00_2DF4) ~ (ADDR : 0x9C00_2DF4)
    UINT32  SPI_EXTRA_CYCLE                       ; // 30  (ADDR : 0x9C00_2DF8)
    UINT32  MST_DMA_DATA_RDY                      ; // 31  (ADDR : 0x9C00_2DFC)

    // Group 092 : SPI_SLAVE
    UINT32  SLV_TX_DATA_2_1_0                     ; // 00  (ADDR : 0x9C00_2E00) 
    UINT32  SLV_TX_DATA_6_5_4_3                   ; // 01  (ADDR : 0x9C00_2E04)
    UINT32  SLV_TX_DATA_10_9_8_7                  ; // 02  (ADDR : 0x9C00_2E08)
    UINT32  SLV_TX_DATA_14_13_12_11               ; // 03  (ADDR : 0x9C00_2E0C)
    UINT32  SLV_TX_DATA_15                        ; // 04  (ADDR : 0x9C00_2E10)
    UINT32  G092_RESERVED_0[4]                    ; //     (ADDR : 0x9C00_2E14) ~ (ADDR : 0x9C00_2E20)
    UINT32  SLV_RX_DATA_3_2_1_0                   ; // 09  (ADDR : 0x9C00_2E24)
    UINT32  SLV_RX_DATA_7_6_5_4                   ; // 10  (ADDR : 0x9C00_2E28)
    UINT32  SLV_RX_DATA_11_10_9_8                 ; // 11  (ADDR : 0x9C00_2E2C)
    UINT32  SLV_RX_DATA_15_14_13_12               ; // 12  (ADDR : 0x9C00_2E30)
    UINT32  G092_RESERVED_1[4]                    ; //     (ADDR : 0x9C00_2E34) ~ (ADDR : 0x9C00_2E40)
    UINT32  RISC_INT_DATA_RDY                     ; // 17  (ADDR : 0x9C00_2E44)
    UINT32  SLV_DMA                               ; // 18  (ADDR : 0x9C00_2E48)
    UINT32  SLV_DMA_LENGTH                        ; // 19  (ADDR : 0x9C00_2E4C)
    UINT32  SLV_DMA_INI_ADDR                      ; // 20  (ADDR : 0x9C00_2E50)
    UINT32  G092_RESERVED_2[2]                    ; //     (ADDR : 0x9C00_2E54) ~ (ADDR : 0x9C00_2E58)
    UINT32  ADDR_SPI_BUSY                         ; // 23  (ADDR : 0x9C00_2E5C)
    UINT32  G092_RESERVED_3[8]                    ; //     (ADDR : 0x9C00_2E60) ~ (ADDR : 0x9C00_2E7C)

    // Group 093 : Reserved
    UINT32  G093_RESERVED[32]                     ;

    // Group 094 : TOUCH CONTROLLER
    UINT32 touch_cfg00;
    UINT32 touch_cfg01;
    UINT32 touch_cfg02;
    UINT32 touch_cfg03;
    UINT32 touch_cfg04;
    UINT32 touch_cfg05;
    UINT32 touch_cfg06;
    UINT32 touch_cfg07;
    UINT32 touch_cfg08;
    UINT32 touch_cfg09;
    UINT32 touch_cfg0a;
    UINT32 touch_cfg0b;
    UINT32 touch_cfg0c;
    UINT32 touch_cfg0d;
    UINT32 touch_cfg0e;
    UINT32 touch_cfg0f;
    UINT32 touch_cfg10;
    UINT32 touch_cfg11;
    UINT32 touch_cfg12;
    UINT32 touch_cfg13;
    UINT32 touch_cfg14;
    UINT32 touch_cfg15;
    UINT32 touch_cfg16;
    UINT32 touch_cfg17;
    UINT32 touch_cfg18;
    UINT32 touch_cfg19;
    UINT32 touch_cfg1a;
    UINT32 touch_cfg1b;
    UINT32 touch_cfg1c;
    UINT32 touch_cfg1d;
    UINT32 touch_cfg1e;
    UINT32 touch_cfg1f;

    // Group 095 : Reserved
    UINT32  G095_RESERVED[32]                     ;

    // Group 096 : STC AV0
    UINT32	stc_15_0_av0;       
    UINT32	stc_31_16_av0;      
    UINT32	stc_64_av0;         
    UINT32	stc_divisor_av0;    
    UINT32	rtc_15_0_av0;       
    UINT32	rtc_31_16_av0;      
    UINT32	rtc_divisor_av0;    
    UINT32	stc_config_av0;     
    UINT32	timer0_ctrl_av0;    
    UINT32	timer0_cnt_av0;     
    UINT32	timer1_ctrl_av0;    
    UINT32	timer1_cnt_av0;     
    UINT32	timerw_ctrl_av0;    
    UINT32	timerw_cnt_av0;     
    UINT32	stc_47_32_av0;      
    UINT32	stc_63_48_av0;      
    UINT32	timer2_ctrl_av0;    
    UINT32	timer2_divisor_av0; 
    UINT32	timer2_reload_av0;  
    UINT32	timer2_cnt_av0;     
    UINT32	timer3_ctrl_av0;    
    UINT32	timer3_divisor_av0; 
    UINT32	timer3_reload_av0;  
    UINT32	timer3_cnt_av0;     
    UINT32	stcl_0_av0;         
    UINT32	stcl_1_av0;         
    UINT32	stcl_2_av0;         
    UINT32	atc_0_av0;          
    UINT32	atc_1_av0;          
    UINT32	atc_2_av0;          
    UINT32	g12_reserved_av0[2];

    // Group 097 : STC AV1
    UINT32	stc_15_0_av1;       
    UINT32	stc_31_16_av1;      
    UINT32	stc_64_av1;         
    UINT32	stc_divisor_av1;    
    UINT32	rtc_15_0_av1;       
    UINT32	rtc_31_16_av1;      
    UINT32	rtc_divisor_av1;    
    UINT32	stc_config_av1;     
    UINT32	timer0_ctrl_av1;    
    UINT32	timer0_cnt_av1;     
    UINT32	timer1_ctrl_av1;    
    UINT32	timer1_cnt_av1;     
    UINT32	timerw_ctrl_av1;    
    UINT32	timerw_cnt_av1;     
    UINT32	stc_47_32_av1;      
    UINT32	stc_63_48_av1;      
    UINT32	timer2_ctrl_av1;    
    UINT32	timer2_divisor_av1; 
    UINT32	timer2_reload_av1;  
    UINT32	timer2_cnt_av1;     
    UINT32	timer3_ctrl_av1;    
    UINT32	timer3_divisor_av1; 
    UINT32	timer3_reload_av1;  
    UINT32	timer3_cnt_av1;     
    UINT32	stcl_0_av1;         
    UINT32	stcl_1_av1;         
    UINT32	stcl_2_av1;         
    UINT32	atc_0_av1;          
    UINT32	atc_1_av1;          
    UINT32	atc_2_av1;          
    UINT32	g12_reserved_av1[2];

    // Group 098 : PMC
    UINT32	PMC_TIMER;  
    UINT32	PMC_CTRL;  
    UINT32	PMC_XTAL27M_PWD_A; 
    UINT32	PMC_XTAL27M_PWD_B; 
    UINT32	PMC_XTAL32K_PWD_A; 
    UINT32	PMC_XTAL32K_PWD_B; 
    UINT32	PMC_CLK27M_PWD_A; 
    UINT32	PMC_CLK27M_PWD_B; 
    UINT32	PMC_TIMER2; 
    UINT32	G098_RESERVED[23]; 

    // Group 099 : STC AV2
    UINT32      stc_15_0_av2                          ; // 00
    UINT32      stc_31_16_av2                         ; // 01
    UINT32      stc_64_av2                            ; // 02
    UINT32      stc_divisor_av2                       ; // 03
    UINT32      rtc_15_0_av2                          ; // 04
    UINT32      rtc_31_16_av2                         ; // 05
    UINT32      rtc_divisor_av2                       ; // 06
    UINT32      stc_config_av2                        ; // 07
    UINT32      timer0_ctrl_av2                       ; // 08
    UINT32      timer0_cnt_av2                        ; // 09
    UINT32      timer1_ctrl_av2                       ; // 10
    UINT32      timer1_cnt_av2                        ; // 11
    UINT32      timerw_ctrl_av2                       ; // 12
    UINT32      timerw_cnt_av2                        ; // 13
    UINT32	stc_47_32_av2                         ; // 14
    UINT32	stc_63_48_av2                         ; // 15
    UINT32      timer2_ctrl_av2                       ; // 16
    UINT32      timer2_divisor_av2                    ; // 17
    UINT32      timer2_reload_av2                     ; // 18
    UINT32      timer2_cnt_av2                        ; // 19
    UINT32      timer3_ctrl_av2                       ; // 20
    UINT32      timer3_divisor_av2                    ; // 21
    UINT32      timer3_reload_av2                     ; // 22
    UINT32      timer3_cnt_av2                        ; // 23
    UINT32      stcl_0_av2                            ; // 24
    UINT32      stcl_1_av2                            ; // 25
    UINT32      stcl_2_av2                            ; // 26
    UINT32      atc_0_av2                             ; // 27
    UINT32      atc_1_av2                             ; // 28
    UINT32      atc_2_av2                             ; // 29
    UINT32      g12_reserved_av2[2]                   ; // 30 ~ 31

    // Group 100 : Reserved
    UINT32  G100_RESERVED[32]                     ;

    // Group 101 : Reserved
    UINT32  G101_RESERVED[32]                     ;

    // Group 102 : CARD3
    UINT32  G102_CARD_CTL3[32]                    ;

    // Group 103 : CARD3
    UINT32  G103_CARD_CTL3[32]                    ;

    // Group 104 : CARD3
    UINT32  G104_CARD_CTL3[32]                    ;

    // Group 105 : CARD3
    UINT32  G105_CARD_CTL3[32]                    ;

    // Group 106 : CARD3
    UINT32  G106_CARD_CTL3[32]                    ;

    // Group 107 : GPU
    UINT32  G107_GPU[32]                     ;

    // Group 108 : FONT_RENDER
    UINT32  font_control                          ;
    UINT32  font_cmd_fifo                         ;
    UINT32  font_cmd_flush                        ;
    UINT32  font_fifo_addr                        ;
    UINT32  font_fifo_w_ptr                       ;
    UINT32  font_fifo_r_ptr                       ;
    UINT32  font_status                           ;
    UINT32  font_debug_status                     ;
    UINT32  font_fifo_empty_cnt                   ;
    UINT32  G108_FONT_RENDER[23]                  ;

    // Group 109 : IMG_DEC/HUF
    UINT32  jpeg_huff_mode                        ;
    UINT32  jpeg_huff_ctrl                        ;
    UINT32  jpeg_huff_stats                       ;
    UINT32  jpeg_huff_scan_ctrl                   ;
    UINT32  jpeg_huff_table                       ;
    UINT32  jpeg_huff_mcuno                       ;
    UINT32  jpeg_huff_jpeghsize                   ;
    UINT32  jpeg_huff_jpegvsize                   ;
    UINT32  jpeg_huff_bitstream                   ;
    UINT32  jpeg_huff_bits_dma_offset             ;
    UINT32  jpeg_huff_wr_huff_info                ;
    UINT32  jpeg_huff_bits_info                   ;
    UINT32  jpeg_huff_rd_huff_info                ;
    UINT32  jpeg_huff_dma_offset_0                ;
    UINT32  jpeg_huff_dma_offset_1                ;
    UINT32  jpeg_huff_acval_offset_0              ;
    UINT32  jpeg_huff_acval_offset_1              ;
    UINT32  jpeg_huff_acval_offset_2              ;
    UINT32  jpeg_huff_acval_offset_3              ;
    UINT32  jpeg_huff_wr_dc_01                    ;
    UINT32  jpeg_huff_wr_dc_23                    ;
    UINT32  jpeg_huff_wr_acval_0                  ;
    UINT32  jpeg_huff_wr_acval_1                  ;
    UINT32  jpeg_huff_wr_acval_2                  ;
    UINT32  jpeg_huff_wr_acval_3                  ;
    UINT32  jpeg_huff_rd_addr                     ;
    UINT32  jpeg_huff_rd_val                      ;
    UINT32  jpeg_huff_probe                       ;
    UINT32  jr_control                            ;
    UINT32  jr_status                             ;
    UINT32  jr_reg_num                            ;
    UINT32  jr_reg_data                           ;

    // Group 110 : IMG_DEC/JDM
    UINT32  jdm_mode                              ;
    UINT32  jdm_status                            ;
    UINT32  jdm_dma1_mode                         ;
    UINT32  jdm_dma1_xaddr                        ;
    UINT32  jdm_dma1_yaddr                        ;
    UINT32  jdm_dma1_done                         ;
    UINT32  jdm_dma1_start_addr                   ;
    UINT32  jdm_dma1_block_num                    ;
    UINT32  g110_reserved_08                      ;
    UINT32  g110_reserved_09                      ;
    UINT32  g110_reserved_10                      ;
    UINT32  g110_reserved_11                      ;
    UINT32  g110_reserved_12                      ;
    UINT32  g110_reserved_13                      ;
    UINT32  g110_reserved_14                      ;
    UINT32  g110_reserved_15                      ;
    UINT32  g110_reserved_16                      ;
    UINT32  g110_reserved_17                      ;
    UINT32  g110_reserved_18                      ;
    UINT32  g110_reserved_19                      ;
    UINT32  g110_reserved_20                      ;
    UINT32  g110_reserved_21                      ;
    UINT32  jdm_dc_luma_base_lsb                  ;
    UINT32  jdm_dc_luma_base_msb                  ;
    UINT32  jdm_dc_chroma_base_lsb                ;
    UINT32  jdm_dc_chroma_base_msb                ;
    UINT32  jdm_dc_width                          ;
    UINT32  g110_reserved_27                      ;
    UINT32  g110_reserved_28                      ;
    UINT32  jdm_jpeg_offset                       ;
    UINT32  g110_reserved_30                      ;
    UINT32  g110_reserved_31                      ;

    // Group 111 : IMG_DEC/INVQ
    UINT32  jpeg_invq_qmx_par                     ;
    UINT32  jpeg_invq_mode                        ;
    UINT32  jpeg_invq_chksum                      ;
    UINT32  g111_reserved[29]                     ;

    // Group 112 : IMG_DEC/PNG
    UINT32  png_dec_mode                          ;
    UINT32  png_bs_str_addr_m                     ;
    UINT32  png_bs_str_addr_l                     ;
    UINT32  png_bs_end_addr_m                     ;
    UINT32  png_bs_end_addr_l                     ;
    UINT32  png_bs_buf_addr                       ;
    UINT32  png_bs_offset                         ;
    UINT32  png_bs_used_cnt                       ;
    UINT32  png_bs_used_bit                       ;
    UINT32  png_dec_ctrl                          ;
    UINT32  png_img_width                         ;
    UINT32  png_img_height                        ;
    UINT32  png_img_type                          ;
    UINT32  png_chunk_size_m                      ;
    UINT32  png_chunk_size_l                      ;
    UINT32  png_blk_info                          ;
    UINT32  png_huf_tb_addr                       ;
    UINT32  png_huf_tb_ctrl                       ;
    UINT32  png_huf_tb_data                       ;
    UINT32  png_interrupt                         ;
    UINT32  png_scl_ext_addr_m                    ;
    UINT32  png_scl_ext_addr_l                    ;
    UINT32  png_scl_buf_addr                      ;
    UINT32  png_bw_ext_addr_m                     ;
    UINT32  png_bw_ext_addr_l                     ;
    UINT32  png_rem_chunk_size_m                  ;
    UINT32  png_rem_chunk_size_l                  ;
    UINT32  png_pxl_ext_addr_m                    ;
    UINT32  png_pxl_ext_addr_l                    ;
    UINT32  png_pxl_line                          ;
    UINT32  png_probe_addr                        ;
    UINT32  png_probe_value                       ;

    // Group 113 : Reserved
    UINT32  G113_RESERVED[32]                     ;

    // Group 114 : BV
    UINT32  bv_ctrl                               ; //
    UINT32  bv_config                             ; //
    UINT32  bv_interrupt                          ; //
    UINT32  bv_ref_base_0                         ; //
    UINT32  bv_ref_base_1                         ; //
    UINT32  bv_ref_base_2                         ; //
    UINT32  bv_ref_base_3                         ; //
    UINT32  bv_ref_base_4                         ; //
    UINT32  bv_ref_base_5                         ; //
    UINT32  bv_ref_stride_03                      ; //
    UINT32  bv_ref_stride_47                      ; //
    UINT32  bv_ref_pic_wh_0                       ; //
    UINT32  bv_ref_pic_wh_1                       ; //
    UINT32  bv_ref_pic_wh_2                       ; //
    UINT32  bv_ref_pic_wh_3                       ; //
    UINT32  bv_ref_pic_wh_4                       ; //
    UINT32  bv_ref_pic_wh_5                       ; //
    UINT32  bv_lut_base                           ; //
    UINT32  bv_lut_size                           ; //
    UINT32  bv_out_base                           ; //
    UINT32  bv_out_pic_wh                         ; //
    UINT32  bv_out_stride                         ; //
    UINT32  bv_chksum                             ; //
    UINT32  bv_status_0                           ; //
    UINT32  bv_status_1                           ; //
    UINT32  bv_status_2                           ; //
    UINT32  bv_status_3                           ; //
    UINT32  G114_RESERVED[5]                      ; //

    // Group 115 : GPS
    UINT32  G115_GPS_RESERVED1                    ;
    UINT32  GPS_CTRL                              ;
    UINT32  G115_GPS_RESERVED2[30]                ;

    // Group 116 : Reserved
    UINT32  G116_RESERVED1                        ;
    UINT32  RTC_CTRL                              ;
    UINT32  RTC_TIMER_CLOCK                       ;
    UINT32  RTC_ONTIME_SET                        ;
    UINT32  RTC_CLOCK_SET                         ;
    UINT32  G116_RESERVED2[27]                    ;

    // Group 117 : Reserved
    UINT32  G117_RESERVED[32]                     ;

    // Group 118 : CARD_CTL0
    UINT32  G118_CARD_CTL0[32]                    ;

    // Group 119 : CARD_CTL0
    UINT32  G119_CARD_CTL0[32]                    ;

    // Group 120 : CARD_CTL0
    UINT32  G120_CARD_CTL0[32]                    ;

    // Group 121 : CARD_CTL0
    UINT32  G121_CARD_CTL0[32]                    ;

    // Group 122 : CARD_CTL0
    UINT32  G122_CARD_CTL0[32]                    ;

    // Group 123 : Reserved
    UINT32  G123_RESERVED[32]                     ;

    // Group 124 : OTP
        UINT32 OTP_RN_CTRL;
	UINT32 OTP_RN_CMD;
	UINT32 OTP_RN_CMD_ST;
	UINT32 OTP_RN_ADDR;
	UINT32 OTP_RN_PGM_CNT;
	UINT32 OTP_RN_RDATA_0;
	UINT32 OTP_RN_RDATA_1;
	UINT32 OTP_RN_RDATA_2;
	UINT32 OTP_RN_RDATA_3;
        UINT32 OTP_RN_WDATA_0;
        UINT32 OTP_RN_WDATA_1;
        UINT32 OTP_RN_WDATA_2;
        UINT32 OTP_RN_WDATA_3;
	UINT32 OTP_RN_SM_STATUS;
	UINT32 OTP_RN_GCLK_CTRL;
        UINT32 OTP_RN_SEL_CTRL;
        UINT32 OTP_RN_GPIO_CTRL;
	UINT32 OTP_RN_GPIO_ADR_MODE;
	UINT32 OTP_RN_CUSTOMER_CMD;
	UINT32 OTP_RN_CUSTOMER_CMD_ST;
	UINT32 OTP_RN_CUSTOMER_ADDR;
	UINT32 OTP_RN_CUSTOMER_RDATA;
	UINT32 OTP_RN_CUSTOMER_WDATA;
	UINT32 OTP_RN_OTP_RR_SR_3;
	UINT32 OTP_RN_OTP_INIT_DATA_0;
	UINT32 OTP_RN_OTP_INIT_DATA_1;
	UINT32 OTP_RN_OTP_INIT_DATA_2;
	UINT32 OTP_RN_OTP_INIT_DATA_3;
	UINT32 OTP_RN_OTP_INIT_DATA_4;
	UINT32 OTP_RN_OTP_INIT_DATA_5;
	UINT32 OTP_RN_OTP_INIT_DATA_6;
	UINT32 OTP_RN_OTP_INIT_DATA_7;


    // Group 125 : CARD_CTL1
    UINT32  G125_CARD_CTL1[32]                    ;

    // Group 126 : CARD_CTL1
    UINT32  G126_CARD_CTL1[32]                    ;

    // Group 127 : CARD_CTL1
    UINT32  G127_CARD_CTL1[32]                    ;

    // Group 128 : CARD_CTL1
    UINT32  G128_CARD_CTL1[32]                    ;

    // Group 129 : CARD_CTL1
    UINT32  G129_CARD_CTL1[32]                    ;

    // Group 130 : CARD_CTL2
    UINT32  G130_CARD_CTL2[32]                    ;

    // Group 131 : CARD_CTL2
    UINT32  G131_CARD_CTL2[32]                    ;

    // Group 132 : CARD_CTL2
    UINT32  G132_CARD_CTL2[32]                    ;

    // Group 133 : CARD_CTL2
    UINT32  G133_CARD_CTL2[32]                    ;

    // Group 134 : CARD_CTL2
    UINT32  G134_CARD_CTL2[32]                    ;

    // Group 135 : Reserved
    UINT32  G135_RESERVED[32]                     ;

    // Group 136 : Reserved
    UINT32  G136_RESERVED[32]                     ;

    // Group 137 : IMG_DEC_SND/HUF
    UINT32  jpeg_2_huff_mode                      ; //
    UINT32  jpeg_2_huff_ctrl                      ; //
    UINT32  jpeg_2_huff_stats                     ; //
    UINT32  jpeg_2_huff_scan_ctrl                 ; //
    UINT32  jpeg_2_huff_table                     ; //
    UINT32  jpeg_2_huff_mcuno                     ; //
    UINT32  jpeg_2_huff_jpeghsize                 ; //
    UINT32  jpeg_2_huff_jpegvsize                 ; //
    UINT32  jpeg_2_huff_bitstream                 ; //
    UINT32  jpeg_2_huff_bits_dma_offset           ; //
    UINT32  jpeg_2_huff_wr_huff_info              ; //
    UINT32  jpeg_2_huff_bits_info                 ; //
    UINT32  jpeg_2_huff_rd_huff_info              ; //
    UINT32  jpeg_2_huff_dma_offset_0              ; //
    UINT32  jpeg_2_huff_dma_offset_1              ; //
    UINT32  jpeg_2_huff_acval_offset_0            ; //
    UINT32  jpeg_2_huff_acval_offset_1            ; //
    UINT32  jpeg_2_huff_acval_offset_2            ; //
    UINT32  jpeg_2_huff_acval_offset_3            ; //
    UINT32  jpeg_2_huff_wr_dc_01                  ; //
    UINT32  jpeg_2_huff_wr_dc_23                  ; //
    UINT32  jpeg_2_huff_wr_acval_0                ; //
    UINT32  jpeg_2_huff_wr_acval_1                ; //
    UINT32  jpeg_2_huff_wr_acval_2                ; //
    UINT32  jpeg_2_huff_wr_acval_3                ; //
    UINT32  jpeg_2_huff_rd_addr                   ; //
    UINT32  jpeg_2_huff_rd_val                    ; //
    UINT32  jpeg_2_huff_probe                     ; //
    UINT32  jr_2_control                          ; //
    UINT32  jr_2_status                           ; //
    UINT32  jr_2_reg_num                          ; //
    UINT32  jr_2_reg_data                         ; //

    // Group 138 : IMG_DEC_SND/JDM
    UINT32  jdm_2_mode;     
    UINT32  jdm_2_status;   
    UINT32  jdm_2_dma1_mode;      
    UINT32  jdm_2_dma1_xaddr;     
    UINT32  jdm_2_dma1_yaddr;     
    UINT32  jdm_2_dma1_done;      
    UINT32  jdm_2_dma1_start_addr;
    UINT32  jdm_2_dma1_block_num; 
    UINT32  g138_reserved_08;  
    UINT32  g138_reserved_09;  
    UINT32  g138_reserved_10;  
    UINT32  g138_reserved_11;  
    UINT32  g138_reserved_12;  
    UINT32  g138_reserved_13;  
    UINT32  g138_reserved_14;  
    UINT32  g138_reserved_15;  
    UINT32  g138_reserved_16;  
    UINT32  g138_reserved_17;  
    UINT32  g138_reserved_18;  
    UINT32  g138_reserved_19;  
    UINT32  g138_reserved_20;  
    UINT32  g138_reserved_21;  
    UINT32  jdm_2_dc_luma_base_lsb;  
    UINT32  jdm_2_dc_luma_base_msb;  
    UINT32  jdm_2_dc_chroma_base_lsb;  
    UINT32  jdm_2_dc_chroma_base_msb;  
    UINT32  jdm_2_dc_width;  
    UINT32  g138_reserved_27;  
    UINT32  g138_reserved_28;  
    UINT32  jdm_2_jpeg_offset;    
    UINT32  g138_reserved_30;  
    UINT32  g138_reserved_31;  

    // Group 139 : IMG_DEC_SND/INVQ
    UINT32  jpeg_2_invq_qmx_par                   ; // 
    UINT32  jpeg_2_invq_mode                      ; // 
    UINT32  jpeg_2_invq_chksum                    ; // 
    UINT32  g139_reserved[29]                     ; // 

    // Group 140 : NAND_CTL
    UINT32  G140_NAND_CTL[32]                     ;

    // Group 141 : Reserved
    UINT32  G141_RESERVED[32]                     ;

    // Group 142 : EHCI_USB0
    UINT32  G142_EHCI_USB0[32]                    ;

    // Group 143 : EHCI_USB0
    UINT32  G143_EHCI_USB0[32]                    ;

    // Group 144 : EHCI_USB1
    UINT32  G144_EHCI_USB1[32]                    ;

    // Group 145 : EHCI_USB1
    UINT32  G145_EHCI_USB1[32]                    ;

    // Group 146 : USBH
    UINT32  G146_EHCI_USB2[32]                    ;

    // Group 147 : USBH
    UINT32  G147_EHCI_USB2[32]                    ;

    // Group 148 : USBH
    UINT32  G148_USBH[32]                         ;

    // Group 149 : UPHY0
    UINT32  usb_phy0_config0      ;
    UINT32  usb_phy0_config1      ;
    UINT32  usb_phy0_config2      ;
    UINT32  usb_phy0_config3      ;
    UINT32  usb_phy0_config4      ;
    UINT32  usb_phy0_config5      ;
    UINT32  usb_phy0_config6      ;
    UINT32  usb_phy0_config7      ;
    UINT32  usb_phy0_bist_cmd     ;
    UINT32  usb_phy0_bist_done    ;
    UINT32  usb_phy0_bist_status  ;
    UINT32  g149_reserved1[13]    ;
    UINT32  g149_reserved2[8]     ;

    // Group 150 : UPHY1
    UINT32  usb_phy1_config0      ;
    UINT32  usb_phy1_config1      ;
    UINT32  usb_phy1_config2      ;
    UINT32  usb_phy1_config3      ;
    UINT32  usb_phy1_config4      ;
    UINT32  usb_phy1_config5      ;
    UINT32  usb_phy1_config6      ;
    UINT32  usb_phy1_config7      ;
    UINT32  usb_phy1_bist_cmd     ;
    UINT32  usb_phy1_bist_done    ;
    UINT32  usb_phy1_bist_status  ;
    UINT32  g150_reserved1[13]    ;
    UINT32  g150_reserved2[8]     ;

    // Group 151 : UPHY2
    UINT32  usb_phy2_config0      ;
    UINT32  usb_phy2_config1      ;
    UINT32  usb_phy2_config2      ;
    UINT32  usb_phy2_config3      ;
    UINT32  usb_phy2_config4      ;
    UINT32  usb_phy2_config5      ;
    UINT32  usb_phy2_config6      ;
    UINT32  usb_phy2_config7      ;
    UINT32  usb_phy2_bist_cmd     ;
    UINT32  usb_phy2_bist_done    ;
    UINT32  usb_phy2_bist_status  ;
    UINT32  g151_reserved1[13]    ;
    UINT32  g151_reserved2[8]     ;

   // Group 152 : Reserved
    UINT32  G152_RESERVED[32]                     ;

    // Group 153 : Reserved
    UINT32  G153_RESERVED[32]                     ;

    // Group 154 : GMAC
    UINT32  mac_glb_int_status                    ; // 00
    UINT32  mac_glb_int_mask                      ; // 01
    UINT32  mac_glb_sys_cfgcmd                    ; // 02
    UINT32  mac_glb_dma_cfg                       ; // 03
    UINT32  mac_glb_port_cfg                      ; // 04
    UINT32  mac_glb_port_power                    ; // 05
    UINT32  mac_glb_port_sta                      ; // 06
    UINT32  mac_glb_pause_cfg                     ; // 07
    UINT32  mac_glb_pause_sta                     ; // 08
    UINT32  mac_glb_reserved1[3]                  ; // 09~11
    UINT32  mac_glb_sip_hw                        ; // 12
    UINT32  mac_glb_dip_hw                        ; // 13
    UINT32  mac_glb_phy_cmd                       ; // 14
    UINT32  mac_glb_phy_sta                       ; // 15
    UINT32  mac_glb_reserved2[8]                  ; // 16~23
    UINT32  mac_eee_on_timer                      ; // 24
    UINT32  mac_eee_wake_timer                    ; // 25
    UINT32  mac_gclk_dis                          ; // 26
    UINT32  mac_glb_reserved3[5]                  ; // 27~31

    // Group 155 : GMAC
    UINT32  mac_tx_cfg                            ; // 00
    UINT32  mac_tx_reserved1[2]                   ; // 01~02
    UINT32  mac_tx_desc_base                      ; // 03
    UINT32  mac_tx_cdatabuffbaseaddr              ; // 04
    UINT32  mac_tx_cdatabuffaddr                  ; // 05
    UINT32  mac_tx_fifo_ptr                       ; // 06
    UINT32  mac_tx_desc_addr                      ; // 07
    UINT32  mac_tx_reserved2                      ; // 08
    UINT32  mac_tx_watchdogtimer                  ; // 09
    UINT32  mac_tx_desc_data_lo                   ; // 10 
    UINT32  mac_tx_desc_data_hi                   ; // 11
    UINT32  mac_tx_desc_csr                       ; // 12
    UINT32  mac_tx_debugcfg                       ; // 13
    UINT32  mac_tx_epg_burst                      ; // 14
    UINT32  mac_tx_epg_pkt                        ; // 15
    UINT32  mac_tx_pre_buf_ptr                    ; // 16
    UINT32  mac_tx_reserved3[9]                   ; // 17~25
    UINT32  mac_tx_desc_info                      ; // 26
    UINT32  mac_tx_dbg_ctrl                       ; // 27
    UINT32  mac_tx_epg_pkt_cnt                    ; // 28
    UINT32  mac_tx_reserved4[3]                   ; // 29~31

    // Group 156 : GMAC
    UINT32  mac_rx_cfg                            ; // 00
    UINT32  mac_rx_rcv_rule_l2                    ; // 01
    UINT32  mac_rx_rcv_rule_l3                    ; // 02
    UINT32  mac_rx_reserved1                      ; // 03
    UINT32  mac_rx_desc_base_q0                   ; // 04
    UINT32  mac_rx_desc_base_q1                   ; // 05
    UINT32  mac_rx_mchash0                        ; // 06
    UINT32  mac_rx_mchash1                        ; // 07
    UINT32  mac_rx_uchash0                        ; // 08
    UINT32  mac_rx_uchash1                        ; // 09
    UINT32  mac_rx_vlan0                          ; // 10
    UINT32  mac_rx_vlan1                          ; // 11
    UINT32  mac_rx_vlan2                          ; // 12
    UINT32  mac_rx_vlan3                          ; // 13
    UINT32  mac_rx_pri                            ; // 14
    UINT32  mac_rx_desc0_addr                     ; // 15
    UINT32  mac_rx_desc1_addr                     ; // 16
    UINT32  mac_rx_reserved2[3]                   ; // 17~19
    UINT32  mac_rx_desc_data_ptr                  ; // 20
    UINT32  mac_rx_sram_data_lo                   ; // 21
    UINT32  mac_rx_sram_data_hi                   ; // 22
    UINT32  mac_rx_sram_csr                       ; // 23
    UINT32  mac_rx_pre_buf_ptr                    ; // 24
    UINT32  mac_rx_fifo_cur_ptr                   ; // 25
    UINT32  mac_rx_fifo_pre_ptr                   ; // 26
    UINT32  mac_rx_desc_info                      ; // 27
    UINT32  mac_rx_sts_desc_ptr_q0                ; // 28
    UINT32  mac_rx_sts_desc_ptr_q1                ; // 29
    UINT32  mac_rx_datbuf_maxentry                ; // 30
    UINT32  mac_rx_watchdogtimer                  ; // 31

    // Group 157 : GMAC
    UINT32  mac_glb_macaddr0                      ; // 00
    UINT32  mac_glb_macaddr1                      ; // 01
    UINT32  mac_glb_phy_addr                      ; // 02
    UINT32  mac_glb_sip_sw                        ; // 03
    UINT32  mac_adr_reserved1[28]                 ; // 04~31

    // Group 158 : GMAC
    UINT32  mac_mib_tx_pause                      ; // 00
    UINT32  mac_mib_tx_fifo_udr                   ; // 01
    UINT32  mac_mib_tx_reserved0[1]               ; // 02 
    UINT32  mac_mib_tx_col                        ; // 03
    UINT32  mac_mib_tx_col_abort                  ; // 04
    UINT32  mac_mib_tx_late_col                   ; // 05
    UINT32  mac_mib_tx_abt_watchdog               ; // 06
    UINT32  mac_mib_tx_totalpkt                   ; // 07
    UINT32  mac_mib_tx_totaldes_cnt               ; // 08
    UINT32  mac_mib_tx_reserved[7]                ; // 09~15
    UINT32  mac_mib_rx_pause                      ; // 16
    UINT32  mac_mib_rx_fcserr                     ; // 17
    UINT32  mac_mib_rx_alignerr                   ; // 18
    UINT32  mac_mib_rx_frag                       ; // 19
    UINT32  mac_mib_rx_runt                       ; // 20
    UINT32  mac_mib_rx_long                       ; // 21
    UINT32  mac_mib_rx_reserved0[1]               ; // 22
    UINT32  mac_mib_rx_abort                      ; // 23
    UINT32  mac_mib_rx_totalpkt                   ; // 24
    UINT32  mac_mib_rx_jambo                      ; // 25
    UINT32  mac_mib_rx_burst_too_long             ; // 26
    UINT32  mac_mib_rx_totaldes_cnt               ; // 27
    UINT32  mac_mib_rx_reserved[3]                ; // 28~30
    UINT32  mac_rxnum_after_pause                 ; // 31

    // Group 159 : EPHY
    UINT32  G159_EPHY[32]                         ;

    // Group 160 : 
    UINT32  SAC1_SRST                             ; // 00
    UINT32  SAC1_DMA_WA_OFFSET                    ; // 01
    UINT32  SAC1_DMA_W_BUFSIZE                    ; // 02
    UINT32  SAC1_DMA_WA_POINTER                   ; // 03
    UINT32  SAC1_DMA_WA_BARRIER                   ; // 04
    UINT32  SAC1_DMA_RA_OFFSET                    ; // 05
    UINT32  SAC1_DMA_R_BUFSIZE                    ; // 06
    UINT32  SAC1_DMA_RA_POINTER                   ; // 07
    UINT32  SAC1_DMA_CTRL                         ; // 08
    UINT32  SAC1_DMA_EN                           ; // 09
    UINT32  SAC1_SECTOR_LENGTH                    ; // 10
    UINT32  SAC1_SECTOR_NUM                       ; // 11
    UINT32  SAC1_XCBC_HEADER_LENGTH               ; // 12
    UINT32  SAC1_XCBC_MAC                         ; // 13
    UINT32  G160_reserved1                        ; // 14
    UINT32  SAC1_FILL_ADDR                        ; // 15
    UINT32  SAC1_CTRL                             ; // 16
    UINT32  SAC1_IN                               ; // 17
    UINT32  SAC1_IV                               ; // 18
    UINT32  SAC1_CLEAR_KEY                        ; // 19
    UINT32  SAC1_SECU_INFO_VBIT                   ; // 20
    UINT32  SAC1_SECU_INFO_RK_STU                 ; // 21
    UINT32  G160_reserved2[10]                    ; // 22~31

    // Group 161 : SAC
    UINT32  SAC0_SRST                             ; // 00
    UINT32  SAC0_DMA_WA_OFFSET                    ; // 01
    UINT32  SAC0_DMA_W_BUFSIZE                    ; // 02
    UINT32  SAC0_DMA_WA_POINTER                   ; // 03
    UINT32  SAC0_DMA_WA_BARRIER                   ; // 04
    UINT32  SAC0_DMA_RA_OFFSET                    ; // 05
    UINT32  SAC0_DMA_R_BUFSIZE                    ; // 06
    UINT32  SAC0_DMA_RA_POINTER                   ; // 07
    UINT32  SAC0_DMA_CTRL                         ; // 08
    UINT32  SAC0_DMA_EN                           ; // 09
    UINT32  SAC0_SECTOR_LENGTH                    ; // 10
    UINT32  SAC0_SECTOR_NUM                       ; // 11
    UINT32  SAC0_XCBC_HEADER_LENGTH               ; // 12
    UINT32  SAC0_XCBC_MAC                         ; // 13
    UINT32  G161_reserved1                        ; // 14
    UINT32  SAC0_FILL_ADDR                        ; // 15
    UINT32  SAC0_CTRL                             ; // 16
    UINT32  SAC0_IN                               ; // 17
    UINT32  SAC0_IV                               ; // 18
    UINT32  SAC0_CLEAR_KEY                        ; // 19
    UINT32  SAC0_SECU_INFO_VBIT                   ; // 20
    UINT32  SAC0_SECU_INFO_RK_STU                 ; // 21
    UINT32  G161_reserved2[10]                    ; // 22-31

    // Group 162 : PCMCIA
    UINT32  G162_PCMCIA[32]                       ;

    // Group 163 : DEMUX0
    UINT32  G163_DEMUX0[32]                       ;

    // Group 164 : DEMUX0
    UINT32  G164_DEMUX0[32]                       ;

    // Group 165 : TPI
    UINT32  G165_TPI_G272[32]                     ;

    // Group 166 : TPI
    UINT32  G166_TPI_G273[32]                     ;

    // Group 167 : TPI
    UINT32  G167_TPI_G274[32]                       ;

    // Group 168 : TPI
    UINT32  G168_TPI_G275[32]                       ;

    // Group 169 : TPI
    UINT32  G169_TPI_G276[32]                       ;

    // Group 170 : TPI
    UINT32  G170_TPI_G277[32]                       ;

    // Group 171 : TPI
    UINT32  G171_TPI_G278[32]                       ;

    // Group 172 : TPI
    UINT32  G172_TPI_G279[32]                       ;

    // Group 173 : TPISUB
    UINT32  G173_TPISUB_G260[32]                    ;

    // Group 174 : TPISUB
    UINT32  G174_TPISUB_G261[32]                    ;

    // Group 175 : TPISUB
    UINT32  G175_TPISUB_G262[32]                    ;

    // Group 176 : TPISUB
    UINT32  G176_TPISUB_G263[32]                    ;

    // Group 177 : TPISUB
    UINT32  G177_TPISUB_G264[32]                    ;

    // Group 178 : TPISUB
    UINT32  G178_TPISUB_G265[32]                    ;

    // Group 179 : TPISUB
    UINT32  G179_TPISUB_G266[32]                    ;

    // Group 180 : TPISUB
    UINT32  G180_TPISUB_G267[32]                    ;

    // Group 181 : Reserved
    UINT32  G181_RESERVED[32]                     ;

    // Group 182 : Reserved
    UINT32  G182_RESERVED[32]                     ;

    // Group 183 : Reserved
    UINT32  G183_RESERVED[32]                     ;

    // Group 184 : Reserved
    UINT32  G184_RESERVED[32]                     ;

    // Group 185 : DDFCH0
    UINT32  ddfch0_latch_en                       ; // 00
    UINT32  ddfch0_mode_option                    ; // 01
    UINT32  ddfch0_enable                         ; // 02
    UINT32  ddfch0_urgent_thd                     ; // 03
    UINT32  ddfch0_cmdq_thd                       ; // 04
    UINT32  ddfch0_mr_base_addr                   ; // 05
    UINT32  ddfch0_luma_base_addr_0               ; // 06
    UINT32  ddfch0_luma_base_addr_1               ; // 07
    UINT32  ddfch0_luma_base_addr_2               ; // 08
    UINT32  ddfch0_crma_base_addr_0               ; // 09
    UINT32  ddfch0_crma_base_addr_1               ; // 10
    UINT32  ddfch0_crma_base_addr_2               ; // 11
    UINT32  ddfch0_indx_base_addr_0               ; // 12
    UINT32  ddfch0_indx_base_addr_1               ; // 13
    UINT32  ddfch0_indx_base_addr_2               ; // 14
    UINT32  ddfch0_pip_mask_size                  ; // 15
    UINT32  ddfch0_pip_mask_offset                ; // 16
    UINT32  ddfch0_mr_frame_size                  ; // 17
    UINT32  ddfch0_mr_crop_size                   ; // 18
    UINT32  ddfch0_mr_crop_offset                 ; // 19
    UINT32  ddfch0_vdo_frame_size                 ; // 20
    UINT32  ddfch0_vdo_crop_size                  ; // 21
    UINT32  ddfch0_vdo_crop_offset                ; // 22
    UINT32  ddfch0_config_0                       ; // 23
    UINT32  ddfch0_config_1                       ; // 24
    UINT32  ddfch0_checksum_sel                   ; // 25
    UINT32  ddfch0_chksum_info                    ; // 26
    UINT32  ddfch0_error_flag_info                ; // 27
    UINT32  ddfch0_bist                           ; // 28
    UINT32  g185_reserved0[3]                     ; // 29~31
    
    // Group 186 : DDFCH1
    UINT32  ddfch1_latch_en                       ; // 00
    UINT32  ddfch1_mode_option                    ; // 01
    UINT32  ddfch1_enable                         ; // 02
    UINT32  ddfch1_urgent_thd                     ; // 03
    UINT32  ddfch1_cmdq_thd                       ; // 04
    UINT32  ddfch1_mr_base_addr                   ; // 05
    UINT32  ddfch1_luma_base_addr_0               ; // 06
    UINT32  ddfch1_luma_base_addr_1               ; // 07
    UINT32  ddfch1_luma_base_addr_2               ; // 08
    UINT32  ddfch1_crma_base_addr_0               ; // 09
    UINT32  ddfch1_crma_base_addr_1               ; // 10
    UINT32  ddfch1_crma_base_addr_2               ; // 11
    UINT32  ddfch1_indx_base_addr_0               ; // 12
    UINT32  ddfch1_indx_base_addr_1               ; // 13
    UINT32  ddfch1_indx_base_addr_2               ; // 14
    UINT32  ddfch1_pip_mask_size                  ; // 15
    UINT32  ddfch1_pip_mask_offset                ; // 16
    UINT32  ddfch1_mr_frame_size                  ; // 17
    UINT32  ddfch1_mr_crop_size                   ; // 18
    UINT32  ddfch1_mr_crop_offset                 ; // 19
    UINT32  ddfch1_vdo_frame_size                 ; // 20
    UINT32  ddfch1_vdo_crop_size                  ; // 21
    UINT32  ddfch1_vdo_crop_offset                ; // 22
    UINT32  ddfch1_config_0                       ; // 23
    UINT32  ddfch1_config_1                       ; // 24
    UINT32  ddfch1_checksum_sel                   ; // 25
    UINT32  ddfch1_chksum_info                    ; // 26
    UINT32  ddfch1_error_flag_info                ; // 27
    UINT32  ddfch1_bist                           ; // 28
    UINT32  g186_reserved0[3]                     ; // 29~31    

    // Group 187 : DEINT0
    UINT32  deint0_param_1                        ; // 00
    UINT32  deint0_cnt_y_his_r                    ; // 01
    UINT32  deint0_gclk_param_1                   ; // 02
    UINT32  deint0_gclk_param_2                   ; // 03
    UINT32  deint0_checksum                       ; // 04
    UINT32  deint0_cnt_y_pre                      ; // 05
    UINT32  deint0_cnt_y_cur                      ; // 06
    UINT32  deint0_cnt_y_nxt                      ; // 07
    UINT32  deint0_scene_change_param_1           ; // 08
    UINT32  deint0_scene_change_param_2           ; // 09
    UINT32  deint0_cnt_y_his_w                    ; // 10
    UINT32  deint0_pulldown22_param_1             ; // 11
    UINT32  deint0_pulldown22_param_2             ; // 12
    UINT32  deint0_pulldown22_param_3             ; // 13
    UINT32  deint0_pulldown22_param_4             ; // 14
    UINT32  deint0_pulldown22_param_5             ; // 15
    UINT32  deint0_pulldown22_param_6             ; // 16
    UINT32  deint0_pulldown22_param_7             ; // 17
    UINT32  deint0_pulldown22_param_8             ; // 18
    UINT32  deint0_x_size                         ; // 19
    UINT32  deint0_pulldown32_param_1             ; // 20
    UINT32  deint0_pulldown32_param_2             ; // 21
    UINT32  deint0_pulldown32_param_3             ; // 22
    UINT32  deint0_pulldown32_param_4             ; // 23
    UINT32  deint0_pulldown32_param_5             ; // 24
    UINT32  deint0_pulldown32_param_6             ; // 25
    UINT32  deint0_pulldown32_param_7             ; // 26
    UINT32  deint0_pulldown32_param_8             ; // 27
    UINT32  deint0_y_size                         ; // 28
    UINT32  deint0_bad_edit_param_1               ; // 29
    UINT32  deint0_bad_edit_param_2               ; // 30
    UINT32  deint0_bad_edit_param_3               ; // 31    

    // Group 188 : DEINT0
    UINT32  deint0_decision_param_1               ; // 00
    UINT32  deint0_decision_param_2               ; // 01
    UINT32  deint0_decision_param_3               ; // 02
    UINT32  deint0_decision_param_4               ; // 03
    UINT32  deint0_decision_param_5               ; // 04
    UINT32  deint0_decision_param_6               ; // 05
    UINT32  deint0_decision_param_7               ; // 06
    UINT32  deint0_decision_param_8               ; // 07
    UINT32  deint0_decision_param_9               ; // 08
    UINT32  deint0_decision_param_a               ; // 09
    UINT32  deint0_decision_param_b               ; // 10
    UINT32  deint0_val_field_luma_l               ; // 11
    UINT32  deint0_val_field_luma_h               ; // 12
    UINT32  deint0_intra_param_1                  ; // 13
    UINT32  deint0_intra_param_2                  ; // 14
    UINT32  deint0_intra_param_3                  ; // 15
    UINT32  deint0_intra_param_4                  ; // 16
    UINT32  deint0_intra_param_5                  ; // 17
    UINT32  deint0_intra_param_6                  ; // 18
    UINT32  deint0_intra_param_7                  ; // 19
    UINT32  deint0_intra_param_8                  ; // 20
    UINT32  deint0_read_cnt_y                     ; // 21
    UINT32  deint0_inter_param_1                  ; // 22
    UINT32  deint0_mixing_param_1                 ; // 23
    UINT32  deint0_val_22_sawtooth                ; // 24
    UINT32  deint0_val_32_motion                  ; // 25
    UINT32  deint0_val_be_tmp                     ; // 26
    UINT32  deint0_val_be_spt                     ; // 27
    UINT32  deint0_val_be_mix                     ; // 28
    UINT32  deint0_val_be_info1                   ; // 29
    UINT32  deint0_val_be_info2                   ; // 30
    UINT32  deint0_val_scene_change               ; // 31

    // Group 189 : DEINT1
    UINT32  deint1_param_1                        ; // 00
    UINT32  deint1_cnt_y_his_r                    ; // 01
    UINT32  deint1_gclk_param_1                   ; // 02
    UINT32  deint1_gclk_param_2                   ; // 03
    UINT32  deint1_checksum                       ; // 04
    UINT32  deint1_cnt_y_pre                      ; // 05
    UINT32  deint1_cnt_y_cur                      ; // 06
    UINT32  deint1_cnt_y_nxt                      ; // 07
    UINT32  deint1_scene_change_param_1           ; // 08
    UINT32  deint1_scene_change_param_2           ; // 09
    UINT32  deint1_cnt_y_his_w                    ; // 10
    UINT32  deint1_pulldown22_param_1             ; // 11
    UINT32  deint1_pulldown22_param_2             ; // 12
    UINT32  deint1_pulldown22_param_3             ; // 13
    UINT32  deint1_pulldown22_param_4             ; // 14
    UINT32  deint1_pulldown22_param_5             ; // 15
    UINT32  deint1_pulldown22_param_6             ; // 16
    UINT32  deint1_pulldown22_param_7             ; // 17
    UINT32  deint1_pulldown22_param_8             ; // 18
    UINT32  deint1_x_size                         ; // 19
    UINT32  deint1_pulldown32_param_1             ; // 20
    UINT32  deint1_pulldown32_param_2             ; // 21
    UINT32  deint1_pulldown32_param_3             ; // 22
    UINT32  deint1_pulldown32_param_4             ; // 23
    UINT32  deint1_pulldown32_param_5             ; // 24
    UINT32  deint1_pulldown32_param_6             ; // 25
    UINT32  deint1_pulldown32_param_7             ; // 26
    UINT32  deint1_pulldown32_param_8             ; // 27
    UINT32  deint1_y_size                         ; // 28
    UINT32  deint1_bad_edit_param_1               ; // 29
    UINT32  deint1_bad_edit_param_2               ; // 30
    UINT32  deint1_bad_edit_param_3               ; // 31 

    // Group 190 : DEINT1
    UINT32  deint1_decision_param_1               ; // 00
    UINT32  deint1_decision_param_2               ; // 01
    UINT32  deint1_decision_param_3               ; // 02
    UINT32  deint1_decision_param_4               ; // 03
    UINT32  deint1_decision_param_5               ; // 04
    UINT32  deint1_decision_param_6               ; // 05
    UINT32  deint1_decision_param_7               ; // 06
    UINT32  deint1_decision_param_8               ; // 07
    UINT32  deint1_decision_param_9               ; // 08
    UINT32  deint1_decision_param_a               ; // 09
    UINT32  deint1_decision_param_b               ; // 10
    UINT32  deint1_val_field_luma_l               ; // 11
    UINT32  deint1_val_field_luma_h               ; // 12
    UINT32  deint1_intra_param_1                  ; // 13
    UINT32  deint1_intra_param_2                  ; // 14
    UINT32  deint1_intra_param_3                  ; // 15
    UINT32  deint1_intra_param_4                  ; // 16
    UINT32  deint1_intra_param_5                  ; // 17
    UINT32  deint1_intra_param_6                  ; // 18
    UINT32  deint1_intra_param_7                  ; // 19
    UINT32  deint1_intra_param_8                  ; // 20
    UINT32  deint1_read_cnt_y                     ; // 21
    UINT32  deint1_inter_param_1                  ; // 22
    UINT32  deint1_mixing_param_1                 ; // 23
    UINT32  deint1_val_22_sawtooth                ; // 24
    UINT32  deint1_val_32_motion                  ; // 25
    UINT32  deint1_val_be_tmp                     ; // 26
    UINT32  deint1_val_be_spt                     ; // 27
    UINT32  deint1_val_be_mix                     ; // 28
    UINT32  deint1_val_be_info1                   ; // 29
    UINT32  deint1_val_be_info2                   ; // 30
    UINT32  deint1_val_scene_change               ; // 31    

    // Group 191 : VSCL0
    UINT32  vscl0_config1                         ; // 00
    UINT32  vscl0_config2                         ; // 01
    UINT32  g191_reserved0                        ; // 02
    UINT32  vscl0_actrl_i_xlen                    ; // 03
    UINT32  vscl0_actrl_i_ylen                    ; // 04
    UINT32  vscl0_actrl_s_xstart                  ; // 05
    UINT32  vscl0_actrl_s_ystart                  ; // 06
    UINT32  vscl0_actrl_s_xlen                    ; // 07
    UINT32  vscl0_actrl_s_ylen                    ; // 08
    UINT32  vscl0_dctrl_o_xlen                    ; // 09
    UINT32  vscl0_dctrl_o_ylen                    ; // 10
    UINT32  vscl0_dctrl_d_xstart                  ; // 11
    UINT32  vscl0_dctrl_d_ystart                  ; // 12
    UINT32  vscl0_dctrl_d_xlen                    ; // 13
    UINT32  vscl0_dctrl_d_ylen                    ; // 14
    UINT32  vscl0_dctrl_bgc_c                     ; // 15
    UINT32  vscl0_dctrl_bgc_y                     ; // 16
    UINT32  g191_reserved1                        ; // 17
    UINT32  vscl0_hint_ctrl                       ; // 18
    UINT32  vscl0_hint_hfactor_low                ; // 19
    UINT32  vscl0_hint_hfactor_high               ; // 20
    UINT32  vscl0_hint_y_initf_low                ; // 21
    UINT32  vscl0_hint_y_initf_high               ; // 22
    UINT32  vscl0_hint_c_initf_low                ; // 23
    UINT32  vscl0_hint_c_initf_high               ; // 24
    UINT32  vscl0_hint_overshoot                  ; // 25
    UINT32  g191_reserved2[2]                     ; // 26,27
    UINT32  vscl0_sram_ctrl                       ; // 28
    UINT32  vscl0_sram_addr                       ; // 29
    UINT32  vscl0_sram_write_data                 ; // 30
    UINT32  vscl0_sram_read_data                  ; // 31

    // Group 192 : VSCL0
    UINT32  vscl0_vint_ctrl                       ; // 00
    UINT32  vscl0_vint_vfactor_low                ; // 01
    UINT32  vscl0_vint_vfactor_high               ; // 02
    UINT32  vscl0_vint_initf_low                  ; // 03
    UINT32  vscl0_vint_initf_high                 ; // 04
    UINT32  vscl0_dr_ctrl                         ; // 05
    UINT32  vscl0_dr_dege_detect1                 ; // 06
    UINT32  vscl0_dr_dege_detect2                 ; // 07
    UINT32  vscl0_dr_clamp_detect                 ; // 08
    UINT32  vscl0_vimg_ctrl                       ; // 09
    UINT32  vscl0_vimg_vpeak_lut_1                ; // 10
    UINT32  vscl0_vimg_vpeak_lut_2                ; // 11
    UINT32  vscl0_vimg_vpeak_lut_3                ; // 12
    UINT32  vscl0_vimg_vpeak_lut_4                ; // 13
    UINT32  vscl0_vimg_vpeak_lut_5                ; // 14
    UINT32  vscl0_vimg_vpeak_lut_6                ; // 15
    UINT32  vscl0_vimg_vpeak_config               ; // 16
    UINT32  vscl0_vimg_diag_config_1              ; // 17
    UINT32  vscl0_vimg_diag_config_2              ; // 18
    UINT32  g192_reserved1[2]                     ; // 19,20
    UINT32  vscl0_checksum_select                 ; // 21
    UINT32  vscl0_checksum_result                 ; // 22
    UINT32  g192_reserved2[9]                     ; // 23~31

    // Group 193 : VSCL1
    UINT32  vscl1_config1                         ; // 00
    UINT32  vscl1_config2                         ; // 01
    UINT32  g193_reserved0                        ; // 02
    UINT32  vscl1_actrl_i_xlen                    ; // 03
    UINT32  vscl1_actrl_i_ylen                    ; // 04
    UINT32  vscl1_actrl_s_xstart                  ; // 05
    UINT32  vscl1_actrl_s_ystart                  ; // 06
    UINT32  vscl1_actrl_s_xlen                    ; // 07
    UINT32  vscl1_actrl_s_ylen                    ; // 08
    UINT32  vscl1_dctrl_o_xlen                    ; // 09
    UINT32  vscl1_dctrl_o_ylen                    ; // 10
    UINT32  vscl1_dctrl_d_xstart                  ; // 11
    UINT32  vscl1_dctrl_d_ystart                  ; // 12
    UINT32  vscl1_dctrl_d_xlen                    ; // 13
    UINT32  vscl1_dctrl_d_ylen                    ; // 14
    UINT32  vscl1_dctrl_bgc_c                     ; // 15
    UINT32  vscl1_dctrl_bgc_y                     ; // 16
    UINT32  g193_reserved1                        ; // 17
    UINT32  vscl1_hint_ctrl                       ; // 18
    UINT32  vscl1_hint_hfactor_low                ; // 19
    UINT32  vscl1_hint_hfactor_high               ; // 20
    UINT32  vscl1_hint_y_initf_low                ; // 21
    UINT32  vscl1_hint_y_initf_high               ; // 22
    UINT32  vscl1_hint_c_initf_low                ; // 23
    UINT32  vscl1_hint_c_initf_high               ; // 24
    UINT32  vscl1_hint_overshoot                  ; // 25
    UINT32  g193_reserved2[2]                     ; // 26,27
    UINT32  vscl1_sram_ctrl                       ; // 28
    UINT32  vscl1_sram_addr                       ; // 29
    UINT32  vscl1_sram_write_data                 ; // 30
    UINT32  vscl1_sram_read_data                  ; // 31    

    // Group 194 : VSCL1
    UINT32  vscl1_vint_ctrl                       ; // 00
    UINT32  vscl1_vint_vfactor_low                ; // 01
    UINT32  vscl1_vint_vfactor_high               ; // 02
    UINT32  vscl1_vint_initf_low                  ; // 03
    UINT32  vscl1_vint_initf_high                 ; // 04
    UINT32  vscl1_dr_ctrl                         ; // 05
    UINT32  vscl1_dr_dege_detect1                 ; // 06
    UINT32  vscl1_dr_dege_detect2                 ; // 07
    UINT32  vscl1_dr_clamp_detect                 ; // 08
    UINT32  vscl1_vimg_ctrl                       ; // 09
    UINT32  vscl1_vimg_vpeak_lut_1                ; // 10
    UINT32  vscl1_vimg_vpeak_lut_2                ; // 11
    UINT32  vscl1_vimg_vpeak_lut_3                ; // 12
    UINT32  vscl1_vimg_vpeak_lut_4                ; // 13
    UINT32  vscl1_vimg_vpeak_lut_5                ; // 14
    UINT32  vscl1_vimg_vpeak_lut_6                ; // 15
    UINT32  vscl1_vimg_vpeak_config               ; // 16
    UINT32  vscl1_vimg_diag_config_1              ; // 17
    UINT32  vscl1_vimg_diag_config_2              ; // 18
    UINT32  g194_reserved1[2]                     ; // 19,20
    UINT32  vscl1_checksum_select                 ; // 21
    UINT32  vscl1_checksum_result                 ; // 22
    UINT32  g194_reserved2[9]                     ; // 23~31

    // Group 195 : NR0
    UINT32  nr0_en                                ; // 00
    UINT32  nr0_deovs_win                         ; // 01
    UINT32  nr0_deovs_cor_th                      ; // 02
    UINT32  nr0_deovs_ring_edge_th                ; // 03
    UINT32  nr0_deovs_ring_l01                    ; // 04
    UINT32  nr0_deovs_ring_l23                    ; // 05
    UINT32  nr0_deovs_fn_l0123                    ; // 06
    UINT32  nr0_dr_ctrl                           ; // 07
    UINT32  nr0_dr_edge_det1                      ; // 08
    UINT32  nr0_dr_edge_det2                      ; // 09
    UINT32  nr0_dr_clamp_det                      ; // 10
    UINT32  nr0_dm_edge_det1                      ; // 11
    UINT32  nr0_dm_edge_det2                      ; // 12
    UINT32  nr0_dm_clamp_det1                     ; // 13
    UINT32  nr0_dm_clamp_det2                     ; // 14
    UINT32  nr0_snr_ctrl                          ; // 15
    UINT32  nr0_snr_edge_det1                     ; // 16
    UINT32  nr0_snr_edge_det2                     ; // 17
    UINT32  nr0_snr_edge_det3                     ; // 18
    UINT32  nr0_snr_clamp_value                   ; // 19
    UINT32  nr0_snr_masnr_factor1                 ; // 20
    UINT32  nr0_snr_masnr_factor2                 ; // 21
    UINT32  nr0_pix_no                            ; // 22
    UINT32  nr0_line_no                           ; // 23
    UINT32  nr0_gated_ctrl                        ; // 24
    UINT32  nr0_chksum_en                         ; // 25
    UINT32  nr0_chksum_ans                        ; // 26
    UINT32  g195_reserved0[5]                     ; // 27~31

    // Group 196 : NR1
    UINT32  nr1_en                                ; // 00
    UINT32  nr1_deovs_win                         ; // 01
    UINT32  nr1_deovs_cor_th                      ; // 02
    UINT32  nr1_deovs_ring_edge_th                ; // 03
    UINT32  nr1_deovs_ring_l01                    ; // 04
    UINT32  nr1_deovs_ring_l23                    ; // 05
    UINT32  nr1_deovs_fn_l0123                    ; // 06
    UINT32  nr1_dr_ctrl                           ; // 07
    UINT32  nr1_dr_edge_det1                      ; // 08
    UINT32  nr1_dr_edge_det2                      ; // 09
    UINT32  nr1_dr_clamp_det                      ; // 10
    UINT32  nr1_dm_edge_det1                      ; // 11
    UINT32  nr1_dm_edge_det2                      ; // 12
    UINT32  nr1_dm_clamp_det1                     ; // 13
    UINT32  nr1_dm_clamp_det2                     ; // 14
    UINT32  nr1_snr_ctrl                          ; // 15
    UINT32  nr1_snr_edge_det1                     ; // 16
    UINT32  nr1_snr_edge_det2                     ; // 17
    UINT32  nr1_snr_edge_det3                     ; // 18
    UINT32  nr1_snr_clamp_value                   ; // 19
    UINT32  nr1_snr_masnr_factor1                 ; // 20
    UINT32  nr1_snr_masnr_factor2                 ; // 21
    UINT32  nr1_pix_no                            ; // 22
    UINT32  nr1_line_no                           ; // 23
    UINT32  nr1_gated_ctrl                        ; // 24
    UINT32  nr1_chksum_en                         ; // 25
    UINT32  nr1_chksum_ans                        ; // 26
    UINT32  g196_reserved0[5]                     ; // 27~31

    // Group 197 : IMG0
    UINT32  img0_config                           ; // 00
    UINT32  img0_pkti_ctrl                        ; // 01
    UINT32  img0_pkti_bp1_coef_1                  ; // 02
    UINT32  img0_pkti_bp1_coef_2                  ; // 03
    UINT32  img0_pkti_bp1_coef_3                  ; // 04
    UINT32  img0_pkti_lpk_config                  ; // 05
    UINT32  img0_pkti_lpk_gain                    ; // 06
    UINT32  img0_pkti_lpk_lut_1                   ; // 07
    UINT32  img0_pkti_lpk_lut_2                   ; // 08
    UINT32  img0_pkti_lpk_lut_3                   ; // 09
    UINT32  img0_pkti_lpk_lut_4                   ; // 10
    UINT32  img0_pkti_lpk_lut_5                   ; // 11
    UINT32  img0_pkti_lpk_lut_6                   ; // 12
    UINT32  img0_pkti_lpk_ovsht_1                 ; // 13
    UINT32  img0_pkti_lpk_ovsht_2                 ; // 14
    UINT32  img0_pkti_lti_gain                    ; // 15
    UINT32  img0_pkti_lti_lut_1                   ; // 16
    UINT32  img0_pkti_lti_lut_2                   ; // 17
    UINT32  img0_pkti_lti_lut_3                   ; // 18
    UINT32  img0_pkti_lti_lut_4                   ; // 19
    UINT32  img0_pkti_lti_lut_5                   ; // 20
    UINT32  img0_pkti_lti_lut_6                   ; // 21
    UINT32  img0_pkti_lti_ovsht_1                 ; // 22
    UINT32  img0_pkti_lti_ovsht_2                 ; // 23
    UINT32  img0_sram_ctrl                        ; // 24
    UINT32  img0_sram_addr                        ; // 25
    UINT32  img0_sram_write_data                  ; // 26
    UINT32  img0_sram_read_data                   ; // 27
    UINT32  img0_pix_no                           ; // 28
    UINT32  img0_line_no                          ; // 29
    UINT32  img0_chksum_ans                       ; // 30
    UINT32  g197_reserved0                        ; // 31

    // Group 198 : IMG1
    UINT32  img1_config                           ; // 00
    UINT32  img1_pkti_ctrl                        ; // 01
    UINT32  img1_pkti_bp1_coef_1                  ; // 02
    UINT32  img1_pkti_bp1_coef_2                  ; // 03
    UINT32  img1_pkti_bp1_coef_3                  ; // 04
    UINT32  img1_pkti_lpk_config                  ; // 05
    UINT32  img1_pkti_lpk_gain                    ; // 06
    UINT32  img1_pkti_lpk_lut_1                   ; // 07
    UINT32  img1_pkti_lpk_lut_2                   ; // 08
    UINT32  img1_pkti_lpk_lut_3                   ; // 09
    UINT32  img1_pkti_lpk_lut_4                   ; // 10
    UINT32  img1_pkti_lpk_lut_5                   ; // 11
    UINT32  img1_pkti_lpk_lut_6                   ; // 12
    UINT32  img1_pkti_lpk_ovsht_1                 ; // 13
    UINT32  img1_pkti_lpk_ovsht_2                 ; // 14
    UINT32  img1_pkti_lti_gain                    ; // 15
    UINT32  img1_pkti_lti_lut_1                   ; // 16
    UINT32  img1_pkti_lti_lut_2                   ; // 17
    UINT32  img1_pkti_lti_lut_3                   ; // 18
    UINT32  img1_pkti_lti_lut_4                   ; // 19
    UINT32  img1_pkti_lti_lut_5                   ; // 20
    UINT32  img1_pkti_lti_lut_6                   ; // 21
    UINT32  img1_pkti_lti_ovsht_1                 ; // 22
    UINT32  img1_pkti_lti_ovsht_2                 ; // 23
    UINT32  img1_sram_ctrl                        ; // 24
    UINT32  img1_sram_addr                        ; // 25
    UINT32  img1_sram_write_data                  ; // 26
    UINT32  img1_sram_read_data                   ; // 27
    UINT32  img1_pix_no                           ; // 28
    UINT32  img1_line_no                          ; // 29
    UINT32  img1_chksum_ans                       ; // 30
    UINT32  g198_reserved0                        ; // 31    

    // Group 199 : VPOST0
    UINT32  vpost0_config                         ; // 00
    UINT32  vpost0_adj_config                     ; // 01
    UINT32  vpost0_adj_src                        ; // 02
    UINT32  vpost0_adj_des                        ; // 03
    UINT32  vpost0_adj_slope0                     ; // 04
    UINT32  vpost0_adj_slope1                     ; // 05
    UINT32  vpost0_adj_slope2                     ; // 06
    UINT32  vpost0_adj_bound                      ; // 07
    UINT32  vpost0_cspace_config                  ; // 08
    UINT32  vpost0_opif_config                    ; // 09
    UINT32  vpost0_opif_bgy                       ; // 10
    UINT32  vpost0_opif_bguv                      ; // 11
    UINT32  vpost0_opif_alpha                     ; // 12
    UINT32  vpost0_opif_msktop                    ; // 13
    UINT32  vpost0_opif_mskbot                    ; // 14
    UINT32  vpost0_xlen                           ; // 15
    UINT32  vpost0_ylen                           ; // 16
    UINT32  vpost0_chksum_ans                     ; // 17
    UINT32  g199_reserved0[14]                    ; // 18~31

    // Group 200 : VPOST1
    UINT32  vpost1_config                         ; // 00
    UINT32  vpost1_adj_config                     ; // 01
    UINT32  vpost1_adj_src                        ; // 02
    UINT32  vpost1_adj_des                        ; // 03
    UINT32  vpost1_adj_slope0                     ; // 04
    UINT32  vpost1_adj_slope1                     ; // 05
    UINT32  vpost1_adj_slope2                     ; // 06
    UINT32  vpost1_adj_bound                      ; // 07
    UINT32  vpost1_cspace_config                  ; // 08
    UINT32  vpost1_opif_config                    ; // 09
    UINT32  vpost1_opif_bgy                       ; // 10
    UINT32  vpost1_opif_bguv                      ; // 11
    UINT32  vpost1_opif_alpha                     ; // 12
    UINT32  vpost1_opif_msktop                    ; // 13
    UINT32  vpost1_opif_mskbot                    ; // 14
    UINT32  vpost1_xlen                           ; // 15
    UINT32  vpost1_ylen                           ; // 16
    UINT32  vpost1_chksum_ans                     ; // 17
    UINT32  g200_reserved0[14]                    ; // 18~31

    // Group 201 : HD2SD
    UINT32 hd2sd_config;                      ; 	//0  
    UINT32 hd2sd_user_hexp0;                  ; 	//1 
    UINT32 hd2sd_user_hexp1;                  ; 	//2
    UINT32 hd2sd_user_vexp0;                  ; 	//3
    UINT32 hd2sd_user_vexp1;                  ; 	//4
    UINT32 hd2sd_user_vexp2;                  ; 	//5
    UINT32 hd2sd_user_sd_reset0;              ; 	//6
    UINT32 hd2sd_user_sd_reset1;              ; 	//7 
    UINT32 hd2sd_status;                      ; 	//8 
    UINT32 hd2sd_peaking_config;              ; 	//9 
    UINT32 hd2sd_peaking_lut0;                ; 	//10
    UINT32 hd2sd_peaking_lut1;                ; 	//11
    UINT32 hd2sd_peaking_limit;               ; 	//12
    UINT32 hd2sd_hd_checksum;                 ; 	//13
    UINT32 hd2sd_sd_checksum;                 ; 	//14
    UINT32 hd2sd_user_sdtve_vrst0;            ; 	//15
    UINT32 hd2sd_user_sdtve_vrst1;            ; 	//16
    UINT32 hd2sd_user_sdtve_hrst;             ; 	//17
    UINT32 hd2sd_bgc_y;                       ; 	//18
    UINT32 hd2sd_bgc_c;                       ; 	//19
    UINT32 hd2sd_config2;                     ; 	//20
    UINT32 hd2sd_user_hact;                   ; 	//21
    UINT32 hd2sd_user_vact;                   ; 	//22
    UINT32 hd2sd_tve_user_mode;               ; 	//23
    UINT32 g201_reserved_0[8];

    // Group 202 : AFRC
    UINT32 afrc_mode0        		      ; 	//0 
    UINT32 afrc_fb_ctrl0     		      ; 	//1 
    UINT32 afrc_fb_ctrl1     		      ; 	//2
    UINT32 afrc_bist_mode0   		      ; 	//3
    UINT32 afrc_bist_mode1   		      ; 	//4
    UINT32 afrc_urgent_thd   		      ; 	//5
    UINT32 afrc_iw_req_clkcnt		      ; 	//6
    UINT32 afrc_faddr0_lw    		      ; 	//7 
    UINT32 afrc_faddr0_hw    		      ; 	//8 
    UINT32 afrc_fb_step      		      ; 	//9 
    UINT32 afrc_tgr_hact     		      ; 	//10
    UINT32 afrc_tgr_htot     		      ; 	//11 
    UINT32 afrc_tgr_vact_st_t		      ; 	//12 
    UINT32 afrc_tgr_vact_ed_t		      ; 	//13 
    UINT32 afrc_tgr_vact_st_b		      ; 	//14  
    UINT32 afrc_tgr_vact_ed_b		      ; 	//15 
    UINT32 afrc_tgr_vtot     		      ; 	//16
    UINT32 afrc_sync_vcnt    		      ; 	//17
    UINT32 afrc_sync_hcnt    		      ; 	//18
    UINT32 afrc_out_vact_st_t		      ; 	//19
    UINT32 afrc_out_vact_st_b		      ; 	//20
    UINT32 afrc_irq_line     		      ; 	//21
    UINT32 afrc_debug        		      ; 	//22
    UINT32 afrc_chkout_a     		      ; 	//23
    UINT32 afrc_chkout_b     		      ; 	//24
    UINT32 afrc_reserved_0   		      ; 	//25
    UINT32 afrc_reserved_1   		      ; 	//26
    UINT32 afrc_reserved_2   		      ; 	//27
    UINT32 afrc_rw_sync2   		      ; 	//28
    UINT32 afrc_mode1        		      ; 	//29 
    UINT32 afrc_mode2        		      ; 	//30 
    UINT32 afrc_rw_sync        		      ; 	//31 

    // Group 203 : TGEN
    UINT32  tgen_0_config                     ; // 00
    UINT32  tgen_0_reset                      ; // 01
    UINT32  tgen_0_user_int1_config           ; // 02
    UINT32  g203_reserved_0                   ; // 03
    UINT32  tgen_dtg_0_config                 ; // 04
    UINT32  g203_reserved_1                   ; // 05
    UINT32  tgen_dtg_0_adjust2                ; // 06
    UINT32  g203_reserved_2                   ; // 07
    UINT32  tgen_dtg_0_total_pixel            ; // 08
    UINT32  tgen_dtg_0_ds_line_start_cd_point ; // 09
    UINT32  tgen_dtg_0_total_line             ; // 10
    UINT32  tgen_dtg_0_field_end_line         ; // 11
    UINT32  tgen_dtg_0_tve_line_rst_cnt       ; // 12
    UINT32  tgen_dtg_0_status1                ; // 13
    UINT32  tgen_dtg_0_status2                ; // 14
    UINT32  tgen_dtg_0_start_line             ; // 15
    UINT32  tgen_atg_0_config                 ; // 16
    UINT32  tgen_atg_0_clk_ratio_low          ; // 17
    UINT32  tgen_atg_0_clk_ratio_high         ; // 18
    UINT32  tgen_atg_0_total_pixel            ; // 19
    UINT32  tgen_atg_0_first_line_total_pixel ; // 20
    UINT32  tgen_atg_0_field_end_line         ; // 21
    UINT32  tgen_atg_0_ds_line_start_cd_point ; // 22
    UINT32  tgen_atg_0_adjust1                ; // 23
    UINT32  tgen_atg_0_adjust2                ; // 24
    UINT32  tgen_atg_0_adjust3                ; // 25
    UINT32  tgen_atg_0_adjust4                ; // 26
    UINT32  tgen_atg_0_status1                ; // 27
    UINT32  tgen_atg_0_status2                ; // 28
    UINT32  tgen_atg_0_source_sel             ; // 29
    UINT32  g203_reserved3[2]                 ; // 30~31

    // Group 204 : TGEN
    UINT32  tgen_1_config                     ; // 00
    UINT32  tgen_1_reset                      ; // 01
    UINT32  tgen_1_user_int1_config           ; // 02
    UINT32  g204_reserved_0                   ; // 03
    UINT32  tgen_dtg_1_config                 ; // 04
    UINT32  g204_reserved_1                   ; // 05
    UINT32  tgen_dtg_1_adjust2                ; // 06
    UINT32  g204_reserved_2                   ; // 07
    UINT32  tgen_dtg_1_total_pixel            ; // 08
    UINT32  tgen_dtg_1_ds_line_start_cd_point ; // 09
    UINT32  tgen_dtg_1_total_line             ; // 10
    UINT32  tgen_dtg_1_field_end_line         ; // 11
    UINT32  tgen_dtg_1_tve_line_rst_cnt       ; // 12
    UINT32  tgen_dtg_1_status1                ; // 13
    UINT32  tgen_dtg_1_status2                ; // 14
    UINT32  tgen_dtg_1_start_line             ; // 15
    UINT32  tgen_atg_1_config                 ; // 16
    UINT32  tgen_atg_1_clk_ratio_low          ; // 17
    UINT32  tgen_atg_1_clk_ratio_high         ; // 18
    UINT32  tgen_atg_1_total_pixel            ; // 19
    UINT32  tgen_atg_1_first_line_total_pixel ; // 20
    UINT32  tgen_atg_1_field_end_line         ; // 21
    UINT32  tgen_atg_1_ds_line_start_cd_point ; // 22
    UINT32  tgen_atg_1_adjust1                ; // 23
    UINT32  tgen_atg_1_adjust2                ; // 24
    UINT32  tgen_atg_1_adjust3                ; // 25
    UINT32  tgen_atg_1_adjust4                ; // 26
    UINT32  tgen_atg_1_status1                ; // 27
    UINT32  tgen_atg_1_status2                ; // 28
    UINT32  tgen_atg_1_source_sel             ; // 29
    UINT32  g204_reserved3[2]                 ; // 30~31

    // Group 205 : AMIX
    UINT32  amix_0_config0                    ; // 00
    UINT32  amix_0_config1                    ; // 01
    UINT32  amix_0_config2                    ; // 02
    UINT32  amix_0_plane_alpha                ; // 03
    UINT32  amix_0_plane_alpha2               ; // 04
    UINT32  amix_0_plane_alpha3               ; // 05
    UINT32  g205_reserved0[8]                 ; // 06~13
    UINT32  amix_0_bgc_y                      ; // 14
    UINT32  amix_0_bgc_c                      ; // 15
    UINT32  g205_reserved1                    ; // 16
    UINT32  amix_0_ptg_config                 ; // 17
    UINT32  amix_0_ptg_config_2               ; // 18 
    UINT32  amix_0_ptg_config_3               ; // 19
    UINT32  amix_0_ptg_config_4               ; // 20 
    UINT32  amix_0_ptg_config_5               ; // 21
    UINT32  amix_0_ptg_config_6               ; // 22
    UINT32  g205_reserved2[9]                 ; // 23~31

    // Group 206 : AMIX
    UINT32  amix_1_config0                    ; // 00
    UINT32  amix_1_config1                    ; // 01
    UINT32  amix_1_config2                    ; // 02
    UINT32  amix_1_plane_alpha                ; // 03
    UINT32  amix_1_plane_alpha2               ; // 04
    UINT32  amix_1_plane_alpha3               ; // 05
    UINT32  g206_reserved0[8]                 ; // 06~13
    UINT32  amix_1_bgc_y                      ; // 14
    UINT32  amix_1_bgc_c                      ; // 15
    UINT32  g206_reserved1                    ; // 16
    UINT32  amix_1_ptg_config                 ; // 17
    UINT32  amix_1_ptg_config_2               ; // 18 
    UINT32  amix_1_ptg_config_3               ; // 19
    UINT32  amix_1_ptg_config_4               ; // 20 
    UINT32  amix_1_ptg_config_5               ; // 21
    UINT32  amix_1_ptg_config_6               ; // 22
    UINT32  g206_reserved2[9]                 ; // 23~31

    // Group 207 : DMIX
    UINT32  dmix_0_config                     ; // 00
    UINT32  g207_reserved0[3]                 ; // 01~3
    UINT32  dmix_0_yc_adjust                  ; // 04
    UINT32  dmix_0_luma_cp1                   ; // 05
    UINT32  dmix_0_luma_cp2                   ; // 06
    UINT32  dmix_0_luma_cp3                   ; // 07
    UINT32  dmix_0_luma_slope0                ; // 08
    UINT32  dmix_0_luma_slope1                ; // 09
    UINT32  dmix_0_luma_slope2                ; // 10
    UINT32  dmix_0_luma_slope3                ; // 11
    UINT32  dmix_0_chroma_satcos              ; // 12
    UINT32  dmix_0_chroma_satsin              ; // 13
    UINT32  dmix_0_checksum_setting           ; // 14
    UINT32  dmix_0_chksum_usermode_V          ; // 15
    UINT32  dmix_0_chksum_usermode_H          ; // 16
    UINT32  dmix_0_chksum_usermode_start_line ; // 17
    UINT32  dmix_0_to_venc_chksum             ; // 18
    UINT32  dmix_0_to_hd2sd_chksum            ; // 19
    UINT32  dmix_0_fgc_y_alpha                ; // 20
    UINT32  dmix_0_fgc_c                      ; // 21
    UINT32  dmix_0_pix_en_sel                 ; // 22
    UINT32  dmix_0_snr_set                    ; // 23
    UINT32  dmix_0_snr_start_set              ; // 24
    UINT32  g207_reserved1[7]                 ; // 25~31

    // Group 208 : DMIX
    UINT32  dmix_1_config                     ; // 00
    UINT32  g208_reserved0[3]                 ; // 01~3
    UINT32  dmix_1_yc_adjust                  ; // 04
    UINT32  dmix_1_luma_cp1                   ; // 05
    UINT32  dmix_1_luma_cp2                   ; // 06
    UINT32  dmix_1_luma_cp3                   ; // 07
    UINT32  dmix_1_luma_slope0                ; // 08
    UINT32  dmix_1_luma_slope1                ; // 09
    UINT32  dmix_1_luma_slope2                ; // 10
    UINT32  dmix_1_luma_slope3                ; // 11
    UINT32  dmix_1_chroma_satcos              ; // 12
    UINT32  dmix_1_chroma_satsin              ; // 13
    UINT32  dmix_1_checksum_setting           ; // 14
    UINT32  dmix_1_chksum_usermode_V          ; // 15
    UINT32  dmix_1_chksum_usermode_H          ; // 16
    UINT32  dmix_1_chksum_usermode_start_line ; // 17
    UINT32  dmix_1_to_venc_chksum             ; // 18
    UINT32  dmix_1_to_hd2sd_chksum            ; // 19
    UINT32  dmix_1_fgc_y_alpha                ; // 20
    UINT32  dmix_1_fgc_c                      ; // 21
    UINT32  dmix_1_pix_en_sel                 ; // 22
    UINT32  dmix_1_snr_set                    ; // 23
    UINT32  dmix_1_snr_start_set              ; // 24
    UINT32  g208_reserved1[7]                 ; // 25~31

    // Group 209 : VENC
    UINT32  tv_mode_0                             ; // 00
    UINT32  tv_mode_1                             ; // 01
    UINT32  tv_mode_2                             ; // 02
    UINT32  tv_mode_3                             ; // 03
    UINT32  tv_mode_4                             ; // 04
    UINT32  tv_mode_5                             ; // 05
    UINT32  tv_subc_f_0                           ; // 06
    UINT32  tv_subc_f_1                           ; // 07
    UINT32  tv_subc_p                             ; // 08
    UINT32  tv_line_t_0                           ; // 09
    UINT32  tv_line_t_1                           ; // 10
    UINT32  tv_line_b_0                           ; // 11
    UINT32  tv_line_b_1                           ; // 12
    UINT32  tv_cc_t                               ; // 13
    UINT32  tv_cc_b                               ; // 14
    UINT32  tv_cgms_0                             ; // 15
    UINT32  tv_cgms_1                             ; // 16
    UINT32  tv_id_status                          ; // 17
    UINT32  tv_dac_0                              ; // 18
    UINT32  tv_reserved0_gxx                      ; // 19
    UINT32  g209_reserved0[9]                     ; // 20~28
    UINT32  tv_y_scf                              ; // 29
    UINT32  tv_y_scf_pc                           ; // 30
    UINT32  tv_temp0_g0                           ; // 31

    // Group 210 : VENC
    UINT32  tv_gamma_0                            ; // 00
    UINT32  tv_gamma_1                            ; // 01
    UINT32  tv_gamma_2                            ; // 02
    UINT32  tv_gamma_3                            ; // 03
    UINT32  tv_gamma_4                            ; // 04
    UINT32  tv_pccon_0                            ; // 05
    UINT32  tv_pccon_1                            ; // 06
    UINT32  tv_pccon_2                            ; // 07
    UINT32  tv_pccon_3                            ; // 08
    UINT32  tv_pccon_4                            ; // 09
    UINT32  tv_pccon_5                            ; // 10
    UINT32  tv_pccon_6                            ; // 11
    UINT32  tv_pccon_7                            ; // 12
    UINT32  tv_pccon_8                            ; // 13
    UINT32  tv_pccon_9                            ; // 14
    UINT32  tv_pccon_10                           ; // 15
    UINT32  tv_pccon_11                           ; // 16
    UINT32  tv_pccon_12                           ; // 17
    UINT32  tv_pccon_13                           ; // 18
    UINT32  tv_pccon_14                           ; // 19
    UINT32  tv_pccon_15                           ; // 20
    UINT32  tv_pccon_16                           ; // 21
    UINT32  tv_pccon_17                           ; // 22
    UINT32  tv_pccon_18                           ; // 23
    UINT32  tv_temp0_g1                           ; // 24
    UINT32  tv_temp1_g1                           ; // 25
    UINT32  tv_dac_checksum                       ; // 26
    UINT32  tv_mode5                              ; // 27
    UINT32  tv_mode6                              ; // 28
    UINT32  tv_mode7                              ; // 29
    UINT32  tv_clk_setting                        ; // 30
    UINT32  tv_pccon_19                           ; // 31

    // Group 211 : VENC
    UINT32  tv2_mode0                             ; // 00
    UINT32  rg_scm_adj                            ; // 01
    UINT32  g211_reserved0[2]                     ; // 02~03
    UINT32  tv2_dac_cor0                          ; // 04
    UINT32  tv2_dac_cor1                          ; // 05
    UINT32  tv2_dac_cor2                          ; // 06
    UINT32  tv2_wc_tline                          ; // 07
    UINT32  tv2_wc_bline                          ; // 08
    UINT32  tv2_wc_psline                         ; // 09
    UINT32  tv2_mode1                             ; // 10
    UINT32  tv2_mode2                             ; // 11
    UINT32  tv2_mode3                             ; // 12
    UINT32  tv2_y_range                           ; // 13
    UINT32  tv2_c_range                           ; // 14
    UINT32  tv2_cgmsb_0                           ; // 15
    UINT32  tv2_cgmsb_1                           ; // 16
    UINT32  tv2_cgmsb_2                           ; // 17
    UINT32  tv2_cgmsb_3                           ; // 18
    UINT32  tv2_vpd_mode0                         ; // 19
    UINT32  tv2_vpd_mode1                         ; // 20
    UINT32  tv2_vpd_thd                           ; // 21
    UINT32  tv2_vpd_stline                        ; // 22
    UINT32  tv2_vpd_edline                        ; // 23
    UINT32  tv2_vpd_vldcnt                        ; // 24
    UINT32  tv2_vpd_mode2                         ; // 25
    UINT32  g211_reserved1                        ; // 26
    UINT32  tv_hd2sd_pnt                          ; // 27
    UINT32  tv_pccon_20                           ; // 28
    UINT32  tv_pccon_21                           ; // 29
    UINT32  tv_pccon_22                           ; // 30
    UINT32  tv_pccon_23                           ; // 31

    // Group 212 : VENC
    UINT32  hdtv_mode0                            ; // 00
    UINT32  hdtv_mode1                            ; // 01
    UINT32  hdtv_usr_intl_pix                     ; // 02
    UINT32  hdtv_usr_intl_line                    ; // 03
    UINT32  hdtv_cgms_lword                       ; // 04
    UINT32  hdtv_cgms_hword                       ; // 05
    UINT32  hdtv_cgms_a_ps_line                   ; // 06
    UINT32  hdtv_cgms_a_vt_line                   ; // 07
    UINT32  hdtv_cgms_a_vb_line                   ; // 08
    UINT32  hdtv_y_range                          ; // 09
    UINT32  hdtv_c_range                          ; // 10
    UINT32  hdtv_mode2                            ; // 11
    UINT32  hdtv_cgms_b_ps_line                   ; // 12
    UINT32  g212_reserved0                        ; // 13
    UINT32  use_free_hdgated                      ; // 14
    UINT32  hd_latch_mode                         ; // 15
    UINT32  g212_reserved1[16]                    ; // 16~31

    // Group 213 : G2D
    UINT32  g2d_ctrl                              ; // 00
    UINT32  g2d_en                                ; // 01
    UINT32  g2d_tlink_addr_l                      ; // 02
    UINT32  g2d_tlink_addr_h                      ; // 03
    UINT32  g213_reserved0[4]                     ; // 04~07
    UINT32  g2d_req_ctrl                          ; // 08
    UINT32  g2d_deflicker_ctrl                    ; // 09
    UINT32  g2d_vexp_step                         ; // 10
    UINT32  g2d_hexp_step                         ; // 11
    UINT32  g213_reserved1[2]                     ; // 12~13
    UINT32  g2d_crc_chksum                        ; // 14
    UINT32  g2d_status                            ; // 15
    UINT32  g2d_hvld_offset                       ; // 16
    UINT32  g2d_hvld_width                        ; // 17
    UINT32  g2d_vvld_offset                       ; // 18
    UINT32  g2d_vvld_height                       ; // 19
    UINT32  g2d_data_fetch_ctrl                   ; // 20
    UINT32  g2d_bist_ctrl                         ; // 21
    UINT32  g2d_vexp_init                         ; // 22
    UINT32  g2d_hexp_init                         ; // 23
    UINT32  g2d_vexp_step_ext                     ; // 24
    UINT32  g2d_hexp_step_ext                     ; // 25
    UINT32  g2d_hexp_flt_sel                      ; // 26
    UINT32  g2d_3d_h_offset                       ; // 27
    UINT32  g2d_vexp_flt_sel                      ; // 28
    UINT32  g2d_src_decimation_sel                ; // 29
    UINT32  g2d_lb_info                           ; // 30
    UINT32  g2d_mbus_Info                         ; // 31

    // Group 214 : OSD0
    UINT32  osd0_ctrl                             ; // 00
    UINT32  osd0_en                               ; // 01
    UINT32  osd0_tlink_addr_l                     ; // 02
    UINT32  osd0_tlink_addr_h                     ; // 03
    UINT32  g214_reserved0[4]                     ; // 04~07
    UINT32  osd0_req_ctrl                         ; // 08
    UINT32  osd0_deflicker_ctrl                   ; // 09
    UINT32  osd0_vexp_step                        ; // 10
    UINT32  osd0_hexp_step                        ; // 11
    UINT32  g214_reserved1[2]                     ; // 12~13
    UINT32  osd0_crc_chksum                       ; // 14
    UINT32  osd0_status                           ; // 15
    UINT32  osd0_hvld_offset                      ; // 16
    UINT32  osd0_hvld_width                       ; // 17
    UINT32  osd0_vvld_offset                      ; // 18
    UINT32  osd0_vvld_height                      ; // 19
    UINT32  osd0_data_fetch_ctrl                  ; // 20
    UINT32  osd0_bist_ctrl                        ; // 21
    UINT32  osd0_vexp_init                        ; // 22
    UINT32  osd0_hexp_init                        ; // 23
    UINT32  osd0_vexp_step_ext                    ; // 24
    UINT32  osd0_hexp_step_ext                    ; // 25
    UINT32  osd0_hexp_flt_sel                     ; // 26
    UINT32  osd0_3d_h_offset                      ; // 27
    UINT32  osd0_vexp_flt_sel                     ; // 28
    UINT32  osd0_src_decimation_sel               ; // 29
    UINT32  osd0_lb_info                          ; // 30
    UINT32  osd0_mbus_Info                        ; // 31

    // Group 215 : OSD1
    UINT32  osd1_ctrl                             ; // 00
    UINT32  osd1_en                               ; // 01
    UINT32  osd1_tlink_addr_l                     ; // 02
    UINT32  osd1_tlink_addr_h                     ; // 03
    UINT32  g215_reserved0[4]                     ; // 04~07
    UINT32  osd1_req_ctrl                         ; // 08
    UINT32  osd1_deflicker_ctrl                   ; // 09
    UINT32  osd1_vexp_step                        ; // 10
    UINT32  osd1_hexp_step                        ; // 11
    UINT32  g215_reserved1[2]                     ; // 12~13
    UINT32  osd1_crc_chksum                       ; // 14
    UINT32  osd1_status                           ; // 15
    UINT32  osd1_hvld_offset                      ; // 16
    UINT32  osd1_hvld_width                       ; // 17
    UINT32  osd1_vvld_offset                      ; // 18
    UINT32  osd1_vvld_height                      ; // 19
    UINT32  osd1_data_fetch_ctrl                  ; // 20
    UINT32  osd1_bist_ctrl                        ; // 21
    UINT32  osd1_vexp_init                        ; // 22
    UINT32  osd1_hexp_init                        ; // 23
    UINT32  osd1_vexp_step_ext                    ; // 24
    UINT32  osd1_hexp_step_ext                    ; // 25
    UINT32  osd1_hexp_flt_sel                     ; // 26
    UINT32  osd1_3d_h_offset                      ; // 27
    UINT32  osd1_vexp_flt_sel                     ; // 28
    UINT32  osd1_src_decimation_sel               ; // 29
    UINT32  osd1_lb_info                          ; // 30
    UINT32  osd1_mbus_Info                        ; // 31

    // Group 216 : GSCL0
    UINT32  gscl0_config1                         ; // 00
    UINT32  gscl0_config2                         ; // 01
    UINT32  g216_reserved0[2]                     ; // 02~03
    UINT32  gscl0_actrl_i_xlen                    ; // 04
    UINT32  gscl0_actrl_i_ylen                    ; // 05
    UINT32  gscl0_actrl_s_xstart                  ; // 06
    UINT32  gscl0_actrl_s_ystart                  ; // 07
    UINT32  gscl0_actrl_s_xlen                    ; // 08
    UINT32  gscl0_actrl_s_ylen                    ; // 09
    UINT32  gscl0_dctrl_o_xlen                    ; // 10
    UINT32  gscl0_dctrl_o_ylen                    ; // 11
    UINT32  gscl0_dctrl_d_xstart                  ; // 12
    UINT32  gscl0_dctrl_d_ystart                  ; // 13
    UINT32  gscl0_dctrl_d_xlen                    ; // 14
    UINT32  gscl0_dctrl_d_ylen                    ; // 15
    UINT32  gscl0_dctrl_bgc_c                     ; // 16
    UINT32  gscl0_dctrl_bgc_y                     ; // 17
    UINT32  g216_reserved1                        ; // 18
    UINT32  gscl0_hint_ctrl                       ; // 19
    UINT32  gscl0_hint_hfactor_low                ; // 20
    UINT32  gscl0_hint_hfactor_high               ; // 21
    UINT32  gscl0_hint_initf_low                  ; // 22
    UINT32  gscl0_hint_initf_high                 ; // 23
    UINT32  gscl0_hint_overshoot                  ; // 24
    UINT32  g216_reserved2                        ; // 25
    UINT32  gscl0_vint_ctrl                       ; // 26
    UINT32  gscl0_vint_vfactor_low                ; // 27
    UINT32  gscl0_vint_vfactor_high               ; // 28
    UINT32  gscl0_vint_initf_low                  ; // 29
    UINT32  gscl0_vint_initf_high                 ; // 30
    UINT32  g216_reserved3                        ; // 31
    
    // Group 217 : GSCL0
    UINT32  gscl0_vimg_ctrl                       ; // 00
    UINT32  gscl0_vimg_vpeak_lut_1                ; // 01
    UINT32  gscl0_vimg_vpeak_lut_2                ; // 02
    UINT32  gscl0_vimg_vpeak_lut_3                ; // 03
    UINT32  gscl0_vimg_vpeak_lut_4                ; // 04
    UINT32  gscl0_vimg_vpeak_lut_5                ; // 05
    UINT32  gscl0_vimg_vpeak_lut_6                ; // 06
    UINT32  gscl0_vimg_vpeak_config               ; // 07
    UINT32  gscl0_vimg_diag_config_1              ; // 08
    UINT32  gscl0_vimg_diag_config_2              ; // 09
    UINT32  g217_reserved0                        ; // 10
    UINT32  gscl0_checksum_select                 ; // 11
    UINT32  gscl0_checksum_result                 ; // 12
    UINT32  g217_reserved1[3]                     ; // 13~15
    UINT32  gscl0_dr_ctrl                         ; // 16
    UINT32  gscl0_dr_dege_detect1                 ; // 17
    UINT32  gscl0_dr_dege_detect2                 ; // 18
    UINT32  gscl0_dr_clamp_detect                 ; // 19
    UINT32  g217_reserved2[12]                    ; // 20~31

    // Group 218 : GSCL1
    UINT32  gscl1_config1                         ; // 00
    UINT32  gscl1_config2                         ; // 01
    UINT32  g218_reserved0[2]                     ; // 02~03
    UINT32  gscl1_actrl_i_xlen                    ; // 04
    UINT32  gscl1_actrl_i_ylen                    ; // 05
    UINT32  gscl1_actrl_s_xstart                  ; // 06
    UINT32  gscl1_actrl_s_ystart                  ; // 07
    UINT32  gscl1_actrl_s_xlen                    ; // 08
    UINT32  gscl1_actrl_s_ylen                    ; // 09
    UINT32  gscl1_dctrl_o_xlen                    ; // 10
    UINT32  gscl1_dctrl_o_ylen                    ; // 11
    UINT32  gscl1_dctrl_d_xstart                  ; // 12
    UINT32  gscl1_dctrl_d_ystart                  ; // 13
    UINT32  gscl1_dctrl_d_xlen                    ; // 14
    UINT32  gscl1_dctrl_d_ylen                    ; // 15
    UINT32  gscl1_dctrl_bgc_c                     ; // 16
    UINT32  gscl1_dctrl_bgc_y                     ; // 17
    UINT32  g218_reserved1                        ; // 18
    UINT32  gscl1_hint_ctrl                       ; // 19
    UINT32  gscl1_hint_hfactor_low                ; // 20
    UINT32  gscl1_hint_hfactor_high               ; // 21
    UINT32  gscl1_hint_initf_low                  ; // 22
    UINT32  gscl1_hint_initf_high                 ; // 23
    UINT32  gscl1_hint_overshoot                  ; // 24
    UINT32  g218_reserved2                        ; // 25
    UINT32  gscl1_vint_ctrl                       ; // 26
    UINT32  gscl1_vint_vfactor_low                ; // 27
    UINT32  gscl1_vint_vfactor_high               ; // 28
    UINT32  gscl1_vint_initf_low                  ; // 29
    UINT32  gscl1_vint_initf_high                 ; // 30
    UINT32  g218_reserved3                        ; // 31

    // Group 219 : GSCL1
    UINT32  gscl1_vimg_ctrl                       ; // 00
    UINT32  gscl1_vimg_vpeak_lut_1                ; // 01
    UINT32  gscl1_vimg_vpeak_lut_2                ; // 02
    UINT32  gscl1_vimg_vpeak_lut_3                ; // 03
    UINT32  gscl1_vimg_vpeak_lut_4                ; // 04
    UINT32  gscl1_vimg_vpeak_lut_5                ; // 05
    UINT32  gscl1_vimg_vpeak_lut_6                ; // 06
    UINT32  gscl1_vimg_vpeak_config               ; // 07
    UINT32  gscl1_vimg_diag_config_1              ; // 08
    UINT32  gscl1_vimg_diag_config_2              ; // 09
    UINT32  g219_reserved0                        ; // 10
    UINT32  gscl1_checksum_select                 ; // 11
    UINT32  gscl1_checksum_result                 ; // 12
    UINT32  g219_reserved1[3]                     ; // 13~15
    UINT32  gscl1_dr_ctrl                         ; // 16
    UINT32  gscl1_dr_dege_detect1                 ; // 17
    UINT32  gscl1_dr_dege_detect2                 ; // 18
    UINT32  gscl1_dr_clamp_detect                 ; // 19
    UINT32  g219_reserved2[12]                    ; // 20~31

    // Group 220 : GSCL2
    UINT32  gscl2_config1                         ; // 00
    UINT32  gscl2_config2                         ; // 01
    UINT32  g220_reserved0[2]                     ; // 02~03
    UINT32  gscl2_actrl_i_xlen                    ; // 04
    UINT32  gscl2_actrl_i_ylen                    ; // 05
    UINT32  gscl2_actrl_s_xstart                  ; // 06
    UINT32  gscl2_actrl_s_ystart                  ; // 07
    UINT32  gscl2_actrl_s_xlen                    ; // 08
    UINT32  gscl2_actrl_s_ylen                    ; // 09
    UINT32  gscl2_dctrl_o_xlen                    ; // 10
    UINT32  gscl2_dctrl_o_ylen                    ; // 11
    UINT32  gscl2_dctrl_d_xstart                  ; // 12
    UINT32  gscl2_dctrl_d_ystart                  ; // 13
    UINT32  gscl2_dctrl_d_xlen                    ; // 14
    UINT32  gscl2_dctrl_d_ylen                    ; // 15
    UINT32  gscl2_dctrl_bgc_c                     ; // 16
    UINT32  gscl2_dctrl_bgc_y                     ; // 17
    UINT32  g220_reserved1                        ; // 18
    UINT32  gscl2_hint_ctrl                       ; // 19
    UINT32  gscl2_hint_hfactor_low                ; // 20
    UINT32  gscl2_hint_hfactor_high               ; // 21
    UINT32  gscl2_hint_initf_low                  ; // 22
    UINT32  gscl2_hint_initf_high                 ; // 23
    UINT32  gscl2_hint_overshoot                  ; // 24
    UINT32  g220_reserved2                        ; // 25
    UINT32  gscl2_vint_ctrl                       ; // 26
    UINT32  gscl2_vint_vfactor_low                ; // 27
    UINT32  gscl2_vint_vfactor_high               ; // 28
    UINT32  gscl2_vint_initf_low                  ; // 29
    UINT32  gscl2_vint_initf_high                 ; // 30
    UINT32  g220_reserved3                        ; // 31

    // Group 221 : GSCL2
    UINT32  gscl2_vimg_ctrl                       ; // 00
    UINT32  gscl2_vimg_vpeak_lut_1                ; // 01
    UINT32  gscl2_vimg_vpeak_lut_2                ; // 02
    UINT32  gscl2_vimg_vpeak_lut_3                ; // 03
    UINT32  gscl2_vimg_vpeak_lut_4                ; // 04
    UINT32  gscl2_vimg_vpeak_lut_5                ; // 05
    UINT32  gscl2_vimg_vpeak_lut_6                ; // 06
    UINT32  gscl2_vimg_vpeak_config               ; // 07
    UINT32  gscl2_vimg_diag_config_1              ; // 08
    UINT32  gscl2_vimg_diag_config_2              ; // 09
    UINT32  g221_reserved0                        ; // 10
    UINT32  gscl2_checksum_select                 ; // 11
    UINT32  gscl2_checksum_result                 ; // 12
    UINT32  g221_reserved1[3]                     ; // 13~15
    UINT32  gscl2_dr_ctrl                         ; // 16
    UINT32  gscl2_dr_dege_detect1                 ; // 17
    UINT32  gscl2_dr_dege_detect2                 ; // 18
    UINT32  gscl2_dr_clamp_detect                 ; // 19
    UINT32  g221_reserved2[12]                    ; // 20~31

    // Group 222 : HTDS0
    UINT32  htds0_latch_mode                      ; // 00
    UINT32  htds0_config                          ; // 01
    UINT32  htds0_line_width                      ; // 02
    UINT32  htds0_pix_st_r0                       ; // 03
    UINT32  htds0_pix_ed_r0                       ; // 04
    UINT32  htds0_shift_r0                        ; // 05
    UINT32  htds0_pix_st_r1                       ; // 06
    UINT32  htds0_pix_ed_r1                       ; // 07
    UINT32  htds0_shift_r1                        ; // 08
    UINT32  htds0_pix_st_r2                       ; // 09
    UINT32  htds0_pix_ed_r2                       ; // 10
    UINT32  htds0_shift_r2                        ; // 11
    UINT32  htds0_pix_st_r3                       ; // 12
    UINT32  htds0_pix_ed_r3                       ; // 13
    UINT32  htds0_shift_r3                        ; // 14
    UINT32  htds0_checksum                        ; // 15
    UINT32  g222_reserved[16]                     ; // 16~31

    // Group 223 : HTDS1
    UINT32  htds1_latch_mode                      ; //0
    UINT32  htds1_config                          ; //1
    UINT32  htds1_line_width                      ; //2
    UINT32  htds1_pix_st_r0                       ; //3
    UINT32  htds1_pix_ed_r0                       ; //4
    UINT32  htds1_shift_r0                        ; //5
    UINT32  htds1_pix_st_r1                       ; //6
    UINT32  htds1_pix_ed_r1                       ; //7
    UINT32  htds1_shift_r1                        ; //8
    UINT32  htds1_pix_st_r2                       ; //9
    UINT32  htds1_pix_ed_r2                       ; //10
    UINT32  htds1_shift_r2                        ; //11
    UINT32  htds1_pix_st_r3                       ; //12
    UINT32  htds1_pix_ed_r3                       ; //13
    UINT32  htds1_shift_r3                        ; //14
    UINT32  htds1_checksum                        ; //15
    UINT32  g223_reserved[16]                     ; //16~31

    // Group 224 : HTDS2
    UINT32  htds2_latch_mode                      ; // 00
    UINT32  htds2_config                          ; // 01
    UINT32  htds2_line_width                      ; // 02
    UINT32  htds2_pix_st_r0                       ; // 03
    UINT32  htds2_pix_ed_r0                       ; // 04
    UINT32  htds2_shift_r0                        ; // 05
    UINT32  htds2_pix_st_r1                       ; // 06
    UINT32  htds2_pix_ed_r1                       ; // 07
    UINT32  htds2_shift_r1                        ; // 08
    UINT32  htds2_pix_st_r2                       ; // 09
    UINT32  htds2_pix_ed_r2                       ; // 10
    UINT32  htds2_shift_r2                        ; // 11
    UINT32  htds2_pix_st_r3                       ; // 12
    UINT32  htds2_pix_ed_r3                       ; // 13
    UINT32  htds2_shift_r3                        ; // 14
    UINT32  htds2_checksum                        ; // 15
    UINT32  g224_reserved[16]                     ; // 16~31

    // Group 225 : DVE
    UINT32  dve_vsync_start_top                   ; // 00
    UINT32  dve_vsync_start_bot                   ; // 01
    UINT32  dve_vsync_h_point                     ; // 02
    UINT32  dve_vsync_pd_cnt                      ; // 03
    UINT32  dve_hsync_start                       ; // 04
    UINT32  dve_hsync_pd_cnt                      ; // 05
    UINT32  dve_vhsync_polar                      ; // 06
    UINT32  dve_v_vld_top_start                   ; // 07
    UINT32  dve_v_vld_top_end                     ; // 08
    UINT32  dve_v_vld_bot_start                   ; // 09
    UINT32  dve_v_vld_bot_end                     ; // 10
    UINT32  dve_de_h_start                        ; // 11
    UINT32  dve_de_h_end                          ; // 12
    UINT32  dve_mp_tg_line_0_length               ; // 13
    UINT32  dve_mp_tg_frame_0_line                ; // 14
    UINT32  dve_mp_tg_act_0_pix                   ; // 15
    UINT32  dve_hdmi_mode_0                       ; // 16
    UINT32  dve_act_0_pix_656                     ; // 17
    UINT32  dve_line_0_length_656                 ; // 18
    UINT32  dve_frame_0_line_656                  ; // 19
    UINT32  dve_noact_end_tl_656                  ; // 20
    UINT32  dve_act_end_tl_656                    ; // 21
    UINT32  dve_noact_end_bl_656                  ; // 22
    UINT32  dve_act_end_bl_656                    ; // 23
    UINT32  dve_ccir656_ctrl_0                    ; // 24
    UINT32  dve_d656_y_range                      ; // 25
    UINT32  dve_d656_c_range                      ; // 26
    UINT32  dve_hdmi_y_range                      ; // 27
    UINT32  dve_hdmi_c_range                      ; // 28
    UINT32  dve_hdmi_sharp                        ; // 29
    UINT32  dve_config1                           ; // 30
    UINT32  dve_config0                           ; // 31
   
    // Group 226 : TTX_P2S
    UINT32  g226_reserved_0[23]                   ; //
    UINT32  ttx_mode                              ; //
    UINT32  ttx_base_addr                         ; //
    UINT32  ttx_top_addr                          ; //
    UINT32  ttx_bot_addr                          ; //
    UINT32  ttx_line                              ; //
    UINT32  ttx_ab_base_add_h                     ; //
    UINT32  ttx_states                            ; //
    UINT32  g226_reserved_1[2]                    ; //

    // Group 227 : 
    UINT32  g227_reserved[32]                     ; //
    
    // Group 228 : 
    UINT32 rte_mode0        			; 	//0 
    UINT32 rte_fb_ctrl0     			; 	//1 
    UINT32 rte_fb_ctrl1     			; 	//2
    UINT32 rte_bist_mode0   			; 	//3
    UINT32 rte_bist_mode1   			; 	//4
    UINT32 rte_urgent_thd   			; 	//5
    UINT32 rte_iw_req_clkcnt			; 	//6
    UINT32 rte_faddr0_lw    			; 	//7 
    UINT32 rte_faddr0_hw    			; 	//8 
    UINT32 rte_fb_step      			; 	//9 
    UINT32 rte_tgr_hact     			; 	//10
    UINT32 rte_tgr_htot     			; 	//11 
    UINT32 rte_tgr_vact_st_t			; 	//12 
    UINT32 rte_tgr_vact_ed_t			; 	//13 
    UINT32 rte_tgr_vact_st_b			; 	//14  
    UINT32 rte_tgr_vact_ed_b			; 	//15 
    UINT32 rte_tgr_vtot     			; 	//16
    UINT32 rte_sync_vcnt    			; 	//17
    UINT32 rte_sync_hcnt    			; 	//18
    UINT32 rte_out_vact_st_t			; 	//19
    UINT32 rte_out_vact_st_b			; 	//20
    UINT32 rte_irq_line     			; 	//21
    UINT32 rte_debug        			; 	//22
    UINT32 rte_chkout_a     			; 	//23
    UINT32 rte_chkout_b     			; 	//24
    UINT32 rte_iw_req2gnt   			; 	//25
    UINT32 rte_iw_gnt2req   			; 	//26
    UINT32 rte_ir_req2gnt   			; 	//27
    UINT32 rte_ir_gnt2req   			; 	//28
    UINT32 rte_mode1        			; 	//29 
    UINT32 g228_reserved0[2]			;                  //30~31
    
    // Group 229 : TCIW
    UINT32  tciw_latch_en                         ; // 00
    UINT32  tciw_enable_config                    ; // 01
    UINT32  tciw_mode_config                      ; // 02
    UINT32  tciw_luma_base_lsb                    ; // 03
    UINT32  tciw_luma_base_msb                    ; // 04
    UINT32  tciw_crma_base_lsb                    ; // 05
    UINT32  tciw_crma_base_msb                    ; // 06
    UINT32  tciw_mbus_config_0                    ; // 07
    UINT32  tciw_mbus_config_1                    ; // 08
    UINT32  tciw_frame_size                       ; // 09
    UINT32  tciw_region_x_size                    ; // 10
    UINT32  tciw_region_x_start                   ; // 11
    UINT32  tciw_region_y_start                   ; // 12
    UINT32  tciw_v_flip                           ; // 13
    UINT32  tciw_alpha_element                    ; // 14
    UINT32  tciw_yuv_config                       ; // 15
    UINT32  tciw_dither_config                    ; // 16
    UINT32  tciw_sideband_config                  ; // 17
    UINT32  tciw_iw_error_flag                    ; // 18
    UINT32  tciw_ift_error_flag                   ; // 19
    UINT32  tciw_luma_checksum                    ; // 20
    UINT32  tciw_chroma_checksum                  ; // 21
    UINT32  g229_reserved[10]                     ; // 22~31

    // Group 230 : TCON
    UINT32  G230_TCON[32]                         ;

    // Group 231 : TCON
    UINT32  G231_TCON[32]                         ;

    // Group 232 : TCON
    UINT32  G232_TCON[32]                         ;

    // Group 233 : TCON
    UINT32  G233_TCON[32]                         ;

    // Group 234 : LVDSIF
    UINT32  G234_LVDSIF[32]                       ;

    // Group 235 : V656IN
    UINT32  v656in_0_ctrl_1                                         ;
    UINT32  v656in_0_cntl_2                                         ;
    UINT32  v656in_0_int                                            ;
    UINT32  v656in_0_int_mask                                       ;
    UINT32  v656in_0_masked_int                                     ;
    UINT32  v656in_0_timer_out_thr_l                                ;
    UINT32  v656in_0_timer_out_thr_h                                ;
    UINT32  G235_RESERVER_0                                         ;
    UINT32  G235_RESERVER_1                                         ;
    UINT32  G235_RESERVER_2                                         ;
    UINT32  G235_RESERVER_3                                         ;
    UINT32  v656in_0_err_cntl                                       ;
    UINT32  v656in_0_pixel_start                                    ;
    UINT32  v656in_0_pixel_end                                      ;
    UINT32  v656in_0_line_start                                     ;
    UINT32  v656in_0_line_end                                       ;
    UINT32  v656in_0_pix_cnt                                        ;
    UINT32  v656in_0_line_cnt                                       ;
    UINT32  G235_RESERVER_4                                         ;
    UINT32  G235_RESERVER_5                                         ;
    UINT32  G235_RESERVER_6                                         ;
    UINT32  G235_RESERVER_7                                         ;
    UINT32  G235_RESERVER_8                                         ;
    UINT32  v656in_0_status                                         ;
    UINT32  G235_RESERVER_9                                         ;
    UINT32  v656in_0_use_free_clk                                   ;
    UINT32  G235_RESERVER_10                                        ;
    UINT32  G235_RESERVER_11                                        ;
    UINT32  G235_RESERVER_12                                        ;
    UINT32  G235_RESERVER_13                                        ;
    UINT32  v656in_0_vs_delay                                       ;
    UINT32  v656in_0_total_pix_per_line_to_iw                       ;
   

    // Group 236 : V656IN
    UINT32 v656in_1_ctrl_1                                          ;
    UINT32 v656in_1_ctrl_2                                          ;
    UINT32 v656in_1_int                                             ;
    UINT32 v656in_1_int_mask                                        ;
    UINT32 v656in_1_masked_int                                      ;
    UINT32 v656in_1_timer_out_thr_l                                 ;
    UINT32 v656in_1_timer_out_thr_h                                 ;
    UINT32 G236_RESERVER_0                                          ;
    UINT32 G236_RESERVER_1                                          ;
    UINT32 G236_RESERVER_2                                          ;
    UINT32 G236_RESERVER_3                                          ;
    UINT32 v656in_1_err_cntl                                        ;
    UINT32 v656in_1_pixel_start                                     ;
    UINT32 v656in_1_pixel_end                                       ;
    UINT32 v656in_1_line_start                                      ;
    UINT32 v656in_1_line_end                                        ;
    UINT32 v656in_1_pix_cnt                                         ;
    UINT32 v656in_1_line_cnt                                        ;
    UINT32 G236_RESERVER_4                                          ;
    UINT32 G236_RESERVER_5                                          ;
    UINT32 G236_RESERVER_6                                          ;
    UINT32 G236_RESERVER_7                                          ;
    UINT32 G236_RESERVER_8                                          ;
    UINT32 v656in_1_status                                          ;
    UINT32 G236_RESERVER_9                                          ;
    UINT32 v656in_1_use_free_clk                                    ;
    UINT32 G236_RESERVER_10                                         ;
    UINT32 G236_RESERVER_11                                         ;
    UINT32 G236_RESERVER_12                                         ;
    UINT32 G236_RESERVER_13                                         ;
    UINT32 v656in_1_vs_delay                                        ;
    UINT32 v656in_1_total_pix_per_line_to_iw                        ;

    // Group 237 : V656IN
    UINT32 v656in_2_ctrl_1                                          ;
    UINT32 v656in_2_ctrl_2                                          ;
    UINT32 v656in_2_int                                             ;
    UINT32 v656in_2_int_mask                                        ;
    UINT32 v656in_2_masked_int                                      ;
    UINT32 v656in_2_timer_out_thr_l                                 ;
    UINT32 v656in_2_timer_out_thr_h                                 ;
    UINT32 G237_RESERVER_0                                          ;
    UINT32 G237_RESERVER_1                                          ;
    UINT32 G237_RESERVER_2                                          ;
    UINT32 G237_RESERVER_3                                          ;
    UINT32 v656in_2_err_cntl                                        ;
    UINT32 v656in_2_pixel_start                                     ;
    UINT32 v656in_2_pixel_end                                       ;
    UINT32 v656in_2_line_start                                      ;
    UINT32 v656in_2_line_end                                        ;
    UINT32 v656in_2_pix_cnt                                         ;
    UINT32 v656in_2_line_cnt                                        ;
    UINT32 G237_RESERVER_4                                          ;
    UINT32 G237_RESERVER_5                                          ;
    UINT32 G237_RESERVER_6                                          ;
    UINT32 G237_RESERVER_7                                          ;
    UINT32 G237_RESERVER_8                                          ;
    UINT32 v656in_2_status                                          ;
    UINT32 G237_RESERVER_9                                          ;
    UINT32 v656in_2_use_free_clk                                    ;
    UINT32 G237_RESERVER_10                                         ;
    UINT32 G237_RESERVER_11                                         ;
    UINT32 G237_RESERVER_12                                         ;
    UINT32 G237_RESERVER_13                                         ;
    UINT32 v656in_2_vs_delay                                        ;
    UINT32 v656in_2_total_pix_per_line_to_iw                        ;

    // Group 238 : V656IN
    UINT32 v656in_3_ctrl_1                                          ;
    UINT32 v656in_3_ctrl_2                                          ;
    UINT32 v656in_3_int                                             ;
    UINT32 v656in_3_int_mask                                        ;
    UINT32 v656in_3_masked_int                                      ;
    UINT32 v656in_3_timer_out_thr_l                                 ;
    UINT32 v656in_3_timer_out_thr_h                                 ;
    UINT32 G238_RESERVER_0                                          ;
    UINT32 G238_RESERVER_1                                          ;
    UINT32 G238_RESERVER_2                                          ;
    UINT32 G238_RESERVER_3                                          ;
    UINT32 v656in_3_err_cntl                                        ;
    UINT32 v656in_3_pixel_start                                     ;
    UINT32 v656in_3_pixel_end                                       ;
    UINT32 v656in_3_line_start                                      ;
    UINT32 v656in_3_line_end                                        ;
    UINT32 v656in_3_pix_cnt                                         ;
    UINT32 v656in_3_line_cnt                                        ;
    UINT32 G238_RESERVER_4                                          ;
    UINT32 G238_RESERVER_5                                          ;
    UINT32 G238_RESERVER_6                                          ;
    UINT32 G238_RESERVER_7                                          ;
    UINT32 G238_RESERVER_8                                          ;
    UINT32 v656in_3_status                                          ;
    UINT32 G238_RESERVER_9                                          ;
    UINT32 v656in_3_use_free_clk                                    ;
    UINT32 G238_RESERVER_10                                         ;
    UINT32 G238_RESERVER_11                                         ;
    UINT32 G238_RESERVER_12                                         ;
    UINT32 G238_RESERVER_13                                         ;
    UINT32 v656in_3_vs_delay                                        ;
    UINT32 v656in_3_total_pix_per_line_to_iw                        ;    
    // Group 239 : V656IN
    UINT32 v656in_4_ctrl_1                                          ;
    UINT32 v656in_4_ctrl_2                                          ;
    UINT32 v656in_4_int                                             ;
    UINT32 v656in_4_int_mask                                        ;
    UINT32 v656in_4_masked_int                                      ;
    UINT32 v656in_4_timer_out_thr_l                                 ;
    UINT32 v656in_4_timer_out_thr_h                                 ;
    UINT32 G239_RESERVER_0                                          ;
    UINT32 G239_RESERVER_1                                          ;
    UINT32 G239_RESERVER_2                                          ;
    UINT32 G239_RESERVER_3                                          ;
    UINT32 v656in_4_err_cntl                                        ;
    UINT32 v656in_4_pixel_start                                     ;
    UINT32 v656in_4_pixel_end                                       ;
    UINT32 v656in_4_line_start                                      ;
    UINT32 v656in_4_line_end                                        ;
    UINT32 v656in_4_pix_cnt                                         ;
    UINT32 v656in_4_line_cnt                                        ;
    UINT32 G239_RESERVER_4                                          ;
    UINT32 G239_RESERVER_5                                          ;
    UINT32 G239_RESERVER_6                                          ;
    UINT32 G239_RESERVER_7                                          ;
    UINT32 G239_RESERVER_8                                          ;
    UINT32 v656in_4_status                                          ;
    UINT32 G239_RESERVER_9                                          ;
    UINT32 v656in_4_use_free_clk                                    ;
    UINT32 G239_RESERVER_10                                         ;
    UINT32 G239_RESERVER_11                                         ;
    UINT32 G239_RESERVER_12                                         ;
    UINT32 G239_RESERVER_13                                         ;
    UINT32 v656in_4_vs_delay                                        ;
    UINT32 v656in_4_total_pix_per_line_to_iw                        ;

    // Group 240 : V656IN
    UINT32 v656in_5_ctrl_1                                          ;
    UINT32 v656in_5_ctrl_2                                          ;
    UINT32 v656in_5_int                                             ;
    UINT32 v656in_5_int_mask                                        ;
    UINT32 v656in_5_masked_int                                      ;
    UINT32 v656in_5_timer_out_thr_l                                 ;
    UINT32 v656in_5_timer_out_thr_h                                 ;
    UINT32 G240_RESERVER_0                                          ;
    UINT32 G240_RESERVER_1                                          ;
    UINT32 G240_RESERVER_2                                          ;
    UINT32 G240_RESERVER_3                                          ;
    UINT32 v656in_5_err_cntl                                        ;
    UINT32 v656in_5_pixel_start                                     ;
    UINT32 v656in_5_pixel_end                                       ;
    UINT32 v656in_5_line_start                                      ;
    UINT32 v656in_5_line_end                                        ;
    UINT32 v656in_5_pix_cnt                                         ;
    UINT32 v656in_5_line_cnt                                        ;
    UINT32 G240_RESERVER_4                                          ;
    UINT32 G240_RESERVER_5                                          ;
    UINT32 G240_RESERVER_6                                          ;
    UINT32 G240_RESERVER_7                                          ;
    UINT32 G240_RESERVER_8                                          ;
    UINT32 v656in_5_status                                          ;
    UINT32 G240_RESERVER_9                                          ;
    UINT32 v656in_5_use_free_clk                                    ;
    UINT32 G240_RESERVER_10                                         ;
    UINT32 G240_RESERVER_11                                         ;
    UINT32 G240_RESERVER_12                                         ;
    UINT32 G240_RESERVER_13                                         ;
    UINT32 v656in_5_vs_delay                                        ;
    UINT32 v656in_5_total_pix_per_line_to_iw                        ;

    // Group 241 : V656IN
    UINT32 v656in_6_ctrl_1                                            ;
    UINT32 v656in_6_ctrl_2                                            ;
    UINT32 v656in_6_int                                               ;
    UINT32 v656in_6_int_mask                                          ;
    UINT32 v656in_6_masked_int                                        ;
    UINT32 v656in_6_timer_out_thr_l                                   ;
    UINT32 v656in_6_timer_out_thr_h                                   ;
    UINT32 G241_RESERVER_0                                            ;
    UINT32 G241_RESERVER_1                                            ;
    UINT32 G241_RESERVER_2                                            ;
    UINT32 G241_RESERVER_3                                            ;
    UINT32 v656in_6_err_cntl                                          ;
    UINT32 v656in_6_pixel_start                                       ;
    UINT32 v656in_6_pixel_end                                         ;
    UINT32 v656in_6_line_start                                        ;
    UINT32 v656in_6_line_end                                          ;
    UINT32 v656in_6_pix_cnt                                           ;
    UINT32 v656in_6_line_cnt                                          ;
    UINT32 G241_RESERVER_4                                            ;
    UINT32 G241_RESERVER_5                                            ;
    UINT32 G241_RESERVER_6                                            ;
    UINT32 G241_RESERVER_7                                            ;
    UINT32 G241_RESERVER_8                                            ;
    UINT32 v656in_6_status                                            ;
    UINT32 v656in_6_programmable_ext_fd_id_thr                        ;
    UINT32 v656in_6_use_free_clk                                      ;
    UINT32 reg_pix_start                                              ;
    UINT32 reg_pix_width                                              ;
    UINT32 reg_line_start                                             ;
    UINT32 reg_line_width                                             ;
    UINT32 v656in_6_vs_delay                                          ;
    UINT32 v656in_6_total_pix_per_line_to_iw                          ;

    // Group 242 : V656IN
    UINT32 v656iniw_0_latch_en                                 ;
    UINT32 v656iniw_0_enable_config                            ;
    UINT32 v656iniw_0_mode_config                              ;
    UINT32 v656iniw_0_luma_base_lsb                            ;
    UINT32 v656iniw_0_luma_base_msb                            ;
    UINT32 v656iniw_0_crma_base_lsb                            ;
    UINT32 v656iniw_0_crma_base_msb                            ;
    UINT32 v656iniw_0_mbus_config_0                            ;
    UINT32 v656iniw_0_mbus_config_1                            ;
    UINT32 v656iniw_0_frame_size                               ;
    UINT32 v656iniw_0_region_x_size                            ;
    UINT32 v656iniw_0_region_x_start                           ;
    UINT32 v656iniw_0_region_y_start                           ;
    UINT32 v656iniw_0_v_flip                                   ;
    UINT32 v656iniw_0_alpha_element                            ;
    UINT32 v656iniw_0_yuv_config                               ;
    UINT32 v656iniw_0_dither_config                            ;
    UINT32 v656iniw_0_sideband_config                          ;
    UINT32 v656iniw_0_iw_error_flag                            ;
    UINT32 v656iniw_0_ift_error_flag                           ;
    UINT32 v656iniw_0_luma_checksum                            ;
    UINT32 v656iniw_0_chroma_checksum                          ;
    UINT32 v656iniw_0_mbus_debug_0                             ;
    UINT32 v656iniw_0_mbus_debug_1                             ;
    UINT32 G242_RESERVER_0                                     ;
    UINT32 G242_RESERVER_1                                     ;
    UINT32 G242_RESERVER_2                                     ;
    UINT32 G242_RESERVER_3                                     ;
    UINT32 G242_RESERVER_4                                     ;
    UINT32 G242_RESERVER_5                                     ;
    UINT32 G242_RESERVER_6                                     ;
    UINT32 G242_RESERVER_7                                     ;

    // Group 243 : V656IN
    UINT32 v656iniw_1_latch_en                                ;
    UINT32 v656iniw_1_enable_config                           ;
    UINT32 v656iniw_1_mode_config                             ;
    UINT32 v656iniw_1_luma_base_lsb                           ;
    UINT32 v656iniw_1_luma_base_msb                           ;
    UINT32 v656iniw_1_crma_base_lsb                           ;
    UINT32 v656iniw_1_crma_base_msb                           ;
    UINT32 v656iniw_1_mbus_config_0                           ;
    UINT32 v656iniw_1_mbus_config_1                           ;
    UINT32 v656iniw_1_frame_size                              ;
    UINT32 v656iniw_1_region_x_size                           ;
    UINT32 v656iniw_1_region_x_start                          ;
    UINT32 v656iniw_1_region_y_start                          ;
    UINT32 v656iniw_1_v_flip                                  ;
    UINT32 v656iniw_1_alpha_element                           ;
    UINT32 v656iniw_1_yuv_config                              ;
    UINT32 v656iniw_1_dither_config                           ;
    UINT32 v656iniw_1_sideband_config                         ;
    UINT32 v656iniw_1_iw_error_flag                           ;
    UINT32 v656iniw_1_ift_error_flag                          ;
    UINT32 v656iniw_1_luma_checksum                           ;
    UINT32 v656iniw_1_chroma_checksum                         ;
    UINT32 v656iniw_1_mbus_debug_0                            ;
    UINT32 v656iniw_1_mbus_debug_1                            ;
    UINT32 G243_RESERVER_0                                    ;
    UINT32 G243_RESERVER_1                                    ;
    UINT32 G243_RESERVER_2                                    ;
    UINT32 G243_RESERVER_3                                    ;
    UINT32 G243_RESERVER_4                                    ;
    UINT32 G243_RESERVER_5                                    ;
    UINT32 G243_RESERVER_6                                    ;
    UINT32 G243_RESERVER_7                                    ;

    // Group 244 : V656IN
    UINT32 v656iniw_2_latch_en                                ;
    UINT32 v656iniw_2_enable_config                           ;
    UINT32 v656iniw_2_mode_config                             ;
    UINT32 v656iniw_2_luma_base_lsb                           ;
    UINT32 v656iniw_2_luma_base_msb                           ;
    UINT32 v656iniw_2_crma_base_lsb                           ;
    UINT32 v656iniw_2_crma_base_msb                           ;
    UINT32 v656iniw_2_mbus_config_0                           ;
    UINT32 v656iniw_2_mbus_config_1                           ;
    UINT32 v656iniw_2_frame_size                              ;
    UINT32 v656iniw_2_region_x_size                           ;
    UINT32 v656iniw_2_region_x_start                          ;
    UINT32 v656iniw_2_region_y_start                          ;
    UINT32 v656iniw_2_v_flip                                  ;
    UINT32 v656iniw_2_alpha_element                           ;
    UINT32 v656iniw_2_yuv_config                              ;
    UINT32 v656iniw_2_dither_config                           ;
    UINT32 v656iniw_2_sideband_config                         ;
    UINT32 v656iniw_2_iw_error_flag                           ;
    UINT32 v656iniw_2_ift_error_flag                          ;
    UINT32 v656iniw_2_luma_checksum                           ;
    UINT32 v656iniw_2_chroma_checksum                         ;
    UINT32 v656iniw_2_mbus_debug_0                            ;
    UINT32 v656iniw_2_mbus_debug_1                            ;
    UINT32 G244_RESERVER_0                                    ;
    UINT32 G244_RESERVER_1                                    ;
    UINT32 G244_RESERVER_2                                    ;
    UINT32 G244_RESERVER_3                                    ;
    UINT32 G244_RESERVER_4                                    ;
    UINT32 G244_RESERVER_5                                    ;
    UINT32 G244_RESERVER_6                                    ;
    UINT32 G244_RESERVER_7                                    ;

    // Group 245 : V656IN
    UINT32 v656iniw_3_latch_en                                ;
    UINT32 v656iniw_3_enable_config                           ;
    UINT32 v656iniw_3_mode_config                             ;
    UINT32 v656iniw_3_luma_base_lsb                           ;
    UINT32 v656iniw_3_luma_base_msb                           ;
    UINT32 v656iniw_3_crma_base_lsb                           ;
    UINT32 v656iniw_3_crma_base_msb                           ;
    UINT32 v656iniw_3_mbus_config_0                           ;
    UINT32 v656iniw_3_mbus_config_1                           ;
    UINT32 v656iniw_3_frame_size                              ;
    UINT32 v656iniw_3_region_x_size                           ;
    UINT32 v656iniw_3_region_x_start                          ;
    UINT32 v656iniw_3_region_y_start                          ;
    UINT32 v656iniw_3_v_flip                                  ;
    UINT32 v656iniw_3_alpha_element                           ;
    UINT32 v656iniw_3_yuv_config                              ;
    UINT32 v656iniw_3_dither_config                           ;
    UINT32 v656iniw_3_sideband_config                         ;
    UINT32 v656iniw_3_iw_error_flag                           ;
    UINT32 v656iniw_3_ift_error_flag                          ;
    UINT32 v656iniw_3_luma_checksum                           ;
    UINT32 v656iniw_3_chroma_checksum                         ;
    UINT32 v656iniw_3_mbus_debug_0                            ;
    UINT32 v656iniw_3_mbus_debug_1                            ;
    UINT32 G245_RESERVER_0                                    ;
    UINT32 G245_RESERVER_1                                    ;
    UINT32 G245_RESERVER_2                                    ;
    UINT32 G245_RESERVER_3                                    ;
    UINT32 G245_RESERVER_4                                    ;
    UINT32 G245_RESERVER_5                                    ;
    UINT32 G245_RESERVER_6                                    ;
    UINT32 G245_RESERVER_7                                    ;

    // Group 246 : V656IN
    UINT32 v656iniw_4_latch_en                               ;
    UINT32 v656iniw_4_enable_config                          ;
    UINT32 v656iniw_4_mode_config                            ;
    UINT32 v656iniw_4_luma_base_lsb                          ;
    UINT32 v656iniw_4_luma_base_msb                          ;
    UINT32 v656iniw_4_crma_base_lsb                          ;
    UINT32 v656iniw_4_crma_base_msb                          ;
    UINT32 v656iniw_4_mbus_config_0                          ;
    UINT32 v656iniw_4_mbus_config_1                          ;
    UINT32 v656iniw_4_frame_size                             ;
    UINT32 v656iniw_4_region_x_size                          ;
    UINT32 v656iniw_4_region_x_start                         ;
    UINT32 v656iniw_4_region_y_start                         ;
    UINT32 v656iniw_4_v_flip                                 ;
    UINT32 v656iniw_4_alpha_element                          ;
    UINT32 v656iniw_4_yuv_config                             ;
    UINT32 v656iniw_4_dither_config                          ;
    UINT32 v656iniw_4_sideband_config                        ;
    UINT32 v656iniw_4_iw_error_flag                          ;
    UINT32 v656iniw_4_ift_error_flag                         ;
    UINT32 v656iniw_4_luma_checksum                          ;
    UINT32 v656iniw_4_chroma_checksum                        ;
    UINT32 v656iniw_4_mbus_debug_0                           ;
    UINT32 v656iniw_4_mbus_debug_1                           ;
    UINT32 G246_RESERVER_0                                   ;
    UINT32 G246_RESERVER_1                                   ;
    UINT32 G246_RESERVER_2                                   ;
    UINT32 G246_RESERVER_3                                   ;
    UINT32 G246_RESERVER_4                                   ;
    UINT32 G246_RESERVER_5                                   ;
    UINT32 G246_RESERVER_6                                   ;
    UINT32 G246_RESERVER_7                                   ;

    // Group 247 : V656IN
    UINT32 v656iniw_5_latch_en                               ;
    UINT32 v656iniw_5_enable_config                          ;
    UINT32 v656iniw_5_mode_config                            ;
    UINT32 v656iniw_5_luma_base_lsb                          ;
    UINT32 v656iniw_5_luma_base_msb                          ;
    UINT32 v656iniw_5_crma_base_lsb                          ;
    UINT32 v656iniw_5_crma_base_msb                          ;
    UINT32 v656iniw_5_mbus_config_0                          ;
    UINT32 v656iniw_5_mbus_config_1                          ;
    UINT32 v656iniw_5_frame_size                             ;
    UINT32 v656iniw_5_region_x_size                          ;
    UINT32 v656iniw_5_region_x_start                         ;
    UINT32 v656iniw_5_region_y_start                         ;
    UINT32 v656iniw_5_v_flip                                 ;
    UINT32 v656iniw_5_alpha_element                          ;
    UINT32 v656iniw_5_yuv_config                             ;
    UINT32 v656iniw_5_dither_config                          ;
    UINT32 v656iniw_5_sideband_config                        ;
    UINT32 v656iniw_5_iw_error_flag                          ;
    UINT32 v656iniw_5_ift_error_flag                         ;
    UINT32 v656iniw_5_luma_checksum                          ;
    UINT32 v656iniw_5_chroma_checksum                        ;
    UINT32 v656iniw_5_mbus_debug_0                           ;
    UINT32 v656iniw_5_mbus_debug_1                           ;
    UINT32 G247_RESERVER_0                                   ;
    UINT32 G247_RESERVER_1                                   ;
    UINT32 G247_RESERVER_2                                   ;
    UINT32 G247_RESERVER_3                                   ;
    UINT32 G247_RESERVER_4                                   ;
    UINT32 G247_RESERVER_5                                   ;
    UINT32 G247_RESERVER_6                                   ;
    UINT32 G247_RESERVER_7                                   ;

    // Group 248 : V656IN
    UINT32 v656iniw_6_latch_en                               ;
    UINT32 v656iniw_6_enable_config                          ;
    UINT32 v656iniw_6_mode_config                            ;
    UINT32 v656iniw_6_luma_base_lsb                          ;
    UINT32 v656iniw_6_luma_base_msb                          ;
    UINT32 v656iniw_6_crma_base_lsb                          ;
    UINT32 v656iniw_6_crma_base_msb                          ;
    UINT32 v656iniw_6_mbus_config_0                          ;
    UINT32 v656iniw_6_mbus_config_1                          ;
    UINT32 v656iniw_6_frame_size                             ;
    UINT32 v656iniw_6_region_x_size                          ;
    UINT32 v656iniw_6_region_x_start                         ;
    UINT32 v656iniw_6_region_y_start                         ;
    UINT32 v656iniw_6_v_flip                                 ;
    UINT32 v656iniw_6_alpha_element                          ;
    UINT32 v656iniw_6_yuv_config                             ;
    UINT32 v656iniw_6_dither_config                          ;
    UINT32 v656iniw_6_sideband_config                        ;
    UINT32 v656iniw_6_iw_error_flag                          ;
    UINT32 v656iniw_6_ift_error_flag                         ;
    UINT32 v656iniw_6_luma_checksum                          ;
    UINT32 v656iniw_6_chroma_checksum                        ;
    UINT32 v656iniw_6_mbus_debug_0                           ;
    UINT32 v656iniw_6_mbus_debug_1                           ;
    UINT32 G248_RESERVER_0                                   ;
    UINT32 G248_RESERVER_1                                   ;
    UINT32 G248_RESERVER_2                                   ;
    UINT32 G248_RESERVER_3                                   ;
    UINT32 G248_RESERVER_4                                   ;
    UINT32 G248_RESERVER_5                                   ;
    UINT32 G248_RESERVER_6                                   ;
    UINT32 G248_RESERVER_7                                   ;

    // Group 249 : Reserved
    UINT32 v656in_top_rf_sd_3_sel                                      ;
    UINT32 v656in_top_MIPI_RX_BT1120_SEL                               ;
    UINT32 v656in_top_int_0_type_sel                                   ;
    UINT32 v656in_top_tw2864_input_chennal_mode                        ;
    UINT32 v656in_input_source_channel_sel                             ;
    UINT32 v656in_input_channel_xy_report_0                            ;
    UINT32 v656in_input_channel_xy_report_1                            ;
    UINT32 v656in_mismatch_report                                      ;
    UINT32 G249_RESERVER_0                                             ;
    UINT32 G249_RESERVER_1                                             ;
    UINT32 G249_RESERVER_2                                             ;
    UINT32 G249_RESERVER_3                                             ;
    UINT32 G249_RESERVER_4                                             ;
    UINT32 G249_RESERVER_5                                             ;
    UINT32 G249_RESERVER_6                                             ;
    UINT32 G249_RESERVER_7                                             ;
    UINT32 G249_RESERVER_8                                             ;
    UINT32 G249_RESERVER_9                                             ;
    UINT32 G249_RESERVER_10                                            ;
    UINT32 G249_RESERVER_11                                            ;
    UINT32 G249_RESERVER_12                                            ;
    UINT32 G249_RESERVER_13                                            ;
    UINT32 G249_RESERVER_14                                            ;
    UINT32 G249_RESERVER_15                                            ;
    UINT32 G249_RESERVER_16                                            ;
    UINT32 G249_RESERVER_17                                            ;
    UINT32 G249_RESERVER_18                                            ;
    UINT32 G249_RESERVER_19                                            ;
    UINT32 G249_RESERVER_20                                            ;
    UINT32 G249_RESERVER_21                                            ;
    UINT32 G249_RESERVER_22                                            ;
    UINT32 G249_RESERVER_23                                            ;

    // Group 250 : Reserved
    UINT32  G250_RESERVED[32]                     ;

    // Group 251 : Reserved
    UINT32  G251_RESERVED[32]                     ;

    // Group 252 : Reserved
    UINT32  G252_RESERVED[32]                     ;

    // Group 253 : Reserved
    UINT32  G253_RESERVED[32]                     ;

    // Group 254 : Reserved
    UINT32  G254_RESERVED[32]                     ;

    // Group 255 : Reserved
    UINT32  G255_RESERVED[32]                     ;

    // Group 256 : Reserved
    UINT32  G256_RESERVED[32]                     ;

    // Group 257 : HDMI_TX
    UINT32  G257_HDMI_TX[32]                      ;

    //Group   258 : MBOX
    UINT32  mbox0_a926_int_trigger                ; // 00  (ADDR : 0x9C00_8100)
    UINT32  mbox0_writelock_flag                  ; // 01  (ADDR : 0x9C00_8104)
    UINT32  mbox0_overwrite_flag                  ; // 02  (ADDR : 0x9C00_8108)
    UINT32  RGST_Group0258_Reg03                  ; // 03  (ADDR : 0x9C00_810C)
    UINT32  mbox0_normal_trans[20]                ; // 04  (ADDR : 0x9C00_8110)
						    // 05  (ADDR : 0x9C00_8114)
						    // 06  (ADDR : 0x9C00_8118)
						    // 07  (ADDR : 0x9C00_811C)
                                                    // 08  (ADDR : 0x9C00_8120)
                                                    // 09  (ADDR : 0x9C00_8124)
                                                    // 10  (ADDR : 0x9C00_8128)
                                                    // 11  (ADDR : 0x9C00_812C)
                                                    // 12  (ADDR : 0x9C00_8130)
                                                    // 13  (ADDR : 0x9C00_8134)
                                                    // 14  (ADDR : 0x9C00_8138)
                                                    // 15  (ADDR : 0x9C00_813C)
                                                    // 16  (ADDR : 0x9C00_8140)
                                                    // 17  (ADDR : 0x9C00_8144)
                                                    // 18  (ADDR : 0x9C00_8148)
                                                    // 19  (ADDR : 0x9C00_814C)
                                                    // 20  (ADDR : 0x9C00_8150)
                                                    // 21  (ADDR : 0x9C00_8154)
                                                    // 22  (ADDR : 0x9C00_8158)
                                                    // 23  (ADDR : 0x9C00_815C)
    UINT32  mbox0_direct_trans[8]                 ; // 24  (ADDR : 0x9C00_8160)
                                                    // 25  (ADDR : 0x9C00_8164)
                                                    // 26  (ADDR : 0x9C00_8168)
                                                    // 27  (ADDR : 0x9C00_816C)
                                                    // 28  (ADDR : 0x9C00_8170)
                                                    // 29  (ADDR : 0x9C00_8174)
                                                    // 30  (ADDR : 0x9C00_8178)
                                                    // 31  (ADDR : 0x9C00_817C)

    //Group   259 : MBOX
    UINT32  mbox1_ca9_int_trigger                 ; // 00  (ADDR : 0x9C00_8180)
    UINT32  mbox1_writelock_flag                  ; // 01  (ADDR : 0x9C00_8184)
    UINT32  mbox1_overwrite_flag                  ; // 02  (ADDR : 0x9C00_8188)
    UINT32  RGST_Group0259_Reg03                  ; // 03  (ADDR : 0x9C00_818C)
    UINT32  mbox1_normal_trans[20]                 ; // 04  (ADDR : 0x9C00_8190)
                                                    // 05  (ADDR : 0x9C00_8194)
                                                    // 06  (ADDR : 0x9C00_8198)
                                                    // 07  (ADDR : 0x9C00_819C)
                                                    // 08  (ADDR : 0x9C00_81A0)
                                                    // 09  (ADDR : 0x9C00_81A4)
                                                    // 10  (ADDR : 0x9C00_81A8)
                                                    // 11  (ADDR : 0x9C00_81AC)
                                                    // 12  (ADDR : 0x9C00_81B0)
                                                    // 13  (ADDR : 0x9C00_81B4)
                                                    // 14  (ADDR : 0x9C00_81B8)
                                                    // 15  (ADDR : 0x9C00_81BC)
                                                    // 16  (ADDR : 0x9C00_81C0)
                                                    // 17  (ADDR : 0x9C00_81C4)
                                                    // 18  (ADDR : 0x9C00_81C8)
                                                    // 19  (ADDR : 0x9C00_81CC)
                                                    // 20  (ADDR : 0x9C00_81D0)
                                                    // 21  (ADDR : 0x9C00_81D4)
                                                    // 22  (ADDR : 0x9C00_81D8)
                                                    // 23  (ADDR : 0x9C00_81DC)
    UINT32  mbox1_direct_trans[8]                 ; // 24  (ADDR : 0x9C00_81E0)
						    // 25  (ADDR : 0x9C00_81E4)
    		                                    // 26  (ADDR : 0x9C00_81E8)
    		                                    // 27  (ADDR : 0x9C00_81EC)
    		                                    // 28  (ADDR : 0x9C00_81F0)
    		                                    // 29  (ADDR : 0x9C00_81F4)
    		                                    // 30  (ADDR : 0x9C00_81F8)
    		                                    // 31  (ADDR : 0x9C00_81FC)

    // Group 260 : HDMI_TX
    UINT32  G260_HDMI_TX[32]                      ;

    // Group 261 : HDMI_TX
    UINT32  G261_HDMI_TX[32]                      ;

    // Group 262 : HDMI_TX
    UINT32  G262_HDMI_TX[32]                      ;

    // Group 263 : HDMI_TX
    UINT32  G263_HDMI_TX[32]                      ;

    // Group 264 : SDIO_CTL
    UINT32  G264_SDIO_CTL[32]                    ;
    
    // Group 265 : SDIO_CTL
    UINT32  G265_SDIO_CTL[32]                    ;

    // Group 266 : SDIO_CTL
    UINT32  G266_SDIO_CTL[32]                    ;

    // Group 267 : SDIO_CTL
    UINT32  G267_SDIO_CTL[32]                    ;

    // Group 268 : SDIO_CTL
    UINT32  G268_SDIO_CTL[32]                    ;
 
    // Group 269 ~ 270 , Reserved
    UINT32  G269_G270_RESERVED[2*32]              ; //

    // Group 271 : UART4
    UINT32  uart4_data                            ; // 0 
    UINT32  uart4_lsr                             ; // 1 
    UINT32  uart4_msr                             ; // 2 
    UINT32  uart4_lcr                             ; // 3 
    UINT32  uart4_mcr                             ; // 4 
    UINT32  uart4_div_l                           ; // 5 
    UINT32  uart4_div_h                           ; // 6 
    UINT32  uart4_isc                             ; // 7 
    UINT32  uart4_tx_residue                      ; // 8
    UINT32  uart4_rx_residue                      ; // 9
    UINT32  uart4_rxfifo_thr                      ; //10
    UINT32  G271_reserved[21]                     ; //

    // Group 272 : UART5
    UINT32  uart5_data                            ; // 0 
    UINT32  uart5_lsr                             ; // 1 
    UINT32  uart5_msr                             ; // 2 
    UINT32  uart5_lcr                             ; // 3 
    UINT32  uart5_mcr                             ; // 4 
    UINT32  uart5_div_l                           ; // 5 
    UINT32  uart5_div_h                           ; // 6 
    UINT32  uart5_isc                             ; // 7 
    UINT32  uart5_tx_residue                      ; // 8
    UINT32  uart5_rx_residue                      ; // 9
    UINT32  uart5_rxfifo_thr                      ; //10
    UINT32  G272_reserved[21]                     ; //

    // Group 273 : UART6
    UINT32  G273_reserved[32]                     ; //

    // Group 274 : Reserved
    UINT32  G274_RESERVED[32]                     ; //

    // Group 275 : UART_DMA
    UINT32  RF_DMA0_ENABLE_SEL                    ; // 00
    UINT32  RF_DMA0_START_ADDR                    ; // 01
    UINT32  RF_DMA0_TIMEOUT_SET                   ; // 02
    UINT32  RF_DMA0_STATUS                        ; // 03
    UINT32  RF_DMA0_WR_ADR                        ; // 04
    UINT32  RF_DMA0_RD_ADR                        ; // 05
    UINT32  RF_DMA0_LENGTH_THR                    ; // 06
    UINT32  RF_DMA0_END_ADDR                      ; // 07
    UINT32  RF_DMA0_DATABYTES                     ; // 08
    UINT32  RF_DMA0_DEBUG_INFO                    ; // 09
    UINT32  RF_DMA0_RESERVED[6]                   ; // 10 ~ 15
    UINT32  RF_DMA1_ENABLE_SEL                    ; // 16
    UINT32  RF_DMA1_START_ADDR                    ; // 17
    UINT32  RF_DMA1_TIMEOUT_SET                   ; // 18
    UINT32  RF_DMA1_STATUS                        ; // 19
    UINT32  RF_DMA1_WR_ADR                        ; // 20
    UINT32  RF_DMA1_RD_ADR                        ; // 21
    UINT32  RF_DMA1_LENGTH_THR                    ; // 22
    UINT32  RF_DMA1_END_ADDR                      ; // 23
    UINT32  RF_DMA1_DATABYTES                     ; // 24
    UINT32  RF_DMA1_DEBUG_INFO                    ; // 25
    UINT32  RF_DMA1_RESERVED[6]                   ; // 26 ~ 31

    // Group 276 ~ 280 , Reserved
    UINT32  G276_G280_RESERVED[5*32]            ;
    
    //Group   281 : EMU_G0
    UINT32  RGST_Group0281_Reg00                  ; // 00  (ADDR : 0x9C00_8C80)
    UINT32  RGST_Group0281_Reg01                  ; // 01  (ADDR : 0x9C00_8C84)
    UINT32  RGST_Group0281_Reg02                  ; // 02  (ADDR : 0x9C00_8C88)
    UINT32  RGST_Group0281_Reg03                  ; // 03  (ADDR : 0x9C00_8C8C)
    UINT32  RGST_Group0281_Reg04                  ; // 04  (ADDR : 0x9C00_8C90)
    UINT32  RGST_Group0281_Reg05                  ; // 05  (ADDR : 0x9C00_8C94)
    UINT32  RGST_Group0281_Reg06                  ; // 06  (ADDR : 0x9C00_8C98)
    UINT32  RGST_Group0281_Reg07                  ; // 07  (ADDR : 0x9C00_8C9C)
    UINT32  RGST_Group0281_Reg08                  ; // 08  (ADDR : 0x9C00_8CA0)
    UINT32  RGST_Group0281_Reg09                  ; // 09  (ADDR : 0x9C00_8CA4)
    UINT32  RGST_Group0281_Reg10                  ; // 10  (ADDR : 0x9C00_8CA8)
    UINT32  RGST_Group0281_Reg11                  ; // 11  (ADDR : 0x9C00_8CAC)
    UINT32  RGST_Group0281_Reg12                  ; // 12  (ADDR : 0x9C00_8CB0)
    UINT32  RGST_Group0281_Reg13                  ; // 13  (ADDR : 0x9C00_8CB4)
    UINT32  RGST_Group0281_Reg14                  ; // 14  (ADDR : 0x9C00_8CB8)
    UINT32  RGST_Group0281_Reg15                  ; // 15  (ADDR : 0x9C00_8CBC)
    UINT32  RGST_Group0281_Reg16                  ; // 16  (ADDR : 0x9C00_8CC0)
    UINT32  RGST_Group0281_Reg17                  ; // 17  (ADDR : 0x9C00_8CC4)
    UINT32  RGST_Group0281_Reg18                  ; // 18  (ADDR : 0x9C00_8CC8)
    UINT32  RGST_Group0281_Reg19                  ; // 19  (ADDR : 0x9C00_8CCC)
    UINT32  RGST_Group0281_Reg20                  ; // 20  (ADDR : 0x9C00_8CD0)
    UINT32  RGST_Group0281_Reg21                  ; // 21  (ADDR : 0x9C00_8CD4)
    UINT32  RGST_Group0281_Reg22                  ; // 22  (ADDR : 0x9C00_8CD8)
    UINT32  RGST_Group0281_Reg23                  ; // 23  (ADDR : 0x9C00_8CDC)
    UINT32  RGST_Group0281_Reg24                  ; // 24  (ADDR : 0x9C00_8CE0)
    UINT32  RGST_Group0281_Reg25                  ; // 25  (ADDR : 0x9C00_8CE4)
    UINT32  RGST_Group0281_Reg26                  ; // 26  (ADDR : 0x9C00_8CE8)
    UINT32  RGST_Group0281_Reg27                  ; // 27  (ADDR : 0x9C00_8CEC)
    UINT32  RGST_Group0281_Reg28                  ; // 28  (ADDR : 0x9C00_8CF0)
    UINT32  RGST_Group0281_Reg29                  ; // 29  (ADDR : 0x9C00_8CF4)
    UINT32  RGST_Group0281_Reg30                  ; // 30  (ADDR : 0x9C00_8CF8)
    UINT32  RGST_Group0281_Reg31                  ; // 31  (ADDR : 0x9C00_8CFC)

    //Group   282 : EMU_G0
    UINT32  RGST_Group0282_Reg00                  ; // 00  (ADDR : 0x9C00_8D00)
    UINT32  RGST_Group0282_Reg01                  ; // 01  (ADDR : 0x9C00_8D04)
    UINT32  RGST_Group0282_Reg02                  ; // 02  (ADDR : 0x9C00_8D08)
    UINT32  RGST_Group0282_Reg03                  ; // 03  (ADDR : 0x9C00_8D0C)
    UINT32  RGST_Group0282_Reg04                  ; // 04  (ADDR : 0x9C00_8D10)
    UINT32  RGST_Group0282_Reg05                  ; // 05  (ADDR : 0x9C00_8D14)
    UINT32  RGST_Group0282_Reg06                  ; // 06  (ADDR : 0x9C00_8D18)
    UINT32  RGST_Group0282_Reg07                  ; // 07  (ADDR : 0x9C00_8D1C)
    UINT32  RGST_Group0282_Reg08                  ; // 08  (ADDR : 0x9C00_8D20)
    UINT32  RGST_Group0282_Reg09                  ; // 09  (ADDR : 0x9C00_8D24)
    UINT32  RGST_Group0282_Reg10                  ; // 10  (ADDR : 0x9C00_8D28)
    UINT32  RGST_Group0282_Reg11                  ; // 11  (ADDR : 0x9C00_8D2C)
    UINT32  RGST_Group0282_Reg12                  ; // 12  (ADDR : 0x9C00_8D30)
    UINT32  RGST_Group0282_Reg13                  ; // 13  (ADDR : 0x9C00_8D34)
    UINT32  RGST_Group0282_Reg14                  ; // 14  (ADDR : 0x9C00_8D38)
    UINT32  RGST_Group0282_Reg15                  ; // 15  (ADDR : 0x9C00_8D3C)
    UINT32  RGST_Group0282_Reg16                  ; // 16  (ADDR : 0x9C00_8D40)
    UINT32  RGST_Group0282_Reg17                  ; // 17  (ADDR : 0x9C00_8D44)
    UINT32  RGST_Group0282_Reg18                  ; // 18  (ADDR : 0x9C00_8D48)
    UINT32  RGST_Group0282_Reg19                  ; // 19  (ADDR : 0x9C00_8D4C)
    UINT32  RGST_Group0282_Reg20                  ; // 20  (ADDR : 0x9C00_8D50)
    UINT32  RGST_Group0282_Reg21                  ; // 21  (ADDR : 0x9C00_8D54)
    UINT32  RGST_Group0282_Reg22                  ; // 22  (ADDR : 0x9C00_8D58)
    UINT32  RGST_Group0282_Reg23                  ; // 23  (ADDR : 0x9C00_8D5C)
    UINT32  RGST_Group0282_Reg24                  ; // 24  (ADDR : 0x9C00_8D60)
    UINT32  RGST_Group0282_Reg25                  ; // 25  (ADDR : 0x9C00_8D64)
    UINT32  RGST_Group0282_Reg26                  ; // 26  (ADDR : 0x9C00_8D68)
    UINT32  RGST_Group0282_Reg27                  ; // 27  (ADDR : 0x9C00_8D6C)
    UINT32  RGST_Group0282_Reg28                  ; // 28  (ADDR : 0x9C00_8D70)
    UINT32  RGST_Group0282_Reg29                  ; // 29  (ADDR : 0x9C00_8D74)
    UINT32  RGST_Group0282_Reg30                  ; // 30  (ADDR : 0x9C00_8D78)
    UINT32  RGST_Group0282_Reg31                  ; // 31  (ADDR : 0x9C00_8D7C)

    //Group   283 : EMU_G1
    UINT32  RGST_Group0283_Reg00                  ; // 00  (ADDR : 0x9C00_8D80)
    UINT32  RGST_Group0283_Reg01                  ; // 01  (ADDR : 0x9C00_8D84)
    UINT32  RGST_Group0283_Reg02                  ; // 02  (ADDR : 0x9C00_8D88)
    UINT32  RGST_Group0283_Reg03                  ; // 03  (ADDR : 0x9C00_8D8C)
    UINT32  RGST_Group0283_Reg04                  ; // 04  (ADDR : 0x9C00_8D90)
    UINT32  RGST_Group0283_Reg05                  ; // 05  (ADDR : 0x9C00_8D94)
    UINT32  RGST_Group0283_Reg06                  ; // 06  (ADDR : 0x9C00_8D98)
    UINT32  RGST_Group0283_Reg07                  ; // 07  (ADDR : 0x9C00_8D9C)
    UINT32  RGST_Group0283_Reg08                  ; // 08  (ADDR : 0x9C00_8DA0)
    UINT32  RGST_Group0283_Reg09                  ; // 09  (ADDR : 0x9C00_8DA4)
    UINT32  RGST_Group0283_Reg10                  ; // 10  (ADDR : 0x9C00_8DA8)
    UINT32  RGST_Group0283_Reg11                  ; // 11  (ADDR : 0x9C00_8DAC)
    UINT32  RGST_Group0283_Reg12                  ; // 12  (ADDR : 0x9C00_8DB0)
    UINT32  RGST_Group0283_Reg13                  ; // 13  (ADDR : 0x9C00_8DB4)
    UINT32  RGST_Group0283_Reg14                  ; // 14  (ADDR : 0x9C00_8DB8)
    UINT32  RGST_Group0283_Reg15                  ; // 15  (ADDR : 0x9C00_8DBC)
    UINT32  RGST_Group0283_Reg16                  ; // 16  (ADDR : 0x9C00_8DC0)
    UINT32  RGST_Group0283_Reg17                  ; // 17  (ADDR : 0x9C00_8DC4)
    UINT32  RGST_Group0283_Reg18                  ; // 18  (ADDR : 0x9C00_8DC8)
    UINT32  RGST_Group0283_Reg19                  ; // 19  (ADDR : 0x9C00_8DCC)
    UINT32  RGST_Group0283_Reg20                  ; // 20  (ADDR : 0x9C00_8DD0)
    UINT32  RGST_Group0283_Reg21                  ; // 21  (ADDR : 0x9C00_8DD4)
    UINT32  RGST_Group0283_Reg22                  ; // 22  (ADDR : 0x9C00_8DD8)
    UINT32  RGST_Group0283_Reg23                  ; // 23  (ADDR : 0x9C00_8DDC)
    UINT32  RGST_Group0283_Reg24                  ; // 24  (ADDR : 0x9C00_8DE0)
    UINT32  RGST_Group0283_Reg25                  ; // 25  (ADDR : 0x9C00_8DE4)
    UINT32  RGST_Group0283_Reg26                  ; // 26  (ADDR : 0x9C00_8DE8)
    UINT32  RGST_Group0283_Reg27                  ; // 27  (ADDR : 0x9C00_8DEC)
    UINT32  RGST_Group0283_Reg28                  ; // 28  (ADDR : 0x9C00_8DF0)
    UINT32  RGST_Group0283_Reg29                  ; // 29  (ADDR : 0x9C00_8DF4)
    UINT32  RGST_Group0283_Reg30                  ; // 30  (ADDR : 0x9C00_8DF8)
    UINT32  RGST_Group0283_Reg31                  ; // 31  (ADDR : 0x9C00_8DFC)

    //Group   284 : EMU_G2
    UINT32  RGST_Group0284_Reg00                  ; // 00  (ADDR : 0x9C00_8E00)
    UINT32  RGST_Group0284_Reg01                  ; // 01  (ADDR : 0x9C00_8E04)
    UINT32  RGST_Group0284_Reg02                  ; // 02  (ADDR : 0x9C00_8E08)
    UINT32  RGST_Group0284_Reg03                  ; // 03  (ADDR : 0x9C00_8E0C)
    UINT32  RGST_Group0284_Reg04                  ; // 04  (ADDR : 0x9C00_8E10)
    UINT32  RGST_Group0284_Reg05                  ; // 05  (ADDR : 0x9C00_8E14)
    UINT32  RGST_Group0284_Reg06                  ; // 06  (ADDR : 0x9C00_8E18)
    UINT32  RGST_Group0284_Reg07                  ; // 07  (ADDR : 0x9C00_8E1C)
    UINT32  RGST_Group0284_Reg08                  ; // 08  (ADDR : 0x9C00_8E20)
    UINT32  RGST_Group0284_Reg09                  ; // 09  (ADDR : 0x9C00_8E24)
    UINT32  RGST_Group0284_Reg10                  ; // 10  (ADDR : 0x9C00_8E28)
    UINT32  RGST_Group0284_Reg11                  ; // 11  (ADDR : 0x9C00_8E2C)
    UINT32  RGST_Group0284_Reg12                  ; // 12  (ADDR : 0x9C00_8E30)
    UINT32  RGST_Group0284_Reg13                  ; // 13  (ADDR : 0x9C00_8E34)
    UINT32  RGST_Group0284_Reg14                  ; // 14  (ADDR : 0x9C00_8E38)
    UINT32  RGST_Group0284_Reg15                  ; // 15  (ADDR : 0x9C00_8E3C)
    UINT32  RGST_Group0284_Reg16                  ; // 16  (ADDR : 0x9C00_8E40)
    UINT32  RGST_Group0284_Reg17                  ; // 17  (ADDR : 0x9C00_8E44)
    UINT32  RGST_Group0284_Reg18                  ; // 18  (ADDR : 0x9C00_8E48)
    UINT32  RGST_Group0284_Reg19                  ; // 19  (ADDR : 0x9C00_8E4C)
    UINT32  RGST_Group0284_Reg20                  ; // 20  (ADDR : 0x9C00_8E50)
    UINT32  RGST_Group0284_Reg21                  ; // 21  (ADDR : 0x9C00_8E54)
    UINT32  RGST_Group0284_Reg22                  ; // 22  (ADDR : 0x9C00_8E58)
    UINT32  RGST_Group0284_Reg23                  ; // 23  (ADDR : 0x9C00_8E5C)
    UINT32  RGST_Group0284_Reg24                  ; // 24  (ADDR : 0x9C00_8E60)
    UINT32  RGST_Group0284_Reg25                  ; // 25  (ADDR : 0x9C00_8E64)
    UINT32  RGST_Group0284_Reg26                  ; // 26  (ADDR : 0x9C00_8E68)
    UINT32  RGST_Group0284_Reg27                  ; // 27  (ADDR : 0x9C00_8E6C)
    UINT32  RGST_Group0284_Reg28                  ; // 28  (ADDR : 0x9C00_8E70)
    UINT32  RGST_Group0284_Reg29                  ; // 29  (ADDR : 0x9C00_8E74)
    UINT32  RGST_Group0284_Reg30                  ; // 30  (ADDR : 0x9C00_8E78)
    UINT32  RGST_Group0284_Reg31                  ; // 31  (ADDR : 0x9C00_8E7C)

    //Group   285 ~ 290 : Reserved
    UINT32  RGST_Group0285_Reserved[32]           ; //     (ADDR : 0x9C00_8E80) ~ (ADDR : 0x9C00_8EFC)
    UINT32  RGST_Group0286_Reserved[32]           ; //     (ADDR : 0x9C00_8F00) ~ (ADDR : 0x9C00_8F7C)
    UINT32  RGST_Group0287_Reserved[32]           ; //     (ADDR : 0x9C00_8F80) ~ (ADDR : 0x9C00_8FFC)
    UINT32  RGST_Group0288_Reserved[32]           ; //     (ADDR : 0x9C00_9000) ~ (ADDR : 0x9C00_907C)
    UINT32  RGST_Group0289_Reserved[32]           ; //     (ADDR : 0x9C00_9080) ~ (ADDR : 0x9C00_90FC)
    UINT32  RGST_Group0290_Reserved[32]           ; //     (ADDR : 0x9C00_9100) ~ (ADDR : 0x9C00_917C)

    //Group   291 : TNR0
    UINT32  RGST_Group0291_Reg00                  ; // 00  (ADDR : 0x9C00_9180)
    UINT32  RGST_Group0291_Reg01                  ; // 01  (ADDR : 0x9C00_9184)
    UINT32  RGST_Group0291_Reg02                  ; // 02  (ADDR : 0x9C00_9188)
    UINT32  RGST_Group0291_Reg03                  ; // 03  (ADDR : 0x9C00_918C)
    UINT32  RGST_Group0291_Reg04                  ; // 04  (ADDR : 0x9C00_9190)
    UINT32  RGST_Group0291_Reg05                  ; // 05  (ADDR : 0x9C00_9194)
    UINT32  RGST_Group0291_Reg06                  ; // 06  (ADDR : 0x9C00_9198)
    UINT32  RGST_Group0291_Reg07                  ; // 07  (ADDR : 0x9C00_919C)
    UINT32  RGST_Group0291_Reg08                  ; // 08  (ADDR : 0x9C00_91A0)
    UINT32  RGST_Group0291_Reg09                  ; // 09  (ADDR : 0x9C00_91A4)
    UINT32  RGST_Group0291_Reg10                  ; // 10  (ADDR : 0x9C00_91A8)
    UINT32  RGST_Group0291_Reg11                  ; // 11  (ADDR : 0x9C00_91AC)
    UINT32  RGST_Group0291_Reg12                  ; // 12  (ADDR : 0x9C00_91B0)
    UINT32  RGST_Group0291_Reg13                  ; // 13  (ADDR : 0x9C00_91B4)
    UINT32  RGST_Group0291_Reg14                  ; // 14  (ADDR : 0x9C00_91B8)
    UINT32  RGST_Group0291_Reg15                  ; // 15  (ADDR : 0x9C00_91BC)
    UINT32  RGST_Group0291_Reg16                  ; // 16  (ADDR : 0x9C00_91C0)
    UINT32  RGST_Group0291_Reg17                  ; // 17  (ADDR : 0x9C00_91C4)
    UINT32  RGST_Group0291_Reg18                  ; // 18  (ADDR : 0x9C00_91C8)
    UINT32  RGST_Group0291_Reg19                  ; // 19  (ADDR : 0x9C00_91CC)
    UINT32  RGST_Group0291_Reg20                  ; // 20  (ADDR : 0x9C00_91D0)
    UINT32  RGST_Group0291_Reg21                  ; // 21  (ADDR : 0x9C00_91D4)
    UINT32  RGST_Group0291_Reg22                  ; // 22  (ADDR : 0x9C00_91D8)
    UINT32  RGST_Group0291_Reg23                  ; // 23  (ADDR : 0x9C00_91DC)
    UINT32  RGST_Group0291_Reg24                  ; // 24  (ADDR : 0x9C00_91E0)
    UINT32  RGST_Group0291_Reg25                  ; // 25  (ADDR : 0x9C00_91E4)
    UINT32  RGST_Group0291_Reg26                  ; // 26  (ADDR : 0x9C00_91E8)
    UINT32  RGST_Group0291_Reg27                  ; // 27  (ADDR : 0x9C00_91EC)
    UINT32  RGST_Group0291_Reg28                  ; // 28  (ADDR : 0x9C00_91F0)
    UINT32  RGST_Group0291_Reg29                  ; // 29  (ADDR : 0x9C00_91F4)
    UINT32  RGST_Group0291_Reg30                  ; // 30  (ADDR : 0x9C00_91F8)
    UINT32  RGST_Group0291_Reg31                  ; // 31  (ADDR : 0x9C00_91FC)

    //Group   292 : TNR0
    UINT32  RGST_Group0292_Reg00                  ; // 00  (ADDR : 0x9C00_9200)
    UINT32  RGST_Group0292_Reg01                  ; // 01  (ADDR : 0x9C00_9204)
    UINT32  RGST_Group0292_Reg02                  ; // 02  (ADDR : 0x9C00_9208)
    UINT32  RGST_Group0292_Reg03                  ; // 03  (ADDR : 0x9C00_920C)
    UINT32  RGST_Group0292_Reg04                  ; // 04  (ADDR : 0x9C00_9210)
    UINT32  RGST_Group0292_Reg05                  ; // 05  (ADDR : 0x9C00_9214)
    UINT32  RGST_Group0292_Reg06                  ; // 06  (ADDR : 0x9C00_9218)
    UINT32  RGST_Group0292_Reg07                  ; // 07  (ADDR : 0x9C00_921C)
    UINT32  RGST_Group0292_Reg08                  ; // 08  (ADDR : 0x9C00_9220)
    UINT32  RGST_Group0292_Reg09                  ; // 09  (ADDR : 0x9C00_9224)
    UINT32  RGST_Group0292_Reg10                  ; // 10  (ADDR : 0x9C00_9228)
    UINT32  RGST_Group0292_Reg11                  ; // 11  (ADDR : 0x9C00_922C)
    UINT32  RGST_Group0292_Reg12                  ; // 12  (ADDR : 0x9C00_9230)
    UINT32  RGST_Group0292_Reg13                  ; // 13  (ADDR : 0x9C00_9234)
    UINT32  RGST_Group0292_Reg14                  ; // 14  (ADDR : 0x9C00_9238)
    UINT32  RGST_Group0292_Reg15                  ; // 15  (ADDR : 0x9C00_923C)
    UINT32  RGST_Group0292_Reg16                  ; // 16  (ADDR : 0x9C00_9240)
    UINT32  RGST_Group0292_Reg17                  ; // 17  (ADDR : 0x9C00_9244)
    UINT32  RGST_Group0292_Reg18                  ; // 18  (ADDR : 0x9C00_9248)
    UINT32  RGST_Group0292_Reg19                  ; // 19  (ADDR : 0x9C00_924C)
    UINT32  RGST_Group0292_Reg20                  ; // 20  (ADDR : 0x9C00_9250)
    UINT32  RGST_Group0292_Reg21                  ; // 21  (ADDR : 0x9C00_9254)
    UINT32  RGST_Group0292_Reg22                  ; // 22  (ADDR : 0x9C00_9258)
    UINT32  RGST_Group0292_Reg23                  ; // 23  (ADDR : 0x9C00_925C)
    UINT32  RGST_Group0292_Reg24                  ; // 24  (ADDR : 0x9C00_9260)
    UINT32  RGST_Group0292_Reg25                  ; // 25  (ADDR : 0x9C00_9264)
    UINT32  RGST_Group0292_Reg26                  ; // 26  (ADDR : 0x9C00_9268)
    UINT32  RGST_Group0292_Reg27                  ; // 27  (ADDR : 0x9C00_926C)
    UINT32  RGST_Group0292_Reg28                  ; // 28  (ADDR : 0x9C00_9270)
    UINT32  RGST_Group0292_Reg29                  ; // 29  (ADDR : 0x9C00_9274)
    UINT32  RGST_Group0292_Reg30                  ; // 30  (ADDR : 0x9C00_9278)
    UINT32  RGST_Group0292_Reg31                  ; // 31  (ADDR : 0x9C00_927C)

    //Group   293 : TNR1
    UINT32  RGST_Group0293_Reg00                  ; // 00  (ADDR : 0x9C00_9280)
    UINT32  RGST_Group0293_Reg01                  ; // 01  (ADDR : 0x9C00_9284)
    UINT32  RGST_Group0293_Reg02                  ; // 02  (ADDR : 0x9C00_9288)
    UINT32  RGST_Group0293_Reg03                  ; // 03  (ADDR : 0x9C00_928C)
    UINT32  RGST_Group0293_Reg04                  ; // 04  (ADDR : 0x9C00_9290)
    UINT32  RGST_Group0293_Reg05                  ; // 05  (ADDR : 0x9C00_9294)
    UINT32  RGST_Group0293_Reg06                  ; // 06  (ADDR : 0x9C00_9298)
    UINT32  RGST_Group0293_Reg07                  ; // 07  (ADDR : 0x9C00_929C)
    UINT32  RGST_Group0293_Reg08                  ; // 08  (ADDR : 0x9C00_92A0)
    UINT32  RGST_Group0293_Reg09                  ; // 09  (ADDR : 0x9C00_92A4)
    UINT32  RGST_Group0293_Reg10                  ; // 10  (ADDR : 0x9C00_92A8)
    UINT32  RGST_Group0293_Reg11                  ; // 11  (ADDR : 0x9C00_92AC)
    UINT32  RGST_Group0293_Reg12                  ; // 12  (ADDR : 0x9C00_92B0)
    UINT32  RGST_Group0293_Reg13                  ; // 13  (ADDR : 0x9C00_92B4)
    UINT32  RGST_Group0293_Reg14                  ; // 14  (ADDR : 0x9C00_92B8)
    UINT32  RGST_Group0293_Reg15                  ; // 15  (ADDR : 0x9C00_92BC)
    UINT32  RGST_Group0293_Reg16                  ; // 16  (ADDR : 0x9C00_92C0)
    UINT32  RGST_Group0293_Reg17                  ; // 17  (ADDR : 0x9C00_92C4)
    UINT32  RGST_Group0293_Reg18                  ; // 18  (ADDR : 0x9C00_92C8)
    UINT32  RGST_Group0293_Reg19                  ; // 19  (ADDR : 0x9C00_92CC)
    UINT32  RGST_Group0293_Reg20                  ; // 20  (ADDR : 0x9C00_92D0)
    UINT32  RGST_Group0293_Reg21                  ; // 21  (ADDR : 0x9C00_92D4)
    UINT32  RGST_Group0293_Reg22                  ; // 22  (ADDR : 0x9C00_92D8)
    UINT32  RGST_Group0293_Reg23                  ; // 23  (ADDR : 0x9C00_92DC)
    UINT32  RGST_Group0293_Reg24                  ; // 24  (ADDR : 0x9C00_92E0)
    UINT32  RGST_Group0293_Reg25                  ; // 25  (ADDR : 0x9C00_92E4)
    UINT32  RGST_Group0293_Reg26                  ; // 26  (ADDR : 0x9C00_92E8)
    UINT32  RGST_Group0293_Reg27                  ; // 27  (ADDR : 0x9C00_92EC)
    UINT32  RGST_Group0293_Reg28                  ; // 28  (ADDR : 0x9C00_92F0)
    UINT32  RGST_Group0293_Reg29                  ; // 29  (ADDR : 0x9C00_92F4)
    UINT32  RGST_Group0293_Reg30                  ; // 30  (ADDR : 0x9C00_92F8)
    UINT32  RGST_Group0293_Reg31                  ; // 31  (ADDR : 0x9C00_92FC)

    //Group   294 : TNR1
    UINT32  RGST_Group0294_Reg00                  ; // 00  (ADDR : 0x9C00_9300)
    UINT32  RGST_Group0294_Reg01                  ; // 01  (ADDR : 0x9C00_9304)
    UINT32  RGST_Group0294_Reg02                  ; // 02  (ADDR : 0x9C00_9308)
    UINT32  RGST_Group0294_Reg03                  ; // 03  (ADDR : 0x9C00_930C)
    UINT32  RGST_Group0294_Reg04                  ; // 04  (ADDR : 0x9C00_9310)
    UINT32  RGST_Group0294_Reg05                  ; // 05  (ADDR : 0x9C00_9314)
    UINT32  RGST_Group0294_Reg06                  ; // 06  (ADDR : 0x9C00_9318)
    UINT32  RGST_Group0294_Reg07                  ; // 07  (ADDR : 0x9C00_931C)
    UINT32  RGST_Group0294_Reg08                  ; // 08  (ADDR : 0x9C00_9320)
    UINT32  RGST_Group0294_Reg09                  ; // 09  (ADDR : 0x9C00_9324)
    UINT32  RGST_Group0294_Reg10                  ; // 10  (ADDR : 0x9C00_9328)
    UINT32  RGST_Group0294_Reg11                  ; // 11  (ADDR : 0x9C00_932C)
    UINT32  RGST_Group0294_Reg12                  ; // 12  (ADDR : 0x9C00_9330)
    UINT32  RGST_Group0294_Reg13                  ; // 13  (ADDR : 0x9C00_9334)
    UINT32  RGST_Group0294_Reg14                  ; // 14  (ADDR : 0x9C00_9338)
    UINT32  RGST_Group0294_Reg15                  ; // 15  (ADDR : 0x9C00_933C)
    UINT32  RGST_Group0294_Reg16                  ; // 16  (ADDR : 0x9C00_9340)
    UINT32  RGST_Group0294_Reg17                  ; // 17  (ADDR : 0x9C00_9344)
    UINT32  RGST_Group0294_Reg18                  ; // 18  (ADDR : 0x9C00_9348)
    UINT32  RGST_Group0294_Reg19                  ; // 19  (ADDR : 0x9C00_934C)
    UINT32  RGST_Group0294_Reg20                  ; // 20  (ADDR : 0x9C00_9350)
    UINT32  RGST_Group0294_Reg21                  ; // 21  (ADDR : 0x9C00_9354)
    UINT32  RGST_Group0294_Reg22                  ; // 22  (ADDR : 0x9C00_9358)
    UINT32  RGST_Group0294_Reg23                  ; // 23  (ADDR : 0x9C00_935C)
    UINT32  RGST_Group0294_Reg24                  ; // 24  (ADDR : 0x9C00_9360)
    UINT32  RGST_Group0294_Reg25                  ; // 25  (ADDR : 0x9C00_9364)
    UINT32  RGST_Group0294_Reg26                  ; // 26  (ADDR : 0x9C00_9368)
    UINT32  RGST_Group0294_Reg27                  ; // 27  (ADDR : 0x9C00_936C)
    UINT32  RGST_Group0294_Reg28                  ; // 28  (ADDR : 0x9C00_9370)
    UINT32  RGST_Group0294_Reg29                  ; // 29  (ADDR : 0x9C00_9374)
    UINT32  RGST_Group0294_Reg30                  ; // 30  (ADDR : 0x9C00_9378)
    UINT32  RGST_Group0294_Reg31                  ; // 31  (ADDR : 0x9C00_937C)

    //Group   295 : TNRIW0
    UINT32  RGST_Group0295_Reg00                  ; // 00  (ADDR : 0x9C00_9380)
    UINT32  RGST_Group0295_Reg01                  ; // 01  (ADDR : 0x9C00_9384)
    UINT32  RGST_Group0295_Reg02                  ; // 02  (ADDR : 0x9C00_9388)
    UINT32  RGST_Group0295_Reg03                  ; // 03  (ADDR : 0x9C00_938C)
    UINT32  RGST_Group0295_Reg04                  ; // 04  (ADDR : 0x9C00_9390)
    UINT32  RGST_Group0295_Reg05                  ; // 05  (ADDR : 0x9C00_9394)
    UINT32  RGST_Group0295_Reg06                  ; // 06  (ADDR : 0x9C00_9398)
    UINT32  RGST_Group0295_Reg07                  ; // 07  (ADDR : 0x9C00_939C)
    UINT32  RGST_Group0295_Reg08                  ; // 08  (ADDR : 0x9C00_93A0)
    UINT32  RGST_Group0295_Reg09                  ; // 09  (ADDR : 0x9C00_93A4)
    UINT32  RGST_Group0295_Reg10                  ; // 10  (ADDR : 0x9C00_93A8)
    UINT32  RGST_Group0295_Reg11                  ; // 11  (ADDR : 0x9C00_93AC)
    UINT32  RGST_Group0295_Reg12                  ; // 12  (ADDR : 0x9C00_93B0)
    UINT32  RGST_Group0295_Reg13                  ; // 13  (ADDR : 0x9C00_93B4)
    UINT32  RGST_Group0295_Reg14                  ; // 14  (ADDR : 0x9C00_93B8)
    UINT32  RGST_Group0295_Reg15                  ; // 15  (ADDR : 0x9C00_93BC)
    UINT32  RGST_Group0295_Reg16                  ; // 16  (ADDR : 0x9C00_93C0)
    UINT32  RGST_Group0295_Reg17                  ; // 17  (ADDR : 0x9C00_93C4)
    UINT32  RGST_Group0295_Reg18                  ; // 18  (ADDR : 0x9C00_93C8)
    UINT32  RGST_Group0295_Reg19                  ; // 19  (ADDR : 0x9C00_93CC)
    UINT32  RGST_Group0295_Reg20                  ; // 20  (ADDR : 0x9C00_93D0)
    UINT32  RGST_Group0295_Reg21                  ; // 21  (ADDR : 0x9C00_93D4)
    UINT32  RGST_Group0295_Reg22                  ; // 22  (ADDR : 0x9C00_93D8)
    UINT32  RGST_Group0295_Reg23                  ; // 23  (ADDR : 0x9C00_93DC)
    UINT32  RGST_Group0295_Reg24                  ; // 24  (ADDR : 0x9C00_93E0)
    UINT32  RGST_Group0295_Reg25                  ; // 25  (ADDR : 0x9C00_93E4)
    UINT32  RGST_Group0295_Reg26                  ; // 26  (ADDR : 0x9C00_93E8)
    UINT32  RGST_Group0295_Reg27                  ; // 27  (ADDR : 0x9C00_93EC)
    UINT32  RGST_Group0295_Reg28                  ; // 28  (ADDR : 0x9C00_93F0)
    UINT32  RGST_Group0295_Reg29                  ; // 29  (ADDR : 0x9C00_93F4)
    UINT32  RGST_Group0295_Reg30                  ; // 30  (ADDR : 0x9C00_93F8)
    UINT32  RGST_Group0295_Reg31                  ; // 31  (ADDR : 0x9C00_93FC)

    //Group   296 : TNRIW1
    UINT32  RGST_Group0296_Reg00                  ; // 00  (ADDR : 0x9C00_9400)
    UINT32  RGST_Group0296_Reg01                  ; // 01  (ADDR : 0x9C00_9404)
    UINT32  RGST_Group0296_Reg02                  ; // 02  (ADDR : 0x9C00_9408)
    UINT32  RGST_Group0296_Reg03                  ; // 03  (ADDR : 0x9C00_940C)
    UINT32  RGST_Group0296_Reg04                  ; // 04  (ADDR : 0x9C00_9410)
    UINT32  RGST_Group0296_Reg05                  ; // 05  (ADDR : 0x9C00_9414)
    UINT32  RGST_Group0296_Reg06                  ; // 06  (ADDR : 0x9C00_9418)
    UINT32  RGST_Group0296_Reg07                  ; // 07  (ADDR : 0x9C00_941C)
    UINT32  RGST_Group0296_Reg08                  ; // 08  (ADDR : 0x9C00_9420)
    UINT32  RGST_Group0296_Reg09                  ; // 09  (ADDR : 0x9C00_9424)
    UINT32  RGST_Group0296_Reg10                  ; // 10  (ADDR : 0x9C00_9428)
    UINT32  RGST_Group0296_Reg11                  ; // 11  (ADDR : 0x9C00_942C)
    UINT32  RGST_Group0296_Reg12                  ; // 12  (ADDR : 0x9C00_9430)
    UINT32  RGST_Group0296_Reg13                  ; // 13  (ADDR : 0x9C00_9434)
    UINT32  RGST_Group0296_Reg14                  ; // 14  (ADDR : 0x9C00_9438)
    UINT32  RGST_Group0296_Reg15                  ; // 15  (ADDR : 0x9C00_943C)
    UINT32  RGST_Group0296_Reg16                  ; // 16  (ADDR : 0x9C00_9440)
    UINT32  RGST_Group0296_Reg17                  ; // 17  (ADDR : 0x9C00_9444)
    UINT32  RGST_Group0296_Reg18                  ; // 18  (ADDR : 0x9C00_9448)
    UINT32  RGST_Group0296_Reg19                  ; // 19  (ADDR : 0x9C00_944C)
    UINT32  RGST_Group0296_Reg20                  ; // 20  (ADDR : 0x9C00_9450)
    UINT32  RGST_Group0296_Reg21                  ; // 21  (ADDR : 0x9C00_9454)
    UINT32  RGST_Group0296_Reg22                  ; // 22  (ADDR : 0x9C00_9458)
    UINT32  RGST_Group0296_Reg23                  ; // 23  (ADDR : 0x9C00_945C)
    UINT32  RGST_Group0296_Reg24                  ; // 24  (ADDR : 0x9C00_9460)
    UINT32  RGST_Group0296_Reg25                  ; // 25  (ADDR : 0x9C00_9464)
    UINT32  RGST_Group0296_Reg26                  ; // 26  (ADDR : 0x9C00_9468)
    UINT32  RGST_Group0296_Reg27                  ; // 27  (ADDR : 0x9C00_946C)
    UINT32  RGST_Group0296_Reg28                  ; // 28  (ADDR : 0x9C00_9470)
    UINT32  RGST_Group0296_Reg29                  ; // 29  (ADDR : 0x9C00_9474)
    UINT32  RGST_Group0296_Reg30                  ; // 30  (ADDR : 0x9C00_9478)
    UINT32  RGST_Group0296_Reg31                  ; // 31  (ADDR : 0x9C00_947C)

    //Group   297 : VSCL0
    UINT32  RGST_Group0297_Reg00                  ; // 00  (ADDR : 0x9C00_9480)
    UINT32  RGST_Group0297_Reg01                  ; // 01  (ADDR : 0x9C00_9484)
    UINT32  RGST_Group0297_Reg02                  ; // 02  (ADDR : 0x9C00_9488)
    UINT32  RGST_Group0297_Reg03                  ; // 03  (ADDR : 0x9C00_948C)
    UINT32  RGST_Group0297_Reg04                  ; // 04  (ADDR : 0x9C00_9490)
    UINT32  RGST_Group0297_Reg05                  ; // 05  (ADDR : 0x9C00_9494)
    UINT32  RGST_Group0297_Reg06                  ; // 06  (ADDR : 0x9C00_9498)
    UINT32  RGST_Group0297_Reg07                  ; // 07  (ADDR : 0x9C00_949C)
    UINT32  RGST_Group0297_Reg08                  ; // 08  (ADDR : 0x9C00_94A0)
    UINT32  RGST_Group0297_Reg09                  ; // 09  (ADDR : 0x9C00_94A4)
    UINT32  RGST_Group0297_Reg10                  ; // 10  (ADDR : 0x9C00_94A8)
    UINT32  RGST_Group0297_Reg11                  ; // 11  (ADDR : 0x9C00_94AC)
    UINT32  RGST_Group0297_Reg12                  ; // 12  (ADDR : 0x9C00_94B0)
    UINT32  RGST_Group0297_Reg13                  ; // 13  (ADDR : 0x9C00_94B4)
    UINT32  RGST_Group0297_Reg14                  ; // 14  (ADDR : 0x9C00_94B8)
    UINT32  RGST_Group0297_Reg15                  ; // 15  (ADDR : 0x9C00_94BC)
    UINT32  RGST_Group0297_Reg16                  ; // 16  (ADDR : 0x9C00_94C0)
    UINT32  RGST_Group0297_Reg17                  ; // 17  (ADDR : 0x9C00_94C4)
    UINT32  RGST_Group0297_Reg18                  ; // 18  (ADDR : 0x9C00_94C8)
    UINT32  RGST_Group0297_Reg19                  ; // 19  (ADDR : 0x9C00_94CC)
    UINT32  RGST_Group0297_Reg20                  ; // 20  (ADDR : 0x9C00_94D0)
    UINT32  RGST_Group0297_Reg21                  ; // 21  (ADDR : 0x9C00_94D4)
    UINT32  RGST_Group0297_Reg22                  ; // 22  (ADDR : 0x9C00_94D8)
    UINT32  RGST_Group0297_Reg23                  ; // 23  (ADDR : 0x9C00_94DC)
    UINT32  RGST_Group0297_Reg24                  ; // 24  (ADDR : 0x9C00_94E0)
    UINT32  RGST_Group0297_Reg25                  ; // 25  (ADDR : 0x9C00_94E4)
    UINT32  RGST_Group0297_Reg26                  ; // 26  (ADDR : 0x9C00_94E8)
    UINT32  RGST_Group0297_Reg27                  ; // 27  (ADDR : 0x9C00_94EC)
    UINT32  RGST_Group0297_Reg28                  ; // 28  (ADDR : 0x9C00_94F0)
    UINT32  RGST_Group0297_Reg29                  ; // 29  (ADDR : 0x9C00_94F4)
    UINT32  RGST_Group0297_Reg30                  ; // 30  (ADDR : 0x9C00_94F8)
    UINT32  RGST_Group0297_Reg31                  ; // 31  (ADDR : 0x9C00_94FC)

    //Group   298 : IMG0
    UINT32  RGST_Group0298_Reg00                  ; // 00  (ADDR : 0x9C00_9500)
    UINT32  RGST_Group0298_Reg01                  ; // 01  (ADDR : 0x9C00_9504)
    UINT32  RGST_Group0298_Reg02                  ; // 02  (ADDR : 0x9C00_9508)
    UINT32  RGST_Group0298_Reg03                  ; // 03  (ADDR : 0x9C00_950C)
    UINT32  RGST_Group0298_Reg04                  ; // 04  (ADDR : 0x9C00_9510)
    UINT32  RGST_Group0298_Reg05                  ; // 05  (ADDR : 0x9C00_9514)
    UINT32  RGST_Group0298_Reg06                  ; // 06  (ADDR : 0x9C00_9518)
    UINT32  RGST_Group0298_Reg07                  ; // 07  (ADDR : 0x9C00_951C)
    UINT32  RGST_Group0298_Reg08                  ; // 08  (ADDR : 0x9C00_9520)
    UINT32  RGST_Group0298_Reg09                  ; // 09  (ADDR : 0x9C00_9524)
    UINT32  RGST_Group0298_Reg10                  ; // 10  (ADDR : 0x9C00_9528)
    UINT32  RGST_Group0298_Reg11                  ; // 11  (ADDR : 0x9C00_952C)
    UINT32  RGST_Group0298_Reg12                  ; // 12  (ADDR : 0x9C00_9530)
    UINT32  RGST_Group0298_Reg13                  ; // 13  (ADDR : 0x9C00_9534)
    UINT32  RGST_Group0298_Reg14                  ; // 14  (ADDR : 0x9C00_9538)
    UINT32  RGST_Group0298_Reg15                  ; // 15  (ADDR : 0x9C00_953C)
    UINT32  RGST_Group0298_Reg16                  ; // 16  (ADDR : 0x9C00_9540)
    UINT32  RGST_Group0298_Reg17                  ; // 17  (ADDR : 0x9C00_9544)
    UINT32  RGST_Group0298_Reg18                  ; // 18  (ADDR : 0x9C00_9548)
    UINT32  RGST_Group0298_Reg19                  ; // 19  (ADDR : 0x9C00_954C)
    UINT32  RGST_Group0298_Reg20                  ; // 20  (ADDR : 0x9C00_9550)
    UINT32  RGST_Group0298_Reg21                  ; // 21  (ADDR : 0x9C00_9554)
    UINT32  RGST_Group0298_Reg22                  ; // 22  (ADDR : 0x9C00_9558)
    UINT32  RGST_Group0298_Reg23                  ; // 23  (ADDR : 0x9C00_955C)
    UINT32  RGST_Group0298_Reg24                  ; // 24  (ADDR : 0x9C00_9560)
    UINT32  RGST_Group0298_Reg25                  ; // 25  (ADDR : 0x9C00_9564)
    UINT32  RGST_Group0298_Reg26                  ; // 26  (ADDR : 0x9C00_9568)
    UINT32  RGST_Group0298_Reg27                  ; // 27  (ADDR : 0x9C00_956C)
    UINT32  RGST_Group0298_Reg28                  ; // 28  (ADDR : 0x9C00_9570)
    UINT32  RGST_Group0298_Reg29                  ; // 29  (ADDR : 0x9C00_9574)
    UINT32  RGST_Group0298_Reg30                  ; // 30  (ADDR : 0x9C00_9578)
    UINT32  RGST_Group0298_Reg31                  ; // 31  (ADDR : 0x9C00_957C)

    //Group   299 : VSCL1
    UINT32  RGST_Group0299_Reg00                  ; // 00  (ADDR : 0x9C00_9580)
    UINT32  RGST_Group0299_Reg01                  ; // 01  (ADDR : 0x9C00_9584)
    UINT32  RGST_Group0299_Reg02                  ; // 02  (ADDR : 0x9C00_9588)
    UINT32  RGST_Group0299_Reg03                  ; // 03  (ADDR : 0x9C00_958C)
    UINT32  RGST_Group0299_Reg04                  ; // 04  (ADDR : 0x9C00_9590)
    UINT32  RGST_Group0299_Reg05                  ; // 05  (ADDR : 0x9C00_9594)
    UINT32  RGST_Group0299_Reg06                  ; // 06  (ADDR : 0x9C00_9598)
    UINT32  RGST_Group0299_Reg07                  ; // 07  (ADDR : 0x9C00_959C)
    UINT32  RGST_Group0299_Reg08                  ; // 08  (ADDR : 0x9C00_95A0)
    UINT32  RGST_Group0299_Reg09                  ; // 09  (ADDR : 0x9C00_95A4)
    UINT32  RGST_Group0299_Reg10                  ; // 10  (ADDR : 0x9C00_95A8)
    UINT32  RGST_Group0299_Reg11                  ; // 11  (ADDR : 0x9C00_95AC)
    UINT32  RGST_Group0299_Reg12                  ; // 12  (ADDR : 0x9C00_95B0)
    UINT32  RGST_Group0299_Reg13                  ; // 13  (ADDR : 0x9C00_95B4)
    UINT32  RGST_Group0299_Reg14                  ; // 14  (ADDR : 0x9C00_95B8)
    UINT32  RGST_Group0299_Reg15                  ; // 15  (ADDR : 0x9C00_95BC)
    UINT32  RGST_Group0299_Reg16                  ; // 16  (ADDR : 0x9C00_95C0)
    UINT32  RGST_Group0299_Reg17                  ; // 17  (ADDR : 0x9C00_95C4)
    UINT32  RGST_Group0299_Reg18                  ; // 18  (ADDR : 0x9C00_95C8)
    UINT32  RGST_Group0299_Reg19                  ; // 19  (ADDR : 0x9C00_95CC)
    UINT32  RGST_Group0299_Reg20                  ; // 20  (ADDR : 0x9C00_95D0)
    UINT32  RGST_Group0299_Reg21                  ; // 21  (ADDR : 0x9C00_95D4)
    UINT32  RGST_Group0299_Reg22                  ; // 22  (ADDR : 0x9C00_95D8)
    UINT32  RGST_Group0299_Reg23                  ; // 23  (ADDR : 0x9C00_95DC)
    UINT32  RGST_Group0299_Reg24                  ; // 24  (ADDR : 0x9C00_95E0)
    UINT32  RGST_Group0299_Reg25                  ; // 25  (ADDR : 0x9C00_95E4)
    UINT32  RGST_Group0299_Reg26                  ; // 26  (ADDR : 0x9C00_95E8)
    UINT32  RGST_Group0299_Reg27                  ; // 27  (ADDR : 0x9C00_95EC)
    UINT32  RGST_Group0299_Reg28                  ; // 28  (ADDR : 0x9C00_95F0)
    UINT32  RGST_Group0299_Reg29                  ; // 29  (ADDR : 0x9C00_95F4)
    UINT32  RGST_Group0299_Reg30                  ; // 30  (ADDR : 0x9C00_95F8)
    UINT32  RGST_Group0299_Reg31                  ; // 31  (ADDR : 0x9C00_95FC)

    //Group   300 : IMG1
    UINT32  RGST_Group0300_Reg00                  ; // 00  (ADDR : 0x9C00_9600)
    UINT32  RGST_Group0300_Reg01                  ; // 01  (ADDR : 0x9C00_9604)
    UINT32  RGST_Group0300_Reg02                  ; // 02  (ADDR : 0x9C00_9608)
    UINT32  RGST_Group0300_Reg03                  ; // 03  (ADDR : 0x9C00_960C)
    UINT32  RGST_Group0300_Reg04                  ; // 04  (ADDR : 0x9C00_9610)
    UINT32  RGST_Group0300_Reg05                  ; // 05  (ADDR : 0x9C00_9614)
    UINT32  RGST_Group0300_Reg06                  ; // 06  (ADDR : 0x9C00_9618)
    UINT32  RGST_Group0300_Reg07                  ; // 07  (ADDR : 0x9C00_961C)
    UINT32  RGST_Group0300_Reg08                  ; // 08  (ADDR : 0x9C00_9620)
    UINT32  RGST_Group0300_Reg09                  ; // 09  (ADDR : 0x9C00_9624)
    UINT32  RGST_Group0300_Reg10                  ; // 10  (ADDR : 0x9C00_9628)
    UINT32  RGST_Group0300_Reg11                  ; // 11  (ADDR : 0x9C00_962C)
    UINT32  RGST_Group0300_Reg12                  ; // 12  (ADDR : 0x9C00_9630)
    UINT32  RGST_Group0300_Reg13                  ; // 13  (ADDR : 0x9C00_9634)
    UINT32  RGST_Group0300_Reg14                  ; // 14  (ADDR : 0x9C00_9638)
    UINT32  RGST_Group0300_Reg15                  ; // 15  (ADDR : 0x9C00_963C)
    UINT32  RGST_Group0300_Reg16                  ; // 16  (ADDR : 0x9C00_9640)
    UINT32  RGST_Group0300_Reg17                  ; // 17  (ADDR : 0x9C00_9644)
    UINT32  RGST_Group0300_Reg18                  ; // 18  (ADDR : 0x9C00_9648)
    UINT32  RGST_Group0300_Reg19                  ; // 19  (ADDR : 0x9C00_964C)
    UINT32  RGST_Group0300_Reg20                  ; // 20  (ADDR : 0x9C00_9650)
    UINT32  RGST_Group0300_Reg21                  ; // 21  (ADDR : 0x9C00_9654)
    UINT32  RGST_Group0300_Reg22                  ; // 22  (ADDR : 0x9C00_9658)
    UINT32  RGST_Group0300_Reg23                  ; // 23  (ADDR : 0x9C00_965C)
    UINT32  RGST_Group0300_Reg24                  ; // 24  (ADDR : 0x9C00_9660)
    UINT32  RGST_Group0300_Reg25                  ; // 25  (ADDR : 0x9C00_9664)
    UINT32  RGST_Group0300_Reg26                  ; // 26  (ADDR : 0x9C00_9668)
    UINT32  RGST_Group0300_Reg27                  ; // 27  (ADDR : 0x9C00_966C)
    UINT32  RGST_Group0300_Reg28                  ; // 28  (ADDR : 0x9C00_9670)
    UINT32  RGST_Group0300_Reg29                  ; // 29  (ADDR : 0x9C00_9674)
    UINT32  RGST_Group0300_Reg30                  ; // 30  (ADDR : 0x9C00_9678)
    UINT32  RGST_Group0300_Reg31                  ; // 31  (ADDR : 0x9C00_967C)

    //Group   301 : IMG0
    UINT32  RGST_Group0301_Reg00                  ; // 00  (ADDR : 0x9C00_9680)
    UINT32  RGST_Group0301_Reg01                  ; // 01  (ADDR : 0x9C00_9684)
    UINT32  RGST_Group0301_Reg02                  ; // 02  (ADDR : 0x9C00_9688)
    UINT32  RGST_Group0301_Reg03                  ; // 03  (ADDR : 0x9C00_968C)
    UINT32  RGST_Group0301_Reg04                  ; // 04  (ADDR : 0x9C00_9690)
    UINT32  RGST_Group0301_Reg05                  ; // 05  (ADDR : 0x9C00_9694)
    UINT32  RGST_Group0301_Reg06                  ; // 06  (ADDR : 0x9C00_9698)
    UINT32  RGST_Group0301_Reg07                  ; // 07  (ADDR : 0x9C00_969C)
    UINT32  RGST_Group0301_Reg08                  ; // 08  (ADDR : 0x9C00_96A0)
    UINT32  RGST_Group0301_Reg09                  ; // 09  (ADDR : 0x9C00_96A4)
    UINT32  RGST_Group0301_Reg10                  ; // 10  (ADDR : 0x9C00_96A8)
    UINT32  RGST_Group0301_Reg11                  ; // 11  (ADDR : 0x9C00_96AC)
    UINT32  RGST_Group0301_Reg12                  ; // 12  (ADDR : 0x9C00_96B0)
    UINT32  RGST_Group0301_Reg13                  ; // 13  (ADDR : 0x9C00_96B4)
    UINT32  RGST_Group0301_Reg14                  ; // 14  (ADDR : 0x9C00_96B8)
    UINT32  RGST_Group0301_Reg15                  ; // 15  (ADDR : 0x9C00_96BC)
    UINT32  RGST_Group0301_Reg16                  ; // 16  (ADDR : 0x9C00_96C0)
    UINT32  RGST_Group0301_Reg17                  ; // 17  (ADDR : 0x9C00_96C4)
    UINT32  RGST_Group0301_Reg18                  ; // 18  (ADDR : 0x9C00_96C8)
    UINT32  RGST_Group0301_Reg19                  ; // 19  (ADDR : 0x9C00_96CC)
    UINT32  RGST_Group0301_Reg20                  ; // 20  (ADDR : 0x9C00_96D0)
    UINT32  RGST_Group0301_Reg21                  ; // 21  (ADDR : 0x9C00_96D4)
    UINT32  RGST_Group0301_Reg22                  ; // 22  (ADDR : 0x9C00_96D8)
    UINT32  RGST_Group0301_Reg23                  ; // 23  (ADDR : 0x9C00_96DC)
    UINT32  RGST_Group0301_Reg24                  ; // 24  (ADDR : 0x9C00_96E0)
    UINT32  RGST_Group0301_Reg25                  ; // 25  (ADDR : 0x9C00_96E4)
    UINT32  RGST_Group0301_Reg26                  ; // 26  (ADDR : 0x9C00_96E8)
    UINT32  RGST_Group0301_Reg27                  ; // 27  (ADDR : 0x9C00_96EC)
    UINT32  RGST_Group0301_Reg28                  ; // 28  (ADDR : 0x9C00_96F0)
    UINT32  RGST_Group0301_Reg29                  ; // 29  (ADDR : 0x9C00_96F4)
    UINT32  RGST_Group0301_Reg30                  ; // 30  (ADDR : 0x9C00_96F8)
    UINT32  RGST_Group0301_Reg31                  ; // 31  (ADDR : 0x9C00_96FC)

    //Group   302 : IMG0
    UINT32  RGST_Group0302_Reg00                  ; // 00  (ADDR : 0x9C00_9700)
    UINT32  RGST_Group0302_Reg01                  ; // 01  (ADDR : 0x9C00_9704)
    UINT32  RGST_Group0302_Reg02                  ; // 02  (ADDR : 0x9C00_9708)
    UINT32  RGST_Group0302_Reg03                  ; // 03  (ADDR : 0x9C00_970C)
    UINT32  RGST_Group0302_Reg04                  ; // 04  (ADDR : 0x9C00_9710)
    UINT32  RGST_Group0302_Reg05                  ; // 05  (ADDR : 0x9C00_9714)
    UINT32  RGST_Group0302_Reg06                  ; // 06  (ADDR : 0x9C00_9718)
    UINT32  RGST_Group0302_Reg07                  ; // 07  (ADDR : 0x9C00_971C)
    UINT32  RGST_Group0302_Reg08                  ; // 08  (ADDR : 0x9C00_9720)
    UINT32  RGST_Group0302_Reg09                  ; // 09  (ADDR : 0x9C00_9724)
    UINT32  RGST_Group0302_Reg10                  ; // 10  (ADDR : 0x9C00_9728)
    UINT32  RGST_Group0302_Reg11                  ; // 11  (ADDR : 0x9C00_972C)
    UINT32  RGST_Group0302_Reg12                  ; // 12  (ADDR : 0x9C00_9730)
    UINT32  RGST_Group0302_Reg13                  ; // 13  (ADDR : 0x9C00_9734)
    UINT32  RGST_Group0302_Reg14                  ; // 14  (ADDR : 0x9C00_9738)
    UINT32  RGST_Group0302_Reg15                  ; // 15  (ADDR : 0x9C00_973C)
    UINT32  RGST_Group0302_Reg16                  ; // 16  (ADDR : 0x9C00_9740)
    UINT32  RGST_Group0302_Reg17                  ; // 17  (ADDR : 0x9C00_9744)
    UINT32  RGST_Group0302_Reg18                  ; // 18  (ADDR : 0x9C00_9748)
    UINT32  RGST_Group0302_Reg19                  ; // 19  (ADDR : 0x9C00_974C)
    UINT32  RGST_Group0302_Reg20                  ; // 20  (ADDR : 0x9C00_9750)
    UINT32  RGST_Group0302_Reg21                  ; // 21  (ADDR : 0x9C00_9754)
    UINT32  RGST_Group0302_Reg22                  ; // 22  (ADDR : 0x9C00_9758)
    UINT32  RGST_Group0302_Reg23                  ; // 23  (ADDR : 0x9C00_975C)
    UINT32  RGST_Group0302_Reg24                  ; // 24  (ADDR : 0x9C00_9760)
    UINT32  RGST_Group0302_Reg25                  ; // 25  (ADDR : 0x9C00_9764)
    UINT32  RGST_Group0302_Reg26                  ; // 26  (ADDR : 0x9C00_9768)
    UINT32  RGST_Group0302_Reg27                  ; // 27  (ADDR : 0x9C00_976C)
    UINT32  RGST_Group0302_Reg28                  ; // 28  (ADDR : 0x9C00_9770)
    UINT32  RGST_Group0302_Reg29                  ; // 29  (ADDR : 0x9C00_9774)
    UINT32  RGST_Group0302_Reg30                  ; // 30  (ADDR : 0x9C00_9778)
    UINT32  RGST_Group0302_Reg31                  ; // 31  (ADDR : 0x9C00_977C)

    //Group   303 : IMG1
    UINT32  RGST_Group0303_Reg00                  ; // 00  (ADDR : 0x9C00_9780)
    UINT32  RGST_Group0303_Reg01                  ; // 01  (ADDR : 0x9C00_9784)
    UINT32  RGST_Group0303_Reg02                  ; // 02  (ADDR : 0x9C00_9788)
    UINT32  RGST_Group0303_Reg03                  ; // 03  (ADDR : 0x9C00_978C)
    UINT32  RGST_Group0303_Reg04                  ; // 04  (ADDR : 0x9C00_9790)
    UINT32  RGST_Group0303_Reg05                  ; // 05  (ADDR : 0x9C00_9794)
    UINT32  RGST_Group0303_Reg06                  ; // 06  (ADDR : 0x9C00_9798)
    UINT32  RGST_Group0303_Reg07                  ; // 07  (ADDR : 0x9C00_979C)
    UINT32  RGST_Group0303_Reg08                  ; // 08  (ADDR : 0x9C00_97A0)
    UINT32  RGST_Group0303_Reg09                  ; // 09  (ADDR : 0x9C00_97A4)
    UINT32  RGST_Group0303_Reg10                  ; // 10  (ADDR : 0x9C00_97A8)
    UINT32  RGST_Group0303_Reg11                  ; // 11  (ADDR : 0x9C00_97AC)
    UINT32  RGST_Group0303_Reg12                  ; // 12  (ADDR : 0x9C00_97B0)
    UINT32  RGST_Group0303_Reg13                  ; // 13  (ADDR : 0x9C00_97B4)
    UINT32  RGST_Group0303_Reg14                  ; // 14  (ADDR : 0x9C00_97B8)
    UINT32  RGST_Group0303_Reg15                  ; // 15  (ADDR : 0x9C00_97BC)
    UINT32  RGST_Group0303_Reg16                  ; // 16  (ADDR : 0x9C00_97C0)
    UINT32  RGST_Group0303_Reg17                  ; // 17  (ADDR : 0x9C00_97C4)
    UINT32  RGST_Group0303_Reg18                  ; // 18  (ADDR : 0x9C00_97C8)
    UINT32  RGST_Group0303_Reg19                  ; // 19  (ADDR : 0x9C00_97CC)
    UINT32  RGST_Group0303_Reg20                  ; // 20  (ADDR : 0x9C00_97D0)
    UINT32  RGST_Group0303_Reg21                  ; // 21  (ADDR : 0x9C00_97D4)
    UINT32  RGST_Group0303_Reg22                  ; // 22  (ADDR : 0x9C00_97D8)
    UINT32  RGST_Group0303_Reg23                  ; // 23  (ADDR : 0x9C00_97DC)
    UINT32  RGST_Group0303_Reg24                  ; // 24  (ADDR : 0x9C00_97E0)
    UINT32  RGST_Group0303_Reg25                  ; // 25  (ADDR : 0x9C00_97E4)
    UINT32  RGST_Group0303_Reg26                  ; // 26  (ADDR : 0x9C00_97E8)
    UINT32  RGST_Group0303_Reg27                  ; // 27  (ADDR : 0x9C00_97EC)
    UINT32  RGST_Group0303_Reg28                  ; // 28  (ADDR : 0x9C00_97F0)
    UINT32  RGST_Group0303_Reg29                  ; // 29  (ADDR : 0x9C00_97F4)
    UINT32  RGST_Group0303_Reg30                  ; // 30  (ADDR : 0x9C00_97F8)
    UINT32  RGST_Group0303_Reg31                  ; // 31  (ADDR : 0x9C00_97FC)

    //Group   304 : IMG1
    UINT32  RGST_Group0304_Reg00                  ; // 00  (ADDR : 0x9C00_9800)
    UINT32  RGST_Group0304_Reg01                  ; // 01  (ADDR : 0x9C00_9804)
    UINT32  RGST_Group0304_Reg02                  ; // 02  (ADDR : 0x9C00_9808)
    UINT32  RGST_Group0304_Reg03                  ; // 03  (ADDR : 0x9C00_980C)
    UINT32  RGST_Group0304_Reg04                  ; // 04  (ADDR : 0x9C00_9810)
    UINT32  RGST_Group0304_Reg05                  ; // 05  (ADDR : 0x9C00_9814)
    UINT32  RGST_Group0304_Reg06                  ; // 06  (ADDR : 0x9C00_9818)
    UINT32  RGST_Group0304_Reg07                  ; // 07  (ADDR : 0x9C00_981C)
    UINT32  RGST_Group0304_Reg08                  ; // 08  (ADDR : 0x9C00_9820)
    UINT32  RGST_Group0304_Reg09                  ; // 09  (ADDR : 0x9C00_9824)
    UINT32  RGST_Group0304_Reg10                  ; // 10  (ADDR : 0x9C00_9828)
    UINT32  RGST_Group0304_Reg11                  ; // 11  (ADDR : 0x9C00_982C)
    UINT32  RGST_Group0304_Reg12                  ; // 12  (ADDR : 0x9C00_9830)
    UINT32  RGST_Group0304_Reg13                  ; // 13  (ADDR : 0x9C00_9834)
    UINT32  RGST_Group0304_Reg14                  ; // 14  (ADDR : 0x9C00_9838)
    UINT32  RGST_Group0304_Reg15                  ; // 15  (ADDR : 0x9C00_983C)
    UINT32  RGST_Group0304_Reg16                  ; // 16  (ADDR : 0x9C00_9840)
    UINT32  RGST_Group0304_Reg17                  ; // 17  (ADDR : 0x9C00_9844)
    UINT32  RGST_Group0304_Reg18                  ; // 18  (ADDR : 0x9C00_9848)
    UINT32  RGST_Group0304_Reg19                  ; // 19  (ADDR : 0x9C00_984C)
    UINT32  RGST_Group0304_Reg20                  ; // 20  (ADDR : 0x9C00_9850)
    UINT32  RGST_Group0304_Reg21                  ; // 21  (ADDR : 0x9C00_9854)
    UINT32  RGST_Group0304_Reg22                  ; // 22  (ADDR : 0x9C00_9858)
    UINT32  RGST_Group0304_Reg23                  ; // 23  (ADDR : 0x9C00_985C)
    UINT32  RGST_Group0304_Reg24                  ; // 24  (ADDR : 0x9C00_9860)
    UINT32  RGST_Group0304_Reg25                  ; // 25  (ADDR : 0x9C00_9864)
    UINT32  RGST_Group0304_Reg26                  ; // 26  (ADDR : 0x9C00_9868)
    UINT32  RGST_Group0304_Reg27                  ; // 27  (ADDR : 0x9C00_986C)
    UINT32  RGST_Group0304_Reg28                  ; // 28  (ADDR : 0x9C00_9870)
    UINT32  RGST_Group0304_Reg29                  ; // 29  (ADDR : 0x9C00_9874)
    UINT32  RGST_Group0304_Reg30                  ; // 30  (ADDR : 0x9C00_9878)
    UINT32  RGST_Group0304_Reg31                  ; // 31  (ADDR : 0x9C00_987C)

    //Group   305 : IMG0
    UINT32  RGST_Group0305_Reg00                  ; // 00  (ADDR : 0x9C00_9880)
    UINT32  RGST_Group0305_Reg01                  ; // 01  (ADDR : 0x9C00_9884)
    UINT32  RGST_Group0305_Reg02                  ; // 02  (ADDR : 0x9C00_9888)
    UINT32  RGST_Group0305_Reg03                  ; // 03  (ADDR : 0x9C00_988C)
    UINT32  RGST_Group0305_Reg04                  ; // 04  (ADDR : 0x9C00_9890)
    UINT32  RGST_Group0305_Reg05                  ; // 05  (ADDR : 0x9C00_9894)
    UINT32  RGST_Group0305_Reg06                  ; // 06  (ADDR : 0x9C00_9898)
    UINT32  RGST_Group0305_Reg07                  ; // 07  (ADDR : 0x9C00_989C)
    UINT32  RGST_Group0305_Reg08                  ; // 08  (ADDR : 0x9C00_98A0)
    UINT32  RGST_Group0305_Reg09                  ; // 09  (ADDR : 0x9C00_98A4)
    UINT32  RGST_Group0305_Reg10                  ; // 10  (ADDR : 0x9C00_98A8)
    UINT32  RGST_Group0305_Reg11                  ; // 11  (ADDR : 0x9C00_98AC)
    UINT32  RGST_Group0305_Reg12                  ; // 12  (ADDR : 0x9C00_98B0)
    UINT32  RGST_Group0305_Reg13                  ; // 13  (ADDR : 0x9C00_98B4)
    UINT32  RGST_Group0305_Reg14                  ; // 14  (ADDR : 0x9C00_98B8)
    UINT32  RGST_Group0305_Reg15                  ; // 15  (ADDR : 0x9C00_98BC)
    UINT32  RGST_Group0305_Reg16                  ; // 16  (ADDR : 0x9C00_98C0)
    UINT32  RGST_Group0305_Reg17                  ; // 17  (ADDR : 0x9C00_98C4)
    UINT32  RGST_Group0305_Reg18                  ; // 18  (ADDR : 0x9C00_98C8)
    UINT32  RGST_Group0305_Reg19                  ; // 19  (ADDR : 0x9C00_98CC)
    UINT32  RGST_Group0305_Reg20                  ; // 20  (ADDR : 0x9C00_98D0)
    UINT32  RGST_Group0305_Reg21                  ; // 21  (ADDR : 0x9C00_98D4)
    UINT32  RGST_Group0305_Reg22                  ; // 22  (ADDR : 0x9C00_98D8)
    UINT32  RGST_Group0305_Reg23                  ; // 23  (ADDR : 0x9C00_98DC)
    UINT32  RGST_Group0305_Reg24                  ; // 24  (ADDR : 0x9C00_98E0)
    UINT32  RGST_Group0305_Reg25                  ; // 25  (ADDR : 0x9C00_98E4)
    UINT32  RGST_Group0305_Reg26                  ; // 26  (ADDR : 0x9C00_98E8)
    UINT32  RGST_Group0305_Reg27                  ; // 27  (ADDR : 0x9C00_98EC)
    UINT32  RGST_Group0305_Reg28                  ; // 28  (ADDR : 0x9C00_98F0)
    UINT32  RGST_Group0305_Reg29                  ; // 29  (ADDR : 0x9C00_98F4)
    UINT32  RGST_Group0305_Reg30                  ; // 30  (ADDR : 0x9C00_98F8)
    UINT32  RGST_Group0305_Reg31                  ; // 31  (ADDR : 0x9C00_98FC)

    //Group   306 : IMG1
    UINT32  RGST_Group0306_Reg00                  ; // 00  (ADDR : 0x9C00_9900)
    UINT32  RGST_Group0306_Reg01                  ; // 01  (ADDR : 0x9C00_9904)
    UINT32  RGST_Group0306_Reg02                  ; // 02  (ADDR : 0x9C00_9908)
    UINT32  RGST_Group0306_Reg03                  ; // 03  (ADDR : 0x9C00_990C)
    UINT32  RGST_Group0306_Reg04                  ; // 04  (ADDR : 0x9C00_9910)
    UINT32  RGST_Group0306_Reg05                  ; // 05  (ADDR : 0x9C00_9914)
    UINT32  RGST_Group0306_Reg06                  ; // 06  (ADDR : 0x9C00_9918)
    UINT32  RGST_Group0306_Reg07                  ; // 07  (ADDR : 0x9C00_991C)
    UINT32  RGST_Group0306_Reg08                  ; // 08  (ADDR : 0x9C00_9920)
    UINT32  RGST_Group0306_Reg09                  ; // 09  (ADDR : 0x9C00_9924)
    UINT32  RGST_Group0306_Reg10                  ; // 10  (ADDR : 0x9C00_9928)
    UINT32  RGST_Group0306_Reg11                  ; // 11  (ADDR : 0x9C00_992C)
    UINT32  RGST_Group0306_Reg12                  ; // 12  (ADDR : 0x9C00_9930)
    UINT32  RGST_Group0306_Reg13                  ; // 13  (ADDR : 0x9C00_9934)
    UINT32  RGST_Group0306_Reg14                  ; // 14  (ADDR : 0x9C00_9938)
    UINT32  RGST_Group0306_Reg15                  ; // 15  (ADDR : 0x9C00_993C)
    UINT32  RGST_Group0306_Reg16                  ; // 16  (ADDR : 0x9C00_9940)
    UINT32  RGST_Group0306_Reg17                  ; // 17  (ADDR : 0x9C00_9944)
    UINT32  RGST_Group0306_Reg18                  ; // 18  (ADDR : 0x9C00_9948)
    UINT32  RGST_Group0306_Reg19                  ; // 19  (ADDR : 0x9C00_994C)
    UINT32  RGST_Group0306_Reg20                  ; // 20  (ADDR : 0x9C00_9950)
    UINT32  RGST_Group0306_Reg21                  ; // 21  (ADDR : 0x9C00_9954)
    UINT32  RGST_Group0306_Reg22                  ; // 22  (ADDR : 0x9C00_9958)
    UINT32  RGST_Group0306_Reg23                  ; // 23  (ADDR : 0x9C00_995C)
    UINT32  RGST_Group0306_Reg24                  ; // 24  (ADDR : 0x9C00_9960)
    UINT32  RGST_Group0306_Reg25                  ; // 25  (ADDR : 0x9C00_9964)
    UINT32  RGST_Group0306_Reg26                  ; // 26  (ADDR : 0x9C00_9968)
    UINT32  RGST_Group0306_Reg27                  ; // 27  (ADDR : 0x9C00_996C)
    UINT32  RGST_Group0306_Reg28                  ; // 28  (ADDR : 0x9C00_9970)
    UINT32  RGST_Group0306_Reg29                  ; // 29  (ADDR : 0x9C00_9974)
    UINT32  RGST_Group0306_Reg30                  ; // 30  (ADDR : 0x9C00_9978)
    UINT32  RGST_Group0306_Reg31                  ; // 31  (ADDR : 0x9C00_997C)

    //Group   307 : V656IN0
    UINT32  tpiiw0_latch_en	                  ; // 00  (ADDR : 0x9C00_9980)
    UINT32  tpiiw0_mode_option	                  ; // 01  (ADDR : 0x9C00_9984)
    UINT32  tpiiw0_en		                  ; // 02  (ADDR : 0x9C00_9988)
    UINT32  tpiiw0_ug_thd	                  ; // 03  (ADDR : 0x9C00_998C)
    UINT32  tpiiw0_cmdq_thd	                  ; // 04  (ADDR : 0x9C00_9990)
    UINT32  tpiiw0_in_status  	       	          ; // 05  (ADDR : 0x9C00_9994)
    UINT32  RGST_Group0307_Reg06                  ; // 06  (ADDR : 0x9C00_9998)
    UINT32  RGST_Group0307_Reg07                  ; // 07  (ADDR : 0x9C00_999C)
    UINT32  RGST_Group0307_Reg08                  ; // 08  (ADDR : 0x9C00_99A0)
    UINT32  RGST_Group0307_Reg09                  ; // 09  (ADDR : 0x9C00_99A4)
    UINT32  tpiiw0_luma_iw_base_addr0             ; // 10  (ADDR : 0x9C00_99A8)
    UINT32  tpiiw0_luma_iw_base_addr1             ; // 11  (ADDR : 0x9C00_99AC)
    UINT32  tpiiw0_crma_iw_base_addr0             ; // 12  (ADDR : 0x9C00_99B0)
    UINT32  tpiiw0_crma_iw_base_addr1             ; // 13  (ADDR : 0x9C00_99B4)
    UINT32  tpiiw0_vdo_frame_size                 ; // 14  (ADDR : 0x9C00_99B8)
    UINT32  tpiiw0_vdo_xy_size	                  ; // 15  (ADDR : 0x9C00_99BC)
    UINT32  tpiiw0_debug_mode	                  ; // 16  (ADDR : 0x9C00_99C0)
    UINT32  tpiiw0_chksum_info                    ; // 17  (ADDR : 0x9C00_99C4)
    UINT32  tpiiw0_irq_field_start_line_cnt       ; // 18  (ADDR : 0x9C00_99C8)
    UINT32  tpiiw0_irq_mask                       ; // 19  (ADDR : 0x9C00_99CC)
    UINT32  tpiiw0_irq_ini	                  ; // 20  (ADDR : 0x9C00_99D0)
    UINT32  tpiiw0_irq_time_out_threshold         ; // 21  (ADDR : 0x9C00_99D4)
    UINT32  tpiiw0_irq_line_start                 ; // 22  (ADDR : 0x9C00_99D8)
    UINT32  tpiiw0_irq_pix_start	          ; // 23  (ADDR : 0x9C00_99DC)
    UINT32  tpiiw0_irq_other_control              ; // 24  (ADDR : 0x9C00_99E0)
    UINT32  RGST_Group0307_Reg25                  ; // 25  (ADDR : 0x9C00_99E4)
    UINT32  RGST_Group0307_Reg26                  ; // 26  (ADDR : 0x9C00_99E8)
    UINT32  tpiiw0_error_flag_info                ; // 27  (ADDR : 0x9C00_99EC)
    UINT32  RGST_Group0307_Reg28                  ; // 28  (ADDR : 0x9C00_99F0)
    UINT32  RGST_Group0307_Reg29                  ; // 29  (ADDR : 0x9C00_99F4)
    UINT32  RGST_Group0307_Reg30                  ; // 30  (ADDR : 0x9C00_99F8)
    UINT32  RGST_Group0307_Reg31                  ; // 31  (ADDR : 0x9C00_99FC)

    //Group   308 : V656IN1
    UINT32  tpiiw1_latch_en	           	  ; // 00  (ADDR : 0x9C00_9A00)
    UINT32  tpiiw1_mode_option	           	  ; // 01  (ADDR : 0x9C00_9A04)
    UINT32  tpiiw1_en		           	  ; // 02  (ADDR : 0x9C00_9A08)
    UINT32  tpiiw1_ug_thd	           	  ; // 03  (ADDR : 0x9C00_9A0C)
    UINT32  tpiiw1_cmdq_thd	           	  ; // 04  (ADDR : 0x9C00_9A10)
    UINT32  tpiiw1_in_status  	       	   	  ; // 05  (ADDR : 0x9C00_9A14)
    UINT32  RGST_Group0308_Reg06           	  ; // 06  (ADDR : 0x9C00_9A18)
    UINT32  RGST_Group0308_Reg07           	  ; // 07  (ADDR : 0x9C00_9A1C)
    UINT32  RGST_Group0308_Reg08           	  ; // 08  (ADDR : 0x9C00_9A20)
    UINT32  RGST_Group0308_Reg09           	  ; // 09  (ADDR : 0x9C00_9A24)
    UINT32  tpiiw1_luma_iw_base_addr0      	  ; // 10  (ADDR : 0x9C00_9A28)
    UINT32  tpiiw1_luma_iw_base_addr1      	  ; // 11  (ADDR : 0x9C00_9A2C)
    UINT32  tpiiw1_crma_iw_base_addr0      	  ; // 12  (ADDR : 0x9C00_9A30)
    UINT32  tpiiw1_crma_iw_base_addr1      	  ; // 13  (ADDR : 0x9C00_9A34)
    UINT32  tpiiw1_vdo_frame_size          	  ; // 14  (ADDR : 0x9C00_9A38)
    UINT32  tpiiw1_vdo_xy_size	           	  ; // 15  (ADDR : 0x9C00_9A3C)
    UINT32  tpiiw1_debug_mode	           	  ; // 16  (ADDR : 0x9C00_9A40)
    UINT32  tpiiw1_chksum_info             	  ; // 17  (ADDR : 0x9C00_9A44)
    UINT32  tpiiw1_irq_field_start_line_cnt	  ; // 18  (ADDR : 0x9C00_9A48)
    UINT32  tpiiw1_irq_mask                	  ; // 19  (ADDR : 0x9C00_9A4C)
    UINT32  tpiiw1_irq_ini	           	  ; // 20  (ADDR : 0x9C00_9A50)
    UINT32  tpiiw1_irq_time_out_threshold  	  ; // 21  (ADDR : 0x9C00_9A54)
    UINT32  tpiiw1_irq_line_start          	  ; // 22  (ADDR : 0x9C00_9A58)
    UINT32  tpiiw1_irq_pix_start	   	  ; // 23  (ADDR : 0x9C00_9A5C)
    UINT32  tpiiw1_irq_other_control       	  ; // 24  (ADDR : 0x9C00_9A60)
    UINT32  RGST_Group0308_Reg25           	  ; // 25  (ADDR : 0x9C00_9A64)
    UINT32  RGST_Group0308_Reg26           	  ; // 26  (ADDR : 0x9C00_9A68)
    UINT32  tpiiw1_error_flag_info         	  ; // 27  (ADDR : 0x9C00_9A6C)
    UINT32  RGST_Group0308_Reg28           	  ; // 28  (ADDR : 0x9C00_9A70)
    UINT32  RGST_Group0308_Reg29           	  ; // 29  (ADDR : 0x9C00_9A74)
    UINT32  RGST_Group0308_Reg30           	  ; // 30  (ADDR : 0x9C00_9A78)
    UINT32  RGST_Group0308_Reg31           	  ; // 31  (ADDR : 0x9C00_9A7C)

    //Group   309 : Reserved
    UINT32  RGST_Group0309_Reserved[32]           ; //     (ADDR : 0x9C00_9A80) ~ (ADDR : 0x9C00_9AFC)

    //Group   310 : TVDEC
    UINT32  RGST_Group0310_Reg00                  ; // 00  (ADDR : 0x9C00_9B00)
    UINT32  RGST_Group0310_Reg01                  ; // 01  (ADDR : 0x9C00_9B04)
    UINT32  RGST_Group0310_Reg02                  ; // 02  (ADDR : 0x9C00_9B08)
    UINT32  RGST_Group0310_Reg03                  ; // 03  (ADDR : 0x9C00_9B0C)
    UINT32  RGST_Group0310_Reg04                  ; // 04  (ADDR : 0x9C00_9B10)
    UINT32  RGST_Group0310_Reg05                  ; // 05  (ADDR : 0x9C00_9B14)
    UINT32  RGST_Group0310_Reg06                  ; // 06  (ADDR : 0x9C00_9B18)
    UINT32  RGST_Group0310_Reg07                  ; // 07  (ADDR : 0x9C00_9B1C)
    UINT32  RGST_Group0310_Reg08                  ; // 08  (ADDR : 0x9C00_9B20)
    UINT32  RGST_Group0310_Reg09                  ; // 09  (ADDR : 0x9C00_9B24)
    UINT32  RGST_Group0310_Reg10                  ; // 10  (ADDR : 0x9C00_9B28)
    UINT32  RGST_Group0310_Reg11                  ; // 11  (ADDR : 0x9C00_9B2C)
    UINT32  RGST_Group0310_Reg12                  ; // 12  (ADDR : 0x9C00_9B30)
    UINT32  RGST_Group0310_Reg13                  ; // 13  (ADDR : 0x9C00_9B34)
    UINT32  RGST_Group0310_Reg14                  ; // 14  (ADDR : 0x9C00_9B38)
    UINT32  RGST_Group0310_Reg15                  ; // 15  (ADDR : 0x9C00_9B3C)
    UINT32  RGST_Group0310_Reg16                  ; // 16  (ADDR : 0x9C00_9B40)
    UINT32  RGST_Group0310_Reg17                  ; // 17  (ADDR : 0x9C00_9B44)
    UINT32  RGST_Group0310_Reg18                  ; // 18  (ADDR : 0x9C00_9B48)
    UINT32  RGST_Group0310_Reg19                  ; // 19  (ADDR : 0x9C00_9B4C)
    UINT32  RGST_Group0310_Reg20                  ; // 20  (ADDR : 0x9C00_9B50)
    UINT32  RGST_Group0310_Reg21                  ; // 21  (ADDR : 0x9C00_9B54)
    UINT32  RGST_Group0310_Reg22                  ; // 22  (ADDR : 0x9C00_9B58)
    UINT32  RGST_Group0310_Reg23                  ; // 23  (ADDR : 0x9C00_9B5C)
    UINT32  RGST_Group0310_Reg24                  ; // 24  (ADDR : 0x9C00_9B60)
    UINT32  RGST_Group0310_Reg25                  ; // 25  (ADDR : 0x9C00_9B64)
    UINT32  RGST_Group0310_Reg26                  ; // 26  (ADDR : 0x9C00_9B68)
    UINT32  RGST_Group0310_Reg27                  ; // 27  (ADDR : 0x9C00_9B6C)
    UINT32  RGST_Group0310_Reg28                  ; // 28  (ADDR : 0x9C00_9B70)
    UINT32  RGST_Group0310_Reg29                  ; // 29  (ADDR : 0x9C00_9B74)
    UINT32  RGST_Group0310_Reg30                  ; // 30  (ADDR : 0x9C00_9B78)
    UINT32  RGST_Group0310_Reg31                  ; // 31  (ADDR : 0x9C00_9B7C)

    //Group   311 : TVDEC
    UINT32  RGST_Group0311_Reg00                  ; // 00  (ADDR : 0x9C00_9B80)
    UINT32  RGST_Group0311_Reg01                  ; // 01  (ADDR : 0x9C00_9B84)
    UINT32  RGST_Group0311_Reg02                  ; // 02  (ADDR : 0x9C00_9B88)
    UINT32  RGST_Group0311_Reg03                  ; // 03  (ADDR : 0x9C00_9B8C)
    UINT32  RGST_Group0311_Reg04                  ; // 04  (ADDR : 0x9C00_9B90)
    UINT32  RGST_Group0311_Reg05                  ; // 05  (ADDR : 0x9C00_9B94)
    UINT32  RGST_Group0311_Reg06                  ; // 06  (ADDR : 0x9C00_9B98)
    UINT32  RGST_Group0311_Reg07                  ; // 07  (ADDR : 0x9C00_9B9C)
    UINT32  RGST_Group0311_Reg08                  ; // 08  (ADDR : 0x9C00_9BA0)
    UINT32  RGST_Group0311_Reg09                  ; // 09  (ADDR : 0x9C00_9BA4)
    UINT32  RGST_Group0311_Reg10                  ; // 10  (ADDR : 0x9C00_9BA8)
    UINT32  RGST_Group0311_Reg11                  ; // 11  (ADDR : 0x9C00_9BAC)
    UINT32  RGST_Group0311_Reg12                  ; // 12  (ADDR : 0x9C00_9BB0)
    UINT32  RGST_Group0311_Reg13                  ; // 13  (ADDR : 0x9C00_9BB4)
    UINT32  RGST_Group0311_Reg14                  ; // 14  (ADDR : 0x9C00_9BB8)
    UINT32  RGST_Group0311_Reg15                  ; // 15  (ADDR : 0x9C00_9BBC)
    UINT32  RGST_Group0311_Reg16                  ; // 16  (ADDR : 0x9C00_9BC0)
    UINT32  RGST_Group0311_Reg17                  ; // 17  (ADDR : 0x9C00_9BC4)
    UINT32  RGST_Group0311_Reg18                  ; // 18  (ADDR : 0x9C00_9BC8)
    UINT32  RGST_Group0311_Reg19                  ; // 19  (ADDR : 0x9C00_9BCC)
    UINT32  RGST_Group0311_Reg20                  ; // 20  (ADDR : 0x9C00_9BD0)
    UINT32  RGST_Group0311_Reg21                  ; // 21  (ADDR : 0x9C00_9BD4)
    UINT32  RGST_Group0311_Reg22                  ; // 22  (ADDR : 0x9C00_9BD8)
    UINT32  RGST_Group0311_Reg23                  ; // 23  (ADDR : 0x9C00_9BDC)
    UINT32  RGST_Group0311_Reg24                  ; // 24  (ADDR : 0x9C00_9BE0)
    UINT32  RGST_Group0311_Reg25                  ; // 25  (ADDR : 0x9C00_9BE4)
    UINT32  RGST_Group0311_Reg26                  ; // 26  (ADDR : 0x9C00_9BE8)
    UINT32  RGST_Group0311_Reg27                  ; // 27  (ADDR : 0x9C00_9BEC)
    UINT32  RGST_Group0311_Reg28                  ; // 28  (ADDR : 0x9C00_9BF0)
    UINT32  RGST_Group0311_Reg29                  ; // 29  (ADDR : 0x9C00_9BF4)
    UINT32  RGST_Group0311_Reg30                  ; // 30  (ADDR : 0x9C00_9BF8)
    UINT32  RGST_Group0311_Reg31                  ; // 31  (ADDR : 0x9C00_9BFC)

    //Group   312 : TVDEC
    UINT32  RGST_Group0312_Reg00                  ; // 00  (ADDR : 0x9C00_9C00)
    UINT32  RGST_Group0312_Reg01                  ; // 01  (ADDR : 0x9C00_9C04)
    UINT32  RGST_Group0312_Reg02                  ; // 02  (ADDR : 0x9C00_9C08)
    UINT32  RGST_Group0312_Reg03                  ; // 03  (ADDR : 0x9C00_9C0C)
    UINT32  RGST_Group0312_Reg04                  ; // 04  (ADDR : 0x9C00_9C10)
    UINT32  RGST_Group0312_Reg05                  ; // 05  (ADDR : 0x9C00_9C14)
    UINT32  RGST_Group0312_Reg06                  ; // 06  (ADDR : 0x9C00_9C18)
    UINT32  RGST_Group0312_Reg07                  ; // 07  (ADDR : 0x9C00_9C1C)
    UINT32  RGST_Group0312_Reg08                  ; // 08  (ADDR : 0x9C00_9C20)
    UINT32  RGST_Group0312_Reg09                  ; // 09  (ADDR : 0x9C00_9C24)
    UINT32  RGST_Group0312_Reg10                  ; // 10  (ADDR : 0x9C00_9C28)
    UINT32  RGST_Group0312_Reg11                  ; // 11  (ADDR : 0x9C00_9C2C)
    UINT32  RGST_Group0312_Reg12                  ; // 12  (ADDR : 0x9C00_9C30)
    UINT32  RGST_Group0312_Reg13                  ; // 13  (ADDR : 0x9C00_9C34)
    UINT32  RGST_Group0312_Reg14                  ; // 14  (ADDR : 0x9C00_9C38)
    UINT32  RGST_Group0312_Reg15                  ; // 15  (ADDR : 0x9C00_9C3C)
    UINT32  RGST_Group0312_Reg16                  ; // 16  (ADDR : 0x9C00_9C40)
    UINT32  RGST_Group0312_Reg17                  ; // 17  (ADDR : 0x9C00_9C44)
    UINT32  RGST_Group0312_Reg18                  ; // 18  (ADDR : 0x9C00_9C48)
    UINT32  RGST_Group0312_Reg19                  ; // 19  (ADDR : 0x9C00_9C4C)
    UINT32  RGST_Group0312_Reg20                  ; // 20  (ADDR : 0x9C00_9C50)
    UINT32  RGST_Group0312_Reg21                  ; // 21  (ADDR : 0x9C00_9C54)
    UINT32  RGST_Group0312_Reg22                  ; // 22  (ADDR : 0x9C00_9C58)
    UINT32  RGST_Group0312_Reg23                  ; // 23  (ADDR : 0x9C00_9C5C)
    UINT32  RGST_Group0312_Reg24                  ; // 24  (ADDR : 0x9C00_9C60)
    UINT32  RGST_Group0312_Reg25                  ; // 25  (ADDR : 0x9C00_9C64)
    UINT32  RGST_Group0312_Reg26                  ; // 26  (ADDR : 0x9C00_9C68)
    UINT32  RGST_Group0312_Reg27                  ; // 27  (ADDR : 0x9C00_9C6C)
    UINT32  RGST_Group0312_Reg28                  ; // 28  (ADDR : 0x9C00_9C70)
    UINT32  RGST_Group0312_Reg29                  ; // 29  (ADDR : 0x9C00_9C74)
    UINT32  RGST_Group0312_Reg30                  ; // 30  (ADDR : 0x9C00_9C78)
    UINT32  RGST_Group0312_Reg31                  ; // 31  (ADDR : 0x9C00_9C7C)

    //Group   313 : TVDEC
    UINT32  RGST_Group0313_Reg00                  ; // 00  (ADDR : 0x9C00_9C80)
    UINT32  RGST_Group0313_Reg01                  ; // 01  (ADDR : 0x9C00_9C84)
    UINT32  RGST_Group0313_Reg02                  ; // 02  (ADDR : 0x9C00_9C88)
    UINT32  RGST_Group0313_Reg03                  ; // 03  (ADDR : 0x9C00_9C8C)
    UINT32  RGST_Group0313_Reg04                  ; // 04  (ADDR : 0x9C00_9C90)
    UINT32  RGST_Group0313_Reg05                  ; // 05  (ADDR : 0x9C00_9C94)
    UINT32  RGST_Group0313_Reg06                  ; // 06  (ADDR : 0x9C00_9C98)
    UINT32  RGST_Group0313_Reg07                  ; // 07  (ADDR : 0x9C00_9C9C)
    UINT32  RGST_Group0313_Reg08                  ; // 08  (ADDR : 0x9C00_9CA0)
    UINT32  RGST_Group0313_Reg09                  ; // 09  (ADDR : 0x9C00_9CA4)
    UINT32  RGST_Group0313_Reg10                  ; // 10  (ADDR : 0x9C00_9CA8)
    UINT32  RGST_Group0313_Reg11                  ; // 11  (ADDR : 0x9C00_9CAC)
    UINT32  RGST_Group0313_Reg12                  ; // 12  (ADDR : 0x9C00_9CB0)
    UINT32  RGST_Group0313_Reg13                  ; // 13  (ADDR : 0x9C00_9CB4)
    UINT32  RGST_Group0313_Reg14                  ; // 14  (ADDR : 0x9C00_9CB8)
    UINT32  RGST_Group0313_Reg15                  ; // 15  (ADDR : 0x9C00_9CBC)
    UINT32  RGST_Group0313_Reg16                  ; // 16  (ADDR : 0x9C00_9CC0)
    UINT32  RGST_Group0313_Reg17                  ; // 17  (ADDR : 0x9C00_9CC4)
    UINT32  RGST_Group0313_Reg18                  ; // 18  (ADDR : 0x9C00_9CC8)
    UINT32  RGST_Group0313_Reg19                  ; // 19  (ADDR : 0x9C00_9CCC)
    UINT32  RGST_Group0313_Reg20                  ; // 20  (ADDR : 0x9C00_9CD0)
    UINT32  RGST_Group0313_Reg21                  ; // 21  (ADDR : 0x9C00_9CD4)
    UINT32  RGST_Group0313_Reg22                  ; // 22  (ADDR : 0x9C00_9CD8)
    UINT32  RGST_Group0313_Reg23                  ; // 23  (ADDR : 0x9C00_9CDC)
    UINT32  RGST_Group0313_Reg24                  ; // 24  (ADDR : 0x9C00_9CE0)
    UINT32  RGST_Group0313_Reg25                  ; // 25  (ADDR : 0x9C00_9CE4)
    UINT32  RGST_Group0313_Reg26                  ; // 26  (ADDR : 0x9C00_9CE8)
    UINT32  RGST_Group0313_Reg27                  ; // 27  (ADDR : 0x9C00_9CEC)
    UINT32  RGST_Group0313_Reg28                  ; // 28  (ADDR : 0x9C00_9CF0)
    UINT32  RGST_Group0313_Reg29                  ; // 29  (ADDR : 0x9C00_9CF4)
    UINT32  RGST_Group0313_Reg30                  ; // 30  (ADDR : 0x9C00_9CF8)
    UINT32  RGST_Group0313_Reg31                  ; // 31  (ADDR : 0x9C00_9CFC)

    //Group   314 : TVDEC
    UINT32  RGST_Group0314_Reg00                  ; // 00  (ADDR : 0x9C00_9D00)
    UINT32  RGST_Group0314_Reg01                  ; // 01  (ADDR : 0x9C00_9D04)
    UINT32  RGST_Group0314_Reg02                  ; // 02  (ADDR : 0x9C00_9D08)
    UINT32  RGST_Group0314_Reg03                  ; // 03  (ADDR : 0x9C00_9D0C)
    UINT32  RGST_Group0314_Reg04                  ; // 04  (ADDR : 0x9C00_9D10)
    UINT32  RGST_Group0314_Reg05                  ; // 05  (ADDR : 0x9C00_9D14)
    UINT32  RGST_Group0314_Reg06                  ; // 06  (ADDR : 0x9C00_9D18)
    UINT32  RGST_Group0314_Reg07                  ; // 07  (ADDR : 0x9C00_9D1C)
    UINT32  RGST_Group0314_Reg08                  ; // 08  (ADDR : 0x9C00_9D20)
    UINT32  RGST_Group0314_Reg09                  ; // 09  (ADDR : 0x9C00_9D24)
    UINT32  RGST_Group0314_Reg10                  ; // 10  (ADDR : 0x9C00_9D28)
    UINT32  RGST_Group0314_Reg11                  ; // 11  (ADDR : 0x9C00_9D2C)
    UINT32  RGST_Group0314_Reg12                  ; // 12  (ADDR : 0x9C00_9D30)
    UINT32  RGST_Group0314_Reg13                  ; // 13  (ADDR : 0x9C00_9D34)
    UINT32  RGST_Group0314_Reg14                  ; // 14  (ADDR : 0x9C00_9D38)
    UINT32  RGST_Group0314_Reg15                  ; // 15  (ADDR : 0x9C00_9D3C)
    UINT32  RGST_Group0314_Reg16                  ; // 16  (ADDR : 0x9C00_9D40)
    UINT32  RGST_Group0314_Reg17                  ; // 17  (ADDR : 0x9C00_9D44)
    UINT32  RGST_Group0314_Reg18                  ; // 18  (ADDR : 0x9C00_9D48)
    UINT32  RGST_Group0314_Reg19                  ; // 19  (ADDR : 0x9C00_9D4C)
    UINT32  RGST_Group0314_Reg20                  ; // 20  (ADDR : 0x9C00_9D50)
    UINT32  RGST_Group0314_Reg21                  ; // 21  (ADDR : 0x9C00_9D54)
    UINT32  RGST_Group0314_Reg22                  ; // 22  (ADDR : 0x9C00_9D58)
    UINT32  RGST_Group0314_Reg23                  ; // 23  (ADDR : 0x9C00_9D5C)
    UINT32  RGST_Group0314_Reg24                  ; // 24  (ADDR : 0x9C00_9D60)
    UINT32  RGST_Group0314_Reg25                  ; // 25  (ADDR : 0x9C00_9D64)
    UINT32  RGST_Group0314_Reg26                  ; // 26  (ADDR : 0x9C00_9D68)
    UINT32  RGST_Group0314_Reg27                  ; // 27  (ADDR : 0x9C00_9D6C)
    UINT32  RGST_Group0314_Reg28                  ; // 28  (ADDR : 0x9C00_9D70)
    UINT32  RGST_Group0314_Reg29                  ; // 29  (ADDR : 0x9C00_9D74)
    UINT32  RGST_Group0314_Reg30                  ; // 30  (ADDR : 0x9C00_9D78)
    UINT32  RGST_Group0314_Reg31                  ; // 31  (ADDR : 0x9C00_9D7C)

    //Group   315 : TVDEC
    UINT32  RGST_Group0315_Reg00                  ; // 00  (ADDR : 0x9C00_9D80)
    UINT32  RGST_Group0315_Reg01                  ; // 01  (ADDR : 0x9C00_9D84)
    UINT32  RGST_Group0315_Reg02                  ; // 02  (ADDR : 0x9C00_9D88)
    UINT32  RGST_Group0315_Reg03                  ; // 03  (ADDR : 0x9C00_9D8C)
    UINT32  RGST_Group0315_Reg04                  ; // 04  (ADDR : 0x9C00_9D90)
    UINT32  RGST_Group0315_Reg05                  ; // 05  (ADDR : 0x9C00_9D94)
    UINT32  RGST_Group0315_Reg06                  ; // 06  (ADDR : 0x9C00_9D98)
    UINT32  RGST_Group0315_Reg07                  ; // 07  (ADDR : 0x9C00_9D9C)
    UINT32  RGST_Group0315_Reg08                  ; // 08  (ADDR : 0x9C00_9DA0)
    UINT32  RGST_Group0315_Reg09                  ; // 09  (ADDR : 0x9C00_9DA4)
    UINT32  RGST_Group0315_Reg10                  ; // 10  (ADDR : 0x9C00_9DA8)
    UINT32  RGST_Group0315_Reg11                  ; // 11  (ADDR : 0x9C00_9DAC)
    UINT32  RGST_Group0315_Reg12                  ; // 12  (ADDR : 0x9C00_9DB0)
    UINT32  RGST_Group0315_Reg13                  ; // 13  (ADDR : 0x9C00_9DB4)
    UINT32  RGST_Group0315_Reg14                  ; // 14  (ADDR : 0x9C00_9DB8)
    UINT32  RGST_Group0315_Reg15                  ; // 15  (ADDR : 0x9C00_9DBC)
    UINT32  RGST_Group0315_Reg16                  ; // 16  (ADDR : 0x9C00_9DC0)
    UINT32  RGST_Group0315_Reg17                  ; // 17  (ADDR : 0x9C00_9DC4)
    UINT32  RGST_Group0315_Reg18                  ; // 18  (ADDR : 0x9C00_9DC8)
    UINT32  RGST_Group0315_Reg19                  ; // 19  (ADDR : 0x9C00_9DCC)
    UINT32  RGST_Group0315_Reg20                  ; // 20  (ADDR : 0x9C00_9DD0)
    UINT32  RGST_Group0315_Reg21                  ; // 21  (ADDR : 0x9C00_9DD4)
    UINT32  RGST_Group0315_Reg22                  ; // 22  (ADDR : 0x9C00_9DD8)
    UINT32  RGST_Group0315_Reg23                  ; // 23  (ADDR : 0x9C00_9DDC)
    UINT32  RGST_Group0315_Reg24                  ; // 24  (ADDR : 0x9C00_9DE0)
    UINT32  RGST_Group0315_Reg25                  ; // 25  (ADDR : 0x9C00_9DE4)
    UINT32  RGST_Group0315_Reg26                  ; // 26  (ADDR : 0x9C00_9DE8)
    UINT32  RGST_Group0315_Reg27                  ; // 27  (ADDR : 0x9C00_9DEC)
    UINT32  RGST_Group0315_Reg28                  ; // 28  (ADDR : 0x9C00_9DF0)
    UINT32  RGST_Group0315_Reg29                  ; // 29  (ADDR : 0x9C00_9DF4)
    UINT32  RGST_Group0315_Reg30                  ; // 30  (ADDR : 0x9C00_9DF8)
    UINT32  RGST_Group0315_Reg31                  ; // 31  (ADDR : 0x9C00_9DFC)

    //Group   316 : TVDEC
    UINT32  RGST_Group0316_Reg00                  ; // 00  (ADDR : 0x9C00_9E00)
    UINT32  RGST_Group0316_Reg01                  ; // 01  (ADDR : 0x9C00_9E04)
    UINT32  RGST_Group0316_Reg02                  ; // 02  (ADDR : 0x9C00_9E08)
    UINT32  RGST_Group0316_Reg03                  ; // 03  (ADDR : 0x9C00_9E0C)
    UINT32  RGST_Group0316_Reg04                  ; // 04  (ADDR : 0x9C00_9E10)
    UINT32  RGST_Group0316_Reg05                  ; // 05  (ADDR : 0x9C00_9E14)
    UINT32  RGST_Group0316_Reg06                  ; // 06  (ADDR : 0x9C00_9E18)
    UINT32  RGST_Group0316_Reg07                  ; // 07  (ADDR : 0x9C00_9E1C)
    UINT32  RGST_Group0316_Reg08                  ; // 08  (ADDR : 0x9C00_9E20)
    UINT32  RGST_Group0316_Reg09                  ; // 09  (ADDR : 0x9C00_9E24)
    UINT32  RGST_Group0316_Reg10                  ; // 10  (ADDR : 0x9C00_9E28)
    UINT32  RGST_Group0316_Reg11                  ; // 11  (ADDR : 0x9C00_9E2C)
    UINT32  RGST_Group0316_Reg12                  ; // 12  (ADDR : 0x9C00_9E30)
    UINT32  RGST_Group0316_Reg13                  ; // 13  (ADDR : 0x9C00_9E34)
    UINT32  RGST_Group0316_Reg14                  ; // 14  (ADDR : 0x9C00_9E38)
    UINT32  RGST_Group0316_Reg15                  ; // 15  (ADDR : 0x9C00_9E3C)
    UINT32  RGST_Group0316_Reg16                  ; // 16  (ADDR : 0x9C00_9E40)
    UINT32  RGST_Group0316_Reg17                  ; // 17  (ADDR : 0x9C00_9E44)
    UINT32  RGST_Group0316_Reg18                  ; // 18  (ADDR : 0x9C00_9E48)
    UINT32  RGST_Group0316_Reg19                  ; // 19  (ADDR : 0x9C00_9E4C)
    UINT32  RGST_Group0316_Reg20                  ; // 20  (ADDR : 0x9C00_9E50)
    UINT32  RGST_Group0316_Reg21                  ; // 21  (ADDR : 0x9C00_9E54)
    UINT32  RGST_Group0316_Reg22                  ; // 22  (ADDR : 0x9C00_9E58)
    UINT32  RGST_Group0316_Reg23                  ; // 23  (ADDR : 0x9C00_9E5C)
    UINT32  RGST_Group0316_Reg24                  ; // 24  (ADDR : 0x9C00_9E60)
    UINT32  RGST_Group0316_Reg25                  ; // 25  (ADDR : 0x9C00_9E64)
    UINT32  RGST_Group0316_Reg26                  ; // 26  (ADDR : 0x9C00_9E68)
    UINT32  RGST_Group0316_Reg27                  ; // 27  (ADDR : 0x9C00_9E6C)
    UINT32  RGST_Group0316_Reg28                  ; // 28  (ADDR : 0x9C00_9E70)
    UINT32  RGST_Group0316_Reg29                  ; // 29  (ADDR : 0x9C00_9E74)
    UINT32  RGST_Group0316_Reg30                  ; // 30  (ADDR : 0x9C00_9E78)
    UINT32  RGST_Group0316_Reg31                  ; // 31  (ADDR : 0x9C00_9E7C)

    //Group   317 : TVDEC
    UINT32  RGST_Group0317_Reg00                  ; // 00  (ADDR : 0x9C00_9E80)
    UINT32  RGST_Group0317_Reg01                  ; // 01  (ADDR : 0x9C00_9E84)
    UINT32  RGST_Group0317_Reg02                  ; // 02  (ADDR : 0x9C00_9E88)
    UINT32  RGST_Group0317_Reg03                  ; // 03  (ADDR : 0x9C00_9E8C)
    UINT32  RGST_Group0317_Reg04                  ; // 04  (ADDR : 0x9C00_9E90)
    UINT32  RGST_Group0317_Reg05                  ; // 05  (ADDR : 0x9C00_9E94)
    UINT32  RGST_Group0317_Reg06                  ; // 06  (ADDR : 0x9C00_9E98)
    UINT32  RGST_Group0317_Reg07                  ; // 07  (ADDR : 0x9C00_9E9C)
    UINT32  RGST_Group0317_Reg08                  ; // 08  (ADDR : 0x9C00_9EA0)
    UINT32  RGST_Group0317_Reg09                  ; // 09  (ADDR : 0x9C00_9EA4)
    UINT32  RGST_Group0317_Reg10                  ; // 10  (ADDR : 0x9C00_9EA8)
    UINT32  RGST_Group0317_Reg11                  ; // 11  (ADDR : 0x9C00_9EAC)
    UINT32  RGST_Group0317_Reg12                  ; // 12  (ADDR : 0x9C00_9EB0)
    UINT32  RGST_Group0317_Reg13                  ; // 13  (ADDR : 0x9C00_9EB4)
    UINT32  RGST_Group0317_Reg14                  ; // 14  (ADDR : 0x9C00_9EB8)
    UINT32  RGST_Group0317_Reg15                  ; // 15  (ADDR : 0x9C00_9EBC)
    UINT32  RGST_Group0317_Reg16                  ; // 16  (ADDR : 0x9C00_9EC0)
    UINT32  RGST_Group0317_Reg17                  ; // 17  (ADDR : 0x9C00_9EC4)
    UINT32  RGST_Group0317_Reg18                  ; // 18  (ADDR : 0x9C00_9EC8)
    UINT32  RGST_Group0317_Reg19                  ; // 19  (ADDR : 0x9C00_9ECC)
    UINT32  RGST_Group0317_Reg20                  ; // 20  (ADDR : 0x9C00_9ED0)
    UINT32  RGST_Group0317_Reg21                  ; // 21  (ADDR : 0x9C00_9ED4)
    UINT32  RGST_Group0317_Reg22                  ; // 22  (ADDR : 0x9C00_9ED8)
    UINT32  RGST_Group0317_Reg23                  ; // 23  (ADDR : 0x9C00_9EDC)
    UINT32  RGST_Group0317_Reg24                  ; // 24  (ADDR : 0x9C00_9EE0)
    UINT32  RGST_Group0317_Reg25                  ; // 25  (ADDR : 0x9C00_9EE4)
    UINT32  RGST_Group0317_Reg26                  ; // 26  (ADDR : 0x9C00_9EE8)
    UINT32  RGST_Group0317_Reg27                  ; // 27  (ADDR : 0x9C00_9EEC)
    UINT32  RGST_Group0317_Reg28                  ; // 28  (ADDR : 0x9C00_9EF0)
    UINT32  RGST_Group0317_Reg29                  ; // 29  (ADDR : 0x9C00_9EF4)
    UINT32  RGST_Group0317_Reg30                  ; // 30  (ADDR : 0x9C00_9EF8)
    UINT32  RGST_Group0317_Reg31                  ; // 31  (ADDR : 0x9C00_9EFC)

    //Group   318 : TVDEC
    UINT32  RGST_Group0318_Reg00                  ; // 00  (ADDR : 0x9C00_9F00)
    UINT32  RGST_Group0318_Reg01                  ; // 01  (ADDR : 0x9C00_9F04)
    UINT32  RGST_Group0318_Reg02                  ; // 02  (ADDR : 0x9C00_9F08)
    UINT32  RGST_Group0318_Reg03                  ; // 03  (ADDR : 0x9C00_9F0C)
    UINT32  RGST_Group0318_Reg04                  ; // 04  (ADDR : 0x9C00_9F10)
    UINT32  RGST_Group0318_Reg05                  ; // 05  (ADDR : 0x9C00_9F14)
    UINT32  RGST_Group0318_Reg06                  ; // 06  (ADDR : 0x9C00_9F18)
    UINT32  RGST_Group0318_Reg07                  ; // 07  (ADDR : 0x9C00_9F1C)
    UINT32  RGST_Group0318_Reg08                  ; // 08  (ADDR : 0x9C00_9F20)
    UINT32  RGST_Group0318_Reg09                  ; // 09  (ADDR : 0x9C00_9F24)
    UINT32  RGST_Group0318_Reg10                  ; // 10  (ADDR : 0x9C00_9F28)
    UINT32  RGST_Group0318_Reg11                  ; // 11  (ADDR : 0x9C00_9F2C)
    UINT32  RGST_Group0318_Reg12                  ; // 12  (ADDR : 0x9C00_9F30)
    UINT32  RGST_Group0318_Reg13                  ; // 13  (ADDR : 0x9C00_9F34)
    UINT32  RGST_Group0318_Reg14                  ; // 14  (ADDR : 0x9C00_9F38)
    UINT32  RGST_Group0318_Reg15                  ; // 15  (ADDR : 0x9C00_9F3C)
    UINT32  RGST_Group0318_Reg16                  ; // 16  (ADDR : 0x9C00_9F40)
    UINT32  RGST_Group0318_Reg17                  ; // 17  (ADDR : 0x9C00_9F44)
    UINT32  RGST_Group0318_Reg18                  ; // 18  (ADDR : 0x9C00_9F48)
    UINT32  RGST_Group0318_Reg19                  ; // 19  (ADDR : 0x9C00_9F4C)
    UINT32  RGST_Group0318_Reg20                  ; // 20  (ADDR : 0x9C00_9F50)
    UINT32  RGST_Group0318_Reg21                  ; // 21  (ADDR : 0x9C00_9F54)
    UINT32  RGST_Group0318_Reg22                  ; // 22  (ADDR : 0x9C00_9F58)
    UINT32  RGST_Group0318_Reg23                  ; // 23  (ADDR : 0x9C00_9F5C)
    UINT32  RGST_Group0318_Reg24                  ; // 24  (ADDR : 0x9C00_9F60)
    UINT32  RGST_Group0318_Reg25                  ; // 25  (ADDR : 0x9C00_9F64)
    UINT32  RGST_Group0318_Reg26                  ; // 26  (ADDR : 0x9C00_9F68)
    UINT32  RGST_Group0318_Reg27                  ; // 27  (ADDR : 0x9C00_9F6C)
    UINT32  RGST_Group0318_Reg28                  ; // 28  (ADDR : 0x9C00_9F70)
    UINT32  RGST_Group0318_Reg29                  ; // 29  (ADDR : 0x9C00_9F74)
    UINT32  RGST_Group0318_Reg30                  ; // 30  (ADDR : 0x9C00_9F78)
    UINT32  RGST_Group0318_Reg31                  ; // 31  (ADDR : 0x9C00_9F7C)

    //Group   319 : TVDEC
    UINT32  RGST_Group0319_Reg00                  ; // 00  (ADDR : 0x9C00_9F80)
    UINT32  RGST_Group0319_Reg01                  ; // 01  (ADDR : 0x9C00_9F84)
    UINT32  RGST_Group0319_Reg02                  ; // 02  (ADDR : 0x9C00_9F88)
    UINT32  RGST_Group0319_Reg03                  ; // 03  (ADDR : 0x9C00_9F8C)
    UINT32  RGST_Group0319_Reg04                  ; // 04  (ADDR : 0x9C00_9F90)
    UINT32  RGST_Group0319_Reg05                  ; // 05  (ADDR : 0x9C00_9F94)
    UINT32  RGST_Group0319_Reg06                  ; // 06  (ADDR : 0x9C00_9F98)
    UINT32  RGST_Group0319_Reg07                  ; // 07  (ADDR : 0x9C00_9F9C)
    UINT32  RGST_Group0319_Reg08                  ; // 08  (ADDR : 0x9C00_9FA0)
    UINT32  RGST_Group0319_Reg09                  ; // 09  (ADDR : 0x9C00_9FA4)
    UINT32  RGST_Group0319_Reg10                  ; // 10  (ADDR : 0x9C00_9FA8)
    UINT32  RGST_Group0319_Reg11                  ; // 11  (ADDR : 0x9C00_9FAC)
    UINT32  RGST_Group0319_Reg12                  ; // 12  (ADDR : 0x9C00_9FB0)
    UINT32  RGST_Group0319_Reg13                  ; // 13  (ADDR : 0x9C00_9FB4)
    UINT32  RGST_Group0319_Reg14                  ; // 14  (ADDR : 0x9C00_9FB8)
    UINT32  RGST_Group0319_Reg15                  ; // 15  (ADDR : 0x9C00_9FBC)
    UINT32  RGST_Group0319_Reg16                  ; // 16  (ADDR : 0x9C00_9FC0)
    UINT32  RGST_Group0319_Reg17                  ; // 17  (ADDR : 0x9C00_9FC4)
    UINT32  RGST_Group0319_Reg18                  ; // 18  (ADDR : 0x9C00_9FC8)
    UINT32  RGST_Group0319_Reg19                  ; // 19  (ADDR : 0x9C00_9FCC)
    UINT32  RGST_Group0319_Reg20                  ; // 20  (ADDR : 0x9C00_9FD0)
    UINT32  RGST_Group0319_Reg21                  ; // 21  (ADDR : 0x9C00_9FD4)
    UINT32  RGST_Group0319_Reg22                  ; // 22  (ADDR : 0x9C00_9FD8)
    UINT32  RGST_Group0319_Reg23                  ; // 23  (ADDR : 0x9C00_9FDC)
    UINT32  RGST_Group0319_Reg24                  ; // 24  (ADDR : 0x9C00_9FE0)
    UINT32  RGST_Group0319_Reg25                  ; // 25  (ADDR : 0x9C00_9FE4)
    UINT32  RGST_Group0319_Reg26                  ; // 26  (ADDR : 0x9C00_9FE8)
    UINT32  RGST_Group0319_Reg27                  ; // 27  (ADDR : 0x9C00_9FEC)
    UINT32  RGST_Group0319_Reg28                  ; // 28  (ADDR : 0x9C00_9FF0)
    UINT32  RGST_Group0319_Reg29                  ; // 29  (ADDR : 0x9C00_9FF4)
    UINT32  RGST_Group0319_Reg30                  ; // 30  (ADDR : 0x9C00_9FF8)
    UINT32  RGST_Group0319_Reg31                  ; // 31  (ADDR : 0x9C00_9FFC)

    //Group   320 : TVDEC
    UINT32  RGST_Group0320_Reg00                  ; // 00  (ADDR : 0x9C00_A000)
    UINT32  RGST_Group0320_Reg01                  ; // 01  (ADDR : 0x9C00_A004)
    UINT32  RGST_Group0320_Reg02                  ; // 02  (ADDR : 0x9C00_A008)
    UINT32  RGST_Group0320_Reg03                  ; // 03  (ADDR : 0x9C00_A00C)
    UINT32  RGST_Group0320_Reg04                  ; // 04  (ADDR : 0x9C00_A010)
    UINT32  RGST_Group0320_Reg05                  ; // 05  (ADDR : 0x9C00_A014)
    UINT32  RGST_Group0320_Reg06                  ; // 06  (ADDR : 0x9C00_A018)
    UINT32  RGST_Group0320_Reg07                  ; // 07  (ADDR : 0x9C00_A01C)
    UINT32  RGST_Group0320_Reg08                  ; // 08  (ADDR : 0x9C00_A020)
    UINT32  RGST_Group0320_Reg09                  ; // 09  (ADDR : 0x9C00_A024)
    UINT32  RGST_Group0320_Reg10                  ; // 10  (ADDR : 0x9C00_A028)
    UINT32  RGST_Group0320_Reg11                  ; // 11  (ADDR : 0x9C00_A02C)
    UINT32  RGST_Group0320_Reg12                  ; // 12  (ADDR : 0x9C00_A030)
    UINT32  RGST_Group0320_Reg13                  ; // 13  (ADDR : 0x9C00_A034)
    UINT32  RGST_Group0320_Reg14                  ; // 14  (ADDR : 0x9C00_A038)
    UINT32  RGST_Group0320_Reg15                  ; // 15  (ADDR : 0x9C00_A03C)
    UINT32  RGST_Group0320_Reg16                  ; // 16  (ADDR : 0x9C00_A040)
    UINT32  RGST_Group0320_Reg17                  ; // 17  (ADDR : 0x9C00_A044)
    UINT32  RGST_Group0320_Reg18                  ; // 18  (ADDR : 0x9C00_A048)
    UINT32  RGST_Group0320_Reg19                  ; // 19  (ADDR : 0x9C00_A04C)
    UINT32  RGST_Group0320_Reg20                  ; // 20  (ADDR : 0x9C00_A050)
    UINT32  RGST_Group0320_Reg21                  ; // 21  (ADDR : 0x9C00_A054)
    UINT32  RGST_Group0320_Reg22                  ; // 22  (ADDR : 0x9C00_A058)
    UINT32  RGST_Group0320_Reg23                  ; // 23  (ADDR : 0x9C00_A05C)
    UINT32  RGST_Group0320_Reg24                  ; // 24  (ADDR : 0x9C00_A060)
    UINT32  RGST_Group0320_Reg25                  ; // 25  (ADDR : 0x9C00_A064)
    UINT32  RGST_Group0320_Reg26                  ; // 26  (ADDR : 0x9C00_A068)
    UINT32  RGST_Group0320_Reg27                  ; // 27  (ADDR : 0x9C00_A06C)
    UINT32  RGST_Group0320_Reg28                  ; // 28  (ADDR : 0x9C00_A070)
    UINT32  RGST_Group0320_Reg29                  ; // 29  (ADDR : 0x9C00_A074)
    UINT32  RGST_Group0320_Reg30                  ; // 30  (ADDR : 0x9C00_A078)
    UINT32  RGST_Group0320_Reg31                  ; // 31  (ADDR : 0x9C00_A07C)

    //Group   321 : TVDEC
    UINT32  RGST_Group0321_Reg00                  ; // 00  (ADDR : 0x9C00_A080)
    UINT32  RGST_Group0321_Reg01                  ; // 01  (ADDR : 0x9C00_A084)
    UINT32  RGST_Group0321_Reg02                  ; // 02  (ADDR : 0x9C00_A088)
    UINT32  RGST_Group0321_Reg03                  ; // 03  (ADDR : 0x9C00_A08C)
    UINT32  RGST_Group0321_Reg04                  ; // 04  (ADDR : 0x9C00_A090)
    UINT32  RGST_Group0321_Reg05                  ; // 05  (ADDR : 0x9C00_A094)
    UINT32  RGST_Group0321_Reg06                  ; // 06  (ADDR : 0x9C00_A098)
    UINT32  RGST_Group0321_Reg07                  ; // 07  (ADDR : 0x9C00_A09C)
    UINT32  RGST_Group0321_Reg08                  ; // 08  (ADDR : 0x9C00_A0A0)
    UINT32  RGST_Group0321_Reg09                  ; // 09  (ADDR : 0x9C00_A0A4)
    UINT32  RGST_Group0321_Reg10                  ; // 10  (ADDR : 0x9C00_A0A8)
    UINT32  RGST_Group0321_Reg11                  ; // 11  (ADDR : 0x9C00_A0AC)
    UINT32  RGST_Group0321_Reg12                  ; // 12  (ADDR : 0x9C00_A0B0)
    UINT32  RGST_Group0321_Reg13                  ; // 13  (ADDR : 0x9C00_A0B4)
    UINT32  RGST_Group0321_Reg14                  ; // 14  (ADDR : 0x9C00_A0B8)
    UINT32  RGST_Group0321_Reg15                  ; // 15  (ADDR : 0x9C00_A0BC)
    UINT32  RGST_Group0321_Reg16                  ; // 16  (ADDR : 0x9C00_A0C0)
    UINT32  RGST_Group0321_Reg17                  ; // 17  (ADDR : 0x9C00_A0C4)
    UINT32  RGST_Group0321_Reg18                  ; // 18  (ADDR : 0x9C00_A0C8)
    UINT32  RGST_Group0321_Reg19                  ; // 19  (ADDR : 0x9C00_A0CC)
    UINT32  RGST_Group0321_Reg20                  ; // 20  (ADDR : 0x9C00_A0D0)
    UINT32  RGST_Group0321_Reg21                  ; // 21  (ADDR : 0x9C00_A0D4)
    UINT32  RGST_Group0321_Reg22                  ; // 22  (ADDR : 0x9C00_A0D8)
    UINT32  RGST_Group0321_Reg23                  ; // 23  (ADDR : 0x9C00_A0DC)
    UINT32  RGST_Group0321_Reg24                  ; // 24  (ADDR : 0x9C00_A0E0)
    UINT32  RGST_Group0321_Reg25                  ; // 25  (ADDR : 0x9C00_A0E4)
    UINT32  RGST_Group0321_Reg26                  ; // 26  (ADDR : 0x9C00_A0E8)
    UINT32  RGST_Group0321_Reg27                  ; // 27  (ADDR : 0x9C00_A0EC)
    UINT32  RGST_Group0321_Reg28                  ; // 28  (ADDR : 0x9C00_A0F0)
    UINT32  RGST_Group0321_Reg29                  ; // 29  (ADDR : 0x9C00_A0F4)
    UINT32  RGST_Group0321_Reg30                  ; // 30  (ADDR : 0x9C00_A0F8)
    UINT32  RGST_Group0321_Reg31                  ; // 31  (ADDR : 0x9C00_A0FC)

    //Group   322 : TVDEC
    UINT32  RGST_Group0322_Reg00                  ; // 00  (ADDR : 0x9C00_A100)
    UINT32  RGST_Group0322_Reg01                  ; // 01  (ADDR : 0x9C00_A104)
    UINT32  RGST_Group0322_Reg02                  ; // 02  (ADDR : 0x9C00_A108)
    UINT32  RGST_Group0322_Reg03                  ; // 03  (ADDR : 0x9C00_A10C)
    UINT32  RGST_Group0322_Reg04                  ; // 04  (ADDR : 0x9C00_A110)
    UINT32  RGST_Group0322_Reg05                  ; // 05  (ADDR : 0x9C00_A114)
    UINT32  RGST_Group0322_Reg06                  ; // 06  (ADDR : 0x9C00_A118)
    UINT32  RGST_Group0322_Reg07                  ; // 07  (ADDR : 0x9C00_A11C)
    UINT32  RGST_Group0322_Reg08                  ; // 08  (ADDR : 0x9C00_A120)
    UINT32  RGST_Group0322_Reg09                  ; // 09  (ADDR : 0x9C00_A124)
    UINT32  RGST_Group0322_Reg10                  ; // 10  (ADDR : 0x9C00_A128)
    UINT32  RGST_Group0322_Reg11                  ; // 11  (ADDR : 0x9C00_A12C)
    UINT32  RGST_Group0322_Reg12                  ; // 12  (ADDR : 0x9C00_A130)
    UINT32  RGST_Group0322_Reg13                  ; // 13  (ADDR : 0x9C00_A134)
    UINT32  RGST_Group0322_Reg14                  ; // 14  (ADDR : 0x9C00_A138)
    UINT32  RGST_Group0322_Reg15                  ; // 15  (ADDR : 0x9C00_A13C)
    UINT32  RGST_Group0322_Reg16                  ; // 16  (ADDR : 0x9C00_A140)
    UINT32  RGST_Group0322_Reg17                  ; // 17  (ADDR : 0x9C00_A144)
    UINT32  RGST_Group0322_Reg18                  ; // 18  (ADDR : 0x9C00_A148)
    UINT32  RGST_Group0322_Reg19                  ; // 19  (ADDR : 0x9C00_A14C)
    UINT32  RGST_Group0322_Reg20                  ; // 20  (ADDR : 0x9C00_A150)
    UINT32  RGST_Group0322_Reg21                  ; // 21  (ADDR : 0x9C00_A154)
    UINT32  RGST_Group0322_Reg22                  ; // 22  (ADDR : 0x9C00_A158)
    UINT32  RGST_Group0322_Reg23                  ; // 23  (ADDR : 0x9C00_A15C)
    UINT32  RGST_Group0322_Reg24                  ; // 24  (ADDR : 0x9C00_A160)
    UINT32  RGST_Group0322_Reg25                  ; // 25  (ADDR : 0x9C00_A164)
    UINT32  RGST_Group0322_Reg26                  ; // 26  (ADDR : 0x9C00_A168)
    UINT32  RGST_Group0322_Reg27                  ; // 27  (ADDR : 0x9C00_A16C)
    UINT32  RGST_Group0322_Reg28                  ; // 28  (ADDR : 0x9C00_A170)
    UINT32  RGST_Group0322_Reg29                  ; // 29  (ADDR : 0x9C00_A174)
    UINT32  RGST_Group0322_Reg30                  ; // 30  (ADDR : 0x9C00_A178)
    UINT32  RGST_Group0322_Reg31                  ; // 31  (ADDR : 0x9C00_A17C)

    //Group   323 : TVDEC
    UINT32  RGST_Group0323_Reg00                  ; // 00  (ADDR : 0x9C00_A180)
    UINT32  RGST_Group0323_Reg01                  ; // 01  (ADDR : 0x9C00_A184)
    UINT32  RGST_Group0323_Reg02                  ; // 02  (ADDR : 0x9C00_A188)
    UINT32  RGST_Group0323_Reg03                  ; // 03  (ADDR : 0x9C00_A18C)
    UINT32  RGST_Group0323_Reg04                  ; // 04  (ADDR : 0x9C00_A190)
    UINT32  RGST_Group0323_Reg05                  ; // 05  (ADDR : 0x9C00_A194)
    UINT32  RGST_Group0323_Reg06                  ; // 06  (ADDR : 0x9C00_A198)
    UINT32  RGST_Group0323_Reg07                  ; // 07  (ADDR : 0x9C00_A19C)
    UINT32  RGST_Group0323_Reg08                  ; // 08  (ADDR : 0x9C00_A1A0)
    UINT32  RGST_Group0323_Reg09                  ; // 09  (ADDR : 0x9C00_A1A4)
    UINT32  RGST_Group0323_Reg10                  ; // 10  (ADDR : 0x9C00_A1A8)
    UINT32  RGST_Group0323_Reg11                  ; // 11  (ADDR : 0x9C00_A1AC)
    UINT32  RGST_Group0323_Reg12                  ; // 12  (ADDR : 0x9C00_A1B0)
    UINT32  RGST_Group0323_Reg13                  ; // 13  (ADDR : 0x9C00_A1B4)
    UINT32  RGST_Group0323_Reg14                  ; // 14  (ADDR : 0x9C00_A1B8)
    UINT32  RGST_Group0323_Reg15                  ; // 15  (ADDR : 0x9C00_A1BC)
    UINT32  RGST_Group0323_Reg16                  ; // 16  (ADDR : 0x9C00_A1C0)
    UINT32  RGST_Group0323_Reg17                  ; // 17  (ADDR : 0x9C00_A1C4)
    UINT32  RGST_Group0323_Reg18                  ; // 18  (ADDR : 0x9C00_A1C8)
    UINT32  RGST_Group0323_Reg19                  ; // 19  (ADDR : 0x9C00_A1CC)
    UINT32  RGST_Group0323_Reg20                  ; // 20  (ADDR : 0x9C00_A1D0)
    UINT32  RGST_Group0323_Reg21                  ; // 21  (ADDR : 0x9C00_A1D4)
    UINT32  RGST_Group0323_Reg22                  ; // 22  (ADDR : 0x9C00_A1D8)
    UINT32  RGST_Group0323_Reg23                  ; // 23  (ADDR : 0x9C00_A1DC)
    UINT32  RGST_Group0323_Reg24                  ; // 24  (ADDR : 0x9C00_A1E0)
    UINT32  RGST_Group0323_Reg25                  ; // 25  (ADDR : 0x9C00_A1E4)
    UINT32  RGST_Group0323_Reg26                  ; // 26  (ADDR : 0x9C00_A1E8)
    UINT32  RGST_Group0323_Reg27                  ; // 27  (ADDR : 0x9C00_A1EC)
    UINT32  RGST_Group0323_Reg28                  ; // 28  (ADDR : 0x9C00_A1F0)
    UINT32  RGST_Group0323_Reg29                  ; // 29  (ADDR : 0x9C00_A1F4)
    UINT32  RGST_Group0323_Reg30                  ; // 30  (ADDR : 0x9C00_A1F8)
    UINT32  RGST_Group0323_Reg31                  ; // 31  (ADDR : 0x9C00_A1FC)

    //Group   324 : TVDEC
    UINT32  RGST_Group0324_Reg00                  ; // 00  (ADDR : 0x9C00_A200)
    UINT32  RGST_Group0324_Reg01                  ; // 01  (ADDR : 0x9C00_A204)
    UINT32  RGST_Group0324_Reg02                  ; // 02  (ADDR : 0x9C00_A208)
    UINT32  RGST_Group0324_Reg03                  ; // 03  (ADDR : 0x9C00_A20C)
    UINT32  RGST_Group0324_Reg04                  ; // 04  (ADDR : 0x9C00_A210)
    UINT32  RGST_Group0324_Reg05                  ; // 05  (ADDR : 0x9C00_A214)
    UINT32  RGST_Group0324_Reg06                  ; // 06  (ADDR : 0x9C00_A218)
    UINT32  RGST_Group0324_Reg07                  ; // 07  (ADDR : 0x9C00_A21C)
    UINT32  RGST_Group0324_Reg08                  ; // 08  (ADDR : 0x9C00_A220)
    UINT32  RGST_Group0324_Reg09                  ; // 09  (ADDR : 0x9C00_A224)
    UINT32  RGST_Group0324_Reg10                  ; // 10  (ADDR : 0x9C00_A228)
    UINT32  RGST_Group0324_Reg11                  ; // 11  (ADDR : 0x9C00_A22C)
    UINT32  RGST_Group0324_Reg12                  ; // 12  (ADDR : 0x9C00_A230)
    UINT32  RGST_Group0324_Reg13                  ; // 13  (ADDR : 0x9C00_A234)
    UINT32  RGST_Group0324_Reg14                  ; // 14  (ADDR : 0x9C00_A238)
    UINT32  RGST_Group0324_Reg15                  ; // 15  (ADDR : 0x9C00_A23C)
    UINT32  RGST_Group0324_Reg16                  ; // 16  (ADDR : 0x9C00_A240)
    UINT32  RGST_Group0324_Reg17                  ; // 17  (ADDR : 0x9C00_A244)
    UINT32  RGST_Group0324_Reg18                  ; // 18  (ADDR : 0x9C00_A248)
    UINT32  RGST_Group0324_Reg19                  ; // 19  (ADDR : 0x9C00_A24C)
    UINT32  RGST_Group0324_Reg20                  ; // 20  (ADDR : 0x9C00_A250)
    UINT32  RGST_Group0324_Reg21                  ; // 21  (ADDR : 0x9C00_A254)
    UINT32  RGST_Group0324_Reg22                  ; // 22  (ADDR : 0x9C00_A258)
    UINT32  RGST_Group0324_Reg23                  ; // 23  (ADDR : 0x9C00_A25C)
    UINT32  RGST_Group0324_Reg24                  ; // 24  (ADDR : 0x9C00_A260)
    UINT32  RGST_Group0324_Reg25                  ; // 25  (ADDR : 0x9C00_A264)
    UINT32  RGST_Group0324_Reg26                  ; // 26  (ADDR : 0x9C00_A268)
    UINT32  RGST_Group0324_Reg27                  ; // 27  (ADDR : 0x9C00_A26C)
    UINT32  RGST_Group0324_Reg28                  ; // 28  (ADDR : 0x9C00_A270)
    UINT32  RGST_Group0324_Reg29                  ; // 29  (ADDR : 0x9C00_A274)
    UINT32  RGST_Group0324_Reg30                  ; // 30  (ADDR : 0x9C00_A278)
    UINT32  RGST_Group0324_Reg31                  ; // 31  (ADDR : 0x9C00_A27C)

    //Group   325 : TVDEC
    UINT32  RGST_Group0325_Reg00                  ; // 00  (ADDR : 0x9C00_A280)
    UINT32  RGST_Group0325_Reg01                  ; // 01  (ADDR : 0x9C00_A284)
    UINT32  RGST_Group0325_Reg02                  ; // 02  (ADDR : 0x9C00_A288)
    UINT32  RGST_Group0325_Reg03                  ; // 03  (ADDR : 0x9C00_A28C)
    UINT32  RGST_Group0325_Reg04                  ; // 04  (ADDR : 0x9C00_A290)
    UINT32  RGST_Group0325_Reg05                  ; // 05  (ADDR : 0x9C00_A294)
    UINT32  RGST_Group0325_Reg06                  ; // 06  (ADDR : 0x9C00_A298)
    UINT32  RGST_Group0325_Reg07                  ; // 07  (ADDR : 0x9C00_A29C)
    UINT32  RGST_Group0325_Reg08                  ; // 08  (ADDR : 0x9C00_A2A0)
    UINT32  RGST_Group0325_Reg09                  ; // 09  (ADDR : 0x9C00_A2A4)
    UINT32  RGST_Group0325_Reg10                  ; // 10  (ADDR : 0x9C00_A2A8)
    UINT32  RGST_Group0325_Reg11                  ; // 11  (ADDR : 0x9C00_A2AC)
    UINT32  RGST_Group0325_Reg12                  ; // 12  (ADDR : 0x9C00_A2B0)
    UINT32  RGST_Group0325_Reg13                  ; // 13  (ADDR : 0x9C00_A2B4)
    UINT32  RGST_Group0325_Reg14                  ; // 14  (ADDR : 0x9C00_A2B8)
    UINT32  RGST_Group0325_Reg15                  ; // 15  (ADDR : 0x9C00_A2BC)
    UINT32  RGST_Group0325_Reg16                  ; // 16  (ADDR : 0x9C00_A2C0)
    UINT32  RGST_Group0325_Reg17                  ; // 17  (ADDR : 0x9C00_A2C4)
    UINT32  RGST_Group0325_Reg18                  ; // 18  (ADDR : 0x9C00_A2C8)
    UINT32  RGST_Group0325_Reg19                  ; // 19  (ADDR : 0x9C00_A2CC)
    UINT32  RGST_Group0325_Reg20                  ; // 20  (ADDR : 0x9C00_A2D0)
    UINT32  RGST_Group0325_Reg21                  ; // 21  (ADDR : 0x9C00_A2D4)
    UINT32  RGST_Group0325_Reg22                  ; // 22  (ADDR : 0x9C00_A2D8)
    UINT32  RGST_Group0325_Reg23                  ; // 23  (ADDR : 0x9C00_A2DC)
    UINT32  RGST_Group0325_Reg24                  ; // 24  (ADDR : 0x9C00_A2E0)
    UINT32  RGST_Group0325_Reg25                  ; // 25  (ADDR : 0x9C00_A2E4)
    UINT32  RGST_Group0325_Reg26                  ; // 26  (ADDR : 0x9C00_A2E8)
    UINT32  RGST_Group0325_Reg27                  ; // 27  (ADDR : 0x9C00_A2EC)
    UINT32  RGST_Group0325_Reg28                  ; // 28  (ADDR : 0x9C00_A2F0)
    UINT32  RGST_Group0325_Reg29                  ; // 29  (ADDR : 0x9C00_A2F4)
    UINT32  RGST_Group0325_Reg30                  ; // 30  (ADDR : 0x9C00_A2F8)
    UINT32  RGST_Group0325_Reg31                  ; // 31  (ADDR : 0x9C00_A2FC)

    //Group   326 : TVDEC
    UINT32  RGST_Group0326_Reg00                  ; // 00  (ADDR : 0x9C00_A300)
    UINT32  RGST_Group0326_Reg01                  ; // 01  (ADDR : 0x9C00_A304)
    UINT32  RGST_Group0326_Reg02                  ; // 02  (ADDR : 0x9C00_A308)
    UINT32  RGST_Group0326_Reg03                  ; // 03  (ADDR : 0x9C00_A30C)
    UINT32  RGST_Group0326_Reg04                  ; // 04  (ADDR : 0x9C00_A310)
    UINT32  RGST_Group0326_Reg05                  ; // 05  (ADDR : 0x9C00_A314)
    UINT32  RGST_Group0326_Reg06                  ; // 06  (ADDR : 0x9C00_A318)
    UINT32  RGST_Group0326_Reg07                  ; // 07  (ADDR : 0x9C00_A31C)
    UINT32  RGST_Group0326_Reg08                  ; // 08  (ADDR : 0x9C00_A320)
    UINT32  RGST_Group0326_Reg09                  ; // 09  (ADDR : 0x9C00_A324)
    UINT32  RGST_Group0326_Reg10                  ; // 10  (ADDR : 0x9C00_A328)
    UINT32  RGST_Group0326_Reg11                  ; // 11  (ADDR : 0x9C00_A32C)
    UINT32  RGST_Group0326_Reg12                  ; // 12  (ADDR : 0x9C00_A330)
    UINT32  RGST_Group0326_Reg13                  ; // 13  (ADDR : 0x9C00_A334)
    UINT32  RGST_Group0326_Reg14                  ; // 14  (ADDR : 0x9C00_A338)
    UINT32  RGST_Group0326_Reg15                  ; // 15  (ADDR : 0x9C00_A33C)
    UINT32  RGST_Group0326_Reg16                  ; // 16  (ADDR : 0x9C00_A340)
    UINT32  RGST_Group0326_Reg17                  ; // 17  (ADDR : 0x9C00_A344)
    UINT32  RGST_Group0326_Reg18                  ; // 18  (ADDR : 0x9C00_A348)
    UINT32  RGST_Group0326_Reg19                  ; // 19  (ADDR : 0x9C00_A34C)
    UINT32  RGST_Group0326_Reg20                  ; // 20  (ADDR : 0x9C00_A350)
    UINT32  RGST_Group0326_Reg21                  ; // 21  (ADDR : 0x9C00_A354)
    UINT32  RGST_Group0326_Reg22                  ; // 22  (ADDR : 0x9C00_A358)
    UINT32  RGST_Group0326_Reg23                  ; // 23  (ADDR : 0x9C00_A35C)
    UINT32  RGST_Group0326_Reg24                  ; // 24  (ADDR : 0x9C00_A360)
    UINT32  RGST_Group0326_Reg25                  ; // 25  (ADDR : 0x9C00_A364)
    UINT32  RGST_Group0326_Reg26                  ; // 26  (ADDR : 0x9C00_A368)
    UINT32  RGST_Group0326_Reg27                  ; // 27  (ADDR : 0x9C00_A36C)
    UINT32  RGST_Group0326_Reg28                  ; // 28  (ADDR : 0x9C00_A370)
    UINT32  RGST_Group0326_Reg29                  ; // 29  (ADDR : 0x9C00_A374)
    UINT32  RGST_Group0326_Reg30                  ; // 30  (ADDR : 0x9C00_A378)
    UINT32  RGST_Group0326_Reg31                  ; // 31  (ADDR : 0x9C00_A37C)

    //Group   327 : TVDEC
    UINT32  RGST_Group0327_Reg00                  ; // 00  (ADDR : 0x9C00_A380)
    UINT32  RGST_Group0327_Reg01                  ; // 01  (ADDR : 0x9C00_A384)
    UINT32  RGST_Group0327_Reg02                  ; // 02  (ADDR : 0x9C00_A388)
    UINT32  RGST_Group0327_Reg03                  ; // 03  (ADDR : 0x9C00_A38C)
    UINT32  RGST_Group0327_Reg04                  ; // 04  (ADDR : 0x9C00_A390)
    UINT32  RGST_Group0327_Reg05                  ; // 05  (ADDR : 0x9C00_A394)
    UINT32  RGST_Group0327_Reg06                  ; // 06  (ADDR : 0x9C00_A398)
    UINT32  RGST_Group0327_Reg07                  ; // 07  (ADDR : 0x9C00_A39C)
    UINT32  RGST_Group0327_Reg08                  ; // 08  (ADDR : 0x9C00_A3A0)
    UINT32  RGST_Group0327_Reg09                  ; // 09  (ADDR : 0x9C00_A3A4)
    UINT32  RGST_Group0327_Reg10                  ; // 10  (ADDR : 0x9C00_A3A8)
    UINT32  RGST_Group0327_Reg11                  ; // 11  (ADDR : 0x9C00_A3AC)
    UINT32  RGST_Group0327_Reg12                  ; // 12  (ADDR : 0x9C00_A3B0)
    UINT32  RGST_Group0327_Reg13                  ; // 13  (ADDR : 0x9C00_A3B4)
    UINT32  RGST_Group0327_Reg14                  ; // 14  (ADDR : 0x9C00_A3B8)
    UINT32  RGST_Group0327_Reg15                  ; // 15  (ADDR : 0x9C00_A3BC)
    UINT32  RGST_Group0327_Reg16                  ; // 16  (ADDR : 0x9C00_A3C0)
    UINT32  RGST_Group0327_Reg17                  ; // 17  (ADDR : 0x9C00_A3C4)
    UINT32  RGST_Group0327_Reg18                  ; // 18  (ADDR : 0x9C00_A3C8)
    UINT32  RGST_Group0327_Reg19                  ; // 19  (ADDR : 0x9C00_A3CC)
    UINT32  RGST_Group0327_Reg20                  ; // 20  (ADDR : 0x9C00_A3D0)
    UINT32  RGST_Group0327_Reg21                  ; // 21  (ADDR : 0x9C00_A3D4)
    UINT32  RGST_Group0327_Reg22                  ; // 22  (ADDR : 0x9C00_A3D8)
    UINT32  RGST_Group0327_Reg23                  ; // 23  (ADDR : 0x9C00_A3DC)
    UINT32  RGST_Group0327_Reg24                  ; // 24  (ADDR : 0x9C00_A3E0)
    UINT32  RGST_Group0327_Reg25                  ; // 25  (ADDR : 0x9C00_A3E4)
    UINT32  RGST_Group0327_Reg26                  ; // 26  (ADDR : 0x9C00_A3E8)
    UINT32  RGST_Group0327_Reg27                  ; // 27  (ADDR : 0x9C00_A3EC)
    UINT32  RGST_Group0327_Reg28                  ; // 28  (ADDR : 0x9C00_A3F0)
    UINT32  RGST_Group0327_Reg29                  ; // 29  (ADDR : 0x9C00_A3F4)
    UINT32  RGST_Group0327_Reg30                  ; // 30  (ADDR : 0x9C00_A3F8)
    UINT32  RGST_Group0327_Reg31                  ; // 31  (ADDR : 0x9C00_A3FC)

    //Group   328 : TVDEC
    UINT32  RGST_Group0328_Reg00                  ; // 00  (ADDR : 0x9C00_A400)
    UINT32  RGST_Group0328_Reg01                  ; // 01  (ADDR : 0x9C00_A404)
    UINT32  RGST_Group0328_Reg02                  ; // 02  (ADDR : 0x9C00_A408)
    UINT32  RGST_Group0328_Reg03                  ; // 03  (ADDR : 0x9C00_A40C)
    UINT32  RGST_Group0328_Reg04                  ; // 04  (ADDR : 0x9C00_A410)
    UINT32  RGST_Group0328_Reg05                  ; // 05  (ADDR : 0x9C00_A414)
    UINT32  RGST_Group0328_Reg06                  ; // 06  (ADDR : 0x9C00_A418)
    UINT32  RGST_Group0328_Reg07                  ; // 07  (ADDR : 0x9C00_A41C)
    UINT32  RGST_Group0328_Reg08                  ; // 08  (ADDR : 0x9C00_A420)
    UINT32  RGST_Group0328_Reg09                  ; // 09  (ADDR : 0x9C00_A424)
    UINT32  RGST_Group0328_Reg10                  ; // 10  (ADDR : 0x9C00_A428)
    UINT32  RGST_Group0328_Reg11                  ; // 11  (ADDR : 0x9C00_A42C)
    UINT32  RGST_Group0328_Reg12                  ; // 12  (ADDR : 0x9C00_A430)
    UINT32  RGST_Group0328_Reg13                  ; // 13  (ADDR : 0x9C00_A434)
    UINT32  RGST_Group0328_Reg14                  ; // 14  (ADDR : 0x9C00_A438)
    UINT32  RGST_Group0328_Reg15                  ; // 15  (ADDR : 0x9C00_A43C)
    UINT32  RGST_Group0328_Reg16                  ; // 16  (ADDR : 0x9C00_A440)
    UINT32  RGST_Group0328_Reg17                  ; // 17  (ADDR : 0x9C00_A444)
    UINT32  RGST_Group0328_Reg18                  ; // 18  (ADDR : 0x9C00_A448)
    UINT32  RGST_Group0328_Reg19                  ; // 19  (ADDR : 0x9C00_A44C)
    UINT32  RGST_Group0328_Reg20                  ; // 20  (ADDR : 0x9C00_A450)
    UINT32  RGST_Group0328_Reg21                  ; // 21  (ADDR : 0x9C00_A454)
    UINT32  RGST_Group0328_Reg22                  ; // 22  (ADDR : 0x9C00_A458)
    UINT32  RGST_Group0328_Reg23                  ; // 23  (ADDR : 0x9C00_A45C)
    UINT32  RGST_Group0328_Reg24                  ; // 24  (ADDR : 0x9C00_A460)
    UINT32  RGST_Group0328_Reg25                  ; // 25  (ADDR : 0x9C00_A464)
    UINT32  RGST_Group0328_Reg26                  ; // 26  (ADDR : 0x9C00_A468)
    UINT32  RGST_Group0328_Reg27                  ; // 27  (ADDR : 0x9C00_A46C)
    UINT32  RGST_Group0328_Reg28                  ; // 28  (ADDR : 0x9C00_A470)
    UINT32  RGST_Group0328_Reg29                  ; // 29  (ADDR : 0x9C00_A474)
    UINT32  RGST_Group0328_Reg30                  ; // 30  (ADDR : 0x9C00_A478)
    UINT32  RGST_Group0328_Reg31                  ; // 31  (ADDR : 0x9C00_A47C)

    //Group   329 : TVDEC
    UINT32  RGST_Group0329_Reg00                  ; // 00  (ADDR : 0x9C00_A480)
    UINT32  RGST_Group0329_Reg01                  ; // 01  (ADDR : 0x9C00_A484)
    UINT32  RGST_Group0329_Reg02                  ; // 02  (ADDR : 0x9C00_A488)
    UINT32  RGST_Group0329_Reg03                  ; // 03  (ADDR : 0x9C00_A48C)
    UINT32  RGST_Group0329_Reg04                  ; // 04  (ADDR : 0x9C00_A490)
    UINT32  RGST_Group0329_Reg05                  ; // 05  (ADDR : 0x9C00_A494)
    UINT32  RGST_Group0329_Reg06                  ; // 06  (ADDR : 0x9C00_A498)
    UINT32  RGST_Group0329_Reg07                  ; // 07  (ADDR : 0x9C00_A49C)
    UINT32  RGST_Group0329_Reg08                  ; // 08  (ADDR : 0x9C00_A4A0)
    UINT32  RGST_Group0329_Reg09                  ; // 09  (ADDR : 0x9C00_A4A4)
    UINT32  RGST_Group0329_Reg10                  ; // 10  (ADDR : 0x9C00_A4A8)
    UINT32  RGST_Group0329_Reg11                  ; // 11  (ADDR : 0x9C00_A4AC)
    UINT32  RGST_Group0329_Reg12                  ; // 12  (ADDR : 0x9C00_A4B0)
    UINT32  RGST_Group0329_Reg13                  ; // 13  (ADDR : 0x9C00_A4B4)
    UINT32  RGST_Group0329_Reg14                  ; // 14  (ADDR : 0x9C00_A4B8)
    UINT32  RGST_Group0329_Reg15                  ; // 15  (ADDR : 0x9C00_A4BC)
    UINT32  RGST_Group0329_Reg16                  ; // 16  (ADDR : 0x9C00_A4C0)
    UINT32  RGST_Group0329_Reg17                  ; // 17  (ADDR : 0x9C00_A4C4)
    UINT32  RGST_Group0329_Reg18                  ; // 18  (ADDR : 0x9C00_A4C8)
    UINT32  RGST_Group0329_Reg19                  ; // 19  (ADDR : 0x9C00_A4CC)
    UINT32  RGST_Group0329_Reg20                  ; // 20  (ADDR : 0x9C00_A4D0)
    UINT32  RGST_Group0329_Reg21                  ; // 21  (ADDR : 0x9C00_A4D4)
    UINT32  RGST_Group0329_Reg22                  ; // 22  (ADDR : 0x9C00_A4D8)
    UINT32  RGST_Group0329_Reg23                  ; // 23  (ADDR : 0x9C00_A4DC)
    UINT32  RGST_Group0329_Reg24                  ; // 24  (ADDR : 0x9C00_A4E0)
    UINT32  RGST_Group0329_Reg25                  ; // 25  (ADDR : 0x9C00_A4E4)
    UINT32  RGST_Group0329_Reg26                  ; // 26  (ADDR : 0x9C00_A4E8)
    UINT32  RGST_Group0329_Reg27                  ; // 27  (ADDR : 0x9C00_A4EC)
    UINT32  RGST_Group0329_Reg28                  ; // 28  (ADDR : 0x9C00_A4F0)
    UINT32  RGST_Group0329_Reg29                  ; // 29  (ADDR : 0x9C00_A4F4)
    UINT32  RGST_Group0329_Reg30                  ; // 30  (ADDR : 0x9C00_A4F8)
    UINT32  RGST_Group0329_Reg31                  ; // 31  (ADDR : 0x9C00_A4FC)

    //Group   330 : TVDEC
    UINT32  RGST_Group0330_Reg00                  ; // 00  (ADDR : 0x9C00_A500)
    UINT32  RGST_Group0330_Reg01                  ; // 01  (ADDR : 0x9C00_A504)
    UINT32  RGST_Group0330_Reg02                  ; // 02  (ADDR : 0x9C00_A508)
    UINT32  RGST_Group0330_Reg03                  ; // 03  (ADDR : 0x9C00_A50C)
    UINT32  RGST_Group0330_Reg04                  ; // 04  (ADDR : 0x9C00_A510)
    UINT32  RGST_Group0330_Reg05                  ; // 05  (ADDR : 0x9C00_A514)
    UINT32  RGST_Group0330_Reg06                  ; // 06  (ADDR : 0x9C00_A518)
    UINT32  RGST_Group0330_Reg07                  ; // 07  (ADDR : 0x9C00_A51C)
    UINT32  RGST_Group0330_Reg08                  ; // 08  (ADDR : 0x9C00_A520)
    UINT32  RGST_Group0330_Reg09                  ; // 09  (ADDR : 0x9C00_A524)
    UINT32  RGST_Group0330_Reg10                  ; // 10  (ADDR : 0x9C00_A528)
    UINT32  RGST_Group0330_Reg11                  ; // 11  (ADDR : 0x9C00_A52C)
    UINT32  RGST_Group0330_Reg12                  ; // 12  (ADDR : 0x9C00_A530)
    UINT32  RGST_Group0330_Reg13                  ; // 13  (ADDR : 0x9C00_A534)
    UINT32  RGST_Group0330_Reg14                  ; // 14  (ADDR : 0x9C00_A538)
    UINT32  RGST_Group0330_Reg15                  ; // 15  (ADDR : 0x9C00_A53C)
    UINT32  RGST_Group0330_Reg16                  ; // 16  (ADDR : 0x9C00_A540)
    UINT32  RGST_Group0330_Reg17                  ; // 17  (ADDR : 0x9C00_A544)
    UINT32  RGST_Group0330_Reg18                  ; // 18  (ADDR : 0x9C00_A548)
    UINT32  RGST_Group0330_Reg19                  ; // 19  (ADDR : 0x9C00_A54C)
    UINT32  RGST_Group0330_Reg20                  ; // 20  (ADDR : 0x9C00_A550)
    UINT32  RGST_Group0330_Reg21                  ; // 21  (ADDR : 0x9C00_A554)
    UINT32  RGST_Group0330_Reg22                  ; // 22  (ADDR : 0x9C00_A558)
    UINT32  RGST_Group0330_Reg23                  ; // 23  (ADDR : 0x9C00_A55C)
    UINT32  RGST_Group0330_Reg24                  ; // 24  (ADDR : 0x9C00_A560)
    UINT32  RGST_Group0330_Reg25                  ; // 25  (ADDR : 0x9C00_A564)
    UINT32  RGST_Group0330_Reg26                  ; // 26  (ADDR : 0x9C00_A568)
    UINT32  RGST_Group0330_Reg27                  ; // 27  (ADDR : 0x9C00_A56C)
    UINT32  RGST_Group0330_Reg28                  ; // 28  (ADDR : 0x9C00_A570)
    UINT32  RGST_Group0330_Reg29                  ; // 29  (ADDR : 0x9C00_A574)
    UINT32  RGST_Group0330_Reg30                  ; // 30  (ADDR : 0x9C00_A578)
    UINT32  RGST_Group0330_Reg31                  ; // 31  (ADDR : 0x9C00_A57C)

    //Group   331 : TVDEC
    UINT32  RGST_Group0331_Reg00                  ; // 00  (ADDR : 0x9C00_A580)
    UINT32  RGST_Group0331_Reg01                  ; // 01  (ADDR : 0x9C00_A584)
    UINT32  RGST_Group0331_Reg02                  ; // 02  (ADDR : 0x9C00_A588)
    UINT32  RGST_Group0331_Reg03                  ; // 03  (ADDR : 0x9C00_A58C)
    UINT32  RGST_Group0331_Reg04                  ; // 04  (ADDR : 0x9C00_A590)
    UINT32  RGST_Group0331_Reg05                  ; // 05  (ADDR : 0x9C00_A594)
    UINT32  RGST_Group0331_Reg06                  ; // 06  (ADDR : 0x9C00_A598)
    UINT32  RGST_Group0331_Reg07                  ; // 07  (ADDR : 0x9C00_A59C)
    UINT32  RGST_Group0331_Reg08                  ; // 08  (ADDR : 0x9C00_A5A0)
    UINT32  RGST_Group0331_Reg09                  ; // 09  (ADDR : 0x9C00_A5A4)
    UINT32  RGST_Group0331_Reg10                  ; // 10  (ADDR : 0x9C00_A5A8)
    UINT32  RGST_Group0331_Reg11                  ; // 11  (ADDR : 0x9C00_A5AC)
    UINT32  RGST_Group0331_Reg12                  ; // 12  (ADDR : 0x9C00_A5B0)
    UINT32  RGST_Group0331_Reg13                  ; // 13  (ADDR : 0x9C00_A5B4)
    UINT32  RGST_Group0331_Reg14                  ; // 14  (ADDR : 0x9C00_A5B8)
    UINT32  RGST_Group0331_Reg15                  ; // 15  (ADDR : 0x9C00_A5BC)
    UINT32  RGST_Group0331_Reg16                  ; // 16  (ADDR : 0x9C00_A5C0)
    UINT32  RGST_Group0331_Reg17                  ; // 17  (ADDR : 0x9C00_A5C4)
    UINT32  RGST_Group0331_Reg18                  ; // 18  (ADDR : 0x9C00_A5C8)
    UINT32  RGST_Group0331_Reg19                  ; // 19  (ADDR : 0x9C00_A5CC)
    UINT32  RGST_Group0331_Reg20                  ; // 20  (ADDR : 0x9C00_A5D0)
    UINT32  RGST_Group0331_Reg21                  ; // 21  (ADDR : 0x9C00_A5D4)
    UINT32  RGST_Group0331_Reg22                  ; // 22  (ADDR : 0x9C00_A5D8)
    UINT32  RGST_Group0331_Reg23                  ; // 23  (ADDR : 0x9C00_A5DC)
    UINT32  RGST_Group0331_Reg24                  ; // 24  (ADDR : 0x9C00_A5E0)
    UINT32  RGST_Group0331_Reg25                  ; // 25  (ADDR : 0x9C00_A5E4)
    UINT32  RGST_Group0331_Reg26                  ; // 26  (ADDR : 0x9C00_A5E8)
    UINT32  RGST_Group0331_Reg27                  ; // 27  (ADDR : 0x9C00_A5EC)
    UINT32  RGST_Group0331_Reg28                  ; // 28  (ADDR : 0x9C00_A5F0)
    UINT32  RGST_Group0331_Reg29                  ; // 29  (ADDR : 0x9C00_A5F4)
    UINT32  RGST_Group0331_Reg30                  ; // 30  (ADDR : 0x9C00_A5F8)
    UINT32  RGST_Group0331_Reg31                  ; // 31  (ADDR : 0x9C00_A5FC)

    //Group   332 : TVDEC
    UINT32  RGST_Group0332_Reg00                  ; // 00  (ADDR : 0x9C00_A600)
    UINT32  RGST_Group0332_Reg01                  ; // 01  (ADDR : 0x9C00_A604)
    UINT32  RGST_Group0332_Reg02                  ; // 02  (ADDR : 0x9C00_A608)
    UINT32  RGST_Group0332_Reg03                  ; // 03  (ADDR : 0x9C00_A60C)
    UINT32  RGST_Group0332_Reg04                  ; // 04  (ADDR : 0x9C00_A610)
    UINT32  RGST_Group0332_Reg05                  ; // 05  (ADDR : 0x9C00_A614)
    UINT32  RGST_Group0332_Reg06                  ; // 06  (ADDR : 0x9C00_A618)
    UINT32  RGST_Group0332_Reg07                  ; // 07  (ADDR : 0x9C00_A61C)
    UINT32  RGST_Group0332_Reg08                  ; // 08  (ADDR : 0x9C00_A620)
    UINT32  RGST_Group0332_Reg09                  ; // 09  (ADDR : 0x9C00_A624)
    UINT32  RGST_Group0332_Reg10                  ; // 10  (ADDR : 0x9C00_A628)
    UINT32  RGST_Group0332_Reg11                  ; // 11  (ADDR : 0x9C00_A62C)
    UINT32  RGST_Group0332_Reg12                  ; // 12  (ADDR : 0x9C00_A630)
    UINT32  RGST_Group0332_Reg13                  ; // 13  (ADDR : 0x9C00_A634)
    UINT32  RGST_Group0332_Reg14                  ; // 14  (ADDR : 0x9C00_A638)
    UINT32  RGST_Group0332_Reg15                  ; // 15  (ADDR : 0x9C00_A63C)
    UINT32  RGST_Group0332_Reg16                  ; // 16  (ADDR : 0x9C00_A640)
    UINT32  RGST_Group0332_Reg17                  ; // 17  (ADDR : 0x9C00_A644)
    UINT32  RGST_Group0332_Reg18                  ; // 18  (ADDR : 0x9C00_A648)
    UINT32  RGST_Group0332_Reg19                  ; // 19  (ADDR : 0x9C00_A64C)
    UINT32  RGST_Group0332_Reg20                  ; // 20  (ADDR : 0x9C00_A650)
    UINT32  RGST_Group0332_Reg21                  ; // 21  (ADDR : 0x9C00_A654)
    UINT32  RGST_Group0332_Reg22                  ; // 22  (ADDR : 0x9C00_A658)
    UINT32  RGST_Group0332_Reg23                  ; // 23  (ADDR : 0x9C00_A65C)
    UINT32  RGST_Group0332_Reg24                  ; // 24  (ADDR : 0x9C00_A660)
    UINT32  RGST_Group0332_Reg25                  ; // 25  (ADDR : 0x9C00_A664)
    UINT32  RGST_Group0332_Reg26                  ; // 26  (ADDR : 0x9C00_A668)
    UINT32  RGST_Group0332_Reg27                  ; // 27  (ADDR : 0x9C00_A66C)
    UINT32  RGST_Group0332_Reg28                  ; // 28  (ADDR : 0x9C00_A670)
    UINT32  RGST_Group0332_Reg29                  ; // 29  (ADDR : 0x9C00_A674)
    UINT32  RGST_Group0332_Reg30                  ; // 30  (ADDR : 0x9C00_A678)
    UINT32  RGST_Group0332_Reg31                  ; // 31  (ADDR : 0x9C00_A67C)

    //Group   333 : TVDEC
    UINT32  RGST_Group0333_Reg00                  ; // 00  (ADDR : 0x9C00_A680)
    UINT32  RGST_Group0333_Reg01                  ; // 01  (ADDR : 0x9C00_A684)
    UINT32  RGST_Group0333_Reg02                  ; // 02  (ADDR : 0x9C00_A688)
    UINT32  RGST_Group0333_Reg03                  ; // 03  (ADDR : 0x9C00_A68C)
    UINT32  RGST_Group0333_Reg04                  ; // 04  (ADDR : 0x9C00_A690)
    UINT32  RGST_Group0333_Reg05                  ; // 05  (ADDR : 0x9C00_A694)
    UINT32  RGST_Group0333_Reg06                  ; // 06  (ADDR : 0x9C00_A698)
    UINT32  RGST_Group0333_Reg07                  ; // 07  (ADDR : 0x9C00_A69C)
    UINT32  RGST_Group0333_Reg08                  ; // 08  (ADDR : 0x9C00_A6A0)
    UINT32  RGST_Group0333_Reg09                  ; // 09  (ADDR : 0x9C00_A6A4)
    UINT32  RGST_Group0333_Reg10                  ; // 10  (ADDR : 0x9C00_A6A8)
    UINT32  RGST_Group0333_Reg11                  ; // 11  (ADDR : 0x9C00_A6AC)
    UINT32  RGST_Group0333_Reg12                  ; // 12  (ADDR : 0x9C00_A6B0)
    UINT32  RGST_Group0333_Reg13                  ; // 13  (ADDR : 0x9C00_A6B4)
    UINT32  RGST_Group0333_Reg14                  ; // 14  (ADDR : 0x9C00_A6B8)
    UINT32  RGST_Group0333_Reg15                  ; // 15  (ADDR : 0x9C00_A6BC)
    UINT32  RGST_Group0333_Reg16                  ; // 16  (ADDR : 0x9C00_A6C0)
    UINT32  RGST_Group0333_Reg17                  ; // 17  (ADDR : 0x9C00_A6C4)
    UINT32  RGST_Group0333_Reg18                  ; // 18  (ADDR : 0x9C00_A6C8)
    UINT32  RGST_Group0333_Reg19                  ; // 19  (ADDR : 0x9C00_A6CC)
    UINT32  RGST_Group0333_Reg20                  ; // 20  (ADDR : 0x9C00_A6D0)
    UINT32  RGST_Group0333_Reg21                  ; // 21  (ADDR : 0x9C00_A6D4)
    UINT32  RGST_Group0333_Reg22                  ; // 22  (ADDR : 0x9C00_A6D8)
    UINT32  RGST_Group0333_Reg23                  ; // 23  (ADDR : 0x9C00_A6DC)
    UINT32  RGST_Group0333_Reg24                  ; // 24  (ADDR : 0x9C00_A6E0)
    UINT32  RGST_Group0333_Reg25                  ; // 25  (ADDR : 0x9C00_A6E4)
    UINT32  RGST_Group0333_Reg26                  ; // 26  (ADDR : 0x9C00_A6E8)
    UINT32  RGST_Group0333_Reg27                  ; // 27  (ADDR : 0x9C00_A6EC)
    UINT32  RGST_Group0333_Reg28                  ; // 28  (ADDR : 0x9C00_A6F0)
    UINT32  RGST_Group0333_Reg29                  ; // 29  (ADDR : 0x9C00_A6F4)
    UINT32  RGST_Group0333_Reg30                  ; // 30  (ADDR : 0x9C00_A6F8)
    UINT32  RGST_Group0333_Reg31                  ; // 31  (ADDR : 0x9C00_A6FC)

    //Group   334 : TVDEC
    UINT32  RGST_Group0334_Reg00                  ; // 00  (ADDR : 0x9C00_A700)
    UINT32  RGST_Group0334_Reg01                  ; // 01  (ADDR : 0x9C00_A704)
    UINT32  RGST_Group0334_Reg02                  ; // 02  (ADDR : 0x9C00_A708)
    UINT32  RGST_Group0334_Reg03                  ; // 03  (ADDR : 0x9C00_A70C)
    UINT32  RGST_Group0334_Reg04                  ; // 04  (ADDR : 0x9C00_A710)
    UINT32  RGST_Group0334_Reg05                  ; // 05  (ADDR : 0x9C00_A714)
    UINT32  RGST_Group0334_Reg06                  ; // 06  (ADDR : 0x9C00_A718)
    UINT32  RGST_Group0334_Reg07                  ; // 07  (ADDR : 0x9C00_A71C)
    UINT32  RGST_Group0334_Reg08                  ; // 08  (ADDR : 0x9C00_A720)
    UINT32  RGST_Group0334_Reg09                  ; // 09  (ADDR : 0x9C00_A724)
    UINT32  RGST_Group0334_Reg10                  ; // 10  (ADDR : 0x9C00_A728)
    UINT32  RGST_Group0334_Reg11                  ; // 11  (ADDR : 0x9C00_A72C)
    UINT32  RGST_Group0334_Reg12                  ; // 12  (ADDR : 0x9C00_A730)
    UINT32  RGST_Group0334_Reg13                  ; // 13  (ADDR : 0x9C00_A734)
    UINT32  RGST_Group0334_Reg14                  ; // 14  (ADDR : 0x9C00_A738)
    UINT32  RGST_Group0334_Reg15                  ; // 15  (ADDR : 0x9C00_A73C)
    UINT32  RGST_Group0334_Reg16                  ; // 16  (ADDR : 0x9C00_A740)
    UINT32  RGST_Group0334_Reg17                  ; // 17  (ADDR : 0x9C00_A744)
    UINT32  RGST_Group0334_Reg18                  ; // 18  (ADDR : 0x9C00_A748)
    UINT32  RGST_Group0334_Reg19                  ; // 19  (ADDR : 0x9C00_A74C)
    UINT32  RGST_Group0334_Reg20                  ; // 20  (ADDR : 0x9C00_A750)
    UINT32  RGST_Group0334_Reg21                  ; // 21  (ADDR : 0x9C00_A754)
    UINT32  RGST_Group0334_Reg22                  ; // 22  (ADDR : 0x9C00_A758)
    UINT32  RGST_Group0334_Reg23                  ; // 23  (ADDR : 0x9C00_A75C)
    UINT32  RGST_Group0334_Reg24                  ; // 24  (ADDR : 0x9C00_A760)
    UINT32  RGST_Group0334_Reg25                  ; // 25  (ADDR : 0x9C00_A764)
    UINT32  RGST_Group0334_Reg26                  ; // 26  (ADDR : 0x9C00_A768)
    UINT32  RGST_Group0334_Reg27                  ; // 27  (ADDR : 0x9C00_A76C)
    UINT32  RGST_Group0334_Reg28                  ; // 28  (ADDR : 0x9C00_A770)
    UINT32  RGST_Group0334_Reg29                  ; // 29  (ADDR : 0x9C00_A774)
    UINT32  RGST_Group0334_Reg30                  ; // 30  (ADDR : 0x9C00_A778)
    UINT32  RGST_Group0334_Reg31                  ; // 31  (ADDR : 0x9C00_A77C)

    //Group   335 : Reserved
    UINT32  RGST_Group0335_Reserved[32]           ; //     (ADDR : 0x9C00_A780) ~ (ADDR : 0x9C00_A7FC)

    //Group   336 : CVBS_ADC
    UINT32  RGST_Group0336_Reg00                  ; // 00  (ADDR : 0x9C00_A800)
    UINT32  RGST_Group0336_Reg01                  ; // 01  (ADDR : 0x9C00_A804)
    UINT32  RGST_Group0336_Reg02                  ; // 02  (ADDR : 0x9C00_A808)
    UINT32  RGST_Group0336_Reg03                  ; // 03  (ADDR : 0x9C00_A80C)
    UINT32  RGST_Group0336_Reg04                  ; // 04  (ADDR : 0x9C00_A810)
    UINT32  RGST_Group0336_Reg05                  ; // 05  (ADDR : 0x9C00_A814)
    UINT32  RGST_Group0336_Reg06                  ; // 06  (ADDR : 0x9C00_A818)
    UINT32  RGST_Group0336_Reg07                  ; // 07  (ADDR : 0x9C00_A81C)
    UINT32  RGST_Group0336_Reg08                  ; // 08  (ADDR : 0x9C00_A820)
    UINT32  RGST_Group0336_Reg09                  ; // 09  (ADDR : 0x9C00_A824)
    UINT32  RGST_Group0336_Reg10                  ; // 10  (ADDR : 0x9C00_A828)
    UINT32  RGST_Group0336_Reg11                  ; // 11  (ADDR : 0x9C00_A82C)
    UINT32  RGST_Group0336_Reg12                  ; // 12  (ADDR : 0x9C00_A830)
    UINT32  RGST_Group0336_Reg13                  ; // 13  (ADDR : 0x9C00_A834)
    UINT32  RGST_Group0336_Reg14                  ; // 14  (ADDR : 0x9C00_A838)
    UINT32  RGST_Group0336_Reg15                  ; // 15  (ADDR : 0x9C00_A83C)
    UINT32  RGST_Group0336_Reg16                  ; // 16  (ADDR : 0x9C00_A840)
    UINT32  RGST_Group0336_Reg17                  ; // 17  (ADDR : 0x9C00_A844)
    UINT32  RGST_Group0336_Reg18                  ; // 18  (ADDR : 0x9C00_A848)
    UINT32  RGST_Group0336_Reg19                  ; // 19  (ADDR : 0x9C00_A84C)
    UINT32  RGST_Group0336_Reg20                  ; // 20  (ADDR : 0x9C00_A850)
    UINT32  RGST_Group0336_Reg21                  ; // 21  (ADDR : 0x9C00_A854)
    UINT32  RGST_Group0336_Reg22                  ; // 22  (ADDR : 0x9C00_A858)
    UINT32  RGST_Group0336_Reg23                  ; // 23  (ADDR : 0x9C00_A85C)
    UINT32  RGST_Group0336_Reg24                  ; // 24  (ADDR : 0x9C00_A860)
    UINT32  RGST_Group0336_Reg25                  ; // 25  (ADDR : 0x9C00_A864)
    UINT32  RGST_Group0336_Reg26                  ; // 26  (ADDR : 0x9C00_A868)
    UINT32  RGST_Group0336_Reg27                  ; // 27  (ADDR : 0x9C00_A86C)
    UINT32  RGST_Group0336_Reg28                  ; // 28  (ADDR : 0x9C00_A870)
    UINT32  RGST_Group0336_Reg29                  ; // 29  (ADDR : 0x9C00_A874)
    UINT32  RGST_Group0336_Reg30                  ; // 30  (ADDR : 0x9C00_A878)
    UINT32  RGST_Group0336_Reg31                  ; // 31  (ADDR : 0x9C00_A87C)

    //Group   337 ~ 349 : Reserved
    UINT32  RGST_Group0337_Reserved[32]           ; //     (ADDR : 0x9C00_A880) ~ (ADDR : 0x9C00_A8FC)
    UINT32  RGST_Group0338_Reserved[32]           ; //     (ADDR : 0x9C00_A900) ~ (ADDR : 0x9C00_A97C)
    UINT32  RGST_Group0339_Reserved[32]           ; //     (ADDR : 0x9C00_A980) ~ (ADDR : 0x9C00_A9FC)
    UINT32  RGST_Group0340_Reserved[32]           ; //     (ADDR : 0x9C00_AA00) ~ (ADDR : 0x9C00_AA7C)
    UINT32  RGST_Group0341_Reserved[32]           ; //     (ADDR : 0x9C00_AA80) ~ (ADDR : 0x9C00_AAFC)
    UINT32  RGST_Group0342_Reserved[32]           ; //     (ADDR : 0x9C00_AB00) ~ (ADDR : 0x9C00_AB7C)
    UINT32  RGST_Group0343_Reserved[32]           ; //     (ADDR : 0x9C00_AB80) ~ (ADDR : 0x9C00_ABFC)
    UINT32  RGST_Group0344_Reserved[32]           ; //     (ADDR : 0x9C00_AC00) ~ (ADDR : 0x9C00_AC7C)
    UINT32  RGST_Group0345_Reserved[32]           ; //     (ADDR : 0x9C00_AC80) ~ (ADDR : 0x9C00_ACFC)
    UINT32  RGST_Group0346_Reserved[32]           ; //     (ADDR : 0x9C00_AD00) ~ (ADDR : 0x9C00_AD7C)
    UINT32  RGST_Group0347_Reserved[32]           ; //     (ADDR : 0x9C00_AD80) ~ (ADDR : 0x9C00_ADFC)
    UINT32  RGST_Group0348_Reserved[32]           ; //     (ADDR : 0x9C00_AE00) ~ (ADDR : 0x9C00_AE7C)
    UINT32  RGST_Group0349_Reserved[32]           ; //     (ADDR : 0x9C00_AE80) ~ (ADDR : 0x9C00_AEFC)

    //Group   350 : HB_GPIO
    //UINT32  grp350[32];
    UINT32  RGST_Group0350_Reg00                  ; // 00  (ADDR : 0x9C00_AF00)
    UINT32  RGST_Group0350_Reg01                  ; // 01  (ADDR : 0x9C00_AF04)
    UINT32  RGST_Group0350_Reg02                  ; // 02  (ADDR : 0x9C00_AF08)
    UINT32  RGST_Group0350_Reg03                  ; // 03  (ADDR : 0x9C00_AF0C)
    UINT32  RGST_Group0350_Reg04                  ; // 04  (ADDR : 0x9C00_AF10)
    UINT32  RGST_Group0350_Reg05                  ; // 05  (ADDR : 0x9C00_AF14)
    UINT32  RGST_Group0350_Reg06                  ; // 06  (ADDR : 0x9C00_AF18)
    UINT32  RGST_Group0350_Reg07                  ; // 07  (ADDR : 0x9C00_AF1C)
    UINT32  RGST_Group0350_Reg08                  ; // 08  (ADDR : 0x9C00_AF20)
    UINT32  RGST_Group0350_Reg09                  ; // 09  (ADDR : 0x9C00_AF24)
    UINT32  RGST_Group0350_Reg10                  ; // 10  (ADDR : 0x9C00_AF28)
    UINT32  RGST_Group0350_Reg11                  ; // 11  (ADDR : 0x9C00_AF2C)
    UINT32  RGST_Group0350_Reg12                  ; // 12  (ADDR : 0x9C00_AF30)
    UINT32  RGST_Group0350_Reg13                  ; // 13  (ADDR : 0x9C00_AF34)
    UINT32  RGST_Group0350_Reg14                  ; // 14  (ADDR : 0x9C00_AF38)
    UINT32  RGST_Group0350_Reg15                  ; // 15  (ADDR : 0x9C00_AF3C)
    UINT32  RGST_Group0350_Reg16                  ; // 16  (ADDR : 0x9C00_AF40)
    UINT32  RGST_Group0350_Reg17                  ; // 17  (ADDR : 0x9C00_AF44)
    UINT32  RGST_Group0350_Reg18                  ; // 18  (ADDR : 0x9C00_AF48)
    UINT32  RGST_Group0350_Reg19                  ; // 19  (ADDR : 0x9C00_AF4C)
    UINT32  RGST_Group0350_Reg20                  ; // 20  (ADDR : 0x9C00_AF50)
    UINT32  RGST_Group0350_Reg21                  ; // 21  (ADDR : 0x9C00_AF54)
    UINT32  RGST_Group0350_Reg22                  ; // 22  (ADDR : 0x9C00_AF58)
    UINT32  RGST_Group0350_Reg23                  ; // 23  (ADDR : 0x9C00_AF5C)
    UINT32  RGST_Group0350_Reg24                  ; // 24  (ADDR : 0x9C00_AF60)
    UINT32  RGST_Group0350_Reg25                  ; // 25  (ADDR : 0x9C00_AF64)
    UINT32  RGST_Group0350_Reg26                  ; // 26  (ADDR : 0x9C00_AF68)
    UINT32  RGST_Group0350_Reg27                  ; // 27  (ADDR : 0x9C00_AF6C)
    UINT32  RGST_Group0350_Reg28                  ; // 28  (ADDR : 0x9C00_AF70)
    UINT32  RGST_Group0350_Reg29                  ; // 29  (ADDR : 0x9C00_AF74)
    UINT32  RGST_Group0350_Reg30                  ; // 30  (ADDR : 0x9C00_AF78)
    UINT32  RGST_Group0350_Reg31                  ; // 31  (ADDR : 0x9C00_AF7C)

    //Group   351 : OTPRX
    UINT32 grp351[16];
    UINT32  KEY_SRST;
    UINT32  OTP_CTRL;
    UINT32  OTP_CMD;
    UINT32  OTP_CMD_ST;
    UINT32  OTP_ADDR;
    UINT32  OTP_DATA;
    UINT32  FLASH_CMD;
    UINT32  FLASH_CMD_ST;
    UINT32  FLASH_DATAIN_ADDR;
    UINT32  FLASH_DATAIN;
    UINT32  FLASH_DATAOUT_ADDR;
    UINT32  FLASH_DATAOUT;
    UINT32  FLASH_PW;
   UINT32  RGST_Group0351_Reg29                  ; // 29  (ADDR : 0x9C00_AFF4)
   UINT32  RGST_Group0351_Reg30                  ; // 30  (ADDR : 0x9C00_AFF8)
   UINT32  RGST_Group0351_Reg31                  ; // 31  (ADDR : 0x9C00_AFFC)


    //Group   352 : M2S_SDIF (Bridge)
    UINT32  rf_sdramif_tbya;                       // 00    (ADDR : 0x9C00_B000)
    UINT32  rf_servo_band_en;                      // 01   (ADDR : 0x9C00_B004)
    UINT32  rf_servo_band_val;                     // 02    (ADDR : 0x9C00_B008)
    UINT32  RGST_Group0352_Reg03                  ; // 03  (ADDR : 0x9C00_B00C)
    UINT32  RGST_Group0352_Reg04                  ; // 04  (ADDR : 0x9C00_B010)
    UINT32  RGST_Group0352_Reg05                  ; // 05  (ADDR : 0x9C00_B014)
    UINT32  RGST_Group0352_Reg06                  ; // 06  (ADDR : 0x9C00_B018)
    UINT32  RGST_Group0352_Reg07                  ; // 07  (ADDR : 0x9C00_B01C)
    UINT32  RGST_Group0352_Reg08                  ; // 08  (ADDR : 0x9C00_B020)
    UINT32  RGST_Group0352_Reg09                  ; // 09  (ADDR : 0x9C00_B024)
    UINT32  RGST_Group0352_Reg10                  ; // 10  (ADDR : 0x9C00_B028)
    UINT32  RGST_Group0352_Reg11                  ; // 11  (ADDR : 0x9C00_B02C)
    UINT32  RGST_Group0352_Reg12                  ; // 12  (ADDR : 0x9C00_B030)
    UINT32  RGST_Group0352_Reg13                  ; // 13  (ADDR : 0x9C00_B034)
    UINT32  RGST_Group0352_Reg14                  ; // 14  (ADDR : 0x9C00_B038)
    UINT32  RGST_Group0352_Reg15                  ; // 15  (ADDR : 0x9C00_B03C)
    UINT32  RGST_Group0352_Reg16                  ; // 16  (ADDR : 0x9C00_B040)
    UINT32  RGST_Group0352_Reg17                  ; // 17  (ADDR : 0x9C00_B044)
    UINT32  RGST_Group0352_Reg18                  ; // 18  (ADDR : 0x9C00_B048)
    UINT32  RGST_Group0352_Reg19                  ; // 19  (ADDR : 0x9C00_B04C)
    UINT32  RGST_Group0352_Reg20                  ; // 20  (ADDR : 0x9C00_B050)
    UINT32  RGST_Group0352_Reg21                  ; // 21  (ADDR : 0x9C00_B054)
    UINT32  RGST_Group0352_Reg22                  ; // 22  (ADDR : 0x9C00_B058)
    UINT32  RGST_Group0352_Reg23                  ; // 23  (ADDR : 0x9C00_B05C)
    UINT32  RGST_Group0352_Reg24                  ; // 24  (ADDR : 0x9C00_B060)
    UINT32  RGST_Group0352_Reg25                  ; // 25  (ADDR : 0x9C00_B064)
    UINT32  RGST_Group0352_Reg26                  ; // 26  (ADDR : 0x9C00_B068)
    UINT32  RGST_Group0352_Reg27                  ; // 27  (ADDR : 0x9C00_B06C)
    UINT32  RGST_Group0352_Reg28                  ; // 28  (ADDR : 0x9C00_B070)
    UINT32  RGST_Group0352_Reg29                  ; // 29  (ADDR : 0x9C00_B074)
    UINT32  RGST_Group0352_Reg30                  ; // 30  (ADDR : 0x9C00_B078)
    UINT32  RGST_Group0352_Reg31                  ; // 31  (ADDR : 0x9C00_B07C)

    //Group   353 : M2S_REGIF (Servo)
    UINT32 rf_regif_addr;                         // 00  (ADDR : 0x9C00_B080)
    UINT32 rf_regif_wdata;                        // 01  (ADDR : 0x9C00_B084)
    UINT32 rf_regif_rdata;                        // 02  (ADDR : 0x9C00_B087)
    UINT32 rf_regif_sample_ctrl;                  // 03  (ADDR : 0x9C00_B08C)
    UINT32 rf_regif_intr_addr;                    // 04  (ADDR : 0x9C00_B090)
    UINT32 rf_regif_intr_wdata;                   // 05  (ADDR : 0x9C00_B094)
    UINT32 rf_regif_intr_rdata;                   // 06  (ADDR : 0x9C00_B098)
    UINT32  RGST_Group0353_Reg07                  ; // 07  (ADDR : 0x9C00_B09C)
    UINT32  RGST_Group0353_Reg08                  ; // 08  (ADDR : 0x9C00_B0A0)
    UINT32  RGST_Group0353_Reg09                  ; // 09  (ADDR : 0x9C00_B0A4)
    UINT32  RGST_Group0353_Reg10                  ; // 10  (ADDR : 0x9C00_B0A8)
    UINT32  RGST_Group0353_Reg11                  ; // 11  (ADDR : 0x9C00_B0AC)
    UINT32  RGST_Group0353_Reg12                  ; // 12  (ADDR : 0x9C00_B0B0)
    UINT32  RGST_Group0353_Reg13                  ; // 13  (ADDR : 0x9C00_B0B4)
    UINT32  RGST_Group0353_Reg14                  ; // 14  (ADDR : 0x9C00_B0B8)
    UINT32  RGST_Group0353_Reg15                  ; // 15  (ADDR : 0x9C00_B0BC)
    UINT32  RGST_Group0353_Reg16                  ; // 16  (ADDR : 0x9C00_B0C0)
    UINT32  RGST_Group0353_Reg17                  ; // 17  (ADDR : 0x9C00_B0C4)
    UINT32  RGST_Group0353_Reg18                  ; // 18  (ADDR : 0x9C00_B0C8)
    UINT32  RGST_Group0353_Reg19                  ; // 19  (ADDR : 0x9C00_B0CC)
    UINT32  RGST_Group0353_Reg20                  ; // 20  (ADDR : 0x9C00_B0D0)
    UINT32  RGST_Group0353_Reg21                  ; // 21  (ADDR : 0x9C00_B0D4)
    UINT32  RGST_Group0353_Reg22                  ; // 22  (ADDR : 0x9C00_B0D8)
    UINT32  RGST_Group0353_Reg23                  ; // 23  (ADDR : 0x9C00_B0DC)
    UINT32  RGST_Group0353_Reg24                  ; // 24  (ADDR : 0x9C00_B0E0)
    UINT32  RGST_Group0353_Reg25                  ; // 25  (ADDR : 0x9C00_B0E4)
    UINT32  RGST_Group0353_Reg26                  ; // 26  (ADDR : 0x9C00_B0E8)
    UINT32  RGST_Group0353_Reg27                  ; // 27  (ADDR : 0x9C00_B0EC)
    UINT32  RGST_Group0353_Reg28                  ; // 28  (ADDR : 0x9C00_B0F0)
    UINT32  RGST_Group0353_Reg29                  ; // 29  (ADDR : 0x9C00_B0F4)
    UINT32  RGST_Group0353_Reg30                  ; // 30  (ADDR : 0x9C00_B0F8)
    UINT32  RGST_Group0353_Reg31                  ; // 31  (ADDR : 0x9C00_B0FC)

    //Group   354 ~ 383 : Reserved
    UINT32  RGST_Group0354_Reserved[32]           ; //     (ADDR : 0x9C00_B100) ~ (ADDR : 0x9C00_B17C)
    UINT32  RGST_Group0355_Reserved[32]           ; //     (ADDR : 0x9C00_B180) ~ (ADDR : 0x9C00_B1FC)
    UINT32  RGST_Group0356_Reserved[32]           ; //     (ADDR : 0x9C00_B200) ~ (ADDR : 0x9C00_B27C)
    UINT32  RGST_Group0357_Reserved[32]           ; //     (ADDR : 0x9C00_B280) ~ (ADDR : 0x9C00_B2FC)
    UINT32  RGST_Group0358_Reserved[32]           ; //     (ADDR : 0x9C00_B300) ~ (ADDR : 0x9C00_B37C)
    UINT32  RGST_Group0359_Reserved[32]           ; //     (ADDR : 0x9C00_B380) ~ (ADDR : 0x9C00_B3FC)
    UINT32  RGST_Group0360_Reserved[32]           ; //     (ADDR : 0x9C00_B400) ~ (ADDR : 0x9C00_B47C)
    UINT32  RGST_Group0361_Reserved[32]           ; //     (ADDR : 0x9C00_B480) ~ (ADDR : 0x9C00_B4FC)
    UINT32  RGST_Group0362_Reserved[32]           ; //     (ADDR : 0x9C00_B500) ~ (ADDR : 0x9C00_B57C)
    UINT32  RGST_Group0363_Reserved[32]           ; //     (ADDR : 0x9C00_B580) ~ (ADDR : 0x9C00_B5FC)
    UINT32  RGST_Group0364_Reserved[32]           ; //     (ADDR : 0x9C00_B600) ~ (ADDR : 0x9C00_B67C)
    UINT32  RGST_Group0365_Reserved[32]           ; //     (ADDR : 0x9C00_B680) ~ (ADDR : 0x9C00_B6FC)
    UINT32  RGST_Group0366_Reserved[32]           ; //     (ADDR : 0x9C00_B700) ~ (ADDR : 0x9C00_B77C)
    UINT32  RGST_Group0367_Reserved[32]           ; //     (ADDR : 0x9C00_B780) ~ (ADDR : 0x9C00_B7FC)
    UINT32  RGST_Group0368_Reserved[32]           ; //     (ADDR : 0x9C00_B800) ~ (ADDR : 0x9C00_B87C)
    UINT32  RGST_Group0369_Reserved[32]           ; //     (ADDR : 0x9C00_B880) ~ (ADDR : 0x9C00_B8FC)
    UINT32  RGST_Group0370_Reserved[32]           ; //     (ADDR : 0x9C00_B900) ~ (ADDR : 0x9C00_B97C)
    UINT32  RGST_Group0371_Reserved[32]           ; //     (ADDR : 0x9C00_B980) ~ (ADDR : 0x9C00_B9FC)
    UINT32  RGST_Group0372_Reserved[32]           ; //     (ADDR : 0x9C00_BA00) ~ (ADDR : 0x9C00_BA7C)
    UINT32  RGST_Group0373_Reserved[32]           ; //     (ADDR : 0x9C00_BA80) ~ (ADDR : 0x9C00_BAFC)
    UINT32  RGST_Group0374_Reserved[32]           ; //     (ADDR : 0x9C00_BB00) ~ (ADDR : 0x9C00_BB7C)
    UINT32  RGST_Group0375_Reserved[32]           ; //     (ADDR : 0x9C00_BB80) ~ (ADDR : 0x9C00_BBFC)
    UINT32  RGST_Group0376_Reserved[32]           ; //     (ADDR : 0x9C00_BC00) ~ (ADDR : 0x9C00_BC7C)
    UINT32  RGST_Group0377_Reserved[32]           ; //     (ADDR : 0x9C00_BC80) ~ (ADDR : 0x9C00_BCFC)
    UINT32  RGST_Group0378_Reserved[32]           ; //     (ADDR : 0x9C00_BD00) ~ (ADDR : 0x9C00_BD7C)
    UINT32  RGST_Group0379_Reserved[32]           ; //     (ADDR : 0x9C00_BD80) ~ (ADDR : 0x9C00_BDFC)
    UINT32  RGST_Group0380_Reserved[32]           ; //     (ADDR : 0x9C00_BE00) ~ (ADDR : 0x9C00_BE7C)
    UINT32  RGST_Group0381_Reserved[32]           ; //     (ADDR : 0x9C00_BE80) ~ (ADDR : 0x9C00_BEFC)
    UINT32  RGST_Group0382_Reserved[32]           ; //     (ADDR : 0x9C00_BF00) ~ (ADDR : 0x9C00_BF7C)
    UINT32  RGST_Group0383_Reserved[32]           ; //     (ADDR : 0x9C00_BF80) ~ (ADDR : 0x9C00_BFFC)

    //Group   384 : MFD
    UINT32  RGST_Group0384_Reg00                  ; // 00  (ADDR : 0x9C00_C000)
    UINT32  RGST_Group0384_Reg01                  ; // 01  (ADDR : 0x9C00_C004)
    UINT32  RGST_Group0384_Reg02                  ; // 02  (ADDR : 0x9C00_C008)
    UINT32  RGST_Group0384_Reg03                  ; // 03  (ADDR : 0x9C00_C00C)
    UINT32  RGST_Group0384_Reg04                  ; // 04  (ADDR : 0x9C00_C010)
    UINT32  RGST_Group0384_Reg05                  ; // 05  (ADDR : 0x9C00_C014)
    UINT32  RGST_Group0384_Reg06                  ; // 06  (ADDR : 0x9C00_C018)
    UINT32  RGST_Group0384_Reg07                  ; // 07  (ADDR : 0x9C00_C01C)
    UINT32  RGST_Group0384_Reg08                  ; // 08  (ADDR : 0x9C00_C020)
    UINT32  RGST_Group0384_Reg09                  ; // 09  (ADDR : 0x9C00_C024)
    UINT32  RGST_Group0384_Reg10                  ; // 10  (ADDR : 0x9C00_C028)
    UINT32  RGST_Group0384_Reg11                  ; // 11  (ADDR : 0x9C00_C02C)
    UINT32  RGST_Group0384_Reg12                  ; // 12  (ADDR : 0x9C00_C030)
    UINT32  RGST_Group0384_Reg13                  ; // 13  (ADDR : 0x9C00_C034)
    UINT32  RGST_Group0384_Reg14                  ; // 14  (ADDR : 0x9C00_C038)
    UINT32  RGST_Group0384_Reg15                  ; // 15  (ADDR : 0x9C00_C03C)
    UINT32  RGST_Group0384_Reg16                  ; // 16  (ADDR : 0x9C00_C040)
    UINT32  RGST_Group0384_Reg17                  ; // 17  (ADDR : 0x9C00_C044)
    UINT32  RGST_Group0384_Reg18                  ; // 18  (ADDR : 0x9C00_C048)
    UINT32  RGST_Group0384_Reg19                  ; // 19  (ADDR : 0x9C00_C04C)
    UINT32  RGST_Group0384_Reg20                  ; // 20  (ADDR : 0x9C00_C050)
    UINT32  RGST_Group0384_Reg21                  ; // 21  (ADDR : 0x9C00_C054)
    UINT32  RGST_Group0384_Reg22                  ; // 22  (ADDR : 0x9C00_C058)
    UINT32  RGST_Group0384_Reg23                  ; // 23  (ADDR : 0x9C00_C05C)
    UINT32  RGST_Group0384_Reg24                  ; // 24  (ADDR : 0x9C00_C060)
    UINT32  RGST_Group0384_Reg25                  ; // 25  (ADDR : 0x9C00_C064)
    UINT32  RGST_Group0384_Reg26                  ; // 26  (ADDR : 0x9C00_C068)
    UINT32  RGST_Group0384_Reg27                  ; // 27  (ADDR : 0x9C00_C06C)
    UINT32  RGST_Group0384_Reg28                  ; // 28  (ADDR : 0x9C00_C070)
    UINT32  RGST_Group0384_Reg29                  ; // 29  (ADDR : 0x9C00_C074)
    UINT32  RGST_Group0384_Reg30                  ; // 30  (ADDR : 0x9C00_C078)
    UINT32  RGST_Group0384_Reg31                  ; // 31  (ADDR : 0x9C00_C07C)

    //Group   385 : MFD
    UINT32  RGST_Group0385_Reg00                  ; // 00  (ADDR : 0x9C00_C080)
    UINT32  RGST_Group0385_Reg01                  ; // 01  (ADDR : 0x9C00_C084)
    UINT32  RGST_Group0385_Reg02                  ; // 02  (ADDR : 0x9C00_C088)
    UINT32  RGST_Group0385_Reg03                  ; // 03  (ADDR : 0x9C00_C08C)
    UINT32  RGST_Group0385_Reg04                  ; // 04  (ADDR : 0x9C00_C090)
    UINT32  RGST_Group0385_Reg05                  ; // 05  (ADDR : 0x9C00_C094)
    UINT32  RGST_Group0385_Reg06                  ; // 06  (ADDR : 0x9C00_C098)
    UINT32  RGST_Group0385_Reg07                  ; // 07  (ADDR : 0x9C00_C09C)
    UINT32  RGST_Group0385_Reg08                  ; // 08  (ADDR : 0x9C00_C0A0)
    UINT32  RGST_Group0385_Reg09                  ; // 09  (ADDR : 0x9C00_C0A4)
    UINT32  RGST_Group0385_Reg10                  ; // 10  (ADDR : 0x9C00_C0A8)
    UINT32  RGST_Group0385_Reg11                  ; // 11  (ADDR : 0x9C00_C0AC)
    UINT32  RGST_Group0385_Reg12                  ; // 12  (ADDR : 0x9C00_C0B0)
    UINT32  RGST_Group0385_Reg13                  ; // 13  (ADDR : 0x9C00_C0B4)
    UINT32  RGST_Group0385_Reg14                  ; // 14  (ADDR : 0x9C00_C0B8)
    UINT32  RGST_Group0385_Reg15                  ; // 15  (ADDR : 0x9C00_C0BC)
    UINT32  RGST_Group0385_Reg16                  ; // 16  (ADDR : 0x9C00_C0C0)
    UINT32  RGST_Group0385_Reg17                  ; // 17  (ADDR : 0x9C00_C0C4)
    UINT32  RGST_Group0385_Reg18                  ; // 18  (ADDR : 0x9C00_C0C8)
    UINT32  RGST_Group0385_Reg19                  ; // 19  (ADDR : 0x9C00_C0CC)
    UINT32  RGST_Group0385_Reg20                  ; // 20  (ADDR : 0x9C00_C0D0)
    UINT32  RGST_Group0385_Reg21                  ; // 21  (ADDR : 0x9C00_C0D4)
    UINT32  RGST_Group0385_Reg22                  ; // 22  (ADDR : 0x9C00_C0D8)
    UINT32  RGST_Group0385_Reg23                  ; // 23  (ADDR : 0x9C00_C0DC)
    UINT32  RGST_Group0385_Reg24                  ; // 24  (ADDR : 0x9C00_C0E0)
    UINT32  RGST_Group0385_Reg25                  ; // 25  (ADDR : 0x9C00_C0E4)
    UINT32  RGST_Group0385_Reg26                  ; // 26  (ADDR : 0x9C00_C0E8)
    UINT32  RGST_Group0385_Reg27                  ; // 27  (ADDR : 0x9C00_C0EC)
    UINT32  RGST_Group0385_Reg28                  ; // 28  (ADDR : 0x9C00_C0F0)
    UINT32  RGST_Group0385_Reg29                  ; // 29  (ADDR : 0x9C00_C0F4)
    UINT32  RGST_Group0385_Reg30                  ; // 30  (ADDR : 0x9C00_C0F8)
    UINT32  RGST_Group0385_Reg31                  ; // 31  (ADDR : 0x9C00_C0FC)

    //Group   386 : MFD
    UINT32  RGST_Group0386_Reg00                  ; // 00  (ADDR : 0x9C00_C100)
    UINT32  RGST_Group0386_Reg01                  ; // 01  (ADDR : 0x9C00_C104)
    UINT32  RGST_Group0386_Reg02                  ; // 02  (ADDR : 0x9C00_C108)
    UINT32  RGST_Group0386_Reg03                  ; // 03  (ADDR : 0x9C00_C10C)
    UINT32  RGST_Group0386_Reg04                  ; // 04  (ADDR : 0x9C00_C110)
    UINT32  RGST_Group0386_Reg05                  ; // 05  (ADDR : 0x9C00_C114)
    UINT32  RGST_Group0386_Reg06                  ; // 06  (ADDR : 0x9C00_C118)
    UINT32  RGST_Group0386_Reg07                  ; // 07  (ADDR : 0x9C00_C11C)
    UINT32  RGST_Group0386_Reg08                  ; // 08  (ADDR : 0x9C00_C120)
    UINT32  RGST_Group0386_Reg09                  ; // 09  (ADDR : 0x9C00_C124)
    UINT32  RGST_Group0386_Reg10                  ; // 10  (ADDR : 0x9C00_C128)
    UINT32  RGST_Group0386_Reg11                  ; // 11  (ADDR : 0x9C00_C12C)
    UINT32  RGST_Group0386_Reg12                  ; // 12  (ADDR : 0x9C00_C130)
    UINT32  RGST_Group0386_Reg13                  ; // 13  (ADDR : 0x9C00_C134)
    UINT32  RGST_Group0386_Reg14                  ; // 14  (ADDR : 0x9C00_C138)
    UINT32  RGST_Group0386_Reg15                  ; // 15  (ADDR : 0x9C00_C13C)
    UINT32  RGST_Group0386_Reg16                  ; // 16  (ADDR : 0x9C00_C140)
    UINT32  RGST_Group0386_Reg17                  ; // 17  (ADDR : 0x9C00_C144)
    UINT32  RGST_Group0386_Reg18                  ; // 18  (ADDR : 0x9C00_C148)
    UINT32  RGST_Group0386_Reg19                  ; // 19  (ADDR : 0x9C00_C14C)
    UINT32  RGST_Group0386_Reg20                  ; // 20  (ADDR : 0x9C00_C150)
    UINT32  RGST_Group0386_Reg21                  ; // 21  (ADDR : 0x9C00_C154)
    UINT32  RGST_Group0386_Reg22                  ; // 22  (ADDR : 0x9C00_C158)
    UINT32  RGST_Group0386_Reg23                  ; // 23  (ADDR : 0x9C00_C15C)
    UINT32  RGST_Group0386_Reg24                  ; // 24  (ADDR : 0x9C00_C160)
    UINT32  RGST_Group0386_Reg25                  ; // 25  (ADDR : 0x9C00_C164)
    UINT32  RGST_Group0386_Reg26                  ; // 26  (ADDR : 0x9C00_C168)
    UINT32  RGST_Group0386_Reg27                  ; // 27  (ADDR : 0x9C00_C16C)
    UINT32  RGST_Group0386_Reg28                  ; // 28  (ADDR : 0x9C00_C170)
    UINT32  RGST_Group0386_Reg29                  ; // 29  (ADDR : 0x9C00_C174)
    UINT32  RGST_Group0386_Reg30                  ; // 30  (ADDR : 0x9C00_C178)
    UINT32  RGST_Group0386_Reg31                  ; // 31  (ADDR : 0x9C00_C17C)

    //Group   387 : MFD
    UINT32  RGST_Group0387_Reg00                  ; // 00  (ADDR : 0x9C00_C180)
    UINT32  RGST_Group0387_Reg01                  ; // 01  (ADDR : 0x9C00_C184)
    UINT32  RGST_Group0387_Reg02                  ; // 02  (ADDR : 0x9C00_C188)
    UINT32  RGST_Group0387_Reg03                  ; // 03  (ADDR : 0x9C00_C18C)
    UINT32  RGST_Group0387_Reg04                  ; // 04  (ADDR : 0x9C00_C190)
    UINT32  RGST_Group0387_Reg05                  ; // 05  (ADDR : 0x9C00_C194)
    UINT32  RGST_Group0387_Reg06                  ; // 06  (ADDR : 0x9C00_C198)
    UINT32  RGST_Group0387_Reg07                  ; // 07  (ADDR : 0x9C00_C19C)
    UINT32  RGST_Group0387_Reg08                  ; // 08  (ADDR : 0x9C00_C1A0)
    UINT32  RGST_Group0387_Reg09                  ; // 09  (ADDR : 0x9C00_C1A4)
    UINT32  RGST_Group0387_Reg10                  ; // 10  (ADDR : 0x9C00_C1A8)
    UINT32  RGST_Group0387_Reg11                  ; // 11  (ADDR : 0x9C00_C1AC)
    UINT32  RGST_Group0387_Reg12                  ; // 12  (ADDR : 0x9C00_C1B0)
    UINT32  RGST_Group0387_Reg13                  ; // 13  (ADDR : 0x9C00_C1B4)
    UINT32  RGST_Group0387_Reg14                  ; // 14  (ADDR : 0x9C00_C1B8)
    UINT32  RGST_Group0387_Reg15                  ; // 15  (ADDR : 0x9C00_C1BC)
    UINT32  RGST_Group0387_Reg16                  ; // 16  (ADDR : 0x9C00_C1C0)
    UINT32  RGST_Group0387_Reg17                  ; // 17  (ADDR : 0x9C00_C1C4)
    UINT32  RGST_Group0387_Reg18                  ; // 18  (ADDR : 0x9C00_C1C8)
    UINT32  RGST_Group0387_Reg19                  ; // 19  (ADDR : 0x9C00_C1CC)
    UINT32  RGST_Group0387_Reg20                  ; // 20  (ADDR : 0x9C00_C1D0)
    UINT32  RGST_Group0387_Reg21                  ; // 21  (ADDR : 0x9C00_C1D4)
    UINT32  RGST_Group0387_Reg22                  ; // 22  (ADDR : 0x9C00_C1D8)
    UINT32  RGST_Group0387_Reg23                  ; // 23  (ADDR : 0x9C00_C1DC)
    UINT32  RGST_Group0387_Reg24                  ; // 24  (ADDR : 0x9C00_C1E0)
    UINT32  RGST_Group0387_Reg25                  ; // 25  (ADDR : 0x9C00_C1E4)
    UINT32  RGST_Group0387_Reg26                  ; // 26  (ADDR : 0x9C00_C1E8)
    UINT32  RGST_Group0387_Reg27                  ; // 27  (ADDR : 0x9C00_C1EC)
    UINT32  RGST_Group0387_Reg28                  ; // 28  (ADDR : 0x9C00_C1F0)
    UINT32  RGST_Group0387_Reg29                  ; // 29  (ADDR : 0x9C00_C1F4)
    UINT32  RGST_Group0387_Reg30                  ; // 30  (ADDR : 0x9C00_C1F8)
    UINT32  RGST_Group0387_Reg31                  ; // 31  (ADDR : 0x9C00_C1FC)

    //Group   388 : MFD
    UINT32  RGST_Group0388_Reg00                  ; // 00  (ADDR : 0x9C00_C200)
    UINT32  RGST_Group0388_Reg01                  ; // 01  (ADDR : 0x9C00_C204)
    UINT32  RGST_Group0388_Reg02                  ; // 02  (ADDR : 0x9C00_C208)
    UINT32  RGST_Group0388_Reg03                  ; // 03  (ADDR : 0x9C00_C20C)
    UINT32  RGST_Group0388_Reg04                  ; // 04  (ADDR : 0x9C00_C210)
    UINT32  RGST_Group0388_Reg05                  ; // 05  (ADDR : 0x9C00_C214)
    UINT32  RGST_Group0388_Reg06                  ; // 06  (ADDR : 0x9C00_C218)
    UINT32  RGST_Group0388_Reg07                  ; // 07  (ADDR : 0x9C00_C21C)
    UINT32  RGST_Group0388_Reg08                  ; // 08  (ADDR : 0x9C00_C220)
    UINT32  RGST_Group0388_Reg09                  ; // 09  (ADDR : 0x9C00_C224)
    UINT32  RGST_Group0388_Reg10                  ; // 10  (ADDR : 0x9C00_C228)
    UINT32  RGST_Group0388_Reg11                  ; // 11  (ADDR : 0x9C00_C22C)
    UINT32  RGST_Group0388_Reg12                  ; // 12  (ADDR : 0x9C00_C230)
    UINT32  RGST_Group0388_Reg13                  ; // 13  (ADDR : 0x9C00_C234)
    UINT32  RGST_Group0388_Reg14                  ; // 14  (ADDR : 0x9C00_C238)
    UINT32  RGST_Group0388_Reg15                  ; // 15  (ADDR : 0x9C00_C23C)
    UINT32  RGST_Group0388_Reg16                  ; // 16  (ADDR : 0x9C00_C240)
    UINT32  RGST_Group0388_Reg17                  ; // 17  (ADDR : 0x9C00_C244)
    UINT32  RGST_Group0388_Reg18                  ; // 18  (ADDR : 0x9C00_C248)
    UINT32  RGST_Group0388_Reg19                  ; // 19  (ADDR : 0x9C00_C24C)
    UINT32  RGST_Group0388_Reg20                  ; // 20  (ADDR : 0x9C00_C250)
    UINT32  RGST_Group0388_Reg21                  ; // 21  (ADDR : 0x9C00_C254)
    UINT32  RGST_Group0388_Reg22                  ; // 22  (ADDR : 0x9C00_C258)
    UINT32  RGST_Group0388_Reg23                  ; // 23  (ADDR : 0x9C00_C25C)
    UINT32  RGST_Group0388_Reg24                  ; // 24  (ADDR : 0x9C00_C260)
    UINT32  RGST_Group0388_Reg25                  ; // 25  (ADDR : 0x9C00_C264)
    UINT32  RGST_Group0388_Reg26                  ; // 26  (ADDR : 0x9C00_C268)
    UINT32  RGST_Group0388_Reg27                  ; // 27  (ADDR : 0x9C00_C26C)
    UINT32  RGST_Group0388_Reg28                  ; // 28  (ADDR : 0x9C00_C270)
    UINT32  RGST_Group0388_Reg29                  ; // 29  (ADDR : 0x9C00_C274)
    UINT32  RGST_Group0388_Reg30                  ; // 30  (ADDR : 0x9C00_C278)
    UINT32  RGST_Group0388_Reg31                  ; // 31  (ADDR : 0x9C00_C27C)

    //Group   389 : MFD
    UINT32  RGST_Group0389_Reg00                  ; // 00  (ADDR : 0x9C00_C280)
    UINT32  RGST_Group0389_Reg01                  ; // 01  (ADDR : 0x9C00_C284)
    UINT32  RGST_Group0389_Reg02                  ; // 02  (ADDR : 0x9C00_C288)
    UINT32  RGST_Group0389_Reg03                  ; // 03  (ADDR : 0x9C00_C28C)
    UINT32  RGST_Group0389_Reg04                  ; // 04  (ADDR : 0x9C00_C290)
    UINT32  RGST_Group0389_Reg05                  ; // 05  (ADDR : 0x9C00_C294)
    UINT32  RGST_Group0389_Reg06                  ; // 06  (ADDR : 0x9C00_C298)
    UINT32  RGST_Group0389_Reg07                  ; // 07  (ADDR : 0x9C00_C29C)
    UINT32  RGST_Group0389_Reg08                  ; // 08  (ADDR : 0x9C00_C2A0)
    UINT32  RGST_Group0389_Reg09                  ; // 09  (ADDR : 0x9C00_C2A4)
    UINT32  RGST_Group0389_Reg10                  ; // 10  (ADDR : 0x9C00_C2A8)
    UINT32  RGST_Group0389_Reg11                  ; // 11  (ADDR : 0x9C00_C2AC)
    UINT32  RGST_Group0389_Reg12                  ; // 12  (ADDR : 0x9C00_C2B0)
    UINT32  RGST_Group0389_Reg13                  ; // 13  (ADDR : 0x9C00_C2B4)
    UINT32  RGST_Group0389_Reg14                  ; // 14  (ADDR : 0x9C00_C2B8)
    UINT32  RGST_Group0389_Reg15                  ; // 15  (ADDR : 0x9C00_C2BC)
    UINT32  RGST_Group0389_Reg16                  ; // 16  (ADDR : 0x9C00_C2C0)
    UINT32  RGST_Group0389_Reg17                  ; // 17  (ADDR : 0x9C00_C2C4)
    UINT32  RGST_Group0389_Reg18                  ; // 18  (ADDR : 0x9C00_C2C8)
    UINT32  RGST_Group0389_Reg19                  ; // 19  (ADDR : 0x9C00_C2CC)
    UINT32  RGST_Group0389_Reg20                  ; // 20  (ADDR : 0x9C00_C2D0)
    UINT32  RGST_Group0389_Reg21                  ; // 21  (ADDR : 0x9C00_C2D4)
    UINT32  RGST_Group0389_Reg22                  ; // 22  (ADDR : 0x9C00_C2D8)
    UINT32  RGST_Group0389_Reg23                  ; // 23  (ADDR : 0x9C00_C2DC)
    UINT32  RGST_Group0389_Reg24                  ; // 24  (ADDR : 0x9C00_C2E0)
    UINT32  RGST_Group0389_Reg25                  ; // 25  (ADDR : 0x9C00_C2E4)
    UINT32  RGST_Group0389_Reg26                  ; // 26  (ADDR : 0x9C00_C2E8)
    UINT32  RGST_Group0389_Reg27                  ; // 27  (ADDR : 0x9C00_C2EC)
    UINT32  RGST_Group0389_Reg28                  ; // 28  (ADDR : 0x9C00_C2F0)
    UINT32  RGST_Group0389_Reg29                  ; // 29  (ADDR : 0x9C00_C2F4)
    UINT32  RGST_Group0389_Reg30                  ; // 30  (ADDR : 0x9C00_C2F8)
    UINT32  RGST_Group0389_Reg31                  ; // 31  (ADDR : 0x9C00_C2FC)

    //Group   390 : MFD
    UINT32  RGST_Group0390_Reg00                  ; // 00  (ADDR : 0x9C00_C300)
    UINT32  RGST_Group0390_Reg01                  ; // 01  (ADDR : 0x9C00_C304)
    UINT32  RGST_Group0390_Reg02                  ; // 02  (ADDR : 0x9C00_C308)
    UINT32  RGST_Group0390_Reg03                  ; // 03  (ADDR : 0x9C00_C30C)
    UINT32  RGST_Group0390_Reg04                  ; // 04  (ADDR : 0x9C00_C310)
    UINT32  RGST_Group0390_Reg05                  ; // 05  (ADDR : 0x9C00_C314)
    UINT32  RGST_Group0390_Reg06                  ; // 06  (ADDR : 0x9C00_C318)
    UINT32  RGST_Group0390_Reg07                  ; // 07  (ADDR : 0x9C00_C31C)
    UINT32  RGST_Group0390_Reg08                  ; // 08  (ADDR : 0x9C00_C320)
    UINT32  RGST_Group0390_Reg09                  ; // 09  (ADDR : 0x9C00_C324)
    UINT32  RGST_Group0390_Reg10                  ; // 10  (ADDR : 0x9C00_C328)
    UINT32  RGST_Group0390_Reg11                  ; // 11  (ADDR : 0x9C00_C32C)
    UINT32  RGST_Group0390_Reg12                  ; // 12  (ADDR : 0x9C00_C330)
    UINT32  RGST_Group0390_Reg13                  ; // 13  (ADDR : 0x9C00_C334)
    UINT32  RGST_Group0390_Reg14                  ; // 14  (ADDR : 0x9C00_C338)
    UINT32  RGST_Group0390_Reg15                  ; // 15  (ADDR : 0x9C00_C33C)
    UINT32  RGST_Group0390_Reg16                  ; // 16  (ADDR : 0x9C00_C340)
    UINT32  RGST_Group0390_Reg17                  ; // 17  (ADDR : 0x9C00_C344)
    UINT32  RGST_Group0390_Reg18                  ; // 18  (ADDR : 0x9C00_C348)
    UINT32  RGST_Group0390_Reg19                  ; // 19  (ADDR : 0x9C00_C34C)
    UINT32  RGST_Group0390_Reg20                  ; // 20  (ADDR : 0x9C00_C350)
    UINT32  RGST_Group0390_Reg21                  ; // 21  (ADDR : 0x9C00_C354)
    UINT32  RGST_Group0390_Reg22                  ; // 22  (ADDR : 0x9C00_C358)
    UINT32  RGST_Group0390_Reg23                  ; // 23  (ADDR : 0x9C00_C35C)
    UINT32  RGST_Group0390_Reg24                  ; // 24  (ADDR : 0x9C00_C360)
    UINT32  RGST_Group0390_Reg25                  ; // 25  (ADDR : 0x9C00_C364)
    UINT32  RGST_Group0390_Reg26                  ; // 26  (ADDR : 0x9C00_C368)
    UINT32  RGST_Group0390_Reg27                  ; // 27  (ADDR : 0x9C00_C36C)
    UINT32  RGST_Group0390_Reg28                  ; // 28  (ADDR : 0x9C00_C370)
    UINT32  RGST_Group0390_Reg29                  ; // 29  (ADDR : 0x9C00_C374)
    UINT32  RGST_Group0390_Reg30                  ; // 30  (ADDR : 0x9C00_C378)
    UINT32  RGST_Group0390_Reg31                  ; // 31  (ADDR : 0x9C00_C37C)

    //Group   391 : MFD
    UINT32  RGST_Group0391_Reg00                  ; // 00  (ADDR : 0x9C00_C380)
    UINT32  RGST_Group0391_Reg01                  ; // 01  (ADDR : 0x9C00_C384)
    UINT32  RGST_Group0391_Reg02                  ; // 02  (ADDR : 0x9C00_C388)
    UINT32  RGST_Group0391_Reg03                  ; // 03  (ADDR : 0x9C00_C38C)
    UINT32  RGST_Group0391_Reg04                  ; // 04  (ADDR : 0x9C00_C390)
    UINT32  RGST_Group0391_Reg05                  ; // 05  (ADDR : 0x9C00_C394)
    UINT32  RGST_Group0391_Reg06                  ; // 06  (ADDR : 0x9C00_C398)
    UINT32  RGST_Group0391_Reg07                  ; // 07  (ADDR : 0x9C00_C39C)
    UINT32  RGST_Group0391_Reg08                  ; // 08  (ADDR : 0x9C00_C3A0)
    UINT32  RGST_Group0391_Reg09                  ; // 09  (ADDR : 0x9C00_C3A4)
    UINT32  RGST_Group0391_Reg10                  ; // 10  (ADDR : 0x9C00_C3A8)
    UINT32  RGST_Group0391_Reg11                  ; // 11  (ADDR : 0x9C00_C3AC)
    UINT32  RGST_Group0391_Reg12                  ; // 12  (ADDR : 0x9C00_C3B0)
    UINT32  RGST_Group0391_Reg13                  ; // 13  (ADDR : 0x9C00_C3B4)
    UINT32  RGST_Group0391_Reg14                  ; // 14  (ADDR : 0x9C00_C3B8)
    UINT32  RGST_Group0391_Reg15                  ; // 15  (ADDR : 0x9C00_C3BC)
    UINT32  RGST_Group0391_Reg16                  ; // 16  (ADDR : 0x9C00_C3C0)
    UINT32  RGST_Group0391_Reg17                  ; // 17  (ADDR : 0x9C00_C3C4)
    UINT32  RGST_Group0391_Reg18                  ; // 18  (ADDR : 0x9C00_C3C8)
    UINT32  RGST_Group0391_Reg19                  ; // 19  (ADDR : 0x9C00_C3CC)
    UINT32  RGST_Group0391_Reg20                  ; // 20  (ADDR : 0x9C00_C3D0)
    UINT32  RGST_Group0391_Reg21                  ; // 21  (ADDR : 0x9C00_C3D4)
    UINT32  RGST_Group0391_Reg22                  ; // 22  (ADDR : 0x9C00_C3D8)
    UINT32  RGST_Group0391_Reg23                  ; // 23  (ADDR : 0x9C00_C3DC)
    UINT32  RGST_Group0391_Reg24                  ; // 24  (ADDR : 0x9C00_C3E0)
    UINT32  RGST_Group0391_Reg25                  ; // 25  (ADDR : 0x9C00_C3E4)
    UINT32  RGST_Group0391_Reg26                  ; // 26  (ADDR : 0x9C00_C3E8)
    UINT32  RGST_Group0391_Reg27                  ; // 27  (ADDR : 0x9C00_C3EC)
    UINT32  RGST_Group0391_Reg28                  ; // 28  (ADDR : 0x9C00_C3F0)
    UINT32  RGST_Group0391_Reg29                  ; // 29  (ADDR : 0x9C00_C3F4)
    UINT32  RGST_Group0391_Reg30                  ; // 30  (ADDR : 0x9C00_C3F8)
    UINT32  RGST_Group0391_Reg31                  ; // 31  (ADDR : 0x9C00_C3FC)

    //Group   392 : MFD
    UINT32  RGST_Group0392_Reg00                  ; // 00  (ADDR : 0x9C00_C400)
    UINT32  RGST_Group0392_Reg01                  ; // 01  (ADDR : 0x9C00_C404)
    UINT32  RGST_Group0392_Reg02                  ; // 02  (ADDR : 0x9C00_C408)
    UINT32  RGST_Group0392_Reg03                  ; // 03  (ADDR : 0x9C00_C40C)
    UINT32  RGST_Group0392_Reg04                  ; // 04  (ADDR : 0x9C00_C410)
    UINT32  RGST_Group0392_Reg05                  ; // 05  (ADDR : 0x9C00_C414)
    UINT32  RGST_Group0392_Reg06                  ; // 06  (ADDR : 0x9C00_C418)
    UINT32  RGST_Group0392_Reg07                  ; // 07  (ADDR : 0x9C00_C41C)
    UINT32  RGST_Group0392_Reg08                  ; // 08  (ADDR : 0x9C00_C420)
    UINT32  RGST_Group0392_Reg09                  ; // 09  (ADDR : 0x9C00_C424)
    UINT32  RGST_Group0392_Reg10                  ; // 10  (ADDR : 0x9C00_C428)
    UINT32  RGST_Group0392_Reg11                  ; // 11  (ADDR : 0x9C00_C42C)
    UINT32  RGST_Group0392_Reg12                  ; // 12  (ADDR : 0x9C00_C430)
    UINT32  RGST_Group0392_Reg13                  ; // 13  (ADDR : 0x9C00_C434)
    UINT32  RGST_Group0392_Reg14                  ; // 14  (ADDR : 0x9C00_C438)
    UINT32  RGST_Group0392_Reg15                  ; // 15  (ADDR : 0x9C00_C43C)
    UINT32  RGST_Group0392_Reg16                  ; // 16  (ADDR : 0x9C00_C440)
    UINT32  RGST_Group0392_Reg17                  ; // 17  (ADDR : 0x9C00_C444)
    UINT32  RGST_Group0392_Reg18                  ; // 18  (ADDR : 0x9C00_C448)
    UINT32  RGST_Group0392_Reg19                  ; // 19  (ADDR : 0x9C00_C44C)
    UINT32  RGST_Group0392_Reg20                  ; // 20  (ADDR : 0x9C00_C450)
    UINT32  RGST_Group0392_Reg21                  ; // 21  (ADDR : 0x9C00_C454)
    UINT32  RGST_Group0392_Reg22                  ; // 22  (ADDR : 0x9C00_C458)
    UINT32  RGST_Group0392_Reg23                  ; // 23  (ADDR : 0x9C00_C45C)
    UINT32  RGST_Group0392_Reg24                  ; // 24  (ADDR : 0x9C00_C460)
    UINT32  RGST_Group0392_Reg25                  ; // 25  (ADDR : 0x9C00_C464)
    UINT32  RGST_Group0392_Reg26                  ; // 26  (ADDR : 0x9C00_C468)
    UINT32  RGST_Group0392_Reg27                  ; // 27  (ADDR : 0x9C00_C46C)
    UINT32  RGST_Group0392_Reg28                  ; // 28  (ADDR : 0x9C00_C470)
    UINT32  RGST_Group0392_Reg29                  ; // 29  (ADDR : 0x9C00_C474)
    UINT32  RGST_Group0392_Reg30                  ; // 30  (ADDR : 0x9C00_C478)
    UINT32  RGST_Group0392_Reg31                  ; // 31  (ADDR : 0x9C00_C47C)

    //Group   393 : MFD
    UINT32  RGST_Group0393_Reg00                  ; // 00  (ADDR : 0x9C00_C480)
    UINT32  RGST_Group0393_Reg01                  ; // 01  (ADDR : 0x9C00_C484)
    UINT32  RGST_Group0393_Reg02                  ; // 02  (ADDR : 0x9C00_C488)
    UINT32  RGST_Group0393_Reg03                  ; // 03  (ADDR : 0x9C00_C48C)
    UINT32  RGST_Group0393_Reg04                  ; // 04  (ADDR : 0x9C00_C490)
    UINT32  RGST_Group0393_Reg05                  ; // 05  (ADDR : 0x9C00_C494)
    UINT32  RGST_Group0393_Reg06                  ; // 06  (ADDR : 0x9C00_C498)
    UINT32  RGST_Group0393_Reg07                  ; // 07  (ADDR : 0x9C00_C49C)
    UINT32  RGST_Group0393_Reg08                  ; // 08  (ADDR : 0x9C00_C4A0)
    UINT32  RGST_Group0393_Reg09                  ; // 09  (ADDR : 0x9C00_C4A4)
    UINT32  RGST_Group0393_Reg10                  ; // 10  (ADDR : 0x9C00_C4A8)
    UINT32  RGST_Group0393_Reg11                  ; // 11  (ADDR : 0x9C00_C4AC)
    UINT32  RGST_Group0393_Reg12                  ; // 12  (ADDR : 0x9C00_C4B0)
    UINT32  RGST_Group0393_Reg13                  ; // 13  (ADDR : 0x9C00_C4B4)
    UINT32  RGST_Group0393_Reg14                  ; // 14  (ADDR : 0x9C00_C4B8)
    UINT32  RGST_Group0393_Reg15                  ; // 15  (ADDR : 0x9C00_C4BC)
    UINT32  RGST_Group0393_Reg16                  ; // 16  (ADDR : 0x9C00_C4C0)
    UINT32  RGST_Group0393_Reg17                  ; // 17  (ADDR : 0x9C00_C4C4)
    UINT32  RGST_Group0393_Reg18                  ; // 18  (ADDR : 0x9C00_C4C8)
    UINT32  RGST_Group0393_Reg19                  ; // 19  (ADDR : 0x9C00_C4CC)
    UINT32  RGST_Group0393_Reg20                  ; // 20  (ADDR : 0x9C00_C4D0)
    UINT32  RGST_Group0393_Reg21                  ; // 21  (ADDR : 0x9C00_C4D4)
    UINT32  RGST_Group0393_Reg22                  ; // 22  (ADDR : 0x9C00_C4D8)
    UINT32  RGST_Group0393_Reg23                  ; // 23  (ADDR : 0x9C00_C4DC)
    UINT32  RGST_Group0393_Reg24                  ; // 24  (ADDR : 0x9C00_C4E0)
    UINT32  RGST_Group0393_Reg25                  ; // 25  (ADDR : 0x9C00_C4E4)
    UINT32  RGST_Group0393_Reg26                  ; // 26  (ADDR : 0x9C00_C4E8)
    UINT32  RGST_Group0393_Reg27                  ; // 27  (ADDR : 0x9C00_C4EC)
    UINT32  RGST_Group0393_Reg28                  ; // 28  (ADDR : 0x9C00_C4F0)
    UINT32  RGST_Group0393_Reg29                  ; // 29  (ADDR : 0x9C00_C4F4)
    UINT32  RGST_Group0393_Reg30                  ; // 30  (ADDR : 0x9C00_C4F8)
    UINT32  RGST_Group0393_Reg31                  ; // 31  (ADDR : 0x9C00_C4FC)

    //Group   394 : MFD
    UINT32  RGST_Group0394_Reg00                  ; // 00  (ADDR : 0x9C00_C500)
    UINT32  RGST_Group0394_Reg01                  ; // 01  (ADDR : 0x9C00_C504)
    UINT32  RGST_Group0394_Reg02                  ; // 02  (ADDR : 0x9C00_C508)
    UINT32  RGST_Group0394_Reg03                  ; // 03  (ADDR : 0x9C00_C50C)
    UINT32  RGST_Group0394_Reg04                  ; // 04  (ADDR : 0x9C00_C510)
    UINT32  RGST_Group0394_Reg05                  ; // 05  (ADDR : 0x9C00_C514)
    UINT32  RGST_Group0394_Reg06                  ; // 06  (ADDR : 0x9C00_C518)
    UINT32  RGST_Group0394_Reg07                  ; // 07  (ADDR : 0x9C00_C51C)
    UINT32  RGST_Group0394_Reg08                  ; // 08  (ADDR : 0x9C00_C520)
    UINT32  RGST_Group0394_Reg09                  ; // 09  (ADDR : 0x9C00_C524)
    UINT32  RGST_Group0394_Reg10                  ; // 10  (ADDR : 0x9C00_C528)
    UINT32  RGST_Group0394_Reg11                  ; // 11  (ADDR : 0x9C00_C52C)
    UINT32  RGST_Group0394_Reg12                  ; // 12  (ADDR : 0x9C00_C530)
    UINT32  RGST_Group0394_Reg13                  ; // 13  (ADDR : 0x9C00_C534)
    UINT32  RGST_Group0394_Reg14                  ; // 14  (ADDR : 0x9C00_C538)
    UINT32  RGST_Group0394_Reg15                  ; // 15  (ADDR : 0x9C00_C53C)
    UINT32  RGST_Group0394_Reg16                  ; // 16  (ADDR : 0x9C00_C540)
    UINT32  RGST_Group0394_Reg17                  ; // 17  (ADDR : 0x9C00_C544)
    UINT32  RGST_Group0394_Reg18                  ; // 18  (ADDR : 0x9C00_C548)
    UINT32  RGST_Group0394_Reg19                  ; // 19  (ADDR : 0x9C00_C54C)
    UINT32  RGST_Group0394_Reg20                  ; // 20  (ADDR : 0x9C00_C550)
    UINT32  RGST_Group0394_Reg21                  ; // 21  (ADDR : 0x9C00_C554)
    UINT32  RGST_Group0394_Reg22                  ; // 22  (ADDR : 0x9C00_C558)
    UINT32  RGST_Group0394_Reg23                  ; // 23  (ADDR : 0x9C00_C55C)
    UINT32  RGST_Group0394_Reg24                  ; // 24  (ADDR : 0x9C00_C560)
    UINT32  RGST_Group0394_Reg25                  ; // 25  (ADDR : 0x9C00_C564)
    UINT32  RGST_Group0394_Reg26                  ; // 26  (ADDR : 0x9C00_C568)
    UINT32  RGST_Group0394_Reg27                  ; // 27  (ADDR : 0x9C00_C56C)
    UINT32  RGST_Group0394_Reg28                  ; // 28  (ADDR : 0x9C00_C570)
    UINT32  RGST_Group0394_Reg29                  ; // 29  (ADDR : 0x9C00_C574)
    UINT32  RGST_Group0394_Reg30                  ; // 30  (ADDR : 0x9C00_C578)
    UINT32  RGST_Group0394_Reg31                  ; // 31  (ADDR : 0x9C00_C57C)

    //Group   395 : MFD
    UINT32  RGST_Group0395_Reg00                  ; // 00  (ADDR : 0x9C00_C580)
    UINT32  RGST_Group0395_Reg01                  ; // 01  (ADDR : 0x9C00_C584)
    UINT32  RGST_Group0395_Reg02                  ; // 02  (ADDR : 0x9C00_C588)
    UINT32  RGST_Group0395_Reg03                  ; // 03  (ADDR : 0x9C00_C58C)
    UINT32  RGST_Group0395_Reg04                  ; // 04  (ADDR : 0x9C00_C590)
    UINT32  RGST_Group0395_Reg05                  ; // 05  (ADDR : 0x9C00_C594)
    UINT32  RGST_Group0395_Reg06                  ; // 06  (ADDR : 0x9C00_C598)
    UINT32  RGST_Group0395_Reg07                  ; // 07  (ADDR : 0x9C00_C59C)
    UINT32  RGST_Group0395_Reg08                  ; // 08  (ADDR : 0x9C00_C5A0)
    UINT32  RGST_Group0395_Reg09                  ; // 09  (ADDR : 0x9C00_C5A4)
    UINT32  RGST_Group0395_Reg10                  ; // 10  (ADDR : 0x9C00_C5A8)
    UINT32  RGST_Group0395_Reg11                  ; // 11  (ADDR : 0x9C00_C5AC)
    UINT32  RGST_Group0395_Reg12                  ; // 12  (ADDR : 0x9C00_C5B0)
    UINT32  RGST_Group0395_Reg13                  ; // 13  (ADDR : 0x9C00_C5B4)
    UINT32  RGST_Group0395_Reg14                  ; // 14  (ADDR : 0x9C00_C5B8)
    UINT32  RGST_Group0395_Reg15                  ; // 15  (ADDR : 0x9C00_C5BC)
    UINT32  RGST_Group0395_Reg16                  ; // 16  (ADDR : 0x9C00_C5C0)
    UINT32  RGST_Group0395_Reg17                  ; // 17  (ADDR : 0x9C00_C5C4)
    UINT32  RGST_Group0395_Reg18                  ; // 18  (ADDR : 0x9C00_C5C8)
    UINT32  RGST_Group0395_Reg19                  ; // 19  (ADDR : 0x9C00_C5CC)
    UINT32  RGST_Group0395_Reg20                  ; // 20  (ADDR : 0x9C00_C5D0)
    UINT32  RGST_Group0395_Reg21                  ; // 21  (ADDR : 0x9C00_C5D4)
    UINT32  RGST_Group0395_Reg22                  ; // 22  (ADDR : 0x9C00_C5D8)
    UINT32  RGST_Group0395_Reg23                  ; // 23  (ADDR : 0x9C00_C5DC)
    UINT32  RGST_Group0395_Reg24                  ; // 24  (ADDR : 0x9C00_C5E0)
    UINT32  RGST_Group0395_Reg25                  ; // 25  (ADDR : 0x9C00_C5E4)
    UINT32  RGST_Group0395_Reg26                  ; // 26  (ADDR : 0x9C00_C5E8)
    UINT32  RGST_Group0395_Reg27                  ; // 27  (ADDR : 0x9C00_C5EC)
    UINT32  RGST_Group0395_Reg28                  ; // 28  (ADDR : 0x9C00_C5F0)
    UINT32  RGST_Group0395_Reg29                  ; // 29  (ADDR : 0x9C00_C5F4)
    UINT32  RGST_Group0395_Reg30                  ; // 30  (ADDR : 0x9C00_C5F8)
    UINT32  RGST_Group0395_Reg31                  ; // 31  (ADDR : 0x9C00_C5FC)

    //Group   396 : MFD
    UINT32  RGST_Group0396_Reg00                  ; // 00  (ADDR : 0x9C00_C600)
    UINT32  RGST_Group0396_Reg01                  ; // 01  (ADDR : 0x9C00_C604)
    UINT32  RGST_Group0396_Reg02                  ; // 02  (ADDR : 0x9C00_C608)
    UINT32  RGST_Group0396_Reg03                  ; // 03  (ADDR : 0x9C00_C60C)
    UINT32  RGST_Group0396_Reg04                  ; // 04  (ADDR : 0x9C00_C610)
    UINT32  RGST_Group0396_Reg05                  ; // 05  (ADDR : 0x9C00_C614)
    UINT32  RGST_Group0396_Reg06                  ; // 06  (ADDR : 0x9C00_C618)
    UINT32  RGST_Group0396_Reg07                  ; // 07  (ADDR : 0x9C00_C61C)
    UINT32  RGST_Group0396_Reg08                  ; // 08  (ADDR : 0x9C00_C620)
    UINT32  RGST_Group0396_Reg09                  ; // 09  (ADDR : 0x9C00_C624)
    UINT32  RGST_Group0396_Reg10                  ; // 10  (ADDR : 0x9C00_C628)
    UINT32  RGST_Group0396_Reg11                  ; // 11  (ADDR : 0x9C00_C62C)
    UINT32  RGST_Group0396_Reg12                  ; // 12  (ADDR : 0x9C00_C630)
    UINT32  RGST_Group0396_Reg13                  ; // 13  (ADDR : 0x9C00_C634)
    UINT32  RGST_Group0396_Reg14                  ; // 14  (ADDR : 0x9C00_C638)
    UINT32  RGST_Group0396_Reg15                  ; // 15  (ADDR : 0x9C00_C63C)
    UINT32  RGST_Group0396_Reg16                  ; // 16  (ADDR : 0x9C00_C640)
    UINT32  RGST_Group0396_Reg17                  ; // 17  (ADDR : 0x9C00_C644)
    UINT32  RGST_Group0396_Reg18                  ; // 18  (ADDR : 0x9C00_C648)
    UINT32  RGST_Group0396_Reg19                  ; // 19  (ADDR : 0x9C00_C64C)
    UINT32  RGST_Group0396_Reg20                  ; // 20  (ADDR : 0x9C00_C650)
    UINT32  RGST_Group0396_Reg21                  ; // 21  (ADDR : 0x9C00_C654)
    UINT32  RGST_Group0396_Reg22                  ; // 22  (ADDR : 0x9C00_C658)
    UINT32  RGST_Group0396_Reg23                  ; // 23  (ADDR : 0x9C00_C65C)
    UINT32  RGST_Group0396_Reg24                  ; // 24  (ADDR : 0x9C00_C660)
    UINT32  RGST_Group0396_Reg25                  ; // 25  (ADDR : 0x9C00_C664)
    UINT32  RGST_Group0396_Reg26                  ; // 26  (ADDR : 0x9C00_C668)
    UINT32  RGST_Group0396_Reg27                  ; // 27  (ADDR : 0x9C00_C66C)
    UINT32  RGST_Group0396_Reg28                  ; // 28  (ADDR : 0x9C00_C670)
    UINT32  RGST_Group0396_Reg29                  ; // 29  (ADDR : 0x9C00_C674)
    UINT32  RGST_Group0396_Reg30                  ; // 30  (ADDR : 0x9C00_C678)
    UINT32  RGST_Group0396_Reg31                  ; // 31  (ADDR : 0x9C00_C67C)

    //Group   397 : MFD
    UINT32  RGST_Group0397_Reg00                  ; // 00  (ADDR : 0x9C00_C680)
    UINT32  RGST_Group0397_Reg01                  ; // 01  (ADDR : 0x9C00_C684)
    UINT32  RGST_Group0397_Reg02                  ; // 02  (ADDR : 0x9C00_C688)
    UINT32  RGST_Group0397_Reg03                  ; // 03  (ADDR : 0x9C00_C68C)
    UINT32  RGST_Group0397_Reg04                  ; // 04  (ADDR : 0x9C00_C690)
    UINT32  RGST_Group0397_Reg05                  ; // 05  (ADDR : 0x9C00_C694)
    UINT32  RGST_Group0397_Reg06                  ; // 06  (ADDR : 0x9C00_C698)
    UINT32  RGST_Group0397_Reg07                  ; // 07  (ADDR : 0x9C00_C69C)
    UINT32  RGST_Group0397_Reg08                  ; // 08  (ADDR : 0x9C00_C6A0)
    UINT32  RGST_Group0397_Reg09                  ; // 09  (ADDR : 0x9C00_C6A4)
    UINT32  RGST_Group0397_Reg10                  ; // 10  (ADDR : 0x9C00_C6A8)
    UINT32  RGST_Group0397_Reg11                  ; // 11  (ADDR : 0x9C00_C6AC)
    UINT32  RGST_Group0397_Reg12                  ; // 12  (ADDR : 0x9C00_C6B0)
    UINT32  RGST_Group0397_Reg13                  ; // 13  (ADDR : 0x9C00_C6B4)
    UINT32  RGST_Group0397_Reg14                  ; // 14  (ADDR : 0x9C00_C6B8)
    UINT32  RGST_Group0397_Reg15                  ; // 15  (ADDR : 0x9C00_C6BC)
    UINT32  RGST_Group0397_Reg16                  ; // 16  (ADDR : 0x9C00_C6C0)
    UINT32  RGST_Group0397_Reg17                  ; // 17  (ADDR : 0x9C00_C6C4)
    UINT32  RGST_Group0397_Reg18                  ; // 18  (ADDR : 0x9C00_C6C8)
    UINT32  RGST_Group0397_Reg19                  ; // 19  (ADDR : 0x9C00_C6CC)
    UINT32  RGST_Group0397_Reg20                  ; // 20  (ADDR : 0x9C00_C6D0)
    UINT32  RGST_Group0397_Reg21                  ; // 21  (ADDR : 0x9C00_C6D4)
    UINT32  RGST_Group0397_Reg22                  ; // 22  (ADDR : 0x9C00_C6D8)
    UINT32  RGST_Group0397_Reg23                  ; // 23  (ADDR : 0x9C00_C6DC)
    UINT32  RGST_Group0397_Reg24                  ; // 24  (ADDR : 0x9C00_C6E0)
    UINT32  RGST_Group0397_Reg25                  ; // 25  (ADDR : 0x9C00_C6E4)
    UINT32  RGST_Group0397_Reg26                  ; // 26  (ADDR : 0x9C00_C6E8)
    UINT32  RGST_Group0397_Reg27                  ; // 27  (ADDR : 0x9C00_C6EC)
    UINT32  RGST_Group0397_Reg28                  ; // 28  (ADDR : 0x9C00_C6F0)
    UINT32  RGST_Group0397_Reg29                  ; // 29  (ADDR : 0x9C00_C6F4)
    UINT32  RGST_Group0397_Reg30                  ; // 30  (ADDR : 0x9C00_C6F8)
    UINT32  RGST_Group0397_Reg31                  ; // 31  (ADDR : 0x9C00_C6FC)

    //Group   398 : MFD
    UINT32  RGST_Group0398_Reg00                  ; // 00  (ADDR : 0x9C00_C700)
    UINT32  RGST_Group0398_Reg01                  ; // 01  (ADDR : 0x9C00_C704)
    UINT32  RGST_Group0398_Reg02                  ; // 02  (ADDR : 0x9C00_C708)
    UINT32  RGST_Group0398_Reg03                  ; // 03  (ADDR : 0x9C00_C70C)
    UINT32  RGST_Group0398_Reg04                  ; // 04  (ADDR : 0x9C00_C710)
    UINT32  RGST_Group0398_Reg05                  ; // 05  (ADDR : 0x9C00_C714)
    UINT32  RGST_Group0398_Reg06                  ; // 06  (ADDR : 0x9C00_C718)
    UINT32  RGST_Group0398_Reg07                  ; // 07  (ADDR : 0x9C00_C71C)
    UINT32  RGST_Group0398_Reg08                  ; // 08  (ADDR : 0x9C00_C720)
    UINT32  RGST_Group0398_Reg09                  ; // 09  (ADDR : 0x9C00_C724)
    UINT32  RGST_Group0398_Reg10                  ; // 10  (ADDR : 0x9C00_C728)
    UINT32  RGST_Group0398_Reg11                  ; // 11  (ADDR : 0x9C00_C72C)
    UINT32  RGST_Group0398_Reg12                  ; // 12  (ADDR : 0x9C00_C730)
    UINT32  RGST_Group0398_Reg13                  ; // 13  (ADDR : 0x9C00_C734)
    UINT32  RGST_Group0398_Reg14                  ; // 14  (ADDR : 0x9C00_C738)
    UINT32  RGST_Group0398_Reg15                  ; // 15  (ADDR : 0x9C00_C73C)
    UINT32  RGST_Group0398_Reg16                  ; // 16  (ADDR : 0x9C00_C740)
    UINT32  RGST_Group0398_Reg17                  ; // 17  (ADDR : 0x9C00_C744)
    UINT32  RGST_Group0398_Reg18                  ; // 18  (ADDR : 0x9C00_C748)
    UINT32  RGST_Group0398_Reg19                  ; // 19  (ADDR : 0x9C00_C74C)
    UINT32  RGST_Group0398_Reg20                  ; // 20  (ADDR : 0x9C00_C750)
    UINT32  RGST_Group0398_Reg21                  ; // 21  (ADDR : 0x9C00_C754)
    UINT32  RGST_Group0398_Reg22                  ; // 22  (ADDR : 0x9C00_C758)
    UINT32  RGST_Group0398_Reg23                  ; // 23  (ADDR : 0x9C00_C75C)
    UINT32  RGST_Group0398_Reg24                  ; // 24  (ADDR : 0x9C00_C760)
    UINT32  RGST_Group0398_Reg25                  ; // 25  (ADDR : 0x9C00_C764)
    UINT32  RGST_Group0398_Reg26                  ; // 26  (ADDR : 0x9C00_C768)
    UINT32  RGST_Group0398_Reg27                  ; // 27  (ADDR : 0x9C00_C76C)
    UINT32  RGST_Group0398_Reg28                  ; // 28  (ADDR : 0x9C00_C770)
    UINT32  RGST_Group0398_Reg29                  ; // 29  (ADDR : 0x9C00_C774)
    UINT32  RGST_Group0398_Reg30                  ; // 30  (ADDR : 0x9C00_C778)
    UINT32  RGST_Group0398_Reg31                  ; // 31  (ADDR : 0x9C00_C77C)

    //Group   399 : MFD
    UINT32  RGST_Group0399_Reg00                  ; // 00  (ADDR : 0x9C00_C780)
    UINT32  RGST_Group0399_Reg01                  ; // 01  (ADDR : 0x9C00_C784)
    UINT32  RGST_Group0399_Reg02                  ; // 02  (ADDR : 0x9C00_C788)
    UINT32  RGST_Group0399_Reg03                  ; // 03  (ADDR : 0x9C00_C78C)
    UINT32  RGST_Group0399_Reg04                  ; // 04  (ADDR : 0x9C00_C790)
    UINT32  RGST_Group0399_Reg05                  ; // 05  (ADDR : 0x9C00_C794)
    UINT32  RGST_Group0399_Reg06                  ; // 06  (ADDR : 0x9C00_C798)
    UINT32  RGST_Group0399_Reg07                  ; // 07  (ADDR : 0x9C00_C79C)
    UINT32  RGST_Group0399_Reg08                  ; // 08  (ADDR : 0x9C00_C7A0)
    UINT32  RGST_Group0399_Reg09                  ; // 09  (ADDR : 0x9C00_C7A4)
    UINT32  RGST_Group0399_Reg10                  ; // 10  (ADDR : 0x9C00_C7A8)
    UINT32  RGST_Group0399_Reg11                  ; // 11  (ADDR : 0x9C00_C7AC)
    UINT32  RGST_Group0399_Reg12                  ; // 12  (ADDR : 0x9C00_C7B0)
    UINT32  RGST_Group0399_Reg13                  ; // 13  (ADDR : 0x9C00_C7B4)
    UINT32  RGST_Group0399_Reg14                  ; // 14  (ADDR : 0x9C00_C7B8)
    UINT32  RGST_Group0399_Reg15                  ; // 15  (ADDR : 0x9C00_C7BC)
    UINT32  RGST_Group0399_Reg16                  ; // 16  (ADDR : 0x9C00_C7C0)
    UINT32  RGST_Group0399_Reg17                  ; // 17  (ADDR : 0x9C00_C7C4)
    UINT32  RGST_Group0399_Reg18                  ; // 18  (ADDR : 0x9C00_C7C8)
    UINT32  RGST_Group0399_Reg19                  ; // 19  (ADDR : 0x9C00_C7CC)
    UINT32  RGST_Group0399_Reg20                  ; // 20  (ADDR : 0x9C00_C7D0)
    UINT32  RGST_Group0399_Reg21                  ; // 21  (ADDR : 0x9C00_C7D4)
    UINT32  RGST_Group0399_Reg22                  ; // 22  (ADDR : 0x9C00_C7D8)
    UINT32  RGST_Group0399_Reg23                  ; // 23  (ADDR : 0x9C00_C7DC)
    UINT32  RGST_Group0399_Reg24                  ; // 24  (ADDR : 0x9C00_C7E0)
    UINT32  RGST_Group0399_Reg25                  ; // 25  (ADDR : 0x9C00_C7E4)
    UINT32  RGST_Group0399_Reg26                  ; // 26  (ADDR : 0x9C00_C7E8)
    UINT32  RGST_Group0399_Reg27                  ; // 27  (ADDR : 0x9C00_C7EC)
    UINT32  RGST_Group0399_Reg28                  ; // 28  (ADDR : 0x9C00_C7F0)
    UINT32  RGST_Group0399_Reg29                  ; // 29  (ADDR : 0x9C00_C7F4)
    UINT32  RGST_Group0399_Reg30                  ; // 30  (ADDR : 0x9C00_C7F8)
    UINT32  RGST_Group0399_Reg31                  ; // 31  (ADDR : 0x9C00_C7FC)

    //Group   400 : MFD
    UINT32  RGST_Group0400_Reg00                  ; // 00  (ADDR : 0x9C00_C800)
    UINT32  RGST_Group0400_Reg01                  ; // 01  (ADDR : 0x9C00_C804)
    UINT32  RGST_Group0400_Reg02                  ; // 02  (ADDR : 0x9C00_C808)
    UINT32  RGST_Group0400_Reg03                  ; // 03  (ADDR : 0x9C00_C80C)
    UINT32  RGST_Group0400_Reg04                  ; // 04  (ADDR : 0x9C00_C810)
    UINT32  RGST_Group0400_Reg05                  ; // 05  (ADDR : 0x9C00_C814)
    UINT32  RGST_Group0400_Reg06                  ; // 06  (ADDR : 0x9C00_C818)
    UINT32  RGST_Group0400_Reg07                  ; // 07  (ADDR : 0x9C00_C81C)
    UINT32  RGST_Group0400_Reg08                  ; // 08  (ADDR : 0x9C00_C820)
    UINT32  RGST_Group0400_Reg09                  ; // 09  (ADDR : 0x9C00_C824)
    UINT32  RGST_Group0400_Reg10                  ; // 10  (ADDR : 0x9C00_C828)
    UINT32  RGST_Group0400_Reg11                  ; // 11  (ADDR : 0x9C00_C82C)
    UINT32  RGST_Group0400_Reg12                  ; // 12  (ADDR : 0x9C00_C830)
    UINT32  RGST_Group0400_Reg13                  ; // 13  (ADDR : 0x9C00_C834)
    UINT32  RGST_Group0400_Reg14                  ; // 14  (ADDR : 0x9C00_C838)
    UINT32  RGST_Group0400_Reg15                  ; // 15  (ADDR : 0x9C00_C83C)
    UINT32  RGST_Group0400_Reg16                  ; // 16  (ADDR : 0x9C00_C840)
    UINT32  RGST_Group0400_Reg17                  ; // 17  (ADDR : 0x9C00_C844)
    UINT32  RGST_Group0400_Reg18                  ; // 18  (ADDR : 0x9C00_C848)
    UINT32  RGST_Group0400_Reg19                  ; // 19  (ADDR : 0x9C00_C84C)
    UINT32  RGST_Group0400_Reg20                  ; // 20  (ADDR : 0x9C00_C850)
    UINT32  RGST_Group0400_Reg21                  ; // 21  (ADDR : 0x9C00_C854)
    UINT32  RGST_Group0400_Reg22                  ; // 22  (ADDR : 0x9C00_C858)
    UINT32  RGST_Group0400_Reg23                  ; // 23  (ADDR : 0x9C00_C85C)
    UINT32  RGST_Group0400_Reg24                  ; // 24  (ADDR : 0x9C00_C860)
    UINT32  RGST_Group0400_Reg25                  ; // 25  (ADDR : 0x9C00_C864)
    UINT32  RGST_Group0400_Reg26                  ; // 26  (ADDR : 0x9C00_C868)
    UINT32  RGST_Group0400_Reg27                  ; // 27  (ADDR : 0x9C00_C86C)
    UINT32  RGST_Group0400_Reg28                  ; // 28  (ADDR : 0x9C00_C870)
    UINT32  RGST_Group0400_Reg29                  ; // 29  (ADDR : 0x9C00_C874)
    UINT32  RGST_Group0400_Reg30                  ; // 30  (ADDR : 0x9C00_C878)
    UINT32  RGST_Group0400_Reg31                  ; // 31  (ADDR : 0x9C00_C87C)

    //Group   401 : MFD
    UINT32  RGST_Group0401_Reg00                  ; // 00  (ADDR : 0x9C00_C880)
    UINT32  RGST_Group0401_Reg01                  ; // 01  (ADDR : 0x9C00_C884)
    UINT32  RGST_Group0401_Reg02                  ; // 02  (ADDR : 0x9C00_C888)
    UINT32  RGST_Group0401_Reg03                  ; // 03  (ADDR : 0x9C00_C88C)
    UINT32  RGST_Group0401_Reg04                  ; // 04  (ADDR : 0x9C00_C890)
    UINT32  RGST_Group0401_Reg05                  ; // 05  (ADDR : 0x9C00_C894)
    UINT32  RGST_Group0401_Reg06                  ; // 06  (ADDR : 0x9C00_C898)
    UINT32  RGST_Group0401_Reg07                  ; // 07  (ADDR : 0x9C00_C89C)
    UINT32  RGST_Group0401_Reg08                  ; // 08  (ADDR : 0x9C00_C8A0)
    UINT32  RGST_Group0401_Reg09                  ; // 09  (ADDR : 0x9C00_C8A4)
    UINT32  RGST_Group0401_Reg10                  ; // 10  (ADDR : 0x9C00_C8A8)
    UINT32  RGST_Group0401_Reg11                  ; // 11  (ADDR : 0x9C00_C8AC)
    UINT32  RGST_Group0401_Reg12                  ; // 12  (ADDR : 0x9C00_C8B0)
    UINT32  RGST_Group0401_Reg13                  ; // 13  (ADDR : 0x9C00_C8B4)
    UINT32  RGST_Group0401_Reg14                  ; // 14  (ADDR : 0x9C00_C8B8)
    UINT32  RGST_Group0401_Reg15                  ; // 15  (ADDR : 0x9C00_C8BC)
    UINT32  RGST_Group0401_Reg16                  ; // 16  (ADDR : 0x9C00_C8C0)
    UINT32  RGST_Group0401_Reg17                  ; // 17  (ADDR : 0x9C00_C8C4)
    UINT32  RGST_Group0401_Reg18                  ; // 18  (ADDR : 0x9C00_C8C8)
    UINT32  RGST_Group0401_Reg19                  ; // 19  (ADDR : 0x9C00_C8CC)
    UINT32  RGST_Group0401_Reg20                  ; // 20  (ADDR : 0x9C00_C8D0)
    UINT32  RGST_Group0401_Reg21                  ; // 21  (ADDR : 0x9C00_C8D4)
    UINT32  RGST_Group0401_Reg22                  ; // 22  (ADDR : 0x9C00_C8D8)
    UINT32  RGST_Group0401_Reg23                  ; // 23  (ADDR : 0x9C00_C8DC)
    UINT32  RGST_Group0401_Reg24                  ; // 24  (ADDR : 0x9C00_C8E0)
    UINT32  RGST_Group0401_Reg25                  ; // 25  (ADDR : 0x9C00_C8E4)
    UINT32  RGST_Group0401_Reg26                  ; // 26  (ADDR : 0x9C00_C8E8)
    UINT32  RGST_Group0401_Reg27                  ; // 27  (ADDR : 0x9C00_C8EC)
    UINT32  RGST_Group0401_Reg28                  ; // 28  (ADDR : 0x9C00_C8F0)
    UINT32  RGST_Group0401_Reg29                  ; // 29  (ADDR : 0x9C00_C8F4)
    UINT32  RGST_Group0401_Reg30                  ; // 30  (ADDR : 0x9C00_C8F8)
    UINT32  RGST_Group0401_Reg31                  ; // 31  (ADDR : 0x9C00_C8FC)

    //Group   402 : MFD
    UINT32  RGST_Group0402_Reg00                  ; // 00  (ADDR : 0x9C00_C900)
    UINT32  RGST_Group0402_Reg01                  ; // 01  (ADDR : 0x9C00_C904)
    UINT32  RGST_Group0402_Reg02                  ; // 02  (ADDR : 0x9C00_C908)
    UINT32  RGST_Group0402_Reg03                  ; // 03  (ADDR : 0x9C00_C90C)
    UINT32  RGST_Group0402_Reg04                  ; // 04  (ADDR : 0x9C00_C910)
    UINT32  RGST_Group0402_Reg05                  ; // 05  (ADDR : 0x9C00_C914)
    UINT32  RGST_Group0402_Reg06                  ; // 06  (ADDR : 0x9C00_C918)
    UINT32  RGST_Group0402_Reg07                  ; // 07  (ADDR : 0x9C00_C91C)
    UINT32  RGST_Group0402_Reg08                  ; // 08  (ADDR : 0x9C00_C920)
    UINT32  RGST_Group0402_Reg09                  ; // 09  (ADDR : 0x9C00_C924)
    UINT32  RGST_Group0402_Reg10                  ; // 10  (ADDR : 0x9C00_C928)
    UINT32  RGST_Group0402_Reg11                  ; // 11  (ADDR : 0x9C00_C92C)
    UINT32  RGST_Group0402_Reg12                  ; // 12  (ADDR : 0x9C00_C930)
    UINT32  RGST_Group0402_Reg13                  ; // 13  (ADDR : 0x9C00_C934)
    UINT32  RGST_Group0402_Reg14                  ; // 14  (ADDR : 0x9C00_C938)
    UINT32  RGST_Group0402_Reg15                  ; // 15  (ADDR : 0x9C00_C93C)
    UINT32  RGST_Group0402_Reg16                  ; // 16  (ADDR : 0x9C00_C940)
    UINT32  RGST_Group0402_Reg17                  ; // 17  (ADDR : 0x9C00_C944)
    UINT32  RGST_Group0402_Reg18                  ; // 18  (ADDR : 0x9C00_C948)
    UINT32  RGST_Group0402_Reg19                  ; // 19  (ADDR : 0x9C00_C94C)
    UINT32  RGST_Group0402_Reg20                  ; // 20  (ADDR : 0x9C00_C950)
    UINT32  RGST_Group0402_Reg21                  ; // 21  (ADDR : 0x9C00_C954)
    UINT32  RGST_Group0402_Reg22                  ; // 22  (ADDR : 0x9C00_C958)
    UINT32  RGST_Group0402_Reg23                  ; // 23  (ADDR : 0x9C00_C95C)
    UINT32  RGST_Group0402_Reg24                  ; // 24  (ADDR : 0x9C00_C960)
    UINT32  RGST_Group0402_Reg25                  ; // 25  (ADDR : 0x9C00_C964)
    UINT32  RGST_Group0402_Reg26                  ; // 26  (ADDR : 0x9C00_C968)
    UINT32  RGST_Group0402_Reg27                  ; // 27  (ADDR : 0x9C00_C96C)
    UINT32  RGST_Group0402_Reg28                  ; // 28  (ADDR : 0x9C00_C970)
    UINT32  RGST_Group0402_Reg29                  ; // 29  (ADDR : 0x9C00_C974)
    UINT32  RGST_Group0402_Reg30                  ; // 30  (ADDR : 0x9C00_C978)
    UINT32  RGST_Group0402_Reg31                  ; // 31  (ADDR : 0x9C00_C97C)

    //Group   403 : MFD
    UINT32  RGST_Group0403_Reg00                  ; // 00  (ADDR : 0x9C00_C980)
    UINT32  RGST_Group0403_Reg01                  ; // 01  (ADDR : 0x9C00_C984)
    UINT32  RGST_Group0403_Reg02                  ; // 02  (ADDR : 0x9C00_C988)
    UINT32  RGST_Group0403_Reg03                  ; // 03  (ADDR : 0x9C00_C98C)
    UINT32  RGST_Group0403_Reg04                  ; // 04  (ADDR : 0x9C00_C990)
    UINT32  RGST_Group0403_Reg05                  ; // 05  (ADDR : 0x9C00_C994)
    UINT32  RGST_Group0403_Reg06                  ; // 06  (ADDR : 0x9C00_C998)
    UINT32  RGST_Group0403_Reg07                  ; // 07  (ADDR : 0x9C00_C99C)
    UINT32  RGST_Group0403_Reg08                  ; // 08  (ADDR : 0x9C00_C9A0)
    UINT32  RGST_Group0403_Reg09                  ; // 09  (ADDR : 0x9C00_C9A4)
    UINT32  RGST_Group0403_Reg10                  ; // 10  (ADDR : 0x9C00_C9A8)
    UINT32  RGST_Group0403_Reg11                  ; // 11  (ADDR : 0x9C00_C9AC)
    UINT32  RGST_Group0403_Reg12                  ; // 12  (ADDR : 0x9C00_C9B0)
    UINT32  RGST_Group0403_Reg13                  ; // 13  (ADDR : 0x9C00_C9B4)
    UINT32  RGST_Group0403_Reg14                  ; // 14  (ADDR : 0x9C00_C9B8)
    UINT32  RGST_Group0403_Reg15                  ; // 15  (ADDR : 0x9C00_C9BC)
    UINT32  RGST_Group0403_Reg16                  ; // 16  (ADDR : 0x9C00_C9C0)
    UINT32  RGST_Group0403_Reg17                  ; // 17  (ADDR : 0x9C00_C9C4)
    UINT32  RGST_Group0403_Reg18                  ; // 18  (ADDR : 0x9C00_C9C8)
    UINT32  RGST_Group0403_Reg19                  ; // 19  (ADDR : 0x9C00_C9CC)
    UINT32  RGST_Group0403_Reg20                  ; // 20  (ADDR : 0x9C00_C9D0)
    UINT32  RGST_Group0403_Reg21                  ; // 21  (ADDR : 0x9C00_C9D4)
    UINT32  RGST_Group0403_Reg22                  ; // 22  (ADDR : 0x9C00_C9D8)
    UINT32  RGST_Group0403_Reg23                  ; // 23  (ADDR : 0x9C00_C9DC)
    UINT32  RGST_Group0403_Reg24                  ; // 24  (ADDR : 0x9C00_C9E0)
    UINT32  RGST_Group0403_Reg25                  ; // 25  (ADDR : 0x9C00_C9E4)
    UINT32  RGST_Group0403_Reg26                  ; // 26  (ADDR : 0x9C00_C9E8)
    UINT32  RGST_Group0403_Reg27                  ; // 27  (ADDR : 0x9C00_C9EC)
    UINT32  RGST_Group0403_Reg28                  ; // 28  (ADDR : 0x9C00_C9F0)
    UINT32  RGST_Group0403_Reg29                  ; // 29  (ADDR : 0x9C00_C9F4)
    UINT32  RGST_Group0403_Reg30                  ; // 30  (ADDR : 0x9C00_C9F8)
    UINT32  RGST_Group0403_Reg31                  ; // 31  (ADDR : 0x9C00_C9FC)

    //Group   404 : MFD
    UINT32  RGST_Group0404_Reg00                  ; // 00  (ADDR : 0x9C00_CA00)
    UINT32  RGST_Group0404_Reg01                  ; // 01  (ADDR : 0x9C00_CA04)
    UINT32  RGST_Group0404_Reg02                  ; // 02  (ADDR : 0x9C00_CA08)
    UINT32  RGST_Group0404_Reg03                  ; // 03  (ADDR : 0x9C00_CA0C)
    UINT32  RGST_Group0404_Reg04                  ; // 04  (ADDR : 0x9C00_CA10)
    UINT32  RGST_Group0404_Reg05                  ; // 05  (ADDR : 0x9C00_CA14)
    UINT32  RGST_Group0404_Reg06                  ; // 06  (ADDR : 0x9C00_CA18)
    UINT32  RGST_Group0404_Reg07                  ; // 07  (ADDR : 0x9C00_CA1C)
    UINT32  RGST_Group0404_Reg08                  ; // 08  (ADDR : 0x9C00_CA20)
    UINT32  RGST_Group0404_Reg09                  ; // 09  (ADDR : 0x9C00_CA24)
    UINT32  RGST_Group0404_Reg10                  ; // 10  (ADDR : 0x9C00_CA28)
    UINT32  RGST_Group0404_Reg11                  ; // 11  (ADDR : 0x9C00_CA2C)
    UINT32  RGST_Group0404_Reg12                  ; // 12  (ADDR : 0x9C00_CA30)
    UINT32  RGST_Group0404_Reg13                  ; // 13  (ADDR : 0x9C00_CA34)
    UINT32  RGST_Group0404_Reg14                  ; // 14  (ADDR : 0x9C00_CA38)
    UINT32  RGST_Group0404_Reg15                  ; // 15  (ADDR : 0x9C00_CA3C)
    UINT32  RGST_Group0404_Reg16                  ; // 16  (ADDR : 0x9C00_CA40)
    UINT32  RGST_Group0404_Reg17                  ; // 17  (ADDR : 0x9C00_CA44)
    UINT32  RGST_Group0404_Reg18                  ; // 18  (ADDR : 0x9C00_CA48)
    UINT32  RGST_Group0404_Reg19                  ; // 19  (ADDR : 0x9C00_CA4C)
    UINT32  RGST_Group0404_Reg20                  ; // 20  (ADDR : 0x9C00_CA50)
    UINT32  RGST_Group0404_Reg21                  ; // 21  (ADDR : 0x9C00_CA54)
    UINT32  RGST_Group0404_Reg22                  ; // 22  (ADDR : 0x9C00_CA58)
    UINT32  RGST_Group0404_Reg23                  ; // 23  (ADDR : 0x9C00_CA5C)
    UINT32  RGST_Group0404_Reg24                  ; // 24  (ADDR : 0x9C00_CA60)
    UINT32  RGST_Group0404_Reg25                  ; // 25  (ADDR : 0x9C00_CA64)
    UINT32  RGST_Group0404_Reg26                  ; // 26  (ADDR : 0x9C00_CA68)
    UINT32  RGST_Group0404_Reg27                  ; // 27  (ADDR : 0x9C00_CA6C)
    UINT32  RGST_Group0404_Reg28                  ; // 28  (ADDR : 0x9C00_CA70)
    UINT32  RGST_Group0404_Reg29                  ; // 29  (ADDR : 0x9C00_CA74)
    UINT32  RGST_Group0404_Reg30                  ; // 30  (ADDR : 0x9C00_CA78)
    UINT32  RGST_Group0404_Reg31                  ; // 31  (ADDR : 0x9C00_CA7C)

    //Group   405 : MFD
    UINT32  RGST_Group0405_Reg00                  ; // 00  (ADDR : 0x9C00_CA80)
    UINT32  RGST_Group0405_Reg01                  ; // 01  (ADDR : 0x9C00_CA84)
    UINT32  RGST_Group0405_Reg02                  ; // 02  (ADDR : 0x9C00_CA88)
    UINT32  RGST_Group0405_Reg03                  ; // 03  (ADDR : 0x9C00_CA8C)
    UINT32  RGST_Group0405_Reg04                  ; // 04  (ADDR : 0x9C00_CA90)
    UINT32  RGST_Group0405_Reg05                  ; // 05  (ADDR : 0x9C00_CA94)
    UINT32  RGST_Group0405_Reg06                  ; // 06  (ADDR : 0x9C00_CA98)
    UINT32  RGST_Group0405_Reg07                  ; // 07  (ADDR : 0x9C00_CA9C)
    UINT32  RGST_Group0405_Reg08                  ; // 08  (ADDR : 0x9C00_CAA0)
    UINT32  RGST_Group0405_Reg09                  ; // 09  (ADDR : 0x9C00_CAA4)
    UINT32  RGST_Group0405_Reg10                  ; // 10  (ADDR : 0x9C00_CAA8)
    UINT32  RGST_Group0405_Reg11                  ; // 11  (ADDR : 0x9C00_CAAC)
    UINT32  RGST_Group0405_Reg12                  ; // 12  (ADDR : 0x9C00_CAB0)
    UINT32  RGST_Group0405_Reg13                  ; // 13  (ADDR : 0x9C00_CAB4)
    UINT32  RGST_Group0405_Reg14                  ; // 14  (ADDR : 0x9C00_CAB8)
    UINT32  RGST_Group0405_Reg15                  ; // 15  (ADDR : 0x9C00_CABC)
    UINT32  RGST_Group0405_Reg16                  ; // 16  (ADDR : 0x9C00_CAC0)
    UINT32  RGST_Group0405_Reg17                  ; // 17  (ADDR : 0x9C00_CAC4)
    UINT32  RGST_Group0405_Reg18                  ; // 18  (ADDR : 0x9C00_CAC8)
    UINT32  RGST_Group0405_Reg19                  ; // 19  (ADDR : 0x9C00_CACC)
    UINT32  RGST_Group0405_Reg20                  ; // 20  (ADDR : 0x9C00_CAD0)
    UINT32  RGST_Group0405_Reg21                  ; // 21  (ADDR : 0x9C00_CAD4)
    UINT32  RGST_Group0405_Reg22                  ; // 22  (ADDR : 0x9C00_CAD8)
    UINT32  RGST_Group0405_Reg23                  ; // 23  (ADDR : 0x9C00_CADC)
    UINT32  RGST_Group0405_Reg24                  ; // 24  (ADDR : 0x9C00_CAE0)
    UINT32  RGST_Group0405_Reg25                  ; // 25  (ADDR : 0x9C00_CAE4)
    UINT32  RGST_Group0405_Reg26                  ; // 26  (ADDR : 0x9C00_CAE8)
    UINT32  RGST_Group0405_Reg27                  ; // 27  (ADDR : 0x9C00_CAEC)
    UINT32  RGST_Group0405_Reg28                  ; // 28  (ADDR : 0x9C00_CAF0)
    UINT32  RGST_Group0405_Reg29                  ; // 29  (ADDR : 0x9C00_CAF4)
    UINT32  RGST_Group0405_Reg30                  ; // 30  (ADDR : 0x9C00_CAF8)
    UINT32  RGST_Group0405_Reg31                  ; // 31  (ADDR : 0x9C00_CAFC)

    //Group   406 : MFD
    UINT32  RGST_Group0406_Reg00                  ; // 00  (ADDR : 0x9C00_CB00)
    UINT32  RGST_Group0406_Reg01                  ; // 01  (ADDR : 0x9C00_CB04)
    UINT32  RGST_Group0406_Reg02                  ; // 02  (ADDR : 0x9C00_CB08)
    UINT32  RGST_Group0406_Reg03                  ; // 03  (ADDR : 0x9C00_CB0C)
    UINT32  RGST_Group0406_Reg04                  ; // 04  (ADDR : 0x9C00_CB10)
    UINT32  RGST_Group0406_Reg05                  ; // 05  (ADDR : 0x9C00_CB14)
    UINT32  RGST_Group0406_Reg06                  ; // 06  (ADDR : 0x9C00_CB18)
    UINT32  RGST_Group0406_Reg07                  ; // 07  (ADDR : 0x9C00_CB1C)
    UINT32  RGST_Group0406_Reg08                  ; // 08  (ADDR : 0x9C00_CB20)
    UINT32  RGST_Group0406_Reg09                  ; // 09  (ADDR : 0x9C00_CB24)
    UINT32  RGST_Group0406_Reg10                  ; // 10  (ADDR : 0x9C00_CB28)
    UINT32  RGST_Group0406_Reg11                  ; // 11  (ADDR : 0x9C00_CB2C)
    UINT32  RGST_Group0406_Reg12                  ; // 12  (ADDR : 0x9C00_CB30)
    UINT32  RGST_Group0406_Reg13                  ; // 13  (ADDR : 0x9C00_CB34)
    UINT32  RGST_Group0406_Reg14                  ; // 14  (ADDR : 0x9C00_CB38)
    UINT32  RGST_Group0406_Reg15                  ; // 15  (ADDR : 0x9C00_CB3C)
    UINT32  RGST_Group0406_Reg16                  ; // 16  (ADDR : 0x9C00_CB40)
    UINT32  RGST_Group0406_Reg17                  ; // 17  (ADDR : 0x9C00_CB44)
    UINT32  RGST_Group0406_Reg18                  ; // 18  (ADDR : 0x9C00_CB48)
    UINT32  RGST_Group0406_Reg19                  ; // 19  (ADDR : 0x9C00_CB4C)
    UINT32  RGST_Group0406_Reg20                  ; // 20  (ADDR : 0x9C00_CB50)
    UINT32  RGST_Group0406_Reg21                  ; // 21  (ADDR : 0x9C00_CB54)
    UINT32  RGST_Group0406_Reg22                  ; // 22  (ADDR : 0x9C00_CB58)
    UINT32  RGST_Group0406_Reg23                  ; // 23  (ADDR : 0x9C00_CB5C)
    UINT32  RGST_Group0406_Reg24                  ; // 24  (ADDR : 0x9C00_CB60)
    UINT32  RGST_Group0406_Reg25                  ; // 25  (ADDR : 0x9C00_CB64)
    UINT32  RGST_Group0406_Reg26                  ; // 26  (ADDR : 0x9C00_CB68)
    UINT32  RGST_Group0406_Reg27                  ; // 27  (ADDR : 0x9C00_CB6C)
    UINT32  RGST_Group0406_Reg28                  ; // 28  (ADDR : 0x9C00_CB70)
    UINT32  RGST_Group0406_Reg29                  ; // 29  (ADDR : 0x9C00_CB74)
    UINT32  RGST_Group0406_Reg30                  ; // 30  (ADDR : 0x9C00_CB78)
    UINT32  RGST_Group0406_Reg31                  ; // 31  (ADDR : 0x9C00_CB7C)

    //Group   407 : MFD
    UINT32  RGST_Group0407_Reg00                  ; // 00  (ADDR : 0x9C00_CB80)
    UINT32  RGST_Group0407_Reg01                  ; // 01  (ADDR : 0x9C00_CB84)
    UINT32  RGST_Group0407_Reg02                  ; // 02  (ADDR : 0x9C00_CB88)
    UINT32  RGST_Group0407_Reg03                  ; // 03  (ADDR : 0x9C00_CB8C)
    UINT32  RGST_Group0407_Reg04                  ; // 04  (ADDR : 0x9C00_CB90)
    UINT32  RGST_Group0407_Reg05                  ; // 05  (ADDR : 0x9C00_CB94)
    UINT32  RGST_Group0407_Reg06                  ; // 06  (ADDR : 0x9C00_CB98)
    UINT32  RGST_Group0407_Reg07                  ; // 07  (ADDR : 0x9C00_CB9C)
    UINT32  RGST_Group0407_Reg08                  ; // 08  (ADDR : 0x9C00_CBA0)
    UINT32  RGST_Group0407_Reg09                  ; // 09  (ADDR : 0x9C00_CBA4)
    UINT32  RGST_Group0407_Reg10                  ; // 10  (ADDR : 0x9C00_CBA8)
    UINT32  RGST_Group0407_Reg11                  ; // 11  (ADDR : 0x9C00_CBAC)
    UINT32  RGST_Group0407_Reg12                  ; // 12  (ADDR : 0x9C00_CBB0)
    UINT32  RGST_Group0407_Reg13                  ; // 13  (ADDR : 0x9C00_CBB4)
    UINT32  RGST_Group0407_Reg14                  ; // 14  (ADDR : 0x9C00_CBB8)
    UINT32  RGST_Group0407_Reg15                  ; // 15  (ADDR : 0x9C00_CBBC)
    UINT32  RGST_Group0407_Reg16                  ; // 16  (ADDR : 0x9C00_CBC0)
    UINT32  RGST_Group0407_Reg17                  ; // 17  (ADDR : 0x9C00_CBC4)
    UINT32  RGST_Group0407_Reg18                  ; // 18  (ADDR : 0x9C00_CBC8)
    UINT32  RGST_Group0407_Reg19                  ; // 19  (ADDR : 0x9C00_CBCC)
    UINT32  RGST_Group0407_Reg20                  ; // 20  (ADDR : 0x9C00_CBD0)
    UINT32  RGST_Group0407_Reg21                  ; // 21  (ADDR : 0x9C00_CBD4)
    UINT32  RGST_Group0407_Reg22                  ; // 22  (ADDR : 0x9C00_CBD8)
    UINT32  RGST_Group0407_Reg23                  ; // 23  (ADDR : 0x9C00_CBDC)
    UINT32  RGST_Group0407_Reg24                  ; // 24  (ADDR : 0x9C00_CBE0)
    UINT32  RGST_Group0407_Reg25                  ; // 25  (ADDR : 0x9C00_CBE4)
    UINT32  RGST_Group0407_Reg26                  ; // 26  (ADDR : 0x9C00_CBE8)
    UINT32  RGST_Group0407_Reg27                  ; // 27  (ADDR : 0x9C00_CBEC)
    UINT32  RGST_Group0407_Reg28                  ; // 28  (ADDR : 0x9C00_CBF0)
    UINT32  RGST_Group0407_Reg29                  ; // 29  (ADDR : 0x9C00_CBF4)
    UINT32  RGST_Group0407_Reg30                  ; // 30  (ADDR : 0x9C00_CBF8)
    UINT32  RGST_Group0407_Reg31                  ; // 31  (ADDR : 0x9C00_CBFC)

    //Group   408 : MFD
    UINT32  RGST_Group0408_Reg00                  ; // 00  (ADDR : 0x9C00_CC00)
    UINT32  RGST_Group0408_Reg01                  ; // 01  (ADDR : 0x9C00_CC04)
    UINT32  RGST_Group0408_Reg02                  ; // 02  (ADDR : 0x9C00_CC08)
    UINT32  RGST_Group0408_Reg03                  ; // 03  (ADDR : 0x9C00_CC0C)
    UINT32  RGST_Group0408_Reg04                  ; // 04  (ADDR : 0x9C00_CC10)
    UINT32  RGST_Group0408_Reg05                  ; // 05  (ADDR : 0x9C00_CC14)
    UINT32  RGST_Group0408_Reg06                  ; // 06  (ADDR : 0x9C00_CC18)
    UINT32  RGST_Group0408_Reg07                  ; // 07  (ADDR : 0x9C00_CC1C)
    UINT32  RGST_Group0408_Reg08                  ; // 08  (ADDR : 0x9C00_CC20)
    UINT32  RGST_Group0408_Reg09                  ; // 09  (ADDR : 0x9C00_CC24)
    UINT32  RGST_Group0408_Reg10                  ; // 10  (ADDR : 0x9C00_CC28)
    UINT32  RGST_Group0408_Reg11                  ; // 11  (ADDR : 0x9C00_CC2C)
    UINT32  RGST_Group0408_Reg12                  ; // 12  (ADDR : 0x9C00_CC30)
    UINT32  RGST_Group0408_Reg13                  ; // 13  (ADDR : 0x9C00_CC34)
    UINT32  RGST_Group0408_Reg14                  ; // 14  (ADDR : 0x9C00_CC38)
    UINT32  RGST_Group0408_Reg15                  ; // 15  (ADDR : 0x9C00_CC3C)
    UINT32  RGST_Group0408_Reg16                  ; // 16  (ADDR : 0x9C00_CC40)
    UINT32  RGST_Group0408_Reg17                  ; // 17  (ADDR : 0x9C00_CC44)
    UINT32  RGST_Group0408_Reg18                  ; // 18  (ADDR : 0x9C00_CC48)
    UINT32  RGST_Group0408_Reg19                  ; // 19  (ADDR : 0x9C00_CC4C)
    UINT32  RGST_Group0408_Reg20                  ; // 20  (ADDR : 0x9C00_CC50)
    UINT32  RGST_Group0408_Reg21                  ; // 21  (ADDR : 0x9C00_CC54)
    UINT32  RGST_Group0408_Reg22                  ; // 22  (ADDR : 0x9C00_CC58)
    UINT32  RGST_Group0408_Reg23                  ; // 23  (ADDR : 0x9C00_CC5C)
    UINT32  RGST_Group0408_Reg24                  ; // 24  (ADDR : 0x9C00_CC60)
    UINT32  RGST_Group0408_Reg25                  ; // 25  (ADDR : 0x9C00_CC64)
    UINT32  RGST_Group0408_Reg26                  ; // 26  (ADDR : 0x9C00_CC68)
    UINT32  RGST_Group0408_Reg27                  ; // 27  (ADDR : 0x9C00_CC6C)
    UINT32  RGST_Group0408_Reg28                  ; // 28  (ADDR : 0x9C00_CC70)
    UINT32  RGST_Group0408_Reg29                  ; // 29  (ADDR : 0x9C00_CC74)
    UINT32  RGST_Group0408_Reg30                  ; // 30  (ADDR : 0x9C00_CC78)
    UINT32  RGST_Group0408_Reg31                  ; // 31  (ADDR : 0x9C00_CC7C)

    //Group   409 : MFD
    UINT32  RGST_Group0409_Reg00                  ; // 00  (ADDR : 0x9C00_CC80)
    UINT32  RGST_Group0409_Reg01                  ; // 01  (ADDR : 0x9C00_CC84)
    UINT32  RGST_Group0409_Reg02                  ; // 02  (ADDR : 0x9C00_CC88)
    UINT32  RGST_Group0409_Reg03                  ; // 03  (ADDR : 0x9C00_CC8C)
    UINT32  RGST_Group0409_Reg04                  ; // 04  (ADDR : 0x9C00_CC90)
    UINT32  RGST_Group0409_Reg05                  ; // 05  (ADDR : 0x9C00_CC94)
    UINT32  RGST_Group0409_Reg06                  ; // 06  (ADDR : 0x9C00_CC98)
    UINT32  RGST_Group0409_Reg07                  ; // 07  (ADDR : 0x9C00_CC9C)
    UINT32  RGST_Group0409_Reg08                  ; // 08  (ADDR : 0x9C00_CCA0)
    UINT32  RGST_Group0409_Reg09                  ; // 09  (ADDR : 0x9C00_CCA4)
    UINT32  RGST_Group0409_Reg10                  ; // 10  (ADDR : 0x9C00_CCA8)
    UINT32  RGST_Group0409_Reg11                  ; // 11  (ADDR : 0x9C00_CCAC)
    UINT32  RGST_Group0409_Reg12                  ; // 12  (ADDR : 0x9C00_CCB0)
    UINT32  RGST_Group0409_Reg13                  ; // 13  (ADDR : 0x9C00_CCB4)
    UINT32  RGST_Group0409_Reg14                  ; // 14  (ADDR : 0x9C00_CCB8)
    UINT32  RGST_Group0409_Reg15                  ; // 15  (ADDR : 0x9C00_CCBC)
    UINT32  RGST_Group0409_Reg16                  ; // 16  (ADDR : 0x9C00_CCC0)
    UINT32  RGST_Group0409_Reg17                  ; // 17  (ADDR : 0x9C00_CCC4)
    UINT32  RGST_Group0409_Reg18                  ; // 18  (ADDR : 0x9C00_CCC8)
    UINT32  RGST_Group0409_Reg19                  ; // 19  (ADDR : 0x9C00_CCCC)
    UINT32  RGST_Group0409_Reg20                  ; // 20  (ADDR : 0x9C00_CCD0)
    UINT32  RGST_Group0409_Reg21                  ; // 21  (ADDR : 0x9C00_CCD4)
    UINT32  RGST_Group0409_Reg22                  ; // 22  (ADDR : 0x9C00_CCD8)
    UINT32  RGST_Group0409_Reg23                  ; // 23  (ADDR : 0x9C00_CCDC)
    UINT32  RGST_Group0409_Reg24                  ; // 24  (ADDR : 0x9C00_CCE0)
    UINT32  RGST_Group0409_Reg25                  ; // 25  (ADDR : 0x9C00_CCE4)
    UINT32  RGST_Group0409_Reg26                  ; // 26  (ADDR : 0x9C00_CCE8)
    UINT32  RGST_Group0409_Reg27                  ; // 27  (ADDR : 0x9C00_CCEC)
    UINT32  RGST_Group0409_Reg28                  ; // 28  (ADDR : 0x9C00_CCF0)
    UINT32  RGST_Group0409_Reg29                  ; // 29  (ADDR : 0x9C00_CCF4)
    UINT32  RGST_Group0409_Reg30                  ; // 30  (ADDR : 0x9C00_CCF8)
    UINT32  RGST_Group0409_Reg31                  ; // 31  (ADDR : 0x9C00_CCFC)

    //Group   410 : MFD
    UINT32  RGST_Group0410_Reg00                  ; // 00  (ADDR : 0x9C00_CD00)
    UINT32  RGST_Group0410_Reg01                  ; // 01  (ADDR : 0x9C00_CD04)
    UINT32  RGST_Group0410_Reg02                  ; // 02  (ADDR : 0x9C00_CD08)
    UINT32  RGST_Group0410_Reg03                  ; // 03  (ADDR : 0x9C00_CD0C)
    UINT32  RGST_Group0410_Reg04                  ; // 04  (ADDR : 0x9C00_CD10)
    UINT32  RGST_Group0410_Reg05                  ; // 05  (ADDR : 0x9C00_CD14)
    UINT32  RGST_Group0410_Reg06                  ; // 06  (ADDR : 0x9C00_CD18)
    UINT32  RGST_Group0410_Reg07                  ; // 07  (ADDR : 0x9C00_CD1C)
    UINT32  RGST_Group0410_Reg08                  ; // 08  (ADDR : 0x9C00_CD20)
    UINT32  RGST_Group0410_Reg09                  ; // 09  (ADDR : 0x9C00_CD24)
    UINT32  RGST_Group0410_Reg10                  ; // 10  (ADDR : 0x9C00_CD28)
    UINT32  RGST_Group0410_Reg11                  ; // 11  (ADDR : 0x9C00_CD2C)
    UINT32  RGST_Group0410_Reg12                  ; // 12  (ADDR : 0x9C00_CD30)
    UINT32  RGST_Group0410_Reg13                  ; // 13  (ADDR : 0x9C00_CD34)
    UINT32  RGST_Group0410_Reg14                  ; // 14  (ADDR : 0x9C00_CD38)
    UINT32  RGST_Group0410_Reg15                  ; // 15  (ADDR : 0x9C00_CD3C)
    UINT32  RGST_Group0410_Reg16                  ; // 16  (ADDR : 0x9C00_CD40)
    UINT32  RGST_Group0410_Reg17                  ; // 17  (ADDR : 0x9C00_CD44)
    UINT32  RGST_Group0410_Reg18                  ; // 18  (ADDR : 0x9C00_CD48)
    UINT32  RGST_Group0410_Reg19                  ; // 19  (ADDR : 0x9C00_CD4C)
    UINT32  RGST_Group0410_Reg20                  ; // 20  (ADDR : 0x9C00_CD50)
    UINT32  RGST_Group0410_Reg21                  ; // 21  (ADDR : 0x9C00_CD54)
    UINT32  RGST_Group0410_Reg22                  ; // 22  (ADDR : 0x9C00_CD58)
    UINT32  RGST_Group0410_Reg23                  ; // 23  (ADDR : 0x9C00_CD5C)
    UINT32  RGST_Group0410_Reg24                  ; // 24  (ADDR : 0x9C00_CD60)
    UINT32  RGST_Group0410_Reg25                  ; // 25  (ADDR : 0x9C00_CD64)
    UINT32  RGST_Group0410_Reg26                  ; // 26  (ADDR : 0x9C00_CD68)
    UINT32  RGST_Group0410_Reg27                  ; // 27  (ADDR : 0x9C00_CD6C)
    UINT32  RGST_Group0410_Reg28                  ; // 28  (ADDR : 0x9C00_CD70)
    UINT32  RGST_Group0410_Reg29                  ; // 29  (ADDR : 0x9C00_CD74)
    UINT32  RGST_Group0410_Reg30                  ; // 30  (ADDR : 0x9C00_CD78)
    UINT32  RGST_Group0410_Reg31                  ; // 31  (ADDR : 0x9C00_CD7C)

    //Group   411 : MFD
    UINT32  RGST_Group0411_Reg00                  ; // 00  (ADDR : 0x9C00_CD80)
    UINT32  RGST_Group0411_Reg01                  ; // 01  (ADDR : 0x9C00_CD84)
    UINT32  RGST_Group0411_Reg02                  ; // 02  (ADDR : 0x9C00_CD88)
    UINT32  RGST_Group0411_Reg03                  ; // 03  (ADDR : 0x9C00_CD8C)
    UINT32  RGST_Group0411_Reg04                  ; // 04  (ADDR : 0x9C00_CD90)
    UINT32  RGST_Group0411_Reg05                  ; // 05  (ADDR : 0x9C00_CD94)
    UINT32  RGST_Group0411_Reg06                  ; // 06  (ADDR : 0x9C00_CD98)
    UINT32  RGST_Group0411_Reg07                  ; // 07  (ADDR : 0x9C00_CD9C)
    UINT32  RGST_Group0411_Reg08                  ; // 08  (ADDR : 0x9C00_CDA0)
    UINT32  RGST_Group0411_Reg09                  ; // 09  (ADDR : 0x9C00_CDA4)
    UINT32  RGST_Group0411_Reg10                  ; // 10  (ADDR : 0x9C00_CDA8)
    UINT32  RGST_Group0411_Reg11                  ; // 11  (ADDR : 0x9C00_CDAC)
    UINT32  RGST_Group0411_Reg12                  ; // 12  (ADDR : 0x9C00_CDB0)
    UINT32  RGST_Group0411_Reg13                  ; // 13  (ADDR : 0x9C00_CDB4)
    UINT32  RGST_Group0411_Reg14                  ; // 14  (ADDR : 0x9C00_CDB8)
    UINT32  RGST_Group0411_Reg15                  ; // 15  (ADDR : 0x9C00_CDBC)
    UINT32  RGST_Group0411_Reg16                  ; // 16  (ADDR : 0x9C00_CDC0)
    UINT32  RGST_Group0411_Reg17                  ; // 17  (ADDR : 0x9C00_CDC4)
    UINT32  RGST_Group0411_Reg18                  ; // 18  (ADDR : 0x9C00_CDC8)
    UINT32  RGST_Group0411_Reg19                  ; // 19  (ADDR : 0x9C00_CDCC)
    UINT32  RGST_Group0411_Reg20                  ; // 20  (ADDR : 0x9C00_CDD0)
    UINT32  RGST_Group0411_Reg21                  ; // 21  (ADDR : 0x9C00_CDD4)
    UINT32  RGST_Group0411_Reg22                  ; // 22  (ADDR : 0x9C00_CDD8)
    UINT32  RGST_Group0411_Reg23                  ; // 23  (ADDR : 0x9C00_CDDC)
    UINT32  RGST_Group0411_Reg24                  ; // 24  (ADDR : 0x9C00_CDE0)
    UINT32  RGST_Group0411_Reg25                  ; // 25  (ADDR : 0x9C00_CDE4)
    UINT32  RGST_Group0411_Reg26                  ; // 26  (ADDR : 0x9C00_CDE8)
    UINT32  RGST_Group0411_Reg27                  ; // 27  (ADDR : 0x9C00_CDEC)
    UINT32  RGST_Group0411_Reg28                  ; // 28  (ADDR : 0x9C00_CDF0)
    UINT32  RGST_Group0411_Reg29                  ; // 29  (ADDR : 0x9C00_CDF4)
    UINT32  RGST_Group0411_Reg30                  ; // 30  (ADDR : 0x9C00_CDF8)
    UINT32  RGST_Group0411_Reg31                  ; // 31  (ADDR : 0x9C00_CDFC)

    //Group   412 : MFD
    UINT32  RGST_Group0412_Reg00                  ; // 00  (ADDR : 0x9C00_CE00)
    UINT32  RGST_Group0412_Reg01                  ; // 01  (ADDR : 0x9C00_CE04)
    UINT32  RGST_Group0412_Reg02                  ; // 02  (ADDR : 0x9C00_CE08)
    UINT32  RGST_Group0412_Reg03                  ; // 03  (ADDR : 0x9C00_CE0C)
    UINT32  RGST_Group0412_Reg04                  ; // 04  (ADDR : 0x9C00_CE10)
    UINT32  RGST_Group0412_Reg05                  ; // 05  (ADDR : 0x9C00_CE14)
    UINT32  RGST_Group0412_Reg06                  ; // 06  (ADDR : 0x9C00_CE18)
    UINT32  RGST_Group0412_Reg07                  ; // 07  (ADDR : 0x9C00_CE1C)
    UINT32  RGST_Group0412_Reg08                  ; // 08  (ADDR : 0x9C00_CE20)
    UINT32  RGST_Group0412_Reg09                  ; // 09  (ADDR : 0x9C00_CE24)
    UINT32  RGST_Group0412_Reg10                  ; // 10  (ADDR : 0x9C00_CE28)
    UINT32  RGST_Group0412_Reg11                  ; // 11  (ADDR : 0x9C00_CE2C)
    UINT32  RGST_Group0412_Reg12                  ; // 12  (ADDR : 0x9C00_CE30)
    UINT32  RGST_Group0412_Reg13                  ; // 13  (ADDR : 0x9C00_CE34)
    UINT32  RGST_Group0412_Reg14                  ; // 14  (ADDR : 0x9C00_CE38)
    UINT32  RGST_Group0412_Reg15                  ; // 15  (ADDR : 0x9C00_CE3C)
    UINT32  RGST_Group0412_Reg16                  ; // 16  (ADDR : 0x9C00_CE40)
    UINT32  RGST_Group0412_Reg17                  ; // 17  (ADDR : 0x9C00_CE44)
    UINT32  RGST_Group0412_Reg18                  ; // 18  (ADDR : 0x9C00_CE48)
    UINT32  RGST_Group0412_Reg19                  ; // 19  (ADDR : 0x9C00_CE4C)
    UINT32  RGST_Group0412_Reg20                  ; // 20  (ADDR : 0x9C00_CE50)
    UINT32  RGST_Group0412_Reg21                  ; // 21  (ADDR : 0x9C00_CE54)
    UINT32  RGST_Group0412_Reg22                  ; // 22  (ADDR : 0x9C00_CE58)
    UINT32  RGST_Group0412_Reg23                  ; // 23  (ADDR : 0x9C00_CE5C)
    UINT32  RGST_Group0412_Reg24                  ; // 24  (ADDR : 0x9C00_CE60)
    UINT32  RGST_Group0412_Reg25                  ; // 25  (ADDR : 0x9C00_CE64)
    UINT32  RGST_Group0412_Reg26                  ; // 26  (ADDR : 0x9C00_CE68)
    UINT32  RGST_Group0412_Reg27                  ; // 27  (ADDR : 0x9C00_CE6C)
    UINT32  RGST_Group0412_Reg28                  ; // 28  (ADDR : 0x9C00_CE70)
    UINT32  RGST_Group0412_Reg29                  ; // 29  (ADDR : 0x9C00_CE74)
    UINT32  RGST_Group0412_Reg30                  ; // 30  (ADDR : 0x9C00_CE78)
    UINT32  RGST_Group0412_Reg31                  ; // 31  (ADDR : 0x9C00_CE7C)

    //Group   413 : MFD
    UINT32  RGST_Group0413_Reg00                  ; // 00  (ADDR : 0x9C00_CE80)
    UINT32  RGST_Group0413_Reg01                  ; // 01  (ADDR : 0x9C00_CE84)
    UINT32  RGST_Group0413_Reg02                  ; // 02  (ADDR : 0x9C00_CE88)
    UINT32  RGST_Group0413_Reg03                  ; // 03  (ADDR : 0x9C00_CE8C)
    UINT32  RGST_Group0413_Reg04                  ; // 04  (ADDR : 0x9C00_CE90)
    UINT32  RGST_Group0413_Reg05                  ; // 05  (ADDR : 0x9C00_CE94)
    UINT32  RGST_Group0413_Reg06                  ; // 06  (ADDR : 0x9C00_CE98)
    UINT32  RGST_Group0413_Reg07                  ; // 07  (ADDR : 0x9C00_CE9C)
    UINT32  RGST_Group0413_Reg08                  ; // 08  (ADDR : 0x9C00_CEA0)
    UINT32  RGST_Group0413_Reg09                  ; // 09  (ADDR : 0x9C00_CEA4)
    UINT32  RGST_Group0413_Reg10                  ; // 10  (ADDR : 0x9C00_CEA8)
    UINT32  RGST_Group0413_Reg11                  ; // 11  (ADDR : 0x9C00_CEAC)
    UINT32  RGST_Group0413_Reg12                  ; // 12  (ADDR : 0x9C00_CEB0)
    UINT32  RGST_Group0413_Reg13                  ; // 13  (ADDR : 0x9C00_CEB4)
    UINT32  RGST_Group0413_Reg14                  ; // 14  (ADDR : 0x9C00_CEB8)
    UINT32  RGST_Group0413_Reg15                  ; // 15  (ADDR : 0x9C00_CEBC)
    UINT32  RGST_Group0413_Reg16                  ; // 16  (ADDR : 0x9C00_CEC0)
    UINT32  RGST_Group0413_Reg17                  ; // 17  (ADDR : 0x9C00_CEC4)
    UINT32  RGST_Group0413_Reg18                  ; // 18  (ADDR : 0x9C00_CEC8)
    UINT32  RGST_Group0413_Reg19                  ; // 19  (ADDR : 0x9C00_CECC)
    UINT32  RGST_Group0413_Reg20                  ; // 20  (ADDR : 0x9C00_CED0)
    UINT32  RGST_Group0413_Reg21                  ; // 21  (ADDR : 0x9C00_CED4)
    UINT32  RGST_Group0413_Reg22                  ; // 22  (ADDR : 0x9C00_CED8)
    UINT32  RGST_Group0413_Reg23                  ; // 23  (ADDR : 0x9C00_CEDC)
    UINT32  RGST_Group0413_Reg24                  ; // 24  (ADDR : 0x9C00_CEE0)
    UINT32  RGST_Group0413_Reg25                  ; // 25  (ADDR : 0x9C00_CEE4)
    UINT32  RGST_Group0413_Reg26                  ; // 26  (ADDR : 0x9C00_CEE8)
    UINT32  RGST_Group0413_Reg27                  ; // 27  (ADDR : 0x9C00_CEEC)
    UINT32  RGST_Group0413_Reg28                  ; // 28  (ADDR : 0x9C00_CEF0)
    UINT32  RGST_Group0413_Reg29                  ; // 29  (ADDR : 0x9C00_CEF4)
    UINT32  RGST_Group0413_Reg30                  ; // 30  (ADDR : 0x9C00_CEF8)
    UINT32  RGST_Group0413_Reg31                  ; // 31  (ADDR : 0x9C00_CEFC)

    //Group   414 : MFD
    UINT32  RGST_Group0414_Reg00                  ; // 00  (ADDR : 0x9C00_CF00)
    UINT32  RGST_Group0414_Reg01                  ; // 01  (ADDR : 0x9C00_CF04)
    UINT32  RGST_Group0414_Reg02                  ; // 02  (ADDR : 0x9C00_CF08)
    UINT32  RGST_Group0414_Reg03                  ; // 03  (ADDR : 0x9C00_CF0C)
    UINT32  RGST_Group0414_Reg04                  ; // 04  (ADDR : 0x9C00_CF10)
    UINT32  RGST_Group0414_Reg05                  ; // 05  (ADDR : 0x9C00_CF14)
    UINT32  RGST_Group0414_Reg06                  ; // 06  (ADDR : 0x9C00_CF18)
    UINT32  RGST_Group0414_Reg07                  ; // 07  (ADDR : 0x9C00_CF1C)
    UINT32  RGST_Group0414_Reg08                  ; // 08  (ADDR : 0x9C00_CF20)
    UINT32  RGST_Group0414_Reg09                  ; // 09  (ADDR : 0x9C00_CF24)
    UINT32  RGST_Group0414_Reg10                  ; // 10  (ADDR : 0x9C00_CF28)
    UINT32  RGST_Group0414_Reg11                  ; // 11  (ADDR : 0x9C00_CF2C)
    UINT32  RGST_Group0414_Reg12                  ; // 12  (ADDR : 0x9C00_CF30)
    UINT32  RGST_Group0414_Reg13                  ; // 13  (ADDR : 0x9C00_CF34)
    UINT32  RGST_Group0414_Reg14                  ; // 14  (ADDR : 0x9C00_CF38)
    UINT32  RGST_Group0414_Reg15                  ; // 15  (ADDR : 0x9C00_CF3C)
    UINT32  RGST_Group0414_Reg16                  ; // 16  (ADDR : 0x9C00_CF40)
    UINT32  RGST_Group0414_Reg17                  ; // 17  (ADDR : 0x9C00_CF44)
    UINT32  RGST_Group0414_Reg18                  ; // 18  (ADDR : 0x9C00_CF48)
    UINT32  RGST_Group0414_Reg19                  ; // 19  (ADDR : 0x9C00_CF4C)
    UINT32  RGST_Group0414_Reg20                  ; // 20  (ADDR : 0x9C00_CF50)
    UINT32  RGST_Group0414_Reg21                  ; // 21  (ADDR : 0x9C00_CF54)
    UINT32  RGST_Group0414_Reg22                  ; // 22  (ADDR : 0x9C00_CF58)
    UINT32  RGST_Group0414_Reg23                  ; // 23  (ADDR : 0x9C00_CF5C)
    UINT32  RGST_Group0414_Reg24                  ; // 24  (ADDR : 0x9C00_CF60)
    UINT32  RGST_Group0414_Reg25                  ; // 25  (ADDR : 0x9C00_CF64)
    UINT32  RGST_Group0414_Reg26                  ; // 26  (ADDR : 0x9C00_CF68)
    UINT32  RGST_Group0414_Reg27                  ; // 27  (ADDR : 0x9C00_CF6C)
    UINT32  RGST_Group0414_Reg28                  ; // 28  (ADDR : 0x9C00_CF70)
    UINT32  RGST_Group0414_Reg29                  ; // 29  (ADDR : 0x9C00_CF74)
    UINT32  RGST_Group0414_Reg30                  ; // 30  (ADDR : 0x9C00_CF78)
    UINT32  RGST_Group0414_Reg31                  ; // 31  (ADDR : 0x9C00_CF7C)

    //Group   415 : MFD
    UINT32  RGST_Group0415_Reg00                  ; // 00  (ADDR : 0x9C00_CF80)
    UINT32  RGST_Group0415_Reg01                  ; // 01  (ADDR : 0x9C00_CF84)
    UINT32  RGST_Group0415_Reg02                  ; // 02  (ADDR : 0x9C00_CF88)
    UINT32  RGST_Group0415_Reg03                  ; // 03  (ADDR : 0x9C00_CF8C)
    UINT32  RGST_Group0415_Reg04                  ; // 04  (ADDR : 0x9C00_CF90)
    UINT32  RGST_Group0415_Reg05                  ; // 05  (ADDR : 0x9C00_CF94)
    UINT32  RGST_Group0415_Reg06                  ; // 06  (ADDR : 0x9C00_CF98)
    UINT32  RGST_Group0415_Reg07                  ; // 07  (ADDR : 0x9C00_CF9C)
    UINT32  RGST_Group0415_Reg08                  ; // 08  (ADDR : 0x9C00_CFA0)
    UINT32  RGST_Group0415_Reg09                  ; // 09  (ADDR : 0x9C00_CFA4)
    UINT32  RGST_Group0415_Reg10                  ; // 10  (ADDR : 0x9C00_CFA8)
    UINT32  RGST_Group0415_Reg11                  ; // 11  (ADDR : 0x9C00_CFAC)
    UINT32  RGST_Group0415_Reg12                  ; // 12  (ADDR : 0x9C00_CFB0)
    UINT32  RGST_Group0415_Reg13                  ; // 13  (ADDR : 0x9C00_CFB4)
    UINT32  RGST_Group0415_Reg14                  ; // 14  (ADDR : 0x9C00_CFB8)
    UINT32  RGST_Group0415_Reg15                  ; // 15  (ADDR : 0x9C00_CFBC)
    UINT32  RGST_Group0415_Reg16                  ; // 16  (ADDR : 0x9C00_CFC0)
    UINT32  RGST_Group0415_Reg17                  ; // 17  (ADDR : 0x9C00_CFC4)
    UINT32  RGST_Group0415_Reg18                  ; // 18  (ADDR : 0x9C00_CFC8)
    UINT32  RGST_Group0415_Reg19                  ; // 19  (ADDR : 0x9C00_CFCC)
    UINT32  RGST_Group0415_Reg20                  ; // 20  (ADDR : 0x9C00_CFD0)
    UINT32  RGST_Group0415_Reg21                  ; // 21  (ADDR : 0x9C00_CFD4)
    UINT32  RGST_Group0415_Reg22                  ; // 22  (ADDR : 0x9C00_CFD8)
    UINT32  RGST_Group0415_Reg23                  ; // 23  (ADDR : 0x9C00_CFDC)
    UINT32  RGST_Group0415_Reg24                  ; // 24  (ADDR : 0x9C00_CFE0)
    UINT32  RGST_Group0415_Reg25                  ; // 25  (ADDR : 0x9C00_CFE4)
    UINT32  RGST_Group0415_Reg26                  ; // 26  (ADDR : 0x9C00_CFE8)
    UINT32  RGST_Group0415_Reg27                  ; // 27  (ADDR : 0x9C00_CFEC)
    UINT32  RGST_Group0415_Reg28                  ; // 28  (ADDR : 0x9C00_CFF0)
    UINT32  RGST_Group0415_Reg29                  ; // 29  (ADDR : 0x9C00_CFF4)
    UINT32  RGST_Group0415_Reg30                  ; // 30  (ADDR : 0x9C00_CFF8)
    UINT32  RGST_Group0415_Reg31                  ; // 31  (ADDR : 0x9C00_CFFC)

    //Group   416 : MFD
    UINT32  SL_START                              ; // 00  (ADDR : 0x9C00_D000)
    UINT32  PROFILE                               ; // 01  (ADDR : 0x9C00_D004)
    UINT32  HSIZE_ADD                             ; // 02  (ADDR : 0x9C00_D008)
    UINT32  VSIZE_ADD                             ; // 03  (ADDR : 0x9C00_D00C)
    UINT32  CHR_IDX_OFF                           ; // 04  (ADDR : 0x9C00_D010)
    UINT32  CS_IPMD                               ; // 05  (ADDR : 0x9C00_D014)
    UINT32  FPIC_FLAG                             ; // 06  (ADDR : 0x9C00_D018)
    UINT32  TFLD_FLAG                             ; // 07  (ADDR : 0x9C00_D01C)
    UINT32  MBFF_MD                               ; // 08  (ADDR : 0x9C00_D020)
    UINT32  SL_TYPE                               ; // 09  (ADDR : 0x9C00_D024)
    UINT32  BASE_REG                              ; // 10  (ADDR : 0x9C00_D028)
    UINT32  FMBASE_REG                            ; // 11  (ADDR : 0x9C00_D02C)
    UINT32  VLC_IRQ_REG                           ; // 12  (ADDR : 0x9C00_D030)
    UINT32  VLC_IRQ_MASK                          ; // 13  (ADDR : 0x9C00_D034)
    UINT32  FMO_IRQ                               ; // 14  (ADDR : 0x9C00_D038)
    UINT32  FMO_IRQMASK                           ; // 15  (ADDR : 0x9C00_D03C)
    UINT32  SL_RST                                ; // 16  (ADDR : 0x9C00_D040)
    UINT32  HW_SOFT_RST                           ; // 17  (ADDR : 0x9C00_D044)
    UINT32  H_PIC_START                           ; // 18  (ADDR : 0x9C00_D048)
    UINT32  reserved_G416[13]                     ; // 19  (ADDR : 0x9C00_D04C) ~ 31  (ADDR : 0x9C00_D07C)

    //Group   417 : MFD
    UINT32  CRF_FLAG                              ; // 00  (ADDR : 0x9C00_D080)
    UINT32  ENTROPY_FLAG                          ; // 01  (ADDR : 0x9C00_D084)
    UINT32  FIRST_ROW                             ; // 02  (ADDR : 0x9C00_D088)
    UINT32  FIRST_COL                             ; // 03  (ADDR : 0x9C00_D08C)
    UINT32  SPMV                                  ; // 04  (ADDR : 0x9C00_D090)
    UINT32  IDX0_SIZE                             ; // 05  (ADDR : 0x9C00_D094)
    UINT32  IDX1_SIZE                             ; // 06  (ADDR : 0x9C00_D098)
    UINT32  INIT_IDC                              ; // 07  (ADDR : 0x9C00_D09C)
    UINT32  INIT_QT                               ; // 08  (ADDR : 0x9C00_D0A0)
    UINT32  PTYPE_L1                              ; // 09  (ADDR : 0x9C00_D0A4)
    UINT32  MB_COUNT                              ; // 10  (ADDR : 0x9C00_D0A8)
    UINT32  VLC_STUS                              ; // 11  (ADDR : 0x9C00_D0AC)
    UINT32  SP_TIMEOUT                            ; // 12  (ADDR : 0x9C00_D0B0)
    UINT32  ERRC_MASK                             ; // 13  (ADDR : 0x9C00_D0B4)
    UINT32  reserved_G417[18]                     ; // 14  (ADDR : 0x9C00_D0B8) ~ 31  (ADDR : 0x9C00_D0FC)

    //Group   418 : MFD
    UINT32  H264_MP_ADDR2[32]                     ; // 00  (ADDR : 0x9C00_D100) ~ 31  (ADDR : 0x9C00_D17C)

    //Group   419 : MFD
    UINT32  RGST_Group0419_Reg00                  ; // 00  (ADDR : 0x9C00_D180)
    UINT32  RGST_Group0419_Reg01                  ; // 01  (ADDR : 0x9C00_D184)
    UINT32  RGST_Group0419_Reg02                  ; // 02  (ADDR : 0x9C00_D188)
    UINT32  RGST_Group0419_Reg03                  ; // 03  (ADDR : 0x9C00_D18C)
    UINT32  RGST_Group0419_Reg04                  ; // 04  (ADDR : 0x9C00_D190)
    UINT32  RGST_Group0419_Reg05                  ; // 05  (ADDR : 0x9C00_D194)
    UINT32  RGST_Group0419_Reg06                  ; // 06  (ADDR : 0x9C00_D198)
    UINT32  RGST_Group0419_Reg07                  ; // 07  (ADDR : 0x9C00_D19C)
    UINT32  RGST_Group0419_Reg08                  ; // 08  (ADDR : 0x9C00_D1A0)
    UINT32  RGST_Group0419_Reg09                  ; // 09  (ADDR : 0x9C00_D1A4)
    UINT32  RGST_Group0419_Reg10                  ; // 10  (ADDR : 0x9C00_D1A8)
    UINT32  RGST_Group0419_Reg11                  ; // 11  (ADDR : 0x9C00_D1AC)
    UINT32  RGST_Group0419_Reg12                  ; // 12  (ADDR : 0x9C00_D1B0)
    UINT32  RGST_Group0419_Reg13                  ; // 13  (ADDR : 0x9C00_D1B4)
    UINT32  RGST_Group0419_Reg14                  ; // 14  (ADDR : 0x9C00_D1B8)
    UINT32  RGST_Group0419_Reg15                  ; // 15  (ADDR : 0x9C00_D1BC)
    UINT32  RGST_Group0419_Reg16                  ; // 16  (ADDR : 0x9C00_D1C0)
    UINT32  RGST_Group0419_Reg17                  ; // 17  (ADDR : 0x9C00_D1C4)
    UINT32  RGST_Group0419_Reg18                  ; // 18  (ADDR : 0x9C00_D1C8)
    UINT32  RGST_Group0419_Reg19                  ; // 19  (ADDR : 0x9C00_D1CC)
    UINT32  RGST_Group0419_Reg20                  ; // 20  (ADDR : 0x9C00_D1D0)
    UINT32  RGST_Group0419_Reg21                  ; // 21  (ADDR : 0x9C00_D1D4)
    UINT32  RGST_Group0419_Reg22                  ; // 22  (ADDR : 0x9C00_D1D8)
    UINT32  RGST_Group0419_Reg23                  ; // 23  (ADDR : 0x9C00_D1DC)
    UINT32  RGST_Group0419_Reg24                  ; // 24  (ADDR : 0x9C00_D1E0)
    UINT32  RGST_Group0419_Reg25                  ; // 25  (ADDR : 0x9C00_D1E4)
    UINT32  RGST_Group0419_Reg26                  ; // 26  (ADDR : 0x9C00_D1E8)
    UINT32  RGST_Group0419_Reg27                  ; // 27  (ADDR : 0x9C00_D1EC)
    UINT32  RGST_Group0419_Reg28                  ; // 28  (ADDR : 0x9C00_D1F0)
    UINT32  RGST_Group0419_Reg29                  ; // 29  (ADDR : 0x9C00_D1F4)
    UINT32  RGST_Group0419_Reg30                  ; // 30  (ADDR : 0x9C00_D1F8)
    UINT32  RGST_Group0419_Reg31                  ; // 31  (ADDR : 0x9C00_D1FC)

    //Group   420 : MFD
    UINT32  RGST_Group0420_Reg00                  ; // 00  (ADDR : 0x9C00_D200)
    UINT32  RGST_Group0420_Reg01                  ; // 01  (ADDR : 0x9C00_D204)
    UINT32  RGST_Group0420_Reg02                  ; // 02  (ADDR : 0x9C00_D208)
    UINT32  RGST_Group0420_Reg03                  ; // 03  (ADDR : 0x9C00_D20C)
    UINT32  RGST_Group0420_Reg04                  ; // 04  (ADDR : 0x9C00_D210)
    UINT32  RGST_Group0420_Reg05                  ; // 05  (ADDR : 0x9C00_D214)
    UINT32  RGST_Group0420_Reg06                  ; // 06  (ADDR : 0x9C00_D218)
    UINT32  RGST_Group0420_Reg07                  ; // 07  (ADDR : 0x9C00_D21C)
    UINT32  RGST_Group0420_Reg08                  ; // 08  (ADDR : 0x9C00_D220)
    UINT32  RGST_Group0420_Reg09                  ; // 09  (ADDR : 0x9C00_D224)
    UINT32  RGST_Group0420_Reg10                  ; // 10  (ADDR : 0x9C00_D228)
    UINT32  RGST_Group0420_Reg11                  ; // 11  (ADDR : 0x9C00_D22C)
    UINT32  RGST_Group0420_Reg12                  ; // 12  (ADDR : 0x9C00_D230)
    UINT32  RGST_Group0420_Reg13                  ; // 13  (ADDR : 0x9C00_D234)
    UINT32  RGST_Group0420_Reg14                  ; // 14  (ADDR : 0x9C00_D238)
    UINT32  RGST_Group0420_Reg15                  ; // 15  (ADDR : 0x9C00_D23C)
    UINT32  RGST_Group0420_Reg16                  ; // 16  (ADDR : 0x9C00_D240)
    UINT32  RGST_Group0420_Reg17                  ; // 17  (ADDR : 0x9C00_D244)
    UINT32  RGST_Group0420_Reg18                  ; // 18  (ADDR : 0x9C00_D248)
    UINT32  RGST_Group0420_Reg19                  ; // 19  (ADDR : 0x9C00_D24C)
    UINT32  RGST_Group0420_Reg20                  ; // 20  (ADDR : 0x9C00_D250)
    UINT32  RGST_Group0420_Reg21                  ; // 21  (ADDR : 0x9C00_D254)
    UINT32  RGST_Group0420_Reg22                  ; // 22  (ADDR : 0x9C00_D258)
    UINT32  RGST_Group0420_Reg23                  ; // 23  (ADDR : 0x9C00_D25C)
    UINT32  RGST_Group0420_Reg24                  ; // 24  (ADDR : 0x9C00_D260)
    UINT32  RGST_Group0420_Reg25                  ; // 25  (ADDR : 0x9C00_D264)
    UINT32  RGST_Group0420_Reg26                  ; // 26  (ADDR : 0x9C00_D268)
    UINT32  RGST_Group0420_Reg27                  ; // 27  (ADDR : 0x9C00_D26C)
    UINT32  RGST_Group0420_Reg28                  ; // 28  (ADDR : 0x9C00_D270)
    UINT32  RGST_Group0420_Reg29                  ; // 29  (ADDR : 0x9C00_D274)
    UINT32  RGST_Group0420_Reg30                  ; // 30  (ADDR : 0x9C00_D278)
    UINT32  RGST_Group0420_Reg31                  ; // 31  (ADDR : 0x9C00_D27C)

    //Group   421 : MFD
    UINT32  RGST_Group0421_Reg00                  ; // 00  (ADDR : 0x9C00_D280)
    UINT32  RGST_Group0421_Reg01                  ; // 01  (ADDR : 0x9C00_D284)
    UINT32  RGST_Group0421_Reg02                  ; // 02  (ADDR : 0x9C00_D288)
    UINT32  RGST_Group0421_Reg03                  ; // 03  (ADDR : 0x9C00_D28C)
    UINT32  RGST_Group0421_Reg04                  ; // 04  (ADDR : 0x9C00_D290)
    UINT32  RGST_Group0421_Reg05                  ; // 05  (ADDR : 0x9C00_D294)
    UINT32  RGST_Group0421_Reg06                  ; // 06  (ADDR : 0x9C00_D298)
    UINT32  RGST_Group0421_Reg07                  ; // 07  (ADDR : 0x9C00_D29C)
    UINT32  RGST_Group0421_Reg08                  ; // 08  (ADDR : 0x9C00_D2A0)
    UINT32  RGST_Group0421_Reg09                  ; // 09  (ADDR : 0x9C00_D2A4)
    UINT32  RGST_Group0421_Reg10                  ; // 10  (ADDR : 0x9C00_D2A8)
    UINT32  RGST_Group0421_Reg11                  ; // 11  (ADDR : 0x9C00_D2AC)
    UINT32  RGST_Group0421_Reg12                  ; // 12  (ADDR : 0x9C00_D2B0)
    UINT32  RGST_Group0421_Reg13                  ; // 13  (ADDR : 0x9C00_D2B4)
    UINT32  RGST_Group0421_Reg14                  ; // 14  (ADDR : 0x9C00_D2B8)
    UINT32  RGST_Group0421_Reg15                  ; // 15  (ADDR : 0x9C00_D2BC)
    UINT32  RGST_Group0421_Reg16                  ; // 16  (ADDR : 0x9C00_D2C0)
    UINT32  RGST_Group0421_Reg17                  ; // 17  (ADDR : 0x9C00_D2C4)
    UINT32  RGST_Group0421_Reg18                  ; // 18  (ADDR : 0x9C00_D2C8)
    UINT32  RGST_Group0421_Reg19                  ; // 19  (ADDR : 0x9C00_D2CC)
    UINT32  RGST_Group0421_Reg20                  ; // 20  (ADDR : 0x9C00_D2D0)
    UINT32  RGST_Group0421_Reg21                  ; // 21  (ADDR : 0x9C00_D2D4)
    UINT32  RGST_Group0421_Reg22                  ; // 22  (ADDR : 0x9C00_D2D8)
    UINT32  RGST_Group0421_Reg23                  ; // 23  (ADDR : 0x9C00_D2DC)
    UINT32  RGST_Group0421_Reg24                  ; // 24  (ADDR : 0x9C00_D2E0)
    UINT32  RGST_Group0421_Reg25                  ; // 25  (ADDR : 0x9C00_D2E4)
    UINT32  RGST_Group0421_Reg26                  ; // 26  (ADDR : 0x9C00_D2E8)
    UINT32  RGST_Group0421_Reg27                  ; // 27  (ADDR : 0x9C00_D2EC)
    UINT32  RGST_Group0421_Reg28                  ; // 28  (ADDR : 0x9C00_D2F0)
    UINT32  RGST_Group0421_Reg29                  ; // 29  (ADDR : 0x9C00_D2F4)
    UINT32  RGST_Group0421_Reg30                  ; // 30  (ADDR : 0x9C00_D2F8)
    UINT32  RGST_Group0421_Reg31                  ; // 31  (ADDR : 0x9C00_D2FC)

    //Group   422 : MFD
    UINT32  RGST_Group0422_Reg00                  ; // 00  (ADDR : 0x9C00_D300)
    UINT32  RGST_Group0422_Reg01                  ; // 01  (ADDR : 0x9C00_D304)
    UINT32  RGST_Group0422_Reg02                  ; // 02  (ADDR : 0x9C00_D308)
    UINT32  RGST_Group0422_Reg03                  ; // 03  (ADDR : 0x9C00_D30C)
    UINT32  RGST_Group0422_Reg04                  ; // 04  (ADDR : 0x9C00_D310)
    UINT32  RGST_Group0422_Reg05                  ; // 05  (ADDR : 0x9C00_D314)
    UINT32  RGST_Group0422_Reg06                  ; // 06  (ADDR : 0x9C00_D318)
    UINT32  RGST_Group0422_Reg07                  ; // 07  (ADDR : 0x9C00_D31C)
    UINT32  RGST_Group0422_Reg08                  ; // 08  (ADDR : 0x9C00_D320)
    UINT32  RGST_Group0422_Reg09                  ; // 09  (ADDR : 0x9C00_D324)
    UINT32  RGST_Group0422_Reg10                  ; // 10  (ADDR : 0x9C00_D328)
    UINT32  RGST_Group0422_Reg11                  ; // 11  (ADDR : 0x9C00_D32C)
    UINT32  RGST_Group0422_Reg12                  ; // 12  (ADDR : 0x9C00_D330)
    UINT32  RGST_Group0422_Reg13                  ; // 13  (ADDR : 0x9C00_D334)
    UINT32  RGST_Group0422_Reg14                  ; // 14  (ADDR : 0x9C00_D338)
    UINT32  RGST_Group0422_Reg15                  ; // 15  (ADDR : 0x9C00_D33C)
    UINT32  RGST_Group0422_Reg16                  ; // 16  (ADDR : 0x9C00_D340)
    UINT32  RGST_Group0422_Reg17                  ; // 17  (ADDR : 0x9C00_D344)
    UINT32  RGST_Group0422_Reg18                  ; // 18  (ADDR : 0x9C00_D348)
    UINT32  RGST_Group0422_Reg19                  ; // 19  (ADDR : 0x9C00_D34C)
    UINT32  RGST_Group0422_Reg20                  ; // 20  (ADDR : 0x9C00_D350)
    UINT32  RGST_Group0422_Reg21                  ; // 21  (ADDR : 0x9C00_D354)
    UINT32  RGST_Group0422_Reg22                  ; // 22  (ADDR : 0x9C00_D358)
    UINT32  RGST_Group0422_Reg23                  ; // 23  (ADDR : 0x9C00_D35C)
    UINT32  RGST_Group0422_Reg24                  ; // 24  (ADDR : 0x9C00_D360)
    UINT32  RGST_Group0422_Reg25                  ; // 25  (ADDR : 0x9C00_D364)
    UINT32  RGST_Group0422_Reg26                  ; // 26  (ADDR : 0x9C00_D368)
    UINT32  RGST_Group0422_Reg27                  ; // 27  (ADDR : 0x9C00_D36C)
    UINT32  RGST_Group0422_Reg28                  ; // 28  (ADDR : 0x9C00_D370)
    UINT32  RGST_Group0422_Reg29                  ; // 29  (ADDR : 0x9C00_D374)
    UINT32  RGST_Group0422_Reg30                  ; // 30  (ADDR : 0x9C00_D378)
    UINT32  RGST_Group0422_Reg31                  ; // 31  (ADDR : 0x9C00_D37C)

    //Group   423 : MFD
    UINT32  RGST_Group0423_Reg00                  ; // 00  (ADDR : 0x9C00_D380)
    UINT32  RGST_Group0423_Reg01                  ; // 01  (ADDR : 0x9C00_D384)
    UINT32  RGST_Group0423_Reg02                  ; // 02  (ADDR : 0x9C00_D388)
    UINT32  RGST_Group0423_Reg03                  ; // 03  (ADDR : 0x9C00_D38C)
    UINT32  RGST_Group0423_Reg04                  ; // 04  (ADDR : 0x9C00_D390)
    UINT32  RGST_Group0423_Reg05                  ; // 05  (ADDR : 0x9C00_D394)
    UINT32  RGST_Group0423_Reg06                  ; // 06  (ADDR : 0x9C00_D398)
    UINT32  RGST_Group0423_Reg07                  ; // 07  (ADDR : 0x9C00_D39C)
    UINT32  RGST_Group0423_Reg08                  ; // 08  (ADDR : 0x9C00_D3A0)
    UINT32  RGST_Group0423_Reg09                  ; // 09  (ADDR : 0x9C00_D3A4)
    UINT32  RGST_Group0423_Reg10                  ; // 10  (ADDR : 0x9C00_D3A8)
    UINT32  RGST_Group0423_Reg11                  ; // 11  (ADDR : 0x9C00_D3AC)
    UINT32  RGST_Group0423_Reg12                  ; // 12  (ADDR : 0x9C00_D3B0)
    UINT32  RGST_Group0423_Reg13                  ; // 13  (ADDR : 0x9C00_D3B4)
    UINT32  RGST_Group0423_Reg14                  ; // 14  (ADDR : 0x9C00_D3B8)
    UINT32  RGST_Group0423_Reg15                  ; // 15  (ADDR : 0x9C00_D3BC)
    UINT32  RGST_Group0423_Reg16                  ; // 16  (ADDR : 0x9C00_D3C0)
    UINT32  RGST_Group0423_Reg17                  ; // 17  (ADDR : 0x9C00_D3C4)
    UINT32  RGST_Group0423_Reg18                  ; // 18  (ADDR : 0x9C00_D3C8)
    UINT32  RGST_Group0423_Reg19                  ; // 19  (ADDR : 0x9C00_D3CC)
    UINT32  RGST_Group0423_Reg20                  ; // 20  (ADDR : 0x9C00_D3D0)
    UINT32  RGST_Group0423_Reg21                  ; // 21  (ADDR : 0x9C00_D3D4)
    UINT32  RGST_Group0423_Reg22                  ; // 22  (ADDR : 0x9C00_D3D8)
    UINT32  RGST_Group0423_Reg23                  ; // 23  (ADDR : 0x9C00_D3DC)
    UINT32  RGST_Group0423_Reg24                  ; // 24  (ADDR : 0x9C00_D3E0)
    UINT32  RGST_Group0423_Reg25                  ; // 25  (ADDR : 0x9C00_D3E4)
    UINT32  RGST_Group0423_Reg26                  ; // 26  (ADDR : 0x9C00_D3E8)
    UINT32  RGST_Group0423_Reg27                  ; // 27  (ADDR : 0x9C00_D3EC)
    UINT32  RGST_Group0423_Reg28                  ; // 28  (ADDR : 0x9C00_D3F0)
    UINT32  RGST_Group0423_Reg29                  ; // 29  (ADDR : 0x9C00_D3F4)
    UINT32  RGST_Group0423_Reg30                  ; // 30  (ADDR : 0x9C00_D3F8)
    UINT32  RGST_Group0423_Reg31                  ; // 31  (ADDR : 0x9C00_D3FC)

    //Group   424 : MFD
    UINT32  RGST_Group0424_Reg00                  ; // 00  (ADDR : 0x9C00_D400)
    UINT32  RGST_Group0424_Reg01                  ; // 01  (ADDR : 0x9C00_D404)
    UINT32  RGST_Group0424_Reg02                  ; // 02  (ADDR : 0x9C00_D408)
    UINT32  RGST_Group0424_Reg03                  ; // 03  (ADDR : 0x9C00_D40C)
    UINT32  RGST_Group0424_Reg04                  ; // 04  (ADDR : 0x9C00_D410)
    UINT32  RGST_Group0424_Reg05                  ; // 05  (ADDR : 0x9C00_D414)
    UINT32  RGST_Group0424_Reg06                  ; // 06  (ADDR : 0x9C00_D418)
    UINT32  RGST_Group0424_Reg07                  ; // 07  (ADDR : 0x9C00_D41C)
    UINT32  RGST_Group0424_Reg08                  ; // 08  (ADDR : 0x9C00_D420)
    UINT32  RGST_Group0424_Reg09                  ; // 09  (ADDR : 0x9C00_D424)
    UINT32  RGST_Group0424_Reg10                  ; // 10  (ADDR : 0x9C00_D428)
    UINT32  RGST_Group0424_Reg11                  ; // 11  (ADDR : 0x9C00_D42C)
    UINT32  RGST_Group0424_Reg12                  ; // 12  (ADDR : 0x9C00_D430)
    UINT32  RGST_Group0424_Reg13                  ; // 13  (ADDR : 0x9C00_D434)
    UINT32  RGST_Group0424_Reg14                  ; // 14  (ADDR : 0x9C00_D438)
    UINT32  RGST_Group0424_Reg15                  ; // 15  (ADDR : 0x9C00_D43C)
    UINT32  RGST_Group0424_Reg16                  ; // 16  (ADDR : 0x9C00_D440)
    UINT32  RGST_Group0424_Reg17                  ; // 17  (ADDR : 0x9C00_D444)
    UINT32  RGST_Group0424_Reg18                  ; // 18  (ADDR : 0x9C00_D448)
    UINT32  RGST_Group0424_Reg19                  ; // 19  (ADDR : 0x9C00_D44C)
    UINT32  RGST_Group0424_Reg20                  ; // 20  (ADDR : 0x9C00_D450)
    UINT32  RGST_Group0424_Reg21                  ; // 21  (ADDR : 0x9C00_D454)
    UINT32  RGST_Group0424_Reg22                  ; // 22  (ADDR : 0x9C00_D458)
    UINT32  RGST_Group0424_Reg23                  ; // 23  (ADDR : 0x9C00_D45C)
    UINT32  RGST_Group0424_Reg24                  ; // 24  (ADDR : 0x9C00_D460)
    UINT32  RGST_Group0424_Reg25                  ; // 25  (ADDR : 0x9C00_D464)
    UINT32  RGST_Group0424_Reg26                  ; // 26  (ADDR : 0x9C00_D468)
    UINT32  RGST_Group0424_Reg27                  ; // 27  (ADDR : 0x9C00_D46C)
    UINT32  RGST_Group0424_Reg28                  ; // 28  (ADDR : 0x9C00_D470)
    UINT32  RGST_Group0424_Reg29                  ; // 29  (ADDR : 0x9C00_D474)
    UINT32  RGST_Group0424_Reg30                  ; // 30  (ADDR : 0x9C00_D478)
    UINT32  RGST_Group0424_Reg31                  ; // 31  (ADDR : 0x9C00_D47C)

    //Group   425 : MFD
    UINT32  RGST_Group0425_Reg00                  ; // 00  (ADDR : 0x9C00_D480)
    UINT32  RGST_Group0425_Reg01                  ; // 01  (ADDR : 0x9C00_D484)
    UINT32  RGST_Group0425_Reg02                  ; // 02  (ADDR : 0x9C00_D488)
    UINT32  RGST_Group0425_Reg03                  ; // 03  (ADDR : 0x9C00_D48C)
    UINT32  RGST_Group0425_Reg04                  ; // 04  (ADDR : 0x9C00_D490)
    UINT32  RGST_Group0425_Reg05                  ; // 05  (ADDR : 0x9C00_D494)
    UINT32  RGST_Group0425_Reg06                  ; // 06  (ADDR : 0x9C00_D498)
    UINT32  RGST_Group0425_Reg07                  ; // 07  (ADDR : 0x9C00_D49C)
    UINT32  RGST_Group0425_Reg08                  ; // 08  (ADDR : 0x9C00_D4A0)
    UINT32  RGST_Group0425_Reg09                  ; // 09  (ADDR : 0x9C00_D4A4)
    UINT32  RGST_Group0425_Reg10                  ; // 10  (ADDR : 0x9C00_D4A8)
    UINT32  RGST_Group0425_Reg11                  ; // 11  (ADDR : 0x9C00_D4AC)
    UINT32  RGST_Group0425_Reg12                  ; // 12  (ADDR : 0x9C00_D4B0)
    UINT32  RGST_Group0425_Reg13                  ; // 13  (ADDR : 0x9C00_D4B4)
    UINT32  RGST_Group0425_Reg14                  ; // 14  (ADDR : 0x9C00_D4B8)
    UINT32  RGST_Group0425_Reg15                  ; // 15  (ADDR : 0x9C00_D4BC)
    UINT32  RGST_Group0425_Reg16                  ; // 16  (ADDR : 0x9C00_D4C0)
    UINT32  RGST_Group0425_Reg17                  ; // 17  (ADDR : 0x9C00_D4C4)
    UINT32  RGST_Group0425_Reg18                  ; // 18  (ADDR : 0x9C00_D4C8)
    UINT32  RGST_Group0425_Reg19                  ; // 19  (ADDR : 0x9C00_D4CC)
    UINT32  RGST_Group0425_Reg20                  ; // 20  (ADDR : 0x9C00_D4D0)
    UINT32  RGST_Group0425_Reg21                  ; // 21  (ADDR : 0x9C00_D4D4)
    UINT32  RGST_Group0425_Reg22                  ; // 22  (ADDR : 0x9C00_D4D8)
    UINT32  RGST_Group0425_Reg23                  ; // 23  (ADDR : 0x9C00_D4DC)
    UINT32  RGST_Group0425_Reg24                  ; // 24  (ADDR : 0x9C00_D4E0)
    UINT32  RGST_Group0425_Reg25                  ; // 25  (ADDR : 0x9C00_D4E4)
    UINT32  RGST_Group0425_Reg26                  ; // 26  (ADDR : 0x9C00_D4E8)
    UINT32  RGST_Group0425_Reg27                  ; // 27  (ADDR : 0x9C00_D4EC)
    UINT32  RGST_Group0425_Reg28                  ; // 28  (ADDR : 0x9C00_D4F0)
    UINT32  RGST_Group0425_Reg29                  ; // 29  (ADDR : 0x9C00_D4F4)
    UINT32  RGST_Group0425_Reg30                  ; // 30  (ADDR : 0x9C00_D4F8)
    UINT32  RGST_Group0425_Reg31                  ; // 31  (ADDR : 0x9C00_D4FC)

    //Group   426 : MFD
    UINT32  RGST_Group0426_Reg00                  ; // 00  (ADDR : 0x9C00_D500)
    UINT32  RGST_Group0426_Reg01                  ; // 01  (ADDR : 0x9C00_D504)
    UINT32  RGST_Group0426_Reg02                  ; // 02  (ADDR : 0x9C00_D508)
    UINT32  RGST_Group0426_Reg03                  ; // 03  (ADDR : 0x9C00_D50C)
    UINT32  RGST_Group0426_Reg04                  ; // 04  (ADDR : 0x9C00_D510)
    UINT32  RGST_Group0426_Reg05                  ; // 05  (ADDR : 0x9C00_D514)
    UINT32  RGST_Group0426_Reg06                  ; // 06  (ADDR : 0x9C00_D518)
    UINT32  RGST_Group0426_Reg07                  ; // 07  (ADDR : 0x9C00_D51C)
    UINT32  RGST_Group0426_Reg08                  ; // 08  (ADDR : 0x9C00_D520)
    UINT32  RGST_Group0426_Reg09                  ; // 09  (ADDR : 0x9C00_D524)
    UINT32  RGST_Group0426_Reg10                  ; // 10  (ADDR : 0x9C00_D528)
    UINT32  RGST_Group0426_Reg11                  ; // 11  (ADDR : 0x9C00_D52C)
    UINT32  RGST_Group0426_Reg12                  ; // 12  (ADDR : 0x9C00_D530)
    UINT32  RGST_Group0426_Reg13                  ; // 13  (ADDR : 0x9C00_D534)
    UINT32  RGST_Group0426_Reg14                  ; // 14  (ADDR : 0x9C00_D538)
    UINT32  RGST_Group0426_Reg15                  ; // 15  (ADDR : 0x9C00_D53C)
    UINT32  RGST_Group0426_Reg16                  ; // 16  (ADDR : 0x9C00_D540)
    UINT32  RGST_Group0426_Reg17                  ; // 17  (ADDR : 0x9C00_D544)
    UINT32  RGST_Group0426_Reg18                  ; // 18  (ADDR : 0x9C00_D548)
    UINT32  RGST_Group0426_Reg19                  ; // 19  (ADDR : 0x9C00_D54C)
    UINT32  RGST_Group0426_Reg20                  ; // 20  (ADDR : 0x9C00_D550)
    UINT32  RGST_Group0426_Reg21                  ; // 21  (ADDR : 0x9C00_D554)
    UINT32  RGST_Group0426_Reg22                  ; // 22  (ADDR : 0x9C00_D558)
    UINT32  RGST_Group0426_Reg23                  ; // 23  (ADDR : 0x9C00_D55C)
    UINT32  RGST_Group0426_Reg24                  ; // 24  (ADDR : 0x9C00_D560)
    UINT32  RGST_Group0426_Reg25                  ; // 25  (ADDR : 0x9C00_D564)
    UINT32  RGST_Group0426_Reg26                  ; // 26  (ADDR : 0x9C00_D568)
    UINT32  RGST_Group0426_Reg27                  ; // 27  (ADDR : 0x9C00_D56C)
    UINT32  RGST_Group0426_Reg28                  ; // 28  (ADDR : 0x9C00_D570)
    UINT32  RGST_Group0426_Reg29                  ; // 29  (ADDR : 0x9C00_D574)
    UINT32  RGST_Group0426_Reg30                  ; // 30  (ADDR : 0x9C00_D578)
    UINT32  RGST_Group0426_Reg31                  ; // 31  (ADDR : 0x9C00_D57C)

    //Group   427 : MFD
    UINT32  RGST_Group0427_Reg00                  ; // 00  (ADDR : 0x9C00_D580)
    UINT32  RGST_Group0427_Reg01                  ; // 01  (ADDR : 0x9C00_D584)
    UINT32  RGST_Group0427_Reg02                  ; // 02  (ADDR : 0x9C00_D588)
    UINT32  RGST_Group0427_Reg03                  ; // 03  (ADDR : 0x9C00_D58C)
    UINT32  RGST_Group0427_Reg04                  ; // 04  (ADDR : 0x9C00_D590)
    UINT32  RGST_Group0427_Reg05                  ; // 05  (ADDR : 0x9C00_D594)
    UINT32  RGST_Group0427_Reg06                  ; // 06  (ADDR : 0x9C00_D598)
    UINT32  RGST_Group0427_Reg07                  ; // 07  (ADDR : 0x9C00_D59C)
    UINT32  RGST_Group0427_Reg08                  ; // 08  (ADDR : 0x9C00_D5A0)
    UINT32  RGST_Group0427_Reg09                  ; // 09  (ADDR : 0x9C00_D5A4)
    UINT32  RGST_Group0427_Reg10                  ; // 10  (ADDR : 0x9C00_D5A8)
    UINT32  RGST_Group0427_Reg11                  ; // 11  (ADDR : 0x9C00_D5AC)
    UINT32  RGST_Group0427_Reg12                  ; // 12  (ADDR : 0x9C00_D5B0)
    UINT32  RGST_Group0427_Reg13                  ; // 13  (ADDR : 0x9C00_D5B4)
    UINT32  RGST_Group0427_Reg14                  ; // 14  (ADDR : 0x9C00_D5B8)
    UINT32  RGST_Group0427_Reg15                  ; // 15  (ADDR : 0x9C00_D5BC)
    UINT32  RGST_Group0427_Reg16                  ; // 16  (ADDR : 0x9C00_D5C0)
    UINT32  RGST_Group0427_Reg17                  ; // 17  (ADDR : 0x9C00_D5C4)
    UINT32  RGST_Group0427_Reg18                  ; // 18  (ADDR : 0x9C00_D5C8)
    UINT32  RGST_Group0427_Reg19                  ; // 19  (ADDR : 0x9C00_D5CC)
    UINT32  RGST_Group0427_Reg20                  ; // 20  (ADDR : 0x9C00_D5D0)
    UINT32  RGST_Group0427_Reg21                  ; // 21  (ADDR : 0x9C00_D5D4)
    UINT32  RGST_Group0427_Reg22                  ; // 22  (ADDR : 0x9C00_D5D8)
    UINT32  RGST_Group0427_Reg23                  ; // 23  (ADDR : 0x9C00_D5DC)
    UINT32  RGST_Group0427_Reg24                  ; // 24  (ADDR : 0x9C00_D5E0)
    UINT32  RGST_Group0427_Reg25                  ; // 25  (ADDR : 0x9C00_D5E4)
    UINT32  RGST_Group0427_Reg26                  ; // 26  (ADDR : 0x9C00_D5E8)
    UINT32  RGST_Group0427_Reg27                  ; // 27  (ADDR : 0x9C00_D5EC)
    UINT32  RGST_Group0427_Reg28                  ; // 28  (ADDR : 0x9C00_D5F0)
    UINT32  RGST_Group0427_Reg29                  ; // 29  (ADDR : 0x9C00_D5F4)
    UINT32  RGST_Group0427_Reg30                  ; // 30  (ADDR : 0x9C00_D5F8)
    UINT32  RGST_Group0427_Reg31                  ; // 31  (ADDR : 0x9C00_D5FC)

    //Group   428 : MFD
    UINT32  RGST_Group0428_Reg00                  ; // 00  (ADDR : 0x9C00_D600)
    UINT32  RGST_Group0428_Reg01                  ; // 01  (ADDR : 0x9C00_D604)
    UINT32  RGST_Group0428_Reg02                  ; // 02  (ADDR : 0x9C00_D608)
    UINT32  RGST_Group0428_Reg03                  ; // 03  (ADDR : 0x9C00_D60C)
    UINT32  RGST_Group0428_Reg04                  ; // 04  (ADDR : 0x9C00_D610)
    UINT32  RGST_Group0428_Reg05                  ; // 05  (ADDR : 0x9C00_D614)
    UINT32  RGST_Group0428_Reg06                  ; // 06  (ADDR : 0x9C00_D618)
    UINT32  RGST_Group0428_Reg07                  ; // 07  (ADDR : 0x9C00_D61C)
    UINT32  RGST_Group0428_Reg08                  ; // 08  (ADDR : 0x9C00_D620)
    UINT32  RGST_Group0428_Reg09                  ; // 09  (ADDR : 0x9C00_D624)
    UINT32  RGST_Group0428_Reg10                  ; // 10  (ADDR : 0x9C00_D628)
    UINT32  RGST_Group0428_Reg11                  ; // 11  (ADDR : 0x9C00_D62C)
    UINT32  RGST_Group0428_Reg12                  ; // 12  (ADDR : 0x9C00_D630)
    UINT32  RGST_Group0428_Reg13                  ; // 13  (ADDR : 0x9C00_D634)
    UINT32  RGST_Group0428_Reg14                  ; // 14  (ADDR : 0x9C00_D638)
    UINT32  RGST_Group0428_Reg15                  ; // 15  (ADDR : 0x9C00_D63C)
    UINT32  RGST_Group0428_Reg16                  ; // 16  (ADDR : 0x9C00_D640)
    UINT32  RGST_Group0428_Reg17                  ; // 17  (ADDR : 0x9C00_D644)
    UINT32  RGST_Group0428_Reg18                  ; // 18  (ADDR : 0x9C00_D648)
    UINT32  RGST_Group0428_Reg19                  ; // 19  (ADDR : 0x9C00_D64C)
    UINT32  RGST_Group0428_Reg20                  ; // 20  (ADDR : 0x9C00_D650)
    UINT32  RGST_Group0428_Reg21                  ; // 21  (ADDR : 0x9C00_D654)
    UINT32  RGST_Group0428_Reg22                  ; // 22  (ADDR : 0x9C00_D658)
    UINT32  RGST_Group0428_Reg23                  ; // 23  (ADDR : 0x9C00_D65C)
    UINT32  RGST_Group0428_Reg24                  ; // 24  (ADDR : 0x9C00_D660)
    UINT32  RGST_Group0428_Reg25                  ; // 25  (ADDR : 0x9C00_D664)
    UINT32  RGST_Group0428_Reg26                  ; // 26  (ADDR : 0x9C00_D668)
    UINT32  RGST_Group0428_Reg27                  ; // 27  (ADDR : 0x9C00_D66C)
    UINT32  RGST_Group0428_Reg28                  ; // 28  (ADDR : 0x9C00_D670)
    UINT32  RGST_Group0428_Reg29                  ; // 29  (ADDR : 0x9C00_D674)
    UINT32  RGST_Group0428_Reg30                  ; // 30  (ADDR : 0x9C00_D678)
    UINT32  RGST_Group0428_Reg31                  ; // 31  (ADDR : 0x9C00_D67C)

    //Group   429 : MFD
    UINT32  RGST_Group0429_Reg00                  ; // 00  (ADDR : 0x9C00_D680)
    UINT32  RGST_Group0429_Reg01                  ; // 01  (ADDR : 0x9C00_D684)
    UINT32  RGST_Group0429_Reg02                  ; // 02  (ADDR : 0x9C00_D688)
    UINT32  RGST_Group0429_Reg03                  ; // 03  (ADDR : 0x9C00_D68C)
    UINT32  RGST_Group0429_Reg04                  ; // 04  (ADDR : 0x9C00_D690)
    UINT32  RGST_Group0429_Reg05                  ; // 05  (ADDR : 0x9C00_D694)
    UINT32  RGST_Group0429_Reg06                  ; // 06  (ADDR : 0x9C00_D698)
    UINT32  RGST_Group0429_Reg07                  ; // 07  (ADDR : 0x9C00_D69C)
    UINT32  RGST_Group0429_Reg08                  ; // 08  (ADDR : 0x9C00_D6A0)
    UINT32  RGST_Group0429_Reg09                  ; // 09  (ADDR : 0x9C00_D6A4)
    UINT32  RGST_Group0429_Reg10                  ; // 10  (ADDR : 0x9C00_D6A8)
    UINT32  RGST_Group0429_Reg11                  ; // 11  (ADDR : 0x9C00_D6AC)
    UINT32  RGST_Group0429_Reg12                  ; // 12  (ADDR : 0x9C00_D6B0)
    UINT32  RGST_Group0429_Reg13                  ; // 13  (ADDR : 0x9C00_D6B4)
    UINT32  RGST_Group0429_Reg14                  ; // 14  (ADDR : 0x9C00_D6B8)
    UINT32  RGST_Group0429_Reg15                  ; // 15  (ADDR : 0x9C00_D6BC)
    UINT32  RGST_Group0429_Reg16                  ; // 16  (ADDR : 0x9C00_D6C0)
    UINT32  RGST_Group0429_Reg17                  ; // 17  (ADDR : 0x9C00_D6C4)
    UINT32  RGST_Group0429_Reg18                  ; // 18  (ADDR : 0x9C00_D6C8)
    UINT32  RGST_Group0429_Reg19                  ; // 19  (ADDR : 0x9C00_D6CC)
    UINT32  RGST_Group0429_Reg20                  ; // 20  (ADDR : 0x9C00_D6D0)
    UINT32  RGST_Group0429_Reg21                  ; // 21  (ADDR : 0x9C00_D6D4)
    UINT32  RGST_Group0429_Reg22                  ; // 22  (ADDR : 0x9C00_D6D8)
    UINT32  RGST_Group0429_Reg23                  ; // 23  (ADDR : 0x9C00_D6DC)
    UINT32  RGST_Group0429_Reg24                  ; // 24  (ADDR : 0x9C00_D6E0)
    UINT32  RGST_Group0429_Reg25                  ; // 25  (ADDR : 0x9C00_D6E4)
    UINT32  RGST_Group0429_Reg26                  ; // 26  (ADDR : 0x9C00_D6E8)
    UINT32  RGST_Group0429_Reg27                  ; // 27  (ADDR : 0x9C00_D6EC)
    UINT32  RGST_Group0429_Reg28                  ; // 28  (ADDR : 0x9C00_D6F0)
    UINT32  RGST_Group0429_Reg29                  ; // 29  (ADDR : 0x9C00_D6F4)
    UINT32  RGST_Group0429_Reg30                  ; // 30  (ADDR : 0x9C00_D6F8)
    UINT32  RGST_Group0429_Reg31                  ; // 31  (ADDR : 0x9C00_D6FC)

    //Group   430 : MFD
    UINT32  RGST_Group0430_Reg00                  ; // 00  (ADDR : 0x9C00_D700)
    UINT32  RGST_Group0430_Reg01                  ; // 01  (ADDR : 0x9C00_D704)
    UINT32  RGST_Group0430_Reg02                  ; // 02  (ADDR : 0x9C00_D708)
    UINT32  RGST_Group0430_Reg03                  ; // 03  (ADDR : 0x9C00_D70C)
    UINT32  RGST_Group0430_Reg04                  ; // 04  (ADDR : 0x9C00_D710)
    UINT32  RGST_Group0430_Reg05                  ; // 05  (ADDR : 0x9C00_D714)
    UINT32  RGST_Group0430_Reg06                  ; // 06  (ADDR : 0x9C00_D718)
    UINT32  RGST_Group0430_Reg07                  ; // 07  (ADDR : 0x9C00_D71C)
    UINT32  RGST_Group0430_Reg08                  ; // 08  (ADDR : 0x9C00_D720)
    UINT32  RGST_Group0430_Reg09                  ; // 09  (ADDR : 0x9C00_D724)
    UINT32  RGST_Group0430_Reg10                  ; // 10  (ADDR : 0x9C00_D728)
    UINT32  RGST_Group0430_Reg11                  ; // 11  (ADDR : 0x9C00_D72C)
    UINT32  RGST_Group0430_Reg12                  ; // 12  (ADDR : 0x9C00_D730)
    UINT32  RGST_Group0430_Reg13                  ; // 13  (ADDR : 0x9C00_D734)
    UINT32  RGST_Group0430_Reg14                  ; // 14  (ADDR : 0x9C00_D738)
    UINT32  RGST_Group0430_Reg15                  ; // 15  (ADDR : 0x9C00_D73C)
    UINT32  RGST_Group0430_Reg16                  ; // 16  (ADDR : 0x9C00_D740)
    UINT32  RGST_Group0430_Reg17                  ; // 17  (ADDR : 0x9C00_D744)
    UINT32  RGST_Group0430_Reg18                  ; // 18  (ADDR : 0x9C00_D748)
    UINT32  RGST_Group0430_Reg19                  ; // 19  (ADDR : 0x9C00_D74C)
    UINT32  RGST_Group0430_Reg20                  ; // 20  (ADDR : 0x9C00_D750)
    UINT32  RGST_Group0430_Reg21                  ; // 21  (ADDR : 0x9C00_D754)
    UINT32  RGST_Group0430_Reg22                  ; // 22  (ADDR : 0x9C00_D758)
    UINT32  RGST_Group0430_Reg23                  ; // 23  (ADDR : 0x9C00_D75C)
    UINT32  RGST_Group0430_Reg24                  ; // 24  (ADDR : 0x9C00_D760)
    UINT32  RGST_Group0430_Reg25                  ; // 25  (ADDR : 0x9C00_D764)
    UINT32  RGST_Group0430_Reg26                  ; // 26  (ADDR : 0x9C00_D768)
    UINT32  RGST_Group0430_Reg27                  ; // 27  (ADDR : 0x9C00_D76C)
    UINT32  RGST_Group0430_Reg28                  ; // 28  (ADDR : 0x9C00_D770)
    UINT32  RGST_Group0430_Reg29                  ; // 29  (ADDR : 0x9C00_D774)
    UINT32  RGST_Group0430_Reg30                  ; // 30  (ADDR : 0x9C00_D778)
    UINT32  RGST_Group0430_Reg31                  ; // 31  (ADDR : 0x9C00_D77C)

    //Group   431 : MFD
    UINT32  RGST_Group0431_Reg00                  ; // 00  (ADDR : 0x9C00_D780)
    UINT32  RGST_Group0431_Reg01                  ; // 01  (ADDR : 0x9C00_D784)
    UINT32  RGST_Group0431_Reg02                  ; // 02  (ADDR : 0x9C00_D788)
    UINT32  RGST_Group0431_Reg03                  ; // 03  (ADDR : 0x9C00_D78C)
    UINT32  RGST_Group0431_Reg04                  ; // 04  (ADDR : 0x9C00_D790)
    UINT32  RGST_Group0431_Reg05                  ; // 05  (ADDR : 0x9C00_D794)
    UINT32  RGST_Group0431_Reg06                  ; // 06  (ADDR : 0x9C00_D798)
    UINT32  RGST_Group0431_Reg07                  ; // 07  (ADDR : 0x9C00_D79C)
    UINT32  RGST_Group0431_Reg08                  ; // 08  (ADDR : 0x9C00_D7A0)
    UINT32  RGST_Group0431_Reg09                  ; // 09  (ADDR : 0x9C00_D7A4)
    UINT32  RGST_Group0431_Reg10                  ; // 10  (ADDR : 0x9C00_D7A8)
    UINT32  RGST_Group0431_Reg11                  ; // 11  (ADDR : 0x9C00_D7AC)
    UINT32  RGST_Group0431_Reg12                  ; // 12  (ADDR : 0x9C00_D7B0)
    UINT32  RGST_Group0431_Reg13                  ; // 13  (ADDR : 0x9C00_D7B4)
    UINT32  RGST_Group0431_Reg14                  ; // 14  (ADDR : 0x9C00_D7B8)
    UINT32  RGST_Group0431_Reg15                  ; // 15  (ADDR : 0x9C00_D7BC)
    UINT32  RGST_Group0431_Reg16                  ; // 16  (ADDR : 0x9C00_D7C0)
    UINT32  RGST_Group0431_Reg17                  ; // 17  (ADDR : 0x9C00_D7C4)
    UINT32  RGST_Group0431_Reg18                  ; // 18  (ADDR : 0x9C00_D7C8)
    UINT32  RGST_Group0431_Reg19                  ; // 19  (ADDR : 0x9C00_D7CC)
    UINT32  RGST_Group0431_Reg20                  ; // 20  (ADDR : 0x9C00_D7D0)
    UINT32  RGST_Group0431_Reg21                  ; // 21  (ADDR : 0x9C00_D7D4)
    UINT32  RGST_Group0431_Reg22                  ; // 22  (ADDR : 0x9C00_D7D8)
    UINT32  RGST_Group0431_Reg23                  ; // 23  (ADDR : 0x9C00_D7DC)
    UINT32  RGST_Group0431_Reg24                  ; // 24  (ADDR : 0x9C00_D7E0)
    UINT32  RGST_Group0431_Reg25                  ; // 25  (ADDR : 0x9C00_D7E4)
    UINT32  RGST_Group0431_Reg26                  ; // 26  (ADDR : 0x9C00_D7E8)
    UINT32  RGST_Group0431_Reg27                  ; // 27  (ADDR : 0x9C00_D7EC)
    UINT32  RGST_Group0431_Reg28                  ; // 28  (ADDR : 0x9C00_D7F0)
    UINT32  RGST_Group0431_Reg29                  ; // 29  (ADDR : 0x9C00_D7F4)
    UINT32  RGST_Group0431_Reg30                  ; // 30  (ADDR : 0x9C00_D7F8)
    UINT32  RGST_Group0431_Reg31                  ; // 31  (ADDR : 0x9C00_D7FC)

    //Group   432 : MFD
    UINT32  RGST_Group0432_Reg00                  ; // 00  (ADDR : 0x9C00_D800)
    UINT32  RGST_Group0432_Reg01                  ; // 01  (ADDR : 0x9C00_D804)
    UINT32  RGST_Group0432_Reg02                  ; // 02  (ADDR : 0x9C00_D808)
    UINT32  RGST_Group0432_Reg03                  ; // 03  (ADDR : 0x9C00_D80C)
    UINT32  RGST_Group0432_Reg04                  ; // 04  (ADDR : 0x9C00_D810)
    UINT32  RGST_Group0432_Reg05                  ; // 05  (ADDR : 0x9C00_D814)
    UINT32  RGST_Group0432_Reg06                  ; // 06  (ADDR : 0x9C00_D818)
    UINT32  RGST_Group0432_Reg07                  ; // 07  (ADDR : 0x9C00_D81C)
    UINT32  RGST_Group0432_Reg08                  ; // 08  (ADDR : 0x9C00_D820)
    UINT32  RGST_Group0432_Reg09                  ; // 09  (ADDR : 0x9C00_D824)
    UINT32  RGST_Group0432_Reg10                  ; // 10  (ADDR : 0x9C00_D828)
    UINT32  RGST_Group0432_Reg11                  ; // 11  (ADDR : 0x9C00_D82C)
    UINT32  RGST_Group0432_Reg12                  ; // 12  (ADDR : 0x9C00_D830)
    UINT32  RGST_Group0432_Reg13                  ; // 13  (ADDR : 0x9C00_D834)
    UINT32  RGST_Group0432_Reg14                  ; // 14  (ADDR : 0x9C00_D838)
    UINT32  RGST_Group0432_Reg15                  ; // 15  (ADDR : 0x9C00_D83C)
    UINT32  RGST_Group0432_Reg16                  ; // 16  (ADDR : 0x9C00_D840)
    UINT32  RGST_Group0432_Reg17                  ; // 17  (ADDR : 0x9C00_D844)
    UINT32  RGST_Group0432_Reg18                  ; // 18  (ADDR : 0x9C00_D848)
    UINT32  RGST_Group0432_Reg19                  ; // 19  (ADDR : 0x9C00_D84C)
    UINT32  RGST_Group0432_Reg20                  ; // 20  (ADDR : 0x9C00_D850)
    UINT32  RGST_Group0432_Reg21                  ; // 21  (ADDR : 0x9C00_D854)
    UINT32  RGST_Group0432_Reg22                  ; // 22  (ADDR : 0x9C00_D858)
    UINT32  RGST_Group0432_Reg23                  ; // 23  (ADDR : 0x9C00_D85C)
    UINT32  RGST_Group0432_Reg24                  ; // 24  (ADDR : 0x9C00_D860)
    UINT32  RGST_Group0432_Reg25                  ; // 25  (ADDR : 0x9C00_D864)
    UINT32  RGST_Group0432_Reg26                  ; // 26  (ADDR : 0x9C00_D868)
    UINT32  RGST_Group0432_Reg27                  ; // 27  (ADDR : 0x9C00_D86C)
    UINT32  RGST_Group0432_Reg28                  ; // 28  (ADDR : 0x9C00_D870)
    UINT32  RGST_Group0432_Reg29                  ; // 29  (ADDR : 0x9C00_D874)
    UINT32  RGST_Group0432_Reg30                  ; // 30  (ADDR : 0x9C00_D878)
    UINT32  RGST_Group0432_Reg31                  ; // 31  (ADDR : 0x9C00_D87C)

    //Group   433 : MFD
    UINT32  RGST_Group0433_Reg00                  ; // 00  (ADDR : 0x9C00_D880)
    UINT32  RGST_Group0433_Reg01                  ; // 01  (ADDR : 0x9C00_D884)
    UINT32  RGST_Group0433_Reg02                  ; // 02  (ADDR : 0x9C00_D888)
    UINT32  RGST_Group0433_Reg03                  ; // 03  (ADDR : 0x9C00_D88C)
    UINT32  RGST_Group0433_Reg04                  ; // 04  (ADDR : 0x9C00_D890)
    UINT32  RGST_Group0433_Reg05                  ; // 05  (ADDR : 0x9C00_D894)
    UINT32  RGST_Group0433_Reg06                  ; // 06  (ADDR : 0x9C00_D898)
    UINT32  RGST_Group0433_Reg07                  ; // 07  (ADDR : 0x9C00_D89C)
    UINT32  RGST_Group0433_Reg08                  ; // 08  (ADDR : 0x9C00_D8A0)
    UINT32  RGST_Group0433_Reg09                  ; // 09  (ADDR : 0x9C00_D8A4)
    UINT32  RGST_Group0433_Reg10                  ; // 10  (ADDR : 0x9C00_D8A8)
    UINT32  RGST_Group0433_Reg11                  ; // 11  (ADDR : 0x9C00_D8AC)
    UINT32  RGST_Group0433_Reg12                  ; // 12  (ADDR : 0x9C00_D8B0)
    UINT32  RGST_Group0433_Reg13                  ; // 13  (ADDR : 0x9C00_D8B4)
    UINT32  RGST_Group0433_Reg14                  ; // 14  (ADDR : 0x9C00_D8B8)
    UINT32  RGST_Group0433_Reg15                  ; // 15  (ADDR : 0x9C00_D8BC)
    UINT32  RGST_Group0433_Reg16                  ; // 16  (ADDR : 0x9C00_D8C0)
    UINT32  RGST_Group0433_Reg17                  ; // 17  (ADDR : 0x9C00_D8C4)
    UINT32  RGST_Group0433_Reg18                  ; // 18  (ADDR : 0x9C00_D8C8)
    UINT32  RGST_Group0433_Reg19                  ; // 19  (ADDR : 0x9C00_D8CC)
    UINT32  RGST_Group0433_Reg20                  ; // 20  (ADDR : 0x9C00_D8D0)
    UINT32  RGST_Group0433_Reg21                  ; // 21  (ADDR : 0x9C00_D8D4)
    UINT32  RGST_Group0433_Reg22                  ; // 22  (ADDR : 0x9C00_D8D8)
    UINT32  RGST_Group0433_Reg23                  ; // 23  (ADDR : 0x9C00_D8DC)
    UINT32  RGST_Group0433_Reg24                  ; // 24  (ADDR : 0x9C00_D8E0)
    UINT32  RGST_Group0433_Reg25                  ; // 25  (ADDR : 0x9C00_D8E4)
    UINT32  RGST_Group0433_Reg26                  ; // 26  (ADDR : 0x9C00_D8E8)
    UINT32  RGST_Group0433_Reg27                  ; // 27  (ADDR : 0x9C00_D8EC)
    UINT32  RGST_Group0433_Reg28                  ; // 28  (ADDR : 0x9C00_D8F0)
    UINT32  RGST_Group0433_Reg29                  ; // 29  (ADDR : 0x9C00_D8F4)
    UINT32  RGST_Group0433_Reg30                  ; // 30  (ADDR : 0x9C00_D8F8)
    UINT32  RGST_Group0433_Reg31                  ; // 31  (ADDR : 0x9C00_D8FC)

    //Group   434 : MFD
    UINT32  RGST_Group0434_Reg00                  ; // 00  (ADDR : 0x9C00_D900)
    UINT32  RGST_Group0434_Reg01                  ; // 01  (ADDR : 0x9C00_D904)
    UINT32  RGST_Group0434_Reg02                  ; // 02  (ADDR : 0x9C00_D908)
    UINT32  RGST_Group0434_Reg03                  ; // 03  (ADDR : 0x9C00_D90C)
    UINT32  RGST_Group0434_Reg04                  ; // 04  (ADDR : 0x9C00_D910)
    UINT32  RGST_Group0434_Reg05                  ; // 05  (ADDR : 0x9C00_D914)
    UINT32  RGST_Group0434_Reg06                  ; // 06  (ADDR : 0x9C00_D918)
    UINT32  RGST_Group0434_Reg07                  ; // 07  (ADDR : 0x9C00_D91C)
    UINT32  RGST_Group0434_Reg08                  ; // 08  (ADDR : 0x9C00_D920)
    UINT32  RGST_Group0434_Reg09                  ; // 09  (ADDR : 0x9C00_D924)
    UINT32  RGST_Group0434_Reg10                  ; // 10  (ADDR : 0x9C00_D928)
    UINT32  RGST_Group0434_Reg11                  ; // 11  (ADDR : 0x9C00_D92C)
    UINT32  RGST_Group0434_Reg12                  ; // 12  (ADDR : 0x9C00_D930)
    UINT32  RGST_Group0434_Reg13                  ; // 13  (ADDR : 0x9C00_D934)
    UINT32  RGST_Group0434_Reg14                  ; // 14  (ADDR : 0x9C00_D938)
    UINT32  RGST_Group0434_Reg15                  ; // 15  (ADDR : 0x9C00_D93C)
    UINT32  RGST_Group0434_Reg16                  ; // 16  (ADDR : 0x9C00_D940)
    UINT32  RGST_Group0434_Reg17                  ; // 17  (ADDR : 0x9C00_D944)
    UINT32  RGST_Group0434_Reg18                  ; // 18  (ADDR : 0x9C00_D948)
    UINT32  RGST_Group0434_Reg19                  ; // 19  (ADDR : 0x9C00_D94C)
    UINT32  RGST_Group0434_Reg20                  ; // 20  (ADDR : 0x9C00_D950)
    UINT32  RGST_Group0434_Reg21                  ; // 21  (ADDR : 0x9C00_D954)
    UINT32  RGST_Group0434_Reg22                  ; // 22  (ADDR : 0x9C00_D958)
    UINT32  RGST_Group0434_Reg23                  ; // 23  (ADDR : 0x9C00_D95C)
    UINT32  RGST_Group0434_Reg24                  ; // 24  (ADDR : 0x9C00_D960)
    UINT32  RGST_Group0434_Reg25                  ; // 25  (ADDR : 0x9C00_D964)
    UINT32  RGST_Group0434_Reg26                  ; // 26  (ADDR : 0x9C00_D968)
    UINT32  RGST_Group0434_Reg27                  ; // 27  (ADDR : 0x9C00_D96C)
    UINT32  RGST_Group0434_Reg28                  ; // 28  (ADDR : 0x9C00_D970)
    UINT32  RGST_Group0434_Reg29                  ; // 29  (ADDR : 0x9C00_D974)
    UINT32  RGST_Group0434_Reg30                  ; // 30  (ADDR : 0x9C00_D978)
    UINT32  RGST_Group0434_Reg31                  ; // 31  (ADDR : 0x9C00_D97C)

    //Group   435 : MFD
    UINT32  RGST_Group0435_Reg00                  ; // 00  (ADDR : 0x9C00_D980)
    UINT32  RGST_Group0435_Reg01                  ; // 01  (ADDR : 0x9C00_D984)
    UINT32  RGST_Group0435_Reg02                  ; // 02  (ADDR : 0x9C00_D988)
    UINT32  RGST_Group0435_Reg03                  ; // 03  (ADDR : 0x9C00_D98C)
    UINT32  RGST_Group0435_Reg04                  ; // 04  (ADDR : 0x9C00_D990)
    UINT32  RGST_Group0435_Reg05                  ; // 05  (ADDR : 0x9C00_D994)
    UINT32  RGST_Group0435_Reg06                  ; // 06  (ADDR : 0x9C00_D998)
    UINT32  RGST_Group0435_Reg07                  ; // 07  (ADDR : 0x9C00_D99C)
    UINT32  RGST_Group0435_Reg08                  ; // 08  (ADDR : 0x9C00_D9A0)
    UINT32  RGST_Group0435_Reg09                  ; // 09  (ADDR : 0x9C00_D9A4)
    UINT32  RGST_Group0435_Reg10                  ; // 10  (ADDR : 0x9C00_D9A8)
    UINT32  RGST_Group0435_Reg11                  ; // 11  (ADDR : 0x9C00_D9AC)
    UINT32  RGST_Group0435_Reg12                  ; // 12  (ADDR : 0x9C00_D9B0)
    UINT32  RGST_Group0435_Reg13                  ; // 13  (ADDR : 0x9C00_D9B4)
    UINT32  RGST_Group0435_Reg14                  ; // 14  (ADDR : 0x9C00_D9B8)
    UINT32  RGST_Group0435_Reg15                  ; // 15  (ADDR : 0x9C00_D9BC)
    UINT32  RGST_Group0435_Reg16                  ; // 16  (ADDR : 0x9C00_D9C0)
    UINT32  RGST_Group0435_Reg17                  ; // 17  (ADDR : 0x9C00_D9C4)
    UINT32  RGST_Group0435_Reg18                  ; // 18  (ADDR : 0x9C00_D9C8)
    UINT32  RGST_Group0435_Reg19                  ; // 19  (ADDR : 0x9C00_D9CC)
    UINT32  RGST_Group0435_Reg20                  ; // 20  (ADDR : 0x9C00_D9D0)
    UINT32  RGST_Group0435_Reg21                  ; // 21  (ADDR : 0x9C00_D9D4)
    UINT32  RGST_Group0435_Reg22                  ; // 22  (ADDR : 0x9C00_D9D8)
    UINT32  RGST_Group0435_Reg23                  ; // 23  (ADDR : 0x9C00_D9DC)
    UINT32  RGST_Group0435_Reg24                  ; // 24  (ADDR : 0x9C00_D9E0)
    UINT32  RGST_Group0435_Reg25                  ; // 25  (ADDR : 0x9C00_D9E4)
    UINT32  RGST_Group0435_Reg26                  ; // 26  (ADDR : 0x9C00_D9E8)
    UINT32  RGST_Group0435_Reg27                  ; // 27  (ADDR : 0x9C00_D9EC)
    UINT32  RGST_Group0435_Reg28                  ; // 28  (ADDR : 0x9C00_D9F0)
    UINT32  RGST_Group0435_Reg29                  ; // 29  (ADDR : 0x9C00_D9F4)
    UINT32  RGST_Group0435_Reg30                  ; // 30  (ADDR : 0x9C00_D9F8)
    UINT32  RGST_Group0435_Reg31                  ; // 31  (ADDR : 0x9C00_D9FC)

    //Group   436 : MFD
    UINT32  RGST_Group0436_Reg00                  ; // 00  (ADDR : 0x9C00_DA00)
    UINT32  RGST_Group0436_Reg01                  ; // 01  (ADDR : 0x9C00_DA04)
    UINT32  RGST_Group0436_Reg02                  ; // 02  (ADDR : 0x9C00_DA08)
    UINT32  RGST_Group0436_Reg03                  ; // 03  (ADDR : 0x9C00_DA0C)
    UINT32  RGST_Group0436_Reg04                  ; // 04  (ADDR : 0x9C00_DA10)
    UINT32  RGST_Group0436_Reg05                  ; // 05  (ADDR : 0x9C00_DA14)
    UINT32  RGST_Group0436_Reg06                  ; // 06  (ADDR : 0x9C00_DA18)
    UINT32  RGST_Group0436_Reg07                  ; // 07  (ADDR : 0x9C00_DA1C)
    UINT32  RGST_Group0436_Reg08                  ; // 08  (ADDR : 0x9C00_DA20)
    UINT32  RGST_Group0436_Reg09                  ; // 09  (ADDR : 0x9C00_DA24)
    UINT32  RGST_Group0436_Reg10                  ; // 10  (ADDR : 0x9C00_DA28)
    UINT32  RGST_Group0436_Reg11                  ; // 11  (ADDR : 0x9C00_DA2C)
    UINT32  RGST_Group0436_Reg12                  ; // 12  (ADDR : 0x9C00_DA30)
    UINT32  RGST_Group0436_Reg13                  ; // 13  (ADDR : 0x9C00_DA34)
    UINT32  RGST_Group0436_Reg14                  ; // 14  (ADDR : 0x9C00_DA38)
    UINT32  RGST_Group0436_Reg15                  ; // 15  (ADDR : 0x9C00_DA3C)
    UINT32  RGST_Group0436_Reg16                  ; // 16  (ADDR : 0x9C00_DA40)
    UINT32  RGST_Group0436_Reg17                  ; // 17  (ADDR : 0x9C00_DA44)
    UINT32  RGST_Group0436_Reg18                  ; // 18  (ADDR : 0x9C00_DA48)
    UINT32  RGST_Group0436_Reg19                  ; // 19  (ADDR : 0x9C00_DA4C)
    UINT32  RGST_Group0436_Reg20                  ; // 20  (ADDR : 0x9C00_DA50)
    UINT32  RGST_Group0436_Reg21                  ; // 21  (ADDR : 0x9C00_DA54)
    UINT32  RGST_Group0436_Reg22                  ; // 22  (ADDR : 0x9C00_DA58)
    UINT32  RGST_Group0436_Reg23                  ; // 23  (ADDR : 0x9C00_DA5C)
    UINT32  RGST_Group0436_Reg24                  ; // 24  (ADDR : 0x9C00_DA60)
    UINT32  RGST_Group0436_Reg25                  ; // 25  (ADDR : 0x9C00_DA64)
    UINT32  RGST_Group0436_Reg26                  ; // 26  (ADDR : 0x9C00_DA68)
    UINT32  RGST_Group0436_Reg27                  ; // 27  (ADDR : 0x9C00_DA6C)
    UINT32  RGST_Group0436_Reg28                  ; // 28  (ADDR : 0x9C00_DA70)
    UINT32  RGST_Group0436_Reg29                  ; // 29  (ADDR : 0x9C00_DA74)
    UINT32  RGST_Group0436_Reg30                  ; // 30  (ADDR : 0x9C00_DA78)
    UINT32  RGST_Group0436_Reg31                  ; // 31  (ADDR : 0x9C00_DA7C)

    //Group   437 : MFD
    UINT32  RGST_Group0437_Reg00                  ; // 00  (ADDR : 0x9C00_DA80)
    UINT32  RGST_Group0437_Reg01                  ; // 01  (ADDR : 0x9C00_DA84)
    UINT32  RGST_Group0437_Reg02                  ; // 02  (ADDR : 0x9C00_DA88)
    UINT32  RGST_Group0437_Reg03                  ; // 03  (ADDR : 0x9C00_DA8C)
    UINT32  RGST_Group0437_Reg04                  ; // 04  (ADDR : 0x9C00_DA90)
    UINT32  RGST_Group0437_Reg05                  ; // 05  (ADDR : 0x9C00_DA94)
    UINT32  RGST_Group0437_Reg06                  ; // 06  (ADDR : 0x9C00_DA98)
    UINT32  RGST_Group0437_Reg07                  ; // 07  (ADDR : 0x9C00_DA9C)
    UINT32  RGST_Group0437_Reg08                  ; // 08  (ADDR : 0x9C00_DAA0)
    UINT32  RGST_Group0437_Reg09                  ; // 09  (ADDR : 0x9C00_DAA4)
    UINT32  RGST_Group0437_Reg10                  ; // 10  (ADDR : 0x9C00_DAA8)
    UINT32  RGST_Group0437_Reg11                  ; // 11  (ADDR : 0x9C00_DAAC)
    UINT32  RGST_Group0437_Reg12                  ; // 12  (ADDR : 0x9C00_DAB0)
    UINT32  RGST_Group0437_Reg13                  ; // 13  (ADDR : 0x9C00_DAB4)
    UINT32  RGST_Group0437_Reg14                  ; // 14  (ADDR : 0x9C00_DAB8)
    UINT32  RGST_Group0437_Reg15                  ; // 15  (ADDR : 0x9C00_DABC)
    UINT32  RGST_Group0437_Reg16                  ; // 16  (ADDR : 0x9C00_DAC0)
    UINT32  RGST_Group0437_Reg17                  ; // 17  (ADDR : 0x9C00_DAC4)
    UINT32  RGST_Group0437_Reg18                  ; // 18  (ADDR : 0x9C00_DAC8)
    UINT32  RGST_Group0437_Reg19                  ; // 19  (ADDR : 0x9C00_DACC)
    UINT32  RGST_Group0437_Reg20                  ; // 20  (ADDR : 0x9C00_DAD0)
    UINT32  RGST_Group0437_Reg21                  ; // 21  (ADDR : 0x9C00_DAD4)
    UINT32  RGST_Group0437_Reg22                  ; // 22  (ADDR : 0x9C00_DAD8)
    UINT32  RGST_Group0437_Reg23                  ; // 23  (ADDR : 0x9C00_DADC)
    UINT32  RGST_Group0437_Reg24                  ; // 24  (ADDR : 0x9C00_DAE0)
    UINT32  RGST_Group0437_Reg25                  ; // 25  (ADDR : 0x9C00_DAE4)
    UINT32  RGST_Group0437_Reg26                  ; // 26  (ADDR : 0x9C00_DAE8)
    UINT32  RGST_Group0437_Reg27                  ; // 27  (ADDR : 0x9C00_DAEC)
    UINT32  RGST_Group0437_Reg28                  ; // 28  (ADDR : 0x9C00_DAF0)
    UINT32  RGST_Group0437_Reg29                  ; // 29  (ADDR : 0x9C00_DAF4)
    UINT32  RGST_Group0437_Reg30                  ; // 30  (ADDR : 0x9C00_DAF8)
    UINT32  RGST_Group0437_Reg31                  ; // 31  (ADDR : 0x9C00_DAFC)

    //Group   438 : MFD
    UINT32  RGST_Group0438_Reg00                  ; // 00  (ADDR : 0x9C00_DB00)
    UINT32  RGST_Group0438_Reg01                  ; // 01  (ADDR : 0x9C00_DB04)
    UINT32  RGST_Group0438_Reg02                  ; // 02  (ADDR : 0x9C00_DB08)
    UINT32  RGST_Group0438_Reg03                  ; // 03  (ADDR : 0x9C00_DB0C)
    UINT32  RGST_Group0438_Reg04                  ; // 04  (ADDR : 0x9C00_DB10)
    UINT32  RGST_Group0438_Reg05                  ; // 05  (ADDR : 0x9C00_DB14)
    UINT32  RGST_Group0438_Reg06                  ; // 06  (ADDR : 0x9C00_DB18)
    UINT32  RGST_Group0438_Reg07                  ; // 07  (ADDR : 0x9C00_DB1C)
    UINT32  RGST_Group0438_Reg08                  ; // 08  (ADDR : 0x9C00_DB20)
    UINT32  RGST_Group0438_Reg09                  ; // 09  (ADDR : 0x9C00_DB24)
    UINT32  RGST_Group0438_Reg10                  ; // 10  (ADDR : 0x9C00_DB28)
    UINT32  RGST_Group0438_Reg11                  ; // 11  (ADDR : 0x9C00_DB2C)
    UINT32  RGST_Group0438_Reg12                  ; // 12  (ADDR : 0x9C00_DB30)
    UINT32  RGST_Group0438_Reg13                  ; // 13  (ADDR : 0x9C00_DB34)
    UINT32  RGST_Group0438_Reg14                  ; // 14  (ADDR : 0x9C00_DB38)
    UINT32  RGST_Group0438_Reg15                  ; // 15  (ADDR : 0x9C00_DB3C)
    UINT32  RGST_Group0438_Reg16                  ; // 16  (ADDR : 0x9C00_DB40)
    UINT32  RGST_Group0438_Reg17                  ; // 17  (ADDR : 0x9C00_DB44)
    UINT32  RGST_Group0438_Reg18                  ; // 18  (ADDR : 0x9C00_DB48)
    UINT32  RGST_Group0438_Reg19                  ; // 19  (ADDR : 0x9C00_DB4C)
    UINT32  RGST_Group0438_Reg20                  ; // 20  (ADDR : 0x9C00_DB50)
    UINT32  RGST_Group0438_Reg21                  ; // 21  (ADDR : 0x9C00_DB54)
    UINT32  RGST_Group0438_Reg22                  ; // 22  (ADDR : 0x9C00_DB58)
    UINT32  RGST_Group0438_Reg23                  ; // 23  (ADDR : 0x9C00_DB5C)
    UINT32  RGST_Group0438_Reg24                  ; // 24  (ADDR : 0x9C00_DB60)
    UINT32  RGST_Group0438_Reg25                  ; // 25  (ADDR : 0x9C00_DB64)
    UINT32  RGST_Group0438_Reg26                  ; // 26  (ADDR : 0x9C00_DB68)
    UINT32  RGST_Group0438_Reg27                  ; // 27  (ADDR : 0x9C00_DB6C)
    UINT32  RGST_Group0438_Reg28                  ; // 28  (ADDR : 0x9C00_DB70)
    UINT32  RGST_Group0438_Reg29                  ; // 29  (ADDR : 0x9C00_DB74)
    UINT32  RGST_Group0438_Reg30                  ; // 30  (ADDR : 0x9C00_DB78)
    UINT32  RGST_Group0438_Reg31                  ; // 31  (ADDR : 0x9C00_DB7C)

    //Group   439 : MFD
    UINT32  RGST_Group0439_Reg00                  ; // 00  (ADDR : 0x9C00_DB80)
    UINT32  RGST_Group0439_Reg01                  ; // 01  (ADDR : 0x9C00_DB84)
    UINT32  RGST_Group0439_Reg02                  ; // 02  (ADDR : 0x9C00_DB88)
    UINT32  RGST_Group0439_Reg03                  ; // 03  (ADDR : 0x9C00_DB8C)
    UINT32  RGST_Group0439_Reg04                  ; // 04  (ADDR : 0x9C00_DB90)
    UINT32  RGST_Group0439_Reg05                  ; // 05  (ADDR : 0x9C00_DB94)
    UINT32  RGST_Group0439_Reg06                  ; // 06  (ADDR : 0x9C00_DB98)
    UINT32  RGST_Group0439_Reg07                  ; // 07  (ADDR : 0x9C00_DB9C)
    UINT32  RGST_Group0439_Reg08                  ; // 08  (ADDR : 0x9C00_DBA0)
    UINT32  RGST_Group0439_Reg09                  ; // 09  (ADDR : 0x9C00_DBA4)
    UINT32  RGST_Group0439_Reg10                  ; // 10  (ADDR : 0x9C00_DBA8)
    UINT32  RGST_Group0439_Reg11                  ; // 11  (ADDR : 0x9C00_DBAC)
    UINT32  RGST_Group0439_Reg12                  ; // 12  (ADDR : 0x9C00_DBB0)
    UINT32  RGST_Group0439_Reg13                  ; // 13  (ADDR : 0x9C00_DBB4)
    UINT32  RGST_Group0439_Reg14                  ; // 14  (ADDR : 0x9C00_DBB8)
    UINT32  RGST_Group0439_Reg15                  ; // 15  (ADDR : 0x9C00_DBBC)
    UINT32  RGST_Group0439_Reg16                  ; // 16  (ADDR : 0x9C00_DBC0)
    UINT32  RGST_Group0439_Reg17                  ; // 17  (ADDR : 0x9C00_DBC4)
    UINT32  RGST_Group0439_Reg18                  ; // 18  (ADDR : 0x9C00_DBC8)
    UINT32  RGST_Group0439_Reg19                  ; // 19  (ADDR : 0x9C00_DBCC)
    UINT32  RGST_Group0439_Reg20                  ; // 20  (ADDR : 0x9C00_DBD0)
    UINT32  RGST_Group0439_Reg21                  ; // 21  (ADDR : 0x9C00_DBD4)
    UINT32  RGST_Group0439_Reg22                  ; // 22  (ADDR : 0x9C00_DBD8)
    UINT32  RGST_Group0439_Reg23                  ; // 23  (ADDR : 0x9C00_DBDC)
    UINT32  RGST_Group0439_Reg24                  ; // 24  (ADDR : 0x9C00_DBE0)
    UINT32  RGST_Group0439_Reg25                  ; // 25  (ADDR : 0x9C00_DBE4)
    UINT32  RGST_Group0439_Reg26                  ; // 26  (ADDR : 0x9C00_DBE8)
    UINT32  RGST_Group0439_Reg27                  ; // 27  (ADDR : 0x9C00_DBEC)
    UINT32  RGST_Group0439_Reg28                  ; // 28  (ADDR : 0x9C00_DBF0)
    UINT32  RGST_Group0439_Reg29                  ; // 29  (ADDR : 0x9C00_DBF4)
    UINT32  RGST_Group0439_Reg30                  ; // 30  (ADDR : 0x9C00_DBF8)
    UINT32  RGST_Group0439_Reg31                  ; // 31  (ADDR : 0x9C00_DBFC)

    //Group   440 : MFD
    UINT32  RGST_Group0440_Reg00                  ; // 00  (ADDR : 0x9C00_DC00)
    UINT32  RGST_Group0440_Reg01                  ; // 01  (ADDR : 0x9C00_DC04)
    UINT32  RGST_Group0440_Reg02                  ; // 02  (ADDR : 0x9C00_DC08)
    UINT32  RGST_Group0440_Reg03                  ; // 03  (ADDR : 0x9C00_DC0C)
    UINT32  RGST_Group0440_Reg04                  ; // 04  (ADDR : 0x9C00_DC10)
    UINT32  RGST_Group0440_Reg05                  ; // 05  (ADDR : 0x9C00_DC14)
    UINT32  RGST_Group0440_Reg06                  ; // 06  (ADDR : 0x9C00_DC18)
    UINT32  RGST_Group0440_Reg07                  ; // 07  (ADDR : 0x9C00_DC1C)
    UINT32  RGST_Group0440_Reg08                  ; // 08  (ADDR : 0x9C00_DC20)
    UINT32  RGST_Group0440_Reg09                  ; // 09  (ADDR : 0x9C00_DC24)
    UINT32  RGST_Group0440_Reg10                  ; // 10  (ADDR : 0x9C00_DC28)
    UINT32  RGST_Group0440_Reg11                  ; // 11  (ADDR : 0x9C00_DC2C)
    UINT32  RGST_Group0440_Reg12                  ; // 12  (ADDR : 0x9C00_DC30)
    UINT32  RGST_Group0440_Reg13                  ; // 13  (ADDR : 0x9C00_DC34)
    UINT32  RGST_Group0440_Reg14                  ; // 14  (ADDR : 0x9C00_DC38)
    UINT32  RGST_Group0440_Reg15                  ; // 15  (ADDR : 0x9C00_DC3C)
    UINT32  RGST_Group0440_Reg16                  ; // 16  (ADDR : 0x9C00_DC40)
    UINT32  RGST_Group0440_Reg17                  ; // 17  (ADDR : 0x9C00_DC44)
    UINT32  RGST_Group0440_Reg18                  ; // 18  (ADDR : 0x9C00_DC48)
    UINT32  RGST_Group0440_Reg19                  ; // 19  (ADDR : 0x9C00_DC4C)
    UINT32  RGST_Group0440_Reg20                  ; // 20  (ADDR : 0x9C00_DC50)
    UINT32  RGST_Group0440_Reg21                  ; // 21  (ADDR : 0x9C00_DC54)
    UINT32  RGST_Group0440_Reg22                  ; // 22  (ADDR : 0x9C00_DC58)
    UINT32  RGST_Group0440_Reg23                  ; // 23  (ADDR : 0x9C00_DC5C)
    UINT32  RGST_Group0440_Reg24                  ; // 24  (ADDR : 0x9C00_DC60)
    UINT32  RGST_Group0440_Reg25                  ; // 25  (ADDR : 0x9C00_DC64)
    UINT32  RGST_Group0440_Reg26                  ; // 26  (ADDR : 0x9C00_DC68)
    UINT32  RGST_Group0440_Reg27                  ; // 27  (ADDR : 0x9C00_DC6C)
    UINT32  RGST_Group0440_Reg28                  ; // 28  (ADDR : 0x9C00_DC70)
    UINT32  RGST_Group0440_Reg29                  ; // 29  (ADDR : 0x9C00_DC74)
    UINT32  RGST_Group0440_Reg30                  ; // 30  (ADDR : 0x9C00_DC78)
    UINT32  RGST_Group0440_Reg31                  ; // 31  (ADDR : 0x9C00_DC7C)

    //Group   441 : MFD (VLD, original G4)
    UINT32  vld_ctrl                              ; // 00  (ADDR : 0x9C00_DC80)
    UINT32  vld_status                            ; // 01  (ADDR : 0x9C00_DC84)
    UINT32  vld_decode_time                       ; // 02  (ADDR : 0x9C00_DC88)
    UINT32  vld_int_trb_trd                       ; // 03  (ADDR : 0x9C00_DC8C)
    UINT32  vld_mon0                              ; // 04  (ADDR : 0x9C00_DC90)
    UINT32  vld_mon1                              ; // 05  (ADDR : 0x9C00_DC94)
    UINT32  vld_mon2                              ; // 06  (ADDR : 0x9C00_DC98)
    UINT32  vld_mon3                              ; // 07  (ADDR : 0x9C00_DC9C)
    UINT32  jpeg_control                          ; // 08  (ADDR : 0x9C00_DCA0)
    UINT32  jpeg_write_addr                       ; // 09  (ADDR : 0x9C00_DCA4)
    UINT32  jpeg_write_data                       ; // 10  (ADDR : 0x9C00_DCA8)
    UINT32  jpeg_read_addr                        ; // 11  (ADDR : 0x9C00_DCAC)
    UINT32  jpeg_read_data                        ; // 12  (ADDR : 0x9C00_DCB0)
    UINT32  error_mb_threshold                    ; // 13  (ADDR : 0x9C00_DCB4)
    UINT32  vld_config                            ; // 14  (ADDR : 0x9C00_DCB8)
    UINT32  vld_trb_trd                           ; // 15  (ADDR : 0x9C00_DCBC)
    UINT32  vld_trb_trd_ext                       ; // 16  (ADDR : 0x9C00_DCC0)
    UINT32  vld_2trbp_2dp                         ; // 17  (ADDR : 0x9C00_DCC4)
    UINT32  vld_2trbm_2dm                         ; // 18  (ADDR : 0x9C00_DCC8)
    UINT32  vld_packet_header                     ; // 19  (ADDR : 0x9C00_DCCC)
    UINT32  vld_vol_header                        ; // 20  (ADDR : 0x9C00_DCD0)
    UINT32  vld_vop_header                        ; // 21  (ADDR : 0x9C00_DCD4)
    UINT32  vld_mp4_misc                          ; // 22  (ADDR : 0x9C00_DCD8)
    UINT32  g441_reserved[8]                      ; // 23  (ADDR : 0x9C00_DCDC) ~ 30  (ADDR : 0x9C00_DCF8) 
    UINT32  vld_gob_num                           ; // 31  (ADDR : 0x9C00_DCFC)

    //Group   442 : MFD
    UINT32  RGST_Group0442_Reg00                  ; // 00  (ADDR : 0x9C00_DD00)
    UINT32  RGST_Group0442_Reg01                  ; // 01  (ADDR : 0x9C00_DD04)
    UINT32  RGST_Group0442_Reg02                  ; // 02  (ADDR : 0x9C00_DD08)
    UINT32  RGST_Group0442_Reg03                  ; // 03  (ADDR : 0x9C00_DD0C)
    UINT32  RGST_Group0442_Reg04                  ; // 04  (ADDR : 0x9C00_DD10)
    UINT32  RGST_Group0442_Reg05                  ; // 05  (ADDR : 0x9C00_DD14)
    UINT32  RGST_Group0442_Reg06                  ; // 06  (ADDR : 0x9C00_DD18)
    UINT32  RGST_Group0442_Reg07                  ; // 07  (ADDR : 0x9C00_DD1C)
    UINT32  RGST_Group0442_Reg08                  ; // 08  (ADDR : 0x9C00_DD20)
    UINT32  RGST_Group0442_Reg09                  ; // 09  (ADDR : 0x9C00_DD24)
    UINT32  RGST_Group0442_Reg10                  ; // 10  (ADDR : 0x9C00_DD28)
    UINT32  RGST_Group0442_Reg11                  ; // 11  (ADDR : 0x9C00_DD2C)
    UINT32  RGST_Group0442_Reg12                  ; // 12  (ADDR : 0x9C00_DD30)
    UINT32  RGST_Group0442_Reg13                  ; // 13  (ADDR : 0x9C00_DD34)
    UINT32  RGST_Group0442_Reg14                  ; // 14  (ADDR : 0x9C00_DD38)
    UINT32  RGST_Group0442_Reg15                  ; // 15  (ADDR : 0x9C00_DD3C)
    UINT32  RGST_Group0442_Reg16                  ; // 16  (ADDR : 0x9C00_DD40)
    UINT32  RGST_Group0442_Reg17                  ; // 17  (ADDR : 0x9C00_DD44)
    UINT32  RGST_Group0442_Reg18                  ; // 18  (ADDR : 0x9C00_DD48)
    UINT32  RGST_Group0442_Reg19                  ; // 19  (ADDR : 0x9C00_DD4C)
    UINT32  RGST_Group0442_Reg20                  ; // 20  (ADDR : 0x9C00_DD50)
    UINT32  RGST_Group0442_Reg21                  ; // 21  (ADDR : 0x9C00_DD54)
    UINT32  RGST_Group0442_Reg22                  ; // 22  (ADDR : 0x9C00_DD58)
    UINT32  RGST_Group0442_Reg23                  ; // 23  (ADDR : 0x9C00_DD5C)
    UINT32  RGST_Group0442_Reg24                  ; // 24  (ADDR : 0x9C00_DD60)
    UINT32  RGST_Group0442_Reg25                  ; // 25  (ADDR : 0x9C00_DD64)
    UINT32  RGST_Group0442_Reg26                  ; // 26  (ADDR : 0x9C00_DD68)
    UINT32  RGST_Group0442_Reg27                  ; // 27  (ADDR : 0x9C00_DD6C)
    UINT32  RGST_Group0442_Reg28                  ; // 28  (ADDR : 0x9C00_DD70)
    UINT32  RGST_Group0442_Reg29                  ; // 29  (ADDR : 0x9C00_DD74)
    UINT32  RGST_Group0442_Reg30                  ; // 30  (ADDR : 0x9C00_DD78)
    UINT32  RGST_Group0442_Reg31                  ; // 31  (ADDR : 0x9C00_DD7C)

    //Group   443 : MFD
    UINT32  RGST_Group0443_Reg00                  ; // 00  (ADDR : 0x9C00_DD80)
    UINT32  RGST_Group0443_Reg01                  ; // 01  (ADDR : 0x9C00_DD84)
    UINT32  RGST_Group0443_Reg02                  ; // 02  (ADDR : 0x9C00_DD88)
    UINT32  RGST_Group0443_Reg03                  ; // 03  (ADDR : 0x9C00_DD8C)
    UINT32  RGST_Group0443_Reg04                  ; // 04  (ADDR : 0x9C00_DD90)
    UINT32  RGST_Group0443_Reg05                  ; // 05  (ADDR : 0x9C00_DD94)
    UINT32  RGST_Group0443_Reg06                  ; // 06  (ADDR : 0x9C00_DD98)
    UINT32  RGST_Group0443_Reg07                  ; // 07  (ADDR : 0x9C00_DD9C)
    UINT32  RGST_Group0443_Reg08                  ; // 08  (ADDR : 0x9C00_DDA0)
    UINT32  RGST_Group0443_Reg09                  ; // 09  (ADDR : 0x9C00_DDA4)
    UINT32  RGST_Group0443_Reg10                  ; // 10  (ADDR : 0x9C00_DDA8)
    UINT32  RGST_Group0443_Reg11                  ; // 11  (ADDR : 0x9C00_DDAC)
    UINT32  RGST_Group0443_Reg12                  ; // 12  (ADDR : 0x9C00_DDB0)
    UINT32  RGST_Group0443_Reg13                  ; // 13  (ADDR : 0x9C00_DDB4)
    UINT32  RGST_Group0443_Reg14                  ; // 14  (ADDR : 0x9C00_DDB8)
    UINT32  RGST_Group0443_Reg15                  ; // 15  (ADDR : 0x9C00_DDBC)
    UINT32  RGST_Group0443_Reg16                  ; // 16  (ADDR : 0x9C00_DDC0)
    UINT32  RGST_Group0443_Reg17                  ; // 17  (ADDR : 0x9C00_DDC4)
    UINT32  RGST_Group0443_Reg18                  ; // 18  (ADDR : 0x9C00_DDC8)
    UINT32  RGST_Group0443_Reg19                  ; // 19  (ADDR : 0x9C00_DDCC)
    UINT32  RGST_Group0443_Reg20                  ; // 20  (ADDR : 0x9C00_DDD0)
    UINT32  RGST_Group0443_Reg21                  ; // 21  (ADDR : 0x9C00_DDD4)
    UINT32  RGST_Group0443_Reg22                  ; // 22  (ADDR : 0x9C00_DDD8)
    UINT32  RGST_Group0443_Reg23                  ; // 23  (ADDR : 0x9C00_DDDC)
    UINT32  RGST_Group0443_Reg24                  ; // 24  (ADDR : 0x9C00_DDE0)
    UINT32  RGST_Group0443_Reg25                  ; // 25  (ADDR : 0x9C00_DDE4)
    UINT32  RGST_Group0443_Reg26                  ; // 26  (ADDR : 0x9C00_DDE8)
    UINT32  RGST_Group0443_Reg27                  ; // 27  (ADDR : 0x9C00_DDEC)
    UINT32  RGST_Group0443_Reg28                  ; // 28  (ADDR : 0x9C00_DDF0)
    UINT32  RGST_Group0443_Reg29                  ; // 29  (ADDR : 0x9C00_DDF4)
    UINT32  RGST_Group0443_Reg30                  ; // 30  (ADDR : 0x9C00_DDF8)
    UINT32  RGST_Group0443_Reg31                  ; // 31  (ADDR : 0x9C00_DDFC)

    //Group   444 : MFD (INVQ, original G25)
    UINT32  invq_qmx_par                          ; // 00  (ADDR : 0x9C00_DE00)
    UINT32  invq_mode                             ; // 01  (ADDR : 0x9C00_DE04)
    UINT32  invq_chksum                           ; // 02  (ADDR : 0x9C00_DE08)
    UINT32  g444_reserved_0[2]                    ; // 03  (ADDR : 0x9C00_DE0C) ~ 04  (ADDR : 0x9C00_DE10)
    UINT32  invq_vol_header                       ; // 05  (ADDR : 0x9C00_DE14)
    UINT32  g444_reserved_1[26]                   ; // 06  (ADDR : 0x9C00_DE18) ~ 31  (ADDR : 0x9C00_DE7C)

    //Group   445 : MFD
    UINT32  RGST_Group0445_Reg00                  ; // 00  (ADDR : 0x9C00_DE80)
    UINT32  RGST_Group0445_Reg01                  ; // 01  (ADDR : 0x9C00_DE84)
    UINT32  RGST_Group0445_Reg02                  ; // 02  (ADDR : 0x9C00_DE88)
    UINT32  RGST_Group0445_Reg03                  ; // 03  (ADDR : 0x9C00_DE8C)
    UINT32  RGST_Group0445_Reg04                  ; // 04  (ADDR : 0x9C00_DE90)
    UINT32  RGST_Group0445_Reg05                  ; // 05  (ADDR : 0x9C00_DE94)
    UINT32  RGST_Group0445_Reg06                  ; // 06  (ADDR : 0x9C00_DE98)
    UINT32  RGST_Group0445_Reg07                  ; // 07  (ADDR : 0x9C00_DE9C)
    UINT32  RGST_Group0445_Reg08                  ; // 08  (ADDR : 0x9C00_DEA0)
    UINT32  RGST_Group0445_Reg09                  ; // 09  (ADDR : 0x9C00_DEA4)
    UINT32  RGST_Group0445_Reg10                  ; // 10  (ADDR : 0x9C00_DEA8)
    UINT32  RGST_Group0445_Reg11                  ; // 11  (ADDR : 0x9C00_DEAC)
    UINT32  RGST_Group0445_Reg12                  ; // 12  (ADDR : 0x9C00_DEB0)
    UINT32  RGST_Group0445_Reg13                  ; // 13  (ADDR : 0x9C00_DEB4)
    UINT32  RGST_Group0445_Reg14                  ; // 14  (ADDR : 0x9C00_DEB8)
    UINT32  RGST_Group0445_Reg15                  ; // 15  (ADDR : 0x9C00_DEBC)
    UINT32  RGST_Group0445_Reg16                  ; // 16  (ADDR : 0x9C00_DEC0)
    UINT32  RGST_Group0445_Reg17                  ; // 17  (ADDR : 0x9C00_DEC4)
    UINT32  RGST_Group0445_Reg18                  ; // 18  (ADDR : 0x9C00_DEC8)
    UINT32  RGST_Group0445_Reg19                  ; // 19  (ADDR : 0x9C00_DECC)
    UINT32  RGST_Group0445_Reg20                  ; // 20  (ADDR : 0x9C00_DED0)
    UINT32  RGST_Group0445_Reg21                  ; // 21  (ADDR : 0x9C00_DED4)
    UINT32  RGST_Group0445_Reg22                  ; // 22  (ADDR : 0x9C00_DED8)
    UINT32  RGST_Group0445_Reg23                  ; // 23  (ADDR : 0x9C00_DEDC)
    UINT32  RGST_Group0445_Reg24                  ; // 24  (ADDR : 0x9C00_DEE0)
    UINT32  RGST_Group0445_Reg25                  ; // 25  (ADDR : 0x9C00_DEE4)
    UINT32  RGST_Group0445_Reg26                  ; // 26  (ADDR : 0x9C00_DEE8)
    UINT32  RGST_Group0445_Reg27                  ; // 27  (ADDR : 0x9C00_DEEC)
    UINT32  RGST_Group0445_Reg28                  ; // 28  (ADDR : 0x9C00_DEF0)
    UINT32  RGST_Group0445_Reg29                  ; // 29  (ADDR : 0x9C00_DEF4)
    UINT32  RGST_Group0445_Reg30                  ; // 30  (ADDR : 0x9C00_DEF8)
    UINT32  RGST_Group0445_Reg31                  ; // 31  (ADDR : 0x9C00_DEFC)

    //Group   446 : MFD (AGDC, original G22)
    UINT32  mbus_bridge_config                    ; // 00  (ADDR : 0x9C00_DF00)
    UINT32  evbya                                 ; // 01  (ADDR : 0x9C00_DF04)
    UINT32  osdya                                 ; // 02  (ADDR : 0x9C00_DF08)
    UINT32  cdwya                                 ; // 03  (ADDR : 0x9C00_DF0C)
    UINT32  cdrya                                 ; // 04  (ADDR : 0x9C00_DF10)
    UINT32  supya                                 ; // 05  (ADDR : 0x9C00_DF14)
    UINT32  evbya_limit                           ; // 06  (ADDR : 0x9C00_DF18)
    UINT32  osdya_limit                           ; // 07  (ADDR : 0x9C00_DF1C)
    UINT32  cdwya_limit                           ; // 08  (ADDR : 0x9C00_DF20)
    UINT32  cdrya_limit                           ; // 09  (ADDR : 0x9C00_DF24)
    UINT32  supya_limit                           ; // 10  (ADDR : 0x9C00_DF28)
    UINT32  bs_ystop                              ; // 11  (ADDR : 0x9C00_DF2C)
    UINT32  bs_ry                                 ; // 12  (ADDR : 0x9C00_DF30)
    UINT32  bs_yinit                              ; // 13  (ADDR : 0x9C00_DF34)
    UINT32  bs_xinit                              ; // 14  (ADDR : 0x9C00_DF38)
    UINT32  dvddsp_vy                             ; // 15  (ADDR : 0x9C00_DF3C)
    UINT32  dvddsp_vx                             ; // 16  (ADDR : 0x9C00_DF40)
    UINT32  dvddsp_ry                             ; // 17  (ADDR : 0x9C00_DF44)
    UINT32  cdr_vy                                ; // 18  (ADDR : 0x9C00_DF48)
    UINT32  cdr_vx                                ; // 19  (ADDR : 0x9C00_DF4C)
    UINT32  supya2                                ; // 20  (ADDR : 0x9C00_DF50)
    UINT32  supya2_limit                          ; // 21  (ADDR : 0x9C00_DF54)
    UINT32  iopya                                 ; // 22  (ADDR : 0x9C00_DF58)
    UINT32  cdrya_xlimit                          ; // 23  (ADDR : 0x9C00_DF5C)
    UINT32  mvcya                                 ; // 24  (ADDR : 0x9C00_DF60)
    UINT32  agdcya                                ; // 25  (ADDR : 0x9C00_DF64)
    UINT32  g446_basehigh                         ; // 26  (ADDR : 0x9C00_DF68)
    UINT32  g446_reserved[5]                      ; // 27  (ADDR : 0x9C00_DF6C) ~ 31  (ADDR : 0x9C00_DF7C)

    //Group   447 : MFD (BUF CTL, original G24)
    UINT32  bufctl[8]                             ; // 00  (ADDR : 0x9C00_DF80) ~ 07  (ADDR : 0x9C00_DF9C)
    UINT32  g24_reserved[24]                      ; // 08  (ADDR : 0x9C00_DFA0) ~ 31  (ADDR : 0x9C00_DFFC)
    //Group   448 : MFD
    UINT32  RGST_Group0448_Reg00                  ; // 00  (ADDR : 0x9C00_E000)
    UINT32  RGST_Group0448_Reg01                  ; // 01  (ADDR : 0x9C00_E004)
    UINT32  RGST_Group0448_Reg02                  ; // 02  (ADDR : 0x9C00_E008)
    UINT32  RGST_Group0448_Reg03                  ; // 03  (ADDR : 0x9C00_E00C)
    UINT32  RGST_Group0448_Reg04                  ; // 04  (ADDR : 0x9C00_E010)
    UINT32  RGST_Group0448_Reg05                  ; // 05  (ADDR : 0x9C00_E014)
    UINT32  RGST_Group0448_Reg06                  ; // 06  (ADDR : 0x9C00_E018)
    UINT32  RGST_Group0448_Reg07                  ; // 07  (ADDR : 0x9C00_E01C)
    UINT32  RGST_Group0448_Reg08                  ; // 08  (ADDR : 0x9C00_E020)
    UINT32  RGST_Group0448_Reg09                  ; // 09  (ADDR : 0x9C00_E024)
    UINT32  RGST_Group0448_Reg10                  ; // 10  (ADDR : 0x9C00_E028)
    UINT32  RGST_Group0448_Reg11                  ; // 11  (ADDR : 0x9C00_E02C)
    UINT32  RGST_Group0448_Reg12                  ; // 12  (ADDR : 0x9C00_E030)
    UINT32  RGST_Group0448_Reg13                  ; // 13  (ADDR : 0x9C00_E034)
    UINT32  RGST_Group0448_Reg14                  ; // 14  (ADDR : 0x9C00_E038)
    UINT32  RGST_Group0448_Reg15                  ; // 15  (ADDR : 0x9C00_E03C)
    UINT32  RGST_Group0448_Reg16                  ; // 16  (ADDR : 0x9C00_E040)
    UINT32  RGST_Group0448_Reg17                  ; // 17  (ADDR : 0x9C00_E044)
    UINT32  RGST_Group0448_Reg18                  ; // 18  (ADDR : 0x9C00_E048)
    UINT32  RGST_Group0448_Reg19                  ; // 19  (ADDR : 0x9C00_E04C)
    UINT32  RGST_Group0448_Reg20                  ; // 20  (ADDR : 0x9C00_E050)
    UINT32  RGST_Group0448_Reg21                  ; // 21  (ADDR : 0x9C00_E054)
    UINT32  RGST_Group0448_Reg22                  ; // 22  (ADDR : 0x9C00_E058)
    UINT32  RGST_Group0448_Reg23                  ; // 23  (ADDR : 0x9C00_E05C)
    UINT32  RGST_Group0448_Reg24                  ; // 24  (ADDR : 0x9C00_E060)
    UINT32  RGST_Group0448_Reg25                  ; // 25  (ADDR : 0x9C00_E064)
    UINT32  RGST_Group0448_Reg26                  ; // 26  (ADDR : 0x9C00_E068)
    UINT32  RGST_Group0448_Reg27                  ; // 27  (ADDR : 0x9C00_E06C)
    UINT32  RGST_Group0448_Reg28                  ; // 28  (ADDR : 0x9C00_E070)
    UINT32  RGST_Group0448_Reg29                  ; // 29  (ADDR : 0x9C00_E074)
    UINT32  RGST_Group0448_Reg30                  ; // 30  (ADDR : 0x9C00_E078)
    UINT32  RGST_Group0448_Reg31                  ; // 31  (ADDR : 0x9C00_E07C)

    //Group   449 : MFD
    UINT32  RGST_Group0449_Reg00                  ; // 00  (ADDR : 0x9C00_E080)
    UINT32  RGST_Group0449_Reg01                  ; // 01  (ADDR : 0x9C00_E084)
    UINT32  RGST_Group0449_Reg02                  ; // 02  (ADDR : 0x9C00_E088)
    UINT32  RGST_Group0449_Reg03                  ; // 03  (ADDR : 0x9C00_E08C)
    UINT32  RGST_Group0449_Reg04                  ; // 04  (ADDR : 0x9C00_E090)
    UINT32  RGST_Group0449_Reg05                  ; // 05  (ADDR : 0x9C00_E094)
    UINT32  RGST_Group0449_Reg06                  ; // 06  (ADDR : 0x9C00_E098)
    UINT32  RGST_Group0449_Reg07                  ; // 07  (ADDR : 0x9C00_E09C)
    UINT32  RGST_Group0449_Reg08                  ; // 08  (ADDR : 0x9C00_E0A0)
    UINT32  RGST_Group0449_Reg09                  ; // 09  (ADDR : 0x9C00_E0A4)
    UINT32  RGST_Group0449_Reg10                  ; // 10  (ADDR : 0x9C00_E0A8)
    UINT32  RGST_Group0449_Reg11                  ; // 11  (ADDR : 0x9C00_E0AC)
    UINT32  RGST_Group0449_Reg12                  ; // 12  (ADDR : 0x9C00_E0B0)
    UINT32  RGST_Group0449_Reg13                  ; // 13  (ADDR : 0x9C00_E0B4)
    UINT32  RGST_Group0449_Reg14                  ; // 14  (ADDR : 0x9C00_E0B8)
    UINT32  RGST_Group0449_Reg15                  ; // 15  (ADDR : 0x9C00_E0BC)
    UINT32  RGST_Group0449_Reg16                  ; // 16  (ADDR : 0x9C00_E0C0)
    UINT32  RGST_Group0449_Reg17                  ; // 17  (ADDR : 0x9C00_E0C4)
    UINT32  RGST_Group0449_Reg18                  ; // 18  (ADDR : 0x9C00_E0C8)
    UINT32  RGST_Group0449_Reg19                  ; // 19  (ADDR : 0x9C00_E0CC)
    UINT32  RGST_Group0449_Reg20                  ; // 20  (ADDR : 0x9C00_E0D0)
    UINT32  RGST_Group0449_Reg21                  ; // 21  (ADDR : 0x9C00_E0D4)
    UINT32  RGST_Group0449_Reg22                  ; // 22  (ADDR : 0x9C00_E0D8)
    UINT32  RGST_Group0449_Reg23                  ; // 23  (ADDR : 0x9C00_E0DC)
    UINT32  RGST_Group0449_Reg24                  ; // 24  (ADDR : 0x9C00_E0E0)
    UINT32  RGST_Group0449_Reg25                  ; // 25  (ADDR : 0x9C00_E0E4)
    UINT32  RGST_Group0449_Reg26                  ; // 26  (ADDR : 0x9C00_E0E8)
    UINT32  RGST_Group0449_Reg27                  ; // 27  (ADDR : 0x9C00_E0EC)
    UINT32  RGST_Group0449_Reg28                  ; // 28  (ADDR : 0x9C00_E0F0)
    UINT32  RGST_Group0449_Reg29                  ; // 29  (ADDR : 0x9C00_E0F4)
    UINT32  RGST_Group0449_Reg30                  ; // 30  (ADDR : 0x9C00_E0F8)
    UINT32  RGST_Group0449_Reg31                  ; // 31  (ADDR : 0x9C00_E0FC)

    //Group   450 : MFD
    UINT32  RGST_Group0450_Reg00                  ; // 00  (ADDR : 0x9C00_E100)
    UINT32  RGST_Group0450_Reg01                  ; // 01  (ADDR : 0x9C00_E104)
    UINT32  RGST_Group0450_Reg02                  ; // 02  (ADDR : 0x9C00_E108)
    UINT32  RGST_Group0450_Reg03                  ; // 03  (ADDR : 0x9C00_E10C)
    UINT32  RGST_Group0450_Reg04                  ; // 04  (ADDR : 0x9C00_E110)
    UINT32  RGST_Group0450_Reg05                  ; // 05  (ADDR : 0x9C00_E114)
    UINT32  RGST_Group0450_Reg06                  ; // 06  (ADDR : 0x9C00_E118)
    UINT32  RGST_Group0450_Reg07                  ; // 07  (ADDR : 0x9C00_E11C)
    UINT32  RGST_Group0450_Reg08                  ; // 08  (ADDR : 0x9C00_E120)
    UINT32  RGST_Group0450_Reg09                  ; // 09  (ADDR : 0x9C00_E124)
    UINT32  RGST_Group0450_Reg10                  ; // 10  (ADDR : 0x9C00_E128)
    UINT32  RGST_Group0450_Reg11                  ; // 11  (ADDR : 0x9C00_E12C)
    UINT32  RGST_Group0450_Reg12                  ; // 12  (ADDR : 0x9C00_E130)
    UINT32  RGST_Group0450_Reg13                  ; // 13  (ADDR : 0x9C00_E134)
    UINT32  RGST_Group0450_Reg14                  ; // 14  (ADDR : 0x9C00_E138)
    UINT32  RGST_Group0450_Reg15                  ; // 15  (ADDR : 0x9C00_E13C)
    UINT32  RGST_Group0450_Reg16                  ; // 16  (ADDR : 0x9C00_E140)
    UINT32  RGST_Group0450_Reg17                  ; // 17  (ADDR : 0x9C00_E144)
    UINT32  RGST_Group0450_Reg18                  ; // 18  (ADDR : 0x9C00_E148)
    UINT32  RGST_Group0450_Reg19                  ; // 19  (ADDR : 0x9C00_E14C)
    UINT32  RGST_Group0450_Reg20                  ; // 20  (ADDR : 0x9C00_E150)
    UINT32  RGST_Group0450_Reg21                  ; // 21  (ADDR : 0x9C00_E154)
    UINT32  RGST_Group0450_Reg22                  ; // 22  (ADDR : 0x9C00_E158)
    UINT32  RGST_Group0450_Reg23                  ; // 23  (ADDR : 0x9C00_E15C)
    UINT32  RGST_Group0450_Reg24                  ; // 24  (ADDR : 0x9C00_E160)
    UINT32  RGST_Group0450_Reg25                  ; // 25  (ADDR : 0x9C00_E164)
    UINT32  RGST_Group0450_Reg26                  ; // 26  (ADDR : 0x9C00_E168)
    UINT32  RGST_Group0450_Reg27                  ; // 27  (ADDR : 0x9C00_E16C)
    UINT32  RGST_Group0450_Reg28                  ; // 28  (ADDR : 0x9C00_E170)
    UINT32  RGST_Group0450_Reg29                  ; // 29  (ADDR : 0x9C00_E174)
    UINT32  RGST_Group0450_Reg30                  ; // 30  (ADDR : 0x9C00_E178)
    UINT32  RGST_Group0450_Reg31                  ; // 31  (ADDR : 0x9C00_E17C)

    //Group   451 : MFD
    UINT32  RGST_Group0451_Reg00                  ; // 00  (ADDR : 0x9C00_E180)
    UINT32  RGST_Group0451_Reg01                  ; // 01  (ADDR : 0x9C00_E184)
    UINT32  RGST_Group0451_Reg02                  ; // 02  (ADDR : 0x9C00_E188)
    UINT32  RGST_Group0451_Reg03                  ; // 03  (ADDR : 0x9C00_E18C)
    UINT32  RGST_Group0451_Reg04                  ; // 04  (ADDR : 0x9C00_E190)
    UINT32  RGST_Group0451_Reg05                  ; // 05  (ADDR : 0x9C00_E194)
    UINT32  RGST_Group0451_Reg06                  ; // 06  (ADDR : 0x9C00_E198)
    UINT32  RGST_Group0451_Reg07                  ; // 07  (ADDR : 0x9C00_E19C)
    UINT32  RGST_Group0451_Reg08                  ; // 08  (ADDR : 0x9C00_E1A0)
    UINT32  RGST_Group0451_Reg09                  ; // 09  (ADDR : 0x9C00_E1A4)
    UINT32  RGST_Group0451_Reg10                  ; // 10  (ADDR : 0x9C00_E1A8)
    UINT32  RGST_Group0451_Reg11                  ; // 11  (ADDR : 0x9C00_E1AC)
    UINT32  RGST_Group0451_Reg12                  ; // 12  (ADDR : 0x9C00_E1B0)
    UINT32  RGST_Group0451_Reg13                  ; // 13  (ADDR : 0x9C00_E1B4)
    UINT32  RGST_Group0451_Reg14                  ; // 14  (ADDR : 0x9C00_E1B8)
    UINT32  RGST_Group0451_Reg15                  ; // 15  (ADDR : 0x9C00_E1BC)
    UINT32  RGST_Group0451_Reg16                  ; // 16  (ADDR : 0x9C00_E1C0)
    UINT32  RGST_Group0451_Reg17                  ; // 17  (ADDR : 0x9C00_E1C4)
    UINT32  RGST_Group0451_Reg18                  ; // 18  (ADDR : 0x9C00_E1C8)
    UINT32  RGST_Group0451_Reg19                  ; // 19  (ADDR : 0x9C00_E1CC)
    UINT32  RGST_Group0451_Reg20                  ; // 20  (ADDR : 0x9C00_E1D0)
    UINT32  RGST_Group0451_Reg21                  ; // 21  (ADDR : 0x9C00_E1D4)
    UINT32  RGST_Group0451_Reg22                  ; // 22  (ADDR : 0x9C00_E1D8)
    UINT32  RGST_Group0451_Reg23                  ; // 23  (ADDR : 0x9C00_E1DC)
    UINT32  RGST_Group0451_Reg24                  ; // 24  (ADDR : 0x9C00_E1E0)
    UINT32  RGST_Group0451_Reg25                  ; // 25  (ADDR : 0x9C00_E1E4)
    UINT32  RGST_Group0451_Reg26                  ; // 26  (ADDR : 0x9C00_E1E8)
    UINT32  RGST_Group0451_Reg27                  ; // 27  (ADDR : 0x9C00_E1EC)
    UINT32  RGST_Group0451_Reg28                  ; // 28  (ADDR : 0x9C00_E1F0)
    UINT32  RGST_Group0451_Reg29                  ; // 29  (ADDR : 0x9C00_E1F4)
    UINT32  RGST_Group0451_Reg30                  ; // 30  (ADDR : 0x9C00_E1F8)
    UINT32  RGST_Group0451_Reg31                  ; // 31  (ADDR : 0x9C00_E1FC)

    //Group   452 : MFD
    UINT32  RGST_Group0452_Reg00                  ; // 00  (ADDR : 0x9C00_E200)
    UINT32  RGST_Group0452_Reg01                  ; // 01  (ADDR : 0x9C00_E204)
    UINT32  RGST_Group0452_Reg02                  ; // 02  (ADDR : 0x9C00_E208)
    UINT32  RGST_Group0452_Reg03                  ; // 03  (ADDR : 0x9C00_E20C)
    UINT32  RGST_Group0452_Reg04                  ; // 04  (ADDR : 0x9C00_E210)
    UINT32  RGST_Group0452_Reg05                  ; // 05  (ADDR : 0x9C00_E214)
    UINT32  RGST_Group0452_Reg06                  ; // 06  (ADDR : 0x9C00_E218)
    UINT32  RGST_Group0452_Reg07                  ; // 07  (ADDR : 0x9C00_E21C)
    UINT32  RGST_Group0452_Reg08                  ; // 08  (ADDR : 0x9C00_E220)
    UINT32  RGST_Group0452_Reg09                  ; // 09  (ADDR : 0x9C00_E224)
    UINT32  RGST_Group0452_Reg10                  ; // 10  (ADDR : 0x9C00_E228)
    UINT32  RGST_Group0452_Reg11                  ; // 11  (ADDR : 0x9C00_E22C)
    UINT32  RGST_Group0452_Reg12                  ; // 12  (ADDR : 0x9C00_E230)
    UINT32  RGST_Group0452_Reg13                  ; // 13  (ADDR : 0x9C00_E234)
    UINT32  RGST_Group0452_Reg14                  ; // 14  (ADDR : 0x9C00_E238)
    UINT32  RGST_Group0452_Reg15                  ; // 15  (ADDR : 0x9C00_E23C)
    UINT32  RGST_Group0452_Reg16                  ; // 16  (ADDR : 0x9C00_E240)
    UINT32  RGST_Group0452_Reg17                  ; // 17  (ADDR : 0x9C00_E244)
    UINT32  RGST_Group0452_Reg18                  ; // 18  (ADDR : 0x9C00_E248)
    UINT32  RGST_Group0452_Reg19                  ; // 19  (ADDR : 0x9C00_E24C)
    UINT32  RGST_Group0452_Reg20                  ; // 20  (ADDR : 0x9C00_E250)
    UINT32  RGST_Group0452_Reg21                  ; // 21  (ADDR : 0x9C00_E254)
    UINT32  RGST_Group0452_Reg22                  ; // 22  (ADDR : 0x9C00_E258)
    UINT32  RGST_Group0452_Reg23                  ; // 23  (ADDR : 0x9C00_E25C)
    UINT32  RGST_Group0452_Reg24                  ; // 24  (ADDR : 0x9C00_E260)
    UINT32  RGST_Group0452_Reg25                  ; // 25  (ADDR : 0x9C00_E264)
    UINT32  RGST_Group0452_Reg26                  ; // 26  (ADDR : 0x9C00_E268)
    UINT32  RGST_Group0452_Reg27                  ; // 27  (ADDR : 0x9C00_E26C)
    UINT32  RGST_Group0452_Reg28                  ; // 28  (ADDR : 0x9C00_E270)
    UINT32  RGST_Group0452_Reg29                  ; // 29  (ADDR : 0x9C00_E274)
    UINT32  RGST_Group0452_Reg30                  ; // 30  (ADDR : 0x9C00_E278)
    UINT32  RGST_Group0452_Reg31                  ; // 31  (ADDR : 0x9C00_E27C)

    //Group   453 : MFD
    UINT32  RGST_Group0453_Reg00                  ; // 00  (ADDR : 0x9C00_E280)
    UINT32  RGST_Group0453_Reg01                  ; // 01  (ADDR : 0x9C00_E284)
    UINT32  RGST_Group0453_Reg02                  ; // 02  (ADDR : 0x9C00_E288)
    UINT32  RGST_Group0453_Reg03                  ; // 03  (ADDR : 0x9C00_E28C)
    UINT32  RGST_Group0453_Reg04                  ; // 04  (ADDR : 0x9C00_E290)
    UINT32  RGST_Group0453_Reg05                  ; // 05  (ADDR : 0x9C00_E294)
    UINT32  RGST_Group0453_Reg06                  ; // 06  (ADDR : 0x9C00_E298)
    UINT32  RGST_Group0453_Reg07                  ; // 07  (ADDR : 0x9C00_E29C)
    UINT32  RGST_Group0453_Reg08                  ; // 08  (ADDR : 0x9C00_E2A0)
    UINT32  RGST_Group0453_Reg09                  ; // 09  (ADDR : 0x9C00_E2A4)
    UINT32  RGST_Group0453_Reg10                  ; // 10  (ADDR : 0x9C00_E2A8)
    UINT32  RGST_Group0453_Reg11                  ; // 11  (ADDR : 0x9C00_E2AC)
    UINT32  RGST_Group0453_Reg12                  ; // 12  (ADDR : 0x9C00_E2B0)
    UINT32  RGST_Group0453_Reg13                  ; // 13  (ADDR : 0x9C00_E2B4)
    UINT32  RGST_Group0453_Reg14                  ; // 14  (ADDR : 0x9C00_E2B8)
    UINT32  RGST_Group0453_Reg15                  ; // 15  (ADDR : 0x9C00_E2BC)
    UINT32  RGST_Group0453_Reg16                  ; // 16  (ADDR : 0x9C00_E2C0)
    UINT32  RGST_Group0453_Reg17                  ; // 17  (ADDR : 0x9C00_E2C4)
    UINT32  RGST_Group0453_Reg18                  ; // 18  (ADDR : 0x9C00_E2C8)
    UINT32  RGST_Group0453_Reg19                  ; // 19  (ADDR : 0x9C00_E2CC)
    UINT32  RGST_Group0453_Reg20                  ; // 20  (ADDR : 0x9C00_E2D0)
    UINT32  RGST_Group0453_Reg21                  ; // 21  (ADDR : 0x9C00_E2D4)
    UINT32  RGST_Group0453_Reg22                  ; // 22  (ADDR : 0x9C00_E2D8)
    UINT32  RGST_Group0453_Reg23                  ; // 23  (ADDR : 0x9C00_E2DC)
    UINT32  RGST_Group0453_Reg24                  ; // 24  (ADDR : 0x9C00_E2E0)
    UINT32  RGST_Group0453_Reg25                  ; // 25  (ADDR : 0x9C00_E2E4)
    UINT32  RGST_Group0453_Reg26                  ; // 26  (ADDR : 0x9C00_E2E8)
    UINT32  RGST_Group0453_Reg27                  ; // 27  (ADDR : 0x9C00_E2EC)
    UINT32  RGST_Group0453_Reg28                  ; // 28  (ADDR : 0x9C00_E2F0)
    UINT32  RGST_Group0453_Reg29                  ; // 29  (ADDR : 0x9C00_E2F4)
    UINT32  RGST_Group0453_Reg30                  ; // 30  (ADDR : 0x9C00_E2F8)
    UINT32  RGST_Group0453_Reg31                  ; // 31  (ADDR : 0x9C00_E2FC)

    //Group   454 : MFD
    UINT32  RGST_Group0454_Reg00                  ; // 00  (ADDR : 0x9C00_E300)
    UINT32  RGST_Group0454_Reg01                  ; // 01  (ADDR : 0x9C00_E304)
    UINT32  RGST_Group0454_Reg02                  ; // 02  (ADDR : 0x9C00_E308)
    UINT32  RGST_Group0454_Reg03                  ; // 03  (ADDR : 0x9C00_E30C)
    UINT32  RGST_Group0454_Reg04                  ; // 04  (ADDR : 0x9C00_E310)
    UINT32  RGST_Group0454_Reg05                  ; // 05  (ADDR : 0x9C00_E314)
    UINT32  RGST_Group0454_Reg06                  ; // 06  (ADDR : 0x9C00_E318)
    UINT32  RGST_Group0454_Reg07                  ; // 07  (ADDR : 0x9C00_E31C)
    UINT32  RGST_Group0454_Reg08                  ; // 08  (ADDR : 0x9C00_E320)
    UINT32  RGST_Group0454_Reg09                  ; // 09  (ADDR : 0x9C00_E324)
    UINT32  RGST_Group0454_Reg10                  ; // 10  (ADDR : 0x9C00_E328)
    UINT32  RGST_Group0454_Reg11                  ; // 11  (ADDR : 0x9C00_E32C)
    UINT32  RGST_Group0454_Reg12                  ; // 12  (ADDR : 0x9C00_E330)
    UINT32  RGST_Group0454_Reg13                  ; // 13  (ADDR : 0x9C00_E334)
    UINT32  RGST_Group0454_Reg14                  ; // 14  (ADDR : 0x9C00_E338)
    UINT32  RGST_Group0454_Reg15                  ; // 15  (ADDR : 0x9C00_E33C)
    UINT32  RGST_Group0454_Reg16                  ; // 16  (ADDR : 0x9C00_E340)
    UINT32  RGST_Group0454_Reg17                  ; // 17  (ADDR : 0x9C00_E344)
    UINT32  RGST_Group0454_Reg18                  ; // 18  (ADDR : 0x9C00_E348)
    UINT32  RGST_Group0454_Reg19                  ; // 19  (ADDR : 0x9C00_E34C)
    UINT32  RGST_Group0454_Reg20                  ; // 20  (ADDR : 0x9C00_E350)
    UINT32  RGST_Group0454_Reg21                  ; // 21  (ADDR : 0x9C00_E354)
    UINT32  RGST_Group0454_Reg22                  ; // 22  (ADDR : 0x9C00_E358)
    UINT32  RGST_Group0454_Reg23                  ; // 23  (ADDR : 0x9C00_E35C)
    UINT32  RGST_Group0454_Reg24                  ; // 24  (ADDR : 0x9C00_E360)
    UINT32  RGST_Group0454_Reg25                  ; // 25  (ADDR : 0x9C00_E364)
    UINT32  RGST_Group0454_Reg26                  ; // 26  (ADDR : 0x9C00_E368)
    UINT32  RGST_Group0454_Reg27                  ; // 27  (ADDR : 0x9C00_E36C)
    UINT32  RGST_Group0454_Reg28                  ; // 28  (ADDR : 0x9C00_E370)
    UINT32  RGST_Group0454_Reg29                  ; // 29  (ADDR : 0x9C00_E374)
    UINT32  RGST_Group0454_Reg30                  ; // 30  (ADDR : 0x9C00_E378)
    UINT32  RGST_Group0454_Reg31                  ; // 31  (ADDR : 0x9C00_E37C)

    //Group   455 : MFD
    UINT32  RGST_Group0455_Reg00                  ; // 00  (ADDR : 0x9C00_E380)
    UINT32  RGST_Group0455_Reg01                  ; // 01  (ADDR : 0x9C00_E384)
    UINT32  RGST_Group0455_Reg02                  ; // 02  (ADDR : 0x9C00_E388)
    UINT32  RGST_Group0455_Reg03                  ; // 03  (ADDR : 0x9C00_E38C)
    UINT32  RGST_Group0455_Reg04                  ; // 04  (ADDR : 0x9C00_E390)
    UINT32  RGST_Group0455_Reg05                  ; // 05  (ADDR : 0x9C00_E394)
    UINT32  RGST_Group0455_Reg06                  ; // 06  (ADDR : 0x9C00_E398)
    UINT32  RGST_Group0455_Reg07                  ; // 07  (ADDR : 0x9C00_E39C)
    UINT32  RGST_Group0455_Reg08                  ; // 08  (ADDR : 0x9C00_E3A0)
    UINT32  RGST_Group0455_Reg09                  ; // 09  (ADDR : 0x9C00_E3A4)
    UINT32  RGST_Group0455_Reg10                  ; // 10  (ADDR : 0x9C00_E3A8)
    UINT32  RGST_Group0455_Reg11                  ; // 11  (ADDR : 0x9C00_E3AC)
    UINT32  RGST_Group0455_Reg12                  ; // 12  (ADDR : 0x9C00_E3B0)
    UINT32  RGST_Group0455_Reg13                  ; // 13  (ADDR : 0x9C00_E3B4)
    UINT32  RGST_Group0455_Reg14                  ; // 14  (ADDR : 0x9C00_E3B8)
    UINT32  RGST_Group0455_Reg15                  ; // 15  (ADDR : 0x9C00_E3BC)
    UINT32  RGST_Group0455_Reg16                  ; // 16  (ADDR : 0x9C00_E3C0)
    UINT32  RGST_Group0455_Reg17                  ; // 17  (ADDR : 0x9C00_E3C4)
    UINT32  RGST_Group0455_Reg18                  ; // 18  (ADDR : 0x9C00_E3C8)
    UINT32  RGST_Group0455_Reg19                  ; // 19  (ADDR : 0x9C00_E3CC)
    UINT32  RGST_Group0455_Reg20                  ; // 20  (ADDR : 0x9C00_E3D0)
    UINT32  RGST_Group0455_Reg21                  ; // 21  (ADDR : 0x9C00_E3D4)
    UINT32  RGST_Group0455_Reg22                  ; // 22  (ADDR : 0x9C00_E3D8)
    UINT32  RGST_Group0455_Reg23                  ; // 23  (ADDR : 0x9C00_E3DC)
    UINT32  RGST_Group0455_Reg24                  ; // 24  (ADDR : 0x9C00_E3E0)
    UINT32  RGST_Group0455_Reg25                  ; // 25  (ADDR : 0x9C00_E3E4)
    UINT32  RGST_Group0455_Reg26                  ; // 26  (ADDR : 0x9C00_E3E8)
    UINT32  RGST_Group0455_Reg27                  ; // 27  (ADDR : 0x9C00_E3EC)
    UINT32  RGST_Group0455_Reg28                  ; // 28  (ADDR : 0x9C00_E3F0)
    UINT32  RGST_Group0455_Reg29                  ; // 29  (ADDR : 0x9C00_E3F4)
    UINT32  RGST_Group0455_Reg30                  ; // 30  (ADDR : 0x9C00_E3F8)
    UINT32  RGST_Group0455_Reg31                  ; // 31  (ADDR : 0x9C00_E3FC)

    //Group   456 : MFD
    UINT32  RGST_Group0456_Reg00                  ; // 00  (ADDR : 0x9C00_E400)
    UINT32  RGST_Group0456_Reg01                  ; // 01  (ADDR : 0x9C00_E404)
    UINT32  RGST_Group0456_Reg02                  ; // 02  (ADDR : 0x9C00_E408)
    UINT32  RGST_Group0456_Reg03                  ; // 03  (ADDR : 0x9C00_E40C)
    UINT32  RGST_Group0456_Reg04                  ; // 04  (ADDR : 0x9C00_E410)
    UINT32  RGST_Group0456_Reg05                  ; // 05  (ADDR : 0x9C00_E414)
    UINT32  RGST_Group0456_Reg06                  ; // 06  (ADDR : 0x9C00_E418)
    UINT32  RGST_Group0456_Reg07                  ; // 07  (ADDR : 0x9C00_E41C)
    UINT32  RGST_Group0456_Reg08                  ; // 08  (ADDR : 0x9C00_E420)
    UINT32  RGST_Group0456_Reg09                  ; // 09  (ADDR : 0x9C00_E424)
    UINT32  RGST_Group0456_Reg10                  ; // 10  (ADDR : 0x9C00_E428)
    UINT32  RGST_Group0456_Reg11                  ; // 11  (ADDR : 0x9C00_E42C)
    UINT32  RGST_Group0456_Reg12                  ; // 12  (ADDR : 0x9C00_E430)
    UINT32  RGST_Group0456_Reg13                  ; // 13  (ADDR : 0x9C00_E434)
    UINT32  RGST_Group0456_Reg14                  ; // 14  (ADDR : 0x9C00_E438)
    UINT32  RGST_Group0456_Reg15                  ; // 15  (ADDR : 0x9C00_E43C)
    UINT32  RGST_Group0456_Reg16                  ; // 16  (ADDR : 0x9C00_E440)
    UINT32  RGST_Group0456_Reg17                  ; // 17  (ADDR : 0x9C00_E444)
    UINT32  RGST_Group0456_Reg18                  ; // 18  (ADDR : 0x9C00_E448)
    UINT32  RGST_Group0456_Reg19                  ; // 19  (ADDR : 0x9C00_E44C)
    UINT32  RGST_Group0456_Reg20                  ; // 20  (ADDR : 0x9C00_E450)
    UINT32  RGST_Group0456_Reg21                  ; // 21  (ADDR : 0x9C00_E454)
    UINT32  RGST_Group0456_Reg22                  ; // 22  (ADDR : 0x9C00_E458)
    UINT32  RGST_Group0456_Reg23                  ; // 23  (ADDR : 0x9C00_E45C)
    UINT32  RGST_Group0456_Reg24                  ; // 24  (ADDR : 0x9C00_E460)
    UINT32  RGST_Group0456_Reg25                  ; // 25  (ADDR : 0x9C00_E464)
    UINT32  RGST_Group0456_Reg26                  ; // 26  (ADDR : 0x9C00_E468)
    UINT32  RGST_Group0456_Reg27                  ; // 27  (ADDR : 0x9C00_E46C)
    UINT32  RGST_Group0456_Reg28                  ; // 28  (ADDR : 0x9C00_E470)
    UINT32  RGST_Group0456_Reg29                  ; // 29  (ADDR : 0x9C00_E474)
    UINT32  RGST_Group0456_Reg30                  ; // 30  (ADDR : 0x9C00_E478)
    UINT32  RGST_Group0456_Reg31                  ; // 31  (ADDR : 0x9C00_E47C)

    //Group   457 : MFD
    UINT32  RGST_Group0457_Reg00                  ; // 00  (ADDR : 0x9C00_E480)
    UINT32  RGST_Group0457_Reg01                  ; // 01  (ADDR : 0x9C00_E484)
    UINT32  RGST_Group0457_Reg02                  ; // 02  (ADDR : 0x9C00_E488)
    UINT32  RGST_Group0457_Reg03                  ; // 03  (ADDR : 0x9C00_E48C)
    UINT32  RGST_Group0457_Reg04                  ; // 04  (ADDR : 0x9C00_E490)
    UINT32  RGST_Group0457_Reg05                  ; // 05  (ADDR : 0x9C00_E494)
    UINT32  RGST_Group0457_Reg06                  ; // 06  (ADDR : 0x9C00_E498)
    UINT32  RGST_Group0457_Reg07                  ; // 07  (ADDR : 0x9C00_E49C)
    UINT32  RGST_Group0457_Reg08                  ; // 08  (ADDR : 0x9C00_E4A0)
    UINT32  RGST_Group0457_Reg09                  ; // 09  (ADDR : 0x9C00_E4A4)
    UINT32  RGST_Group0457_Reg10                  ; // 10  (ADDR : 0x9C00_E4A8)
    UINT32  RGST_Group0457_Reg11                  ; // 11  (ADDR : 0x9C00_E4AC)
    UINT32  RGST_Group0457_Reg12                  ; // 12  (ADDR : 0x9C00_E4B0)
    UINT32  RGST_Group0457_Reg13                  ; // 13  (ADDR : 0x9C00_E4B4)
    UINT32  RGST_Group0457_Reg14                  ; // 14  (ADDR : 0x9C00_E4B8)
    UINT32  RGST_Group0457_Reg15                  ; // 15  (ADDR : 0x9C00_E4BC)
    UINT32  RGST_Group0457_Reg16                  ; // 16  (ADDR : 0x9C00_E4C0)
    UINT32  RGST_Group0457_Reg17                  ; // 17  (ADDR : 0x9C00_E4C4)
    UINT32  RGST_Group0457_Reg18                  ; // 18  (ADDR : 0x9C00_E4C8)
    UINT32  RGST_Group0457_Reg19                  ; // 19  (ADDR : 0x9C00_E4CC)
    UINT32  RGST_Group0457_Reg20                  ; // 20  (ADDR : 0x9C00_E4D0)
    UINT32  RGST_Group0457_Reg21                  ; // 21  (ADDR : 0x9C00_E4D4)
    UINT32  RGST_Group0457_Reg22                  ; // 22  (ADDR : 0x9C00_E4D8)
    UINT32  RGST_Group0457_Reg23                  ; // 23  (ADDR : 0x9C00_E4DC)
    UINT32  RGST_Group0457_Reg24                  ; // 24  (ADDR : 0x9C00_E4E0)
    UINT32  RGST_Group0457_Reg25                  ; // 25  (ADDR : 0x9C00_E4E4)
    UINT32  RGST_Group0457_Reg26                  ; // 26  (ADDR : 0x9C00_E4E8)
    UINT32  RGST_Group0457_Reg27                  ; // 27  (ADDR : 0x9C00_E4EC)
    UINT32  RGST_Group0457_Reg28                  ; // 28  (ADDR : 0x9C00_E4F0)
    UINT32  RGST_Group0457_Reg29                  ; // 29  (ADDR : 0x9C00_E4F4)
    UINT32  RGST_Group0457_Reg30                  ; // 30  (ADDR : 0x9C00_E4F8)
    UINT32  RGST_Group0457_Reg31                  ; // 31  (ADDR : 0x9C00_E4FC)

    //Group   458 : MFD
    UINT32  RGST_Group0458_Reg00                  ; // 00  (ADDR : 0x9C00_E500)
    UINT32  RGST_Group0458_Reg01                  ; // 01  (ADDR : 0x9C00_E504)
    UINT32  RGST_Group0458_Reg02                  ; // 02  (ADDR : 0x9C00_E508)
    UINT32  RGST_Group0458_Reg03                  ; // 03  (ADDR : 0x9C00_E50C)
    UINT32  RGST_Group0458_Reg04                  ; // 04  (ADDR : 0x9C00_E510)
    UINT32  RGST_Group0458_Reg05                  ; // 05  (ADDR : 0x9C00_E514)
    UINT32  RGST_Group0458_Reg06                  ; // 06  (ADDR : 0x9C00_E518)
    UINT32  RGST_Group0458_Reg07                  ; // 07  (ADDR : 0x9C00_E51C)
    UINT32  RGST_Group0458_Reg08                  ; // 08  (ADDR : 0x9C00_E520)
    UINT32  RGST_Group0458_Reg09                  ; // 09  (ADDR : 0x9C00_E524)
    UINT32  RGST_Group0458_Reg10                  ; // 10  (ADDR : 0x9C00_E528)
    UINT32  RGST_Group0458_Reg11                  ; // 11  (ADDR : 0x9C00_E52C)
    UINT32  RGST_Group0458_Reg12                  ; // 12  (ADDR : 0x9C00_E530)
    UINT32  RGST_Group0458_Reg13                  ; // 13  (ADDR : 0x9C00_E534)
    UINT32  RGST_Group0458_Reg14                  ; // 14  (ADDR : 0x9C00_E538)
    UINT32  RGST_Group0458_Reg15                  ; // 15  (ADDR : 0x9C00_E53C)
    UINT32  RGST_Group0458_Reg16                  ; // 16  (ADDR : 0x9C00_E540)
    UINT32  RGST_Group0458_Reg17                  ; // 17  (ADDR : 0x9C00_E544)
    UINT32  RGST_Group0458_Reg18                  ; // 18  (ADDR : 0x9C00_E548)
    UINT32  RGST_Group0458_Reg19                  ; // 19  (ADDR : 0x9C00_E54C)
    UINT32  RGST_Group0458_Reg20                  ; // 20  (ADDR : 0x9C00_E550)
    UINT32  RGST_Group0458_Reg21                  ; // 21  (ADDR : 0x9C00_E554)
    UINT32  RGST_Group0458_Reg22                  ; // 22  (ADDR : 0x9C00_E558)
    UINT32  RGST_Group0458_Reg23                  ; // 23  (ADDR : 0x9C00_E55C)
    UINT32  RGST_Group0458_Reg24                  ; // 24  (ADDR : 0x9C00_E560)
    UINT32  RGST_Group0458_Reg25                  ; // 25  (ADDR : 0x9C00_E564)
    UINT32  RGST_Group0458_Reg26                  ; // 26  (ADDR : 0x9C00_E568)
    UINT32  RGST_Group0458_Reg27                  ; // 27  (ADDR : 0x9C00_E56C)
    UINT32  RGST_Group0458_Reg28                  ; // 28  (ADDR : 0x9C00_E570)
    UINT32  RGST_Group0458_Reg29                  ; // 29  (ADDR : 0x9C00_E574)
    UINT32  RGST_Group0458_Reg30                  ; // 30  (ADDR : 0x9C00_E578)
    UINT32  RGST_Group0458_Reg31                  ; // 31  (ADDR : 0x9C00_E57C)

    //Group   459 : MFD
    UINT32  RGST_Group0459_Reg00                  ; // 00  (ADDR : 0x9C00_E580)
    UINT32  RGST_Group0459_Reg01                  ; // 01  (ADDR : 0x9C00_E584)
    UINT32  RGST_Group0459_Reg02                  ; // 02  (ADDR : 0x9C00_E588)
    UINT32  RGST_Group0459_Reg03                  ; // 03  (ADDR : 0x9C00_E58C)
    UINT32  RGST_Group0459_Reg04                  ; // 04  (ADDR : 0x9C00_E590)
    UINT32  RGST_Group0459_Reg05                  ; // 05  (ADDR : 0x9C00_E594)
    UINT32  RGST_Group0459_Reg06                  ; // 06  (ADDR : 0x9C00_E598)
    UINT32  RGST_Group0459_Reg07                  ; // 07  (ADDR : 0x9C00_E59C)
    UINT32  RGST_Group0459_Reg08                  ; // 08  (ADDR : 0x9C00_E5A0)
    UINT32  RGST_Group0459_Reg09                  ; // 09  (ADDR : 0x9C00_E5A4)
    UINT32  RGST_Group0459_Reg10                  ; // 10  (ADDR : 0x9C00_E5A8)
    UINT32  RGST_Group0459_Reg11                  ; // 11  (ADDR : 0x9C00_E5AC)
    UINT32  RGST_Group0459_Reg12                  ; // 12  (ADDR : 0x9C00_E5B0)
    UINT32  RGST_Group0459_Reg13                  ; // 13  (ADDR : 0x9C00_E5B4)
    UINT32  RGST_Group0459_Reg14                  ; // 14  (ADDR : 0x9C00_E5B8)
    UINT32  RGST_Group0459_Reg15                  ; // 15  (ADDR : 0x9C00_E5BC)
    UINT32  RGST_Group0459_Reg16                  ; // 16  (ADDR : 0x9C00_E5C0)
    UINT32  RGST_Group0459_Reg17                  ; // 17  (ADDR : 0x9C00_E5C4)
    UINT32  RGST_Group0459_Reg18                  ; // 18  (ADDR : 0x9C00_E5C8)
    UINT32  RGST_Group0459_Reg19                  ; // 19  (ADDR : 0x9C00_E5CC)
    UINT32  RGST_Group0459_Reg20                  ; // 20  (ADDR : 0x9C00_E5D0)
    UINT32  RGST_Group0459_Reg21                  ; // 21  (ADDR : 0x9C00_E5D4)
    UINT32  RGST_Group0459_Reg22                  ; // 22  (ADDR : 0x9C00_E5D8)
    UINT32  RGST_Group0459_Reg23                  ; // 23  (ADDR : 0x9C00_E5DC)
    UINT32  RGST_Group0459_Reg24                  ; // 24  (ADDR : 0x9C00_E5E0)
    UINT32  RGST_Group0459_Reg25                  ; // 25  (ADDR : 0x9C00_E5E4)
    UINT32  RGST_Group0459_Reg26                  ; // 26  (ADDR : 0x9C00_E5E8)
    UINT32  RGST_Group0459_Reg27                  ; // 27  (ADDR : 0x9C00_E5EC)
    UINT32  RGST_Group0459_Reg28                  ; // 28  (ADDR : 0x9C00_E5F0)
    UINT32  RGST_Group0459_Reg29                  ; // 29  (ADDR : 0x9C00_E5F4)
    UINT32  RGST_Group0459_Reg30                  ; // 30  (ADDR : 0x9C00_E5F8)
    UINT32  RGST_Group0459_Reg31                  ; // 31  (ADDR : 0x9C00_E5FC)

    //Group   460 : MFD
    UINT32  RGST_Group0460_Reg00                  ; // 00  (ADDR : 0x9C00_E600)
    UINT32  RGST_Group0460_Reg01                  ; // 01  (ADDR : 0x9C00_E604)
    UINT32  RGST_Group0460_Reg02                  ; // 02  (ADDR : 0x9C00_E608)
    UINT32  RGST_Group0460_Reg03                  ; // 03  (ADDR : 0x9C00_E60C)
    UINT32  RGST_Group0460_Reg04                  ; // 04  (ADDR : 0x9C00_E610)
    UINT32  RGST_Group0460_Reg05                  ; // 05  (ADDR : 0x9C00_E614)
    UINT32  RGST_Group0460_Reg06                  ; // 06  (ADDR : 0x9C00_E618)
    UINT32  RGST_Group0460_Reg07                  ; // 07  (ADDR : 0x9C00_E61C)
    UINT32  RGST_Group0460_Reg08                  ; // 08  (ADDR : 0x9C00_E620)
    UINT32  RGST_Group0460_Reg09                  ; // 09  (ADDR : 0x9C00_E624)
    UINT32  RGST_Group0460_Reg10                  ; // 10  (ADDR : 0x9C00_E628)
    UINT32  RGST_Group0460_Reg11                  ; // 11  (ADDR : 0x9C00_E62C)
    UINT32  RGST_Group0460_Reg12                  ; // 12  (ADDR : 0x9C00_E630)
    UINT32  RGST_Group0460_Reg13                  ; // 13  (ADDR : 0x9C00_E634)
    UINT32  RGST_Group0460_Reg14                  ; // 14  (ADDR : 0x9C00_E638)
    UINT32  RGST_Group0460_Reg15                  ; // 15  (ADDR : 0x9C00_E63C)
    UINT32  RGST_Group0460_Reg16                  ; // 16  (ADDR : 0x9C00_E640)
    UINT32  RGST_Group0460_Reg17                  ; // 17  (ADDR : 0x9C00_E644)
    UINT32  RGST_Group0460_Reg18                  ; // 18  (ADDR : 0x9C00_E648)
    UINT32  RGST_Group0460_Reg19                  ; // 19  (ADDR : 0x9C00_E64C)
    UINT32  RGST_Group0460_Reg20                  ; // 20  (ADDR : 0x9C00_E650)
    UINT32  RGST_Group0460_Reg21                  ; // 21  (ADDR : 0x9C00_E654)
    UINT32  RGST_Group0460_Reg22                  ; // 22  (ADDR : 0x9C00_E658)
    UINT32  RGST_Group0460_Reg23                  ; // 23  (ADDR : 0x9C00_E65C)
    UINT32  RGST_Group0460_Reg24                  ; // 24  (ADDR : 0x9C00_E660)
    UINT32  RGST_Group0460_Reg25                  ; // 25  (ADDR : 0x9C00_E664)
    UINT32  RGST_Group0460_Reg26                  ; // 26  (ADDR : 0x9C00_E668)
    UINT32  RGST_Group0460_Reg27                  ; // 27  (ADDR : 0x9C00_E66C)
    UINT32  RGST_Group0460_Reg28                  ; // 28  (ADDR : 0x9C00_E670)
    UINT32  RGST_Group0460_Reg29                  ; // 29  (ADDR : 0x9C00_E674)
    UINT32  RGST_Group0460_Reg30                  ; // 30  (ADDR : 0x9C00_E678)
    UINT32  RGST_Group0460_Reg31                  ; // 31  (ADDR : 0x9C00_E67C)

    //Group   461 : MFD
    UINT32  RGST_Group0461_Reg00                  ; // 00  (ADDR : 0x9C00_E680)
    UINT32  RGST_Group0461_Reg01                  ; // 01  (ADDR : 0x9C00_E684)
    UINT32  RGST_Group0461_Reg02                  ; // 02  (ADDR : 0x9C00_E688)
    UINT32  RGST_Group0461_Reg03                  ; // 03  (ADDR : 0x9C00_E68C)
    UINT32  RGST_Group0461_Reg04                  ; // 04  (ADDR : 0x9C00_E690)
    UINT32  RGST_Group0461_Reg05                  ; // 05  (ADDR : 0x9C00_E694)
    UINT32  RGST_Group0461_Reg06                  ; // 06  (ADDR : 0x9C00_E698)
    UINT32  RGST_Group0461_Reg07                  ; // 07  (ADDR : 0x9C00_E69C)
    UINT32  RGST_Group0461_Reg08                  ; // 08  (ADDR : 0x9C00_E6A0)
    UINT32  RGST_Group0461_Reg09                  ; // 09  (ADDR : 0x9C00_E6A4)
    UINT32  RGST_Group0461_Reg10                  ; // 10  (ADDR : 0x9C00_E6A8)
    UINT32  RGST_Group0461_Reg11                  ; // 11  (ADDR : 0x9C00_E6AC)
    UINT32  RGST_Group0461_Reg12                  ; // 12  (ADDR : 0x9C00_E6B0)
    UINT32  RGST_Group0461_Reg13                  ; // 13  (ADDR : 0x9C00_E6B4)
    UINT32  RGST_Group0461_Reg14                  ; // 14  (ADDR : 0x9C00_E6B8)
    UINT32  RGST_Group0461_Reg15                  ; // 15  (ADDR : 0x9C00_E6BC)
    UINT32  RGST_Group0461_Reg16                  ; // 16  (ADDR : 0x9C00_E6C0)
    UINT32  RGST_Group0461_Reg17                  ; // 17  (ADDR : 0x9C00_E6C4)
    UINT32  RGST_Group0461_Reg18                  ; // 18  (ADDR : 0x9C00_E6C8)
    UINT32  RGST_Group0461_Reg19                  ; // 19  (ADDR : 0x9C00_E6CC)
    UINT32  RGST_Group0461_Reg20                  ; // 20  (ADDR : 0x9C00_E6D0)
    UINT32  RGST_Group0461_Reg21                  ; // 21  (ADDR : 0x9C00_E6D4)
    UINT32  RGST_Group0461_Reg22                  ; // 22  (ADDR : 0x9C00_E6D8)
    UINT32  RGST_Group0461_Reg23                  ; // 23  (ADDR : 0x9C00_E6DC)
    UINT32  RGST_Group0461_Reg24                  ; // 24  (ADDR : 0x9C00_E6E0)
    UINT32  RGST_Group0461_Reg25                  ; // 25  (ADDR : 0x9C00_E6E4)
    UINT32  RGST_Group0461_Reg26                  ; // 26  (ADDR : 0x9C00_E6E8)
    UINT32  RGST_Group0461_Reg27                  ; // 27  (ADDR : 0x9C00_E6EC)
    UINT32  RGST_Group0461_Reg28                  ; // 28  (ADDR : 0x9C00_E6F0)
    UINT32  RGST_Group0461_Reg29                  ; // 29  (ADDR : 0x9C00_E6F4)
    UINT32  RGST_Group0461_Reg30                  ; // 30  (ADDR : 0x9C00_E6F8)
    UINT32  RGST_Group0461_Reg31                  ; // 31  (ADDR : 0x9C00_E6FC)

    //Group   462 : MFD
    UINT32  RGST_Group0462_Reg00                  ; // 00  (ADDR : 0x9C00_E700)
    UINT32  RGST_Group0462_Reg01                  ; // 01  (ADDR : 0x9C00_E704)
    UINT32  RGST_Group0462_Reg02                  ; // 02  (ADDR : 0x9C00_E708)
    UINT32  RGST_Group0462_Reg03                  ; // 03  (ADDR : 0x9C00_E70C)
    UINT32  RGST_Group0462_Reg04                  ; // 04  (ADDR : 0x9C00_E710)
    UINT32  RGST_Group0462_Reg05                  ; // 05  (ADDR : 0x9C00_E714)
    UINT32  RGST_Group0462_Reg06                  ; // 06  (ADDR : 0x9C00_E718)
    UINT32  RGST_Group0462_Reg07                  ; // 07  (ADDR : 0x9C00_E71C)
    UINT32  RGST_Group0462_Reg08                  ; // 08  (ADDR : 0x9C00_E720)
    UINT32  RGST_Group0462_Reg09                  ; // 09  (ADDR : 0x9C00_E724)
    UINT32  RGST_Group0462_Reg10                  ; // 10  (ADDR : 0x9C00_E728)
    UINT32  RGST_Group0462_Reg11                  ; // 11  (ADDR : 0x9C00_E72C)
    UINT32  RGST_Group0462_Reg12                  ; // 12  (ADDR : 0x9C00_E730)
    UINT32  RGST_Group0462_Reg13                  ; // 13  (ADDR : 0x9C00_E734)
    UINT32  RGST_Group0462_Reg14                  ; // 14  (ADDR : 0x9C00_E738)
    UINT32  RGST_Group0462_Reg15                  ; // 15  (ADDR : 0x9C00_E73C)
    UINT32  RGST_Group0462_Reg16                  ; // 16  (ADDR : 0x9C00_E740)
    UINT32  RGST_Group0462_Reg17                  ; // 17  (ADDR : 0x9C00_E744)
    UINT32  RGST_Group0462_Reg18                  ; // 18  (ADDR : 0x9C00_E748)
    UINT32  RGST_Group0462_Reg19                  ; // 19  (ADDR : 0x9C00_E74C)
    UINT32  RGST_Group0462_Reg20                  ; // 20  (ADDR : 0x9C00_E750)
    UINT32  RGST_Group0462_Reg21                  ; // 21  (ADDR : 0x9C00_E754)
    UINT32  RGST_Group0462_Reg22                  ; // 22  (ADDR : 0x9C00_E758)
    UINT32  RGST_Group0462_Reg23                  ; // 23  (ADDR : 0x9C00_E75C)
    UINT32  RGST_Group0462_Reg24                  ; // 24  (ADDR : 0x9C00_E760)
    UINT32  RGST_Group0462_Reg25                  ; // 25  (ADDR : 0x9C00_E764)
    UINT32  RGST_Group0462_Reg26                  ; // 26  (ADDR : 0x9C00_E768)
    UINT32  RGST_Group0462_Reg27                  ; // 27  (ADDR : 0x9C00_E76C)
    UINT32  RGST_Group0462_Reg28                  ; // 28  (ADDR : 0x9C00_E770)
    UINT32  RGST_Group0462_Reg29                  ; // 29  (ADDR : 0x9C00_E774)
    UINT32  RGST_Group0462_Reg30                  ; // 30  (ADDR : 0x9C00_E778)
    UINT32  RGST_Group0462_Reg31                  ; // 31  (ADDR : 0x9C00_E77C)

    //Group   463 : MFD
    UINT32  RGST_Group0463_Reg00                  ; // 00  (ADDR : 0x9C00_E780)
    UINT32  RGST_Group0463_Reg01                  ; // 01  (ADDR : 0x9C00_E784)
    UINT32  RGST_Group0463_Reg02                  ; // 02  (ADDR : 0x9C00_E788)
    UINT32  RGST_Group0463_Reg03                  ; // 03  (ADDR : 0x9C00_E78C)
    UINT32  RGST_Group0463_Reg04                  ; // 04  (ADDR : 0x9C00_E790)
    UINT32  RGST_Group0463_Reg05                  ; // 05  (ADDR : 0x9C00_E794)
    UINT32  RGST_Group0463_Reg06                  ; // 06  (ADDR : 0x9C00_E798)
    UINT32  RGST_Group0463_Reg07                  ; // 07  (ADDR : 0x9C00_E79C)
    UINT32  RGST_Group0463_Reg08                  ; // 08  (ADDR : 0x9C00_E7A0)
    UINT32  RGST_Group0463_Reg09                  ; // 09  (ADDR : 0x9C00_E7A4)
    UINT32  RGST_Group0463_Reg10                  ; // 10  (ADDR : 0x9C00_E7A8)
    UINT32  RGST_Group0463_Reg11                  ; // 11  (ADDR : 0x9C00_E7AC)
    UINT32  RGST_Group0463_Reg12                  ; // 12  (ADDR : 0x9C00_E7B0)
    UINT32  RGST_Group0463_Reg13                  ; // 13  (ADDR : 0x9C00_E7B4)
    UINT32  RGST_Group0463_Reg14                  ; // 14  (ADDR : 0x9C00_E7B8)
    UINT32  RGST_Group0463_Reg15                  ; // 15  (ADDR : 0x9C00_E7BC)
    UINT32  RGST_Group0463_Reg16                  ; // 16  (ADDR : 0x9C00_E7C0)
    UINT32  RGST_Group0463_Reg17                  ; // 17  (ADDR : 0x9C00_E7C4)
    UINT32  RGST_Group0463_Reg18                  ; // 18  (ADDR : 0x9C00_E7C8)
    UINT32  RGST_Group0463_Reg19                  ; // 19  (ADDR : 0x9C00_E7CC)
    UINT32  RGST_Group0463_Reg20                  ; // 20  (ADDR : 0x9C00_E7D0)
    UINT32  RGST_Group0463_Reg21                  ; // 21  (ADDR : 0x9C00_E7D4)
    UINT32  RGST_Group0463_Reg22                  ; // 22  (ADDR : 0x9C00_E7D8)
    UINT32  RGST_Group0463_Reg23                  ; // 23  (ADDR : 0x9C00_E7DC)
    UINT32  RGST_Group0463_Reg24                  ; // 24  (ADDR : 0x9C00_E7E0)
    UINT32  RGST_Group0463_Reg25                  ; // 25  (ADDR : 0x9C00_E7E4)
    UINT32  RGST_Group0463_Reg26                  ; // 26  (ADDR : 0x9C00_E7E8)
    UINT32  RGST_Group0463_Reg27                  ; // 27  (ADDR : 0x9C00_E7EC)
    UINT32  RGST_Group0463_Reg28                  ; // 28  (ADDR : 0x9C00_E7F0)
    UINT32  RGST_Group0463_Reg29                  ; // 29  (ADDR : 0x9C00_E7F4)
    UINT32  RGST_Group0463_Reg30                  ; // 30  (ADDR : 0x9C00_E7F8)
    UINT32  RGST_Group0463_Reg31                  ; // 31  (ADDR : 0x9C00_E7FC)

    //Group   464 : MFD
    UINT32  RGST_Group0464_Reg00                  ; // 00  (ADDR : 0x9C00_E800)
    UINT32  RGST_Group0464_Reg01                  ; // 01  (ADDR : 0x9C00_E804)
    UINT32  RGST_Group0464_Reg02                  ; // 02  (ADDR : 0x9C00_E808)
    UINT32  RGST_Group0464_Reg03                  ; // 03  (ADDR : 0x9C00_E80C)
    UINT32  RGST_Group0464_Reg04                  ; // 04  (ADDR : 0x9C00_E810)
    UINT32  RGST_Group0464_Reg05                  ; // 05  (ADDR : 0x9C00_E814)
    UINT32  RGST_Group0464_Reg06                  ; // 06  (ADDR : 0x9C00_E818)
    UINT32  RGST_Group0464_Reg07                  ; // 07  (ADDR : 0x9C00_E81C)
    UINT32  RGST_Group0464_Reg08                  ; // 08  (ADDR : 0x9C00_E820)
    UINT32  RGST_Group0464_Reg09                  ; // 09  (ADDR : 0x9C00_E824)
    UINT32  RGST_Group0464_Reg10                  ; // 10  (ADDR : 0x9C00_E828)
    UINT32  RGST_Group0464_Reg11                  ; // 11  (ADDR : 0x9C00_E82C)
    UINT32  RGST_Group0464_Reg12                  ; // 12  (ADDR : 0x9C00_E830)
    UINT32  RGST_Group0464_Reg13                  ; // 13  (ADDR : 0x9C00_E834)
    UINT32  RGST_Group0464_Reg14                  ; // 14  (ADDR : 0x9C00_E838)
    UINT32  RGST_Group0464_Reg15                  ; // 15  (ADDR : 0x9C00_E83C)
    UINT32  RGST_Group0464_Reg16                  ; // 16  (ADDR : 0x9C00_E840)
    UINT32  RGST_Group0464_Reg17                  ; // 17  (ADDR : 0x9C00_E844)
    UINT32  RGST_Group0464_Reg18                  ; // 18  (ADDR : 0x9C00_E848)
    UINT32  RGST_Group0464_Reg19                  ; // 19  (ADDR : 0x9C00_E84C)
    UINT32  RGST_Group0464_Reg20                  ; // 20  (ADDR : 0x9C00_E850)
    UINT32  RGST_Group0464_Reg21                  ; // 21  (ADDR : 0x9C00_E854)
    UINT32  RGST_Group0464_Reg22                  ; // 22  (ADDR : 0x9C00_E858)
    UINT32  RGST_Group0464_Reg23                  ; // 23  (ADDR : 0x9C00_E85C)
    UINT32  RGST_Group0464_Reg24                  ; // 24  (ADDR : 0x9C00_E860)
    UINT32  RGST_Group0464_Reg25                  ; // 25  (ADDR : 0x9C00_E864)
    UINT32  RGST_Group0464_Reg26                  ; // 26  (ADDR : 0x9C00_E868)
    UINT32  RGST_Group0464_Reg27                  ; // 27  (ADDR : 0x9C00_E86C)
    UINT32  RGST_Group0464_Reg28                  ; // 28  (ADDR : 0x9C00_E870)
    UINT32  RGST_Group0464_Reg29                  ; // 29  (ADDR : 0x9C00_E874)
    UINT32  RGST_Group0464_Reg30                  ; // 30  (ADDR : 0x9C00_E878)
    UINT32  RGST_Group0464_Reg31                  ; // 31  (ADDR : 0x9C00_E87C)

    //Group   465 : MFD
    UINT32  RGST_Group0465_Reg00                  ; // 00  (ADDR : 0x9C00_E880)
    UINT32  RGST_Group0465_Reg01                  ; // 01  (ADDR : 0x9C00_E884)
    UINT32  RGST_Group0465_Reg02                  ; // 02  (ADDR : 0x9C00_E888)
    UINT32  RGST_Group0465_Reg03                  ; // 03  (ADDR : 0x9C00_E88C)
    UINT32  RGST_Group0465_Reg04                  ; // 04  (ADDR : 0x9C00_E890)
    UINT32  RGST_Group0465_Reg05                  ; // 05  (ADDR : 0x9C00_E894)
    UINT32  RGST_Group0465_Reg06                  ; // 06  (ADDR : 0x9C00_E898)
    UINT32  RGST_Group0465_Reg07                  ; // 07  (ADDR : 0x9C00_E89C)
    UINT32  RGST_Group0465_Reg08                  ; // 08  (ADDR : 0x9C00_E8A0)
    UINT32  RGST_Group0465_Reg09                  ; // 09  (ADDR : 0x9C00_E8A4)
    UINT32  RGST_Group0465_Reg10                  ; // 10  (ADDR : 0x9C00_E8A8)
    UINT32  RGST_Group0465_Reg11                  ; // 11  (ADDR : 0x9C00_E8AC)
    UINT32  RGST_Group0465_Reg12                  ; // 12  (ADDR : 0x9C00_E8B0)
    UINT32  RGST_Group0465_Reg13                  ; // 13  (ADDR : 0x9C00_E8B4)
    UINT32  RGST_Group0465_Reg14                  ; // 14  (ADDR : 0x9C00_E8B8)
    UINT32  RGST_Group0465_Reg15                  ; // 15  (ADDR : 0x9C00_E8BC)
    UINT32  RGST_Group0465_Reg16                  ; // 16  (ADDR : 0x9C00_E8C0)
    UINT32  RGST_Group0465_Reg17                  ; // 17  (ADDR : 0x9C00_E8C4)
    UINT32  RGST_Group0465_Reg18                  ; // 18  (ADDR : 0x9C00_E8C8)
    UINT32  RGST_Group0465_Reg19                  ; // 19  (ADDR : 0x9C00_E8CC)
    UINT32  RGST_Group0465_Reg20                  ; // 20  (ADDR : 0x9C00_E8D0)
    UINT32  RGST_Group0465_Reg21                  ; // 21  (ADDR : 0x9C00_E8D4)
    UINT32  RGST_Group0465_Reg22                  ; // 22  (ADDR : 0x9C00_E8D8)
    UINT32  RGST_Group0465_Reg23                  ; // 23  (ADDR : 0x9C00_E8DC)
    UINT32  RGST_Group0465_Reg24                  ; // 24  (ADDR : 0x9C00_E8E0)
    UINT32  RGST_Group0465_Reg25                  ; // 25  (ADDR : 0x9C00_E8E4)
    UINT32  RGST_Group0465_Reg26                  ; // 26  (ADDR : 0x9C00_E8E8)
    UINT32  RGST_Group0465_Reg27                  ; // 27  (ADDR : 0x9C00_E8EC)
    UINT32  RGST_Group0465_Reg28                  ; // 28  (ADDR : 0x9C00_E8F0)
    UINT32  RGST_Group0465_Reg29                  ; // 29  (ADDR : 0x9C00_E8F4)
    UINT32  RGST_Group0465_Reg30                  ; // 30  (ADDR : 0x9C00_E8F8)
    UINT32  RGST_Group0465_Reg31                  ; // 31  (ADDR : 0x9C00_E8FC)

    //Group   466 : MFD
    UINT32  RGST_Group0466_Reg00                  ; // 00  (ADDR : 0x9C00_E900)
    UINT32  RGST_Group0466_Reg01                  ; // 01  (ADDR : 0x9C00_E904)
    UINT32  RGST_Group0466_Reg02                  ; // 02  (ADDR : 0x9C00_E908)
    UINT32  RGST_Group0466_Reg03                  ; // 03  (ADDR : 0x9C00_E90C)
    UINT32  RGST_Group0466_Reg04                  ; // 04  (ADDR : 0x9C00_E910)
    UINT32  RGST_Group0466_Reg05                  ; // 05  (ADDR : 0x9C00_E914)
    UINT32  RGST_Group0466_Reg06                  ; // 06  (ADDR : 0x9C00_E918)
    UINT32  RGST_Group0466_Reg07                  ; // 07  (ADDR : 0x9C00_E91C)
    UINT32  RGST_Group0466_Reg08                  ; // 08  (ADDR : 0x9C00_E920)
    UINT32  RGST_Group0466_Reg09                  ; // 09  (ADDR : 0x9C00_E924)
    UINT32  RGST_Group0466_Reg10                  ; // 10  (ADDR : 0x9C00_E928)
    UINT32  RGST_Group0466_Reg11                  ; // 11  (ADDR : 0x9C00_E92C)
    UINT32  RGST_Group0466_Reg12                  ; // 12  (ADDR : 0x9C00_E930)
    UINT32  RGST_Group0466_Reg13                  ; // 13  (ADDR : 0x9C00_E934)
    UINT32  RGST_Group0466_Reg14                  ; // 14  (ADDR : 0x9C00_E938)
    UINT32  RGST_Group0466_Reg15                  ; // 15  (ADDR : 0x9C00_E93C)
    UINT32  RGST_Group0466_Reg16                  ; // 16  (ADDR : 0x9C00_E940)
    UINT32  RGST_Group0466_Reg17                  ; // 17  (ADDR : 0x9C00_E944)
    UINT32  RGST_Group0466_Reg18                  ; // 18  (ADDR : 0x9C00_E948)
    UINT32  RGST_Group0466_Reg19                  ; // 19  (ADDR : 0x9C00_E94C)
    UINT32  RGST_Group0466_Reg20                  ; // 20  (ADDR : 0x9C00_E950)
    UINT32  RGST_Group0466_Reg21                  ; // 21  (ADDR : 0x9C00_E954)
    UINT32  RGST_Group0466_Reg22                  ; // 22  (ADDR : 0x9C00_E958)
    UINT32  RGST_Group0466_Reg23                  ; // 23  (ADDR : 0x9C00_E95C)
    UINT32  RGST_Group0466_Reg24                  ; // 24  (ADDR : 0x9C00_E960)
    UINT32  RGST_Group0466_Reg25                  ; // 25  (ADDR : 0x9C00_E964)
    UINT32  RGST_Group0466_Reg26                  ; // 26  (ADDR : 0x9C00_E968)
    UINT32  RGST_Group0466_Reg27                  ; // 27  (ADDR : 0x9C00_E96C)
    UINT32  RGST_Group0466_Reg28                  ; // 28  (ADDR : 0x9C00_E970)
    UINT32  RGST_Group0466_Reg29                  ; // 29  (ADDR : 0x9C00_E974)
    UINT32  RGST_Group0466_Reg30                  ; // 30  (ADDR : 0x9C00_E978)
    UINT32  RGST_Group0466_Reg31                  ; // 31  (ADDR : 0x9C00_E97C)

    //Group   467 : MFD
    UINT32  RGST_Group0467_Reg00                  ; // 00  (ADDR : 0x9C00_E980)
    UINT32  RGST_Group0467_Reg01                  ; // 01  (ADDR : 0x9C00_E984)
    UINT32  RGST_Group0467_Reg02                  ; // 02  (ADDR : 0x9C00_E988)
    UINT32  RGST_Group0467_Reg03                  ; // 03  (ADDR : 0x9C00_E98C)
    UINT32  RGST_Group0467_Reg04                  ; // 04  (ADDR : 0x9C00_E990)
    UINT32  RGST_Group0467_Reg05                  ; // 05  (ADDR : 0x9C00_E994)
    UINT32  RGST_Group0467_Reg06                  ; // 06  (ADDR : 0x9C00_E998)
    UINT32  RGST_Group0467_Reg07                  ; // 07  (ADDR : 0x9C00_E99C)
    UINT32  RGST_Group0467_Reg08                  ; // 08  (ADDR : 0x9C00_E9A0)
    UINT32  RGST_Group0467_Reg09                  ; // 09  (ADDR : 0x9C00_E9A4)
    UINT32  RGST_Group0467_Reg10                  ; // 10  (ADDR : 0x9C00_E9A8)
    UINT32  RGST_Group0467_Reg11                  ; // 11  (ADDR : 0x9C00_E9AC)
    UINT32  RGST_Group0467_Reg12                  ; // 12  (ADDR : 0x9C00_E9B0)
    UINT32  RGST_Group0467_Reg13                  ; // 13  (ADDR : 0x9C00_E9B4)
    UINT32  RGST_Group0467_Reg14                  ; // 14  (ADDR : 0x9C00_E9B8)
    UINT32  RGST_Group0467_Reg15                  ; // 15  (ADDR : 0x9C00_E9BC)
    UINT32  RGST_Group0467_Reg16                  ; // 16  (ADDR : 0x9C00_E9C0)
    UINT32  RGST_Group0467_Reg17                  ; // 17  (ADDR : 0x9C00_E9C4)
    UINT32  RGST_Group0467_Reg18                  ; // 18  (ADDR : 0x9C00_E9C8)
    UINT32  RGST_Group0467_Reg19                  ; // 19  (ADDR : 0x9C00_E9CC)
    UINT32  RGST_Group0467_Reg20                  ; // 20  (ADDR : 0x9C00_E9D0)
    UINT32  RGST_Group0467_Reg21                  ; // 21  (ADDR : 0x9C00_E9D4)
    UINT32  RGST_Group0467_Reg22                  ; // 22  (ADDR : 0x9C00_E9D8)
    UINT32  RGST_Group0467_Reg23                  ; // 23  (ADDR : 0x9C00_E9DC)
    UINT32  RGST_Group0467_Reg24                  ; // 24  (ADDR : 0x9C00_E9E0)
    UINT32  RGST_Group0467_Reg25                  ; // 25  (ADDR : 0x9C00_E9E4)
    UINT32  RGST_Group0467_Reg26                  ; // 26  (ADDR : 0x9C00_E9E8)
    UINT32  RGST_Group0467_Reg27                  ; // 27  (ADDR : 0x9C00_E9EC)
    UINT32  RGST_Group0467_Reg28                  ; // 28  (ADDR : 0x9C00_E9F0)
    UINT32  RGST_Group0467_Reg29                  ; // 29  (ADDR : 0x9C00_E9F4)
    UINT32  RGST_Group0467_Reg30                  ; // 30  (ADDR : 0x9C00_E9F8)
    UINT32  RGST_Group0467_Reg31                  ; // 31  (ADDR : 0x9C00_E9FC)

    //Group   468 : MFD
    UINT32  RGST_Group0468_Reg00                  ; // 00  (ADDR : 0x9C00_EA00)
    UINT32  RGST_Group0468_Reg01                  ; // 01  (ADDR : 0x9C00_EA04)
    UINT32  RGST_Group0468_Reg02                  ; // 02  (ADDR : 0x9C00_EA08)
    UINT32  RGST_Group0468_Reg03                  ; // 03  (ADDR : 0x9C00_EA0C)
    UINT32  RGST_Group0468_Reg04                  ; // 04  (ADDR : 0x9C00_EA10)
    UINT32  RGST_Group0468_Reg05                  ; // 05  (ADDR : 0x9C00_EA14)
    UINT32  RGST_Group0468_Reg06                  ; // 06  (ADDR : 0x9C00_EA18)
    UINT32  RGST_Group0468_Reg07                  ; // 07  (ADDR : 0x9C00_EA1C)
    UINT32  RGST_Group0468_Reg08                  ; // 08  (ADDR : 0x9C00_EA20)
    UINT32  RGST_Group0468_Reg09                  ; // 09  (ADDR : 0x9C00_EA24)
    UINT32  RGST_Group0468_Reg10                  ; // 10  (ADDR : 0x9C00_EA28)
    UINT32  RGST_Group0468_Reg11                  ; // 11  (ADDR : 0x9C00_EA2C)
    UINT32  RGST_Group0468_Reg12                  ; // 12  (ADDR : 0x9C00_EA30)
    UINT32  RGST_Group0468_Reg13                  ; // 13  (ADDR : 0x9C00_EA34)
    UINT32  RGST_Group0468_Reg14                  ; // 14  (ADDR : 0x9C00_EA38)
    UINT32  RGST_Group0468_Reg15                  ; // 15  (ADDR : 0x9C00_EA3C)
    UINT32  RGST_Group0468_Reg16                  ; // 16  (ADDR : 0x9C00_EA40)
    UINT32  RGST_Group0468_Reg17                  ; // 17  (ADDR : 0x9C00_EA44)
    UINT32  RGST_Group0468_Reg18                  ; // 18  (ADDR : 0x9C00_EA48)
    UINT32  RGST_Group0468_Reg19                  ; // 19  (ADDR : 0x9C00_EA4C)
    UINT32  RGST_Group0468_Reg20                  ; // 20  (ADDR : 0x9C00_EA50)
    UINT32  RGST_Group0468_Reg21                  ; // 21  (ADDR : 0x9C00_EA54)
    UINT32  RGST_Group0468_Reg22                  ; // 22  (ADDR : 0x9C00_EA58)
    UINT32  RGST_Group0468_Reg23                  ; // 23  (ADDR : 0x9C00_EA5C)
    UINT32  RGST_Group0468_Reg24                  ; // 24  (ADDR : 0x9C00_EA60)
    UINT32  RGST_Group0468_Reg25                  ; // 25  (ADDR : 0x9C00_EA64)
    UINT32  RGST_Group0468_Reg26                  ; // 26  (ADDR : 0x9C00_EA68)
    UINT32  RGST_Group0468_Reg27                  ; // 27  (ADDR : 0x9C00_EA6C)
    UINT32  RGST_Group0468_Reg28                  ; // 28  (ADDR : 0x9C00_EA70)
    UINT32  RGST_Group0468_Reg29                  ; // 29  (ADDR : 0x9C00_EA74)
    UINT32  RGST_Group0468_Reg30                  ; // 30  (ADDR : 0x9C00_EA78)
    UINT32  RGST_Group0468_Reg31                  ; // 31  (ADDR : 0x9C00_EA7C)

    //Group   469 : MFD
    UINT32  RGST_Group0469_Reg00                  ; // 00  (ADDR : 0x9C00_EA80)
    UINT32  RGST_Group0469_Reg01                  ; // 01  (ADDR : 0x9C00_EA84)
    UINT32  RGST_Group0469_Reg02                  ; // 02  (ADDR : 0x9C00_EA88)
    UINT32  RGST_Group0469_Reg03                  ; // 03  (ADDR : 0x9C00_EA8C)
    UINT32  RGST_Group0469_Reg04                  ; // 04  (ADDR : 0x9C00_EA90)
    UINT32  RGST_Group0469_Reg05                  ; // 05  (ADDR : 0x9C00_EA94)
    UINT32  RGST_Group0469_Reg06                  ; // 06  (ADDR : 0x9C00_EA98)
    UINT32  RGST_Group0469_Reg07                  ; // 07  (ADDR : 0x9C00_EA9C)
    UINT32  RGST_Group0469_Reg08                  ; // 08  (ADDR : 0x9C00_EAA0)
    UINT32  RGST_Group0469_Reg09                  ; // 09  (ADDR : 0x9C00_EAA4)
    UINT32  RGST_Group0469_Reg10                  ; // 10  (ADDR : 0x9C00_EAA8)
    UINT32  RGST_Group0469_Reg11                  ; // 11  (ADDR : 0x9C00_EAAC)
    UINT32  RGST_Group0469_Reg12                  ; // 12  (ADDR : 0x9C00_EAB0)
    UINT32  RGST_Group0469_Reg13                  ; // 13  (ADDR : 0x9C00_EAB4)
    UINT32  RGST_Group0469_Reg14                  ; // 14  (ADDR : 0x9C00_EAB8)
    UINT32  RGST_Group0469_Reg15                  ; // 15  (ADDR : 0x9C00_EABC)
    UINT32  RGST_Group0469_Reg16                  ; // 16  (ADDR : 0x9C00_EAC0)
    UINT32  RGST_Group0469_Reg17                  ; // 17  (ADDR : 0x9C00_EAC4)
    UINT32  RGST_Group0469_Reg18                  ; // 18  (ADDR : 0x9C00_EAC8)
    UINT32  RGST_Group0469_Reg19                  ; // 19  (ADDR : 0x9C00_EACC)
    UINT32  RGST_Group0469_Reg20                  ; // 20  (ADDR : 0x9C00_EAD0)
    UINT32  RGST_Group0469_Reg21                  ; // 21  (ADDR : 0x9C00_EAD4)
    UINT32  RGST_Group0469_Reg22                  ; // 22  (ADDR : 0x9C00_EAD8)
    UINT32  RGST_Group0469_Reg23                  ; // 23  (ADDR : 0x9C00_EADC)
    UINT32  RGST_Group0469_Reg24                  ; // 24  (ADDR : 0x9C00_EAE0)
    UINT32  RGST_Group0469_Reg25                  ; // 25  (ADDR : 0x9C00_EAE4)
    UINT32  RGST_Group0469_Reg26                  ; // 26  (ADDR : 0x9C00_EAE8)
    UINT32  RGST_Group0469_Reg27                  ; // 27  (ADDR : 0x9C00_EAEC)
    UINT32  RGST_Group0469_Reg28                  ; // 28  (ADDR : 0x9C00_EAF0)
    UINT32  RGST_Group0469_Reg29                  ; // 29  (ADDR : 0x9C00_EAF4)
    UINT32  RGST_Group0469_Reg30                  ; // 30  (ADDR : 0x9C00_EAF8)
    UINT32  RGST_Group0469_Reg31                  ; // 31  (ADDR : 0x9C00_EAFC)

    //Group   470 : MFD
    UINT32  RGST_Group0470_Reg00                  ; // 00  (ADDR : 0x9C00_EB00)
    UINT32  RGST_Group0470_Reg01                  ; // 01  (ADDR : 0x9C00_EB04)
    UINT32  RGST_Group0470_Reg02                  ; // 02  (ADDR : 0x9C00_EB08)
    UINT32  RGST_Group0470_Reg03                  ; // 03  (ADDR : 0x9C00_EB0C)
    UINT32  RGST_Group0470_Reg04                  ; // 04  (ADDR : 0x9C00_EB10)
    UINT32  RGST_Group0470_Reg05                  ; // 05  (ADDR : 0x9C00_EB14)
    UINT32  RGST_Group0470_Reg06                  ; // 06  (ADDR : 0x9C00_EB18)
    UINT32  RGST_Group0470_Reg07                  ; // 07  (ADDR : 0x9C00_EB1C)
    UINT32  RGST_Group0470_Reg08                  ; // 08  (ADDR : 0x9C00_EB20)
    UINT32  RGST_Group0470_Reg09                  ; // 09  (ADDR : 0x9C00_EB24)
    UINT32  RGST_Group0470_Reg10                  ; // 10  (ADDR : 0x9C00_EB28)
    UINT32  RGST_Group0470_Reg11                  ; // 11  (ADDR : 0x9C00_EB2C)
    UINT32  RGST_Group0470_Reg12                  ; // 12  (ADDR : 0x9C00_EB30)
    UINT32  RGST_Group0470_Reg13                  ; // 13  (ADDR : 0x9C00_EB34)
    UINT32  RGST_Group0470_Reg14                  ; // 14  (ADDR : 0x9C00_EB38)
    UINT32  RGST_Group0470_Reg15                  ; // 15  (ADDR : 0x9C00_EB3C)
    UINT32  RGST_Group0470_Reg16                  ; // 16  (ADDR : 0x9C00_EB40)
    UINT32  RGST_Group0470_Reg17                  ; // 17  (ADDR : 0x9C00_EB44)
    UINT32  RGST_Group0470_Reg18                  ; // 18  (ADDR : 0x9C00_EB48)
    UINT32  RGST_Group0470_Reg19                  ; // 19  (ADDR : 0x9C00_EB4C)
    UINT32  RGST_Group0470_Reg20                  ; // 20  (ADDR : 0x9C00_EB50)
    UINT32  RGST_Group0470_Reg21                  ; // 21  (ADDR : 0x9C00_EB54)
    UINT32  RGST_Group0470_Reg22                  ; // 22  (ADDR : 0x9C00_EB58)
    UINT32  RGST_Group0470_Reg23                  ; // 23  (ADDR : 0x9C00_EB5C)
    UINT32  RGST_Group0470_Reg24                  ; // 24  (ADDR : 0x9C00_EB60)
    UINT32  RGST_Group0470_Reg25                  ; // 25  (ADDR : 0x9C00_EB64)
    UINT32  RGST_Group0470_Reg26                  ; // 26  (ADDR : 0x9C00_EB68)
    UINT32  RGST_Group0470_Reg27                  ; // 27  (ADDR : 0x9C00_EB6C)
    UINT32  RGST_Group0470_Reg28                  ; // 28  (ADDR : 0x9C00_EB70)
    UINT32  RGST_Group0470_Reg29                  ; // 29  (ADDR : 0x9C00_EB74)
    UINT32  RGST_Group0470_Reg30                  ; // 30  (ADDR : 0x9C00_EB78)
    UINT32  RGST_Group0470_Reg31                  ; // 31  (ADDR : 0x9C00_EB7C)

    //Group   471 : MFD
    UINT32  RGST_Group0471_Reg00                  ; // 00  (ADDR : 0x9C00_EB80)
    UINT32  RGST_Group0471_Reg01                  ; // 01  (ADDR : 0x9C00_EB84)
    UINT32  RGST_Group0471_Reg02                  ; // 02  (ADDR : 0x9C00_EB88)
    UINT32  RGST_Group0471_Reg03                  ; // 03  (ADDR : 0x9C00_EB8C)
    UINT32  RGST_Group0471_Reg04                  ; // 04  (ADDR : 0x9C00_EB90)
    UINT32  RGST_Group0471_Reg05                  ; // 05  (ADDR : 0x9C00_EB94)
    UINT32  RGST_Group0471_Reg06                  ; // 06  (ADDR : 0x9C00_EB98)
    UINT32  RGST_Group0471_Reg07                  ; // 07  (ADDR : 0x9C00_EB9C)
    UINT32  RGST_Group0471_Reg08                  ; // 08  (ADDR : 0x9C00_EBA0)
    UINT32  RGST_Group0471_Reg09                  ; // 09  (ADDR : 0x9C00_EBA4)
    UINT32  RGST_Group0471_Reg10                  ; // 10  (ADDR : 0x9C00_EBA8)
    UINT32  RGST_Group0471_Reg11                  ; // 11  (ADDR : 0x9C00_EBAC)
    UINT32  RGST_Group0471_Reg12                  ; // 12  (ADDR : 0x9C00_EBB0)
    UINT32  RGST_Group0471_Reg13                  ; // 13  (ADDR : 0x9C00_EBB4)
    UINT32  RGST_Group0471_Reg14                  ; // 14  (ADDR : 0x9C00_EBB8)
    UINT32  RGST_Group0471_Reg15                  ; // 15  (ADDR : 0x9C00_EBBC)
    UINT32  RGST_Group0471_Reg16                  ; // 16  (ADDR : 0x9C00_EBC0)
    UINT32  RGST_Group0471_Reg17                  ; // 17  (ADDR : 0x9C00_EBC4)
    UINT32  RGST_Group0471_Reg18                  ; // 18  (ADDR : 0x9C00_EBC8)
    UINT32  RGST_Group0471_Reg19                  ; // 19  (ADDR : 0x9C00_EBCC)
    UINT32  RGST_Group0471_Reg20                  ; // 20  (ADDR : 0x9C00_EBD0)
    UINT32  RGST_Group0471_Reg21                  ; // 21  (ADDR : 0x9C00_EBD4)
    UINT32  RGST_Group0471_Reg22                  ; // 22  (ADDR : 0x9C00_EBD8)
    UINT32  RGST_Group0471_Reg23                  ; // 23  (ADDR : 0x9C00_EBDC)
    UINT32  RGST_Group0471_Reg24                  ; // 24  (ADDR : 0x9C00_EBE0)
    UINT32  RGST_Group0471_Reg25                  ; // 25  (ADDR : 0x9C00_EBE4)
    UINT32  RGST_Group0471_Reg26                  ; // 26  (ADDR : 0x9C00_EBE8)
    UINT32  RGST_Group0471_Reg27                  ; // 27  (ADDR : 0x9C00_EBEC)
    UINT32  RGST_Group0471_Reg28                  ; // 28  (ADDR : 0x9C00_EBF0)
    UINT32  RGST_Group0471_Reg29                  ; // 29  (ADDR : 0x9C00_EBF4)
    UINT32  RGST_Group0471_Reg30                  ; // 30  (ADDR : 0x9C00_EBF8)
    UINT32  RGST_Group0471_Reg31                  ; // 31  (ADDR : 0x9C00_EBFC)

    //Group   472 : MFD (MC_CHECKSUM, original G5)
    UINT32  mfd_version                           ; // 00  (ADDR : 0x9C00_EC00)
    UINT32  gmv_chksum                            ; // 01  (ADDR : 0x9C00_EC04)
    UINT32  lmv_chksum                            ; // 02  (ADDR : 0x9C00_EC08)
    UINT32  cmv_chksum                            ; // 03  (ADDR : 0x9C00_EC0C)
    UINT32  mp_qt_chksum                          ; // 04  (ADDR : 0x9C00_EC10)
    UINT32  mp_mr_chksum                          ; // 05  (ADDR : 0x9C00_EC14)
    UINT32  mp_lf_chksum                          ; // 06  (ADDR : 0x9C00_EC18)
    UINT32  qt_mr_chksum                          ; // 07  (ADDR : 0x9C00_EC1C)
    UINT32  mr_lf_chksum                          ; // 08  (ADDR : 0x9C00_EC20)
    UINT32  mr_mc_chksum                          ; // 09  (ADDR : 0x9C00_EC24)
    UINT32  lf_mc_chksum                          ; // 10  (ADDR : 0x9C00_EC28)
    UINT32  mon_cur_urgent_count                  ; // 11  (ADDR : 0x9C00_EC2C)
    UINT32  stop_urgent_count                     ; // 12  (ADDR : 0x9C00_EC30)
    UINT32  mb_desire_cycle                       ; // 13  (ADDR : 0x9C00_EC34)
    UINT32  mon_total_cycle                       ; // 14  (ADDR : 0x9C00_EC38)
    UINT32  xmb_desire_cycle                      ; // 15  (ADDR : 0x9C00_EC3C)
    UINT32  measure_mb_count                      ; // 16  (ADDR : 0x9C00_EC40)
    UINT32  mfd_sbar_ctrl                         ; // 17  (ADDR : 0x9C00_EC44)
    UINT32  cycel_cnt                             ; // 18  (ADDR : 0x9C00_EC48)
    UINT32  debug_ctrl0                           ; // 19  (ADDR : 0x9C00_EC4C)
    UINT32  debug_ctrl1                           ; // 20  (ADDR : 0x9C00_EC50)
    UINT32  mon_misc_1                            ; // 21  (ADDR : 0x9C00_EC54)
    UINT32  mon_misc_2                            ; // 22  (ADDR : 0x9C00_EC58)
    UINT32  mon_misc_3                            ; // 23  (ADDR : 0x9C00_EC5C)
    UINT32  mon_misc_4                            ; // 24  (ADDR : 0x9C00_EC60)
    UINT32  mon_misc_5                            ; // 25  (ADDR : 0x9C00_EC64)
    UINT32  lfbwr_mc_chksum                       ; // 26  (ADDR : 0x9C00_EC68)
    UINT32  g472_reserved[5]                      ; // 27  (ADDR : 0x9C00_EC6C) ~ 31  (ADDR : 0x9C00_EC7C)

    //Group   473 : MFD (PIC_PARM, original G3)
    UINT32  seq_ext                               ; // 00  (ADDR : 0x9C00_EC80)
    UINT32  h_size                                ; // 01  (ADDR : 0x9C00_EC84)
    UINT32  v_size                                ; // 02  (ADDR : 0x9C00_EC88)
    UINT32  g3_unused                             ; // 03  (ADDR : 0x9C00_EC8C)
    UINT32  pic_coding_type                       ; // 04  (ADDR : 0x9C00_EC90)
    UINT32  pic_f_code                            ; // 05  (ADDR : 0x9C00_EC94)
    UINT32  pic_coding_ext0                       ; // 06  (ADDR : 0x9C00_EC98)
    UINT32  pic_coding_ext1                       ; // 07  (ADDR : 0x9C00_EC9C)
    UINT32  pic_start                             ; // 08  (ADDR : 0x9C00_ECA0)
    UINT32  dis_pic_id                            ; // 09  (ADDR : 0x9C00_ECA4)
    UINT32  dis_seq_tag                           ; // 10  (ADDR : 0x9C00_ECA8)
    UINT32  g473_reserved[21]                     ; // 11  (ADDR : 0x9C00_ECAC) ~ 31  (ADDR : 0x9C00_ECFC)

    //Group   474 : MFD
    UINT32  RGST_Group0474_Reg00                  ; // 00  (ADDR : 0x9C00_ED00)
    UINT32  RGST_Group0474_Reg01                  ; // 01  (ADDR : 0x9C00_ED04)
    UINT32  RGST_Group0474_Reg02                  ; // 02  (ADDR : 0x9C00_ED08)
    UINT32  RGST_Group0474_Reg03                  ; // 03  (ADDR : 0x9C00_ED0C)
    UINT32  RGST_Group0474_Reg04                  ; // 04  (ADDR : 0x9C00_ED10)
    UINT32  RGST_Group0474_Reg05                  ; // 05  (ADDR : 0x9C00_ED14)
    UINT32  RGST_Group0474_Reg06                  ; // 06  (ADDR : 0x9C00_ED18)
    UINT32  RGST_Group0474_Reg07                  ; // 07  (ADDR : 0x9C00_ED1C)
    UINT32  RGST_Group0474_Reg08                  ; // 08  (ADDR : 0x9C00_ED20)
    UINT32  RGST_Group0474_Reg09                  ; // 09  (ADDR : 0x9C00_ED24)
    UINT32  RGST_Group0474_Reg10                  ; // 10  (ADDR : 0x9C00_ED28)
    UINT32  RGST_Group0474_Reg11                  ; // 11  (ADDR : 0x9C00_ED2C)
    UINT32  RGST_Group0474_Reg12                  ; // 12  (ADDR : 0x9C00_ED30)
    UINT32  RGST_Group0474_Reg13                  ; // 13  (ADDR : 0x9C00_ED34)
    UINT32  RGST_Group0474_Reg14                  ; // 14  (ADDR : 0x9C00_ED38)
    UINT32  RGST_Group0474_Reg15                  ; // 15  (ADDR : 0x9C00_ED3C)
    UINT32  RGST_Group0474_Reg16                  ; // 16  (ADDR : 0x9C00_ED40)
    UINT32  RGST_Group0474_Reg17                  ; // 17  (ADDR : 0x9C00_ED44)
    UINT32  RGST_Group0474_Reg18                  ; // 18  (ADDR : 0x9C00_ED48)
    UINT32  RGST_Group0474_Reg19                  ; // 19  (ADDR : 0x9C00_ED4C)
    UINT32  RGST_Group0474_Reg20                  ; // 20  (ADDR : 0x9C00_ED50)
    UINT32  RGST_Group0474_Reg21                  ; // 21  (ADDR : 0x9C00_ED54)
    UINT32  RGST_Group0474_Reg22                  ; // 22  (ADDR : 0x9C00_ED58)
    UINT32  RGST_Group0474_Reg23                  ; // 23  (ADDR : 0x9C00_ED5C)
    UINT32  RGST_Group0474_Reg24                  ; // 24  (ADDR : 0x9C00_ED60)
    UINT32  RGST_Group0474_Reg25                  ; // 25  (ADDR : 0x9C00_ED64)
    UINT32  RGST_Group0474_Reg26                  ; // 26  (ADDR : 0x9C00_ED68)
    UINT32  RGST_Group0474_Reg27                  ; // 27  (ADDR : 0x9C00_ED6C)
    UINT32  RGST_Group0474_Reg28                  ; // 28  (ADDR : 0x9C00_ED70)
    UINT32  RGST_Group0474_Reg29                  ; // 29  (ADDR : 0x9C00_ED74)
    UINT32  RGST_Group0474_Reg30                  ; // 30  (ADDR : 0x9C00_ED78)
    UINT32  RGST_Group0474_Reg31                  ; // 31  (ADDR : 0x9C00_ED7C)

    //Group   475 : MFD
    UINT32  RGST_Group0475_Reg00                  ; // 00  (ADDR : 0x9C00_ED80)
    UINT32  RGST_Group0475_Reg01                  ; // 01  (ADDR : 0x9C00_ED84)
    UINT32  RGST_Group0475_Reg02                  ; // 02  (ADDR : 0x9C00_ED88)
    UINT32  RGST_Group0475_Reg03                  ; // 03  (ADDR : 0x9C00_ED8C)
    UINT32  RGST_Group0475_Reg04                  ; // 04  (ADDR : 0x9C00_ED90)
    UINT32  RGST_Group0475_Reg05                  ; // 05  (ADDR : 0x9C00_ED94)
    UINT32  RGST_Group0475_Reg06                  ; // 06  (ADDR : 0x9C00_ED98)
    UINT32  RGST_Group0475_Reg07                  ; // 07  (ADDR : 0x9C00_ED9C)
    UINT32  RGST_Group0475_Reg08                  ; // 08  (ADDR : 0x9C00_EDA0)
    UINT32  RGST_Group0475_Reg09                  ; // 09  (ADDR : 0x9C00_EDA4)
    UINT32  RGST_Group0475_Reg10                  ; // 10  (ADDR : 0x9C00_EDA8)
    UINT32  RGST_Group0475_Reg11                  ; // 11  (ADDR : 0x9C00_EDAC)
    UINT32  RGST_Group0475_Reg12                  ; // 12  (ADDR : 0x9C00_EDB0)
    UINT32  RGST_Group0475_Reg13                  ; // 13  (ADDR : 0x9C00_EDB4)
    UINT32  RGST_Group0475_Reg14                  ; // 14  (ADDR : 0x9C00_EDB8)
    UINT32  RGST_Group0475_Reg15                  ; // 15  (ADDR : 0x9C00_EDBC)
    UINT32  RGST_Group0475_Reg16                  ; // 16  (ADDR : 0x9C00_EDC0)
    UINT32  RGST_Group0475_Reg17                  ; // 17  (ADDR : 0x9C00_EDC4)
    UINT32  RGST_Group0475_Reg18                  ; // 18  (ADDR : 0x9C00_EDC8)
    UINT32  RGST_Group0475_Reg19                  ; // 19  (ADDR : 0x9C00_EDCC)
    UINT32  RGST_Group0475_Reg20                  ; // 20  (ADDR : 0x9C00_EDD0)
    UINT32  RGST_Group0475_Reg21                  ; // 21  (ADDR : 0x9C00_EDD4)
    UINT32  RGST_Group0475_Reg22                  ; // 22  (ADDR : 0x9C00_EDD8)
    UINT32  RGST_Group0475_Reg23                  ; // 23  (ADDR : 0x9C00_EDDC)
    UINT32  RGST_Group0475_Reg24                  ; // 24  (ADDR : 0x9C00_EDE0)
    UINT32  RGST_Group0475_Reg25                  ; // 25  (ADDR : 0x9C00_EDE4)
    UINT32  RGST_Group0475_Reg26                  ; // 26  (ADDR : 0x9C00_EDE8)
    UINT32  RGST_Group0475_Reg27                  ; // 27  (ADDR : 0x9C00_EDEC)
    UINT32  RGST_Group0475_Reg28                  ; // 28  (ADDR : 0x9C00_EDF0)
    UINT32  RGST_Group0475_Reg29                  ; // 29  (ADDR : 0x9C00_EDF4)
    UINT32  RGST_Group0475_Reg30                  ; // 30  (ADDR : 0x9C00_EDF8)
    UINT32  RGST_Group0475_Reg31                  ; // 31  (ADDR : 0x9C00_EDFC)

    //Group   476 : MFD
    UINT32  RGST_Group0476_Reg00                  ; // 00  (ADDR : 0x9C00_EE00)
    UINT32  RGST_Group0476_Reg01                  ; // 01  (ADDR : 0x9C00_EE04)
    UINT32  RGST_Group0476_Reg02                  ; // 02  (ADDR : 0x9C00_EE08)
    UINT32  RGST_Group0476_Reg03                  ; // 03  (ADDR : 0x9C00_EE0C)
    UINT32  RGST_Group0476_Reg04                  ; // 04  (ADDR : 0x9C00_EE10)
    UINT32  RGST_Group0476_Reg05                  ; // 05  (ADDR : 0x9C00_EE14)
    UINT32  RGST_Group0476_Reg06                  ; // 06  (ADDR : 0x9C00_EE18)
    UINT32  RGST_Group0476_Reg07                  ; // 07  (ADDR : 0x9C00_EE1C)
    UINT32  RGST_Group0476_Reg08                  ; // 08  (ADDR : 0x9C00_EE20)
    UINT32  RGST_Group0476_Reg09                  ; // 09  (ADDR : 0x9C00_EE24)
    UINT32  RGST_Group0476_Reg10                  ; // 10  (ADDR : 0x9C00_EE28)
    UINT32  RGST_Group0476_Reg11                  ; // 11  (ADDR : 0x9C00_EE2C)
    UINT32  RGST_Group0476_Reg12                  ; // 12  (ADDR : 0x9C00_EE30)
    UINT32  RGST_Group0476_Reg13                  ; // 13  (ADDR : 0x9C00_EE34)
    UINT32  RGST_Group0476_Reg14                  ; // 14  (ADDR : 0x9C00_EE38)
    UINT32  RGST_Group0476_Reg15                  ; // 15  (ADDR : 0x9C00_EE3C)
    UINT32  RGST_Group0476_Reg16                  ; // 16  (ADDR : 0x9C00_EE40)
    UINT32  RGST_Group0476_Reg17                  ; // 17  (ADDR : 0x9C00_EE44)
    UINT32  RGST_Group0476_Reg18                  ; // 18  (ADDR : 0x9C00_EE48)
    UINT32  RGST_Group0476_Reg19                  ; // 19  (ADDR : 0x9C00_EE4C)
    UINT32  RGST_Group0476_Reg20                  ; // 20  (ADDR : 0x9C00_EE50)
    UINT32  RGST_Group0476_Reg21                  ; // 21  (ADDR : 0x9C00_EE54)
    UINT32  RGST_Group0476_Reg22                  ; // 22  (ADDR : 0x9C00_EE58)
    UINT32  RGST_Group0476_Reg23                  ; // 23  (ADDR : 0x9C00_EE5C)
    UINT32  RGST_Group0476_Reg24                  ; // 24  (ADDR : 0x9C00_EE60)
    UINT32  RGST_Group0476_Reg25                  ; // 25  (ADDR : 0x9C00_EE64)
    UINT32  RGST_Group0476_Reg26                  ; // 26  (ADDR : 0x9C00_EE68)
    UINT32  RGST_Group0476_Reg27                  ; // 27  (ADDR : 0x9C00_EE6C)
    UINT32  RGST_Group0476_Reg28                  ; // 28  (ADDR : 0x9C00_EE70)
    UINT32  RGST_Group0476_Reg29                  ; // 29  (ADDR : 0x9C00_EE74)
    UINT32  RGST_Group0476_Reg30                  ; // 30  (ADDR : 0x9C00_EE78)
    UINT32  RGST_Group0476_Reg31                  ; // 31  (ADDR : 0x9C00_EE7C)

    //Group   477 : MFD
    UINT32  RGST_Group0477_Reg00                  ; // 00  (ADDR : 0x9C00_EE80)
    UINT32  RGST_Group0477_Reg01                  ; // 01  (ADDR : 0x9C00_EE84)
    UINT32  RGST_Group0477_Reg02                  ; // 02  (ADDR : 0x9C00_EE88)
    UINT32  RGST_Group0477_Reg03                  ; // 03  (ADDR : 0x9C00_EE8C)
    UINT32  RGST_Group0477_Reg04                  ; // 04  (ADDR : 0x9C00_EE90)
    UINT32  RGST_Group0477_Reg05                  ; // 05  (ADDR : 0x9C00_EE94)
    UINT32  RGST_Group0477_Reg06                  ; // 06  (ADDR : 0x9C00_EE98)
    UINT32  RGST_Group0477_Reg07                  ; // 07  (ADDR : 0x9C00_EE9C)
    UINT32  RGST_Group0477_Reg08                  ; // 08  (ADDR : 0x9C00_EEA0)
    UINT32  RGST_Group0477_Reg09                  ; // 09  (ADDR : 0x9C00_EEA4)
    UINT32  RGST_Group0477_Reg10                  ; // 10  (ADDR : 0x9C00_EEA8)
    UINT32  RGST_Group0477_Reg11                  ; // 11  (ADDR : 0x9C00_EEAC)
    UINT32  RGST_Group0477_Reg12                  ; // 12  (ADDR : 0x9C00_EEB0)
    UINT32  RGST_Group0477_Reg13                  ; // 13  (ADDR : 0x9C00_EEB4)
    UINT32  RGST_Group0477_Reg14                  ; // 14  (ADDR : 0x9C00_EEB8)
    UINT32  RGST_Group0477_Reg15                  ; // 15  (ADDR : 0x9C00_EEBC)
    UINT32  RGST_Group0477_Reg16                  ; // 16  (ADDR : 0x9C00_EEC0)
    UINT32  RGST_Group0477_Reg17                  ; // 17  (ADDR : 0x9C00_EEC4)
    UINT32  RGST_Group0477_Reg18                  ; // 18  (ADDR : 0x9C00_EEC8)
    UINT32  RGST_Group0477_Reg19                  ; // 19  (ADDR : 0x9C00_EECC)
    UINT32  RGST_Group0477_Reg20                  ; // 20  (ADDR : 0x9C00_EED0)
    UINT32  RGST_Group0477_Reg21                  ; // 21  (ADDR : 0x9C00_EED4)
    UINT32  RGST_Group0477_Reg22                  ; // 22  (ADDR : 0x9C00_EED8)
    UINT32  RGST_Group0477_Reg23                  ; // 23  (ADDR : 0x9C00_EEDC)
    UINT32  RGST_Group0477_Reg24                  ; // 24  (ADDR : 0x9C00_EEE0)
    UINT32  RGST_Group0477_Reg25                  ; // 25  (ADDR : 0x9C00_EEE4)
    UINT32  RGST_Group0477_Reg26                  ; // 26  (ADDR : 0x9C00_EEE8)
    UINT32  RGST_Group0477_Reg27                  ; // 27  (ADDR : 0x9C00_EEEC)
    UINT32  RGST_Group0477_Reg28                  ; // 28  (ADDR : 0x9C00_EEF0)
    UINT32  RGST_Group0477_Reg29                  ; // 29  (ADDR : 0x9C00_EEF4)
    UINT32  RGST_Group0477_Reg30                  ; // 30  (ADDR : 0x9C00_EEF8)
    UINT32  RGST_Group0477_Reg31                  ; // 31  (ADDR : 0x9C00_EEFC)

    //Group   478 : MFD
    UINT32  RGST_Group0478_Reg00                  ; // 00  (ADDR : 0x9C00_EF00)
    UINT32  RGST_Group0478_Reg01                  ; // 01  (ADDR : 0x9C00_EF04)
    UINT32  RGST_Group0478_Reg02                  ; // 02  (ADDR : 0x9C00_EF08)
    UINT32  RGST_Group0478_Reg03                  ; // 03  (ADDR : 0x9C00_EF0C)
    UINT32  RGST_Group0478_Reg04                  ; // 04  (ADDR : 0x9C00_EF10)
    UINT32  RGST_Group0478_Reg05                  ; // 05  (ADDR : 0x9C00_EF14)
    UINT32  RGST_Group0478_Reg06                  ; // 06  (ADDR : 0x9C00_EF18)
    UINT32  RGST_Group0478_Reg07                  ; // 07  (ADDR : 0x9C00_EF1C)
    UINT32  RGST_Group0478_Reg08                  ; // 08  (ADDR : 0x9C00_EF20)
    UINT32  RGST_Group0478_Reg09                  ; // 09  (ADDR : 0x9C00_EF24)
    UINT32  RGST_Group0478_Reg10                  ; // 10  (ADDR : 0x9C00_EF28)
    UINT32  RGST_Group0478_Reg11                  ; // 11  (ADDR : 0x9C00_EF2C)
    UINT32  RGST_Group0478_Reg12                  ; // 12  (ADDR : 0x9C00_EF30)
    UINT32  RGST_Group0478_Reg13                  ; // 13  (ADDR : 0x9C00_EF34)
    UINT32  RGST_Group0478_Reg14                  ; // 14  (ADDR : 0x9C00_EF38)
    UINT32  RGST_Group0478_Reg15                  ; // 15  (ADDR : 0x9C00_EF3C)
    UINT32  RGST_Group0478_Reg16                  ; // 16  (ADDR : 0x9C00_EF40)
    UINT32  RGST_Group0478_Reg17                  ; // 17  (ADDR : 0x9C00_EF44)
    UINT32  RGST_Group0478_Reg18                  ; // 18  (ADDR : 0x9C00_EF48)
    UINT32  RGST_Group0478_Reg19                  ; // 19  (ADDR : 0x9C00_EF4C)
    UINT32  RGST_Group0478_Reg20                  ; // 20  (ADDR : 0x9C00_EF50)
    UINT32  RGST_Group0478_Reg21                  ; // 21  (ADDR : 0x9C00_EF54)
    UINT32  RGST_Group0478_Reg22                  ; // 22  (ADDR : 0x9C00_EF58)
    UINT32  RGST_Group0478_Reg23                  ; // 23  (ADDR : 0x9C00_EF5C)
    UINT32  RGST_Group0478_Reg24                  ; // 24  (ADDR : 0x9C00_EF60)
    UINT32  RGST_Group0478_Reg25                  ; // 25  (ADDR : 0x9C00_EF64)
    UINT32  RGST_Group0478_Reg26                  ; // 26  (ADDR : 0x9C00_EF68)
    UINT32  RGST_Group0478_Reg27                  ; // 27  (ADDR : 0x9C00_EF6C)
    UINT32  RGST_Group0478_Reg28                  ; // 28  (ADDR : 0x9C00_EF70)
    UINT32  RGST_Group0478_Reg29                  ; // 29  (ADDR : 0x9C00_EF74)
    UINT32  RGST_Group0478_Reg30                  ; // 30  (ADDR : 0x9C00_EF78)
    UINT32  RGST_Group0478_Reg31                  ; // 31  (ADDR : 0x9C00_EF7C)

    //Group   479 : MFD
    UINT32  RGST_Group0479_Reg00                  ; // 00  (ADDR : 0x9C00_EF80)
    UINT32  RGST_Group0479_Reg01                  ; // 01  (ADDR : 0x9C00_EF84)
    UINT32  RGST_Group0479_Reg02                  ; // 02  (ADDR : 0x9C00_EF88)
    UINT32  RGST_Group0479_Reg03                  ; // 03  (ADDR : 0x9C00_EF8C)
    UINT32  RGST_Group0479_Reg04                  ; // 04  (ADDR : 0x9C00_EF90)
    UINT32  RGST_Group0479_Reg05                  ; // 05  (ADDR : 0x9C00_EF94)
    UINT32  RGST_Group0479_Reg06                  ; // 06  (ADDR : 0x9C00_EF98)
    UINT32  RGST_Group0479_Reg07                  ; // 07  (ADDR : 0x9C00_EF9C)
    UINT32  RGST_Group0479_Reg08                  ; // 08  (ADDR : 0x9C00_EFA0)
    UINT32  RGST_Group0479_Reg09                  ; // 09  (ADDR : 0x9C00_EFA4)
    UINT32  RGST_Group0479_Reg10                  ; // 10  (ADDR : 0x9C00_EFA8)
    UINT32  RGST_Group0479_Reg11                  ; // 11  (ADDR : 0x9C00_EFAC)
    UINT32  RGST_Group0479_Reg12                  ; // 12  (ADDR : 0x9C00_EFB0)
    UINT32  RGST_Group0479_Reg13                  ; // 13  (ADDR : 0x9C00_EFB4)
    UINT32  RGST_Group0479_Reg14                  ; // 14  (ADDR : 0x9C00_EFB8)
    UINT32  RGST_Group0479_Reg15                  ; // 15  (ADDR : 0x9C00_EFBC)
    UINT32  RGST_Group0479_Reg16                  ; // 16  (ADDR : 0x9C00_EFC0)
    UINT32  RGST_Group0479_Reg17                  ; // 17  (ADDR : 0x9C00_EFC4)
    UINT32  RGST_Group0479_Reg18                  ; // 18  (ADDR : 0x9C00_EFC8)
    UINT32  RGST_Group0479_Reg19                  ; // 19  (ADDR : 0x9C00_EFCC)
    UINT32  RGST_Group0479_Reg20                  ; // 20  (ADDR : 0x9C00_EFD0)
    UINT32  RGST_Group0479_Reg21                  ; // 21  (ADDR : 0x9C00_EFD4)
    UINT32  RGST_Group0479_Reg22                  ; // 22  (ADDR : 0x9C00_EFD8)
    UINT32  RGST_Group0479_Reg23                  ; // 23  (ADDR : 0x9C00_EFDC)
    UINT32  RGST_Group0479_Reg24                  ; // 24  (ADDR : 0x9C00_EFE0)
    UINT32  RGST_Group0479_Reg25                  ; // 25  (ADDR : 0x9C00_EFE4)
    UINT32  RGST_Group0479_Reg26                  ; // 26  (ADDR : 0x9C00_EFE8)
    UINT32  RGST_Group0479_Reg27                  ; // 27  (ADDR : 0x9C00_EFEC)
    UINT32  RGST_Group0479_Reg28                  ; // 28  (ADDR : 0x9C00_EFF0)
    UINT32  RGST_Group0479_Reg29                  ; // 29  (ADDR : 0x9C00_EFF4)
    UINT32  RGST_Group0479_Reg30                  ; // 30  (ADDR : 0x9C00_EFF8)
    UINT32  RGST_Group0479_Reg31                  ; // 31  (ADDR : 0x9C00_EFFC)

    //Group   480 : MFD
    UINT32  RGST_Group0480_Reg00                  ; // 00  (ADDR : 0x9C00_F000)
    UINT32  RGST_Group0480_Reg01                  ; // 01  (ADDR : 0x9C00_F004)
    UINT32  RGST_Group0480_Reg02                  ; // 02  (ADDR : 0x9C00_F008)
    UINT32  RGST_Group0480_Reg03                  ; // 03  (ADDR : 0x9C00_F00C)
    UINT32  RGST_Group0480_Reg04                  ; // 04  (ADDR : 0x9C00_F010)
    UINT32  RGST_Group0480_Reg05                  ; // 05  (ADDR : 0x9C00_F014)
    UINT32  RGST_Group0480_Reg06                  ; // 06  (ADDR : 0x9C00_F018)
    UINT32  RGST_Group0480_Reg07                  ; // 07  (ADDR : 0x9C00_F01C)
    UINT32  RGST_Group0480_Reg08                  ; // 08  (ADDR : 0x9C00_F020)
    UINT32  RGST_Group0480_Reg09                  ; // 09  (ADDR : 0x9C00_F024)
    UINT32  RGST_Group0480_Reg10                  ; // 10  (ADDR : 0x9C00_F028)
    UINT32  RGST_Group0480_Reg11                  ; // 11  (ADDR : 0x9C00_F02C)
    UINT32  RGST_Group0480_Reg12                  ; // 12  (ADDR : 0x9C00_F030)
    UINT32  RGST_Group0480_Reg13                  ; // 13  (ADDR : 0x9C00_F034)
    UINT32  RGST_Group0480_Reg14                  ; // 14  (ADDR : 0x9C00_F038)
    UINT32  RGST_Group0480_Reg15                  ; // 15  (ADDR : 0x9C00_F03C)
    UINT32  RGST_Group0480_Reg16                  ; // 16  (ADDR : 0x9C00_F040)
    UINT32  RGST_Group0480_Reg17                  ; // 17  (ADDR : 0x9C00_F044)
    UINT32  RGST_Group0480_Reg18                  ; // 18  (ADDR : 0x9C00_F048)
    UINT32  RGST_Group0480_Reg19                  ; // 19  (ADDR : 0x9C00_F04C)
    UINT32  RGST_Group0480_Reg20                  ; // 20  (ADDR : 0x9C00_F050)
    UINT32  RGST_Group0480_Reg21                  ; // 21  (ADDR : 0x9C00_F054)
    UINT32  RGST_Group0480_Reg22                  ; // 22  (ADDR : 0x9C00_F058)
    UINT32  RGST_Group0480_Reg23                  ; // 23  (ADDR : 0x9C00_F05C)
    UINT32  RGST_Group0480_Reg24                  ; // 24  (ADDR : 0x9C00_F060)
    UINT32  RGST_Group0480_Reg25                  ; // 25  (ADDR : 0x9C00_F064)
    UINT32  RGST_Group0480_Reg26                  ; // 26  (ADDR : 0x9C00_F068)
    UINT32  RGST_Group0480_Reg27                  ; // 27  (ADDR : 0x9C00_F06C)
    UINT32  RGST_Group0480_Reg28                  ; // 28  (ADDR : 0x9C00_F070)
    UINT32  RGST_Group0480_Reg29                  ; // 29  (ADDR : 0x9C00_F074)
    UINT32  RGST_Group0480_Reg30                  ; // 30  (ADDR : 0x9C00_F078)
    UINT32  RGST_Group0480_Reg31                  ; // 31  (ADDR : 0x9C00_F07C)

    //Group   481 : MFD
    UINT32  RGST_Group0481_Reg00                  ; // 00  (ADDR : 0x9C00_F080)
    UINT32  RGST_Group0481_Reg01                  ; // 01  (ADDR : 0x9C00_F084)
    UINT32  RGST_Group0481_Reg02                  ; // 02  (ADDR : 0x9C00_F088)
    UINT32  RGST_Group0481_Reg03                  ; // 03  (ADDR : 0x9C00_F08C)
    UINT32  RGST_Group0481_Reg04                  ; // 04  (ADDR : 0x9C00_F090)
    UINT32  RGST_Group0481_Reg05                  ; // 05  (ADDR : 0x9C00_F094)
    UINT32  RGST_Group0481_Reg06                  ; // 06  (ADDR : 0x9C00_F098)
    UINT32  RGST_Group0481_Reg07                  ; // 07  (ADDR : 0x9C00_F09C)
    UINT32  RGST_Group0481_Reg08                  ; // 08  (ADDR : 0x9C00_F0A0)
    UINT32  RGST_Group0481_Reg09                  ; // 09  (ADDR : 0x9C00_F0A4)
    UINT32  RGST_Group0481_Reg10                  ; // 10  (ADDR : 0x9C00_F0A8)
    UINT32  RGST_Group0481_Reg11                  ; // 11  (ADDR : 0x9C00_F0AC)
    UINT32  RGST_Group0481_Reg12                  ; // 12  (ADDR : 0x9C00_F0B0)
    UINT32  RGST_Group0481_Reg13                  ; // 13  (ADDR : 0x9C00_F0B4)
    UINT32  RGST_Group0481_Reg14                  ; // 14  (ADDR : 0x9C00_F0B8)
    UINT32  RGST_Group0481_Reg15                  ; // 15  (ADDR : 0x9C00_F0BC)
    UINT32  RGST_Group0481_Reg16                  ; // 16  (ADDR : 0x9C00_F0C0)
    UINT32  RGST_Group0481_Reg17                  ; // 17  (ADDR : 0x9C00_F0C4)
    UINT32  RGST_Group0481_Reg18                  ; // 18  (ADDR : 0x9C00_F0C8)
    UINT32  RGST_Group0481_Reg19                  ; // 19  (ADDR : 0x9C00_F0CC)
    UINT32  RGST_Group0481_Reg20                  ; // 20  (ADDR : 0x9C00_F0D0)
    UINT32  RGST_Group0481_Reg21                  ; // 21  (ADDR : 0x9C00_F0D4)
    UINT32  RGST_Group0481_Reg22                  ; // 22  (ADDR : 0x9C00_F0D8)
    UINT32  RGST_Group0481_Reg23                  ; // 23  (ADDR : 0x9C00_F0DC)
    UINT32  RGST_Group0481_Reg24                  ; // 24  (ADDR : 0x9C00_F0E0)
    UINT32  RGST_Group0481_Reg25                  ; // 25  (ADDR : 0x9C00_F0E4)
    UINT32  RGST_Group0481_Reg26                  ; // 26  (ADDR : 0x9C00_F0E8)
    UINT32  RGST_Group0481_Reg27                  ; // 27  (ADDR : 0x9C00_F0EC)
    UINT32  RGST_Group0481_Reg28                  ; // 28  (ADDR : 0x9C00_F0F0)
    UINT32  RGST_Group0481_Reg29                  ; // 29  (ADDR : 0x9C00_F0F4)
    UINT32  RGST_Group0481_Reg30                  ; // 30  (ADDR : 0x9C00_F0F8)
    UINT32  RGST_Group0481_Reg31                  ; // 31  (ADDR : 0x9C00_F0FC)

    //Group   482 : MFD
    UINT32  RGST_Group0482_Reg00                  ; // 00  (ADDR : 0x9C00_F100)
    UINT32  RGST_Group0482_Reg01                  ; // 01  (ADDR : 0x9C00_F104)
    UINT32  RGST_Group0482_Reg02                  ; // 02  (ADDR : 0x9C00_F108)
    UINT32  RGST_Group0482_Reg03                  ; // 03  (ADDR : 0x9C00_F10C)
    UINT32  RGST_Group0482_Reg04                  ; // 04  (ADDR : 0x9C00_F110)
    UINT32  RGST_Group0482_Reg05                  ; // 05  (ADDR : 0x9C00_F114)
    UINT32  RGST_Group0482_Reg06                  ; // 06  (ADDR : 0x9C00_F118)
    UINT32  RGST_Group0482_Reg07                  ; // 07  (ADDR : 0x9C00_F11C)
    UINT32  RGST_Group0482_Reg08                  ; // 08  (ADDR : 0x9C00_F120)
    UINT32  RGST_Group0482_Reg09                  ; // 09  (ADDR : 0x9C00_F124)
    UINT32  RGST_Group0482_Reg10                  ; // 10  (ADDR : 0x9C00_F128)
    UINT32  RGST_Group0482_Reg11                  ; // 11  (ADDR : 0x9C00_F12C)
    UINT32  RGST_Group0482_Reg12                  ; // 12  (ADDR : 0x9C00_F130)
    UINT32  RGST_Group0482_Reg13                  ; // 13  (ADDR : 0x9C00_F134)
    UINT32  RGST_Group0482_Reg14                  ; // 14  (ADDR : 0x9C00_F138)
    UINT32  RGST_Group0482_Reg15                  ; // 15  (ADDR : 0x9C00_F13C)
    UINT32  RGST_Group0482_Reg16                  ; // 16  (ADDR : 0x9C00_F140)
    UINT32  RGST_Group0482_Reg17                  ; // 17  (ADDR : 0x9C00_F144)
    UINT32  RGST_Group0482_Reg18                  ; // 18  (ADDR : 0x9C00_F148)
    UINT32  RGST_Group0482_Reg19                  ; // 19  (ADDR : 0x9C00_F14C)
    UINT32  RGST_Group0482_Reg20                  ; // 20  (ADDR : 0x9C00_F150)
    UINT32  RGST_Group0482_Reg21                  ; // 21  (ADDR : 0x9C00_F154)
    UINT32  RGST_Group0482_Reg22                  ; // 22  (ADDR : 0x9C00_F158)
    UINT32  RGST_Group0482_Reg23                  ; // 23  (ADDR : 0x9C00_F15C)
    UINT32  RGST_Group0482_Reg24                  ; // 24  (ADDR : 0x9C00_F160)
    UINT32  RGST_Group0482_Reg25                  ; // 25  (ADDR : 0x9C00_F164)
    UINT32  RGST_Group0482_Reg26                  ; // 26  (ADDR : 0x9C00_F168)
    UINT32  RGST_Group0482_Reg27                  ; // 27  (ADDR : 0x9C00_F16C)
    UINT32  RGST_Group0482_Reg28                  ; // 28  (ADDR : 0x9C00_F170)
    UINT32  RGST_Group0482_Reg29                  ; // 29  (ADDR : 0x9C00_F174)
    UINT32  RGST_Group0482_Reg30                  ; // 30  (ADDR : 0x9C00_F178)
    UINT32  RGST_Group0482_Reg31                  ; // 31  (ADDR : 0x9C00_F17C)

    //Group   483 : MFD
    UINT32  RGST_Group0483_Reg00                  ; // 00  (ADDR : 0x9C00_F180)
    UINT32  RGST_Group0483_Reg01                  ; // 01  (ADDR : 0x9C00_F184)
    UINT32  RGST_Group0483_Reg02                  ; // 02  (ADDR : 0x9C00_F188)
    UINT32  RGST_Group0483_Reg03                  ; // 03  (ADDR : 0x9C00_F18C)
    UINT32  RGST_Group0483_Reg04                  ; // 04  (ADDR : 0x9C00_F190)
    UINT32  RGST_Group0483_Reg05                  ; // 05  (ADDR : 0x9C00_F194)
    UINT32  RGST_Group0483_Reg06                  ; // 06  (ADDR : 0x9C00_F198)
    UINT32  RGST_Group0483_Reg07                  ; // 07  (ADDR : 0x9C00_F19C)
    UINT32  RGST_Group0483_Reg08                  ; // 08  (ADDR : 0x9C00_F1A0)
    UINT32  RGST_Group0483_Reg09                  ; // 09  (ADDR : 0x9C00_F1A4)
    UINT32  RGST_Group0483_Reg10                  ; // 10  (ADDR : 0x9C00_F1A8)
    UINT32  RGST_Group0483_Reg11                  ; // 11  (ADDR : 0x9C00_F1AC)
    UINT32  RGST_Group0483_Reg12                  ; // 12  (ADDR : 0x9C00_F1B0)
    UINT32  RGST_Group0483_Reg13                  ; // 13  (ADDR : 0x9C00_F1B4)
    UINT32  RGST_Group0483_Reg14                  ; // 14  (ADDR : 0x9C00_F1B8)
    UINT32  RGST_Group0483_Reg15                  ; // 15  (ADDR : 0x9C00_F1BC)
    UINT32  RGST_Group0483_Reg16                  ; // 16  (ADDR : 0x9C00_F1C0)
    UINT32  RGST_Group0483_Reg17                  ; // 17  (ADDR : 0x9C00_F1C4)
    UINT32  RGST_Group0483_Reg18                  ; // 18  (ADDR : 0x9C00_F1C8)
    UINT32  RGST_Group0483_Reg19                  ; // 19  (ADDR : 0x9C00_F1CC)
    UINT32  RGST_Group0483_Reg20                  ; // 20  (ADDR : 0x9C00_F1D0)
    UINT32  RGST_Group0483_Reg21                  ; // 21  (ADDR : 0x9C00_F1D4)
    UINT32  RGST_Group0483_Reg22                  ; // 22  (ADDR : 0x9C00_F1D8)
    UINT32  RGST_Group0483_Reg23                  ; // 23  (ADDR : 0x9C00_F1DC)
    UINT32  RGST_Group0483_Reg24                  ; // 24  (ADDR : 0x9C00_F1E0)
    UINT32  RGST_Group0483_Reg25                  ; // 25  (ADDR : 0x9C00_F1E4)
    UINT32  RGST_Group0483_Reg26                  ; // 26  (ADDR : 0x9C00_F1E8)
    UINT32  RGST_Group0483_Reg27                  ; // 27  (ADDR : 0x9C00_F1EC)
    UINT32  RGST_Group0483_Reg28                  ; // 28  (ADDR : 0x9C00_F1F0)
    UINT32  RGST_Group0483_Reg29                  ; // 29  (ADDR : 0x9C00_F1F4)
    UINT32  RGST_Group0483_Reg30                  ; // 30  (ADDR : 0x9C00_F1F8)
    UINT32  RGST_Group0483_Reg31                  ; // 31  (ADDR : 0x9C00_F1FC)

    //Group   484 : MFD
    UINT32  RGST_Group0484_Reg00                  ; // 00  (ADDR : 0x9C00_F200)
    UINT32  RGST_Group0484_Reg01                  ; // 01  (ADDR : 0x9C00_F204)
    UINT32  RGST_Group0484_Reg02                  ; // 02  (ADDR : 0x9C00_F208)
    UINT32  RGST_Group0484_Reg03                  ; // 03  (ADDR : 0x9C00_F20C)
    UINT32  RGST_Group0484_Reg04                  ; // 04  (ADDR : 0x9C00_F210)
    UINT32  RGST_Group0484_Reg05                  ; // 05  (ADDR : 0x9C00_F214)
    UINT32  RGST_Group0484_Reg06                  ; // 06  (ADDR : 0x9C00_F218)
    UINT32  RGST_Group0484_Reg07                  ; // 07  (ADDR : 0x9C00_F21C)
    UINT32  RGST_Group0484_Reg08                  ; // 08  (ADDR : 0x9C00_F220)
    UINT32  RGST_Group0484_Reg09                  ; // 09  (ADDR : 0x9C00_F224)
    UINT32  RGST_Group0484_Reg10                  ; // 10  (ADDR : 0x9C00_F228)
    UINT32  RGST_Group0484_Reg11                  ; // 11  (ADDR : 0x9C00_F22C)
    UINT32  RGST_Group0484_Reg12                  ; // 12  (ADDR : 0x9C00_F230)
    UINT32  RGST_Group0484_Reg13                  ; // 13  (ADDR : 0x9C00_F234)
    UINT32  RGST_Group0484_Reg14                  ; // 14  (ADDR : 0x9C00_F238)
    UINT32  RGST_Group0484_Reg15                  ; // 15  (ADDR : 0x9C00_F23C)
    UINT32  RGST_Group0484_Reg16                  ; // 16  (ADDR : 0x9C00_F240)
    UINT32  RGST_Group0484_Reg17                  ; // 17  (ADDR : 0x9C00_F244)
    UINT32  RGST_Group0484_Reg18                  ; // 18  (ADDR : 0x9C00_F248)
    UINT32  RGST_Group0484_Reg19                  ; // 19  (ADDR : 0x9C00_F24C)
    UINT32  RGST_Group0484_Reg20                  ; // 20  (ADDR : 0x9C00_F250)
    UINT32  RGST_Group0484_Reg21                  ; // 21  (ADDR : 0x9C00_F254)
    UINT32  RGST_Group0484_Reg22                  ; // 22  (ADDR : 0x9C00_F258)
    UINT32  RGST_Group0484_Reg23                  ; // 23  (ADDR : 0x9C00_F25C)
    UINT32  RGST_Group0484_Reg24                  ; // 24  (ADDR : 0x9C00_F260)
    UINT32  RGST_Group0484_Reg25                  ; // 25  (ADDR : 0x9C00_F264)
    UINT32  RGST_Group0484_Reg26                  ; // 26  (ADDR : 0x9C00_F268)
    UINT32  RGST_Group0484_Reg27                  ; // 27  (ADDR : 0x9C00_F26C)
    UINT32  RGST_Group0484_Reg28                  ; // 28  (ADDR : 0x9C00_F270)
    UINT32  RGST_Group0484_Reg29                  ; // 29  (ADDR : 0x9C00_F274)
    UINT32  RGST_Group0484_Reg30                  ; // 30  (ADDR : 0x9C00_F278)
    UINT32  RGST_Group0484_Reg31                  ; // 31  (ADDR : 0x9C00_F27C)

    //Group   485 : MFD
    UINT32  RGST_Group0485_Reg00                  ; // 00  (ADDR : 0x9C00_F280)
    UINT32  RGST_Group0485_Reg01                  ; // 01  (ADDR : 0x9C00_F284)
    UINT32  RGST_Group0485_Reg02                  ; // 02  (ADDR : 0x9C00_F288)
    UINT32  RGST_Group0485_Reg03                  ; // 03  (ADDR : 0x9C00_F28C)
    UINT32  RGST_Group0485_Reg04                  ; // 04  (ADDR : 0x9C00_F290)
    UINT32  RGST_Group0485_Reg05                  ; // 05  (ADDR : 0x9C00_F294)
    UINT32  RGST_Group0485_Reg06                  ; // 06  (ADDR : 0x9C00_F298)
    UINT32  RGST_Group0485_Reg07                  ; // 07  (ADDR : 0x9C00_F29C)
    UINT32  RGST_Group0485_Reg08                  ; // 08  (ADDR : 0x9C00_F2A0)
    UINT32  RGST_Group0485_Reg09                  ; // 09  (ADDR : 0x9C00_F2A4)
    UINT32  RGST_Group0485_Reg10                  ; // 10  (ADDR : 0x9C00_F2A8)
    UINT32  RGST_Group0485_Reg11                  ; // 11  (ADDR : 0x9C00_F2AC)
    UINT32  RGST_Group0485_Reg12                  ; // 12  (ADDR : 0x9C00_F2B0)
    UINT32  RGST_Group0485_Reg13                  ; // 13  (ADDR : 0x9C00_F2B4)
    UINT32  RGST_Group0485_Reg14                  ; // 14  (ADDR : 0x9C00_F2B8)
    UINT32  RGST_Group0485_Reg15                  ; // 15  (ADDR : 0x9C00_F2BC)
    UINT32  RGST_Group0485_Reg16                  ; // 16  (ADDR : 0x9C00_F2C0)
    UINT32  RGST_Group0485_Reg17                  ; // 17  (ADDR : 0x9C00_F2C4)
    UINT32  RGST_Group0485_Reg18                  ; // 18  (ADDR : 0x9C00_F2C8)
    UINT32  RGST_Group0485_Reg19                  ; // 19  (ADDR : 0x9C00_F2CC)
    UINT32  RGST_Group0485_Reg20                  ; // 20  (ADDR : 0x9C00_F2D0)
    UINT32  RGST_Group0485_Reg21                  ; // 21  (ADDR : 0x9C00_F2D4)
    UINT32  RGST_Group0485_Reg22                  ; // 22  (ADDR : 0x9C00_F2D8)
    UINT32  RGST_Group0485_Reg23                  ; // 23  (ADDR : 0x9C00_F2DC)
    UINT32  RGST_Group0485_Reg24                  ; // 24  (ADDR : 0x9C00_F2E0)
    UINT32  RGST_Group0485_Reg25                  ; // 25  (ADDR : 0x9C00_F2E4)
    UINT32  RGST_Group0485_Reg26                  ; // 26  (ADDR : 0x9C00_F2E8)
    UINT32  RGST_Group0485_Reg27                  ; // 27  (ADDR : 0x9C00_F2EC)
    UINT32  RGST_Group0485_Reg28                  ; // 28  (ADDR : 0x9C00_F2F0)
    UINT32  RGST_Group0485_Reg29                  ; // 29  (ADDR : 0x9C00_F2F4)
    UINT32  RGST_Group0485_Reg30                  ; // 30  (ADDR : 0x9C00_F2F8)
    UINT32  RGST_Group0485_Reg31                  ; // 31  (ADDR : 0x9C00_F2FC)

    //Group   486 : MFD
    UINT32  RGST_Group0486_Reg00                  ; // 00  (ADDR : 0x9C00_F300)
    UINT32  RGST_Group0486_Reg01                  ; // 01  (ADDR : 0x9C00_F304)
    UINT32  RGST_Group0486_Reg02                  ; // 02  (ADDR : 0x9C00_F308)
    UINT32  RGST_Group0486_Reg03                  ; // 03  (ADDR : 0x9C00_F30C)
    UINT32  RGST_Group0486_Reg04                  ; // 04  (ADDR : 0x9C00_F310)
    UINT32  RGST_Group0486_Reg05                  ; // 05  (ADDR : 0x9C00_F314)
    UINT32  RGST_Group0486_Reg06                  ; // 06  (ADDR : 0x9C00_F318)
    UINT32  RGST_Group0486_Reg07                  ; // 07  (ADDR : 0x9C00_F31C)
    UINT32  RGST_Group0486_Reg08                  ; // 08  (ADDR : 0x9C00_F320)
    UINT32  RGST_Group0486_Reg09                  ; // 09  (ADDR : 0x9C00_F324)
    UINT32  RGST_Group0486_Reg10                  ; // 10  (ADDR : 0x9C00_F328)
    UINT32  RGST_Group0486_Reg11                  ; // 11  (ADDR : 0x9C00_F32C)
    UINT32  RGST_Group0486_Reg12                  ; // 12  (ADDR : 0x9C00_F330)
    UINT32  RGST_Group0486_Reg13                  ; // 13  (ADDR : 0x9C00_F334)
    UINT32  RGST_Group0486_Reg14                  ; // 14  (ADDR : 0x9C00_F338)
    UINT32  RGST_Group0486_Reg15                  ; // 15  (ADDR : 0x9C00_F33C)
    UINT32  RGST_Group0486_Reg16                  ; // 16  (ADDR : 0x9C00_F340)
    UINT32  RGST_Group0486_Reg17                  ; // 17  (ADDR : 0x9C00_F344)
    UINT32  RGST_Group0486_Reg18                  ; // 18  (ADDR : 0x9C00_F348)
    UINT32  RGST_Group0486_Reg19                  ; // 19  (ADDR : 0x9C00_F34C)
    UINT32  RGST_Group0486_Reg20                  ; // 20  (ADDR : 0x9C00_F350)
    UINT32  RGST_Group0486_Reg21                  ; // 21  (ADDR : 0x9C00_F354)
    UINT32  RGST_Group0486_Reg22                  ; // 22  (ADDR : 0x9C00_F358)
    UINT32  RGST_Group0486_Reg23                  ; // 23  (ADDR : 0x9C00_F35C)
    UINT32  RGST_Group0486_Reg24                  ; // 24  (ADDR : 0x9C00_F360)
    UINT32  RGST_Group0486_Reg25                  ; // 25  (ADDR : 0x9C00_F364)
    UINT32  RGST_Group0486_Reg26                  ; // 26  (ADDR : 0x9C00_F368)
    UINT32  RGST_Group0486_Reg27                  ; // 27  (ADDR : 0x9C00_F36C)
    UINT32  RGST_Group0486_Reg28                  ; // 28  (ADDR : 0x9C00_F370)
    UINT32  RGST_Group0486_Reg29                  ; // 29  (ADDR : 0x9C00_F374)
    UINT32  RGST_Group0486_Reg30                  ; // 30  (ADDR : 0x9C00_F378)
    UINT32  RGST_Group0486_Reg31                  ; // 31  (ADDR : 0x9C00_F37C)

    //Group   487 : MFD
    UINT32  RGST_Group0487_Reg00                  ; // 00  (ADDR : 0x9C00_F380)
    UINT32  RGST_Group0487_Reg01                  ; // 01  (ADDR : 0x9C00_F384)
    UINT32  RGST_Group0487_Reg02                  ; // 02  (ADDR : 0x9C00_F388)
    UINT32  RGST_Group0487_Reg03                  ; // 03  (ADDR : 0x9C00_F38C)
    UINT32  RGST_Group0487_Reg04                  ; // 04  (ADDR : 0x9C00_F390)
    UINT32  RGST_Group0487_Reg05                  ; // 05  (ADDR : 0x9C00_F394)
    UINT32  RGST_Group0487_Reg06                  ; // 06  (ADDR : 0x9C00_F398)
    UINT32  RGST_Group0487_Reg07                  ; // 07  (ADDR : 0x9C00_F39C)
    UINT32  RGST_Group0487_Reg08                  ; // 08  (ADDR : 0x9C00_F3A0)
    UINT32  RGST_Group0487_Reg09                  ; // 09  (ADDR : 0x9C00_F3A4)
    UINT32  RGST_Group0487_Reg10                  ; // 10  (ADDR : 0x9C00_F3A8)
    UINT32  RGST_Group0487_Reg11                  ; // 11  (ADDR : 0x9C00_F3AC)
    UINT32  RGST_Group0487_Reg12                  ; // 12  (ADDR : 0x9C00_F3B0)
    UINT32  RGST_Group0487_Reg13                  ; // 13  (ADDR : 0x9C00_F3B4)
    UINT32  RGST_Group0487_Reg14                  ; // 14  (ADDR : 0x9C00_F3B8)
    UINT32  RGST_Group0487_Reg15                  ; // 15  (ADDR : 0x9C00_F3BC)
    UINT32  RGST_Group0487_Reg16                  ; // 16  (ADDR : 0x9C00_F3C0)
    UINT32  RGST_Group0487_Reg17                  ; // 17  (ADDR : 0x9C00_F3C4)
    UINT32  RGST_Group0487_Reg18                  ; // 18  (ADDR : 0x9C00_F3C8)
    UINT32  RGST_Group0487_Reg19                  ; // 19  (ADDR : 0x9C00_F3CC)
    UINT32  RGST_Group0487_Reg20                  ; // 20  (ADDR : 0x9C00_F3D0)
    UINT32  RGST_Group0487_Reg21                  ; // 21  (ADDR : 0x9C00_F3D4)
    UINT32  RGST_Group0487_Reg22                  ; // 22  (ADDR : 0x9C00_F3D8)
    UINT32  RGST_Group0487_Reg23                  ; // 23  (ADDR : 0x9C00_F3DC)
    UINT32  RGST_Group0487_Reg24                  ; // 24  (ADDR : 0x9C00_F3E0)
    UINT32  RGST_Group0487_Reg25                  ; // 25  (ADDR : 0x9C00_F3E4)
    UINT32  RGST_Group0487_Reg26                  ; // 26  (ADDR : 0x9C00_F3E8)
    UINT32  RGST_Group0487_Reg27                  ; // 27  (ADDR : 0x9C00_F3EC)
    UINT32  RGST_Group0487_Reg28                  ; // 28  (ADDR : 0x9C00_F3F0)
    UINT32  RGST_Group0487_Reg29                  ; // 29  (ADDR : 0x9C00_F3F4)
    UINT32  RGST_Group0487_Reg30                  ; // 30  (ADDR : 0x9C00_F3F8)
    UINT32  RGST_Group0487_Reg31                  ; // 31  (ADDR : 0x9C00_F3FC)

    //Group   488 ~ 499 : Reserved
    UINT32  RGST_Group0488_Reserved[32]           ; //     (ADDR : 0x9C00_F400) ~ (ADDR : 0x9C00_F47C)
    UINT32  RGST_Group0489_Reserved[32]           ; //     (ADDR : 0x9C00_F480) ~ (ADDR : 0x9C00_F4FC)
    UINT32  RGST_Group0490_Reserved[32]           ; //     (ADDR : 0x9C00_F500) ~ (ADDR : 0x9C00_F57C)
    UINT32  RGST_Group0491_Reserved[32]           ; //     (ADDR : 0x9C00_F580) ~ (ADDR : 0x9C00_F5FC)
    UINT32  RGST_Group0492_Reserved[32]           ; //     (ADDR : 0x9C00_F600) ~ (ADDR : 0x9C00_F67C)
    UINT32  RGST_Group0493_Reserved[32]           ; //     (ADDR : 0x9C00_F680) ~ (ADDR : 0x9C00_F6FC)
    UINT32  RGST_Group0494_Reserved[32]           ; //     (ADDR : 0x9C00_F700) ~ (ADDR : 0x9C00_F77C)
    UINT32  RGST_Group0495_Reserved[32]           ; //     (ADDR : 0x9C00_F780) ~ (ADDR : 0x9C00_F7FC)
    UINT32  RGST_Group0496_Reserved[32]           ; //     (ADDR : 0x9C00_F800) ~ (ADDR : 0x9C00_F87C)
    UINT32  RGST_Group0497_Reserved[32]           ; //     (ADDR : 0x9C00_F880) ~ (ADDR : 0x9C00_F8FC)
    UINT32  RGST_Group0498_Reserved[32]           ; //     (ADDR : 0x9C00_F900) ~ (ADDR : 0x9C00_F97C)
    UINT32  RGST_Group0499_Reserved[32]           ; //     (ADDR : 0x9C00_F980) ~ (ADDR : 0x9C00_F9FC)

    //Group   500 : RBUS
    UINT32  RBUS_MONITOR_CFG                      ; // 00  (ADDR : 0xBFFE_FA00)
    UINT32  RBUS_MONITOR_EVENT                    ; // 01  (ADDR : 0xBFFE_FA04)
    UINT32  RBUS_TIMEOUT_CYCLE                    ; // 02  (ADDR : 0xBFFE_FA08)
    UINT32  RBUS_TERMINATE_CYCLE                  ; // 03  (ADDR : 0xBFFE_FA0C)
    UINT32  RBUS_TRANS_INFO0                      ; // 04  (ADDR : 0xBFFE_FA10)
    UINT32  RBUS_TRANS_INFO1                      ; // 05  (ADDR : 0xBFFE_FA14)
    UINT32  RBUS_TRANS_INFO2                      ; // 06  (ADDR : 0xBFFE_FA18)
    UINT32  RBUS_RESERVED[25]                     ; //     (ADDR : 0xBFFE_FA1C) ~ (ADDR : 0xBFFE_FA7C) 

    //Group   501 : RBUS
    UINT32  RGST_Group0501_Reserved[32]           ; //     (ADDR : 0xBFFE_FA80) ~ (ADDR : 0xBFFE_FAFC)

    //Group   502 : RBUS
    UINT32  RGST_Group0502_Reserved[32]           ; //     (ADDR : 0xBFFE_FB00) ~ (ADDR : 0xBFFE_FB7C)

    //Group   503 : RBUS
    UINT32  RGST_Group0503_Reserved[32]           ; //     (ADDR : 0xBFFE_FB80) ~ (ADDR : 0xBFFE_FBFC)


    // Group 504 ~ 511 , Reserved
    UINT32  G504_G511_RESERVED[8*32]            ;

} RegisterFile0;

//#define regs0   ((volatile RegisterFile0 *)(REG_BASE))

//=================================================================================================

/*
 * TYPE: RegisterFile1
 */
typedef struct
{
    // GROUP 1021 : C-Sim Error Message
    UINT32  CSIM_ERROR[32]                          ; // 00~31 (ADDR : 0x9C01_FE80) ~ (ADDR : 0x9C01_FEFC)

    // GROUP 1022 : C-Sim printf
    UINT32  PRINTF_REG[16]                          ; // 00~15 (ADDR : 0x9C01_FF00) ~ (ADDR : 0x9C01_FF3C)
    UINT32  SPI_MODEL[16]                           ; // 16~31 (ADDR : 0x9C01_FF40) ~ (ADDR : 0x9C01_FF7C)

    // GROUP 1023 : C-Sim DDR3 Setter
    UINT32  D3_SETTER_RESERVED_0[16]                ; // 00~15 (ADDR : 0x9C01_FF80) ~ (ADDR : 0x9C01_FFBC)
    UINT32  D3_SETTER_dram_start_addr_filetype      ; // 16    (ADDR : 0x9C01_FFC0)
    UINT32  D3_SETTER_dram_write_size_filetype      ; // 17    (ADDR : 0x9C01_FFC4)
    UINT32  D3_SETTER_dram_start_addr_romtype       ; // 18    (ADDR : 0x9C01_FFC8)
    UINT32  D3_SETTER_dram_write_size_romtype       ; // 19    (ADDR : 0x9C01_FFCC)
    UINT32  D3_SETTER_rom_data_section_addr_romtype ; // 20    (ADDR : 0x9C01_FFD0)
    UINT32  D3_SETTER_dram_start_addr_nulltype      ; // 21    (ADDR : 0x9C01_FFD4)
    UINT32  D3_SETTER_dram_write_size_nulltype      ; // 22    (ADDR : 0x9C01_FFD8)
    UINT32  D3_SETTER_RESERVED_1[9]                 ; // 23~31 (ADDR : 0x9C01_FFDC) ~ (ADDR : 0x9C01_FFFC)
} RegisterFile1;

#define regs1   ((volatile RegisterFile1 *)(REG_BASE + 1021*32*4))

//=================================================================================================

/*
 * TYPE: RegisterFile_bch
 */
typedef struct
{
    UINT32  BCH_Configuration                     ; // 0x9C10_1000  //bch control register
    UINT32  BCH_Data_Pointer                      ; // 0x9C10_1004  //encode/decode data start address
    UINT32  BCH_Parity_Pointer                    ; // 0x9C10_1008  //encode/decode parity start address
    UINT32  BCH_Intr_status                       ; // 0x9C10_100C  //bch interrupt status, write 1 clear
    UINT32  BCH_Soft_Reset                        ; // 0x9C10_1010  //write 1 to reset bch
    UINT32  BCH_Intr_Mask                         ; // 0x9C10_1014  //bch interrupt mask control register
    UINT32  BCH_Rpt_Status                        ; // 0x9C10_1018  //bch report status
    UINT32  BCH_Sector_Error                      ; // 0x9C10_101C  //report the Nth(N=0~31) block has error bit or not
    UINT32  BCH_Sector_Fail                       ; // 0x9C10_1020  //report the Nth(N=0~31) block has too much error bit(can't fix)
    UINT32  BCH_LastW_Address                     ; // 0x9C10_1024  //Record the last write operation's address
    UINT32  BCH_LastW_Data                        ; // 0x9C10_1028  //Record the last write operation's data
    UINT32  BCH_Control_reg                       ; // 0x9C10_102C  //the control reg for ff&00 check and corrtection method
    UINT32  BCH_IP_Number                         ; // 0x9C10_1030  //32'h7303_0309//s339
    UINT32  BCH_Update_Date                       ; // 0x9C10_1034  //32'h2013_0309//s339
} RegisterFile_bch;

#define regs_bch   ((volatile RegisterFile_bch *)(REG_BASE + 0x101000))
//=================================================================================================

/*
 * TYPE: RegisterFile_nand
 */
typedef struct
{
    UINT32  NAND_CSR                              ; // 0x9C82_B000
    UINT32  NAND_DESC_BA                          ; // 0x9C82_B004
    UINT32  NAND_AC_TIMING0                       ; // 0x9C82_B008
    UINT32  NAND_RDYBSY_EN                        ; // 0x9C82_B00C
    UINT32  NAND_PIO_CTRL1                        ; // 0x9C82_B010
    UINT32  NAND_PIO_CTRL2                        ; // 0x9C82_B014
    UINT32  NAND_PIO_CTRL3                        ; // 0x9C82_B018
    UINT32  NAND_PIO_CTRL4                        ; // 0x9C82_B01C
    UINT32  NAND_PIO_CTRL5                        ; // 0x9C82_B020
    UINT32  NAND_PIO_CTRL6                        ; // 0x9C82_B024
    UINT32  NAND_PIO_CTRL7                        ; // 0x9C82_B028
    UINT32  NAND_PIO_CTRL8                        ; // 0x9C82_B02C
    UINT32  NAND_TGL_CTRL                         ; // 0x9C82_B030
    UINT32  NAND_SWITCH_CTRL                      ; // 0x9C82_B034
    UINT32  NAND_BIT0_TOLERANCE_CNT               ; // 0x9C82_B038
    UINT32  NAND_RESERVE_0                        ; // 0x9C82_B03C
    UINT32  NAND_INTRMSK                          ; // 0x9C82_B040
    UINT32  NAND_INTR_STS                         ; // 0x9C82_B044
    UINT32  NAND_RDYBSY_DLY_INT                   ; // 0x9C82_B048
    UINT32  NAND_RESERVE_1                        ; // 0x9C82_B04C
    UINT32  NAND_RANDOM_EN                        ; // 0x9C82_B050
    UINT32  NAND_PRBS_COE                         ; // 0x9C82_B054
    UINT32  NAND_SeedGen_Ctrl                     ; // 0x9C82_B058
    UINT32  NAND_PRE_NUM0                         ; // 0x9C82_B05C
    UINT32  NAND_PRE_NUM1                         ; // 0x9C82_B060
    UINT32  NAND_PRE_NUM2                         ; // 0x9C82_B064
    UINT32  NAND_PRE_NUM3                         ; // 0x9C82_B068
    UINT32  NAND_PRE_NUM4                         ; // 0x9C82_B06C
    UINT32  NAND_PRE_NUM5                         ; // 0x9C82_B070
    UINT32  NAND_PRE_NUM6                         ; // 0x9C82_B074
    UINT32  NAND_PRE_NUM7                         ; // 0x9C82_B078
    UINT32  NAND_PIO1_PAGE_ADDR                   ; // 0x9C82_B07C
    UINT32  NAND_PIO1_LOAD_SEED                   ; // 0x9C82_B080
    UINT32  NAND_SUCMD_WAIT_CTRL                  ; // 0x9C82_B084
    UINT32  NAND_STS_RD_STS                       ; // 0x9C82_B088
    UINT32  NAND_AC_TIMING1                       ; // 0x9C82_B08C
} RegisterFile_NAND;

#define regs_nand   ((volatile RegisterFile_NAND *)(REG_BASE + 0x100000))

//=================================================================================================
/*
 * TYPE: RegisterFile_USBD
 */
typedef struct
{
    UINT32  USBD_EP12DMACS                        ; //0x000
    UINT32  USBD_EP12DMADA                        ; //0x004
    UINT32  USBD_AUDMACS                          ; //0x008
    UINT32  USBD_AUDMADA                          ; //0x00C
    UINT32  USBD_EP89DMACS                        ; //0x010
    UINT32  USBD_EP89DMADA                        ; //0x014
    UINT32  USBD_EPABDMACS                        ; //0x018
    UINT32  USBD_EPABDMADA                        ; //0x01C
    UINT32  USBD_WRAPPEREN                        ; //0x020
    UINT32  USBD_RESERVED1[23]                    ; //0x024 - 0x07C
    UINT32  USBD_UDCCS                            ; //0x080
    UINT32  USBD_UDCIE                            ; //0x084
    UINT32  USBD_UDCIF                            ; //0x088
    UINT32  USBD_UDCIS                            ; //0x08C
    UINT32  USBD_RESERVED2[28]                    ; //0x090 - 0x0FC
    UINT32  USBD_EP4CTRL                          ; //0x100
    UINT32  USBD_EP4PTR                           ; //0x104
    UINT32  USBD_EP4DATA                          ; //0x108
    UINT32  USBD_EP4VB                            ; //0x10C
    UINT32  USBD_RESERVED3[12]                    ; //0x110 - 0x13C
    UINT32  USBD_EP5CTRL                          ; //0x140
    UINT32  USBD_EP5HDLEN                         ; //0x144
    UINT32  USBD_EP5FRAME                         ; //0x148
    UINT32  USBD_EP5HDCTRL                        ; //0x14C
    UINT32  USBD_EP5EN                            ; //0x150
    UINT32  USBD_EP5RPT                           ; //0x154
    UINT32  USBD_EP5WPT                           ; //0x158
    UINT32  USBD_EP5DATA                          ; //0x15C
    UINT32  USBD_VDMAEN                           ; //0x160
    UINT32  USBD_SRE                              ; //0x164
    UINT32  USBD_FTLB                             ; //0x168
    UINT32  USBD_FTHB                             ; //0x16C
    UINT32  USBD_EP5VB                            ; //0x170
    UINT32  USBD_STCDIVL                          ; //0x174
    UINT32  USBD_STCDIVH                          ; //0x178
    UINT32  USBD_RESERVED4                        ; //0x17C
    UINT32  USBD_EP6CTRL                          ; //0x180
    UINT32  USBD_EP6PTR                           ; //0x184
    UINT32  USBD_EP6DATA                          ; //0x188
    UINT32  USBD_EP6VB                            ; //0x18C
    UINT32  USBD_RESERVED5[12]                    ; //0x190 - 0x1BC
    UINT32  USBD_EP7CTRL                          ; //0x1C0
    UINT32  USBD_EP7PTRR                          ; //0x1C4
    UINT32  USBD_EP7PTR                           ; //0x1C8
    UINT32  USBD_EP7DATA                          ; //0x1CC
    UINT32  USBD_EP7VB                            ; //0x1D0
    UINT32  USBD_RESERVED6[75]                    ; //0x1D4 - 0x2FC
    UINT32  USBD_EP0SCS                           ; //0x300
    UINT32  USBD_EP0SDP                           ; //0x304
    UINT32  USBD_EP0CS                            ; //0x308
    UINT32  USBD_EP0DC                            ; //0x30C
    UINT32  USBD_EP0DP                            ; //0x310
    UINT32  USBD_EP0VB                            ; //0x314
    UINT32  USBD_EP0ONKC                          ; //0x318
    UINT32  USBD_EP0INAKCN                        ; //0x31C
    UINT32  USBD_RESERVED7[2]                     ; //0x320 - 0x324
    UINT32  USBD_MSTC                             ; //0x328
    UINT32  USBD_RESERVED8                        ; //0x32C
    UINT32  USBD_EP12C                            ; //0x330
    UINT32  USBD_EP12PPC                          ; //0x334
    UINT32  USBD_EP12FS                           ; //0x338
    UINT32  USBD_EP12PICL                         ; //0x33C
    UINT32  USBD_EP12PICH                         ; //0x340
    UINT32  USBD_EP12POCL                         ; //0x344
    UINT32  USBD_EP12POCH                         ; //0x348
    UINT32  USBD_EP12FDP                          ; //0x34C
    UINT32  USBD_EP12VB                           ; //0x350
    UINT32  USBD_RESERVED9                        ; //0x354
    UINT32  USBD_EP1SCS                           ; //0x358
    UINT32  USBD_EP1SDP                           ; //0x35C
    UINT32  USBD_EP1INAKCN                        ; //0x360
    UINT32  USBD_EP2ONAKCN                        ; //0x364
    UINT32  USBD_RESERVED10[2]                    ; //0x368 - 0x36C
    UINT32  USBD_EP3VB                            ; //0x370
    UINT32  USBD_EP3CTRL                          ; //0x374
    UINT32  USBD_EP3PTR                           ; //0x378
    UINT32  USBD_EP3DATA                          ; //0x37C
    UINT32  USBD_RESERVED11[12]                   ; //0x380 - 0x3AC
    UINT32  USBD_LCSET0                           ; //0x3B0
    UINT32  USBD_LCSET1                           ; //0x3B4
    UINT32  USBD_LCSET2                           ; //0x3B8
    UINT32  USBD_LCS3                             ; //0x3BC
    UINT32  USBD_LCS                              ; //0x3C0
    UINT32  USBD_LCS2                             ; //0x3C4
    UINT32  USBD_LCSTL                            ; //0x3C8
    UINT32  USBD_RESERVED12[9]                    ; //0x3CC - 0x3EC
    UINT32  USBD_LCPTG                            ; //0x3F0
    UINT32  USBD_LCADDR                           ; //0x3F4
    UINT32  USBD_RESERVED13[2]                    ; //0x3F8 - 0x3FC
    UINT32  USBD_UDC_IF                           ; //0x400
    UINT32  USBD_UDC_IE                           ; //0x404
    UINT32  USBD_UDC_IS                           ; //0x408
    UINT32  USBD_RESERVED14                       ; //0x40C
    UINT32  USBD_STD_IF                           ; //0x410
    UINT32  USBD_STD_IE                           ; //0x414
    UINT32  USBD_STD_IS                           ; //0x418
    UINT32  USBD_RESERVED15                       ; //0x41C
    UINT32  USBD_NEP_IF                           ; //0x420
    UINT32  USBD_NEP_IE                           ; //0x424
    UINT32  USBD_NEP_IS                           ; //0x428
    UINT32  USBD_RESERVED16[53]                   ; //0x42C - 0x4FC
    UINT32  USBD_EP89C                            ; //0x500
    UINT32  USBD_EP89PPC                          ; //0x504
    UINT32  USBD_EP89FS                           ; //0x508
    UINT32  USBD_EP89PICL                         ; //0x50C
    UINT32  USBD_EP89PICH                         ; //0x510
    UINT32  USBD_EP89POCL                         ; //0x514
    UINT32  USBD_EP89POCH                         ; //0x518
    UINT32  USBD_EP89FDP                          ; //0x51C
    UINT32  USBD_EP89VB                           ; //0x520
    UINT32  USBD_EP8INAKCN                        ; //0x524
    UINT32  USBD_EP90NAKCN                        ; //0x528
    UINT32  USBD_EP89S                            ; //0x52C
    UINT32  USBD_RESERVED17[8]                    ; //0x530 - 0x54C
    UINT32  USBD_EPABC                            ; //0x550
    UINT32  USBD_EPABPPC                          ; //0x554
    UINT32  USBD_EPABFS                           ; //0x558
    UINT32  USBD_EPABPICL                         ; //0x55C
    UINT32  USBD_EPABPICH                         ; //0x560
    UINT32  USBD_EPABPOCL                         ; //0x564
    UINT32  USBD_EPABPOCH                         ; //0x568
    UINT32  USBD_EPABFDP                          ; //0x56C
    UINT32  USBD_EPABVB                           ; //0x570
    UINT32  USBD_EPAINAKCN                        ; //0x574
    UINT32  USBD_EPB0NAKCN                        ; //0x578
    UINT32  USBD_EPABS                            ; //0x57C
    UINT32  USBD_RESERVED18[30]                   ; //0x580 - 0x5F4
    UINT32  USBD_IP_NAME                          ; //0x5F8
    UINT32  USBD_VERSION                          ; //0x5FC
} RegisterFile_USBD;

#define regs_usbd   ((volatile RegisterFile_USBD *)(REG_BASE + 0x82d000))
// mankind added for register r/w check  start
 #define GRP_RW0(a, b)           *((volatile unsigned int   *)(REG_BASE +       a*128 + b*4      ))
// mankind added for register r/w check  end

//=================================================================================================

/*
 * TYPE: RegisterFile_CBUS_SRAM
 */
typedef struct
{
    UINT32  CBUS_SRAM[4000]                       ; // 0x9E830000  //16KB CBUS SRAM
} RegisterFile_CBUS_SRAM;


#define regs_cbus_sram   ((volatile RegisterFile_CBUS_SRAM *)(0x9E830000))

// Type define for USB Host Registers
typedef struct {
 //   UINT32 g141_reserved[32];                      // 00 1952~1983 (1e80) $bffe9e80
    UINT32 HostVer;
    UINT32 HostCtrl;
    UINT32 OhciCtrl0;
    UINT32 EhciCtrl0;
    UINT32 PowerCtrl;
    UINT32 dummy0[23];
    UINT32 OhciFsm0;
    UINT32 OhciFsm1;
    UINT32 EhciFsm0;
    UINT32 EhciFsm1;

 //   UINT32 g142_reserved[32];                      // 00 1984~2015 (1f00) $bffe9f00
    UINT32 OhciVer;
    UINT32 OhciCtrl;
    UINT32 OhciCmd;
    UINT32 OhciIntr;
    UINT32 OhciIntrEn;
    UINT32 OhciIntrDis;
    UINT32 OhciHCCA;
    UINT32 OhciPCED;
    UINT32 OhciCHED;
    UINT32 OhciCCED;
    UINT32 OhciBHED;
    UINT32 OhciBCED;
    UINT32 OhciDH;
    UINT32 OhciFI;
    UINT32 OhciFR;
    UINT32 OhciFN;
    UINT32 OhciPS;
    UINT32 OhciLST;
    UINT32 OhciRDA;
    UINT32 OhciRDB;
    UINT32 OhciRS;
    UINT32 OhciRPS;
    UINT32 dummy1[3];
    UINT32 OhciRPSA;
    UINT32 dummy2[6];

 //   UINT32 g143_reserved[32];                      // 00 2016~2047 (1f80) $bffe9f80
    UINT32 EhciVer;
    UINT32 EhciSP;
    UINT32 EhciCP;
    UINT32 EhciSPPR;
    UINT32 dummy3[4];
    UINT32 EhciCMD;
    UINT32 EhciSTS;
    UINT32 EhciIntr;
    UINT32 EhciFIdx;
    UINT32 EhciCDS;
    UINT32 EhciPLB;
    UINT32 EhciALA;
    UINT32 dummy4[9];
    UINT32 EhciCFG;
    UINT32 EhciPortSC;
    UINT32 dummy5[3];
    UINT32 EhciPortSCA;
    UINT32 dummy6[2];
} RegisterFiles_USBH;
#endif /*__REGMAP_8600_H__*/

