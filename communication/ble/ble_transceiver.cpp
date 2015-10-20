/******************************************************************************
 * @file    ble_transceiver.cpp
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
#include "ble_transceiver.h"
#include "application_config.h"
#include "logger.h"

//RP 30/10/2014.... TODO check all headers
extern "C" {
#include "softdevice_handler.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "pstorage.h"
#include "app_timer.h"
}

const uint16_t BLETransceiver::u16_appAdvInterval                 = 64;
const uint8_t BLETransceiver::IS_SRVC_CHANGED_CHARACT_PRESENT   = 0;

const uint32_t BLETransceiver::FIRST_CONN_PARAMS_UPDATE_DELAY   = APP_TIMER_TICKS(20000, APP_TIMER_PRESCALER);
const uint32_t BLETransceiver::NEXT_CONN_PARAMS_UPDATE_DELAY    = APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER);
const uint8_t BLETransceiver::MAX_CONN_PARAMS_UPDATE_COUNT      = 3;

const uint16_t BLETransceiver::u16_minConnInterval                = MSEC_TO_UNITS(100, UNIT_1_25_MS);
const uint16_t BLETransceiver::u16_maxConnInterval                = MSEC_TO_UNITS(200, UNIT_1_25_MS);
const uint16_t BLETransceiver::SLAVE_LATENCY                    = 0;
const uint16_t BLETransceiver::CONN_SUP_TIMEOUT                 = MSEC_TO_UNITS(4000, UNIT_10_MS);

const uint16_t BLETransceiver::SEC_PARAM_TIMEOUT                 = 30;
const uint8_t BLETransceiver::SEC_PARAM_BOND                     = 1;
const uint8_t BLETransceiver::SEC_PARAM_MITM                     = 0;
const uint8_t BLETransceiver::SEC_PARAM_IO_CAPABILITIES          = BLE_GAP_IO_CAPS_NONE;
const uint8_t BLETransceiver::SEC_PARAM_OOB                      = 0;
const uint8_t BLETransceiver::SEC_PARAM_MIN_KEY_SIZE             = 7;
const uint8_t BLETransceiver::SEC_PARAM_MAX_KEY_SIZE             = 16;

uint16_t  BLETransceiver::u16_connHandle                          = BLE_CONN_HANDLE_INVALID;
ble_nus_t BLETransceiver::nus;
ble_gap_sec_params_t             BLETransceiver::secParams;

#ifdef USE_PDIRECT_SDSTORAGE
/**
 * sd_flash direct handler
 */
extern void sd_flash_handler(uint32_t sys_evt);
#endif

BLETransceiver::BLETransceiver(void):as8_deviceName("ble_transceiver"),
		b_isAdvertising(false),
		b_isInitalized(false),
		p_transceiverListener(NULL)
{
}

BLETransceiver::~BLETransceiver()
{
	if(p_transceiverListener != NULL)
	{
		unregisterListener(p_transceiverListener);
	}
	b_isAdvertising = false;
	b_isInitalized = false;
}

void BLETransceiver::registerListener(IBleTransceiverListener* arg_p_transceiverListener)
{
	p_transceiverListener = arg_p_transceiverListener;
}
void BLETransceiver::unregisterListener(IBleTransceiverListener* arg_p_transceiverListener)
{
	p_transceiverListener = arg_p_transceiverListener;
}

void BLETransceiver::init(const char* arg_as8_deviceName)
{
	as8_deviceName = arg_as8_deviceName;
	init();
}

/** Initialize transceiver, after this call,
 * a connection can be established by calling
 * connect()
 * @return
 */
BLETransceiver::Error BLETransceiver::init(void)
{
	bleStackInit();
	gapParamsInit();
	servicesInit();
	advertisingInit();
	connParamsInit();
	secParamsInit();
	b_isInitalized = true;

	return NO_ERROR;
}

bool BLETransceiver::isInitialized(void)
{
	return b_isInitalized;
}

