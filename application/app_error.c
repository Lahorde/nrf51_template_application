/******************************************************************************
 * @file    app_error.c 
 * @author  Rémi Pincent - INRIA
 * @date    23 janv. 2015   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

/**************************************************************************
 * Include Files
 **************************************************************************/
#include <stddef.h>
#include <string.h>
#include "app_error.h"
#include "nordic_common.h"
#include "nrf51.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/
#define MAX_LENGTH_FILENAME          128         /**< Max length of filename to copy for the debug error handler. */
#define MAX_LENGTH_ASSERTION         128         /**< Max length of assertion to copy for the debug error handler. */
#define ASSERT_CONDITION_ERROR_CODE  UINT32_MAX  /**< Error code for assertions conditions . */

/**************************************************************************
 * Type Definitions
 **************************************************************************/

/**************************************************************************
 * Variables
 **************************************************************************/

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Local Functions
 **************************************************************************/
void error_handler(uint32_t arg_u32_errorCode, uint32_t arg_u32_lineNum, const char * arg_pcs8_fileName, const char * arg_pcs8_assertion);
void on_debug_assert_handler(uint32_t arg_u32_errorCode, uint32_t arg_u32_lineNum, const char * arg_pcs8_fileName, const char * arg_pcs8_assertion);
/**************************************************************************
 * Global Functions
 **************************************************************************/
/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of error.
 *
 * @param[in] arg_u32_errorCode  Error code supplied to the handler.
 * @param[in] arg_u32_lineNum    Line number where the handler is called.
 * @param[in] arg_pcs8_fileName  Pointer to the file name.
 */
void app_error_handler(uint32_t arg_u32_errorCode, uint32_t arg_u32_lineNum, const uint8_t * arg_pcu8_fileName);

/**
 * Generic assertions i.e. when  assert() get called
 * @param arg_pcs8_fileName
 * @param arg_s32_line
 * @param arg_pcs8_func
 * @param arg_pcs8_assertion
 */
void __assert_func(const char * arg_pcs8_fileName,  int arg_s32_line, const char * arg_pcs8_func, const char * arg_pcs8_assertion);

/**************************************************************************
 * Global functions definitions
 **************************************************************************/
void app_error_handler(uint32_t arg_u32_errorCode, uint32_t arg_u32_lineNum, const uint8_t * arg_pcu8_fileName)
{
	error_handler(arg_u32_errorCode, arg_u32_lineNum, (const char*) arg_pcu8_fileName, NULL);
}

void __assert_func(const char * arg_pcs8_fileName,  int arg_s32_line, const char * arg_pcs8_func, const char * arg_pcs8_assertion)
{
	error_handler(ASSERT_CONDITION_ERROR_CODE, arg_s32_line, arg_pcs8_fileName, arg_pcs8_assertion);
}

/**************************************************************************
 * Local functions definitions
 **************************************************************************/



// WARNING - DO NOT USE THIS FUNCTION IN END PRODUCT. - WARNING
// WARNING -         FOR DEBUG PURPOSES ONLY.         - WARNING
void on_debug_assert_handler(uint32_t arg_u32_errorCode, uint32_t arg_u32_lineNum, const char * arg_pcs8_fileName, const char * arg_pcs8_assertion)
{
    // Copying parameters to static variables because parameters may not be accessible in debugger.
    static volatile uint8_t  s_file_name[MAX_LENGTH_FILENAME];
    static volatile uint8_t  s_assert_reason[MAX_LENGTH_ASSERTION];
    static volatile uint16_t s_line_num;
    static volatile uint32_t s_error_code;

    strncpy((char *)s_file_name, arg_pcs8_fileName, MAX_LENGTH_FILENAME - 1);
    s_file_name[MAX_LENGTH_FILENAME - 1] = '\0';
    strncpy((char *)s_assert_reason, (const char *)arg_pcs8_assertion, MAX_LENGTH_ASSERTION - 1);
    s_file_name[MAX_LENGTH_ASSERTION - 1] = '\0';
    s_line_num                           = arg_u32_lineNum;
    s_error_code                         = arg_u32_errorCode;
    UNUSED_VARIABLE(s_file_name);
    UNUSED_VARIABLE(s_assert_reason);
    UNUSED_VARIABLE(s_line_num);
    UNUSED_VARIABLE(s_error_code);

    // WARNING: The PRIMASK register is set to disable ALL interrups during writing the error log.
    //
    // Do not use __disable_irq() in normal operation.
    __disable_irq();

    // This function will write error code, filename, and line number to the flash.
    // In addition, the Cortex-M0 stack memory will also be written to the flash.
    //(void) ble_error_log_write(error_code, p_file_name, line_num);

    // For debug purposes, this function never returns.
    // Attach a debugger for tracing the error cause.
    for (;;)
    {
        // Do nothing.
    }
}


void error_handler(uint32_t arg_u32_errorCode, uint32_t arg_u32_lineNum, const char * arg_pcs8_fileName, const char * arg_pcs8_assertion)
{
    // This call can be used for debug purposes during application development.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
	on_debug_assert_handler(arg_u32_errorCode, arg_u32_lineNum, arg_pcs8_fileName, arg_pcs8_assertion);

    // On assert, the system can only recover with a reset.
    NVIC_SystemReset();
}

