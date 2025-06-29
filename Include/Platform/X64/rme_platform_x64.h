/******************************************************************************
Filename    : rme_platform_x64.h
Author      : pry
Date        : 01/04/2017
Licence     : The Unlicense; see LICENSE for details.
Description : The header of "platform_x64.c".
******************************************************************************/

/* Define ********************************************************************/
#include "multiboot.h"
#ifdef __HDR_DEF__
#ifndef __RME_PLATFORM_X64_DEF__
#define __RME_PLATFORM_X64_DEF__
/*****************************************************************************/
/* Basic Types ***************************************************************/
#ifndef __RME_S64_T__
#define __RME_S64_T__
typedef signed long long rme_s64_t;
#endif

#ifndef __RME_S32_T__
#define __RME_S32_T__
typedef signed int rme_s32_t;
#endif

#ifndef __RME_S16_T__
#define __RME_S16_T__
typedef signed short rme_s16_t;
#endif

#ifndef __RME_S8_T__
#define __RME_S8_T__
typedef signed char rme_s8_t;
#endif

#ifndef __RME_U64_T__
#define __RME_U64_T__
typedef unsigned long long rme_u64_t;
#endif

#ifndef __RME_U32_T__
#define __RME_U32_T__
typedef unsigned int rme_u32_t;
#endif

#ifndef __RME_U16_T__
#define __RME_U16_T__
typedef unsigned short rme_u16_t;
#endif

#ifndef __RME_U8_T__
#define __RME_U8_T__
typedef unsigned char rme_u8_t;
#endif
/* End Basic Types ***********************************************************/

/* Extended Types ************************************************************/
#ifndef __RME_TID_T__
#define __RME_TID_T__
/* The typedef for the Thread ID */
typedef rme_s64_t rme_tid_t;
#endif

#ifndef __RME_PTR_T__
#define __RME_PTR_T__
/* The typedef for the pointers - This is the raw style. Pointers must be unsigned */
typedef rme_u64_t rme_ptr_t;
#endif

#ifndef __RME_CNT_T__
#define __RME_CNT_T__
/* The typedef for the count variables */
typedef rme_s64_t rme_cnt_t;
#endif

#ifndef __RME_CID_T__
#define __RME_CID_T__
/* The typedef for capability ID */
typedef rme_s64_t rme_cid_t;
#endif

#ifndef __RME_RET_T__
#define __RME_RET_T__
/* The type for process return value */
typedef rme_s64_t rme_ret_t;
#endif
/* End Extended Types ********************************************************/

/* System macros *************************************************************/
/* Compiler "extern" keyword setting */
#define EXTERN                               extern
#define RME_EXTERN                           extern
/* Compiler "inline" keyword setting */
#define INLINE                               inline
/* Compiler likely & unlikely setting */
#define likely(X)							 (__builtin_expect(!!(X), 1))
#define unlikely(X)							 (__builtin_expect(!!(X), 0))
#ifdef likely
#define RME_LIKELY(X)                        (likely(X))
#else
#define RME_LIKELY(X)                        (X)
#endif
#ifdef unlikely
#define RME_UNLIKELY(X)                      (unlikely(X))
#else
#define RME_UNLIKELY(X)                      (X)
#endif
/* Get CPU-local data structure */
#define RME_CPU_LOCAL()                      __RME_X64_CPU_Local_Get()
/* The order of bits in one CPU machine word */
#define RME_WORD_ORDER                       (6U)
/* Forcing VA=PA in user memory segments */
#define RME_VA_EQU_PA                        (0U)
/* Quiescence timeslice value - always 10 slices, roughly equivalent to 100ms */
#define RME_QUIE_TIME                        (10U)
/* Cpt size limit - not restricted, user-level decides this */
#define RME_CPT_LIMIT                         (0U)
/* Normal page directory size calculation macro */
#define RME_PGT_SIZE_NOM(NUM_ORDER)           ((1<<(NUM_ORDER))*sizeof(rme_ptr_t))
/* Top-level page directory size calculation macro */
#define RME_PGT_SIZE_TOP(NUM_ORDER)           RME_PGT_SIZE_NOM(NUM_ORDER)
/* Initial stack size and address */
#define RME_KOM_STACK_ADDR                    ((rme_ptr_t)__RME_X64_Kern_Boot_Stack)
/* The virtual memory start address for the kernel objects */
#define RME_KOM_VA_BASE                       0xFFFF800001600000ULL
/* The size of the kernel object virtual memory - dummy, we will detect the actual values */
#define RME_KOM_VA_SIZE                       0x1000
/* The virtual memory start address for the virtual machines - If no virtual machines is used, set to 0 */
#define RME_HYP_VA_START                      (0U)
/*User-mode page table direct management switch */
#define RME_PGT_RAW_ENABLE                     (0U)
/* The kernel object allocation table address - relocated */
#define RME_KOT_VA_BASE						  ((rme_ptr_t*)0xFFFF800001000000)
/* Hypervisor reserved virtual address base - when no in use ,we set it to zero */
#define RME_HYP_VA_BASE                 	  (0U)
/* Hypervisor reserved virtual memory size - when no in use ,we set it to zero */
#define RME_HYP_VA_SIZE                 	  (0U)
/* Word bits of this architecture, in x86-64, it's 64bit */
#define RME_WORD_BITS                   	  (64U)
/* Upper limit of preemption priority */
#define RME_PREEMPT_PRIO_NUM         		  (64U)
/* Timestamp of system , in x86-64 ,we use RDTSC to count time */
#define RME_TIMESTAMP 						  __RME_Get_timestamp()
/* Kernel object table round function */
#define RME_KOT_VA_BASE_ROUND(x)			  RME_ROUND_UP(x,12)
/* 2-level cap flag*/
#define RME_CAPID_2L						  (((rme_cid_t)1)<<(sizeof(rme_ptr_t)*2-1))
/* Get 2-level capid */
#define RME_CAPID(X,Y)						  (((X)<<(sizeof(rme_ptr_t)*2))|(Y)|RME_CAPID_2L)
/* VGA text area address - this is a paddr */
#define RME_X64_VGA_BASE					  ((volatile rme_u16_t*)(RME_X64_PA2VA(0xB8000)))
/* VGA text area settings , we create a 80*25 area */
#define RME_X64_VGA_ROW_MAX					  (25U)
#define RME_X64_VGA_COL_MAX					  (80U)

/* Atomic instructions - The oficial release replaces all these with inline
 * assembly to boost speed. Sometimes this can harm compiler compatibility. If
 * you need normal assembly version, consider uncommenting the macro below. */
/* #define RME_X64_USE_DEFAULT_ATOMICS */
/* Default implementation */
#ifdef RME_X64_USE_DEFAULT_ATOMICS
/* Compare-and-Swap */
#define RME_COMP_SWAP(PTR,OLD,NEW)           __RME_X64_Comp_Swap(PTR,OLD,NEW)
/* Fetch-and-Add(FAA) */
#define RME_FETCH_ADD(PTR,ADDEND)            __RME_X64_Fetch_Add(PTR,ADDEND)
/* Fetch-and-And(FAND) */
#define RME_FETCH_AND(PTR,OPERAND)           __RME_X64_Fetch_And(PTR,OPERAND)
/* Get most significant bit */
#define RME_MSB_GET(VAL)                     __RME_X64_MSB_Get(VAL)
/* Inline assembly implementation */
#else
static INLINE rme_ptr_t _RME_X64_Comp_Swap(volatile rme_ptr_t* Ptr, rme_ptr_t Old, rme_ptr_t New)
{
	rme_u8_t Zero;
	__asm__ __volatile__("LOCK CMPXCHGQ %[New], %[Ptr]; SETZ %[Zero]"
	                     :[Ptr]"+m"(*Ptr), [Zero]"=r"(Zero)
	                     :[New]"r"(New), [Old]"a"(Old)
	                     :"memory", "cc");
	return (rme_ptr_t)Zero;
}
static INLINE rme_ptr_t _RME_X64_Fetch_Add(volatile rme_ptr_t* Ptr, rme_cnt_t Addend)
{
	__asm__ __volatile__("LOCK XADDQ %[Addend], %[Ptr]"
	                     :[Ptr]"+m"(*Ptr), [Addend]"+r"(Addend)
	                     :
	                     :"memory", "cc");
	return Addend;
}
static INLINE rme_ptr_t _RME_X64_Fetch_And(rme_ptr_t* Ptr, rme_cnt_t Operand)
{
	rme_u64_t Old;
	__asm__ __volatile__("MOVQ %[Ptr],%[Old]; LOCK ANDQ %[Operand], %[Ptr]"
	                     :[Ptr]"+m"(*Ptr), [Old]"=r"(Old)
	                     :[Operand]"r"(Operand)
	                     :"memory", "cc");
	return Old;
}
static INLINE rme_ptr_t _RME_X64_MSB_Get(rme_ptr_t Val)
{
	rme_u64_t Ret;
	__asm__ __volatile__("BSRQ %[Val],%[Ret]"
	                     :[Ret]"=r"(Ret)
	                     :[Val]"r"(Val)
	                     :"cc");
	return Ret;
}

static INLINE void __RME_Int_Disable()
{
	__asm__ __volatile__ (
		"cli\n\t"
	);
}

static INLINE void __RME_Int_Enable()
{
	__asm__ __volatile__ (
		"sti\n\t"
	);
}

static INLINE rme_ptr_t __RME_User_Enter()
{
	rme_ptr_t Ret;
	asm volatile (
	"movq %1, %%rcx\n\t"
	"movq %2, %%rsp\n\t"
	"movq $0x3200, %%r11\n\t"
	"movq %3, %%rdi\n\t"
	"sysretq"
);
	return Ret;
}

static INLINE rme_ptr_t __RME_Get_timestamp()
{
    rme_u32_t lo, hi;
    __asm__ __volatile__ (
        "rdtsc"
        : "=a" (lo), "=d" (hi)
    );
    return ((rme_ptr_t)hi << 32) | lo;
}

