
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

void HelloInitial(){
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
}

void LoadingInitial(){
    long int ii = 9999;
    printf("                                                                           \n");while(ii-->0);ii=9999;

    printf("    ##          \n");
    printf("    ##          \n");
    printf("    ##          \n");
    printf("    ##          \n");
    printf("    ##          \n");
    printf("    ##          \n");
    printf("    ##          \n");
    printf("    ##          \n");
    printf("    ########    \n");
    printf("    ########    \n");
    milli_delay(1000);
    Clear();
    printf("    ##       #######  \n");
    printf("    ##       #######  \n");
    printf("    ##       ##   ##  \n");
    printf("    ##       ##   ##  \n");
    printf("    ##       ##   ##  \n");
    printf("    ##       ##   ##  \n");
    printf("    ##       ##   ##  \n");
    printf("    ##       ##   ##  \n");
    printf("    ######## #######  \n");
    printf("    ######## #######  \n");
    milli_delay(1000);
    Clear();
    printf("    ##       #######     ##      \n");
    printf("    ##       #######    ####     \n");
    printf("    ##       ##   ##   ##  ##    \n");
    printf("    ##       ##   ##  ##    ##   \n");
    printf("    ##       ##   ##  #########  \n");
    printf("    ##       ##   ##  #########  \n");
    printf("    ##       ##   ##  ###   ###  \n");
    printf("    ##       ##   ##  ###   ###  \n");
    printf("    ######## #######  ###   ###  \n");
    printf("    ######## #######  ###   ###  \n");
    milli_delay(1000);
    Clear();
    printf("    ##       #######     ##      #####     \n");
    printf("    ##       #######    ####     ######    \n");
    printf("    ##       ##   ##   ##  ##    ##   ##   \n");
    printf("    ##       ##   ##  ##    ##   ##    ##  \n");
    printf("    ##       ##   ##  #########  ##     ## \n");
    printf("    ##       ##   ##  #########  ##     ## \n");
    printf("    ##       ##   ##  ###   ###  ##     ## \n");
    printf("    ##       ##   ##  ###   ###  ##   ##   \n");
    printf("    ######## #######  ###   ###  ######    \n");
    printf("    ######## #######  ###   ###  ####      \n");
    milli_delay(1000);
    Clear();
    printf("    ##       #######     ##      #####     ####  \n");
    printf("    ##       #######    ####     ######     ##   \n");
    printf("    ##       ##   ##   ##  ##    ##   ##    ##   \n");
    printf("    ##       ##   ##  ##    ##   ##    ##   ##   \n");
    printf("    ##       ##   ##  #########  ##     ##  ##   \n");
    printf("    ##       ##   ##  #########  ##     ##  ##   \n");
    printf("    ##       ##   ##  ###   ###  ##     ##  ##   \n");
    printf("    ##       ##   ##  ###   ###  ##   ##    ##   \n");
    printf("    ######## #######  ###   ###  ######     ##   \n");
    printf("    ######## #######  ###   ###  ####      ####  \n");
    milli_delay(1000);
    Clear();
    printf("    ##       #######     ##      #####     ####  ####    ##  \n");
    printf("    ##       #######    ####     ######     ##   ####    ##  \n");
    printf("    ##       ##   ##   ##  ##    ##   ##    ##   ## ##   ##  \n");
    printf("    ##       ##   ##  ##    ##   ##    ##   ##   ##  ##  ##  \n");
    printf("    ##       ##   ##  #########  ##     ##  ##   ##  ##  ##  \n");
    printf("    ##       ##   ##  #########  ##     ##  ##   ##   ## ##  \n");
    printf("    ##       ##   ##  ###   ###  ##     ##  ##   ##   ## ##  \n");
    printf("    ##       ##   ##  ###   ###  ##   ##    ##   ##    ####  \n");
    printf("    ######## #######  ###   ###  ######     ##   ##    ####  \n");
    printf("    ######## #######  ###   ###  ####      ####  ##    ####  \n");
    milli_delay(1000);
    Clear();

    printf("    ##       #######     ##      #####     ####  ####    ##   ##########   \n");
    printf("    ##       #######    ####     ######     ##   ####    ##   ##########   \n");
    printf("    ##       ##   ##   ##  ##    ##   ##    ##   ## ##   ##   ##           \n");
    printf("    ##       ##   ##  ##    ##   ##    ##   ##   ##  ##  ##   ##           \n");
    printf("    ##       ##   ##  #########  ##     ##  ##   ##  ##  ##   ##           \n");
    printf("    ##       ##   ##  #########  ##     ##  ##   ##   ## ##   ##   #####   \n");
    printf("    ##       ##   ##  ###   ###  ##     ##  ##   ##   ## ##   ##   #####   \n");
    printf("    ##       ##   ##  ###   ###  ##   ##    ##   ##    ####   ##    # ##   \n");
    printf("    ######## #######  ###   ###  ######     ##   ##    ####   ####### ##   \n");
    printf("    ######## #######  ###   ###  ####      ####  ##    ####   ####### ##   \n\n");

    startAnimate();
}

