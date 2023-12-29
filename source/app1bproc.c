#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "openglheader.h"   /* najpierw ten */
#include <GLFW/glfw3.h>  /* potem ten */

#include "utilities.h"
#include "lights.h"
#include "app1b.h"

GLuint  shader_id[5];
GLuint  program_id[2];
GLuint  trbi, trbuf, trbbp, lsbi, lsbuf, lsbbp;
GLint   trbsize, trbofs[5], lsbsize, lsbofs[7];

GLuint  sphere_vao, sphere_vbo[3];

float   model_rot_axis[3] = {1.0,0.0,0.0},
        model_rot_angle0 = 0.0, model_rot_angle;
const float viewer_pos0[4] = {0.0,0.0,10.0,0.0};
TransBl trans;               


void LoadMyShaders ( void )
{
  static const char *filename[] =
    { "../shaders/app1b0.glsl.vert", "../shaders/app1b0.glsl.frag",
      "../shaders/app1b1.glsl.vert", "../shaders/app1b1.glsl.geom", "../shaders/app1b1.glsl.frag" };
  static const GLchar *UTBNames[] =
    { "TransBlock", "TransBlock.mm", "TransBlock.vm", "TransBlock.pm",
      "TransBlock.mvpm", "TransBlock.eyepos" };
  static const GLchar *ULSNames[] =
    { "LSBlock", "LSBlock.nls", "LSBlock.mask",
      "LSBlock.ls[0].ambient", "LSBlock.ls[0].direct", "LSBlock.ls[0].position",
      "LSBlock.ls[0].attenuation", "LSBlock.ls[1].ambient" };

  shader_id[0] = CompileShaderFiles ( GL_VERTEX_SHADER, 1, &filename[0] );
  shader_id[1] = CompileShaderFiles ( GL_FRAGMENT_SHADER, 1, &filename[1]);
  program_id[0] = LinkShaderProgram ( 2, &shader_id[0] );
  shader_id[2] = CompileShaderFiles ( GL_VERTEX_SHADER, 1, &filename[2] );
  shader_id[3] = CompileShaderFiles ( GL_GEOMETRY_SHADER, 1, &filename[3] );
  shader_id[4] = CompileShaderFiles ( GL_FRAGMENT_SHADER, 1, &filename[4] );
  program_id[1] = LinkShaderProgram ( 3, &shader_id[2] );
  GetAccessToUniformBlock ( program_id[1], 5, &UTBNames[0],
                            &trbi, &trbsize, trbofs, &trbbp );
  GetAccessToUniformBlock ( program_id[1], 7, &ULSNames[0],
                            &lsbi, &lsbsize, lsbofs, &lsbbp );
  AttachUniformBlockToBP ( program_id[0], UTBNames[0], trbbp );
  trbuf = NewUniformBlockObject ( trbsize, trbbp );
  lsbuf = NewUniformBlockObject ( lsbsize, lsbbp );
  ExitIfGLError ( "LoadMyShaders" );
} /*LoadMyShaders*/

void SetupMVPMatrix ( void )
{
  GLfloat m[16], mvp[16];

  M4x4Multf ( m, trans.vm, trans.mm );
  M4x4Multf ( mvp, trans.pm, m );
  glBindBuffer ( GL_UNIFORM_BUFFER, trbuf );
  glBufferSubData ( GL_UNIFORM_BUFFER, trbofs[3], 16*sizeof(GLfloat), mvp );
  ExitIfGLError ( "SetupNVPMatrix" );
} /*SetupMVPMatrix*/

void SetupModelMatrix ( float axis[3], float angle )
{
  M4x4RotateVf ( trans.mm, axis[0], axis[1], axis[2], angle );
  glBindBuffer ( GL_UNIFORM_BUFFER, trbuf );
  glBufferSubData ( GL_UNIFORM_BUFFER, trbofs[0], 16*sizeof(GLfloat), trans.mm );
  ExitIfGLError ( "SetupModelMatrix" );
  SetupMVPMatrix ();
} /*SetupModelMatrix*/

