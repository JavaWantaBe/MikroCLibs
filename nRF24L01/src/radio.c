#include "radio.h"
#include "hal_nrf.h"
#include <stdbool.h>


sbit radio_chip_select at GPIOG_ODR.B9;
sbit radio_chip_enable at GPIOG_ODR.B12;

static bool bus_initialized;


/*
 * The payload sent over the radio. Also contains the recieved data.
 * Should be read with radio_get_pload_byte().
 */
static uint8_t pload[ RF_PAYLOAD_LENGTH ];

/*
 * The current status of the radio. Should be set with radio_set_status(),
 * and read with radio_get_status().
 */
static volatile radio_status_t status;
static radio_mode_t radio_mode;

/** The address of the radio. Parameter to the radio init */
static const code uint8_t address[ HAL_NRF_AW_5BYTES ] = { RADIO_ADDRESS };


static void radio_set_status( radio_status_t new_status );
static void radio_wait( void );
static void radio_bus_init( void );

// Set status
static void radio_set_status( radio_status_t new_status )
{
    status = new_status;
}

static void radio_wait()
{
    Delay_ms( RF_POWER_UP_DELAY );
}


static void radio_bus_init()
{
    status = RF_IDLE;

    SPI2_Init_Advanced( _SPI_FPCLK_DIV64, _SPI_MASTER | _SPI_8_BIT
                        | _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION
                        | _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE
                        | _SPI_SSI_1, &_GPIO_MODULE_SPI2_PB13_14_15 );

    GPIO_Digital_Output( &GPIOG_BASE, _GPIO_PINMASK_12 | _GPIO_PINMASK_9 );
    GPIO_Digital_Input( &GPIOG_BASE, _GPIO_PINMASK_10 );  // Set input for IRQ

    // Enable clock for alternate pin functions
    RCC_APB2ENR |= ( 1 << RCC_APB2ENR );  //SYSCFGEN_bit = 1;

    // Map external interrupt on PG10
    SYSCFG_EXTICR3 |= ( 1 << EXTI81 ) | ( 1 << EXTI82 );

    EXTI_RTSR &= ~( 1 << TR10 );     // Set interrupt on Rising edge (none)

    EXTI_FTSR |= ( 1 << TR10 );      // Set Interrupt on Falling edge (PG10)

    EXTI_IMR |= ( 1 << MR10 );       // Set mask

    NVIC_IntEnable( IVT_INT_EXTI15_10 );   // Enable External interrupt

    bus_initialized = true;
    // Note: Radio - By default all IRQ sources are enabled.
}


void radio_sb_init( hal_nrf_operation_mode_t operational_mode )
{
    if( !bus_initialized  )
        radio_bus_init();
        
    switch( operational_mode )
    {
        case HAL_NRF_PRX:
            radio_mode = DEVICE_PRX_SB;
            break;
        case HAL_NRF_PTX:
            radio_mode = DEVICE_PTX_SB;
            break;
    }

    hal_nrf_close_pipe(HAL_NRF_ALL);               // First close all radio pipes
    // Pipe 0 and 1 open by default
    hal_nrf_open_pipe(HAL_NRF_PIPE0, false);       // Open pipe0, without/autoack

    hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);       // Operates in 16bits CRC mode
    hal_nrf_set_auto_retr(0, RF_RETRANS_DELAY);    // Disables auto retransmit

    hal_nrf_set_address_width(HAL_NRF_AW_5BYTES);  // 5 bytes address width
    hal_nrf_set_address(HAL_NRF_TX, address);      // Set device's addresses
    hal_nrf_set_address(HAL_NRF_PIPE0, address);   // Sets recieving address on
    // pipe0

    if(operational_mode == HAL_NRF_PTX)            // Mode depentant settings
    {
        hal_nrf_set_operation_mode(HAL_NRF_PTX);     // Enter TX mode
    }
    else
    {
        hal_nrf_set_operation_mode(HAL_NRF_PRX);     // Enter RX mode
        hal_nrf_set_rx_pload_width((uint8_t)HAL_NRF_PIPE0, RF_PAYLOAD_LENGTH);
        // Pipe0 expect
        // PAYLOAD_LENGTH byte payload
        // PAYLOAD_LENGTH in radio.h
    }

    hal_nrf_set_rf_channel(RF_CHANNEL);            // Operating on static channel
    // Defined in radio.h.
    // Frequenzy =
    //        2400 + RF_CHANNEL
    hal_nrf_set_power_mode(HAL_NRF_PWR_UP);        // Power up device

    //hal_nrf_set_datarate(HAL_NRF_1MBPS);           // Uncomment this line for
    // compatibility with nRF2401
    // and nRF24E1

    radio_wait();

    radio_set_status (RF_IDLE);                    // Radio now ready
}



