#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "openglheader.h"

#include "utilities.h"
#include "lights.h"

GLuint  lsbi, lsbuf, lsbbp;
GLint   lsbsize, lsbofs[7];
LightBl light;

void SetLightAmbient ( int l, GLfloat amb[4] )
{
  GLint ofs;

  if ( l < 0 || l >= MAX_NLIGHTS )
    return;
  memcpy ( light.ls[l].ambient, amb, 4*sizeof(GLfloat) );
  ofs = l*(lsbofs[6]-lsbofs[2]) + lsbofs[2];
  glBindBuffer ( GL_UNIFORM_BUFFER, lsbuf );
  glBufferSubData ( GL_UNIFORM_BUFFER, ofs, 4*sizeof(GLfloat), amb );
  ExitIfGLError ( "SetLightAmbient" );
} /*SetLightAmbient*/

void SetLightDiffuse ( int l, GLfloat dif[4] )
{
  GLint ofs;

  if ( l < 0 || l >= MAX_NLIGHTS )
    return;
  memcpy ( light.ls[l].diffuse, dif, 4*sizeof(GLfloat) );
  ofs = l*(lsbofs[6]-lsbofs[2]) + lsbofs[3];
  glBindBuffer ( GL_UNIFORM_BUFFER, lsbuf );
  glBufferSubData ( GL_UNIFORM_BUFFER, ofs, 4*sizeof(GLfloat), dif );
  ExitIfGLError ( "SetLightDiffuse" );
} /*SetLightDiffuse*/

void SetLightPosition ( int l, GLfloat pos[4] )
{
  GLint ofs;

  if ( l < 0 || l >= MAX_NLIGHTS )
    return;
  memcpy ( light.ls[l].position, pos, 4*sizeof(GLfloat) );
  ofs = l*(lsbofs[6]-lsbofs[2]) + lsbofs[4];
  glBindBuffer ( GL_UNIFORM_BUFFER, lsbuf );
  glBufferSubData ( GL_UNIFORM_BUFFER, ofs, 4*sizeof(GLfloat), pos );
  ExitIfGLError ( "SetLightPosition" );
} /*SetLightPosition*/

void SetLightAttenuation ( int l, GLfloat atn[3] )
{
  GLint ofs;

  if ( l < 0 || l >= MAX_NLIGHTS )
    return;
  memcpy ( light.ls[l].attenuation, atn, 3*sizeof(GLfloat) );
  ofs = l*(lsbofs[6]-lsbofs[2]) + lsbofs[5];
  glBindBuffer ( GL_UNIFORM_BUFFER, lsbuf );
  glBufferSubData ( GL_UNIFORM_BUFFER, ofs, 3*sizeof(GLfloat), atn );
  ExitIfGLError ( "SetLightAttenuation" );
} /*SetLightAttenuation*/

void SetLightOnOff ( int l, char on )
{
  GLuint mask;

  if ( l < 0 || l >= MAX_NLIGHTS )
    return;
  mask = 0x01 << l;
  if ( on ) {
    light.mask |= mask;
    if ( l >= light.nls )
      light.nls = l+1;
  }
  else {
    light.mask &= ~mask;
    for ( mask = 0x01 << (light.nls-1); mask; mask >>= 1 ) {
      if ( light.mask & mask )
        break;
      else
        light.nls --;
    }
  }
  glBindBuffer ( GL_UNIFORM_BUFFER, lsbuf );
  glBufferSubData ( GL_UNIFORM_BUFFER, lsbofs[0], sizeof(GLuint), &light.nls );
  glBufferSubData ( GL_UNIFORM_BUFFER, lsbofs[1], sizeof(GLuint), &light.mask );
  ExitIfGLError ( "SetLightOnOff" );
} /*SetLightOnOff*/

