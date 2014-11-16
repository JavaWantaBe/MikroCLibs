#include "adxl345.h"
#include <math.h>

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#define ADXL345_DEVICE_WRITE (0x3A)  // ADXL345 device address
#define ADXL345_DEVICE_READ (0x3B)
#define ADXL345_TO_READ (6)   // num of bytes we are going to read each time (two bytes for each axis)
#define ACKNOWLEDGE 1
#define NO_ACKNOWLEDGE 0

//private:
static void writeTo( char address, char val );
static void readFrom( char address, int num, char buff[] );
static void setRegisterBit( char regAdress, int bitPos, bool state );
static bool getRegisterBit( char regAdress, int bitPos );
static void print_char( char val );

// Variables
static char _buff[6] ;  //6 chars buffer for saving data read from the device
static bool status;        // set when error occurs

// see error code for details
static char error_code;    // Initial state
static double gains[3];    // counts to Gs

static void ( *printch )( char* s );

// Writes val to address register on device
static void writeTo(  char address,  char val )
{
    TWI_Start();              // issue I2C start signal
    TWI_Write( ADXL345_DEVICE_WRITE );          // send byte via I2C  (device address + W)
    TWI_Write( address );       // send byte (address of the location)
    TWI_Write( val );         // send data (data to be written)
    TWI_Stop();               // issue I2C stop signal
}

// Reads num  chars starting from address register on device in to _buff array
static void readFrom(  char address, int num,  char _buff[] )
{
    int i = 0;
    
    TWI_Start();              // issue I2C start signal
    TWI_Write( ADXL345_DEVICE_WRITE );        // send byte via I2C (device address + W)
    TWI_Write( address );     // send byte (data address)

    TWI_Start();              // issue I2C signal repeated start
    TWI_Write( ADXL345_DEVICE_READ );        // send byte (device address + R)

    while( i < num )
    {
        if( i == ( num - 1 ) )
            _buff[i] = TWI_Read( NO_ACKNOWLEDGE );
        else
            _buff[i] = TWI_Read( ACKNOWLEDGE );
        i++;
    }
    
    if ( i != num )
    {
        status = ADXL345_ERROR;
        error_code = ADXL345_READ_ERROR;
    }
    
    TWI_Stop();
    
}

static void setRegisterBit( char regAdress, int bitPos, bool state )
{
    char _b = '\0';
    
    readFrom( regAdress, 1, &_b );

    if( state )
    {
        _b |= ( 1 << bitPos ); // forces nth bit of _b to be 1. all other bits left alone.
    }
    else
    {
        _b &= ~( 1 << bitPos ); // forces nth bit of _b to be 0. all other bits left alone.
    }

    writeTo( regAdress, _b );
}

static bool getRegisterBit( char regAdress, int bitPos )
{
    char _b = '\0';
    
    readFrom( regAdress, 1, &_b );
    return ( ( _b >> bitPos ) & 1 );
}

static void print_char( char val )
{
    if( printch != 0 )
    {
        int i = 0;
        char txt[4] = {0};
        
        printch("B");
        
        for( i = 7; i >= 0; i-- )
        {
            ByteToStr( ( val >> i & 1 ), txt );
            printch( txt );
        }
    }
}




// Public
void adxl345_init( void ( *printme )( char* s ) )
{
    status = ADXL345_OK;
    error_code = ADXL345_NO_ERROR;

    gains[0] = 0.00376390;
    gains[1] = 0.00376009;
    gains[2] = 0.00349265;
    
    //Turning on the ADXL345
    writeTo( ADXL345_POWER_CTL, 0 );
    writeTo( ADXL345_POWER_CTL, 16 );
    writeTo( ADXL345_POWER_CTL, 8 );
    
    if( printme != 0 )
        printch = printme;
}

// Reads the acceleration into three variable x, y and z
void adxl345_readAccelxyz( int *xyz )
{
    adxl345_readAccel( xyz, xyz + 1, xyz + 2 );
}

