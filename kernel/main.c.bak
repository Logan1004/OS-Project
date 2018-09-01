
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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

// my code here
#define MAX_ARRAY_NUM 1000 //文件树最大数目

//


int tot = 0;
#define MAX 100
int C[MAX] = {};

void gameShow(int n){
	int i;
	int j;
	for(i=0;i < n;i++){
		
		for(j=0;j < n;j++){
			if(j == C[i]){
				printf("X");
			}
			else{
				printf("0");
			}
		}
		printf("\n");
	}
}

void search(int cur, int n){ 
	int i, j;
	if (cur == n){
		tot++;
		printf("case:%d\n",tot);
		gameShow(n);
		printf("%d\n",n);
		
	}
	else {
		for (i = 0; i < n; i++){
			int ok = 1;
			C[cur] = i;
			for (j = 0; j < cur; j++){
				if ((C[cur] == C[j]) || (C[cur] - cur == C[j] - j) || (C[cur] + cur == C[j] + j)){
					ok = 0;
					break;
				}
			}
			if (ok){
				search(cur + 1, n);
			}

		}
	}
}
int countError;
int step(int choice, int n, int cur,int fd_stdin,int fd_stdout){
	char rdbuf[128];
	int i = 0, j = 0;
	countError = 0;
	C[cur] = choice;
	int ok = 1;
	for (j = 0; j < cur; j++){
		if ((C[cur] == C[j]) || (C[cur] - cur == C[j] - j) || (C[cur] + cur == C[j] + j))	{
			ok = 0;
			C[cur] = 0;
			printf("select again!");
			countError++;
			if (countError == 2) return -1;
			int r = read(fd_stdin, rdbuf, 70);
			rdbuf[r] = 0;
			atoi(rdbuf, &choice);
			step(choice - 1, n, cur,fd_stdin,fd_stdout);
			return 1;

		}
	}
	if (ok){
		for (i = 0; i <= cur; i++){
			for (j = 0; j < n; j++){
				if (j == C[i])
				{
					printf("x");
				}
				else
				{
					printf("0");
				}
			}
			printf("\n");
		}


	}
	printf("\n");
}
void game(fd_stdin, fd_stdout){
	char rdbuf[128];
	printf("input number(1~100):");
	int n = 0;
	int r = read(fd_stdin, rdbuf, 70);
	rdbuf[r] = 0;
	atoi(rdbuf, &n);



	printf("Play!(step by step) or look at answer(y/n)?\n");



	r = read(fd_stdin, rdbuf, 70);
	rdbuf[r] = 0;
	if (strcmp(rdbuf, "y") == 0){
		//C[MAX] = {};
		int ii;
		for (ii = 0; ii < n; ii++){
			printf("input your choice of line%d\n", ii + 1);
			int choice;
	
			r = read(fd_stdin, rdbuf, 70);
			rdbuf[r] = 0;
			atoi(rdbuf, &choice);
			if (choice > n || choice < 1){
				printf("Invalid and Exit!\n");
				break;
			}
			if (step(choice - 1, n, ii,fd_stdin,fd_stdout) != -1) continue;
			printf("You lose!");
			break;


		}
	}
	else if (strcmp(rdbuf, "answer") == 0){
		search(0, n);
		printf("hard mode,please input the solutions in a time!\n");
	}
	printf("Do you want to look at answers?");

	if (tot == 0) printf("invaild！\n");
	
}

//gobal vary
int treeCount = 0;
int treeCurrSize = 0; //largest number id of file
int currFatherDir = -1;

int currPathDepth = 0;
char path[50][12]={0};

struct FTreeArray
{
	int currDir;
	int fatherDir;
	int isDir;  //0 no,1 yes
	char fileName[12];  //dir is "dir"

}fTreeArray[MAX_ARRAY_NUM];

// struct ListNode
// {
// 	char fileName[12];
// 	struct ListNode* next;
// }ListNode[MAX_NODE];
void initFTreeArray()
{
	int i = 0;
	for (; i < MAX_ARRAY_NUM; ++i)
	{
		fTreeArray[i].currDir = -1;
		fTreeArray[i].fatherDir = -1;
		fTreeArray[i].isDir = -1;
	}
}