void CommonInitial(int year,int month,int day){
    char cMonth[128]="";
    char cDay[128]="";
    switch (month){
        case 1:
            strcpy( cMonth, "Janauary");
            break;
        case 2:
            strcpy( cMonth, "February");
            break;
        case 3:
            strcpy( cMonth, "March");
            break;
        case 4:
            strcpy( cMonth, "April");
            break;
        case 5:
            strcpy( cMonth, "May");
            break;
        case 6:
            strcpy( cMonth, "June");
            break;
        case 7:
            strcpy( cMonth, "July");
            break;
        case 8:
            strcpy( cMonth, "August");
            break;
        case 9:
            strcpy( cMonth, "September");
            break;
        case 10:
            strcpy( cMonth, "Octorber");
            break;
        case 11:
            strcpy( cMonth, "November");
            break;
        case 12:
            strcpy( cMonth, "December");
            break;
    }
    int weekday = Calculate(year, month);//计算出与基准年月差多少天2018-0101，星期一
    int flag = 0;
    flag = (weekday+day-1) % 7;
    switch (flag){
        case 1:
            strcpy( cDay, "Monday");
            break;
        case 2:
            strcpy( cDay, "Tuesday");
            break;
        case 3:
            strcpy( cDay, "Wednesday");
            break;
        case 4:
            strcpy( cDay, "Thursday");
            break;
        case 5:
            strcpy( cDay, "Friday");
            break;
        case 6:
            strcpy( cDay, "Saturday");
            break;
        case 0:
            strcpy( cDay, "Sunday");
            break;
        default:
            break;
    }
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
    printf("                           %s,%s %d,%d\n",cDay,cMonth,day,year);
    printf("    ********************************************************************   \n");
}

void clearArr(char *arr, int length)
{
    int i;
    for (i = 0; i < length; i++)
        arr[i] = 0;
}

void ProcessManage(){

    int i;
    printf("                                                                           \n");
    printf("       ***       ***      *******                                          \n");
    printf("       ***       ***      *********                                        \n");
    printf("         ***    ***       **     **                                        \n");
    printf("           *** ***        *******                   Welcome                \n");
    printf("             ***          *******                                          \n");
    printf("           *** ***        **    ***                SHABBY OS               \n");
    printf("          ***   ***       **      **                                       \n");
    printf("        ***      ***      **********             Process Manage            \n");
    printf("        ***      ***      *********                                        \n");
    printf("                                                                           \n");
    printf("                                                \n");
    printf("    ********************************************************************   \n");

    printf("=============================================================================\n");
    printf("   ProcessID   |   ProcessName   |   ProcessPriority   |   Running?\n");
    //进程号，进程名，优先级，是否是系统进程，是否在运行
    printf("-----------------------------------------------------------------------------\n");
    for ( i = 0 ; i < NR_TASKS + NR_PROCS ; ++i )//逐个遍历
    {
//		if ( proc_table[i].priority == 0) continue;//系统资源跳过
        printf("    %d             %s                %d                    yes\n", proc_table[i].pid, proc_table[i].name, proc_table[i].priority);
    }
    printf("=============================================================================\n");


}

void Clear()
{
    clear_screen(0,console_table[current_console].cursor);
    console_table[current_console].crtc_start = 0;
    console_table[current_console].cursor = 0;
}

/*****************************************************************************
 *                            计算机  Calculator Begin!
 *****************************************************************************/

#define MAXSIZE 100

