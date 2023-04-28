/*
 * motor.h
 *
 *  Created on: 28 de abr. de 2023
 *      Author: practiques
 */

#ifndef SRC_MOTOR_H_
#define SRC_MOTOR_H_

typedef struct {
	uint8_t reg;
	GPIO_Port_TypeDef port;
	uint8_t pin0;
	uint8_t pin1;
	uint8_t pin2;
	uint8_t pin3;
} Motor;

typedef enum {
	CW,
	CCW
} Direction;

Motor motor_init(GPIO_Port_TypeDef port, uint8_t pin1, uint8_t pin2, uint8_t pin3);

void motor_step_(Direction dir, Motor* mot);

#endif /* SRC_MOTOR_H_ */