void creatFTreeArray(char fTreeInfo[])
{
	char currDir[4]={0};
	char fatherDir[4]={0};
	char isDir = 0;
	char fileName[12] = {0};

	// int index = 0;

	char temp[12]={0};

	initFTreeArray();
	// int i = 0;
	// for (; i < 20 && fTreeInfo != '\0'; ++i)
	// {
	// 	printf("%c\n", fTreeInfo[i]);
	// 	if(fTreeInfo[i] == ' ')
	// 	{
	// 		printf("SPACE!!!!\n");
	// 	}
	// }
	int count = 0,i=0,j=0;
	i = 0;
	j = 0;
	for(;fTreeInfo[j] == ' ';++j)
	{
	}
	for(;fTreeInfo[j] != ';';)
	{
		count = 0;
		i = 0;
		while(count < 4)
		{
			temp[i] = fTreeInfo[j];
			i++;
			j++;

			if(fTreeInfo[j] == ' ')
			{
				j++;  // 不要空格
				if(count == 0)
				{
					// printf("temp:%s\n", temp);
					atoi(temp,&(fTreeArray[treeCurrSize].currDir));
					// printf("currDir:%d\n",fTreeArray[treeCurrSize].currDir);
				}else if(count == 1)
				{
					// printf("temp:%s\n", temp);
					atoi(temp,&(fTreeArray[treeCurrSize].fatherDir));
					// printf("fatherDir:%d\n",fTreeArray[treeCurrSize].fatherDir);
			
				}else if(count == 2)
				{
					// printf("temp:%s\n", temp);
					atoi(temp,&(fTreeArray[treeCurrSize].isDir));
					// printf("isDir:%d\n",fTreeArray[treeCurrSize].isDir);
				}else if(count == 3)
				{
					// printf("temp:%s\n", temp);
					strcpy(fTreeArray[treeCurrSize].fileName, temp);
					// printf("fileName:%s\n",fTreeArray[treeCurrSize].fileName);
				}
				
				//clear temp
				int k;
				for (k = 0; k < 12; ++k)
				{
					temp[k] = 0;
				}
				count++;
				i = 0;
			}
		}
		treeCurrSize++;
	}
	//zui da bian hao
	treeCount = treeCurrSize;
	treeCurrSize = fTreeArray[treeCurrSize-1].currDir + 1;
}

void getTreeInfo(char fTreeInfo[])
{
	// char* fTreeInfo;
	fTreeInfo[0]=0;
	char temp[12]={0};

	printf("\n");

	// strcat(fTreeInfo,temp);

	// printf("%s??\n", fTreeInfo);

	
	// itoaDec(temp,0);
	// printf("::%s:??:\n", temp);
	printf("treeCurrSize :%d, treeCount :%d\n", treeCurrSize,treeCount);

	int count = 0;
	int i = 0;
	for(;count < treeCount && i < MAX_ARRAY_NUM;++i)
	{
		// printf("%d\n",fTreeArray[i].currDir);
		// printf("%d\n",fTreeArray[i].fatherDir);
		// printf("%d\n",fTreeArray[i].isDir);

		//-1 means being deleted
		if(fTreeArray[i].currDir != -1)
		{
			itoaDec(temp,fTreeArray[i].currDir);
			strcat(fTreeInfo,temp);
			strcat(fTreeInfo," ");

			itoaDec(temp,fTreeArray[i].fatherDir);
			// printf("::%s::\n", temp);
			strcat(fTreeInfo,temp);
			strcat(fTreeInfo," ");


			if(fTreeArray[i].isDir == 1)
			{
				temp[0] = '1';
				temp[1] = 0;
			}else
			{
				temp[0] = '0';
				temp[1] = 0;
			}
			strcat(fTreeInfo,temp);
			strcat(fTreeInfo," ");

			strcat(fTreeInfo,fTreeArray[i].fileName);
			strcat(fTreeInfo," ");

			count++;
		}
		// printf("%s\n", fTreeInfo);
	}

	// printf("exit while\n");
	strcat(fTreeInfo,";");

	// // printf("fTreeInfo :%s\n", fTreeInfo);
	// // add end tag
	// int k = 0;
	// for(;fTreeInfo[k] != ';' && k < 20;++k)
	// {
	// 	printf("%d     %d\n", k,k);
	// 	printf("%c\n", fTreeInfo[k]);
	// }
	// printf("%c\n", fTreeInfo[k]);
	// fTreeInfo[20] = 0;

	// printf("%s\n", fTreeInfo);

	// return fTreeInfo;

	// return "notnot\0";
}

