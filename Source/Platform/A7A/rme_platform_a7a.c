/******************************************************************************
Filename    : platform_a7a.c
Author      : hjw
Date        : 01/04/2025
Licence     : LGPL v3+; see COPYING for details.
Description : The hardware abstraction layer for ARMv7-A machines.
              1. Interrupt controller
              2. Timer
              3. Pgt setup
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "Platform/A7A/rme_platform_a7a.h"
#include "Kernel/rme_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/A7A/rme_platform_a7a.h"
#include "Kernel/rme_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/A7A/rme_platform_a7a.h"

#define __HDR_PUBLIC__
#include "Kernel/rme_kernel.h"
#undef __HDR_PUBLIC__
/* End Include ***************************************************************/

/* Function:main **************************************************************
Description : The entrance of the operating system.
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
int main(void)
{
   RME_Kmain();

    return 0;
}
/* End Function:main *********************************************************/

/* Function:RME_Out32 *****************************************************
Description : Performs an output operation for a 32-bit memory location by writing the
 specified Value to the the specified address
Input       : volatile rme_ptr_t Addr - Addr contains the address to perform the output operation
              rme_ptr_t Value - Value contains the Value to be output at the specified address.
Output      : None.
Return      : rme_ptr_t - Always 0.
******************************************************************************/
void RME_Out32(volatile rme_ptr_t Addr, rme_ptr_t Value)
{
    volatile rme_ptr_t *LocalAddr = (rme_ptr_t *)Addr;
    *LocalAddr = Value;
}

/* Function:__RME_Putchar *****************************************************
Description : Output a character to console. In Cortex-M, under most circumstances, 
              we should use the ITM for such outputs.
Input       : char Char - The character to print.
Output      : None.
Return      : rme_ptr_t - Always 0.
******************************************************************************/
rme_ptr_t __RME_Putchar(char Char)
{
    RME_A7A_PUTCHAR(Char);
    return 0;
}
/* End Function:__RME_Putchar ************************************************/

/* Function:__RME_A7A_CPU_Local_Get ******************************************
Description : Get the CPU-local data structures. This is to identify where we are
              executing.
Input       : None.
Output      : None.
Return      : struct RME_CPU_Local* - The CPU-local data structures.
******************************************************************************/
/*struct RME_CPU_Local* __RME_A7A_CPU_Local_Get(void)
{
    return &RME_A7A_Local[__RME_A7A_MPIDR_Get()&0x03];
}*/
/* End Function:__RME_A7A_CPU_Local_Get *************************************/

/* Function:__RME_A7A_Int_Init ***********************************************
Description : Initialize the interrupt controller of the Cortex-A ARMv7 platform.
              This will only initialize the distributor and will only be run by
              the first CPU. This will initialize the timer as well.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_A7A_Int_Init(void)
{
    rme_ptr_t Temp;
    rme_ptr_t Lines;

    /* Who implemented the GIC, what variant, how many interrupts? */
    Temp=RME_A7A_GICD_IIDR;
    RME_DBG_S("\r\nA7A-GIC: ProductID: ");
    RME_DBG_I(Temp>>24);
    RME_DBG_S("\r\nA7A-GIC: Variant: ");
    RME_DBG_I((Temp>>20)&0xF);
    RME_DBG_S("\r\nA7A-GIC: Revision: ");
    RME_DBG_I((Temp>>12)&0xF);
    RME_DBG_S("\r\nA7A-GIC: Implementer: 0x");
    RME_DBG_H(Temp&0xFFF);

    /* How many locked SPIs, security extension enabled or not, number of
     * actual CPUs and interrupt lines */
    Temp=RME_A7A_GICD_TYPER;
    RME_DBG_S("\r\nA7A-GIC: SPI number: ");
    RME_DBG_I(Temp>>16);
    RME_DBG_S("\r\nA7A-GIC: Security extension: ");
    RME_DBG_I((Temp>>10)&0x1);
    RME_DBG_S("\r\nA7A-GIC: CPU number: ");
    RME_DBG_I(((Temp>>5)&0x7)+1);
    RME_DBG_S("\r\nA7A-GIC: Interrupt line number: ");
    Lines=((Temp&0x1F)+1)*32;
    RME_DBG_I(Lines);


#if(RME_A7A_GIC_TYPE==RME_A7A_GIC_V1)
    /* Initialize all vectors to group 0, and disable all */
    for(Temp=0;Temp<Lines/32;Temp++)
    {
        RME_A7A_GICD_ICPENDR(Temp)=0xFFFFFFFFU;
        RME_A7A_GICD_IGROUPR(Temp)=0x00000000U;
        RME_A7A_GICD_ICENABLER(Temp)=0xFFFFFFFFU;
    }

    /* Set the priority of all such interrupts to the lowest level */
    for(Temp=0;Temp<Lines/4;Temp++)
        RME_A7A_GICD_IPRIORITYR(Temp)=0xA0A0A0A0U;

    /* All interrupts target CPU0 */
    for(Temp=8;Temp<Lines/4;Temp++)
        RME_A7A_GICD_ITARGETSR(Temp)=0x01010101U;

    /* All interrupts are edge triggered, and use 1-N model */
    for(Temp=0;Temp<Lines/16;Temp++)
        RME_A7A_GICD_ICFGR(Temp)=0x55555555U;

    /* Enable the interrupt controller */
    RME_A7A_GICD_CTLR=RME_A7A_GICD_CTLR_GRP1EN|RME_A7A_GICD_CTLR_GRP0EN;
#else
    /* Initialize all vectors to group 1, unless otherwise noted, and disable all */
    for(Temp=0;Temp<Lines/32;Temp++)
    {
        RME_A7A_GICD_ICPENDR(Temp)=0xFFFFFFFFU;
        RME_A7A_GICD_IGROUPR(Temp)=0xFFFFFFFFU;
        RME_A7A_GICD_ICENABLER(Temp)=0xFFFFFFFFU;
    }

    /* Set the priority of all such interrupts to the lowest level*/
    for(Temp=0;Temp<Lines/4;Temp++)
        RME_A7A_GICD_IPRIORITYR(Temp)=0xE0E0E0E0U;

    /* All interrupts target CPU0 */
    for(Temp=8;Temp<Lines/4;Temp++)
        RME_A7A_GICD_ITARGETSR(Temp)=0x01010101U;

    /* All interrupts are edge triggered, and use 1-N model */
    for(Temp=0;Temp<Lines/16;Temp++)
        RME_A7A_GICD_ICFGR(Temp)=0xFFFFFFFFU;

    /* Enable the interrupt controller */
    RME_A7A_GICD_CTLR=RME_A7A_GICD_CTLR_GRP1EN|RME_A7A_GICD_CTLR_GRP0EN;
#endif
}
/* End Function:__RME_A7A_Int_Init ******************************************/

static void RME_L2CacheSync(void)
{
#ifdef CONFIG_PL310_ERRATA_753970
    RME_Out32(RME_L2CC_BASEADDR + RME_L2CC_DUMMY_CACHE_SYNC_OFFSET, 0x0U);
#else
    RME_Out32(RME_L2CC_BASEADDR + RME_L2CC_CACHE_SYNC_OFFSET, 0x0U);
#endif
}

/* Function: __RME_L2CacheInvalidate *****************************************
Description : Invalidate the L2 cache. If the byte specified by the address (adr)
              is cached by the Data cache, the cacheline containing that byte is
              invalidated.    If the cacheline is modified (dirty), the modified contents
              are lost and are NOT written to system memory before the line is
              invalidated.The bottom 4 bits are set to 0, forced by architecture.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_L2CacheInvalidate(void)
{
     rme_ptr_t ResultDCache;
    /* Invalidate the caches */
    RME_Out32(RME_L2CC_BASEADDR + RME_L2CC_CACHE_INVLD_WAY_OFFSET,
          0x0000FFFFU);
    ResultDCache = RME_A7A_REG(RME_L2CC_BASEADDR + RME_L2CC_CACHE_INVLD_WAY_OFFSET)
                            & 0x0000FFFFU;
    while(ResultDCache != (rme_ptr_t)0U) {
        ResultDCache = RME_A7A_REG(RME_L2CC_BASEADDR + RME_L2CC_CACHE_INVLD_WAY_OFFSET)
                            & 0x0000FFFFU;
    }

    /* Wait for the invalidate to complete */
    RME_L2CacheSync();

    /* synchronize the processor */
    dsb();
}

