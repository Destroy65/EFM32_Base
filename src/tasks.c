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

void tasks_init() {
	xQueueRGB16 = xQueueCreate(10, sizeof(RGB16));
	xQueueRGB8 = xQueueCreate(20, sizeof(RGB8));
	xTaskCreate(read_task, (const char *) "READ", STACK_SIZE_FOR_TASK, NULL, TASK_PRIORITY, NULL);
	xTaskCreate(convert_task, (const char *) "CONVERT", STACK_SIZE_FOR_TASK, NULL, TASK_PRIORITY, NULL);
	xTaskCreate(write_task, (const char *) "WRITE", STACK_SIZE_FOR_TASK, NULL, TASK_PRIORITY, NULL);
	vTaskStartScheduler();
}

void read_task(void *pParameters) {
	while(1) {
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

}

void convert_task(void *pParameters) {
	while(1) {
		RGB16 rgb16;
		xQueueReceive(xQueueRGB16, &rgb16, portMAX_DELAY);

		RGB8 rgb8;
		rgb8.R = ((float)rgb16.R*4.04)/65535*255 <= 255 ? ((float)rgb16.R*4.04)/65535*255 : 255;
		rgb8.G = ((float)rgb16.G*3)/65535*255 <= 255 ? ((float)rgb16.G*3)/65535*255 : 255;
		rgb8.B = ((float)rgb16.B*5.54)/65535*255 <= 255 ? ((float)rgb16.B*5.54)/65535*255 : 255;

		xQueueSend(xQueueRGB8, &rgb8, portMAX_DELAY);
	}

}

void write_task(void *pParameters) {
	while(1) {
		RGB8 rgb;
		xQueueReceive(xQueueRGB8, &rgb, portMAX_DELAY);
		printf("R%d\tG%d\tB%d\n", rgb.R, rgb.G, rgb.B);
	}
}
