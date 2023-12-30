
#define STATE_NOTHING 0
#define STATE_TURNING 1

typedef struct TransBl {
          GLfloat mm[16], vm[16], pm[16];
          GLfloat eyepos[4];
        } TransBl;

extern GLFWwindow* window;

extern GLuint  shader_id[5];
extern GLuint  program_id[2];
extern GLuint  trbi, trbuf, trbbp;
extern GLint   trbsize, trbofs[5];
extern float   model_rot_axis[3], model_rot_angle0, model_rot_angle;
extern const float viewer_pos0[4];
extern TransBl trans;

extern int   WindowHandle;
extern int   win_width, win_height;
extern int   last_xi, last_eta;
extern float left, right, bottom, top, near, far;
extern int   app_state;
extern float viewer_rvec[3];
extern float viewer_rangle;
extern int   option;
extern char  animate, enlight;

void SetupMVPMatrix ( void );
void SetupModelMatrix ( float axis[3], float angle );
void InitViewMatrix ( void );
void RotateViewer ( int deltaxi, int deltaeta );
void InitLights ( void );

extern GLuint sphere_vao, sphere_vbo[3];
void ConstructSphereVAO(void);
void DrawSphere(int opt, char enlight);

void LoadMyShaders ( void );
void InitMyObject ( void );
void Cleanup ( void );

void ToggleAnimation ( void );
void ToggleLight ( void );

void ReshapeFunc (GLFWwindow* window, int width, int height );
void DisplayFunc ( void );
void KeyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseFunc(GLFWwindow* window, int button, int state, int mods);
void MotionFunc ( int x, int y );
void IdleFunc ( void );

void Initialise ( int argc, char *argv[] );