/* End Function:__RME_L2CacheInvalidate ******************************************/
/* Function: __RME_L2CacheEnable *****************************************
Description : Initialize the local CPU interface of the processor.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_L2CacheEnable(void)
{
    rme_ptr_t L2CCReg;

    L2CCReg=RME_A7A_REG(RME_L2CC_BASEADDR + RME_L2CC_CNTRL_OFFSET);
    /* check the L2CC, only enable if L2CC is currently disabled */
        if ((L2CCReg & 0x01U) == 0U)
        {
            L2CCReg = RME_A7A_REG(RME_L2CC_BASEADDR +
                       RME_L2CC_AUX_CNTRL_OFFSET);
            L2CCReg &= RME_L2CC_AUX_REG_ZERO_MASK;
            L2CCReg |= RME_L2CC_AUX_REG_DEFAULT_MASK;
            RME_Out32(RME_L2CC_BASEADDR + RME_L2CC_AUX_CNTRL_OFFSET,
                  L2CCReg);
            RME_Out32(RME_L2CC_BASEADDR + RME_L2CC_TAG_RAM_CNTRL_OFFSET,
                  RME_L2CC_TAG_RAM_DEFAULT_MASK);
            RME_Out32(RME_L2CC_BASEADDR + RME_L2CC_DATA_RAM_CNTRL_OFFSET,
                  RME_L2CC_DATA_RAM_DEFAULT_MASK);
            /* Clear the pending interrupts */
            L2CCReg = RME_A7A_REG(RME_L2CC_BASEADDR +
                       RME_L2CC_ISR_OFFSET);
            RME_Out32(RME_L2CC_BASEADDR + RME_L2CC_IAR_OFFSET, L2CCReg);

            __RME_L2CacheInvalidate();
            /* Enable the L2CC */
            L2CCReg = RME_A7A_REG(RME_L2CC_BASEADDR +
                       RME_L2CC_CNTRL_OFFSET);
            RME_Out32(RME_L2CC_BASEADDR + RME_L2CC_CNTRL_OFFSET,
                  (L2CCReg | (0x01U)));

            RME_L2CacheSync();

        }
}
/* End Function:__RME_L2CacheEnable ******************************************/

/* Function:__RME_A7A_Int_Local_Init *****************************************
Description : Initialize the local CPU interface of the processor.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_A7A_Int_Local_Init(void)
{
    /* Priority grouping */
    RME_A7A_GICC_BPR=RME_A7A_GIC_GROUPING;

#if(RME_A7A_GIC_TYPE==RME_A7A_GIC_V1)
    /* Enable all interrupts to this interface - FIQ is bypassed, and all
     * interrupts go through the IRQ. The FIQ feature is only available on
     * the standalone FIQ interrupt line */
    RME_A7A_GICC_CTLR=RME_A7A_GICC_ENABLEGRP0;
#else
    RME_A7A_GICC_CTLR=RME_A7A_GICC_CBPR|RME_A7A_GICC_FIQEN|
                       RME_A7A_GICC_ENABLEGRP1|RME_A7A_GICC_ENABLEGRP0;
#endif

    /* No interrupts are masked - This must be set at last because enabling
     * will trash the contents of this register if previously set. To maintain
     * compatibility across all possible implementations, no priority level
     * lower than 0xF0 will be considered valid */
    RME_A7A_GICC_PMR=0xF0U;
}
/* End Function:__RME_A7A_Int_Local_Init ************************************/

/* Function:__RME_A7A_Timer_Init *********************************************
Description : Initialize the interrupt controller of the Cortex-A ARMv7 platform.
              This will only initialize the distributor and will only be run by
              the first CPU. This will initialize the timer as well.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_A7A_Timer_Init(void)
{
    RME_DBG_S("\r\ntimer init begin");
#if((RME_A7A_CPU_TYPE==RME_A7A_CPU_CORTEX_A5)|| \
    (RME_A7A_CPU_TYPE==RME_A7A_CPU_CORTEX_A9))
    /* Writing this will also write the counter register as well */
    RME_A7A_PTWD_PTLR=RME_A7A_SYSTICK_VAL;
    /* Clear the interrupt flag */
    RME_A7A_PTWD_PTISR=0;
     //Start the timer
    /*RME_A7A_PTWD_PTCTLR=RME_A7A_PTWD_PTCTLR_PRESC(0)|
                         RME_A7A_PTWD_PTCTLR_IRQEN|
                         RME_A7A_PTWD_PTCTLR_AUTOREL|
                         RME_A7A_PTWD_PTCTLR_TIMEN;*/
//RME_A7A_PTWD_PTLR
    /* Enable the timer interrupt in the GIC */
    RME_A7A_GICD_ISENABLER(0)|=1<<29;
#else
    #error Cortex-A7/8/15/17 is not supported at the moment.
    Cortex-A7/15/17 use the new generic timer, and Cortex-A8 does not
    have a processor timer due to very early release dates.
#endif

    // init global timer here
    // global timer free-running
    // 0-0xFFFFFFFF
    RME_A7A_GTWD_GTCR0=RME_A7A_SYSTICK_VAL;
    RME_A7A_GTWD_GTCTLR=RME_A7A_GTWD_GTCTLR_TIMEN;


    RME_DBG_S("\r\ntimer init finish");

}
/* End Function:__RME_A7A_Timer_Init ****************************************/

/* Function:__RME_A7A_Feature_Get ********************************************
Description : Use the CPUID instruction extensively to get all the processor
              information. We assume that all processors installed have the same
              features.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_A7A_Feature_Get(void)
{

    /* TODO: Check these flags. If not satisfied, we hang immediately. */
}
/* End Function:__RME_A7A_Feature_Get ***************************************/

/* Function:__RME_A7A_Mem_Init ***********************************************
Description : Initialize the memory map, and get the size of kernel object
              allocation registration table(Kot) and page table reference
              count registration table(Pgreg).
Input       : rme_ptr_t MMap_Addr - The GRUB multiboot memory map data address.
              rme_ptr_t MMap_Length - The GRUB multiboot memory map data length.
Output      : None.
Return      : None.
******************************************************************************/
const rme_ptr_t RME_A7A_Mem_Info[RME_A7A_MEM_ENTRIES]={RME_A7A_MEM_CONTENTS};
void __RME_A7A_Mem_Init(rme_ptr_t MMap_Addr, rme_ptr_t MMap_Length)
{

}
/* End Function:__RME_A7A_Mem_Init ******************************************/

/* Function:__RME_A7A_SMP_Init ***********************************************
Description : Start all other processors, one by one.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_A7A_SMP_Init(void)
{

}
/* End Function:__RME_A7A_SMP_Init ******************************************/

/* Function:__RME_A7A_SMP_Tick ***********************************************
Description : Send IPI to all other cores,to run their handler on the time.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_A7A_SMP_Tick(void)
{

}
/* End Function:__RME_A7A_SMP_Tick *******************************************/

/* Function:__RME_Lowlvl_Init **********************************************
Description : Initialize the low-level hardware. This assumes that we are already
              in Supervisor (SVC) mode.
Input       : None.
Output      : None.
Return      : rme_ptr_t - Always 0.
******************************************************************************/
void __RME_Lowlvl_Init(void)
{
    /* Initialize hardware */
    RME_A7A_LOW_LEVEL_INIT();

    /* Initialize our own CPU-local data structure */
    _RME_CPU_Local_Init(&RME_A7A_Local,0);

    /* Initialize the interrupt controller */
    __RME_A7A_Int_Init();

    /* Initialize CPU-local interrupt resources */
    __RME_A7A_Int_Local_Init();

    /* Initialize the vector table */
    RME_DBG_S("\r\nA7A-Vector: 0x");
    RME_DBG_H((rme_ptr_t)&__RME_A7A_Vector_Table);
    __RME_A7A_VBAR_Set((rme_ptr_t)&__RME_A7A_Vector_Table);

    RME_DBG_S("\r\nA7A-Non-Secure: ");
    RME_DBG_H(__RME_A7A_SCR_Get());
}
/* End Function:__RME_Lowlvl_Init *****************************************/

