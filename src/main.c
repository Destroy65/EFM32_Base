/***************************************************************************//**
 * @file
 * @brief FreeRTOS Blink Demo for Energy Micro EFM32GG_STK3700 Starter Kit
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "croutine.h"

#include "em_chip.h"
#include "bsp.h"
#include "bsp_trace.h"

#include "sleep.h"

#include "i2c.h"
#include "tasks.h"
#include "motor.h"

int _write(int file, const char *ptr, int len) {
	int x;
	for (x = 0; x < len; ++x) {
		ITM_SendChar(*ptr++);
	}
	return len;
}

/***************************************************************************//**
 * @brief  Main function
 ******************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();
  /* If first word of user data page is non-zero, enable Energy Profiler trace */
  BSP_TraceProfilerSetup();


  /* Initialize SLEEP driver, no calbacks are used */
  SLEEP_Init(NULL, NULL);
#if (configSLEEP_MODE < 3)
  /* do not let to sleep deeper than define */
  SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t)(configSLEEP_MODE + 1));
#endif

  BSP_Semaphor();
  BSP_I2C_Init();

  if(!I2C_Test()){
	  printf("ERROR RGB module not available.");
	  return -1;
  }

  I2C_WriteRegister(CONF_1_REG, RGB_MODE);
  I2C_WriteRegister(CONF_3_REG, NO_INT);

  tasks_init();

  /*Motor mot = motor_init(gpioPortD, 0);

  motor_move_deg(&mot, CCW, 90);*/

  while(1){}


  return 0;
}
