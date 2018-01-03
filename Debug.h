#ifndef DEBUG_H
#define DEBUG_H


#ifdef __DEBUG__
#include <Arduino.h>
#warning    "in debug mode."
// #define DBG_DECLARATION()       HardwareSerial *ptrSerial      
#define DBG_INIT(x)              Serial.begin(x)
#define DBG_FLASH_PRINT(x)      Serial.print(F(x))
#define DBG_FLASH_PRINTLN(x)    Serial.println(F(x))
#define DBG_PRINT(x)            Serial.print(x)
#define DBG_PRINTP(x,y)         Serial.print(x,y)
#define DBG_PRINTLN(x)          Serial.println(x)
#define DBG_WRITE(x)            Serial.write(x)
#else
#define DBG_DECLARATION()      
#define DBG_INIT(x)             
#define DBG_FLASH_PRINT(x)
#define DBG_FLASH_PRINTLN(x)    
#define DBG_PRINT(x)
#define DBG_PRINTP(x,y)
#define DBG_PRINTLN(x)    
#define DBG_WRITE(x)      
#endif
#endif
