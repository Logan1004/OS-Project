
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

void clearArr(char *arr, int length)
{
    int i;
    for (i = 0; i < length; i++)
        arr[i] = 0;
}

void ProcessManage(){

}

void Clear()
{
    clear_screen(0,console_table[current_console].cursor);
    console_table[current_console].crtc_start = 0;
    console_table[current_console].cursor = 0;
}

void Calculator(int fd_stdin,int fd_stdout)
{

}

void Calendar(int fd_stdin,int fd_stdout){

}

void G3(int fd_stdin,int fd_stdout){

}

void G4(int fd_stdin,int fd_stdout){

}

//void show()
//{
//	printf("%d  %d  %d  %d",console_table[current_console].con_size, console_table[current_console].crtc_start, console_table[current_console].cursor, console_table[current_console].orig);
//}

void Help()
{
    printf("                                                                           \n");
    printf("       ***       ***      *******                                          \n");
    printf("       ***       ***      *********                Welcome!                \n");
    printf("         ***    ***       **     **               SHABBY OS                \n");
    printf("           *** ***        *******                                          \n");
    printf("             ***          *******                                          \n");
    printf("           *** ***        **    ***           by 1652761 Huang Yaoxian     \n");
    printf("          ***   ***       **      **          by 1652791 Wen Tingjie       \n");
    printf("        ***      ***      **********          by 1652792 Luo Jihao         \n");
    printf("        ***      ***      *********                                        \n");
    printf("                                                                           \n");
    printf("    ********************************************************************   \n");

    printf("                            Command List                                   \n");
    printf("\n");
    printf("         1. processmng                  Run the Process Manager            \n");
    printf("         2. filemng                     Run the File Manager               \n");
    printf("         3. clear                       Initial the screen                 \n");
    printf("         4. help                        Display Information                \n");
    printf("         5. G1:Calculator               Run an easy Calculator             \n");
    printf("         6. G2:Calendar                 Run a simple Calendar              \n");
    printf("         7. G3                          Uncertain                          \n");
    printf("         8. G4                          Uncertain                          \n");
    printf("\n");
    printf("    ********************************************************************   \n");
}

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

    Clear();
    long int ii = 9999;
    printf("                                                                           \n");while(ii-->0);ii=9999;

    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ###########\n");
    printf("    ###########\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("    ##      ###\n");
    printf("               \n\n");
    milli_delay(1000);
    Clear();
    printf("    ##      ###              \n");
    printf("    ##      ###              \n");
    printf("    ##      ###              \n");
    printf("    ##      ###              \n");
    printf("    ##      ###              \n");
    printf("    ##      ###              \n");
    printf("    ##      ###      #####   \n");
    printf("    ##      ###     ######## \n");
    printf("    ###########    ###   ####\n");
    printf("    ###########   ###     ###\n");
    printf("    ##      ###  ###      ###\n");
    printf("    ##      ###  ############\n");
    printf("    ##      ###  ###         \n");
    printf("    ##      ###  ###         \n");
    printf("    ##      ###   ###     ###\n");
    printf("    ##      ###   ####    ###\n");
    printf("    ##      ###    ####  ####\n");
    printf("    ##      ###     ######## \n");
    printf("                      ####   \n\n");
    milli_delay(1000);
    Clear();
    printf("    ##      ###                     ###\n");
    printf("    ##      ###                     ###\n");
    printf("    ##      ###                     ###\n");
    printf("    ##      ###                     ###\n");
    printf("    ##      ###                     ###\n");
    printf("    ##      ###                     ###\n");
    printf("    ##      ###      #####          ###\n");
    printf("    ##      ###     ########        ###\n");
    printf("    ###########    ###  #####       ###\n");
    printf("    ###########   ###     ###       ###\n");
    printf("    ##      ###  ###      ###       ###\n");
    printf("    ##      ###  ############       ###\n");
    printf("    ##      ###  ###                ###\n");
    printf("    ##      ###  ###                ###\n");
    printf("    ##      ###   ###     ###       ###\n");
    printf("    ##      ###   ####    ###       ###\n");
    printf("    ##      ###    ####  ####       ###\n");
    printf("    ##      ###     ########        ###\n");
    printf("                      ####          ###\n\n");
    milli_delay(1000);
    Clear();

    printf("    ##      ###                     ###           ###\n");
    printf("    ##      ###                     ###           ###\n");
    printf("    ##      ###                     ###           ###\n");
    printf("    ##      ###                     ###           ###\n");
    printf("    ##      ###                     ###           ###\n");
    printf("    ##      ###                     ###           ###\n");
    printf("    ##      ###      #####          ###           ###\n");
    printf("    ##      ###     ########        ###           ###\n");
    printf("    ###########    ###  #####       ###           ###\n");
    printf("    ###########   ###     ###       ###           ###\n");
    printf("    ##      ###  ###      ###       ###           ###\n");
    printf("    ##      ###  ############       ###           ###\n");
    printf("    ##      ###  ###                ###           ###\n");
    printf("    ##      ###  ###                ###           ###\n");
    printf("    ##      ###   ###     ###       ###           ###\n");
    printf("    ##      ###   ####    ###       ###           ###\n");
    printf("    ##      ###    ####  ####       ###           ###\n");
    printf("    ##      ###     ########        ###           ###\n");
    printf("                      ###           ###           ###\n\n");
    milli_delay(1000);
    Clear();
    printf("    ##      ###                     ###           ###                      \n");
    printf("    ##      ###                     ###           ###                      \n");
    printf("    ##      ###                     ###           ###                      \n");
    printf("    ##      ###                     ###           ###                      \n");
    printf("    ##      ###                     ###           ###                      \n");
    printf("    ##      ###                     ###           ###                      \n");
    printf("    ##      ###      #####          ###           ###           ####       \n");
    printf("    ##      ###     ########        ###           ###         ########     \n");
    printf("    ###########    ###  #####       ###           ###        ###    ###    \n");
    printf("    ###########   ###     ###       ###           ###       ###      ###   \n");
    printf("    ##      ###  ###      ###       ###           ###       ###      ###   \n");
    printf("    ##      ###  ############       ###           ###       ###      ###   \n");
    printf("    ##      ###  ###                ###           ###       ###      ###   \n");
    printf("    ##      ###  ###                ###           ###       ###      ###   \n");
    printf("    ##      ###   ###     ###       ###           ###       ###      ###   \n");
    printf("    ##      ###   ####    ###       ###           ###       ####    ####   \n");
    printf("    ##      ###    ####  ####       ###           ###        ####  ####    \n");
    printf("    ##      ###     ########        ###           ###         ########     \n");
    printf("                      ###           ###           ###           ####       \n\n");


    startAnimate();

    Clear();
    printf("                                                                           \n");
    printf("       ***       ***      *******                                          \n");
    printf("       ***       ***      *********                Welcome!                \n");
    printf("         ***    ***       **     **               SHABBY OS                \n");
    printf("           *** ***        *******                                          \n");
    printf("             ***          *******                                          \n");
    printf("           *** ***        **    ***           by 1652761 Huang Yaoxian     \n");
    printf("          ***   ***       **      **          by 1652791 Wen Tingjie       \n");
    printf("        ***      ***      **********          by 1652792 Luo Jihao         \n");
    printf("        ***      ***      *********                                        \n");
    printf("                                                                           \n");
    printf("                        Input [help] for more information                  \n");
    printf("                                                                           \n");
    printf("    ********************************************************************   \n");

    while (1) {
        printf("\n\n\n OS:");
        clearArr(rdbuf,128);
        //清空数据，初始化，存放新的数据
        int r = read(fd_stdin, rdbuf, 70);
        rdbuf[r] = 0;
        if (strcmp(rdbuf, "processmng") == 0){
            ProcessManage();
            //printf("processmng");
            continue;
        }
        else if (strcmp(rdbuf, "filemng") == 0)
        {
            printf("File Manager is already running on CONSOLE-1 ! \n");
            //printf("filemng");
            continue;
        }
        else if (strcmp(rdbuf, "clear") == 0){
            Clear();
            printf("                                                                           \n");
            printf("       ***       ***      *******                                          \n");
            printf("       ***       ***      *********                Welcome!                \n");
            printf("         ***    ***       **     **               SHABBY OS                \n");
            printf("           *** ***        *******                                          \n");
            printf("             ***          *******                                          \n");
            printf("           *** ***        **    ***           by 1652761 Huang Yaoxian     \n");
            printf("          ***   ***       **      **          by 1652791 Wen Tingjie       \n");
            printf("        ***      ***      **********          by 1652792 Luo Jihao         \n");
            printf("        ***      ***      *********                                        \n");
            printf("                                                                           \n");
            printf("                        Input [help] for more information                  \n");
            printf("                                                                           \n");
            printf("    ********************************************************************   \n");
            continue;
        }
        else if (strcmp(rdbuf, "help") == 0){
            Help();
            continue;
        }
        else if (strcmp(rdbuf, "G1") == 0 || strcmp(rdbuf,"G1:Calculator")==0 || strcmp(rdbuf,"Calculator")==0){
            //Clear();
            Calculator(fd_stdin,fd_stdout);
            //printf("G1");
            continue;
        }
        else if (strcmp(rdbuf, "G2") == 0 || strcmp(rdbuf,"G2:Calendar")==0 || strcmp(rdbuf,"Calendar")==0){
            Calendar(fd_stdin,fd_stdout);
            //printf("G2");
            continue;
        }
        else if (strcmp(rdbuf, "G3") == 0){
            G3(fd_stdin,fd_stdout);
            //printf("G3");
            continue;
        }
        else if (strcmp(rdbuf, "G4") == 0){
            G4(fd_stdin,fd_stdout);
            //printf("G4");
            continue;
        }else{
            printf("                                                                           \n");
            printf("       ***       ***      *******                                          \n");
            printf("       ***       ***      *********                Welcome!                \n");
            printf("         ***    ***       **     **               SHABBY OS                \n");
            printf("           *** ***        *******                                          \n");
            printf("             ***          *******                                          \n");
            printf("           *** ***        **    ***           Error:                       \n");
            printf("          ***   ***       **      **          Commands not found           \n");
            printf("        ***      ***      **********          Please Check!                \n");
            printf("        ***      ***      *********                                        \n");
            printf("                                                                           \n");
            printf("                        Input [help] for more information                  \n");
            printf("                                                                           \n");
            printf("    ********************************************************************   \n");
            //continue;
        }

    }
}




/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
    spin("TestB");
}

/*****************************************************************************
 *                                TestC
 *****************************************************************************/
void TestC()
{
    //ProcessManage();
    spin("Testc");
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



