/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_SYSCTL_CLK_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initDigitalOutput(LCD_GPIO_RS_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_GPIO_WR_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_GPIO_RD_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_GPIO_CS_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_GPIO_RST_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D0_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D1_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D2_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D3_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D4_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D5_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D6_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D7_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D8_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D9_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D10_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D11_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D12_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D13_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D14_IOMUX);

    DL_GPIO_initDigitalOutput(LCD_Data_D15_IOMUX);

    DL_GPIO_initDigitalOutput(TOUCH_SDA_IOMUX);

    DL_GPIO_initDigitalOutput(TOUCH_SCL_IOMUX);

    DL_GPIO_initDigitalInputFeatures(TOUCH_PEN_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalOutput(TOUCH_TCS_IOMUX);

    DL_GPIO_clearPins(GPIOA, LCD_GPIO_RS_PIN |
		LCD_GPIO_WR_PIN |
		LCD_GPIO_RD_PIN |
		LCD_GPIO_CS_PIN |
		LCD_GPIO_RST_PIN |
		TOUCH_SDA_PIN |
		TOUCH_SCL_PIN |
		TOUCH_TCS_PIN);
    DL_GPIO_enableOutput(GPIOA, LCD_GPIO_RS_PIN |
		LCD_GPIO_WR_PIN |
		LCD_GPIO_RD_PIN |
		LCD_GPIO_CS_PIN |
		LCD_GPIO_RST_PIN |
		TOUCH_SDA_PIN |
		TOUCH_SCL_PIN |
		TOUCH_TCS_PIN);
    DL_GPIO_clearPins(LCD_Data_PORT, LCD_Data_D0_PIN |
		LCD_Data_D1_PIN |
		LCD_Data_D2_PIN |
		LCD_Data_D3_PIN |
		LCD_Data_D4_PIN |
		LCD_Data_D5_PIN |
		LCD_Data_D6_PIN |
		LCD_Data_D7_PIN |
		LCD_Data_D8_PIN |
		LCD_Data_D9_PIN |
		LCD_Data_D10_PIN |
		LCD_Data_D11_PIN |
		LCD_Data_D12_PIN |
		LCD_Data_D13_PIN |
		LCD_Data_D14_PIN |
		LCD_Data_D15_PIN);
    DL_GPIO_enableOutput(LCD_Data_PORT, LCD_Data_D0_PIN |
		LCD_Data_D1_PIN |
		LCD_Data_D2_PIN |
		LCD_Data_D3_PIN |
		LCD_Data_D4_PIN |
		LCD_Data_D5_PIN |
		LCD_Data_D6_PIN |
		LCD_Data_D7_PIN |
		LCD_Data_D8_PIN |
		LCD_Data_D9_PIN |
		LCD_Data_D10_PIN |
		LCD_Data_D11_PIN |
		LCD_Data_D12_PIN |
		LCD_Data_D13_PIN |
		LCD_Data_D14_PIN |
		LCD_Data_D15_PIN);

}


SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}
SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_CLK_init(void) {
    while ((DL_SYSCTL_getClockStatus() & (DL_SYSCTL_CLK_STATUS_LFOSC_GOOD))
	       != (DL_SYSCTL_CLK_STATUS_LFOSC_GOOD))
	{
		/* Ensure that clocks are in default POR configuration before initialization.
		* Additionally once LFXT is enabled, the internal LFOSC is disabled, and cannot
		* be re-enabled other than by executing a BOOTRST. */
		;
	}
}