/* Function:__RME_Pgt_Kom_Init *********************************************
Description : Initialize the kernel mapping tables, so it can be added to all the
              top-level page tables. Currently this have no consideration for >1TB
              RAM, and is not NUMA-aware.
Input       : None.
Output      : None.
Return      : rme_ptr_t - If successful, 0; else RME_ERR_HAL_FAIL.
******************************************************************************/
rme_ptr_t __RME_Pgt_Kom_Init(void)
{
    /* Use initial page table here instead for kernel, which is sufficient */

    return 0;
}
/* End Function:__RME_Pgt_Kom_Init ****************************************/

/* Function:__RME_SMP_Low_Level_Init ******************************************
Description : Low-level initialization for all other cores.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
rme_ptr_t __RME_SMP_Low_Level_Init(void)
{
    /* Initialize hardware */
    RME_A7A_SMP_LOW_LEVEL_INIT();
    /* Initialize our own CPU-local variables */
   // _RME_CPU_Local_Init(&RME_A7A_Local[RME_A7A_CPU_Cnt],RME_A7A_CPU_Cnt);
    return 0;
}
/* End Function:__RME_SMP_Low_Level_Init *************************************/

/* Function:__RME_Boot ********************************************************
Description : Boot the first process in the system.
Input       : None.
Output      : None.
Return      : rme_ptr_t - Always 0.
******************************************************************************/
void __RME_Boot(void)
{
    rme_ptr_t Count;
    rme_ptr_t Cur_Addr;

    Cur_Addr=RME_KOM_VA_BASE;

    /* Create the capability table for the init process */
    RME_DBG_S("\r\nCreating boot-time capability table @ ");
    RME_DBG_H(Cur_Addr);
    RME_ASSERT(_RME_Cpt_Boot_Init(RME_BOOT_INIT_CPT,
                                  Cur_Addr,
                                  RME_A7A_INIT_CPT_SIZE)==0);
    Cur_Addr+=RME_KOM_ROUND(RME_CPT_SIZE(RME_A7A_INIT_CPT_SIZE));

    /* The top-level page table - covers 1M address range - align to 16kB */
    Cur_Addr=RME_ROUND_UP(Cur_Addr,16U);
    RME_DBG_S("\r\nCreating boot-time page table @ ");
    RME_DBG_H(Cur_Addr);
    RME_ASSERT(_RME_Pgt_Boot_Crt(RME_A7A_CPT,
                                 RME_BOOT_INIT_CPT,
                                 RME_BOOT_INIT_PGT,
                                 Cur_Addr,
                                 0x00000000U,
                                 RME_PGT_TOP,
                                 RME_PGT_SIZE_1M,
                                 RME_PGT_NUM_4K)==0);

    /* Normal memory, 1GiB 0x00000000 -> 0x00000000 */
    for(Count=0U;Count<0x400U;Count++)
    {
        RME_ASSERT(_RME_Pgt_Boot_Add(RME_A7A_CPT,
                                     RME_BOOT_INIT_PGT,
                                     Count*RME_POW2(RME_PGT_SIZE_1M),
                                     Count,
                                     RME_PGT_ALL_DYN)==0);
              /*RME_DBG_S("\r\npaddr=");
                RME_DBG_H((Count)*RME_POW2(RME_PGT_SIZE_1M));
                RME_DBG_S("\r\nvaddr=");
                RME_DBG_H((Count)*RME_POW2(RME_PGT_SIZE_1M));*/
    }
    /* Device memory 1, 512MiB 0x40000000 -> 0x40000000 */
    for(Count=0U;Count<0x200U;Count++)
    {
        RME_ASSERT(_RME_Pgt_Boot_Add(RME_A7A_CPT,
                                     RME_BOOT_INIT_PGT,
                                     (Count+0x400U)*RME_POW2(RME_PGT_SIZE_1M),
                                     (Count+0x400U),
                                     RME_PGT_READ|RME_PGT_WRITE)==0);
                 /*RME_DBG_S("\r\npaddr=");
                RME_DBG_H((Count+0x400U)*RME_POW2(RME_PGT_SIZE_1M));
                RME_DBG_S("\r\nvaddr=");
                RME_DBG_H((Count+0x400U)*RME_POW2(RME_PGT_SIZE_1M));*/
    }
    
    /* Device memory 2, 512MiB 0x60000000 -> 0xE0000000 */
    for(Count=0U;Count<0x200U;Count++)
    {
        RME_ASSERT(_RME_Pgt_Boot_Add(RME_A7A_CPT,
                                     RME_BOOT_INIT_PGT,
                                     (Count+0xE00U)*RME_POW2(RME_PGT_SIZE_1M),
                                     (Count+0x600U),
                                     RME_PGT_READ|RME_PGT_WRITE)==0);
              /*RME_DBG_S("\r\npaddr=");
                RME_DBG_H((Count+0xE00U)*RME_POW2(RME_PGT_SIZE_1M));
                RME_DBG_S("\r\nvaddr=");
                RME_DBG_H((Count+0x600U)*RME_POW2(RME_PGT_SIZE_1M));*/
    }

    RME_DBG_S("\r\nFirst section's first entry ");
    RME_DBG_H(RME_A7A_REG(Cur_Addr));
    RME_DBG_S(" @ ");
    RME_DBG_H(Cur_Addr);

    RME_DBG_S("\r\nFirst section's 0x080th entry ");
    RME_DBG_H(RME_A7A_REG(Cur_Addr+0x080*RME_WORD_BYTE));
    RME_DBG_S(" @ ");
    RME_DBG_H(Cur_Addr+0x080*RME_WORD_BYTE);

    RME_DBG_S("\r\nSecond section's first entry ");
    RME_DBG_H(RME_A7A_REG(Cur_Addr+0x400*RME_WORD_BYTE));
    RME_DBG_S(" @ ");
    RME_DBG_H(Cur_Addr+0x400*RME_WORD_BYTE);

    RME_DBG_S("\r\nThird section's first entry ");
    RME_DBG_H(RME_A7A_REG(Cur_Addr+0x600*RME_WORD_BYTE));
    RME_DBG_S(" @ ");
    RME_DBG_H(Cur_Addr+0x600*RME_WORD_BYTE);

    RME_DBG_S("\r\nKernel pgtbl's 80th entry ");
    RME_DBG_H(RME_A7A_REG(((rme_ptr_t)(&__RME_A7A_Kern_Pgt))+0x080*RME_WORD_BYTE));
    RME_DBG_S(" @ ");
    RME_DBG_H(((rme_ptr_t)(&__RME_A7A_Kern_Pgt))+0x080*RME_WORD_BYTE);

    Cur_Addr+=RME_KOM_ROUND(RME_PGT_SIZE_TOP(RME_PGT_NUM_4K));

    /* Activate the first process - This process cannot be deleted */
    RME_DBG_S("\r\nstart prc creation.");
    RME_ASSERT(_RME_Prc_Boot_Crt(RME_A7A_CPT,
                                 RME_BOOT_INIT_CPT,
                                 RME_BOOT_INIT_PRC,
                                 RME_BOOT_INIT_CPT,
                                 RME_BOOT_INIT_PGT)==0U);

    /* Activate the first thread, and set its priority */
    RME_DBG_S("\r\nCreating boot-time thread @ ");
    RME_DBG_H(Cur_Addr);
    RME_ASSERT(_RME_Thd_Boot_Crt(RME_A7A_CPT,
                                 RME_BOOT_INIT_CPT,
                                 RME_BOOT_INIT_THD,
                                 RME_BOOT_INIT_PRC,
                                 Cur_Addr,
                                 0U,
                                 &RME_A7A_Local)==0);
    Cur_Addr+=RME_KOM_ROUND(RME_THD_SIZE(0U));
    RME_DBG_S("\r\nFinish creating boot-time thread @ ");
    RME_DBG_H(Cur_Addr);
    /* Create the initial kernel function capability, and kernel memory capability */
    RME_DBG_S("\r\nCreating boot-time kernel function");
    RME_ASSERT(_RME_Kfn_Boot_Crt(RME_A7A_CPT,
                                 RME_BOOT_INIT_CPT,
                                 RME_BOOT_INIT_KFN)==0);
    RME_DBG_S("\r\nCreating boot-time kernel memory");
    RME_ASSERT(_RME_Kom_Boot_Crt(RME_A7A_CPT, 
                                 RME_BOOT_INIT_CPT, 
                                 RME_BOOT_INIT_KOM,
                                 RME_KOM_VA_BASE,
                                 RME_KOM_VA_BASE+RME_KOM_VA_SIZE-1U,
                                 RME_KOM_FLAG_ALL)==0U);
    
    /* Create the initial kernel endpoint for timer ticks and interrupts */
    RME_A7A_Local.Sig_Tim=(struct RME_Cap_Sig*)&(RME_A7A_CPT[RME_BOOT_INIT_VCT]);
    RME_A7A_Local.Sig_Vct=(struct RME_Cap_Sig*)&(RME_A7A_CPT[RME_BOOT_INIT_VCT]);
    RME_DBG_S("\r\nCreate the initial kernel endpoint for timer ticks and interrupts");
    RME_ASSERT(_RME_Sig_Boot_Crt(RME_A7A_CPT,
                                 RME_BOOT_INIT_CPT,
                                 RME_BOOT_INIT_VCT)==0);
    
    /* Set page table as current */
    __RME_Pgt_Set(RME_A7A_Local.Thd_Cur->Sched.Prc->Pgt);

    /* Initialize timer and enable interrupts */
    RME_DBG_S("\r\nenable interrupts\r\n");
    __RME_A7A_Timer_Init();
    __RME_Int_Enable();

    /* enable l2 cache */
    __RME_L2CacheEnable();
    RME_DBG_S("\r\nenable l2 cache\r\n");
    /* Boot into the init thread */
    __RME_User_Enter(RME_A7A_INIT_ENTRY,RME_A7A_INIT_STACK,0U);
    
    /* Should never reach here */
    while(1);
}
/* End Function:__RME_Boot ***************************************************/

