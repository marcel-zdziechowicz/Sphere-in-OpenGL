#include <stdlib.h>
#include <stdio.h>
#include "openglheader.h" /* najpierw ten */
#include <GLFW/glfw3.h>  /* potem ten */

#include "utilities.h"
#include "lights.h"
#include "app1b.h"

int     WindowHandle;
int     win_width, win_height;
int     last_xi, last_eta;
float   left, right, bottom, top, near, far;

int     app_state = STATE_NOTHING;
float   viewer_rvec[3] = {1.0,0.0,0.0};
float   viewer_rangle = 0.0;

int     option = 0;
char    animate = false;
char    enlight = false;

GLFWwindow* window = NULL;
double x, y;

void ReshapeFunc (GLFWwindow* window, int width, int height )
{
  float lr;

  glViewport ( 0, 0, width, height );      /* klatka jest calym oknem */
  lr = 0.5533*(float)width/(float)height;  /* przyjmujemy aspekt rowny 1 */
  M4x4Frustumf ( trans.pm, NULL, -lr, lr, -0.5533, 0.5533, 5.0, 15.0 );
  glBindBuffer ( GL_UNIFORM_BUFFER, trbuf );
  glBufferSubData ( GL_UNIFORM_BUFFER, trbofs[2], 16*sizeof(GLfloat), trans.pm );
  win_width = width,  win_height = height;
  left = -(right = lr);  bottom = -(top = 0.5533);  near = 5.0;  far = 15.0;
  ExitIfGLError ( "ReshapeFunc" );
  SetupMVPMatrix ();
} /*ReshapeFunc*/

void DisplayFunc ( void )
{
    glfwPollEvents();
    glClearColor ( 0.0, 0.0, 0.0, 1.0 );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable ( GL_DEPTH_TEST );

    if (animate) {
        model_rot_angle = model_rot_angle0 + 0.78539816 * TimerToc();
        SetupModelMatrix(model_rot_axis, model_rot_angle);
    }
    glfwGetCursorPos(window, &x, &y);
    MotionFunc((int)x, (int)y);
    DrawSphere(option, enlight);
    glUseProgram ( 0 );
    glFlush ();
    glfwSwapBuffers(window);
} /*DisplayFunc*/

void ToggleAnimation ( void )
{
  if ( (animate = !animate) ) {
    TimerTic ();
  }
  else {
    model_rot_angle0 = model_rot_angle;
  }
} /*ToggleAnimation*/

void ToggleLight ( void )
{
  enlight = !enlight;
  DisplayFunc();
} /*ToggleLight*/

void KeyboardFunc (GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        option = 0;
    }
    else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        option = 1;
    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        option = 2;
    }
    else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        ToggleLight();
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        ToggleAnimation();
    }
} /*KeyboardFunc*/

void MouseFunc (GLFWwindow* window,  int button, int state, int mods)
{
  switch ( app_state ) {
case STATE_NOTHING:
    if (button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS) {
        glfwGetCursorPos(window, &last_xi, &last_eta);
        app_state = STATE_TURNING;
    }
    break;
case STATE_TURNING:
    if (button == GLFW_MOUSE_BUTTON_LEFT && state != GLFW_PRESS) {
        app_state = STATE_NOTHING;
    }
    break;
default:
    break;
  }
} /*MouseFunc*/

void MotionFunc ( int x, int y )
{
  switch ( app_state ) {
case STATE_TURNING:
    if ( x != last_xi || y != last_eta ) {
      RotateViewer ( x-last_xi, y-last_eta );
      last_xi = x,  last_eta = y;
      //DisplayFunc();
    }
    break;
default:
    break;
  }
} /*MotionFunc*/

void IdleFunc ( void )
{
  model_rot_angle = model_rot_angle0 + 0.78539816 * TimerToc ();
  SetupModelMatrix ( model_rot_axis, model_rot_angle );
  DisplayFunc();
} /*IdleFunc*/

void Initialise ( int argc, char *argv[] )
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(480, 360, "Sphere", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Error: Could not create window!\n");
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, ReshapeFunc);
    glfwSetKeyCallback(window, KeyboardFunc);
    glfwSetMouseButtonCallback(window, MouseFunc);
    glfwSwapInterval(true);
  GetGLProcAddresses();
  LoadMyShaders();
  InitMyObject();
} /*Initialise*/

int main ( int argc, char *argv[] )
{
  Initialise ( argc, argv );
  ReshapeFunc(window, 480, 360);
  while (!glfwWindowShouldClose(window)) {
      DisplayFunc();
  }

  Cleanup();
  glfwTerminate();
  return 0;
} /*main*/
