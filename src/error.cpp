#include "error.h"

#ifdef NO_EXCEPTIONS

using namespace sdl_opengl_cpp;

std::ostream &sdl_opengl_cpp::operator<<(std::ostream &os, const error &e) {
  os << error_as_string(e);

  return os;
}

std::string sdl_opengl_cpp::error_as_string(const error &e) {
  std::string error_string;

  switch (e) {
  case error::UnspecifiedStateError:
    error_string = "UnspecifiedStateError";
    break;

  case error::BufferDataError:
    error_string = "BufferDataError";
    break;
  case error::GenBuffersError:
    error_string = "GenBuffersError";
    break;
  case error::InvalidOperationError:
    error_string = "InvalidOperationError";
    break;

  case error::GenVertexArraysError:
    error_string = "GenVertexArraysError";
    break;

  case error::ShaderCreationError:
    error_string = "ShaderCreationError";
    break;
  case error::ShaderCompilationError:
    error_string = "ShaderCompilationError";
    break;

  case error::ProgramCreationError:
    error_string = "ProgramCreationError";
    break;
  case error::ProgramLinkingError:
    error_string = "ProgramLinkingError";
    break;
  case error::GetUniformLocationError:
    error_string = "GetUniformLocationError";
    break;

  default:
    error_string = "Unknown error type";
    break;
  }

  return error_string;
}

// The below spdlog formatters aren't working with the error type currently
// spdlog/fmt/bundled/base.h:2235:45: error:
// ‘fmt::v11::detail::type_is_unformattable_for<sdl_opengl_cpp::vertex_buffer_object::my_type,
// char> _’ has incomplete type

// #ifndef SPDLOG_USE_STD_FORMAT  // when using fmtlib
// template <>
// struct fmt::formatter<my_type> : fmt::formatter<std::string> {
//     auto format(my_type my, format_context &ctx) const -> decltype(ctx.out())
//     {
//       return fmt::format_to(ctx.out(), "[error i={}]",
//       error_as_string(my.err));
//     }
// };

// #else  // when using std::format
// template <>
// struct std::formatter<my_type> : std::formatter<std::string> {
//     auto format(my_type my, format_context &ctx) const -> decltype(ctx.out())
//     {
//       return std::format_to(ctx.out(), "[error i={}]",
//       error_as_string(my.err));
//     }
// };
// #endif

#endif
