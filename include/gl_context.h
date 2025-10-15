// Comments and documentation come from the OpenGL 1.2 Reference Manual
// The OpenGL SuperBible and The OpenGL Programming Guide 8th Edition
// Comments are copied verbatim from the OpenGL 1.2 Reference Manual

#ifndef _SDL_OPENGL_CPP_GL_CONTEXT_H_
#define _SDL_OPENGL_CPP_GL_CONTEXT_H_

#include <memory>

#include "SDL_opengl.h"
#include <SDL.h>

#include "opengl.h"

namespace sdl_opengl_cpp {
// gMock (google-mock, googlemock) doesn't allow testing directly on free
// functions. (From the gMock cook book (gmock_cook_book.md):
//
//   ### Mocking Free Functions
//
//   It is not possible to directly mock a free function (i.e. a
//   C-style function or a static method). If you need to, you can
//   rewrite your code to use an interface (abstract class).
//
// We're doing an adapter for the OpenGL class instead of a full
// macro expansion.  There might be some cleverness with writable
// RTTI or templates I could use to rewrite the struct or whatever,
// but this is basic and it works for now.
//
// Yes, it adds an extra layer to OpenGL calls.  If you have a
// suggestion for a portable and clean solution, let me know.
//
// Right now, there are not tests and wrappers for everything and
// most documentation is not included.  In particular, some of the
// uniform functions in here are used by my own downstream
// applications but don't have full C++ wrapper classes yet.
//
//
// For documentation, I'd recommend:
//
//   The OpenGL Programming Guide by Dave Shreiner, Graham Sellers,
//   John Kessenich, and Bill Licea-Kane. Eighth Edition or newer.
//
//   The OpenGL SuperBible by Graham Sellers, Richard S. Wright,
//   Jr. and Nicholas Haemel.  Seventh Edition or newer.
//
//   The OpenGL Graphics System A Specification (Version 4.1 (Core
//   Profile) was used here but there are more recent ones.
class GLContext {
public:
  // GLContext() { };
  GLContext(const std::shared_ptr<GL_Context> &ctx) noexcept
      : gl_context{ctx} {};
  virtual ~GLContext(){};

  // General functions

  //! Pushes the attribute stack
  //!
  //! parameters:
  //!
  //! \param mask Specifies a mask that indicates which attributes to
  //!        save. Values for Mask are provided in the preceding list.
  //!
  //! Description
  //!
  //! The glPushAttrib subroutine takes one argument, a mask that
  //! indicates which groups of state variables to save on the
  //! attribute stack. Symbolic constants are used to set bits in the
  //! mask. The Mask parameter is typically constructed by ORing
  //! several of these constants together. The GL_ALL_ATTRIB_BITS
  //! special mask can be used to save all stackable states.
  //!
  //! The symbolic mask constants and their associated GL states are
  //! in the following list.
  //!
  //! Mask                   Attributes saved
  //!
  //! GL_ACCUM_BUFFER_BIT    Accumulation buffer clear value
  //! GL_COLOR_BUFFER_BIT    GL_ALPHA_TEST enable bit
  //!                        Alpha test function and reference value
  //!                        GL_BLEND enable bit
  //!                        Blending source and destination functions
  //!                        GL_COLOR_LOGIC_OP enable bit
  //!                        GL_DITHER enable bit
  //!                        GL_DRAW_BUFFER setting
  //!                        GL_LOGIC_OP enable bit
  //!                        Logic op function
  //!                        Color mode and index mode clear values
  //!                        Color mode and index mode write masks
  //!                        GL_BLEND_EQUATION_EXT setting
  //! GL_CURRENT_BIT         Current red, green, blue, alpha (RGBA) color
  //!                        Current color index
  //!                        Current normal vector
  //!                        Current texture coordinates
  //!                        Current raster position
  //!                        GL_CURRENT_RASTER_POSITION_VALID flag
  //!                        RGBA color associated with current raster position
  //!                        Color index associated with current raster position
  //!                        Texture coordinates associated with current raster
  //!                        position GL_EDGE_FLAG flag
  //! GL_DEPTH_BUFFER_BIT    GL_DEPTH_TEST enable bit
  //!                        Depth buffer test function
  //!                        Depth buffer clear value
  //!                        GL_DEPTH_WRITEMASK enable bit
  //!                        GL_ENABLE_BIT GL_ALPHA_TEST flag
  //!                        GL_AUTO_NORMAL flag
  //!                        GL_BLEND flag
  //!                        Enable bits for the user-definable clipping planes
  //!                        GL_COLOR_LOGIC_OP flag
  //!                        GL_COLOR_MATERIAL
  //!                        GL_CULL_FACE flag
  //!                        GL_DEPTH_TEST flag
  //!                        GL_DITHER flag
  //!                        GL_FOG flag
  //!                        GL_LIGHTi, where 0 < i<GL_MAX_LIGHTS
  //!                        GL_LIGHTING flag
  //!                        GL_LINE_SMOOTH flag
  //!                        GL_LINE_STIPPLE flag
  //!                        GL_LOGIC_OP flag
  //!                        GL_MAP1_x, where x is a map type
  //!                        GL_MAP2_x, where x is a map type
  //!                        GL_NORMALIZE flag
  //!                        GL_POINT_SMOOTH flag
  //!                        GL_POLYGON_OFFSET_EXT flag
  //!                        GL_POLYGON_OFFSET_FILL flag
  //!                        GL_POLYGON_OFFSET_LINE flag
  //!                        GL_POLYGON_OFFSET_POINT flag
  //!                        GL_POLYGON_SMOOTH flag
  //!                        GL_POLYGON_STIPPLE flag
  //!                        GL_SCISSOR_TEST flag
  //!                        GL_STENCIL_TEST flag
  //!                        GL_TEXTURE_1D flag
  //!                        GL_TEXTURE_2D flag
  //!                        GL_TEXTURE_3D_EXT flag
  //!                        Flags GL_TEXTURE_GEN_x, where x is S, T, R, or Q
  //! GL_EVAL_BIT            GL_MAP1_x enable bits, where x is a map type
  //!                        GL_MAP2_x enable bits, where x is a map type
  //!                        1-dimensional (1D) grid endpoints and divisions
  //!                        2-dimensional (2D) grid endpoints and divisions
  //!                        GL_AUTO_NORMAL enable bit
  //! GL_FOG_BIT             GL_FOG enable flag
  //!                        Fog color
  //!                        Fog density
  //!                        Linear fog start
  //!                        Linear fog end
  //!                        Fog index
  //!                        GL_FOG_MODE value
  //! GL_HINT_BIT            GL_PERSPECTIVE_CORRECTION_HINT setting
  //!                        GL_POINT_SMOOTH_HINT setting
  //!                        GL_LINE_SMOOTH_HINT setting
  //!                        GL_POLYGON_SMOOTH_HINT setting
  //!                        GL_FOG_HINT setting
  //!                        GL_SUBPIXEL_HINT_IBM setting
  //! GL_LIGHTING_BIT        GL_COLOR_MATERIAL enable bit
  //!                        GL_COLOR_MATERIAL_FACE value
  //!                        Color material parameters that are tracking the
  //!                        current color Ambient scene color
  //!                        GL_LIGHT_MODEL_LOCAL_VIEWER value
  //!                        GL_LIGHT_MODEL_TWO_SIDE setting
  //!                        GL_LIGHTING enable bit
  //!                        Enable bit for each light
  //!                        Ambient, diffuse, and specular intensity for each
  //!                        light Direction, position, exponent, and cutoff
  //!                        angle for each light Constant,
  //!                        linear, and quadratic attenuation factors for each
  //!                        light Ambient, diffuse, specular, and emissive
  //!                        color for each material Ambient, diffuse, and
  //!                        specular color indices for each material Specular
  //!                        exponent for each material GL_SHADE_MODEL setting
  //! GL_LINE_BIT            GL_LINE_SMOOTH flag
  //!                        GL_LINE_STIPPLE enable bit
  //!                        Line stipple pattern and repeat counter
  //!                        Line width
  //! GL_LIST_BIT            GL_LIST_BASE setting
  //! GL_PIXEL_MODE_BIT      GL_RED_BIAS and GL_RED_SCALE settings
  //!                        GL_GREEN_BIAS and GL_GREEN_SCALE values
  //!                        GL_BLUE_BIAS and GL_BLUE_SCALE
  //!                        GL_ALPHA_BIAS and GL_ALPHA_SCALE
  //!                        GL_DEPTH_BIAS and GL_DEPTH_SCALE
  //!                        GL_INDEX_OFFSET and GL_INDEX_SHIFT values
  //!                        GL_MAP_COLOR and GL_MAP_STENCIL flags
  //!                        GL_ZOOM_X and GL_ZOOM_Y factors
  //!                        GL_READ_BUFFER setting
  //! GL_POINT_BIT           GL_POINT_SMOOTH flag
  //!                        Point size
  //! GL_POLYGON_BIT         GL_CULL_FACE enable bit
  //!                        GL_CULL_FACE_MODE value
  //!                        GL_FRONT_FACE indicator
  //!                        GL_POLYGON_OFFSET_BIAS_EXT setting
  //!                        GL_POLYGON_OFFSET_EXT flag
  //!                        GL_POLYGON_OFFSET_FACTOR setting
  //!                        GL_POLYGON_OFFSET_FACTOR_EXT setting
  //!                        GL_POLYGON_OFFSET_FILL flag
  //!                        GL_POLYGON_OFFSET_LINE flag
  //!                        GL_POLYGON_OFFSET_POINT flag
  //!                        GL_POLYGON_OFFSET_UNITS setting
  //!                        GL_POLYGON_MODE setting
  //!                        GL_POLYGON_SMOOTH flag
  //!                        GL_POLYGON_STIPPLE enable bit
  //! GL_POLYGON_STIPPLE_BIT Polygon stipple image
  //! GL_SCISSOR_BIT         GL_SCISSOR_TEST flag
  //!                        Scissor box
  //! GL_STENCIL_BUFFER_BIT  GL_STENCIL_TEST enable bit
  //!                        Stencil function and reference value
  //!                        Stencil value mask
  //!                        Stencil fail, pass, and depth buffer pass actions
  //!                        Stencil buffer clear value
  //!                        Stencil buffer writemask
  //!
  //! GL_TEXTURE_BIT         Enable bits for the four texture coordinates
  //!                        Border color for each texture image
  //!                        Minification function for each texture image
  //!                        Magnification function for each texture image
  //!                        Texture coordinates and wrap mode for each texture
  //!                        image Color and mode for each texture environment
  //!                        Enable bits GL_TEXTURE_GEN_x, x is S, T, R, and Q
  //!                        GL_TEXTURE_GEN_MODE setting for S, T, R, and Q
  //!                        glTexGen plane equations for S, T, R, and Q
  //!                        Enables for 1D, 2D, and 3D_EXT testuresglEnd
  //! GL_TRANSFORM_BIT       Coefficients of the six clipping planes
  //!                        Enable bits for the user-definable clipping planes
  //!                        GL_MATRIX_MODE value
  //!                        GL_NORMALIZE flag
  //! GL_VIEWPORT_BIT Depth  range (near and far)
  //!                        Viewport origin and extent
  //!
  //!
  //! The glPopAttrib subroutine restores the values of the state
  //! variables saved with the last glPushAttrib subroutine. Those not
  //! saved are left unchanged.
  //!
  //! It is an error to push attributes onto a full stack, or to pop
  //! attributes off an empty stack. In either case, the error flag is
  //! set, and no other change is made to GL state.
  //!
  //! Initially, the attribute stack is empty
  //!
  //! Notes
  //!
  //! Not all values for the GL state can be saved on the attribute
  //! stack. For example, pixel pack and unpack state, render mode
  //! state, and select and feedback state cannot be saved.
  //!
  //! The depth of the attribute stack is dependent on the
  //! implementation, but it must be at least 16.
  //!
  //! Errors
  //!
  //! GL_STACK_OVERFLOW The glPushAttrib subroutine is called while
  //!                   the attribute stack is full.
  //! GL_STACK_UNDERFLOW The glPopAttrib subroutine is called while
  //!                    the attribute stack is empty.
  //! GL_INVALID_OPERATION The glPushAttrib subroutine is called
  //!                      between a call to glBegin and the
  //!                      corresponding call to glEnd.
  virtual void glPushAttrib(GLbitfield mask);

  //! Pops the attribute stack
  //!
  //! The glPopAttrib subroutine restores the values of the state
  //! variables saved with the last glPushAttrib subroutine. Those not
  //! saved are left unchanged.
  //!
  //! It is an error to push attributes onto a full stack, or to pop
  //! attributes off an empty stack. In either case, the error flag is
  //! set, and no other change is made to GL state.
  //!
  //! Initially, the attribute stack is empty.
  //!
  //! Notes
  //!
  //! Not all values for the GL state can be saved on the attribute
  //! stack. For example, pixel pack and unpack state, render mode
  //! state, and select and feedback state cannot be saved.
  //!
  //! The depth of the attribute stack is dependent on the
  //! implementation, but it must be at least 16.
  //!
  //! Errors
  //!
  //! GL_STACK_OVERFLOW The glPushAttrib subroutine is called while
  //!                   the attribute stack is full.
  //! GL_STACK_UNDERFLOW The glPopAttrib subroutine is called while
  //!                    the attribute stack is empty.
  //! GL_INVALID_OPERATION The glPushAttrib subroutine is called
  //!                      between a call to glBegin and the
  //!                      corresponding call to glEnd.
  virtual void glPopAttrib();

  virtual void glClear(GLbitfield mask);

