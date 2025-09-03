/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_STM32
#include "PeripheralConfiguration_STM32.h"
#ifdef KLST_PERIPHERAL_ENABLE_AUDIODEVICE

#include <cstdio>
#include <cstdint>
#include "main.h"

#include "Console.h"
#include "WM8904Configure.h"
#include "WM8904.h"

#ifdef __cplusplus
extern "C" {
#endif

static void delay_ms(uint32_t duration) {
    HAL_Delay(duration);
}

static void set_lineout_volume(uint8_t volume_left, uint8_t volume_right) {
    WM8904_write_register(WM8904_R59_ANALOGUE_OUT2_LEFT, WM8904_LINEOUT_VU | WM8904_LINEOUTL_VOL(volume_left));
    WM8904_write_register(WM8904_R60_ANALOGUE_OUT2_RIGHT, WM8904_LINEOUT_VU | WM8904_LINEOUTR_VOL(volume_right));
}

static void set_headphone_volume(uint8_t volume_left, uint8_t volume_right) {
    WM8904_write_register(WM8904_R57_ANALOGUE_OUT1_LEFT, WM8904_HPOUT_VU | WM8904_HPOUTL_VOL(volume_left));
    WM8904_write_register(WM8904_R58_ANALOGUE_OUT1_RIGHT, WM8904_HPOUT_VU | WM8904_HPOUTR_VOL(volume_right));
}

static void setup_manually() {
    /* --- POWER ---------------------------------------------------------------------------------------------------- */
    WM8904_write_register(WM8904_BIAS_CONTROL_0, WM8904_ISEL_HP_BIAS);
    WM8904_write_register(WM8904_VMID_CONTROL_0, WM8904_VMID_BUF_ENA | WM8904_VMID_RES_FAST | WM8904_VMID_ENA);
    delay_ms(5);
    WM8904_write_register(WM8904_VMID_CONTROL_0, WM8904_VMID_BUF_ENA | WM8904_VMID_RES_NORMAL | WM8904_VMID_ENA);
    WM8904_write_register(WM8904_BIAS_CONTROL_0, WM8904_ISEL_HP_BIAS | WM8904_BIAS_ENA);
    WM8904_write_register(WM8904_POWER_MANAGEMENT_0, WM8904_INL_ENA | WM8904_INR_ENA);
    WM8904_write_register(WM8904_POWER_MANAGEMENT_2, WM8904_HPL_PGA_ENA | WM8904_HPR_PGA_ENA);
    WM8904_write_register(WM8904_DAC_DIGITAL_1, WM8904_DEEMPH(0));
    WM8904_write_register(WM8904_ANALOGUE_OUT12_ZC, 0x0000);
    WM8904_write_register(WM8904_CHARGE_PUMP_0, WM8904_CP_ENA);
    WM8904_write_register(WM8904_CLASS_W_0, WM8904_CP_DYN_PWR);

    /* --- AUDIO_INTERFACE ------------------------------------------------------------------------------------------ */

#ifdef TEST_WM8904_SET
    KLST_BSP_serialdebug_println("TEST register values below should not be the same");
    print("WM8904_R25_AUDIO_INTERFACE_1: ");
    console_binary(WM8904_read_register(WM8904_R25_AUDIO_INTERFACE_1), 16); // TEST
#endif

    WM8904_write_register(WM8904_R25_AUDIO_INTERFACE_1, WM8904_AIF_WL_16BIT | WM8904_AIF_FMT_I2S);
    WM8904_write_register(WM8904_R26_AUDIO_INTERFACE_2, 0);
    WM8904_write_register(WM8904_R27_AUDIO_INTERFACE_3, 0);
    WM8904_write_register(WM8904_R18_WM8904_POWER_MANAGEMENT_6,
                          WM8904_DACL_ENA |
                              WM8904_DACR_ENA |
                              WM8904_ADCL_ENA |
                              WM8904_ADCR_ENA);
    delay_ms(5);

    /* --- INPUT_OUTPUT --------------------------------------------------------------------------------------------- */

    WM8904_write_register(WM8904_ANALOGUE_LEFT_INPUT_0, WM8904_LIN_VOL(0x10));
    WM8904_write_register(WM8904_ANALOGUE_RIGHT_INPUT_0, WM8904_RIN_VOL(0x10));
    WM8904_write_register(WM8904_R67_DC_SERVO_0,
                          WM8904_DCS_ENA_CHAN_3 |
                              WM8904_DCS_ENA_CHAN_2 |
                              WM8904_DCS_ENA_CHAN_1 |
                              WM8904_DCS_ENA_CHAN_0);
    WM8904_write_register(WM8904_R68_DC_SERVO_1,
                          WM8904_DCS_TRIG_STARTUP_3 |
                              WM8904_DCS_TRIG_STARTUP_2 |
                              WM8904_DCS_TRIG_STARTUP_1 |
                              WM8904_DCS_TRIG_STARTUP_0);
    delay_ms(100);

    /* --- LINE_OUT ------------------------------------------------------------------------------------------------- */

    WM8904_write_register(WM8904_R94_WM8904_ANALOGUE_LINEOUT_0,
                          WM8904_LINEOUTL_ENA_OUTP |
                              WM8904_LINEOUTL_ENA_DLY |
                              WM8904_LINEOUTL_ENA |
                              WM8904_LINEOUTR_ENA_OUTP |
                              WM8904_LINEOUTR_ENA_DLY |
                              WM8904_LINEOUTR_ENA);
    set_lineout_volume(0x30, 0x30); // TODO check if this is 80% ( with max 0x40 )
    delay_ms(100);

    /* --- HEADPHONES ----------------------------------------------------------------------------------------------- */

    WM8904_write_register(WM8904_R90_WM8904_ANALOGUE_HP_0,
                          WM8904_HPL_ENA_OUTP |
                              WM8904_HPL_ENA_DLY |
                              WM8904_HPL_ENA |
                              WM8904_HPR_ENA_OUTP |
                              WM8904_HPR_ENA_DLY |
                              WM8904_HPR_ENA);
    set_headphone_volume(0x30, 0x30);
    delay_ms(100);

#ifdef TEST_WM8904_SET
    print("WM8904_R25_AUDIO_INTERFACE_1: ");
    console_binary(WM8904_read_register(WM8904_R25_AUDIO_INTERFACE_1), 16); // TEST
#endif
}

static void setup_default_start_sequence() {
    // SYSCLK required for write sequencer to run, SYSCLK_SRC = 0 (system clock comes from MCLK, not from FLL)
    WM8904_write_register(WM8904_R22_CLOCK_RATES_2, WM8904_CLK_SYS_ENA);

    // use CONTROL WRITE SEQUENCER (p122ff)
    // when done writing it asserts the WSEQ_EINT flag in Register R127
    // R112 (70h) Write Sequencer 4, WSEQ_BUSY: 0 = Sequencer idle, 1 = Sequencer busy
    // "The Start-up sequence is initiated by writing 0100h to Register R111 (6Fh)"

    /* using the default startup sequence (p131) */
    //                                                          (R108 > 0100h) - This enables the Write Sequencer
    WM8904_write_register(WM8904_R108_WM8904_WRITE_SEQUENCER_0, WM8904_WSEQ_ENA);
    //                                                          (R111 > 0100h) - This starts the Write Sequencer at Index address 0 (00h)
    WM8904_write_register(WM8904_R111_WM8904_WRITE_SEQUENCER_3, WM8904_WSEQ_START);

#ifdef TEST_WM8904_SET
    KLST_BSP_serialdebug_println("starting WM8904_WSEQ_START");
    KLST_BSP_serialdebug_println("TEST register values below should not be the same");
    print("WM8904_R5_VMID_CONTROL_0: ");
    console_binary(WM8904_read_register(WM8904_R5_VMID_CONTROL_0), 16); // TEST
#endif
    /* "... this sequence takes approximately 300ms to run." ( at 12.288MHz ) */
    delay_ms(333);
#ifdef TEST_WM8904_SET
    print("WM8904_R5_VMID_CONTROL_0: ");
    console_binary(WM8904_read_register(WM8904_R5_VMID_CONTROL_0), 16); // TEST
    KLST_BSP_serialdebug_println("finished WM8904_WSEQ_START");
#endif
}

static void setup_SCLK_FLL() {
    //                                              select SYSCLK / fs ratio = 12288000Hz/48000Hz = 256 = 0b0011 = 3
    //                                              select Sample Rate (fs) = 44.1kHz, 48kHz = 0b101 = 5
    WM8904_write_register(WM8904_R21_CLOCK_RATES_1, WM8904_CLK_SYS_RATE(3) | WM8904_SAMPLE_RATE(5));
    //                                              0 = SYSCLK = MCLK
    WM8904_write_register(WM8904_R20_CLOCK_RATES_0, 0x0000);
    //                                              SYSCLK Source Select to FLL output
    //                                              System Clock enable
    //                                              DSP Clock enable
    WM8904_write_register(WM8904_R22_CLOCK_RATES_2, WM8904_SYSCLK_SRC | WM8904_CLK_SYS_ENA | WM8904_CLK_DSP_ENA);
}

static void setup_SCLK_MCLK() {
    //                                              select SYSCLK / fs ratio = 12288000Hz/48000Hz = 256 = 0b0011 = 3
    //                                              select Sample Rate (fs) = 44.1kHz, 48kHz = 0b101 = 5
    WM8904_write_register(WM8904_R21_CLOCK_RATES_1, WM8904_CLK_SYS_RATE(3) | WM8904_SAMPLE_RATE(5));
    //                                              0 = SYSCLK = MCLK
    WM8904_write_register(WM8904_R20_CLOCK_RATES_0, 0x0000);
    //                                              DSP Clock enable
    //                                              System Clock enable
    //                                              ( SYSCLK Source is MCLK )
    WM8904_write_register(WM8904_R22_CLOCK_RATES_2, WM8904_CLK_DSP_ENA | WM8904_CLK_SYS_ENA);
}

/**
 * see "FREQUENCY LOCKED LOOP (FLL)" (p104ff)
 *
 * The FLL operating frequency, FVCO is set according to the following equation:
 * FVCO = (FREF x N.K x FLL_FRATIO) ( "FVCO must be in the range 90-100 MHz" )
 *
 * FREF = 12500000Hz
 * FLL_FRATIO = 1MHz - 13.5MHz = 0h (divide by 1) = 0
 *
 * The FLL output frequency is generated according to the following equation:
 * FOUT = (FVCO / FLL_OUTDIV)
 * FLL_OUTDIV = 11.25 MHz - 12.5 MHz = 000111 (divide by 8) = 7
 *
 * FVCO = (FOUT x FLL_OUTDIV) = 12288000 * 8 = 98304000Hz
 * N.K  = FVCO / (FLL_FRATIO x FREF) = 98304000Hz / ( 1 * 12500000Hz ) = 7.86432
 * K    = 0.86432 * 65536 = 56644.07552 = 56644
 * N    = 7
 */
static void setup_FLL() {
    //                                               FLL Disable
    WM8904_write_register(WM8904_R116_FLL_CONTROL_1, 0x0000);
    //                                               FLL FOUT clock divider: 7=divide by 8
    //                                               FVCO clock divider:     0=divide by 1 TODO(not sure about this `4`)
    WM8904_write_register(WM8904_R117_FLL_CONTROL_2, WM8904_FLL_OUTDIV(7) | WM8904_FLL_FRATIO(0));
    //                                               Fractional multiply for FREF
    WM8904_write_register(WM8904_R118_FLL_CONTROL_3, WM8904_FLL_K(56644));
    //                                               Integer multiply for FREF
    WM8904_write_register(WM8904_R119_FLL_CONTROL_4, WM8904_FLL_N(7));
    //                                               FLL Clock source: 0 = MCLK
    //                                               Frequency of the FLL: 0 = FVCO / 1
    //                                               FLL Clock Reference Divider: 0 = MCLK / 1 "... bring the reference down to 13.5MHz or below."
    WM8904_write_register(WM8904_R120_FLL_CONTROL_5, WM8904_FLL_CLK_REF_SRC(0) | WM8904_FLL_CTRL_RATE(0) | WM8904_FLL_CLK_REF_DIV(0));
    //                                               FLL Fractional enable
    //                                               FLL Enable
    WM8904_write_register(WM8904_R116_FLL_CONTROL_1, WM8904_FLL_FRACN_ENA | WM8904_FLL_ENA);
    delay_ms(5);
}

void WM8904_configure(bool use_FLL, bool use_start_sequence) {
    console_status("configuring WM8904 at I2C 0x%02X", WM8904_I2C_ADDRESS);
    if (use_FLL) {
        setup_FLL();
    }
    delay_ms(50);
    if (use_start_sequence) {
        setup_default_start_sequence();
    } else {
        setup_manually();
    }
    delay_ms(50);
    if (use_FLL) {
        setup_SCLK_FLL();
    } else {
        setup_SCLK_MCLK();
    }

    //    setup_default_start_sequence(); // TODO investigate why this is necessary to generate decent volume ( albeit DC offset?!? )

    console_status("OK.");
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PERIPHERAL_ENABLE_AUDIODEVICE
#endif // KLST_ARCH_IS_STM32