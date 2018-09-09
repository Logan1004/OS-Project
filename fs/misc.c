/*************************************************************************//**
 *****************************************************************************
 * @file   misc.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   2008
 *****************************************************************************
 *****************************************************************************/

/* Orange'S FS */

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
#include "keyboard.h"
#include "proto.h"
#include "hd.h"
#include "fs.h"

/*****************************************************************************
 *                                search_file
 *****************************************************************************/
/**
 * Search the file and return the inode_nr.
 *
 * @param[in] path The full path of the file to search.
 * @return         Ptr to the i-node of the file if successful, otherwise zero.
 * 
 * @see open()
 * @see do_open()
 *****************************************************************************/
PUBLIC int search_file(char * path)
{
	int i, j;

	char filename[MAX_PATH];
	memset(filename, 0, MAX_FILENAME_LEN);
	struct inode * dir_inode;
	if (strip_path(filename, path, &dir_inode) != 0)
		return 0;

	if (filename[0] == 0)	/* path: "/" */
		return dir_inode->i_num;

	/**
	 * Search the dir for the file.
	 */
	int dir_blk0_nr = dir_inode->i_start_sect;
	int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
	int nr_dir_entries =
	  dir_inode->i_size / DIR_ENTRY_SIZE; /**
					       * including unused slots
					       * (the file has been deleted
					       * but the slot is still there)
					       */
	int m = 0;
	struct dir_entry * pde;
	for (i = 0; i < nr_dir_blks; i++) {
		RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);
		pde = (struct dir_entry *)fsbuf;
		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
			if (memcmp(filename, pde->name, MAX_FILENAME_LEN) == 0)
				return pde->inode_nr;
			if (++m > nr_dir_entries)
				break;
		}
		if (m > nr_dir_entries) /* all entries have been iterated */
			break;
	}

	/* file not found */
	return 0;
}

/*****************************************************************************
 *                                strip_path
 *****************************************************************************/
/**
 * Get the basename from the fullpath.
 *
 * In Orange'S FS v1.0, all files are stored in the root directory.
 * There is no sub-folder thing.
 *
 * This routine should be called at the very beginning of file operations
 * such as open(), read() and write(). It accepts the full path and returns
 * two things: the basename and a ptr of the root dir's i-node.
 *
 * e.g. After stip_path(filename, "/blah", ppinode) finishes, we get:
 *      - filename: "blah"
 *      - *ppinode: root_inode
 *      - ret val:  0 (successful)
 *
 * Currently an acceptable pathname should begin with at most one `/'
 * preceding a filename.
 *
 * Filenames may contain any character except '/' and '\\0'.
 *
 * @param[out] filename The string for the result.
 * @param[in]  pathname The full pathname.
 * @param[out] ppinode  The ptr of the dir's inode will be stored here.
 * 
 * @return Zero if success, otherwise the pathname is not valid.
 *****************************************************************************/
PUBLIC int strip_path(char * filename, const char * pathname,
		      struct inode** ppinode)
{
	const char * s = pathname;
	char * t = filename;

	if (s == 0)
		return -1;

	if (*s == '/')
		s++;

	while (*s) {		/* check each character */
		if (*s == '/')
			return -1;
		*t++ = *s++;
		/* if filename is too long, just truncate it */
		if (t - filename >= MAX_FILENAME_LEN)
			break;
	}
	*t = 0;

	*ppinode = root_inode;

	return 0;
}


/*****************************************************************************
 *                                search_file_in_dir
 *****************************************************************************/
/**
 * Search the file and return the inode_nr.
 *
 * @param[in] path The full path of the file to search.
 * @return         Ptr to the i-node of the file if successful, otherwise zero.
 * 
 * @see open()
 * @see do_open()
 *****************************************************************************/
