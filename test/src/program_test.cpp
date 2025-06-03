#include <doctest/doctest.h>

#define HAVE_OPENGL

#include "SDL_opengl.h"
#include "SDL_test_common.h"
#include "opengl.h"
#include "program.h"
#include "program_test.h"
#include "shader.h"

using ::testing::_;
using testing::Assign;
using testing::ByRef;
using testing::SaveArg;
using testing::SetArgPointee;

using namespace sdl_opengl_cpp;

const char vertex_shader_src[] = {"#version 330 core\n"
                                  "layout(location=0) in vec3 in_Position;\n"
                                  "void main(void)\n"
                                  "{\n"
                                  "    gl_Position = vec4(in_Position, 1.0);\n"
                                  "}\n"};
const char fragment_shader_src[] = {
    "#version 330 core\n"
    "layout(location=0) out vec4 out_FragmentColor;"
    "uniform vec3 object_color;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    out_FragmentColor = vec4(object_color, 1.0f);\n"
    "}\n"};

std::deque<std::unique_ptr<Shader>> ProgramTester::createShaders(
    const std::shared_ptr<GLContext> &ctx,
    const std::vector<UncompiledShader> &uncompiled_shaders) {
  std::deque<std::unique_ptr<Shader>> shaders = {};

  for (auto s : uncompiled_shaders) {
    std::unique_ptr<Shader> vertex_shader =
        std::make_unique<Shader>(Shader(s.name, ctx, s.source.c_str(), s.type));

    shaders.push_back(std::move(vertex_shader));
  }

  return shaders;
}

ProgramTester::ProgramTester(
    const std::shared_ptr<GLContext> &ctx,
    const std::vector<UncompiledShader> &uncompiled_shaders) {
  std::deque<std::unique_ptr<Shader>> shaders =
      createShaders(ctx, uncompiled_shaders);

  program.emplace(Program(string("main"), ctx, shaders));
}

GLuint ProgramTester::program_handle() { return program->program; }

void ProgramTester::set_invalid_program() { program->program = 0; }

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that the Program constructor works with exceptions") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  GLint success;

  GLint return_success_value = 1;

  // Shader OpenGL calls

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

  // Program OpenGL calls
  EXPECT_CALL(*mock_opengl_context, glCreateProgram())
      .Times(1)
      .WillOnce(testing::Return(1));

  // Program OpenGL calls
  EXPECT_CALL(*mock_opengl_context, glAttachShader(1, 1)).Times(1);

  GLint program_link_success;
  GLint return_program_link_success_value = 1;

  EXPECT_CALL(*mock_opengl_context, glLinkProgram(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glGetProgramiv(1, _, _))
      .Times(1)
      .WillOnce(
          testing::DoAll(testing::SetArgPointee<2>(
                             testing::ByRef(return_program_link_success_value)),
                         testing::SaveArgPointee<2>(&program_link_success)));

  EXPECT_CALL(*mock_opengl_context, glGetError()).Times(1);

  // We don't actually call glGetAttachedShaders, since everything is
  // owned and kept track of by the objects themselves.
  //
  // GLint count;
  // GLint return_count = 1;

  // EXPECT_CALL(*mock_opengl_context, glGetAttachedShaders(1, MAX_SHADERS, _,
  // _))
  //   .Times(1)
  //   .WillOnce(testing::DoAll(testing::SetArgPointee<2>(testing::ByRef(return_count)),
  // 			       testing::SaveArgPointee<2>(&count)));

  EXPECT_CALL(*mock_opengl_context, glDeleteProgram(1)).Times(1);

  UncompiledShader uncompiled_shader = {string("vertex-shader"),
                                        vertex_shader_src, GL_VERTEX_SHADER};
  vector<UncompiledShader> uncompiled_shaders = {uncompiled_shader};
  ProgramTester program_tester(mock_opengl_context, uncompiled_shaders);

  // Expect a successful compile (parameter three, success, is set
  // to 1)
  CHECK_EQ(success, 1);

  // Test that the actual VertexBufferObject object was also updated
  CHECK_EQ(program_tester.program_handle(), 1);
}

#else

