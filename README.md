CommandHandler
==============
Handle Serial Command for AVR. Save the list command in FLASH (PROGMEM).

Format
======
| < STD_CMD >+ | < COMMAND > | =< PARAMETER > |
|:--------:|:-------:|:----------:|
|    M     |    M    |      O     |

M:Mandatory
O:Optional

Define 
======
### #define STD_CMD  "ANU"

This define used for header command for identity. like AT command with 'AT' Header for send data behind.

##### Example with "ANU"
Serial : ANU+CMD1

### #define CHAR_CMD		5

This define for limitation of C char(end char with '\0') < COMMAND > . 

##### Example with 5
Serial : ANU+CMD1   //OK
Serial : ANU+COMD1  //ERROR becouse no place of'\0'
Serial : ANU+COMAND //ERROR

### #define CMD_BUFFER		127

This define for length buffer.

Function
========
``` C++
CommandHandler(byte(*funcIn)(void), void (*error)(void),const TSCmdTbl *cmdTbl,byte len);
```
Constructor.

| Parameter | Description |
| --- | --- |
| byte(*funcIn)(void) | pointer to function for handle incomming data. |
| void (*error)(void) | pointer to function for handle error format for incomming data. |
| const TSCmdTbl *cmdTbl | pointer to TSCmdTbl that have list of command and function to run. |
| byte len | length of command in TSCmdTbl that you make. |

____

``` C++
void run();
```

Run All Command.

____

``` C++
byte available();
```
To know how  much char of < PARAMETER > that save in buffer

| Retrun | Description |
| --- | --- |
| byte x | length of char in buffer for < PARAMETER >. |

____

``` C++
void push(char x);
```
save a char to buffer.

| Parameter | Description |
| --- | --- |
| char x | char that save in buffer. |

____

``` C++
char pop();
```
pop of char in < PARAMETER >.

| Retrun | Description |
| --- | --- |
| char x | pop the char for < PARAMETER > in buffer. |

____
