
#ifndef __SDL3_SHARED_H__
#define __SDL3_SHARED_H__

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_loadso.h>
#include <SDL3/SDL_video.h>


#if SDL_PLATFORM_WINDOWS
#	define SHARED_LIB_EXT "dll"
#elif SDL_PLATFORM_LINUX
#	define SHARED_LIB_EXT "so"
#endif
 

/*
** VID_GetModeInfo
*/
typedef struct vidmode_s
{
    const char *description;
    int         width, height;
    int         mode;
} vidmode_t;

#define VID_NUM_MODES 10

extern vidmode_t vid_modes[VID_NUM_MODES];

typedef struct
{
    SDL_GLContext   context;
} SDL_glimp_t;

typedef struct 
{
    const char*         ref_name[100];
    SDL_SharedObject*   ref_lib;
    SDL_Window*         window;
} SDL_video_t;

extern SDL_glimp_t GLimp;
extern SDL_video_t video;

extern const char* Sys_cwd( void );

extern void VID_NewWindow ( int width, int height );

/*
====================================================================

IMPLEMENTATION SPECIFIC FUNCTIONS

====================================================================
*/

void		GLimp_BeginFrame( float camera_separation );
void		GLimp_EndFrame( void );
int 		GLimp_Init( void );
void		GLimp_Shutdown( void );
int     	GLimp_SetMode( int *pwidth, int *pheight, int mode, bool fullscreen );
void		GLimp_AppActivate( bool active );
void		GLimp_EnableLogging( bool enable );
void		GLimp_LogNewFrame( void );
void        GLimp_LoadLibary( const char* name );
void*		GLimp_GetProcAddress( const char* name );

#endif //!__SDL3_SHARED_H__