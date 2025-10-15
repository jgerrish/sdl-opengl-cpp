#ifndef _SDL_OPENGL_CPP_CLIPPING_PLANES_H_
#define _SDL_OPENGL_CPP_CLIPPING_PLANES_H_

namespace sdl_opengl_cpp {

//! Data structure to define a vertical, horizontal and depth clipping
//! plane
class ClippingPlanes {
public:
  //! Create a new ClippingPlanes
  //!
  //! \param left_ Left vertical clipping plane coordinate
  //! \param right_ Right vertical clipping plane coordinate
  //! \param bottom_ Bottom horizontal clipping plane coordinate
  //! \param top_ Top horizontal clipping plane coordinate
  //! \param near_ Nearer depth clipping plane coordinate
  //! \param far_ Farther depth clipping plane coordinate
  //!
  //! \returns the new ClippingPlanes object
  ClippingPlanes(const GLdouble &left_, const GLdouble &right_,
                 const GLdouble &bottom_, const GLdouble &top_,
                 const GLdouble &near_, const GLdouble &far_) // noexcept
      : left{left_}, right{right_}, bottom{bottom_}, top{top_}, near{near_},
        far{far_} {};

  //! copy constructor
  // ClippingPlanes(const ClippingPlanes &cp)// noexcept
  //   : left { cp.left }, right { cp.right },
  //     bottom { cp.bottom }, top { cp.top },
  //     near { cp.near }, far { cp.far } { };

  // // copy assignment operator
  // ClippingPlanes &operator=(const ClippingPlanes &cp) {
  //   left = cp.left;
  //   right = cp.right;
  //   bottom = cp.bottom;
  //   top = cp.top;
  //   near = cp.near;
  //   far = cp.far;

  //   return *this;
  // }

  // //! move constructor
  // ClippingPlanes(ClippingPlanes &&cp) noexcept
  //   : left { cp.left }, right { cp.right },
  //     bottom { cp.bottom }, top { cp.top },
  //     near { cp.near }, far { cp.far } { };

  // ClippingPlanes &operator=(ClippingPlanes &&cp) noexcept {
  //   if (&cp != this) {
  //     left = cp.left;
  //     right = cp.right;
  //     bottom = cp.bottom;
  //     top = cp.top;
  //     near = cp.near;
  //     far = cp.far;
  //   }

  //   return *this;
  // }

  //! Left vertical clipping plane coordinate
  GLdouble left;

  //! Right vertical clipping plane coordinate
  GLdouble right;

  //! Bottom horizontal clipping plane coordinate
  GLdouble bottom;

  //! Top horizontal clipping plane coordinate
  GLdouble top;

  //! Nearer depth clipping plane coordinate
  //! The distances are negative if the plane is to be behind the
  //! viewer
  // TODO: Do we want axis prefixes?  We should probably have them for all of
  // the member variables if we do.
  GLdouble near;

  //! Farther depth clipping plane coordinate
  //! The distances are negative if the plane is to be behind the
  //! viewer
  GLdouble far;
};

} // namespace sdl_opengl_cpp

#endif
