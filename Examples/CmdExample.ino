

#include "main.h"

#include <Arduino.h>
#include <CommandHandler.h>


byte cmd_incom();
void cmd_error(byte);

class IOD: public CommandContainer { 
	public:
	IOD():CommandContainer("IOD") {}
	byte exe(void);
	void rsp(byte r);
};


class IOA: public CommandContainer { 
	public:
	IOA():CommandContainer("IOA") {}
	byte exe(void);
	void rsp(byte r);
};

CommandHandler<2,2,127> Command;
IOD IOD;
IOA IOA;

void setup() {
	CommandContainer * contnr[] = {&IOA, &IOD};
	Serial.begin(115200);
	Command.attachSys(&cmd_incom, &cmd_error);	
	Command.registerCmd(contnr);
}

void loop() {
  // put your main code here, to run repeatedly:
  Command.run();
  

}

//####################################### SYSTEM
byte cmd_incom(){

	char chr;

	if(Serial.available() > 0)
	{
		while(Serial.available() >0){
			chr = Serial.read();
			// Serial.write(chr);
			if(chr == ';')
				return 0x01;
			Command.push(chr);
		}
	}
	return 0x00;
}
void cmd_error(byte x){
	Serial.print(F("Format Error: "));
	Serial.print(x);
}

//####################################### IOD Command
// FORMAT COMMAND : "AN+IOD=<CONFIG><DIGITAL PIN><PARAM2>;
// 	CONFIG		: O --> WRITE OUTPUT
//  			  P --> CONFIG PIN
//				  I --> READ INPUT
//	DIGITAL PIN	: 00h ~ 0Dh (D1 ~ D13);
// 	PARAM		: O --> 01h = HIGH ; 00h = LOW
//  			  P --> 00h = INPUT ; 01h = OUTPUT ; 02h = INPUT_PULLUP
//				  I --> 01h = HIGH ; 00h = LOW
// 	RESPOND		: O --> OK = SUCCES ; ?? = UNKNOWN INSTRUCTION
//  			  P --> OK = SUCCES ; ?? = UNKNOWN INSTRUCTION
//				  I --> "+IODI:<R_PARAM>" 
//								 |__01h = HIGH ; 00h = LOW

byte IOD::exe(){
	byte c = (byte)Command.pop();
	byte cc;
	

	switch(c){
		case 'O':
			c = (byte)Command.pop();
			cc= (byte)Command.pop();
			digitalWrite(c,cc);
			return 0xFF;
		case 'P':
			c = (byte)Command.pop();
			cc= (byte)Command.pop();
			pinMode(c,cc);
			return 0xFF;
		case 'I':
			c = (byte)Command.pop();
			return digitalRead(c);
		default:
			return 0xEF;
	}
}

void IOD::rsp(byte r){
	switch(r){
		case 0xFF:
			Serial.print("OK");
			break;
		case 0xEF:
			Serial.print("??");
			break;
		default:
			Serial.print("+IODI:");
			Serial.print(r,HEX);
			Serial.print(";");

	}
}

//####################################### IOA Command
// FORMAT COMMAND : "AN+IOA=I<DIGITAL PIN>;
//	ANALOG PIN	: A0h ~ A5h (A0 ~ A5);
// 	RESPOND		: I --> "+IOAI:<R_PARAM>" 
//								 |__00h ~ FFh

byte IOA::exe(void){
	byte c = (byte)Command.pop();
	

	switch(c){
		case 'I':
		{
			c = (byte)Command.pop();
			switch(c){
				case 0xA0:
				case 0xA1:
				case 0xA2:
				case 0xA3:
				case 0xA4:
				case 0xA5:
					return analogRead(c);
			}
		}
	}
	return 0x00;
}

void IOA::rsp(byte r){
	Serial.print("+IOAI:");
	Serial.print(r,HEX);
	Serial.print(";");
}