void showTree()
{
	int i = 0;
	for(;i < treeCurrSize;++i)
	{
		printf("currDir:%d\n",fTreeArray[i].currDir);
		printf("fatherDir:%d\n",fTreeArray[i].fatherDir);
		printf("isDir:%d\n",fTreeArray[i].isDir);
		printf("fileName:%s\n",fTreeArray[i].fileName);
		
	}
}

void saveTreeToDsik()
{
	//write TreeRecord
	int fd = -1;
	int buf[2014] = {0};

	char treeInfo[2048]={0};
	getTreeInfo(treeInfo);
	printf("%s\n", treeInfo);

	fd = open("TreeRecord", O_RDWR);
	if (fd == -1)
	{
		printf("Failed to open 'TreeRecord'!\n");
		return;
	}
	strcpy(buf,treeInfo);
	printf("buf :%s\n", buf);

	write(fd, buf, 1024);
	close(fd);
}

void readTreeFromDisk(char buf[])
{
	int fd = -1;
	// char buf[2048];
	int n;

	fd = open("TreeRecord", O_RDWR);
	if (fd == -1)
	{
		fd = open("TreeRecord", O_CREAT | O_RDWR);
		if (fd == -1)
		{
			printf("Failed to create file! Please check the fileaname!\n");
			return;
		}
		buf[0] = ';';
		buf[1] = 0;

		write(fd, buf, 2);
		printf("File created: (fd %d)\n",  fd);
		close(fd);

		fd = open("TreeRecord", O_RDWR);
	}
	
	n = read(fd, buf, 2048);
	
	// printf("%s\n", buf);
	close(fd);

}

void deleteDirRecord(char fileaname[])
{	
	int i = 0;
	for (; i < treeCurrSize; ++i)
	{
		if(strcmp(fTreeArray[i].fileName,fileaname) == 0
			&& fTreeArray[i].isDir == 1)
		{
			deleteDirRecusive(fTreeArray[i].currDir);
			fTreeArray[i].currDir = -1;
			return;
		}
	}
}
void deleteDirRecusive(int fatherDir)
{
	int i = 0;
	for (; i < treeCurrSize; ++i)
	{
		if(fatherDir == fTreeArray[i].fatherDir)
		{
			deleteDirRecusive(fTreeArray[i].currDir);
			//is a file?
			if(fTreeArray[i].isDir == 0)
			{
				int m=unlink(fTreeArray[i].fileName);
				if (m != 0)
				{
					printf("Failed to delete file! Please check the fileaname!\n");
				}
			}
			fTreeArray[i].currDir = -1;
		}
	}
}

void printPath()
{
	if(currPathDepth == 0)
	{
		printf("/");
		return;
	}
	int i = 0;
	for(; i < currPathDepth; ++i)
	{
		printf("/%s", path[i]);
	}
}
void printCurrFile()
{
	printf("filename   |   type   |   size\n");
	int i = 0;
	for (; i < treeCurrSize; ++i)
	{
		if(fTreeArray[i].currDir != -1
			&& fTreeArray[i].fatherDir == currFatherDir)
		{
			if(fTreeArray[i].isDir == 0)
			{
				printf("%s   |   File    |  --\n", fTreeArray[i].fileName);
			}
			else
			{
				printf("%s   |   Dir     |  --\n", fTreeArray[i].fileName);
			}
		}
	}
}

void openDir(char filename[])
{
	int i = 0;
	for (; i < treeCurrSize; ++i)
	{
		if(	fTreeArray[i].currDir != -1
			&& strcmp(fTreeArray[i].fileName,filename) == 0 
			&& fTreeArray[i].fatherDir == currFatherDir
			&& fTreeArray[i].isDir == 1)
		{
			strcpy(path[currPathDepth],fTreeArray[i].fileName);
			
			currFatherDir = fTreeArray[i].currDir;
			currPathDepth++;
			return;
		}
	}
	printf("fail to open %s\n", filename);
}

void backDir()
{
	int i = 0;
	for (; i < treeCurrSize; ++i)
	{
		if(fTreeArray[i].currDir == currFatherDir && fTreeArray[i].isDir == 1)
		{
			currFatherDir = fTreeArray[i].fatherDir;

			currPathDepth--;

			return;
		}
	}
}

