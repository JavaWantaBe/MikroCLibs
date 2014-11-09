#include "bargraph.h"

/*******************
 *   Variables
 ******************/
extern sfr sbit bar_graph_cs;
extern sfr sbit bar_graph_rst;
static uint16_t pwm_period;

#ifdef __MIKROC_PRO_FOR_ARM__
static uint8_t pwm_channel_used;
static void (* PWM_set_duty )( uint16_t duty, uint8_t inverted, uint8_t channel );
static void (* write_spi )( uint16_t data_in );
#endif

#ifdef __MIKROC_PRO_FOR_AVR__
static void (* PWM_set_duty )( unsigned short duty );
static void (* write_spi )( unsigned short data_in );
extern sfr sbit bar_graph_cs_direction;
extern sfr sbit bar_graph_rst_direction;
#endif

/********************
 *   Prototypes
 *******************/
static void pwm_init( bar_graph_t* config );
static void bar_graph_reset( void );
static void bar_graph_chip_select( void );
static void fade_in_out( void );


static void pwm_init( bar_graph_t* config )
{
    #ifdef __MIKROC_PRO_FOR_ARM__
    uint16_t frequency = 5000;

    switch( config->timer )
    {
       case 1:
           pwm_period = PWM_TIM1_Init(frequency);
           PWM_set_duty = PWM_TIM1_Set_Duty;
           PWM_TIM1_Start( config->pwm_channel, config->pwm_module );
       break;
       case 2:
           pwm_period = PWM_TIM2_Init(frequency);
           PWM_set_duty = PWM_TIM2_Set_Duty;
           PWM_TIM2_Start( config->pwm_channel, config->pwm_module );
       break;
       case 3:
           pwm_period = PWM_TIM3_Init(frequency);
           PWM_set_duty = PWM_TIM3_Set_Duty;
           PWM_TIM3_Start( config->pwm_channel, config->pwm_module );
       break;
       case 4:
           pwm_period = PWM_TIM4_Init(frequency);
           PWM_set_duty = PWM_TIM4_Set_Duty;
           PWM_TIM4_Start( config->pwm_channel, config->pwm_module );
       break;
       case 5:
           pwm_period = PWM_TIM5_Init(frequency);
           PWM_set_duty = PWM_TIM5_Set_Duty;
           PWM_TIM5_Start( config->pwm_channel, config->pwm_module );
       break;
       case 8:
           pwm_period = PWM_TIM8_Init(frequency);
           PWM_set_duty = PWM_TIM8_Set_Duty;
           PWM_TIM8_Start( config->pwm_channel, config->pwm_module );
       break;
       case 9:
           pwm_period = PWM_TIM9_Init(frequency);
           PWM_set_duty = PWM_TIM9_Set_Duty;
           PWM_TIM9_Start( config->pwm_channel, config->pwm_module );
       break;
       case 10:
           pwm_period = PWM_TIM10_Init(frequency);
           PWM_set_duty = PWM_TIM10_Set_Duty;
           PWM_TIM10_Start( config->pwm_channel, config->pwm_module );
       break;
       case 11:
           pwm_period = PWM_TIM11_Init(frequency);
           PWM_set_duty = PWM_TIM11_Set_Duty;
           PWM_TIM11_Start( config->pwm_channel, config->pwm_module );
       break;
       case 12:
           pwm_period = PWM_TIM12_Init(frequency);
           PWM_set_duty = PWM_TIM12_Set_Duty;
           PWM_TIM12_Start( config->pwm_channel, config->pwm_module );
       break;
       case 13:
           pwm_period = PWM_TIM13_Init(frequency);
           PWM_set_duty = PWM_TIM13_Set_Duty;
           PWM_TIM13_Start( config->pwm_channel, config->pwm_module );
       break;
       case 14:
           pwm_period = PWM_TIM14_Init(frequency);
           PWM_set_duty = PWM_TIM14_Set_Duty;
           PWM_TIM14_Start( config->pwm_channel, config->pwm_module );
       break;
    }
    #endif
  
    #ifdef __MIKROC_PRO_FOR_AVR__
    pwm_period = 255;

    switch( config->pwm_channel )
    {
       case 1:
           PWM1_Init( _PWM1_FAST_MODE, _PWM1_PRESCALER_8, _PWM1_NON_INVERTED, 32 );
           PWM_set_duty = PWM1_Set_Duty;
           DDB3_bit = 1;                   // Set PORTB pin 3 as output pin for the PWM (according to datasheet)
       break;
       /*case 1:
           PWM16bit_Init( _PWM16_PHASE_CORRECT_MODE_8BIT, _PWM16_PRESCALER_16bit_8, _PWM16_NON_INVERTED, 255, 1 );
           PWM_set_duty = PWM16bit_Change_Duty;
       break;*/
       case 2:
           PWM2_Init( _PWM2_FAST_MODE, _PWM2_PRESCALER_8, _PWM2_NON_INVERTED, 255 );
           PWM_set_duty = PWM2_Set_Duty;
           DDD7_bit = 1;                   // Set PORTD pin 7 as output pin for the PWM (according to datasheet)
       break;
    }
    #endif

}


