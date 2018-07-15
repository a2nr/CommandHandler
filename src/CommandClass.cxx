
#include "CommandClass.h"
#include <string.h>

#define STAGE_END 255
#define STAGE_ERROR 254
#define STAGE_COLLECT_DATA 0
#define STAGE_CHECK_CMD_HEADER 1
#define STAGE_SEARCH 2
#define STAGE_EXE 3
#define STAGE_RESP 4

static unsigned char sgBuffer[COMMAND_CLASS_MAX_INDEX];
static unsigned int currentSize = 0;
static unsigned char STAGE = STAGE_COLLECT_DATA;

template <typename T>
CommandHandler<T>::CommandHandler()
{
  if (currentSize >= COMMAND_CLASS_MAX_INDEX)
  {
    this->buffer = nullptr;
    return;
  }
  this->buffer = (ptrBuffer)&sgBuffer[currentSize];
  currentSize += sizeof(tuSharedBufferCommand);
}
template <typename T>
CommandHandler<T>::~CommandHandler()
{
  if (currentSize != 0)
    currentSize = currentSize - sizeof(tuSharedBufferCommand);
}
template <typename T>
void CommandHandler<T>::init(T *o, struct cm_init_class<T> *init)
{
  if (this->buffer != nullptr)
  {
    obj = o != nullptr ? o : nullptr;
    this->pIncomming = init->inc != nullptr ? init->inc : nullptr;
    this->pOutgoing = init->out != nullptr ? init->out : nullptr;
    this->pError = init->err != nullptr ? init->err : nullptr;
    this->lengtUserCmd = init->length_user_command;
    this->ptrUser = init->all_user_command != nullptr ? init->all_user_command : nullptr;
    this->pos_pop = 0;
    this->pos_push = 0;
  }
};
template <typename T>
unsigned long CommandHandler<T>::available()
{
  return this->pos_push - this->pos_pop;
}

template <typename T>
void CommandHandler<T>::push(const unsigned char *x, unsigned int len)
{
  unsigned char *ptrBuff = nullptr;
  switch (STAGE)
  {
  case STAGE_COLLECT_DATA:
  {
    ptrBuff = this->buffer->byte;
    break;
  }
  case STAGE_EXE:
  {
    ptrBuff = this->buffer->out.param;
    break;
  }
  default:
    break;
  }
  memcpy(ptrBuff, x, len);
  this->pos_push += len;
}
template <typename T>
void *CommandHandler<T>::getParam()
{
  unsigned char *ptrBuff = nullptr;
  switch (STAGE)
  {
  case STAGE_RESP:
  {
    ptrBuff = this->buffer->out.param;
    break;
  }
  case STAGE_EXE:
  {
    ptrBuff = this->buffer->in.param;
    break;
  }
  default:
    break;
  }
  return (void *)ptrBuff;
}
template <typename T>
unsigned long CommandHandler<T>::Return(unsigned char oknok, unsigned char info)
{
  this->buffer->out.sw.WORD = 0;

  if (oknok)
    this->buffer->out.sw.bit.OK = 0x1;
  else
  {
    this->buffer->out.sw.bit.NOK = 0x1;
  }
  if (this->pos_push > 0)
  {
    this->buffer->out.sw.bit.data_available = 0x1;
    this->buffer->out.sw.byte.low = this->pos_push;
  }
  else
  {

    this->buffer->out.sw.byte.low = info;
  }

  return this->buffer->out.sw.WORD;
}

template <typename T>
void CommandHandler<T>::run()
{
  unsigned long i = 0;
  user_command<T> *ptrCurrentUser = nullptr;
  if (nullptr != this->buffer)
    STAGE = STAGE_COLLECT_DATA;
  else
    STAGE = STAGE_ERROR;
  do
  {
    switch (STAGE)
    {
    case STAGE_COLLECT_DATA:
    {
      this->flush();
      if ((this->obj->*pIncomming)(this) != 0x01)
      {
        STAGE = STAGE_END;
        break;
      }
      STAGE = STAGE_SEARCH;
    }
    case STAGE_SEARCH:
    {
      if (this->lengtUserCmd < i)
      {
        STAGE = STAGE_ERROR;
        break;
      }
      else
      {
        ptrCurrentUser = this->ptrUser[i];
        const char *s = ptrCurrentUser->getCommand();
        if (this->buffer->in.header.byte.ins != s[0])
        {

          i++;
        }
        else
        {
          STAGE = STAGE_EXE;
        }
      }
    }
    case STAGE_EXE:
    {
      if (this->pos_push > (sizeof(tuCmdHeader) + this->buffer->in.header.byte.lenParam))
      {
        STAGE = STAGE_ERROR;
        break;
      }
      else
      {
        this->flush();
        this->buffer->out.sw.WORD = ptrCurrentUser->exe(this);
        STAGE = STAGE_RESP;
      }
    }
    case STAGE_RESP:
    {
      if ((this->obj->*pOutgoing)(this) != 0x01)
      {
        STAGE = STAGE_ERROR;
        break;
      }
      else
      {
        STAGE = STAGE_END;
        break;
      }
    }
    case STAGE_ERROR:
    {
      (this->obj->*pError)(STAGE - 1);
      STAGE = STAGE_END;
    }
    }
  } while (STAGE != STAGE_END);
}