typedef struct {
    int size;
    int top;
    int base;
    char body[MAXSIZE];
}opStack;
typedef struct {
    int size;
    int top;
    int base;
    int body[MAXSIZE];
}calStack;
typedef struct {
    int size;
    int top;
    int base;
    double body[MAXSIZE];
}floStack;
opStack ops;
calStack cals;
floStack flo;
int A[MAXSIZE];
double B[MAXSIZE];
void calculate_int();
void calculate_float();
void calculator_start(){
    int i=0;
    ops.base = -1;
    ops.size = 0;
    ops.top = -1;

    cals.base = -1;
    cals.size = 0;
    cals.top = -1;

    flo.base = -1;
    flo.size = 0;
    flo.top = -1;
    for(i=0;i<MAXSIZE;i++){
        A[i] = 0;
        ops.body[i] = 0;
        cals.body[i] = 0;
        flo.body[i] = 0.0;
    }
}
void transform(int fd_stdin, int fd_stdout){
    char form[MAXSIZE];
    /***/
    char ch[100];
    int r = read(fd_stdin, form, 70);
    form[r] = 0;
    /***/
    int isOver = 0;
    int i = 0;
    int j = 0;
    int isfloat = 0;
    int isdigit = 0;
    do{
        if(form[i]<='9'&&form[i]>='0'){
            if(isdigit==0){
                A[j] = form[i]-'0' + 100;
                i++;j++;
            }else{
                A[j-1] = (A[j-1]-100)*10 + form[i]-'0' + 100;
                i++;
            }
            isdigit=1;
        }else if(form[i]=='+'||form[i]=='-'){
            isdigit = 0;
            int flag = 0;
            while(flag==0){
                if(ops.size==0||ops.body[ops.top]=='('){
                    ops.top++;ops.size++;
                    ops.body[ops.top] = form[i];
                    flag = 1;
                }else{
                    A[j]=ops.body[ops.top];
                    ops.top--;
                    ops.size--;
                    j++;
                }
            }
            i++;
        }else if(form[i]=='*'||form[i]=='/'){
            isdigit = 0;
            int flag = 0;
            while(flag==0){
                if(ops.size==0||ops.body[ops.top]=='('||ops.body[ops.top]=='+'||ops.body[ops.top]=='-'){
                    ops.top++;
                    ops.body[ops.top] = form[i];
                    ops.size++;
                    flag = 1;
                }else{
                    A[j]=ops.body[ops.top];
                    ops.top--;
                    ops.size--;
                    j++;
                }
            }
            i++;
        }else if(form[i]=='('){
            isdigit = 0;
            ops.top++;ops.size++;
            ops.body[ops.top] = form[i];
            i++;
        }else if(form[i]==')'){
            isdigit = 0;
            while(ops.size!=0&&ops.body[ops.top]!='('){
                A[j]=ops.body[ops.top];
                ops.top--;
                ops.size--;
                j++;
            }
            if(ops.size==0){
                isOver = 1;
            }else{
                ops.top--;
                ops.size--;
            }
            i++;
        }else if(form[i]=='.'){
            isfloat++;
            int k = 1;
            double index = 0.1;
            B[isfloat] = A[j-1]-100;
            A[j-1] = -isfloat;
            while(form[i+k]<='9'&&form[i+k]>='0'){
                B[isfloat]= B[isfloat] + index * (form[i+k]-'0');
                index = index/10.0;
                k++;
            }
            i = i+k;
        }else{
            if(ops.size>0){
                while(ops.size>0){
                    A[j]=ops.body[ops.top];
                    ops.top--;
                    ops.size--;
                    j++;
                }
            }
            isOver = 1;
        }
    }while(isOver==0);
    A[j] = 0;
    if(isfloat==0){
        calculate_int();
    }else{
        calculate_float();
    }

}
void calculate_float(){
    int i = 0;
    int j = 0;
    int isOver = 0;
    int isdigit = 0;
    int error=0;
    while(isOver==0){
        if(A[i]>=100){
            flo.top++;
            flo.body[flo.top] = A[i]-100;
            flo.size++;
        }else if(A[i]<0){
            flo.top++;
            flo.body[flo.top] = B[-A[i]];
            flo.size++;
        }else{
            isdigit = 0;
            switch(A[i]){
                case '+':
                    if(flo.size<2){
                        isOver = 1;
                        error=1;
                    }else{
                        double a = flo.body[flo.top];
                        flo.top--;flo.size--;
                        double b = flo.body[flo.top];
                        flo.body[flo.top] = a + b;
                    }
                    break;
                case '-':
                    if(flo.size<2){
                        isOver = 1;error=1;
                    }else{
                        double a = flo.body[flo.top];
                        flo.top--;flo.size--;
                        double b = flo.body[flo.top];
                        flo.body[flo.top] = b - a ;
                    }
                    break;
                case '*':
                    if(flo.size<2){
                        isOver = 1;error=1;
                    }else{
                        double a = flo.body[flo.top];
                        flo.top--;flo.size--;
                        double b = flo.body[flo.top];
                        flo.body[flo.top] = a * b;
                    }
                    break;
                case '/':
                    if(flo.size<2){
                        isOver = 1;error=1;
                    }else{
                        double a = flo.body[flo.top];
                        flo.top--;flo.size--;
                        double b = flo.body[flo.top];
                        if(a-0<=0.000000001){
                            printf("    chu 0");
                        }else{
                            flo.body[flo.top] = b / a ;
                        }
                    }
                    break;
                default:
                    isOver = 1;
                    break;
            }
        }
        i++;
    }
    if(error!=0||flo.size!=1){
        printf("    -----ERROR-----\n    CAN'T CALCULATE\n    -----ERROR-----\n");
    }else{
        printf("    Result is %lf\n",flo.body[flo.top]);
    }
}
void calculate_int(){
    int i = 0;
    int j = 0;
    int isOver = 0;
    int isdigit = 0;
    int error=0;
    while(isOver==0){
        if(A[i]>=100){
            cals.top++;
            cals.body[cals.top]=A[i]-100;
            cals.size++;
        }else{
            isdigit = 0;
            switch(A[i]){
                case '+':
                    if(cals.size<2){
                        isOver = 1;
                        error=1;
                    }else{
                        int a = cals.body[cals.top];
                        cals.top--;cals.size--;
                        int b = cals.body[cals.top];
                        cals.body[cals.top] = a + b;
                    }
                    break;
                case '-':
                    if(cals.size<2){
                        isOver = 1;error=1;
                    }else{
                        int a = cals.body[cals.top];
                        cals.top--;cals.size--;
                        int b = cals.body[cals.top];
                        cals.body[cals.top] = b - a;
                    }
                    break;
                case '*':
                    if(cals.size<2){
                        isOver = 1;error=1;
                    }else{
                        int a = cals.body[cals.top];
                        cals.top--;cals.size--;
                        int b = cals.body[cals.top];
                        cals.body[cals.top] = a * b;
                    }
                    break;
                case '/':
                    if(cals.size<2){
                        isOver = 1;error=1;
                    }else{
                        int a = cals.body[cals.top];
                        cals.top--;cals.size--;
                        int b = cals.body[cals.top];
                        if(a==0){
                            printf("    chu 0");
                        }else{
                            cals.body[cals.top] = b / a ;
                        }
                    }
                    break;
                default:
                    isOver = 1;
                    break;
            }
        }
        i++;
    }
    if(error!=0||cals.size!=1){
        printf("    -----ERROR-----\n    CAN'T CALCULATE\n    -----ERROR-----\n");
    }else{
        printf("    Result is %d\n",cals.body[cals.top]);
    }
}