void InitViewMatrix ( void )
{
  memcpy ( trans.eyepos, viewer_pos0, 4*sizeof(GLfloat) );
  M4x4Translatef ( trans.vm, -viewer_pos0[0], -viewer_pos0[1], -viewer_pos0[2] );
  glBindBuffer ( GL_UNIFORM_BUFFER, trbuf );
  glBufferSubData ( GL_UNIFORM_BUFFER, trbofs[1], 16*sizeof(GLfloat), trans.vm );
  glBufferSubData ( GL_UNIFORM_BUFFER, trbofs[4], 4*sizeof(GLfloat), trans.eyepos );
  ExitIfGLError ( "InitViewMatrix" );
  SetupMVPMatrix ();
} /*InitViewMatrix*/

void RotateViewer ( int delta_xi, int delta_eta )
{
  float   vi[3], lgt, angi, vk[3], angk;
  GLfloat tm[16];

  if ( delta_xi == 0 && delta_eta == 0 )
    return;  /* natychmiast uciekamy - nie chcemy dzielic przez 0 */
  vi[0] = (float)delta_eta*(right-left)/(float)win_height;
  vi[1] = (float)delta_xi*(top-bottom)/(float)win_width;
  vi[2] = 0.0;
  lgt = sqrt ( V3DotProductf ( vi, vi ) );
  vi[0] /= lgt;
  vi[1] /= lgt;
  angi = -0.052359878;  /* -3 stopnie */
  V3CompRotationsf ( vk, &angk, viewer_rvec, viewer_rangle, vi, angi );
  memcpy ( viewer_rvec, vk, 3*sizeof(float) );
  viewer_rangle = angk;
  M4x4Translatef ( trans.vm, -viewer_pos0[0], -viewer_pos0[1], -viewer_pos0[2] );
  M4x4MRotateVf ( trans.vm, viewer_rvec[0], viewer_rvec[1], viewer_rvec[2],
                  -viewer_rangle );
  glBindBuffer ( GL_UNIFORM_BUFFER, trbuf );
  glBufferSubData ( GL_UNIFORM_BUFFER, trbofs[1], 16*sizeof(GLfloat), trans.vm );
  M4x4Transposef ( tm, trans.vm );  tm[3] = tm[7] = tm[11] = 0.0;
  M4x4MultMVf ( trans.eyepos, tm, viewer_pos0 );
  glBufferSubData ( GL_UNIFORM_BUFFER, trbofs[4], 4*sizeof(GLfloat), trans.eyepos );
  ExitIfGLError ( "RotateViewer" );
  SetupMVPMatrix ();
} /*RotateViewer*/

void InitLights ( void )
{
  GLfloat amb0[4] = { 0.2, 0.2, 0.3, 1.0 };
  GLfloat dif0[4] = { 0.8, 0.8, 0.8, 1.0 };
  GLfloat pos0[4] = { 1.0, 1.0, 1.0, 0.0 };
  GLfloat atn0[3] = { 1.0, 0.0, 0.0 };

  SetLightAmbient ( 0, amb0 );
  SetLightDiffuse ( 0, dif0 );
  SetLightPosition ( 0, pos0 );
  SetLightAttenuation ( 0, atn0 );
  SetLightOnOff ( 0, 1 );
} /*InitLights*/

#define RADIUS              1
#define PRECISION           1
#define CIRCLES             ((180 / PRECISION) - 1)
#define ANGLES_IN_CIRCLE    (360 / PRECISION)
#define TRIANGLE_STRIPS     (CIRCLES - 1)
#define TRIANGLE_FANS       2
#define FANS                (ANGLES_IN_CIRCLE + 2)
#define STRIPS              (ANGLES_IN_CIRCLE * 2 + 2)
#define SIZE                (3 * CIRCLES * ANGLES_IN_CIRCLE + 6)
#define INDICES             ((TRIANGLE_STRIPS * STRIPS) + (TRIANGLE_FANS * FANS))
#define POINTS              (SIZE / 3)
#define PI                  3.14159265358979323846
#define degToRad(angleInDegrees) ((angleInDegrees) * PI / 180.0)