BLETransceiver::Error BLETransceiver::advertise(uint16_t arg_u16_advDuration)
{
	uint32_t             err_code;
	ble_gap_adv_params_t adv_params;
	Error loc_s8_err = GENERAL_ERROR;

	// Start advertising
	memset(&adv_params, 0, sizeof(adv_params));

	adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
	adv_params.p_peer_addr = NULL;
	adv_params.fp          = BLE_GAP_ADV_FP_ANY;
	adv_params.interval    = u16_appAdvInterval;
	adv_params.timeout     = arg_u16_advDuration;

	err_code = sd_ble_gap_adv_start(&adv_params);
	if(err_code != NRF_SUCCESS)
	{
		APP_ERROR_CHECK(err_code);
	}
	else
	{
		b_isAdvertising = true;
		loc_s8_err = NO_ERROR;
	}
	return loc_s8_err;
}

BLETransceiver::Error BLETransceiver::stopAdvertisement(void)
{
	uint32_t             err_code;
	Error loc_s8_err = GENERAL_ERROR;

	err_code = sd_ble_gap_adv_stop();
	if(err_code != NRF_SUCCESS)
	{
		APP_ERROR_CHECK(err_code);
	}
	else
	{
		b_isAdvertising = false;
		loc_s8_err = NO_ERROR;
	}
	return loc_s8_err;
}

bool BLETransceiver::isAdvertising(void)
{
	return b_isAdvertising;
}

BLETransceiver::Error BLETransceiver::connect(void)
{
	/** connection initiated by central */
	return NO_ERROR;
}

bool BLETransceiver::isConnected(void)
{
	return u16_connHandle != BLE_CONN_HANDLE_INVALID;
}

BLETransceiver::Error BLETransceiver::disconnect(void)
{
	Error loc_s8_err = GENERAL_ERROR;
	uint32_t err_code = sd_ble_gap_disconnect(u16_connHandle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);

	if(err_code == NRF_ERROR_INVALID_STATE)
	{
		loc_s8_err = INVALID_STATE;
	}
	else if(err_code == NRF_SUCCESS)
	{
		loc_s8_err = NO_ERROR;
	}
	else
	{
		loc_s8_err = GENERAL_ERROR;
		/** other error codes not handled */
		APP_ERROR_CHECK(err_code);
	}
	return loc_s8_err;
}

BLETransceiver::Error BLETransceiver::send(uint8_t& arg_u8_nb_bytes, uint8_t arg_au8_bytes_send[])
{
	uint32_t err_code;
	Error loc_s8_err = GENERAL_ERROR;

	err_code = ble_nus_send_string(&nus, arg_au8_bytes_send, arg_u8_nb_bytes);

	if(err_code == BLE_ERROR_NO_TX_BUFFERS)
	{
		/* ble tx buffer full*/
		loc_s8_err = TX_BUFFER_FULL;
		arg_u8_nb_bytes = 0;
	}
	else if(err_code == NRF_ERROR_INVALID_STATE)
	{
		loc_s8_err = INVALID_STATE;
		arg_u8_nb_bytes = 0;
	}
	else if(err_code == BLE_ERROR_GATTS_SYS_ATTR_MISSING)
	{
		loc_s8_err = INVALID_PROFILE_ATTRIBUTE;
		arg_u8_nb_bytes = 0;
	}
	else if (err_code == NRF_SUCCESS)
	{
		loc_s8_err = NO_ERROR;
	}
	else
	{
		loc_s8_err = GENERAL_ERROR;
		/** other error codes not handled */
		APP_ERROR_CHECK(err_code);
	}

	return loc_s8_err;
}

BLETransceiver::Error BLETransceiver::receive(uint8_t& arg_u8_nb_bytes, uint8_t arg_au8_bytesRead[])
{
	/** rx buffer not implemented */
	APP_ERROR_CHECK_BOOL(false);
}

void BLETransceiver::startRSSIMeasure(void)
{
	sd_ble_gap_rssi_start(u16_connHandle);
}

void BLETransceiver::stopRSSIMeasure(void)
{
	sd_ble_gap_rssi_stop(u16_connHandle);
}