void CalculatorInitial(){
    printf("                                                                           \n");
    printf("       ***       ***      *******                                          \n");
    printf("       ***       ***      *********                Welcome!                \n");
    printf("         ***    ***       **     **               SHABBY OS                \n");
    printf("           *** ***        *******              Calculator System           \n");
    printf("             ***          *******                                          \n");
    printf("           *** ***        **    ***           by 1652761 Huang Yaoxian     \n");
    printf("          ***   ***       **      **          by 1652791 Wen Tingjie       \n");
    printf("        ***      ***      **********          by 1652792 Luo Jihao         \n");
    printf("        ***      ***      *********                                        \n");
    printf("                                                                           \n");
    printf("    ********************************************************************   \n");

    printf("                                   Tips                                    \n");
    printf("\n");
    printf("         Enter the formula that needs to be calculated.                    \n");
    printf("         e.x: Input  :    (5+2)*3                                          \n");
    printf("         e.x: Output :       21                                            \n");
    printf("         The formula may contain elements like:                            \n");
    printf("                + - * / ( ) .  numbers 0~9                                 \n");
    printf("\n");
    printf("    ********************************************************************   \n");
}

void Calculator(int fd_stdin,int fd_stdout)
{
    CalculatorInitial();
    while(1){
        printf("    Enter the formula : ");
        calculator_start(); //initialize
        transform(fd_stdin,fd_stdout);
        printf("    CONTINUE?(Y/N)");
        char ch[100];
        int r = read(fd_stdin, ch, 70);
        ch[r] = 0;
        if(ch[0]!='Y'&&ch[0]!='y'){
            break;
        }
    }
}

/*****************************************************************************
 *                       计算机 Calculator Ends!
 *****************************************************************************/


/*****************************************************************************
 *                        日历 Calendar Begin!
 *****************************************************************************/

int AtoiSelf(char *str) {
    int temp = 0;
    const char *ptr = str;  //ptr保存str字符串开头

    if (*str == '-' || *str == '+')  //如果第一个字符是正负号，
    {                      //则移到下一个字符
        str++;
    }
    while (*str != 0) {
        if ((*str < '0') || (*str > '9'))  //如果当前字符不是数字
        {                       //则退出循环
            break;
        }
        temp = temp * 10 + (*str - '0'); //如果当前字符是数字则计算数值
        str++;      //移到下一个字符
    }
    if (*ptr == '-')     //如果字符串是以“-”开头，则转换成其相反数
    {
        temp = -temp;
    }

    return temp;
}

