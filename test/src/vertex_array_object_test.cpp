#include <doctest/doctest.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define HAVE_OPENGL

#include "SDL_opengl.h"
#include "SDL_test_common.h"
#include "opengl.h"
#include "vertex_array_object.h"
#include "vertex_array_object_test.h"
#include "vertex_buffer_object.h"

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
using namespace vertex_array_object;

VertexArrayObjectTester::VertexArrayObjectTester(
    const std::shared_ptr<GLContext> &ctx) {
  vector<GLfloat> vertices = {0, 1, 2, 3, 4, 5, 6, 7, 8};

  VertexBufferObject vbo =
      VertexBufferObject(string("test-vbo"), ctx, vertices);

  CHECK(vbo.VBO != 0);
  CHECK(vbo.gl_context != nullptr);

  vao.emplace(VertexArrayObject(string("test-vao"), ctx, std::move(vbo)));
}

GLuint VertexArrayObjectTester::VAO() { return vao->VAO; }

void VertexArrayObjectTester::set_invalid_VAO() { vao->VAO = 0; }

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that VertexArrayObject constructor throws GenVertexArrays "
          "exception") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  // This is what we are testing, it should set this buffer
  GLuint buffer1 = 0, array1 = 0;

  // This is what the mocked function GenBuffersError should set the
  // second parameter to on the first and second call.
  GLuint first_available_buffer = 1, first_available_array = 0;

  EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<1>(testing::ByRef(first_available_buffer)),
          testing::SaveArgPointee<1>(&buffer1)));

  EXPECT_CALL(*mock_opengl_context, glGenVertexArrays(1, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<1>(testing::ByRef(first_available_array)),
          testing::SaveArgPointee<1>(&array1)));

  EXPECT_CALL(*mock_opengl_context, glGetError())
      .Times(3)
      .WillOnce(testing::Return(GL_NO_ERROR))
      .WillOnce(testing::Return(GL_NO_ERROR))
      .WillOnce(testing::Return(GL_OUT_OF_MEMORY));

  // Bind the VBO buffer
  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, first_available_buffer))
      .Times(1);

  // Set the buffer data
  EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

  // Unbind the VBO buffer
  // This only gets called once, in the VertexBufferObject
  // constructor.
  // The VertexArrayObject never binds the VAO and throws before the
  // unbind call.
  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(1);

  // Called on destruction of the VertexBufferObject
  EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(1);

  // VAO calls

  // We don't bind the array because we exit before that point
  EXPECT_CALL(*mock_opengl_context, glBindVertexArray(first_available_buffer))
      .Times(0);

  CHECK_THROWS_WITH_AS(
      [mock_opengl_context] {
        VertexArrayObjectTester vao_tester(mock_opengl_context);
      }(),
      "ERROR::VERTEX_ARRAY::GEN_VERTEX_ARRAYS_FAILED", GenVertexArraysError);
}

#else

TEST_CASE("testing that VertexArrayObject constructor sets error flag") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  // This is what we are testing, it should set this buffer
  GLuint buffer1 = 0, array1 = 0;

  // This is what the mocked function GenBuffersError should set the
  // second parameter to on the first and second call.
  GLuint first_available_buffer = 1, first_available_array = 0;

  EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<1>(testing::ByRef(first_available_buffer)),
          testing::SaveArgPointee<1>(&buffer1)));

  EXPECT_CALL(*mock_opengl_context, glGenVertexArrays(1, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<1>(testing::ByRef(first_available_array)),
          testing::SaveArgPointee<1>(&array1)));

  EXPECT_CALL(*mock_opengl_context, glGetError())
      .Times(3)
      .WillOnce(testing::Return(GL_NO_ERROR))
      .WillOnce(testing::Return(GL_NO_ERROR))
      .WillOnce(testing::Return(GL_OUT_OF_MEMORY));

  // Bind the VBO and VAO buffer
  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, first_available_buffer))
      .Times(1);

  // Set the buffer data
  EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

  // Unbind the VBO buffer
  // This only gets called once, in the VertexBufferObject
  // constructor.
  // The VertexArrayObject never binds the VAO and throws before the
  // unbind call.
  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(1);

  // Called on destruction of the VertexBufferObject
  EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(1);

  // VAO calls

  // We don't bind the array because we exit before that point
  EXPECT_CALL(*mock_opengl_context, glBindVertexArray(first_available_buffer))
      .Times(0);

  VertexArrayObjectTester vao_tester(mock_opengl_context);
  if (vao_tester.vao) {
    CHECK_EQ(vao_tester.vao->valid(), false);
    CHECK_EQ(vao_tester.vao->get_last_error(),
             vertex_array_object::error::GenVertexArraysError);
  } else {
    CHECK(false);
  }
  EXPECT_EQ(array1, 0);
}

