
#include "q_shared.h"

#if defined __linux__

#define __USE_GNU
#include <sys/mman.h>
#include <errno.h>
//===============================================================================

byte *membase;
size_t maxhunksize;
size_t curhunksize;

void *Hunk_Begin( const size_t maxsize )
{
	// reserve a huge chunk of memory, but don't commit any yet
	maxhunksize = maxsize + sizeof(int);
	curhunksize = 0;
	membase = mmap(0, maxhunksize, PROT_READ | PROT_WRITE,  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (membase == NULL || membase == (byte *)-1)
		Sys_Error("unable to virtual allocate %d bytes", maxsize);

	*((int *)membase) = curhunksize;

	return membase + sizeof(int);
}

void *Hunk_Alloc ( size_t size)
{
	byte *buf;

	// round to cacheline
	size = (size+31)&~31;
	if (curhunksize + size > maxhunksize)
		Sys_Error("Hunk_Alloc overflow");
	buf = membase + sizeof(int) + curhunksize;
	curhunksize += size;
	return buf;
}

size_t Hunk_End (void)
{
	byte *n;

	n = (byte*)mremap( membase, maxhunksize, curhunksize + sizeof(int), 0);
	if (n != membase)
		Sys_Error("Hunk_End:  Could not remap virtual block (%d)", errno);
	*((int *)membase) = curhunksize + sizeof(int);
	
	return curhunksize;
}

void Hunk_Free (void *base)
{
	byte *m;

	if (base) {
		m = ((byte *)base) - sizeof(int);
		if (munmap(m, *((int *)m)))
			Sys_Error("Hunk_Free: munmap failed (%d)", errno);
	}
}

//===============================================================================
#elif defined _WIN32 

#include "win32/winquake.h"

//===============================================================================

int		hunkcount;


byte	*membase;
size_t	hunkmaxsize;
size_t	cursize;

#define	VIRTUAL_ALLOC

void *Hunk_Begin ( size_t maxsize)
{
	// reserve a huge chunk of memory, but don't commit any yet
	cursize = 0;
	hunkmaxsize = maxsize;
#ifdef VIRTUAL_ALLOC
	membase = VirtualAlloc (NULL, maxsize, MEM_RESERVE, PAGE_NOACCESS);
#else
	membase = malloc (maxsize);
	memset (membase, 0, maxsize);
#endif
	if (!membase)
		Sys_Error ("VirtualAlloc reserve failed");
	return (void *)membase;
}

void *Hunk_Alloc ( size_t size)
{
	void	*buf;

	// round to cacheline
	size = (size+31)&~31;

#ifdef VIRTUAL_ALLOC
	// commit pages as needed
//	buf = VirtualAlloc (membase+cursize, size, MEM_COMMIT, PAGE_READWRITE);
	buf = VirtualAlloc (membase, cursize+size, MEM_COMMIT, PAGE_READWRITE);
	if (!buf)
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &buf, 0, NULL);
		Sys_Error ("VirtualAlloc commit failed.\n%s", buf);
	}
#endif
	cursize += size;
	if (cursize > hunkmaxsize)
		Sys_Error ("Hunk_Alloc overflow");

	return (void *)(membase+cursize-size);
}

int Hunk_End (void)
{

	// free the remaining unused virtual memory
#if 0
	void	*buf;

	// write protect it
	buf = VirtualAlloc (membase, cursize, MEM_COMMIT, PAGE_READONLY);
	if (!buf)
		Sys_Error ("VirtualAlloc commit failed");
#endif

	hunkcount++;
//Com_Printf ("hunkcount: %i\n", hunkcount);
	return cursize;
}

void Hunk_Free (void *base)
{
	if ( base )
#ifdef VIRTUAL_ALLOC
		VirtualFree (base, 0, MEM_RELEASE);
#else
		free (base);
#endif

	hunkcount--;
}

//===============================================================================

#else
// TODO a error or just go fuck arround 
#endif 