void adxl345_readAccel( int *x, int *y, int *z )
{
    readFrom( ADXL345_DATAX0, ADXL345_TO_READ, _buff ); //read the acceleration data from the ADXL345

    // each axis reading comes in 10 bit resolution, ie 2  chars. Least Significat  char first!!
    // thus we are converting both  chars in to one int
    *x = ( ( ( int ) _buff[1] ) << 8 ) | _buff[0];
    *y = ( ( ( int ) _buff[3] ) << 8 ) | _buff[2];
    *z = ( ( ( int ) _buff[5] ) << 8 ) | _buff[4];
}

void adxl345_get_Gxyz( double *xyz )
{
    int i = 0;
    int xyz_int[3] = {0};
    
    adxl345_readAccelxyz( xyz_int );
    
    for( i = 0; i < 3; i++ )
    {
        xyz[i] = xyz_int[i] * gains[i];
    }
}


// Sets the THRESH_TAP  char value
// it should be between 0 and 255
// the scale factor is 62.5 mg/LSB
// A value of 0 may result in undesirable behavior
void adxl345_setTapThreshold(int tapThreshold )
{
    char _b = '\0';

    tapThreshold = constrain( tapThreshold, 0, 255 );
    _b = ( char )tapThreshold;
    writeTo( ADXL345_THRESH_TAP, _b );
}

// Gets the THRESH_TAP  char value
// return value is comprised between 0 and 255
// the scale factor is 62.5 mg/LSB
int adxl345_getTapThreshold()
{
    char _b = '\0';

    readFrom( ADXL345_THRESH_TAP, 1, &_b );
    return ( int )_b;
}

// set/get the gain for each axis in Gs / count
void adxl345_setAxisGains( double *_gains )
{
    int i;

    for ( i = 0; i < 3; i++ )
    {
        gains[i] = _gains[i];
    }
}


void adxl345_getAxisGains( double *_gains )
{
    int i;

    for( i = 0; i < 3; i++ )
    {
       _gains[i] = gains[i];
    }
}

// Sets the OFSX, OFSY and OFSZ  chars
// OFSX, OFSY and OFSZ are user offset adjustments in twos complement format with
// a scale factor of 15,6mg/LSB
// OFSX, OFSY and OFSZ should be comprised between
void adxl345_setAxisOffset( int x, int y, int z )
{
    writeTo( ADXL345_OFSX, ( char )x );
    writeTo( ADXL345_OFSY, ( char )y );
    writeTo( ADXL345_OFSZ, ( char )z );
}

// Gets the OFSX, OFSY and OFSZ  chars
void adxl345_getAxisOffset( int* x, int* y, int*z )
{
    char _b = '\0';

    readFrom( ADXL345_OFSX, 1, &_b );
    *x = ( int )_b;
    readFrom( ADXL345_OFSY, 1, &_b );
    *y = ( int )_b;
    readFrom( ADXL345_OFSZ, 1, &_b );
    *z = ( int )_b;
}

// Sets the DUR  char
// The DUR  char contains an unsigned time value representing the maximum time
// that an event must be above THRESH_TAP threshold to qualify as a tap event
// The scale factor is 625µs/LSB
// A value of 0 disables the tap/double tap funcitons. Max value is 255.
void adxl345_setTapDuration( int tapDuration )
{
    char _b = '\0';

    tapDuration = constrain( tapDuration, 0, 255 );
    _b = ( char )tapDuration;
    writeTo( ADXL345_DUR, _b );
}

// Gets the DUR  char
int adxl345_getTapDuration()
{
    char _b = '\0';

    readFrom( ADXL345_DUR, 1, &_b );
    return ( int )_b;
}

// Sets the latency (latent register) which contains an unsigned time value
// representing the wait time from the detection of a tap event to the start
// of the time window, during which a possible second tap can be detected.
// The scale factor is 1.25ms/LSB. A value of 0 disables the double tap function.
// It accepts a maximum value of 255.
void adxl345_setDoubleTapLatency( int doubleTapLatency )
{
    char _b = ( char )doubleTapLatency;
    writeTo( ADXL345_LATENT, _b );
}

