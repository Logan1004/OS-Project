/*************************************************************************//**
 *****************************************************************************
 * @file   fs/open.c
 * The file contains:
 *   - do_open()
 *   - do_close()
 *   - do_lseek()
 *   - create_file()
 * @author Forrest Yu
 * @date   2007
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
#include "keyboard.h"
#include "proto.h"

PRIVATE struct inode * create_file(char * filename, struct inode* dir_inode);
PRIVATE struct inode * create_dir(char * filename, struct inode* dir_inode);
PRIVATE int alloc_imap_bit(int dev);
PRIVATE int alloc_smap_bit(int dev, int nr_sects_to_alloc);
PRIVATE struct inode * new_inode(int dev, int inode_nr, int start_sect, int type);
PRIVATE void new_dir_entry(struct inode * dir_inode, int inode_nr, char * filename);

/*****************************************************************************
 *                                do_open
 *****************************************************************************/
/**
 * Open a file and return the file descriptor.
 * 
 * @return File descriptor if successful, otherwise a negative error code.
 *****************************************************************************/
PUBLIC int do_open()
{
	int fd = -1;		/* return value */

	char filename[MAX_PATH];

	/* get parameters from the message */
	int flags = fs_msg.FLAGS;	/* access mode */
	int name_len = fs_msg.NAME_LEN;	/* length of filename */
	int src = fs_msg.source;	/* caller proc nr. */
	assert(name_len < MAX_PATH);
	phys_copy((void*)va2la(TASK_FS, filename),
		  (void*)va2la(src, fs_msg.FILENAME),
		  name_len);
	filename[name_len] = 0;

	/* find a free slot in PROCESS::filp[] */
	int i;
	for (i = 0; i < NR_FILES; i++) {
		if (pcaller->filp[i] == 0) {
			fd = i;
			break;
		}
	}
	if ((fd < 0) || (fd >= NR_FILES))
		panic("filp[] is full (PID:%d)", proc2pid(pcaller));

	/* find a free slot in f_desc_table[] */
	for (i = 0; i < NR_FILE_DESC; i++)
		if (f_desc_table[i].fd_inode == 0)
			break;
	if (i >= NR_FILE_DESC)
		panic("f_desc_table[] is full (PID:%d)", proc2pid(pcaller));

	int inode_nr = search_file_in_dir(filename, cur_inode);

	struct inode * pin = 0;
	if (flags & O_CREAT) {
		if (inode_nr) {
			printl("file exists.\n");
			return -1;
		}
		else {
			pin = create_file(filename, cur_inode);
		}
	}
	else {
		assert(flags & O_RDWR);

		//char filename[MAX_PATH];
		//struct inode * dir_inode;
		//if (strip_path(filename, pathname, &dir_inode) != 0)
			//return -1;
		pin = get_inode(root_inode->i_dev, inode_nr);
		pin->i_cnt++;
	}

	if (pin) {
		/* connects proc with file_descriptor */
		pcaller->filp[fd] = &f_desc_table[i];

		/* connects file_descriptor with inode */
		f_desc_table[i].fd_inode = pin;

		f_desc_table[i].fd_mode = flags;
		/* f_desc_table[i].fd_cnt = 1; */
		f_desc_table[i].fd_pos = 0;

		int imode = pin->i_mode & I_TYPE_MASK;

		if (imode == I_CHAR_SPECIAL) {
			MESSAGE driver_msg;
			driver_msg.type = DEV_OPEN;
			int dev = pin->i_start_sect;
			driver_msg.DEVICE = MINOR(dev);
			assert(MAJOR(dev) == 4);
			assert(dd_map[MAJOR(dev)].driver_nr != INVALID_DRIVER);
			send_recv(BOTH,
				  dd_map[MAJOR(dev)].driver_nr,
				  &driver_msg);
		}
		else if (imode == I_DIRECTORY) {
			//assert(pin->i_num == ROOT_INODE);
		}
		else {
			assert(pin->i_mode == I_REGULAR);
		}
	}
	else {
		return -1;
	}

	return fd;
}

/*****************************************************************************
 *                                create_file
 *****************************************************************************/
/**
 * Create a file and return it's inode ptr.
 *
 * @param[in] path   The full path of the new file
 * @param[in] flags  Attribiutes of the new file
 *
 * @return           Ptr to i-node of the new file if successful, otherwise 0.
 * 
 * @see open()
 * @see do_open()
 *
 * @todo return values of routines called, return values of self.
 *****************************************************************************/
