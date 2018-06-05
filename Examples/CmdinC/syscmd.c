#include "syscmd.h"
#include "test.h"
#include "CommandHandler.h"
#include <stdio.h>
#include <string.h>

unsigned char cmd_incoming(ts_cmd_hdl *self)
{

	unsigned char command[0xFF] = {0xAF, 0x01, 0x00, 0x00, 0x03, 0x01, 0x01, 0x0D};
	unsigned char len;
	unsigned char *p;
	p = command;
	self->push(self, p, 5 + p[4]);

	len = self->available(self);
	printf("\nCommand : \r\n\t");
	while (len--)
		printf(" 0x%02X", *(p++));

	return 0x01;
}

unsigned char cmd_outgoing(ts_cmd_hdl *self)
{
	unsigned char b[0xff];
	unsigned char len = self->available(self);
	unsigned char *pData = self->getParam(self);
	stsrsp *sw = self->getStatusResponds(self);

	printf("\r\nStatus Respond : \n\t");
	if (sw->bits.OK)
	{
		if (!sw->bits.data_available)
		{
			printf("9000");
		}
		else
		{
			printf("91%02d", sw->byte.lo);
		}
	}
	else if (sw->bits.NOK)
	{
		if (!sw->bits.data_available)
		{
			printf("6F00");
		}
		else
		{
			printf("6F%02d", sw->byte.lo);
		}
	}

	printf("\nRespond : \r\n\t");
	if (len != 0)
	{
		memcpy(b, pData, len);
		pData = b;
		while (len--)
			printf(" 0x%02X", *(pData++));
	}
	else
	{
		printf("-");
	}

	printf("\n");
	return 0x01;
}

void cmd_error(ts_cmd_hdl *self, unsigned char s)
{
	printf("\r\nStatus Respond : \n\t6D00\r\n");
}