void createDir(char filename[])
{
	if(filename[0] == 0)
	{
		printf("fail to create dir because of empty name\n");
		return;
	}
	int i = 0;
	for (; i < treeCurrSize; ++i)
	{
		if(strcmp(fTreeArray[i].fileName,filename) == 0
			&& fTreeArray[i].isDir == 1)
		{
			printf("fail to create dir because of the same name\n");
			return;
		}
	}

	fTreeArray[treeCurrSize].currDir = treeCurrSize;
	fTreeArray[treeCurrSize].fatherDir = currFatherDir;
	fTreeArray[treeCurrSize].isDir = 1;
	strcpy(fTreeArray[treeCurrSize].fileName ,filename);
	treeCurrSize++;
	treeCount++;
}

int isThereFile(char filename[])
{
	int i = 0;
	for (; i < treeCurrSize; ++i)
	{
		if(strcmp(fTreeArray[i].fileName,filename) == 0
			&& fTreeArray[i].fatherDir == currFatherDir)
		{
			return 1;
		}
	}
	return 0;
}
// end of my code

/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");

	struct task* p_task;
	struct proc* p_proc= proc_table;
	char* p_task_stack = task_stack + STACK_SIZE_TOTAL;
	u16   selector_ldt = SELECTOR_LDT_FIRST;
        u8    privilege;
        u8    rpl;
	int   eflags;
	int   i, j;
	int   prio;
	for (i = 0; i < NR_TASKS+NR_PROCS; i++) {
	        if (i < NR_TASKS) {     /* 任务 */
                        p_task    = task_table + i;
                        privilege = PRIVILEGE_TASK;
                        rpl       = RPL_TASK;
                        eflags    = 0x1202; /* IF=1, IOPL=1, bit 2 is always 1 */
			prio      = 15;
                }
                else {                  /* 用户进程 */
                        p_task    = user_proc_table + (i - NR_TASKS);
                        privilege = PRIVILEGE_USER;
                        rpl       = RPL_USER;
                        eflags    = 0x202; /* IF=1, bit 2 is always 1 */
			prio      = 5;
                }

		strcpy(p_proc->name, p_task->name);	/* name of the process */
		p_proc->pid = i;			/* pid */

		p_proc->ldt_sel = selector_ldt;

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
		       sizeof(struct descriptor));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
		       sizeof(struct descriptor));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;
		p_proc->regs.cs	= (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = eflags;

		/* p_proc->nr_tty		= 0; */

		p_proc->p_flags = 0;
		p_proc->p_msg = 0;
		p_proc->p_recvfrom = NO_TASK;
		p_proc->p_sendto = NO_TASK;
		p_proc->has_int_msg = 0;
		p_proc->q_sending = 0;
		p_proc->next_sending = 0;

		for (j = 0; j < NR_FILES; j++)
			p_proc->filp[j] = 0;

		p_proc->ticks = p_proc->priority = prio;

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

        /* proc_table[NR_TASKS + 0].nr_tty = 0; */
        /* proc_table[NR_TASKS + 1].nr_tty = 1; */
        /* proc_table[NR_TASKS + 2].nr_tty = 1; */

	k_reenter = 0;
	ticks = 0;

	p_proc_ready	= proc_table;

	init_clock();
        init_keyboard();

	restart();

	while(1){}
}


/*****************************************************************************
 *                                get_ticks
 *****************************************************************************/
PUBLIC int get_ticks()
{
	MESSAGE msg;
	reset_msg(&msg);
	msg.type = GET_TICKS;
	send_recv(BOTH, TASK_SYS, &msg);
	return msg.RETVAL;
}


