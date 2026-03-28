
#ifndef __GL_IMP_HPP__
#define __GL_IMP_HPP__

class SDLGLimp
{
public:
    SDLGLimp( void );
    ~SDLGLimp( void );

    void		BeginFrame( const float camera_separation );
    void		EndFrame( void );
    int 		Init( void );
    void		Shutdown( void );
    int     	SetMode( unsigned *pwidth, unsigned *pheight, const int mode, const bool fullscreen );
    void		AppActivate( const bool active );
    void		EnableLogging( const bool enable );
    void		LogNewFrame( void );
    void        LoadLibary( const char* name );
    void*		GetProcAddress( const char* name ) const;

private:
};

#endif //!__GL_IMP_HPP__