#define RME_COMP_SWAP(PTR,OLD,NEW)           _RME_X64_Comp_Swap(PTR,OLD,NEW)
#define RME_FETCH_ADD(PTR,ADDEND)            _RME_X64_Fetch_Add(PTR,ADDEND)
#define RME_FETCH_AND(PTR,OPERAND)           _RME_X64_Fetch_And(PTR,OPERAND)
#define RME_MSB_GET(VAL)                     _RME_X64_MSB_Get(VAL)
#endif
/* No read acquires needed because x86-64 guarantees read-read and read-write consistency */
#define RME_READ_ACQUIRE(X)                  (*(X)) 
/* No read acquires needed because x86-64 guarantees write-write consistency. In RME, we do
 * not use this to restrict the reordering of write-read (because all OCCUPY or similar
 * operations are performed with compare-and-swap which is already required to be serializing
 * when it is implemented in assembly), thus this can also be left empty. In the future, if
 * x86-64 write-write consistency is not perserved, we may need memory fencing. */
#define RME_WRITE_RELEASE(X,V)               ((*(X))=(V))

/* FPU type definitions */
#define RME_X64_FPU_AVX                      (1)
#define RME_X64_FPU_AVX512                   (2)
/* The CPU and application specific macros are here */
#include "rme_platform_x64_conf.h"
/* End System macros *********************************************************/

/* X64 specific macros *******************************************************/
/* Initial boot capabilities */
/* The capability table of the init process */
#define RME_BOOT_INIT_CPT                      0
/* The top-level page table of the init process - an array */
#define RME_BOOT_TBL_PGT                   1
/* The init process */
#define RME_BOOT_INIT_PRC                   2
/* The init thread - this is a per-core array */
#define RME_BOOT_TBL_THD                     3
/* The initial kernel function capability */
#define RME_BOOT_INIT_KERN                   4
/* The initial kernel memory capability - this is a per-NUMA node array */
#define RME_BOOT_TBL_KOM                    5
/* The initial timer endpoint - this is a per-core array */
#define RME_BOOT_TBL_TIMER                   6
/* The initial default endpoint for all other interrupts - this is a per-core array */
#define RME_BOOT_TBL_INT                     7

/* The initial page table indices in the RME_BOOT_TBL_PGT */
#define RME_BOOT_PML4                        0
#define RME_BOOT_PDP(X)                      (RME_BOOT_PML4+1+(X))
#define RME_BOOT_PDE(X)                      (RME_BOOT_PDP(16)+(X))

/* Booting capability layout */
#define RME_X64_CPT                          (Cpt)
/* Kernel VA mapping base address - PML5 currently unsupported */
#define RME_X64_VA_BASE                      (0xFFFF800000000000ULL)
#define RME_X64_TEXT_VA_BASE                 (0xFFFFFFFF80000000ULL)
/* The offset of kernel object table */
#define RME_X64_KOT_OFFSET                   (0x1000000ULL)
/* The offset of device hole */
#define RME_X64_DEVICE_OFFSET                (0xFE000000ULL)
/* Convert PA-VA and VA-PA in the first block of memory (16MB - 3.25GB)*/
#define RME_X64_PA2VA(PA)                    (((rme_ptr_t)(PA))+RME_X64_VA_BASE)
#define RME_X64_VA2PA(VA)                    (((rme_ptr_t)(VA))-RME_X64_VA_BASE)
/* Convert PA-VA and VA-PA in the text memory (16MB - 2GB )*/
#define RME_X64_TEXT_PA2VA(PA)               (((rme_ptr_t)(PA))+RME_X64_TEXT_VA_BASE)
#define RME_X64_TEXT_VA2PA(VA)               (((rme_ptr_t)(VA))-RME_X64_TEXT_VA_BASE)
/* How many segments under 4G are allowed for Kom1 - If this exceeded the kernel will just hang */
#define RME_X64_KOM1_MAXSEGS                32

/* Kernel stact size per CPU - currently set to 1MB */
#define RME_X64_KSTACK_ORDER                 (20)
/* Get the actual table positions */
#define RME_X64_PGT_TBL_NOM(X)             (X)
#define RME_X64_PGT_TBL_TOP(X)             (X)

/* Device types */
#define RME_X64_MADT_LAPIC                   0
#define RME_X64_MADT_IOAPIC                  1
#define RME_X64_MADT_INT_SRC_OVERRIDE        2
#define RME_X64_MADT_NMI_INT_SRC             3
#define RME_X64_MADT_LAPIC_NMI               4

#define RME_X64_APIC_LAPIC_ENABLED           1

/* Page entry bit definitions */
/* No execution */
#define RME_X64_MMU_NX                       (((rme_ptr_t)1)<<63)
/* Present */
#define RME_X64_MMU_P                        (((rme_ptr_t)1)<<0)
/* Is it read-only or writable? */
#define RME_X64_MMU_RW                       (((rme_ptr_t)1)<<1)
/* Is it user or system? */
#define RME_X64_MMU_US                       (((rme_ptr_t)1)<<2)
/* Is it write-through? */
#define RME_X64_MMU_PWT                      (((rme_ptr_t)1)<<3)
/* Can we cache it? */
#define RME_X64_MMU_PCD                      (((rme_ptr_t)1)<<4)


/* Is this accessed? */
#define RME_X64_MMU_A                        (((rme_ptr_t)1)<<5)
/* Is this page dirty? */
#define RME_X64_MMU_D                        (((rme_ptr_t)1)<<6)
/* The page-attribute table bit for 4k pages */
#define RME_X64_MMU_PTE_PAT                  (((rme_ptr_t)1)<<7)
/* The super-page bit */
#define RME_X64_MMU_PDE_SUP                  (((rme_ptr_t)1)<<7)
/* The global page bit - use this for all kernel memories */
#define RME_X64_MMU_G                        (((rme_ptr_t)1)<<8)
/* The page-attribute table bit for other page sizes */
#define RME_X64_MMU_PDE_PAT                  (((rme_ptr_t)1)<<12)
/* The generic address mask */
#define RME_X64_MMU_ADDR(X)                  ((X)&0x000FFFFFFFFFF000)
/* Initial PML4 entries */
#define RME_X64_MMU_KERN_PML4                (RME_X64_MMU_P|RME_X64_MMU_RW|RME_X64_MMU_G)
/* Initial PDP entries - note that the P bit is not set */
#define RME_X64_MMU_KERN_PDP                 (RME_X64_MMU_RW|RME_X64_MMU_G)
/* Initial PDE entries */
#define RME_X64_MMU_KERN_PDE                 (RME_X64_MMU_P|RME_X64_MMU_PDE_SUP|RME_X64_MMU_RW|RME_X64_MMU_G)

/* MMU definitions */
/* Write info to MMU - no longer used because we have PCID */
#define RME_X64_CR3_PCD                      (1<<4)
#define RME_X64_CR3_PWT                      (1<<3)

/* Aggregate the X64 flags and prepare for translation - NX, PCD, PWT, RW */
#define RME_X64_PGFLG_RME2NAT(FLAGS)         (RME_X64_Pgflg_RME2NAT[(FLAGS)&(~RME_PGT_STATIC)])
#define RME_X64_PGFLG_NAT2RME(FLAGS)         (RME_X64_Pgflg_NAT2RME[(((FLAGS)>>63)<<3)|(((FLAGS)&0x18)>>2)|(((FLAGS)&0x02)>>1)])

/* Hardware port definitions */
#define RME_X64_COM1                         (0x3F8)
#define RME_X64_PIT_CH0                      (0x40)
#define RME_X64_PIT_CH1                      (0x41)
#define RME_X64_PIT_CH2                      (0x42)
#define RME_X64_PIT_CMD                      (0x43)
#define RME_X64_RTC_CMD                      (0x70)
#define RME_X64_RTC_DATA                     (0x71)
#define RME_X64_PIC1                         (0x20)
#define RME_X64_PIC2                         (0xA0)

/* CPUID feature tables */
/* Vendor ID/highest function parameter */
#define RME_X64_CPUID_0_VENDOR_ID            (0x0)
/* Processor info and feature bits */
#define RME_X64_CPUID_1_INFO_FEATURE         (0x1)
/* Cache and TLB descriptor information */
#define RME_X64_CPUID_2_CACHE_TLB            (0x2)
/* Processor serial number */
#define RME_X64_CPUID_3_SERIAL_NUM           (0x3)
/* Intel thread/core and cache topology 1 */
#define RME_X64_CPUID_4_INTEL_TOPO1          (0x4)
/* ECX=0, returns Intel extended features */
#define RME_X64_CPUID_7_ECX0_INTEL_EXT       (0x7)
/* Intel thread/core and cache topology 2 */
#define RME_X64_CPUID_B_INTEL_TOPO2          (0xB)

/* Get highest extenbded function supported */
#define RME_X64_CPUID_E0_EXT_MAX             (0x80000000)
/* Extended processor info and feature bits */
#define RME_X64_CPUID_E1_INFO_FEATURE        (0x80000001)
#define RME_X64_E1_EDX_FPU                   (1<<0)
#define RME_X64_E1_EDX_VME                   (1<<1)
#define RME_X64_E1_EDX_DE                    (1<<2)
#define RME_X64_E1_EDX_PSE                   (1<<3)
#define RME_X64_E1_EDX_TSC                   (1<<4)
#define RME_X64_E1_EDX_MSR                   (1<<5)
#define RME_X64_E1_EDX_PAE                   (1<<6)
#define RME_X64_E1_EDX_MCE                   (1<<7)
#define RME_X64_E1_EDX_CX8                   (1<<8)
#define RME_X64_E1_EDX_APIC                  (1<<9)
#define RME_X64_E1_EDX_SYSCALL               (1<<11)
#define RME_X64_E1_EDX_MTRR                  (1<<12)
#define RME_X64_E1_EDX_PGE                   (1<<13)
#define RME_X64_E1_EDX_MCA                   (1<<14)
#define RME_X64_E1_EDX_CMOV                  (1<<15)
#define RME_X64_E1_EDX_PAT                   (1<<16)
#define RME_X64_E1_EDX_PSE36                 (1<<17)
#define RME_X64_E1_EDX_MP                    (1<<19)
#define RME_X64_E1_EDX_NX                    (1<<20)
#define RME_X64_E1_EDX_MMXEXT                (1<<22)
#define RME_X64_E1_EDX_MMX                   (1<<23)
#define RME_X64_E1_EDX_FXSR                  (1<<24)
#define RME_X64_E1_EDX_FXSR_OPT              (1<<25)
#define RME_X64_E1_EDX_PDPE1GB               (1<<26)
#define RME_X64_E1_EDX_RDTSCP                (1<<27)
#define RME_X64_E1_EDX_LM                    (1<<29)
#define RME_X64_E1_EDX_3DNOWEXT              (1<<30)
#define RME_X64_E1_EDX_3DNOW                 (1<<31)

