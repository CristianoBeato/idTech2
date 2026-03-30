
#ifndef __RMAIN_HPP__
#define __RMAIN_HPP__

class glRenderer
{
public:
    glRenderer( void );
    ~glRenderer( void );

    int 	Init( void );
    void	Shutdown( void );

    void	BeginFrame( float camera_separation );
    void	SwapBuffers( int );
    void	SetPalette ( const unsigned char *palette);
    void    RenderFrame (refdef_t *fd );
    void    RenderView (refdef_t *fd);
    void    ScreenShot_f (void);
    void    DrawEntitiesOnList (void);
    void    DrawNullModel (void);
    void    DrawInlineBModel (void);
    void    DrawAliasModel (entity_t *e);
    void    DrawBrushModel (entity_t *e);
    void    DrawSpriteModel (entity_t *e);
    void    DrawParticles( int num_particles, const particle_t particles[], const unsigned colortable[768] );
    void    DrawParticles (void);
    void    DrawBeam( entity_t *e );
    void    DrawWorld (void);
    void    RenderDlights (void);
    void    BlendLightmaps( void );
    void    BuildLightMap (msurface_t *surf, byte *dest, int stride);
    void    DrawAlphaSurfaces (void);
    void    DrawTextureChains (void);
    void    RenderBrushPoly (msurface_t *fa);
    void    InitParticleTexture (void);
    bool    SetMode (void);
    void    Register( void );

    bool    CullAliasModel( vec3_t bbox[8], entity_t *e );
    void    RotateForEntity (entity_t *e);
    void    MarkLeaves (void);
    bool    CullBox ( vec3_t mins, vec3_t maxs );

    glDraw  Draw( void ) const { return m_draw; }

private:
    viddef_t	m_vid;
    cplane_t	m_frustum[4];
    glDraw      m_draw;

    void    PolyBlend( void );
    void    SetFrustum (void);
    void    SetupFrame ( void );
    void    Clear( void );
    void    Flash( void );
    void    SetLightLevel (void);
    void    RecursiveWorldNode ( mnode_t *node );
    void    RenderLightmappedPoly( msurface_t *surf );
    bool    QGL_Init( const char *dllname );
    void    QGL_Shutdown( void );
    void	GL_InitImages (void);
    void    GL_ShutdownImages (void);
};

extern glRenderer gRenderer;

#endif //!__RMAIN_HPP__