/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

void sender(void *pvParameters);
void readqueue(void *pvParameters);

xQueueHandle _queue = NULL;

int main(void)
{
    _queue=xQueueCreate(14,sizeof(char));
    if(_queue != NULL){
    
    	xTaskCreate();
    	xTaskCreate();
    	vTaskStartScheduler();
    
    }
    else{
       printf("Error _queue\n\r");
    }


    return 0;
}

void sender(void *pvParameters)
{
    for (;;)
    {

    }
}

void readqueue(void *pvParameters)
{

    for (;;)
    {

    }
}
