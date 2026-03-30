
#include "qgl.hpp"
#include "gl_rmain.hpp"
#include "gl_local.hpp"

/// BEATO Begin

PFNGLGETERRORPROC            qglGetError = nullptr;

PFNGLFINISHPROC              qglFinish = nullptr;
PFNGLFLUSHPROC               qglFlush = nullptr;

PFNGLENABLEPROC              qglEnable = nullptr;
PFNGLDISABLEPROC             qglDisable = nullptr;

PFNGLGETFLOATVPROC           qglGetFloatv = nullptr;
PFNGLGETSTRINGPROC           qglGetString = nullptr;

PFNGLDRAWBUFFERPROC          qglDrawBuffer = nullptr;

PFNGLREADPIXELSPROC          qglReadPixels = nullptr;

PFNGLPOINTSIZEPROC           qglPointSize = nullptr;

PFNGLCLEARCOLORPROC          qglClearColor = nullptr;
PFNGLCLEARDEPTHPROC          qglClearDepth = nullptr;
PFNGLCLEARPROC               qglClear = nullptr;

PFNGLVIEWPORTPROC            qglViewport = nullptr;
PFNGLSCISSORPROC             qglScissor = nullptr;

PFNGLDEPTHFUNCPROC           qglDepthFunc = nullptr;
PFNGLDEPTHMASKPROC           qglDepthMask = nullptr;
PFNGLDEPTHRANGEPROC          qglDepthRange = nullptr;

PFNGLCULLFACEPROC            qglCullFace = nullptr;

PFNGLPOLYGONMODEPROC         qglPolygonMode = nullptr;

PFNGLBINDTEXTUREPROC         qglBindTexture = nullptr;
PFNGLGENTEXTURESPROC         qglGenTextures = nullptr;
PFNGLDELETETEXTURESPROC      qglDeleteTextures = nullptr;
PFNGLTEXIMAGE1DPROC          qglTexImage1D = nullptr;
PFNGLTEXIMAGE2DPROC          qglTexImage2D = nullptr;
PFNGLTEXPARAMETERFPROC       qglTexParameterf = nullptr;
PFNGLTEXPARAMETERFVPROC      qglTexParameterfv = nullptr;
PFNGLTEXPARAMETERIPROC       qglTexParameteri = nullptr;
PFNGLTEXPARAMETERIVPROC      qglTexParameteriv = nullptr;
PFNGLTEXSUBIMAGE1DPROC       qglTexSubImage1D = nullptr;
PFNGLTEXSUBIMAGE2DPROC       qglTexSubImage2D = nullptr;

PFNGLBLENDFUNCPROC           qglBlendFunc = nullptr;

PNFGLMATRIXMODEPROC          qglMatrixMode = nullptr;
PNFGLLOADIDENTITYPROC        qglLoadIdentity = nullptr;
PFNGLLOADMATRIXFPROC         qglLoadMatrixf = nullptr;
PFNGLPUSHMATRIXPROC          qglPushMatrix = nullptr;
PFNGLPOPMATRIXPROC           qglPopMatrix = nullptr;
PNFGLORTHOPROC               qglOrtho = nullptr;
PFNGLFRUSTUMPROC             qglFrustum = nullptr;

PFNGLTRANSLATEFPROC          qglTranslatef = nullptr;
PFNGLROTATEFPROC             qglRotatef = nullptr;
PFNGLSCALEFPROC              qglScalef = nullptr;

PFNGLENABLECLIENTSTATEPROC   qglEnableClientState = nullptr;
PFNGLVERTEXPOINTERPROC       qglVertexPointer = nullptr;
PFNGLCOLORPOINTERPROC        qglColorPointer = nullptr;
PFNGLARRAYELEMENTPROC        qglArrayElement = nullptr;

PFNGLSHADEMODELPROC          qglShadeModel = nullptr;

PFNGLBEGINPROC               qglBegin = nullptr;
PFNGLENDPROC                 qglEnd = nullptr;

