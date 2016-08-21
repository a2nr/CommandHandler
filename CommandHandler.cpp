#include "CommandHandler.h"

CommandHandler::CommandHandler(byte(*funcIn)(void), 
	void (*errorFnc)(void),
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
void CommandHandler::run(){
	char buff[CHAR_CMD];
	// char* s ;
	byte i =0;
	boolean errorIndication = true;
	ptrExecuteFnc exe;
	ptrRespondFnc rsp;
	poss_pop = 0;
	if(PincommingFncHanddler() == 0x01){
		if(strncmp(Buffer,STD_CMD,strlen(STD_CMD)) == 0){
			DBG_FLASH_PRINT(" >:");
			if (strchr(Buffer,'+') != NULL){
				errorIndication = true;
				DBG_PRINT(Buffer);
				DBG_FLASH_PRINT(":search.");
				for (i=0;i <cmdNum; i++) {
					// delay(1);//why ? but work :(
					strcpy_P(buff, &ptrTSCommandTbl[i].cmdString[0]);
					exe = (ptrExecuteFnc)pgm_read_word(&ptrTSCommandTbl[i].pExecuteFnc);
					rsp = (ptrRespondFnc)pgm_read_word(&ptrTSCommandTbl[i].pRespondFnc);
					if ( strncmp(&Buffer[strlen(STD_CMD)+1], buff,strlen(buff)) == 0 ) {
						poss_pop = strlen(STD_CMD)+strlen(buff)+1;
						DBG_PRINT(buff);
						DBG_FLASH_PRINT(">");
						if ((Buffer[poss_pop] == '=') || (Buffer[poss_pop] == '\0'))
						{
							poss_pop = (Buffer[poss_pop] == '=') ? poss_pop+1 : poss_push;
							DBG_FLASH_PRINTLN(" Run...");
							rsp( exe() );
							i=cmdNum;
							errorIndication = false;
						}else {
							errorIndication = true;
						}
						DBG_FLASH_PRINTLN("...End");
					}
#					ifdef __DEBUG__
					else {
						DBG_FLASH_PRINT("..");
					}
#					endif
		      	}
		      	if (errorIndication == true)
		      	{
		      		DBG_FLASH_PRINTLN(" Error...");
		      		errorFncHandler();
		      	}
		    } else {
		    	DBG_FLASH_PRINTLN(" Error...");
		      	errorFncHandler();
		    }
		    DBG_PRINTLN();
		}
		flush();
	}
}