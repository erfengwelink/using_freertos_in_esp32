/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define mainDELAY_LOOP_COUNT (0xfffffff)

TaskHandle_t xTask2Handle = NULL;

void vTaskFunc2(void *pvParameters)
{
	printf("Task 2 is running & about to delete it by itself!\n");
	vTaskDelete(xTask2Handle);
}

void vTaskFunc1(void *pvParameters)
{
	const TickType_t xDelay100ms = pdMS_TO_TICKS(100UL);

	for (;;)
	{

		/* Print out the name of this task. */
		printf("Task 1 is running\r\n");
		/* Create task 2 at a higher priority.  Again the task parameter is not
			used so is set to NULL - BUT this time the task handle is required so
			the address of xTask2Handle is passed as the last parameter. */
		xTaskCreate(vTaskFunc2, "Task 2", 1000, NULL, 2, &xTask2Handle);
		/* The task handle is the last parameter _____^^^^^^^^^^^^^ */
		/* Task 2 has/had the higher priority, so for Task 1 to reach here Task 2
			must have already executed and deleted itself.  Delay for 100
			milliseconds. */
		vTaskDelay(xDelay100ms);
	}
}

void app_main()
{
	printf("Hello world!\n");

	/* Print chip information */
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);
	printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
		   chip_info.cores,
		   (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
		   (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

	printf("silicon revision %d, ", chip_info.revision);

	printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
		   (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded\n" : "external\n");

	/* Create one of the two tasks. */
	xTaskCreate(vTaskFunc1,   /* Pointer to the function that implements the task. */
				"vTaskFunction", /* Text name for the task.  This is to facilitate debugging only. */
				1024,			 /* Stack depth in words. */
				(void *)NULL,	/* We are not using the task parameter. */
				1,				 /* This task will run at priority 1. */
				NULL);			 /* We are not using the task handle. */

}