void OutputChoice1(int weekday, int day)
{
    int i = 0;
    int date = 1;
    int flag = weekday;
    printf("    Sun  Mon  Tue  Wed  Thu  Fri  Sat\n    ");

    for (i = 0; i < (day + weekday) / 7 + 1; i++)
    {
        int j = 0;
        int k = 0;
        for (j = 0; j < weekday; j++)
        {
            if (flag != 0)
            {
                printf("     ");
            }
            flag--;
        }
        //判断第一天是星期几
        for (k = 0; k < 7 - weekday; k++)
        {
            printf("%2d   ", date);
            date++;
            if (date > day)
                break;
        }
        weekday = 0;
        printf("\n    ");
    }
}

void OutputChoice2(int weekday, int curday)
{
    int flag = 0;
    flag = (weekday+curday-1) % 7;
    switch (flag){
        case 1:
            printf("Monday\n");
            break;
        case 2:
            printf("Tuesday\n");
            break;
        case 3:
            printf("Wednesday\n");
            break;
        case 4:
            printf("Thursday\n");
            break;
        case 5:
            printf("Friday\n");
            break;
        case 6:
            printf("Saturday\n");
            break;
        case 0:
            printf("Sunday\n");
            break;
        default:
            break;
    }
}

int Calculate(int year, int month)
{
    int standardYear = 2018;
    int i = 0;
    int sum = 0;//用来保存所输入的日期与基准相差的天数
    int flag1 = 0;//用来判断闰年二月
    int flag2 = 0;//用来判断输入年份与基准大小关系
    int weekday = 0;//用来保存算出来每月第一天是周几

    if (standardYear > year)
    {
        standardYear ^= year;
        year ^= standardYear;
        standardYear ^= year;
        flag2 = 1;
    }
    //确保standardyear比当前需要查询的year要小

    for (i = standardYear; i < year; i++)
    {
        if (((i % 4 == 0) && (i % 100 == 0)) || (i % 400 == 0))
        {
            sum = sum + 366;
            flag1 = 1;
        }
        else
            sum = sum + 365;
    }//计算出年份与基准年份相差的天数

    for (i = 1; i < month; i++)
    {

        if ((i == 1) || (i == 3) || (i == 5) || (i == 7)
            || (i == 8) || (i == 10) || (i == 12))
        {
            sum = sum + 31;
        }

        else if ((i == 2) && (flag1 == 1))
        {
            sum = sum + 29;
        }

        else if ((i == 2) && (flag1 == 0))
        {
            sum = sum + 28;
        }
        else
            sum = sum + 30;

    }
    if (flag2 == 0)
        weekday = sum % 7 + 1;//加一是由于基准月份第一天是周一
    else
        weekday = sum % 7 - 1;//比基准年份小是要减一
    return weekday;

}

int AllDay(int year ,int month)
{
    int flag = 0;
    int day = 30;

    if (((year % 4 == 0) && (year % 100 == 0)) || (year % 400 == 0))
    {
        flag = 1;
    }
    //判断闰年
    if ((month == 1) || (month == 3) || (month == 5) || (month == 7)
        || (month == 8) || (month == 10) || (month == 12))
    {
        day = 31;
    }
        //判断大月或小月
    else if ((month == 2) && (flag == 1))
    {
        day = 29;
    }
        //闰年的二月
    else if ((month == 2) && (flag == 0))
    {
        day = 28;
    }
    else
        day = 30;

    return day;
}

void JudgeFestival(int month,int day){
    switch (month)
    {
        case 1:switch (day)
            {
                default:printf("    not festival");
            }break;
        case 2:switch (day)
            {
                case 14:printf("    Valentines Day"); break;
                default:printf("    not festival");
            }break;
        case 3:switch (day)
            {
                case 8:printf("    Women's Day"); break;
                case 12:printf("    Arbor Day"); break;
                default:printf("    not festival");
            }break;
        case 4:switch (day)
            {
                case 1:printf("    April Fools Day"); break;
                case 5:printf("    Tomb-sweeping Day"); break;
                default:printf("    not festival");
            }break;
        case 5:switch (day)
            {
                case 1:printf("    Labor Day"); break;
                case 4:printf("    Chinese Youth Day");
                default:printf("    not festival");
            }break;
        case 6:switch (day)
            {
                case 1:printf("    Children's Day"); break;
                default:printf("    not festival");
            }break;
        case 8:switch (day)
            {
                case 1:printf("    the Army's Day"); break;
                default:printf("    not festival");
            }break;
        case 9:switch (day)
            {
                case 10:printf("    Teacher's Day"); break;
                default:printf("    not festival");
            }break;
        case 10:switch (day)
            {
                case 1: printf("    National Day"); break;
                case 31:printf("    Helloween Day"); break;
                default:printf("    not festival");
            }break;
        case 12:switch (day)
            {
                case 25:printf("    Christmas Day"); break;
                default:printf("    not festival");
            }break;
    }
    printf("\n");
}

