/*************************************************************************//**
 *****************************************************************************
 * @file   open.c
 * @brief  open()
 * @author Forrest Y. Yu
 * @date   2008
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

/*****************************************************************************
 *                                touch
 *****************************************************************************/
/**
 * open/create a file.
 * 
 * @param pathname  The full path of the file to be opened/created.
 * @param flags     O_CREAT, O_RDWR, etc.
 * 
 * @return File descriptor if successful, otherwise -1.
 *****************************************************************************/
PUBLIC int touch(char* filename)
{
	MESSAGE msg;
	msg.type   = TOUCH;
	msg.FILENAME	= (void*)filename;
	msg.NAME_LEN	= strlen(filename);
	send_recv(BOTH, TASK_FS, &msg);

	return msg.RETVAL;
}
