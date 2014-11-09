/**
 * @file bargraph.h
 *
 * @brief Multiplexed LED bargraph library for Bargraph Click
 *
 * @author Richard Lowe
 * @copyright AlphaLoewe
 *
 * @date 08/11/2014
 *
 * @version .1 - Initiaial with support for ARM and AVR
 *
 * @details
 *
 * Status: <XX% completed.>
 *
 * \note
 * Test configuration:
 *   MCU:             %DEVICE%
 *   Dev.Board:       x
 *   Oscillator:      %DEVICE_CLOCK%
 *   Ext. Modules:    x
 *   SW:              %COMPILER%
 *
 * \par
 *   <all that matters>
 */

#ifndef _BARGRAPH_H
#define _BARGRAPH_H

#include "stddef.h"
#include "stdint.h"

/**
 * @struct Members needed for system initialization
 */
#ifdef __MIKROC_PRO_FOR_ARM__
typedef struct
{
    uint8_t spi_channel;
    uint8_t timer;
    uint8_t pwm_channel;
    const void* pwm_module;
} bar_graph_t;
#endif

#ifdef __MIKROC_PRO_FOR_AVR__
typedef struct
{
    uint8_t spi_channel;
    uint8_t pwm_channel;
} bar_graph_t;
#endif

/**
 *  @brief Graph SPI initialization
 *
 *  @param bar_graph_t* config - struct passed to init
 *
 */
void bar_graph_init( bar_graph_t* config );

/**
 *  @brief Displays the digit passed to it
 *
 *  @pre <Preconditions that need to be met before called>
 *
 *  @param <Parameters Accepted>
 *
 *  @return <Returns>
 *    @retval <Values that might be returned>
 *
 *  @note
 *   <notes>
 */
void bar_graph_display( uint8_t digit );

/**
 *  @brief <Basic Description>
 *
 *  @pre <Preconditions that need to be met before called>
 *
 *  @param <Parameters Accepted>
 *
 *  @return <Returns>
 *    @retval <Values that might be returned>
 *
 *  @note
 *   <notes>
 */
void bar_graph_set_intensity( uint8_t percent );

#endif