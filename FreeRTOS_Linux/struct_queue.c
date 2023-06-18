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

typedef enum{
	mainSENDER_1,
	mainSENDER_2
}mainSENDER;

typedef struct 
{
	unsigned char ucValue;
	unsigned char ucSource;
}xData;

static const xData xStructsToSend[2] = {{100,mainSENDER_1},{200,mainSENDER_2}};

void sender(void *pvParameters);
void readqueue(void *pvParameters);

xQueueHandle _queue = NULL;

int main(void)
{
    _queue=xQueueCreate(3,sizeof(xData));
    if(_queue != NULL){
    
    	xTaskCreate(&sender, "Sender1", 1024,(void*)&(xStructsToSend[0]), 2, NULL);
    	xTaskCreate(&sender, "Sender2", 1024, (void*)&(xStructsToSend[1]), 2, NULL);
    	xTaskCreate(&readqueue, "Reader", 1024, NULL, 1, NULL);
    	vTaskStartScheduler();
    
    }
    else{
       printf("Error _queue\n\r");
    }


    return 0;
}

void sender(void *pvParameters)
{
    portBASE_TYPE qstatus;
    const portTickType xTicksToWait = 1000/portTICK_RATE_MS;

    for (;;)
    {

    	  qstatus = xQueueSendToBack(_queue,pvParameters,xTicksToWait);
    	  if(qstatus !=pdPASS){
    	      printf("Send\r\n");
              vTaskDelay(pdMS_TO_TICKS(1000)); 
    	  } 
    	  taskYIELD();  	
    }
}

void readqueue(void *pvParameters)
{
    xData receiveValue;
    portBASE_TYPE xStatus;
    
    for (;;)
    {    
    	if(uxQueueMessagesWaiting(_queue) != 3)
    	{
    	    printf("Queue should have been full!\r\n");
    	}
    
        xStatus = xQueueReceive(_queue,&receiveValue,0);
        if(xStatus == pdPASS){
            if(receiveValue.ucSource == mainSENDER_1)
            {
               printf("From Sender 1 = %d\r\n",receiveValue.ucValue);
            }
            else
            {
               printf("From Sender 2 = %d\r\n",receiveValue.ucValue);
            }
        }
        else
        {
             printf("Could not receive from the queue.\r\n");
        }
    }
}
