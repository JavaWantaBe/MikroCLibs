/**
 * @file adxl345.h
 *
 * @brief Accelerometer ADXL345 Driver
 *
 *  This drive was adapted from the arduino library for the same sensor.  It
 *  supports both SPI and I2C bus communications as well as full featured
 *  access to the registers.
 *
 * @author Richard Lowe
 * @copyright AlphaLoewe
 *
 * @date 16/11/2014
 *
 * @version 1.0 - Initial
 *
 * @details
 *
 * Status: <XX% completed.>
 *
 * \note
 * Test configuration:
 *   MCU:             %DEVICE%
 *   Dev.Board:       x
 *   Oscillator:      %DEVICE_CLOCK%
 *   Ext. Modules:    x
 *   SW:              %COMPILER%
 *
 * \par
 *   <all that matters>
 */

#ifndef ADXL345_H
#define ADXL345_H


#include <stdbool.h>

/* ------- Register names ------- */
#define ADXL345_DEVID 0x00
#define ADXL345_RESERVED1 0x01
#define ADXL345_THRESH_TAP 0x1d
#define ADXL345_OFSX 0x1e
#define ADXL345_OFSY 0x1f
#define ADXL345_OFSZ 0x20
#define ADXL345_DUR 0x21
#define ADXL345_LATENT 0x22
#define ADXL345_WINDOW 0x23
#define ADXL345_THRESH_ACT 0x24
#define ADXL345_THRESH_INACT 0x25
#define ADXL345_TIME_INACT 0x26
#define ADXL345_ACT_INACT_CTL 0x27
#define ADXL345_THRESH_FF 0x28
#define ADXL345_TIME_FF 0x29
#define ADXL345_TAP_AXES 0x2a
#define ADXL345_ACT_TAP_STATUS 0x2b
#define ADXL345_BW_RATE 0x2c
#define ADXL345_POWER_CTL 0x2d
#define ADXL345_INT_ENABLE 0x2e
#define ADXL345_INT_MAP 0x2f
#define ADXL345_INT_SOURCE 0x30
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32
#define ADXL345_DATAX1 0x33
#define ADXL345_DATAY0 0x34
#define ADXL345_DATAY1 0x35
#define ADXL345_DATAZ0 0x36
#define ADXL345_DATAZ1 0x37
#define ADXL345_FIFO_CTL 0x38
#define ADXL345_FIFO_STATUS 0x39

#define ADXL345_BW_1600 0xF // 1111
#define ADXL345_BW_800 0xE  // 1110
#define ADXL345_BW_400 0xD  // 1101
#define ADXL345_BW_200 0xC  // 1100
#define ADXL345_BW_100 0xB  // 1011
#define ADXL345_BW_50  0xA  // 1010
#define ADXL345_BW_25  0x9  // 1001
#define ADXL345_BW_12  0x8  // 1000
#define ADXL345_BW_6  0x7   // 0111
#define ADXL345_BW_3  0x6   // 0110


/**
Interrupt PINs
INT1: 0
INT2: 1
*/
#define ADXL345_INT1_PIN 0x00
#define ADXL345_INT2_PIN 0x01

/*
Interrupt bit position
*/
#define ADXL345_INT_DATA_READY_BIT 0x07
#define ADXL345_INT_SINGLE_TAP_BIT 0x06
#define ADXL345_INT_DOUBLE_TAP_BIT 0x05
#define ADXL345_INT_ACTIVITY_BIT  0x04
#define ADXL345_INT_INACTIVITY_BIT 0x03
#define ADXL345_INT_FREE_FALL_BIT 0x02
#define ADXL345_INT_WATERMARK_BIT 0x01
#define ADXL345_INT_OVERRUNY_BIT  0x00

#define ADXL345_DATA_READY 0x07
#define ADXL345_SINGLE_TAP 0x06
#define ADXL345_DOUBLE_TAP 0x05
#define ADXL345_ACTIVITY  0x04
#define ADXL345_INACTIVITY 0x03
#define ADXL345_FREE_FALL 0x02
#define ADXL345_WATERMARK 0x01
#define ADXL345_OVERRUNY  0x00


#define ADXL345_OK  1        // no error
#define ADXL345_ERROR 0      // indicates error is predent

#define ADXL345_NO_ERROR  0  // initial state
#define ADXL345_READ_ERROR 1 // problem reading accel
#define ADXL345_BAD_ARG  2   // bad method argument


