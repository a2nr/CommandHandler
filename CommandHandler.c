

#include <stdio.h>
#include <string.h>

#include "CommandHandler.h"

#ifndef CM_SET_SIZE_BUFF
#define CM_SET_SIZE_BUFF 0x108
#endif

#ifndef CM_SET_TOTAL_KHIND_COMMAND
#define CM_SET_TOTAL_KHIND_COMMAND 1
#endif

#define CM_SIZE_PRIVATE_DATA 7
#define CM_SIZE_BUFFER (CM_SET_SIZE_BUFF + CM_SIZE_PRIVATE_DATA) * CM_SET_TOTAL_KHIND_COMMAND

typedef union _header { 
	struct _d
	{
		unsigned char cmdStd;
		unsigned char cmd[3];
		unsigned char cmdLen;
		unsigned char firstParam;
	} d;
} tu_header;

typedef union _share_buff {
	unsigned char b[CM_SET_SIZE_BUFF];
	struct _rsp
	{
		stsrsp statusRespond;
		unsigned char b[CM_SET_SIZE_BUFF];
	} resp;
} tu_sb;

union un_cmd_data {
	struct cmData
	{
		unsigned char poss_push;
		unsigned char poss_pop;
		unsigned char STD_CMD;
		unsigned char sizeCmd;
		unsigned char sizeBuff;
		tu_sb buffer;
	} data;
};

unsigned char GLOBAL_BUFFER[CM_SIZE_BUFFER];
unsigned int currentSize = 0;

static unsigned char cmAvailable(ts_cmd_hdl *self);
static void cmRun(ts_cmd_hdl *self);
static void cmPush(ts_cmd_hdl *self, const unsigned char *x, unsigned int len);
static void cmRespPush(ts_cmd_hdl *self, const unsigned char *x, unsigned int len);
static void *cmGetParam(ts_cmd_hdl *self);
static void *cmGetParamRsp(ts_cmd_hdl *self);
static void cmFlush(ts_cmd_hdl *self, unsigned int offsetHigh);
static unsigned int cmReturn(ts_cmd_hdl *self, unsigned char oknok, unsigned char *p);
static inline stsrsp *cmGetStatusResponds(ts_cmd_hdl *self);

ts_cmd_hdl cmNew_Command(cm_init_ts *init)
{
	ts_cmd_hdl cm;
	unsigned int tmp = sizeof(un_prv_data);

	GLOBAL_BUFFER[5] = 0xFF;
	cm.pPrvt = (un_prv_data *)&GLOBAL_BUFFER[currentSize];
	currentSize = currentSize + tmp;

	cm.pPrvt->data.sizeBuff = init->sizeBuf;
	cm.pPrvt->data.sizeCmd = init->len;
	cm.pPrvt->data.STD_CMD = init->fbyte;
	cm.pPrvt->data.poss_pop = 0;
	cm.pPrvt->data.poss_push = 0;

	cm.pIncommingHdl = init->inc;
	cm.pOutgoingHdl = init->out;
	cm.pCmdContainer = init->container;
	cm.pErrorHdl = init->err;

	cm.available = cmAvailable;
	cm.run = cmRun;
	cm.push = cmPush;
	cm.getParam = cmGetParam;
	cm.Return = cmReturn;
	cm.getStatusResponds = cmGetStatusResponds;

	return cm;
}

static unsigned char cmAvailable(ts_cmd_hdl *self)
{
	return self->pPrvt->data.poss_push - self->pPrvt->data.poss_pop;
}

static void cmFlush(ts_cmd_hdl *self, unsigned int offsetHigh)
{
	if ((self->pPrvt->data.poss_push > self->pPrvt->data.sizeBuff) || ((self->pPrvt->data.poss_push + offsetHigh) > self->pPrvt->data.sizeBuff) || (self->pPrvt->data.poss_push == self->pPrvt->data.poss_pop))
	{
		self->pPrvt->data.poss_push = 0;
		self->pPrvt->data.poss_pop = 0;
	}
}
static void cmRespPush(ts_cmd_hdl *self, const unsigned char *x, unsigned int len)
{
	cmFlush(self, len);
	memcpy(&self->pPrvt->data.buffer.resp.b[self->pPrvt->data.poss_push], x, len);
	self->pPrvt->data.poss_push += len;
}
static void cmPush(ts_cmd_hdl *self, const unsigned char *x, unsigned int len)
{
	cmFlush(self, len);
	memcpy(&self->pPrvt->data.buffer.b[self->pPrvt->data.poss_push], x, len);
	self->pPrvt->data.poss_push += len;
}