PFNGLTEXENVFPROC             qglTexEnvf = nullptr;

PFNGLCOLOR3FPROC             qglColor3f = nullptr;
PFNGLCOLOR3FVPROC            qglColor3fv = nullptr;
PFNGLCOLOR4FPROC             qglColor4f = nullptr;
PFNGLCOLOR4FVPROC            qglColor4fv = nullptr;
PFMGLCOLOR4UBVPROC           qglColor4ubv = nullptr;
PFNGLVERTEX3FPROC            qglVertex3f = nullptr;
PFNGLVERTEX3FVPROC           qglVertex3fv = nullptr;
PFNGLTEXCOORD2FPROC          qglTexCoord2f = nullptr;
PFNGLVERTEX2FPROC            qglVertex2f = nullptr;

PFNGLALPHAFUNCPROC           qglAlphaFunc = nullptr;

PFNGLDELETEBUFFERSPROC               qglDeleteBuffers = nullptr;
PFNGLCREATEBUFFERSPROC               qglCreateBuffers = nullptr;
PFNGLNAMEDBUFFERSTORAGEPROC          qglNamedBufferStorage = nullptr;
PFNGLMAPNAMEDBUFFERRANGEPROC         qglMapNamedBufferRange = nullptr;
PFNGLUNMAPNAMEDBUFFERPROC            qglUnmapNamedBuffer = nullptr;
PFNGLCOPYNAMEDBUFFERSUBDATAPROC      qglCopyNamedBufferSubData = nullptr;
PFNGLNAMEDBUFFERSUBDATAPROC          qglNamedBufferSubData = nullptr;

PFNGLCREATEVERTEXARRAYSPROC          qglCreateVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC          qglDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC             qglBindVertexArray = nullptr;
PFNGLENABLEVERTEXARRAYATTRIBPROC     qglEnableVertexArrayAttrib = nullptr;
PFNGLDISABLEVERTEXARRAYATTRIBPROC    qglDisableVertexArrayAttrib = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC         qglVertexAttribPointer = nullptr;
PFNGLVERTEXARRAYATTRIBBINDINGPROC    qglVertexArrayAttribBinding = nullptr;
PFNGLVERTEXARRAYATTRIBFORMATPROC     qglVertexArrayAttribFormat = nullptr;
PFNGLVERTEXARRAYELEMENTBUFFERPROC    qglVertexArrayElementBuffer = nullptr;
PFNGLVERTEXARRAYVERTEXBUFFERPROC     qglVertexArrayVertexBuffer = nullptr;
/// BEATO End

void ( APIENTRY * qglColorTableEXT)( int, int, int, int, int, const void * );
void ( APIENTRY * qglLockArraysEXT) (int , int);
void ( APIENTRY * qglUnlockArraysEXT) (void);
void ( APIENTRY * qglPointParameterfEXT)( GLenum param, GLfloat value );
void ( APIENTRY * qglPointParameterfvEXT)( GLenum param, const GLfloat *value );
void ( APIENTRY * qglSelectTextureSGIS)( GLenum );
void ( APIENTRY * qglMTexCoord2fSGIS)( GLenum, GLfloat, GLfloat );


