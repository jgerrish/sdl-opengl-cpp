#include "sdl_surface_base.h"

using namespace sdl_opengl_cpp;

SDLSurface::SDLSurface(const std::shared_ptr<SDL> &sdl_, SDL_Surface *s)
    : sdl{sdl_}, surface{s} {}

SDLSurface::SDLSurface(const std::shared_ptr<SDL> &sdl_, Uint32 flags,
                       int width, int height, int depth, Uint32 format)
    : sdl{sdl_} {
  surface =
      sdl->CreateRGBSurfaceWithFormat(flags, width, height, depth, format);

  if (surface == nullptr) {
#ifndef NO_EXCEPTIONS
    spdlog::error("ERROR::SDL_SURFACE::CREATE_SDL_SURFACE_FAILED");
    throw sdl_surface::CreationError(
        "ERROR::SDL_SURFACE::CREATE_SDL_SURFACE_FAILED");
#else
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::SDLSurfaceCreationError));
    cleanup();
    return;
#endif
  }
}

SDLSurface::~SDLSurface() { cleanup(); }

void SDLSurface::cleanup() noexcept {
  if (surface != nullptr) {
    sdl->FreeSurface(surface);
  }
}

// move constructor
SDLSurface::SDLSurface(SDLSurface &&s) noexcept
    : sdl{s.sdl}, surface{s.surface} {

#ifdef NO_EXCEPTIONS
  last_operation_failed = s.last_operation_failed;
  last_error = s.last_error;
#endif

  s.surface = nullptr;
  s.sdl = nullptr;
}

// move assignment operator
SDLSurface &SDLSurface::operator=(SDLSurface &&s) noexcept {
  if (&s != this) {
    surface = s.surface;
    sdl = s.sdl;

#ifdef NO_EXCEPTIONS
    last_operation_failed = s.last_operation_failed;
    last_error = s.last_error;
#endif

    s.surface = nullptr;
    s.sdl = nullptr;
  }

  return *this;
}

int SDLSurface::w() {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  if (surface != nullptr) {
    return surface->w;
  }

  return -1;
}

int SDLSurface::h() {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  if (surface != nullptr) {
    return surface->h;
  }

  return -1;
}

GLuint SDLSurface::GL_LoadTexture(const std::shared_ptr<GLContext> &gl_context,
                                  GLfloat *texcoord) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  GLuint texture;
  int w, h;
  SDL_Rect area;
  Uint8 saved_alpha;
  SDL_BlendMode saved_mode;

  /* Use the surface width and height expanded to powers of 2 */
  w = power_of_two(surface->w);
  h = power_of_two(surface->h);
  texcoord[0] = 0.0f;                    /* Min X */
  texcoord[1] = 0.0f;                    /* Min Y */
  texcoord[2] = (GLfloat)surface->w / w; /* Max X */
  texcoord[3] = (GLfloat)surface->h / h; /* Max Y */

#ifndef NO_EXCEPTIONS
  try {
#endif

    SDLSurface image(sdl, 0, w, h, 0, SDL_PIXELFORMAT_RGBA32);

#ifdef NO_EXCEPTIONS
    if (!image.valid()) {
      set_error(std::optional<error>(
          sdl_opengl_cpp::error::SDLSurfaceLoadTextureError));
      cleanup();
      return -1;
    }
#endif

    /* Save the alpha blending attributes */
    GetAlphaMod(&saved_alpha);
    SetAlphaMod(0xFF);
    GetBlendMode(&saved_mode);
    SetBlendMode(SDL_BLENDMODE_NONE);

    /* Copy the surface into the GL texture image */
    area.x = 0;
    area.y = 0;
    area.w = surface->w;
    area.h = surface->h;
    BlitSurfaceTo(&area, image, &area);

    /* Restore the alpha blending attributes */
    SetAlphaMod(saved_alpha);
    SetBlendMode(saved_mode);

    /* Create an OpenGL texture for the image */
    gl_context->glGenTextures(1, &texture);
    gl_context->glBindTexture(GL_TEXTURE_2D, texture);
    gl_context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                GL_NEAREST);
    gl_context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_NEAREST);
    // TODO: Figure out / design and implement visibility issues
    gl_context->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                             GL_UNSIGNED_BYTE, image.pixels());

#ifndef NO_EXCEPTIONS
  } catch (sdl_surface::CreationError &e) {
    throw sdl_surface::LoadTextureError("Error trying to load texture");
  }
#endif
  return texture;
}

int SDLSurface::BlitSurfaceFrom(const SDLSurface &src, const SDL_Rect *srcrect,
                                SDL_Rect *dstrect) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  return sdl->BlitSurface(src.surface, srcrect, surface, dstrect);
}

int SDLSurface::BlitSurfaceTo(const SDL_Rect *srcrect, SDLSurface &dst,
                              SDL_Rect *dstrect) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  return sdl->BlitSurface(surface, srcrect, dst.surface, dstrect);
}

int SDLSurface::SaveBMP(const std::string &filename) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  return sdl->SaveBMP(surface, filename);
}

int SDLSurface::SetColorMod(const Uint8 r, const Uint8 g, const Uint8 b) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  return sdl->SetSurfaceColorMod(surface, r, g, b);
}

int SDLSurface::GetColorMod(Uint8 *r, Uint8 *g, Uint8 *b) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  return sdl->GetSurfaceColorMod(surface, r, g, b);
}

int SDLSurface::SetAlphaMod(const Uint8 alpha) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  return sdl->SetSurfaceAlphaMod(surface, alpha);
}

int SDLSurface::GetAlphaMod(Uint8 *alpha) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  return sdl->GetSurfaceAlphaMod(surface, alpha);
}

int SDLSurface::SetBlendMode(const SDL_BlendMode blendMode) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  return sdl->SetSurfaceBlendMode(surface, blendMode);
}

int SDLSurface::GetBlendMode(SDL_BlendMode *blendMode) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return -1;
#endif
  }

  return sdl->GetSurfaceBlendMode(surface, blendMode);
}

// Quick utility function for texture creation
constexpr int SDLSurface::power_of_two(const int input) const {
  int value = 1;

  while (value < input) {
    value <<= 1;
  }
  return value;
}

void *SDLSurface::pixels() {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl_surface::UnspecifiedStateError(
        "SDLSurface is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    return nullptr;
#endif
  }

  return surface->pixels;
}

bool SDLSurface::is_in_unspecified_state() const {
  if ((surface == nullptr) || (sdl == nullptr))
    return true;
  else
    return false;
}
