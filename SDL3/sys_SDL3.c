// sys_null.h -- null system driver to aid porting efforts

#include "qcommon/qcommon.h"
#include "errno.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int curtime;

#if SDL_PLATFORM_WINDOWS
#	define SHARED_LIB_EXT "dll"
#elif SDL_PLATFORM_LINUX
#	define SHARED_LIB_EXT "so"
#endif 
 
static struct sys_SDL3
{
	
	unsigned int		sys_frame_time;
	SDL_SharedObject*	game_library;	
} sys;

void Sys_Error ( const char *error, ...)
{
	char		text[1024];
	va_list		argptr;

	CL_Shutdown ();
	Qcommon_Shutdown ();

	va_start (argptr, error);
	vsprintf ( text, error, argptr );
	va_end (argptr);

	SDL_Log ("Sys_Error: ");	
	SDL_Log( text );
	SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "Error:", text, NULL );

	exit ( EXIT_FAILURE );
}

void Sys_Quit (void)
{
	Qcommon_Shutdown ();

	exit ( EXIT_FAILURE );
}

void	Sys_UnloadGame (void)
{
	if( sys.game_library != NULL )
	{
		SDL_UnloadObject( sys.game_library );
		sys.game_library = NULL;
	}
}

void	*Sys_GetGameAPI (void *parms)
{
	void	*(*GetGameAPI) (void *);

	char	name[MAX_OSPATH];
	const char	*curpath;
	char	*path;

	const char *gamename = "game";
	if (sys.game_library)
		Com_Error (ERR_FATAL, "Sys_GetGameAPI without Sys_UnloadingGame");

#if 0
	SDL_GetCurrentDirectory
	getcwd(curpath, sizeof(curpath));
#else
	curpath = Sys_cwd();
#endif 

	Com_Printf("------- Loading %s -------", gamename);

	// now run through the search paths
	path = NULL;
	while (1)
	{
		path = FS_NextPath (path);
		if (!path)
			return NULL;		// couldn't find one anywhere
		
		sprintf( name, "%s/%s/%s.%s", curpath, path, gamename, SHARED_LIB_EXT );
		sys.game_library = SDL_LoadObject( name );
		if ( sys.game_library )
		{
			Com_DPrintf ("LoadLibrary (%s)\n",name);
			break;
		}
	}

	GetGameAPI = (void *)SDL_LoadFunction( sys.game_library, "GetGameAPI");
	if (!GetGameAPI)
	{
		Sys_UnloadGame ();		
		return NULL;
	}

	return GetGameAPI( parms );
}

const char *Sys_ConsoleInput (void)
{
	return NULL;
}

void	Sys_ConsoleOutput ( const char *string )
{
	SDL_Log( string );
}

void Sys_SendKeyEvents (void)
{
	SDL_Event e;
	while ( SDL_PollEvent( &e ) )
	{
	}
	
}

void Sys_AppActivate (void)
{
}

void Sys_CopyProtect (void)
{
}

const char *Sys_GetClipboardData( void )
{
	const char* clipboard = NULL;
	if( SDL_HasClipboardText() )
	{
		const char* SDLclipboard = SDL_GetClipboardText();
		size_t len = strlen( SDLclipboard );
		clipboard = memset( malloc( len + 1 ), 0x00, len + 1 ); // set to null ( '\0' )
		strncpy( clipboard, SDLclipboard, strlen( clipboard ) );
		SDL_free( (void*)SDLclipboard );
	}

	return clipboard;
}

int Sys_Milliseconds ( void )
{
	curtime = SDL_GetTicks();
	return curtime;
}

static char CURRENT_WORKIN_DIR[MAX_OSPATH];
const char* Sys_cwd( void )
{
	if( CURRENT_WORKIN_DIR[0] == '\0' )
	{
		const char* path = SDL_GetCurrentDirectory();
		strncpy( CURRENT_WORKIN_DIR, path, MAX_OSPATH );
		SDL_free( path );
	}

	return CURRENT_WORKIN_DIR;
}

void	Sys_Mkdir ( const char *path )
{
	if( !SDL_CreateDirectory( path ) )
		Sys_Error( "Failed to create path %s\n" );
}

static	char	findbase[MAX_OSPATH];
static	char	findpath[MAX_OSPATH];
static	char	findpattern[MAX_OSPATH];
const char	*Sys_FindFirst ( const char *path, unsigned musthave, unsigned canthave)
{
	return NULL;
}

const char	*Sys_FindNext (unsigned musthave, unsigned canthave)
{
	return NULL;
}

void	Sys_FindClose (void)
{
}

void	Sys_Init (void)
{
	VID_NewWindow( 800, 600 );
}


//=============================================================================

int main ( int argc, char **argv )
{
	if( !SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD ) )
		Sys_Error( SDL_GetError() );

	Qcommon_Init (argc, argv);

	while (1)
	{
		Qcommon_Frame (0.1);
	}

	return 0;
}


