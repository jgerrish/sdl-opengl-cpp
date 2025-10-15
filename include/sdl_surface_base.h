#ifndef _SDL_TTF_OPENGL_SDL_SURFACE_BASE_H_
#define _SDL_TTF_OPENGL_SDL_SURFACE_BASE_H_

#include <string>

#ifdef NO_EXCEPTIONS
#include "errors.h"
#include <optional>
#else
#include "move_checker.h"
#include "spdlog/spdlog.h"
#include <stdexcept>
#endif

#include "SDL.h"

#define HAVE_OPENGL

#ifdef HAVE_OPENGL

#include "SDL_opengl.h"

#include "gl_context.h"
#include "sdl_base.h"

using namespace std;

namespace sdl_opengl_cpp {

//! Forward declare the tester friend.  If we explicitly include the
//! namespace down below in the friend statement, compilation fails
//! with: "'SDLSurfaceTester in namespace 'sdl_opengl_cpp' does not name
//! a type"
//!
//! This fixes that
class SDLSurfaceTester;

// nested namespaces added in C++17
namespace sdl_surface {

#ifndef NO_EXCEPTIONS

//! A CreationError exception
//!
//! An error occurred trying to create a surface
//!
class CreationError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

//! A LoadTextureError exception
//!
//! An error occurred trying to load a texture
//!
class LoadTextureError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

//! An UnspecifiedStateError exception
//!
//! This exception is thrown when the SDLSurface is in an
//! valid but unspecified state after a move operation.
//!
class UnspecifiedStateError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

#endif

} // namespace sdl_surface

#ifndef NO_EXCEPTIONS
class SDLSurface : private MoveChecker {
#else
class SDLSurface : public Errors {
#endif
  // This is one way of allowing us to test private member variables
  // and state.
  friend class sdl_opengl_cpp::SDLSurfaceTester;

public:
  //! Create a new SDL Surface from a low-level SDL_Surface
  //!
  //! \param sdl_ The SDL object to use for operations
  //! \param s The raw SDL_Surface object to create an SDLSurface
  //!          from.
  //!
  //! \returns a new SDLSurface object
  SDLSurface(const std::shared_ptr<SDL> &sdl_, SDL_Surface *s);

  //! Create a new SDL Surface with the given width, size, depth and
  //! format.
  //!
  //! \param sdl_ The SDL object to use for operations
  //! \param flags the flags are unused and should be set to 0.
  //! \param width the width of the surface.
  //! \param height the height of the surface.
  //! \param depth the depth of the surface in bits.
  //! \param format the SDL_PixelFormatEnum for the new surface's
  //!               pixel format.
  //! TODO: Create a custom enum class for this
  //!
  //! \throws sdl_surface::CreationError if the surface could not be
  //!                                    created
  //!
  //! \returns a new SDLSurface object
  //!
  //! Documentation copied and modified from SDL_surface.h branch
  //! release-2.32.x on 2025-10-08
  SDLSurface(const std::shared_ptr<SDL> &sdl_, Uint32 flags, int width,
             int height, int depth, Uint32 format);

  ~SDLSurface();

  //! Cleanup the surface
  void cleanup() noexcept;

  // Explicitly delete the generated default copy constructor
  SDLSurface(const SDLSurface &) = delete;

  // Explicitly delete the generated default copy assignment
  // operator
  SDLSurface &operator=(const SDLSurface &) = delete;

  //! move constructor
  SDLSurface(SDLSurface &&) noexcept;

  //! move assignment operator
  SDLSurface &operator=(SDLSurface &&) noexcept;

  //! Returns the width of the surface
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns positive width on success or a negative error code on
  //!          failure; call get_last_error() for more information.
  int w();

  //! Returns the height of the surface
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns positive height on success or a negative error code on
  //!          failure; call get_last_error() for more information.
  int h();

  //! Create an OpenGL texture from the pixel data on this surface
  //!
  //! \param gl_context The OpenGL context to use for operations
  //! \param texcoord The texture coordinates that were written
  //!        to. The minimum X is 0, the minimum Y is 0, the maximum X
  //!        is the surface width and the maximum y is the surface
  //!        height.
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //! \throws an SDLSurfaceLoadTextureError if there is an issue
  //!         loading the texture.
  //!
  //! \returns The texture as an OpenGL handle
  //! TODO: Manage OpenGL textures as C++ classes
  GLuint GL_LoadTexture(const std::shared_ptr<GLContext> &gl_context,
                        GLfloat *texcoord);

