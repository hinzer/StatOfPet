/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <stddef.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "mqtt/MQTTClient.h"

#include "user_config.h"

#include "cJSON.h"
#include "Test.h"
#include "user_adc.h"





#define MQTT_CLIENT_THREAD_NAME         "mqtt_client_thread"
#define MQTT_CLIENT_THREAD_STACK_WORDS  2048
#define MQTT_CLIENT_THREAD_PRIO         8

//封装发布主题(上报数据)和订阅主题(订阅发布响应)
#define TOPIC_PROPERTY_POST	 			"/sys/%s/%s/thing/event/property/post"
#define TOPIC_PROPERTY_POST_REPLY		"/sys/%s/%s/thing/event/property/post_reply"

//对数据的json格式进行封装
// #define DATAPAYLOAD						"{\r\n\"id\" : \"1123\",\r\n\"version\":\"1.0\",\r\n\"params\" : {\r\n\"CurrentVoltage\": %d,\r\n\"Current\": %d,\r\n},\r\n\"method\":\"thing.event.property.post\"\r\n}"


#define DATAPAYLOAD   "{\r\n\"id\" : \"1123\",\r\n\"version\":\"1.0\",\r\n\"params\" : {\r\n\"CurrentVoltage\": %d.%d,\r\n\"Current\": %d.%d,\
    \r\n\"RealTimePower\": %d.%d,\r\n\"BatteryPercentage\": %d.%d,\r\n\"WalkCount\": %d,\
    \r\n},\r\n\"method\":\"thing.event.property.post\"\r\n}"





//设备信息
char *clientId  = "j7CSySS7f9oeZCJNnt0L";
char *timestamp = "789";
char *deviceName = "j7CSySS7f9oeZCJNnt0L";
char *productKey = "a1DY5H8GJaV";
char *devicesecret = "GBeaEGzSAClJjGbGEOJsPQ7dcIvdNTCj";

//mqtt连接信息
char mqttClientId[100];
char mqttUsername[100];
char mqttPassword[100];
char hmac_source[512];

//存储主题信息
char JsonDataPayLoad[500];
char MqttTopicPropertyPost[200];
char MqttTopicPropertyPostReply[200];


void GetJsonData(void)
{//对获取温湿度数据进行json格式封装

    sprintf(JsonDataPayLoad,DATAPAYLOAD,CurrentVoltage,Rand_01, \
                                        Current,Rand_02,\
                                         RealTimePower,Rand_03,  \
                                          BatteryPercentage,Rand_04, \
                                          Walk_count);

}
int CreateJsData(char* JsData)
{//
    int rc = 0;
    cJSON *Root;
    cJSON *Params;
    char *cJsData;
    //创建json对象
    Root = cJSON_CreateObject();
    if(!Root)
    {
        printf("get Root faild !\r\n");
        rc = -1;
        goto EXIT;

    }
    else
    {
        printf("get Root success!\r\n");

    }

    cJSON_AddStringToObject(Root,"id","123");
    cJSON_AddStringToObject(Root,"version","1.0");
    cJSON_AddItemToObject(Root, "params", Params=cJSON_CreateObject());
    cJSON_AddStringToObject(Root,"method","thing.event.property.post");
    cJSON_AddNumberToObject(Params,"temperature",30.1);
    cJSON_AddNumberToObject(Params,"humidity",60.2);


    cJsData = cJSON_Print(Root);
    if(cJsData)
    {
        printf("JSON lenth is %d\r\n",strlen(cJsData));
        printf("JSON is %s\r\n",cJsData);
        strcpy(JsData, cJsData);

    }
    else
    {
        printf("get JSON is fail!r\\n");
        rc = -1;

    }
     EXIT:
     cJSON_Delete(Root);
     return rc;
    

}


void GetMqttTopicPropertyPost(void)
{//获取发布主题


    sprintf(MqttTopicPropertyPost,TOPIC_PROPERTY_POST,productKey,deviceName);
    printf("%s\r\n",MqttTopicPropertyPost);


}

void GetMqttTopicPropertyPostReply(void)
{//获取订阅主题


    sprintf(MqttTopicPropertyPostReply,TOPIC_PROPERTY_POST_REPLY,productKey,deviceName);
    printf("%s\r\n",MqttTopicPropertyPostReply);


}


