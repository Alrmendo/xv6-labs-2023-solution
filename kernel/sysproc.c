#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  int start_va, num_pages;
  uint64 buffer_addr;

  argint(0, &start_va);
  argint(1, &num_pages);
  argaddr(2, &buffer_addr);
    
  uint64 buf = 0; // Initialize buffer to 0
  pte_t *pte;

  struct proc *p = myproc(); // Get current process
  for (int i = 0; i < num_pages; i++) {
    uint64 va = (uint64)(start_va + i * PGSIZE); // Cast to uint64
    pte = walk(p->pagetable, va, 0);

    if (*pte & PTE_V && *pte & PTE_A) {
      buf |= (1ULL << i); // Set the corresponding bit in the buffer if page is accessed
      *pte &= ~PTE_A; // Clear the access bit
    }
  }
  copyout(p->pagetable, buffer_addr, (char *)&buf, sizeof(buf)); // Cast buffer_addr to uint64
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}