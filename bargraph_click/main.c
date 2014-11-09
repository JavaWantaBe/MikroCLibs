/**
 * @file main.c
 *
 * @brief Test unit for bargraph library
 *
 * @author Richard Lowe
 * @copyright AlphaLoewe
 *
 * @date 08/11/2014
 *
 * @version 1 - Initial test
 *
 * @details
 *
 * Status: 50% - Added pwm ability to ARM and AVR chips
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

#include "bargraph.h"

#ifdef __MIKROC_PRO_FOR_ARM__
sbit bar_graph_cs            at GPIOD_ODR.B13;
sbit bar_graph_rst           at GPIOC_ODR.B2;
#endif

#ifdef __MIKROC_PRO_FOR_AVR__
sbit bar_graph_cs            at PORTB4_bit;
sbit bar_graph_cs_direction  at DDB4_bit;
sbit bar_graph_rst           at PORTB0_bit;
sbit bar_graph_rst_direction at DDB0_bit;
#endif



void main() {

     bar_graph_t bg_config = {0};
     uint8_t i = 1;
     float intensity = 0.0f;

     #ifdef __MIKROC_PRO_FOR_ARM__
     GPIO_Digital_Output( &GPIOD_BASE, _GPIO_PINMASK_13 );
     GPIO_Digital_Output( &GPIOD_BASE, _GPIO_PINMASK_2 );
     
     SPI3_Init_Advanced( _SPI_FPCLK_DIV64,
                         _SPI_MASTER | _SPI_8_BIT | _SPI_CLK_IDLE_LOW |
                         _SPI_FIRST_CLK_EDGE_TRANSITION | _SPI_MSB_FIRST |
                         _SPI_SS_DISABLE | _SPI_SSM_ENABLE | _SPI_SSI_1,
                         &_GPIO_MODULE_SPI3_PC10_11_12 );
                         
     bg_config.spi_channel = 3;
     bg_config.timer = 2;
     bg_config.pwm_channel = _PWM_CHANNEL1;
     bg_config.pwm_module = &_GPIO_MODULE_TIM2_CH1_PA0;
     #endif

     #ifdef __MIKROC_PRO_FOR_AVR__
     SPI1_Init();
     ADC_Init();
     bg_config.spi_channel = 1;
     bg_config.pwm_channel = 1;
     #endif

     bar_graph_init( &bg_config );
     bar_graph_display( 10 );

     while(1)
     {
         //intensity = ( ( ( float )ADC_Read( 6 ) ) / 1023f ) * 100f ;
         Delay_ms( 100 );
         bar_graph_display(i);
         bar_graph_set_intensity( ( uint8_t )intensity );
         Delay_ms( 50 );

         if( i++ == 10 )
         {
            i = 1;
         }
     }
}