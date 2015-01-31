/* Demo for scheduler */

#include "scheduler.h"

void initTimer( void );
void task1( void );
void task2( void );


void main()
{
    uint8_t task1_id = 0;
    uint8_t task2_id = 0;
    initTimer();
    
    #ifdef __MIKROC_PRO_FOR_AVR__
    asm sei;
    UART1_Init(38400);
    #endif
    
    #ifdef __MIKROC_PRO_FOR_ARM__
    UART1_Init_Advanced( 119200, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PA9_10 );
    Delay_ms( 100 );
    EnableInterrupts();
    #endif

    UART1_Write_Text("System Startup .. ");
    
    task_scheduler_init( 100 );
    task1_id = task_add( task1, SCH_SECONDS_30 );
    task2_id = task_add( task2, SCH_SECONDS_5 );

    UART1_Write_Text( "Enabling task scheduler\r\n" );
    
    task_scheduler_start();

    while(1) 
    {
        task_dispatch();
    }
}

void task1()
{
    UART1_Write_Text("30 Seconds has passed\r\n");
}

void task2()
{
    UART1_Write_Text("5 Seconds has passed\r\n");
}


void initTimer()
{
    #ifdef __MIKROC_PRO_FOR_AVR__
    TCCR1A = 0x80;
    TCCR1B = 0x0B;
    OCR1AH = 0x30;
    OCR1AL = 0xD3;
    OCIE1A_bit = 1;
    #endif

    #ifdef __MIKROC_PRO_FOR_ARM__
    RCC_APB1ENR.TIM2EN = 1;
    TIM2_CR1.CEN = 0;
    TIM2_PSC = 2399;
    TIM2_ARR = 62500;
    NVIC_IntEnable(IVT_INT_TIM2);
    TIM2_DIER.UIE = 1;
    TIM2_CR1.CEN = 1;
    #endif
}


#ifdef __MIKROC_PRO_FOR_AVR__
void Timer1Overflow_ISR() org IVT_ADDR_TIMER1_COMPA 
{
    task_scheduler_clock();
}
#endif

#ifdef __MIKROC_PRO_FOR_ARM__
void Timer2_interrupt() iv IVT_INT_TIM2 
{
    TIM2_SR.UIF = 0;
    task_scheduler_clock();
}

#endif