void ConstructSphereVAO(void)
{
    static float vertices[SIZE];
    static float color[SIZE];
    static GLuint indices[INDICES];
    srand((unsigned int)time(NULL));
    for (int i = 0; i < SIZE; i++) {
        color[i] = (float)rand() / RAND_MAX;
    }

    // TOP
    vertices[0] = 0.0; 
    vertices[1] = RADIUS;
    vertices[2] = 0.0;

    float layer = 90.0f;
    for (int i = 0; i < CIRCLES; i++) {
        layer += PRECISION;
        float Y = RADIUS * sin(degToRad(layer));
        float rad = RADIUS * cos(degToRad(layer));
        float ALPHA = 360.0f;
        for (int j = 0; j < ANGLES_IN_CIRCLE * 3; j += 3) {
            vertices[(int)(i * ANGLES_IN_CIRCLE * 3 + j + 0 + 3)] = rad * cos(degToRad(ALPHA)); // X
            vertices[(int)(i * ANGLES_IN_CIRCLE * 3 + j + 1 + 3)] = Y;                          // Y
            vertices[(int)(i * ANGLES_IN_CIRCLE * 3 + j + 2 + 3)] = rad * sin(degToRad(ALPHA)); // Z
            ALPHA -= PRECISION;
        }
    }

    // BOTTOM
    vertices[SIZE - 3] = 0.0;       // X
    vertices[SIZE - 2] = -RADIUS;   // Y
    vertices[SIZE - 1] = 0.0;       // Z

    int i = 0;
    for (; i <= ANGLES_IN_CIRCLE; i++) {
        indices[i] = i;
    }
    indices[i++] = 1;

    indices[i++] = POINTS - 1;
    for (int j = 0; j < ANGLES_IN_CIRCLE; j++, i++) {
        indices[i] = POINTS - j - 2;
    }
    indices[i++] = POINTS - 2;

    for (int s = 0; s < TRIANGLE_STRIPS; s++) {
        for (int a = 0; a < ANGLES_IN_CIRCLE; a++, i += 2) {
            indices[i] = s * ANGLES_IN_CIRCLE + a + 1;
            indices[i + 1] = (s + 1) * ANGLES_IN_CIRCLE + a + 1;
        }
        indices[i++] = s * ANGLES_IN_CIRCLE + 1;
        indices[i++] = (s + 1) * ANGLES_IN_CIRCLE + 1;
    }

    glGenVertexArrays(1, &sphere_vao);
    glBindVertexArray(sphere_vao);

    glGenBuffers(3, &sphere_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_vbo[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
} /*ConstructSphere*/

void DrawSphere(int opt, char enlight)
{
    glBindVertexArray(sphere_vao);

    switch (opt)
    {
    case 0:
        glUseProgram(program_id[0]);
        glPointSize(1.0);
        glDrawArrays(GL_POINTS, 0, POINTS);
        break;
    case 2:
        glUseProgram(program_id[enlight ? 1 : 0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_vbo[2]);
        glDrawElements(GL_TRIANGLE_FAN, FANS, GL_UNSIGNED_INT, (GLvoid*)0);
        glDrawElements(GL_TRIANGLE_FAN, FANS, GL_UNSIGNED_INT, (GLvoid*)(FANS * sizeof(GLuint)));
        for (int i = 0; i < TRIANGLE_STRIPS; i++) {
            glDrawElements(GL_TRIANGLE_STRIP, 
                STRIPS, GL_UNSIGNED_INT, 
                (GLvoid*)(((TRIANGLE_FANS * FANS) + (i * STRIPS)) * sizeof(GLuint))
            );
        }

    }
} /*DrawSphere*/

void InitMyObject ( void )
{
  TimerInit ();
  memset ( &trans, 0, sizeof(TransBl) );
  memset ( &light, 0, sizeof(LightBl) );
  SetupModelMatrix ( model_rot_axis, model_rot_angle );
  InitViewMatrix ();
  ConstructSphereVAO();
  InitLights ();
} /*InitMyObject*/

void Cleanup ( void )
{
  int i;

  glUseProgram ( 0 );
  for ( i = 0; i < 5; i++ )
    glDeleteShader ( shader_id[i] );
  glDeleteProgram ( program_id[0] );
  glDeleteProgram ( program_id[1] );
  glDeleteBuffers ( 1, &trbuf );
  glDeleteBuffers ( 1, &lsbuf );
  glDeleteVertexArrays ( 1, &sphere_vao );
  glDeleteBuffers ( 3, sphere_vbo );
  ExitIfGLError ( "Cleanup" );
  glfwDestroyWindow(window);
} /*Cleanup*/

