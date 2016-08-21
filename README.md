CommandHandler
==============
Handle Serial Command for AVR. Save the list command in FLASH (PROGMEM).

Format
======
|===M===| |===M===||====O=====|
|STD_CMD|+|COMMAND|=|PARAMETER|
M:Mandatory
O:Optional

# Define 
## #define STD_CMD  "ANU"

This define used for header command for identity. like AT command with 'AT' Header for send data behind.

### Example with "ANU"
ANU+<COMMAND>=<PARAMETER>