#endif

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that VertexArrayObject constructor works with exceptions") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  // This is what we are testing, it should set this buffer
  GLuint buffer1 = 0, array1 = 0;

  // This is what the mocked function GenBuffersError should set the
  // second parameter to on the first and second call.
  GLuint first_available_buffer = 1, first_available_array = 1;

  EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<1>(testing::ByRef(first_available_buffer)),
          testing::SaveArgPointee<1>(&buffer1)));

  EXPECT_CALL(*mock_opengl_context, glGenVertexArrays(1, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<1>(testing::ByRef(first_available_array)),
          testing::SaveArgPointee<1>(&array1)));

  EXPECT_CALL(*mock_opengl_context, glGetError())
      .Times(5)
      .WillRepeatedly(testing::Return(GL_NO_ERROR));

  // Bind the VBO buffer
  // Called once in the VBO constructor and once in the VAO constructor
  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, first_available_buffer))
      .Times(2);

  // Set the buffer data
  EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

  // Unbind the VBO and VBO buffer
  // This gets called twice, in the VertexBufferObject constructor
  // at the end and the VertexArrayObject constructor at the end.

  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(2);

  EXPECT_CALL(*mock_opengl_context, glBindVertexArray(0)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glBindVertexArray(1)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glEnableVertexAttribArray(0)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glVertexAttribPointer(0, 3, _, _, 0, NULL))
      .Times(1);

  EXPECT_CALL(*mock_opengl_context, glDeleteVertexArrays(1, _)).Times(1);

  // Called on destruction of the VertexBufferObject
  EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(1);

  VertexArrayObjectTester vao_tester(mock_opengl_context);
  EXPECT_EQ(array1, 1);
}

#else

TEST_CASE("testing that VertexBufferObject constructor works with exceptions "
          "disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  // This is what we are testing, it should set this buffer
  GLuint buffer1 = 0, array1 = 0;

  // This is what the mocked function GenBuffersError should set the
  // second parameter to on the first and second call.
  GLuint first_available_buffer = 1, first_available_array = 1;

  EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<1>(testing::ByRef(first_available_buffer)),
          testing::SaveArgPointee<1>(&buffer1)));

  EXPECT_CALL(*mock_opengl_context, glGenVertexArrays(1, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<1>(testing::ByRef(first_available_array)),
          testing::SaveArgPointee<1>(&array1)));

  EXPECT_CALL(*mock_opengl_context, glGetError())
      .Times(5)
      .WillRepeatedly(testing::Return(GL_NO_ERROR));

  // Bind the VBO buffer
  // Called once in the VBO constructor and once in the VAO constructor
  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, first_available_buffer))
      .Times(2);

  // Set the buffer data
  EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

  // Unbind the VBO and VBO buffer
  // This gets called twice, in the VertexBufferObject constructor
  // at the end and the VertexArrayObject constructor at the end.
  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(2);

  EXPECT_CALL(*mock_opengl_context, glBindVertexArray(0)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glBindVertexArray(1)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glEnableVertexAttribArray(0)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glVertexAttribPointer(0, 3, _, _, 0, NULL))
      .Times(1);

  EXPECT_CALL(*mock_opengl_context, glDeleteVertexArrays(1, _)).Times(1);

  // Called on destruction of the VertexBufferObject
  EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(1);

  VertexArrayObjectTester vao_tester(mock_opengl_context);
  if (vao_tester.vao) {
    CHECK_EQ(vao_tester.vao->valid(), true);
  } else {
    CHECK(false);
  }
  EXPECT_EQ(array1, 1);
}

