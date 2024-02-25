/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

static int canal1[25]= {0,121,224,296,327,312,252,158,41,-81,-193,-277,-322,\
			-322,-277,-193,-81,41,158,312,327,296,224,121};
static int canal2[25]={0,145,269,356,392,374,303,189,49,-98,-231,-332,-386,\
			-386,-332,-231,-98,49,189,303,374,392,356,269,145};

typedef struct {
	int buffer_adc_sensor;
	int buffer_adc_ref;
}data;

static float buffer_result[25][3];

xQueueHandle _queue = NULL;

void readDataSensor(void *sendData);
void valuePP(void *receivedData);
void interface();
void consumoProcess();

int main(void)
{
    
    _queue=xQueueCreate(25,sizeof(data));

    if(_queue != NULL)
    {
    	xTaskCreate(&readDataSensor, "Task 1", 1024, NULL, 3, NULL);
    	xTaskCreate(&valuePP, "Task 2", 1024, NULL, 3, NULL);
    	xTaskCreate(&interface, "Task 3", 1024, NULL, 1, NULL);
    	//xTaskCreate(&consumoProcess, "Task 4", 1024, NULL, 1, NULL);
    	vTaskStartScheduler();
    }
    return 0;
}

void readDataSensor(void *sendData)
{
    unsigned portBASE_TYPE uxPriority;
    portBASE_TYPE qstatus;
    uxPriority = uxTaskPriorityGet(NULL);
    const portTickType xTicksToWait = 1000/portTICK_RATE_MS;
    data sensorData[25];
   
    int i=0;
    for (;;)
    {     
    	 //printf("Task readDataSensor priority %lu\n\r",uxPriority);
    	for(int i=0; i<25;i++){
    	  sensorData[i].buffer_adc_sensor = canal1[i]; sensorData[i].buffer_adc_ref = canal2[i];
    	  qstatus = xQueueSendToBack(_queue,&sensorData[i],0);
    	  if(qstatus !=pdPASS){
    	      //printf("Send\r\n");
              vTaskDelay(1000/portTICK_RATE_MS); 
    	  }   	
    	}
    }
    vTaskDelete(NULL);

}

void valuePP(void *receivedData)
{
    portBASE_TYPE xStatus;
    unsigned portBASE_TYPE uxPriority;
    uxPriority = uxTaskPriorityGet(NULL);
    
    data recvdData;
    int i = 0;
    for (;;)
    {
        
        xStatus = xQueueReceive(_queue,&recvdData,25);
        if(xStatus == pdPASS){
            //printf("Task valuePP priority [%lu] ",uxPriority);
            buffer_result[i%25][0]= 2*recvdData.buffer_adc_sensor/16Bits; buffer_result[i%25][1]= 2*recvdData.buffer_adc_ref; buffer_result[i%25][2]=buffer_result[i%25][1]-buffer_result[i%25][0];
            //printf("%d => Data recivida %f | %f | %f\n\r",i%25, buffer_result[i%25][0], buffer_result[i%25][1], buffer_result[i%25][2]);
            i++;
        }
    }


}


void interface()
{

    unsigned portBASE_TYPE uxPriority;
    uxPriority = uxTaskPriorityGet(NULL);
    
    data recvdData;
    char menu[1024];
    
         		//system("clear");
    		//printf("Task valuePP priority [%lu]\n\r",uxPriority);
    		//printf("1=> Escreva obter se quiser pegar dados\n\r");
    		//printf("2=> Escreva zerar se quiser apagar dados\n\r");
    		char userData[32];
    		int fopen;
    		fopen = open("datadump.csv", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    
    for (;;)
    {
  
     		//system("clear");
    		printf("Task valuePP priority [%lu]\n\r",uxPriority);
    		printf("1=> Escreva obter se quiser pegar dados\n\r");
    		printf("2=> Escreva zerar se quiser apagar dados\n\r");
    		printf("3=> Escreva clean se quiser limpar tela\n\r");
    		while(!fgets(menu, 32, stdin));
    	        printf("Escrivio %s\n\r",menu);
    	        
    	        if(strcmp(menu,"obter\n") == 0)
    	        {
    	            lseek(fopen,0,SEEK_SET);
    	            for(int i=0; i<3; i++)
    	            {
    	              
    	              sprintf(userData,"%d=> %f\t%f\t%f\n",i, buffer_result[i][0], buffer_result[i][1], buffer_result[i][2]);
    	              write(fopen,userData,strlen(userData));
    	            }    	                     
    	            printf("\n\rEscrivir en file\n\r");
    	            
    	        }else if(strcmp(menu,"zerar\n") == 0)
    	        {
    	            
    	            off_t size = lseek(fopen,0, SEEK_END);
    	            
    	            printf("\n\rSIZE file [%ld]\n\r",size);
    	            memset(userData,' ',size);
    	            lseek(fopen,0,SEEK_SET);
    	            write(fopen,userData,size); 	                     
    	            
    	            printf("\n\rZERAR  file\n\r");
    	            
    	        }else if(strcmp(menu,"clean\n") == 0)
    	        {
    	            system("clear");
    	        }else
    	        {
    	           printf("Intente de novo\n\r");
    	        }
    	        
    		vTaskDelay(100/portTICK_RATE_MS); 
 
    }

     close(fopen);

}




/*


    int fd;
    
    fd = open("../canal_1.csv",O_RDONLY | O_NONBLOCK);
    if(fd == -1)
    	perror("Open file");
    
    if (lseek(fd,67,SEEK_SET) == -1)
    	perror("lseek");    	
    
     read(fd,data,8);
     
     printf("=> 1 data read [%s]\n\r",data);
    
        if (lseek(fd,0,SEEK_CUR) == -1)
    	perror("lseek");
    	
    
     read(fd,data,8);
     
     printf("=> 2 data read [%s]\n\r",data);
    
    close(fd);
    printf("Close program \n\r");
    
*/