  //! Blit onto this surface from another surface
  //!
  //! \param src The source surface
  //! \param srcrect The source rectangle to blit from
  //! \param dstrect The source rectangle to blit onto
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns 0 if the blit is successful, otherwise it returns -1
  int BlitSurfaceFrom(const SDLSurface &src, const SDL_Rect *srcrect,
                      SDL_Rect *dstrect);

  //! Blit from this surface onto another surface
  //!
  //! \param srcrect The source rectangle to blit from
  //! \param dst The destination surface
  //! \param dstrect The source rectangle to blit onto
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns 0 if the blit is successful, otherwise it returns -1
  int BlitSurfaceTo(const SDL_Rect *srcrect, SDLSurface &dst,
                    SDL_Rect *dstrect);

  //! Save a surface to a file in BMP format.
  //!
  //! Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
  //! BMP directly. Other RGB formats with 8-bit or higher get converted to a
  //! 24-bit surface or, if they have an alpha mask or a colorkey, to a 32-bit
  //! surface before they are saved. YUV and paletted 1-bit and 4-bit formats
  //! are not supported.
  //!
  //! \param filename the filename to save to
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns 0 on success or a negative error code on failure; call
  //!          get_last_error() for more information.
  //!
  //! Documentation copied and modified from SDL_surface.h branch
  //! release-2.32.x on 2025-10-08
  int SaveBMP(const std::string &filename);

  //! Set an additional color value multiplied into blit operations
  //!
  //! When this surface is blitted, during the blit operation each source color
  //! channel is modulated by the appropriate color value according to the
  //! following formula:
  //!
  //! `srcC = srcC * (color / 255)`
  //!
  //! \param r the red color value multiplied into blit operations.
  //! \param g the green color value multiplied into blit operations.
  //! \param b the blue color value multiplied into blit operations.
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied from SDL_surface.h branch release-2.32.x on
  //! 2025-10-08
  int SetColorMod(const Uint8 r, const Uint8 g, const Uint8 b);

  //! Get the additional color value multiplied into blit operations.
  //!
  //! \param r a pointer filled in with the current red color value.
  //! \param g a pointer filled in with the current green color value.
  //! \param b a pointer filled in with the current blue color value.
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied and modified from SDL_surface.h branch
  //! release-2.32.x on 2025-10-08
  int GetColorMod(Uint8 *r, Uint8 *g, Uint8 *b);

  //! Set an additional alpha value used in blit operations.
  //!
  //! When this surface is blitted, during the blit operation the source alpha
  //! value is modulated by this alpha value according to the following formula:
  //!
  //! `srcA = srcA * (alpha / 255)`
  //!
  //! \param alpha the alpha value multiplied into blit operations.
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied and modified from SDL_surface.h branch
  //! release-2.32.x on 2025-10-08
  int SetAlphaMod(const Uint8 alpha);

  //! Get the additional alpha value used in blit operations.
  //!
  //! \param alpha a pointer filled in with the current alpha value.
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied and modified from SDL_surface.h branch
  //! release-2.32.x on 2025-10-08
  int GetAlphaMod(Uint8 *alpha);

  //! Set the blend mode used for blit operations.
  //!
  //! To copy a surface to another surface (or texture) without blending with
  //! the existing data, the blendmode of the SOURCE surface should be set to
  //! `SDL_BLENDMODE_NONE`.
  //!
  //! \param blendMode the SDL_BlendMode to use for blit blending.
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied and modified from SDL_surface.h branch
  //! release-2.32.x on 2025-10-08
  int SetBlendMode(const SDL_BlendMode blendMode);

  //! Get the blend mode used for blit operations.
  //!
  //! \param blendMode a pointer filled in with the current SDL_BlendMode.
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied and modified from SDL_surface.h branch
  //! release-2.32.x on 2025-10-08
  int GetBlendMode(SDL_BlendMode *blendMode);

  //! Return the pixel data for this surface
  //!
  //! \throws an UnspecifiedStateError if the surface is in an
  //!         unspecified state.
  //!
  //! \returns the pixel data on success or nullptr on failure; call
  //!          get_last_error() for more information.
  void *pixels();

  //! True if the the object is in an unspecified state
  bool is_in_unspecified_state() const override;

private:
  //! The underlying SDL object to use for functions on this surface
  std::shared_ptr<SDL> sdl = nullptr;

  SDL_Surface *surface;

  constexpr int power_of_two(const int input) const;
};

} // namespace sdl_opengl_cpp

#endif

#endif