/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()
{
	int fd;
	int i, n;

	char tty_name[] = "/dev_tty0";

	char rdbuf[128];


	int fd_stdin  = open(tty_name, O_RDWR);
	assert(fd_stdin  == 0);
	int fd_stdout = open(tty_name, O_RDWR);
	assert(fd_stdout == 1);

//	char filename[MAX_FILENAME_LEN+1] = "zsp01";
	const char bufw[80] = {0};
//	const int rd_bytes = 3;
//	char bufr[rd_bytes];

	clear();
	long int ii = 9999;

printf("                                                                           \n");while(ii-->0);ii=9999;
printf("    ##      D##                     ###           .##                      \n");
printf("    ##      D##                     W##           :##                      \n");
printf("    ##      G##                     ###           .##                      \n");
printf("    ##      G##                     ###           .##                      \n");
printf("    ##      G##                     ###           .##                      \n");
printf("    ##      G##                     ###           .##                      \n");
printf("    ##      D##      L###K          ###           .##           ##W# .     \n");
printf("    ##      G##    j#######i        ###           .##        .########     \n");
printf("    ###########    ##t  .W##        W##           .##        ###   W##D    \n");
printf("    ###########   ##D     ###       W##           .##       D##      ##    \n");
printf("    ##      G##  .##      ###       W##           .##       ##E      ##.   \n");
printf("    ##      G##  .###########       W##           .##       ##       ##E   \n");
printf("    ##      D##  :##                W##           .##       ##       ##E   \n");
printf("    ##      G##  .##                W##           .##       ##       ##G   \n");
printf("    ##      G##   ##.     f##       W##           .##       ##K      ##    \n");
printf("    ##      G##   ###.    ###       W##           .##       i#W     ;##    \n");
printf("    ##      G##    ###,  ###.       W##           .##        #### .###:    \n");
printf("    ##      D##     #######:        W##           :##         #######:     \n");
printf("                      K##           . .             .           i##  .     \n\n");
	startAnimate();
	
	clear();
	disp_color_str(" \n\n                   ==================================\n",5);
	disp_color_str("                                 Kernel on Orange's \n\n",3);
	disp_color_str("                                     Welcome !\n",4);
	disp_color_str("                        ====================================",5);

	

	while (1) {           
		printf("\n\n\n\n\n\n command:");


		int r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		//show();
        if (strcmp(rdbuf, "process") == 0)
        {
			ProcessManage();
        }
		else if (strcmp(rdbuf, "filemng") == 0)
		{
			printf("File Manager is already running on CONSOLE-1 ! \n");

			continue;

		}else if (strcmp(rdbuf, "help") == 0)
		{
			help();
		}
		else if (strcmp(rdbuf, "timer") == 0)
		{

			timer(fd_stdin, fd_stdout);
		}
		else if(strcmp(rdbuf, "game") == 0){
			clear();
			game(fd_stdin, fd_stdout);
		}
		
		else if (strcmp(rdbuf, "clear") == 0)
		{
			clear();
			printf("                        ==================================\n");
			
			printf("                                 Kernel on Orange's \n\n");
			printf("                                     Welcome !\n");
			printf("                        ==================================\n");
		}
		

		else
			printf("Command not found, please check!\n");
	}
}

/*======================================================================*
                               TestB
 *======================================================================*/
void initFSTree()
{
	char treeBuf[2048]={0};

	readTreeFromDisk(treeBuf);
	// printf("%s\n", treeBuf);

	// printf("treeCurrSize :%d, treeCount :%d\n", treeCurrSize,treeCount);
	creatFTreeArray(treeBuf);
}

