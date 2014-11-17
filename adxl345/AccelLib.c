#include "adxl345.h"

void main() 
{
    int x, y, z;
    
    UART1_Init( 9600 );
    TWI_Init( 100000 );
    Delay_ms( 100 );
    
    adxl345_init( );
    
    //set activity/ inactivity thresholds (0-255)
    adxl345_setActivityThreshold(75); //62.5mg per increment
    adxl345_setInactivityThreshold(75); //62.5mg per increment
    adxl345_setTimeInactivity(10); // how many seconds of no activity is inactive?

    //look of activity movement on this axes - 1 == on; 0 == off
    adxl345_setActivityX(1);
    adxl345_setActivityY(1);
    adxl345_setActivityZ(1);

    //look of inactivity movement on this axes - 1 == on; 0 == off
    adxl345_setInactivityX(1);
    adxl345_setInactivityY(1);
    adxl345_setInactivityZ(1);

    //look of tap movement on this axes - 1 == on; 0 == off
    adxl345_setTapDetectionOnX(0);
    adxl345_setTapDetectionOnY(0);
    adxl345_setTapDetectionOnZ(1);

    //set values for what is a tap, and what is a double tap (0-255)
    adxl345_setTapThreshold(50); //62.5mg per increment
    adxl345_setTapDuration(15); //625\u03bcs per increment
    adxl345_setDoubleTapLatency(80); //1.25ms per increment
    adxl345_setDoubleTapWindow(200); //1.25ms per increment

    //set values for what is considered freefall (0-255)
    adxl345_setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
    adxl345_setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment

    //setting all interupts to take place on int pin 1
    //I had issues with int pin 2, was unable to reset it
    adxl345_setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT, ADXL345_INT1_PIN );
    adxl345_setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT, ADXL345_INT1_PIN );
    adxl345_setInterruptMapping( ADXL345_INT_FREE_FALL_BIT, ADXL345_INT1_PIN );
    adxl345_setInterruptMapping( ADXL345_INT_ACTIVITY_BIT, ADXL345_INT1_PIN );
    adxl345_setInterruptMapping( ADXL345_INT_INACTIVITY_BIT, ADXL345_INT1_PIN );

    //register interupt actions - 1 == on; 0 == off
    adxl345_setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
    adxl345_setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
    adxl345_setInterrupt( ADXL345_INT_FREE_FALL_BIT, 1);
    adxl345_setInterrupt( ADXL345_INT_ACTIVITY_BIT, 1);
    adxl345_setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);

    while( 1 )
    {
        //Boring accelerometer stuff
        adxl345_readAccel( &x, &y, &z ); //read the accelerometer values and store them in variables x,y,z
    }
}