//#include <Arduino.h>
#include "CommandHandler.h"


unsigned char cmd_incom();
void cmd_error(unsigned char);

#define IOD_CMD	{0x01, 0x00, 0x00 }
unsigned char 	iod_exe	(void);
void 			iod_rsp	(unsigned char);
const ts_cmd_cntr ts_IOD = {
	IOD_CMD,
	&iod_exe,
	&iod_rsp
};

// class IOD: public CommandContainer { 
// 	public:
// 	IOD():CommandContainer("IOD") {}
// 	byte exe(void);
// 	void rsp(byte r);
// };

#define IOA_CMD	{0x02, 0x00, 0x00 }
unsigned char 	ioa_exe	(void);
void 			ioa_rsp	(unsigned char);
const ts_cmd_cntr ts_IOA = {
	IOA_CMD,
	&ioa_exe,
	&ioa_rsp
};

// class IOA: public CommandContainer { 
// 	public:
// 	IOA():CommandContainer("IOA") {}
// 	byte exe(void);
// 	void rsp(byte r);
// };

ts_cmd_cntr ALL_CMD[] = {
	ts_IOD,
	ts_IOA
};

ts_cmd_hdl Command;

// CommandHandler<2,2,127> Command;
// IOD IOD;
// IOA IOA;

void setup() {
	cm_init_ts ts_cmdInit;
	ts_cmdInit.inc 			= &cmd_incom;
	ts_cmdInit.err 			= &cmd_error;
	ts_cmdInit.container 	= ALL_CMD;
	ts_cmdInit.fbyte		= 0x00;
	ts_cmdInit.len			= 2; 
	CM_SYS_INIT(&Command, &ts_cmdInit);

	//CommandContainer * contnr[] = {&IOA, &IOD};
	Serial.begin(115200);
	//Command.attachSys(&cmd_incom, &cmd_error);	
	//Command.registerCmd(contnr);
}

void loop() {
  // put your main code here,Command to run repeatedly:
  Command.run(&Command);
  

}

//####################################### SYSTEM
unsigned char cmd_incom(){

	char header[5];
	unsigned char chr;

	if(Serial.available() > 0)
	{
		header[0] = Serial.read();
		header[1] = Serial.read();
		header[2] = Serial.read();
		header[3] = Serial.read();
		header[4] = chr = Serial.read();

		Serial.print(header[0],HEX);
		Serial.print(header[1],HEX);
		Serial.print(header[2],HEX);
		Serial.print(header[3],HEX);
		Serial.print(header[4],HEX);



		while(chr--){
			Command.push(&Command, Serial.read());
		}
		
		return 0x01;
	}
	return 0x00;
}
void cmd_error(unsigned char x){
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

unsigned char iod_exe(){
	byte c = (byte)Command.pop(&Command);
	byte cc;
	

	switch(c){
		case 'O':
			c = (byte)Command.pop(&Command);
			cc= (byte)Command.pop(&Command);
			digitalWrite(c,cc);
			return 0xFF;
		case 'P':
			c = (byte)Command.pop(&Command);
			cc= (byte)Command.pop(&Command);
			pinMode(c,cc);
			return 0xFF;
		case 'I':
			c = (byte)Command.pop(&Command);
			return digitalRead(c);
		default:
			return 0xEF;
	}
}

void iod_rsp(unsigned char r){
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

unsigned char ioa_exe(void){
	byte c = (byte)Command.pop(&Command);
	

	switch(c){
		case 'I':
		{
			c = (byte)Command.pop(&Command);
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

void ioa_rsp(unsigned char r){
	Serial.print("+IOAI:");
	Serial.print(r,HEX);
	Serial.print(";");
}