#endif

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that VertexArrayObject bind() throws an exception when the "
          "VAO handle is invalid") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  CHECK_THROWS_WITH_AS(
      [mock_opengl_context] {
        EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
            .Times(1)
            .WillOnce(testing::DoAll(testing::SetArgPointee<1>(1)));

        EXPECT_CALL(*mock_opengl_context, glGenVertexArrays(1, _))
            .Times(1)
            .WillOnce(testing::DoAll(testing::SetArgPointee<1>(1)));

        EXPECT_CALL(*mock_opengl_context, glGetError())
            .Times(5)
            .WillRepeatedly(testing::Return(GL_NO_ERROR));

        // Bind the VBO buffer
        EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 1)).Times(2);

        // Set the buffer data
        EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

        // Unbind the VBO and VBO buffer
        // This gets called twice, in the VertexBufferObject constructor
        // at the end and the VertexArrayObject constructor at the end.
        EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(2);

        EXPECT_CALL(*mock_opengl_context, glBindVertexArray(0)).Times(1);
        EXPECT_CALL(*mock_opengl_context, glBindVertexArray(1)).Times(1);

        EXPECT_CALL(*mock_opengl_context, glEnableVertexAttribArray(0))
            .Times(1);

        EXPECT_CALL(*mock_opengl_context,
                    glVertexAttribPointer(0, 3, _, _, 0, NULL))
            .Times(1);

        // This may not be what we want.  Behavior should probably
        // mirror that with no exceptions enabled.
        EXPECT_CALL(*mock_opengl_context, glDeleteVertexArrays(1, _)).Times(0);

        // Called on destruction of the VertexBufferObject
        EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(1);

        VertexArrayObjectTester vao_tester(mock_opengl_context);

        // Set the VAO to an invalid state (0 for OpenGL)
        vao_tester.set_invalid_VAO();

        // Test the bind() method
        (*vao_tester.vao).bind();
      }(),
      "Vertex Array Object is in an unspecified state",
      VertexArrayObjectUnspecifiedStateError);
}

#else
TEST_CASE("testing that VertexArrayObject bind() sets error flag when the VAO "
          "handle is invalid") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  EXPECT_CALL(*mock_opengl_context, glGenBuffers(1, _))
      .Times(1)
      .WillOnce(testing::DoAll(testing::SetArgPointee<1>(1)));

  EXPECT_CALL(*mock_opengl_context, glGenVertexArrays(1, _))
      .Times(1)
      .WillOnce(testing::DoAll(testing::SetArgPointee<1>(1)));

  EXPECT_CALL(*mock_opengl_context, glGetError())
      .Times(5)
      .WillRepeatedly(testing::Return(GL_NO_ERROR));

  // Bind the VBO buffer
  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 1)).Times(2);

  // Set the buffer data
  EXPECT_CALL(*mock_opengl_context, glBufferData(_, _, _, _)).Times(1);

  // Unbind the VBO and VBO buffer
  // This gets called twice, in the VertexBufferObject constructor
  // at the end and the VertexArrayObject constructor at the end.
  EXPECT_CALL(*mock_opengl_context, glBindBuffer(_, 0)).Times(2);

  EXPECT_CALL(*mock_opengl_context, glBindVertexArray(0)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glBindVertexArray(1)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glEnableVertexAttribArray(0)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glVertexAttribPointer(0, 3, _, _, 0, NULL))
      .Times(1);

  // TODO: The non-exception error code does some stuff differently
  // than the exception-based code.  We probably want to make them
  // do the same things.
  EXPECT_CALL(*mock_opengl_context, glDeleteVertexArrays(1, _)).Times(0);

  // Called on destruction of the VertexBufferObject
  EXPECT_CALL(*mock_opengl_context, glDeleteBuffers(1, _)).Times(1);

  VertexArrayObjectTester vao_tester(mock_opengl_context);

  // Set the VAO to an invalid state (0 for OpenGL)
  vao_tester.set_invalid_VAO();

  // Test the bind() method
  (*vao_tester.vao).bind();

  if (vao_tester.vao) {
    CHECK_EQ(vao_tester.vao->valid(), false);
    CHECK_EQ(
        vao_tester.vao->get_last_error(),
        vertex_array_object::error::VertexArrayObjectUnspecifiedStateError);
  } else {
    CHECK(false);
  }
}

#endif
