/*
 * motor.c
 *
 *  Created on: 28 de abr. de 2023
 *      Author: practiques
 */
#include "motor.h"

Motor motor_init(GPIO_Port_TypeDef port, uint8_t pin) {
	Motor motor;

	motor.pin_start = pin;

	motor.port = port;

	for (int i = 0; i < 4; i++)
		GPIO_PinModeSet(port, i+pin, gpioModePushPull, 0);

	for (int i = 0; i < 2; i++)
		GPIO_PinOutSet(port, i+pin);

	motor.reg = 1;

	return motor;
}

void motor_step(Direction dir, Motor* mot) {
	if (dir == CCW) {
		mot->reg = ((int)mot->reg-1 < 0)? 3 : (mot->reg-1);
	}

		GPIO_PinOutToggle(mot->port, mot->pin_start+((int)mot->reg-1 < 0)? 3 : (mot->reg-1));
		GPIO_PinOutToggle(mot->port, mot->pin_start+(mot->reg+1)%4);

	if (dir == CW)
		mot->reg = (mot->reg + 1) % 4;
}