void radio_pl_init( hal_nrf_operation_mode_t operational_mode )
{
    if( !bus_initialized  )
        radio_bus_init();
        
    switch( operational_mode )
    {
        case HAL_NRF_PRX:
            radio_mode = DEVICE_PRX_PL;
            break;
        case HAL_NRF_PTX:
            radio_mode = DEVICE_PTX_PL;
            break;
    }

    hal_nrf_close_pipe(HAL_NRF_ALL);               // First close all radio pipes
    // Pipe 0 and 1 open by default
    hal_nrf_open_pipe(HAL_NRF_PIPE0, true);        // Then open pipe0, w/autoack

    hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);       // Operates in 16bits CRC mode
    hal_nrf_set_auto_retr(RF_RETRANSMITS, RF_RETRANS_DELAY);
    // Enables auto retransmit.
    // 3 retrans with 250ms delay

    hal_nrf_set_address_width(HAL_NRF_AW_5BYTES);  // 5 bytes address width
    hal_nrf_set_address(HAL_NRF_TX, address);      // Set device's addresses
    hal_nrf_set_address(HAL_NRF_PIPE0, address);   // Sets recieving address on
    // pipe0

    /*****************************************************************************
     * Changed from esb/radio_esb.c                                              *
     * Enables:                                                                  *
     *  - ACK payload                                                            *
     *  - Dynamic payload width                                                  *
     *  - Dynamic ACK                                                            *
     *****************************************************************************/
    hal_nrf_enable_ack_pl();                       // Try to enable ack payload

    // When the features are locked, the FEATURE and DYNPD are read out 0x00
    // even after we have tried to enable ack payload. This mean that we need to
    // activate the features.
    if(hal_nrf_read_reg(FEATURE) == 0x00 && (hal_nrf_read_reg( DYNPD ) == 0x00))
    {
        hal_nrf_lock_unlock ();                    // Activate features
        hal_nrf_enable_ack_pl();                   // Enables payload in ack
    }

    hal_nrf_enable_dynamic_pl();                   // Enables dynamic payload
    hal_nrf_setup_dyn_pl( ALL_PIPES );             // Sets up dynamic payload on
    // all data pipes.
    /*****************************************************************************
     * End changes from esb/radio_esb.c                                          *
     *****************************************************************************/

    if(operational_mode == HAL_NRF_PTX)            // Mode depentant settings
    {
        hal_nrf_set_operation_mode(HAL_NRF_PTX);   // Enter TX mode
    }
    else
    {
        hal_nrf_set_operation_mode(HAL_NRF_PRX);   // Enter RX mode
        hal_nrf_set_rx_pload_width((uint8_t)HAL_NRF_PIPE0, RF_PAYLOAD_LENGTH);
        // Pipe0 expect
        // PAYLOAD_LENGTH byte payload
        // PAYLOAD_LENGTH in radio.h
    }

    hal_nrf_set_rf_channel(RF_CHANNEL);            // Operating on static channel
    // Defined in radio.h.
    // Frequenzy =
    //        2400 + RF_CHANNEL
    hal_nrf_set_power_mode(HAL_NRF_PWR_UP);        // Power up device

    radio_wait();

    radio_set_status (RF_IDLE);                    // Radio now ready
}


