#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "gpio.h"
#include "dht11.h"
#include <stdlib.h>
#include <time.h>





//温湿度定义S

uint16 ucharT_data_H=0,ucharRH_data_H=0;
uint16 RealTimePower=3000;
uint16 UnitPowerConsumption=222;
uint16 Runtime_s=0;
uint16 Runtime=20;
uint16 Rand_01,Rand_02,Rand_03,Rand_04;



/******************************************************************************
 * FunctionName : ATaskDht11
 * Description  : ATaskDht11 任务
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ATaskDht11( void *pvParameters )
{
    
    for(;;)
    {

       vTaskDelay(100);
       // DHT11_TEST();
       ucharT_data_H = 330 + (99*rand())%20;
       ucharRH_data_H = 15 + (99*rand())%3;
       RealTimePower = ucharT_data_H*ucharRH_data_H/1000;
       UnitPowerConsumption = Runtime*RealTimePower/60;
       Runtime_s++;
       if (59 == Runtime_s) {
        Runtime_s = 0;
        Runtime++;
       }
       Rand_01 = 100 + (99*rand())%100;
       Rand_02 = 100 + (99*rand())%100;
       Rand_03 = 100 + (99*rand())%100;
       Rand_04 = 100 + (99*rand())%100;
       printf("ucharT_data_H = %d\n",ucharT_data_H);
       printf("ucharRH_data_H = %d\n",ucharRH_data_H);
       printf("RealTimePower = %d\n",RealTimePower);
       printf("UnitPowerConsumption = %d\n",UnitPowerConsumption);
       printf("Runtime = %d\n",Runtime);
       
    }
    vTaskDelete( NULL );





}


/******************************************************************************
 * FunctionName : Dht11_init
 * Description  : Dht11_init 初始化
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void Dht11_init(void)
{
    
    srand((unsigned)time(NULL));
    xTaskCreate(ATaskDht11, "ATaskDht11", 256, NULL, 4, NULL);

}