/* Processor brand string 1 */
#define RME_X64_CPUID_E2_BRAND1              (0x80000002)
/* Processor brand string 2 */
#define RME_X64_CPUID_E3_BRAND2              (0x80000003)
/* Processor brand string 3 */
#define RME_X64_CPUID_E4_BRAND3              (0x80000004)
/* L1 cache and TLB identifiers */
#define RME_X64_CPUID_E5_L1_TLB              (0x80000005)
/* Extended L2 cache features */
#define RME_X64_CPUID_E6_L2                  (0x80000006)
/* Advanced power management information */
#define RME_X64_CPUID_E7_APMI                (0x80000007)
/* Virtual and physical address sizes */
#define RME_X64_CPUID_E8_VA_PA_SIZE          (0x80000008)
/* AMD Easter egg - IT'S HAMMER TIME */
#define RME_X64_CPUID_EX_AMD_EASTER          (0x8FFFFFFF)

/* Feature detection macros */
#define RME_X64_FUNC(FUNC,REG)               (RME_X64_Feature.Func[FUNC][REG])
#define RME_X64_EXT(EXT,REG)                 (RME_X64_Feature.Ext[(EXT)-RME_X64_CPUID_E0_EXT_MAX][REG])

/* Vector and trap types - for trap we use vector type but allow DPL3 */
#define RME_X64_IDT_VECT                     (0x8E)
#define RME_X64_IDT_TRAP                     (0xEE)

/* Set up a normal interrupt/trap gate descriptor. 8 is the GDT CS offset */
#define RME_X64_SET_IDT(TABLE, VECT, TYPE_ATTR, ADDR) \
do \
{ \
    (TABLE)[VECT].Offset1=((rme_ptr_t)(ADDR))&0xFFFF; \
    (TABLE)[VECT].Selector=8; \
    (TABLE)[VECT].IST_Offset=0; \
    (TABLE)[VECT].Type_Attr=(TYPE_ATTR); \
    (TABLE)[VECT].Offset2=(((rme_ptr_t)(ADDR))>>16)&0xFFFF; \
    (TABLE)[VECT].Offset3=((rme_ptr_t)(ADDR))>>32; \
    (TABLE)[VECT].Zero=0; \
} \
while(0)