PUBLIC int search_file_in_dir(char* filename, struct inode* dir_inode)
{
	/**
	 * Search the dir for the file.
	 */
	int dir_blk0_nr = dir_inode->i_start_sect;
	int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
	int nr_dir_entries =
	  dir_inode->i_size / DIR_ENTRY_SIZE; /**
					       * including unused slots
					       * (the file has been deleted
					       * but the slot is still there)
					       */
	//printl("%d\n", nr_dir_entries);
	int m = 0; int i, j;
	struct dir_entry * pde;
	for (i = 0; i < nr_dir_blks; i++) {
		RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);
		pde = (struct dir_entry *)fsbuf;
		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
			if (strcmp(filename, pde->name) == 0)
				return pde->inode_nr;
			if (++m > nr_dir_entries)
				break;
		}
		if (m > nr_dir_entries) /* all entries have been iterated */
			break;
	}

	/* file not found */
	return 0;
}


/*****************************************************************************
 *                                search_file_by_path
 *****************************************************************************/
/**
 * Search the file and return the inode_nr.
 *
 * @param[in] path The full path of the file to search.
 * @return         Ptr to the i-node of the file if successful, otherwise zero.
 * 
 * @see open()
 * @see do_open()
 *****************************************************************************/
PUBLIC int search_file_by_path(char* path)
{
	/**
	 * Search the dir for the file.
	 */
	struct inode* tem_inode=root_inode;
	char filename[MAX_PATH];
	memset(filename, 0, MAX_FILENAME_LEN);
	char* lo=path;
	char* hi=lo;
	if(*lo=='/'){
		lo++; hi++;
	}
	else{
		return 0;
	}
	if(*hi==0){
		return root_inode->i_num;
	}
	//printl("============================\n");
	while(*hi){
		if(*hi=='/'){
			memcpy(filename, lo, hi-lo); filename[hi-lo]=0;
			//printl("%s\n", filename);
			hi++; lo=hi;
			int inode_nr=search_file_in_dir(filename, tem_inode);
			if(inode_nr==0)
				return 0;
			tem_inode=get_inode(root_inode->i_dev, inode_nr);
			if(tem_inode->i_mode!=I_DIRECTORY)
				return 0;
		}
		else
			hi++;
	}
	if(hi-lo==0){   //path end with'/'
		return tem_inode->i_num;
	}
	memcpy(filename, lo, hi-lo);
	filename[hi-lo]=0;
	//printl("%s\n", filename);
	int inode_nr=search_file_in_dir(filename, tem_inode);
	//printl("6666666666666\n");
	//printl("%d\n", inode_nr);
	//printl("6666666666666\n");
	//printl("============================\n");
	/* file not found */
	return inode_nr;
}


/*****************************************************************************
 *                                cd
 *****************************************************************************/
/**
 * Generate a new i-node and write it to disk.
 * 
 * @param dev  Home device of the i-node.
 * @param inode_nr  I-node nr.
 * @param start_sect  Start sector of the file pointed by the new i-node.
 * 
 * @return  Ptr of the new i-node.
 *****************************************************************************/
PUBLIC int do_cd()
{
    char pathname[MAX_PATH];
    /* get parameters from the message */
    int name_len = fs_msg.NAME_LEN; /* length of filename */
    int src = fs_msg.source;    /* caller proc nr. */
    assert(name_len < MAX_PATH);

    phys_copy((void*)va2la(TASK_FS, pathname),
          (void*)va2la(src, fs_msg.PATHNAME),
          name_len);
    pathname[name_len] = 0;
    int inode_nr=search_file_by_path(pathname);
    struct inode* tar_inode=get_inode(root_inode->i_dev, inode_nr);
    if(tar_inode->i_mode!=I_DIRECTORY){
	//printl("target path is not a dir\n");
	//return -1;
    }
    cur_inode=tar_inode;
    //printl("**********************\n");
    //printl("%s\n", pathname);
    //printl("%d\n", inode_nr);
    //printl("**********************\n");
    return 0;
}