/* Function:__RME_Reboot ******************************************************
Description : Reboot the machine, abandon all operating system states.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_Reboot(void)
{
    //RME_ASSERT(RME_WORD_BITS!=RME_POW2(RME_WORD_ORDER));
}
/* End Function:__RME_Reboot *************************************************/

/* Function:__RME_Shutdown ****************************************************
Description : Shutdown the machine, abandon all operating system states.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_Shutdown(void)
{
   // RME_ASSERT(RME_WORD_BITS!=RME_POW2(RME_WORD_ORDER));
}
/* End Function:__RME_Shutdown ***********************************************/

/* Function:__RME_Get_Syscall_Param *******************************************
Description : Get the system call parameters from the stack frame.
Input       : struct RME_Reg_Struct* Reg - The register set.
Output      : rme_ptr_t* Svc - The system service number.
              rme_ptr_t* Capid - The capability ID number.
              rme_ptr_t* Param - The parameters.
Return      : None.
******************************************************************************/
void __RME_Svc_Param_Get(struct RME_Reg_Struct* Reg,rme_ptr_t* Svc,rme_ptr_t* Capid,rme_ptr_t* Param)
{
    *Svc=(Reg->R0)>>16;
    *Capid=(Reg->R0)&0xFFFF;
    Param[0]=Reg->R1;
    Param[1]=Reg->R2;
    Param[2]=Reg->R3;
}
/* End Function:__RME_Get_Syscall_Param **************************************/

/* Function:__RME_Set_Syscall_Retval ******************************************
Description : Set the system call return value to the stack frame. This function 
              may carry up to 4 return values. If the last 3 is not needed, just set
              them to zero.
Input       : rme_ret_t Retval - The return value.
Output      : struct RME_Reg_Struct* Reg - The register set.
Return      : None.
******************************************************************************/
void __RME_Svc_Retval_Set(struct RME_Reg_Struct* Reg,rme_ret_t Retval)
{
    Reg->R0=(rme_ptr_t)Retval;
}
/* End Function:__RME_Set_Syscall_Retval *************************************/



/* Function:__RME_Thd_Reg_Init ************************************************
Description : Initialize the register set for the thread.
Input       : rme_ptr_t Entry - The thread entry address.
              rme_ptr_t Stack - The thread stack address.
              rme_ptr_t Param - The parameter to pass to it.
Output      : struct RME_Reg_Struct* Reg - The register set content generated.
Return      : None.
******************************************************************************/
void __RME_Thd_Reg_Init(rme_ptr_t Attr,rme_ptr_t Entry,rme_ptr_t Stack,rme_ptr_t Param,
                                       struct RME_Reg_Struct* Reg)
{
    /* Set the entry and stack */
    Reg->PC=Entry;
    Reg->SP=Stack;
    /* LR needs to be a multiple of 4 upon return, ARMv7-A does not allow
     * unaligned LR upon exception entry. Setting it to PC for convenience. */
    Reg->LR=Entry;
    /* Set the parameter */
    Reg->R0=Param;
    Reg->R1=1;
    Reg->R2=2;
    Reg->R3=3;
    Reg->R4=4;
    Reg->R5=5;
    Reg->R6=6;
    Reg->R7=7;
    Reg->R8=8;
    Reg->R9=9;
    Reg->R10=10;
    Reg->R11=11;
    Reg->R12=12;
    /* Initialize CPSR */
    Reg->CPSR=0x00000010U;
}
/* End Function:__RME_Thd_Reg_Init *******************************************/

/* Function:__RME_Thd_Reg_Copy ************************************************
Description : Copy one set of registers into another.
Input       : struct RME_Reg_Struct* Src - The source register set.
Output      : struct RME_Reg_Struct* Dst - The destination register set.
Return      : None.
******************************************************************************/
void __RME_Thd_Reg_Copy(struct RME_Reg_Struct* Dst, struct RME_Reg_Struct* Src)
{
    /* Make sure that the ordering is the same so the compiler can optimize */
    Dst->CPSR=Src->CPSR;
    Dst->R0=Src->R0;
    Dst->R1=Src->R1;
    Dst->R2=Src->R2;
    Dst->R3=Src->R3;
    Dst->R4=Src->R4;
    Dst->R5=Src->R5;
    Dst->R6=Src->R6;
    Dst->R7=Src->R7;
    Dst->R8=Src->R8;
    Dst->R9=Src->R9;
    Dst->R10=Src->R10;
    Dst->R11=Src->R11;
    Dst->R12=Src->R12;
    Dst->SP=Src->SP;
    Dst->LR=Src->LR;
    Dst->PC=Src->PC;
}
/* End Function:__RME_Thd_Reg_Copy *******************************************/

/* Function:__RME_Thd_Reg_Print ***********************************************
Description : Print thread registers. This is used exclusively for debugging.
Input       : struct RME_Reg_Struct* Reg - The register set.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_Thd_Reg_Print(struct RME_Reg_Struct* Reg)
{
    RME_DBG_SHS("R4: 0x",Reg->R4,"\r\n");

    RME_DBG_SHS("R5: 0x",Reg->R5,"\r\n");
    RME_DBG_SHS("R6: 0x",Reg->R6,"\r\n");
    RME_DBG_SHS("R7: 0x",Reg->R7,"\r\n");
    RME_DBG_SHS("R8: 0x",Reg->R8,"\r\n");
    RME_DBG_SHS("R9: 0x",Reg->R9,"\r\n");
    RME_DBG_SHS("R10: 0x",Reg->R10,"\r\n");
    RME_DBG_SHS("R11: 0x",Reg->R11,"\r\n");
    RME_DBG_SHS("LR: 0x",Reg->LR,"\r\n");
}
/* End Function:__RME_Thd_Reg_Print ******************************************/

/* Function:__RME_Thd_Cop_Init ************************************************
Description : Initialize the coprocessor register set for the thread.
Input       : struct RME_Reg_Struct* Reg - The register struct to help initialize the coprocessor.
Output      : struct RME_Reg_Cop_Struct* Cop_Reg - The register set content generated.
Return      : None.
******************************************************************************/
void __RME_Thd_Cop_Init(struct RME_Reg_Struct* Reg, struct RME_Cop_Struct* Cop_Reg)
{
    /* Empty function, return immediately. The FPU contents is not predictable */
}
/* End Function:__RME_Thd_Cop_Reg_Init ***************************************/

