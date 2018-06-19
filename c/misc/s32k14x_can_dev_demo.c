/* ###################################################################
**     Filename    : main.c
**     Processor   : S32K14x
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "pin_mux.h"
#include "clockMan1.h"

volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */

#define SEND_MB_IDX (0u)
#define SEND_ID (0x100u)
#define RCV_MB_IDX (1u)
#define RCV_ID (0x101u)
#define DATA_LEN (8U)

typedef struct {
    uint8_t data[DATA_LEN];
    uint8_t len;
} rcv_data_t;

static volatile rcv_data_t rcv = {.len = 0};

static void delay(uint32_t cnt)
{
    volatile uint32_t i, j, k;

    for (i = 0; i < cnt; i++)
    {
        for(k = 0; k < 80; k++)
        {
            for(j = 0; j < 1000; j++)
            {
                ;
            }
        }
    }
}

static void rcv_handler(uint8_t instance, flexcan_event_type_t eventType,\
        flexcan_state_t *flexcanState)
{
    if (NULL == flexcanState)
    {
        return ;
    }

    switch (eventType)
    {
        case FLEXCAN_EVENT_RX_COMPLETE:
            if (flexcanState->mbs->mb_message != NULL\
                    && flexcanState->mbs->mb_message->dataLen <= DATA_LEN)
            {
                uint8_t len = flexcanState->mbs->mb_message->dataLen;
                
                for (uint8_t i = 0; i < len; i++)
                {
                    rcv.data[i] = flexcanState->mbs->mb_message->data[i];
                }
                rcv.len = len;
            }
            break;
        case FLEXCAN_EVENT_RXFIFO_COMPLETE:
            break;
        case FLEXCAN_EVENT_TX_COMPLETE:
            break;
        default:
            break;
    }
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
    /* 初始化系统和 TODO 外设的时钟 */
    (void)CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,\
            g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    (void)CLOCK_SYS_UpdateConfiguration(0, CLOCK_MANAGER_POLICY_FORCIBLE);

    (void)FLEXCAN_DRV_Init(INST_CANCOM2, &canCom2_State, &canCom2_InitConfig0);
    /*
     * rcv_handler 函数原型
     * void (*flexcan_callback_t)(
     *         uint8_t instance,
     *         flexcan_event_type_t eventType,
     *         flexcan_state_t *flexcanState);
     */
    FLEXCAN_DRV_InstallEventCallback(INST_CANCOM2, rcv_handler, NULL);

    /* 设置MB */
    flexcan_data_info_t tx_info = {
        .data_length = DATA_LEN,
        .msg_id_type = FLEXCAN_MSG_ID_STD,
        .enable_brs  = false,
        .fd_enable   = false,
        .fd_padding  = 0U
    };
    (void)FLEXCAN_DRV_ConfigTxMb(INST_CANCOM2, SEND_MB_IDX, &tx_info, SEND_ID);

    flexcan_data_info_t rx_info = {
        .data_length = DATA_LEN,
        .msg_id_type = FLEXCAN_MSG_ID_STD,
        .enable_brs  = false,
        .fd_enable   = false,
        .fd_padding  = 0U
    };
    (void)FLEXCAN_DRV_ConfigRxMb(INST_CANCOM2, RCV_MB_IDX, &rx_info, RCV_ID);

    /* 设置can2的端口 */
    (void)PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    /* TODO loop */
    for (;;)
    {
        if (rcv.len != 0)
        {
            status_t res = FLEXCAN_DRV_Send(INST_CANCOM2, SEND_MB_IDX,&tx_info,\
                    SEND_ID, (uint8_t*)(rcv.data));
            if (res != STATUS_SUCCESS)
            {
                exit_code = 1;
                break;
            }
            rcv.len = 0;
        }

        /* blink test */
        PINS_DRV_WritePin(PTE, 5, 0);
        PINS_DRV_WritePin(PTA, 8, 0);
        delay(500);
        PINS_DRV_WritePin(PTE, 5, 1);
        PINS_DRV_WritePin(PTA, 8, 1);
        delay(500);
    }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