// Gets the Latent value
int adxl345_getDoubleTapLatency()
{
    char _b = '\0';

    readFrom( ADXL345_LATENT, 1, &_b );
    return ( int )_b;
}

// Sets the Window register, which contains an unsigned time value representing
// the amount of time after the expiration of the latency time (Latent register)
// during which a second valud tap can begin. The scale factor is 1.25ms/LSB. A
// value of 0 disables the double tap function. The maximum value is 255.
void adxl345_setDoubleTapWindow( int doubleTapWindow )
{
    char _b = '\0';

    doubleTapWindow = constrain( doubleTapWindow, 0, 255 );
    _b = ( char )doubleTapWindow;
    writeTo( ADXL345_WINDOW, _b );
}

// Gets the Window register
int adxl345_getDoubleTapWindow()
{
    char _b = '\0';

    readFrom( ADXL345_WINDOW, 1, &_b );
    return ( int )_b;
}

// Sets the THRESH_ACT  char which holds the threshold value for detecting activity.
// The data format is unsigned, so the magnitude of the activity event is compared
// with the value is compared with the value in the THRESH_ACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the
// activity interrupt is enabled. The maximum value is 255.
void adxl345_setActivityThreshold( int activityThreshold )
{
    char _b = '\0';

    activityThreshold = constrain( activityThreshold, 0, 255 );
    _b = ( char )activityThreshold;
    writeTo( ADXL345_THRESH_ACT, _b );
}

// Gets the THRESH_ACT  char
int adxl345_getActivityThreshold()
{
    char _b = '\0';

    readFrom( ADXL345_THRESH_ACT, 1, &_b );
    return ( int )_b;
}

// Sets the THRESH_INACT  char which holds the threshold value for detecting inactivity.
// The data format is unsigned, so the magnitude of the inactivity event is compared
// with the value is compared with the value in the THRESH_INACT register. The scale
// factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the
// inactivity interrupt is enabled. The maximum value is 255.
void adxl345_setInactivityThreshold( int inactivityThreshold )
{
    char _b = '\0';

    inactivityThreshold = constrain( inactivityThreshold, 0, 255 );
    _b = ( char )inactivityThreshold;
    writeTo( ADXL345_THRESH_INACT, _b );
}

// Gets the THRESH_INACT  char
int adxl345_getInactivityThreshold()
{
    char _b = '\0';

    readFrom( ADXL345_THRESH_INACT, 1, &_b );
    return ( int )_b;
}

// Sets the TIME_INACT register, which contains an unsigned time value representing the
// amount of time that acceleration must be less thant the value in the THRESH_INACT
// register for inactivity to be declared. The scale factor is 1sec/LSB. The value must
// be between 0 and 255.
void adxl345_setTimeInactivity( int timeInactivity )
{
    char _b = '\0';

    timeInactivity = constrain( timeInactivity, 0, 255 );
    _b = ( char )timeInactivity;
    writeTo( ADXL345_TIME_INACT, _b );
}

// Gets the TIME_INACT register
int adxl345_getTimeInactivity()
{
    char _b = '\0';

    readFrom( ADXL345_TIME_INACT, 1, &_b );
    return ( int )_b;
}

// Sets the THRESH_FF register which holds the threshold value, in an unsigned format, for
// free-fall detection. The root-sum-square (RSS) value of all axes is calculated and
// compared whith the value in THRESH_FF to determine if a free-fall event occured. The
// scale factor is 62.5mg/LSB. A value of 0 may result in undesirable behavior if the free-fall
// interrupt is enabled. The maximum value is 255.
void adxl345_setFreeFallThreshold( int freeFallThreshold )
{
    char _b = '\0';

    freeFallThreshold = constrain( freeFallThreshold, 0, 255 );
    _b = ( char )freeFallThreshold;
    writeTo( ADXL345_THRESH_FF, _b );
}

// Gets the THRESH_FF register.
int adxl345_getFreeFallThreshold()
{
    char _b = '\0';

    readFrom( ADXL345_THRESH_FF, 1, &_b );
    return ( int )_b;
}

