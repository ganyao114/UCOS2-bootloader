/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                          (c) Copyright 1992-2006, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* File    : uCOS_II.C
* By      : Jean J. Labrosse
* Version : V2.83
*********************************************************************************************************
*/

#define  OS_GLOBALS                           /* Declare GLOBAL variables                              */
#include "ucos_ii.h"


#define  OS_MASTER_FILE                       /* Prevent the following files from including includes.h */
#include "./UCOS/os_core.c"
#include "./UCOS/os_flag.c"
#include "./UCOS/os_mbox.c"
#include "./UCOS/os_mem.c"
#include "./UCOS/os_mutex.c"
#include "./UCOS/os_q.c"
#include "./UCOS/os_sem.c"
#include "./UCOS/os_task.c"
#include "./UCOS/os_time.c"
#include "./UCOS/os_tmr.c"
