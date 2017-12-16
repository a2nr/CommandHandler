#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <avr/pgmspace.h>

#undef __DEBUG__
#include "Debug.h"

#define STD_CMD			"AN"
#define CHAR_CMD		5
#define CMD_BUFFER		127

typedef unsigned char (*ptrExecuteFnc)(void);
typedef void (*ptrRespondFnc)(unsigned char);

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
	unsigned char (*PincommingFncHanddler)(void);
	void (*errorFncHandler)(void);
	unsigned char poss_push,poss_pop, cmdNum;
	void flush(void);
public:
	CommandHandler(unsigned char(*funcIn)(void), void (*error)(void),const TSCmdTbl *cmdTbl,unsigned char len);
	void run();
	void push(char);
	char pop();
	unsigned char available();

};

#endif //COMMAND_HANDLLER_H