// Sets the TIME_FF register, which holds an unsigned time value representing the minimum
// time that the RSS value of all axes must be less than THRESH_FF to generate a free-fall
// interrupt. The scale factor is 5ms/LSB. A value of 0 may result in undesirable behavior if
// the free-fall interrupt is enabled. The maximum value is 255.
void adxl345_setFreeFallDuration( int freeFallDuration )
{
    char _b = '\0';

    freeFallDuration = constrain( freeFallDuration, 0, 255 );
    _b = ( char )freeFallDuration;
    writeTo( ADXL345_TIME_FF, _b );
}

// Gets the TIME_FF register.
int adxl345_getFreeFallDuration()
{
    char _b = '\0';

    readFrom( ADXL345_TIME_FF, 1, &_b );
    return ( int )_b;
}







bool adxl345_isActivityXEnabled()
{
    return getRegisterBit( ADXL345_ACT_INACT_CTL, 6 );
}

bool adxl345_isActivityYEnabled()
{
    return getRegisterBit( ADXL345_ACT_INACT_CTL, 5 );
}

bool adxl345_isActivityZEnabled()
{
    return getRegisterBit( ADXL345_ACT_INACT_CTL, 4 );
}

bool adxl345_isInactivityXEnabled()
{
    return getRegisterBit( ADXL345_ACT_INACT_CTL, 2 );
}

bool adxl345_isInactivityYEnabled()
{
    return getRegisterBit( ADXL345_ACT_INACT_CTL, 1 );
}

bool adxl345_isInactivityZEnabled()
{
    return getRegisterBit( ADXL345_ACT_INACT_CTL, 0 );
}

void adxl345_setActivityX( bool state )
{
    setRegisterBit( ADXL345_ACT_INACT_CTL, 6, state );
}

void adxl345_setActivityY( bool state )
{
    setRegisterBit( ADXL345_ACT_INACT_CTL, 5, state );
}

void adxl345_setActivityZ( bool state )
{
    setRegisterBit( ADXL345_ACT_INACT_CTL, 4, state );
}

void adxl345_setInactivityX( bool state )
{
    setRegisterBit( ADXL345_ACT_INACT_CTL, 2, state );
}

void adxl345_setInactivityY( bool state )
{
    setRegisterBit( ADXL345_ACT_INACT_CTL, 1, state );
}

void adxl345_setInactivityZ( bool state )
{
    setRegisterBit( ADXL345_ACT_INACT_CTL, 0, state );
}

bool adxl345_isActivityAc()
{
    return getRegisterBit( ADXL345_ACT_INACT_CTL, 7 );
}

bool adxl345_isInactivityAc()
{
    return getRegisterBit( ADXL345_ACT_INACT_CTL, 3 );
}

void adxl345_setActivityAc( bool state )
{
    setRegisterBit( ADXL345_ACT_INACT_CTL, 7, state );
}

void adxl345_setInactivityAc( bool state )
{
    setRegisterBit( ADXL345_ACT_INACT_CTL, 3, state );
}









bool adxl345_getSuppressBit()
{
    return getRegisterBit( ADXL345_TAP_AXES, 3 );
}

void adxl345_setSuppressBit( bool state )
{
    setRegisterBit( ADXL345_TAP_AXES, 3, state );
}

bool adxl345_isTapDetectionOnX()
{
    return getRegisterBit( ADXL345_TAP_AXES, 2 );
}

void adxl345_setTapDetectionOnX( bool state )
{
    setRegisterBit( ADXL345_TAP_AXES, 2, state );
}

bool adxl345_isTapDetectionOnY()
{
    return getRegisterBit( ADXL345_TAP_AXES, 1 );
}

void adxl345_setTapDetectionOnY( bool state )
{
    setRegisterBit( ADXL345_TAP_AXES, 1, state );
}

bool adxl345_isTapDetectionOnZ()
{
    return getRegisterBit( ADXL345_TAP_AXES, 0 );
}

void adxl345_setTapDetectionOnZ( bool state )
{
    setRegisterBit( ADXL345_TAP_AXES, 0, state );
}







bool adxl345_isActivitySourceOnX()
{
    return getRegisterBit( ADXL345_ACT_TAP_STATUS, 6 );
}