/* Function:__RME_Thd_Cop_Save ************************************************
Description : Save the co-op register sets. This operation is flexible - If the
              program does not use the FPU, we do not save its context.
Input       : struct RME_Reg_Struct* Reg - The context, used to decide whether
                                           to save the context of the coprocessor.
Output      : struct RME_Cop_Struct* Cop_Reg - The pointer to the coprocessor contents.
Return      : None.
******************************************************************************/
void __RME_Thd_Cop_Save(struct RME_Reg_Struct* Reg, struct RME_Cop_Struct* Cop_Reg)
{
    /* Not used for now */
}
/* End Function:__RME_Thd_Cop_Save *******************************************/

/* Function:__RME_Thd_Cop_Restore *********************************************
Description : Restore the co-op register sets. This operation is flexible - If the
              FPU is not used, we do not restore its context.
Input       : struct RME_Reg_Struct* Reg - The context, used to decide whether
                                           to save the context of the coprocessor.
Output      : struct RME_Cop_Struct* Cop_Reg - The pointer to the coprocessor contents.
Return      : None.
******************************************************************************/
void __RME_Thd_Cop_Restore(struct RME_Reg_Struct* Reg, struct RME_Cop_Struct* Cop_Reg)
{
    /* Not used for now */
}
/* End Function:__RME_Thd_Cop_Restore ****************************************/

/* Function:__RME_Inv_Reg_Save ************************************************
Description : Save the necessary registers on invocation for returning. Only the
              registers that will influence program control flow will be saved.
Input       : struct RME_Reg_Struct* Reg - The register set.
Output      : struct RME_Iret_Struct* Ret - The invocation return register context.
Return      : None.
******************************************************************************/
void __RME_Inv_Reg_Save(struct RME_Iret_Struct* Ret, struct RME_Reg_Struct* Reg)
{
    Ret->PC=Reg->PC;
    Ret->SP=Reg->SP;
}
/* End Function:__RME_Inv_Reg_Save *******************************************/

/* Function:__RME_Inv_Reg_Restore *********************************************
Description : Restore the necessary registers for returning from an invocation.
Input       : struct RME_Iret_Struct* Ret - The invocation return register context.
Output      : struct RME_Reg_Struct* Reg - The register set.
Return      : None.
******************************************************************************/
void __RME_Inv_Reg_Restore(struct RME_Reg_Struct* Reg, struct RME_Iret_Struct* Ret)
{
    Reg->PC=Ret->PC;
    Reg->SP=Ret->SP;
}
/* End Function:__RME_Inv_Reg_Restore ****************************************/

/* Function:__RME_Inv_Retval_Set **********************************************
Description : Set the invocation return value to the stack frame.
Input       : rme_ret_t Retval - The return value.
Output      : struct RME_Reg_Struct* Reg - The register set.
Return      : None.
******************************************************************************/
void __RME_Inv_Retval_Set(struct RME_Reg_Struct* Reg,
                          rme_ret_t Retval)
{
    Reg->R1=(rme_ptr_t)Retval;
}
/* End Function:__RME_Inv_Retval_Set *****************************************/

/* Function:__RME_Thd_Cop_Swap ************************************************
Description : Swap the cop register sets. This operation is flexible - If the
              program does not use the FPU, we do not save/restore its context.
              We do not need to turn off lazy stacking, because even if a fault
              occurs, it (1) tails the execution of activates the will get dropped
              by our handler deliberately and will not cause wrong attribution.
              They can be alternatively disabled as well if you wish.
Input       : rme_ptr_t Attr_New - The attribute of the context to switch to.
              rme_ptr_t Is_Hyp_New - Whether the context to switch to is a
                                     hypervisor dedicated one.
              struct RME_Reg_Struct* Reg_New - The context to switch to.
              rme_ptr_t Attr_Cur - The attribute of the context to switch from.
              rme_ptr_t Is_Hyp_Cur - Whether the context to switch from is a
                                     hypervisor dedicated one.
              struct RME_Reg_Struct* Reg_Cur - The context to switch from.
Output      : void* Cop_New - The coprocessor context to switch to.
              void* Cop_Cur - The coprocessor context to switch from.
Return      : None.
******************************************************************************/
void __RME_Thd_Cop_Swap(rme_ptr_t Attr_New,
                        rme_ptr_t Is_Hyp_New,
                        struct RME_Reg_Struct* Reg_New,
                        void* Cop_New,
                        rme_ptr_t Attr_Cur,
                        rme_ptr_t Is_Hyp_Cur,
                        struct RME_Reg_Struct* Reg_Cur,
                        void* Cop_Cur)
{
    
}
/* End Function:__RME_Thd_Cop_Swap *******************************************/

/* Function:__RME_Kern_Func_Handler *******************************************
Description : Handle kernel function calls.
Input       : struct RME_Reg_Struct* Reg - The current register set.
              rme_ptr_t Func_ID - The function ID.
              rme_ptr_t Sub_ID - The sub function ID.
              rme_ptr_t Param1 - The first parameter.
              rme_ptr_t Param2 - The second parameter.
Output      : None.
Return      : rme_ptr_t - The value that the function returned.
******************************************************************************/
rme_ret_t __RME_Kfn_Handler(struct RME_Cap_Cpt* Cpt,
                                           struct RME_Reg_Struct* Reg,
                                           rme_ptr_t Func_ID,
                                           rme_ptr_t Sub_ID,
                                           rme_ptr_t Param1,
                                           rme_ptr_t Param2)
{
    /* Currently no kernel function implemented */
    return 0;
}
/* End Function:__RME_Kern_Func_Handler **************************************/

/* Function:__RME_A7A_Undefined_Handler **************************************
Description : The undefined instruction vector handler of RME.
Input       : struct RME_Reg_Struct* Reg - The register set when entering the handler.
Output      : struct RME_Reg_Struct* Reg - The register set when exiting the handler.
Return      : None.
******************************************************************************/
void __RME_A7A_Undefined_Handler(struct RME_Reg_Struct* Reg)
{
    /* We don't handle undefined instructions now */
    rme_ptr_t end;
    end=RME_A7A_REG(&RME_A7A_GTWD_GTCR0);
    RME_DBG_S(" \r\ntime ");
    RME_DBG_H(end);
    RME_DBG_S("\r\nUndefined_Handler");
    RME_DBG_S(" PC - ");
    RME_DBG_H(Reg->PC);
    RME_DBG_S(" SP - ");
    RME_DBG_H(Reg->SP);
    RME_DBG_S(" CPSR - ");
    RME_DBG_H(Reg->CPSR);
    while(1);
}
/* End Function:__RME_A7A_Undefined_Handler *********************************/

/* Function:__RME_A7A_Prefetch_Abort_Handler *********************************
Description : The prefetch abort vector handler of RME.
Input       : struct RME_Reg_Struct* Reg - The register set when entering the handler.
Output      : struct RME_Reg_Struct* Reg - The register set when exiting the handler.
Return      : None.
******************************************************************************/
void __RME_A7A_Prefetch_Abort_Handler(struct RME_Reg_Struct* Reg)
{
    /* We don't handle prefetch aborts now */
    RME_DBG_S("Prefetch_Abort_Handler");
    RME_DBG_S(" PC - ");
    RME_DBG_H(Reg->PC);
    RME_DBG_S(" SP - ");
    RME_DBG_H(Reg->SP);
    RME_DBG_S(" CPSR - ");
    RME_DBG_H(Reg->CPSR);
    while(1);
}
/* End Function:__RME_A7A_Prefetch_Abort_Handler ****************************/

/* Function:__RME_A7A_Data_Abort_Handler *************************************
Description : The data abort vector handler of RME.
Input       : struct RME_Reg_Struct* Reg - The register set when entering the handler.
Output      : struct RME_Reg_Struct* Reg - The register set when exiting the handler.
Return      : None.
******************************************************************************/
void __RME_A7A_Data_Abort_Handler(struct RME_Reg_Struct* Reg)
{
    /* We don't handle data aborts now */
    RME_DBG_S("\r\nData_Abort_Handler");
    RME_DBG_S(" PC - ");
    RME_DBG_H(Reg->PC);
    RME_DBG_S(" SP - ");
    RME_DBG_H(Reg->SP);
    RME_DBG_S(" CPSR - ");
    RME_DBG_H(Reg->CPSR);
    while(1);
}
/* End Function:__RME_A7A_Data_Abort_Handler ********************************/

