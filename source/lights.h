
#define MAX_NLIGHTS 8

typedef struct LSPar {
          GLfloat ambient[4];
          GLfloat diffuse[4];
          GLfloat position[4];
          GLfloat attenuation[3];
        } LSPar;

typedef struct LightBl {
          GLuint nls, mask;
          LSPar  ls[MAX_NLIGHTS];
        } LightBl;

extern GLuint  lsbi, lsbuf, lsbbp;
extern GLint   lsbsize, lsbofs[7];
extern LightBl light;

void SetLightAmbient ( int l, GLfloat amb[4] );
void SetLightDiffuse ( int l, GLfloat dif[4] );
void SetLightPosition ( int l, GLfloat pos[4] );
void SetLightAttenuation ( int l, GLfloat at3[3] );
void SetLightOnOff ( int l, char on );