//
// QGL_Init
//
// This is responsible for binding our qgl function pointers to 
// the appropriate GL stuff.  In Windows this means doing a 
// LoadLibrary and a bunch of calls to GetProcAddress.  On other
// operating systems we need to do the right thing, whatever that
// might be.
// 
//
bool glRenderer::QGL_Init( const char *dllname )
{
	ri.GLimp_LoadLibary( dllname );
	
	qglGetError = reinterpret_cast<PFNGLGETERRORPROC>( ri.GLimp_GetProcAddress( "glGetError" ) );

	qglFinish = reinterpret_cast<PFNGLFINISHPROC>( ri.GLimp_GetProcAddress( "glFinish" ) );
	qglFlush = reinterpret_cast<PFNGLFLUSHPROC>( ri.GLimp_GetProcAddress( "glFlush" ) );

	qglEnable = reinterpret_cast<PFNGLENABLEPROC>( ri.GLimp_GetProcAddress( "glEnable" ) );
	qglDisable = reinterpret_cast<PFNGLDISABLEPROC>( ri.GLimp_GetProcAddress( "glDisable" ) );

	qglGetFloatv = reinterpret_cast<PFNGLGETFLOATVPROC>( ri.GLimp_GetProcAddress( "glGetFloatv" ) );
	qglGetString = reinterpret_cast<PFNGLGETSTRINGPROC>( ri.GLimp_GetProcAddress( "glGetString" ) );

	qglDrawBuffer = reinterpret_cast<PFNGLDRAWBUFFERPROC>( ri.GLimp_GetProcAddress( "glDrawBuffer" ) );

	qglReadPixels = reinterpret_cast<PFNGLREADPIXELSPROC>( ri.GLimp_GetProcAddress( "glReadPixels" ) );

	qglPointSize = reinterpret_cast<PFNGLPOINTSIZEPROC>( ri.GLimp_GetProcAddress( "glPointSize" ) );

	qglClearColor = reinterpret_cast<PFNGLCLEARCOLORPROC>( ri.GLimp_GetProcAddress( "glClearColor" ) );
	qglClearDepth = reinterpret_cast<PFNGLCLEARDEPTHPROC>( ri.GLimp_GetProcAddress( "glClearDepth" ) );
	qglClear = reinterpret_cast<PFNGLCLEARPROC>( ri.GLimp_GetProcAddress( "glClear" ) );

	qglViewport = reinterpret_cast<PFNGLVIEWPORTPROC>( ri.GLimp_GetProcAddress( "glViewport" ) );
	qglScissor = reinterpret_cast<PFNGLSCISSORPROC>( ri.GLimp_GetProcAddress( "glScissor" ) );

	qglDepthFunc = reinterpret_cast<PFNGLDEPTHFUNCPROC>( ri.GLimp_GetProcAddress( "glDepthFunc" ) );
	qglDepthMask = reinterpret_cast<PFNGLDEPTHMASKPROC>( ri.GLimp_GetProcAddress( "glDepthMask" ) );
	qglDepthRange = reinterpret_cast<PFNGLDEPTHRANGEPROC>( ri.GLimp_GetProcAddress( "glDepthRange" ) );

	qglCullFace = reinterpret_cast<PFNGLCULLFACEPROC>( ri.GLimp_GetProcAddress( "glCullFace" ) );

	qglPolygonMode = reinterpret_cast<PFNGLPOLYGONMODEPROC>( ri.GLimp_GetProcAddress( "glPolygonMode" ) );

	qglBindTexture = reinterpret_cast<PFNGLBINDTEXTUREPROC>( ri.GLimp_GetProcAddress( "glBindTexture" ) );
	qglGenTextures = reinterpret_cast<PFNGLGENTEXTURESPROC>( ri.GLimp_GetProcAddress( "glGenTextures" ) );
	qglDeleteTextures = reinterpret_cast<PFNGLDELETETEXTURESPROC>( ri.GLimp_GetProcAddress( "glDeleteTextures" ) );
	qglTexImage1D = reinterpret_cast<PFNGLTEXIMAGE1DPROC>( ri.GLimp_GetProcAddress( "glTexImage1D" ) );
	qglTexImage2D = reinterpret_cast<PFNGLTEXIMAGE2DPROC>( ri.GLimp_GetProcAddress( "glTexImage2D" ) );
	qglTexParameterf = reinterpret_cast<PFNGLTEXPARAMETERFPROC>( ri.GLimp_GetProcAddress( "glTexParameterf" ) );
	qglTexParameterfv = reinterpret_cast<PFNGLTEXPARAMETERFVPROC>( ri.GLimp_GetProcAddress( "glTexParameterfv" ) );
	qglTexParameteri = reinterpret_cast<PFNGLTEXPARAMETERIPROC>( ri.GLimp_GetProcAddress( "glTexParameteri" ) );
	qglTexParameteriv = reinterpret_cast<PFNGLTEXPARAMETERIVPROC>( ri.GLimp_GetProcAddress( "glTexParameteriv" ) );
	qglTexSubImage1D = reinterpret_cast<PFNGLTEXSUBIMAGE1DPROC>( ri.GLimp_GetProcAddress( "glTexSubImage1D" ) );
	qglTexSubImage2D = reinterpret_cast<PFNGLTEXSUBIMAGE2DPROC>( ri.GLimp_GetProcAddress( "glTexSubImage2D" ) );

	qglBlendFunc = reinterpret_cast<PFNGLBLENDFUNCPROC>( ri.GLimp_GetProcAddress( "glBlendFunc" ) );

	qglMatrixMode = reinterpret_cast<PNFGLMATRIXMODEPROC>( ri.GLimp_GetProcAddress( "glMatrixMode" ) );
	qglLoadIdentity = reinterpret_cast<PNFGLLOADIDENTITYPROC>( ri.GLimp_GetProcAddress( "glLoadIdentity" ) );
	qglLoadMatrixf = reinterpret_cast<PFNGLLOADMATRIXFPROC>( ri.GLimp_GetProcAddress( "glLoadMatrixf" ) );
	qglPushMatrix = reinterpret_cast<PFNGLPUSHMATRIXPROC>( ri.GLimp_GetProcAddress( "glPushMatrix" ) );
	qglPopMatrix = reinterpret_cast<PFNGLPOPMATRIXPROC>( ri.GLimp_GetProcAddress( "glPopMatrix" ) );
	qglOrtho = reinterpret_cast<PNFGLORTHOPROC>( ri.GLimp_GetProcAddress( "glOrtho" ) );
	qglFrustum = reinterpret_cast<PFNGLFRUSTUMPROC>( ri.GLimp_GetProcAddress( "glFrustum" ) );

	qglTranslatef = reinterpret_cast<PFNGLTRANSLATEFPROC>( ri.GLimp_GetProcAddress( "glTranslatef" ) );
	qglRotatef = reinterpret_cast<PFNGLROTATEFPROC>( ri.GLimp_GetProcAddress( "glRotatef" ) );
	qglScalef = reinterpret_cast<PFNGLSCALEFPROC>( ri.GLimp_GetProcAddress( "glScalef" ) );

	qglEnableClientState = reinterpret_cast<PFNGLENABLECLIENTSTATEPROC>( ri.GLimp_GetProcAddress( "glEnableClientState" ) );
	qglVertexPointer = reinterpret_cast<PFNGLVERTEXPOINTERPROC>( ri.GLimp_GetProcAddress( "glVertexPointer" ) );
	qglColorPointer = reinterpret_cast<PFNGLCOLORPOINTERPROC>( ri.GLimp_GetProcAddress( "glColorPointer" ) );
	qglArrayElement = reinterpret_cast<PFNGLARRAYELEMENTPROC>( ri.GLimp_GetProcAddress( "glArrayElement" ) );

	qglShadeModel = reinterpret_cast<PFNGLSHADEMODELPROC>( ri.GLimp_GetProcAddress( "glShadeModel" ) );

	qglBegin = reinterpret_cast<PFNGLBEGINPROC>( ri.GLimp_GetProcAddress( "glBegin" ) );
	qglEnd = reinterpret_cast<PFNGLENDPROC>( ri.GLimp_GetProcAddress( "glEnd" ) );

	qglTexEnvf = reinterpret_cast<PFNGLTEXENVFPROC>( ri.GLimp_GetProcAddress( "glTexEnvf" ) );

	qglColor3f = reinterpret_cast<PFNGLCOLOR3FPROC>( ri.GLimp_GetProcAddress( "glColor3f" ) );
	qglColor3fv = reinterpret_cast<PFNGLCOLOR3FVPROC>( ri.GLimp_GetProcAddress( "glColor3fv" ) );
	qglColor4f = reinterpret_cast<PFNGLCOLOR4FPROC>( ri.GLimp_GetProcAddress( "glColor4f" ) );
	qglColor4fv = reinterpret_cast<PFNGLCOLOR4FVPROC>( ri.GLimp_GetProcAddress( "glColor4fv" ) );
	qglColor4ubv = reinterpret_cast<PFMGLCOLOR4UBVPROC>( ri.GLimp_GetProcAddress( "glColor4ubv" ) );
	qglVertex3f = reinterpret_cast<PFNGLVERTEX3FPROC>( ri.GLimp_GetProcAddress( "glVertex3f" ) );
	qglVertex3fv = reinterpret_cast<PFNGLVERTEX3FVPROC>( ri.GLimp_GetProcAddress( "glVertex3fv" ) );
	qglTexCoord2f = reinterpret_cast<PFNGLTEXCOORD2FPROC>( ri.GLimp_GetProcAddress( "glTexCoord2f" ) );
	qglVertex2f = reinterpret_cast<PFNGLVERTEX2FPROC>( ri.GLimp_GetProcAddress( "glVertex2f" ) );

	qglAlphaFunc = reinterpret_cast<PFNGLALPHAFUNCPROC>( ri.GLimp_GetProcAddress( "glAlphaFunc" ) );

	qglDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSPROC>( ri.GLimp_GetProcAddress( "glDeleteBuffers" ) );
	qglCreateBuffers = reinterpret_cast<PFNGLCREATEBUFFERSPROC>( ri.GLimp_GetProcAddress( "glCreateBuffers" ) );
	qglNamedBufferStorage = reinterpret_cast<PFNGLNAMEDBUFFERSTORAGEPROC>( ri.GLimp_GetProcAddress( "glNamedBufferStorage" ) );
	qglMapNamedBufferRange = reinterpret_cast<PFNGLMAPNAMEDBUFFERRANGEPROC>( ri.GLimp_GetProcAddress( "glMapNamedBufferRange" ) );
	qglUnmapNamedBuffer = reinterpret_cast<PFNGLUNMAPNAMEDBUFFERPROC>( ri.GLimp_GetProcAddress( "glUnmapNamedBuffer" ) );
	qglCopyNamedBufferSubData = reinterpret_cast<PFNGLCOPYNAMEDBUFFERSUBDATAPROC>( ri.GLimp_GetProcAddress( "glCopyNamedBufferSubData" ) );
	qglNamedBufferSubData = reinterpret_cast<PFNGLNAMEDBUFFERSUBDATAPROC>( ri.GLimp_GetProcAddress( "glNamedBufferSubData" ) );

	qglCreateVertexArrays = reinterpret_cast<PFNGLCREATEVERTEXARRAYSPROC>( ri.GLimp_GetProcAddress( "glCreateVertexArrays" ) );
	qglDeleteVertexArrays = reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>( ri.GLimp_GetProcAddress( "glDeleteVertexArrays" ) );
	qglBindVertexArray = reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>( ri.GLimp_GetProcAddress( "glBindVertexArray" ) );
	qglEnableVertexArrayAttrib = reinterpret_cast<PFNGLENABLEVERTEXARRAYATTRIBPROC>( ri.GLimp_GetProcAddress( "glEnableVertexArrayAttrib" ) );
	qglDisableVertexArrayAttrib = reinterpret_cast<PFNGLDISABLEVERTEXARRAYATTRIBPROC>( ri.GLimp_GetProcAddress( "glDisableVertexArrayAttrib" ) );
	qglVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>( ri.GLimp_GetProcAddress( "glVertexAttribPointer" ) );
	qglVertexArrayAttribBinding = reinterpret_cast<PFNGLVERTEXARRAYATTRIBBINDINGPROC>( ri.GLimp_GetProcAddress( "glVertexArrayAttribBinding" ) );
	qglVertexArrayAttribFormat = reinterpret_cast<PFNGLVERTEXARRAYATTRIBFORMATPROC>( ri.GLimp_GetProcAddress( "glVertexArrayAttribFormat" ) );
	qglVertexArrayElementBuffer = reinterpret_cast<PFNGLVERTEXARRAYELEMENTBUFFERPROC>( ri.GLimp_GetProcAddress( "glVertexArrayElementBuffer" ) );
	qglVertexArrayVertexBuffer = reinterpret_cast<PFNGLVERTEXARRAYVERTEXBUFFERPROC>( ri.GLimp_GetProcAddress( "glVertexArrayVertexBuffer" ) );

	qglPointParameterfEXT = 0;
	qglPointParameterfvEXT = 0;
	qglColorTableEXT = 0;
	qglSelectTextureSGIS = 0;
	qglMTexCoord2fSGIS = 0;

	return true;
}

