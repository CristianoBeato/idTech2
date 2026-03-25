#include "ref_gl/gl_local.h"

#include <SDL3/SDL_video.h>

static struct glimp_SDL3
{
    SDL_GLContext   context;
} GLimp;

void		GLimp_BeginFrame( float camera_separation )
{
}

void		GLimp_EndFrame( void )
{
}

int 		GLimp_Init( void *hinstance, void *hWnd )
{
    /// context version 1.0
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );

    /// compatibility
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );

    /// defalt frame buffer color dpeth
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    GLimp.context = SDL_GL_CreateContext( (SDL_Window*)hWnd );
    if( !GLimp.context )
    {
        Sys_Error( SDL_GetError() );
        return 0;
    }

    return 1;
}

void		GLimp_Shutdown( void )
{
    if( GLimp.context != NULL )
    {
        SDL_GL_DestroyContext( GLimp.context );
        GLimp.context = NULL;
    }
}

int     	GLimp_SetMode( int *pwidth, int *pheight, int mode, bool fullscreen )
{
}

void		GLimp_AppActivate( bool active )
{
}

void		GLimp_EnableLogging( bool enable )
{
}

void		GLimp_LogNewFrame( void )
{
}

