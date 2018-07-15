#ifndef COMMAND_CLASS_H
#define COMMAND_CLASS_H
//#include "CommandHandler.h"

#ifndef COMMAND_CLASS_MAX_COMMAND
#define COMMAND_CLASS_MAX_COMMAND 1
#endif

#define COMMAND_CLASS_SIZE_BUFFER 0xFF + 9
#define COMMAND_CLASS_MAX_INDEX ((COMMAND_CLASS_SIZE_BUFFER)*COMMAND_CLASS_MAX_COMMAND)

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
	unsigned char byte[COMMAND_CLASS_SIZE_BUFFER];
} tuSharedBufferCommand;

typedef tuSharedBufferCommand *ptrBuffer;

template <typename T>
class CommandHandler;
template <typename T>
class user_command;

template <typename C>
struct cm_init_class
{
	unsigned char (C::*inc)(CommandHandler<C> *cmd);
	unsigned char (C::*out)(CommandHandler<C> *cmd);
	void (C::*err)(unsigned char c);

	user_command<C> **all_user_command;

	unsigned char length_user_command;
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
	unsigned long pos_push;
	unsigned long pos_pop;
	ptrBuffer buffer;
	user_command<T> **ptrUser;
	unsigned long lengtUserCmd;
	T *obj;
	unsigned char (T::*pIncomming)(CommandHandler<T> *cmd);
	unsigned char (T::*pOutgoing)(CommandHandler<T> *cmd);
	void (T::*pError)(unsigned char);
	void flush(void){
		pos_push =0 ;
		pos_pop = 0;

	}

  public:
	CommandHandler();
	// CommandHandler(T *o, struct cm_init_class<T> *init);
	~CommandHandler();

	void init(T *o, struct cm_init_class<T> *init);
	unsigned long available();
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
	inline unsigned char* operator=(unsigned long length_push) { this->pos_push = length_push; return (unsigned char * )&this->buffer;}
	inline unsigned char* operator[](unsigned long idx) { return (this->buffer != nullptr) ? &this->buffer->byte[idx] : nullptr; }
	// unsigned long operator=(unsigned long length_push) { this->pos_push = length_push; return this->pos_push;}
};

#include "src/CommandClass.cxx"
#endif