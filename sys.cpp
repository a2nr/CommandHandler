#include <CommandHandler.h>


unsigned char CommandHandler::available(){
	return poss_push - poss_pop;
}
void CommandHandler::flush(void){
	poss_push =0;
	poss_pop = 0;
	for (unsigned char i = 0; i < CMD_BUFFER; i++)
	{
		Buffer[i] = '\0';
	}
	DBG_PRINTLN("CommandHandler::flush");
}
void CommandHandler::push(char x) {
	if (poss_push >= CMD_BUFFER)
	{
		flush();
	}
	Buffer[poss_push] = x;
	poss_push++; 

	DBG_PRINT("CommandHandler::push() : ");
	DBG_WRITE(x);
	DBG_PRINTLN("")	;
}
char CommandHandler::pop(){
	DBG_PRINTLN("CommandHandler::pop() : ");
	return (poss_push > poss_pop) ? Buffer[poss_pop++] : -1;   
}
void CommandHandler::run(){
	char buff[CHAR_CMD+1];
	// char* s ;
	unsigned char i =0;
	bool errorIndication = true;
	// ptrExecuteFnc exe;
	// ptrRespondFnc rsp;
	poss_pop = 0;
	if(PincommingFncHanddler() == 0x01){
		if(strncmp(Buffer,STD_CMD,strlen(STD_CMD)) == 0){
			DBG_PRINT(" >:");
			if (strchr(Buffer,'+') != NULL){
				errorIndication = true;
				DBG_PRINT(Buffer);
				DBG_PRINT(":search.");
				Container* obj ;
				for (i=0;i <cmdNum; i++) {
					// delay(1);//why ? but work :(
					// obj = (Container *)pgm_read_word(&ptrObjCommandTbl[i]);
					// strcpy_P(buff, (PGM_P)pgm_read_word(&obj->ptrString));
					// obj = (Container*)&ptrObjCommandTbl;
					obj = &ptrObjCommandTbl[i];
					strcpy(buff, obj->ptrString);
					DBG_PRINT(obj->ptrString);
					DBG_PRINT(":");
					if ( strncmp(&Buffer[strlen(STD_CMD)+1], buff,strlen(buff)) == 0 ) {
						poss_pop = strlen(STD_CMD)+strlen(buff)+1;
						DBG_PRINT(i);
						DBG_PRINT(" ");
						DBG_PRINT(buff);
						DBG_PRINT(">");
						if ((Buffer[poss_pop] == '=') || (Buffer[poss_pop] == '\0'))
						{
							poss_pop = (Buffer[poss_pop] == '=') ? poss_pop+1 : poss_push;
							DBG_PRINTLN(" Run...");
							obj->RespondCommand( obj->ExecuteCommand() );
							i=cmdNum;
							errorIndication = false;
						}else {
							errorIndication = true;
						}
						DBG_PRINTLN("...End");
					}
#					ifdef __DEBUG__
					else {
						DBG_PRINT("..");
					}
#					endif
		      	}
		      	if (errorIndication == true)
		      	{
		      		DBG_PRINTLN(" Error...");
		      		errorFncHandler();
		      	}
		    } else {
		    	DBG_PRINTLN(" Error...");
		      	errorFncHandler();
		    }
		    DBG_PRINTLN();
		}
		flush();
	}
}