/**
 * @file adxl345.h
 *
 * @brief Accelerometer ADXL345 Driver
 *
 * This driver was adapted from the arduino library for the same sensor.  It
 * supports both SPI and I2C bus communications as well as full featured
 * access to the registers.
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
 * @note
 * <strong>Test configuration:</strong></br>
 *   <ul>
 *   <li>MCU:             ATMega32</li>
 *   <li>Dev.Board:       EasyAVR v7</li>
 *   <li>Oscillator:      8Mhz</li>
 *   <li>Ext. Modules:    x</li>
 *   <li>SW:              MikroC v6.0</li>
 *  </ul>
 *  </br>
 *  @par The ADXL345 operates in a 100 Hz ODR with a DATA_READY 
 *  interrupt on the INT1 pin during this start-up sequence. When 
 *  setting other interrupts or using the FIFO, it is recommended 
 *  that those registers used are set before the POWER_CTL and 
 *  INT_ENABLE registers
 *
 * @mainpage
 *  @section Documentation Quick Links
 *  <ul>
 *	<li>@subpage Intro
 *	<li>@subpage ADXL345_Registers
 *  </ul>
 *  @section Features
 *  <ul>
 *  <li>@par Ultralow power: as low as 23 µA in measurement mode and 
 *  0.1 µA in standby mode at VS = 2.5 V (typical)</li>
 *  <li>@par Power consumption scales automatically with bandwidth</li>
 *  <li>@par User-selectable resolution</br>
 *	Fixed 10-bit resolution</br>
 *  Full resolution, where resolution increases with g range, 
 *  up to 13-bit resolution at ±16 g (maintaining 4 mg/LSB 
 *  scale factor in all g ranges)</li>
 *  <li>@par Patent pending, embedded memory management system 
 *  with FIFO technology minimizes host processor load</li>
 *  <li>@par Single tap/double tap detection</li>
 *  <li>@par Activity/inactivity monitoring</li>
 *  <li>@par Free-fall detection</li>
 *  <li>@par Supply voltage range: 2.0 V to 3.6 V</li>
 *  <li>@par I/O voltage range: 1.7 V to VS</li>
 *  <li>@par SPI (3- and 4-wire) and I2C digital interfaces</li>
 *  <li>@par Flexible interrupt modes mappable to either interrupt pin</li>
 *  <li>@par Measurement ranges selectable via serial command</li>
 *  <li>@par Bandwidth selectable via serial command</li>
 *  <li>@par Wide temperature range (−40°C to +85°C)</li>
 *  <li>@par 10,000 g shock survival</li>
 *  </ul>
 */

#ifndef ADXL345_H
#define ADXL345_H


#include <stdbool.h>
#include <stdint.h>

/**
 *  @defgroup Registers 
 *  <strong>REGISTER DEFINITIONS</strong>
 *	@{
 */
