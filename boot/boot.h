/* ELF32二进制文件头 */
struct ELFHeader {
    unsigned int   magic;
    unsigned char  elf[12];
    unsigned short type;
    unsigned short machine;
    unsigned int   version;
    unsigned int   entry;
    unsigned int   phoff;
    unsigned int   shoff;
    unsigned int   flags;
    unsigned short ehsize;
    unsigned short phentsize;
    unsigned short phnum;
    unsigned short shentsize;
    unsigned short shnum;
    unsigned short shstrndx;
};
/* ELF32 Program header */
struct ProgramHeader {
    unsigned int type;
    unsigned int off;
    unsigned int vaddr;
    unsigned int paddr;
    unsigned int filesz;
    unsigned int memsz;
    unsigned int flags;
    unsigned int align;
};
static __inline char
inb(int port)
{
	char data;
	__asm __volatile("inb %w1,%0" : "=a" (data) : "d" (port));
	return data;
}
static __inline int
inl(int port)
{
	int data;
	__asm __volatile("inl %w1,%0" : "=a" (data) : "d" (port));
	return data;
}
static __inline void
insl(int port, void *addr, int cnt)
{
	__asm __volatile("cld\n\trepne\n\tinsl"			:
			 "=D" (addr), "=c" (cnt)		:
			 "d" (port), "0" (addr), "1" (cnt)	:
			 "memory", "cc");
}
static __inline void
outb(int port, char data)
{
	__asm __volatile("outb %0,%w1" : : "a" (data), "d" (port));
}