/* Function:__RME_A7A_IRQ_Handler ********************************************
Description : The regular interrupt vector handler of RME.
Input       : struct RME_Reg_Struct* Reg - The register set when entering the handler.
Output      : struct RME_Reg_Struct* Reg - The register set when exiting the handler.
Return      : None.
******************************************************************************/
void __RME_A7A_IRQ_Handler(struct RME_Reg_Struct* Reg)
{
    rme_ptr_t Int_ID;
    rme_ptr_t CPUID;

    /* What interrupt is this? */
    Int_ID=RME_A7A_GICC_IAR;
    CPUID=Int_ID>>10;
    Int_ID&=0x3FFU;
    //RME_DBG_S("\r\nINT_ID is ");
    //RME_DBG_I(Int_ID);

#if(RME_A7A_GIC_TYPE==RME_A7A_GIC_V1)
    /* Is this a spurious interrupt? (Can't be 1022 because GICv1 don't have group1) */
    RME_ASSERT(Int_ID!=1022);
    if(Int_ID==1023)
        return;
    /* Only the booting processor will receive timer interrupts */
#if((RME_A7A_CPU_TYPE==RME_A7A_CPU_CORTEX_A5)|| \
    (RME_A7A_CPU_TYPE==RME_A7A_CPU_CORTEX_A9))
    /* Is is an timer interrupt? (we know that it is at 29) */
    if(Int_ID==29)
    {
        /* Clear the interrupt flag */
        RME_A7A_PTWD_PTISR=0;
        //_RME_Tick_Handler(Reg);
        _RME_Tim_Handler(Reg,1);
        /* Send interrupt to all other processors to notify them about this */
        /* EOI the interrupt */
        RME_A7A_GICC_EOIR=Int_ID;
        return;
    }
#else

#endif

#else

#endif
    /* Is this a coprocessor timer interrupt? (We use interrupt number 0 for these) */
    if(Int_ID==0)
    {
        /* This must have originated from interface 0 */
        RME_ASSERT(CPUID==0);
        //_RME_Tick_SMP_Handler(Reg);
        /* EOI the interrupt */
        RME_A7A_GICC_EOIR=Int_ID;
        return;
    }

    /* Is this an other IPI? (All the rest of the SGIs are these) */
    if(Int_ID<16)
    {
        _RME_A7A_SGI_Handler(Reg,CPUID,Int_ID);
        /* EOI the interrupt */
        RME_A7A_GICC_EOIR=(CPUID<<10U)|Int_ID;
        return;
    }

    /* Is this an casual interrupt? */
    RME_ASSERT(CPUID==0);

}
/* End Function:__RME_A7A_IRQ_Handler ***************************************/

/* Function:_RME_A7A_SGI_Handler *********************************************
Description : The generic interrupt handler of RME for x64.
Input       : struct RME_Reg_Struct* Reg - The register set when entering the handler.
              rme_ptr_t CPUID - The ID of the sender CPU.
              rme_ptr_t Int_ID - The ID of the SGI.
Output      : struct RME_Reg_Struct* Reg - The register set when exiting the handler.
Return      : None.
******************************************************************************/
void _RME_A7A_SGI_Handler(struct RME_Reg_Struct* Reg, rme_ptr_t CPUID, rme_ptr_t Int_ID)
{
    /* Not handling SGIs */
    return;
}
/* End Function:_RME_A7A_SGI_Handler ****************************************/

/* Function:__RME_A7A_Generic_Handler ****************************************
Description : The generic interrupt handler of RME for Cortex-A (ARMv7).
Input       : struct RME_Reg_Struct* Reg - The register set when entering the handler.
              rme_ptr_t Int_Num - The interrupt number.
Output      : struct RME_Reg_Struct* Reg - The register set when exiting the handler.
Return      : None.
******************************************************************************/
void __RME_A7A_Generic_Handler(struct RME_Reg_Struct* Reg, rme_ptr_t Int_Num)
{
    /* Not handling interrupts */
    RME_DBG_S("\r\nGeneral int:");
    RME_DBG_I(Int_Num);

    /* Not handling anything */
    switch(Int_Num)
    {
        default:break;
    }
    /* Remember to perform context switch after any kernel sends */
}
/* End Function:__RME_A7A_Generic_Handler ***********************************/

/* Function:__RME_Pgt_Set ***************************************************
Description : Set the processor's page table.
Input       : rme_ptr_t Pgt - The virtual address of the page table.
Output      : None.
Return      : None.
******************************************************************************/
void __RME_Pgt_Set(struct RME_Cap_Pgt* Pgt)
{
    rme_ptr_t* Ptr;

    /* Get the actual table */
    Ptr=RME_CAP_GETOBJ(Pgt,rme_ptr_t*);

    /*RME_DBG_S("\r\n__RME_Pgt_Set table kernel VA @ ");
    RME_DBG_H(Ptr);*/

    __RME_A7A_TTBR0_Set(RME_A7A_VA2PA(Ptr)|0x4A);
    __RME_A7A_TLBIALL_Set(0);
}
/* End Function:__RME_Pgt_Set **********************************************/

/* Function:__RME_Pgt_Check *************************************************
Description : Check if the page table parameters are feasible, according to the
              parameters. This is only used in page table creation. The following
              restrictions apply:
1. PAE is not supported by this port: if you have that much memory, use the Cortex-A
   64-bit port instead. The translation scheme here is always:
   4GB VA -> {Supersection, Section} -> {Large Page, Small Page}.
   The supersection is described in ARMv7-AR ARM as optional, but is implemented on
   all known ARMv7-A processors.
   We only user TTBR0 here and we set value N to 0 as always. Using TTBR1 is nonsense
   in our context, as it gives little benefit.
2. The ARM page tables have idiosyncrasy that allow two page size mappings to exist
   in the same page table.
3. ARM page tables use repetition when it comes to supersections and large pages.
   This breaks our numorder-sizeorder assumption, and for this reason, we do not
   support these two page sizes explicitly. These two sizes are supported by kernel
   functions instead, and the kernel function always maps/unmaps the pages as needed
   without checking for any permissions; this can be viewed as a hack, and require
   some caution when we are mapping pages in.
Input       : rme_ptr_t Base - The start mapping address.
              rme_ptr_t Is_Top - The top-level flag,
              rme_ptr_t Size_Order - The size order of the page directory.
              rme_ptr_t Num_Order - The number order of the page directory.
              rme_ptr_t Vaddr - The virtual address of the page directory.
Output      : None.
Return      : rme_ptr_t - If successful, 0; else RME_ERR_HAL_FAIL.
******************************************************************************/
rme_ptr_t __RME_Pgt_Check(rme_ptr_t Base, rme_ptr_t Is_Top,
                          rme_ptr_t Size_Order, rme_ptr_t Num_Order, rme_ptr_t Vaddr)
{
    /* Top-level - 1MiB pages, 4096 entries, 16KiB alignment */
    if(Is_Top!=0)
    {
        if(Size_Order!=RME_PGT_SIZE_1M)
            return RME_ERR_HAL_FAIL;
        if(Num_Order!=RME_PGT_NUM_4K)
            return RME_ERR_HAL_FAIL;
        if((Vaddr&0x3FFF)!=0)
        {
            RME_DBG_S("\r\nvaddr alignment error= ");
            RME_DBG_H(Vaddr);
            return RME_ERR_HAL_FAIL;
        }
    }
    /* Second-level - 4KiB pages, 256 entries, 1KiB alignment */
    else
    {
        if(Size_Order!=RME_PGT_SIZE_4K)
            return RME_ERR_HAL_FAIL;
        if(Num_Order!=RME_PGT_NUM_256)
            return RME_ERR_HAL_FAIL;
        if((Vaddr&0x3FF)!=0)
        {
            RME_DBG_S("\r\nvaddr alignment error= ");
            RME_DBG_H(Vaddr);
            return RME_ERR_HAL_FAIL;
        }
    }

    return 0;
}
/* End Function:__RME_Pgt_Check ********************************************/

