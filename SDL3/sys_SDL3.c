// sys_null.h -- null system driver to aid porting efforts

#include "qcommon/qcommon.h"
#include "errno.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3_shared.h"

int curtime;
unsigned int	sys_frame_time;

static struct sys_SDL3
{
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

	const char	name[MAX_OSPATH];
	const char	*curpath;
	const char	*path;

#if SDL_PLATFORM_LINUX
	const char *gamename = "libgame";
#elif SDL_PLATFORM_WINDOWS
	const char *gamename = "game";
#endif 
	if (sys.game_library)
		Com_Error (ERR_FATAL, "Sys_GetGameAPI without Sys_UnloadingGame");

	curpath = Sys_cwd();

	Com_Printf("------- Loading %s -------", gamename);

	// now run through the search paths
	path = NULL;
	while (1)
	{
		path = FS_NextPath ( path );
		if (!path)
			return NULL;		// couldn't find one anywhere
		
#if 1 // TODO On debuf mode
	snprintf( name, MAX_OSPATH, "%s/%s.%s", path, gamename, SHARED_LIB_EXT );
#else
	snprintf( name, MAX_OSPATH, "%s/%s.%s", path, gamename, SHARED_LIB_EXT );
#endif
		Com_Printf( "Trying to load game lib %s\n", name );
		sys.game_library = SDL_LoadObject( name );
		if ( sys.game_library )
		{
			Com_DPrintf ("LoadLibrary (%s)\n",name);
			break;
		}
	}

	if( sys.game_library == NULL )
	{
		///TODO: drop a fatal error
		Com_Error( ERR_FATAL, "Failed to load LoadLibrary %s\n", gamename  );
	}

	GetGameAPI = (void *)SDL_LoadFunction( sys.game_library, "GetGameAPI");
	if (!GetGameAPI)
	{
		Sys_UnloadGame ();		
		return NULL;
	}

	return GetGameAPI( parms );
}

void Sys_SendKeyEvents (void)
{
	SDL_Event e;
	while ( SDL_PollEvent( &e ) )
	{
	}

	sys_frame_time = SDL_GetTicks();	
}

/*
=================
Sys_AppActivate
=================
*/
void Sys_AppActivate (void)
{
	SDL_RestoreWindow( video.window );
	SDL_ShowWindow( video.window );
}

void Sys_CopyProtect (void)
{
}

/*
================
Sys_GetClipboardData
================
*/
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

/*
================
Sys_Milliseconds
================
*/
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

		CURRENT_WORKIN_DIR[strlen( CURRENT_WORKIN_DIR ) - 1] = '\0';
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
	//GLimp_LoadLibary( NULL );
	//VID_NewWindow( 800, 600 );
}


//=============================================================================

int main ( int argc, char **argv )
{
	if( !SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD ) )
		Sys_Error( SDL_GetError() );

	Qcommon_Init (argc, argv);

	while (1)
	{
		Qcommon_Frame ( 16 );
	}

	return 0;
}


