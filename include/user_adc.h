/*
 * user_adc.h
 *
 *  Created on: 2017年7月5日
 *      Author: Administrator
 */



#ifndef _USER_ADC_H_
#define _USER_ADC_H_


#include "esp_common.h"

extern void adc_timer_init(void);
extern void ATaskAdc( void *pvParameters );

extern uint8 look_up_table(uint16 *a,uint8 ArrayLong,uint16 sample_value);
extern uint8 num_to_temperature(uint8 num);

extern float Temp_data;	//温度数据

#endif /* _USER_ADC_H_ */
