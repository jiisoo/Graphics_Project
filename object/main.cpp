// Defines the entry point for the console application.
//
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "Object.h"
#include "Camera.h"
#include "Shader.h"

#include "transform.hpp"

void init();
void display();
void reshape(int, int);
void idle();
void keyboard(unsigned char, int, int);
void special(int, int, int);

GLuint program;

Object		g_trike, g_dilo, g_allo, g_grass, g_gate;  // dinosour

Object    g_cctv;

Camera		g_camera;											// viewer (you)

GLint  loc_a_vertex;
GLint  loc_a_normal;

GLint  loc_u_pvm_matrix;
GLint  loc_u_view_matrix;
GLint  loc_u_model_matrix;
GLint  loc_u_normal_matrix;

GLint  loc_u_light_vector;

GLint  loc_u_light_ambient;
GLint  loc_u_light_diffuse;
GLint  loc_u_light_specular;

GLint  loc_u_material_ambient;
GLint  loc_u_material_diffuse;
GLint  loc_u_material_specular;
GLint  loc_u_material_shininess;

kmuvcl::math::mat4x4f   mat_PVM;

kmuvcl::math::vec4f light_vector      = kmuvcl::math::vec4f(10.0f, 10.0f, 10.0f);
kmuvcl::math::vec4f light_ambient     = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
kmuvcl::math::vec4f light_diffuse     = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
kmuvcl::math::vec4f light_specular    = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);

std::string g_filename;
//Object      g_model;        // object

float model_scale = 1.0f;
float model_angle = 0.0f;

std::chrono::time_point<std::chrono::system_clock> prev, curr;

int main(int argc, char* argv[])
{
  /*
  if (argc > 1)
  {
    g_filename = argv[2];
  }
  else
  {
    g_filename = "./data/cube.obj";
  }
  */

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 640);
  glutCreateWindow("Modeling & Navigating Your Studio");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutIdleFunc(idle);

  if (glewInit() != GLEW_OK)
  {
      std::cerr << "failed to initialize glew" << std::endl;
      return -1;
  }

  init();

  glutMainLoop();

  return 0;
}

