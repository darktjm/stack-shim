/* for RTLD_NEXT */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
/* functions used by code */
#include <dlfcn.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* functions overridden by code */
#include <xcb/xcb.h>
#include <xcb/xcbext.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* XOpenDisplay was the first to crash, but instead of adding a bunch of X11
 * routines, I decided to fix the underlying xcb calls instead */
/* Display *XOpenDisplay(const char *display_name) */

/* These are the prototypes of the functions to override.  A few notes:
 *   - The entire prototype must be on one line
 *   - there must be at least one blank line following the prototype
 *   - Anything between the prototype and the blank line will be replaced
 *     or deleted by the sed scripts
 *   - remove the trailing semicolon, and have no space before final )
 *   - the sed script may still become confused; no guarantees
 */
/*+ void *xcb_wait_for_reply(xcb_connection_t *c, unsigned int request, xcb_generic_error_t **e) */

/*+ int xcb_take_socket(xcb_connection_t *c, void (*return_socket)(void *closure), void *closure, int flags, uint64_t *sent) */

/*+ int xcb_writev(xcb_connection_t *c, struct iovec *vector, int count, uint64_t requests) */

/*+ void *xcb_wait_for_reply64(xcb_connection_t *c, uint64_t request, xcb_generic_error_t **e) */

/*+ Bool glXQueryVersion( Display *dpy, int *maj, int *min) */

/*+ GLXContext glXCreateContext( Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct) */

/*+ xcb_generic_error_t *xcb_request_check(xcb_connection_t *c, xcb_void_cookie_t cookie) */

/*+ Bool glXMakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx) */

/*+ void glXSwapBuffers( Display *dpy, GLXDrawable drawable) */

/* Note for GL:  Initial GLAPI was removed because GCC doesn't like it */
/*+ void GLAPIENTRY glBegin(GLenum mode) */

/*+ void GLAPIENTRY glEnd(void) */

/*+ void GLAPIENTRY glOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val) */

/*+ void GLAPIENTRY glAlphaFunc( GLenum func, GLclampf ref) */

/*+ void GLAPIENTRY glTexImage2D( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels) */

/*+ void GLAPIENTRY glBindTexture( GLenum target, GLuint texture) */

/*+ void GLAPIENTRY glColor4ubv( const GLubyte *v) */

/*+ void GLAPIENTRY glEnable( GLenum cap) */

/*+ void GLAPIENTRY glPushAttrib( GLbitfield mask) */

/*+ void GLAPIENTRY glTexParameteri( GLenum target, GLenum pname, GLint param) */

/*+ void GLAPIENTRY glRasterPos2i( GLint x, GLint y) */

/*+ void GLAPIENTRY glPixelZoom( GLfloat xfactor, GLfloat yfactor) */

/*+ void GLAPIENTRY glDeleteTextures( GLsizei n, const GLuint *textures) */

/*+ void GLAPIENTRY glDrawPixels( GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels) */

/*+ void GLAPIENTRY glTexCoord2f( GLfloat s, GLfloat t) */

/*+ void GLAPIENTRY glBitmap( GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap) */

/*+ void GLAPIENTRY glMatrixMode( GLenum mode) */

/*+ void GLAPIENTRY glGetDoublev( GLenum pname, GLdouble *params) */

/*+ void GLAPIENTRY glLoadMatrixd( const GLdouble *m) */

/*+ void GLAPIENTRY glVertex2f( GLfloat x, GLfloat y) */

/*+ void GLAPIENTRY glVertex2i( GLint x, GLint y) */

/*+ void GLAPIENTRY glLineWidth( GLfloat width) */

/*+ void GLAPIENTRY glDisable( GLenum cap) */

/*+ void GLAPIENTRY glBlendFunc( GLenum sfactor, GLenum dfactor) */

/*+ void GLAPIENTRY glClear( GLbitfield mask) */

/*+ void GLAPIENTRY glGenTextures( GLsizei n, GLuint *textures) */

/*+ void GLAPIENTRY glScissor( GLint x, GLint y, GLsizei width, GLsizei height) */

/*+ void GLAPIENTRY glGetTexLevelParameteriv( GLenum target, GLint level, GLenum pname, GLint *params) */

/*+ void GLAPIENTRY glLoadIdentity( void) */

/*+ void GLAPIENTRY glViewport( GLint x, GLint y, GLsizei width, GLsizei height) */

/*+ void GLAPIENTRY glGetIntegerv( GLenum pname, GLint *params) */

/*+ void GLAPIENTRY glPixelStorei( GLenum pname, GLint param) */

/*+ void GLAPIENTRY glPopAttrib( void) */

/*+ void GLAPIENTRY glTexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels) */

/*+ void GLAPIENTRY glClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) */

/*+ void GLAPIENTRY glReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels) */

/* Procs obtained from glXGetProcAddressARB have to do things differently */

static PFNGLXGETPROCADDRESSPROC getproc = NULL;

int glXSwapIntervalSGI(int interval)
{
    static PFNGLXSWAPINTERVALSGIPROC real = NULL;
    if(!real)
	real = (PFNGLXSWAPINTERVALSGIPROC)getproc("glXSwapIntervalSGI");
    return real(interval);
}

#if 0
/* this is just here so I can copy the code into the function below */
/*+ __GLXextFuncPtr glXGetProcAddressARB(const GLubyte *proc) */

#endif
/* I should probably make this a table to avoid the lookup, but I don't care */
__GLXextFuncPtr glXGetProcAddressARB(const GLubyte *proc)
{
  pthread_mutex_lock(&lock);
  if(!getproc && !(getproc = dlsym(RTLD_NEXT, "glXGetProcAddressARB"))) {
    pthread_mutex_unlock(&lock);
    fprintf(stderr, "Can't find bind glXGetProcAddressARB\n");
    exit(1);
  }
  pthread_mutex_unlock(&lock);
  __GLXextFuncPtr ret = getproc(proc);
  if(ret) {
      void *dret = dlsym(RTLD_DEFAULT, proc);
      if(dret)
	  return (__GLXextFuncPtr)dret;
  }
  fprintf(stderr, "got addr for %s %p\n", proc, ret);
  return ret;
}
