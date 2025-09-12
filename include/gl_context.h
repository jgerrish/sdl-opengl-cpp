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
  //! capability Specifies a symbolic constant indicating a GL
  //! capability. Initially, all are disabled except GL_DITHER.
  //!
  //! Errors:
  //!
  //! GL_INVALID_ENUM capability is not an accepted value.
  //! GL_INVALID_OPERATION The glEnable subroutine is called between a
  //! call to glBegin and the corresponding call to glEnd.
  virtual void glEnable(GLenum cap);

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