void radio_esb_init( hal_nrf_operation_mode_t operational_mode )
{
    if( !bus_initialized  )
        radio_bus_init();
        
    switch( operational_mode )
    {
        case HAL_NRF_PRX:
            radio_mode = DEVICE_PRX_ESB;
            break;
        case HAL_NRF_PTX:
            radio_mode = DEVICE_PTX_ESB;
            break;
    }

    hal_nrf_close_pipe( HAL_NRF_ALL );               // First close all radio pipes
    // Pipe 0 and 1 open by default
    hal_nrf_open_pipe( HAL_NRF_PIPE0, true );        // Then open pipe0, w/autoack
    // Changed from sb/radio_sb.c

    hal_nrf_set_crc_mode( HAL_NRF_CRC_16BIT );       // Operates in 16bits CRC mode
    hal_nrf_set_auto_retr( RF_RETRANSMITS, RF_RETRANS_DELAY );
    // Enables auto retransmit.
    // 3 retrans with 250ms delay
    // Changed from sb/radio_sb.c

    hal_nrf_set_address_width( HAL_NRF_AW_5BYTES );  // 5 bytes address width
    hal_nrf_set_address( HAL_NRF_TX, address );      // Set device's addresses
    hal_nrf_set_address( HAL_NRF_PIPE0, address );   // Sets recieving address on
    // pipe0

    if( operational_mode == HAL_NRF_PTX )            // Mode depentant settings
    {
        hal_nrf_set_operation_mode( HAL_NRF_PTX );     // Enter TX mode
    }
    else
    {
        hal_nrf_set_operation_mode( HAL_NRF_PRX );     // Enter RX mode
        hal_nrf_set_rx_pload_width( (uint8_t)HAL_NRF_PIPE0, RF_PAYLOAD_LENGTH );
        // Pipe0 expect
        // PAYLOAD_LENGTH byte payload
        // PAYLOAD_LENGTH in radio.h
    }

    hal_nrf_set_rf_channel( RF_CHANNEL );            // Operating on static channel
    // Defined in radio.h.
    // Frequenzy =
    //        2400 + RF_CHANNEL
    hal_nrf_set_power_mode( HAL_NRF_PWR_UP );        // Power up device

    radio_wait();

    radio_set_status( RF_IDLE );                     // Radio now ready
}


uint8_t* radio_get_address()
{
    return address;
}

void radio_send_packet( uint8_t* packet, uint8_t length )
{
    // trans. in progress; RF_BUSY
    radio_set_status( RF_BUSY );
    // load message into radio
    switch( radio_mode )
    {
        case DEVICE_PRX_PL:
            hal_nrf_write_ack_pload( 0, packet, length );
            break;
        case DEVICE_PTX_PL:
            hal_nrf_write_tx_pload( packet, length );
            break;
        case DEVICE_PRX_SB:
            break;
        case DEVICE_PTX_SB:
            break;
        case DEVICE_PRX_ESB:
            break;
        case DEVICE_PTX_ESB:
            break;
    }
    //hal_nrf_write_tx_pload( packet, length );
    // send packet
    chip_enable_pulse();
    // Set back to idle
    radio_set_status( RF_IDLE );
}

radio_status_t radio_get_status()
{
    return status;
}

uint8_t radio_get_pload_byte( uint8_t byte_index )
{
    return pload[ byte_index ];
}

uint8_t hal_nrf_rw( uint8_t value )
{
    return SPI2_Read( value );
}





void radio_ISR() iv IVT_INT_EXTI15_10 ics ICS_AUTO
{
    EXTI_PR |= ( 1 << PR10 );               // clear flag
    DisableInterrupts();

    switch( hal_nrf_get_clear_irq_flags() )
    {
        case ( 1 << HAL_NRF_MAX_RT ):       // Max retries reached
            //hal_nrf_flush_tx();             // flush tx fifo, avoid fifo jam
            radio_set_status( RF_MAX_RT );
            break;

        case ( 1 << HAL_NRF_TX_DS ):        // Packet sent
            radio_set_status( RF_TX_DS );
            break;

        case ( 1 << HAL_NRF_RX_DR ):        // Packet received
            /*
            while ( !hal_nrf_rx_fifo_empty() )
            {
                hal_nrf_read_rx_pload( pload );
            }
            */
            radio_set_status( RF_RX_DR );
            break;

        case ( ( 1 << HAL_NRF_RX_DR ) | ( 1 << HAL_NRF_TX_DS ) ): // Ack payload recieved
            /*
            while ( !hal_nrf_rx_fifo_empty() )
            {
                hal_nrf_read_rx_pload( pload );
            }
            */
            radio_set_status( RF_TX_AP );
            break;

        default:
            // Pretty debugging goes here
            break;
    }

    EnableInterrupts();
}