static void *cmGetParamGeneric(ts_cmd_hdl *self, unsigned char *b)
{
	if (b != NULL)
	{
		unsigned int l = self->available(self);
		self->pPrvt->data.poss_pop += l;
		cmFlush(self, 0);
		return (void *)b;
	}
	return NULL;
}
static void *cmGetParamRsp(ts_cmd_hdl *self)
{
	return cmGetParamGeneric(self, ((self->pPrvt->data.poss_push > self->pPrvt->data.poss_pop)
										? &self->pPrvt->data.buffer.resp.b[self->pPrvt->data.poss_pop]
										: NULL));
}
static void *cmGetParam(ts_cmd_hdl *self)
{
	return cmGetParamGeneric(self, ((self->pPrvt->data.poss_push > self->pPrvt->data.poss_pop)
										? &self->pPrvt->data.buffer.b[self->pPrvt->data.poss_pop]
										: NULL));
}

static unsigned int cmReturn(ts_cmd_hdl *self, unsigned char oknok, unsigned char *p)
{
	stsrsp sr;
	sr.word = 0;

	if (oknok)
		sr.bits.OK = 0x1;
	else
	{
		cmFlush(self, 0);
		sr.bits.NOK = 0x1;
	}
	if (self->pPrvt->data.poss_push > 0)
	{
		sr.bits.data_available = 0x1;
		sr.byte.lo = self->pPrvt->data.poss_push;
	}
	else
	{
		if (p != NULL)
		{
			sr.byte.lo = *p;
		}
	}
	//self->pPrvt->data.poss_push += 2;
	return sr.word;
}

static inline stsrsp *cmGetStatusResponds(ts_cmd_hdl *self)
{
	return &self->pPrvt->data.buffer.resp.statusRespond;
}

#define STAGE_END 255
#define STAGE_ERROR 254
#define STAGE_COLLECT_DATA 0
#define STAGE_CHECK_CMD_HEADER 1
#define STAGE_SEARCH 2

static void cmRun(ts_cmd_hdl *self)
{

	unsigned char STAGE = STAGE_COLLECT_DATA;
	unsigned char s;
	unsigned char i = 0;
	unsigned char len_std = 1; //strlen(STD_CMD);
	unsigned char len_buff;
	tu_header *pHeader;

	self->pPrvt->data.poss_pop = 0;

	do
	{
		switch (STAGE)
		{
		case STAGE_COLLECT_DATA:
		{
			if (self->pIncommingHdl(self) != 0x01)
			{
				s = STAGE;
				STAGE = STAGE_END;
				break;
			}
			pHeader = (tu_header *)&self->pPrvt->data.buffer.b[0];
			STAGE = STAGE_CHECK_CMD_HEADER;
		}
		case STAGE_CHECK_CMD_HEADER:
		{
			if (pHeader->d.cmdStd != self->pPrvt->data.STD_CMD)
			{
				s = STAGE;
				STAGE = STAGE_ERROR;
				break;
			}
			STAGE = STAGE_SEARCH;
		}
		case STAGE_SEARCH:
		{
			ts_cmd_cntr *pCntr = &self->pCmdContainer[i];

			if (memcmp(pHeader->d.cmd, pCntr->cmd, 3) != 0)
			{
				s = STAGE;
				if (i == self->pPrvt->data.sizeCmd)
					STAGE = STAGE_ERROR;
				else
					i++; //next
				break;
			}

			if ((pHeader->d.cmdLen > 0) || (self->pPrvt->data.poss_push == (pHeader->d.cmdLen + 5)))
			{
				self->pPrvt->data.poss_pop = 5;
				self->push = cmRespPush;
				self->pPrvt->data.buffer.resp.statusRespond.word = pCntr->exe(self);
				self->getParam = cmGetParamRsp;
				self->pOutgoingHdl(self);
				self->getParam = cmGetParam;
				self->push = cmPush;
				i = self->pPrvt->data.sizeCmd;
				cmFlush(self, 0);
				STAGE = STAGE_END;
				break;
			}
			s = STAGE;
			STAGE = STAGE_ERROR;
			break;
		}
		case STAGE_ERROR:
		{
			self->pErrorHdl(self, s);
			STAGE = STAGE_END;
			cmFlush(self, 0);
			break;
		}
		}
	} while (STAGE != STAGE_END);
}