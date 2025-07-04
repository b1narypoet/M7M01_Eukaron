/******************************************************************************
Filename    : rme_platform_xc7z020.h
Author      : hjw
Date        : 2/06/2025
Licence     : LGPL v3+; see COPYING for details.
Description : The configuration file for XC7Z020, with 1MB memory.
              Kernel : 0x80100000 - 0x80FFFFFF.
              Kom1   : 0x81000000 - 0x81FFFFFF.
              User   : 0x84000000 - 0x84FFFFFF.(test)
              Kom2   : None.
******************************************************************************/

/* Define ********************************************************************/
/* Debugging *****************************************************************/
#define RME_ASSERT_ENABLE                   (1U)
#define RME_DBGLOG_ENABLE                   (1U)
/* The HAL library */
/* Are we using raw memory mappings? */
#define RME_PGT_RAW_ENABLE                  (0U)
/* The virtual memory start address for the kernel objects */
#define RME_KOM_VA_BASE                     0x81000000U
/* The size of the kernel object virtual memory - the excess 16MB is for mapping
 * in extra memory, like on-chip SRAM, if they are to be used */
#define RME_KOM_VA_SIZE                     0x01000000U
/* The granularity of kernel memory allocation, in bytes */
#define RME_KOM_SLOT_ORDER                  4U
/* Kernel stack size and address */
#define RME_KOM_STACK_ADDR                  (((rme_ptr_t)&__RME_A7A_Stack_Start)+0x10000U)
/* Kernel stack base and size */
#define RME_KSTK_VA_BASE                    (0x10000FF0U)
#define RME_KSTK_VA_SIZE                    (0x400U)
/* Reboot the processor if the assert fails in this port */
#define RME_ASSERT_FAILED(F,L,D,T)          __RME_Reboot()
/* The maximum number of preemption priority levels in the system.
 * This parameter must be divisible by the word length - 32 is usually sufficient */
#define RME_MAX_PREEMPT_PRIO                32U
#define RME_PREEMPT_PRIO_NUM                32U
/* Hypervisor context virtual memory base - set to 0 if no VM */
#define RME_HYP_VA_BASE                     (0x20000000U)
/* Hypervisor context virtual memory size - set to 0 if no VM */
#define RME_HYP_VA_SIZE                     (0x20000U)
/* Memory mapping PA-VA relationships for the page tables. Tailor this to your own
 * memory mappings if your chip have idiosyncrasies. */
#define RME_A7A_VA2PA(X)                    (((rme_ptr_t)(X))-RME_A7A_VA_BASE)
#define RME_A7A_PA2VA(X)                    (((rme_ptr_t)(X))+RME_A7A_VA_BASE)
/* Init process's first thread's entry point address */
#define RME_A7A_INIT_ENTRY                  0x08004000U
/* Init process's first thread's stack address */
#define RME_A7A_INIT_STACK                  0x081FFF00U
/* What is the Systick value? in ticks. For XC7Z020, always clocked at 1/2 Fcpu */
/* Each Cortex-A9 processor has its own private 32-bit timer and 32-bit watchdog timer,
 * and the two processors share a global 64-bit timer that is always timed at 1/2 of the CPU frequency (CPU_3x2x). */
#define RME_A7A_SYSTICK_VAL                 (6670000U/2U)
/* Size of initial capability table */
#define RME_A7A_INIT_CPT_SIZE               (128U)

/* Processor type - This can make a huge difference when it comes
 * to timer configurations, as Cortex-A5/9 use private timer and
 * Cortex-A7/15/17 use generic timer. Cortex-A8 doesn't have a timer */
#define RME_A7A_CPU_TYPE                    RME_A7A_CPU_CORTEX_A9
/* What is the FPU type? */
#define RME_A7A_FPU_TYPE                    RME_A7A_FPU_VFPV3U
/* What is the GIC type? */
#define RME_A7A_GIC_TYPE                    RME_A7A_GIC_V1
/* What is the interrupt priority grouping? */
#define RME_A7A_GIC_GROUPING                RME_A7A_GIC_GROUPING_P7S1
/* GIC distributor base address */
#define RME_A7A_GICD_BASE                   0xF8F01000
/* GIC CPU interface base address */
#define RME_A7A_GICC_BASE                   0xF8F00100
/* Private timer and watchdog block base */
#define RME_A7A_PTWD_BASE                   0xF8F00600
/* global timer base */
#define RME_A7A_GTWD_BASE                   0xF8F00200

