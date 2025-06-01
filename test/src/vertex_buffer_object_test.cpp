#include <doctest/doctest.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <spdlog/spdlog.h>

#include "gl_context.h"
#include "mock_opengl.h"
#include "vertex_buffer_object.h"
#include "vertex_buffer_object_test.h"

using ::testing::_;
using testing::Assign;
using testing::ByRef;
using testing::Rethrow;
using testing::SaveArg;
using testing::SetArgPointee;
using testing::Throw;

using namespace sdl_opengl_cpp;
using namespace vertex_buffer_object;

VertexBufferObjectTester::VertexBufferObjectTester(
    const std::shared_ptr<GLContext> &ctx) {
  vector<GLfloat> vertices = {0, 1, 2, 3, 4, 5, 6, 7, 8};

  vbo.emplace(VertexBufferObject(string("test-vbo"), ctx, vertices));
}

GLuint VertexBufferObjectTester::VBO() { return vbo->VBO; }

void VertexBufferObjectTester::set_invalid_VBO() { vbo->VBO = 0; }

namespace sdl_opengl_cpp {
void vertex_buffer_constructor_works_with_exceptions_expectations(
    std::shared_ptr<MockOpenGLContext> &mock_opengl_context, GLuint &buffer,
    GLuint &first_available_buffer) {
  EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          // SetArgPointee sets the nth argument
          // (zero-based, so the second argument
          // here) to the variable passed in.
          // Here we pass in a reference to
          // first_available_buffer.  So when we
          // call glGenBuffers, whatever is
          // passed into the second argument is
          // set to the same thing as
          // first_available_buffer
          testing::SetArgPointee<1>(testing::ByRef(first_available_buffer)),
          // SaveArgPointee saves whatever the
          // nth (zero-based) argument is.  Here
          // we save the second argument, which
          // is set by SetArgPointee above.  So
          // it's kind of redundant, but we also
          // want to check the behavior and not
          // just set expectations.
          testing::SaveArgPointee<1>(&buffer)));

  EXPECT_CALL(*mock_opengl_context, glGetError())
      .Times(2)
      .WillRepeatedly(testing::Return(GL_NO_ERROR));

  // Bind the VBO buffer
  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, first_available_buffer))
      .Times(1);

  // Set the buffer data
  EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

  // Unbind the VBO buffer
  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(1);

  // Called on destruction of the VertexBufferObject
  EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(1);
}
} // namespace sdl_opengl_cpp

TEST_SUITE("sdl_opengl_cpp_vertex_buffer_object") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

#ifndef NO_EXCEPTIONS

  TEST_CASE("testing that VertexBufferObject constructor throws "
            "GenBuffersError exception") {
    std::shared_ptr<MockOpenGLContext> mock_opengl_context =
        std::make_shared<MockOpenGLContext>(glcontext);

    GLuint buffer = 0;

    // If there is a failure with glGenBuffers, the value of buffer isn't
    // set or is set to zero.
    GLuint first_available_buffer = 0;

    // ON_CALL defines only the behavior.  Not the expectations on how the mock
    // is called. ON_CALL(*mock_opengl_context, glGenBuffers(1, &buffer))
    //   .WillOnce(Assign(buffer, 1));

    // EXPECT_CALL defines both the behavior and the expectations on how the
    // mock is called.
    EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
        .Times(1)
        .WillOnce(testing::DoAll(
            testing::SetArgPointee<1>(testing::ByRef(first_available_buffer)),
            testing::SaveArgPointee<1>(&buffer)));

    EXPECT_CALL(*mock_opengl_context, glGetError())
        .Times(1)
        .WillRepeatedly(testing::Return(GL_NO_ERROR));

    CHECK_THROWS_WITH_AS(
        [mock_opengl_context] {
          VertexBufferObjectTester vbo_tester(mock_opengl_context);
        }(),
        "ERROR::VERTEX_BUFFER::GEN_BUFFERS_FAILED", GenBuffersError);

    CHECK_EQ(buffer, 0);
  }