void CalendarInitial(){
    printf("                                                                           \n");
    printf("       ***       ***      *******                                          \n");
    printf("       ***       ***      *********                Welcome!                \n");
    printf("         ***    ***       **     **               SHABBY OS                \n");
    printf("           *** ***        *******               Calendar System            \n");
    printf("             ***          *******                                          \n");
    printf("           *** ***        **    ***                Command List            \n");
    printf("          ***   ***       **      **             1. Month Search           \n");
    printf("        ***      ***      **********             2. Day Search             \n");
    printf("        ***      ***      *********              3. Quit                   \n");
    printf("                                                                           \n");
    printf("         Select the number at the begining to choose the function          \n");
    printf("         e.x: Input  :    1                                                \n");
    printf("\n");
    printf("    ********************************************************************   \n");
}

void Calendar(int fd_stdin,int fd_stdout,int todayYear,int todayMonth,int todayDay){
    CalendarInitial();
    char rdbuf[128];
    int r = 0;
    char choice[128];
    printf("    Please input your choice: ");
    r = read(fd_stdin, choice, 70);
    choice[r] = 0;
    // printf("%s",choice);
    while (strcmp(choice,"3")!=0) {
        char IYear[128]="";
        char IMonth[128]="";
        char IDay[128]="";
        int year = 0;
        int month = 0;
        int weekday = 0;//用来保存输入月份第一天是周几
        int day = 0;//用来保存每月共有多少天
        if (strcmp(choice,"1")==0) //选择1 按照年月 输出日历
        {
            printf("    Please input the year and the month you want to explore:\n");
            printf("    Please input the year:");
            r = read(fd_stdin, IYear, 70);
            IYear[r] = 0;
            year = AtoiSelf(IYear);
            printf("    Please input the month:");
            r = read(fd_stdin, IMonth, 70);
            IMonth[r] = 0;
            month = AtoiSelf(IMonth);
            while (month > 12 || month < 1) {
                printf("    Input Error! Please Check\n");
                printf("    Please input the year:");
                r = read(fd_stdin, IYear, 70);
                IYear[r] = 0;
                year = AtoiSelf(IYear);
                printf("    Please input the month:");
                r = read(fd_stdin, IMonth, 70);
                IMonth[r] = 0;
                month = AtoiSelf(IMonth);
            }
            printf("    ============================\n");
            printf("    Year = %d   Month = %d\n", year, month);
            weekday = Calculate(year, month);//计算出与基准年月差多少天2018-0101，星期一
            day = AllDay(year, month);//用来计算每月共有多少天
            OutputChoice1(weekday, day);
        }
        else if (strcmp(choice,"2")==0)//选择2 按照日期 确定周几
        {
            int curday = 0;
            printf("    Please input the date you want to explore:(format: year month day)\n");
            printf("    Please input the year:");
            r = read(fd_stdin, IYear, 70);
            IYear[r] = 0;
            year = AtoiSelf(IYear);
            printf("    Please input the month:");
            r = read(fd_stdin, IMonth, 70);
            IMonth[r] = 0;
            month = AtoiSelf(IMonth);
            printf("    Please input the day:");
            r = read(fd_stdin, IDay, 70);
            IDay[r] = 0;
            curday = AtoiSelf(IDay);
            //scanf("%d%d%d", &year, &month, &curday);
            while (month > 12 || month < 1 || curday<1 || curday>31) {
                printf("    Input Error! Please Check\n");
                printf("    Please input the date you want to explore:(format: year month day)\n");
                printf("    Please input the year:");
                r = read(fd_stdin, IYear, 70);
                IYear[r] = 0;
                year = AtoiSelf(IYear);
                printf("    Please input the month:");
                r = read(fd_stdin, IMonth, 70);
                IMonth[r] = 0;
                month = AtoiSelf(IMonth);
                printf("    Please input the day:");
                r = read(fd_stdin, IDay, 70);
                IDay[r] = 0;
                curday = AtoiSelf(IDay);
            }
            printf("    ================================================\n");
            printf("    Year = %d   Month = %d   Day = %d is ", year, month, curday);
            weekday = Calculate(year, month);//计算出与基准年月差多少天2018-0101，星期一
            OutputChoice2(weekday, curday);
            JudgeFestival(month,curday);
        }else{
            printf("    Input Error! Please Check!\n");
            printf("    Today is %d/%d/%d",todayYear,todayMonth,todayDay);
            //choice = 0;
        }
        CalendarInitial();
        printf("    Please input your choice: ");
        r = read(fd_stdin, choice, 70);
        choice[r] = 0;
    }
}
/*****************************************************************************
 *                            日历   Calendar ends!
 *****************************************************************************/

