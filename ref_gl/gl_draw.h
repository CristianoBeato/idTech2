
#ifndef __GL_DRAW_H__
#define __GL_DRAW_H__

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

static void R_CreateBuffers( void );
static void R_DestroyBuffers( void );
static void R_CreatePrograms( void );
static void R_DestroyPrograms( void );
static void R_CreateVAOS( void );
static void R_DestroyVAOS( void );

#endif //!__GL_DRAW_H__