  //! For every error code except for GL_OUT_OF_MEMORY, if an error
  //! code was set by calling a function, that function has no
  //! effect.  For GL_OUT_OF_MEMORY, the result is undefined.
  //!
  //!
  //! Error codes:
  //! GL_INVALID_ENUM      The enum argument is out of range.
  //!
  //! GL_INVALID_VALUE     The numeric argument is out of range.
  //!
  //! GL_INVALID_OPERATION The operation is illegal in its current
  //!                      state.  This is set for any *Pointer
  //!                      command (like glVertexAttribPointer)
  //!                      specifying the location and organization
  //!                      of vertex array data while zero is bound
  //!                      to the ARRAY_BUFFER buffer object binding
  //!                      point and the pointer argument is not
  //!                      NULL.  OpenGL 4.1 (Core Profile)
  //!
  //! GL_OUT_OF_MEMORY     Not enough memory is left to execute the command.
  //!
  //! GL_NO_ERROR          No error has occurred.
  virtual GLenum glGetError();

  virtual void glFlush();

  virtual void glEnableClientState(GLenum array);
  virtual void glDisableClientState(GLenum array);
  virtual void glDrawArrays(GLenum mode, GLint first, GLsizei count);

  virtual void glVertexPointer(GLint size, GLenum type, GLsizei stride,
                               const GLvoid *pointer);

  // Uniform functions
  virtual void glUniform3fv(GLint location, GLsizei count,
                            const GLfloat *value);
  virtual void glUniformMatrix4fv(GLint location, GLsizei count,
                                  GLboolean transpose, const GLfloat *value);

  // Virtual Buffer Object functions
  virtual void glGenBuffers(GLsizei n, GLuint *buffers);
  virtual void glBindBuffer(GLenum target, GLuint buffer);
  virtual void glBufferData(GLenum target, GLsizeiptr size, const void *data,
                            GLenum usage);
  virtual void glDeleteBuffers(GLsizei n, const GLuint *buffers);

  // Virtual Array Object functions
  virtual void glGenVertexArrays(GLsizei n, GLuint *arrays);
  virtual void glBindVertexArray(GLuint array);
  virtual void glEnableVertexAttribArray(GLuint index);

