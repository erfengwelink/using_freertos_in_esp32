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
const char *pcTextForPeriodicTask = "Periodic task is running\n";

void vContinuousProcessingTask( void *pvParameters )
{
char *pcTaskName;
//volatile unsigned long ul;

	/* The string to print out is passed in via the parameter.  Cast this to a
	character pointer. */
	pcTaskName = ( char * ) pvParameters;

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		printf("%s\n", pcTaskName );
		//vTaskDelay(25 / portTICK_RATE_MS);
		/* A null loop has been inserted just to slow down the rate at which
		messages are sent down the debug link to the console.  Without this
		messages print out too quickly for the debugger display and controls
		to keep up.  For clarity this null loop is not shown in the eBook text
		as it is not relevant to the behaviour being demonstrated. */
		// for( ul = 0; ul < 0xfff; ul++ )
		// {
		// 	__nop();
		// }
	}
}

void vPeriodicTask( void *pvParameters )
{
//char *pcTaskName;
//volatile unsigned long ul;
portTickType xLastWakeTime;
	/* The string to print out is passed in via the parameter.  Cast this to a
	character pointer. */
	//pcTaskName = ( char * ) pvParameters;

	/* The xLastWakeTime variable needs to be initialized with the current tick
	count.  Note that this is the only time we access this variable.  From this
	point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	API function. */
	xLastWakeTime = xTaskGetTickCount();

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		printf("%s:Periodic task is running..........\n", __func__ );

#if 0
		/* Delay for a period. */
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
			/* This loop is just a very crude delay implementation.  There is
			nothing to do in here.  Later exercises will replace this crude
			loop with a proper delay/sleep function. */
		}
#endif
		/* We want this task to execute exactly every 10 milliseconds. */
		vTaskDelayUntil( &xLastWakeTime, ( 10 / portTICK_RATE_MS ) );
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

    /* Create two instances of the continuous processing task, both at priority	1. */
	xTaskCreate( vContinuousProcessingTask, "Task 1", 1024, (void*)pcTextForTask1, 1, NULL );
	xTaskCreate( vContinuousProcessingTask, "Task 2", 1024, (void*)pcTextForTask2, 1, NULL );

	/* Create one instance of the periodic task at priority 2. */
	xTaskCreate( vPeriodicTask, "Task 3", 8*1024, (void*)pcTextForPeriodicTask, 2, NULL );

	printf("fished\n" );

}
