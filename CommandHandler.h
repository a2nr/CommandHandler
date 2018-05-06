#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

//#include <Arduino.h>
// #include <Stream.h>
// #include <stdio.h>
// #include <string.h>
//#include <avr/pgmspace.h>

// #undef __DEBUG__
// #include "Debug.h"	


// #define STD_CMD		"AN"

// class CommandContainer {
// 	public:ifndef
// 	const char * cmd;
// 	CommandContainer(const char * c):cmd(c) {

// 	}
// 	virtual byte exe(void);
// 	virtual void rsp(byte);
// };
typedef struct ts_cmd_cntr	ts_cmd_cntr;
typedef struct ts_cmd_hdl	ts_cmd_hdl;
typedef unsigned char 	(*pTuc_fnc_v)	(ts_cmd_hdl * self);
typedef void 			(*pTv_fnc_uc)	(ts_cmd_hdl * self, unsigned char);
typedef void 			(*pTv_fnc_v)	(ts_cmd_hdl * self);
struct ts_cmd_cntr {
	unsigned char 	cmd[3];
	unsigned char 	(*exe) (void);
	void  			(*rsp) (unsigned char);
};
// template <unsigned int sizeCmd, unsigned int lenHeader, unsigned int sizeBuff>
// class CommandHandler
// {
// private:
// 	DBG_DECLARATION();
// 	char Buffer[sizeBuff];
// 	byte (*PincommingFncHanddler)(void);
// 	void (*errorFncHandler)(byte);
// 	CommandContainer *cc[sizeCmd];
// 	byte poss_push,poss_pop;
// 	void flush(void);
// public:
// 	CommandHandler();
// 	void attachSys(byte(*funcIn)(void), void (*error)(byte));
// 	void registerCmd(CommandContainer *c[]);
// 	unsigned int maxCommand(){return sizeBuff;}
// 	void run();
// 	void push(char);
// 	char pop();
// 	void cpyChar(char * c, byte l);
// 	byte available();

// };

struct ts_cmd_hdl {

	unsigned char 	poss_push;
	unsigned char	poss_pop;

//	void 		(*attachSys) 	(ts_cmd_hdl * , pTuc_fnc_v	 , pTv_fnc_uc);
//	void 		(*registerCmd)	(ts_cmd_hdl * , ts_cmd_cntr* , unsigned char );

	pTuc_fnc_v	available;
	pTv_fnc_v 	run;
	pTv_fnc_uc 	push;
	pTuc_fnc_v 	pop;

	void		(*GetHeader)	(ts_cmd_hdl * self, unsigned char * c);
	void		(*cpy2buff)		(ts_cmd_hdl * , unsigned char * , unsigned char );
	
};

//void 			cmAttachSys		(ts_cmd_hdl * self, pTuc_fnc_v 	inc, 			pTv_fnc_uc 		err);
//void			cmRegisterCmd	(ts_cmd_hdl * self, ts_cmd_cntr *pContainer,	unsigned char 	len);

unsigned char	cmAvailable		(ts_cmd_hdl * self);
void			cmRun			(ts_cmd_hdl * self);
void			cmPush			(ts_cmd_hdl * self, unsigned char);
unsigned char	cmPop			(ts_cmd_hdl * self);
void 			cmCpy2buff		(ts_cmd_hdl * self, unsigned char * c, unsigned char len);
void			cmGetHeader		(ts_cmd_hdl * self, unsigned char * c);
void            cmFlush         (ts_cmd_hdl * self);


typedef struct cm_init_ts {
	unsigned char 	(*inc) (void);
	void  			(*err) (unsigned char);	

	ts_cmd_cntr		*container;	
	unsigned char	fbyte;	
	unsigned char 	len;
}cm_init_ts;

void CM_SYS_INIT( ts_cmd_hdl * cm , cm_init_ts * init );

//#include "cpp/CommandHandler.cpp"
#endif //COMMAND_HANDLLER_H