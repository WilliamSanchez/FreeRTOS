/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/io.h>

#include <signal.h>
#include <errno.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static void vHandlerTask(void *q);
static void sigHandler();

xSemaphoreHandle binarysem;


/*

1. An interrupt occurred.
2. The interrupt service routine executed, ‘giving’ the semaphore to unblock the Handler task.
3. The Handler task executed as soon as the interrupt completed. The first thing the Handler task did
   was ‘take’ the semaphore.
4. The Handler task performed the event processing before attempting to ‘take’ the semaphore again
– entering the Blocked state if the semaphore was not immediately available.

*/


int main()
{

   int j;
   
   if(signal(SIGINT, sigHandler) == SIG_ERR)
   {
       perror("Signal");
   }
   
  binarysem = xSemaphoreCreateCounting(10,0);
  
  xTaskCreate(vHandlerTask,"task 1",128,NULL,3,NULL);
  vTaskStartScheduler();
  
  while(1);
  return 0;

}

 
void vHandlerTask(void *a)
{
 	while(1)
 	{
 	
 	/* Use the semaphore to wait for an event. The semaphore was created
	before the scheduler was started so before this task ran for the first
	time. The task blocks indefinitely so the function call will only
	return once the semaphore has been successfully taken (obtained). There
	is therefore no need to check the function return value. */

 	   xSemaphoreTake(binarysem,portMAX_DELAY);
 	   
	/* To get here the event must have occurred. Process the event.
	case processing is simply a matter of printing out a message. */

 	   printf("HAndler task - Processing event.\r\n");
 	}
}

static void sigHandler()
{
    static portBASE_TYPE xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    
    xSemaphoreGiveFromISR(binarysem,&xHigherPriorityTaskWoken);
    xSemaphoreGiveFromISR(binarysem,&xHigherPriorityTaskWoken);
    xSemaphoreGiveFromISR(binarysem,&xHigherPriorityTaskWoken);
    
    if(xHigherPriorityTaskWoken == pdTRUE)
    {
         printf("Ouch!\r\n");
    }

}