void init()
{
  //g_model.load_simple_obj(g_filename);
  //g_model.print();
  g_trike.load_simple_obj("./object/Triceratops/trike.obj");
  g_cctv.load_simple_obj("./object/Camera/Camera.obj");
  g_dilo.load_simple_obj("./object/Dilophosaurus/dilo.obj");
  g_allo.load_simple_obj("./object/Allosarus/allo.obj");
  g_grass.load_simple_obj("./object/grass/Grass_02.obj");
  g_gate.load_simple_obj("./object/gate.obj");

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // for filled polygon rendering

  glEnable(GL_DEPTH_TEST);

  program = Shader::create_program("./shader/phong_vert.glsl", "./shader/phong_frag.glsl");

  loc_u_pvm_matrix         = glGetUniformLocation(program, "u_pvm_matrix");
  loc_u_view_matrix        = glGetUniformLocation(program, "u_view_matrix");
  loc_u_model_matrix       = glGetUniformLocation(program, "u_model_matrix");
  loc_u_normal_matrix      = glGetUniformLocation(program, "u_normal_matrix");

  loc_u_light_vector       = glGetUniformLocation(program, "u_light_vector");

  loc_u_light_ambient      = glGetUniformLocation(program, "u_light_ambient");
  loc_u_light_diffuse      = glGetUniformLocation(program, "u_light_diffuse");
  loc_u_light_specular     = glGetUniformLocation(program, "u_light_specular");

  loc_u_material_ambient   = glGetUniformLocation(program, "u_material_ambient");
  loc_u_material_diffuse   = glGetUniformLocation(program, "u_material_diffuse");
  loc_u_material_specular  = glGetUniformLocation(program, "u_material_specular");
  loc_u_material_shininess = glGetUniformLocation(program, "u_material_shininess");

  loc_a_vertex             = glGetAttribLocation(program, "a_vertex");
  loc_a_normal             = glGetAttribLocation(program, "a_normal");

  prev = curr = std::chrono::system_clock::now();
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  // Camera setting
  kmuvcl::math::mat4x4f   mat_Proj, mat_View, mat_Model;
  kmuvcl::math::mat4x4f  S, T, R;
  kmuvcl::math::mat3x3f mat_Normal;

  // camera extrinsic param
	mat_View = kmuvcl::math::lookAt(
		g_camera.position()(0), g_camera.position()(1), g_camera.position()(2),				// eye position
		g_camera.center_position()(0), g_camera.center_position()(1), g_camera.center_position()(2), // center position
		g_camera.up_direction()(0), g_camera.up_direction()(1), g_camera.up_direction()(2)			// up direction
		);

  // camera intrinsic param
  mat_Proj = kmuvcl::math::perspective(g_camera.fovy(), 1.0f, 0.001f, 10000.0f);

// TODO: draw furniture by properly transforming each object
////////////////////Gate//////////////////////
  S = kmuvcl::math::scale(13.0f, 13.0f, 13.0f);
  T = kmuvcl::math::translate(-5.0f, 0.0f, 5.0f);
  mat_Model = T * S ;
  mat_PVM = mat_Proj*mat_View*mat_Model;

  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glUniform3fv(loc_u_light_vector, 1, light_vector);
  glUniform4fv(loc_u_light_ambient, 1, light_ambient);
  glUniform4fv(loc_u_light_diffuse, 1, light_diffuse);
  glUniform4fv(loc_u_light_specular, 1, light_specular);
  g_gate.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

/////////////////////Triceratops//////////////////////
  S = kmuvcl::math::scale(1.0f, 1.0f, 1.0f);
  R = kmuvcl::math::rotate(-90.0f, 90.0f, 0.0f, 0.0f);
  T = kmuvcl::math::translate(-3.0f, 0.0f, -15.0f);
  mat_Model = T * S * R;
  mat_PVM = mat_Proj*mat_View*mat_Model;

  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glUniform3fv(loc_u_light_vector, 1, light_vector);
  glUniform4fv(loc_u_light_ambient, 1, light_ambient);
  glUniform4fv(loc_u_light_diffuse, 1, light_diffuse);
  glUniform4fv(loc_u_light_specular, 1, light_specular);
  g_trike.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

////////////////////////////cctv//////////////////////////////
  S = kmuvcl::math::scale(1.5f, 1.5f, 1.5f);
  T = kmuvcl::math::translate(0.0f, 10.0f, -13.0f);
  mat_Model = T * S;
  mat_PVM = mat_Proj*mat_View*mat_Model;

  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glUniform3fv(loc_u_light_vector, 1, light_vector);
  glUniform4fv(loc_u_light_ambient, 1, light_ambient);
  glUniform4fv(loc_u_light_diffuse, 1, light_diffuse);
  glUniform4fv(loc_u_light_specular, 1, light_specular);
  g_cctv.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

/////////////////////dilo///////////////////////////////
  S = kmuvcl::math::scale(2.0f, 2.0f, 2.0f);
  R = kmuvcl::math::rotate(-90.0f, 90.0f, 0.0f, 0.0f);
  T = kmuvcl::math::translate(6.0f, 0.0f, -15.0f);
  mat_Model = T * S *R;
  mat_PVM = mat_Proj*mat_View*mat_Model;

  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glUniform3fv(loc_u_light_vector, 1, light_vector);
  glUniform4fv(loc_u_light_ambient, 1, light_ambient);
  glUniform4fv(loc_u_light_diffuse, 1, light_diffuse);
  glUniform4fv(loc_u_light_specular, 1, light_specular);

  g_dilo.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

///////////////////////////allo//////////////////////////////
  S = kmuvcl::math::scale(1.0f, 1.0f, 1.0f);
  R = kmuvcl::math::rotate(-90.0f, 90.0f, 0.0f, 0.0f);
  T = kmuvcl::math::translate(3.0f, 0.0f, -15.0f);
  mat_Model = T * S * R;
  mat_PVM = mat_Proj*mat_View*mat_Model;

  mat_Normal(0, 0) = mat_Model(0, 0);
  mat_Normal(0, 1) = mat_Model(0, 1);
  mat_Normal(0, 2) = mat_Model(0, 2);
  mat_Normal(1, 0) = mat_Model(1, 0);
  mat_Normal(1, 1) = mat_Model(1, 1);
  mat_Normal(1, 2) = mat_Model(1, 2);
  mat_Normal(2, 0) = mat_Model(2, 0);
  mat_Normal(2, 1) = mat_Model(2, 1);
  mat_Normal(2, 2) = mat_Model(2, 2);

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glUniform3fv(loc_u_light_vector, 1, light_vector);
  glUniform4fv(loc_u_light_ambient, 1, light_ambient);
  glUniform4fv(loc_u_light_diffuse, 1, light_diffuse);
  glUniform4fv(loc_u_light_specular, 1, light_specular);
  g_allo.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

//Grass_02
  float x = -20.0f;
  for(int i=0; i<10; i++){
    S = kmuvcl::math::scale(1.0f, 1.0f, 1.0f);
    T = kmuvcl::math::translate(x, 0.0f, -15.0f);
    mat_Model = T * S;
    mat_PVM = mat_Proj*mat_View*mat_Model;

    mat_Normal(0, 0) = mat_Model(0, 0);
    mat_Normal(0, 1) = mat_Model(0, 1);
    mat_Normal(0, 2) = mat_Model(0, 2);
    mat_Normal(1, 0) = mat_Model(1, 0);
    mat_Normal(1, 1) = mat_Model(1, 1);
    mat_Normal(1, 2) = mat_Model(1, 2);
    mat_Normal(2, 0) = mat_Model(2, 0);
    mat_Normal(2, 1) = mat_Model(2, 1);
    mat_Normal(2, 2) = mat_Model(2, 2);

    glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
    glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
    glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);
    glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

    glUniform3fv(loc_u_light_vector, 1, light_vector);
    glUniform4fv(loc_u_light_ambient, 1, light_ambient);
    glUniform4fv(loc_u_light_diffuse, 1, light_diffuse);
    glUniform4fv(loc_u_light_specular, 1, light_specular);
    g_grass.draw(loc_a_vertex, loc_a_normal,
      loc_u_material_ambient, loc_u_material_diffuse,
      loc_u_material_specular, loc_u_material_shininess);
    x+= 3.0f;
    }


