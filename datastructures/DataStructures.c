#include "sstack.h"
#include "squeue.h"
#include <stdlib.h>
#include <ctype.h>

#define MAX 25
#define MIN 1

void test_sstack( void );
void test_squeue( void );

sstack_t static_stack;
squeue_t static_queue;

int s_array[MAX];
int tmpnum;
char txt[7];
int range = ( ( MAX + 1 - MIN) ) + MIN;



void main() 
{
   UART1_Init( 38400 );
   Delay_ms( 100 );
   UART_Write_Text( "Starting Stuctures Tests\r\n" );

   test_sstack();
   test_squeue();
   
   UART_Write_Text( "Tests Ended" );
}

// Tests the static stack modules
void test_sstack()
{
    int i, runs;
    
    memset( s_array, 0, MAX );

    srand( 40 );
    runs = rand() % range;

    if( sstack_init( &static_stack, MAX, sizeof( int ), s_array ) < 0 )
        return;
    
    if( sstack_full( &static_stack ) )
        UART_Write_Text( "FULL" );
    else
        UART_Write_Text( "Not Full\r\n" );
    
    for( i = 0; i < runs; i++ )
    {
        tmpnum = rand();
        IntToStr( tmpnum, txt );
        UART_Write_Text( "Pushing value: " );
        UART_Write_Text( Ltrim( txt ) );
        UART_Write_Text( "\r\n" );
        if( sstack_push( &static_stack, &tmpnum ) < 0 )
            break;
    }

    IntToStr( sstack_size( &static_stack ), txt );
    UART_Write_Text( "Final Stack Size is: " );
    UART_Write_Text( Ltrim( txt ) );
    UART_Write_Text( "\r\n" );

    while( sstack_pop( &static_stack, &tmpnum ) != -1 )
    {
        IntToStr( sstack_size( &static_stack ), txt );
        UART_Write_Text( "Stack Size: " );
        UART_Write_Text( Ltrim( txt ) );
        IntToStr( tmpnum, txt );
        UART_Write_Text( " Value: " );
        UART_Write_Text( Ltrim( txt ) );
        UART_Write_Text( "\r\n" );
    }
    
}

// Tests the static queue modules
void test_squeue()
{
    int i, j, runs, tests;
    
    memset( s_array, 0, MAX );
    srand( 41 );
    runs = rand() % range;
    tests = rand() % range;
    
    if( squeue_init( &static_queue, MAX, sizeof( int ), s_array ) < 0 )
        return;
        
    for( j = 0; j < tests; j++ )
    {
        runs = rand() % range;
        
        for( i = 0; i < runs; i++ )
        {
           tmpnum = rand();
           
           if( squeue_enqueue( &static_queue, &tmpnum ) < 0 )
               break;
               
           IntToStr( tmpnum, txt );
           UART_Write_Text( "Enqueue value: " );
           UART_Write_Text( Ltrim( txt ) );
           UART_Write_Text( "\r\n" );
        }

        runs = rand() % range;

        for( i = 0; i < runs; i++ )
        {
            if( squeue_dequeue( &static_queue, &tmpnum ) < 0 )
                break;

            IntToStr( squeue_size( &static_queue ), txt );
            UART_Write_Text( "Queue Size: " );
            UART_Write_Text( Ltrim( txt ) );
            IntToStr( tmpnum, txt );
            UART_Write_Text( " Value: " );
            UART_Write_Text( Ltrim( txt ) );
            UART_Write_Text( "\r\n" );
        }
    }
}