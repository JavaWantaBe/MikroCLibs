#include "ds1307.h"
#include <stdbool.h>
#include <stddef.h>

#define msg( txt ) UART1_Write_Text( txt )

// LCD module connections
sbit LCD_RS at PORTA2_bit;
sbit LCD_EN at PORTD6_bit;
sbit LCD_D4 at PORTC4_bit;
sbit LCD_D5 at PORTC5_bit;
sbit LCD_D6 at PORTC6_bit;
sbit LCD_D7 at PORTC7_bit;

sbit LCD_RS_Direction at DDA2_bit;
sbit LCD_EN_Direction at DDD6_bit;
sbit LCD_D4_Direction at DDC4_bit;
sbit LCD_D5_Direction at DDC5_bit;
sbit LCD_D6_Direction at DDC6_bit;
sbit LCD_D7_Direction at DDC7_bit;
// End LCD module connections

const char character[] = {0,0,0,14,14,14,14,0,0,0,0};

void setup_rtc( void );




void setup_rtc()
{
    TimeStruct ts1;
    
    ds1307_config_t rtc;
    rtc.time_zone = -8;
    rtc.dst_enable = true;
    rtc.output_config = DS1307_SQW_1HZ;
    rtc.mode_24hour = true;

    UART1_Init( 38400 );
    Delay_ms( 100 );
    msg( "Start\r\n" );

    TWI_Init(100000);
    Delay_ms(1000);

    ds1307_init( &rtc );

    /*
    ts1.yy = 2014;
    ts1.mo = 1;
    ts1.md = 26;
    ts1.hh = 17;
    ts1.mn = 49;

    ds1307_set_time_GMT_ts( &ts1 );
    */
}


void main() 
{
     setup_rtc();
     
     MCUCR |= ( 1<<ISC01 ); // The falling edge of INT0 generates an interrupt request
     GICR  |= ( 1<<INT0 ); // External Interrupt Request 0 Enable

     SREG_I_bit = 1;

     Lcd_Init();
     Lcd_Cmd( _LCD_CURSOR_OFF );
     Lcd_Cmd( _LCD_CLEAR );

     while(1)
     {
       Lcd_Out( 1, 1, ds1307_get_local_time_str( MODE24HOUR ) );
       Lcd_Out( 2, 1, ds1307_get_http_gmt_str() );
       msg( "Local Time: " );
       msg( ds1307_get_local_time_str( MODE24HOUR ) );
       msg( "\r\n" );
       msg( "GMT Time: " );
       msg( ds1307_get_GMT_time_str( MODE24HOUR ) );
       msg( "\r\n" );
       msg( "HTTP Time: " );
       msg( ds1307_get_http_gmt_str() );
       msg( "\r\n" );
       Delay_ms( 2000 );
     }
}

void rtc_sw_ISR() iv IVT_ADDR_INT0 ics ICS_AUTO {
    
    char i;
    
    Lcd_Cmd( 64 );
    
    for (i = 0; i<=10; i++)
    {
        Lcd_Chr_CP(character[i]);
    }
    
    Lcd_Cmd(_LCD_RETURN_HOME);
    Lcd_Chr( 2, 16, 0);
}