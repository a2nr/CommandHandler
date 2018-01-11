#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <Arduino.h>
#include <Stream.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

#undef __DEBUG__
#include "Debug.h"	


#define STD_CMD		"AN"

class CommandContainer {
	public:
	const char * cmd;
	CommandContainer(const char * c):cmd(c) {

	}
	virtual byte exe(void);
	virtual void rsp(byte);
};

template <unsigned int sizeCmd, unsigned int lenHeader, unsigned int sizeBuff>
class CommandHandler
{
private:
	DBG_DECLARATION();
	char Buffer[sizeBuff];
	byte (*PincommingFncHanddler)(void);
	void (*errorFncHandler)(byte);
	CommandContainer *cc[sizeCmd];
	byte poss_push,poss_pop;
	void flush(void);
public:
	CommandHandler();
	void attachSys(byte(*funcIn)(void), void (*error)(byte));
	void registerCmd(CommandContainer *c[]);
	unsigned int maxCommand(){return sizeBuff;}
	void run();
	void push(char);
	char pop();
	void cpyChar(char * c, byte l);
	byte available();

};

#include "cpp/CommandHandler.cpp"
#endif //COMMAND_HANDLLER_H