// vid_null.c -- null video driver to aid porting efforts
// this assumes that one of the refs is statically linked to the executable

#include "../client/client.h"

#include <SDL3/SDL_loadso.h>
#include <SDL3/SDL_video.h>

static struct SDL_video_t
{
    const char*         ref_name[100];
    SDL_SharedObject*   ref_lib;
    SDL_Window*         window;
} video;


viddef_t	viddef;				// global video state
cvar_t		*vid_ref;			// Name of Refresh DLL loaded
cvar_t		*vid_xpos;			// X coordinate of window position
cvar_t		*vid_ypos;			// Y coordinate of window position
cvar_t		*vid_fullscreen;

refexport_t	re;

refexport_t GetRefAPI (refimport_t rimp);

/*
==========================================================================

DIRECT LINK GLUE

==========================================================================
*/
/*
============
VID_Restart_f

Console command to re-start the video mode and refresh DLL. We do this
simply by setting the modified flag for the vid_ref variable, which will
cause the entire video mode and refresh DLL to be reset on the next frame.
============
*/
void VID_Restart_f (void)
{
	vid_ref->modified = true;
}

#define	MAXPRINTMSG	4096
void VID_Printf (int print_level, char *fmt, ...)
{
        va_list		argptr;
        char		msg[MAXPRINTMSG];

        va_start (argptr,fmt);
        vsprintf (msg,fmt,argptr);
        va_end (argptr);

        if (print_level == PRINT_ALL)
                Com_Printf ("%s", msg);
        else
                Com_DPrintf ("%s", msg);
}

void VID_Error (int err_level, char *fmt, ...)
{
        va_list		argptr;
        char		msg[MAXPRINTMSG];

        va_start (argptr,fmt);
        vsprintf (msg,fmt,argptr);
        va_end (argptr);

		Com_Error (err_level, "%s", msg);
}

void VID_NewWindow (int width, int height)
{
    if( video.window )
    {
        SDL_DestroyWindow( video.window );
        video.window = NULL;
    }

    viddef.width = width;
    viddef.height = height;

    video.window = SDL_CreateWindow( "SDL3-Quake2", width, height, SDL_WINDOW_OPENGL );
    if( !video.window )
        Sys_Error( "Failed to create main window %s\n", SDL_GetError() );
}

/*
** VID_UpdateWindowPosAndSize
*/
void VID_UpdateWindowPosAndSize( int x, int y )
{
    if ( video.window == NULL )
        return;    

    SDL_SetWindowPosition( video.window, x, y );
}

/*
** VID_GetModeInfo
*/
typedef struct vidmode_s
{
    const char *description;
    int         width, height;
    int         mode;
} vidmode_t;

vidmode_t vid_modes[] =
{
    { "Mode 0: 320x240",   320, 240,   0 },
    { "Mode 1: 400x300",   400, 300,   1 },
    { "Mode 2: 512x384",   512, 384,   2 },
    { "Mode 3: 640x480",   640, 480,   3 },
    { "Mode 4: 800x600",   800, 600,   4 },
    { "Mode 5: 960x720",   960, 720,   5 },
    { "Mode 6: 1024x768",  1024, 768,  6 },
    { "Mode 7: 1152x864",  1152, 864,  7 },
    { "Mode 8: 1280x960",  1280, 960, 8 },
    { "Mode 9: 1600x1200", 1600, 1200, 9 }
};
#define VID_NUM_MODES ( sizeof( vid_modes ) / sizeof( vid_modes[0] ) )

bool VID_GetModeInfo( int *width, int *height, int mode )
{
    // TODO List display types whit SDL 

    if ( mode < 0 || mode >= VID_NUM_MODES )
        return false;

    *width  = vid_modes[mode].width;
    *height = vid_modes[mode].height;

    return true;
}

static bool VID_LoadRefresh( const char *name )
{
    if ( video.ref_lib != NULL )
	{
		re.Shutdown();
        if( video.ref_lib != NULL )
        {
            SDL_UnloadObject( video.ref_lib );
            video.ref_lib = NULL;
        }
	}

	Com_Printf( "------- Loading %s -------\n", name );

    video.ref_lib = SDL_LoadObject( name );
    if( video.ref_lib )
    {
        Sys_Error( SDL_GetError() );
        return false;
    }

    return true;
}

void	VID_Init (void)
{
    char name[128];
    refimport_t	ri;
    GetRefAPI_t	GetRefAPI;

    /* Create the video variables so we know how to start the graphics drivers */
	vid_ref = Cvar_Get ("vid_ref", "soft", CVAR_ARCHIVE);
	vid_xpos = Cvar_Get ("vid_xpos", "3", CVAR_ARCHIVE);
	vid_ypos = Cvar_Get ("vid_ypos", "22", CVAR_ARCHIVE);
	vid_fullscreen = Cvar_Get ("vid_fullscreen", "0", CVAR_ARCHIVE);
	//vid_gamma = Cvar_Get( "vid_gamma", "1", CVAR_ARCHIVE );

    Com_sprintf( name, sizeof(name), "ref_%s.dll", vid_ref->string );
    if( !VID_LoadRefresh( name ) )
        Sys_Error( "Can't load render library %s\n", name );

    viddef.width = 320;
    viddef.height = 240;

    ri.Cmd_AddCommand = Cmd_AddCommand;
    ri.Cmd_RemoveCommand = Cmd_RemoveCommand;
    ri.Cmd_Argc = Cmd_Argc;
    ri.Cmd_Argv = Cmd_Argv;
    ri.Cmd_ExecuteText = Cbuf_ExecuteText;
    ri.Con_Printf = VID_Printf;
    ri.Sys_Error = VID_Error;
    ri.FS_LoadFile = FS_LoadFile;
    ri.FS_FreeFile = FS_FreeFile;
    ri.FS_Gamedir = FS_Gamedir;
	ri.Vid_NewWindow = VID_NewWindow;
    ri.Cvar_Get = Cvar_Get;
    ri.Cvar_Set = Cvar_Set;
    ri.Cvar_SetValue = Cvar_SetValue;
    ri.Vid_GetModeInfo = VID_GetModeInfo;

    if ( ( GetRefAPI = (void *) SDL_LoadFunction( video.ref_lib, "GetRefAPI" ) ) == 0 )
		Com_Error( ERR_FATAL, "GetProcAddress failed on %s", video.ref_name );

    re = GetRefAPI(ri);

    if (re.api_version != API_VERSION)
        Com_Error ( ERR_FATAL, "Re has incompatible api_version");
    
        // call the init function
    if (re.Init (NULL, (void*)video.window ) == -1)
		Com_Error ( ERR_FATAL, "Couldn't start refresh");
}

void	VID_Shutdown (void)
{
    if (re.Shutdown)
	    re.Shutdown ();

    /// Release renderer lib
    if( video.ref_lib != NULL )
    {
        SDL_UnloadObject( video.ref_lib );
        video.ref_lib = NULL;
    }
}

void	VID_CheckChanges (void)
{
}

void	VID_MenuInit (void)
{
}

void	VID_MenuDraw (void)
{
}

const char *VID_MenuKey( int k)
{
	return NULL;
}
