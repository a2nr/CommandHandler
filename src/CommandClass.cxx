
#include "CommandClass.h"
#include <string.h>

#define STAGE_END 255
#define STAGE_ERROR 254
#define STAGE_COLLECT_DATA 0
#define STAGE_CHECK_CMD_HEADER 1
#define STAGE_SEARCH 2
#define STAGE_EXE 3
#define STAGE_RESP 4

static unsigned char sgBuffer[((sizeBuff)*COMMAND_CLASS_MAX_COMMAND)];
static unsigned int currentSize = 0;
static unsigned char STAGE = STAGE_COLLECT_DATA;

template <typename T>
CommandHandler<T>::CommandHandler(T *o, struct cm_init_class<T> *init)
{

  obj = o;
  this->pIncomming = init->inc;
  this->pOutgoing = init->out;
  this->pError = init->err;
  this->lengtUserCmd = init->length_user_command;
  this->ptrUser = init->all_user_command;
  this->pos_pop =0;
  this->pos_push=0;
  this->buffer = (ptrBuffer)&sgBuffer[currentSize];
  currentSize += sizeof(tuSharedBufferCommand);
};

template <typename T>
unsigned char CommandHandler<T>::available()
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
  STAGE = STAGE_COLLECT_DATA;
  do
  {
    switch (STAGE)
    {
    case STAGE_COLLECT_DATA:
    {
      if ((this->obj->*pIncomming)() != 0x01)
      {
        STAGE = STAGE_END;
        break;
      } 
      STAGE = STAGE_SEARCH;
    }
    case STAGE_SEARCH:
    {
      ptrCurrentUser = &this->ptrUser[i];
      const char *s = ptrCurrentUser->getCommand();
      if (this->buffer->in.header.byte.ins != s[0])
      {
        if (this->lengtUserCmd < i)
        {
          STAGE = STAGE_ERROR;
        }
        else
        {
          i++;
        }
        break;
      }
      else
      {
        STAGE = STAGE_EXE;
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
        this->buffer->out.sw.WORD = ptrCurrentUser->exe(this);
        STAGE = STAGE_RESP;
      }
    }
    case STAGE_RESP:
    {
      if ((this->obj->*pOutgoing)() != 0x01)
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