  //! parameters:
  //! GLuint index:         The shader attribute location
  //!
  //!                       It's 0 here for shader attribute in_Position:
  //!                       "layout(location=0) in vec3 in_Position;"
  //!
  //! GLint size:           The number of components to be updated per vertex,
  //!                       can be either 1, 2, 3, 4 or GL_BGRA
  //!
  //!                       It's 3 here to indicate a set of 3D vectors were
  //!                       used with x, y and z coordinates
  //!
  //! GLenum type:          the data type (GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,
  //! GL_UNSIGNED_SHORT,
  //!                       GL_INT, GL_UNSIGNED_INT, GL_FIXED, GL_HALF_FLOAT,
  //!                       GL_FLOAT, or GL_DOUBLE.
  //!
  //!                       The data is an array of floats.
  //!
  //! GLboolean normalized: indicates that the vertex data should be normalized
  //! before being stored
  //!                       using the same method as glVertexAttribFourN*()
  //!
  //!                       The data is already normalized between 0 and 1.
  //!
  //! GLsizei stride:       The byte offset between consecutive elements in the
  //! array
  //!                       If stride is zero, the data is assumed to be tightly
  //!                       packed.
  //!
  //!                       This is 0 here because the data is a continuous
  //!                       array of floats tightly packed.  Each vertex of (x,
  //!                       y, z) coordinates is immediately followed by the
  //!                       next.
  //!
  //! const void *pointer:  The offset from the start of the buffer object
  //!                       (using zero-based addressing) for the first set of
  //!                       values in the array This is in bytes.
  virtual void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
                                     GLboolean normalized, GLsizei stride,
                                     const void *pointer);
  virtual void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);

  // Shader functions
  virtual GLuint glCreateShader(GLenum type);
  virtual void glShaderSource(GLuint shader, GLsizei count,
                              const GLchar *const *string, const GLint *length);
  virtual void glCompileShader(GLuint shader);
  virtual void glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
  virtual void glGetShaderInfoLog(GLuint program, GLsizei bufSize,
                                  GLsizei *length, GLchar *infoLog);
  virtual void glDeleteShader(GLuint shader);

  // Program functions
  virtual GLuint glCreateProgram();
  virtual GLuint glAttachShader(GLuint program, GLuint shader);
  virtual void glLinkProgram(GLuint program);
  virtual void glGetProgramiv(GLuint program, GLenum pname, GLint *params);
  virtual void glGetProgramInfoLog(GLuint program, GLsizei bufSize,
                                   GLsizei *length, GLchar *infoLog);
  virtual void glUseProgram(GLuint program);
  virtual GLint glGetUniformLocation(GLuint program, const GLchar *name);
  virtual void glGetAttachedShaders(GLuint program, GLsizei maxCount,
                                    GLsizei *count, GLuint *shaders);
  virtual void glDeleteProgram(GLuint program);

  // Misc functions
  // Needed for initialization

  //! Specifies the current matrix.
  //!
  //! The glMatrixMode subroutine sets the current matrix mode. The
  //! Mode parameter can assume one of the following three values:
  //!   GL_MODELVIEW Applies subsequent matrix operations to the model
  //!   view matrix stack.
  //!   GL_PROJECTION Applies subsequent matrix operations to the
  //!   projection matrix stack.
  //!   GL_TEXTURE Applies subsequent matrix operations to the texture
  //!   matrix stack.
  //!
  //! parameters:
  //!
  //! mode: Mode Specifies which matrix stack is the target for
  //! subsequent matrix operations. The following three values are
  //! accepted:
  //!   GL_MODELVIEW
  //!   GL_PROJECTION
  //!   GL_TEXTURE
  virtual void glMatrixMode(GLenum mode);

  //! Replaces the current matrix with the identity matrix.
  //!
  //! The glLoadIdentity subroutine replaces the current matrix with
  //! the identity matrix. It is semantically equivalent to calling
  //! the glLoadMatrix subroutine with the following identity matrix:
  //!
  //! | 1 0 0 0 |
  //! | 0 1 0 0 |
  //! | 0 0 1 0 |
  //! | 0 0 0 1 |
  //!
  //! Calling glLoadIdentity is in some cases more efficient.
  //!
  //! Errors
  //!   GL_INVALID_OPERATION
  //!
  //! The glLoadIdentity subroutine is called between a call to
  //! glBegin and the corresponding call to glEnd.
  virtual void glLoadIdentity();

  //! Multiplies the current matrix by an orthographic matrix.
  //!
  //! The glOrtho subroutine describes a perspective matrix that
  //! produces a parallel projection. (Left, Bottom, -Near) and
  //! (Right, Top, -Near) specify the points on the near clipping
  //! plane that are mapped to the lower left and upper right corners
  //! of the window, respectively, assuming that the eye is located at
  //! (0, 0, 0). -Far specifies the location of the far clipping
  //! plane. Both Near and Far can be either positive or negative. The
  //! corresponding matrix is as follows
  //!
  //! |     2
  //! | ----------    0          0        t_x    |
  //! | Right-Left                               |
  //! |                                          |
  //! |               2                          |
  //! |     0     ----------     0        t_y    |
  //! |           Top-Bottom                     |
  //! |                                          |
  //! |                          2               |
  //! |     0         0       ----------  t_z    |
  //! |                        Far-Near          |
  //! |                                          |
  //! |     0         0          0         0     |
  //! |                                          |
  //!
  //!
  //!         Right+Left
  //! t_x = - ----------
  //!         Right-Left
  //!
  //!         Top+Bottom
  //! t_y = - ----------
  //!         Top-Bottom
  //!
  //!         Far+Near
  //! t_z = - --------
  //!         Far-Near
  //!
  //! The current matrix is multiplied by this matrix with the result
  //! replacing the current matrix. That is, if M is the current
  //! matrix and O is the ortho matrix, M is replaced with MO.
  //!
  //! Use the glPushMatrix and glPopMatrix subroutines to save and
  //! restore the current matrix stack.
  //!
  //! parameters:
  //!
  //! Left, Right Specify the coordinates for the left and right
  //! vertical clipping planes.
  //!
  //! Bottom, Top Specify the coordinates for the bottom and top
  //! horizontal clipping planes.
  //!
  //! Near, Far Specify the distances to the nearer and farther depth
  //! clipping planes. These distances are negative if the plane is to
  //! be behind the viewer.
  //!
  //! Errors:
  //!
  //! GL_INVALID_OPERATION The glOrtho subroutine is called between a
  //! call to glBegin and the corresponding call to glEnd
  virtual void glOrtho(GLdouble left, GLdouble right, GLdouble bottom,
                       GLdouble top, GLdouble zNear, GLdouble zFar);

  //! Enables a GL capability.
  //!
  //! glEnable enables various capabilities. Use glIsEnable or glGet
  //! to determine the current setting of any capability. glEnable
  //! takes a single argument, capability, which may assume one of the
  //! following values:
  //!
  //! GL_ALPHA_TEST If enabled, do alpha testing. (See glAlphaFunc.)
  //! GL_AUTO_NORMAL If enabled, compute surface normal vectors
  //! analytically when either GL_MAP2_VERTEX_3 or GL_MAP2_VERTEX_4 is
  //! used to generate vertices. (See glMap2.)
  //!
  //! GL_BLEND If enabled, blend the incoming red, green, blue, alpha
  //! (RGBA) color values with the values in the color buffers. (See
  //! glBlendFunc.)
  //!
  //! GL_CLIP_PLANEi If enabled, clip geometry against user-defined
  //! clipping plane i. (See glClipPlane.)
  //!
  //! GL_COLOR_ARRAY_EXT If enabled, colors are taken from the color
  //! array when glArrayElementEXT or glDrawArraysEXT is
  //! called. (SeeglColorPointerEXT, glArrayElementEXT and
  //! glDrawArraysEXT.)
  //!
  //! GL_COLOR_LOGIC_OP If enabled, apply the currently selected
  //! logical operation to the incoming color and color buffer
  //! values. The initial value is GL_FALSE. (See glLogicOp.)
  //!
  //! GL_COLOR_MATERIAL If enabled, have one or more material
  //! parameters track the current color. (See glColorMaterial.)
  //!
  //! GL_COLOR_SUM_EXT If enabled, user may specify the RGB components
  //! of the secondary color used in the Color Sum stage, instead of
  //! using the default (0,0,0,0) color. This applies only in RGBA
  //! mode and when LIGHTING is disabled. (See glSecondaryColorEXT.)
  //!
  //! GL_CULL_FACE If enabled, cull polygons based on their winding in
  //! window coordinates. (See glCullFace.)
  //!
  //! GL_CULL_VERTEX_IBM If enabled, cull polygons based on their
  //! vertex normals. When vertex culling is enabled, vertices are
  //! classified as front or back facing according to the sign of the
  //! dot product between the normal at the vertex and an eye
  //! direction vector from the vertex toward the eye position. When
  //! (normal dot eye_direction) <= 0 the vertex is classified as back
  //! facing. When (normal dot eye_direction) > 0 the vertex is
  //! classified as front facing. Vertices are culled when the face
  //! orientation determined by the dot product is the same as the
  //! face specified by CullFace. When all of the vertices of a
  //! polygon are culled, then the polygon may be culled. Unlike
  //! GL_CULL_VERTEX_EXT, vertex culling using GL_CULL_VERTEX_IBM does
  //! not necessarily result in polygons being culled even if all of
  //! the vertices of the polygon are culled. The eye direction is
  //! determined by transforming the column vector (0, 0, 1) by the
  //! upper leftmost 3x3 matrix taken from the inverse of the
  //! modelview matrix. The eye direction is undefined if the
  //! modelview matrix is singular or nearly singular. This operation
  //! in effect projects the z axis in eye coordinates back into
  //! object space. If the projection matrix or DepthRange settings
  //! cause the z axis in window coordinates to be misaligned with the
  //! z axis in eye coordinates, this extension should not be
  //! used. Vertex culling is performed independently of face
  //! culling. Polygons on the silhouettes of objects may have both
  //! front and back facing vertices. Since polygons are culled only
  //! if all of their vertices are culled and are not necessarily
  //! culled by GL_CULL_VERTEX_IBM even in that case, face culling may
  //! have to be used in addition to vertex culling in order to
  //! correctly cull silhouette polygons.
  //!
  //! GL_DEPTH_TEST If enabled, do depth comparisons and update the
  //! depth buffer. (See glDepthFunc and glDepthRange.)
  //!
  //! GL_DITHER If enabled, dither color components or indices before
  //! they are written to the color buffer.
  //!
  //! GL_EDGE_FLAG_ARRAY_EXT If enabled, edge flags are taken from the
  //! edge flags array when glArrayElementEXT or glDrawArraysEXT is
  //! called. (See glEdgeFlagPointerEXT, glArrayElementEXT and
  //! glDrawArraysEXT.)
  //!
  //! GL_FOG If enabled, blend a fog color into the post-texturing
  //! color. (See glFog.)
  //!
  //! GL_INDEX_ARRAY_EXT If enabled, color indexes are taken from the
  //! color index array when glArrayElementEXT or glDrawArraysEXT is
  //! called. (See glIndexPointerEXT, glArrayElementEXT and
  //! glDrawArraysEXT.)
  //!
  //! GL_LIGHTi If enabled, include light i in the evaluation of the
  //! lighting equation. (See glLightModel and glLight.)
  //!
  //! GL_LIGHTING If enabled, use the current lighting parameters to
  //! compute the vertex color or index. Otherwise, simply associate
  //! the current color or index with each vertex. (See glMaterial,
  //! glLightModel, and glLight.)
  //!
  //! GL_LINE_SMOOTH If enabled, draw lines with correct
  //! filtering. Otherwise, draw aliased lines. (See glLineWidth.)
  //! GL_LINE_STIPPLE If enabled, use the current line stipple pattern
  //! when drawing lines. (See glLineStipple.)
  //!
  //! GL_LOGIC_OP If enabled, apply the currently selected logical
  //! operation to the incoming and color buffer indices. (See
  //! glLogicOp.)
  //!
  //! GL_MAP1_COLOR_4 If enabled, calls to glEvalCoord1, glEvalMesh1,
  //! and glEvalPoint1 will generate RGBA values. (See glMap1.)
  //!
  //! GL_MAP1_INDEX If enabled, calls to glEvalCoord1, glEvalMesh1,
  //! and glEvalPoint1 will generate color indices. (See glMap1.)
  //!
  //! GL_MAP1_NORMAL If enabled, calls to glEvalCoord1, glEvalMesh1,
  //! and glEvalPoint1 will generate normals. (See glMap1.)
  //!
  //! GL_MAP1_TEXTURE_COORD_1 If enabled, calls to glEvalCoord1,
  //! glEvalMesh1, and glEvalPoint1 will generate s texture
  //! coordinates. (See glMap1.)
  //!
  //! GL_MAP1_TEXTURE_COORD_2 If enabled, calls to glEvalCoord1,
  //! glEvalMesh1, and glEvalPoint1 will generate s and t texture
  //! coordinates. (See glMap1.)
  //!
  //! GL_MAP1_TEXTURE_COORD_3 If enabled, calls to glEvalCoord1,
  //! glEvalMesh1, and glEvalPoint1 will generate s, t, and r texture
  //! coordinates. (See glMap1.)
  //!
  //! GL_MAP1_TEXTURE_COORD_4 If enabled, calls to glEvalCoord1,
  //! glEvalMesh1, and glEvalPoint1 will generate s, t, r, and q
  //! texture coordinates. (See glMap1.)
  //!
  //! GL_MAP1_VERTEX_3 If enabled, calls to glEvalCoord1, glEvalMesh1,
  //! and glEvalPoint1 will generate will generate x, y, and z vertex
  //! coordinates. (See glMap1.)
  //!
  //! GL_MAP1_VERTEX_4 If enabled, calls to glEvalCoord1, glEvalMesh1,
  //! and glEvalPoint1 will generate homogeneous x, y, z, and w vertex
  //! coordinates. (See glMap1.)
  //!
  //! GL_MAP2_COLOR_4 If enabled, calls to glEvalCoord2, glEvalMesh2,
  //! and glEvalPoint2 will generate RGBA values. (See glMap2.)
  //!
  //! GL_MAP2_INDEX If enabled, calls to glEvalCoord2, glEvalMesh2,
  //! and glEvalPoint2 will generate color indices. (See glMap2.)
  //!
  //! GL_MAP2_NORMAL If enabled, calls to glEvalCoord2, glEvalMesh2,
  //! and glEvalPoint2 will generate normals. (See glMap2.)
  //!
  //! GL_MAP2_TEXTURE_COORD_1 If enabled, calls to glEvalCoord2,
  //! glEvalMesh2, and glEvalPoint2 will generate s texture
  //! coordinates. (See glMap2.)
  //!
  //! GL_MAP2_TEXTURE_COORD_2 If enabled, calls to glEvalCoord2,
  //! glEvalMesh2, and glEvalPoint2 will generate s and t texture
  //! coordinates. (See glMap2.)
  //!
  //! GL_MAP2_TEXTURE_COORD_3 If enabled, calls to glEvalCoord2,
  //! glEvalMesh2, and glEvalPoint2 will generate s, t, and r texture
  //! coordinates. (See glMap2.)
  //!
  //! GL_MAP2_TEXTURE_COORD_4 If enabled, calls to glEvalCoord2,
  //! glEvalMesh2, and glEvalPoint2 will generate s, t, r, and q
  //! texture coordinates. (See glMap2.)
  //!
  //! GL_MAP2_VERTEX_3 If enabled, calls to glEvalCoord2, glEvalMesh2,
  //! and glEvalPoint2 will generate will generate x, y, and z vertex
  //! coordinates. (See glMap2.)
  //!
  //! GL_MAP2_VERTEX_4 If enabled, calls to glEvalCoord2, glEvalMesh2,
  //! and glEvalPoint2 will generate homogeneous x, y, z, and w vertex
  //! coordinates. (See glMap2.)
  //!
  //! GL_NORMAL_ARRAY_EXT If enabled, normals are taken from the
  //! normal array when glArrayElementEXT or glDrawArraysEXT is
  //! called. (See glNormalPointerEXT, glArrayElementEXT and
  //! glDrawArraysEXT.)
  //!
  //! GL_NORMALIZE If enabled, normal vectors specified with glNormal
  //! are scaled to unit length after transformation. (See glNormal.)
  //!
  //! GL_OCCLUSION_CULLING_HP If enabled, the occlusion testing
  //! described within extension HP_occlusion_test is performed. This
  //! extension allows an application to render some geometry and, at
  //! the completion of the rendering, to determine if any of the
  //! geometry could or did modify the depth buffer (in other words, a
  //! depth buffer test succeeded). (See glGet with parameter
  //! GL_OCCLUSION_TEST_RESULT_HP). Occlusion culling operates
  //! independently of the current rendering state (in other words,
  //! when occlusion culling is enabled, fragments are generated and
  //! the depth and/or color buffer may be updated). To prevent
  //! updating the depth/color buffers, the application must disable
  //! updates to these buffers. As a side effect of calling glGet with
  //! parameter GL_OCCLUSION_TEST_RESULT_HP, the internal result state
  //! is cleared, and it is reset for a new bounding box test.
  //!
  //! GL_POLYGON_OFFSET_EXT If enabled, an offset is added to z values
  //! of a polygon's fragments before the depth comparison is
  //! performed. (See glPolygonOffsetEXT.)
  //!
  //! GL_POLYGON_OFFSET_FILL If enabled, and if the polygon is
  //! rendered in GL_FILL mode, anoffset is added to z values of a
  //! polygon's fragments before the depth comparison is
  //! performed. The initial value is GL_FALSE. (See glPolygonOffset.)
  //!
  //! GL_POLYGON_OFFSET_LINE If enabled, and if the polygon is
  //! rendered in GL_LINE mode, an offset is added to z values of a
  //! polygon's fragments before the depth comparison is
  //! performed. The initial value is GL_FALSE. (See glPolygonOffset.)
  //!
  //! GL_POLYGON_OFFSET_POINT If enabled, an offset is added to z values
  //! of a polygon's fragments before the depth comparison is performed,
  //! if the polygon is rendered in GL_POINT mode. The initial value is
  //! GL_FALSE. (See glPolygonOffset.)
  //!
  //! GL_POINT_SMOOTH If enabled, draw points with proper
  //! filtering. Otherwise, draw aliased points. (See glPointSize.)
  //!
  //! GL_POLYGON_SMOOTH If enabled, draw polygons with proper
  //! filtering. Otherwise, draw aliased polygons. (See glPolygonMode.)
  //!
  //! GL_POLYGON_STIPPLE If enabled, use the current polygon stipple
  //! pattern when rendering polygons. (See glPolygonStipple.)
  //!
  //! GL_RESCALE_NORMAL If normal rescaling is enabled, a new operation
  //! is added to the transformation of the normal vector into eye
  //! coordinates. The normal vector is rescaled after it is multiplied
  //! by the inverse modelview matrix and before it is normalized.
  //!
  //! GL_RESCALE_NORMAL_EXT If normal rescaling is enabled, a new
  //! operation is added to the transformation of the normal vector into
  //! eye coordinates. The normal vector is rescaled after it is
  //! multiplied by the inverse modelview matrix and before it is
  //! normalized.
  //!
  //! GL_SCISSOR_TEST If enabled, discard fragments that are outside the
  //! scissor rectangle. (See glScissor.)
  //!
  //! GL_STENCIL_TEST If enabled, do stencil testing and update the
  //! stencil buffer. (See glStencilFunc and glStencilOp.)
  //!
  //! GL_TEXTURE_1D If enabled, one-dimensional texturing is performed
  //! (unless two-dimensional texturing is also enabled). (See
  //! glTexImage1D.)
  //!
  //! GL_TEXTURE_2D If enabled, two-dimensional texturing is
  //! performed. (See glTexImage2D.)
  //!
  //! GL_TEXTURE_3D If enabled, three-dimensional texturing is
  //! performed. (See glTexImage3D.)
  //!
  //! GL_TEXTURE_3D_EXT If enabled, three-dimensional texture mapping
  //! is performed. (See glTexImage3DEXT.)
  //!
  //! GL_TEXTURE_COLOR_TABLE_EXT If enabled, a color lookup table is
  //! added to the texture mechanism. (See glColorTable.)
  //!
  //! GL_TEXTURE_COORD_ARRAY_EXT If enabled, texture coordinates are
  //! taken from the texture coordinates array when glArrayElementEXT
  //! or glDrawArraysEXT is called. (See glTexCoordPointerEXT,
  //! glArrayElementEXT and glDrawArraysEXT.)
  //!
  //! GL_TEXTURE_GEN_Q If enabled, the q texture coordinate is
  //! computed using the texture generation function defined with
  //! glTexGen. Otherwise the current q texture coordinate is
  //! used. (See glTexGen.)
  //!
  //! GL_TEXTURE_GEN_R If enabled, the r texture coordinate is
  //! computed using the texture generation function defined with
  //! glTexGen. Otherwise the current r texture coordinate is
  //! used. (See glTexGen.)
  //!
  //! GL_TEXTURE_GEN_S If enabled, the s texture coordinate is
  //! computed using the texture generation function defined with
  //! glTexGen. Otherwise the current s texture coordinate is
  //! used. (See glTexGen.)
  //!
  //! GL_TEXTURE_GEN_T If enabled, the t texture coordinate is
  //! computed using the texture generation function defined with
  //! glTexGen. Otherwise, the current t texture coordinate is
  //! used. (See glTexGen.)
  //!
  //! GL_UPDATE_CLIP_VOLUME_HINT If enabled, calls to
  //! ClipBoundingBoxIBM, ClipBoundingSphereIBM, and
  //! ClipBoundingVerticesIBM will result in updates to the
  //! VOLUME_CLIPPING_HINT_EXT state. A result of REJECT_IBM causes
  //! the hint to be set to DONT_CARE. A result of CLIP_IBM causes the
  //! hint to be set to NICEST. A result of ACCEPT_IBM causes the hint
  //! to be set to FASTEST. If the EXT_clip_volume_hint extension is
  //! not supported, then the UPDATE_CLIP_VOLUME_HINT enable state has
  //! no effect. (See glClipBoundingBoxIBM, glClipBoundingSphereIBM,
  //! or glClipBoundingVerticesIBM, )
  //!
  //! GL_VERTEX_ARRAY_EXT If enabled, vertexes are taken from the
  //! vertex array when glArrayElementEXT or glDrawArraysEXT is
  //! called. (See glVertexPointerEXT, glArrayElementEXT and
  //! glDrawArraysEXT.)
  //!
  //! parameters:
  //!
  //! \param capability Specifies a symbolic constant indicating a GL
  //!                   capability. Initially, all are disabled except
  //!                   GL_DITHER.
  //!
  //! Errors:
  //!
  //! GL_INVALID_ENUM capability is not an accepted value.
  //! GL_INVALID_OPERATION The glEnable subroutine is called between a
  //! call to glBegin and the corresponding call to glEnd.
  virtual void glEnable(GLenum cap);

  //! Disables a GL capability.
  //!
  //! Description
  //!
  //! glEnable    and   glDisable    enable   and    disable   various
  //! capabilities. Use  glIsEnable or glGet to  determine the current
  //! setting of  any capability. Both  glEnable and glDisable  take a
  //! single  argument,  capability,  which  may  assume  one  of  the
  //! following values:
  //!

  //! GL_ALPHA_TEST If enabled, do alpha testing. (See glAlphaFunc.)
  //! GL_AUTO_NORMAL If enabled, compute surface normal vectors
  //! analytically when either GL_MAP2_VERTEX_3 or GL_MAP2_VERTEX_4 is
  //! used to generate vertices. (See glMap2.)
  //!
  //! GL_BLEND If enabled, blend the incoming red, green, blue, alpha
  //! (RGBA) color values with the values in the color buffers. (See
  //! glBlendFunc.)
  //!
  //! GL_CLIP_PLANEi If enabled, clip geometry against user-defined
  //! clipping plane i. (See glClipPlane.)
  //!
  //! GL_COLOR_ARRAY_EXT If enabled, colors are taken from the color
  //! array when glArrayElementEXT or glDrawArraysEXT is
  //! called. (SeeglColorPointerEXT, glArrayElementEXT and
  //! glDrawArraysEXT.)
  //!
  //! GL_COLOR_LOGIC_OP If enabled, apply the currently selected
  //! logical operation to the incoming color and color buffer
  //! values. The initial value is GL_FALSE. (See glLogicOp.)
  //!
  //! GL_COLOR_MATERIAL If enabled, have one or more material
  //! parameters track the current color. (See glColorMaterial.)
  //!
  //! GL_COLOR_SUM_EXT If enabled, user may specify the RGB components
  //! of the secondary color used in the Color Sum stage, instead of
  //! using the default (0,0,0,0) color. This applies only in RGBA
  //! mode and when LIGHTING is disabled. (See glSecondaryColorEXT.)
  //!
  //! GL_CULL_FACE If enabled, cull polygons based on their winding in
  //! window coordinates. (See glCullFace.)
  //!
  //! GL_CULL_VERTEX_IBM If enabled, cull polygons based on their
  //! vertex normals. When vertex culling is enabled, vertices are
  //! classified as front or back facing according to the sign of the
  //! dot product between the normal at the vertex and an eye
  //! direction vector from the vertex toward the eye position. When
  //! (normal dot eye_direction) <= 0 the vertex is classified as back
  //! facing. When (normal dot eye_direction) > 0 the vertex is
  //! classified as front facing. Vertices are culled when the face
  //! orientation determined by the dot product is the same as the
  //! face specified by CullFace. When all of the vertices of a
  //! polygon are culled, then the polygon may be culled. Unlike
  //! GL_CULL_VERTEX_EXT, vertex culling using GL_CULL_VERTEX_IBM does
  //! not necessarily result in polygons being culled even if all of
  //! the vertices of the polygon are culled. The eye direction is
  //! determined by transforming the column vector (0, 0, 1) by the
  //! upper leftmost 3x3 matrix taken from the inverse of the
  //! modelview matrix. The eye direction is undefined if the
  //! modelview matrix is singular or nearly singular. This operation
  //! in effect projects the z axis in eye coordinates back into
  //! object space. If the projection matrix or DepthRange settings
  //! cause the z axis in window coordinates to be misaligned with the
  //! z axis in eye coordinates, this extension should not be
  //! used. Vertex culling is performed independently of face
  //! culling. Polygons on the silhouettes of objects may have both
  //! front and back facing vertices. Since polygons are culled only
  //! if all of their vertices are culled and are not necessarily
  //! culled by GL_CULL_VERTEX_IBM even in that case, face culling may
  //! have to be used in addition to vertex culling in order to
  //! correctly cull silhouette polygons.
  //!
  //! GL_DEPTH_TEST If enabled, do depth comparisons and update the
  //! depth buffer. (See glDepthFunc and glDepthRange.)
  //!
  //! GL_DITHER If enabled, dither color components or indices before
  //! they are written to the color buffer.
  //!
  //! GL_EDGE_FLAG_ARRAY_EXT If enabled, edge flags are taken from the
  //! edge flags array when glArrayElementEXT or glDrawArraysEXT is
  //! called. (See glEdgeFlagPointerEXT, glArrayElementEXT and
  //! glDrawArraysEXT.)
  //!
  //! GL_FOG If enabled, blend a fog color into the post-texturing
  //! color. (See glFog.)
  //!
  //! GL_INDEX_ARRAY_EXT If enabled, color indexes are taken from the
  //! color index array when glArrayElementEXT or glDrawArraysEXT is
  //! called. (See glIndexPointerEXT, glArrayElementEXT and
  //! glDrawArraysEXT.)
  //!
  //! GL_LIGHTi If enabled, include light i in the evaluation of the
  //! lighting equation. (See glLightModel and glLight.)
  //!
  //! GL_LIGHTING If enabled, use the current lighting parameters to
  //! compute the vertex color or index. Otherwise, simply associate
  //! the current color or index with each vertex. (See glMaterial,
  //! glLightModel, and glLight.)
  //!
  //! GL_LINE_SMOOTH If enabled, draw lines with correct
  //! filtering. Otherwise, draw aliased lines. (See glLineWidth.)
  //! GL_LINE_STIPPLE If enabled, use the current line stipple pattern
  //! when drawing lines. (See glLineStipple.)
  //!
  //! GL_LOGIC_OP If enabled, apply the currently selected logical
  //! operation to the incoming and color buffer indices. (See
  //! glLogicOp.)
  //!
  //! GL_MAP1_COLOR_4 If enabled, calls to glEvalCoord1, glEvalMesh1,
  //! and glEvalPoint1 will generate RGBA values. (See glMap1.)
  //!
  //! GL_MAP1_INDEX If enabled, calls to glEvalCoord1, glEvalMesh1,
  //! and glEvalPoint1 will generate color indices. (See glMap1.)
  //!
  //! GL_MAP1_NORMAL If enabled, calls to glEvalCoord1, glEvalMesh1,
  //! and glEvalPoint1 will generate normals. (See glMap1.)
  //!
  //! GL_MAP1_TEXTURE_COORD_1 If enabled, calls to glEvalCoord1,
  //! glEvalMesh1, and glEvalPoint1 will generate s texture
  //! coordinates. (See glMap1.)
  //!
  //! GL_MAP1_TEXTURE_COORD_2 If enabled, calls to glEvalCoord1,
  //! glEvalMesh1, and glEvalPoint1 will generate s and t texture
  //! coordinates. (See glMap1.)
  //!
  //! GL_MAP1_TEXTURE_COORD_3 If enabled, calls to glEvalCoord1,
  //! glEvalMesh1, and glEvalPoint1 will generate s, t, and r texture
  //! coordinates. (See glMap1.)
  //!
  //! GL_MAP1_TEXTURE_COORD_4 If enabled, calls to glEvalCoord1,
  //! glEvalMesh1, and glEvalPoint1 will generate s, t, r, and q
  //! texture coordinates. (See glMap1.)
  //!
  //! GL_MAP1_VERTEX_3 If enabled, calls to glEvalCoord1, glEvalMesh1,
  //! and glEvalPoint1 will generate will generate x, y, and z vertex
  //! coordinates. (See glMap1.)
  //!
  //! GL_MAP1_VERTEX_4 If enabled, calls to glEvalCoord1, glEvalMesh1,
  //! and glEvalPoint1 will generate homogeneous x, y, z, and w vertex
  //! coordinates. (See glMap1.)
  //!
  //! GL_MAP2_COLOR_4 If enabled, calls to glEvalCoord2, glEvalMesh2,
  //! and glEvalPoint2 will generate RGBA values. (See glMap2.)
  //!
  //! GL_MAP2_INDEX If enabled, calls to glEvalCoord2, glEvalMesh2,
  //! and glEvalPoint2 will generate color indices. (See glMap2.)
  //!
  //! GL_MAP2_NORMAL If enabled, calls to glEvalCoord2, glEvalMesh2,
  //! and glEvalPoint2 will generate normals. (See glMap2.)
  //!
  //! GL_MAP2_TEXTURE_COORD_1 If enabled, calls to glEvalCoord2,
  //! glEvalMesh2, and glEvalPoint2 will generate s texture
  //! coordinates. (See glMap2.)
  //!
  //! GL_MAP2_TEXTURE_COORD_2 If enabled, calls to glEvalCoord2,
  //! glEvalMesh2, and glEvalPoint2 will generate s and t texture
  //! coordinates. (See glMap2.)
  //!
  //! GL_MAP2_TEXTURE_COORD_3 If enabled, calls to glEvalCoord2,
  //! glEvalMesh2, and glEvalPoint2 will generate s, t, and r texture
  //! coordinates. (See glMap2.)
  //!
  //! GL_MAP2_TEXTURE_COORD_4 If enabled, calls to glEvalCoord2,
  //! glEvalMesh2, and glEvalPoint2 will generate s, t, r, and q
  //! texture coordinates. (See glMap2.)
  //!
  //! GL_MAP2_VERTEX_3 If enabled, calls to glEvalCoord2, glEvalMesh2,
  //! and glEvalPoint2 will generate will generate x, y, and z vertex
  //! coordinates. (See glMap2.)
  //!
  //! GL_MAP2_VERTEX_4 If enabled, calls to glEvalCoord2, glEvalMesh2,
  //! and glEvalPoint2 will generate homogeneous x, y, z, and w vertex
  //! coordinates. (See glMap2.)
  //!
  //! GL_NORMAL_ARRAY_EXT If enabled, normals are taken from the
  //! normal array when glArrayElementEXT or glDrawArraysEXT is
  //! called. (See glNormalPointerEXT, glArrayElementEXT and
  //! glDrawArraysEXT.)
  //!
  //! GL_NORMALIZE If enabled, normal vectors specified with glNormal
  //! are scaled to unit length after transformation. (See glNormal.)
  //!
  //! GL_OCCLUSION_CULLING_HP If enabled, the occlusion testing
  //! described within extension HP_occlusion_test is performed. This
  //! extension allows an application to render some geometry and, at
  //! the completion of the rendering, to determine if any of the
  //! geometry could or did modify the depth buffer (in other words, a
  //! depth buffer test succeeded). (See glGet with parameter
  //! GL_OCCLUSION_TEST_RESULT_HP). Occlusion culling operates
  //! independently of the current rendering state (in other words,
  //! when occlusion culling is enabled, fragments are generated and
  //! the depth and/or color buffer may be updated). To prevent
  //! updating the depth/color buffers, the application must disable
  //! updates to these buffers. As a side effect of calling glGet with
  //! parameter GL_OCCLUSION_TEST_RESULT_HP, the internal result state
  //! is cleared, and it is reset for a new bounding box test.
  //!
  //! GL_POLYGON_OFFSET_EXT If enabled, an offset is added to z values
  //! of a polygon's fragments before the depth comparison is
  //! performed. (See glPolygonOffsetEXT.)
  //!
  //! GL_POLYGON_OFFSET_FILL If enabled, and if the polygon is
  //! rendered in GL_FILL mode, anoffset is added to z values of a
  //! polygon's fragments before the depth comparison is
  //! performed. The initial value is GL_FALSE. (See glPolygonOffset.)
  //!
  //! GL_POLYGON_OFFSET_LINE If enabled, and if the polygon is
  //! rendered in GL_LINE mode, an offset is added to z values of a
  //! polygon's fragments before the depth comparison is
  //! performed. The initial value is GL_FALSE. (See glPolygonOffset.)
  //!
  //! GL_POLYGON_OFFSET_POINT If enabled, an offset is added to z values
  //! of a polygon's fragments before the depth comparison is performed,
  //! if the polygon is rendered in GL_POINT mode. The initial value is
  //! GL_FALSE. (See glPolygonOffset.)
  //!
  //! GL_POINT_SMOOTH If enabled, draw points with proper
  //! filtering. Otherwise, draw aliased points. (See glPointSize.)
  //!
  //! GL_POLYGON_SMOOTH If enabled, draw polygons with proper
  //! filtering. Otherwise, draw aliased polygons. (See glPolygonMode.)
  //!
  //! GL_POLYGON_STIPPLE If enabled, use the current polygon stipple
  //! pattern when rendering polygons. (See glPolygonStipple.)
  //!
  //! GL_RESCALE_NORMAL If normal rescaling is enabled, a new operation
  //! is added to the transformation of the normal vector into eye
  //! coordinates. The normal vector is rescaled after it is multiplied
  //! by the inverse modelview matrix and before it is normalized.
  //!
  //! GL_RESCALE_NORMAL_EXT If normal rescaling is enabled, a new
  //! operation is added to the transformation of the normal vector into
  //! eye coordinates. The normal vector is rescaled after it is
  //! multiplied by the inverse modelview matrix and before it is
  //! normalized.
  //!
  //! GL_SCISSOR_TEST If enabled, discard fragments that are outside the
  //! scissor rectangle. (See glScissor.)
  //!
  //! GL_STENCIL_TEST If enabled, do stencil testing and update the
  //! stencil buffer. (See glStencilFunc and glStencilOp.)
  //!
  //! GL_TEXTURE_1D If enabled, one-dimensional texturing is performed
  //! (unless two-dimensional texturing is also enabled). (See
  //! glTexImage1D.)
  //!
  //! GL_TEXTURE_2D If enabled, two-dimensional texturing is
  //! performed. (See glTexImage2D.)
  //!
  //! GL_TEXTURE_3D If enabled, three-dimensional texturing is
  //! performed. (See glTexImage3D.)
  //!
  //! GL_TEXTURE_3D_EXT If enabled, three-dimensional texture mapping
  //! is performed. (See glTexImage3DEXT.)
  //!
  //! GL_TEXTURE_COLOR_TABLE_EXT If enabled, a color lookup table is
  //! added to the texture mechanism. (See glColorTable.)
  //!
  //! GL_TEXTURE_COORD_ARRAY_EXT If enabled, texture coordinates are
  //! taken from the texture coordinates array when glArrayElementEXT
  //! or glDrawArraysEXT is called. (See glTexCoordPointerEXT,
  //! glArrayElementEXT and glDrawArraysEXT.)
  //!
  //! GL_TEXTURE_GEN_Q If enabled, the q texture coordinate is
  //! computed using the texture generation function defined with
  //! glTexGen. Otherwise the current q texture coordinate is
  //! used. (See glTexGen.)
  //!
  //! GL_TEXTURE_GEN_R If enabled, the r texture coordinate is
  //! computed using the texture generation function defined with
  //! glTexGen. Otherwise the current r texture coordinate is
  //! used. (See glTexGen.)
  //!
  //! GL_TEXTURE_GEN_S If enabled, the s texture coordinate is
  //! computed using the texture generation function defined with
  //! glTexGen. Otherwise the current s texture coordinate is
  //! used. (See glTexGen.)
  //!
  //! GL_TEXTURE_GEN_T If enabled, the t texture coordinate is
  //! computed using the texture generation function defined with
  //! glTexGen. Otherwise, the current t texture coordinate is
  //! used. (See glTexGen.)
  //!
  //! GL_UPDATE_CLIP_VOLUME_HINT  If enabled, calls to
  //! ClipBoundingBoxIBM, ClipBoundingSphereIBM, and
  //! ClipBoundingVerticesIBM will result in updates to the
  //! VOLUME_CLIPPING_HINT_EXT state. A result of REJECT_IBM causes
  //! the hint to be set to DONT_CARE. A result of CLIP_IBM causes the
  //! hint to be set to NICEST. A result of ACCEPT_IBM causes the hint
  //! to be set to FASTEST. If the EXT_clip_volume_hint extension is
  //! not supported, then the UPDATE_CLIP_VOLUME_HINT enable state has
  //! no effect. (See glClipBoundingBoxIBM, glClipBoundingSphereIBM,
  //! or glClipBoundingVerticesIBM, )
  //!
  //! GL_VERTEX_ARRAY_EXT If enabled, vertexes are taken from the
  //! vertex array when glArrayElementEXT or glDrawArraysEXT is
  //! called. (See glVertexPointerEXT, glArrayElementEXT and
  //! glDrawArraysEXT.)
  //!
  //! parameters:
  //!
  //! \param capability Specifies a symbolic constant indicating a GL
  //!                   capability. Initially, all are disabled except
  //!                   GL_DITHER.
  //!
  //! Errors:
  //!
  //! GL_INVALID_ENUM capability is not an accepted value.
  //! GL_INVALID_OPERATION The glEnable subroutine is called between a
  //! call to glBegin and the corresponding call to glEnd.
  virtual void glDisable(GLenum cap);

  //! Specifies the function used for depth buffer comparisons.
  //!
  //! The glDepthFunc subroutine specifies the function used to
  //! compare each incoming pixel z value with the z value present in
  //! the depth buffer. The comparison is performed only if depth
  //! testing is enabled. (See glEnable and glDisable of
  //! GL_DEPTH_TEST.)
  //!
  //! The function parameter specifies the conditions under which the
  //! pixel will be drawn. The comparison functions are as follows:
  //!
  //! GL_NEVER Never passes.
  //! GL_LESS Passes if the incoming z value is less than the stored z
  //! value.
  //! GL_EQUAL Passes if the incoming z value is equal to the stored z
  //! value.
  //! GL_LEQUAL Passes if the incoming z value is less than or equal
  //! to the stored z value.
  //! GL_GREATER Passes if the incoming z value is greater than the
  //! stored z value.
  //! GL_NOTEQUAL Passes if the incoming z value is not equal to the
  //! stored z value.
  //! GL_GEQUAL Passes if the incoming z value is greater than or
  //! equal to the stored z value.
  //! GL_ALWAYS Always passes.
  //!
  //! The default value of function is GL_LESS. Initially, depth
  //! testing is disabled.
  //!
  //! parameters:
  //!   function Specifies the depth comparison function. Symbolic
  //!   constants GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER,
  //!   GL_NOTEQUAL, GL_GEQUAL, and GL_ALWAYS are accepted. The
  //!   default function is GL_LESS.
  //!
  //! Errors:
  //!  GL_INVALID_ENUM function is not an accepted value.
  //!  GL_INVALID_OPERATION The glDepthFunc subroutine is called
  //!  between a call to glBegin and the corresponding call to glEnd
  virtual void glDepthFunc(GLenum func);

  //! Selects flat or smooth shading.
  //!
  //! parameters:
  //! Mode Specifies a symbolic value representing a shading technique. Accepted
  //! values are GL_FLAT and GL_SMOOTH. The default is GL_SMOOTH.
  //!
  //! GL primitives can have either flat or smooth shading. Smooth
  //! shading, the default, causes the computed colors of vertices to
  //! be interpolated as the primitive is rasterized, typically
  //! assigning different colors to each resulting pixel
  //! fragment. Flat shading selects the computed color of just one
  //! vertex and assigns it to all the pixel fragments generated by
  //! rasterizing a single primitive. In either case, the computed
  //! color of a vertex is the result of lighting, if lighting is
  //! enabled, or it is the current color at the time the vertex was
  //! specified, if lighting is disabled.
  //!
  //! Flat and smooth shading are indistinguishable for
  //! points. Counting vertices and primitives from 1 (one) starting
  //! when the glBegin subroutine is issued, each flat-shaded line
  //! segment i is given the computed color of vertex i + 1, its
  //! second vertex. Counting similarly from 1, each flat-shaded
  //! polygon is given the computed color of the vertex in the
  //! following list. This is the last vertex to specify the polygon
  //! in all cases except single polygons, where the first vertex
  //! specifies the flat-shaded color.
  //!
  //! Primitive type of polygon i Vertex
  //! Single polygon (i == 1) 1
  //! Triangle strip i + 2
  //! Triangle fan i + 2
  //! Independent triangle 3 i
  //! Quad strip 2 i + 2
  //! Independent quad 4 i
  //!
  //! Flat and smooth shading are specified by glShadeModel with the
  //! Mode parameter set to GL_FLAT and GL_SMOOTH, respectively.
  //!
  //! Errors:
  //!
  //! GL_INVALID_ENUM Mode is any value other than GL_FLAT or
  //! GL_SMOOTH.
  //! GL_INVALID_OPERATION The glShadeModel subroutine is called
  //! between a call to glBegin and the corresponding call to glEnd.
  virtual void glShadeModel(GLenum mode);

  //! Specifies clear values for the color buffers.
  //!
  //! The glClearColor subroutine specifies the red, green, blue, and
  //! alpha values used by the glClear subroutine to clear the color
  //! buffers. Values specified by glClearColor are clamped to the
  //! range [0,1].

  //! parameters:
  //!
  //! Red Specifies the red value used when the color buffer is
  //! cleared. The default value is 0 (zero).
  //!
  //! Green Specifies the green value used when the color buffer is cleared. The
  //! default value is 0.
  //!
  //! Blue Specifies the blue value used when the color buffer is
  //! cleared. The default value is 0.
  //!
  //! Alpha Specifies the alpha value used when the color buffer is
  //! cleared. The default value is 0.
  //!
  //! Errors:
  //!
  //! GL_INVALID_OPERATION The glClearColor subroutine is called
  //! between a call to glBegin and the corresponding call to glEnd
  virtual void glClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a);

  //! Sets the current color
  //!
  //! The Graphics Library stores both a current single-valued color
  //! index and a current four-valued red, green, blue, alpha (RGBA)
  //! color. The glColor subroutine sets a new four-valued RGBA
  //! color. The glColor subroutine has two major variants: glColor3
  //! and glColor4. glColor3 variants specify new red, green, and blue
  //! values explicitly, and set the current alpha value to 1.0
  //! implicitly. glColor4 variants specify all four color components
  //! explicitly.
  //!
  //! glColor3b, glColor4b, glColor3s, glColor4s, glColor3i, and
  //! glColor4i take 3 or 4 unsigned byte, short, or long integers as
  //! arguments. When v is appended to the name, the color subroutines
  //! can take a pointer to an array of such values.
  //!
  //! Current color values are stored in floating-point format, with
  //! unspecified mantissa and exponent sizes.  Unsigned integer color
  //! components, when specified, are linearly mapped to
  //! floating-point values such that the largest representable value
  //! maps to 1.0 (full intensity), and 0 (zero) maps to 0.0 (zero
  //! intensity). Signed integer color components, when specified, are
  //! linearly mapped to floating-point values such that the most
  //! positive representable value maps to 1.0, and the most negative
  //! representable value maps to -1.0. Floating-point values are
  //! mapped directly.
  //!
  //! Neither floating-point nor signed integer specified values are
  //! clamped to the range [0,1] before updating the current
  //! color. However, color components are clamped to this range
  //! before they are interpolated or written into a color buffer.
  //!
  //! parameters:
  //!
  //! Red Specifies a red value for the current color. The initial
  //! value is 1 (one).
  //!
  //! Green Specifies a green value for the current color. The initial
  //! value is 1 (one).
  //!
  //! Blue Specifies a blue value for the current color. The initial
  //! value is 1 (one).
  //!
  //! Alpha Specifies a new alpha value for the current
  //! color. Included only in the four-argument glColor
  //! subroutine. The initial value is 1 (one).
  //!
  //! Variable Specifies a pointer to an array that contains red,
  //! green, blue, and (sometimes) alpha values.
  //!
  //! Notes:
  //!
  //! The current color can be updated at any time. In particular,
  //! glColor can be called between a call to glBegin and the
  //! corresponding call to glEnd.
  //!
  virtual void glColor3fv(const GLfloat *color);

  // Drawing Functions

  //! Delimits the vertices of a primitive or group of like primitives.
  //!
  //! Description
  //!
  //! The glBegin and glEnd subroutines delimit the vertices that
  //! define a primitive or group of like primitives.  The glBegin
  //! subroutine accepts a single argument that specifies which of 10
  //! ways the vertices will be interpreted. Taking n as an integer
  //! count starting at 1 (one), and N as the total number of vertices
  //! specified, the interpretations are:
  //!
  //! GL_POINTS          Treats each vertex as a single point. Vertex n defines
  //!                    point n. N points are drawn.
  //!
  //! GL_LINES           Treats each pair of vertices as an independent line
  //!                    segment. Vertices 2n-1 and 2n define line
  //!                    n. N/2 lines are drawn.
  //!
  //! GL_LINE_STRIP      Draws a connected group of line segments from the
  //!                    first vertex to the last. Vertices n and n+1
  //!                    define line n. N-1 lines are drawn.
  //!
  //! GL_LINE_LOOP       Draws a connected group of line segments from the
  //!                    first vertex to the last, then back to the
  //!                    first. Vertices n and n+1 define line n. The
  //!                    last line, however, is defined by vertices N
  //!                    and 1. N lines are drawn.
  //!
  //! GL_TRIANGLES       Treats each triplet of vertices as an independent
  //!                    triangle. Vertices 3n-2, 3n-1, and 3n define
  //!                    triangle n. N/3 triangles are drawn.
  //!
  //! GL_TRIANGLE_STRIP  Draws a connected group of triangles. One
  //!                    triangle is defined for each vertex presented
  //!                    after the first two vertices. For odd n,
  //!                    vertices n, n+1, and n+2 define triangle
  //!                    n. For even n, vertices n+1, n, and n+2
  //!                    define triangle n. N-2 triangles are drawn.
  //!
  //! GL_TRIANGLE_FAN    Draws a connected group of triangles. One
  //!                    triangle is defined for each vertex presented
  //!                    after the first two vertices. Vertices 1,
  //!                    n+1, and n+2 define triangle n. N-2 triangles
  //!                    are drawn.
  //!
  //! GL_QUADS           Treats each group of four vertices as an independent
  //!                    quadrilateral. Vertices 4n-3, 4n-2, 4n-1, and
  //!                    4n define quadrilateral n. N/4 quadrilaterals
  //!                    are drawn.
  //!
  //! GL_QUAD_STRIP      Draws a connected group of quadrilaterals. One
  //!                    quadrilateral is defined for each pair of
  //!                    vertices presented after the first
  //!                    pair. Vertices 2n-1, 2n, 2n+2, and 2n+1
  //!                    define quadrilateral n. N/2-1 quadrilaterals
  //!                    are drawn. Note that the order in which
  //!                    vertices are used to construct a
  //!                    quadrilateral from strip data is different
  //!                    from that used with independent data.
  //!
  //! GL_POLYGON         Draws a single, convex polygon. Vertices 1 through N
  //!                    define this polygon.
  //!
  //! Only a subset of GL subroutines can be used between the glBegin
  //! and glEnd subroutines. The subroutines are: glVertex, glColor,
  //! glIndex, glNormal, glTexCoord, glEvalCoord, glEvalPoint,
  //! glMaterial, and glEdgeFlag. Also, it is acceptable to use
  //! glCallList or glCallLists to execute display lists that include
  //! only the preceding subroutines. If any other GL subroutine is
  //! called between the glBegin and glEnd subroutines, the error flag
  //! is set and the subroutine is ignored.
  //!
  //! Regardless of the value chosen for mode, there is no limit to
  //! the number of vertices that can be defined between the glBegin
  //! and glEnd subroutines. Lines, triangles, quadrilaterals, and
  //! polygons that are incompletely specified are not
  //! drawn. Incomplete specification results when either too few
  //! vertices are provided to specify even a single primitive or when
  //! an incorrect multiple of vertices is specified. The incomplete
  //! primitive is ignored; the rest are drawn.
  //!
  //! The minimum specification of vertices for each primitive is as
  //! follows: 1 for a point, 2 for a line, 3 for a triangle, 4 for a
  //! quadrilateral, and 3 for a polygon. Modes that require a certain
  //! multiple of vertices are: GL_LINES (2), GL_TRIANGLES (3),
  //! GL_QUADS (4), and GL_QUAD_STRIP (2)
  //!
  //! Parameters
  //!
  //! \param mode Specifies the primitive or primitives that will be
  //!             created from vertices presented between glBegin and
  //!             the subsequent glEnd. Ten symbolic constants are
  //!             accepted: GL_POINTS, GL_LINES, GL_LINE_STRIP,
  //!             GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP,
  //!             GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, and
  //!             GL_POLYGON.
  //!
  //! Errors
  //!
  //! INVALID_ENUM Indicates that mode is set to an unaccepted value.
  //!
  //! GL_INVALID_OPERATION Indicates that a subroutine other than
  //!                      glVertex, glColor, glIndex, glNormal,
  //!                      glTexCoord, glEvalCoord, glEvalPoint,
  //!                      glMaterial, glEdgeFlag, glCallList, or
  //!                      glCallLists subroutine is called between
  //!                      glBegin and the corresponding glEnd.
  //!
  //! GL_INVALID_OPERATION Indicates that glEnd is called before the
  //!                      corresponding glBegin is called
  virtual void glBegin(GLenum mode);

  //! Delimits the vertices of a primitive or group of like primitives.
  //!
  //! Description
  //!
  //! The glBegin and glEnd subroutines delimit the vertices that
  //! define a primitive or group of like primitives.  The glBegin
  //! subroutine accepts a single argument that specifies which of 10
  //! ways the vertices will be interpreted. Taking n as an integer
  //! count starting at 1 (one), and N as the total number of vertices
  //! specified, the interpretations are:
  //!
  //! GL_POINTS          Treats each vertex as a single point. Vertex n defines
  //!                    point n. N points are drawn.
  //!
  //! GL_LINES           Treats each pair of vertices as an independent line
  //!                    segment. Vertices 2n-1 and 2n define line
  //!                    n. N/2 lines are drawn.
  //!
  //! GL_LINE_STRIP      Draws a connected group of line segments from the
  //!                    first vertex to the last. Vertices n and n+1
  //!                    define line n. N-1 lines are drawn.
  //!
  //! GL_LINE_LOOP       Draws a connected group of line segments from the
  //!                    first vertex to the last, then back to the
  //!                    first. Vertices n and n+1 define line n. The
  //!                    last line, however, is defined by vertices N
  //!                    and 1. N lines are drawn.
  //!
  //! GL_TRIANGLES       Treats each triplet of vertices as an independent
  //!                    triangle. Vertices 3n-2, 3n-1, and 3n define
  //!                    triangle n. N/3 triangles are drawn.
  //!
  //! GL_TRIANGLE_STRIP  Draws a connected group of triangles. One
  //!                    triangle is defined for each vertex presented
  //!                    after the first two vertices. For odd n,
  //!                    vertices n, n+1, and n+2 define triangle
  //!                    n. For even n, vertices n+1, n, and n+2
  //!                    define triangle n. N-2 triangles are drawn.
  //!
  //! GL_TRIANGLE_FAN    Draws a connected group of triangles. One
  //!                    triangle is defined for each vertex presented
  //!                    after the first two vertices. Vertices 1,
  //!                    n+1, and n+2 define triangle n. N-2 triangles
  //!                    are drawn.
  //!
  //! GL_QUADS           Treats each group of four vertices as an independent
  //!                    quadrilateral. Vertices 4n-3, 4n-2, 4n-1, and
  //!                    4n define quadrilateral n. N/4 quadrilaterals
  //!                    are drawn.
  //!
  //! GL_QUAD_STRIP      Draws a connected group of quadrilaterals. One
  //!                    quadrilateral is defined for each pair of
  //!                    vertices presented after the first
  //!                    pair. Vertices 2n-1, 2n, 2n+2, and 2n+1
  //!                    define quadrilateral n. N/2-1 quadrilaterals
  //!                    are drawn. Note that the order in which
  //!                    vertices are used to construct a
  //!                    quadrilateral from strip data is different
  //!                    from that used with independent data.
  //!
  //! GL_POLYGON         Draws a single, convex polygon. Vertices 1 through N
  //!                    define this polygon.
  //!
  //! Only a subset of GL subroutines can be used between the glBegin
  //! and glEnd subroutines. The subroutines are: glVertex, glColor,
  //! glIndex, glNormal, glTexCoord, glEvalCoord, glEvalPoint,
  //! glMaterial, and glEdgeFlag. Also, it is acceptable to use
  //! glCallList or glCallLists to execute display lists that include
  //! only the preceding subroutines. If any other GL subroutine is
  //! called between the glBegin and glEnd subroutines, the error flag
  //! is set and the subroutine is ignored.
  //!
  //! Regardless of the value chosen for mode, there is no limit to
  //! the number of vertices that can be defined between the glBegin
  //! and glEnd subroutines. Lines, triangles, quadrilaterals, and
  //! polygons that are incompletely specified are not
  //! drawn. Incomplete specification results when either too few
  //! vertices are provided to specify even a single primitive or when
  //! an incorrect multiple of vertices is specified. The incomplete
  //! primitive is ignored; the rest are drawn.
  //!
  //! The minimum specification of vertices for each primitive is as
  //! follows: 1 for a point, 2 for a line, 3 for a triangle, 4 for a
  //! quadrilateral, and 3 for a polygon. Modes that require a certain
  //! multiple of vertices are: GL_LINES (2), GL_TRIANGLES (3),
  //! GL_QUADS (4), and GL_QUAD_STRIP (2)
  //!
  //! Parameters
  //!
  //! \param mode Specifies the primitive or primitives that will be
  //!             created from vertices presented between glBegin and
  //!             the subsequent glEnd. Ten symbolic constants are
  //!             accepted: GL_POINTS, GL_LINES, GL_LINE_STRIP,
  //!             GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP,
  //!             GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, and
  //!             GL_POLYGON.
  //!
  //! Errors
  //!
  //! INVALID_ENUM Indicates that mode is set to an unaccepted value.
  //!
  //! GL_INVALID_OPERATION Indicates that a subroutine other than
  //!                      glVertex, glColor, glIndex, glNormal,
  //!                      glTexCoord, glEvalCoord, glEvalPoint,
  //!                      glMaterial, glEdgeFlag, glCallList, or
  //!                      glCallLists subroutine is called between
  //!                      glBegin and the corresponding glEnd.
  //!
  //! GL_INVALID_OPERATION Indicates that glEnd is called before the
  //!                      corresponding glBegin is called
  virtual void glEnd();

  //! Sets the current texture coordinates.
  //!
  //! Parameters
  //!
  //! \param s Specify S texture coordinate. Not all parameters are
  //!          present in all forms of the command.
  //! \param t Specify T texture coordinate. Not all parameters are
  //!          present in all forms of the command.
  //!
  //! Description
  //!
  //! The glTexCoord subroutine specifies texture coordinates in one,
  //! two, three, or four dimensions. The glTexCoord1 subroutine sets
  //! the current texture coordinates to (S,0,0,1); a call to
  //! glTexCoord2 sets them to (S,T,0,1). Similarly, glTexCoord3
  //! specifies the texture coordinates as (S,T,R,1), and glTexCoord4
  //! defines all four components explicitly as (S,T,R,Q).
  //!
  //! The current texture coordinates are part of the data that is
  //! associated with each vertex and with the current raster
  //! position. Initially, the values for S, T, R, and Q are (0, 0, 0,
  //! 1).
  //!
  //!  Notes
  //!
  //! The current texture coordinates can be updated at any time. In
  //! particular, the glTexCoord subroutine can be called between a
  //! call to glBegin and the corresponding call to glEnd.
  //!
  //! If the GL_ARB_multitexture extension is present, then there will
  //! be multiple texture units present. This call will only affect
  //! the current textrue coordinate on Texture Unit 0. Use
  //! glMultiTexCoord*ARB to affect texture coordinates on other
  //! Texture Units.
  virtual void glTexCoord2f(GLfloat s, GLfloat t);

  // Miscellaneous

  //! Specifies pixel arithmetic.
  //!
  //! Description
  //!
  //! In RGB mode, pixels can be drawn using a function that blends
  //! the incoming (source) red, green, blue, and alpha (RGBA) values
  //! with the RGBA values that are already in the frame buffer (the
  //! destination values). By default, blending is disabled. Use the
  //! glEnable and glDisable subroutines with argument GL_BLEND to
  //! enable and disable blending.
  //!
  //! When blending is enabled, glBlendFunc and glBlendEquationEXT
  //! determine the blending operation. SourceFactor and
  //! DestinationFactor specify the scaling rules used for scaling the
  //! source and destination color components, respectively. Each rule
  //! defines four scale factors, one each for red, green, blue, and
  //! alpha. The rules are described in the table below.
  //!
  //! In the table and in subsequent equations, source color
  //! components are referred to as:
  //! (Rs, Gs, Bs, As)
  //!
  //! Destination color components are referred to as:
  //! (Rd, Gd, Bd, Ad)
  //!
  //! Constant color components are referred to as:
  //! (Rc, Gc, Bc, Ac)
  //!
  //! They are understood to have integer values between 0 (zero) and:
  //! (kR, kG, kB, kA)
  //!
  //! where
  //!
  //! (kc = 2mc - 1)
  //! (m R, m G, m B, m A)
  //!
  //! represents the number of RGBA bit planes.
  //!
  //! Source scale factors are referred to as:
  //! (s R, s G, s B, s A)
  //!
  //! Destination scale factors are referred to as:
  //! (d R, d G, d B, d A)
  //!
  //! The scale factors:
  //! (fR, fG, fB, fA)
  //!
  //! represent either source or destination factors. All scale
  //! factors have the range [0,1].
  //!
  //! |-----------------------------+---------------------------|
  //! | Parameter                   | f_R f_G f_B f_A           |
  //! |-----------------------------+---------------------------|
  //! | GL_ZERO                     | 0 0 0 0                   |
  //! |                             |                           |
  //! | GL_ONE                      | 1 1 1 1                   |
  //! |                             |                           |
  //! | GL_SRC_COLOR                | R_s G_s B_s A_s           |
  //! |                             | --- --- --- ---           |
  //! |                             | k_R k_G k_B k_A           |
  //! |                             |                           |
  //! | GL_ONE_MINUS_SRC_COLOR      |           R_s G_s B_s A_s |
  //! |                             | 1 1 1 1 - --- --- --- --- |
  //! |                             |           k_R k_G k_B k_A |
  //! |                             |                           |
  //! | GL_DST_COLOR                | R_d G_d B_d A_d           |
  //! |                             | --- --- --- ---           |
  //! |                             | k_R k_G k_B k_A           |
  //! |                             |                           |
  //! | GL_ONE_MINUS_DST_COLOR      |           R_d G_d B_d A_d |
  //! |                             | 1 1 1 1 - --- --- --- --- |
  //! |                             |           k_R k_G k_B k_A |
  //! |                             |                           |
  //! | GL_SRC_ALPHA                | A_s A_s A_s A_s           |
  //! |                             | --- --- --- ---           |
  //! |                             | k_A k_A k_A k_A           |
  //! |                             |                           |
  //! | GL_ONE_MINUS_SRC_ALPHA      |           A_s A_s A_s A_s |
  //! |                             | 1 1 1 1 - --- --- --- --- |
  //! |                             |           k_A k_A k_A k_A |
  //! |                             |                           |
  //! | GL_DST_ALPHA                | A_d A_d A_d A_d           |
  //! |                             | --- --- --- ---           |
  //! |                             | k_A k_A k_A k_A           |
  //! |                             |                           |
  //! | GL_ONE_MINUS_DST_ALPHA      |           A_d A_d A_d A_d |
  //! |                             | 1 1 1 1 - --- --- --- --- |
  //! |                             |           k_A k_A k_A k_A |
  //! |                             |                           |
  //! | GL_CONSTANT_COLOR           | R_c G_c B_c A_c           |
  //! |                             |                           |
  //! | GL_ONE_MINUS_CONSTANT_COLOR | 1 1 1 1 - R_c G_c B_c A_c |
  //! |                             |                           |
  //! | GL_CONSTANT_ALPHA           | A_c A_c A_c A_c           |
  //! |                             |                           |
  //! | GL_ONE_MINUS_CONSTANT_ALPHA | 1 1 1 1 - A_c A_c A_c A_c |
  //! |                             |                           |
  //! | GL_SRC_ALPHA_SATURATE       | i i i 1                   |
  //! |-----------------------------+---------------------------|
  //!
  //! i = min (As, kA - Ad )/kA
  //!
  //! To determine the blended RGBA values of a pixel when drawing in
  //! RGB mode, the system uses the following equations:
  //!
  //! Rd = min (kR, RssR + RddR)
  //! Gd = min (kG, GssG + GddG)
  //! Bd = min (kB, BssB + BddB)
  //! Ad = min (kA, AssA + AddA)
  //!
  //! Blending combines corresponding source and destination color
  //! components according to the blending operation specified by
  //! GL_BLEND_EQUATION_EXT. The blending operations are:
  //!
  //! |------------------------------+-------------------------|
  //! | GL_BLEND_EQUATION_EXT        | Binary Operation        |
  //! |------------------------------+-------------------------|
  //! | GL_FUNC_ADD_EXT              | min(Cs x sC+Cd x dC,kC) |
  //! | GL_BLEND_EQUATION_EXT        | Binary Operation        |
  //! | GL_FUNC_SUBTRACT_EXT         | max(Cs x sC-Cd x dC,0)  |
  //! | GL_FUNC_REVERSE_SUBTRACT_EXT | max(Cd x dC-Cs x sC,0)  |
  //! | GL_LOGIC_OP                  | Cs Lop Cd               |
  //! | GL_MIN_EXT                   | min(Cs, Cd)             |
  //! | GL_MAX_EXT                   | max(Cs, Cd)             |
  //! |------------------------------+-------------------------|
  //!
  //! where C is the relevant color component (R, G, B, or A), Cs and
  //! Cd are the source and destination color components,
  //! respectively, sC and sD are the source and destination scale
  //! factors, respectively, and Lop is one of 16 bitwise operators
  //! specified by glLogicOp.
  //!
  //! Despite the apparent precision of the preceding equations,
  //! blending arithmetic is not exactly specified, because blending
  //! operates with imprecise integer color values. However, a blend
  //! factor that should be equal to 1 is guaranteed not to modify its
  //! multiplicand, and a blend factor equal to 0 reduces its
  //! multiplicand to 0. Thus, for example, when SourceFactor is
  //! GL_SRC_ALPHA, DestinationFactor is GL_ONE_MINUS_SRC_ALPHA, and
  //! As is equal to kA, the equations reduce to simple replacement:
  //!
  //! Rd = Rs
  //! Gd = Gs
  //! Rd = Bs
  //! Ad = As
  //!
  //! Parameters
  //!
  //! \param SourceFactor Specifies how the RGBA source-blending
  //!                          factors are computed. Thirteen symbolic
  //!                          constants are accepted: GL_ZERO,
  //!                          GL_ONE, GL_DST_COLOR,
  //!                          GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA,
  //!                          GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
  //!                          GL_ONE_MINUS_DST_ALPHA,
  //!                          GL_CONSTANT_COLOR,
  //!                          GL_CONSTANT_COLOR_EXT,
  //!                          GL_ONE_MINUS_CONSTANT_COLOR,
  //!                          GL_ONE_MINUS_CONSTANT_COLOR_EXT,
  //!                          GL_CONSTANT_ALPHA,
  //!                          GL_CONSTANT_ALPHA_EXT,
  //!                          GL_ONE_MINUS_CONSTANT_ALPHA,
  //!                          GL_ONE_MINUS_CONSTANT_ALPHA_EXT, and
  //!                          GL_SRC_ALPHA_SATURATE.
  //!
  //!                          These symbolic constants are defined in
  //!                          the Description section. The initial
  //!                          value is GL_ONE.
  //!
  //! \param DestinationFactor Specifies how the RGBA
  //!                          destination-blending factors are
  //!                          computed. Twelve symbolic constants are
  //!                          accepted: GL_ZERO, GL_ONE,
  //!                          GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR,
  //!                          GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
  //!                          GL_DST_ALPHA , GL_ONE_MINUS_DST_ALPHA,
  //!                          GL_CONSTANT_COLOR,
  //!                          GL_CONSTANT_COLOR_EXT,
  //!                          GL_ONE_MINUS_CONSTANT_COLOR,
  //!                          GL_ONE_MINUS_CONSTANT_COLOR_EXT,
  //!                          GL_CONSTANT_ALPHA,
  //!                          GL_CONSTANT_ALPHA_EXT,
  //!                          GL_ONE_MINUS_CONSTANT_ALPHA, and
  //!                          GL_ONE_MINUS_CONSTANT_ALPHA_EXT.
  //!
  //!                          These symbolic constants are defined in
  //!                          the Description section. The initial
  //!                          value is GL_ZERO.
  //!
  //! Notes
  //!
  //! Incoming (source) alpha is correctly thought of as a material
  //! opacity, ranging from 1.0 (KA), representing complete opacity,
  //! to 0.0 (0), representing complete transparency.
  //!
  //! When more than one color buffer is enabled for drawing, blending
  //! is done separately for each enabled buffer, using for
  //! destination color the contents of that buffer. (See the
  //! glDrawBuffer subroutine.)
  //!
  //! Blending affects only RGB rendering. It is ignored by color
  //! index renderers.
  //!
  //! The Source and destination factors GL_CONSTANT_COLOR,
  //! GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA,
  //! GL_ONE_MINUS_CONSTANT_ALPHA, and their _EXT versions are only
  //! valid if the ARB imaging subset is supported and/or the Blend
  //! Color extension.
  //!
  //! Errors
  //!
  //! GL_INVALID_ENUM      Either SourceFactor or DestinationFactor is set
  //!                      to an unaccepted value.
  //! GL_INVALID_OPERATION The glBlendFunc subroutine is called
  //!                      between a call to glBegin and the
  //!                      corresponding call to glEnd
  //!
  //! Examples
  //!
  //! Transparency is best implemented using a blend function
  //! (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) with primitives sorted
  //! from farthest to nearest. Note that this transparency
  //! calculation does not require the presence of alpha bit planes in
  //! the frame buffer.
  //!
  //! The blend function operation (GL_SRC_ALPHA,
  //! GL_ONE_MINUS_SRC_ALPHA) is also useful for rendering antialiased
  //! points and lines in arbitrary order.
  //!
  //! Polygon antialiasing is optimized using a blend function
  //! (GL_SRC_ALPHA_SATURATE, GL_ONE) with polygons sorted from
  //! nearest to farthest. (See the glEnable or glDisable subroutine
  //! and the GL_POLYGON_SMOOTH argument for information on polygon
  //! antialiasing.) Destination alpha bit planes, which must be
  //! present for this blend function to operate correctly, store the
  //! accumulated coverage.
  virtual void glBlendFunc(GLenum sfactor, GLenum dfactor);

  // Texture mapping

  //! Sets texture environment parameters.
  //!
  //! Parameters
  //!
  //! \param target  Specifies a texture environment. May be
  //!                GL_TEXTURE_ENV, GL_TEXTURE_FILTER_CONTROL or
  //!                GL_POINT_SPRITE.
  //!
  //! \param pname   Specifies the symbolic name of a single-valued
  //!                texture environment parameter. May be either
  //!                GL_TEXTURE_ENV_MODE, GL_TEXTURE_LOD_BIAS,
  //!                GL_COMBINE_RGB, GL_COMBINE_ALPHA, GL_SRC0_RGB,
  //!                GL_SRC1_RGB, GL_SRC2_RGB, GL_SRC0_ALPHA,
  //!                GL_SRC1_ALPHA, GL_SRC2_ALPHA, GL_OPERAND0_RGB,
  //!                GL_OPERAND1_RGB, GL_OPERAND2_RGB,
  //!                GL_OPERAND0_ALPHA, GL_OPERAND1_ALPHA,
  //!                GL_OPERAND2_ALPHA, GL_RGB_SCALE, GL_ALPHA_SCALE,
  //!                or GL_COORD_REPLACE.
  //!
  //! \param param   Specifies a single symbolic constant, one of
  //!                GL_ADD, GL_ADD_SIGNED, GL_INTERPOLATE,
  //!                GL_MODULATE, GL_DECAL, GL_BLEND, GL_REPLACE,
  //!                GL_SUBTRACT, GL_COMBINE, GL_TEXTURE, GL_CONSTANT,
  //!                GL_PRIMARY_COLOR, GL_PREVIOUS, GL_SRC_COLOR,
  //!                GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA,
  //!                GL_ONE_MINUS_SRC_ALPHA, a single boolean value
  //!                for the point sprite texture coordinate
  //!                replacement, a single floating-point value for
  //!                the texture level-of-detail bias, or 1.0, 2.0, or
  //!                4.0 when specifying the GL_RGB_SCALE or
  //!                GL_ALPHA_SCALE.
  //!
  //! Description
  //!
  //! A texture environment specifies how texture values are
  //! interpreted when a fragment is textured. When target is
  //! GL_TEXTURE_FILTER_CONTROL, pname must be
  //! GL_TEXTURE_LOD_BIAS. When target is GL_TEXTURE_ENV, pname can be
  //! GL_TEXTURE_ENV_MODE, GL_TEXTURE_ENV_COLOR, GL_COMBINE_RGB,
  //! GL_COMBINE_ALPHA, GL_RGB_SCALE, GL_ALPHA_SCALE, GL_SRC0_RGB,
  //! GL_SRC1_RGB, GL_SRC2_RGB, GL_SRC0_ALPHA, GL_SRC1_ALPHA, or
  //! GL_SRC2_ALPHA.
  //!
  //! If pname is GL_TEXTURE_ENV_MODE, then params is (or points to)
  //! the symbolic name of a texture function. Six texture functions
  //! may be specified: GL_ADD, GL_MODULATE, GL_DECAL, GL_BLEND,
  //! GL_REPLACE, or GL_COMBINE.
  //!
  //! The following table shows the correspondence of filtered texture
  //! values R t, G t, B t, A t, L t, I t to texture source
  //! components. C s and A s are used by the texture functions
  //! described below.
  //!
  //! |------------------------------+-------------------+-----|
  //! | Texture Base Internal Format | C s               | A s |
  //! |------------------------------+-------------------+-----|
  //! | GL_ALPHA                     | (0, 0, 0)         | A t |
  //! | GL_LUMINANCE                 | ( L t, L t, L t ) | 1   |
  //! | GL_LUMINANCE_ALPHA           | ( L t, L t, L t ) | A t |
  //! | GL_INTENSITY                 | ( I t, I t, I t ) | I t |
  //! | GL_RGB                       | ( R t, G t, B t ) | 1   |
  //! | GL_RGBA                      | ( R t, G t, B t ) | A t |
  //! |------------------------------+-------------------+-----|
  //!
  //! A texture function acts on the fragment to be textured using the
  //! texture image value that applies to the fragment (see
  //! glTexParameter) and produces an RGBA color for that
  //! fragment. The following table shows how the RGBA color is
  //! produced for each of the first five texture functions that can
  //! be chosen. C is a triple of color values (RGB) and A is the
  //! associated alpha value. RGBA values extracted from a texture
  //! image are in the range [0,1]. The subscript p refers to the
  //! color computed from the previous texture stage (or the incoming
  //! fragment if processing texture stage 0), the subscript s to the
  //! texture source color, the subscript c to the texture environment
  //! color, and the subscript v indicates a value produced by the
  //! texture function.
  //!
  //! |------------------------------+-------+---------------------+----------------------+---------------------------+---------------------------+-----------------|
  //! | Texture Base Internal Format | Value | GL_REPLACE Function | GL_MODULATE Function | GL_DECAL Function         | GL_BLEND Function         | GL_ADD Function |
  //! |------------------------------+-------+---------------------+----------------------+---------------------------+---------------------------+-----------------|
  //! | GL_ALPHA                     | C v = | C p                 | C p                  | undefined                 | C p                       | C p             |
  //! |                              | A v = | A s                 | A p ⁢ A s            |                           | A v = A p ⁢ A s           | A p ⁢ A s       |
  //! | GL_LUMINANCE                 | C v = | C s                 | C p ⁢ C s            | undefined                 | C p ⁢ 1 - C s + C c ⁢ C s | C p + C s       |
  //! | (or 1)                       | A v = | A p                 | A p                  |                           | A p                       | A p             |
  //! | GL_LUMINANCE_ALPHA           | C v = | C s                 | C p ⁢ C s            | undefined                 | C p ⁢ 1 - C s + C c ⁢ C s | C p + C s       |
  //! | (or 2)                       | A v = | A s                 | A p ⁢ A s            |                           | A p ⁢ A s                 | A p ⁢ A s       |
  //! | GL_INTENSITY                 | C v = | C s                 | C p ⁢ C s            | undefined                 | C p ⁢ 1 - C s + C c ⁢ C s | C p + C s       |
  //! |                              | A v = | A s                 | A p ⁢ A s            |                           | A p ⁢ 1 - A s + A c ⁢ A s | A p + A s       |
  //! | GL_RGB                       | C v = | C s                 | C p ⁢ C s            | C s                       | C p ⁢ 1 - C s + C c ⁢ C s | C p + C s       |
  //! | (or 3)                       | A v = | A p                 | A p                  | A p                       | A p                       | A p             |
  //! | GL_RGBA                      | C v = | C s                 | C p ⁢ C s            | C p ⁢ 1 - A s + C s ⁢ A s | C p ⁢ 1 - C s + C c ⁢ C s | C p + C s       |
  //! | (or 4)                       | A v = | A s                 | A p ⁢ A s            | A p                       | A p ⁢ A s                 | A p ⁢ A s       |
  //! |------------------------------+-------+---------------------+----------------------+---------------------------+---------------------------+-----------------|
  //!   //!
  //! If pname is GL_TEXTURE_ENV_MODE, and params is GL_COMBINE, the
  //! form of the texture function depends on the values of
  //! GL_COMBINE_RGB and GL_COMBINE_ALPHA.
  //!
  //! The following describes how the texture sources, as specified by
  //! GL_SRC0_RGB, GL_SRC1_RGB, GL_SRC2_RGB, GL_SRC0_ALPHA,
  //! GL_SRC1_ALPHA, and GL_SRC2_ALPHA, are combined to produce a
  //! final texture color. In the following tables, GL_SRC0_c is
  //! represented by Arg0, GL_SRC1_c is represented by Arg1, and
  //! GL_SRC2_c is represented by Arg2.
  //!
  //! GL_COMBINE_RGB accepts any of GL_REPLACE, GL_MODULATE, GL_ADD,
  //! GL_ADD_SIGNED, GL_INTERPOLATE, GL_SUBTRACT, GL_DOT3_RGB, or
  //! GL_DOT3_RGBA.
  //!
  //! |-----------------------------+---------------------------------------------------------------------------------------------|
  //! | GL_COMBINE_RGB              | Texture Function                                                                            |
  //! |-----------------------------+---------------------------------------------------------------------------------------------|
  //! | GL_REPLACE                  | Arg0                                                                                        |
  //! | GL_MODULATE                 | Arg0 × Arg1                                                                                 |
  //! | GL_ADD                      | Arg0 + Arg1                                                                                 |
  //! | GL_ADD_SIGNED               | Arg0 + Arg1 - 0.5                                                                           |
  //! | GL_INTERPOLATE              | Arg0 × Arg2 + Arg1 × 1 - Arg2                                                               |
  //! | GL_SUBTRACT                 | Arg0 - Arg1                                                                                 |
  //! | GL_DOT3_RGB or GL_DOT3_RGBA | 4 × Arg0 r - 0.5 × Arg1 r - 0.5 + Arg0 g - 0.5 × Arg1 g - 0.5 + Arg0 b - 0.5 × Arg1 b - 0.5 |
  //! |-----------------------------+---------------------------------------------------------------------------------------------|
  //!
  //! The scalar results for GL_DOT3_RGB and GL_DOT3_RGBA are placed
  //! into each of the 3 (RGB) or 4 (RGBA) components on output.
  //!
  //! Likewise, GL_COMBINE_ALPHA accepts any of GL_REPLACE,
  //! GL_MODULATE, GL_ADD, GL_ADD_SIGNED, GL_INTERPOLATE, or
  //! GL_SUBTRACT. The following table describes how alpha values are
  //! combined:
  //!
  //! |------------------+-------------------------------|
  //! | GL_COMBINE_ALPHA | Texture Function              |
  //! |------------------+-------------------------------|
  //! | GL_REPLACE       | Arg0                          |
  //! | GL_MODULATE      | Arg0 × Arg1                   |
  //! | GL_ADD           | Arg0 + Arg1                   |
  //! | GL_ADD_SIGNED    | Arg0 + Arg1 - 0.5             |
  //! | GL_INTERPOLATE   | Arg0 × Arg2 + Arg1 × 1 - Arg2 |
  //! | GL_SUBTRACT      | Arg0 - Arg1                   |
  //! |------------------+-------------------------------|
  //!
  //! In the following tables, the value C s represents the color
  //! sampled from the currently bound texture, C c represents the
  //! constant texture-environment color, C f represents the primary
  //! color of the incoming fragment, and C p represents the color
  //! computed from the previous texture stage or C f if processing
  //! texture stage 0. Likewise, A s , A c , A f , and A p represent
  //! the respective alpha values.
  //!
  //! The following table describes the values assigned to Arg0, Arg1,
  //! and Arg2 based upon the RGB sources and operands:
  //!
  //! |------------------+------------------------+----------------|
  //! | GL_SRCn_RGB      | GL_OPERANDn_RGB        | Argument Value |
  //! |------------------+------------------------+----------------|
  //! | GL_TEXTURE       | GL_SRC_COLOR           | C_s            |
  //! |                  | GL_ONE_MINUS_SRC_COLOR | 1 - C_s        |
  //! |                  | GL_SRC_ALPHA           | A_s            |
  //! |                  | GL_ONE_MINUS_SRC_ALPHA | 1 - A_s        |
  //! | GL_TEXTUREn      | GL_SRC_COLOR           | C_s            |
  //! |                  | GL_ONE_MINUS_SRC_COLOR | 1 - C_s        |
  //! |                  | GL_SRC_ALPHA           | A_s            |
  //! |                  | GL_ONE_MINUS_SRC_ALPHA | 1 - A_s        |
  //! | GL_CONSTANT      | GL_SRC_COLOR           | C_c            |
  //! |                  | GL_ONE_MINUS_SRC_COLOR | 1 - C_c        |
  //! |                  | GL_SRC_ALPHA           | A_c            |
  //! |                  | GL_ONE_MINUS_SRC_ALPHA | 1 - A_c        |
  //! | GL_PRIMARY_COLOR | GL_SRC_COLOR           | C_f            |
  //! |                  | GL_ONE_MINUS_SRC_COLOR | 1 - C_f        |
  //! |                  | GL_SRC_ALPHA           | A_f            |
  //! |                  | GL_ONE_MINUS_SRC_ALPHA | 1 - A_f        |
  //! | GL_PREVIOUS      | GL_SRC_COLOR           | C_p            |
  //! |                  | GL_ONE_MINUS_SRC_COLOR | 1 - C_p        |
  //! |                  | GL_SRC_ALPHA           | A_p            |
  //! |                  | GL_ONE_MINUS_SRC_ALPHA | 1 - A_p        |
  //! |------------------+------------------------+----------------|
  //!
  //! For GL_TEXTUREn sources, C s and A s represent the color and
  //! alpha, respectively, produced from texture stage n.
  //!
  //! The follow table describes the values assigned to Arg0, Arg1,
  //! and Arg2 based upon the alpha sources and operands:
  //!
  //! |------------------+------------------------+----------------|
  //! | GL_SRCn_ALPHA    | GL_OPERANDn_ALPHA      | Argument Value |
  //! |------------------+------------------------+----------------|
  //! | GL_TEXTURE       | GL_SRC_ALPHA           | A_s            |
  //! |                  | GL_ONE_MINUS_SRC_ALPHA | 1 - A_s        |
  //! | GL_TEXTUREn      | GL_SRC_ALPHA           | A_s            |
  //! |                  | GL_ONE_MINUS_SRC_ALPHA | 1 - A_s        |
  //! | GL_CONSTANT      | GL_SRC_ALPHA           | A_c            |
  //! |                  | GL_ONE_MINUS_SRC_ALPHA | 1 - A_c        |
  //! | GL_PRIMARY_COLOR | GL_SRC_ALPHA           | A_f            |
  //! |                  | GL_ONE_MINUS_SRC_ALPHA | 1 - A_f        |
  //! | GL_PREVIOUS      | GL_SRC_ALPHA           | A_p            |
  //! |                  | GL_ONE_MINUS_SRC_ALPHA | 1 - A_p        |
  //! |------------------+------------------------+----------------|
  //!
  //! The RGB and alpha results of the texture function are multiplied
  //! by the values of GL_RGB_SCALE and GL_ALPHA_SCALE, respectively,
  //! and clamped to the range 0 1 .
  //!
  //! If pname is GL_TEXTURE_ENV_COLOR, params is a pointer to an
  //! array that holds an RGBA color consisting of four
  //! values. Integer color components are interpreted linearly such
  //! that the most positive integer maps to 1.0, and the most
  //! negative integer maps to -1.0. The values are clamped to the
  //! range [0,1] when they are specified. C c takes these four
  //! values.
  //!
  //! If pname is GL_TEXTURE_LOD_BIAS, the value specified is added to
  //! the texture level-of-detail parameter, that selects which
  //! mipmap, or mipmaps depending upon the selected
  //! GL_TEXTURE_MIN_FILTER, will be sampled.
  //!
  //! GL_TEXTURE_ENV_MODE defaults to GL_MODULATE and
  //! GL_TEXTURE_ENV_COLOR defaults to (0, 0, 0, 0).
  //!
  //! If target is GL_POINT_SPRITE and pname is GL_COORD_REPLACE, the
  //! boolean value specified is used to either enable or disable
  //! point sprite texture coordinate replacement. The default value
  //! is GL_FALSE.
  //!
  //! Notes
  //!
  //! GL_REPLACE may only be used if the GL version is 1.1 or greater.
  //!
  //! GL_TEXTURE_FILTER_CONTROL and GL_TEXTURE_LOD_BIAS may only be
  //! used if the GL version is 1.4 or greater.
  //!
  //! GL_COMBINE mode and its associated constants may only be used if
  //! the GL version is 1.3 or greater.
  //!
  //! GL_TEXTUREn may only be used if the GL version is 1.4 or
  //! greater.
  //!
  //! Internal formats other than 1, 2, 3, or 4 may only be used if
  //! the GL version is 1.1 or greater.
  //!
  //! For OpenGL versions 1.3 and greater, or when the
  //! ARB_multitexture extension is supported, glTexEnv controls the
  //! texture environment for the current active texture unit,
  //! selected by glActiveTexture.
  //!
  //! GL_POINT_SPRITE and GL_COORD_REPLACE are available only if the
  //! GL version is 2.0 or greater.
  //!
  //! Errors
  //!
  //! GL_INVALID_ENUM is generated when target or pname is not one of
  //! the accepted defined values, or when params should have a
  //! defined constant value (based on the value of pname) and does
  //! not.
  //!
  //! GL_INVALID_VALUE is generated if the params value for
  //! GL_RGB_SCALE or GL_ALPHA_SCALE are not one of 1.0, 2.0, or 4.0.
  //!
  //! GL_INVALID_OPERATION is generated if glTexEnv is executed
  //! between the execution of glBegin and the corresponding execution
  //! of glEnd.
  virtual void glTexEnvf(GLenum target, GLenum pname, GLfloat param);

  virtual void glTexParameteri(GLenum target, GLenum pname, GLint param);

  virtual void glTexImage2D(GLenum target, GLint level, GLint internalFormat,
                            GLsizei width, GLsizei height, GLint border,
                            GLenum format, GLenum type, const GLvoid *pixels);

  // 1.1 functions

  virtual void glGenTextures(GLsizei n, GLuint *textures);

  //! Binds a named texture to a texturing target.
  //!
  //! Description
  //!
  //! The glBindTexture subroutine lets you create or use a named
  //! texture. Calling glBindTexture with target set to GL_TEXTURE_1D,
  //! GL_TEXTURE_2D, GL_TEXTURE_3D, or GL_TEXTURE_3D_EXT and texture
  //! set to the name of the new texture binds the texture name to the
  //! target. When a texture is bound to a target, the previous
  //! binding for that target is automatically broken.
  //!
  //! Texture names are unsigned integers. The value zero is reserved
  //! to represent the default texture for each texture
  //! target. Texture names and the corresponding texture contents are
  //! local to the shared display-list space (see glXCreateContext) of
  //! the current GL rendering context; two rendering contexts share
  //! texture names only if they also share display lists.
  //!
  //! You can use glGenTextures to generate a set of new texture
  //! names.
  //!
  //! When a texture is first bound, it assumes the dimensionality of
  //! its target: A texture first bound to GL_TEXTURE_1D becomes
  //! one-dimensional (1D), a texture first bound to GL_TEXTURE_2D
  //! becomes two-dimensional (2D), a texture first bound to
  //! GL_TEXTURE_3D becomes three-dimensional (3D), a texture first
  //! bound to GL_TEXTURE_3D_EXT becomes three-dimensional (3D). The
  //! state of a (1D) texture immediately after it is first bound is
  //! equivalent to the state of the default GL_TEXTURE_1D at GL
  //! initialization, and similarly for 2D and 3D textures.
  //!
  //! While a texture is bound, GL operations on the target to which
  //! it is bound affect the bound texture, and queries of the target
  //! to which it is bound return state from the bound texture. If
  //! texture mapping of the dimensionality of the target to which a
  //! texture is bound is active, the bound texture is used. In
  //! effect, the texture targets become aliases for the textures
  //! currently bound to them, and the texture name zero refers to the
  //! default textures that were bound to them at initialization.
  //!
  //! A texture binding created with glBindTexture remains active
  //! until a different texture is bound to the same target, or until
  //! the bound texture is deleted with glDeleteTextures.
  //!
  //! Once created, a named texture may be rebound to the target of
  //! the matching dimensionality as often as needed. It is usually
  //! much faster to use glBindTexture to bind an existing named
  //! texture to one of the texture targets than it is to reload the
  //! texture image using glTexImage1D or glTexImage2D. For additional
  //! control over performance, use glPrioritizeTextures.
  //!
  //! The glBindTexture subroutine is included in display lists.
  //!
  //! Parameters
  //!
  //! \param target        Specifies the target to which the texture is
  //!                      bound. Must be either GL_TEXTURE_1D,
  //!                      GL_TEXTURE_2D, GL_TEXTURE_3D, or
  //!                      GL_TEXTURE_3D_EXT (EXT_texture3D).
  //!
  //! \param texture       Specifies the name of a texture.
  //!
  //! Errors
  //!
  //! GL_INVALID_ENUM      is generated if target is not one of the
  //!                      allowable values.
  //!
  //! GL_INVALID_OPERATION is generated if texture has a
  //!                      dimensionality which doesn't match that of
  //!                      target.
  //!
  //! GL_INVALID_OPERATION is generated if glBindTexture is executed
  //!                      between the execution of glBegin and the
  //!                      corresponding execution of glEnd
  virtual void glBindTexture(GLenum target, GLuint texture);

  // Transformation

  //! Pushes the current matrix stack.
  //!
  //! Description
  //!
  //! There is a stack of matrices for each of the matrix modes. In
  //! GL_MODELVIEW mode, the stack depth is at least 32. In the other
  //! two modes, GL_PROJECTION and GL_TEXTURE, the depth is at least
  //! 2. The current matrix in any mode is the matrix on the top of
  //! the stack for that mode.
  //!
  //! The glPushMatrix subroutine pushes the current matrix stack down
  //! by one, duplicating the current matrix. That is, after a
  //! glPushMatrix call, the matrix on the top of the stack is
  //! identical to the one below it.
  //!
  //! The glPopMatrix subroutine pops the current matrix stack,
  //! replacing the current matrix with the one below it on the stack.
  //!
  //! Initially, each of the stacks contains one matrix, an identity
  //! matrix.
  //!
  //! It is an error to push a full matrix stack, or to pop a matrix
  //! stack that contains only a single matrix. In either case, the
  //! error flag is set, and no other change is made to GL state.
  //!
  //! Error Codes
  //!
  //! GL_STACK_OVERFLOW    The glPushMatrix subroutine is called while
  //!                      the current matrix stack is full.
  //!
  //! GL_STACK_UNDERFLOW   The glPopMatrix subroutine is called while
  //!                      the current matrix stack contains only a
  //!                      single matrix.
  //!
  //! GL_INVALID_OPERATION The glPushMatrix subroutine is called
  //!                      between a call to glBegin and the
  //!                      corresponding call to glEnd.
  virtual void glPushMatrix();

  //! Pops the current matrix stack.
  //!
  //! Description
  //!
  //! There is a stack of matrices for each of the matrix modes. In
  //! GL_MODELVIEW mode, the stack depth is at least 32. In the other
  //! two modes, GL_PROJECTION and GL_TEXTURE, the depth is at least
  //! 2. The current matrix in any mode is the matrix on the top of
  //! the stack for that mode.
  //!
  //! The glPushMatrix subroutine pushes the current matrix stack down
  //! by one, duplicating the current matrix. That is, after a
  //! glPushMatrix call, the matrix on the top of the stack is
  //! identical to the one below it.
  //!
  //! The glPopMatrix subroutine pops the current matrix stack,
  //! replacing the current matrix with the one below it on the stack.
  //!
  //! Initially, each of the stacks contains one matrix, an identity
  //! matrix.
  //!
  //! It is an error to push a full matrix stack, or to pop a matrix
  //! stack that contains only a single matrix. In either case, the
  //! error flag is set, and no other change is made to GL state.
  //!
  //! Error Codes
  //!
  //! GL_STACK_OVERFLOW    The glPushMatrix subroutine is called while
  //!                      the current matrix stack is full.
  //!
  //! GL_STACK_UNDERFLOW   The glPopMatrix subroutine is called while
  //!                      the current matrix stack contains only a
  //!                      single matrix.
  //!
  //! GL_INVALID_OPERATION The glPushMatrix subroutine is called
  //!                      between a call to glBegin and the
  //!                      corresponding call to glEnd.
  virtual void glPopMatrix();

  //! Sets the viewport.
  //!
  //! parameters:
  //!
  //! X, Y Specify the lower left corner of the viewport rectangle in pixels.
  //! The default is (0,0).
  //!
  //! Width, Height Specify the width and height, respectively, of the viewport.
  //! When a GL context is first attached to a window, Width and Height are set
  //! to the dimensions of that window.
  //!
  //! Description:
  //!
  //! The glViewport subroutine specifies the affine transformation of
  //! X and Y from normalized device coordinates to window
  //! coordinates. Let (Xnd, Ynd) be normalized device
  //! coordinates. Then the window coordinates (Xw, Yw) are computed
  //! as follows:
  //!
  //! Viewport width and height are silently clamped to a range that
  //! depends on the implementation. This range is queried by calling
  //! the glGet subroutine with the GL_MAX_VIEWPORT_DIMS argument.
  //!
  //! errors:
  //!
  //! GL_INVALID_VALUE Width or Height is negative.
  //!
  //! GL_INVALID_OPERATION The glViewport subroutine is called between
  //! a call to glBegin and the corresponding call to glEnd.
  //!
  virtual void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

private:
  // The OpenGL context this program uses
  std::shared_ptr<GL_Context> gl_context = nullptr;
};

} // namespace sdl_opengl_cpp

#endif
