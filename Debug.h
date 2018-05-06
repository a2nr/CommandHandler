#ifndef DEBUG_H
#define DEBUG_H

#ifdef __DEBUG__
// #warning    "in debug mode."
// #define DBG_DECLARATION()       HardwareSerial *ptrSerial      
// #define DBG_INIT()              ptrSerial = &Serial
// #define DBG_FLASH_PRINT(x)      ptrSerial->print(F(x))
// #define DBG_FLASH_PRINTLN(x)    ptrSerial->println(F(x))
// #define DBG_PRINT(x)            ptrSerial->print(x)
// #define DBG_PRINTP(x,y)         ptrSerial->print(x,y)
// #define DBG_PRINTLN(x)          ptrSerial->println(x)
#define DBG_DECLARATION()      
#define DBG_INIT()             
#define DBG_FLASH_PRINT(x)
#define DBG_FLASH_PRINTLN(x)    
#define DBG_PRINT(x)
#define DBG_PRINTP(x,y)
#define DBG_PRINTLN(x)  
#else
#define DBG_DECLARATION()      
#define DBG_INIT()             
#define DBG_FLASH_PRINT(x)
#define DBG_FLASH_PRINTLN(x)    
#define DBG_PRINT(x)
#define DBG_PRINTP(x,y)
#define DBG_PRINTLN(x)          
#endif
#endif
