/** @file hal_nrf.h
 * Header file defining the hardware dependent interface
 * for MikroMedia Plus
 *
 */

#ifndef HAL_NRF_HW_H
#define HAL_NRF_HW_H

#ifdef __MIKROE_PRO_FOR_AVR__
// TODO: Setup pins for AVR
#endif
#ifdef __MIKROC_PRO_FOR_ARM__
// TODO: Need to change these to bitbanding for performance
extern sbit sfr radio_chip_select;          // CSN PIN for BFB
extern sbit sfr radio_chip_enable;          // CE PIN
#endif

/**
 * Macro that set radio's Chip Select line LOW.
 *
 */
#define chip_select_low() radio_chip_select = 0


/**
 * Macro that set radio's Chip Select line HIGH.
 *
 */
#define chip_select_high() radio_chip_select = 1

/**
 * Macro that set radio's Chip Enable line LOW.
 *
 */
#define chip_enable_low() radio_chip_enable = 0

/**
 * Macro that set radio's Chip Enable line HIGH.
 *
 */
#define chip_enable_high() radio_chip_enable = 1

/**
 * Pulses the Chip Enable to nRF24L01 for at least 10 us
 */
#define chip_enable_pulse() chip_enable_high(); Delay_10us(); chip_enable_low()

#endif 

/* HAL_NRF_HW_H__ */