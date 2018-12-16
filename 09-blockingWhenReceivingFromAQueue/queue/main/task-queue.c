/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

static QueueHandle_t xQueue;

static void vTaskQueueSend( void *pvParameters )
{
	int32_t lValueToSend;
	BaseType_t xStatus;
	lValueToSend = ( int32_t ) pvParameters;

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		xStatus = xQueueSendToBack( xQueue, &lValueToSend, 0 );
        if( xStatus != pdPASS )
        {
			printf( "Could not send to the queue.\r\n" );
		}
		//vTaskDelay(250 / portTICK_RATE_MS);
	}
}

static void vTaskQueueReceiver( void *pvParameters )
{
	int32_t lReceivedValue;
	BaseType_t xStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );
	for(;;)
	{
		if( uxQueueMessagesWaiting( xQueue ) != 0 )
        {
            printf( "Queue should have been empty!\r\n" );
        }

		xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );
        if( xStatus == pdPASS )
        {
			printf( "Received = %d \n", lReceivedValue );
		}
		else
		{
			printf( "Could not receive from the queue.\r\n" );
		}
		xQueueReset(xQueue);
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

	xQueue = xQueueCreate(5, sizeof(int32_t));
	if(NULL == xQueue)
	{
		printf(" xQueue allocate failed!!!\n");
		return ;
	}
    /* Create one of the two tasks. */
	xTaskCreate(	vTaskQueueSend,		/* Pointer to the function that implements the task. */
					"vTaskQueueSend1",	/* Text name for the task.  This is to facilitate debugging only. */
					1024,		/* Stack depth in words. */
					(void*)100,		/* We are not using the task parameter. */
					1,			/* This task will run at priority 1. */
					NULL );		/* We are not using the task handle. */


    xTaskCreate(	vTaskQueueSend,		/* Pointer to the function that implements the task. */
					"vTaskQueueSend2",	/* Text name for the task.  This is to facilitate debugging only. */
					1024,		/* Stack depth in words. */
					(void*)200,		/* We are not using the task parameter. */
					1,			/* This task will run at priority 1. */
					NULL );		/* We are not using the task handle. */

	

	xTaskCreate(	vTaskQueueReceiver,		/* Pointer to the function that implements the task. */
					"vTaskQueueReceiver",	/* Text name for the task.  This is to facilitate debugging only. */
					8*1024,		/* Stack depth in words. */
					(void*)NULL,		/* We are not using the task parameter. */
					2,			/* This task will run at priority 1. */
					NULL );		/* We are not using the task handle. */

}
