
#ifndef __SDL3_SHARED_H__
#define __SDL3_SHARED_H__

#include "SDL3/SDL_stdinc.h"

#if SDL_PLATFORM_WINDOWS
#	define SHARED_LIB_EXT "dll"
#elif SDL_PLATFORM_LINUX
#	define SHARED_LIB_EXT "so"
#endif
 
extern const char* Sys_cwd( void );

extern void VID_NewWindow ( int width, int height );

/*
====================================================================

IMPLEMENTATION SPECIFIC FUNCTIONS

====================================================================
*/

void		GLimp_BeginFrame( float camera_separation );
void		GLimp_EndFrame( void );
int 		GLimp_Init( void *hinstance, void *hWnd );
void		GLimp_Shutdown( void );
int     	GLimp_SetMode( int *pwidth, int *pheight, int mode, bool fullscreen );
void		GLimp_AppActivate( bool active );
void		GLimp_EnableLogging( bool enable );
void		GLimp_LogNewFrame( void );
void*		GLimp_GetProcAddress( const char* name );

#endif //!__SDL3_SHARED_H__