#define ADXL345_DEVID       	0x00 /**< 0x00 - Device ID | Type R | Reset Value - 11100101 */  
#define ADXL345_RESERVED1   	0x01 /**< 0x01 - 0x1c Reserved; do not access*/
#define ADXL345_THRESH_TAP  	0x1d /**< 0x1d - Tap threshold | Type R/W | Reset Value - 00000000 | @ref THRESH_TAP "Threshold Description" */
#define ADXL345_OFSX 			0x1e /**< 0x1e - X-axis offset | Type R/W | Reset Value - 00000000 | @ref OFFSET "Offset Description" */
#define ADXL345_OFSY 			0x1f /**< 0x1f - Y-axis offset | Type R/W | Reset Value - 00000000 | @ref OFFSET "Offset Description" */
#define ADXL345_OFSZ 			0x20 /**< 0x20 - Z-axis offset | Type R/W | Reset Value - 00000000 | @ref OFFSET "Offset Description" */
#define ADXL345_DUR 			0x21 /**< 0x21 - Tap duration | Type R/W | Reset Value - 00000000 | @ref DUR "Duration Description */
#define ADXL345_LATENT 			0x22 /**< 0x22 - Tap latency | Type R/W | Reset Value - 00000000 | @ref LATENT "Latency Description */
#define ADXL345_WINDOW 			0x23 /**< 0x23 - Tap window | Type R/W | Reset Value - 00000000 */
#define ADXL345_THRESH_ACT 		0x24 /**< 0x24 - Activity threshold | Type R/W | Reset Value - 00000000 */
#define ADXL345_THRESH_INACT 	0x25 /**< 0x25 - Inactivity threshold | Type R/W | Reset Value - 00000000 */
#define ADXL345_TIME_INACT 		0x26 /**< 0x26 - Inactivity time | Type R/W | Reset Value - 00000000 */
#define ADXL345_ACT_INACT_CTL 	0x27 /**< 0x27 - Axis enable control for activity and inactivity detection | Type R/W | Reset Value - 00000000 */
#define ADXL345_THRESH_FF 		0x28 /**< 0x28 - Free-fall threshold | Type R/W | Reset Value - 00000000 */
#define ADXL345_TIME_FF 		0x29 /**< 0x29 - Free-fall time | Type R/W | Reset Value - 00000000 */
#define ADXL345_TAP_AXES 		0x2a /**< 0x2a - Axis control for single tap/double tap | Type R/W | Reset Value - 00000000 */
#define ADXL345_ACT_TAP_STATUS 	0x2b /**< 0x2b - Source of single tap/double tap | Type R | Reset Value - 00000000*/  
#define ADXL345_BW_RATE 		0x2c /**< 0x2c - Data rate and power mode control| Type R/W | Reset Value - 00001010 */
#define ADXL345_POWER_CTL 		0x2d /**< 0x2d - Power-saving features control | Type R/W | Reset Value - 00000000 */
#define ADXL345_INT_ENABLE 		0x2e /**< 0x2e - Interrupt enable control | Type R/W | Reset Value - 00000000 */
#define ADXL345_INT_MAP 		0x2f /**< 0x2f - Interrupt mapping control | Type R/W | Reset Value - 00000000 */
#define ADXL345_INT_SOURCE 		0x30 /**< 0x30 - Source of interrupts | Type R | Reset Value - 00000010 */  
#define ADXL345_DATA_FORMAT 	0x31 /**< 0x31 - Data format control | Type R/W | Reset Value - 00000000 */
#define ADXL345_DATAX0 			0x32 /**< 0x32 - X-Axis Data 0 | Type R | Reset Value - 00000000 */  
#define ADXL345_DATAX1 			0x33 /**< 0x33 - X-Axis Data 1 | Type R | Reset Value - 00000000 */  
#define ADXL345_DATAY0 			0x34 /**< 0x34 - Y-Axis Data 0 | Type R | Reset Value - 00000000 */ 
#define ADXL345_DATAY1 			0x35 /**< 0x35 - Y-Axis Data 1 | Type R | Reset Value - 00000000 */ 
#define ADXL345_DATAZ0 			0x36 /**< 0x36 - Z-Axis Data 0 | Type R | Reset Value - 00000000 */ 
#define ADXL345_DATAZ1 			0x37 /**< 0x37 - Z-Axis Data 1 | Type R | Reset Value - 00000000 */ 
#define ADXL345_FIFO_CTL 		0x38 /**< 0x38 - FIFO control | Type R/W | Reset Value - 00000000 */
#define ADXL345_FIFO_STATUS 	0x39 /**< 0x39 - FIFO status | Type R | Reset Value - 00000000 */ 
 /**@}*/


#define ADXL345_BW_1600 0xF  // 1111
#define ADXL345_BW_800  0xE  // 1110
#define ADXL345_BW_400  0xD  // 1101
#define ADXL345_BW_200  0xC  // 1100
#define ADXL345_BW_100  0xB  // 1011
#define ADXL345_BW_50   0xA  // 1010
#define ADXL345_BW_25   0x9  // 1001
#define ADXL345_BW_12   0x8  // 1000
#define ADXL345_BW_6    0x7  // 0111
#define ADXL345_BW_3    0x6  // 0110


/**
 * @defgroup Interrupt_PINs
 * @{
 */
#define ADXL345_INT1_PIN 0x00 /**< Enable INT1 pin */
#define ADXL345_INT2_PIN 0x01 /**< Enable INT2 pin */
/**@}*/

/*
Interrupt bit position
*/
#define ADXL345_INT_DATA_READY_BIT 0x07
#define ADXL345_INT_SINGLE_TAP_BIT 0x06
#define ADXL345_INT_DOUBLE_TAP_BIT 0x05
#define ADXL345_INT_ACTIVITY_BIT   0x04
#define ADXL345_INT_INACTIVITY_BIT 0x03
#define ADXL345_INT_FREE_FALL_BIT  0x02
#define ADXL345_INT_WATERMARK_BIT  0x01
#define ADXL345_INT_OVERRUNY_BIT   0x00

/**
 *  @defgroup Interrupt_Sources
 *  @{
 */
