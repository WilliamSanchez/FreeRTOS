/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

xSemaphoreHandle _mutex = NULL;

int main(void)
{
    _mutex=xSemaphoreCreateMutex();
    if(_mutex != NULL){
    
    	xTaskCreate(&vTask1, "Task 1", 1024, NULL, 1, NULL);
    	xTaskCreate(&vTask2, "Task 2", 1024, NULL, 1, NULL);
    	vTaskStartScheduler();
    
    }
    else{
       printf("Error _mutex\n\r");
    }


    return 0;
}

void vTask1(void *pvParameters)
{
    for (;;)
    {
        xSemaphoreTake(_mutex,portMAX_DELAY);
        printf("Task 1 functioning with mutex\r\n");
        xSemaphoreGive(_mutex);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2(void *pvParameters)
{
    for (;;)
    {
        xSemaphoreTake(_mutex,portMAX_DELAY);
        printf("Task 2 functioning with mutex\r\n");
        xSemaphoreGive(_mutex);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