TEST_CASE(
    "testing that the Program constructor works with exceptions disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  GLint success;

  GLint return_success_value = 1;

  // Shader OpenGL calls

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

  EXPECT_CALL(*mock_opengl_context, glGetError()).Times(1);

  EXPECT_CALL(*mock_opengl_context, glDeleteShader(1)).Times(1);

  // Program OpenGL calls
  EXPECT_CALL(*mock_opengl_context, glCreateProgram())
      .Times(1)
      .WillOnce(testing::Return(1));

  // Program OpenGL calls
  EXPECT_CALL(*mock_opengl_context, glAttachShader(1, 1)).Times(1);

  GLint program_link_success;
  GLint return_program_link_success_value = 1;

  EXPECT_CALL(*mock_opengl_context, glLinkProgram(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glGetProgramiv(1, _, _))
      .Times(1)
      .WillOnce(
          testing::DoAll(testing::SetArgPointee<2>(
                             testing::ByRef(return_program_link_success_value)),
                         testing::SaveArgPointee<2>(&program_link_success)));

  // We don't actually call glGetAttachedShaders, since everything is
  // owned and kept track of by the objects themselves.
  //
  // GLint count;
  // GLint return_count = 1;

  // EXPECT_CALL(*mock_opengl_context, glGetAttachedShaders(1, MAX_SHADERS, _,
  // _))
  //   .Times(1)
  //   .WillOnce(testing::DoAll(testing::SetArgPointee<2>(testing::ByRef(return_count)),
  // 			       testing::SaveArgPointee<2>(&count)));

  EXPECT_CALL(*mock_opengl_context, glDeleteProgram(1)).Times(1);

  UncompiledShader uncompiled_shader = {string("vertex-shader"),
                                        vertex_shader_src, GL_VERTEX_SHADER};
  vector<UncompiledShader> uncompiled_shaders = {uncompiled_shader};
  ProgramTester program_tester(mock_opengl_context, uncompiled_shaders);

  // Expect a successful compile (parameter three, success, is set
  // to 1)
  CHECK_EQ(success, 1);

  // Test that the actual VertexBufferObject object was also updated
  CHECK_EQ(program_tester.program_handle(), 1);
}

#endif

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that the Program constructor works with two shaders and "
          "exceptions") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  GLint s1_success, s2_success;

  GLint s1_return_success_value = 1, s2_return_success_value = 1;

  // Shader OpenGL calls

  // TODO: If we want, make sure the shader types line up correctly
  EXPECT_CALL(*mock_opengl_context, glCreateShader(_))
      .Times(2)
      .WillOnce(testing::Return(1))
      .WillOnce(testing::Return(2));

  EXPECT_CALL(*mock_opengl_context, glShaderSource(1, 1, _, NULL)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glShaderSource(2, 1, _, NULL)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glCompileShader(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glCompileShader(2)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glGetShaderiv(1, GL_COMPILE_STATUS, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<2>(testing::ByRef(s1_return_success_value)),
          testing::SaveArgPointee<2>(&s1_success)));

  EXPECT_CALL(*mock_opengl_context, glGetShaderiv(2, GL_COMPILE_STATUS, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<2>(testing::ByRef(s2_return_success_value)),
          testing::SaveArgPointee<2>(&s2_success)));

  EXPECT_CALL(*mock_opengl_context, glDeleteShader(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glDeleteShader(2)).Times(1);

  // Program OpenGL calls
  EXPECT_CALL(*mock_opengl_context, glCreateProgram())
      .Times(1)
      .WillOnce(testing::Return(1));

  // Program OpenGL calls
  EXPECT_CALL(*mock_opengl_context, glAttachShader(1, 1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glAttachShader(1, 2)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glGetError()).Times(1);

  GLint program_link_success;
  GLint return_program_link_success_value = 1;

  EXPECT_CALL(*mock_opengl_context, glLinkProgram(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glGetProgramiv(1, _, _))
      .Times(1)
      .WillOnce(
          testing::DoAll(testing::SetArgPointee<2>(
                             testing::ByRef(return_program_link_success_value)),
                         testing::SaveArgPointee<2>(&program_link_success)));

  EXPECT_CALL(*mock_opengl_context, glDeleteProgram(1)).Times(1);

  UncompiledShader uncompiled_vertex_shader = {
      string("vertex-shader"), vertex_shader_src, GL_VERTEX_SHADER};
  UncompiledShader uncompiled_fragment_shader = {
      string("fragment-shader"), fragment_shader_src, GL_FRAGMENT_SHADER};
  vector<UncompiledShader> uncompiled_shaders = {uncompiled_vertex_shader,
                                                 uncompiled_fragment_shader};
  ProgramTester program_tester(mock_opengl_context, uncompiled_shaders);

  // Expect a successful compile (parameter three, success, is set
  // to 1)
  CHECK_EQ(s1_success, 1);
  CHECK_EQ(s2_success, 1);

  // Test that the actual VertexBufferObject object was also updated
  CHECK_EQ(program_tester.program_handle(), 1);
}

#else

TEST_CASE("testing that the Program constructor works with two shaders and "
          "exceptions disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockOpenGLContext> mock_opengl_context =
      std::make_shared<MockOpenGLContext>(glcontext);

  GLint s1_success, s2_success;

  GLint s1_return_success_value = 1, s2_return_success_value = 1;

  // Shader OpenGL calls

  // TODO: If we want, make sure the shader types line up correctly
  EXPECT_CALL(*mock_opengl_context, glCreateShader(_))
      .Times(2)
      .WillOnce(testing::Return(1))
      .WillOnce(testing::Return(2));

  EXPECT_CALL(*mock_opengl_context, glShaderSource(1, 1, _, NULL)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glShaderSource(2, 1, _, NULL)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glCompileShader(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glCompileShader(2)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glGetShaderiv(1, GL_COMPILE_STATUS, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<2>(testing::ByRef(s1_return_success_value)),
          testing::SaveArgPointee<2>(&s1_success)));

  EXPECT_CALL(*mock_opengl_context, glGetShaderiv(2, GL_COMPILE_STATUS, _))
      .Times(1)
      .WillOnce(testing::DoAll(
          testing::SetArgPointee<2>(testing::ByRef(s2_return_success_value)),
          testing::SaveArgPointee<2>(&s2_success)));

  EXPECT_CALL(*mock_opengl_context, glDeleteShader(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glDeleteShader(2)).Times(1);

  // Program OpenGL calls
  EXPECT_CALL(*mock_opengl_context, glCreateProgram())
      .Times(1)
      .WillOnce(testing::Return(1));

  // Program OpenGL calls
  EXPECT_CALL(*mock_opengl_context, glAttachShader(1, 1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glAttachShader(1, 2)).Times(1);

  EXPECT_CALL(*mock_opengl_context, glGetError()).Times(1);

  GLint program_link_success;
  GLint return_program_link_success_value = 1;

  EXPECT_CALL(*mock_opengl_context, glLinkProgram(1)).Times(1);
  EXPECT_CALL(*mock_opengl_context, glGetProgramiv(1, _, _))
      .Times(1)
      .WillOnce(
          testing::DoAll(testing::SetArgPointee<2>(
                             testing::ByRef(return_program_link_success_value)),
                         testing::SaveArgPointee<2>(&program_link_success)));

  EXPECT_CALL(*mock_opengl_context, glDeleteProgram(1)).Times(1);

  UncompiledShader uncompiled_vertex_shader = {
      string("vertex-shader"), vertex_shader_src, GL_VERTEX_SHADER};
  UncompiledShader uncompiled_fragment_shader = {
      string("fragment-shader"), fragment_shader_src, GL_FRAGMENT_SHADER};
  vector<UncompiledShader> uncompiled_shaders = {uncompiled_vertex_shader,
                                                 uncompiled_fragment_shader};
  ProgramTester program_tester(mock_opengl_context, uncompiled_shaders);

  // Expect a successful compile (parameter three, success, is set
  // to 1)
  CHECK_EQ(s1_success, 1);
  CHECK_EQ(s2_success, 1);

  // Test that the actual VertexBufferObject object was also updated
  CHECK_EQ(program_tester.program_handle(), 1);
}

#endif
