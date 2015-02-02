#include "lwprintf.h"

void system_setup( void );

void system_setup()
{
    UART1_Init( 38400 );
    Delay_ms( 100 );
    printfInit( ( void(*)( char c ) )UART1_Write );
}


void main() 
{
    int x = 12;
    system_setup();
    printf( "Hello World = %d\r\n", x );
    
    while( 1 )
    {

    }
}