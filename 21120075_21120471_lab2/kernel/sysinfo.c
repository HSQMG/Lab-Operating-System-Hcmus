#include "types.h"
#include "riscv.h"
#include "param.h"
#include "spinlock.h"
#include "defs.h"
#include "sysinfo.h"
#include "proc.h"

// Hàm lấy thông tin hệ thống hiện tại
// addr là một địa chỉ ảo của người dùng, trỏ đến một cấu trúc sysinfo.
int
systeminfo(uint64 addr) {
  struct proc *p = myproc();
  struct sysinfo info;

  info.freemem = getfreemem();
  info.nproc = get_nproc();

  if(copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0)
    return -1;
  return 0;
}