#else
  TEST_CASE("testing that VertexBufferObject constructor sets error flag") {
    std::shared_ptr<MockOpenGLContext> mock_opengl_context =
        std::make_shared<MockOpenGLContext>(glcontext);

    GLuint buffer = 0;

    GLuint first_available_buffer = 0;

    // EXPECT_CALL defines both the behavior and the expectations on how the
    // mock is called.
    EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
        .Times(1)
        .WillOnce(testing::DoAll(
            // SetArgPointee sets the nth argument
            // (zero-based, so the second argument
            // here) to the variable passed in.
            // Here we pass in a reference to
            // first_available_buffer.  So when we
            // call glGenBuffers, whatever is
            // passed into the second argument is
            // set to the same thing as
            // first_available_buffer
            testing::SetArgPointee<1>(testing::ByRef(first_available_buffer)),
            // SaveArgPointee saves whatever the
            // nth (zero-based) argument is.  Here
            // we save the second argument, which
            // is set by SetArgPointee above.  So
            // it's kind of redundant, but we also
            // want to check the behavior and not
            // just set expectations.
            testing::SaveArgPointee<1>(&buffer)));

    EXPECT_CALL(*mock_opengl_context, glGetError())
        .Times(1)
        .WillRepeatedly(testing::Return(GL_NO_ERROR));

    VertexBufferObjectTester vbo_tester(mock_opengl_context);
    if (vbo_tester.vbo) {
      CHECK_EQ(vbo_tester.vbo->valid(), false);
      CHECK_EQ(vbo_tester.vbo->get_last_error(),
               vertex_buffer_object::error::GenBuffersError);
    } else {
      CHECK(false);
    }
    CHECK_EQ(buffer, 0);
  }
#endif

#ifndef NO_EXCEPTIONS

  TEST_CASE(
      "testing that VertexBufferObject constructor works with exceptions") {
    std::shared_ptr<MockOpenGLContext> mock_opengl_context =
        std::make_shared<MockOpenGLContext>(glcontext);

    // This is what we are testing, it should set this buffer
    GLuint buffer = 0;

    // This is what the mocked function GenBuffersError should set the
    // second parameter to on the first call.
    GLuint first_available_buffer = 1;

    EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
        .Times(1)
        .WillOnce(testing::DoAll(
            // SetArgPointee sets the nth argument
            // (zero-based, so the second argument
            // here) to the variable passed in.
            // Here we pass in a reference to
            // first_available_buffer.  So when we
            // call glGenBuffers, whatever is
            // passed into the second argument is
            // set to the same thing as
            // first_available_buffer
            testing::SetArgPointee<1>(testing::ByRef(first_available_buffer)),
            // SaveArgPointee saves whatever the
            // nth (zero-based) argument is.  Here
            // we save the second argument, which
            // is set by SetArgPointee above.  So
            // it's kind of redundant, but we also
            // want to check the behavior and not
            // just set expectations.
            testing::SaveArgPointee<1>(&buffer)));

    EXPECT_CALL(*mock_opengl_context, glGetError())
        .Times(2)
        .WillRepeatedly(testing::Return(GL_NO_ERROR));

    // Bind the VBO buffer
    EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, first_available_buffer))
        .Times(1);

    // Set the buffer data
    EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

    // Unbind the VBO buffer
    EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(1);

    // Called on destruction of the VertexBufferObject
    EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(1);

    VertexBufferObjectTester vbo_tester(mock_opengl_context);

    CHECK_EQ(buffer, 1);

    // Test that the actual VertexBufferObject object was also updated
    CHECK_EQ(vbo_tester.VBO(), 1);
  }

#else

  TEST_CASE("testing that VertexBufferObject constructor works with exceptions "
            "disabled") {
    std::shared_ptr<MockOpenGLContext> mock_opengl_context =
        std::make_shared<MockOpenGLContext>(glcontext);

    // This is what we are testing, it should set this buffer
    GLuint buffer = 0;

    // This is what the mocked function GenBuffersError should set the
    // second parameter to on the first call.
    GLuint first_available_buffer = 1;

    // EXPECT_CALL defines both the behavior and the expectations on how the
    // mock is called.
    EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
        .Times(1)
        .WillOnce(testing::DoAll(
            // SetArgPointee sets the nth argument
            // (zero-based, so the second argument
            // here) to the variable passed in.
            // Here we pass in a reference to
            // first_available_buffer.  So when we
            // call glGenBuffers, whatever is
            // passed into the second argument is
            // set to the same thing as
            // first_available_buffer
            testing::SetArgPointee<1>(testing::ByRef(first_available_buffer)),
            // SaveArgPointee saves whatever the
            // nth (zero-based) argument is.  Here
            // we save the second argument, which
            // is set by SetArgPointee above.  So
            // it's kind of redundant, but we also
            // want to check the behavior and not
            // just set expectations.
            testing::SaveArgPointee<1>(&buffer)));

    EXPECT_CALL(*mock_opengl_context, glGetError())
        .Times(2)
        .WillRepeatedly(testing::Return(GL_NO_ERROR));

    // Bind the VBO buffer
    EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, first_available_buffer))
        .Times(1);

    // Set the buffer data
    EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

    // Unbind the VBO buffer
    EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(1);

    // Called on destruction of the VertexBufferObject
    EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(1);

    VertexBufferObjectTester vbo_tester(mock_opengl_context);

    CHECK_EQ(buffer, 1);

    // Test that the actual VertexBufferObject object was also updated
    CHECK_EQ(vbo_tester.VBO(), 1);

    if (vbo_tester.vbo) {
      CHECK(vbo_tester.vbo->valid());
    } else {
      CHECK(false);
    }
  }