PRIVATE struct inode * create_file(char* filename, struct inode* dir_inode)
{	
	//struct inode* dir_inode=get_inode(root_inode->i_dev, 6);
	//struct inode * dir_inode=cur_inode;
	int inode_nr = alloc_imap_bit(dir_inode->i_dev);
	//printl("%d\n", inode_nr);
	int free_sect_nr = alloc_smap_bit(dir_inode->i_dev,
					  NR_DEFAULT_FILE_SECTS);
	//printl("%d\n", free_sect_nr);
	struct inode *newino = new_inode(dir_inode->i_dev, inode_nr,
					 free_sect_nr, I_REGULAR);
	//printl("%d\n", newino->i_num);
	new_dir_entry(dir_inode, newino->i_num, filename);
	//printl("come here \n");

	return newino;
}

PRIVATE struct inode * create_dir(char* filename, struct inode* dir_inode)
{
	//struct inode * dir_inode=cur_inode;
	int inode_nr = alloc_imap_bit(dir_inode->i_dev);
	int free_sect_nr = alloc_smap_bit(dir_inode->i_dev,
					  NR_DEFAULT_FILE_SECTS);
	struct inode *newino = new_inode(dir_inode->i_dev, inode_nr,
					 free_sect_nr, I_DIRECTORY);
	//newino->i_mode = I_DIRECTORY;
	new_dir_entry(dir_inode, newino->i_num, filename);
	return newino;
}


/*****************************************************************************
 *                                do_close
 *****************************************************************************/
/**
 * Handle the message CLOSE.
 * 
 * @return Zero if success.
 *****************************************************************************/
PUBLIC int do_close()
{
	int fd = fs_msg.FD;
	put_inode(pcaller->filp[fd]->fd_inode);
	pcaller->filp[fd]->fd_inode = 0;
	pcaller->filp[fd] = 0;

	return 0;
}

/*****************************************************************************
 *                                do_lseek
 *****************************************************************************/
/**
 * Handle the message LSEEK.
 * 
 * @return The new offset in bytes from the beginning of the file if successful,
 *         otherwise a negative number.
 *****************************************************************************/
PUBLIC int do_lseek()
{
	int fd = fs_msg.FD;
	int off = fs_msg.OFFSET;
	int whence = fs_msg.WHENCE;

	int pos = pcaller->filp[fd]->fd_pos;
	int f_size = pcaller->filp[fd]->fd_inode->i_size;

	switch (whence) {
	case SEEK_SET:
		pos = off;
		break;
	case SEEK_CUR:
		pos += off;
		break;
	case SEEK_END:
		pos = f_size + off;
		break;
	default:
		return -1;
		break;
	}
	if ((pos > f_size) || (pos < 0)) {
		return -1;
	}
	pcaller->filp[fd]->fd_pos = pos;
	return pos;
}

/*****************************************************************************
 *                                alloc_imap_bit
 *****************************************************************************/
/**
 * Allocate a bit in inode-map.
 * 
 * @param dev  In which device the inode-map is located.
 * 
 * @return  I-node nr.
 *****************************************************************************/
PRIVATE int alloc_imap_bit(int dev)
{
	int inode_nr = 0;
	int i, j, k;

	int imap_blk0_nr = 1 + 1; /* 1 boot sector & 1 super block */
	struct super_block * sb = get_super_block(dev);

	for (i = 0; i < sb->nr_imap_sects; i++) {
		RD_SECT(dev, imap_blk0_nr + i);

		for (j = 0; j < SECTOR_SIZE; j++) {
			/* skip `11111111' bytes */
			if (fsbuf[j] == 0xFF)
				continue;
			/* skip `1' bits */
			for (k = 0; ((fsbuf[j] >> k) & 1) != 0; k++) {}
			/* i: sector index; j: byte index; k: bit index */
			inode_nr = (i * SECTOR_SIZE + j) * 8 + k;
			fsbuf[j] |= (1 << k);
			/* write the bit to imap */
			WR_SECT(dev, imap_blk0_nr + i);
			break;
		}

		return inode_nr;
	}

	/* no free bit in imap */
	panic("inode-map is probably full.\n");

	return 0;
}

/*****************************************************************************
 *                                alloc_smap_bit
 *****************************************************************************/
/**
 * Allocate a bit in sector-map.
 * 
 * @param dev  In which device the sector-map is located.
 * @param nr_sects_to_alloc  How many sectors are allocated.
 * 
 * @return  The 1st sector nr allocated.
 *****************************************************************************/
