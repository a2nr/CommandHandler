#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <Arduino.h>
#include <Stream.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

#undef __DEBUG__
#include "Debug.h"

#define STD_CMD		"ANU"
#define CHAR_CMD		5
#define CMD_BUFFER		127

typedef byte (*ptrExecuteFnc)(void);
typedef void (*ptrRespondFnc)(byte);

typedef struct {
  const char cmdString[CHAR_CMD];
  ptrExecuteFnc pExecuteFnc;
  ptrRespondFnc pRespondFnc;
}TSCmdTbl;

class CommandHandler
{
private:
	DBG_DECLARATION();

	const TSCmdTbl *ptrTSCommandTbl;
	PGM_P * arrayCommand;
	char Buffer[CMD_BUFFER];
	byte (*PincommingFncHanddler)(void);
	void (*errorFncHandler)(void);
	byte poss_push,poss_pop, cmdNum;
	void flush(void);
public:
	CommandHandler(byte(*funcIn)(void), void (*error)(void),const TSCmdTbl *cmdTbl,byte len);
	void run();
	void push(char);
	char pop();
	byte available();

};

#endif //COMMAND_HANDLLER_H