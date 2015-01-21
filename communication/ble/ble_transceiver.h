/******************************************************************************
 * @file    ble_transceiver.h
 * @author  Rémi Pincent - INRIA
 * @date    14 janv. 2015   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/
#ifndef BLETRANSCEIVER_H_
#define BLETRANSCEIVER_H_

#include "ble_uart_service.h"

extern "C"{
	#include "ble_conn_params.h"
}

/**
 * @class BLETransceiver
 * @brief TODO_one_line_description_of_class.
 *
 * TODO_longer_description_of_class_meant_for_users._
 * 
 */
class BLETransceiver {
private :
	/** The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
	static const uint16_t APP_ADV_INTERVAL;
	/** The advertising timeout (in units of seconds). */
	static const uint16_t APP_ADV_TIMEOUT_IN_SECONDS;
	/** Include the Service Changed characteristic in the local attributes */
	static const uint8_t IS_SRVC_CHANGED_CHARACT_PRESENT;

	/**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (15 seconds). */
	static const uint32_t FIRST_CONN_PARAMS_UPDATE_DELAY;
	/**< Time between each call to sd_ble_gap_conn_param_update after the first call (5 seconds). */
	static const uint32_t NEXT_CONN_PARAMS_UPDATE_DELAY;
	static const uint8_t MAX_CONN_PARAMS_UPDATE_COUNT;
	 /**< Name of device. Will be included in the advertising data. */
	static const char* DEVICE_NAME;

	/**< Minimum acceptable connection interval (0.5 seconds). */
	static const uint16_t MIN_CONN_INTERVAL;
	/**< Maximum acceptable connection interval (1 second). */
	static const uint16_t MAX_CONN_INTERVAL;
	/**< Slave latency. */
	static const uint16_t SLAVE_LATENCY;
	/**< Connection supervisory timeout (4 seconds). */
	static const uint16_t CONN_SUP_TIMEOUT;

	/**< Timeout for Pairing Request or Security Request (in seconds). */
	static const uint16_t SEC_PARAM_TIMEOUT;
	/**< Perform bonding. */
	static const uint8_t SEC_PARAM_BOND;
	/**< Man In The Middle protection not required. */
	static const uint8_t SEC_PARAM_MITM;
	/**< No I/O capabilities. */
	static const uint8_t SEC_PARAM_IO_CAPABILITIES;
	/**< Out Of Band data not available. */
	static const uint8_t SEC_PARAM_OOB;
	/**< Minimum encryption key size. */
	static const uint8_t SEC_PARAM_MIN_KEY_SIZE;
	/**< Maximum encryption key size. */
	static const uint8_t SEC_PARAM_MAX_KEY_SIZE;


	/**< Structure to identify the Nordic UART Service. */
    static ble_nus_t                        _m_nus;
    /**< Security requirements for this application. */
    static ble_gap_sec_params_t             m_sec_params;
    /**< Handle of the current connection. */
    static uint16_t  m_conn_handle;
public:
  BLETransceiver();
  virtual ~BLETransceiver();

  /**
   * Initialize transceiver with needed BLE services
   */
  void init(void);

  /**
   * Start BLE advertisement
   */
  static void advertise(void);

  /**
   * Send given date
   * @param arg_au8_dataToSend
   * @param arg_u8_dataLength
   * @return true if data successfully been sent
   */
  bool sendData(uint8_t* arg_au8_dataToSend, uint8_t arg_u8_dataLength);

private :

  /**@brief Function for initializing the BLE stack.
   *
   * @details Initializes the SoftDevice and the BLE event interrupt.
   */
  void bleStackInit(void);

  /**@brief Function for initializing the Advertising functionality.
   *
   * @details Encodes the required advertising data and passes it to the stack.
   *          Also builds a structure to be passed to the stack when starting advertising.
   */
  void advertisingInit(void);

  /**@brief Function for initializing the Connection Parameters module.
   */
  void connParamsInit(void);

  /**@brief Function for initializing services that will be used by the application.
   */
  void servicesInit(void);

  /**@brief Function for the GAP initialization.
   *
   * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
   *          device including the device name, appearance, and the preferred connection parameters.
   */
  void gapParamsInit(void);

  /**@brief Function for dispatching a BLE stack event to all modules with a BLE stack event handler.
   *
   * @details This function is called from the scheduler in the main loop after a BLE stack
   *          event has been received.
   *
   * @param[in]   p_ble_evt   Bluetooth stack event.
   */
  static void bleEvtDispatch(ble_evt_t * p_ble_evt);

  /**@brief Function for initializing security parameters.
   */
  void secParamsInit(void);

  /**@brief Function for handling a Connection Parameters error.
   *
   * @param[in]   nrf_error   Error code containing information about what went wrong.
   */
  static void connParamsErrorHandler(uint32_t nrf_error);

  /**@brief Function for handling the Application's BLE Stack events.
   *
   * @param[in]   p_ble_evt   Bluetooth stack event.
   */
  static void onBleEvt(ble_evt_t * p_ble_evt);

  /**@brief    Function for handling the data from the Nordic UART Service.
   *
   * @details  This function will process the data received from the Nordic UART BLE Service and send
   *           it to the UART module.
   */
  /**@snippet [Handling the data received over BLE] */
  static void onBLERX(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);

  /**@brief Function for handling the Connection Parameters Module.
   *
   * @details This function will be called for all events in the Connection Parameters Module which
   *          are passed to the application.
   *          @note All this function does is to disconnect. This could have been done by simply
   *                setting the disconnect_on_fail config parameter, but instead we use the event
   *                handler mechanism to demonstrate its use.
   *
   * @param[in]   p_evt   Event received from the Connection Parameters Module.
   */
  static void onConnParamsEvt(ble_conn_params_evt_t * p_evt);

  /**@brief Function for dispatching a system event to interested modules.
   *
   * @details This function is called from the System event interrupt handler after a system
   *          event has been received.
   *
   * @param[in]   sys_evt   System stack event.
   */
  static void sysEvtDispatch(uint32_t sys_evt);
};

#endif /* BLETRANSCEIVER_H_ */