#endif

  TEST_CASE("testing that VertexBufferObject bind() works") {
    std::shared_ptr<MockOpenGLContext> mock_opengl_context =
        std::make_shared<MockOpenGLContext>(glcontext);

    // EXPECT_CALL defines both the behavior and the expectations on how the
    // mock is called.
    EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
        .Times(1)
        .WillOnce(testing::DoAll(testing::SetArgPointee<1>(1)));

    EXPECT_CALL(*mock_opengl_context, glGetError())
        .Times(2)
        .WillRepeatedly(testing::Return(GL_NO_ERROR));

    // glBindBuffer gets call twice, once in the constructor and once
    // when bind() is called.
    EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 1)).Times(2);

    // Set the buffer data
    EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

    // Unbind the VBO buffer
    EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(1);

    // Called on destruction of the VertexBufferObject
    EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(1);

    VertexBufferObjectTester vbo_tester(mock_opengl_context);

    // Test the bind() method
    (*vbo_tester.vbo).bind();
  }

#ifndef NO_EXCEPTIONS

  TEST_CASE("testing that VertexBufferObject bind() throws an exception when "
            "the VBO handle is invalid") {
    std::shared_ptr<MockOpenGLContext> mock_opengl_context =
        std::make_shared<MockOpenGLContext>(glcontext);

    CHECK_THROWS_WITH_AS(
        [mock_opengl_context] {
          // EXPECT_CALL defines both the behavior and the expectations on how
          // the mock is called.
          EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
              .Times(1)
              .WillOnce(testing::DoAll(testing::SetArgPointee<1>(1)));

          EXPECT_CALL(*mock_opengl_context, glGetError())
              .Times(2)
              .WillRepeatedly(testing::Return(GL_NO_ERROR));

          // glBindBuffer gets call once, once in the constructor.
          // It doesn't get called in bind() because we have an invalid
          // handle.
          EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 1)).Times(1);

          // Set the buffer data
          EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

          // Unbind the VBO buffer, should get called once in the constructor
          EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(1);

          // Called on destruction of the VertexBufferObject
          //
          // glDeleteBuffers doesn't get called since we explicity set the
          // VBO handle to zero.  The object competely loses track of the
          // "valid" handle.  (It's not a real handle, just a mock one).
          EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(0);

          VertexBufferObjectTester vbo_tester(mock_opengl_context);

          // Set the VBO to an invalid state (0 for OpenGL)
          vbo_tester.set_invalid_VBO();

          // Test the bind() method
          (*vbo_tester.vbo).bind();
        }(),
        "Vertex Buffer Object is in an unspecified state",
        VertexBufferObjectUnspecifiedStateError);
  }

