/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000




/* Port definition for Pin Group LCD_GPIO */
#define LCD_GPIO_PORT                                                    (GPIOA)

/* Defines for RS: GPIOA.3 with pinCMx 8 on package pin 43 */
#define LCD_GPIO_RS_PIN                                          (DL_GPIO_PIN_3)
#define LCD_GPIO_RS_IOMUX                                         (IOMUX_PINCM8)
/* Defines for WR: GPIOA.30 with pinCMx 5 on package pin 37 */
#define LCD_GPIO_WR_PIN                                         (DL_GPIO_PIN_30)
#define LCD_GPIO_WR_IOMUX                                         (IOMUX_PINCM5)
/* Defines for RD: GPIOA.29 with pinCMx 4 on package pin 36 */
#define LCD_GPIO_RD_PIN                                         (DL_GPIO_PIN_29)
#define LCD_GPIO_RD_IOMUX                                         (IOMUX_PINCM4)
/* Defines for CS: GPIOA.4 with pinCMx 9 on package pin 44 */
#define LCD_GPIO_CS_PIN                                          (DL_GPIO_PIN_4)
#define LCD_GPIO_CS_IOMUX                                         (IOMUX_PINCM9)
/* Defines for RST: GPIOA.28 with pinCMx 3 on package pin 35 */
#define LCD_GPIO_RST_PIN                                        (DL_GPIO_PIN_28)
#define LCD_GPIO_RST_IOMUX                                        (IOMUX_PINCM3)
/* Port definition for Pin Group LCD_Data */
#define LCD_Data_PORT                                                    (GPIOB)

/* Defines for D0: GPIOB.0 with pinCMx 12 on package pin 47 */
#define LCD_Data_D0_PIN                                          (DL_GPIO_PIN_0)
#define LCD_Data_D0_IOMUX                                        (IOMUX_PINCM12)
/* Defines for D1: GPIOB.1 with pinCMx 13 on package pin 48 */
#define LCD_Data_D1_PIN                                          (DL_GPIO_PIN_1)
#define LCD_Data_D1_IOMUX                                        (IOMUX_PINCM13)
/* Defines for D2: GPIOB.2 with pinCMx 15 on package pin 50 */
#define LCD_Data_D2_PIN                                          (DL_GPIO_PIN_2)
#define LCD_Data_D2_IOMUX                                        (IOMUX_PINCM15)
/* Defines for D3: GPIOB.3 with pinCMx 16 on package pin 51 */
#define LCD_Data_D3_PIN                                          (DL_GPIO_PIN_3)
#define LCD_Data_D3_IOMUX                                        (IOMUX_PINCM16)
/* Defines for D4: GPIOB.4 with pinCMx 17 on package pin 52 */
#define LCD_Data_D4_PIN                                          (DL_GPIO_PIN_4)
#define LCD_Data_D4_IOMUX                                        (IOMUX_PINCM17)
/* Defines for D5: GPIOB.5 with pinCMx 18 on package pin 53 */
#define LCD_Data_D5_PIN                                          (DL_GPIO_PIN_5)
#define LCD_Data_D5_IOMUX                                        (IOMUX_PINCM18)
/* Defines for D6: GPIOB.6 with pinCMx 23 on package pin 58 */
#define LCD_Data_D6_PIN                                          (DL_GPIO_PIN_6)
#define LCD_Data_D6_IOMUX                                        (IOMUX_PINCM23)
/* Defines for D7: GPIOB.7 with pinCMx 24 on package pin 59 */
#define LCD_Data_D7_PIN                                          (DL_GPIO_PIN_7)
#define LCD_Data_D7_IOMUX                                        (IOMUX_PINCM24)
/* Defines for D8: GPIOB.8 with pinCMx 25 on package pin 60 */
#define LCD_Data_D8_PIN                                          (DL_GPIO_PIN_8)
#define LCD_Data_D8_IOMUX                                        (IOMUX_PINCM25)
/* Defines for D9: GPIOB.9 with pinCMx 26 on package pin 61 */
#define LCD_Data_D9_PIN                                          (DL_GPIO_PIN_9)
#define LCD_Data_D9_IOMUX                                        (IOMUX_PINCM26)
/* Defines for D10: GPIOB.10 with pinCMx 27 on package pin 62 */
#define LCD_Data_D10_PIN                                        (DL_GPIO_PIN_10)
#define LCD_Data_D10_IOMUX                                       (IOMUX_PINCM27)
/* Defines for D11: GPIOB.11 with pinCMx 28 on package pin 63 */
#define LCD_Data_D11_PIN                                        (DL_GPIO_PIN_11)
#define LCD_Data_D11_IOMUX                                       (IOMUX_PINCM28)
/* Defines for D12: GPIOB.12 with pinCMx 29 on package pin 64 */
#define LCD_Data_D12_PIN                                        (DL_GPIO_PIN_12)
#define LCD_Data_D12_IOMUX                                       (IOMUX_PINCM29)
/* Defines for D13: GPIOB.13 with pinCMx 30 on package pin 1 */
#define LCD_Data_D13_PIN                                        (DL_GPIO_PIN_13)
#define LCD_Data_D13_IOMUX                                       (IOMUX_PINCM30)
/* Defines for D14: GPIOB.14 with pinCMx 31 on package pin 2 */
#define LCD_Data_D14_PIN                                        (DL_GPIO_PIN_14)
#define LCD_Data_D14_IOMUX                                       (IOMUX_PINCM31)
/* Defines for D15: GPIOB.15 with pinCMx 32 on package pin 3 */
#define LCD_Data_D15_PIN                                        (DL_GPIO_PIN_15)
#define LCD_Data_D15_IOMUX                                       (IOMUX_PINCM32)
/* Port definition for Pin Group TOUCH */
#define TOUCH_PORT                                                       (GPIOA)

/* Defines for SDA: GPIOA.16 with pinCMx 38 on package pin 9 */
#define TOUCH_SDA_PIN                                           (DL_GPIO_PIN_16)
#define TOUCH_SDA_IOMUX                                          (IOMUX_PINCM38)
/* Defines for SCL: GPIOA.17 with pinCMx 39 on package pin 10 */
#define TOUCH_SCL_PIN                                           (DL_GPIO_PIN_17)
#define TOUCH_SCL_IOMUX                                          (IOMUX_PINCM39)
/* Defines for PEN: GPIOA.15 with pinCMx 37 on package pin 8 */
#define TOUCH_PEN_PIN                                           (DL_GPIO_PIN_15)
#define TOUCH_PEN_IOMUX                                          (IOMUX_PINCM37)
/* Defines for TCS: GPIOA.2 with pinCMx 7 on package pin 42 */
#define TOUCH_TCS_PIN                                            (DL_GPIO_PIN_2)
#define TOUCH_TCS_IOMUX                                           (IOMUX_PINCM7)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_SYSCTL_CLK_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