/*
** QGL_Shutdown
**
** Unloads the specified DLL then nulls out all the proc pointers.
*/
void glRenderer::QGL_Shutdown( void )
{
	qglGetError = nullptr;

	qglFinish = nullptr;
	qglFlush = nullptr;

	qglEnable = nullptr;
	qglDisable = nullptr;

	qglGetFloatv = nullptr;
	qglGetString = nullptr;

	qglDrawBuffer = nullptr;

	qglReadPixels = nullptr;

	qglPointSize = nullptr;

	qglClearColor = nullptr;
	qglClearDepth = nullptr;
	qglClear = nullptr;

	qglViewport = nullptr;
	qglScissor = nullptr;

	qglDepthFunc = nullptr;
	qglDepthMask = nullptr;
	qglDepthRange = nullptr;

	qglCullFace = nullptr;

	qglPolygonMode = nullptr;

	qglBindTexture = nullptr;
	qglGenTextures = nullptr;
	qglDeleteTextures = nullptr;
	qglTexImage1D = nullptr;
	qglTexImage2D = nullptr;
	qglTexParameterf = nullptr;
	qglTexParameterfv = nullptr;
	qglTexParameteri = nullptr;
	qglTexParameteriv = nullptr;
	qglTexSubImage1D = nullptr;
	qglTexSubImage2D = nullptr;

	qglBlendFunc = nullptr;

	qglMatrixMode = nullptr;
	qglLoadIdentity = nullptr;
	qglLoadMatrixf = nullptr;
	qglPushMatrix = nullptr;
	qglPopMatrix = nullptr;
	qglOrtho = nullptr;
	qglFrustum = nullptr;

	qglTranslatef = nullptr;
	qglRotatef = nullptr;
	qglScalef = nullptr;

	qglEnableClientState = nullptr;	
	qglVertexPointer = nullptr;
	qglColorPointer = nullptr;
	qglArrayElement = nullptr;

	qglShadeModel = nullptr;

	qglBegin = nullptr;
	qglEnd = nullptr;

	qglTexEnvf = nullptr;

	qglColor3f = nullptr;
	qglColor3fv = nullptr;
	qglColor4f = nullptr;
	qglColor4fv = nullptr;
	qglColor4ubv = nullptr;
	qglVertex3f = nullptr;
	qglVertex3fv = nullptr;
	qglTexCoord2f = nullptr;
	qglVertex2f = nullptr;

	qglAlphaFunc = nullptr;

	qglDeleteBuffers = nullptr;
	qglCreateBuffers = nullptr;
	qglNamedBufferStorage = nullptr;
	qglMapNamedBufferRange = nullptr;
	qglUnmapNamedBuffer = nullptr;
	qglCopyNamedBufferSubData = nullptr;
	qglNamedBufferSubData = nullptr;

	qglCreateVertexArrays = nullptr;
	qglDeleteVertexArrays = nullptr;
	qglBindVertexArray = nullptr;
	qglEnableVertexArrayAttrib = nullptr;
	qglDisableVertexArrayAttrib = nullptr;
	qglVertexAttribPointer = nullptr;
	qglVertexArrayAttribBinding = nullptr;
	qglVertexArrayAttribFormat = nullptr;
	qglVertexArrayElementBuffer = nullptr;
	qglVertexArrayVertexBuffer = nullptr;
}