/*****************************************************************************
 *                            时钟   Clock Begins!
 *****************************************************************************/

int hour = 0;
int minute = 0;
int second = 0;

void Timer(int fd_stdin,int fd_stdout)
{
    printf("    Seconds Input:");
    char rdbuf[128];
    int r = read(fd_stdin, rdbuf, 70);
    rdbuf[r] = 0;
    int i = AtoiSelf(rdbuf);
    printf("    The time you set is %d\n    Now begin....\n",i);
    for(;i >=0;i--){
        printf("    %d  Left\n",i);
        milli_delay(15000);
    }
    printf("    Time up!\n");
}

void ClockInitial(){
    printf("                                                                           \n");
    printf("       ***       ***      *******                                          \n");
    printf("       ***       ***      *********                Welcome!                \n");
    printf("         ***    ***       **     **               SHABBY OS                \n");
    printf("           *** ***        *******                Clock System              \n");
    printf("             ***          *******                                          \n");
    printf("           *** ***        **    ***                Command List            \n");
    printf("          ***   ***       **      **             1. Show Current Time      \n");
    printf("        ***      ***      **********             2. Timer                  \n");
    printf("        ***      ***      *********              3. Quit                   \n");
    printf("                                                                           \n");
    printf("         Select the number at the begining to choose the function          \n");
    printf("         e.x: Input  :    1                                                \n");
    printf("\n");
    printf("    ********************************************************************   \n");
}

void TimerInitial(){
    printf("                                                                           \n");
    printf("       ***       ***      *******                                          \n");
    printf("       ***       ***      *********                                        \n");
    printf("         ***    ***       **     **                                        \n");
    printf("           *** ***        *******                   Welcome                \n");
    printf("             ***          *******                                          \n");
    printf("           *** ***        **    ***                SHABBY OS               \n");
    printf("          ***   ***       **      **                                       \n");
    printf("        ***      ***      **********                 Timer                 \n");
    printf("        ***      ***      *********                                        \n");
    printf("                                                                           \n");
    printf("         Enter the seconds to count:                                       \n");
    printf("         e.x: Input  :    60                                               \n");
    printf("\n");
    printf("    ********************************************************************   \n");
}

void Clock(int fd_stdin,int fd_stdout){

    ClockInitial();
    char rdbuf[128];
    int r = 0;
    char choice[128];
    printf("    Please input your choice: ");
    r = read(fd_stdin, choice, 70);
    choice[r] = 0;
    // printf("%s",choice);
    while (strcmp(choice,"3")!=0) {
        if (strcmp(choice,"1")==0) //选择1 时钟
        {
            if (hour==0 && minute==0 && second==0){
                printf("    Please first turn on the time machine!\n");
                printf("    Enter: Ctrl+F2 then enter Time Machine");
            }else{
                printf("    Current time is %d : %d : %d\n", hour, minute, second);
            }
        }else if (strcmp(choice,"2")==0){
            Clear();
            TimerInitial();
            Timer(fd_stdin,fd_stdout);
        }else{
            printf("    Input Error! Please Check!\n");
        }
        printf("    ********************************************************************   \n");
        ClockInitial();
        printf("    Please input your choice: ");
        r = read(fd_stdin, choice, 70);
        choice[r] = 0;
    }
}

/*****************************************************************************
 *                            时钟   Clock ends!
 *****************************************************************************/

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