void GetmqttClientId(void)
{//获取mqttClientId
    sprintf(mqttClientId,"%s|securemode=3,signmethod=hmacsha1,timestamp=%s|",clientId,timestamp);
    printf("%s\r\n",mqttClientId);
}
void GetmqttUsername(void)
{//获取mqttUsername
    sprintf(mqttUsername,"%s&%s",deviceName,productKey);
    printf("%s\r\n",mqttUsername);
}
void GetmqttPassword(void)
{//获取mqttPassword
    sprintf(hmac_source,
        "clientId%s" "deviceName%s" "productKey%s" "timestamp%s",
        clientId,deviceName,productKey,timestamp);
    utils_hmac_sha1(hmac_source,strlen(hmac_source),
        mqttPassword,devicesecret,strlen(devicesecret));
    printf("%s\r\n",mqttPassword);
}


LOCAL xTaskHandle mqttc_client_handle;

static void messageArrived(MessageData* data)
{
    printf("Message arrived: %s\n", data->message->payload);
}

static void mqtt_client_thread(void* pvParameters)
{
    printf("mqtt client thread starts\n");
    MQTTClient client;
    Network network;
    unsigned char sendbuf[300], readbuf[300] = {0};
    int rc = 0, count = 0;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    pvParameters = 0;
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    char* address = MQTT_BROKER;
	printf("address = %s\r\n",address);
	
    if ((rc = NetworkConnect(&network, address, MQTT_PORT)) != 0) {
        printf("Return code from network connect is %d\n", rc);
    }

#if defined(MQTT_TASK)

    if ((rc = MQTTStartTask(&client)) != pdPASS) {
        printf("Return code from start tasks is %d\n", rc);
    } else {
        printf("Use MQTTStartTask\n");
    }


#endif
    GetmqttClientId();
    GetmqttUsername();
    GetmqttPassword();
    GetMqttTopicPropertyPost();
    GetMqttTopicPropertyPostReply();
    connectData.clientID.cstring = mqttClientId;
    connectData.username.cstring = mqttUsername;
    connectData.password.cstring = mqttPassword;
    connectData.MQTTVersion = 4;	//mqtt v3.1

    printf("connectData.struct_id = %s\r\n",connectData.struct_id);
    printf("connectData.struct_version = %d\r\n",connectData.struct_version);
    printf("connectData.MQTTVersion = %d\r\n",connectData.MQTTVersion);
    printf("connectData.clientID = %s\r\n",connectData.clientID.cstring);
    printf("connectData.keepAliveInterval = %d\r\n",connectData.keepAliveInterval);
    printf("connectData.cleansession = %d\r\n",connectData.cleansession);
    printf("connectData.willFlag = %d\r\n",connectData.willFlag);
    printf("connectData.username = %s\r\n",connectData.username.cstring);
    printf("connectData.password = %s\r\n",connectData.password.cstring);

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Return code from MQTT connect is %d\n", rc);
    } else {
        printf("MQTT Connected\n");
    }


	//订阅主题响应...
    if ((rc = MQTTSubscribe(&client, MqttTopicPropertyPostReply, 2, messageArrived)) != 0) {
        printf("Return code from MQTT subscribe is %d\n", rc);
    } else {
        printf("MQTT subscribe to topic %s\n",MqttTopicPropertyPostReply);
    }

    while (++count) {
		GetJsonData();		//获取jison对象
        MQTTMessage message;

        message.qos = QOS0;
        message.retained = 0;
        message.payload = JsonDataPayLoad;
        printf("JSON is %s\r\n",message.payload);
        message.payloadlen = strlen(JsonDataPayLoad);

        //发布数据主题
        if ((rc = MQTTPublish(&client, MqttTopicPropertyPost, &message)) != 0) {
            printf("Return code from MQTT publish is %d\n", rc);
        } else {
            printf("MQTT publish topic %s, message  is %s\n",MqttTopicPropertyPost,message.payload);
        }
        //等待应答主题
        MQTTYield(&client,200);
        //定时10s发送
        vTaskDelay(3000 / portTICK_RATE_MS);  //send every 3 seconds

    }

    printf("mqtt_client_thread going to be deleted\n");
    vTaskDelete(NULL);
    return;
}


void user_conn_init(void)
{
    int ret;
    ret = xTaskCreate(mqtt_client_thread,
                      MQTT_CLIENT_THREAD_NAME,
                      MQTT_CLIENT_THREAD_STACK_WORDS,
                      NULL,
                      MQTT_CLIENT_THREAD_PRIO,
                      &mqttc_client_handle);

    if (ret != pdPASS)  {
        printf("mqtt create client thread %s failed\n", MQTT_CLIENT_THREAD_NAME);
    }
	Power_init();
    Body_init();
    // adc_timer_init();   //温度ADC任务调度
}
