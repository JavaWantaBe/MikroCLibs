/**
 * @file ds1307.h
 *
 * @brief DS1307 RTC Library
 *
 * @copyright AlphaLoewe
 *
 * @author Richard Lowe
 *
 * @date 06/12/2013
 *
 * @version 1.0 - Initial
 *
 * Status: 99% completed.
 *
 * @note
 * Test configuration:
 *   MCU:             %DEVICE%
 *   Dev.Board:       x
 *   Oscillator:      %DEVICE_CLOCK%
 *   Ext. Modules:    x
 *   SW:              %COMPILER%
 *  Can use either software bit banged or hardware TWI / I2C
 *
 * @par
 *   <all that matters>
 */

/**
 * The rules for DST changed in 2007 for the first time in more than 20 years.
 * The new changes were enacted by the Energy Policy Act of 2005, which extended
 * the length of DST in the interest of reducing energy consumption. The new
 * rules increased the duration of DST by about one month. DST will now be in
 * effect for 238 days, or about 65% of the year, although Congress retained the
 * right to revert to the prior law should the change prove unpopular or if
 * energy savings are not significant. At present, Daylight Saving Time in the
 * United States
 *
 * begins at 2:00 a.m. on the second Sunday of March and
 * ends at 2:00 a.m. on the first Sunday of November
 *
 * A lithium battery with 48mAh or greater will back up the DS1307 for more than 
 * 10 years in the absence of power at +25°C
 *
 */

#ifndef _DS1307_H
#define _DS1307_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define DS1307_TWI_INTERFACE
//#define DS1307_I2C_INTERFACE

/**
 * @brief 24 or 12 hour modes
 *
 */ 
#define MODE12HOUR 1
#define MODE24HOUR 2

#define JANUARY   0x01
#define FEBUARY   0x02
#define MARCH     0x03
#define APRIL     0x04
#define MAY       0x05
#define JUNE      0x06
#define JULY      0x07
#define AUGUST    0x08
#define SEPTEMBER 0x09
#define OCTOBER   0x0A
#define NOVEMBER  0x0B
#define DECEMBER  0x0C

#define MONDAY    0x01
#define TUESDAY   0x02
#define WEDNESDAY 0x03
#define THURSDAY  0x04
#define FRIDAY    0x05
#define SATURDAY  0x06
#define SUNDAY    0x07

/**
 * @brief RTC Modes
 *
 */
#define DS1307_SQW_1HZ        0x10
#define DS1307_SQW_4KHZ       0x11
#define DS1307_SQW_8KHZ       0x12
#define DS1307_SQW_32.768KHZ  0x13
#define DS1307_TGL_OUT        0x80

/**
 *  @struct holds rtc config
 *
 */
typedef struct
{
    int8_t time_zone;         /**< Local time zone                         */
    bool dst_enable;          /**< Enable or disable daylight savings time */
    uint8_t output_config;    // Configuration for output pin
    bool mode_24hour;         // Keep time in 24 or 12 hour modes
} ds1307_config_t;


/**
 *  @brief Initializes RTC
 *
 *  @param ds1307_config_t : configuration of rtc
 *
 *  @code
 *   ds1307_config_t rtc_config;
 *   rtc_config.time_zone = -8;
 *   rtc_config.dst_enable = true;
 *   rtc_config.config = DS1307_SQW_1HZ;
 *
 *   ds1307_init( &rtc_config );
 *  @endcode
 *
 *  @note
 *     Please note that the initial power-on state of all registers is not defined. Therefore, it is important
 *     to enable the oscillator (CH bit = 0) during initial configuration.
 */
void ds1307_init( ds1307_config_t* rtc_config );

/**
 *  @brief Setting Time with GMT unix timestamp
 *
 *  Sets time of RTC based on GMT Time passed to it.  Time passed is in
 *  UNIX timestamp format.
 *
 *  @param uint32_t GMTTime
 *
 */
void ds1307_set_time_GMT( uint32_t GMTTime );

