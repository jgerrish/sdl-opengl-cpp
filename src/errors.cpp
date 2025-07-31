#ifdef NO_EXCEPTIONS

#include "errors.h"

using namespace std;

using namespace sdl_opengl_cpp;

Errors::Errors() {}

bool Errors::valid() {
  // Here, ! is operating on the std::optional to test if it has been
  // set or is std::nullopt
  // if (!last_operation_failed) {
  //   return true;
  // }

  if (is_in_unspecified_state()) {
    if (!last_error) {
      // If we didn't do this it would require two calls to valid()
      set_error(std::optional<error>(error::UnspecifiedStateError));
    }
  }

  return !last_operation_failed;
}

std::optional<error> Errors::get_last_error() {
  if (is_in_unspecified_state()) {
    if (!last_error) {
      // last_error hasn't been set yet.  There wasn't an error that
      // caused gl_context or VBO to be reset, so we assume it was a
      // move construction or assignment.
      //
      // If we didn't do this it would require two calls to get_last_error()
      set_error(std::optional<error>(error::UnspecifiedStateError));
    }
  }

  return last_error;
}

bool Errors::set_error(const std::optional<error> &error) {
  bool state_changed = false;

  if (last_error != error) {
    // One optional is set and the other isn't
    state_changed = true;
  } else {
    if (!last_error)
      // There was no last error, and the new optional is equal, so it
      // also is no error.
      state_changed = false;
    else
      // There was a last error, and the new error optional is also an error.
      // The state changed if they are different
      state_changed = (*last_error == *error);
  }

  if (!last_error) {
    last_operation_failed = true;
    last_error = error;
  }

  // We don't call the error handler on every set_error call
  // Only if all the following conditions are met:
  // 1. The error state changed
  // 2. An error handler is registered
  // 3. The error is an actual error, and not std::nullopt
  //
  // This means it doesn't get called on things like resetting the
  // error, setting an error that was already set, or when there is no
  // error handler.
  if (state_changed && error_handler && error) {
    (*error_handler)(*error);
  }

  return state_changed;
}

int Errors::register_error_handler(
    const std::function<void(const error &error)> &handler) {
  error_handler.emplace(handler);
  return 0;
}

#endif