PRIVATE int alloc_smap_bit(int dev, int nr_sects_to_alloc)
{
	/* int nr_sects_to_alloc = NR_DEFAULT_FILE_SECTS; */

	int i; /* sector index */
	int j; /* byte index */
	int k; /* bit index */

	struct super_block * sb = get_super_block(dev);

	int smap_blk0_nr = 1 + 1 + sb->nr_imap_sects;
	int free_sect_nr = 0;

	for (i = 0; i < sb->nr_smap_sects; i++) { /* smap_blk0_nr + i :
						     current sect nr. */
		RD_SECT(dev, smap_blk0_nr + i);

		/* byte offset in current sect */
		for (j = 0; j < SECTOR_SIZE && nr_sects_to_alloc > 0; j++) {
			k = 0;
			if (!free_sect_nr) {
				/* loop until a free bit is found */
				if (fsbuf[j] == 0xFF) continue;
				for (; ((fsbuf[j] >> k) & 1) != 0; k++) {}
				free_sect_nr = (i * SECTOR_SIZE + j) * 8 +
					k - 1 + sb->n_1st_sect;
			}

			for (; k < 8; k++) { /* repeat till enough bits are set */
				assert(((fsbuf[j] >> k) & 1) == 0);
				fsbuf[j] |= (1 << k);
				if (--nr_sects_to_alloc == 0)
					break;
			}
		}

		if (free_sect_nr) /* free bit found, write the bits to smap */
			WR_SECT(dev, smap_blk0_nr + i);

		if (nr_sects_to_alloc == 0)
			break;
	}

	assert(nr_sects_to_alloc == 0);

	return free_sect_nr;
}

/*****************************************************************************
 *                                new_inode
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
PRIVATE struct inode * new_inode(int dev, int inode_nr, int start_sect, int type)
{
	struct inode * new_inode = get_inode(dev, inode_nr);

	new_inode->i_mode = type;
	new_inode->i_size = 0;
	new_inode->i_start_sect = start_sect;
	new_inode->i_nr_sects = NR_DEFAULT_FILE_SECTS;

	new_inode->i_dev = dev;
	//new_inode->i_cnt = 1;
	new_inode->i_num = inode_nr;

	/* write to the inode array */
	sync_inode(new_inode);

	return new_inode;
}

/*****************************************************************************
 *                                new_dir_entry
 *****************************************************************************/
/**
 * Write a new entry into the directory.
 * 
 * @param dir_inode  I-node of the directory.
 * @param inode_nr   I-node nr of the new file.
 * @param filename   Filename of the new file.
 *****************************************************************************/
PRIVATE void new_dir_entry(struct inode *dir_inode,int inode_nr,char *filename)
{
	/* write the dir_entry */
	int dir_blk0_nr = dir_inode->i_start_sect;
	int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE) / SECTOR_SIZE;
	int nr_dir_entries =
		dir_inode->i_size / DIR_ENTRY_SIZE; /**
						     * including unused slots
						     * (the file has been
						     * deleted but the slot
						     * is still there)
						     */
	int m = 0;
	struct dir_entry * pde;
	struct dir_entry * new_de = 0;

	int i, j;
	for (i = 0; i < nr_dir_blks; i++) {
		RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);

		pde = (struct dir_entry *)fsbuf;
		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
			if (++m > nr_dir_entries)
				break;

			if (pde->inode_nr == 0) { /* it's a free slot */
				new_de = pde;
				break;
			}
		}
		if (m > nr_dir_entries ||/* all entries have been iterated or */
		    new_de)              /* free slot is found */
			break;
	}
	if (!new_de) { /* reached the end of the dir */
		new_de = pde;
		//printl("6666666666666666666\n");
		dir_inode->i_size += DIR_ENTRY_SIZE;
	}
	new_de->inode_nr = inode_nr;
	strcpy(new_de->name, filename);

	/* write dir block -- ROOT dir block */
	WR_SECT(dir_inode->i_dev, dir_blk0_nr + i);

	/* update dir inode */
	sync_inode(dir_inode);
}

/*****************************************************************************
 *                                ls
 *****************************************************************************/
 /**
  * list files under cur dir.
  *****************************************************************************/
