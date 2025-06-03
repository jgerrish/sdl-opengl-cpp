#include <doctest/doctest.h>
#include <memory>

#define HAVE_OPENGL

#include "SDL_opengl.h"
#include "SDL_test_common.h"
#include "opengl.h"
#include "shader.h"
#include "shader_test.h"

using ::testing::_;
using testing::Assign;
using testing::ByRef;
using testing::SaveArg;
using testing::SetArgPointee;

using namespace sdl_opengl_cpp;

ShaderTester::ShaderTester(const std::shared_ptr<GLContext> &ctx) {
  //  string vertex_shader_src = string("
  const char vertex_shader_src[] = {
      "#version 330 core\n"
      "layout(location=0) in vec3 in_Position;\n"
      "void main(void)\n"
      "{\n"
      "    gl_Position = vec4(in_Position, 1.0);\n"
      "}\n"};

  shader.emplace(
      Shader(string("test-shader"), ctx, vertex_shader_src, GL_VERTEX_SHADER));
}

GLuint ShaderTester::shader_handle() { return shader->shader; }

void ShaderTester::set_invalid_shader() { shader->shader = 0; }

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that the Shader constructor works with exceptions") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  GLint success;

  GLint return_success_value = 1;

  EXPECT_CALL(*mock_opengl_context, glCreateShader(GL_VERTEX_SHADER))
      .Times(1)
      .WillOnce(testing::Return(1));

  EXPECT_CALL(*mock_opengl_context, glShaderSource(1, 1, _, NULL)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glCompileShader(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glGetShaderiv(1, GL_COMPILE_STATUS, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<2>(testing::ByRef(return_success_value)),
          testing::SaveArgPointee<2>(&success)));

  EXPECT_CALL(*mock_opengl_context, glDeleteShader(1)).Times(1);

  ShaderTester shader_tester(mock_opengl_context);

  // Expect a successful compile (parameter three, success, is set
  // to 1)
  CHECK_EQ(success, 1);

  // Test that the actual VertexBufferObject object was also updated
  CHECK_EQ(shader_tester.shader_handle(), 1);
}

#else

TEST_CASE(
    "testing that the Shader constructor works with exceptions disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  GLint success;

  GLint return_success_value = 1;

  EXPECT_CALL(*mock_opengl_context, glCreateShader(GL_VERTEX_SHADER))
      .Times(1)
      .WillOnce(testing::Return(1));

  EXPECT_CALL(*mock_opengl_context, glShaderSource(1, 1, _, NULL)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glCompileShader(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glGetShaderiv(1, GL_COMPILE_STATUS, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<2>(testing::ByRef(return_success_value)),
          testing::SaveArgPointee<2>(&success)));

  EXPECT_CALL(*mock_opengl_context, glDeleteShader(1)).Times(1);

  ShaderTester shader_tester(mock_opengl_context);

  // Expect a successful compile (parameter three, success, is set
  // to 1)
  CHECK_EQ(success, 1);

  // Test that the actual VertexBufferObject object was also updated
  CHECK_EQ(shader_tester.shader_handle(), 1);
}

#endif

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that the Shader constructor throws ShaderCreationError with "
          "exceptions enabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  EXPECT_CALL(*mock_opengl_context, glCreateShader(GL_VERTEX_SHADER))
      .Times(1)
      .WillOnce(testing::Return(0));

  CHECK_THROWS_WITH_AS(
      [mock_opengl_context] {
        ShaderTester shader_tester(mock_opengl_context);
      }(),
      "ERROR::SHADER::CREATE_SHADER_FAILED", ShaderCreationError);
}

#else

TEST_CASE("testing that the Shader constructor sets ShaderCreationError error "
          "code with exceptions disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  EXPECT_CALL(*mock_opengl_context, glCreateShader(GL_VERTEX_SHADER))
      .Times(1)
      .WillOnce(testing::Return(0));

  ShaderTester shader_tester(mock_opengl_context);

  if (shader_tester.shader) {
    CHECK(!shader_tester.shader->valid());
    CHECK_EQ(shader_tester.shader->get_last_error(),
              shader::error::ShaderCreationError);
  } else {
    CHECK(false);
  }

  // Test that the actual shader handle was not updated
  CHECK_EQ(shader_tester.shader_handle(), 0);
}

