
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_light.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_model.h"
# include "light.h"
#include "sofloor.h"
//# include "so_capsule.h"
# include <time.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
# include "curve_eval.h"
# include "so_poly.h"
// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
	 GlShader _vertexsh, _fragsh;
	 GlProgram _prog;

    // My scene objects:
    SoAxis _axis;
	GsArray<GsVec> verts;
	int numRecursions = 0;
	bool smoothShading = false;
    SoModel _bod;
	SoModel _head;
	SoModel _rightArm;
	SoModel _rightFore;
	SoModel _leftArm;
	SoModel _leftFore;
	SoModel _rightLeg;
	SoModel _rightCalf;
	SoModel _rightFoot;
	SoModel _leftLeg;
	SoModel _leftCalf;
	SoModel _leftFoot;
	Sofloor _floor; 
	//SoModel background;
	CurveEval _ce;

	SoPoly _poly;
    // Scene data:
    bool  _viewaxis = false;
	bool uOd = false;

    GsModel _gsm;
	GsModel _gsm1;
	GsModel _gsm2;
	GsModel _gsm3;
	GsModel _gsm4;
	GsModel _gsm5;
	GsModel _gsm6;
	GsModel _gsm7;
	GsModel _gsm8;
	GsModel _gsm9;
	GsModel _gsm10;
	GsModel _gsm11;
	GsModel _gsm12;

	GsArray<GsVec> locPts;
	int counter = 0;

	light _light = light(GsVec(1, 0, 1), GsColor::white, GsColor::white, GsColor::white);
    
	double rArm = 0, rArm2 = 0, rFore = 0, rFore2 = 0, rFore3 = 0, lArm = 0, lArm2 = 0, lFore = 0, lFore2 = 0, lFore3 = 0,
		head = 0, head2 = 0, rLeg = 0, rLeg2 = 0, rCalf = 0, rFoot = 0, lLeg = 0, lLeg2 = 0, lCalf = 0, lFoot = 0, bodi = 0, bodUD = 0, bodFB = 0;
	double movements[23] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	double movement2[23] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	const int MAX_FRAMES = 60;
	const int MAX_ANIMS = 20;
	double anims[20][60][23];
	bool editMode = false;
	int nextAnim = -1;
    // App data:
    enum MenuEv { evOption0, evOption1 };
    float _rotx, _roty, _fovy;
	float side = 0, bnf = 0;
    int _w, _h;

	float xShadow, yShadow, zShadow;

	FILE *f;
	//FILE *a[10];
	std::fstream *a[15];
	int movementFrame = 0;
	int direction = 1;
	int animSelected = 0;
	float animFrames[10];
	bool upperMovement = true;
	bool moving = false;
   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    void loadModel ( int model );
    GsVec2 windowToScene ( const GsVec2& v );

	bool cameraView;
	double startTime = glutGet(GLUT_ELAPSED_TIME);
	double frameStart, frameEnd, animLength;
   private : // functions derived from the base class
	GsMat shadowMatrix();
	virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y );
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	virtual void glutIdle();
	void WriteParams();
	void ILIKETOMoveITMoveIT();
	void animStep(double frame1[], double frame2[], int direction);
 };

#endif // APP_WINDOW_H
