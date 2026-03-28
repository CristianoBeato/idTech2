
#ifndef __GL_DRAW_HPP__
#define __GL_DRAW_HPP__

typedef struct
{
    float   x;
    float   y;
    float   z;  /// 96
    float   s;
    float   t; /// 160
    byte    r;
    byte    g;
    byte    b;
    byte    a; /// 192 
} drawVertex_t;  // 16 * 12

typedef struct 
{
    GLuint  meshesVAO;
    GLuint  vertexBuffer;
    GLuint  indexBuffer;
} draw_t;

extern draw_t vaos;

typedef struct 
{
    GLuint  vertexArray;
    GLuint  renderProgram;

} backend_t;

extern backend_t backend;

typedef struct image_s image_t;

class glDraw
{
public:
    glDraw( void );
    ~glDraw( void );

    void        InitLocal ( void );
    void        SetGL2D (void);
    void        DrawColoredStereoLinePair( float r, float g, float b, float y );
    void        DrawStereoPattern( void );
    void        DrawChar ( int x, int y, int num );
    image_t*    FindPic ( const char *name );
    void        GetPicSize ( int *w, int *h, const char *pic);
    void        StretchPic ( int x, int y, int w, int h, const char *pic );
    void        DrawPic ( int x, int y, const char *pic );
    void        TileClear (int x, int y, int w, int h, const char *pic);
    void        Fill ( int x, int y, int w, int h, int c );
    void        FadeScreen ( void );
    void        StretchRaw (int x, int y, int w, int h, int cols, int rows, byte *data );
    int		    GetPalette (void);

private:
    bool    m_scrap_dirty;

    void Scrap_Upload (void);
};

static void R_CreateBuffers( void );
static void R_DestroyBuffers( void );
static void R_CreatePrograms( void );
static void R_DestroyPrograms( void );
static void R_CreateVAOS( void );
static void R_DestroyVAOS( void );

#endif //!__GL_DRAW_H__