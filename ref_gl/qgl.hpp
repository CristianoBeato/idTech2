/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
/*
** QGL.H
*/

#ifndef __QGL_H__
#define __QGL_H__

#ifdef _WIN32
#  include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>

typedef GLenum ( APIENTRYP PFNGLGETERRORPROC )(void);

typedef void ( APIENTRYP PFNGLFINISHPROC )(void);
typedef void ( APIENTRYP PFNGLFLUSHPROC )(void);

typedef void ( APIENTRYP PFNGLENABLEPROC )(GLenum cap);
typedef void ( APIENTRYP PFNGLDISABLEPROC )(GLenum cap);

typedef void ( APIENTRYP PFNGLGETFLOATVPROC )(GLenum pname, GLfloat *params);
typedef const GLubyte * ( APIENTRYP PFNGLGETSTRINGPROC )(GLenum name);

typedef void ( APIENTRYP PFNGLDRAWBUFFERPROC )(GLenum mode);

typedef void ( APIENTRYP PFNGLREADPIXELSPROC )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);

typedef void ( APIENTRYP PFNGLPOINTSIZEPROC )(GLfloat size);

typedef void ( APIENTRYP PFNGLCLEARCOLORPROC )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void ( APIENTRYP PFNGLCLEARDEPTHPROC )(GLclampd depth);
typedef void ( APIENTRYP PFNGLCLEARPROC )(GLbitfield mask);

