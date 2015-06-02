

#ifndef RADIO_H__
#define RADIO_H__

#include <stdint.h>
#include "hal_nrf_hw.h"
#include "hal_nrf_reg.h"

/** Defines the channel the radio should operate on */
#define RF_CHANNEL 40

/** Defines the time it takes for the radio to come up to operational mode */
#define RF_POWER_UP_DELAY 2

/** Defines the payload length the radio should use */
#define RF_PAYLOAD_LENGTH 32

/** Defines how many retransmitts that should be performed */
#define RF_RETRANSMITS 15

/**
 * @brief Defines the retransmit delay.
 *
 * Should be a multiple of 250. If the
 * RF_PAYLOAD_LENGTH is larger than 18, a higher retransmitt delay need to
 * be set. This is because both the original package and ACK payload will
 * be of this size. When the ACK payload exeedes 18 byte, it will not be able
 * to recieve the full ACK in the ordinary 250 mikroseconds, so the delay
 * will need to be increased.
 */
#if (RF_PAYLOAD_LENGTH <= 18)
#define RF_RETRANS_DELAY 250
#else
#define RF_RETRANS_DELAY 500
#endif

#define RADIO_ADDRESS 0x22, 0x33, 0x44, 0x55, 0x01

/** For turning on dynamic payload on all pipes. Sets bits 0-6 */
#define ALL_PIPES (0x3F)

/**
 * @enum Enumerates the different states the radio may
 * be in.
 */
typedef enum
{
    RF_IDLE,    /**< Radio is idle */
    RF_MAX_RT,  /**< Maximum number of retries have occured */
    RF_TX_DS,   /**< Data is sent */
    RF_RX_DR,   /**< Data recieved */
    RF_TX_AP,   /**< Ack payload recieved */
    RF_BUSY     /**< Radio is busy */
} radio_status_t;

/**
 * @enum The possible states of the system.
 */
typedef enum
{
    DEVICE_IDLE = 0, /**< The device is idle */
    DEVICE_PRX_IDLE, /**< The device will operate in @b PRX mode */
    DEVICE_PTX_IDLE, /**< The device will operate in @b PTX mode */
    DEVICE_PRX_SB,   /**< The device will operate in @b PRX mode with ShockBurst functionailty */
    DEVICE_PRX_ESB,  /**< The device will operate in @b PRX mode with Enhanced ShockBurst functionailty */
    DEVICE_PRX_PL,   /**< The device will operate in @b PRX mode with Enhanced ShockBurst functionailty with Bidirectional data */
    DEVICE_PTX_SB,   /**< The device will operate in @b PTX mode with ShockBurst functionailty */
    DEVICE_PTX_ESB,  /**< The device will operate in @b PTX mode with Enhanced ShockBurst functionailty */
    DEVICE_PTX_PL,   /**< The device will operate in @b PTX mode with Enhanced ShockBurst functionailty with Bidirectional data */
    NO_CHANGE        /**< No state change */
} radio_mode_t;


/**
 * @brief Initializes the radio in ShockBurst mode.
 *
 * This mean that there are no auto-retransmit or auto-acknowledgment enabled.
 *
 * @param operational_mode The operational mode, either @c HAL_NRF_PRX or @c HAL_NRF_PTX
 */
void radio_sb_init( hal_nrf_operation_mode_t operational_mode );

/**
 * @brief Initializes the radio in Enhanced ShockBurst mode with ACK payload.
 *
 * This mean that we enable auto-retransmit and auto-acknowledgment as in
 * Enhanced ShockBurst, and the features auto-ack payload and dynamic payload
 * width.
 *
 * @param operational_mode The operational mode, either @c HAL_NRF_PRX or @c HAL_NRF_PTX
 */
void radio_pl_init( hal_nrf_operation_mode_t operational_mode );

/**
 * @brief Initializes the radio in Enhanced ShockBurst mode.
 *
 * This mean that we enable auto-retransmit and auto-acknowledgment.
 *
 * @param operational_mode The operational mode, either @c HAL_NRF_PRX or @c HAL_NRF_PTX
 */
void radio_esb_init( hal_nrf_operation_mode_t operational_mode );


uint8_t* radio_get_address( void );

/**
 * @brief Get the current status of the radio.
 *
 * @return Status of the radio. May be one of:
 * @retval RF_IDLE   Radio is idle
 * @retval RF_MAX_RT Maximum number of retries have occured
 * @retval RF_TX_DS  The data are sent
 * @retval RF_RX_DR  Data is recieved
 * @retval RF_TX_AP  Ack payload recieved
 * @retval RF_BUSY   Radio is busy
 */
radio_status_t radio_get_status( void );

/**
 * @brief Gets the bit at position @a byte_index in @b pload.
 *
 * @param byte_index The index of the bit
 * @return The value of @b pload[byte_index]
 */
uint8_t radio_get_pload_byte( uint8_t byte_index );

/**
 * @brief This function load the data to be sent into the radio, sends it,
 * and waits for the response.
 *
 * @param packet The data to send. Maximum 2 byte
 * @param length The length of the data
*/
void radio_send_packet( uint8_t* packet, uint8_t length );


#endif