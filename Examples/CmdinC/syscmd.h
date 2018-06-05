#ifndef SYSCMD_H
#define SYSCMD_H

#include "CommandHandler.h"

unsigned char cmd_incoming(ts_cmd_hdl *);
unsigned char cmd_outgoing(ts_cmd_hdl *);
void cmd_error(ts_cmd_hdl *, unsigned char );


#endif