#define ADXL345_DATA_READY 0x07
#define ADXL345_SINGLE_TAP 0x06
#define ADXL345_DOUBLE_TAP 0x05
#define ADXL345_ACTIVITY   0x04
#define ADXL345_INACTIVITY 0x03
#define ADXL345_FREE_FALL  0x02
#define ADXL345_WATERMARK  0x01
#define ADXL345_OVERRUNY   0x00
/**@}*/

#define ADXL345_OK  1        // no error
#define ADXL345_ERROR 0      // indicates error is predent

#define ADXL345_NO_ERROR  0  // initial state
#define ADXL345_READ_ERROR 1 // problem reading accel
#define ADXL345_BAD_ARG  2   // bad method argument

/**
 *  @defgroup General_Use_Functions
 *  @{
 */

/**
 *  @ingroup General_Use_Functions
 *  @brief Initializes the Sensor
 *
 *  @pre TWI_Init needs to be called before.
 *
 */
void adxl345_init( void );

/**
 *  @brief Reads the acceleration into an array of 3 ints
 *
 *  @param[out] xyz - array of 3 int variables
 */
void adxl345_readAccelxyz( int* xyx );

/**
 *  @brief Reads the acceleration into seperate int variables
 *
 *  @param[out] x,y,z Values of sensor axis
 *
 */
void adxl345_readAccel( int* x, int* y, int* z );

/**
 *  @brief Gets the G forces on axis x,y,z into array of double
 *
 *  @param[out] xyz - array of 3 double variables
 *
 */
void adxl345_get_Gxyz( double* xyz );

/**@}*/

/**
 *  @defgroup Behavior_Settings
 *	@{
 */
/**
 *  @brief Set the tap threshold
 *  @ref THRESH_TAP
 *  @param[in] tapThreshold - values from 0 - 255
 */
void adxl345_setTapThreshold( uint8_t tapThreshold );

/**
 *  @brief Get the tap threshold
 *  @ref THRESH_TAP
 *  @returns int
 *      @retval 0 - 255
 */
uint8_t adxl345_getTapThreshold( void );

/**
 *  @brief Set the axis gain
 *  @param[in,out] _gains - 
 */
void adxl345_setAxisGains( double *_gains );

/**
 *  @brief Get the axis gain
 *  @param[out] _gains
 */
void adxl345_getAxisGains( double *_gains );

/**
 *  @brief Set axis offset
 *  @param[in] x,y,z - Offset of axis x, y, and z
 */
void adxl345_setAxisOffset( int x, int y, int z );

/**
 *  @brief Get axis offset
 *  @param[out] x,y,z - axis offsets
 */
void adxl345_getAxisOffset( int* x, int* y, int*z );

/**
 *  @brief Set tap duration
 *  @param[in] int tapDuration
 *  @parblock
 *  Sets the DUR(duration)
 *	The DUR contains an unsigned time value representing the maximum time
 *  that an event must be above THRESH_TAP threshold to qualify as a tap event
 *  The scale factor is 625µs/LSB
 *	A value of 0 disables the tap/double tap funcitons. Max value is 255.
 *  @endparblock
 */
void adxl345_setTapDuration( uint8_t tapDuration );

/**
 *  @brief Get tap duration
 *
 *  @returns int
 *		@retval 0 - 255
 */
uint8_t adxl345_getTapDuration( void );


void adxl345_setDoubleTapLatency( uint8_t doubleTapLatency );

uint8_t adxl345_getDoubleTapLatency( void );

void adxl345_setDoubleTapWindow( uint8_t doubleTapWindow );

uint8_t adxl345_getDoubleTapWindow( void );

void adxl345_setActivityThreshold( uint8_t activityThreshold );

uint8_t adxl345_getActivityThreshold( void );

void adxl345_setInactivityThreshold( uint8_t inactivityThreshold );

uint8_t adxl345_getInactivityThreshold( void );

void adxl345_setTimeInactivity( uint8_t timeInactivity );

uint8_t adxl345_getTimeInactivity( void );


/**
 *  @brief Sets the freefall threshold
 *
 *  @param[in] int freeFallThreshold
 *  @parblock
 *  Sets the TIME_FF register, which holds an unsigned time value representing the minimum
 * 	time that the RSS value of all axes must be less than THRESH_FF to generate a free-fall
 *  interrupt. The scale factor is 5ms/LSB. The maximum value is 255.
 *  @endparblock
 * 
 *  @note 
 *	A value of 0 may result in undesirable behavior if the free-fall interrupt is enabled.
 */