void BLETransceiver::bleStackInit(void)
{
	uint32_t err_code;

	//RP new sequence initialization
	//https://devzone.nordicsemi.com/question/18132/softdevice-7071-hangs-on-sd_softdevice_enable/

	// Initialize the SoftDevice handler module. Use scheduler
	SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_SYNTH_250_PPM, USE_EVENT_SCHEDULER);

	//RP new BLE sequence initialization
	//https://devzone.nordicsemi.com/question/18132/softdevice-7071-hangs-on-sd_softdevice_enable/
	// Enable BLE stack
	ble_enable_params_t ble_enable_params;
	memset(&ble_enable_params, 0, sizeof(ble_enable_params));
	ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
	err_code = sd_ble_enable(&ble_enable_params);
	APP_ERROR_CHECK(err_code);

	// Register with the SoftDevice handler module for BLE events.
	err_code = softdevice_ble_evt_handler_set({(ble_evt_cb_t)&bleEvtDispatch, (void*) this});
	APP_ERROR_CHECK(err_code);

	// Register with the SoftDevice handler module for BLE events.
	err_code = softdevice_sys_evt_handler_set(sysEvtDispatch);
	APP_ERROR_CHECK(err_code);
}

void BLETransceiver::advertisingInit(void)
{
	uint32_t      err_code;
	ble_advdata_t advdata;
	ble_advdata_t scanrsp;
	uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

	ble_uuid_t adv_uuids[] = {{BLE_UUID_NUS_SERVICE, nus.uuid_type}};

	// Build and set advertising data
	memset(&advdata, 0, sizeof(advdata));

	advdata.name_type               = BLE_ADVDATA_FULL_NAME;
	advdata.include_appearance      = true;
	advdata.flags.size              = sizeof(flags);
	advdata.flags.p_data            = &flags;

	memset(&scanrsp, 0, sizeof(scanrsp));
	scanrsp.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
	scanrsp.uuids_complete.p_uuids  = adv_uuids;

	err_code = ble_advdata_set(&advdata, &scanrsp);
	APP_ERROR_CHECK(err_code);
}

void BLETransceiver::connParamsInit(void)
{
	uint32_t               err_code;
	ble_conn_params_init_t cp_init;

	memset(&cp_init, 0, sizeof(cp_init));

	cp_init.p_conn_params                  = NULL;
	cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
	cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
	cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
	cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
	cp_init.disconnect_on_fail             = false;
	cp_init.evt_handler                    = onConnParamsEvt;
	cp_init.error_handler                  = connParamsErrorHandler;

	err_code = ble_conn_params_init(&cp_init);
	APP_ERROR_CHECK(err_code);
}

void BLETransceiver::servicesInit(void)
{
	uint32_t         err_code;
	ble_nus_init_t   nus_init;

	memset(&nus_init, 0, sizeof(nus_init));

	nus_init.data_handler.ble_nus_data_cb = (ble_nus_data_cb_t)&onBLERX;
	nus_init.data_handler.ble_nus_data_cb_payload = (void*)this;

	/** initialize UART service */
	err_code = ble_nus_init(&nus, &nus_init);
	APP_ERROR_CHECK(err_code);
}

void BLETransceiver::gapParamsInit(void)
{
	uint32_t                err_code;
	ble_gap_conn_params_t   gap_conn_params;
	ble_gap_conn_sec_mode_t sec_mode;

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

	err_code = sd_ble_gap_device_name_set(&sec_mode,
			(const uint8_t *)as8_deviceName,
			strlen(as8_deviceName));
	APP_ERROR_CHECK(err_code);

	memset(&gap_conn_params, 0, sizeof(gap_conn_params));

	gap_conn_params.min_conn_interval = u16_minConnInterval;
	gap_conn_params.max_conn_interval = u16_maxConnInterval;
	gap_conn_params.slave_latency     = SLAVE_LATENCY;
	gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

	err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
	APP_ERROR_CHECK(err_code);
}

void BLETransceiver::bleEvtDispatch(ble_evt_t * p_ble_evt, BLETransceiver* me)
{
	ASSERT(me != NULL);
	ble_conn_params_on_ble_evt(p_ble_evt);
	ble_nus_on_ble_evt(&nus, p_ble_evt);
	me->onBleEvt(p_ble_evt);
}