/* Description: L2 Cache sync operation may be faulty ,form xlinx */  
#define CONFIG_PL310_ERRATA_753970 1
/* Data Synchronization Barrier */
#define dsb() __asm__ __volatile__ ("dsb" : : : "memory")

/* L2 cache baseaddr */
#define RME_L2CC_BASEADDR                   0xF8F02000U
/* L2CC Register Offsets */
#define RME_L2CC_ID_OFFSET                  0x0000U
#define RME_L2CC_TYPE_OFFSET                0x0004U
#define RME_L2CC_CNTRL_OFFSET               0x0100U
#define RME_L2CC_AUX_CNTRL_OFFSET           0x0104U
#define RME_L2CC_TAG_RAM_CNTRL_OFFSET       0x0108U
#define RME_L2CC_DATA_RAM_CNTRL_OFFSET      0x010CU

#define RME_L2CC_EVNT_CNTRL_OFFSET          0x0200U
#define RME_L2CC_EVNT_CNT1_CTRL_OFFSET      0x0204U
#define RME_L2CC_EVNT_CNT0_CTRL_OFFSET      0x0208U
#define RME_L2CC_EVNT_CNT1_VAL_OFFSET       0x020CU
#define RME_L2CC_EVNT_CNT0_VAL_OFFSET       0x0210U

#define RME_L2CC_IER_OFFSET                 0x0214U     /* Interrupt Mask */
#define RME_L2CC_IPR_OFFSET                 0x0218U     /* Masked interrupt status */
#define RME_L2CC_ISR_OFFSET                 0x021CU     /* Raw Interrupt Status */
#define RME_L2CC_IAR_OFFSET                 0x0220U     /* Interrupt Clear */

#define RME_L2CC_CACHE_SYNC_OFFSET          0x0730U     /* Cache Sync */
#define RME_L2CC_DUMMY_CACHE_SYNC_OFFSET    0x0740U     /* Dummy Register for Cache Sync */
#define RME_L2CC_CACHE_INVLD_PA_OFFSET      0x0770U     /* Cache Invalid by PA */
#define RME_L2CC_CACHE_INVLD_WAY_OFFSET     0x077CU     /* Cache Invalid by Way */
#define RME_L2CC_CACHE_CLEAN_PA_OFFSET      0x07B0U     /* Cache Clean by PA */
#define RME_L2CC_CACHE_CLEAN_INDX_OFFSET    0x07B8U     /* Cache Clean by Index */
#define RME_L2CC_CACHE_CLEAN_WAY_OFFSET     0x07BCU     /* Cache Clean by Way */
#define RME_L2CC_CACHE_INV_CLN_PA_OFFSET    0x07F0U     /* Cache Invalidate and Clean by PA */
#define RME_L2CC_CACHE_INV_CLN_INDX_OFFSET  0x07F8U     /* Cache Invalidate and Clean by Index */
#define RME_L2CC_CACHE_INV_CLN_WAY_OFFSET   0x07FCU     /* Cache Invalidate and Clean by Way */

#define RME_L2CC_TAG_RAM_DEFAULT_MASK       0x00000111U /* latency for TAG RAM */
#define RME_L2CC_DATA_RAM_DEFAULT_MASK      0x00000121U /* latency for DATA RAM */
#define RME_L2CC_AUX_REG_DEFAULT_MASK       0x72360000U /* Enable all prefetching, */
                                                    /* Cache replacement policy, Parity enable, */
                                                    /* Event monitor bus enable and Way Size (64 KB) */
#define RME_L2CC_AUX_REG_ZERO_MASK          0xFFF1FFFFU 
/* Because the system memory map of Cortex-A based systems are not decided by
 * a particular standard (unlike x86-64), and they cannot be probed as well
 * (memory probing is dangerous). Additionally, the layout of embedded systems
 * are usually fairly volatile, and there is no good policy to dictate the
 * allocation of anything. As a result, we are forced to write down all the
 * mappings that we wish to make. This also needs to be in accordance with
 * the linker script. */