void TestB()
{
	char tty_name[] = "/dev_tty1";

	int fd_stdin  = open(tty_name, O_RDWR);
	assert(fd_stdin  == 0);
	int fd_stdout = open(tty_name, O_RDWR);
	assert(fd_stdout == 1);

	char rdbuf[128];
	char cmd[8];
	char filename[120];
	char buf[1024];
	int m,n;

	initFSTree();


	printf("                        ==================================\n");
	printf("                                    File Manager           \n");
	printf("                                 Kernel on Orange's \n\n");
	printf("                        ==================================\n");
	while (1) {
		printPath();
		printf("$ :");
		int r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;

		if (strcmp(rdbuf, "i") == 0)
		{
			char treeInfo[2048] = {0};
			getTreeInfo(treeInfo);
			printf("%s\n", treeInfo);
		}
		else if(strcmp(rdbuf, "s") == 0)
		{
			showTree();
		}
		else if(strcmp(rdbuf, "p") == 0)
		{
			printPath();
			printf("\n");
		}
		else if (strcmp(rdbuf, "help") == 0)
		{
			printf("=============================================================================\n");
			printf("Command List     :\n");
			printf("1. create [filename]       : Create a new file \n");
			printf("2. read [filename]         : Read the file\n");
			printf("3. write [filename]        : Write at the end of the file\n");
			printf("4. delete [filename]       : Delete the file\n");
			printf("5. help                    : Display the help message\n");
			printf("==============================================================================\n");		
		}
		else if (strcmp(rdbuf, "dir") == 0)
		{
			printCurrFile();
			continue;
		}
		else
		{
			int fd;
			int i = 0;
			int j = 0;
			char temp = -1;
			while(rdbuf[i]!=' ' && rdbuf[i] != 0)
			{
				cmd[i] = rdbuf[i];
				i++;
			}
			cmd[i++] = 0;
			while(rdbuf[i] != 0)
			{
				filename[j] = rdbuf[i];
				i++;
				j++;
			}
			filename[j] = 0;

			if(strcmp(cmd, "mkdir") == 0)
			{
				createDir(filename);

				saveTreeToDsik();
				// //write TreeRecord
				// char treeInfo[2048]={0};
				// getTreeInfo(treeInfo);
				// printf("%s\n", treeInfo);

				// fd = open("TreeRecord", O_RDWR);
				// if (fd == -1)
				// {
				// 	printf("Failed to open 'TreeRecord'!\n");
				// 	continue ;
				// }
				// strcpy(buf,treeInfo);
				// printf("buf :%s\n", buf);

				// n = write(fd, buf, 1024);
				// close(fd);

			}
			else if(strcmp(cmd, "cd") == 0)
			{
				//open dir
				if(strcmp(filename, "..") == 0)
				{
					backDir();
				}
				else
				{
					openDir(filename);
				}
			}
			else if(strcmp(cmd,"rmdir") == 0)
			{
				//delete dir
				if(isThereFile(filename) == 0)
				{
					printf("fail to remove dir, not here!\n");
					continue;
				}
				deleteDirRecord(filename);

				saveTreeToDsik();
				// //write TreeRecord
				// char treeInfo[2048]={0};
				// getTreeInfo(treeInfo);
				// printf("%s\n", treeInfo);

				// fd = open("TreeRecord", O_RDWR);
				// if (fd == -1)
				// {
				// 	printf("Failed to open 'TreeRecord'!\n");
				// 	continue ;
				// }
				// strcpy(buf,treeInfo);
				// printf("buf :%s\n", buf);

				// n = write(fd, buf, 1024);
				// close(fd);
			}
			else if (strcmp(cmd, "create") == 0)
			{
				fd = open(filename, O_CREAT | O_RDWR);
				if (fd == -1)
				{
					printf("Failed to create file! Please check the fileaname!\n");
					continue ;
				}
				buf[0] = 0;
				write(fd, buf, 1);
				printf("File created: %s (fd %d)\n", filename, fd);
				close(fd);

				//add to tree currFatherDir
				fTreeArray[treeCurrSize].currDir = treeCurrSize;
				fTreeArray[treeCurrSize].fatherDir = currFatherDir;
				fTreeArray[treeCurrSize].isDir = 0;  //not a dir
				strcpy(fTreeArray[treeCurrSize].fileName, filename);
				treeCurrSize++;
				treeCount++;

				// showTree();

				// char* treeInfo;
				// treeInfo = getTreeInfo();
				// printf("%d\n", treeCurrSize);

				saveTreeToDsik();
				// //write TreeRecord
				// char treeInfo[2048]={0};
				// getTreeInfo(treeInfo);
				// printf("%s\n", treeInfo);

				// fd = open("TreeRecord", O_RDWR);
				// if (fd == -1)
				// {
				// 	printf("Failed to open 'TreeRecord'!\n");
				// 	continue ;
				// }
				// strcpy(buf,treeInfo);
				// printf("buf :%s\n", buf);

				// n = write(fd, buf, 1024);
				// close(fd);

			}
			else if (strcmp(cmd, "read") == 0)
			{
				if(isThereFile(filename) == 0)
				{
					printf("fail to read file, not here!\n");
					continue;
				}
				fd = open(filename, O_RDWR);
				if (fd == -1)
				{
					printf("Failed to open file! Please check the fileaname!\n");
					continue ;
				}
				
				n = read(fd, buf, 1024);
				
				printf("%s\n", buf);
				close(fd);

			}
			else if (strcmp(cmd, "write") == 0)
			{
				// if(isThereFile(filename) == 0)
				// {
				// 	printf("fail to write file, not here!\n");
				// 	continue;
				// }
				fd = open(filename, O_RDWR);
				if (fd == -1)
				{
					printf("Failed to open file! Please check the fileaname!\n");
					continue ;
				}

				m = read(fd_stdin, rdbuf,80);
				rdbuf[m] = 0;
				
				n = write(fd, rdbuf, m+1);
				close(fd);


			}
			else if (strcmp(cmd, "delete") == 0)
			{
				if(isThereFile(filename) == 0)
				{
					printf("fail to delete file, not here!\n");
					continue;
				}

				m=unlink(filename);
				if (m == 0)
				{
					printf("File deleted!\n");

					//delete file from tree records, not a dir
					int i = 0;
					for (; i < treeCurrSize; ++i)
					{
						if(strcmp(fTreeArray[i].fileName,filename) == 0)
						{
							fTreeArray[i].currDir = -1;
							continue;
						}
					}
					treeCount--;
					// showTree();

					saveTreeToDsik();
					// //write TreeRecord
					// char treeInfo[2048]={0};
					// getTreeInfo(treeInfo);
					// printf("%s\n", treeInfo);

					// fd = open("TreeRecord", O_RDWR);
					// if (fd == -1)
					// {
					// 	printf("Failed to open 'TreeRecord'!\n");
					// 	continue ;
					// }
					// strcpy(buf,treeInfo);
					// printf("buf :%s\n", buf);

					// n = write(fd, buf, 1024);
					// close(fd);

					continue;
				}
				else
				{
					printf("Failed to delete file! Please check the fileaname!\n");
					continue;
				}

			}
			else 
			{
				printf("Command not found, Please check!\n");
				continue;
			}

			
			
		}
		
			
	}

	assert(0); /* never arrive here */
}


