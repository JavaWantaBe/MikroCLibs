#include "terminal_driver.h"

const unsigned char menu[] = "Choice A\nChoice B\nChoice C\n";

void send( unsigned char c );
unsigned char get( void );
void system_setup( void );

void main() 
{
   unsigned char ret;
   
   system_setup();
   
   /* Handle menu and print result */
   ret = term_handle_menu( menu, 1, 10, 10, 1 );
   
   term_erase_screen();
   term_send( 'A' + ret - 1 );
   
   while( 1 )
   {

   }
}


void system_setup()
{
    UART1_Init( 38400 );
    Delay_ms( 100 );
    UART1_Write_Text( "Uart Initialized\r\n" );

    term_initialise( send, get );

}

void send( unsigned char c )
{
    UART1_Write( c );
}
unsigned char get()
{
    return UART1_Read();
}