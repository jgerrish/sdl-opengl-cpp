#ifndef _SDL_OPENGL_CPP_ERRORS_H_
#define _SDL_OPENGL_CPP_ERRORS_H_

#ifdef NO_EXCEPTIONS

#include <functional>
#include <memory>
#include <optional>

#include "error.h"
#include "move_checker.h"

namespace sdl_opengl_cpp {

class Errors : public MoveChecker {
public:
  //! Create an Errors implementation
  Errors();

  //! Return true if the last method call was successful.
  //!
  //! This method will also return false if the object is in a "valid
  //! but unspecified state", for example after a move assignment or
  //! move construction.  This is so that the user doesn't have to
  //! call it twice.  It should ALWAYS return false if the object
  //! can't be used or the last operation had an error.

  //!
  //! If this library has been compiled with NO_EXCEPTIONS defined,
  //! this must be called after every function that can throw an
  //! error.
  //!
  //! \return true if the last method call was successful AND the
  //!         object is not in a "valid but unspecified state".
  bool valid();

  //! If the object is not valid(), this method will get the last error
  //! that occurred.
  //!
  //! If the error caused the object to be put into a "valid but
  //! uncertain state" then this method will return the error that
  //! caused that, NOT UnspecifiedStateError.  If the operation that
  //! caused the "valid but unspecified state" was just a move
  //! assignment or move constructor, then it should return
  //! UnspecifiedStateError.
  //!
  //! This is a little confusing because getting put into a "valid
  //! but unspecified state" is not necessarily an error.  But for
  //! the valid() call and any subsequent call, it is an error, but
  //! the object was attempted to be used in a "valid but
  //! unspecified state".
  //!
  //! \return the last error that occurred during a method call.
  std::optional<error> get_last_error();

  //! Set the error code
  //!
  //! This should be the only function that changes error_code.
  //! It also calls any error handler functions.
  //!
  //!
  //! \return true if the error state changed
  //!         false if there was no change in the error state
  bool set_error(const std::optional<error> &error);

  //! Register an error handler
  //!
  //! This error handler will get called everytime the state of
  //! last_error changes.
  //!
  //! We don't call the error handler on every set_error call
  //! Only if all the following conditions are met:
  //! 1. The error state changed
  //! 2. An error handler is registered
  //! 3. The error is an actual error, and not std::nullopt
  //!
  //! This means it doesn't get called on things like resetting the
  //! error, setting an error that was already set, or when there is no
  //! error handler.
  //!
  //! There is no information about what function caused the error.
  //!
  //! \return 0 on success, nonzero on an error registering the
  //!         error handler
  int register_error_handler(
      const std::function<void(const error &error)> &handler);

protected:
  //! Whether the last operation failed
  bool last_operation_failed = false;

  //! The last error that occured, or std::nullopt if there was none.
  std::optional<error> last_error = std::nullopt;

  //! The error callback function
  std::optional<std::function<void(const error &error)>> error_handler =
      std::nullopt;
};

} // namespace sdl_opengl_cpp

#endif

#endif
