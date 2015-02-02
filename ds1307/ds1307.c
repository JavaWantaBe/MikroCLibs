#include "ds1307.h"

#define SECONDS   0x7F
#define MINUTES   0x7F
#define HOURS     0x3F
#define DAY       0x07
#define DATE      0x3F
#define MONTH     0x1F
#define YEAR      0xFF
#define CONTROL   0x93

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


#define DS1307_SECONDS_ADDR   0x00
#define DS1307_MINUTES_ADDR   0x01
#define DS1307_HOURS_ADDR     0x02
#define DS1307_DAY_ADDR       0x03
#define DS1307_DATE_ADDR      0x04
#define DS1307_MONTH_ADDR     0x05
#define DS1307_YEAR_ADDR      0x06
#define DS1307_CONFIG_ADDR    0x07

#define DS1307_ACK       1
#define DS1307_NO_ACK    0

#define DS1307_24_12_BIT 6
#define DS1307_OSC_BIT   7
#define DS1307_AMPM_BIT  0x20

#ifdef DS1307_I2C_INTERFACE
#define RTC_ADDR         0x68
#define DS1307_READ      0
#define DS1307_WRITE     1
#endif

#ifdef DS1307_TWI_INTERFACE
#define DS1307_WRITE     0xD0
#define DS1307_READ      0xD1
#endif


/*********************
 *  Globals
 ********************/
static const code uint8_t* days_week[] =
{      
    "Sun","Mon","Tue","Wed","Thu","Fri","Sat"
};