PUBLIC int do_ls()
{
	
	char filename[MAX_PATH];

	/* get parameters from the message */
	int name_len = fs_msg.NAME_LEN; /* length of filename */
	int src = fs_msg.source;    /* caller proc nr. */
	assert(name_len < MAX_PATH);

	phys_copy((void*)va2la(TASK_FS, filename),
		(void*)va2la(src, fs_msg.FILENAME),
		name_len);
	filename[name_len] = 0;
	//printl("ffffffffffffffffffffffffff\n");
	//printl("%s\n", filename);
	//printl("ffffffffffffffffffffffffff\n");

	//printl("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	//int n1=search_file_by_path("/blah");
	//printl("%d\n", n1);
	//int n2=search_file_by_path("/dir1/logan");
	//printl("%d\n", n2);
	//int n3=search_file_by_path("/dir1/");
	//printl("%d\n", n3);
	//int n4=search_file_by_path("/no");
	//printl("%d\n", n4);
	//int n5=search_file_by_path("no");
	//printl("%d\n", n5);
	//printl("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

	printl("files under cur dir!!!\n");
	printl("=========================================\n");
	struct inode* dir_inode = cur_inode;
	//struct inode* dir_inode=get_inode(root_inode->i_dev, 6);
        int dir_blk0_nr = dir_inode->i_start_sect;
        int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
        int nr_dir_entries = dir_inode->i_size / DIR_ENTRY_SIZE;
        int m = 0;
	int i, j;
	struct dir_entry * pde;
    	for (i = 0; i < nr_dir_blks; i++)
    	{
		//printl("enter for\n");
        	RD_SECT(root_inode->i_dev, dir_blk0_nr + i);
		//printl("rd finish\n");

        	pde = (struct dir_entry *)fsbuf;

        	for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++, pde++)
        	{
			if(pde->inode_nr>0)
            			printl("  %2d        %s\n", pde->inode_nr , pde->name);
            		if (++m >= nr_dir_entries){
                		//printl("\n");
                		break;
            		}
        	}
        	if (m > nr_dir_entries) //[> all entries have been iterated <]
            		break;
    	}
	printl("=========================================\n");
	return 0;
}

/*****************************************************************************
 *                                mkdir
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

PUBLIC int do_mkdir()
{
    char filename[MAX_PATH];

    /* get parameters from the message */
    int name_len = fs_msg.NAME_LEN; /* length of filename */
    int src = fs_msg.source;    /* caller proc nr. */
    assert(name_len < MAX_PATH);

    phys_copy((void*)va2la(TASK_FS, filename),
          (void*)va2la(src, fs_msg.FILENAME),
          name_len);
    filename[name_len] = 0;
    //printl("**********************\n");
    //printl("%s\n", filename);
    create_dir(filename, cur_inode);
    return 0;
	//if(!result){
		//printl("Create dir %s fail!\n", pathname);
		//return -1;
	//}
	
	//printl("Create dir %s success!\n", pathname);
	//return 0;
}

/*****************************************************************************
 *                                touch
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
PUBLIC int do_touch()
{
    char filename[MAX_PATH];

    /* get parameters from the message */
    int name_len = fs_msg.NAME_LEN; /* length of filename */
    int src = fs_msg.source;    /* caller proc nr. */
    assert(name_len < MAX_PATH);

    phys_copy((void*)va2la(TASK_FS, filename),
          (void*)va2la(src, fs_msg.FILENAME),
          name_len);
    filename[name_len] = 0;
    //printl("**********************\n");
    //printl("%s\n", filename);
    //printl("%d\n", cur_inode->i_num);
    //printl("**********************\n");
    create_file(filename, cur_inode);
    return 0;
	//if(!result){
		//printl("Create dir %s fail!\n", pathname);
		//return -1;
	//}
	
	//printl("Create dir %s success!\n", pathname);
	//return 0;
}


