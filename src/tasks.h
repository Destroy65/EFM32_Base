/*
 * tasks.h
 *
 *  Created on: 12 de may. de 2023
 *      Author: practiques
 */

#ifndef SRC_TASKS_H_
#define SRC_TASKS_H_

#include "i2c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "motor.h"

#define STACK_SIZE_FOR_TASK    (configMINIMAL_STACK_SIZE + 10)
#define TASK_PRIORITY          (tskIDLE_PRIORITY + 1)

#define HEIGHT 10
#define WIDTH 10

#define MOTOR_DELAY_MS 3

#define DEG 3

#define DEG_PER_STEP 1.40625

QueueHandle_t xQueueRGB16;
QueueHandle_t xQueueRGB8;

void tasks_init();

void read_task(void *pParameters);
void convert_task(void *pParameters);
void write_task(void *pParameters);

#endif /* SRC_TASKS_H_ */
