#include "sstack.h"
#include "squeue.h"
#include "heap.h"
#include <stdlib.h>
#include <ctype.h>

#define MAX 12
#define MIN 1

void test_sstack( void );
void test_squeue( void );
void test_heap( void );
int compare( void* const key, void* const key2 );

sstack_t stack;
squeue_t static_queue;
heap_t my_heap;

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
   //test_heap();
   
   UART_Write_Text( "Tests Ended" );
}

// Tests the static stack modules
void test_sstack()
{
    int i, runs;
    
    memset( s_array, 0, MAX * sizeof( int ) );

    srand( 40 );
    runs = rand() % range;

    if( sstack_init( &stack, MAX, sizeof( int ), s_array ) < 0 )
        return;
    
    if( sstack_full( &stack ) )
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
        
        if( sstack_push( &stack, &tmpnum ) < 0 )
            break;
    }

    IntToStr( sstack_size( &stack ), txt );
    UART_Write_Text( "Final Stack Size is: " );
    UART_Write_Text( Ltrim( txt ) );
    UART_Write_Text( "\r\n" );
    
    //Delay_ms( 2000 );

    while( sstack_pop( &stack, &tmpnum ) != -1 )
    {
        IntToStr( sstack_size( &stack ), txt );
        UART_Write_Text( "Stack Size: " );
        UART_Write_Text( Ltrim( txt ) );
        IntToStr( tmpnum, txt );
        UART_Write_Text( " Value: " );
        UART_Write_Text( Ltrim( txt ) );
        UART_Write_Text( "\r\n" );
        //Delay_ms( 250 );
    }
    
    IntToStr( sstack_size( &stack ), txt );
    UART_Write_Text( "Final Stack Size is: " );
    UART_Write_Text( Ltrim( txt ) );
    UART_Write_Text( "\r\n" );
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

void test_heap()
{
    if( heap_init( &my_heap, MAX, sizeof( int ), compare, s_array ) < 0 )
    {
        UART_Write_Text( "Something off with heap init\r\n" );
        return;
    }

}

int compare( void* const key, void* const key2 )
{
    if( *( int* )key > *( int* )key2 )
        return 1;
    else
        return -1;
}