/*
  kmuvcl::math::mat4x4f mat_View = kmuvcl::math::inverse(mat_View_inv);

	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glUniform3fv(loc_u_light_vector, 1, light_vector);
  glUniform4fv(loc_u_light_ambient, 1, light_ambient);
  glUniform4fv(loc_u_light_diffuse, 1, light_diffuse);
  glUniform4fv(loc_u_light_specular, 1, light_specular);
  Shader::check_gl_error("glUniform4fv");

  g_model.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

*/



	glUseProgram(0);
  Shader::check_gl_error("draw");

  glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

//카메라 회전
void keyboard(unsigned char key, int x, int y)
{
  if(key=='a' || key=='A')
  {
    g_camera.rotate_left(10.0f);
  }
  else if(key=='d' || key=='D')
  {
    g_camera.rotate_right(10.0f);
  }
  else if(key=='s' || key=='S')
  {
    g_camera.rotate_down(10.0f);
  }
  else if(key=='w' || key=='W')
  {
    g_camera.rotate_up(10.0f);
  }
	glutPostRedisplay();
}

//카메라 이동
void special(int key, int x, int y)
{
  switch (key) {
    case GLUT_KEY_LEFT:
      g_camera.move_left(0.3f);
      break;
    case GLUT_KEY_RIGHT:
      g_camera.move_right(0.3f);
      break;
    case GLUT_KEY_UP:
      g_camera.move_forward(0.3f);
      break;
    case GLUT_KEY_DOWN:
      g_camera.move_backward(0.3f);
      break;
    }
}

void idle()
{
  curr = std::chrono::system_clock::now();

  std::chrono::duration<float> elaped_seconds = (curr - prev);

  model_angle += 10 * elaped_seconds.count();

  prev = curr;

  glutPostRedisplay();
}