/* Function:__RME_Pgt_Init **************************************************
Description : Initialize the page table data structure, according to the capability.
Input       : struct RME_Cap_Pgt* - The capability to the page table to operate on.
Output      : None.
Return      : rme_ptr_t - If successful, 0; else RME_ERR_HAL_FAIL.
******************************************************************************/
rme_ptr_t __RME_Pgt_Init(struct RME_Cap_Pgt* Pgt_Op)
{
    rme_cnt_t Count;
    rme_ptr_t* Ptr;

    /* Get the actual table */
    Ptr=RME_CAP_GETOBJ(Pgt_Op,rme_ptr_t*);  //*((volatile rme_ptr_t*)0x12345678)

    /* Is this a first-level or a second-level? */
    if((Pgt_Op->Base&RME_PGT_TOP)!=0)
    {
        /* First-level - clean up the first half and map in the second half as kernel entries */
        for(Count=0;Count<2048;Count++)
            Ptr[Count]=0;

        for(;Count<4096;Count++)
            Ptr[Count]=(&__RME_A7A_Kern_Pgt)[Count];
    }
    else
    {
        /* Second-level - just clean it up to all zeros */
        for(Count=0;Count<256;Count++)
            Ptr[Count]=0;
    }

    return 0;
}
/* End Function:__RME_Pgt_Init *********************************************/

/* Function:__RME_Pgt_Del_Check *********************************************
Description : Check if the page table can be deleted.
Input       : struct RME_Cap_Pgt Pgt_Op* - The capability to the page table to operate on.
Output      : None.
Return      : rme_ptr_t - If can be deleted, 0; else RME_ERR_HAL_FAIL.
******************************************************************************/
rme_ptr_t __RME_Pgt_Del_Check(struct RME_Cap_Pgt* Pgt_Op)
{
    return 0;
}
/* End Function:__RME_Pgt_Del_Check ****************************************/

/* Function:__RME_Pgt_Page_Map **********************************************
Description : Map a page into the page table. This architecture requires that the
              mapping is always at least readable. The Cortex-A's super pages are
              not supported by this. Also, RME dfoesn't use the domain model; all
              page tables' domain is always client, which means that the permission
              flags will always be checked.
Input       : struct RME_Cap_Pgt* - The cap ability to the page table to operate on.
              rme_ptr_t Paddr - The physical address to map to. If we are unmapping,
                                this have no effect.
              rme_ptr_t Pos - The position in the page table.
              rme_ptr_t Flags - The RME standard page attributes. Need to translate
                                them into architecture specific page table's settings.
Output      : None.
Return      : rme_ptr_t - If successful, 0; else RME_ERR_HAL_FAIL.
******************************************************************************/
rme_ptr_t __RME_Pgt_Page_Map(struct RME_Cap_Pgt* Pgt_Op, rme_ptr_t Paddr, rme_ptr_t Pos, rme_ptr_t Flags)
{

    /*RME_DBG_S("\r\nPgt_Op->Order");
    RME_DBG_H(Pgt_Op->Order);
    RME_DBG_S("\r\nPgt_Op->Order ");
    RME_DBG_H(RME_PGT_SZORD(Pgt_Op->Order));
    RME_DBG_S("\r\nPgt_Op->Order ");
    RME_DBG_H(RME_PGT_SZORD(Pgt_Op->Order));
    if(((Pgt_Op->Base)&RME_PGT_TOP)!=0U)
    {
        RME_DBG_S("\r\nhhhhhhhhhhhhhhh ");
    }*/

    rme_ptr_t* Table;
    rme_ptr_t A7A_Flags;

    /* It should at least be readable */
    if((Flags&RME_PGT_READ)==0)
    {
        RME_DBG_S("\r\nunreadable");
        return RME_ERR_HAL_FAIL;
    }

    /* Are we trying to map into the kernel space on the top level? */
    if(((Pgt_Op->Base&RME_PGT_TOP)!=0)&&(Pos>=2048)){
        RME_DBG_S("\r\nAre we trying to map into the kernel space on the top level?");
        return RME_ERR_HAL_FAIL;
    }
    /* Get the table */
    Table=RME_CAP_GETOBJ(Pgt_Op,rme_ptr_t*);

    /* Generate flags */
    //if(RME_PGT_SZORD(Pgt_Op->Order)==RME_PGT_SIZE_4K)
    if(((Pgt_Op->Base)&RME_PGT_TOP)!=0U)
    {
        Flags&=(~RME_PGT_STATIC);
        /*RME_DBG_S("\r\n Flags ");
        RME_DBG_H(Flags); */
        
        A7A_Flags=RME_A7A_MMU_1M_PAGE_ADDR(Paddr)|RME_A7A_PGFLG_1M_RME2NAT(Flags);
        /*RME_DBG_S("\r\n A7A_Flags= ");
        RME_DBG_H(A7A_Flags);*/
    }
    else
        A7A_Flags=RME_A7A_MMU_4K_PAGE_ADDR(Paddr)|RME_A7A_PGFLG_4K_RME2NAT(Flags);
    /* Try to map it in */
    if(RME_COMP_SWAP(&(Table[Pos]),0,A7A_Flags)==0)
    {
        RME_DBG_S("\r\n&Table[Pos] = ");
        RME_DBG_H(&Table[Pos]);
        RME_DBG_S("\r\n*(Table[Pos])");
        RME_DBG_H(Table[Pos]);
        RME_DBG_S("\r\nmap it in error");
        return RME_ERR_HAL_FAIL;
    }

    /*RME_DBG_S("\r\n&Table[Pos] = ");
    RME_DBG_H(&Table[Pos]);
    RME_DBG_S("\r\n*(Table[Pos])");
    RME_DBG_H(Table[Pos]);*/

    return 0;
}
/* End Function:__RME_Pgt_Page_Map *****************************************/

/* Function:__RME_Pgt_Page_Unmap ********************************************
Description : Unmap a page from the page table.
Input       : struct RME_Cap_Pgt* - The capability to the page table to operate on.
              rme_ptr_t Pos - The position in the page table.
Output      : None.
Return      : rme_ptr_t - If successful, 0; else RME_ERR_HAL_FAIL.
******************************************************************************/
rme_ptr_t __RME_Pgt_Page_Unmap(struct RME_Cap_Pgt* Pgt_Op, rme_ptr_t Pos)
{
    rme_ptr_t* Table;
    rme_ptr_t Temp;

    /* Are we trying to unmap the kernel space on the top level? */
    if(((Pgt_Op->Base&RME_PGT_TOP)!=0)&&(Pos>=2048))
        return RME_ERR_HAL_FAIL;

    /* Get the table */
    Table=RME_CAP_GETOBJ(Pgt_Op,rme_ptr_t*);

    /* Make sure that there is something */
    Temp=Table[Pos];
    if(Temp==0)
        return RME_ERR_HAL_FAIL;

    /* Is this a page directory? We cannot unmap page directories like this */
    if((RME_PGT_SZORD(Pgt_Op->Order)!=RME_PGT_SIZE_4K)&&((Temp&RME_A7A_MMU_1M_PGDIR_PRESENT)!=0))
        return RME_ERR_HAL_FAIL;

    /* Try to unmap it. Use CAS just in case */
    if(RME_COMP_SWAP(&(Table[Pos]),Temp,0)==0)
        return RME_ERR_HAL_FAIL;

    return 0;
}
/* End Function:__RME_Pgt_Page_Unmap ***************************************/

/* Function:__RME_Pgt_Pgdir_Map *********************************************
Description : Map a page directory into the page table. It is surprising that
              Cortex-A (ARMv7) does not support any kind of page directory flags.
Input       : struct RME_Cap_Pgt* Pgt_Parent - The parent page table.
              struct RME_Cap_Pgt* Pgt_Child - The child page table.
              rme_ptr_t Pos - The position in the destination page table.
              rme_ptr_t Flags - The RME standard flags for the child page table.
Output      : None.
Return      : rme_ptr_t - If successful, 0; else RME_ERR_HAL_FAIL.
******************************************************************************/
rme_ptr_t __RME_Pgt_Pgdir_Map(struct RME_Cap_Pgt* Pgt_Parent, rme_ptr_t Pos,
                              struct RME_Cap_Pgt* Pgt_Child, rme_ptr_t Flags)
{
    rme_ptr_t* Parent_Table;
    rme_ptr_t* Child_Table;
    rme_ptr_t A7A_Flags;

    /* It should at least be readable */
    if((Flags&RME_PGT_READ)==0)
        return RME_ERR_HAL_FAIL;

    /* Are we trying to map into the kernel space on the top level? */
    if(((Pgt_Parent->Base&RME_PGT_TOP)!=0)&&(Pos>=2048))
        return RME_ERR_HAL_FAIL;

