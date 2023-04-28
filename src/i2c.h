/*
 * i2c.h
 *
 *  Created on: 28 de abr. de 2023
 *      Author: practiques
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include <stdio.h>
#include <stdbool.h>
#include "em_i2c.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "projdefs.h"
#include "portmacro.h"
#include "queue.h"

#define RGB_R_ADDR 0b10001000
#define RGB_W_ADDR 0b10001001

//REG 1
#define CONF_1_REG 0x01
#define RGB_MODE   0x05
#define STDY_MODE  0x04
#define RES_16_BIT 0x08
#define RES_12_BIT 0x10

//REG 2 USED FOR IR COMPENSATION
#define CONF_2_REG 0x02

//REG 3
#define CONF_3_REG 0x03
#define NO_INT     0x00
#define G_INT      0x01
#define R_INT      0x02
#define B_INT  	   0x03
#define DONE_INT   0x10

//STATUS REGISTER
#define ST_REG 	   0x08
#define CONV_END   0x02

#define G_REG_L    0x09
#define G_REG_H    0x0A
#define R_REG_L    0x0B
#define R_REG_H    0x0C
#define B_REG_L    0x0D
#define B_REG_H    0x0E

void BSP_Semaphor();

void BSP_I2C_Init();
bool I2C_WriteRegister(uint8_t reg, uint8_t data);
bool I2C_ReadRegister(uint8_t reg, uint8_t *val);

bool I2C_Test();


#endif /* SRC_I2C_H_ */
