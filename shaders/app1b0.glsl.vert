#version 420 

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Colour;

out Vertex {
  vec4 Colour;
} Out;

uniform TransBlock {
  mat4 mm, vm, pm, mvpm;
} trb;

void main ( void )
{
  gl_Position = trb.mvpm * in_Position;
  Out.Colour = in_Colour;
} /*main*/