void TestC()
{
	spin("TestC");
}

void timer(int fd_stdin,int fd_stdout)
{

	
		printf("How many seconds do you want to set?\n");
		char rdbuf[128];
		int tmp = 0;
		int r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		atoi(rdbuf,&tmp);

		int i = tmp;
printf("the time you set is %d\nNow begin....\n",tmp);
		for(;i >=0;i--){
			printf("%d left\n",i);
			milli_delay(10000);
		}
		printf("Time up!");
}

void startAnimate(){
	long int ii = 9999;
		printf("System is loading...");
	

	while(ii-->0);ii=2999999;
	printf("................");
	while(ii-->0);ii=2999999;
	printf("......");
	while(ii-->0);ii=999999;
	printf("..");
	while(ii-->0);ii=6999999;
	printf("..............................");
	while(ii-->0);ii=9999;
	printf("......");
	while(ii-->0);ii=999999;
	printf("..");
	while(ii-->0);ii=9999;
	printf("..");
	while(ii-->0);ii=999;
	printf(".......");
}

/*****************************************************************************
 *                                panic
 *****************************************************************************/
PUBLIC void panic(const char *fmt, ...)
{
	int i;
	char buf[256];

	/* 4 is the size of fmt in the stack */
	va_list arg = (va_list)((char*)&fmt + 4);

	i = vsprintf(buf, fmt, arg);

	printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

	/* should never arrive here */
	__asm__ __volatile__("ud2");
}

void clear()
{
	clear_screen(0,console_table[current_console].cursor);
	console_table[current_console].crtc_start = 0;
	console_table[current_console].cursor = 0;
	
}

//void show()
//{
//	printf("%d  %d  %d  %d",console_table[current_console].con_size, console_table[current_console].crtc_start, console_table[current_console].cursor, console_table[current_console].orig);
//}

void help()
{
	printf("=============================================================================\n");
	printf("Command List     :\n");
	printf("1. process       : A process manage,show you all process-info here\n");
	printf("2. filemng       : Run the file manager\n");
	printf("3. clear         : Clear the screen\n");
	printf("4. help          : Show this help message\n");
	printf("5. game   : Play a interesting game which help you improve math\n");
	printf("5. timer        : Run a easy timer Accurate to seconds\n");
	printf("==============================================================================\n");		
}

void ProcessManage()
{
	int i;
	printf("=============================================================================\n");
	printf("      myID      |    name       | spriority    | running?\n");
	//进程号，进程名，优先级，是否是系统进程，是否在运行
	printf("-----------------------------------------------------------------------------\n");
	for ( i = 0 ; i < NR_TASKS + NR_PROCS ; ++i )//逐个遍历
	{
//		if ( proc_table[i].priority == 0) continue;//系统资源跳过
		printf("        %d           %s            %d                yes\n", proc_table[i].pid, proc_table[i].name, proc_table[i].priority);
	}
	printf("=============================================================================\n");
}
