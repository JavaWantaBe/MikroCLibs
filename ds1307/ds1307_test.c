#include "ds1307.h"
#include <stdbool.h>

#define STM32

void sys_init( void );

void sys_init()
{
    ds1307_config_t conf;
    conf.time_zone = -8;
    conf.dst_enable = TRUE;
    conf.config = 0;
    
    ds1307_init( &conf );

    UART1_Init_Advanced( 115200, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10 );
    Delay_ms( 100 );
    UART1_Write_Text( "System Initialized\r\n" );

}

void main() 
{
    while( RCC_CR & ( ( 1 << PLLRDY ) | ( 1 << HSERDY ) ) == 0 );
    I2C1_Init_Advanced( 100000, &_GPIO_MODULE_I2C1_PB67 );
    Delay_ms( 1000 );
    
    sys_init();
    
    UART1_Write_Text( ds1307_get_local_time_str( MODE24HOUR ) );
    

}