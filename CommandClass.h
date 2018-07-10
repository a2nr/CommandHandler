#ifndef COMMAND_CLASS_H
#define COMMAND_CLASS_H
//#include "CommandHandler.h"

#ifndef COMMAND_CLASS_MAX_COMMAND
#define COMMAND_CLASS_MAX_COMMAND 1
#endif

#define sizeBuff 0xFF + 7

struct sHeader
{
	unsigned char fb;
	unsigned char ins;
	unsigned char c1;
	unsigned char c2;
	unsigned char lenParam;
};
struct byteInWord
{
	unsigned char high;
	unsigned char low;
};
struct stsInWord
{
	unsigned long data_available : 1;
	unsigned long OK : 1;
	unsigned long NOK : 1;
	unsigned long RFU : 5;
};
typedef union cmd_header_ {
	struct sHeader byte;
} tuCmdHeader;
typedef union cmd_sw_ {
	struct byteInWord byte;
	struct stsInWord bit;
	unsigned long WORD;
} tuStsResp;

typedef unsigned char tuParam[0xFF];

typedef union tuSharedBufferCommand_ {
	struct in_command
	{
		tuCmdHeader header;
		tuParam param;
	} in;
	struct out_command
	{
		tuStsResp sw;
		tuParam param;
	} out;
	unsigned char byte[sizeBuff];
} tuSharedBufferCommand;

typedef tuSharedBufferCommand *ptrBuffer;

template <typename T>
class CommandHandler;
template <typename T>
class user_command;

template <typename C>
struct cm_init_class
{
	unsigned char (C::*inc)();
	unsigned char (C::*out)();
	void (C::*err)(unsigned char c);

	user_command<C> *all_user_command;

	unsigned char fbyte;
	unsigned char length_user_command;
	// unsigned char sizeBuf;
};

template <typename T>
class user_command
{
	friend CommandHandler<T>;

  protected:
	user_command(){};

  public:
	virtual unsigned short exe(CommandHandler<T> *cmd)
	{
		//default value;
	}
	virtual const char *getCommand(void)
	{
		return "\xff\xff\xff";
	}
};

template <typename T>
class CommandHandler
{
  private:
	//ts_cmd_hdl hdl;
	unsigned long pos_push;
	unsigned long pos_pop;
	ptrBuffer buffer;
	user_command<T> *ptrUser;
	unsigned long lengtUserCmd;
	T *obj;
	unsigned char (T::*pIncomming)();
	unsigned char (T::*pOutgoing)();
	void (T::*pError)(unsigned char);

  public:
	CommandHandler(){};
	CommandHandler(T *o, struct cm_init_class<T> *init);

	unsigned char available();
	void run();
	void push(const unsigned char *x, unsigned int len);
	void *getParam();
	unsigned long Return(unsigned char oknok, unsigned char info);
	unsigned long *getStatusResponds()
	{
		return &this->buffer->out.sw.WORD;
	}
	void *getHeader()
	{
		return (void *)&this->buffer->in.header;
	}
	unsigned char* operator[](unsigned long idx) { return &this->buffer->byte[idx]; }
	unsigned long operator=(unsigned long length_push) { this->pos_push = length_push; return this->pos_push;}
};

#include "src/CommandClass.cxx"
#endif