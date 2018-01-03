#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <avr/pgmspace.h>

#undef __DEBUG__
#include "Debug.h"


#define STD_CMD			"ANU"
#define CHAR_CMD		6
#define CMD_BUFFER		127

class CommandHandler
{
private:
	DBG_DECLARATION();

	char Buffer[CMD_BUFFER];
	unsigned char poss_push,poss_pop;
protected:
	void flush(void);
	void push(char);
	char pop();
	unsigned char available();
public:

	CommandHandler()
	{
			flush();
			DBG_INIT(9600);
			DBG_PRINTLN("CommandHandler Debug Mode :)");
	}
	virtual unsigned char CommandCollector(void) { return 0;};
	virtual void CommandError(void) { } ;
	void run();
					

};

#endif //COMMAND_HANDLLER_H