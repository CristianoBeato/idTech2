#include "ref_gl/gl_local.h"
#include "SDL3_shared.h"
#include <SDL3/SDL_video.h>

SDL_glimp_t GLimp;

void		GLimp_BeginFrame( float camera_separation )
{
}

void		GLimp_EndFrame( void )
{
}

int 		GLimp_Init( void )
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
    GLimp.context = SDL_GL_CreateContext( video.window );
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

    SDL_GL_UnloadLibrary();
}

int GLimp_SetMode( int *pwidth, int *pheight, int mode, bool fullscreen )
{
    if ( mode < 0 || mode >= VID_NUM_MODES )
        return rserr_invalid_mode;

    *pwidth  = vid_modes[mode].width;
    *pheight = vid_modes[mode].height;

    if ( fullscreen )
    {
        // TODO:
        SDL_SetWindowSize( video.window, *pwidth, *pheight );
    }
    else
    {
        SDL_SetWindowSize( video.window, *pwidth, *pheight );
    }



    return rserr_ok;
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

void    GLimp_LoadLibary( const char* name )
{
    if( !SDL_GL_LoadLibrary( name ) )
        Com_Error( ERR_FATAL, SDL_GetError() );
}

void*		GLimp_GetProcAddress( const char* name )
{
    SDL_FunctionPointer proc = SDL_GL_GetProcAddress( name );
    assert( proc != NULL && name );
    return SDL_reinterpret_cast( void*, proc );
}