/* Page table registration table */
#define RME_A7A_PGREG_START                 0x80F00000U
#define REM_A7A_PGREG_SIZE                  0x100000U
/* Kernel memory 1 - creating page tables on these memories is allowed */
#define RME_A7A_KOM1_START                  0x81000000U
#define RME_A7A_KOM1_SIZE                   0x40000000U
/* Kernel memory 2 - creating page tables on these memories are not allowed */
#define RME_A7A_KOM2_START                  0x0U
#define RME_A7A_KOM2_SIZE                   0x0U

/* Initial page table contents - They must contain only sections and supersections.
 * The initial page table will be generated according to this. Also, the kernel will
 * seek to use supersections when it can. The first word of the table is the length of
 * this section.
 * The user memory mapped in here will become the user portion of the initial process,
 * and the kernel memory mapped in here will become the shared portion of processes.
 * We will map in the kernel entries as supersections whenever we can */
#define RME_A7A_MEM_ENTRIES                 (6*4U+1U)
#define RME_A7A_MEM_CONTENTS \
/* Number of entries */ \
  RME_A7A_MEM_ENTRIES, \
/* Start_PA    Start_VA    Num             Attributes         */ \
  0x00000000, 0x00000000, 0x400, RME_A7A_MMU_1M_PAGE_USER_DEF /* User memory - 1008 MB DDR2 SDRAM identically mapped */, \
  0x40000000, 0x40000000, 0x200, RME_A7A_MMU_1M_PAGE_USER_SEQ /* User devices - 512MB identical mapping */, \
  0xE0000000, 0x60000000, 0x200, RME_A7A_MMU_1M_PAGE_USER_SEQ /* User devices - 512MB identical mapping */, \
  0x00000000, 0x80000000, 0x400, RME_A7A_MMU_1M_PAGE_KERN_DEF /* Kernel memory - 1023MB DDR2 SDRAM */, \
  0x40000000, 0xC0000000, 0x200, RME_A7A_MMU_1M_PAGE_KERN_SEQ /* Kernel devices - 512MB device space */, \
  0xE0000000, 0xE0000000, 0x200, RME_A7A_MMU_1M_PAGE_KERN_SEQ /* Kernel devices - 512MB Cortex device space */

///* Start_PA    Start_VA    Num             Attributes   0xc02      */ \
//  0x00000000, 0x00000000, 0x1000, RME_A7A_MMU_1M_PAGE_USER_COMMON /* Kernel devices - 512MB Cortex device space */


///* Start_PA    Start_VA    Num             Attributes         */ \
//  0x01000000, 0x01000000, 0x3F0, RME_A7A_MMU_1M_PAGE_USER_DEF /* User memory - 1008 MB DDR2 SDRAM identically mapped */, \
//  0xFFF00000, 0x00000000, 0x1,   RME_A7A_MMU_1M_PAGE_USER_DEF /* User memory - 64kB User-OCSRAM */, \
//  0x40000000, 0x40000000, 0x200, RME_A7A_MMU_1M_PAGE_USER_DEF /* User devices - 512MB identical mapping */, \
//  0xE0000000, 0x60000000, 0x200, RME_A7A_MMU_1M_PAGE_USER_DEF /* User devices - 512MB identical mapping */, \
//  0x00000000, 0x80000000, 0x400, RME_A7A_MMU_1M_PAGE_KERN_DEF /* Kernel memory - 1023MB DDR2 SDRAM */, \
//  0x00000000, 0xC0000000, 0x1,   RME_A7A_MMU_1M_PAGE_KERN_DEF /* Kernel memory - 192kB OCSRAM */, \
//  0x40100000, 0xC0100000, 0x1FF, RME_A7A_MMU_1M_PAGE_KERN_SEQ /* Kernel devices - 512MB device space */, \
//  0xE0000000, 0xE0000000, 0x200, RME_A7A_MMU_1M_PAGE_KERN_SEQ /* Kernel devices - 512MB Cortex device space */