/**
 *  @brief Sets time with GMT timestruct
 *
 *  @param TimeStruct* set_time - time in GMT timestruct
 *
 */
void ds1307_set_time_GMT_ts( TimeStruct* set_time );

/**
 *  @brief Sets local time with timestamp
 *
 *  @param uint32_t localtime - timestamp
 *
 */
void ds1307_set_time_local( uint32_t localTime );

/**
 *  @brief Setting Time with individual hour, min, etc.
 *
 *  Sets time of RTC based on TimeStruct passed to it.
 *
 *  @param TimeStruct* set_time
 *
 */
void ds1307_set_time_local_ts( TimeStruct* set_time );

/**
 *  @brief Get GMT time
 *
 *  @returns TimeStruct* : Pointer to timestruct
 *
 */
TimeStruct* ds1307_get_GMT_time( void );

/**
 *  @brief Get Local time
 *
 *  Returns a TimeStruct that represents gmt time with adjustment to timezone.
 *
 *  @returns TimeStruct* : Pointer to timestruct
 */
TimeStruct* ds1307_get_local_time( void );

/**
 *  @brief Get GMT Time in unix timestamp form
 *
 *  Returns the local time in the form of a TimeStruct
 *
 *  @returns TimeStruct* - Divides time into various components
 *
 */
int32_t ds1307_get_GMT_unix_time( void );

/**
 *  @brief Get Local Time in unix timestamp form
 *
 *  Returns the UNIX timestamp for GMT time.
 *
 *  @returns int32_t - UNIX timestamp
 *
 */
int32_t ds1307_get_local_unix_time( void );

/**
 *  @brief Get local Time in string format
 *
 *  Retrieves the local time string in the mode selected.  Either 12 or 24
 *  hour formatting is provided.
 *
 *  @param int mode - Mode, either 12 or 24 mode
 *
 *  @returns char* - string of formatted time
 *
 */
char* ds1307_get_local_time_str( int mode );

/**
 *  @brief Get GMT time in string format
 *
 *  Returns the GMT string formatted in the mode selected.  Either 12 or 24
 *  hour formatting is provided.
 *
 *  @param int mode - Mode, either 12 or 24 mode
 *
 *  @returns char* - string of formatted time
 *
 */
char* ds1307_get_GMT_time_str( int mode );

/**
 *  @brief Get default system time in string format
 *
 */
char* ds1307_get_system_time_str( void );

/**
 *  @brief Writes scratch ram area of ds1307
 *
 *  Writes a single byte to address provided.
 *
 *  @param uint8_t addr - address to write to
 *  @param uint8_t datam - data to write
 *
 */
void ds1307_write_ram( uint8_t addr, uint8_t datam );

/**
 *  @brief Bulk write
 *
 *  Writes to DS1307 ram in bulk fashion.
 *
 *  @param void* write_data - data to be written
 *  @param uint8_t size - size of data to be written
 *
 */
void ds1307_write_ram_bulk( void* write_data, uint8_t size );

/**
 *  @brief Reads scratch ram area of ds1307
 *
 *  Reads a single byte from provided address.
 *
 *  @param uint8_t addr - address to read from
 *
 *  @returns uint8_t - value of byte read
 *
 */
uint8_t ds1307_read_ram( uint8_t addr );

/**
 *  @brief Bulk read
 *
 *  Reads a bulk amount of memory from the storage area.  Provide a place to
 *  store it and the size to read.
 *
 *  @param void* read_data - variable to store read area
 *  @prarm uint8_t size - size of data to be read
 *
 */
void ds1307_read_ram_bulk( void* read_data, uint8_t size );

/**
 *  @brief Read address
 *
 *  @param uint8_t address - address of value stored in DS1307
 *
 *  @returns uint8_t - 1 byte value read from passed address
 *
 */
uint8_t ds1307_read_address( uint8_t address );

/**
 *  @brief Get http get http gmt string
 *
 *  This returns the HTTP formatted string used in web server and client
 *  communications.
 *
 *  @returns char* - string containing http string
 *
 */
char* ds1307_get_http_gmt_str( void );

#endif