bool adxl345_isActivitySourceOnY()
{
    return getRegisterBit( ADXL345_ACT_TAP_STATUS, 5 );
}

bool adxl345_isActivitySourceOnZ()
{
    return getRegisterBit( ADXL345_ACT_TAP_STATUS, 4 );
}

bool adxl345_isTapSourceOnX()
{
    return getRegisterBit( ADXL345_ACT_TAP_STATUS, 2 );
}

bool adxl345_isTapSourceOnY()
{
    return getRegisterBit( ADXL345_ACT_TAP_STATUS, 1 );
}

bool adxl345_isTapSourceOnZ()
{
    return getRegisterBit( ADXL345_ACT_TAP_STATUS, 0 );
}

bool adxl345_isAsleep()
{
    return getRegisterBit( ADXL345_ACT_TAP_STATUS, 3 );
}





bool adxl345_isLowPower()
{
    return getRegisterBit( ADXL345_BW_RATE, 4 );
}

void adxl345_setLowPower( bool state )
{
    setRegisterBit( ADXL345_BW_RATE, 4, state );
}

double adxl345_getRate()
{
    char _b = '\0';

    readFrom( ADXL345_BW_RATE, 1, &_b );
    _b &= 0b00001111;

    return ( pow( 2, ( (int) _b ) - 6) ) * 6.25;
}

void adxl345_setRate( double rate )
{
    char _b = '\0', _s = '\0';
    int v = (int) (rate / 6.25);
    int r = 0;

    while( v >>= 1 )
    {
        r++;
    }

    if( r <= 9 )
    {
        readFrom( ADXL345_BW_RATE, 1, &_b );
        _s = ( char) (r + 6) | (_b & 0b11110000);
        writeTo( ADXL345_BW_RATE, _s );
    }
}

void adxl345_set_bw( char bw_code )
{
    if( ( bw_code < ADXL345_BW_3 ) || ( bw_code > ADXL345_BW_1600 ) )
    {
        status = false;
        error_code = ADXL345_BAD_ARG;
    }
    else
    {
        writeTo( ADXL345_BW_RATE, bw_code );
    }
}

char adxl345_get_bw_code()
{
    char bw_code = '\0';

    readFrom( ADXL345_BW_RATE, 1, &bw_code );
    return bw_code;
}








//Used to check if action was triggered in interrupts
//Example triggered(interrupts, ADXL345_SINGLE_TAP);
bool adxl345_triggered( char interrupts, int mask )
{
    return ( ( interrupts >> mask ) & 1 );
}






/*
 ADXL345_DATA_READY
 ADXL345_SINGLE_TAP
 ADXL345_DOUBLE_TAP
 ADXL345_ACTIVITY
 ADXL345_INACTIVITY
 ADXL345_FREE_FALL
 ADXL345_WATERMARK
 ADXL345_OVERRUNY
 */

char adxl345_getInterruptSource()
{
    char _b = '\0';
    readFrom( ADXL345_INT_SOURCE, 1, &_b );
    return _b;
}

bool adxl345_getInterruptSourceBit( char interruptBit )
{
    return getRegisterBit( ADXL345_INT_SOURCE, interruptBit );
}

bool adxl345_getInterruptMapping( char interruptBit )
{
    return getRegisterBit( ADXL345_INT_MAP, interruptBit );
}

// Set the mapping of an interrupt to pin1 or pin2
// eg: setInterruptMapping(ADXL345_INT_DOUBLE_TAP_BIT,ADXL345_INT2_PIN);
void adxl345_setInterruptMapping( char interruptBit, bool interruptPin )
{
    setRegisterBit( ADXL345_INT_MAP, interruptBit, interruptPin );
}

bool adxl345_isInterruptEnabled( char interruptBit )
{
    return getRegisterBit( ADXL345_INT_ENABLE, interruptBit );
}

void adxl345_setInterrupt( char interruptBit, bool state )
{
    setRegisterBit( ADXL345_INT_ENABLE, interruptBit, state );
}












