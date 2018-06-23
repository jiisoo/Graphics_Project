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
#include "Camera.h"

#include "transform.hpp"

void init();
void display();
void reshape(int, int);
void idle();
void keyboard(unsigned char, int, int);
void special(int, int, int);
void processMenuEvents(int option);
void setBigger(int option);
void setSmaller(int option);
void createGLUTMenus();
void mouseButton(int xCursor, int yCursor);

GLuint program;

Object		g_trike, g_dilo, g_allo, g_bird, g_stry, g_dryo, g_albert, g_carcar, g_steg;  // dinosour

Object    g_cctv, g_grass, g_gate, g_rock, g_wall;

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

static GLfloat TopLeftX, TopLeftY, BottomRightX, BottomRightY;
kmuvcl::math::mat4x4f   mat_PVM;

kmuvcl::math::vec4f light_vector      = kmuvcl::math::vec4f(10.0f,10.0f, 10.0f);
kmuvcl::math::vec4f light_ambient     = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
kmuvcl::math::vec4f light_diffuse     = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
kmuvcl::math::vec4f light_specular    = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);

std::string g_filename;
//Object      g_model;        // object

float model_scale = 1.0f;
float model_angle = 0.0f;
float size = 1.0f;

std::chrono::time_point<std::chrono::system_clock> prev, curr;

