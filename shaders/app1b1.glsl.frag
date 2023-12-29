#version 420

#define MAX_NLIGHTS 8

in NVertex {
  vec4 Colour;
  vec3 Position;
  vec3 Normal;
} In;

out vec4 out_Colour;

uniform TransBlock {
  mat4 mm, vm, pm, mvpm;
  vec4 eyepos;
} trb;

struct LSPar {
    vec4 ambient;
    vec4 direct;
    vec4 position;
    vec3 attenuation;
  };

uniform LSBlock {
  uint  nls;              /* liczba zrodel swiatla */
  uint  mask;             /* maska wlaczonych zrodel */
  LSPar ls[MAX_NLIGHTS];  /* poszczegolne zrodla swiatla */
} light;

vec3 posDifference ( vec4 p, vec3 pos, out float dist )
{
  vec3 v;

  if ( p.w != 0.0 ) {
    v = p.xyz/p.w-pos.xyz;
    dist = sqrt ( dot ( v, v ) );
  }
  else
    v = p.xyz;
  return normalize ( v );
} /*posDifference*/

float attFactor ( vec3 att, float dist )
{
  return 1.0/(((att.z*dist)+att.y)*dist+att.x);
} /*attFactor*/

void main ( void )
{
  vec3  normal, lv, vv;
  float d, dist;
  uint  i, mask;

  normal = normalize ( In.Normal );
  vv = posDifference ( trb.eyepos, In.Position, dist );

  out_Colour = vec4(0.0);
  for ( i = 0, mask = 0x00000001;  i < light.nls;  i++, mask <<= 1 )
    if ( (light.mask & mask) != 0 ) {
      out_Colour += light.ls[i].ambient * In.Colour;
      lv = posDifference ( light.ls[i].position, In.Position, dist );
      d = dot ( lv, normal ); // <li;n>
      if ( dot ( vv, normal ) > 0.0 ) {
        if ( d > 0.0 ) {
          if ( light.ls[i].position.w != 0.0 )
            d *= attFactor ( light.ls[i].attenuation, dist );
          out_Colour += (d * light.ls[i].direct) * In.Colour;
        }
      }
      else {
        if ( d < 0.0 ) {
          if ( light.ls[i].position.w != 0.0 )
            d *= attFactor ( light.ls[i].attenuation, dist );
          out_Colour -= (d * light.ls[i].direct) * In.Colour;
        }
      }
    }
} /*main*/
