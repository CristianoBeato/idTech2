

#include "SDL3_shared.h"
#include <SDL3/SDL_log.h>

# if SDL_PLATFORM_LINUX
#	include <signal.h>
#	include <limits.h>
#	include <sys/time.h>
#	include <sys/types.h>
#	include <unistd.h>
#	include <fcntl.h>
#	include <sys/ipc.h>
#	include <sys/shm.h>
#	include <sys/stat.h>
#	include <string.h>
#	include <ctype.h>
#	include <errno.h>
#	include <dlfcn.h>
#	include <unistd.h>
# elif SDL_PLATFORM_WINDOWS
#	include <windows.h> 
extern void InitConProc ( int argc, char **argv );
# endif 

cvar_t *nostdout;
extern cvar_t *dedicated;
bool stdin_active = true;

/// TODO: Windows only
static char			console_text[256];
static int			console_textlen;
static HANDLE		hinput, houtput;

void Sys_InitConsole( void )
{
	if (dedicated->value)
	{
		if (!AllocConsole ())
			Sys_Error ("Couldn't create dedicated server console");
		hinput = GetStdHandle (STD_INPUT_HANDLE);
		houtput = GetStdHandle (STD_OUTPUT_HANDLE);
	
		// let QHOST hook in
		InitConProc ( sys.argc, sys.argv );
	}
}

/*
================
Sys_ConsoleOutput
Print text to the dedicated console
================
*/
void Sys_ConsoleOutput (char *string)
{
#if SDL_PLATFORM_LINUX
	if (nostdout && nostdout->value)
		return;

#elif SDL_PLATFORM_WINDOWS
	DWORD	dummy;
	char	text[256];

	if (!dedicated || !dedicated->value)
		return;

	if (console_textlen)
	{
		text[0] = '\r';
		memset(&text[1], ' ', console_textlen);
		text[console_textlen+1] = '\r';
		text[console_textlen+2] = 0;
		WriteFile(houtput, text, console_textlen+2, &dummy, NULL);
	}

	WriteFile( houtput, string, strlen( string ), &dummy, NULL);

	if (console_textlen)
		WriteFile( houtput, console_text, console_textlen, &dummy, NULL);
#endif // SDL_PLATFORM_WINDOWS
	SDL_Log( string );
}

/*
================
Sys_ConsoleInput
================
*/
char *Sys_ConsoleInput(void)
{
    int len = 0;
    static char text[256];

	if (!dedicated || !dedicated->value)
		return NULL;

#if SDL_PLATFORM_LINUX
	fd_set	fdset;
    struct timeval timeout;

	if (!stdin_active)
		return NULL;

	FD_ZERO(&fdset);
	FD_SET(0, &fdset); // stdin
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	if (select (1, &fdset, NULL, NULL, &timeout) == -1 || !FD_ISSET(0, &fdset))
		return NULL;

	len = read (0, text, sizeof(text));
	if (len == 0) { // eof!
		stdin_active = false;
		return NULL;
	}

	if (len < 1)
		return NULL;
	text[len-1] = 0;    // rip off the /n and terminate
#elif SDL_PLATFORM_WINDOWS
	INPUT_RECORD	recs[1024];
	DWORD	dummy = 0, numread = 0, numevents = 0;
	int ch;

	for ( ;; )
	{
		if (!GetNumberOfConsoleInputEvents( hinput, &numevents ) )
			Sys_Error ("Error getting # of console events");

		if (numevents <= 0)
			break;

		if (!ReadConsoleInput(hinput, recs, 1, &numread))
			Sys_Error ("Error reading console input");

		if (numread != 1)
			Sys_Error ("Couldn't read console input");

		if (recs[0].EventType == KEY_EVENT)
		{
			if (!recs[0].Event.KeyEvent.bKeyDown)
			{
				ch = recs[0].Event.KeyEvent.uChar.AsciiChar;

				switch (ch)
				{
					case '\r':
						WriteFile(houtput, "\r\n", 2, &dummy, NULL);	

						if (console_textlen)
						{
							console_text[console_textlen] = 0;
							console_textlen = 0;
							return console_text;
						}
						break;

					case '\b':
						if (console_textlen)
						{
							console_textlen--;
							WriteFile(houtput, "\b \b", 3, &dummy, NULL);
						}
						break;

					default:
						if (ch >= ' ')
						{
							if (console_textlen < sizeof(console_text)-2)
							{
								WriteFile(houtput, &ch, 1, &dummy, NULL);
								console_text[console_textlen] = ch;
								console_textlen++;
							}
						}

						break;

				}
			}
		}
	}
#endif 
	return text;
}