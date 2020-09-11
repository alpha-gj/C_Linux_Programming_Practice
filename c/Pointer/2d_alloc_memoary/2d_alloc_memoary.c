#include<iostream> 
#include<string.h> 
#include<stdio.h> 
#include<stdlib.h> 
using namespace std; 
typedef unsigned char uint8;
typedef bool boolean;

#define MAX_CMD_SIZE 20 
#define MAX_LOCK_LEVEL_FLAG_SIZE 1
#define FALSE 0
#define TRUE  1

boolean atGetHexaVal(
	char c,
	uint8 * value)
{
  boolean result = TRUE;

  if ((c >= '0') && (c <= '9'))
  {
	*value = c - '0';
  }
  else if ((c >= 'a') && (c <= 'f'))
  {
	*value = c - 'a' + 10;
  }
  else if ((c >= 'A') && (c <= 'F'))
  {
	*value = c - 'A' + 10;
  }
  else
  {
	result = FALSE;
  }
  return (result);
}


int swi_parse_allowed_list_efs(char **cmd, uint8 **cmd_lock_level, uint8 num_cmds, char *buffer)
{
  uint8 index;
  char temp_buffer[MAX_CMD_SIZE] = {'\0'};
  uint8 temp_lock_level_buffer = 0;
  char *c_ptr = buffer;

  printf("buffer:%s\n", buffer);

  for (int index = 0; index < num_cmds; index++)
  {

	for ( int i = 0; *c_ptr != ':' && *c_ptr != ',' && i < MAX_CMD_SIZE - 1; i++)
	{
	  temp_buffer[i] = *c_ptr;
	  c_ptr++;
	}
    printf("temp_buffer:%s\n", temp_buffer);
	strncpy(*(cmd+index), temp_buffer, sizeof(char) * MAX_CMD_SIZE);
    printf("*(cmd+index):%d:%s\n", index, *(cmd+index));
	memset(temp_buffer, 0, sizeof(temp_buffer));

    /* STEP2: Parse FLAG as lock level */
    boolean is_valid_flag = FALSE;
    do {

      /* Check the char is ':' or not
         CMD1:FLAG1,CMD2:FLAG2,CMD3,FLAG3,..
             ^                              */
      if (*c_ptr != ':')
      {
		printf("No find the ':' char for FLAG");
        break;
      }
      c_ptr++;

      /* Check the length of FLAG is valid or not
         CMD1:FLAG1,CMD2:FLAG2,CMD3,FLAG3,..
              |<->|                              */
      char *len_check_ptr = c_ptr;
      uint8 i = 0;
      for (i = 0; *len_check_ptr != ',' && *len_check_ptr != NULL; i++)
      {
        len_check_ptr++;
      }
      if (i != MAX_LOCK_LEVEL_FLAG_SIZE)
      {
		printf("Check the invalid len:%d, The length of FLAG is  invalid", i);
        break;
      }

      /* Check the char is HEX or not
         CMD1:FLAG1,CMD2:FLAG2,CMD3,FLAG3,..
              -----
                ^                           */
      char *hex_check_ptr = c_ptr;
      if (isxdigit(*hex_check_ptr) == 0)
      {
		printf("The one char of FLAG is not HEX");
        break;
      }

      is_valid_flag = TRUE;

    } while (FALSE);

    /* STEP4: Assign FLAG to lock level list */
    if (is_valid_flag && atGetHexaVal(*c_ptr, &temp_lock_level_buffer))
    {
	  printf("The FLAG of %s is %X", *(cmd+index), temp_lock_level_buffer);
    }
    else
    {
      temp_lock_level_buffer = 15; /* For Hex is F */
	  printf("The FLAG of %s is invalid. Set FLAG to F as        default", *(cmd+index));
    }
    **(cmd_lock_level+index) = temp_lock_level_buffer;

	if (index < num_cmds -1)
	{
	  printf("Find next CMD\n");
	  /* NOTE: We can't make sure how long we can find the next CMD from txt file */
	  while (*c_ptr != ',')
	  {
		c_ptr++;
	  }
	  c_ptr++;
	}
	else
	{
	  printf("CMD is over\n");
	}
  }

  printf("==================\n");
  for (int index = 0; index < num_cmds; index++)
  {
	printf("*(cmd+index):%d:%s\n", index, *(cmd+index));
	printf("**(cmd_lock_level+index):%d:%X\n", index, **(cmd_lock_level+index));
  }
}

// Driver program to test above function 
int main() 
{ 
  int num_cmds = 6;
  char **cmd_list;
  unsigned char **cmd_lock_level_list;

  cmd_list = (char **)malloc(sizeof(char*) * num_cmds);
  cmd_lock_level_list = (unsigned char **)malloc(sizeof(unsigned char*) * num_cmds);

  for (int index = 0; index < num_cmds; index++) {
    *(cmd_list + index) = (char *)malloc(sizeof(char) * MAX_CMD_SIZE);
    *(cmd_lock_level_list + index) = (unsigned char *)malloc(sizeof(unsigned char) * MAX_LOCK_LEVEL_FLAG_SIZE);
  }

  char *buffer = "!GJ_level_lock_0,!GJ_level_lock_4:,!GJ_level_lock_8:11111,!GJ_level_lock_1:1,!GJ_level_lock_5:AH,!MCUINFO:H";

  swi_parse_allowed_list_efs(cmd_list, cmd_lock_level_list, num_cmds, buffer);
} 