static void bar_graph_reset()
{
    bar_graph_rst = 0;
    asm {nop};
    asm {nop};
    asm {nop};
    bar_graph_rst = 1;
}

static void bar_graph_chip_select()
{
    bar_graph_cs = 0;
    asm {nop};
    asm {nop};
    asm {nop};
    bar_graph_cs = 1;
}

static void fade_in_out()
{
    uint16_t current_duty;
    
    for ( current_duty = 0; current_duty < pwm_period; current_duty++ )
    {
        #ifdef __MIKROC_PRO_FOR_ARM__
        PWM_set_duty( current_duty, _PWM_NON_INVERTED, pwm_channel_used );
        Delay_10us();
        #endif
        #ifdef __MIKROC_PRO_FOR_AVR__
        PWM_set_duty( current_duty );
        Delay_10us();
        #endif
    }

    for ( current_duty = pwm_period; current_duty > 0; current_duty-- )
    {
        #ifdef __MIKROC_PRO_FOR_ARM__
        PWM_set_duty( current_duty, _PWM_NON_INVERTED, pwm_channel_used );
        Delay_10us();
        #endif
        #ifdef __MIKROC_PRO_FOR_AVR__
        PWM_set_duty( current_duty );
        Delay_10us();
        #endif
    }
}

/***************
 *  Public
 **************/
void bar_graph_init( bar_graph_t* config )
{
    pwm_init( config );
    pwm_channel_used = config->pwm_channel;

    switch( config->spi_channel)
    {
        case 1:
            write_spi = SPI1_Write;
            #ifdef __MIKROC_PRO_FOR_AVR__
            bar_graph_cs_direction = 1;
            bar_graph_rst_direction = 1;
            #endif
        break;
        #ifdef __MIKROC_PRO_FOR_ARM__
        case 2:
            write_spi = SPI2_Write;
        break;
        case 3:
            write_spi = SPI3_Write;
        break;
        #endif
    }
    
    bar_graph_reset();

}


void bar_graph_display( uint8_t digit )
{
    uint8_t temp1 = 0, temp2 = 0;
    uint16_t temp = 0;

    temp  = ( 1 << digit ) - 1;
    temp1 = ( temp & 0x00FF );
    temp2 = ( temp & 0xFF00 ) >> 8;

    write_spi( temp2 );
    write_spi( temp1 );
    bar_graph_chip_select();
}


void bar_graph_set_intensity( uint8_t percent )
{
    float period;
    char txt_buffer[50];

    period = ( ( float )percent / 100 ) * ( float )pwm_period;

    #ifdef __MIKROC_PRO_FOR_ARM__
    PWM_set_duty( ( uint16_t )period, _PWM_NON_INVERTED, pwm_channel_used );
    #endif
    #ifdef __MIKROC_PRO_FOR_AVR__
    PWM_set_duty( ( uint8_t )period );

    #endif
}