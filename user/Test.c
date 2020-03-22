#include "esp_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "gpio.h"
#include "Test.h"
#include <stdlib.h>
#include <time.h>



// charge power
uint16 CurrentVoltage=5,Current=1;
uint16 RealTimePower=10;
uint16 BatteryPercentage=20;
uint16 Runtime_s=0;
uint16 Runtime=20;
uint16 Rand_01,Rand_02,Rand_03,Rand_04;

//Stat Data
uint16 Body_stat=1;
uint16 Walk_count=999;
uint16 BodyTemp=40;
uint16 Rand_11,Rand_12,Rand_13,Rand_14;



/******************************************************************************
 * FunctionName : ATaskPower
 * Description  : ssssss
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ATaskPower( void *pvParameters )
{
    
    for(;;)
    {

       vTaskDelay(100);
       // DHT11_TEST();
       CurrentVoltage = 5;
       Current = 15;
       RealTimePower = CurrentVoltage*Current/1000;
       BatteryPercentage = (60-Runtime)/60;
       Runtime_s++;
       if (59 == Runtime_s) {
        Runtime_s = 0;
        Runtime++;
       }
       Rand_01 = 100 + (99*rand())%100;
       Rand_02 = 100 + (99*rand())%100;
       Rand_03 = 100 + (99*rand())%100;
       Rand_04 = 100 + (99*rand())%100;
       printf("CurrentVoltage = %d\n",CurrentVoltage);
       printf("Current = %d\n",Current);
       printf("RealTimePower = %d\n",RealTimePower);
       printf("BatteryPercentage = %d\n",BatteryPercentage);
       printf("Runtime = %d\n",Runtime);
       
    }
    vTaskDelete( NULL );


}

/******************************************************************************
 * FunctionName : Power_init
 * Description  : 
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
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
       Walk_count=999;
       BodyTemp=40;

       Rand_11 = 100 + (99*rand())%100;
       Rand_12 = 100 + (99*rand())%100;
       Rand_13 = 100 + (99*rand())%100;
       Rand_14 = 100 + (99*rand())%100;  
       printf("Body_stat = %d\n",Body_stat);
       printf("Walk_count = %d\n",Walk_count);
       printf("BodyTemp = %d\n",BodyTemp);
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