#define RME_X64_USER_IDT(TABLE, VECT) \
do \
{ \
	RME_X64_SET_IDT(TABLE, VECT, RME_X64_IDT_VECT, __RME_X64_USER##VECT##_Handler); \
} \
while(0)

/* Interrupt vector numbers */
/* Divide error */
#define RME_X64_FAULT_DE                     (0)
/* Debug exception */
#define RME_X64_TRAP_DB                      (1)
/* NMI error */
#define RME_X64_INT_NMI                      (2)
/* Debug breakpoint */
#define RME_X64_TRAP_BP                      (3)
/* Overflow exception */
#define RME_X64_TRAP_OF                      (4)
/* Bound range exception */
#define RME_X64_FAULT_BR                     (5)
/* Undefined instruction */
#define RME_X64_FAULT_UD                     (6)
/* Device not available */
#define RME_X64_FAULT_NM                     (7)
/* Double(nested) fault exception */
#define RME_X64_ABORT_DF                     (8)
/* Coprocessor overrun - not used later on */
#define RME_X64_ABORT_OLD_MF                 (9)
/* Invalid TSS exception */
#define RME_X64_FAULT_TS                     (10)
/* Segment not present */
#define RME_X64_FAULT_NP                     (11)
/* Stack fault exception */
#define RME_X64_FAULT_SS                     (12)
/* General protection exception */
#define RME_X64_FAULT_GP                     (13)
/* Page fault exception */
#define RME_X64_FAULT_PF                     (14)
/* Number 15 reserved */
/* X87 FPU floating-point error */
#define RME_X64_FAULT_MF                     (16)
/* Alignment check exception */
#define RME_X64_FAULT_AC                     (17)
/* Machine check exception */
#define RME_X64_ABORT_MC                     (18)
/* SIMD floating-point exception */
#define RME_X64_FAULT_XM                     (19)
/* Virtualization exception */
#define RME_X64_FAULT_VE                     (20)
/* User interrupts */
#define RME_X64_INT_USER(INT)                ((INT)+32)
#define RME_X64_INT_SYSTICK                  RME_X64_INT_USER(2)

/* User interrupts that are used by RME - map these two even further away */
#define RME_X64_INT_SPUR                     RME_X64_INT_USER(0x80-32)
#define RME_X64_INT_ERROR                    RME_X64_INT_USER(0x81-32)
#define RME_X64_INT_IPI                      RME_X64_INT_USER(0x82-32)
#define RME_X64_INT_SMP_SYSTICK              RME_X64_INT_USER(0x83-32)

/* LAPIC offsets - maybe we should use structs later on */
#define RME_X64_LAPIC_ID                     (0x0020/4)
#define RME_X64_LAPIC_VER                    (0x0030/4)
#define RME_X64_LAPIC_TPR                    (0x0080/4)
#define RME_X64_LAPIC_EOI                    (0x00B0/4)
#define RME_X64_LAPIC_SVR                    (0x00F0/4)
#define RME_X64_LAPIC_SVR_ENABLE             (0x00000100)

#define RME_X64_LAPIC_ESR                    (0x0280/4)
#define RME_X64_LAPIC_ICRLO                  (0x0300/4)
#define RME_X64_LAPIC_ICRLO_INIT             (0x00000500)
#define RME_X64_LAPIC_ICRLO_STARTUP          (0x00000600)
#define RME_X64_LAPIC_ICRLO_DELIVS           (0x00001000)
#define RME_X64_LAPIC_ICRLO_ASSERT           (0x00004000)
#define RME_X64_LAPIC_ICRLO_DEASSERT         (0x00000000)
#define RME_X64_LAPIC_ICRLO_LEVEL            (0x00008000)
#define RME_X64_LAPIC_ICRLO_BCAST            (0x00080000)
#define RME_X64_LAPIC_ICRLO_EXC_SELF         (0x000C0000)
#define RME_X64_LAPIC_ICRLO_BUSY             (0x00001000)
#define RME_X64_LAPIC_ICRLO_FIXED            (0x00000000)

#define RME_X64_LAPIC_ICRHI                  (0x0310/4)
#define RME_X64_LAPIC_TIMER                  (0x0320/4)
#define RME_X64_LAPIC_TIMER_X1               (0x0000000B)
#define RME_X64_LAPIC_TIMER_PERIODIC         (0x00020000)

#define RME_X64_LAPIC_PCINT                  (0x0340/4)
#define RME_X64_LAPIC_LINT0                  (0x0350/4)
#define RME_X64_LAPIC_LINT1                  (0x0360/4)
#define RME_X64_LAPIC_ERROR                  (0x0370/4)
#define RME_X64_LAPIC_MASKED                 (0x00010000)

#define RME_X64_LAPIC_TICR                   (0x0380/4)
#define RME_X64_LAPIC_TCCR                   (0x0390/4)
#define RME_X64_LAPIC_TDCR                   (0x03E0/4)

/* LAPIC R/W */
#define RME_X64_LAPIC_READ(REG)              (((rme_u32_t*)RME_X64_PA2VA(RME_X64_LAPIC_Addr))[REG])
#define RME_X64_LAPIC_WRITE(REG,VAL)         (((rme_u32_t*)RME_X64_PA2VA(RME_X64_LAPIC_Addr))[REG]=(VAL))

/* IOAPIC address - consider supporting multiple ones */
#define RME_X64_IOAPIC_ADDR                 (RME_X64_PA2VA(0xFEC00000))

/* IOAPIC registers */
#define RME_X64_IOAPIC_REG_ID               (0x00)
#define RME_X64_IOAPIC_REG_VER              (0x01)
#define RME_X64_IOAPIC_REG_TABLE            (0x10)

#define RME_X64_IOAPIC_INT_DISABLED         (0x00010000)
#define RME_X64_IOAPIC_INT_LEVEL            (0x00008000)
#define RME_X64_IOAPIC_INT_ACTIVELOW        (0x00002000)
#define RME_X64_IOAPIC_INT_LOGICAL          (0x00000800)

#define RME_X64_IOAPIC_READ(REG,DATA) \
do \
{ \
	((struct RME_X64_IOAPIC_Map*)RME_X64_IOAPIC_ADDR)->Reg=(REG); \
	(DATA)=((struct RME_X64_IOAPIC_Map*)RME_X64_IOAPIC_ADDR)->Data; \
} \
while(0)

#define RME_X64_IOAPIC_WRITE(REG,DATA) \
do \
{ \
	((struct RME_X64_IOAPIC_Map*)RME_X64_IOAPIC_ADDR)->Reg=(REG); \
	((struct RME_X64_IOAPIC_Map*)RME_X64_IOAPIC_ADDR)->Data=(DATA); \
} \
while(0)

/* Processor RFLAGS register bits */
#define RME_X64_RFLAGS_IF                  (1<<9)

/* MSR addresses */
#define RME_X64_MSR_IA32_EFER              (0xC0000080)
#define RME_X64_MSR_IA32_GS_BASE           (0xC0000101)
#define RME_X64_MSR_IA32_KERNEL_GS_BASE    (0xC0000102)
#define RME_X64_MSR_IA32_STAR              (0xC0000081)
#define RME_X64_MSR_IA32_LSTAR             (0xC0000082)
#define RME_X64_MSR_IA32_FMASK             (0xC0000084)

/* MSR bits */
#define RME_X64_MSR_IA32_EFER_SCE          (1)

/* Segment definitions */
#define RME_X64_SEG_KERNEL_CODE            (1*8)
#define RME_X64_SEG_KERNEL_DATA            (2*8)
#define RME_X64_SEG_USER_CODE              (5*8+3)
#define RME_X64_SEG_USER_DATA              (4*8+3)
#define RME_X64_SEG_EMPTY                  (3*8+3)

/* Get kernel stack addresses */
#define RME_X64_KSTACK(CPU)                (RME_X64_Layout.Stack_Start+((1+(CPU))<<RME_X64_KSTACK_ORDER))
/* Get boot-time user stack addresses */
#define RME_X64_USTACK(CPU)                (RME_POW2(RME_PGT_SIZE_2M)+((CPU)+1)*RME_POW2(RME_PGT_SIZE_2K))
/* The base of CPU-local data area */
#define RME_X64_CPU_LOCAL_BASE(CPU)        (RME_X64_Layout.PerCPU_Start+(CPU)*2*RME_POW2(RME_PGT_SIZE_4K))
/* Microsecond delay function - not needed in most cases */
#define RME_X64_UDELAY(US)
/*****************************************************************************/
/* __RME_PLATFORM_X64_DEF__ */
#endif
/* __HDR_DEF__ */
#endif
/* End Define ****************************************************************/

/* Struct ********************************************************************/
#ifdef __HDR_STRUCT__
#ifndef __RME_PLATFORM_X64_STRUCT__
#define __RME_PLATFORM_X64_STRUCT__
/* We used structs in the header */

/* Use defines in these headers */
#define __HDR_DEF__
#undef __HDR_DEF__
/*****************************************************************************/
/* Architecture-related structures - we only target GCC so attribute packed is fine */
/* Root System Description Pointer descriptor */
struct RME_X64_ACPI_RDSP_Desc
{
	rme_u8_t Signature[8];
	rme_u8_t Checksum;
	rme_u8_t OEM_ID[6];
	rme_u8_t Revision;
	rme_u32_t RSDT_Addr_Phys;

	/* These are the extended parts that we do not use now */
	rme_u32_t Length;
	rme_u64_t XSDT_Addr_Phys;
	rme_u8_t Xchecksum;
	rme_u8_t Reserved[3];
} __attribute__((__packed__));

/* General ACPI descriptor header */
struct RME_X64_ACPI_Desc_Hdr
{
	rme_u8_t Signature[4];
	rme_u32_t Length;
	rme_u8_t Revision;
	rme_u8_t Checksum;
	rme_u8_t OEM_ID[6];
	rme_u8_t OEM_Table_ID[8];
	rme_u32_t OEM_Revision;
	rme_u8_t Creator_ID[4];
	rme_u32_t Creator_Revision;
} __attribute__((__packed__));

/* Root System Description Table header */
struct RME_X64_ACPI_RSDT_Hdr
{
	struct RME_X64_ACPI_Desc_Hdr Header;
    /* This is fine; GCC can take this */
	rme_u32_t Entry[0];
} __attribute__((__packed__));

/* Multiple APIC Description Table header */
struct RME_X64_ACPI_MADT_Hdr
{
	struct RME_X64_ACPI_Desc_Hdr Header;
	rme_u32_t LAPIC_Addr_Phys;
	rme_u32_t Flags;
    /* This is fine; GCC can take this */
	rme_u8_t Table[0];
} __attribute__((__packed__));

/* MADT's LAPIC record */
struct RME_X64_ACPI_MADT_LAPIC_Record
{
	rme_u8_t Type;
	rme_u8_t Length;
	rme_u8_t ACPI_ID;
	rme_u8_t APIC_ID;
	rme_u32_t Flags;
} __attribute__((__packed__));

/* MADT's IOAPIC record */
struct RME_X64_ACPI_MADT_IOAPIC_Record
{
	rme_u8_t Type;
	rme_u8_t Length;
	rme_u8_t ID;
	rme_u8_t Reserved;
	rme_u32_t Addr;
	rme_u32_t Interrupt_Base;
} __attribute__((__packed__));

/* MADT's interrupt source override record*/
struct RME_X64_ACPI_MADT_SRC_OVERRIDE_Record
{
	rme_u8_t Type;
	rme_u8_t Length;
    rme_u8_t Bus;
    rme_u8_t Source;
    rme_u8_t GS_Interrupt;
    rme_u16_t MPS_Int_Flags;
}  __attribute__((__packed__));

/* IDT entry - 16 bytes in length. 256 entries are 4kB in total */
struct RME_X64_IDT_Entry
{
	/* Offset bits 0..15 */
    rme_u16_t Offset1;
    /* A code segment selector in GDT or LDT */
	rme_u16_t Selector;
    /* Bits 0..2 holds Interrupt Stack Table offset, rest of bits zero */
    rme_u8_t IST_Offset;
    /* Type and attributes */
    rme_u8_t Type_Attr;
    /* Offset bits 16..31 */
    rme_u16_t Offset2;
    /* Offset bits 32..63 */
    rme_u32_t Offset3;
    /* Reserved, must be zero */
    rme_u32_t Zero;
} __attribute__((packed));

/* IOAPIC data structure */
struct RME_X64_IOAPIC_Map
{
    rme_u32_t Reg;
    rme_u32_t Pad[3];
    rme_u32_t Data;
};

/* Temporary data structure */
struct RME_X64_Temp
{
	/* The CPU local address. The reason why this is here is because x86-64
	 * does not support instructions to read segment base directly. We cannot
	 * risk fsgsbase instructions because it is not supported on all CPUs. */
	rme_ptr_t CPU_Local_Addr;
	/* The kernel stack for this CPU */
	rme_ptr_t Kernel_SP;
	/* The temporary user stack for this CPU */
	rme_ptr_t Temp_User_SP;
};

/* Per-CPU data structure */
struct RME_X64_CPU_Info
{
	/* The LAPIC ID of the CPU, used to distinguish between different CPUs */
	rme_ptr_t LAPIC_ID;
	/* Is the booting done on this CPU? */
	volatile rme_ptr_t Boot_Done;
};

/* Per-IOAPIC data structure */
struct RME_X64_IOAPIC_Info
{
    rme_ptr_t IOAPIC_ID;
};

/* The 6 registers that are used to pass arguments are RDI, RSI, RDX, RCX, R8, R9.
 * Note that this is different from Micro$oft: M$ use RCX, RDX, R8, R9. The return
 * value is always located at RAX. */
struct RME_Reg_Struct
{
    rme_ptr_t RAX;
    rme_ptr_t RBX;
    rme_ptr_t RCX;
    rme_ptr_t RDX;
    rme_ptr_t RSI;
    rme_ptr_t RDI;
    rme_ptr_t RBP;
    rme_ptr_t R8;
    rme_ptr_t R9;
    rme_ptr_t R10;
    rme_ptr_t R11;
    rme_ptr_t R12;
    rme_ptr_t R13;
    rme_ptr_t R14;
    rme_ptr_t R15;
    /* If we went into this with a SYSCALL, this is 0x10000 */
    rme_ptr_t INT_NUM;
    rme_ptr_t ERROR_CODE;
    rme_ptr_t RIP;
    rme_ptr_t CS;
    rme_ptr_t RFLAGS;
    rme_ptr_t RSP;
    rme_ptr_t SS;
};

/* Exception context structure
 * This structure holds the metadata when an exception or interrupt occurs.
 * It supplements the general-purpose register structure by providing exception-specific info.
 */
struct RME_Exc_Struct
{
    /* The interrupt vector number that caused the trap, e.g., 14 = page fault */
    rme_ptr_t INT_NUM;

    /* The error code pushed by the CPU automatically (only for some exceptions). 
     * For example, page fault, GPF, double fault, etc. 
     * If not present, this is set to 0. */
    rme_ptr_t ERROR_CODE;

    /* The faulting instruction address (same as RIP in most cases) */
    rme_ptr_t RIP;

    /* Code segment selector at the time of the exception */
    rme_ptr_t CS;

    /* RFLAGS register at the time of exception, including interrupt flag, etc. */
    rme_ptr_t RFLAGS;

    /* Stack pointer at the time of exception (user/kernel depending on CPL switch) */
    rme_ptr_t RSP;

    /* Stack segment selector (only pushed if transitioning from user to kernel) */
    rme_ptr_t SS;
};

/* The coprocessor register set structure. MMX and SSE */
struct RME_Cop_Struct
{
	/* MMX registers first */
	rme_ptr_t FPR_MMX0[2];
	rme_ptr_t FPR_MMX1[2];
	rme_ptr_t FPR_MMX2[2];
	rme_ptr_t FPR_MMX3[2];
	rme_ptr_t FPR_MMX4[2];
	rme_ptr_t FPR_MMX5[2];
	rme_ptr_t FPR_MMX6[2];
	rme_ptr_t FPR_MMX7[2];
	/* SSE registers follow */
#if(RME_X64_FPU_TYPE==RME_FALSE)
	rme_ptr_t XMM0[2];
	rme_ptr_t XMM1[2];
	rme_ptr_t XMM2[2];
	rme_ptr_t XMM3[2];
	rme_ptr_t XMM4[2];
	rme_ptr_t XMM5[2];
	rme_ptr_t XMM6[2];
	rme_ptr_t XMM7[2];
	rme_ptr_t XMM8[2];
	rme_ptr_t XMM9[2];
	rme_ptr_t XMM10[2];
	rme_ptr_t XMM11[2];
	rme_ptr_t XMM12[2];
	rme_ptr_t XMM13[2];
	rme_ptr_t XMM14[2];
	rme_ptr_t XMM15[2];
#elif(RME_X64_FPU_TYPE==RME_X64_FPU_AVX)
	rme_ptr_t XYMM0[4];
	rme_ptr_t XYMM1[4];
	rme_ptr_t XYMM2[4];
	rme_ptr_t XYMM3[4];
	rme_ptr_t XYMM4[4];
	rme_ptr_t XYMM5[4];
	rme_ptr_t XYMM6[4];
	rme_ptr_t XYMM7[4];
	rme_ptr_t XYMM8[4];
	rme_ptr_t XYMM9[4];
	rme_ptr_t XYMM10[4];
	rme_ptr_t XYMM11[4];
	rme_ptr_t XYMM12[4];
	rme_ptr_t XYMM13[4];
	rme_ptr_t XYMM14[4];
	rme_ptr_t XYMM15[4];
#elif(RME_X64_FPU_TYPE==RME_X64_FPU_AVX512)
	rme_ptr_t XYZMM0[8];
	rme_ptr_t XYZMM1[8];
	rme_ptr_t XYZMM2[8];
	rme_ptr_t XYZMM3[8];
	rme_ptr_t XYZMM4[8];
	rme_ptr_t XYZMM5[8];
	rme_ptr_t XYZMM6[8];
	rme_ptr_t XYZMM7[8];
	rme_ptr_t XYZMM8[8];
	rme_ptr_t XYZMM9[8];
	rme_ptr_t XYZMM10[8];
	rme_ptr_t XYZMM11[8];
	rme_ptr_t XYZMM12[8];
	rme_ptr_t XYZMM13[8];
	rme_ptr_t XYZMM14[8];
	rme_ptr_t XYZMM15[8];
	rme_ptr_t XYZMM16[8];
	rme_ptr_t XYZMM17[8];
	rme_ptr_t XYZMM18[8];
	rme_ptr_t XYZMM19[8];
	rme_ptr_t XYZMM20[8];
	rme_ptr_t XYZMM21[8];
	rme_ptr_t XYZMM22[8];
	rme_ptr_t XYZMM23[8];
	rme_ptr_t XYZMM24[8];
	rme_ptr_t XYZMM25[8];
	rme_ptr_t XYZMM26[8];
	rme_ptr_t XYZMM27[8];
	rme_ptr_t XYZMM28[8];
	rme_ptr_t XYZMM29[8];
	rme_ptr_t XYZMM30[8];
	rme_ptr_t XYZMM31[8];
#endif
};

/* Need to save SP and IP across synchronous invocation */
struct RME_Iret_Struct
{
    rme_ptr_t RIP;
    rme_ptr_t RSP;
};

/* Memory information - the layout is (offset from VA base):
 * |0--640k|----------16MB|-----|------|------|-----|3.25G-4G|-----|-----|
 * |Vectors|Kernel&Globals|Kotbl|PerCPU|Kpgtbl|Kom1|  Hole  |Kom2|Stack|
 *  Vectors        : Interrupt vectors.
 *  Kernel&Globals : Initial kernel text segment and all static variables.
 *  Kotbl          : Kernel object registration table.
 *  PerCPU         : Per-CPU data structures.
 *  Kpgtbl         : Kernel page tables.
 *  Kom1          : Kernel memory 1, linear mapping, allow creation of page tables.
 *  Hole           : Memory hole present at 3.25G-4G. For PCI devices.
 *  Kom2          : Kernel memory 2, nonlinear mapping, no page table creation allowed.
 *  Stacks         : Kernel stacks, per-CPU.
 *  All values are in bytes, and are virtual addresses.
 */
struct RME_X64_Layout
{
	rme_ptr_t Kot_Start;
	rme_ptr_t Kot_Size;

	rme_ptr_t PerCPU_Start;
	rme_ptr_t PerCPU_Size;

	rme_ptr_t Kpgtbl_Start;
	rme_ptr_t Kpgtbl_Size;

	/* We allow max. 32 trunks under 4G */
	rme_ptr_t Kom1_Trunks;
	rme_ptr_t Kom1_Start[RME_X64_KOM1_MAXSEGS];
	rme_ptr_t Kom1_Size[RME_X64_KOM1_MAXSEGS];

	rme_ptr_t Hole_Start;
	rme_ptr_t Hole_Size;

	rme_ptr_t Kom2_Start;
	rme_ptr_t Kom2_Size;

	rme_ptr_t Stack_Start;
	rme_ptr_t Stack_Size;
};

/* The processor features */
struct RME_X64_Features
{
	rme_ptr_t Max_Func;
	rme_ptr_t Max_Ext;
	rme_ptr_t Func[16][4];
	rme_ptr_t Ext[16][4];
};

/* The first two levels of the kernel page table. The third level will be constructed on the fly */
struct __RME_X64_Kern_Pgt
{
	rme_ptr_t Dummy[256];
	rme_ptr_t PML4[256];
	rme_ptr_t PDP[256][512];
};

/*****************************************************************************/
/* __RME_PLATFORM_X64_STRUCT__ */
#endif
/* __HDR_STRUCT__ */
#endif
/* End Struct ****************************************************************/

/* Private Variable **********************************************************/
#if(!(defined __HDR_DEF__||defined __HDR_STRUCT__))
#ifndef __RME_PLATFORM_X64_MEMBER__
#define __RME_PLATFORM_X64_MEMBER__

/* In this way we can use the data structures and definitions in the headers */
#define __HDR_DEF__

#undef __HDR_DEF__

#define __HDR_STRUCT__

#undef __HDR_STRUCT__

/* If the header is not used in the public mode */
#ifndef __HDR_PUBLIC__
/*****************************************************************************/
/* Is there a UART in the system? */
static volatile rme_ptr_t RME_X64_UART_Exist;
/* Where is the multiboot information located? */
static volatile struct multiboot_info* RME_X64_MBInfo;
/* The layout of the memory structure */
static volatile struct RME_X64_Layout RME_X64_Layout;
/* We currently support 256 CPUs max */
static volatile rme_ptr_t RME_X64_Num_CPU;
/* CPU counter */
static volatile rme_ptr_t RME_X64_CPU_Cnt;
static volatile struct RME_X64_CPU_Info RME_X64_CPU_Info[RME_X64_CPU_NUM];
/* There can be max. 8 IOAPICs */
static volatile rme_ptr_t RME_X64_Num_IOAPIC;
static volatile struct RME_X64_IOAPIC_Info RME_X64_IOAPIC_Info[RME_X64_IOAPIC_NUM];
/* The LAPIC address */
static volatile rme_ptr_t RME_X64_LAPIC_Addr;
/* The processor features */
static volatile struct RME_X64_Features RME_X64_Feature;
/* The PCID counter */
static volatile rme_ptr_t RME_X64_PCID_Inc;
/* The VGA buffer pointer */
static volatile rme_u16_t* vga_buffer;
/* The VGA text cursor row */
static volatile rme_ptr_t vga_row;
/* The VGA text cursor col */
static volatile rme_ptr_t vga_col;

/* Translate the flags into X64 specific ones - the STATIC bit will never be
 * set thus no need to consider about it here. The flag bits order is shown below:
 * [MSB                                                                                         LSB]
 * RME_PGT_BUFFERABLE | RME_PGT_CACHEABLE | RME_PGT_EXECUTE | RME_PGT_WRITE | RME_PGT_READ
 * The C snippet to generate this (gcc x64):

#include <stdio.h>
#define X64_NX          (((unsigned long long)1)<<63)
#define X64_P           (1<<0)
#define X64_RW          (1<<1)
#define X64_PWT         (1<<3)
#define X64_PCD         (1<<4)
#define RME_READ        (1<<0)
#define RME_WRITE       (1<<1)
#define RME_EXECUTE     (1<<2)
#define RME_CACHEABLE   (1<<3)
#define RME_BUFFERABLE  (1<<4)
int main(void)
{
	unsigned long long result;
	int count;
	for(count=0;count<32;count++)
	{
	    result=X64_P;
		if((count&RME_WRITE)!=0)
			result|=X64_RW;
		if((count&RME_EXECUTE)==0)
			result|=X64_NX;
		if((count&RME_CACHEABLE)==0)
			result|=X64_PCD;
		if((count&RME_BUFFERABLE)==0)
			result|=X64_PWT;
	    printf("0x%016llX,",result);
	    if(count%4==3)
	    	printf("\n");
	}
	return 0;
}
 */
static const rme_ptr_t RME_X64_Pgflg_RME2NAT[32]=
{
	0x8000000000000019,0x8000000000000019,0x800000000000001B,0x800000000000001B,
	0x0000000000000019,0x0000000000000019,0x000000000000001B,0x000000000000001B,
	0x8000000000000009,0x8000000000000009,0x800000000000000B,0x800000000000000B,
	0x0000000000000009,0x0000000000000009,0x000000000000000B,0x000000000000000B,
	0x8000000000000011,0x8000000000000011,0x8000000000000013,0x8000000000000013,
	0x0000000000000011,0x0000000000000011,0x0000000000000013,0x0000000000000013,
	0x8000000000000001,0x8000000000000001,0x8000000000000003,0x8000000000000003,
	0x0000000000000001,0x0000000000000001,0x0000000000000003,0x0000000000000003
};

/* Translate the flags back to RME format. In order to use this table, it is needed to extract the
 * X64 bits: [63](NX) [4](PCD) [3](PWT) [1](RW). The C snippet to generate this (gcc x64): 

#include <stdio.h>
#define X64_NX          (1<<3)
#define X64_PCD         (1<<2)
#define X64_PWT         (1<<1)
#define X64_RW          (1<<0)
#define RME_READ        (1<<0)
#define RME_WRITE       (1<<1)
#define RME_EXECUTE     (1<<2)
#define RME_CACHEABLE   (1<<3)
#define RME_BUFFERABLE  (1<<4)
#define RME_STATIC      (1<<5)
int main(void)
{
    unsigned long long int flag;
    int count;
    for(count=0;count<16;count++)
    {
        flag=RME_READ;
        if((count&X64_NX)==0)
            flag|=RME_EXECUTE;
        if((count&X64_PCD)==0)
            flag|=RME_CACHEABLE;
        if((count&X64_PWT)==0)
            flag|=RME_BUFFERABLE;
        if((count&X64_RW)!=0)
            flag|=RME_WRITE;
        printf("0x%016llX,",flag);
	    if(count%4==3)
	    	printf("\n");
    }
}
 */
static const rme_ptr_t RME_X64_Pgflg_NAT2RME[16]=
{
	0x000000000000001D,0x000000000000001F,0x000000000000000D,0x000000000000000F,
	0x0000000000000015,0x0000000000000017,0x0000000000000005,0x0000000000000007,
	0x0000000000000019,0x000000000000001B,0x0000000000000009,0x000000000000000B,
	0x0000000000000011,0x0000000000000013,0x0000000000000001,0x0000000000000003,
};

/* This boot code is the binary of the following boot.S, compiled with:
 * gcc -fno-pic -nostdinc -I. -o boot.o -c boot.S
 * ld -m elf_x86_64 -nodefaultlibs -N -e Start_16 -Ttext 0x7000 -o boot_block.o boot.o
 * objcopy -S -O binary -j .text boot_block.o boot_block.bin
 * hexdump -v  -e '8/1 "0x%02X,""\n"' < boot_block.bin > boot_block.c
 * objdump -S boot_block.o > boot_block.asm
 * The contents of boot_block.c is placed here. Load this to 0x7000 to boot different
 * processors.

                 .code16
                 .global        Start_16
Start_16:
                 CLI
                 XORW           %AX,%AX
                 MOVW           %AX,%DS
                 MOVW           %AX,%ES
                 MOVW           %AX,%SS
                 LGDT           Boot_GDT_Desc_16
                 MOV            %CR0,%EAX
                 ORL            $0x01,%EAX
                 MOVL           %EAX,%CR0
                 LJMPL          $8,$(Boot_32)
                 .code32
Boot_32:
                 MOVW           $16,%AX
                 MOVW           %AX,%DS
                 MOVW           %AX,%ES
                 MOVW           %AX,%SS
                 XORW           %AX,%AX
                 MOVW           %AX,%FS
                 MOVW           %AX,%GS
                 MOV            $1,%EBX
                 MOVL           (Start_16-4),%ESP
                 CALL           *(Start_16-8)
                 JMP            .

                 .p2align       2
Boot_GDT_16:
                 .word          0x0000,0x0000,0x0000,0x0000;
                 .word          0xFFFF,0x0000
                 .byte          0x00,0x9A,0xCF,0x00
                 .word          0xFFFF,0x0000
                 .byte          0x00,0x92,0xCF,0x00
Boot_GDT_Desc_16:
                 .word          (Boot_GDT_Desc_16-Boot_GDT_16-1)
                 .long          Boot_GDT_16
 */
static const rme_u8_t RME_X64_Boot_Code[]=
{
    0xFA,0x31,0xC0,0x8E,0xD8,0x8E,0xC0,0x8E,
    0xD0,0x0F,0x01,0x16,0x5C,0x70,0x0F,0x20,
    0xC0,0x66,0x83,0xC8,0x01,0x0F,0x22,0xC0,
    0x66,0xEA,0x20,0x70,0x00,0x00,0x08,0x00,
    0x66,0xB8,0x10,0x00,0x8E,0xD8,0x8E,0xC0,
    0x8E,0xD0,0x66,0x31,0xC0,0x8E,0xE0,0x8E,
    0xE8,0xBB,0x01,0x00,0x00,0x00,0x8B,0x25,
    0xFC,0x6F,0x00,0x00,0xFF,0x15,0xF8,0x6F,
    0x00,0x00,0xEB,0xFE,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,
    0x00,0x9A,0xCF,0x00,0xFF,0xFF,0x00,0x00,
    0x00,0x92,0xCF,0x00,0x17,0x00,0x44,0x70,
    0x00,0x00
};
/*****************************************************************************/
/* End Private Variable ******************************************************/

/* Private Function **********************************************************/ 
/*****************************************************************************/
static void __RME_X64_UART_Init(void);
/* Find the RDSP */
static struct RME_X64_ACPI_RDSP_Desc* __RME_X64_RDSP_Scan(rme_ptr_t Base, rme_ptr_t Len);
static struct RME_X64_ACPI_RDSP_Desc* __RME_X64_RDSP_Find(void);
static rme_ret_t __RME_X64_SMP_Detect(struct RME_X64_ACPI_MADT_Hdr* MADT);
/* Debug output helper */
static void __RME_X64_ACPI_Debug(struct RME_X64_ACPI_Desc_Hdr *Header);
/* Initialize the ACPI */
static rme_ret_t __RME_X64_ACPI_Init(void);
/* Get processor feature bits */
static void __RME_X64_Feature_Get(void);
/* Initialize memory according to GRUB multiboot specification */
static void __RME_X64_Mem_Init(rme_ptr_t MMap_Addr, rme_ptr_t MMap_Length);
/* Initialize CPU-local tables */
static void __RME_X64_CPU_Local_Init(void);
/* Get the RME CPU-local data structure given its CPUID */
static struct RME_CPU_Local* __RME_X64_CPU_Local_Get_By_CPUID(rme_ptr_t CPUID);
/* Initialize interrupt controllers */
static void __RME_X64_PIC_Init(void);
static void __RME_X64_LAPIC_Init(void);
static void __RME_X64_IOAPIC_Init(void);
/* Enable/disable a vector in IOAPIC */
static void __RME_X64_IOAPIC_Int_Enable(rme_ptr_t IRQ, rme_ptr_t CPUID);
static void __RME_X64_IOAPIC_Int_Disable(rme_ptr_t IRQ);
/* Initialize timers */
static void __RME_X64_Timer_Init(void);
/*****************************************************************************/
#define __EXTERN__
/* End Private Function ******************************************************/

/* Public Variable ***********************************************************/
/* __HDR_PUBLIC__ */
#else
#define __EXTERN__ EXTERN 
/* __HDR_PUBLIC__ */
#endif

/*****************************************************************************/
EXTERN struct RME_X64_IDT_Entry RME_X64_IDT_Table[256];
EXTERN struct __RME_X64_Kern_Pgt RME_X64_Kpgt;
EXTERN rme_ptr_t __RME_X64_Kern_Boot_Stack[0];
/*****************************************************************************/

/* End Public Variable *******************************************************/

/* Public Function ***********************************************************/
/*****************************************************************************/
/* X64 specific */
EXTERN rme_ptr_t __RME_X64_In(rme_ptr_t Port);
EXTERN void __RME_X64_Out(rme_ptr_t Port, rme_ptr_t Data);
EXTERN rme_ptr_t __RME_X64_Read_MSR(rme_ptr_t MSR);
EXTERN void __RME_X64_Write_MSR(rme_ptr_t MSR, rme_ptr_t Value);
EXTERN void __RME_X64_GDT_Load(rme_ptr_t* GDTR);
EXTERN void __RME_X64_IDT_Load(rme_ptr_t* IDTR);
EXTERN void __RME_X64_TSS_Load(rme_ptr_t TSS);
EXTERN rme_ptr_t __RME_X64_CPUID_Get(rme_ptr_t EAX, rme_ptr_t* EBX, rme_ptr_t* ECX, rme_ptr_t* EDX);
EXTERN void __RME_X64_Pgt_Set(rme_ptr_t Pgt);
EXTERN void __RME_Svc_Param_Get(struct RME_Reg_Struct* Reg,rme_ptr_t* Svc,rme_ptr_t* Cid,rme_ptr_t* Param);
EXTERN void __RME_Svc_Retval_Set(struct RME_Reg_Struct* Reg,rme_ret_t Retval);
EXTERN void __RME_Inv_Retval_Set(struct RME_Reg_Struct* Reg,rme_ret_t Retval);
EXTERN rme_ret_t __RME_Kfn_Handler(struct RME_Cap_Cpt* Cpt,struct RME_Reg_Struct* Reg,rme_ptr_t FuncID,rme_ptr_t SubID,rme_ptr_t Param1,rme_ptr_t Param2);
EXTERN void __RME_List_Crt(struct RME_List* Head);
EXTERN void __RME_List_Ins(struct RME_List* New,struct RME_List* Prev,struct RME_List* Next);
EXTERN void __RME_List_Del(struct RME_List* Prev,struct RME_List* Next);
/* Boot glue */
EXTERN void __RME_X64_SMP_Boot_32(void);
/* Vectors */
EXTERN void __RME_X64_FAULT_DE_Handler(void);
EXTERN void __RME_X64_TRAP_DB_Handler(void);
EXTERN void __RME_X64_INT_NMI_Handler(void);
EXTERN void __RME_X64_TRAP_BP_Handler(void);
EXTERN void __RME_X64_TRAP_OF_Handler(void);
EXTERN void __RME_X64_FAULT_BR_Handler(void);
EXTERN void __RME_X64_FAULT_UD_Handler(void);
EXTERN void __RME_X64_FAULT_NM_Handler(void);
EXTERN void __RME_X64_ABORT_DF_Handler(void);
EXTERN void __RME_X64_ABORT_OLD_MF_Handler(void);
EXTERN void __RME_X64_FAULT_TS_Handler(void);
EXTERN void __RME_X64_FAULT_NP_Handler(void);
EXTERN void __RME_X64_FAULT_SS_Handler(void);
EXTERN void __RME_X64_FAULT_GP_Handler(void);
EXTERN void __RME_X64_FAULT_PF_Handler(void);
EXTERN void __RME_X64_FAULT_MF_Handler(void);
EXTERN void __RME_X64_FAULT_AC_Handler(void);
EXTERN void __RME_X64_ABORT_MC_Handler(void);
EXTERN void __RME_X64_FAULT_XM_Handler(void);
EXTERN void __RME_X64_FAULT_VE_Handler(void);
/* Systick&SVC handler */
EXTERN void SysTick_Handler(void);
EXTERN void SysTick_SMP_Handler(void);
EXTERN void SVC_Handler(void);
/* User handlers */
EXTERN void __RME_X64_USER32_Handler(void);
EXTERN void __RME_X64_USER33_Handler(void);
EXTERN void __RME_X64_USER34_Handler(void);
EXTERN void __RME_X64_USER35_Handler(void);
EXTERN void __RME_X64_USER36_Handler(void);
EXTERN void __RME_X64_USER37_Handler(void);
EXTERN void __RME_X64_USER38_Handler(void);
EXTERN void __RME_X64_USER39_Handler(void);

EXTERN void __RME_X64_USER40_Handler(void);
EXTERN void __RME_X64_USER41_Handler(void);
EXTERN void __RME_X64_USER42_Handler(void);
EXTERN void __RME_X64_USER43_Handler(void);
EXTERN void __RME_X64_USER44_Handler(void);
EXTERN void __RME_X64_USER45_Handler(void);
EXTERN void __RME_X64_USER46_Handler(void);
EXTERN void __RME_X64_USER47_Handler(void);
EXTERN void __RME_X64_USER48_Handler(void);
EXTERN void __RME_X64_USER49_Handler(void);

EXTERN void __RME_X64_USER50_Handler(void);
EXTERN void __RME_X64_USER51_Handler(void);
EXTERN void __RME_X64_USER52_Handler(void);
EXTERN void __RME_X64_USER53_Handler(void);
EXTERN void __RME_X64_USER54_Handler(void);
EXTERN void __RME_X64_USER55_Handler(void);
EXTERN void __RME_X64_USER56_Handler(void);
EXTERN void __RME_X64_USER57_Handler(void);
EXTERN void __RME_X64_USER58_Handler(void);
EXTERN void __RME_X64_USER59_Handler(void);

EXTERN void __RME_X64_USER60_Handler(void);
EXTERN void __RME_X64_USER61_Handler(void);
EXTERN void __RME_X64_USER62_Handler(void);
EXTERN void __RME_X64_USER63_Handler(void);
EXTERN void __RME_X64_USER64_Handler(void);
EXTERN void __RME_X64_USER65_Handler(void);
EXTERN void __RME_X64_USER66_Handler(void);
EXTERN void __RME_X64_USER67_Handler(void);
EXTERN void __RME_X64_USER68_Handler(void);
EXTERN void __RME_X64_USER69_Handler(void);

EXTERN void __RME_X64_USER70_Handler(void);
EXTERN void __RME_X64_USER71_Handler(void);
EXTERN void __RME_X64_USER72_Handler(void);
EXTERN void __RME_X64_USER73_Handler(void);
EXTERN void __RME_X64_USER74_Handler(void);
EXTERN void __RME_X64_USER75_Handler(void);
EXTERN void __RME_X64_USER76_Handler(void);
EXTERN void __RME_X64_USER77_Handler(void);
EXTERN void __RME_X64_USER78_Handler(void);
EXTERN void __RME_X64_USER79_Handler(void);

EXTERN void __RME_X64_USER80_Handler(void);
EXTERN void __RME_X64_USER81_Handler(void);
EXTERN void __RME_X64_USER82_Handler(void);
EXTERN void __RME_X64_USER83_Handler(void);
EXTERN void __RME_X64_USER84_Handler(void);
EXTERN void __RME_X64_USER85_Handler(void);
EXTERN void __RME_X64_USER86_Handler(void);
EXTERN void __RME_X64_USER87_Handler(void);
EXTERN void __RME_X64_USER88_Handler(void);
EXTERN void __RME_X64_USER89_Handler(void);

EXTERN void __RME_X64_USER90_Handler(void);
EXTERN void __RME_X64_USER91_Handler(void);
EXTERN void __RME_X64_USER92_Handler(void);
EXTERN void __RME_X64_USER93_Handler(void);
EXTERN void __RME_X64_USER94_Handler(void);
EXTERN void __RME_X64_USER95_Handler(void);
EXTERN void __RME_X64_USER96_Handler(void);
EXTERN void __RME_X64_USER97_Handler(void);
EXTERN void __RME_X64_USER98_Handler(void);
EXTERN void __RME_X64_USER99_Handler(void);

EXTERN void __RME_X64_USER100_Handler(void);
EXTERN void __RME_X64_USER101_Handler(void);
EXTERN void __RME_X64_USER102_Handler(void);
EXTERN void __RME_X64_USER103_Handler(void);
EXTERN void __RME_X64_USER104_Handler(void);
EXTERN void __RME_X64_USER105_Handler(void);
EXTERN void __RME_X64_USER106_Handler(void);
EXTERN void __RME_X64_USER107_Handler(void);
EXTERN void __RME_X64_USER108_Handler(void);
EXTERN void __RME_X64_USER109_Handler(void);

EXTERN void __RME_X64_USER110_Handler(void);
EXTERN void __RME_X64_USER111_Handler(void);
EXTERN void __RME_X64_USER112_Handler(void);
EXTERN void __RME_X64_USER113_Handler(void);
EXTERN void __RME_X64_USER114_Handler(void);
EXTERN void __RME_X64_USER115_Handler(void);
EXTERN void __RME_X64_USER116_Handler(void);
EXTERN void __RME_X64_USER117_Handler(void);
EXTERN void __RME_X64_USER118_Handler(void);
EXTERN void __RME_X64_USER119_Handler(void);

EXTERN void __RME_X64_USER120_Handler(void);
EXTERN void __RME_X64_USER121_Handler(void);
EXTERN void __RME_X64_USER122_Handler(void);
EXTERN void __RME_X64_USER123_Handler(void);
EXTERN void __RME_X64_USER124_Handler(void);
EXTERN void __RME_X64_USER125_Handler(void);
EXTERN void __RME_X64_USER126_Handler(void);
EXTERN void __RME_X64_USER127_Handler(void);
EXTERN void __RME_X64_USER128_Handler(void);
EXTERN void __RME_X64_USER129_Handler(void);

EXTERN void __RME_X64_USER130_Handler(void);
EXTERN void __RME_X64_USER131_Handler(void);
EXTERN void __RME_X64_USER132_Handler(void);
EXTERN void __RME_X64_USER133_Handler(void);
EXTERN void __RME_X64_USER134_Handler(void);
EXTERN void __RME_X64_USER135_Handler(void);
EXTERN void __RME_X64_USER136_Handler(void);
EXTERN void __RME_X64_USER137_Handler(void);
EXTERN void __RME_X64_USER138_Handler(void);
EXTERN void __RME_X64_USER139_Handler(void);

EXTERN void __RME_X64_USER140_Handler(void);
EXTERN void __RME_X64_USER141_Handler(void);
EXTERN void __RME_X64_USER142_Handler(void);
EXTERN void __RME_X64_USER143_Handler(void);
EXTERN void __RME_X64_USER144_Handler(void);
EXTERN void __RME_X64_USER145_Handler(void);
EXTERN void __RME_X64_USER146_Handler(void);
EXTERN void __RME_X64_USER147_Handler(void);
EXTERN void __RME_X64_USER148_Handler(void);
EXTERN void __RME_X64_USER149_Handler(void);

EXTERN void __RME_X64_USER150_Handler(void);
EXTERN void __RME_X64_USER151_Handler(void);
EXTERN void __RME_X64_USER152_Handler(void);
EXTERN void __RME_X64_USER153_Handler(void);
EXTERN void __RME_X64_USER154_Handler(void);
EXTERN void __RME_X64_USER155_Handler(void);
EXTERN void __RME_X64_USER156_Handler(void);
EXTERN void __RME_X64_USER157_Handler(void);
EXTERN void __RME_X64_USER158_Handler(void);
EXTERN void __RME_X64_USER159_Handler(void);

EXTERN void __RME_X64_USER160_Handler(void);
EXTERN void __RME_X64_USER161_Handler(void);
EXTERN void __RME_X64_USER162_Handler(void);
EXTERN void __RME_X64_USER163_Handler(void);
EXTERN void __RME_X64_USER164_Handler(void);
EXTERN void __RME_X64_USER165_Handler(void);
EXTERN void __RME_X64_USER166_Handler(void);
EXTERN void __RME_X64_USER167_Handler(void);
EXTERN void __RME_X64_USER168_Handler(void);
EXTERN void __RME_X64_USER169_Handler(void);

EXTERN void __RME_X64_USER170_Handler(void);
EXTERN void __RME_X64_USER171_Handler(void);
EXTERN void __RME_X64_USER172_Handler(void);
EXTERN void __RME_X64_USER173_Handler(void);
EXTERN void __RME_X64_USER174_Handler(void);
EXTERN void __RME_X64_USER175_Handler(void);
EXTERN void __RME_X64_USER176_Handler(void);
EXTERN void __RME_X64_USER177_Handler(void);
EXTERN void __RME_X64_USER178_Handler(void);
EXTERN void __RME_X64_USER179_Handler(void);

EXTERN void __RME_X64_USER180_Handler(void);
EXTERN void __RME_X64_USER181_Handler(void);
EXTERN void __RME_X64_USER182_Handler(void);
EXTERN void __RME_X64_USER183_Handler(void);
EXTERN void __RME_X64_USER184_Handler(void);
EXTERN void __RME_X64_USER185_Handler(void);
EXTERN void __RME_X64_USER186_Handler(void);
EXTERN void __RME_X64_USER187_Handler(void);
EXTERN void __RME_X64_USER188_Handler(void);
EXTERN void __RME_X64_USER189_Handler(void);

EXTERN void __RME_X64_USER190_Handler(void);
EXTERN void __RME_X64_USER191_Handler(void);
EXTERN void __RME_X64_USER192_Handler(void);
EXTERN void __RME_X64_USER193_Handler(void);
EXTERN void __RME_X64_USER194_Handler(void);
EXTERN void __RME_X64_USER195_Handler(void);
EXTERN void __RME_X64_USER196_Handler(void);
EXTERN void __RME_X64_USER197_Handler(void);
EXTERN void __RME_X64_USER198_Handler(void);
EXTERN void __RME_X64_USER199_Handler(void);

EXTERN void __RME_X64_USER200_Handler(void);
EXTERN void __RME_X64_USER201_Handler(void);
EXTERN void __RME_X64_USER202_Handler(void);
EXTERN void __RME_X64_USER203_Handler(void);
EXTERN void __RME_X64_USER204_Handler(void);
EXTERN void __RME_X64_USER205_Handler(void);
EXTERN void __RME_X64_USER206_Handler(void);
EXTERN void __RME_X64_USER207_Handler(void);
EXTERN void __RME_X64_USER208_Handler(void);
EXTERN void __RME_X64_USER209_Handler(void);

EXTERN void __RME_X64_USER210_Handler(void);
EXTERN void __RME_X64_USER211_Handler(void);
EXTERN void __RME_X64_USER212_Handler(void);
EXTERN void __RME_X64_USER213_Handler(void);
EXTERN void __RME_X64_USER214_Handler(void);
EXTERN void __RME_X64_USER215_Handler(void);
EXTERN void __RME_X64_USER216_Handler(void);
EXTERN void __RME_X64_USER217_Handler(void);
EXTERN void __RME_X64_USER218_Handler(void);
EXTERN void __RME_X64_USER219_Handler(void);

EXTERN void __RME_X64_USER220_Handler(void);
EXTERN void __RME_X64_USER221_Handler(void);
EXTERN void __RME_X64_USER222_Handler(void);
EXTERN void __RME_X64_USER223_Handler(void);
EXTERN void __RME_X64_USER224_Handler(void);
EXTERN void __RME_X64_USER225_Handler(void);
EXTERN void __RME_X64_USER226_Handler(void);
EXTERN void __RME_X64_USER227_Handler(void);
EXTERN void __RME_X64_USER228_Handler(void);
EXTERN void __RME_X64_USER229_Handler(void);

EXTERN void __RME_X64_USER230_Handler(void);
EXTERN void __RME_X64_USER231_Handler(void);
EXTERN void __RME_X64_USER232_Handler(void);
EXTERN void __RME_X64_USER233_Handler(void);
EXTERN void __RME_X64_USER234_Handler(void);
EXTERN void __RME_X64_USER235_Handler(void);
EXTERN void __RME_X64_USER236_Handler(void);
EXTERN void __RME_X64_USER237_Handler(void);
EXTERN void __RME_X64_USER238_Handler(void);
EXTERN void __RME_X64_USER239_Handler(void);

EXTERN void __RME_X64_USER240_Handler(void);
EXTERN void __RME_X64_USER241_Handler(void);
EXTERN void __RME_X64_USER242_Handler(void);
EXTERN void __RME_X64_USER243_Handler(void);
EXTERN void __RME_X64_USER244_Handler(void);
EXTERN void __RME_X64_USER245_Handler(void);
EXTERN void __RME_X64_USER246_Handler(void);
EXTERN void __RME_X64_USER247_Handler(void);
EXTERN void __RME_X64_USER248_Handler(void);
EXTERN void __RME_X64_USER249_Handler(void);

EXTERN void __RME_X64_USER250_Handler(void);
EXTERN void __RME_X64_USER251_Handler(void);
EXTERN void __RME_X64_USER252_Handler(void);
EXTERN void __RME_X64_USER253_Handler(void);
EXTERN void __RME_X64_USER254_Handler(void);
EXTERN void __RME_X64_USER255_Handler(void);
/* Interrupt control */
EXTERN void __RME_Disable_Int(void);
EXTERN void __RME_Enable_Int(void);
EXTERN void __RME_X64_Halt(void);
__EXTERN__ void __RME_X64_SMP_Tick(void);
__EXTERN__ void __RME_X64_LAPIC_Ack(void);
/* Atomics */
__EXTERN__ rme_ptr_t __RME_X64_Comp_Swap(rme_ptr_t* Ptr, rme_ptr_t Old, rme_ptr_t New);
__EXTERN__ rme_ptr_t __RME_X64_Fetch_Add(rme_ptr_t* Ptr, rme_cnt_t Addend);
__EXTERN__ rme_ptr_t __RME_X64_Fetch_And(rme_ptr_t* Ptr, rme_ptr_t Operand);
__EXTERN__ rme_ptr_t __RME_X64_Write_Release(void);
/* MSB counting */
EXTERN rme_ptr_t __RME_X64_MSB_Get(rme_ptr_t Val);
/* Debugging */
__EXTERN__ rme_ptr_t __RME_Putchar(char Char);
/* Coprocessor */
EXTERN void ___RME_X64_Thd_Cop_Save(struct RME_Cop_Struct* Cop_Reg);
EXTERN void ___RME_X64_Thd_Cop_Restore(struct RME_Cop_Struct* Cop_Reg);
/* Booting */
EXTERN void _RME_Kmain(rme_ptr_t Stack);
EXTERN void __RME_Enter_User_Mode(rme_ptr_t Entry_Addr, rme_ptr_t Stack_Addr, rme_ptr_t CPUID);
__EXTERN__ rme_ptr_t __RME_Lowlvl_Init(void);
__EXTERN__ rme_ptr_t __RME_Boot(void);
__EXTERN__ void __RME_Reboot(void);
__EXTERN__ void __RME_Shutdown(void);
/* Syscall & invocation */
EXTERN struct RME_CPU_Local* __RME_X64_CPU_Local_Get(void);
__EXTERN__ void __RME_Get_Syscall_Param(struct RME_Reg_Struct* Reg, rme_ptr_t* Svc,
                                        rme_ptr_t* Capid, rme_ptr_t* Param);
__EXTERN__ void __RME_Set_Syscall_Retval(struct RME_Reg_Struct* Reg, rme_ret_t Retval);
/* Thread register sets */
__EXTERN__ void __RME_Thd_Reg_Init(rme_ptr_t Attr,rme_ptr_t Entry, rme_ptr_t Stack, rme_ptr_t Param, struct RME_Reg_Struct* Reg);
__EXTERN__ void __RME_Thd_Reg_Copy(struct RME_Reg_Struct* Dst, struct RME_Reg_Struct* Src);
__EXTERN__ void __RME_Thd_Cop_Init(struct RME_Reg_Struct* Reg, struct RME_Cop_Struct* Cop_Reg);
__EXTERN__ void __RME_Thd_Cop_Save(struct RME_Reg_Struct* Reg, struct RME_Cop_Struct* Cop_Reg);
__EXTERN__ void __RME_Thd_Cop_Restore(struct RME_Reg_Struct* Reg, struct RME_Cop_Struct* Cop_Reg);
/* Invocation register sets */
__EXTERN__ void __RME_Inv_Reg_Init(rme_ptr_t Param, struct RME_Reg_Struct* Reg);
__EXTERN__ void __RME_Inv_Reg_Save(struct RME_Iret_Struct* Ret, struct RME_Reg_Struct* Reg);
__EXTERN__ void __RME_Inv_Reg_Restore(struct RME_Reg_Struct* Reg, struct RME_Iret_Struct* Ret);
__EXTERN__ void __RME_Set_Inv_Retval(struct RME_Reg_Struct* Reg, rme_ret_t Retval);
/* Kernel function handler */
__EXTERN__ rme_ret_t __RME_Kern_Func_Handler(struct RME_Cap_Cpt* Cpt, struct RME_Reg_Struct* Reg,
                                             rme_ptr_t Func_ID, rme_ptr_t Sub_ID, rme_ptr_t Param1, rme_ptr_t Param2);
/* Fault handler */
__EXTERN__ void __RME_X64_Fault_Handler(struct RME_Reg_Struct* Reg, rme_ptr_t Reason);
/* Generic interrupt handler */
__EXTERN__ void __RME_X64_Generic_Handler(struct RME_Reg_Struct* Reg, rme_ptr_t Int_Num);
/* Page table operations */
__EXTERN__ void __RME_Pgt_Set(struct RME_Cap_Pgt* Pgt);
__EXTERN__ rme_ptr_t __RME_Pgt_Kom_Init(void);
__EXTERN__ rme_ptr_t __RME_Pgt_Check(rme_ptr_t Base_Addr, rme_ptr_t Is_Top, rme_ptr_t Size_Order, rme_ptr_t Num_Order, rme_ptr_t Vaddr);
__EXTERN__ rme_ptr_t __RME_Pgt_Init(struct RME_Cap_Pgt* Pgt_Op);
__EXTERN__ rme_ptr_t __RME_Pgt_Del_Check(struct RME_Cap_Pgt* Pgt_Op);
__EXTERN__ rme_ptr_t __RME_Pgt_Page_Map(struct RME_Cap_Pgt* Pgt_Op, rme_ptr_t Paddr, rme_ptr_t Pos, rme_ptr_t Flags);
__EXTERN__ rme_ptr_t __RME_Pgt_Page_Unmap(struct RME_Cap_Pgt* Pgt_Op, rme_ptr_t Pos);
__EXTERN__ rme_ptr_t __RME_Pgt_Pgdir_Map(struct RME_Cap_Pgt* Pgt_Parent, rme_ptr_t Pos,
                                           struct RME_Cap_Pgt* Pgt_Child, rme_ptr_t Flags);
__EXTERN__ rme_ptr_t __RME_Pgt_Pgdir_Unmap(struct RME_Cap_Pgt* Pgt_Parent, rme_ptr_t Pos, struct RME_Cap_Pgt* Pgt_Child);
__EXTERN__ rme_ptr_t __RME_Pgt_Lookup(struct RME_Cap_Pgt* Pgt_Op, rme_ptr_t Pos, rme_ptr_t* Paddr, rme_ptr_t* Flags);
__EXTERN__ rme_ptr_t __RME_Pgt_Walk(struct RME_Cap_Pgt* Pgt_Op, rme_ptr_t Vaddr, rme_ptr_t* Pgt,
                                      rme_ptr_t* Map_Vaddr, rme_ptr_t* Paddr, rme_ptr_t* Size_Order, rme_ptr_t* Num_Order, rme_ptr_t* Flags);
/*****************************************************************************/
/* Undefine "__EXTERN__" to avoid redefinition */
#undef __EXTERN__
/* __RME_PLATFORM_X64_MEMBER__ */
#endif
/* !(defined __HDR_DEF__||defined __HDR_STRUCT__) */
#endif
/* End Public Function *******************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