/*****************************************************************************
 *                                mv
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
PUBLIC int do_mv()
{
    char filename[MAX_PATH];
    char tarpath[MAX_PATH];
    /* get parameters from the message */
    int name_len = fs_msg.NAME_LEN; /* length of filename */
    int src = fs_msg.source;    /* caller proc nr. */
    assert(name_len < MAX_PATH);

    phys_copy((void*)va2la(TASK_FS, filename),
          (void*)va2la(src, fs_msg.FILENAME),
          name_len);
    filename[name_len] = 0;

    int path_len = fs_msg.PATH_LEN; /* length of filename */
    assert(path_len < MAX_PATH);

    phys_copy((void*)va2la(TASK_FS, tarpath),
          (void*)va2la(src, fs_msg.PATHNAME),
          path_len);
    tarpath[path_len] = 0;
    //printl("***************\n");
    //printl("%s\n", filename);
    //printl("%s\n", tarpath);
    //printl("***************\n");
    int inode_nr=search_file_in_dir(filename, cur_inode);
    int tar_inode_nr=search_file_by_path(tarpath);
    //printl("====================\n");
    //printl("%d\n", inode_nr);
    //printl("%d\n", tar_inode_nr);
    //printl("====================\n");
    struct inode* tar_inode=get_inode(root_inode->i_dev, tar_inode_nr);


    int dir_blk0_nr = cur_inode->i_start_sect;
    int nr_dir_blks = (cur_inode->i_size + SECTOR_SIZE) / SECTOR_SIZE;
    int nr_dir_entries =
	    cur_inode->i_size / DIR_ENTRY_SIZE; /* including unused slots
						     * (the file has been
						     * deleted but the slot
						     * is still there)
						     */
    int m = 0;
    struct dir_entry * pde = 0;
    int flg = 0;
    int dir_size = 0;
    int i=0;

    for (i = 0; i < nr_dir_blks; i++) {
	    RD_SECT(root_inode->i_dev, dir_blk0_nr + i);

	    pde = (struct dir_entry *)fsbuf;
	    int j;
	    for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
		    if (++m > nr_dir_entries)
			    break;

		    if (pde->inode_nr == inode_nr) {
			    /* pde->inode_nr = 0; */
			    memset(pde, 0, DIR_ENTRY_SIZE);
			    WR_SECT(root_inode->i_dev, dir_blk0_nr + i);
			    flg = 1;
			    break;
		    }

		    if (pde->inode_nr != INVALID_INODE)
			    dir_size = m*DIR_ENTRY_SIZE;
	    }

	    if (m > nr_dir_entries || /* all entries have been iterated OR */
		   flg) /* file is found */
		    break;
    }
    assert(flg);
    if (m == nr_dir_entries) { /* the file is the last one in the dir */
	    cur_inode->i_size = dir_size;
	    sync_inode(tar_inode);
    }

    new_dir_entry(tar_inode, inode_nr, filename);


    //printl("**********************\n");
    //printl("%s\n", filename);
    //create_file(filename, cur_inode);
    return 0;
	//if(!result){
		//printl("Create dir %s fail!\n", pathname);
		//return -1;
	//}
	
	//printl("Create dir %s success!\n", pathname);
	//return 0;
}







/*****************************************************************************
 *                                cp
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
PUBLIC int do_cp()
{
    char filename[MAX_PATH];
    char tarpath[MAX_PATH];
    /* get parameters from the message */
    int name_len = fs_msg.NAME_LEN; /* length of filename */
    int src = fs_msg.source;    /* caller proc nr. */
    assert(name_len < MAX_PATH);

    phys_copy((void*)va2la(TASK_FS, filename),
          (void*)va2la(src, fs_msg.FILENAME),
          name_len);
    filename[name_len] = 0;

    int path_len = fs_msg.PATH_LEN; /* length of filename */
    assert(path_len < MAX_PATH);

    phys_copy((void*)va2la(TASK_FS, tarpath),
          (void*)va2la(src, fs_msg.PATHNAME),
          path_len);
    tarpath[path_len] = 0;
    //printl("***************\n");
    //printl("%s\n", filename);
    //printl("%s\n", tarpath);
    //printl("***************\n");
    int inode_nr=search_file_in_dir(filename, cur_inode);
    int tar_inode_nr=search_file_by_path(tarpath);
    //printl("====================\n");
    //printl("%d\n", inode_nr);
    //printl("%d\n", tar_inode_nr);
    //printl("====================\n");

    struct inode* sr_inode=get_inode(root_inode->i_dev, inode_nr);
    struct inode* tar_inode=get_inode(root_inode->i_dev, tar_inode_nr);
    create_file(filename, tar_inode);
    int ds_inode_nr=search_file_in_dir(filename, tar_inode);
    struct inode* ds_inode=get_inode(root_inode->i_dev, ds_inode_nr);
    ds_inode->i_size=sr_inode->i_size;
    sync_inode(ds_inode);
    
    int sr_blk0_nr = sr_inode->i_start_sect;
    int nr_blks = (sr_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
    int ds_blk0_nr = ds_inode->i_start_sect;
    int i=0;
    for (i = 0; i < nr_blks; i++) {
	RD_SECT(sr_inode->i_dev, sr_blk0_nr + i);
        WR_SECT(ds_inode->i_dev, ds_blk0_nr + i);
    }
    //int nr_ds_blks = (ds_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
    return 0;
	//if(!result){
		//printl("Create dir %s fail!\n", pathname);
		//return -1;
	//}
	
	//printl("Create dir %s success!\n", pathname);
	//return 0;
}


