# CommandHandler API

CommandHanddler is a API that handle command management. User can create command with/without parameter, doing something for it and then send the respond.

## Format 

Format is simple. First 5 Byte contain the header

| **First_Byte** | **CMD_0** | **CMD_1** | **CMD_2** | **Lengt_Paramater** | *Data_Parameter* |
| -------------- | --------- | --------- | --------- | ------------------- | ---------------- |
| 1 Byte         | 1 Byte    | 1 Byte    | 1 Byte    | 1 Byte              | n Byte           |