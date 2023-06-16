/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void task1(void *q);
void task2(void *a);

xSemaphoreHandle binarysem;

int main()
{  
  vSemaphoreCreateBinary(binarysem);
  xTaskCreate(task1,"task 1",128,NULL,1,NULL);
  xTaskCreate(task2,"task 2",128,NULL,1,NULL);
  vTaskStartScheduler();
  while(1);
  return 0;
}

void task1(void *q)
{
 	while(1)
 	{
 	   xSemaphoreTake(binarysem,portMAX_DELAY);
 	   printf("Task1 functioning \r\n");
 	   xSemaphoreGive(binarysem);
 	   vTaskDelay(1);
 	}
}

void task2(void *a)
{
 	while(1)
 	{
 	   xSemaphoreTake(binarysem,portMAX_DELAY);
 	   printf("Task2 functioning \r\n");
 	   xSemaphoreGive(binarysem);
 	   vTaskDelay(1);
 	}
}

