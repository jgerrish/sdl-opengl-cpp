#ifndef _SDL_OPENGL_CPP_MOVE_CHECKER_H_
#define _SDL_OPENGL_CPP_MOVE_CHECKER_H_

namespace sdl_opengl_cpp {

//! MoveChecker is an abstract class that defines an interface for
//! checking whether an object has been moved with a move assignment
//! or move copy constructor.
class MoveChecker {
public:
  //! Checks whether the object is in a "valid but unspecified state"
  //!
  //! This should be called at the top of any
  //! method that requires an object not be in a "valid but unspecified
  //! state"
  //!
  //! \return true if the object is in a "valid but unspecifed state"
  //!         false if the object is in a valid state and can be used.
  //!
  virtual bool is_in_unspecified_state() = 0;
};

} // namespace sdl_opengl_cpp

#endif
