


#include <stdio.h>
#include <string.h>

// #ifdef __cplusplus
// extern "C" {
// #endif

#include "CommandHandler.h"

//#undef __DEBUG__
//#include "Debug.h"	

#define sizeBuff    127

unsigned char Buffer[sizeBuff];

unsigned char 		(*pIncommingHdl) (void);
void		 		(*pErrorHdl)(unsigned char);
ts_cmd_cntr 	*pCmdContainer;
unsigned char	sizeCmd = 0;
unsigned char   STD_CMD;         


void CM_SYS_INIT( ts_cmd_hdl * cm , cm_init_ts * init )
{
    pIncommingHdl   = init->inc;
    pErrorHdl       = init->err;
    pCmdContainer   = init->container;
    sizeCmd         = init->len;
    STD_CMD         = init->fbyte;
    
    //cm->registerCmd  = cmRegisterCmd;
    cm->available    = cmAvailable;
    cm->run          = cmRun;
    cm->push         = cmPush;
    cm->pop          = cmPop;
    cm->cpy2buff     = cmCpy2buff;
    cm->GetHeader    = cmGetHeader;

    cm->poss_pop     = 0;
    cm->poss_pop     = 0;

}

unsigned char	cmAvailable		(ts_cmd_hdl * self)
{
    return self->poss_push - self->poss_pop;
}

void            cmFlush         (ts_cmd_hdl * self){
	self->poss_push =0;
	self->poss_pop = 0;
}

void			cmPush			(ts_cmd_hdl * self, unsigned char x)
 {
	if (self->poss_push >= sizeBuff)
	{
		cmFlush(self);
	}
	Buffer[self->poss_push] = x;
	self->poss_push++; 
}

unsigned char	cmPop			(ts_cmd_hdl * self)
{
    unsigned char x = (self->poss_push > self->poss_pop) ? Buffer[self->poss_pop] : -1;   
	self->poss_pop=self->poss_pop+1;
	return x;
}
void 			cmCpy2buff		(ts_cmd_hdl * self, unsigned char * c, unsigned char len)
{
    memcpy(c, &Buffer[self->poss_push], len);
    self->poss_push += len;
}
void			cmGetHeader		(ts_cmd_hdl * self, unsigned char * c){
    memcpy(c, Buffer, 5);
}

#define STAGE_END				255
#define STAGE_ERROR				254
#define STAGE_COLLECT_DATA		0	
#define STAGE_CHECK_CMD_HEADER 	1
#define STAGE_SEARCH			2

void			cmRun			(ts_cmd_hdl * self)
{
	
	unsigned char STAGE = STAGE_COLLECT_DATA;
	unsigned char s;
	unsigned char i =0;
	unsigned char len_std = 1;//strlen(STD_CMD);
	unsigned char len_buff;
	self->poss_pop = 0;

	do{
		switch(STAGE){
			case STAGE_COLLECT_DATA:
			{	
				if(pIncommingHdl() != 0x01){
					s = STAGE;
					STAGE = STAGE_END;
					break;
				}
				STAGE = STAGE_CHECK_CMD_HEADER;
			}
			case STAGE_CHECK_CMD_HEADER:
			{
				//DBG_FLASH_PRINT(">:");
				//DBG_PRINT(Buffer);
				//if((strncmp(Buffer,STD_CMD,len_std) != 0) && (Buffer[len_std] != '+')) {
				if(Buffer[0] != STD_CMD)
				{
                	s = STAGE;
					STAGE = STAGE_ERROR;
					break;
				}
				//DBG_FLASH_PRINT(":search.");
				STAGE = STAGE_SEARCH;
			}
			case STAGE_SEARCH:
			{
				char buff[3];
				ts_cmd_cntr * pCntr = &pCmdContainer[i];
                memcpy(buff, pCntr->cmd,3);
				// strcpy(buff, pCmdContainer[i]->cmd);
				//len_buff = 3;
                
				if ( memcmp(&Buffer[1], buff,3) != 0 ) {
					//DBG_FLASH_PRINT('.');
					s = STAGE;
					if(i == sizeCmd)
						STAGE = STAGE_END;
					else
						i++; //next
					break;
				}

				self->poss_pop = 4;
				//DBG_PRINT(buff);
				//DBG_FLASH_PRINT(">");
				//if ((Buffer[self->poss_pop] == '=') || (Buffer[self->poss_pop] == '\0'))
				if( (Buffer[self->poss_pop] > 0) && 
				(self->poss_push == (Buffer[self->poss_pop] + self->poss_pop)))
                {

					// if(Buffer[poss_pop] == '=') {
					// 	poss_pop = poss_pop+1 ;
					// } 
					// else {
					// 	poss_pop = poss_push;
					// }

					//DBG_FLASH_PRINTLN(" Run...");
					// rsp( exe() );
					pCntr->rsp(pCntr->exe());
					i=sizeCmd;
					//DBG_FLASH_PRINTLN("...End");
					cmFlush(self);
					STAGE = STAGE_END;
					break;
				}
				s = STAGE;
				STAGE = STAGE_ERROR;
				break;

			}
			case STAGE_ERROR:
			{
				//DBG_FLASH_PRINTLN(" Error...");
				pErrorHdl(s);				
				STAGE = STAGE_END;
				cmFlush(self);
				break;
			}
		}
	}while(STAGE != STAGE_END);

}

// #ifdef __cplusplus
// }
// #endif