/**
 *  @brief Initializes the Sensor
 *
 *  @pre TWI_Init needs to be called before.
 */
void adxl345_init( void ( *print )( char *s ) );

void adxl345_readAccelxyz( int* xyx );
void adxl345_readAccel( int* x, int* y, int* z );
void adxl345_get_Gxyz( double *xyz );

// Thresholds and Gains
void adxl345_setTapThreshold( int tapThreshold );
int adxl345_getTapThreshold( void );
void adxl345_setAxisGains( double *_gains );
void adxl345_getAxisGains( double *_gains );
void adxl345_setAxisOffset( int x, int y, int z );
void adxl345_getAxisOffset( int* x, int* y, int*z );
void adxl345_setTapDuration( int tapDuration );
int adxl345_getTapDuration( void );
void adxl345_setDoubleTapLatency( int doubleTapLatency );
int adxl345_getDoubleTapLatency( void );
void adxl345_setDoubleTapWindow( int doubleTapWindow );
int adxl345_getDoubleTapWindow( void );
void adxl345_setActivityThreshold( int activityThreshold );
int adxl345_getActivityThreshold( void );
void adxl345_setInactivityThreshold( int inactivityThreshold );
int adxl345_getInactivityThreshold( void );
void adxl345_setTimeInactivity( int timeInactivity );
int adxl345_getTimeInactivity( void );
void adxl345_setFreeFallThreshold( int freeFallthreshold );
int adxl345_getFreeFallThreshold( void );
void adxl345_setFreeFallDuration( int freeFallDuration );
int adxl345_getFreeFallDuration( void );


bool adxl345_isActivityXEnabled( void );
bool adxl345_isActivityYEnabled( void );
bool adxl345_isActivityZEnabled( void );
bool adxl345_isInactivityXEnabled( void );
bool adxl345_isInactivityYEnabled( void );
bool adxl345_isInactivityZEnabled( void );
bool adxl345_isActivityAc( void );
bool adxl345_isInactivityAc( void );
void adxl345_setActivityAc( bool state );
void adxl345_setInactivityAc( bool state );


bool adxl345_getSuppressBit( void );
void adxl345_setSuppressBit( bool state );
bool adxl345_isTapDetectionOnX( void );
void adxl345_setTapDetectionOnX( bool state );
bool adxl345_isTapDetectionOnY( void );
void adxl345_setTapDetectionOnY( bool state );
bool adxl345_isTapDetectionOnZ( void );
void adxl345_setTapDetectionOnZ( bool state );

void adxl345_setActivityX( bool state );
void adxl345_setActivityY( bool state );
void adxl345_setActivityZ( bool state );
void adxl345_setInactivityX( bool state );
void adxl345_setInactivityY( bool state );
void adxl345_setInactivityZ( bool state );

bool adxl345_isActivitySourceOnX( void );
bool adxl345_isActivitySourceOnY( void );
bool adxl345_isActivitySourceOnZ( void );
bool adxl345_isTapSourceOnX( void );
bool adxl345_isTapSourceOnY( void );
bool adxl345_isTapSourceOnZ( void );
bool adxl345_isAsleep( void );

bool adxl345_isLowPower( void );
void adxl345_setLowPower( bool state );
double adxl345_getRate( void );
void adxl345_setRate( double rate );
void adxl345_set_bw( char bw_code );
char adxl345_get_bw_code( void );


bool adxl345_triggered( char interrupts, int mask );


char adxl345_getInterruptSource( void );
bool adxl345_getInterruptSourceBit( char interruptBit );
bool adxl345_getInterruptMapping( char interruptBit );
void adxl345_setInterruptMapping( char interruptBit, bool interruptPin );
bool adxl345_isInterruptEnabled( char interruptBit );
void adxl345_setInterrupt( char interruptBit, bool state );

void adxl345_getRangeSetting( char* rangeSetting );
void adxl345_setRangeSetting( int val );
bool adxl345_getSelfTestBit( void );
void adxl345_setSelfTestBit( bool selfTestBit );
bool adxl345_getSpiBit( void );
void adxl345_setSpiBit( bool spiBit );
bool adxl345_getInterruptLevelBit( void );
void adxl345_setInterruptLevelBit( bool interruptLevelBit );
bool adxl345_getFullResBit( void );
void adxl345_setFullResBit( bool fullResBit );
bool adxl345_getJustifyBit( void );
void adxl345_setJustifyBit( bool justifyBit );


void adxl345_printAllRegister( void );



#endif