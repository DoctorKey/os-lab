//#include "x86.h"
#include "boot.h"
#define SECTSIZE 512

void readseg(unsigned char *, int, int);
void waitdisk(void)
{
    // wait for disk reaady
    while ((inb(0x1F7) & 0xC0) != 0x40);
        // do nothing;
}

void readsect(void *dst, int offset)
{
    // wait for disk to be ready
    waitdisk();

    outb(0x1F2, 1);     // count = 1
    outb(0x1F3, offset);    //address = offset | 0xe0000000
    outb(0x1F4, offset >> 8);
    outb(0x1F5, offset >> 16);
    outb(0x1F6, (offset >> 24) | 0xE0);
    outb(0x1F7, 0x20);  // cmd 0x20 - read sectors

    // wait for disk to be ready
    waitdisk();

    // read a sector
    insl(0x1F0, dst, SECTSIZE/4);
}

void bootmain(void)
{
	struct ELFHeader *elf;
	struct ProgramHeader *ph, *eph;
	unsigned char* pa, *i;

	/* 因为引导扇区只有512字节，我们设置了堆栈从0x8000向下生长。
	 * 我们需要一块连续的空间来容纳ELF文件头，因此选定了0x8000。 */
	elf = (struct ELFHeader*)0x8000;

	/* 读入ELF文件头 */
	readseg((unsigned char*)elf, 4096, 0);

	/* 把每个program segement依次读入内存 */
	ph = (struct ProgramHeader*)((char *)elf + elf->phoff);
	eph = ph + elf->phnum;
	for(; ph < eph; ph ++) {
		pa = (unsigned char*)ph->paddr; /* 获取物理地址 */
		readseg(pa, ph->filesz, ph->off); /* 读入数据 */
		for (i = pa + ph->filesz; i < pa + ph->memsz; *i ++ = 0);
	}

	((void(*)(void))elf->entry)();
}
/* 将位于磁盘offset位置的count字节数据读入物理地址pa */
void
readseg(unsigned char *pa, int count, int offset) {
	unsigned char *epa;
	epa = pa + count;
	pa -= offset % SECTSIZE;
	offset = (offset / SECTSIZE) + 1;
	for(; pa < epa; pa += SECTSIZE, offset ++)
		readsect(pa, offset);
}