void BLETransceiver::onBleEvt(ble_evt_t * p_ble_evt)
{
	uint32_t                         err_code;
	static ble_gap_evt_auth_status_t m_auth_status;
	ble_gap_enc_info_t *             p_enc_info;

	LOG_VERBOSE_LN("BLE event received : %d", p_ble_evt->header.evt_id);

	switch (p_ble_evt->header.evt_id)
	{
	case BLE_GAP_EVT_CONNECTED:
		//RP - 14/01/2015 - notify
		u16_connHandle = p_ble_evt->evt.gap_evt.conn_handle;
		b_isAdvertising = false;
		if(p_transceiverListener)
		{
			p_transceiverListener->onConnection();
		}
		break;

	case BLE_GAP_EVT_DISCONNECTED:
		u16_connHandle = BLE_CONN_HANDLE_INVALID;
		if(p_transceiverListener)
		{
			p_transceiverListener->onDisconnection();
		}
		break;

	case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
		err_code = sd_ble_gap_sec_params_reply(u16_connHandle,
				BLE_GAP_SEC_STATUS_SUCCESS,
				&secParams);
		APP_ERROR_CHECK(err_code);
		break;

	case BLE_GATTS_EVT_SYS_ATTR_MISSING:
		err_code = sd_ble_gatts_sys_attr_set(u16_connHandle, NULL, 0);
		APP_ERROR_CHECK(err_code);
		break;

	case BLE_GAP_EVT_AUTH_STATUS:
		m_auth_status = p_ble_evt->evt.gap_evt.params.auth_status;
		break;

	case BLE_GAP_EVT_SEC_INFO_REQUEST:
		p_enc_info = &m_auth_status.periph_keys.enc_info;
		if (p_enc_info->div == p_ble_evt->evt.gap_evt.params.sec_info_request.div)
		{
			err_code = sd_ble_gap_sec_info_reply(u16_connHandle, p_enc_info, NULL);
			APP_ERROR_CHECK(err_code);
		}
		else
		{
			// No keys found for this device
			err_code = sd_ble_gap_sec_info_reply(u16_connHandle, NULL, NULL);
			APP_ERROR_CHECK(err_code);
		}
		break;

	case BLE_GAP_EVT_TIMEOUT:
		if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISEMENT)
		{
			//RP - 14/01/2015 - notify

			// Configure buttons with sense level low as wakeup source.

			//RP do not power off
			//                nrf_gpio_cfg_sense_input(WAKEUP_BUTTON_PIN,
			//                                         BUTTON_PULL,
			//                                         NRF_GPIO_PIN_SENSE_LOW);

			// Go to system-off mode (this function will not return; wakeup will cause a reset)
			//                err_code = sd_power_system_off();
			//                APP_ERROR_CHECK(err_code);
		}
		break;

	case BLE_GAP_EVT_RSSI_CHANGED:
		if(p_transceiverListener)
		{
			p_transceiverListener->onRSSIChange(p_ble_evt->evt.gap_evt.params.rssi_changed.rssi);
		}
		break;

	default:
		// No implementation needed.
		break;
	}
}

void BLETransceiver::dataReceived(uint8_t * p_data, uint16_t length)
{
	if(p_transceiverListener)
	{
		p_transceiverListener->onDataReceived(length, p_data);
	}
}

void BLETransceiver::secParamsInit(void)
{
	secParams.timeout      = SEC_PARAM_TIMEOUT;
	secParams.bond         = SEC_PARAM_BOND;
	secParams.mitm         = SEC_PARAM_MITM;
	secParams.io_caps      = SEC_PARAM_IO_CAPABILITIES;
	secParams.oob          = SEC_PARAM_OOB;
	secParams.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
	secParams.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
}

void BLETransceiver::onConnParamsEvt(ble_conn_params_evt_t * p_evt)
{
	uint32_t err_code;

	if(p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
	{
		err_code = sd_ble_gap_disconnect(u16_connHandle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
		APP_ERROR_CHECK(err_code);
	}
}

inline void BLETransceiver::onBLERX(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length, BLETransceiver* me)
{
	ASSERT(me != NULL);
	me->dataReceived(p_data, length);
}

void BLETransceiver::connParamsErrorHandler(uint32_t nrf_error)
{
	APP_ERROR_HANDLER(nrf_error);
}

void BLETransceiver::sysEvtDispatch(uint32_t sys_evt)
{
	if(USE_PSTORAGE)
	{
		pstorage_sys_event_handler(sys_evt);
	}
	else if(USE_PDIRECT_SDSTORAGE)
	{
		sd_flash_handler(sys_evt);
	}
}

