/*
 * tasks.c
 *
 *  Created on: 12 de may. de 2023
 *      Author: practiques
 */
#include "tasks.h"

typedef struct {
	uint16_t R;
	uint16_t G;
	uint16_t B;
}RGB16;

typedef struct {
	uint8_t R;
	uint8_t G;
	uint8_t B;
}RGB8;

typedef struct {
	Motor base;
	Motor arm;
} MoveParams_t;

void tasks_init() {
	xQueueRGB16 = xQueueCreate(10, sizeof(RGB16));
	xQueueRGB8 = xQueueCreate(20, sizeof(RGB8));

	Motor base = motor_init(gpioPortD, 0);
	Motor arm = motor_init(gpioPortD, 4);

	static MoveParams_t motors;

	motors.arm = arm;
	motors.base = base;

	SLEEP_Init(NULL, NULL);
	#if (configSLEEP_MODE < 3)
	  /* do not let to sleep deeper than define */
	  SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t)(configSLEEP_MODE + 1));
	#endif

	xTaskCreate(move_task, (const char *) "MOVE", STACK_SIZE_FOR_TASK, (void*)&motors, TASK_PRIORITY, NULL);
	xTaskCreate(convert_task, (const char *) "CONVERT", STACK_SIZE_FOR_TASK, NULL, TASK_PRIORITY, NULL);
	xTaskCreate(write_task, (const char *) "WRITE", STACK_SIZE_FOR_TASK, NULL, TASK_PRIORITY, NULL);
	vTaskStartScheduler();
}

void motor_move_deg(Motor *mot, Direction dir, int degrees) {
	for (size_t i = 0; i < (int)degrees/DEG_PER_STEP; i++) {
		motor_step(dir, mot);
		vTaskDelay(pdMS_TO_TICKS(MOTOR_DELAY_MS));
	}
}

void read_sensor() {
	uint8_t st;
	uint16_t r,g,b;
	//I2C_WriteRegister(CONF_1_REG, RGB_MODE | RES_16_BIT);
	do {I2C_ReadRegister(ST_REG, &st);} while(!(st&CONV_END));
	//I2C_WriteRegister(CONF_1_REG, STDY_MODE | RES_16_BIT);

	I2C_ReadRegister(R_REG_L, ((uint8_t*)&r));
	I2C_ReadRegister(R_REG_H, ((uint8_t*)&r) + 1);

	I2C_ReadRegister(G_REG_L, ((uint8_t*)&g));
	I2C_ReadRegister(G_REG_H, ((uint8_t*)&g) + 1);

	I2C_ReadRegister(B_REG_L, ((uint8_t*)&b));
	I2C_ReadRegister(B_REG_H, ((uint8_t*)&b) + 1);

	RGB16 rgb;
	rgb.R = r;
	rgb.G = g;
	rgb.B = b;

	xQueueSend(xQueueRGB16, &rgb, portMAX_DELAY);
}

void move_task(void *pParameters) {
	MoveParams_t* motors = (MoveParams_t*)pParameters;
	Direction dir = CCW;
	
	for (size_t i = 0; i < HEIGHT; i++) {
		for (size_t j = 0; j < WIDTH; j++)	{
			read_sensor();
			motor_move_deg(&motors->base, dir, DEG);
		}
		motor_move_deg(&motors->arm, CW, DEG);
		dir = dir == CCW ? CW : CCW;
	}
}

uint8_t rgb16_to_rgb8(uint16_t pix, float coeff) {
	uint16_t aux = ((float)pix*coeff)/65535*255;
	return aux > 255 ? 255 : aux;
}

void convert_task(void *pParameters) {
	while(1) {
		RGB16 rgb16;
		xQueueReceive(xQueueRGB16, &rgb16, portMAX_DELAY);

		RGB8 rgb8;
		rgb8.R = rgb16_to_rgb8(rgb16.R/*(1/(((1+rgb16.B+rgb16.G)/2)/(65535/2)))*/, 1.0f);
		rgb8.G = rgb16_to_rgb8(rgb16.G/*(1/(((1+rgb16.B+rgb16.R)/2)/(65535/2)))*/, 1.0f);
		rgb8.B = rgb16_to_rgb8(rgb16.B/*(1/(((1+rgb16.R+rgb16.G)/2)/(65535/2)))*/, 1.0f);

		xQueueSend(xQueueRGB8, &rgb8, portMAX_DELAY);
	}
}

void write_task(void *pParameters) {
	while(1) {
		RGB8 rgb;
		xQueueReceive(xQueueRGB8, &rgb, portMAX_DELAY);
		printf("%d %d %d\n", rgb.R, rgb.G, rgb.B);
	}
}