int tYear = 0;
int tMonth = 0;
int tDay = 0;

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

    HelloInitial();

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

    int curYear  = 0;
    int curMonth = 0;
    int curDay   = 0;
    int firstTimeFlag = 0;
    while (1) {
        if (firstTimeFlag==0){
            int t;
            printf("System Initialzation~\n");
            printf("Date Initialzation\n");
            printf("Enter the current year(e.x. 2018):");
            t = read(fd_stdin, rdbuf, 70);
            rdbuf[t] = 0;
            curYear = AtoiSelf(rdbuf);
            tYear = curYear;
            clearArr(rdbuf,128);
            printf("Enter the current month(e.x. 10):");
            t = read(fd_stdin, rdbuf, 70);
            rdbuf[t] = 0;
            curMonth = AtoiSelf(rdbuf);
            tMonth = curMonth;
            clearArr(rdbuf,128);
            printf("Enter the current day(e.x. 4):");
            t = read(fd_stdin, rdbuf, 70);
            rdbuf[t] = 0;
            curDay = AtoiSelf(rdbuf);
            tDay = curDay;
            clearArr(rdbuf,128);
            firstTimeFlag = 1;
            Clear();
            LoadingInitial();
            Clear();
            CommonInitial(curYear,curMonth,curDay);
        }

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
            CommonInitial(curYear,curMonth,curDay);
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
            Calendar(fd_stdin,fd_stdout,curYear,curMonth,curDay);
            //printf("G2");
            continue;
        }
        else if (strcmp(rdbuf, "G3") == 0 || strcmp(rdbuf,"G3:Clock")==0 || strcmp(rdbuf,"Clock")==0){
            Clock(fd_stdin,fd_stdout);
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
void ClockShow(int hour,int minute,int second){
    char cMonth[128]="";
    char cDay[128]="";
    switch (tMonth){
        case 1:
            strcpy( cMonth, "Janauary");
            break;
        case 2:
            strcpy( cMonth, "February");
            break;
        case 3:
            strcpy( cMonth, "March");
            break;
        case 4:
            strcpy( cMonth, "April");
            break;
        case 5:
            strcpy( cMonth, "May");
            break;
        case 6:
            strcpy( cMonth, "June");
            break;
        case 7:
            strcpy( cMonth, "July");
            break;
        case 8:
            strcpy( cMonth, "August");
            break;
        case 9:
            strcpy( cMonth, "September");
            break;
        case 10:
            strcpy( cMonth, "Octorber");
            break;
        case 11:
            strcpy( cMonth, "November");
            break;
        case 12:
            strcpy( cMonth, "December");
            break;
    }
    int weekday = Calculate(tYear, tMonth);//计算出与基准年月差多少天2018-0101，星期一
    int flag = 0;
    flag = (weekday+tDay-1) % 7;
    switch (flag){
        case 1:
            strcpy( cDay, "Monday");
            break;
        case 2:
            strcpy( cDay, "Tuesday");
            break;
        case 3:
            strcpy( cDay, "Wednesday");
            break;
        case 4:
            strcpy( cDay, "Thursday");
            break;
        case 5:
            strcpy( cDay, "Friday");
            break;
        case 6:
            strcpy( cDay, "Saturday");
            break;
        case 0:
            strcpy( cDay, "Sunday");
            break;
        default:
            break;
    }
    printf("                                                                           \n");
    printf("       ***       ***      *******                                          \n");
    printf("       ***       ***      *********                Welcome!                \n");
    printf("         ***    ***       **     **               SHABBY OS                \n");
    printf("           *** ***        *******                Time System               \n");
    printf("             ***          *******                                          \n");
    printf("           *** ***        **    ***           by 1652761 Huang Yaoxian     \n");
    printf("          ***   ***       **      **          by 1652791 Wen Tingjie       \n");
    printf("        ***      ***      **********          by 1652792 Luo Jihao         \n");
    printf("        ***      ***      *********                                        \n");
    printf("                                                                           \n");
    printf("      Initial Time : %s,%s %d,%d   %d : %d : %d\n",cDay,cMonth,tDay,tYear,hour,minute,second);
    printf("    *********************   Time   Machine  Begins      ****************   \n");
    printf("    ********************************************************************   \n");
}


void TestB() {

    char rdbuf[128];
    char tty_name[] = "/dev_tty1";
    int fd_stdin = open(tty_name, O_RDWR);
    assert(fd_stdin == 0);
    int fd_stdout = open(tty_name, O_RDWR);
    assert(fd_stdout == 1);

    Clear();

    int r = read(fd_stdin, rdbuf, 70);
    rdbuf[r] = 0;
    if (strcmp(rdbuf, "Time Machine") == 0) {

        printf("Please enter initial time:\n");
        printf("Please enter hour:");
        r = read(fd_stdin, rdbuf, 70);
        rdbuf[r] = 0;
        hour = AtoiSelf(rdbuf);
        printf("Please enter minute:");
        r = read(fd_stdin, rdbuf, 70);
        rdbuf[r] = 0;
        minute = AtoiSelf(rdbuf);
        printf("Please enter second:");
        r = read(fd_stdin, rdbuf, 70);
        rdbuf[r] = 0;
        second = AtoiSelf(rdbuf);
        ClockShow(hour, minute, second);

        while (1) {
            second += 1;
            if (second >= 60) {
                second = 0;
                minute += 1;
            }
            if (minute >= 60) {
                minute = 0;
                hour += 1;
            }
            if (hour >= 24) {
                hour = 0;
                tDay += 1;
            }
            milli_delay(15000);
            //printf("    Current time is %d : %d : %d\n", hour, minute, second);
            //clearArr(rdbuf, r);
        }
    }
    else{
        printf("    Input Error! Please Check!\n");
    }

    //Clear();
    //ClockShow(hour,minute,second);


}

/*****************************************************************************
 *                                TestC
 *****************************************************************************/


void TestC()
{
    spin("TestC");
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



