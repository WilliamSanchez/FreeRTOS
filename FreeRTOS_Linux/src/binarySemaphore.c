/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void vHandlerTask(void *q);
void vPeriodicTask(void *a);

xSemaphoreHandle binarysem;

int main()
{  
  vSemaphoreCreateBinary(binarysem);
  
  _dos_setvect(0x82,vExampleInterruptHandler);
  
  xTaskCreate(vHandlerTask,"task 1",128,NULL,3,NULL);
  xTaskCreate(vPeriodicTask,"task 2",128,NULL,1,NULL);
  vTaskStartScheduler();
  while(1);
  return 0;
}

static void vPeriodicTask(void *pvParameters)
{
 	while(1)
 	{
 	   vTaskDelay(500/portTICK_RATE_MS);
 	   printf("Periodic task - About to generate an interupt.\n\r");
 	   __asm{int 0x82}
 	   printf("Periodic task - INterrupt generated.\r\n\r\n\r\n");
 	}
}

void vHandlerTask(void *a)
{
 	while(1)
 	{
 	   xSemaphoreTake(binarysem,portMAX_DELAY);
 	   printf("HAndler task - Processing event.\r\n");
 	}
}

static void __interrupt __far vExampleInterruptHandler(void)
{
    static portBASE_TYPE xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    
    xSemaphoreGiverFromISR(binarysem,&xHigherPriorityTaskWoken);
    if(xHigherPriorityTaskWoken == pdTRUE)
    {
       portSWITCH_CONTEXT();
    }
  
}







