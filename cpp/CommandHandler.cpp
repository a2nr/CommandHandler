#include "MemoryFree.h"
template <unsigned int sizeCmd, unsigned int lenHeader, unsigned int sizeBuff>
CommandHandler<sizeCmd,lenHeader, sizeBuff>::
CommandHandler(){

	DBG_INIT(); // for debug
	flush();
}

template <unsigned int sizeCmd, unsigned int lenHeader, unsigned int sizeBuff>
void CommandHandler<sizeCmd,lenHeader, sizeBuff>::
attachSys(byte(*funcIn)(void), void (*error)(byte)){
	PincommingFncHanddler = funcIn;
	errorFncHandler = error;
}


template <unsigned int sizeCmd, unsigned int lenHeader, unsigned int sizeBuff>
void CommandHandler<sizeCmd,lenHeader, sizeBuff>::
registerCmd(CommandContainer *c[]){
	for(byte i = 0; i <sizeCmd; i++){
		cc[i] = c[i];
	}
}


template <unsigned int sizeCmd, unsigned int lenHeader, unsigned int sizeBuff>
byte CommandHandler<sizeCmd,lenHeader, sizeBuff>::
available(){
	return poss_push - poss_pop;
}
template <unsigned int sizeCmd, unsigned int lenHeader, unsigned int sizeBuff>
void CommandHandler<sizeCmd,lenHeader, sizeBuff>::
flush(void){
	poss_push =0;
	poss_pop = 0;
	DBG_FLASH_PRINT("Curent RAM- >");
	DBG_PRINTLN(freeMemory());
	DBG_FLASH_PRINT("Flushing- >");
	DBG_PRINTLN(Buffer);
	for (byte i = 0; i < sizeBuff; i++)
	{
		Buffer[i] = '\0';
	}
}
template <unsigned int sizeCmd, unsigned int lenHeader, unsigned int sizeBuff>
void CommandHandler<sizeCmd,lenHeader, sizeBuff>::
push(char x) {
	if (poss_push >= sizeBuff)
	{
		flush();
	}
	Buffer[poss_push] = x;
	poss_push++; 
}
template <unsigned int sizeCmd, unsigned int lenHeader, unsigned int sizeBuff>
void CommandHandler<sizeCmd,lenHeader, sizeBuff>::
cpyChar(char * c, byte l){
	strlcpy(c, Buffer, l);
}


template <unsigned int sizeCmd, unsigned int lenHeader, unsigned int sizeBuff>
char CommandHandler<sizeCmd,lenHeader, sizeBuff>::
pop(){
	char x = (poss_push > poss_pop) ? Buffer[poss_pop] : -1;   
	poss_pop=poss_pop+1;
	return x;
}

#define STAGE_END				255
#define STAGE_ERROR				254
#define STAGE_COLLECT_DATA		0	
#define STAGE_CHECK_CMD_HEADER 	1
#define STAGE_SEARCH			2

template <unsigned int sizeCmd, unsigned int lenHeader, unsigned int sizeBuff>
void CommandHandler<sizeCmd,lenHeader, sizeBuff>::
run(){
	
	byte STAGE = STAGE_COLLECT_DATA;
	byte s;
	byte i =0;
	byte len_std = strlen(STD_CMD);
	byte len_buff;
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
				char buff[lenHeader];
				strcpy(buff, cc[i]->cmd);

				len_buff = strlen(buff);

				if ( strncmp(&Buffer[len_std+1], buff,len_buff) != 0 ) {
					DBG_FLASH_PRINT("..");
					s = STAGE;
					if(i == sizeCmd)
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
					if(Buffer[poss_pop] == '=') {
						poss_pop = poss_pop+1 ;
					} 
					else {
						poss_pop = poss_push;
					}

					DBG_FLASH_PRINTLN(" Run...");
					// rsp( exe() );
					cc[i]->rsp(cc[i]->exe());
					i=sizeCmd;
					DBG_FLASH_PRINTLN("...End");
					flush();
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

}