    /* Get the table */
    Parent_Table=RME_CAP_GETOBJ(Pgt_Parent,rme_ptr_t*);
    Child_Table=RME_CAP_GETOBJ(Pgt_Child,rme_ptr_t*);

    /* Generate the content */
    A7A_Flags=RME_A7A_MMU_1M_PGT_ADDR(RME_A7A_VA2PA(Child_Table))|RME_A7A_MMU_1M_PGDIR_PRESENT;

    /* Try to map it in - may need to increase some count */
    if(RME_COMP_SWAP(&(Parent_Table[Pos]),0,A7A_Flags)==0)
        return RME_ERR_HAL_FAIL;

    return 0;
}
/* End Function:__RME_Pgt_Pgdir_Map ****************************************/

/* Function:__RME_Pgt_Pgdir_Unmap *******************************************
Description : Unmap a page directory from the page table.
Input       : struct RME_Cap_Pgt* Pgt_Op - The page table to operate on.
              rme_ptr_t Pos - The position in the page table.
Output      : None.
Return      : rme_ptr_t - If successful, 0; else RME_ERR_HAL_FAIL.
******************************************************************************/
rme_ret_t __RME_Pgt_Pgdir_Unmap(struct RME_Cap_Pgt* Pgt_Parent,rme_ptr_t Pos,
                                struct RME_Cap_Pgt* Pgt_Child)
{

   return 0;
}
/* End Function:__RME_Pgt_Pgdir_Unmap **************************************/

/* Function:__RME_Pgt_Lookup **************************************************
Description : Lookup a page entry in a page directory. This function cannot deal
              with large pages and super sections - it is the user's responsibility
              to avoid this.
Input       : struct RME_Cap_Pgt* Pgt_Op - The page directory to lookup.
              rme_ptr_t Pos - The position to look up.
Output      : rme_ptr_t* Paddr - The physical address of the page.
              rme_ptr_t* Flags - The RME standard flags of the page.
Return      : rme_ptr_t - If successful, 0; else RME_ERR_HAL_FAIL.
******************************************************************************/
rme_ptr_t __RME_Pgt_Lookup(struct RME_Cap_Pgt* Pgt_Op, rme_ptr_t Pos, rme_ptr_t* Paddr, rme_ptr_t* Flags)
{
    rme_ptr_t* Table;
    rme_ptr_t Temp;

    /* Check if the position is within the range of this page table */
    if((Pos>>RME_PGT_NMORD(Pgt_Op->Order))!=0)
        return RME_ERR_HAL_FAIL;

    /* Get the table */
    Table=RME_CAP_GETOBJ(Pgt_Op,rme_ptr_t*);
    /* Get the position requested - atomic read */
    Temp=Table[Pos];

    /* Start lookup - is this a terminal page, or? */
    if(RME_PGT_SZORD(Pgt_Op->Order)==RME_PGT_SIZE_4K)
    {
        if((Temp&RME_A7A_MMU_4K_PAGE_PRESENT)==0)
            return RME_ERR_HAL_FAIL;

        /* This is a small page. Return the physical address and flags */
        if(Paddr!=0)
            *Paddr=RME_A7A_MMU_4K_PAGE_ADDR(Temp);
        if(Flags!=0)
            *Flags=RME_A7A_PGFLG_4K_NAT2RME(Temp);
    }
    else
    {
        if((Temp&RME_A7A_MMU_1M_PAGE_PRESENT)==0)
            return RME_ERR_HAL_FAIL;

        /* This is a section. Return the physical address and flags */
        if(Paddr!=0)
            *Paddr=RME_A7A_MMU_1M_PAGE_ADDR(Temp);
        if(Flags!=0)
            *Flags=RME_A7A_PGFLG_1M_NAT2RME(Temp);
    }

    return 0;
}
/* End Function:__RME_Pgt_Lookup *******************************************/

/* Function:__RME_Pgt_Walk **************************************************
Description : Walking function for the page table. This function just does page
              table lookups. The page table that is being walked must be the top-
              level page table. The output values are optional; only pass in pointers
              when you need that value.
              Walking kernel page tables is prohibited.
              This function also does not support large pages and super sections.
Input       : struct RME_Cap_Pgt* Pgt_Op - The page table to walk.
              rme_ptr_t Vaddr - The virtual address to look up.
Output      : rme_ptr_t* Pgt - The pointer to the page table level.
              rme_ptr_t* Map_Vaddr - The virtual address that starts mapping.
              rme_ptr_t* Paddr - The physical address of the page.
              rme_ptr_t* Size_Order - The size order of the page.
              rme_ptr_t* Num_Order - The entry order of the page.
              rme_ptr_t* Flags - The RME standard flags of the page.
Return      : rme_ptr_t - If successful, 0; else RME_ERR_HAL_FAIL.
******************************************************************************/
rme_ptr_t __RME_Pgt_Walk(struct RME_Cap_Pgt* Pgt_Op, 
                         rme_ptr_t Vaddr, 
                         rme_ptr_t* Pgt,
                           rme_ptr_t* Map_Vaddr, 
                           rme_ptr_t* Paddr, 
                           rme_ptr_t* Size_Order, 
                           rme_ptr_t* Num_Order, 
                           rme_ptr_t* Flags)
{
    rme_ptr_t* Table;
    rme_ptr_t Pos;
    rme_ptr_t Temp;

    /* Check if this is the top-level page table */
    if(((Pgt_Op->Base)&RME_PGT_TOP)==0)
        return RME_ERR_HAL_FAIL;

    /* Are we attempting a kernel or non-canonical lookup? If yes, stop immediately */
    if(Vaddr>=0x7FFFFFFFU)
        return RME_ERR_HAL_FAIL;

    /* Get the table and start lookup - We know that there are only 2 levels */
    Table=RME_CAP_GETOBJ(Pgt_Op, rme_ptr_t*);

    /* Calculate where is the entry - always 0 to 4096 */
    Pos=(Vaddr>>RME_PGT_SIZE_1M)&0xFFFU;
    /* Atomic read */
    Temp=Table[Pos];

    /* Find the position of the entry - Is there a page, a directory, or nothing? */
    if((Temp&RME_A7A_MMU_1M_PAGE_PRESENT)!=0)
    {
        /* There is a 1M page */
        if(Pgt!=0)
            *Pgt=(rme_ptr_t)Table;
        if(Map_Vaddr!=0)
            *Map_Vaddr=RME_ROUND_DOWN(Vaddr,RME_PGT_SIZE_1M);
        if(Paddr!=0)
            *Paddr=RME_A7A_MMU_1M_PAGE_ADDR(Temp);
        if(Size_Order!=0)
            *Size_Order=RME_PGT_SIZE_1M;
        if(Num_Order!=0)
            *Num_Order=RME_PGT_NUM_4K;
        if(Flags!=0)
            *Flags=RME_A7A_PGFLG_1M_NAT2RME(Temp);

    }
    else if((Temp&RME_A7A_MMU_1M_PGDIR_PRESENT)!=0)
    {
        Table=(rme_ptr_t*)RME_A7A_PA2VA(RME_A7A_MMU_1M_PGT_ADDR(Temp));
        /* Calculate where is the entry - always 0 to 256 */
        Pos=(Vaddr>>RME_PGT_SIZE_4K)&0xFFU;
        /* Atomic read */
        Temp=Table[Pos];

        if((Temp&RME_A7A_MMU_4K_PAGE_PRESENT)!=0)
        {
            /* There is a 4k page */
            if(Pgt!=0)
                *Pgt=(rme_ptr_t)Table;
            if(Map_Vaddr!=0)
                *Map_Vaddr=RME_ROUND_DOWN(Vaddr,RME_PGT_SIZE_4K);
            if(Paddr!=0)
                *Paddr=RME_A7A_MMU_4K_PAGE_ADDR(Temp);
            if(Size_Order!=0)
                *Size_Order=RME_PGT_SIZE_4K;
            if(Num_Order!=0)
                *Num_Order=RME_PGT_NUM_256;
            if(Flags!=0)
                *Flags=RME_A7A_PGFLG_4K_NAT2RME(Temp);
        }
        else
            return RME_ERR_HAL_FAIL;
    }
    else
        return RME_ERR_HAL_FAIL;

    return 0;
}
/* End Function:__RME_Pgt_Walk *********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