///* Number of entries */ \
//  RME_A7A_MEM_ENTRIES, \
///* Start_PA    Start_VA    Num             Attributes         */ \
//  0x01000000, 0x01000000, 0x3F0, RME_A7A_MMU_1M_PAGE_USER_DEF /* User memory - 1008 MB DDR2 SDRAM identically mapped */, \
//  0xFFF00000, 0x00000000, 0x1,   RME_A7A_MMU_1M_PAGE_USER_DEF /* User memory - 64kB User-OCSRAM */, \
//  0x40000000, 0x40000000, 0x200, RME_A7A_MMU_1M_PAGE_USER_DEF /* User devices - 512MB identical mapping */, \
//  0xE0000000, 0x60000000, 0x200, RME_A7A_MMU_1M_PAGE_USER_DEF /* User devices - 512MB identical mapping */, \
//  0x00000000, 0x80000000, 0x400, RME_A7A_MMU_1M_PAGE_KERN_DEF /* Kernel memory - 1023MB DDR2 SDRAM */, \
//  0x00000000, 0xC0000000, 0x1,   RME_A7A_MMU_1M_PAGE_KERN_DEF /* Kernel memory - 192kB OCSRAM */, \
//  0x40100000, 0xC0100000, 0x1FF, RME_A7A_MMU_1M_PAGE_KERN_SEQ /* Kernel devices - 512MB device space */, \
//  0xE0000000, 0xE0000000, 0x200, RME_A7A_MMU_1M_PAGE_KERN_SEQ /* Kernel devices - 512MB Cortex device space */

///* Number of entries */ \
//  RME_A7A_MEM_ENTRIES, \
///* Start_PA    Start_VA    Num             Attributes         */ \
//  0x01000000, 0x01000000, 0x3F0, RME_A7A_MMU_1M_PAGE_USER_DEF /* User memory - 1008 MB DDR2 SDRAM identically mapped */, \
//  0xFFF00000, 0x00000000, 0x1,   RME_A7A_MMU_1M_PAGE_USER_DEF /* User memory - 64kB User-OCSRAM */, \
//  0x40000000, 0x40000000, 0x200, RME_A7A_MMU_1M_PAGE_USER_DEF /* User devices - 512MB identical mapping */, \
//  0xE0000000, 0x60000000, 0x200, RME_A7A_MMU_1M_PAGE_USER_DEF /* User devices - 512MB identical mapping */, \
//  0x00000000, 0x80000000, 0x400, RME_A7A_MMU_1M_PAGE_KERN_DEF /* Kernel memory - 1023MB DDR2 SDRAM */, \
//  0x00000000, 0xC0000000, 0x1,   RME_A7A_MMU_1M_PAGE_KERN_DEF /* Kernel memory - 192kB OCSRAM */, \
//  0x40100000, 0xC0100000, 0x1FF, RME_A7A_MMU_1M_PAGE_KERN_SEQ /* Kernel devices - 512MB device space */, \
//  0xE0000000, 0xE0000000, 0x200, RME_A7A_MMU_1M_PAGE_KERN_SEQ /* Kernel devices - 512MB Cortex device space */




/* Kernel functions standard to Cortex-A, interrupt management and power */
#define RME_A7A_KERN_INT(X)                 (X)
#define RME_A7A_INT_OP                      0U
#define RME_A7A_INT_ENABLE                  1U
#define RME_A7A_INT_DISABLE                 0U
#define RME_A7A_INT_PRIO                    1U
#define RME_A7A_KERN_PWR                    240U

/* UART peripheral address */
#define RME_A7A_UART_CONTROL                (*((volatile rme_ptr_t*)(0xE0001000)))
#define RME_A7A_UART_MODE                   (*((volatile rme_ptr_t*)(0xE0001004)))
#define RME_A7A_UART_BRGEN                  (*((volatile rme_ptr_t*)(0xE0001018)))
#define RME_A7A_UART_STATUS                 (*((volatile rme_ptr_t*)(0xE000102C)))
#define RME_A7A_UART_FIFO                   (*((volatile rme_ptr_t*)(0xE0001030)))
#define RME_A7A_UART_BRDIV                  (*((volatile rme_ptr_t*)(0xE0001034)))
#define RME_A7A_UART_STATUS_TXE             (1U<<3)

/* Other low-level initialization stuff - The serial port */
#define RME_A7A_LOW_LEVEL_INIT() \
do \
{ \
 /* Somebody have initialized UART for us so we don't need to do it now */ \
 /* Initialize interrupt controller */ \
 \
} \
while(0)

#define RME_A7A_SMP_LOW_LEVEL_INIT() \
do \
{ \
 \
} \
while(0)

/* This is for debugging output */
#define RME_A7A_PUTCHAR(CHAR) \
do \
{ \
    while((RME_A7A_UART_STATUS&RME_A7A_UART_STATUS_TXE)==0); \
 RME_A7A_UART_FIFO=(CHAR); \
} \
while(0)
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
