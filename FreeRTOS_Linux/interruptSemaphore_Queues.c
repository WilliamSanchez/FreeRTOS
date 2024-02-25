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
#include "queue.h"
#include "semphr.h"

static void vHandlerTask(void *q);
static void vIntegerGenerator(void *pvParameters);

static void sigHandler();

xSemaphoreHandle binarysem;

xQueueHandle xIntegerQueue = NULL;
xQueueHandle xStringQueue = NULL;

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

  xIntegerQueue=xQueueCreate(10,sizeof(char));    
  xStringQueue=xQueueCreate(10,sizeof(char)); 
   
  binarysem = xSemaphoreCreateCounting(10,0);
  
  xTaskCreate(vHandlerTask,"task 1",128,NULL,3,NULL);
  vTaskStartScheduler();
  
  while(1);
  return 0;

}

static void vIntegerGenerator(void *pvParameters)
{
   portTickType xLastExecutionTime;
   unsigned portLong ulValueToSned = 0;


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
    static unsigned long ulReceivedNumber;
    
    /* The strings are declared static const to ensure they are not allocated
       on the stack of the ISR, and exist even when the ISR is not executing. */
       
    static const char *pcStrings[] = 
    {
       "String 0\r\n",
       "String 1\r\n",
       "String 2\r\n",
       "String 3\r\n"
    }
    
    xHigherPriorityTaskWoken = pdFALSE;
    
    /* Loop until the queue is empty. */
    
    while(xQueueReceiveFromISR(xIntegerQueue,
    				&ulReceivedNumber,
    				&xHigherPriorityTaskWoken) != errQUEUE_EMPTY)
    {
    	/* Truncate the received value to the last two bits (values 0 to 3 inc.), then
	send a pointer to the string that corresponds to the truncated value to a
	different queue. */
        ulReceivedNumber &= 0x03;
        xQueueSendToBackFromISR( xStringQueue,
        			 &pcStrings[ ulReceivedNumber ],
        			 &xHigherPriorityTaskWoken );
    }
    
    if(xHigherPriorityTaskWoken == pdTRUE)
    {
         printf("Ouch!\r\n");
    }

}






