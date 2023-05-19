/*
 * motor.h
 *
 *  Created on: 28 de abr. de 2023
 *      Author: practiques
 */

#ifndef SRC_MOTOR_H_
#define SRC_MOTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include "em_gpio.h"

typedef struct {
	uint8_t reg;
	GPIO_Port_TypeDef port;
	uint8_t pin_start;
} Motor;

typedef enum {
	CW,
	CCW
} Direction;

Motor motor_init(GPIO_Port_TypeDef port, uint8_t pin);

void motor_step(Direction dir, Motor* mot);

#endif /* SRC_MOTOR_H_ */