#endif

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that the Shader throws ShaderCompilationError with "
          "exceptions enabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  GLint result;

  GLint return_failure_value = 0;

  EXPECT_CALL(*mock_opengl_context, glCreateShader(GL_VERTEX_SHADER))
      .Times(1)
      .WillOnce(testing::Return(1));

  EXPECT_CALL(*mock_opengl_context, glShaderSource(1, 1, _, NULL)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glCompileShader(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glGetShaderiv(1, GL_COMPILE_STATUS, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<2>(testing::ByRef(return_failure_value)),
          testing::SaveArgPointee<2>(&result)));

  // Set infoLen to zero, we don't care about the info log for now
  GLint infoLen = 0;
  GLint info_log_result;

  EXPECT_CALL(*mock_opengl_context, glGetShaderiv(1, GL_INFO_LOG_LENGTH, _))
      .Times(1)
      .WillOnce(
          testing::DoAll(testing::SetArgPointee<2>(testing::ByRef(infoLen)),
                         testing::SaveArgPointee<2>(&info_log_result)));

  // We never delete the shader because we throw an exception.
  // Is this the right behavior?  The OpenGL hardware may not clean
  // up the handles after the program exits.  I don't know.
  // TODO: Research this
  //
  // This mocking is exposing some of these critical design
  // decisions.  TODO: Look at the other classes to see if this happens too.
  // Maybe try and catch and cleanup and rethrow in the constructor
  // but not compile()?
  // Regardless, probably want to have both the exception and non-exception
  // code behave the same way.
  //
  // TODO: Look at the other classes, VertexArrayObject throws some
  // stuff in bind we need to catch and I'm sure there are more in
  // Program.
  EXPECT_CALL(*mock_opengl_context, glDeleteShader(1)).Times(1);

  CHECK_THROWS_WITH_AS(
      [mock_opengl_context] {
        ShaderTester shader_tester(mock_opengl_context);
      }(),
      "ERROR::SHADER::COMPILATION_FAILED", ShaderCompilationError);
}

#else

TEST_CASE("testing that the Shader sets ShaderCompilationError code with "
          "exceptions disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  GLint result;

  GLint return_failure_value = 0;

  EXPECT_CALL(*mock_opengl_context, glCreateShader(GL_VERTEX_SHADER))
      .Times(1)
      .WillOnce(testing::Return(1));

  EXPECT_CALL(*mock_opengl_context, glShaderSource(1, 1, _, NULL)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glCompileShader(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glGetShaderiv(1, GL_COMPILE_STATUS, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<2>(testing::ByRef(return_failure_value)),
          testing::SaveArgPointee<2>(&result)));

  // Set infoLen to zero, we don't care about the info log for now
  GLint infoLen = 0;
  GLint info_log_result;

  EXPECT_CALL(*mock_opengl_context, glGetShaderiv(1, GL_INFO_LOG_LENGTH, _))
      .Times(1)
      .WillOnce(
          testing::DoAll(testing::SetArgPointee<2>(testing::ByRef(infoLen)),
                         testing::SaveArgPointee<2>(&info_log_result)));

  // We never delete the shader because we throw an exception.
  // Is this the right behavior?  the OpenGL hardware may not clean
  // up the handles after the program exits.  I don't know.
  EXPECT_CALL(*mock_opengl_context, glDeleteShader(1)).Times(1);

  ShaderTester shader_tester(mock_opengl_context);

  // Expect a failed compile (parameter three, success, is set to 1)
  if (shader_tester.shader) {
    CHECK_EQ(shader_tester.shader->valid(), false);
    CHECK_EQ(shader_tester.shader->get_last_error(),
             shader::error::ShaderCompilationError);
  } else {
    CHECK(false);
  }

  // Test that the actual VertexBufferObject object was reset during cleanup
  CHECK_EQ(shader_tester.shader_handle(), 0);
}

#endif
