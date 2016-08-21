
#include "CommandHandler.h"

byte exampleOne(void);
void exampleOneRespond(byte respond);
byte exampleTwo(void);
void exampleTwoRespond(byte respond);

const TSCmdTbl CommandTabel[3] PROGMEM ={
	{"satu", exampleOne, exampleOneRespond},
	{"dua" , exampleTwo, exampleTwoRespond},
	{"tiga", exampleOne, exampleTwoRespond},
};


CommandHandler Command(&incommingHanddlerSerial, &errorHandlerSerial,CommandTabel, 3);
void setup() {
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  Command.run();
  

}
byte exampleOne(void){
	Serial.println("Bismillah One");
	return 0x02;
}
void exampleOneRespond(byte respond){
	if(respond == 0x02){
		Serial.println("Alhamdulillah One");
	} else{
		Serial.println("Ya Allah One");
	}
}
byte exampleTwo(void){
	
	while(Command.available() > 0){
		Serial.print(Command.pop());
	}
	Serial.println(" Bismillah Two");
	return 0x01;
}
void exampleTwoRespond(byte respond){
	if(respond == 0x01){
		Serial.println("Alhamdulillah Two");
	} else {
		Serial.println("Ya Allah Two");
	}
}

byte incommingHanddlerSerial(){

	char chr;
	byte x=0;

	while(Serial.available() > 0){
		chr = Serial.read();
		switch (chr) {
	    	case '\n':
		    	break;
		    case '\r':
		    	Serial.flush();
	      		return 0x01;
		    default:
		    	Command.push(chr);
		}
	}
}
void errorHandlerSerial(){
	Serial.println("Format Error");
}