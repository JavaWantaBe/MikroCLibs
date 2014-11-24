#include "sstack.h"
#include <stdlib.h>
#include <ctype.h>

#define MAX 5

void test_sstack( void );

sstack_t static_stack;
int s_array[MAX];

void main() 
{
   UART1_Init( 38400 );
   Delay_ms( 100 );

   test_sstack();
   
}

void test_sstack()
{
    int tmpnum = 0;
    int result = 0;
    char txt[7];
    
    srand( 40 );
    
    if( sstack_init( &static_stack, MAX, sizeof( int ), ( void* )s_array ) < 0 )
        return;

    do
    {
       tmpnum = rand();
       result = sstack_push( &static_stack, ( void* )&tmpnum );
       
       if( result != -1 )
       {
           IntToStr( tmpnum, txt );
           UART_Write_Text( "Pushing value: " );
           UART_Write_Text( Ltrim( txt ) );
           UART_Write_Text( "\r\n" );
       }

    } while ( result == 0 );
    
    IntToStr( sstack_size( &static_stack ), txt );
    UART_Write_Text( "Final Stack Size is: " );
    UART_Write_Text( Ltrim( txt ) );
    UART_Write_Text( "\r\n" );
    
    IntToStr( *( int* )sstack_top( &static_stack ), txt );
    UART_Write_Text( "Top value is: " );
    UART_Write_Text( Ltrim( txt ) );
    UART_Write_Text( "\r\n" );
    Delay_ms( 2000 );

    do
    {
        result = sstack_pop( &static_stack, &tmpnum );
        
        if( result != -1 )
        {
            IntToStr( sstack_size( &static_stack ), txt );
            UART_Write_Text( "Stack Size: " );
            UART_Write_Text( Ltrim( txt ) );
            IntToStr( tmpnum, txt );
            UART_Write_Text( " Value: " );
            UART_Write_Text( Ltrim( txt ) );
            UART_Write_Text( "\r\n" );
        }
    } while ( result == 0 );
}