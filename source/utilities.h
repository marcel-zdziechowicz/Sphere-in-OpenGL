
#ifndef true
#define true  1
#endif
#ifndef false
#define false 0
#endif

#ifndef PI
#define PI 3.1415926535897932384
#endif

#define MAX_UNIFORM_FIELDS 32

#define ROUNDUP(a,b) ((((a)+(b)-1)/(b))*(b))

#ifdef ENABLE_SPIRV
typedef void (*PFNGLSPECIALIZESHADERARB) (GLuint shader, const GLchar* pEntryPoint,
                                          GLuint numSpecializationConstants,
                                          const GLuint* pConstantIndex,
                                          const GLuint* pConstantValue);
extern PFNGLSPECIALIZESHADERARB glSpecializeShaderARB;
#endif
/* ////////////////////////////////////////////////////////////////////////// */
void PrintGLVersion ( void );
void ExitOnError ( const char *msg );
void ExitIfGLError ( const char *msg );
void GetGLProcAddresses ( void );
char IsGLExtensionPresent ( const char *name );

GLuint CompileShaderStrings ( GLenum shader_type, int nsl,
                              const GLchar **srclines );
GLuint CompileShaderFiles ( GLenum shader_type, int nfiles,
                            const char **filenames );
GLuint LinkShaderProgram ( int nsh, const GLuint *shaders );

#ifdef ENABLE_SPIRV
GLuint CreateSPIRVShader ( GLenum shader_type, int size, const GLuint *spirv );
GLuint LoadSPIRVFile ( GLenum shader_type, const char *filename );
#endif

GLuint NewUniformBindingPoint ( void );
void GetAccessToUniformBlock ( GLuint prog, int n, const GLchar **names,
                               GLuint *ind, GLint *size, GLint *ofs,
                               GLuint *bpoint );
GLuint NewUniformBlockObject ( GLint size, GLuint bp );
void AttachUniformBlockToBP ( GLuint prog, const GLchar *name, GLuint bp );

void GetAccessToStorageBlock ( GLuint prog, int n, const GLchar **names,
                               GLuint *ind, GLint *size, GLint *ofs,
                               GLuint *bpoint );

/* ////////////////////////////////////////////////////////////////////////// */
extern double app_time0, app_time, toc_time;

void TimerInit ( void );
double TimerTic ( void );
double TimerToc ( void );
double TimerTocTic ( void );

/* ////////////////////////////////////////////////////////////////////////// */
void QuickSort ( void *data, int n,
                 char (*Less)(void *data, int i, int j),
                 void (*Swap)(void *data, int i, int j) );

/* ////////////////////////////////////////////////////////////////////////// */
#define M4x4Copyf(a,b) memcpy ( a, b, 16*sizeof(GLfloat) )

void M4x4Identf ( GLfloat a[16] );
void M4x4Translatef ( GLfloat a[16], float x, float y, float z );
void M4x4Scalef ( GLfloat a[16], float sx, float sy, float sz );
void M4x4RotateXf ( GLfloat a[16], float phi );
void M4x4RotateYf ( GLfloat a[16], float phi );
void M4x4RotateZf ( GLfloat a[16], float phi );
void M4x4RotateVf ( GLfloat a[16], float x, float y, float z, float phi );
void M4x4MTranslatef ( GLfloat a[16], float x, float y, float z );
void M4x4MScalef ( GLfloat a[16], float sx, float sy, float sz );
void M4x4MRotateXf ( GLfloat a[16], float phi );
void M4x4MRotateYf ( GLfloat a[16], float phi );
void M4x4MRotateZf ( GLfloat a[16], float phi );
void M4x4MRotateVf ( GLfloat a[16], float x, float y, float z, float phi );
void M4x4Multf ( GLfloat ab[16], const GLfloat a[16], const GLfloat b[16] );
char M4x4LUDecompf ( GLfloat lu[16], int p[3], const GLfloat a[16] );
void M4x4LUsolvef ( GLfloat aiv[4], const GLfloat lu[16],
                    const int p[3], const GLfloat v[4] );
char M4x4Invertf ( GLfloat ai[16], const GLfloat a[16] );
void M4x4InvertAffineIsometryf ( GLfloat ai[16], const GLfloat a[16] );
void M4x4UTLTSolvef ( GLfloat ativ[4], const GLfloat lu[16],
                      const int p[3], const GLfloat v[4] );
char M4x4TInvertf ( GLfloat ati[16], const GLfloat a[16] );
void M4x4Transposef ( GLfloat at[16], const GLfloat a[16] );
void M4x4MultMVf ( GLfloat av[4], const GLfloat a[16], const GLfloat v[4] );
void M4x4MultMTVf ( GLfloat av[4], const GLfloat a[16], const GLfloat v[4] );

/* ////////////////////////////////////////////////////////////////////////// */
void M4x4Frustumf ( GLfloat a[16], GLfloat ai[16],
                    float left, float right, float bottom,
                    float top, float near, float far );
void M4x4Orthof ( GLfloat a[16], GLfloat ai[16],
                  float left, float right, float bottom,
                  float top, float near, float far );

void M4x4SkewFrustumf ( int w, int h, float aspect, float F, float dist,
                        float xv, float yv, float xp, float yp,
                        float near, float far,
                        float *left, float *right, float *bottom, float *top,
                        const GLfloat vm[16],
                        GLfloat shvm[16], GLfloat eyepos[4],
                        GLfloat pm[16], GLfloat pmi[16] );

/* ////////////////////////////////////////////////////////////////////////// */
void V4Addf ( float v[4], const float v1[4], const float v2[4] );
void V4Subtractf ( float v[4], const float v1[4], const float v2[4] );
float V4DotProductf ( const float v1[4], const float v2[4] );
void V4CrossProductf ( float v[4], const float v1[4], const float v2[4],
                       const float v3[4] );
float V4Normalisef ( float v[4] );

void V3Addf ( float v[3], const float v1[3], const float v2[3] );
void V3Subtractf ( float v[3], const float v1[3], const float v2[3] );
float V3DotProductf ( const float v1[3], const float v2[3] );
void V3CrossProductf ( float v1xv2[3], const float v1[3], const float v2[3] );
void V3CompRotationsf ( float v[3], float *phi,
                  const float v2[3], float phi2, const float v1[3], float phi1 );
float V3Normalisef ( float v[3] );

/* ////////////////////////////////////////////////////////////////////////// */
void M4x4MultMV3f ( GLfloat av[3], const GLfloat a[16], const GLfloat v[3] );
void M4x4MultMP3f ( GLfloat ap[3], const GLfloat a[16], const GLfloat p[3] );
void V3ReflectPointf ( float r[3],
                       const float p[3], const float nv[3], const float q[3] );

void M4x4RotatePVf ( GLfloat a[16],
                     const float p[3], const float v[3], float phi );
char M4x4RotateP2Vf ( GLfloat a[16], const float p[3],
                      const float v1[3], const float v2[3] );

