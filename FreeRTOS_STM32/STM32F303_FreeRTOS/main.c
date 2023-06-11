/*
	USART_1 PC		USART_2 BLE

	Tx ==> PA9		Tx ==> PA2
	Rx ==> PA10		Rx ==> PA3
*/

#include <stm32f30x.h>
#include <string.h>
#include <stdio.h>

#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>

#include "modulo3G.h"
#include "misc.h"

char trasmitData[64];

volatile int i = 0;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);


int main(void){

    InitializeLEDs();
    delay_init();
    GPIO_SetBits(GPIOE,GPIO_Pin_9);
    GPIO_SetBits(GPIOE,GPIO_Pin_10);
    delay(1000);
    
    xTaskCreate(vTask1,"Task_1",128,NULL,1,NULL);
    xTaskCreate(vTask2,"Task_2",128,NULL,2,NULL);
    
    vTaskStartScheduler();
    
    while(1)
    {  	

    }

  return 0;
}


void vTask1(void *pvParameters){
 
   while(1){
   
        GPIO_SetBits(GPIOE,GPIO_Pin_9);
        GPIO_ResetBits(GPIOE,GPIO_Pin_10);
        vTaskDelay(500);
   }
}

void vTask2(void *pvParameters){
 
   while(1){
   
        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        GPIO_SetBits(GPIOE,GPIO_Pin_10);
        vTaskDelay(500);
   }
}

/*
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
        GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
        for(i=0;i<5000000;i++);
        GPIO_ResetBits(GPIOE,GPIO_Pin_9);
        for(i=0;i<5000000;i++);
*/

