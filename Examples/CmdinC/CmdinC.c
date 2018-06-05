#include <stdio.h>

#include "CommandHandler.h"
#include "syscmd.h"

ts_cmd_hdl Command;

unsigned int iod_exe(ts_cmd_hdl *);
unsigned int ioa_exe(ts_cmd_hdl *);

ts_cmd_cntr ALL_CMD[] = {
	{{0x01, 0x00, 0x00}, iod_exe},
	{{0x02, 0x00, 0x00}, ioa_exe}};

int main(int argc, char *argv[])
{
	int x;
	cm_init_ts ts_cmdInit;
	printf("\n Hello this is the test app for my CommandHandler Library");
	ts_cmdInit.inc = &cmd_incoming;
	ts_cmdInit.out = &cmd_outgoing;
	ts_cmdInit.err = &cmd_error;
	ts_cmdInit.container = ALL_CMD;
	ts_cmdInit.fbyte = 0xAF;
	ts_cmdInit.len = 2;
	ts_cmdInit.sizeBuf = (unsigned char)(0xFF);
	Command = cmNew_Command(&ts_cmdInit);

	Command.run(&Command);
}

unsigned int iod_exe(ts_cmd_hdl *self)
{
	unsigned char tmp;
	unsigned int len = self->available(self);
	typedef union _iod_param {
		struct _param
		{
			unsigned char mode;
			unsigned char prm[2];
		} b;
	} iod_param;
	iod_param *pParam = NULL;

	if (len != sizeof(iod_param))
	{
		len = sizeof(iod_param);
		return self->Return(self, 0x0 /*NOK*/, (unsigned char *)&len);
	}

	pParam = (iod_param *)self->getParam(self);

	if (pParam == NULL)
		return self->Return(self, 0x0 /*NOK*/, NULL);

	self->push(self, &pParam->b.mode, 1);
	printf("\n\theader(0x%02x);", pParam->b.mode);
	switch (pParam->b.mode)
	{
	case 0x01:
		printf("\n\tparam(0x%02x,0x%02x);", pParam->b.prm[0], pParam->b.prm[1]);
		return self->Return(self, 0x1 /*OK*/, NULL);
	case 0xF0:
		printf("\n\tparam(0x%02x,0x%02x);", pParam->b.prm[0], pParam->b.prm[1]);
		return self->Return(self, 0x1 /*OK*/, NULL);
	case 0x00:
		printf("\n\tparam(0x%02x);", pParam->b.prm[0]);
		return self->Return(self, 0x1 /*OK*/, NULL);
	default:
		return 0xEF;
	}
}

unsigned int ioa_exe(ts_cmd_hdl *self)
{
	unsigned char c; 

	switch (c)
	{
	case 'I':
	{
		switch (c)
		{
		case 0xA0:
		case 0xA1:
		case 0xA2:
		case 0xA3:
		case 0xA4:
		case 0xA5:
			break;
		}
	}
	}
	return 0x00;
}