#else
  TEST_CASE("testing that VertexBufferObject bind() sets error flag when the "
            "VBO handle is invalid") {
    std::shared_ptr<MockOpenGLContext> mock_opengl_context =
        std::make_shared<MockOpenGLContext>(glcontext);

    GLuint buffer = 0;

    GLuint first_available_buffer = 1;

    // EXPECT_CALL defines both the behavior and the expectations on how the
    // mock is called.
    EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
        .Times(1)
        .WillOnce(testing::DoAll(
            // SetArgPointee sets the nth argument
            // (zero-based, so the second argument
            // here) to the variable passed in.
            // Here we pass in a reference to
            // first_available_buffer.  So when we
            // call glGenBuffers, whatever is
            // passed into the second argument is
            // set to the same thing as
            // first_available_buffer
            testing::SetArgPointee<1>(testing::ByRef(first_available_buffer)),
            // SaveArgPointee saves whatever the
            // nth (zero-based) argument is.  Here
            // we save the second argument, which
            // is set by SetArgPointee above.  So
            // it's kind of redundant, but we also
            // want to check the behavior and not
            // just set expectations.
            testing::SaveArgPointee<1>(&buffer)));
    ;

    EXPECT_CALL(*mock_opengl_context, glGetError())
        .Times(2)
        .WillRepeatedly(testing::Return(GL_NO_ERROR));

    // glBindBuffer gets call once
    // It doesn't get called with an invalid VBO handle.
    EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 1)).Times(1);

    // Set the buffer data
    EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

    // Unbind the VBO buffer, should get called once in the constructor
    EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(1);

    // Called on destruction of the VertexBufferObject

    // Since we explicitly invalidate the handle, the
    // VertexBufferObject object loses track of the "real" handle.
    // It doesn't call glDeleteBuffers on "invalid" handles.
    EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(0);

    VertexBufferObjectTester vbo_tester(mock_opengl_context);

    if (vbo_tester.vbo) {
      CHECK(vbo_tester.vbo->valid());
    } else {
      CHECK(false);
    }

    // Set the VBO to an invalid state (0 for OpenGL)
    vbo_tester.set_invalid_VBO();

    // Test that the actual VertexBufferObject object was updated
    CHECK_EQ(vbo_tester.VBO(), 0);

    // Test the bind() method
    (*vbo_tester.vbo).bind();

    if (vbo_tester.vbo) {
      CHECK_EQ(vbo_tester.vbo->valid(), false);
      CHECK_EQ(
          vbo_tester.vbo->get_last_error(),
          vertex_buffer_object::error::VertexBufferObjectUnspecifiedStateError);
    } else {
      CHECK(false);
    }
  }

#endif

#ifdef NO_EXCEPTIONS
  TEST_CASE("testing that we can register an error handler with "
            "VertexBufferObject and that it is called") {
    std::shared_ptr<MockOpenGLContext> mock_opengl_context =
        std::make_shared<MockOpenGLContext>(glcontext);

    GLuint buffer = 0;

    GLuint first_available_buffer = 1;

    // EXPECT_CALL defines both the behavior and the expectations on how the
    // mock is called.
    EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
        .Times(1)
        .WillOnce(testing::DoAll(
            // SetArgPointee sets the nth argument
            // (zero-based, so the second argument
            // here) to the variable passed in.
            // Here we pass in a reference to
            // first_available_buffer.  So when we
            // call glGenBuffers, whatever is
            // passed into the second argument is
            // set to the same thing as
            // first_available_buffer
            testing::SetArgPointee<1>(testing::ByRef(first_available_buffer)),
            // SaveArgPointee saves whatever the
            // nth (zero-based) argument is.  Here
            // we save the second argument, which
            // is set by SetArgPointee above.  So
            // it's kind of redundant, but we also
            // want to check the behavior and not
            // just set expectations.
            testing::SaveArgPointee<1>(&buffer)));
    ;

    EXPECT_CALL(*mock_opengl_context, glGetError())
        .Times(2)
        .WillRepeatedly(testing::Return(GL_NO_ERROR));

    // glBindBuffer gets call once
    // It doesn't get called with an invalid VBO handle.
    EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 1)).Times(1);

    // Set the buffer data
    EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

    // Unbind the VBO buffer, should get called once in the constructor
    EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(1);

    // Called on destruction of the VertexBufferObject

    // Since we explicitly invalidate the handle, the
    // VertexBufferObject object loses track of the "real" handle.
    // It doesn't call glDeleteBuffers on "invalid" handles.
    EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(0);

    VertexBufferObjectTester vbo_tester(mock_opengl_context);

    if (vbo_tester.vbo) {
      CHECK(vbo_tester.vbo->valid());
    } else {
      CHECK(false);
    }

    if (vbo_tester.vbo) {
      vbo_tester.vbo->register_error_handler(
          [](const vertex_buffer_object::error &error) {
            spdlog::info("Received error: {}",
                         vertex_buffer_object::error_as_string(error));

            // Not working with the std::format code from
            // spdlog, need to investigate
            // spdlog::info("Received error: {}", my_type(error));
          });
    }

    // Set the VBO to an invalid state (0 for OpenGL)
    vbo_tester.set_invalid_VBO();

    // Test that the actual VertexBufferObject object was updated
    CHECK_EQ(vbo_tester.VBO(), 0);

    // Test the bind() method
    (*vbo_tester.vbo).bind();

    if (vbo_tester.vbo) {
      CHECK_EQ(vbo_tester.vbo->valid(), false);
      CHECK_EQ(
          vbo_tester.vbo->get_last_error(),
          vertex_buffer_object::error::VertexBufferObjectUnspecifiedStateError);
    } else {
      CHECK(false);
    }
  }
#endif
}
