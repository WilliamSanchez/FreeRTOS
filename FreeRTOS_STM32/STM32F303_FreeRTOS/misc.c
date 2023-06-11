#include<misc.h>

GPIO_InitTypeDef 	GPIO_InitStructure;
static u8  fac_us=0;  // micro second count
static u16 fac_ms=0;  // mili second countS

void InitializeLEDs(){	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	gpioStructure.GPIO_Mode = GPIO_Mode_OUT;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &gpioStructure);
	GPIO_WriteBit(GPIOE, GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12, Bit_RESET);
}

void delay_init(void){
   SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
   fac_us=SystemCoreClock/8000000;							//SYSCLK/8  
   fac_ms=(u16)fac_us*1000;
}

void delay(__IO uint32_t nTime){ 

	u32 temp;	    	 
	SysTick->LOAD=nTime*fac_ms; //Load	  		 
	SysTick->VAL=0x00;        		//Clear
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;        
	do{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       
	SysTick->VAL =0X00; 
}

uint8_t parseData(const char *data1, uint8_t len1, const char *data2){
  
  uint8_t i = 0, j = 0; 

  for(i = 0; i<len1; i++){
     if(*(data1+i) == *(data2+j)){
       j++;
       if(*(data2+j) == '\0') return 1; //
     }else {
       j = 0;
     }    
   }
	
  return 0;
}
