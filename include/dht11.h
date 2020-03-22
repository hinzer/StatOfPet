#ifndef __DHT11_H__
#define __DHT11_H__

#ifdef __cplusplus
extern "C" {
#endif

extern uint16 ucharT_data_H,ucharRH_data_H,RealTimePower,UnitPowerConsumption,Runtime;

extern uint16 Rand_01,Rand_02,Rand_03,Rand_04;


void ATaskDht11( void *pvParameters );

void Dht11_init(void);


#ifdef __cplusplus
}
#endif

#endif

