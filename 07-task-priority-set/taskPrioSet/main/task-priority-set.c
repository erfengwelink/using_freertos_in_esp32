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

#define mainDELAY_LOOP_COUNT		( 0xfffffff )

const char *pcTextForTask1 = "Task 1 is running\n";
const char *pcTextForTask2 = "Task 2 is running\n";

TaskHandle_t xTask2Handle = NULL;

void vTaskFunction1( void *pvParameters )
{
	UBaseType_t prio;
	/* Print out the name of this task. */
	//printf("%s is running!\n", __func__);
#if 1
	prio = uxTaskPriorityGet( NULL );

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		printf("About to raise the Task 2 priority\r\n" );
		vTaskDelay(250 / portTICK_RATE_MS);
		vTaskPrioritySet( xTask2Handle, ( prio + 1 ) );
	}
#endif

}

void vTaskFunction2( void *pvParameters )
{
	UBaseType_t prio;
	/* Print out the name of this task. */
	//printf("%s is running!\n", __func__);
	prio = uxTaskPriorityGet( NULL );

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		printf("About to lower the Task 2 priority\r\n" );
		vTaskDelay(250 / portTICK_RATE_MS);
		vTaskPrioritySet( NULL, ( prio - 2 ) );
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

	xTaskCreate(	vTaskFunction1,		/* Pointer to the function that implements the task. */
					"vTaskFunction",	/* Text name for the task.  This is to facilitate debugging only. */
					1024,		/* Stack depth in words. */
					(void*)pcTextForTask1,		/* We are not using the task parameter. */
					1,			/* This task will run at priority 1. */
					NULL );		/* We are not using the task handle. */

	#if 1
    xTaskCreate(	vTaskFunction2,		/* Pointer to the function that implements the task. */
					"vTaskFunction",	/* Text name for the task.  This is to facilitate debugging only. */
					1024,		/* Stack depth in words. */
					(void*)pcTextForTask2,		/* We are not using the task parameter. */
					2,			/* This task will run at priority 1. */
					&xTask2Handle );		/* We are not using the task handle. */
#endif
}