typedef void ( APIENTRYP PFNGLVIEWPORTPROC )(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void ( APIENTRYP PFNGLSCISSORPROC )(GLint x, GLint y, GLsizei width, GLsizei height);

typedef void ( APIENTRYP PFNGLDEPTHFUNCPROC )(GLenum func);
typedef void ( APIENTRYP PFNGLDEPTHMASKPROC )(GLboolean flag);
typedef void ( APIENTRYP PFNGLDEPTHRANGEPROC )(GLclampd zNear, GLclampd zFar);

typedef void ( APIENTRYP PFNGLCULLFACEPROC )(GLenum mode);

typedef void ( APIENTRYP PFNGLPOLYGONMODEPROC )(GLenum face, GLenum mode);

typedef void ( APIENTRYP PFNGLBINDTEXTUREPROC )(GLenum target, GLuint texture);
typedef void ( APIENTRYP PFNGLGENTEXTURESPROC )(GLsizei n, GLuint *textures);
typedef void ( APIENTRYP PFNGLDELETETEXTURESPROC )(GLsizei n, const GLuint *textures);
typedef void ( APIENTRYP PFNGLTEXIMAGE1DPROC )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void ( APIENTRYP PFNGLTEXIMAGE2DPROC )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void ( APIENTRYP PFNGLTEXPARAMETERFPROC )(GLenum target, GLenum pname, GLfloat param);
typedef void ( APIENTRYP PFNGLTEXPARAMETERFVPROC )(GLenum target, GLenum pname, const GLfloat *params);
typedef void ( APIENTRYP PFNGLTEXPARAMETERIPROC )(GLenum target, GLenum pname, GLint param);
typedef void ( APIENTRYP PFNGLTEXPARAMETERIVPROC )(GLenum target, GLenum pname, const GLint *params);
typedef void ( APIENTRYP PFNGLTEXSUBIMAGE1DPROC )(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
typedef void ( APIENTRYP PFNGLTEXSUBIMAGE2DPROC )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);

typedef void ( APIENTRYP PFNGLTEXPARAMETERFPROC )(GLenum target, GLenum pname, GLfloat param);

typedef void ( APIENTRYP PFNGLBLENDFUNCPROC )(GLenum sfactor, GLenum dfactor);

typedef void ( APIENTRYP PNFGLMATRIXMODEPROC )(GLenum mode);
typedef void ( APIENTRYP PNFGLLOADIDENTITYPROC )(void);
typedef void ( APIENTRYP PFNGLLOADMATRIXFPROC )(const GLfloat *m);
typedef void ( APIENTRYP PFNGLPUSHMATRIXPROC )(void);
typedef void ( APIENTRYP PFNGLPOPMATRIXPROC )(void);
typedef void ( APIENTRYP PNFGLORTHOPROC )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void ( APIENTRYP PFNGLFRUSTUMPROC )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

typedef void ( APIENTRYP PFNGLTRANSLATEFPROC )(GLfloat x, GLfloat y, GLfloat z);
typedef void ( APIENTRYP PFNGLROTATEFPROC )(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void ( APIENTRYP PFNGLSCALEFPROC )(GLfloat x, GLfloat y, GLfloat z);

typedef void ( APIENTRYP PFNGLVERTEXPOINTERPROC )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void ( APIENTRYP PFNGLENABLECLIENTSTATEPROC )(GLenum array);
typedef void ( APIENTRYP PFNGLCOLORPOINTERPROC )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void ( APIENTRYP PFNGLARRAYELEMENTPROC )(GLint i);

typedef void ( APIENTRYP PFNGLSHADEMODELPROC )(GLenum mode);

typedef void ( APIENTRYP PFNGLBEGINPROC )(GLenum mode);
typedef void ( APIENTRYP PFNGLENDPROC )(void);

typedef void ( APIENTRYP PFNGLTEXENVFPROC )(GLenum target, GLenum pname, GLfloat param);

typedef void ( APIENTRYP PFNGLDELETEBUFFERSPROC )( GLsizei n, const GLuint *buffers );

typedef void ( APIENTRYP PFNGLCOLOR3FPROC )(GLfloat red, GLfloat green, GLfloat blue);
typedef void ( APIENTRYP PFNGLCOLOR3FVPROC )(const GLfloat *v);
typedef void ( APIENTRYP PFNGLCOLOR4FPROC )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void ( APIENTRYP PFNGLCOLOR4FVPROC )(const GLfloat *v);
typedef void ( APIENTRYP PFMGLCOLOR4UBVPROC )(const GLubyte *v);
typedef void ( APIENTRYP PFNGLVERTEX3FPROC )(GLfloat x, GLfloat y, GLfloat z);
typedef void ( APIENTRYP PFNGLVERTEX3FVPROC )(const GLfloat *v);
typedef void ( APIENTRYP PFNGLVERTEX2FPROC )(GLfloat x, GLfloat y);
typedef void ( APIENTRYP PFNGLTEXCOORD2FPROC )(GLfloat s, GLfloat t);

typedef void ( APIENTRY * PFNGLALPHAFUNCPROC )(GLenum func, GLclampf ref);

extern PFNGLGETERRORPROC            qglGetError;

extern PFNGLFINISHPROC              qglFinish;
extern PFNGLFLUSHPROC               qglFlush;

extern PFNGLENABLEPROC              qglEnable;
extern PFNGLDISABLEPROC             qglDisable;

extern PFNGLGETFLOATVPROC           qglGetFloatv;
extern PFNGLGETSTRINGPROC           qglGetString;

extern PFNGLDRAWBUFFERPROC          qglDrawBuffer;

extern PFNGLREADPIXELSPROC          qglReadPixels;

extern PFNGLPOINTSIZEPROC           qglPointSize;

extern PFNGLCLEARCOLORPROC          qglClearColor;
extern PFNGLCLEARDEPTHPROC          qglClearDepth;
extern PFNGLCLEARPROC               qglClear;

extern PFNGLVIEWPORTPROC            qglViewport;
extern PFNGLSCISSORPROC             qglScissor;

extern PFNGLDEPTHFUNCPROC           qglDepthFunc;
extern PFNGLDEPTHMASKPROC           qglDepthMask;
extern PFNGLDEPTHRANGEPROC          qglDepthRange;

extern PFNGLCULLFACEPROC            qglCullFace;

extern PFNGLPOLYGONMODEPROC         qglPolygonMode;

extern PFNGLBINDTEXTUREPROC         qglBindTexture;
extern PFNGLGENTEXTURESPROC         qglGenTextures;
extern PFNGLDELETETEXTURESPROC      qglDeleteTextures;
extern PFNGLTEXIMAGE1DPROC          qglTexImage1D;
extern PFNGLTEXIMAGE2DPROC          qglTexImage2D;
extern PFNGLTEXPARAMETERFPROC       qglTexParameterf;
extern PFNGLTEXPARAMETERFVPROC      qglTexParameterfv;
extern PFNGLTEXPARAMETERIPROC       qglTexParameteri;
extern PFNGLTEXPARAMETERIVPROC      qglTexParameteriv;
extern PFNGLTEXSUBIMAGE1DPROC       qglTexSubImage1D;
extern PFNGLTEXSUBIMAGE2DPROC       qglTexSubImage2D;

extern PFNGLBLENDFUNCPROC           qglBlendFunc; 

extern PNFGLMATRIXMODEPROC          qglMatrixMode;
extern PNFGLLOADIDENTITYPROC        qglLoadIdentity;
extern PFNGLLOADMATRIXFPROC         qglLoadMatrixf;
extern PFNGLPUSHMATRIXPROC          qglPushMatrix;
extern PFNGLPOPMATRIXPROC           qglPopMatrix;
extern PNFGLORTHOPROC               qglOrtho;
extern PFNGLFRUSTUMPROC             qglFrustum;

extern PFNGLTRANSLATEFPROC          qglTranslatef;
extern PFNGLROTATEFPROC             qglRotatef;
extern PFNGLSCALEFPROC              qglScalef;

extern PFNGLENABLECLIENTSTATEPROC   qglEnableClientState;
extern PFNGLVERTEXPOINTERPROC       qglVertexPointer;
extern PFNGLCOLORPOINTERPROC        qglColorPointer;
extern PFNGLARRAYELEMENTPROC        qglArrayElement;

extern PFNGLSHADEMODELPROC          qglShadeModel;

extern PFNGLBEGINPROC               qglBegin;
extern PFNGLENDPROC                 qglEnd;

extern PFNGLTEXENVFPROC             qglTexEnvf;

extern PFNGLCOLOR3FPROC             qglColor3f;
extern PFNGLCOLOR3FVPROC            qglColor3fv;
extern PFNGLCOLOR4FPROC             qglColor4f;
extern PFNGLCOLOR4FVPROC            qglColor4fv;
extern PFMGLCOLOR4UBVPROC           qglColor4ubv;
extern PFNGLVERTEX3FPROC            qglVertex3f;
extern PFNGLVERTEX3FVPROC           qglVertex3fv;
extern PFNGLTEXCOORD2FPROC          qglTexCoord2f;
extern PFNGLVERTEX2FPROC            qglVertex2f;

extern PFNGLALPHAFUNCPROC           qglAlphaFunc;


/// BEATO Begin
// GL_ARB_vertex_buffer_object
// GL_ARB_uniform_buffer_object

// GL_ARB_buffer_storage
extern PFNGLDELETEBUFFERSPROC               qglDeleteBuffers;
extern PFNGLCREATEBUFFERSPROC               qglCreateBuffers;
extern PFNGLNAMEDBUFFERSTORAGEPROC          qglNamedBufferStorage;
extern PFNGLMAPNAMEDBUFFERRANGEPROC         qglMapNamedBufferRange;
extern PFNGLUNMAPNAMEDBUFFERPROC            qglUnmapNamedBuffer;
extern PFNGLCOPYNAMEDBUFFERSUBDATAPROC      qglCopyNamedBufferSubData;
extern PFNGLNAMEDBUFFERSUBDATAPROC          qglNamedBufferSubData;

// GL_ARB_vertex_array_object
extern PFNGLCREATEVERTEXARRAYSPROC          qglCreateVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC          qglDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC             qglBindVertexArray;
extern PFNGLENABLEVERTEXARRAYATTRIBPROC     qglEnableVertexArrayAttrib;
extern PFNGLDISABLEVERTEXARRAYATTRIBPROC    qglDisableVertexArrayAttrib;
extern PFNGLVERTEXATTRIBPOINTERPROC         qglVertexAttribPointer;
extern PFNGLVERTEXARRAYATTRIBBINDINGPROC    qglVertexArrayAttribBinding;
extern PFNGLVERTEXARRAYATTRIBFORMATPROC     qglVertexArrayAttribFormat;
extern PFNGLVERTEXARRAYELEMENTBUFFERPROC    qglVertexArrayElementBuffer;
extern PFNGLVERTEXARRAYVERTEXBUFFERPROC     qglVertexArrayVertexBuffer;
/// BEATO End

extern	void ( APIENTRY * qglColorTableEXT)( int, int, int, int, int, const void * );
extern	void ( APIENTRY * qglLockArraysEXT) (int , int);
extern	void ( APIENTRY * qglUnlockArraysEXT) (void);
extern	void ( APIENTRY * qglPointParameterfEXT)( GLenum param, GLfloat value );
extern	void ( APIENTRY * qglPointParameterfvEXT)( GLenum param, const GLfloat *value );
extern	void ( APIENTRY * qglSelectTextureSGIS)( GLenum );
extern	void ( APIENTRY * qglMTexCoord2fSGIS)( GLenum, GLfloat, GLfloat );

#ifdef _WIN32

extern  int   ( WINAPI * qwglChoosePixelFormat )(HDC, CONST PIXELFORMATDESCRIPTOR *);
extern  int   ( WINAPI * qwglDescribePixelFormat) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
extern  int   ( WINAPI * qwglGetPixelFormat)(HDC);
extern  BOOL  ( WINAPI * qwglSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
extern  BOOL  ( WINAPI * qwglSwapBuffers)(HDC);

extern BOOL  ( WINAPI * qwglCopyContext)(HGLRC, HGLRC, UINT);
extern HGLRC ( WINAPI * qwglCreateContext)(HDC);
extern HGLRC ( WINAPI * qwglCreateLayerContext)(HDC, int);
extern BOOL  ( WINAPI * qwglDeleteContext)(HGLRC);
extern HGLRC ( WINAPI * qwglGetCurrentContext)(VOID);
extern HDC   ( WINAPI * qwglGetCurrentDC)(VOID);
extern PROC  ( WINAPI * qwglGetProcAddress)(LPCSTR);
extern BOOL  ( WINAPI * qwglMakeCurrent)(HDC, HGLRC);
extern BOOL  ( WINAPI * qwglShareLists)(HGLRC, HGLRC);
extern BOOL  ( WINAPI * qwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);

extern BOOL  ( WINAPI * qwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT,
                                           FLOAT, int, LPGLYPHMETRICSFLOAT);

extern BOOL ( WINAPI * qwglDescribeLayerPlane)(HDC, int, int, UINT,
                                            LPLAYERPLANEDESCRIPTOR);
extern int  ( WINAPI * qwglSetLayerPaletteEntries)(HDC, int, int, int,
                                                CONST COLORREF *);
extern int  ( WINAPI * qwglGetLayerPaletteEntries)(HDC, int, int, int,
                                                COLORREF *);
extern BOOL ( WINAPI * qwglRealizeLayerPalette)(HDC, int, BOOL);
extern BOOL ( WINAPI * qwglSwapLayerBuffers)(HDC, UINT);

extern BOOL ( WINAPI * qwglSwapIntervalEXT)( int interval );

extern BOOL ( WINAPI * qwglGetDeviceGammaRampEXT ) ( unsigned char *pRed, unsigned char *pGreen, unsigned char *pBlue );
extern BOOL ( WINAPI * qwglSetDeviceGammaRampEXT ) ( const unsigned char *pRed, const unsigned char *pGreen, const unsigned char *pBlue );

#endif

/*
** extension constants
*/
#define GL_POINT_SIZE_MIN_EXT				0x8126
#define GL_POINT_SIZE_MAX_EXT				0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT	0x8128
#define GL_DISTANCE_ATTENUATION_EXT			0x8129

#ifdef __sgi
#define GL_SHARED_TEXTURE_PALETTE_EXT		GL_TEXTURE_COLOR_TABLE_SGI
#else
#define GL_SHARED_TEXTURE_PALETTE_EXT		0x81FB
#endif

#define GL_TEXTURE0_SGIS					0x835E
#define GL_TEXTURE1_SGIS					0x835F

#endif
