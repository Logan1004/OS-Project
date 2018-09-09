/*************************************************************************//**
 *****************************************************************************
 * @file   unlink.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Tue Jun  3 16:12:05 2008
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
 *                                unlink
 *****************************************************************************/
/**
 * Delete a file.
 * 
 * @param pathname  The full path of the file to delete.
 * 
 * @return Zero if successful, otherwise -1.
 *****************************************************************************/
PUBLIC int rm(const char* filename)
{
	MESSAGE msg;
	msg.type   = RM;

	msg.FILENAME	= (void*)filename;
	msg.NAME_LEN	= strlen(filename);

	send_recv(BOTH, TASK_FS, &msg);

	return msg.RETVAL;
}
