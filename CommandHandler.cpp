#include "CommandHandler.h"

CommandHandler::CommandHandler(byte(*funcIn)(void), 
	void (*errorFnc)(byte),
	const TSCmdTbl *cmdTbl, 
	byte len){

	DBG_INIT(); // for debug
	cmdNum = len;
	errorFncHandler = errorFnc;
	PincommingFncHanddler = funcIn; 
	ptrTSCommandTbl = cmdTbl;
	flush();
}
byte CommandHandler::available(){
	return poss_push - poss_pop;
}
void CommandHandler::flush(void){
	poss_push =0;
	poss_pop = 0;
	for (byte i = 0; i < CMD_BUFFER; i++)
	{
		Buffer[i] = '\0';
	}
}
void CommandHandler::push(char x) {
	if (poss_push >= CMD_BUFFER)
	{
		flush();
	}
	Buffer[poss_push] = x;
	poss_push++; 
}
char CommandHandler::pop(){
	return (poss_push > poss_pop) ? Buffer[poss_pop++] : -1;   
}

#define STAGE_END				255
#define STAGE_ERROR				254
#define STAGE_COLLECT_DATA		0	
#define STAGE_CHECK_CMD_HEADER 	1
#define STAGE_SEARCH			2

void CommandHandler::run(){
	
	byte STAGE = STAGE_COLLECT_DATA;
	byte s;
	// char* s ;
	byte i =0;
	byte len_std = strlen(STD_CMD);
	byte len_buff;
	// boolean errorIndication = true;
	poss_pop = 0;

	do{
		switch(STAGE){
			case STAGE_COLLECT_DATA:
			{	
				if(PincommingFncHanddler() != 0x01){
					s = STAGE;
					STAGE = STAGE_END;
					break;
				}
				STAGE = STAGE_CHECK_CMD_HEADER;
			}
			case STAGE_CHECK_CMD_HEADER:
			{
				DBG_FLASH_PRINT(">:");
				DBG_PRINT(Buffer);
				if((strncmp(Buffer,STD_CMD,len_std) != 0) && (Buffer[len_std] != '+')) {
					s = STAGE;
					STAGE = STAGE_ERROR;
					break;
				}
				DBG_FLASH_PRINT(":search.");
				STAGE = STAGE_SEARCH;
			}
			case STAGE_SEARCH:
			{
				char buff[CHAR_CMD];
				ptrExecuteFnc exe = (ptrExecuteFnc)pgm_read_word(&ptrTSCommandTbl[i].pExecuteFnc);
				ptrRespondFnc rsp = (ptrRespondFnc)pgm_read_word(&ptrTSCommandTbl[i].pRespondFnc);

				strcpy_P(buff, &ptrTSCommandTbl[i].cmdString[0]);
				len_buff = strlen(buff);

				if ( strncmp(&Buffer[len_std+1], buff,len_buff) != 0 ) {
					DBG_FLASH_PRINT("..");
					s = STAGE;
					if(i == cmdNum)
						STAGE = STAGE_END;
					else
						i++; //next
					break;
				}
				poss_pop = len_std+len_buff+1;
				DBG_PRINT(buff);
				DBG_FLASH_PRINT(">");
				if ((Buffer[poss_pop] == '=') || (Buffer[poss_pop] == '\0'))
				{
					poss_pop = (Buffer[poss_pop] == '=') ? poss_pop+1 : poss_push;
					DBG_FLASH_PRINTLN(" Run...");
					rsp( exe() );
					i=cmdNum;
					DBG_FLASH_PRINTLN("...End");
					
					STAGE = STAGE_END;
					break;
				}
				s = STAGE;
				STAGE = STAGE_ERROR;
				break;

			}
			case STAGE_ERROR:
			{
				DBG_FLASH_PRINTLN(" Error...");
				errorFncHandler(s);				
				STAGE = STAGE_END;
				flush();
				break;
			}
		}
	}while(STAGE != STAGE_END);

	






// 	if(PincommingFncHanddler() == 0x01){
// 		if(strncmp(Buffer,STD_CMD,strlen(STD_CMD)) == 0){
// 			DBG_FLASH_PRINT(" >:");
// 			if (strchr(Buffer,'+') != NULL){
// 				errorIndication = true;
// 				DBG_PRINT(Buffer);
// 				DBG_FLASH_PRINT(":search.");
// 				for (i=0;i <cmdNum; i++) {
// 					// delay(1);//why ? but work :(
// 					strcpy_P(buff, &ptrTSCommandTbl[i].cmdString[0]);
// 					exe = (ptrExecuteFnc)pgm_read_word(&ptrTSCommandTbl[i].pExecuteFnc);
// 					rsp = (ptrRespondFnc)pgm_read_word(&ptrTSCommandTbl[i].pRespondFnc);
// 					if ( strncmp(&Buffer[strlen(STD_CMD)+1], buff,strlen(buff)) == 0 ) {
// 						poss_pop = strlen(STD_CMD)+strlen(buff)+1;
// 						DBG_PRINT(buff);
// 						DBG_FLASH_PRINT(">");
// 						if ((Buffer[poss_pop] == '=') || (Buffer[poss_pop] == '\0'))
// 						{
// 							poss_pop = (Buffer[poss_pop] == '=') ? poss_pop+1 : poss_push;
// 							DBG_FLASH_PRINTLN(" Run...");
// 							rsp( exe() );
// 							i=cmdNum;
// 							errorIndication = false;
// 						}else {
// 							errorIndication = true;
// 						}
// 						DBG_FLASH_PRINTLN("...End");
// 					}
// #					ifdef __DEBUG__
// 					else {
// 						DBG_FLASH_PRINT("..");
// 					}
// #					endif
// 		      	}
// 		      	if (errorIndication == true)
// 		      	{
// 		      		DBG_FLASH_PRINTLN(" Error...");
// 		      		errorFncHandler();
// 		      	}
// 		    } else {
// 		    	DBG_FLASH_PRINTLN(" Error...");
// 		      	errorFncHandler();
// 		    }
// 		    DBG_PRINTLN();
// 		}
// 		flush();
// 	}
}