// Gets the range setting and return it into rangeSetting
// it can be 2, 4, 8 or 16
void adxl345_getRangeSetting( char* rangeSetting )
{
    char _b = '\0';
    
    readFrom( ADXL345_DATA_FORMAT, 1, &_b );
    *rangeSetting = _b & 0b00000011;
}

// Sets the range setting, possible values are: 2, 4, 8, 16
void adxl345_setRangeSetting( int val )
{
    char _s = '\0';
    char _b = '\0';

    switch ( val ) 
    {
        case 2:
            _s = 0b00000000;
            break;
        case 4:
            _s = 0b00000001;
            break;
        case 8:
            _s = 0b00000010;
            break;
        case 16:
            _s = 0b00000011;
            break;
        default:
            _s = 0b00000000;
    }
        
    readFrom( ADXL345_DATA_FORMAT, 1, &_b );
    _s |= ( _b & 0b11101100 );
    writeTo( ADXL345_DATA_FORMAT, _s );
}

// gets the state of the SELF_TEST bit
bool adxl345_getSelfTestBit()
{
    return getRegisterBit( ADXL345_DATA_FORMAT, 7 );
}

// Sets the SELF-TEST bit
// if set to 1 it applies a self-test force to the sensor causing a shift in the output data
// if set to 0 it disables the self-test force
void adxl345_setSelfTestBit( bool selfTestBit )
{
    setRegisterBit( ADXL345_DATA_FORMAT, 7, selfTestBit );
}

// Gets the state of the SPI bit
bool adxl345_getSpiBit()
{
    return getRegisterBit( ADXL345_DATA_FORMAT, 6 );
}

// Sets the SPI bit
// if set to 1 it sets the device to 3-wire mode
// if set to 0 it sets the device to 4-wire SPI mode
void adxl345_setSpiBit( bool spiBit )
{
    setRegisterBit( ADXL345_DATA_FORMAT, 6, spiBit );
}

// Gets the state of the INT_INVERT bit
bool adxl345_getInterruptLevelBit()
{
    return getRegisterBit( ADXL345_DATA_FORMAT, 5 );
}

// Sets the INT_INVERT bit
// if set to 0 sets the interrupts to active high
// if set to 1 sets the interrupts to active low
void adxl345_setInterruptLevelBit( bool interruptLevelBit )
{
    setRegisterBit( ADXL345_DATA_FORMAT, 5, interruptLevelBit );
}

// Gets the state of the FULL_RES bit
bool adxl345_getFullResBit()
{
    return getRegisterBit( ADXL345_DATA_FORMAT, 3 );
}

// Sets the FULL_RES bit
// if set to 1, the device is in full resolution mode, where the output resolution increases with the
//  g range set by the range bits to maintain a 4mg/LSB scal factor
// if set to 0, the device is in 10-bit mode, and the range buts determine the maximum g range
//  and scale factor
void adxl345_setFullResBit( bool fullResBit )
{
    setRegisterBit( ADXL345_DATA_FORMAT, 3, fullResBit );
}

// Gets the state of the justify bit
bool adxl345_getJustifyBit()
{
    return getRegisterBit( ADXL345_DATA_FORMAT, 2 );
}

// Sets the JUSTIFY bit
// if sets to 1 selects the left justified mode
// if sets to 0 selects right justified mode with sign extension
void adxl345_setJustifyBit( bool justifyBit )
{
    setRegisterBit( ADXL345_DATA_FORMAT, 2, justifyBit );
}





// print all register value to the serial ouptut, which requires it to be setup
// this can be used to manually to check the current configuration of the device
void adxl345_printAllRegister()
{
    if( printch != 0 )  
    {
        char _b = '\0';
        int i = 0;
        char txt[4] = {0};
        
        printch( "0x00: " );
        readFrom( 0x00, 1, &_b );
        print_char( _b );
        printch("");

        for( i = 29; i <= 57; i++ )
        {
            printch("0x");
            ByteToHex( i, txt );
            printch( txt );
            printch(": ");
            readFrom(i, 1, &_b);
            print_char( _b );
            printch("");
        }
    }
}