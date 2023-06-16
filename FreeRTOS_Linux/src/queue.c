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
    
    	xTaskCreate(&sender, "Sender", 1024, NULL, 1, NULL);
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
    unsigned char *dat = "[Send William]";
    for (;;)
    {
    	for(int i=0; i<14;i++){
    	  qstatus = xQueueSendToBack(_queue,&dat[i],0);
    	  if(qstatus !=pdPASS){
    	      printf("Send\r\n");
              vTaskDelay(pdMS_TO_TICKS(1000)); 
    	  }   	
    	}
    }
}

void readqueue(void *pvParameters)
{
    unsigned char receiveValue;
    portBASE_TYPE xStatus;
    
    for (;;)
    {
        xStatus = xQueueReceive(_queue,&receiveValue,100);
        if(xStatus == pdPASS){
            printf("Data recivida %c\n\r",receiveValue);
        }
    }
}
