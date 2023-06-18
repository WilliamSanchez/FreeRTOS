/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

static const char *txParameter1 = "parameter task 1"; 
static const char *txParameter2 = "parameter task 2"; 

xTaskHandle xTask2Handle;

int main(void)
{
    xTaskCreate(&vTask1, "Task 1", 1024, (void*)txParameter1, 1, &xTask2Handle);
    xTaskCreate(&vTask2, "Task 2", 1024, (void*)txParameter2, 5, NULL);

    vTaskStartScheduler();

    return 0;
}

void vTask1(void *pvParameters)
{
    unsigned portBASE_TYPE uxPriority;
    uxPriority = uxTaskPriorityGet(NULL);
    
    printf("Task 1 priority: %lu\r\n",uxPriority);
    
    char *rxParameter;
    rxParameter = (char *)pvParameters;
    
    
    for (;;)
    {
        uxPriority = uxTaskPriorityGet(NULL);          
        printf("Task 1 [%s] priority: %lu\r\n",rxParameter,uxPriority);
        vTaskPrioritySet(xTask2Handle,(uxPriority+1));
        vTaskDelay(pdMS_TO_TICKS(200));
    }
     vTaskDelete(NULL);
}

void vTask2(void *pvParameters)
{

    unsigned portBASE_TYPE uxPriority;
    uxPriority = uxTaskPriorityGet(NULL);
    
    char *rxParameter;
    rxParameter = (char *)pvParameters;

    for (;;)
    {
        printf("Task 2 [%s] priority: %lu\r\n",rxParameter,uxPriority);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
         vTaskDelete(NULL);
}