void adxl345_setFreeFallThreshold( uint8_t freeFallThreshold );

/**
 *  @brief Get freefall threshold
 *
 *  @returns uint8_t
 *		@retval Value 0 - 255
 *
 */
uint8_t adxl345_getFreeFallThreshold( void );

/**
 *  @brief Sets the TIME_FF register 
 *
 *  @param[in] int freeFallDuration - The scale factor is 5ms/LSB. A value of 0 may result in undesirable behavior if
 *  the free-fall interrupt is enabled. The maximum value is 255.
 *
 *  @note
 *     Time that the RSS value of all axes must be less than THRESH_FF to generate a free-fall interrupt. 
 *
 */
void adxl345_setFreeFallDuration( uint8_t freeFallDuration );

/** 
 *  @brief Gets the current freefall duration
 *
 *  @returns uint8_t
 *  	@retval 0-255 rate is 5ms*value
 */
uint8_t adxl345_getFreeFallDuration( void );
/**@}*/

/**
 * @defgroup Registry_Functions
 * @{
 */
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

void adxl345_set_bw( int bw_code );
int adxl345_get_bw_code( void );

/**
 *  @brief Used to check if action was triggered in interrupts
 *
 *  @param[in] interrupts -
 *  @param[in] mask -
 *
 *  @returns bool
 *		@retval 1 interrupt triggered
 *		@retval 0 interrupt not triggered
 *
 *	@code
 *  if( triggered( interrupts, ADXL345_SINGLE_TAP ) )
 *     printf( "Triggered" );
 *  @endcode
 *
 */
bool adxl345_triggered( int interrupts, int mask );

/**
 *  @brief Gets the source of interrupt triggered
 *
 *  @returns char
 *		@retval ADXL345_DATA_READY
 *  	@retval ADXL345_SINGLE_TAP
 *  	@retval ADXL345_DOUBLE_TAP
 *  	@retval ADXL345_ACTIVITY
 *  	@retval ADXL345_INACTIVITY
 *  	@retval ADXL345_FREE_FALL
 *  	@retval ADXL345_WATERMARK
 *  	@retval ADXL345_OVERRUNY
 *
 */
int adxl345_getInterruptSource( void );

bool adxl345_getInterruptSourceBit( int interruptBit );

bool adxl345_getInterruptMapping( int interruptBit );

/**  
 *  @brief Set the mapping of an interrupt to pin1 or pin2
 *
 *  @param[in] interruptBit 
 *  @param[in] interruptPin
 *
 *  @code
 *  setInterruptMapping(ADXL345_INT_DOUBLE_TAP_BIT,ADXL345_INT2_PIN);
 *  @endcode
 *
 */
void adxl345_setInterruptMapping( int interruptBit, int interruptPin );
bool adxl345_isInterruptEnabled( int interruptBit );
void adxl345_setInterrupt( int interruptBit, bool state );


/**
 *  @brief Gets the range setting and return it into rangeSetting
 * 
 *  @param[in,out] rangeSetting 
 *
 */
void adxl345_getRangeSetting( uint8_t* rangeSetting );

/**
 *  @brief Sets the range setting
 *
 *  @param[in] int val - , possible values are: 2, 4, 8, 16
 *
 */
void adxl345_setRangeSetting( int val );

/**
 *  @brief Gets the state of the SELF_TEST bit
 *
 *  @returns bool
 *		@retval 1 self-test mode
 *		@retval 0 disabled self-test
 */
bool adxl345_getSelfTestBit( void );

/**
 *  @brief Sets the SELF-TEST bit
 *
 *  @param[in] bool - 
 *  	1 applies a self-test force to the sensor causing a shift in the output data
 *		0 disables the self-test force
 *
 */
void adxl345_setSelfTestBit( bool selfTestBit );

/**
 *  @brief Gets the state of the SPI bit
 *
 *  @returns bool
 *		@retval 1 3 wire SPI mode
 *		@retval 0 4 wire SPI mode
 */
bool adxl345_getSpiBit( void );

/**
 *  @brief Sets the SPI bit
 * 
 *  @param[in] bool spiBit - 1 sets the device to 3-wire mode, 0 sets the device to 4-wire SPI mode
 *
 */
void adxl345_setSpiBit( bool spiBit );

/**
 * @brief Gets the state of the INT_INVERT bit
 *
 *  @returns bool
 *  	@retval 0 interrupts active high
 *		@retval 1 interrupts active low
 */
bool adxl345_getInterruptLevelBit( void );

