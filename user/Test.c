#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "gpio.h"
#include "Test.h"
#include "scan_mac.h"

#include <stdlib.h>
#include <time.h>

/*
模块名
模块功能: 用于
*/

// charge power
uint16 CurrentVoltage=5,Current=1;
uint16 RealTimePower=10;
uint16 BatteryPercentage=20;
uint16 Runtime_s=0;
uint16 Runtime=20;
uint16 Rand_01,Rand_02,Rand_03,Rand_04;

//Stat Data
uint16 Body_stat=1;
uint16 Walk_count=111;
uint16 BodyTemp=40;
uint16 Rand_11,Rand_12,Rand_13,Rand_14;



/******************************************************************************
* 函数介绍：电池参数采集
* 输入参数：--
* 输出参数：无
* 返回值：无
* 备注： 伪随机数模拟生成电池各方面的参数值(电压、电流等等...)
******************************************************************************/
void ATaskPower( void *pvParameters )
{
    
    for(;;)
    {

       vTaskDelay(100);
       // DHT11_TEST();
       CurrentVoltage = 5;
       Current = 15;
       RealTimePower = 70 + (99*rand())%10;
       BatteryPercentage = (int)(100-1.66*Runtime);
       Runtime_s++;
       if (59 == Runtime_s) {
        Runtime_s = 0;
        Runtime++;
       }
       Rand_01 = 100 + (99*rand())%100;
       Rand_02 = 100 + (99*rand())%100;
       Rand_03 = 100 + (99*rand())%100;
       Rand_04 = 100 + (99*rand())%100;
       // printf("CurrentVoltage = %d\n",CurrentVoltage);
       // printf("Current = %d\n",Current);
       // printf("RealTimePower = %d\n",RealTimePower);
       // printf("BatteryPercentage = %d\n",BatteryPercentage);
       // printf("Runtime = %d\n",Runtime);
       
    }
    vTaskDelete( NULL );


}


/******************************************************************************
* 函数介绍：电池任务Init
* 输入参数：无
* 输出参数：无
* 返回值：无
* 备注：无
******************************************************************************/
void Power_init(void)
{
    
    srand((unsigned)time(NULL));
    xTaskCreate(ATaskPower, "ATaskPower", 256, NULL, 4, NULL);

}




/******************************************************************************
 * FunctionName : ATaskBody
 * Description  : ssssss
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ATaskBody( void *pvParameters )
{
    
    for(;;)
    {

       vTaskDelay(100);
       Body_stat=1;
       BodyTemp=35;

       Rand_11 = 100 + (99*rand())%100;
       Rand_12 = 100 + (99*rand())%100;
       Rand_13 = 100 + (99*rand())%100;
       Rand_14 = 100 + (99*rand())%100;  
       // printf("Body_stat = %d\n",Body_stat);
       // printf("Walk_count = %d\n",Walk_count++);
       // printf("BodyTemp = %d\n",BodyTemp);
    }
    vTaskDelete( NULL );


}

/******************************************************************************
 * FunctionName : Body_init
 * Description  : 
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void Body_init(void)
{
    
    srand((unsigned)time(NULL));
    xTaskCreate(ATaskBody, "ATaskBody", 256, NULL, 4, NULL);

}