static const code uint8_t* months_year[] = 
{
    "NULL","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};


static uint8_t hour24_enable;
static uint8_t dst_enable;
static int8_t  time_zone;


/****************************************
 *  Private Prototypes
 ***************************************/
static void ds1307_set_dst( uint16_t year );
static void rtc_write( uint8_t address, uint8_t value );
static uint8_t rtc_read( uint8_t address );
static void rtc_write_time( TimeStruct* time );
static void rtc_read_time( TimeStruct* time );


/*
 * USA starting in 2007, extended DST to begin
 * on the 2nd Sunday in March (Day of month = 14 - (1+Y*5/4)mod 7) and
 * end on the 1st Sunday in November (Day of month = 7 - (1 + 5*Y/4) mod 7).
 */
static void ds1307_set_dst( uint16_t year, TimeStruct* start, TimeStruct* end )
{
    start->ss = 0;
    start->mn = 0;
    start->hh = 2;
    start->wd = SUNDAY;
    start->md = 14 - ( ( 1 + ( ( year * 5 ) / 4 ) ) % 7 );
    start->mo = MARCH;
    start->yy = year;

    end->ss  = 0;
    end->mn  = 0;
    end->hh  = 2;
    end->wd  = SUNDAY;
    end->md  = 7 - ( ( 1 + ( ( year * 5 ) / 4 ) ) % 7 );
    end->mo  = NOVEMBER;
    end->yy  = year;
}


static void rtc_write( uint8_t address, uint8_t value )
{
    //if( cfg & 0x6C ) return;
    #ifdef DS1307_I2C_INTERFACE
    char write_buffer[2] = {0};
    
    I2C_Start();
    #endif
    
    #ifdef DS1307_TWI_INTERFACE
    TWI_Start();
    TWI_Write( DS1307_WRITE );
    TWI_Write( address );
    TWI_Write( value );
    TWI_Stop();
    #endif
}

static uint8_t rtc_read( uint8_t address )
{
    uint8_t read_data;
    
    #ifdef DS1307_TWI_INTERFACE
    TWI_Start();
    TWI_Write( DS1307_WRITE );
    TWI_Write( address );  // Start from address 0x00
    TWI_Start();
    TWI_Write( DS1307_READ );
    read_data = TWI_Read( DS1307_NO_ACK ); // Read address 0x02
    TWI_Stop();
    #endif

    #ifdef DS1307_I2C_INTERFACE
    I2C_Start();
    #endif
    
    return read_data;
}


static void rtc_write_time( TimeStruct* time )
{
    #ifdef DS1307_I2C_INTERFACE
    char write_buffer[2] = {0};
    
    I2C_Start();
    #endif

    #ifdef DS1307_TWI_INTERFACE
    TWI_Start();
    TWI_Write( DS1307_WRITE );
    TWI_Write( DS1307_SECONDS_ADDR );
    TWI_Write( time->ss );
    TWI_Write( time->mn );
    TWI_Write( time->hh );
    TWI_Write( time->wd );
    TWI_Write( time->md );
    TWI_Write( time->mn );
    TWI_Write( ( uint8_t )time->yy );
    TWI_Stop();
    #endif
}

static void rtc_read_time( TimeStruct* time )
{
    #ifdef DS1307_I2C_INTERFACE
    char write_buffer[2] = {0};

    I2C_Start();
    #endif

    #ifdef DS1307_TWI_INTERFACE
    TWI_Start();
    TWI_Write( DS1307_WRITE );
    TWI_Write( DS1307_SECONDS_ADDR );
    TWI_Start();
    TWI_Write( DS1307_READ );
    time->ss = TWI_Read( DS1307_ACK );
    time->mn = TWI_Read( DS1307_ACK );
    time->hh = TWI_Read( DS1307_ACK );
    time->wd = TWI_Read( DS1307_ACK );
    time->md = TWI_Read( DS1307_ACK );
    time->mn = TWI_Read( DS1307_ACK );
    time->yy = TWI_Read( DS1307_NO_ACK );
    TWI_Stop();
    #endif
}






/****************************************
 *  Functions
 ***************************************/
// Initialization
void ds1307_init( ds1307_config_t* rtc_config )
{
    if( rtc_config->time_zone >= -16 && rtc_config->time_zone <= 16 )
    {
        char config = 0, hours = 0;

        hour24_enable = rtc_config->hour24_enable;
        dst_enable    = rtc_config->dst_enable;
        time_zone     = rtc_config->time_zone;
        
        config = rtc_read( DS1307_SECONDS_ADDR );
        hours  = rtc_read( DS1307_HOURS_ADDR );

        /* This (if) checks condition of the clock enable.  When the rtc is
         * initialized for the first time, or after the battery is replaced, 
         * the state of the RTC register is undertermined.
         */
        if( config & 0x80 )
        {
            /*
              Algorithm:
                 1. Initialize TWI communication
                 2. Send read command
                 3. Start from address 0 by sending address
                 4. Reset 8th bit to 0 to start OSC
                 5. Write configuration
                 6. Stop
            */
            config &= ~( 1 << DS1307_OSC_BIT );
            rtc_write( DS1307_SECONDS_ADDR, config );
        }
        
        /*
          This checks the 24 hour bit is set and the mode is set to 12 hour
          setting will change.
        */
        if( hours & ( 1 << DS1307_24_12_BIT ) && !hour24_enable )
        {
            hours &= ~( 1 << DS1307_24_12_BIT );
            rtc_write( DS1307_HOURS_ADDR, hours );
        } 
        else if( ( !( hours & ( 1 << DS1307_24_12_BIT ) ) ) && hour24_enable )
        {
            hours |= ( 1 << DS1307_24_12_BIT );
            rtc_write( DS1307_HOURS_ADDR, hours );
        }
        
        if( rtc_config->output_mode )
        {
            rtc_write( DS1307_CONFIG_ADDR, rtc_config->output_mode );
        }
    }
}


/**************************
   GMT Functions
**************************/

// Set GMT time based on UNIX timestamp
void ds1307_set_time_GMT( uint32_t GMTTime )
{
    TimeStruct ts;
    
    Time_epochToDate( GMTTime, &ts );
    ds1307_set_time_GMT_ts( &ts );
}

// Sets GMT time based on TimeStruct
void ds1307_set_time_GMT_ts( TimeStruct* set_time )
{
    TimeStruct time;

    if( !hour24_enable )
    {
        // bit 5 is the AM/PM bit with logic high being PM
        if( set_time->hh >= 12 )
        {
            if( set_time->hh == 12 )
            {
                time.hh = ( Dec2Bcd( set_time->hh ) | ( 1 << DS1307_AMPM_BIT ) );
            }
            else
            {
                time.hh = ( Dec2Bcd( set_time->hh - 12 ) | ( 1 << DS1307_AMPM_BIT ) );
            }
        }
        else
        {
            time.hh = ( Dec2Bcd( set_time->hh ) & HOURS );
            time.hh &= ~( 1 << DS1307_AMPM_BIT );
        }
    }
    else
    {
        time.hh = Dec2Bcd( set_time->hh ) & HOURS;
    }
    
    /* We need to add one to the day that is stored in the TimeStruct
       because the TimeStruct count begins at 0 while the DS1307 starts
       at 1. Likewise, we need to subtract 2000 from the TS because it
       stores years in 2 digit format*/
    time.ss = Dec2Bcd( set_time->ss ) & SECONDS;
    time.mn = Dec2Bcd( set_time->mn ) & MINUTES;
    
    time.wd = ( Dec2Bcd( set_time->wd ) & DAY ) + 1;
    time.md = Dec2Bcd( set_time->md ) & DATE;
    time.mn = Dec2Bcd( set_time->mo ) & MONTH;
    time.yy = ( uint16_t )Dec2Bcd( ( uint8_t )set_time->yy - 2000 ) & YEAR;
    
    rtc_write_time( &time );
}

// Get GMT time and returns TimeStruct
void ds1307_get_GMT_time( TimeStruct* ts )
{
    TimeStruct time;

    rtc_read_time( &time );

    // When high, the 12-hour mode is selected.
    if( !hour24_enable )
    {
        if( rtc_read( DS1307_HOURS_ADDR ) & ( 1 << DS1307_AMPM_BIT ) )
        {
           ts->hh = ( uint8_t )Bcd2Dec( time.hh & 0x1F );
           ts->hh += 12;
        }
        else
        {
           ts->hh = ( uint8_t )Bcd2Dec( time.hh & 0x1F );

           if( ts->hh == 12 )
           {
               ts->hh = 0;
           }

        }
    }

    ts->ss = ( uint8_t )Bcd2Dec( time.ss & SECONDS );
    ts->mn = ( uint8_t )Bcd2Dec( time.mn & MINUTES );
    ts->wd = ( uint8_t )Bcd2Dec( time.wd & DAY ) - 1;
    ts->md = ( uint8_t )Bcd2Dec( time.wd & DATE );
    ts->mo = ( uint8_t )Bcd2Dec( time.mn & MONTH );
    ts->yy = ( uint16_t )Bcd2Dec( time.yy & YEAR ) + 2000;

    return;
}

/*
 *  Gets gmt unix timestamp
 */
int32_t ds1307_get_GMT_unix_time()
{
    TimeStruct ts;
    
    ds1307_get_GMT_time( &ts );

    return Time_dateToEpoch( &ts );
}





/*************************
   Local Time Functions
*************************/

void ds1307_set_time_local( uint32_t localTime )
{
    TimeStruct set_time;
    
    Time_epochToDate( localTime, &set_time );
    ds1307_set_time_local_ts( &set_time );
}


void ds1307_set_time_local_ts( TimeStruct* set_time )
{
    TimeStruct time;

    //TODO need to compensate for daylight savings time
    if( dst_enable )
    {

    }
    else
    {
    
    }
    
    rtc_write_time( &time );

}


void ds1307_get_local_time( TimeStruct* ts )
{
    uint32_t time = ds1307_get_GMT_unix_time();
    
    
    if( dst_enable )
    {
    
    }
    
    time += time_zone * Time_secInH;
    
    return;
}


int32_t ds1307_get_local_unix_time()
{
   TimeStruct ts;
   ds1307_get_local_time( &ts );
   
   return Time_dateToEpoch( &ts );
}








/*
 *  Return formatted time
 *
 */
char* ds1307_get_local_time_str( int mode )
{
    static char formated_local_time[20];
    TimeStruct tmp_time;

    ds1307_get_local_time( &tmp_time );

    if( mode == MODE12HOUR )
    {
        uint8_t x;

        x = tmp_time.hh;

        if( x == 0 )
        {
            x += 12;
        }

        else if( x >= 13 )
        {
            x -= 12;
        }

        formated_local_time[0] = ( x / 10 ) + 48;
        formated_local_time[1] = ( x % 10 ) + 48;
    }
    else
    {
        formated_local_time[0] = ( tmp_time.hh / 10 ) + 48;
        formated_local_time[1] = ( tmp_time.hh % 10 ) + 48;
    }

    formated_local_time[2] = ':';
    formated_local_time[3] = ( tmp_time.mn / 10 ) + 48;
    formated_local_time[4] = ( tmp_time.mn % 10 ) + 48;
    formated_local_time[5] = ':';
    formated_local_time[6] = ( tmp_time.ss / 10 ) + 48;
    formated_local_time[7] = ( tmp_time.ss % 10 ) + 48;
    formated_local_time[8] = ' ';
    formated_local_time[9] = ( tmp_time.mo / 10 ) + 48;
    formated_local_time[10] = ( tmp_time.mo % 10 ) + 48;
    formated_local_time[11] = '/';
    formated_local_time[12] = ( tmp_time.md / 10 ) + 48;
    formated_local_time[13] = ( tmp_time.md % 10 ) + 48;
    formated_local_time[14] = '/';
    formated_local_time[15] = ( tmp_time.yy / 1000 ) + 48;
    formated_local_time[16] = ( ( tmp_time.yy % 1000 ) / 100 ) + 48;
    formated_local_time[17] = ( ( tmp_time.yy % 100 ) / 10 ) + 48;
    formated_local_time[18] = ( tmp_time.yy % 10 ) + 48;
    formated_local_time[19] = '\0';

    return formated_local_time;

}

/*
 */
char* ds1307_get_GMT_time_str( int mode )
{
    static char formated_gmt_time[20];
    TimeStruct tmp_time;

    ds1307_get_GMT_time( &tmp_time );

    if( mode == MODE12HOUR )
    {
        uint8_t x;

        x = tmp_time.hh;

        if( x == 0 )
        {
            x += 12;
        }

        else if( x >= 13 )
        {
            x -= 12;
        }

        formated_gmt_time[0] = ( x / 10 ) + 48;
        formated_gmt_time[1] = ( x % 10 ) + 48;
    }
    else
    {
        formated_gmt_time[0] = ( tmp_time.hh / 10 ) + 48;
        formated_gmt_time[1] = ( tmp_time.hh % 10 ) + 48;
    }

    formated_gmt_time[2] = ':';
    formated_gmt_time[3] = ( tmp_time.mn / 10 ) + 48;
    formated_gmt_time[4] = ( tmp_time.mn % 10 ) + 48;
    formated_gmt_time[5] = ':';
    formated_gmt_time[6] = ( tmp_time.ss / 10 ) + 48;
    formated_gmt_time[7] = ( tmp_time.ss % 10 ) + 48;
    formated_gmt_time[8] = ' ';
    formated_gmt_time[9] = ( tmp_time.mo / 10 ) + 48;
    formated_gmt_time[10] = ( tmp_time.mo % 10 ) + 48;
    formated_gmt_time[11] = '/';
    formated_gmt_time[12] = ( tmp_time.md / 10 ) + 48;
    formated_gmt_time[13] = ( tmp_time.md % 10 ) + 48;
    formated_gmt_time[14] = '/';
    formated_gmt_time[15] = ( tmp_time.yy / 1000 ) + 48;
    formated_gmt_time[16] = ( ( tmp_time.yy % 1000 ) / 100 ) + 48;
    formated_gmt_time[17] = ( ( tmp_time.yy % 100 ) / 10 ) + 48;
    formated_gmt_time[18] = ( tmp_time.yy % 10 ) + 48;
    formated_gmt_time[19] = '\0';

    return formated_gmt_time;
}

/*
 *
 */
char* ds1307_get_system_time_str()
{
   return ds1307_get_local_time_str( MODE24HOUR );
}


/************************
    Ram Functions
************************/


/*
 *
 */
void ds1307_write_ram( uint8_t addr, uint8_t datam )
{
    if( addr < RAM_START || addr > RAM_END )
    {
       return;
    }
    
    rtc_write( addr, datam );
}

// Bulk write
void ds1307_write_ram_bulk( void* write_data, uint8_t size )
{
    if( size > RAM_SIZE )
    {
       return;
    }
    else
    {
        #ifdef DS1307_I2C_INTERFACE
        uint8_t write_buffer[57];
        #endif
        
        uint8_t* p_data = ( uint8_t* )write_data;
        uint8_t start_addr = 0x08;
        
        #ifdef DS1307_TWI_INTERFACE
        
        TWI_Start();
        TWI_Write( DS1307_WRITE );
        TWI_Write( start_addr );
        
        while( size-- > 0 )
        {
            TWI_Write( *p_data++ );
        }
        
        TWI_Stop();
        
        #endif
        
        #ifdef DS1307_I2C_INTERFACE

        write_buffer[0] = start_addr;
        memcpy( &write_buffer[1], p_data, size );
        I2C_Start();
        I2C_Write( RTC_ADDR, write_buffer, ( size + 1 ), END_MODE_STOP );
        
        #endif
    }
}

/*
 *
 */
uint8_t ds1307_read_ram( uint8_t addr )
{
    return ( addr < RAM_START || addr > RAM_END ) ? 0 : rtc_read( addr );
}

// Bulk read
void ds1307_read_ram_bulk( void* read_data, uint8_t size )
{
    if( size < RAM_SIZE )
    {
        uint8_t* p_data = ( uint8_t* )read_data;
        uint8_t start_addr = RAM_START;

        #ifdef DS1307_TWI_INTERFACE
        
        TWI_Start();
        TWI_Write( DS1307_WRITE );
        TWI_Write( start_addr );
        TWI_Start();
        TWI_Write( DS1307_READ );

        while( size-- > 1 )
        {
            *p_data++ = TWI_Read( DS1307_ACK );
        }
        
        *p_data = TWI_Read( DS1307_NO_ACK );
        TWI_Stop();
        
        #endif
        
        #ifdef DS1307_I2C_INTERFACE

        I2C_Start();
        I2C_Write( RTC_ADDR, &start_addr, 1, END_MODE_RESTART );
        I2C_Read( RTC_ADDR, p_data, size, END_MODE_STOP );

        #endif
    }
}


char* ds1307_get_http_gmt_str()
{
    //Tue, 15 Nov 1994 08:12:31 GMT
    TimeStruct ts1;
    static char gmt_str[30];

    ds1307_get_GMT_time( &ts1 );

    gmt_str[0]  = days_week[ts1.wd][0];
    gmt_str[1]  = days_week[ts1.wd][1];
    gmt_str[2]  = days_week[ts1.wd][2];
    gmt_str[3]  = ',';
    gmt_str[4]  = ' ';
    gmt_str[5]  = ( ts1.md / 10 ) + 48;
    gmt_str[6]  = ( ts1.md % 10 ) + 48;
    gmt_str[7]  = ' ';
    gmt_str[8]  = months_year[ts1.mo][0];
    gmt_str[9]  = months_year[ts1.mo][1];
    gmt_str[10] = months_year[ts1.mo][2];
    gmt_str[11] = ' ';
    gmt_str[12] = ( ts1.yy / 1000 ) + 48;
    gmt_str[13] = ( ( ts1.yy % 1000 ) / 100 ) + 48;
    gmt_str[14] = ( ( ts1.yy % 100 ) / 10 ) + 48;
    gmt_str[15] = ( ts1.yy % 10 ) + 48;
    gmt_str[16] = ' ';
    gmt_str[17] = ( ts1.hh / 10 ) + 48;
    gmt_str[18] = ( ts1.hh % 10 ) + 48;
    gmt_str[19] = ':';
    gmt_str[20] = ( ts1.mn / 10 ) + 48;
    gmt_str[21] = ( ts1.mn % 10 ) + 48;
    gmt_str[22] = ':';
    gmt_str[23] = ( ts1.ss / 10 ) + 48;
    gmt_str[24] = ( ts1.ss % 10 ) + 48;
    gmt_str[25] = ' ';
    gmt_str[26] = 'G';
    gmt_str[27] = 'M';
    gmt_str[28] = 'T';
    gmt_str[29] = '\0';

    return gmt_str;
}