/*
void mouseButton(int xCursor, int yCursor){
    GLdouble projection[16];
    GLdouble modelView[16];
    GLint viewPort[4];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glGetIntegerv(GL_VIEWPORT, viewPort);

    GLfloat zCursor, winX, winY;
    winX = (float)xCursor;
    winY = (float)viewPort[3]-(float)yCursor;
    glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zCursor);

    if(gluUnProject(winX, winY, zCursor, modelView, projection, viewPort, &wx, &wy, &wz)==GLU_FALSE){
	printf("실패\n");
    }
}
*/
void createGLUTMenus()
{
    int menu, submenu1, submenu2;

    submenu1 = glutCreateMenu(setBigger);
    glutAddMenuEntry("2",1);
    glutAddMenuEntry("2.5",2);
    glutAddMenuEntry("3",3);

    submenu2 = glutCreateMenu(setSmaller);
    glutAddMenuEntry("0.8",1);
    glutAddMenuEntry("0.6",2);
    glutAddMenuEntry("0.3",3);


    menu = glutCreateMenu(processMenuEvents);
    glutAddSubMenu("Bigger",submenu1);
    glutAddSubMenu("Smaller",submenu2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
  }

void processMenuEvents(int option)
{
    switch (option)
    {
        case 7 :
            break;
    }
}

void setBigger(int option)
{
    switch (option)
    {
        case 1 :
            size=2.0f;
            break;
        case 2 :
            size=2.5f;
            break;
        case 3 :
            size=3.0f;
            break;

    }
    glutPostRedisplay();
}

void setSmaller(int option)
{
    switch (option)
    {
        case 1 :
            size=0.8f;
            break;
        case 2 :
            size=0.6f;
            break;
        case 3 :
            size=0.3f;
            break;

    }
    glutPostRedisplay();
}

// void MyKeyboard(unsigned char keypressed, int X, int Y)
// {
//   switch(keypressed)
//   {
//     case 'Q' :
//       exit(0);  break;
//     case 'q' :
//       exit(0);  break;
//     case 27 :
//      exit(0);  break;
//   }
// }
void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y)
<<<<<<< HEAD
{
=======
{ 
<<<<<<< HEAD
  TopLeftX = X;
  TopLeftY = Y;
  
  if(Button == GLUT_LEFT_BUTTON)
  {
     g_camera.mouse_click(TopLeftX/300.0, (900-TopLeftY)/300.0);
=======
>>>>>>> 7782ba70e229b4d5cc68d64949b50affc36f3414
  int x = X;
  int y = Y;

  if(Button == GLUT_LEFT_BUTTON)
  {
     g_camera.mouse_click(x, y);
>>>>>>> 57092384fd3cc2f9354fa65f51365fc2d05cef12
  }
}



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
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(900, 900);
  glutCreateWindow("Modeling & Navigating Your Studio");

  glutDisplayFunc(display);

  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutIdleFunc(idle);
  createGLUTMenus();

  if (glewInit() != GLEW_OK)
  {
      std::cerr << "failed to initialize glew" << std::endl;
      return -1;
  }

  init();
  //glutKeyboardFunc(MyKeyboard);
  glutMouseFunc(MyMouseClick);
  glutMainLoop();

  return 0;
}

void init()
{

  g_trike.load_simple_obj("./object/Triceratops/trike.obj");
  g_cctv.load_simple_obj("./object/Camera/Camera.obj");
  g_dilo.load_simple_obj("./object/Dilophosaurus/dilo.obj");
  g_allo.load_simple_obj("./object/Allosarus/allo.obj");
  g_bird.load_simple_obj("./object/lowpoly_bird.obj");
  g_grass.load_simple_obj("./object/grass/Grass_02.obj");
  g_gate.load_simple_obj("./object/gate.obj");
  g_rock.load_simple_obj("./object/Rock/Rock.obj");
//  g_wall.load_simple_obj("./object/wall/walls.obj");
  g_stry.load_simple_obj("./object/Styracosarus/stry.obj");
  g_dryo.load_simple_obj("./object/Dryosarus/dryo.obj");
  g_albert.load_simple_obj("./object/Albertosaurus/albert.obj");
  g_carcar.load_simple_obj("./object/Carcharodontosaurus/carcar.obj");
  g_steg.load_simple_obj("./object/Stegosarus/steg.obj");

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
  glViewport(0, 0, 900, 900);
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
////////////////////Wall//////////////////////


////////////////////Rock//////////////////////
  S = kmuvcl::math::scale(2.0f, 2.0f, 2.0f);
  T = kmuvcl::math::translate(-7.0f, 2.0f, -20.0f);
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
  g_rock.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

////////////////////Gate//////////////////////
  S = kmuvcl::math::scale(13.0f, 13.0f, 13.0f);
  T = kmuvcl::math::translate(0.0f, 0.0f, 5.0f);
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

/////////////////////dryo///////////////////////////////
  S = kmuvcl::math::scale(4.0f, 4.0f, 4.0f);
  R = kmuvcl::math::rotate(-90.0f, 90.0f, 0.0f, 0.0f);
  T = kmuvcl::math::translate(-12.0f, 0.0f, -20.0f); //바꾸지말기
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

  g_dryo.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

/////////////////////stry///////////////////////////////
  S = kmuvcl::math::scale(2.0f, 2.0f, 2.0f);
  R = kmuvcl::math::rotate(-90.0f, 90.0f, 0.0f, 0.0f);
  T = kmuvcl::math::translate(15.0f, 0.0f, -20.0f); //바꾸지말기
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

  g_stry.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

/////////////////////dilo 분홍이///////////////////////////////
  S = kmuvcl::math::scale(2.0f, 2.0f, 2.0f);
  R = kmuvcl::math::rotate(-90.0f, 90.0f, 0.0f, 0.0f);
  T = kmuvcl::math::translate(9.0f, 0.0f, -20.0f); //바꾸지말기
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

///////////////////////////allo 파랑이//////////////////////////////
  S = kmuvcl::math::scale(size, size, size);
  R = kmuvcl::math::rotate(-90.0f, 90.0f, 0.0f, 0.0f);
  T = kmuvcl::math::translate(3.0f, 0.0f, -20.0f); //바꾸지 말기
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


/////////////////////Triceratops 노랑이//////////////////////
  S = kmuvcl::math::scale(1.0f, 1.0f, 1.0f);
  R = kmuvcl::math::rotate(-90.0f, 90.0f, 0.0f, 0.0f);
  T = kmuvcl::math::translate(-3.0f, 0.0f, -20.0f);//바꾸지말기
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


//////////////////////albert   빨강이//////////////////////////////
  S = kmuvcl::math::scale(1.5f, 1.5f, 1.5f);
  R = kmuvcl::math::rotate(-90.0f, 90.0f, 0.0f, 0.0f);
  T = kmuvcl::math::translate(0.0f, 0.0f, -40.0f);
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
  g_albert.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

//////////////////////carcar  보라//////////////////////////////
  S = kmuvcl::math::scale(1.5f, 1.5f, 1.5f);
  R = kmuvcl::math::rotate(-90.0f, 90.0f, 0.0f, 0.0f);
  T = kmuvcl::math::translate(6.0f, 0.0f, -40.0f);
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
  g_carcar.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

//////////////////////steg  초록//////////////////////////////
  S = kmuvcl::math::scale(1.5f, 1.5f, 1.5f);
  R = kmuvcl::math::rotate(-90.0f, 90.0f, 0.0f, 0.0f);
  T = kmuvcl::math::translate(12.0f, 0.0f, -40.0f);
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
  g_steg.draw(loc_a_vertex, loc_a_normal,
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

///////////////////////////bird//////////////////////////////
  S = kmuvcl::math::scale(0.2f, 0.2f, 0.2f);
  R = kmuvcl::math::rotate(model_angle, 0.0f, 1.0f, 0.0f);
  T = kmuvcl::math::translate(-5.0f, 7.0f, -10.0f);
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
  g_bird.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);

//Grass_02
  float x = -8.0f;
  for(int i=0; i<5; i++){
    S = kmuvcl::math::scale(5.0f, 1.0f, 10.0f);
    T = kmuvcl::math::translate(x, 0.0f, -20.0f);
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
  else if(key == 27 || key == 'Q' || key == 'q')
  {
    exit(0);
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
  // curr = std::chrono::system_clock::now();
  //
  // std::chrono::duration<float> elaped_seconds = (curr - prev);
  //
  // model_angle += 20 * elaped_seconds.count();
  //
  // prev = curr;
  //
  // glutPostRedisplay();
  model_angle += 1.0f;

  if(model_angle > 360)
  model_angle = 0.0f;
  glutPostRedisplay();
}