/**
 *  @brief Sets the INT_INVERT bit
 * 
 *  @param[in] bool interruptLevelBit - 0 sets the interrupts to active high, 1 sets the interrupts to active low
 *
 */
void adxl345_setInterruptLevelBit( bool interruptLevelBit );

/**
 *  @brief Gets the state of the FULL_RES bit
 *
 *  @returns bool
 *		@retval 1 full resolution mode
 *		@retval 0 10 bit mode
 */
bool adxl345_getFullResBit( void );

/**
 *  @brief Sets the FULL_RES bit
 *  
 *  @param[in] bool fullResBit
 *  @parblock 
 *  1 = the device is in full resolution mode, where the output resolution increases with the
 *  g range set by the range bits to maintain a 4mg/LSB scal factor
 *  @endparblock
 *  0 = the device is in 10-bit mode, range bits determine the maximum g range
 *  and scale factor
 */
void adxl345_setFullResBit( bool fullResBit );

/**
 *  @brief Gets the state of the justify bit
 *
 *  @returns bool
 *  	@retval 1 left justified mode
 *		@retval 0 right justified with sign 
 */
bool adxl345_getJustifyBit( void );

/**
 *  @brief Sets the JUSTIFY bit
 * 
 *  @param[in] bool justifyBit
 *  @parblock 
 *  1 = left justified mode 
 *	0 = right justified mode with sign extension
 *  @endparblock
 */
void adxl345_setJustifyBit( bool justifyBit );
/**@}*/

#endif

/**
 *  @page ADXL345_Registers
 *  @section Register Descriptions
 *	Threshold Tap reference @ref THRESS_TAP "Threshold Tap"
 *  
 */

/** 
   @page Intro

	@par Introduction to the ADXL345 
	@par
	A small, thin, ultralow power, 3-axis accelerometer with high resolution (13-bit) measurement at up to ±16 g. Digital output 
	data is formatted as 16-bit twos complement and is accessible through either a SPI (3- or 4-wire) or I 2 C digital interface.
	@par 
	The ADXL345 is well suited for mobile device applications. It measures the static acceleration of gravity in tilt-sensing applications, as well as
	dynamic acceleration resulting from motion or shock. Its high resolution (3.9 mg/LSB) enables measurement
 	of inclination changes less than 1.0°.
	@par 
	Several special sensing functions are provided. Activity and inactivity sensing detect the presence or lack of motion by comparing the acceleration
	on any axis with user-set thresholds. Tap sensing detects single and double taps in any direction. Free
	fall sensing detects if the device is falling. These functions can be mapped individually to either of
	two interrupt output pins.  An integrated, patent pending memory management system with a 32-level first in, first out (FIFO) buffer can be used to store data to minimize host
	processor activity and lower overall system power consumption.
	@par 
	Low power modes enable intelligent motion-based power
 	management with threshold sensing and active acceleration measurement at extremely low power
	dissipation.
*/


/**	
 	@page THRESH_TAP 
  
   	@par The THRESH_TAP register
   	is eight bits and holds the threshold
 	value for tap interrupts. The data format is unsigned, therefore,
 	the magnitude of the tap event is compared with the value 
 	in THRESH_TAP for normal tap detection. The scale factor is 
 	62.5 mg/LSB (that is, 0xFF = 16 g). A value of 0 may result in 
 	undesirable behavior if single tap/double tap interrupts are 
 	enabled.
 */

 /** 
    @page OFFSET

    @par The OFSX, OFSY, and OFSZ registers
    are each eight bits and offer user-set offset adjustments in twos complement format
	with a scale factor of 15.6 mg/LSB (that is, 0x7F = 2 g). The 
	value stored in the offset registers is automatically added to the 
	acceleration data, and the resulting value is stored in the output 
	data registers. For additional information regarding offset 
	calibration and the use of the offset registers, refer to the Offset 
	Calibration section.
*/

/**
  	@page DUR

  	@par The DUR register 
  	is eight bits and contains an unsigned time 
  	value representing the maximum time that an event must be 
	above the THRESH_TAP threshold to qualify as a tap event. The 
	scale factor is 625 µs/LSB. A value of 0 disables the single tap/
	double tap functions.
*/

/**
	@page LATENT

	@par The latent register 
	is eight bits and contains an unsigned time 
	value representing the wait time from the detection of a tap 
	event to the start of the time window (defined by the window 
	register) during which a possible second tap event can be detected.
	The scale factor is 1.25 ms/LSB. A value of 0 disables the double tap 
	function.
*/
