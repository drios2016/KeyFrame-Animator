
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   addMenuEntry ( "Option 0", evOption0 );
   addMenuEntry ( "Option 1", evOption1 );
   _viewaxis = true;
   cameraView = true;
   _fovy = GS_TORAD(60.0f);
   _rotx = _roty = 0;
   _w = w;
   _h = h;
   f = fopen("frames-robot.txt", "a");
   fprintf(f, "Next set:\n");
   fclose(f);
   startTime = glutGet(GLUT_ELAPSED_TIME);
   frameStart = glutGet(GLUT_ELAPSED_TIME);
   animLength = 500;

   _poly.init(_prog, GsColor::darkblue, GsColor::darkred);
   GsArray<GsVec> nVec;
   nVec.push(GsVec(-0.941802, 0.129904, 1.0));
   nVec.push(GsVec(-0.591784, 0.433013, 1.0));
   nVec.push(GsVec(0.0974278, 0.389711, 1.0));
   nVec.push(GsVec(0.501573, 0.163582, 1.0));
   //_poly.addToArr(nVec);
   _poly.add(GsVec(-0.941802, -0.129904, 1.0));
   _poly.add(GsVec(-0.591784, -0.433013, 1.0));
   _poly.add(GsVec(0.0974278, -0.389711, 1.0));
   _poly.add(GsVec(0.501573, -0.163582, 1.0));
   /*verts = _ce.eval_bezier(0.5, _poly.vertexarray(), numRecursions);
   _ce.setGuide(0.5, verts, smoothShading);
   _ce.build(1.0f);*/

   a[0] = new std::fstream("anim0.txt");
   a[1] = new std::fstream("anim1.txt");
   a[2] = new std::fstream("anim2.txt");
   a[3] = new std::fstream("anim3.txt");
   a[4] = new std::fstream("anim4.txt");
   a[5] = new std::fstream("anim5.txt");
   a[6] = new std::fstream("anim6.txt");
   /*a[7] = new std::fstream("anim7.txt");
   a[8] = new std::fstream("anim8.txt");
   a[9] = new std::fstream("anim9.txt");
   a[10] = new std::fstream("anim10.txt");
   a[11] = new std::fstream("anim11.txt");
   a[12] = new std::fstream("anim12.txt");
   a[13] = new std::fstream("anim13.txt");
   a[14] = new std::fstream("anim14.txt");*/
   std::string input;
	int nextFloat;
	std::stringstream ss;
	for (int m = 0; m < 7; m++) {
		for (int i = 0; i < MAX_FRAMES; i++) {
			if (std::getline(*a[m], input)) {
				ss = std::stringstream(input);
				animFrames[m] = i;
			}
			else {
				break;
			}
			std::string substr;
			std::vector<std::string> result;
			while (ss.good())
			{
				getline(ss, substr, ',');
				result.push_back(substr);
			}
			getline(ss, substr);
			result.push_back(substr);
			for (int j = 0; j < 23; j++) {
				anims[m][i][j] = stod(result[j]);
				//printf("a:\n%f\n", anims[m][i][j]);
			}
		}
	}

   //a[0] = freopen("anim0.txt", "w");
   /*for (int i = 0; i < 10; i++) {
	   char[] filename = { "a", "n", "i", "m", (char)i };
   }*/
}


void AppWindow::ILIKETOMoveITMoveIT() {
	moving = true;
	frameStart = glutGet(GLUT_ELAPSED_TIME);
	frameEnd = frameStart + animLength;

	printf("Start:\n%f\n", frameStart);
	printf("End:\n%f\n", frameEnd);
}
void AppWindow::WriteParams() {
	printf("Params out\n");
	f = fopen("frames-robot.txt", "a");
	fprintf(f, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", rArm, rArm2, rFore, rFore2, rFore3, lArm, lArm2, lFore, lFore2, lFore3, head, head2, rLeg, rLeg2, rCalf, rFoot, lLeg, lLeg2, lCalf, lFoot, bodi, bodUD, bodFB);
	//fprintf(f, "%f, %f\n", rArm, rArm2);

	fclose(f);
}

void AppWindow::initPrograms ()
 {
   _vertexsh.load_and_compile(GL_VERTEX_SHADER, "../vsh_mcol_flat.glsl");
   _fragsh.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_flat.glsl");
   _prog.init_and_link(_vertexsh, _fragsh);
   
   // Init my scene objects:
   _axis.init ();
   _ce.init(_prog);

   _bod.init();
   _head.init();
   _rightArm.init();
   _leftArm.init();
   _rightLeg.init();
   _leftLeg.init();

   _rightFore.init();
   _leftFore.init();
   _rightCalf.init();
   _rightFoot.init();
   _leftCalf.init();
   _leftFoot.init();
   _floor.init(); 

   //background.init();

   xShadow = 1.0;
   yShadow = 1.0;
   zShadow = 1.0;

   // set light:
   //_light.set ( GsVec(0,0,10), GsColor(90,90,90,255), GsColor::white, GsColor::white );

   // Load demo model:
   loadModel ( 4 );

   GsVec newpoint;
   float incr = 0.04;
   float x = 0.0, y = 0.0, z = 0.0;
   for (int i = 0; i < 30; i++) 
   {
	   x += incr;
	   if (i % 4 == 0)
		   y += incr;
	   newpoint = GsVec(x, y, z);
	   locPts.push(newpoint);
   }
 }

GsMat AppWindow::shadowMatrix() {
	float ground[4] = { 0, 0.5, 0, 0 };
	float light[4] = { xShadow, yShadow, zShadow,0 };

	float dot = ground[0] * light[0] + ground[1] * light[1] + ground[2] * light[2] + ground[3] * light[3];

	GsMat shadowMat(dot - ground[0] * light[0],
		0 - ground[1] * light[0],
		0 - ground[2] * light[0],
		0 - ground[3] * light[0],
		0 - ground[0] * light[1],
		dot - ground[1] * light[1],
		0 - ground[2] * light[1],
		0 - ground[3] * light[1],
		0 - ground[0] * light[2],
		0 - ground[1] * light[2],
		dot - ground[2] * light[2],
		0 - ground[3] * light[2],
		0 - ground[0] * light[3],
		0 - ground[1] * light[3],
		0 - ground[2] * light[3],
		dot - ground[3] * light[3]);

	return shadowMat;
}

static void printInfo ( GsModel& m )
 {
   std::cout<<"V:  "<<m.V.size()<<"\n";
   std::cout<<"F:  "<<m.F.size()<<"\n";
   std::cout<<"N:  "<<m.N.size()<<"\n";
   std::cout<<"M:  "<<m.M.size()<<"\n";
   std::cout<<"Fn: "<<m.Fn.size()<<"\n";
   std::cout<<"Fm: "<<m.Fm.size()<<"\n";
 }

void AppWindow::loadModel ( int model )
 {
   float f;
   GsString file;
   GsString file1;
   GsString file2;
   GsString file3;
   GsString file4;
   GsString file5;
   //GsString file6;
   GsString file7;
   GsString file8;
   GsString file9;
   GsString file10;
   GsString file11;
   GsString file12;
    

   switch ( model )
    { //case 1: f = 0.01f; file = "../models/porsche.obj"; break;
      //case 2: f = 0.20f; file = "../models/al.obj";	break;
      //case 3: f = 0.10f; file = "../models/f-16.obj"; break;
	  case 4: f = 0.10f; 
		  file = "../models/bod.obj"; 
		  file1 = "../models/head.obj";
		  file2 = "../models/right_arm.obj";
		  file3 = "../models/left_arm.obj";
		  file4 = "../models/right_thigh.obj";
		  file5 = "../models/left_thigh.obj";
		  //file6 = "../models/house.obj";
		  file7 = "../models/right_calf.obj";
		  file8 = "../models/left_calf.obj";
		  file9 = "../models/right_foot.obj";
		  file10 = "../models/left_foot.obj";
		  file11 = "../models/right_forearm.obj";
		  file12 = "../models/left_forearm.obj";
		  break;
      default: return;
    }
   std::cout<<"Loading "<<file<<"...\n";
   if ( !_gsm.load ( file ) ) std::cout<<"Error!\n";	//body
   printInfo ( _gsm );

   if (!_gsm1.load(file1)) std::cout << "Error!\n";		//head
	printInfo(_gsm1);

   if (!_gsm2.load(file2)) std::cout << "Error!\n";		//right arm
	printInfo(_gsm2);

   if (!_gsm3.load(file3)) std::cout << "Error!\n";		//left arm
	printInfo(_gsm3);

   if (!_gsm4.load(file4)) std::cout << "Error!\n";		//righ leg
	printInfo(_gsm4);

   if (!_gsm5.load(file5)) std::cout << "Error!\n";		//left leg
	printInfo(_gsm5);

	if (!_gsm7.load(file7)) std::cout << "Error!\n";	//right calf
	printInfo(_gsm7);

	if (!_gsm8.load(file8)) std::cout << "Error!\n";	//left calf
	printInfo(_gsm8);

	if (!_gsm9.load(file9)) std::cout << "Error!\n";	//right foot
	printInfo(_gsm9);

	if (!_gsm10.load(file10)) std::cout << "Error!\n";	//left foot
	printInfo(_gsm10);

	if (!_gsm11.load(file11)) std::cout << "Error!\n";	//right forearm
	printInfo(_gsm11);

	if (!_gsm12.load(file12)) std::cout << "Error!\n";	//left forearm
	printInfo(_gsm12);

	//if (!_gsm6.load(file6)) std::cout << "Error!\n";	//bg
	//printInfo(_gsm6);

   _gsm.scale ( f ); // to fit our camera space
   _gsm1.scale(f);
   _gsm2.scale(f);
   _gsm3.scale(f);
   _gsm4.scale(f);
   _gsm5.scale(f);
   _gsm7.scale(f);
   _gsm8.scale(f);
   _gsm9.scale(f);
   _gsm10.scale(f);
   _gsm11.scale(f);
   _gsm12.scale(f);
   //_gsm6.scale(f);

   _bod.build(_gsm);
   _head.build(_gsm1);
   _rightArm.build(_gsm2);
   _leftArm.build(_gsm3);
   _rightLeg.build(_gsm4);
   _leftLeg.build(_gsm5);

   _rightCalf.build(_gsm7);
   _leftCalf.build(_gsm8);
   _rightFoot.build(_gsm9);
   _leftFoot.build(_gsm10);
   _rightFore.build(_gsm11);
   _leftFore.build(_gsm12);
   _floor.build(); 
   //background.build(_gsm6);

   redraw();
 }

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene ( const GsVec2& v )
 {
   // GsVec2 is a lighteweight class suitable to return by value:
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
 }

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
 {
	bool rd = true;
	const char *filename = new const char();
	const float incr = GS_TORAD(2.5f);
	if (editMode) {
		if (upperMovement) {
			switch (key)
			{ //case 'p': _viewaxis = !_viewaxis; redraw(); break;
			case ' ': cameraView = !cameraView; redraw(); break;
			case 27: exit(1); // Esc was pressed
			case '`': editMode = !editMode; break;
			case '0': upperMovement = !upperMovement; break;
			case '9':
				WriteParams(); break;
				/*case 'z' : std::cout<<"Smoothing normals...\n";
						  _gsm.smooth ( GS_TORAD(35) );
						  _gsm1.smooth(GS_TORAD(35));
						  _gsm2.smooth(GS_TORAD(35));
						  _gsm3.smooth(GS_TORAD(35));
						  _gsm4.smooth(GS_TORAD(35));
						  _gsm5.smooth(GS_TORAD(35));
						  //_gsm6.smooth(GS_TORAD(35));

						   printInfo(_gsm);

						   _bod.build(_gsm);
						   _head.build(_gsm1);
						   _rightArm.build(_gsm2);
						   _leftArm.build(_gsm3);
						   _rightLeg.build(_gsm4);
						   _leftLeg.build(_gsm5);
						  // background.build(_gsm6);

						   redraw();
						   break;
				case 'x' : std::cout<<"Flat normals...\n";
						  _gsm.flat();
						  _gsm1.flat();
						  _gsm2.flat();
						  _gsm3.flat();
						  _gsm4.flat();
						  _gsm5.flat();
						  //_gsm6.flat();

						   printInfo(_gsm);

						   _bod.build(_gsm);
						   _head.build(_gsm1);
						   _rightArm.build(_gsm2);
						   _leftArm.build(_gsm3);
						   _rightLeg.build(_gsm4);
						   _leftLeg.build(_gsm5);
						   //background.build(_gsm6);

						   redraw();
						   break;
				case 'c' : if ( !_bod.phong() )
							{ std::cout<<"Switching to phong shader...\n";
							  _bod.phong(true);
							  _head.phong(true);
							  _rightArm.phong(true);
							  _leftArm.phong(true);
							  _rightLeg.phong(true);
							  _leftLeg.phong(true);
							  //background.phong(true);
							}
						   redraw();
						   break;
				case 'v' : if ( _bod.phong() )
							{ std::cout<<"Switching to gouraud shader...\n";
							  _bod.phong(false);
							  _head.phong(false);
							  _rightArm.phong(false);
							  _leftArm.phong(false);
							  _rightLeg.phong(false);
							  _leftLeg.phong(false);
							  //background.phong(false);
							}
						   redraw();
						   break;*/
			case '8': rArm = 0, rArm2 = 0, rFore = 0, rFore2 = 0, rFore3 = 0, lArm = 0, lArm2 = 0, lFore = 0, lFore2 = 0, lFore3 = 0,
				head = 0, head2 = 0, rLeg = 0, rLeg2 = 0, rCalf = 0, rFoot = 0, lLeg = 0, lLeg2 = 0, lCalf = 0, lFoot = 0,
				bodi = 0, bodUD = 0, bodFB = 0; break;
			case '7':
				movements[0] = rArm;
				movements[1] = rArm2;
				movements[2] = rFore;
				movements[3] = rFore2;
				movements[4] = rFore3;
				movements[5] = lArm;
				movements[6] = lArm2;
				movements[7] = lFore;
				movements[8] = lFore2;
				movements[9] = lFore3;
				movements[10] = head;
				movements[11] = head2;
				movements[12] = rLeg;
				movements[13] = rLeg2;
				movements[14] = rCalf;
				movements[15] = rFoot;
				movements[16] = lLeg;
				movements[17] = lLeg2;
				movements[18] = lCalf;
				movements[19] = lFoot;
				movements[20] = bodi;
				movements[21] = bodUD;
				movements[22] = bodFB; break;
			case '6':
				rArm = movements[0];
				rArm2 = movements[1];
				rFore = movements[2];
				rFore2 = movements[3];
				rFore3 = movements[4];
				lArm = movements[5];
				lArm2 = movements[6];
				lFore = movements[7];
				lFore2 = movements[8];
				lFore3 = movements[9];
				head = movements[10];
				head2 = movements[11];
				rLeg = movements[12];
				rLeg2 = movements[13];
				rCalf = movements[14];
				rFoot = movements[15];
				lLeg = movements[16];
				lLeg2 = movements[17];
				lCalf = movements[18];
				lFoot = movements[19];
				bodi = movements[20];
				bodUD = movements[21];
				bodFB = movements[22]; break;
			case '5':
				frameStart = glutGet(GLUT_ELAPSED_TIME);
				frameEnd = frameStart + animLength;
				printf("Start:\n%.f\n", frameStart);
				printf("End:\n%.f\n", frameEnd);

				movement2[0] = rArm;
				movement2[1] = rArm2;
				movement2[2] = rFore;
				movement2[3] = rFore2;
				movement2[4] = rFore3;
				movement2[5] = lArm;
				movement2[6] = lArm2;
				movement2[7] = lFore;
				movement2[8] = lFore2;
				movement2[9] = lFore3;
				movement2[10] = head;
				movement2[11] = head2;
				movement2[12] = rLeg;
				movement2[13] = rLeg2;
				movement2[14] = rCalf;
				movement2[15] = rFoot;
				movement2[16] = lLeg;
				movement2[17] = lLeg2;
				movement2[18] = lCalf;
				movement2[19] = lFoot;
				movement2[20] = bodi;
				movement2[21] = bodUD;
				movement2[22] = bodFB;
				ILIKETOMoveITMoveIT(); break;
			case '4': moving = !moving; break;

			case '.': //moving = true; 
				if (animSelected == 6) {
					//direction = 5;
					//animLength = 500.0 / direction;
					direction = 1;
					animLength = 70;
				}
				else if (animSelected == 5) {
					direction = 1;
					animLength = 100;
				}
				else {
					direction = 1;
					animLength = 500;
				}
				for (int i = 0; i < 23; i++) {
					movements[i] = anims[animSelected][0][i];
					movement2[i] = anims[animSelected][1][i];
					//printf("a:\n%f", anims[animSelected][0][i], "\n");
				}
				movementFrame = 0; direction = 1; ILIKETOMoveITMoveIT(); redraw(); break;
			case 'O':
				/*if (movementFrame > 1) {
					movementFrame -= 2;
					for (int i = 0; i < 22; i++) {
						movements[i] = anims[movementFrame][i];
						movement2[i] = anims[movementFrame + 1][i];
					}
					direction = 0; ILIKETOMoveITMoveIT(); redraw();
				}
				else*/
				if (movementFrame > 1) {
					movementFrame--;
					for (int i = 0; i < 23; i++) {
						movements[i] = anims[animSelected][movementFrame][i];
						movement2[i] = anims[animSelected][movementFrame + 1][i];
					}
					rArm = movements[0];
					rArm2 = movements[1];
					rFore = movements[2];
					rFore2 = movements[3];
					rFore3 = movements[4];
					lArm = movements[5];
					lArm2 = movements[6];
					lFore = movements[7];
					lFore2 = movements[8];
					lFore3 = movements[9];
					head = movements[10];
					head2 = movements[11];
					rLeg = movements[12];
					rLeg2 = movements[13];
					rCalf = movements[14];
					rFoot = movements[15];
					lLeg = movements[16];
					lLeg2 = movements[17];
					lCalf = movements[18];
					lFoot = movements[19];
					bodi = movements[20];
					bodUD = movements[21];
					bodFB = movements[22];
					redraw();
				}
				break;
			case 'P':
				if (movementFrame < animFrames[animSelected]) {
					movementFrame++;
					for (int i = 0; i < 23; i++) {
						movements[i] = anims[animSelected][movementFrame][i];
						movement2[i] = anims[animSelected][movementFrame + 1][i];
					}
					rArm = movements[0];
					rArm2 = movements[1];
					rFore = movements[2];
					rFore2 = movements[3];
					rFore3 = movements[4];
					lArm = movements[5];
					lArm2 = movements[6];
					lFore = movements[7];
					lFore2 = movements[8];
					lFore3 = movements[9];
					head = movements[10];
					head2 = movements[11];
					rLeg = movements[12];
					rLeg2 = movements[13];
					rCalf = movements[14];
					rFoot = movements[15];
					lLeg = movements[16];
					lLeg2 = movements[17];
					lCalf = movements[18];
					lFoot = movements[19];
					bodi = movements[20];
					bodUD = movements[21];
					bodFB = movements[22];
					direction = 0;// ILIKETOMoveITMoveIT(); 
					redraw();
				} break;
			case 'K':
				anims[animSelected][movementFrame][0] = rArm;
				anims[animSelected][movementFrame][1] = rArm2;
				anims[animSelected][movementFrame][2] = rFore;
				anims[animSelected][movementFrame][3] = rFore2;
				anims[animSelected][movementFrame][4] = rFore3;
				anims[animSelected][movementFrame][5] = lArm;
				anims[animSelected][movementFrame][6] = lArm2;
				anims[animSelected][movementFrame][7] = lFore;
				anims[animSelected][movementFrame][8] = lFore2;
				anims[animSelected][movementFrame][9] = lFore3;
				anims[animSelected][movementFrame][10] = head;
				anims[animSelected][movementFrame][11] = head2;
				anims[animSelected][movementFrame][12] = rLeg;
				anims[animSelected][movementFrame][13] = rLeg2;
				anims[animSelected][movementFrame][14] = rCalf;
				anims[animSelected][movementFrame][15] = rFoot;
				anims[animSelected][movementFrame][16] = lLeg;
				anims[animSelected][movementFrame][17] = lLeg2;
				anims[animSelected][movementFrame][18] = lCalf;
				anims[animSelected][movementFrame][19] = lFoot;
				anims[animSelected][movementFrame][20] = bodi;
				anims[animSelected][movementFrame][21] = bodUD;
				anims[animSelected][movementFrame][22] = bodFB;
				break;
			case 'L':
				printf("New file out\n");
				//filename = "new" + (const char)animFrames[0] + ".txt";
				//f = fopen(filename, "a");
				f = fopen("new0.txt", "w");
				for (int j = 0; j < animFrames[animSelected]; j++) {
					for (int i = 0; i < 23; i++) {
						fprintf(f, "%f, ", anims[animSelected][j][i]);
						//fprintf(f, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", rArm, rArm2, rFore, rFore2, rFore3, lArm, lArm2, lFore, lFore2, head, head2, bodi, bodUD, bodFB, rLeg, rLeg2, rCalf, lLeg, lLeg2, lCalf, rFoot, lFoot);
					}
					fprintf(f, "0.0\n");
				}

				fclose(f);
				break;
			case '1':
				if (animSelected > 0) {
					animSelected--;
					movementFrame = 0;
					for (int i = 0; i < 23; i++) {
						movements[i] = anims[animSelected][movementFrame][i];
						movement2[i] = anims[animSelected][movementFrame + 1][i];
					}
					rArm = movements[0];
					rArm2 = movements[1];
					rFore = movements[2];
					rFore2 = movements[3];
					rFore3 = movements[4];
					lArm = movements[5];
					lArm2 = movements[6];
					lFore = movements[7];
					lFore2 = movements[8];
					lFore3 = movements[9];
					head = movements[10];
					head2 = movements[11];
					rLeg = movements[12];
					rLeg2 = movements[13];
					rCalf = movements[14];
					rFoot = movements[15];
					lLeg = movements[16];
					lLeg2 = movements[17];
					lCalf = movements[18];
					lFoot = movements[19];
					bodi = movements[20];
					bodUD = movements[21];
					bodFB = movements[22];
					direction = 0;// ILIKETOMoveITMoveIT(); 
					redraw();
				}
				break;
			case '2':
				if (animSelected < MAX_ANIMS) {
					animSelected++;
					movementFrame = 0;
					for (int i = 0; i < 23; i++) {
						movements[i] = anims[animSelected][movementFrame][i];
						movement2[i] = anims[animSelected][movementFrame + 1][i];
					}
					rArm = movements[0];
					rArm2 = movements[1];
					rFore = movements[2];
					rFore2 = movements[3];
					rFore3 = movements[4];
					lArm = movements[5];
					lArm2 = movements[6];
					lFore = movements[7];
					lFore2 = movements[8];
					lFore3 = movements[9];
					head = movements[10];
					head2 = movements[11];
					rLeg = movements[12];
					rLeg2 = movements[13];
					rCalf = movements[14];
					rFoot = movements[15];
					lLeg = movements[16];
					lLeg2 = movements[17];
					lCalf = movements[18];
					lFoot = movements[19];
					bodi = movements[20];
					bodUD = movements[21];
					bodFB = movements[22];
					direction = 0;// ILIKETOMoveITMoveIT(); 
					redraw();
				}
				break;
			case 'q': rArm = rArm + .05; redraw(); break;
			case 'a': rArm = rArm - .05; redraw(); break;

			case 'w': rArm2 = rArm2 - .05; redraw(); break;
			case 's': rArm2 = rArm2 + .05; redraw(); break;

			case 'e': rFore = rFore - .05; redraw(); break;
			case 'd': rFore = rFore + .05; redraw(); break;

			case 'p': rFore2 = rFore2 - .05; redraw(); break;
			case ';': rFore2 = rFore2 + .05; redraw(); break;

			case '[': rFore3 = rFore3 + .0f; redraw(); break;
			case '_': rFore3 = rFore3 - .05; redraw(); break;
			case 'r': lArm = lArm + .05; redraw(); break;
			case 'f': lArm = lArm - .05; redraw(); break;

			case 't': lArm2 = lArm2 + .05; redraw(); break;
			case 'g': lArm2 = lArm2 - .05; redraw(); break;

			case 'y': lFore = lFore - .05; redraw(); break;
			case 'h': lFore = lFore + .05; redraw(); break;

			case 'm': lFore2 = lFore2 - .05; redraw(); break;
			case ',': lFore2 = lFore2 + .05; redraw(); break;

			case 'u': head = head + .05; redraw(); break;
			case 'j': head = head - .05; redraw(); break;

			case 'z': head2 = head2 - .05; redraw(); break;
			case 'x': head2 = head2 + .05; redraw(); break;
			}
		}
		else {
			switch (key)
			{ //case 'p': _viewaxis = !_viewaxis; redraw(); break;
			case ' ': cameraView = !cameraView; redraw(); break;
			case 27: exit(1); // Esc was pressed
			case '`': editMode = !editMode; break;
			case '0': upperMovement = !upperMovement; break;
			case '9':
				WriteParams(); break;
			case '8': rArm = 0, rArm2 = 0, rFore = 0, rFore2 = 0, rFore3 = 0, lArm = 0, lArm2 = 0, lFore = 0, lFore2 = 0, lFore3 = 0,
				head = 0, head2 = 0, rLeg = 0, rLeg2 = 0, rCalf = 0, rFoot = 0, lLeg = 0, lLeg2 = 0, lCalf = 0, lFoot = 0,
				bodi = 0, bodUD = 0, bodFB = 0; break;
			case '7':
				movements[0] = rArm;
				movements[1] = rArm2;
				movements[2] = rFore;
				movements[3] = rFore2;
				movements[4] = rFore3;
				movements[5] = lArm;
				movements[6] = lArm2;
				movements[7] = lFore;
				movements[8] = lFore2;
				movements[9] = lFore3;
				movements[10] = head;
				movements[11] = head2;
				movements[12] = rLeg;
				movements[13] = rLeg2;
				movements[14] = rCalf;
				movements[15] = rFoot;
				movements[16] = lLeg;
				movements[17] = lLeg2;
				movements[18] = lCalf;
				movements[19] = lFoot;
				movements[20] = bodi;
				movements[21] = bodUD;
				movements[22] = bodFB; break;
			case '6':
				rArm = movements[0];
				rArm2 = movements[1];
				rFore = movements[2];
				rFore2 = movements[3];
				rFore3 = movements[4];
				lArm = movements[5];
				lArm2 = movements[6];
				lFore = movements[7];
				lFore2 = movements[8];
				lFore3 = movements[9];
				head = movements[10];
				head2 = movements[11];
				rLeg = movements[12];
				rLeg2 = movements[13];
				rCalf = movements[14];
				rFoot = movements[15];
				lLeg = movements[16];
				lLeg2 = movements[17];
				lCalf = movements[18];
				lFoot = movements[19];
				bodi = movements[20];
				bodUD = movements[21];
				bodFB = movements[22]; break;
			case '5':
				frameStart = glutGet(GLUT_ELAPSED_TIME);
				frameEnd = animLength + frameStart;
				printf("Start:\n%f", frameStart, "\n");
				printf("End:\n%f", frameEnd, "\n");
				movement2[0] = rArm;
				movement2[1] = rArm2;
				movement2[2] = rFore;
				movement2[3] = rFore2;
				movement2[4] = rFore3;
				movement2[5] = lArm;
				movement2[6] = lArm2;
				movement2[7] = lFore;
				movement2[8] = lFore2;
				movement2[9] = lFore3;
				movement2[10] = head;
				movement2[11] = head2;
				movement2[12] = rLeg;
				movement2[13] = rLeg2;
				movement2[14] = rCalf;
				movement2[15] = rFoot;
				movement2[16] = lLeg;
				movement2[17] = lLeg2;
				movement2[18] = lCalf;
				movement2[19] = lFoot;
				movement2[20] = bodi;
				movement2[21] = bodUD;
				movement2[22] = bodFB;
				ILIKETOMoveITMoveIT(); break;

			case '4': moving = !moving; break;

			case '.': //moving = true; 
				if (animSelected == 6) {
					//direction = 5;
					//animLength = 500.0 / direction;
					direction = 1;
					animLength = 70;
				}
				else if (animSelected == 5) {
					direction = 1;
					animLength = 100;
				}
				else {
					direction = 1;
					animLength = 500;
				}
				for (int i = 0; i < 23; i++) {
					movements[i] = anims[animSelected][0][i];
					movement2[i] = anims[animSelected][1][i];
					//printf("a:\n%f", anims[0][i], "\n");
				}
				movementFrame = 0; ILIKETOMoveITMoveIT(); redraw(); break;
			case 'O':
				/*if (movementFrame > 1) {
				movementFrame -= 2;
				for (int i = 0; i < 22; i++) {
				movements[i] = anims[movementFrame][i];
				movement2[i] = anims[movementFrame + 1][i];
				}
				direction = 0; ILIKETOMoveITMoveIT(); redraw();
				}
				else*/
				if (movementFrame > 0) {
					movementFrame--;
					for (int i = 0; i < 23; i++) {
						movements[i] = anims[animSelected][movementFrame][i];
						movement2[i] = anims[animSelected][movementFrame + 1][i];
					}
					rArm = movements[0];
					rArm2 = movements[1];
					rFore = movements[2];
					rFore2 = movements[3];
					rFore3 = movements[4];
					lArm = movements[5];
					lArm2 = movements[6];
					lFore = movements[7];
					lFore2 = movements[8];
					lFore3 = movements[9];
					head = movements[10];
					head2 = movements[11];
					rLeg = movements[12];
					rLeg2 = movements[13];
					rCalf = movements[14];
					rFoot = movements[15];
					lLeg = movements[16];
					lLeg2 = movements[17];
					lCalf = movements[18];
					lFoot = movements[19];
					bodi = movements[20];
					bodUD = movements[21];
					bodFB = movements[22];
					redraw();
				}
				break;
			case 'P':
				if (movementFrame < animFrames[animSelected]) {
					movementFrame++;
					for (int i = 0; i < 23; i++) {
						movements[i] = anims[animSelected][movementFrame][i];
						movement2[i] = anims[animSelected][movementFrame + 1][i];
					}
					rArm = movements[0];
					rArm2 = movements[1];
					rFore = movements[2];
					rFore2 = movements[3];
					rFore3 = movements[4];
					lArm = movements[5];
					lArm2 = movements[6];
					lFore = movements[7];
					lFore2 = movements[8];
					lFore3 = movements[9];
					head = movements[10];
					head2 = movements[11];
					rLeg = movements[12];
					rLeg2 = movements[13];
					rCalf = movements[14];
					rFoot = movements[15];
					lLeg = movements[16];
					lLeg2 = movements[17];
					lCalf = movements[18];
					lFoot = movements[19];
					bodi = movements[20];
					bodUD = movements[21];
					bodFB = movements[22];
					direction = 0;// ILIKETOMoveITMoveIT(); 
					redraw();
				} break;
			case 'K':
				anims[animSelected][movementFrame][0] = rArm;
				anims[animSelected][movementFrame][1] = rArm2;
				anims[animSelected][movementFrame][2] = rFore;
				anims[animSelected][movementFrame][3] = rFore2;
				anims[animSelected][movementFrame][4] = rFore3;
				anims[animSelected][movementFrame][5] = lArm;
				anims[animSelected][movementFrame][6] = lArm2;
				anims[animSelected][movementFrame][7] = lFore;
				anims[animSelected][movementFrame][8] = lFore2;
				anims[animSelected][movementFrame][9] = lFore3;
				anims[animSelected][movementFrame][10] = head;
				anims[animSelected][movementFrame][11] = head2;
				anims[animSelected][movementFrame][12] = rLeg;
				anims[animSelected][movementFrame][13] = rLeg2;
				anims[animSelected][movementFrame][14] = rCalf;
				anims[animSelected][movementFrame][15] = rFoot;
				anims[animSelected][movementFrame][16] = lLeg;
				anims[animSelected][movementFrame][17] = lLeg2;
				anims[animSelected][movementFrame][18] = lCalf;
				anims[animSelected][movementFrame][19] = lFoot;
				anims[animSelected][movementFrame][20] = bodi;
				anims[animSelected][movementFrame][21] = bodUD;
				anims[animSelected][movementFrame][22] = bodFB;
				break;
			case 'L':
				printf("New file out\n");
				//filename = "new" + (const char)animFrames[0] + ".txt";
				//f = fopen(filename, "a");
				f = fopen("new0.txt", "w");
				for (int j = 0; j < animFrames[animSelected]; j++) {
					for (int i = 0; i < 23; i++) {
						fprintf(f, "%f, ", anims[animSelected][j][i]);
						//fprintf(f, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", rArm, rArm2, rFore, rFore2, rFore3, lArm, lArm2, lFore, lFore2, head, head2, bodi, bodUD, bodFB, rLeg, rLeg2, rCalf, lLeg, lLeg2, lCalf, rFoot, lFoot);
					}
					fprintf(f, "0.0\n");
				}

				fclose(f);
				break;
			case '1':
				if (animSelected > 0) {
					animSelected--;
					movementFrame = 0;
					for (int i = 0; i < 23; i++) {
						movements[i] = anims[animSelected][movementFrame][i];
						movement2[i] = anims[animSelected][movementFrame + 1][i];
					}
					rArm = movements[0];
					rArm2 = movements[1];
					rFore = movements[2];
					rFore2 = movements[3];
					rFore3 = movements[4];
					lArm = movements[5];
					lArm2 = movements[6];
					lFore = movements[7];
					lFore2 = movements[8];
					lFore3 = movements[9];
					head = movements[10];
					head2 = movements[11];
					rLeg = movements[12];
					rLeg2 = movements[13];
					rCalf = movements[14];
					rFoot = movements[15];
					lLeg = movements[16];
					lLeg2 = movements[17];
					lCalf = movements[18];
					lFoot = movements[19];
					bodi = movements[20];
					bodUD = movements[21];
					bodFB = movements[22];
					direction = 0;// ILIKETOMoveITMoveIT(); 
					redraw();
				}
				break;
			case '2':
				if (animSelected < MAX_ANIMS) {
					animSelected++;
					movementFrame = 0;
					for (int i = 0; i < 23; i++) {
						movements[i] = anims[animSelected][movementFrame][i];
						movement2[i] = anims[animSelected][movementFrame + 1][i];
					}
					rArm = movements[0];
					rArm2 = movements[1];
					rFore = movements[2];
					rFore2 = movements[3];
					rFore3 = movements[4];
					lArm = movements[5];
					lArm2 = movements[6];
					lFore = movements[7];
					lFore2 = movements[8];
					lFore3 = movements[9];
					head = movements[10];
					head2 = movements[11];
					rLeg = movements[12];
					rLeg2 = movements[13];
					rCalf = movements[14];
					rFoot = movements[15];
					lLeg = movements[16];
					lLeg2 = movements[17];
					lCalf = movements[18];
					lFoot = movements[19];
					bodi = movements[20];
					bodUD = movements[21];
					bodFB = movements[22];
					direction = 0;// ILIKETOMoveITMoveIT(); 
					redraw();
				}
				break;
			case 'c': bodi = bodi + 0.05; redraw(); break;
			case 'v': bodi = bodi - 0.05; redraw(); break;

			case 'b': bodUD = bodUD + 0.01; redraw(); break;
			case 'n': bodUD = bodUD - 0.01; redraw(); break;

			case 'm': bodFB = bodFB + 0.05; redraw(); break;
			case ',': bodFB = bodFB - 0.05; redraw(); break;

			case 'q': rLeg = rLeg + .05; redraw(); break;
			case 'a': rLeg = rLeg - .05; redraw(); break;

			case 'w': rLeg2 = rLeg2 + .05; redraw(); break;
			case 's': rLeg2 = rLeg2 - .05; redraw(); break;

			case 'e': rCalf = rCalf + .05; redraw(); break;
			case 'd': rCalf = rCalf - .05; redraw(); break;

			case 'r': lLeg = lLeg + .05; redraw(); break;
			case 'f': lLeg = lLeg - .05; redraw(); break;

			case 't': lLeg2 = lLeg2 + .05; redraw(); break;
			case 'g': lLeg2 = lLeg2 - .05; redraw(); break;

			case 'y': lCalf = lCalf + .05; redraw(); break;
			case 'h': lCalf = lCalf - .05; redraw(); break;

			case 'u': rFoot = rFoot + .05; redraw(); break;
			case 'j': rFoot = rFoot - .05; redraw(); break;

			case 'i': lFoot = lFoot + .05; redraw(); break;
			case 'k': lFoot = lFoot - .05; redraw(); break;

				//case '1': xShadow += .01; redraw(); break;
				//case '2': xShadow -= .01; redraw(); break;
				//case '3': yShadow += .01; redraw(); break;
				//case '4': yShadow -= .01; redraw(); break;
				//case '5': zShadow += .01; redraw(); break;
				//case '6': zShadow -= .01; redraw(); break;

			case 'p': _rotx -= incr; redraw(); break;
			case '[': _rotx += incr; redraw(); break;
			case ']': _roty -= incr; redraw(); break;
			case '|': _roty += incr; redraw(); break;

			//default: loadModel(int(key - '0'));
				//break;
			}
		}
	}
 else {
	 switch (key) {
	 case ' ': cameraView = !cameraView; redraw(); break;
	 case 27: exit(1); // Esc was pressed
	 case '`': editMode = !editMode; break;
	 case '1':
		 if (moving) {
			 nextAnim = 0;
		 }
		 else {
			 animSelected = 6;
			 direction = 1;
			 animLength = 70;
			 for (int i = 0; i < 23; i++) {
				 movements[i] = anims[animSelected][0][i];
				 movement2[i] = anims[animSelected][1][i];
				 //printf("a:\n%f", anims[animSelected][0][i], "\n");
			 }
			 movementFrame = 0; direction = 1; ILIKETOMoveITMoveIT(); redraw();
		 }
		 break;
	 case '2':
		 if (moving) {
			 nextAnim = 1;
		 }
		 else {
			 animSelected = 5;
			 direction = 1;
			 animLength = 100;
			 for (int i = 0; i < 23; i++) {
				 movements[i] = anims[animSelected][0][i];
				 movement2[i] = anims[animSelected][1][i];
				 //printf("a:\n%f", anims[animSelected][0][i], "\n");
			 }
			 movementFrame = 0; direction = 1; ILIKETOMoveITMoveIT(); redraw();
		 }
		 break;
	 case '3':
		 if (moving) {
			 nextAnim = 4;
		 }
		 else {
			 animSelected = 4;
			 direction = 1;
			 animLength = 500;
			 for (int i = 0; i < 23; i++) {
				 movements[i] = anims[animSelected][0][i];
				 movement2[i] = anims[animSelected][1][i];
				 //printf("a:\n%f", anims[animSelected][0][i], "\n");
			 }
			 movementFrame = 0; direction = 1; ILIKETOMoveITMoveIT(); redraw();
		 }
		 break;
	 case '4':
		 if (moving) {
			 nextAnim = 3;
		 }
		 else {
			 animSelected = 3;
			 direction = 1;
			 animLength = 500;
			 for (int i = 0; i < 23; i++) {
				 movements[i] = anims[animSelected][0][i];
				 movement2[i] = anims[animSelected][1][i];
				 //printf("a:\n%f", anims[animSelected][0][i], "\n");
			 }
			 movementFrame = 0; direction = 1; ILIKETOMoveITMoveIT(); redraw();
		 }
		 break;
	 case '5':
		 if (moving) {
			 nextAnim = 0;
		 }
		 else {
			 animSelected = 0;
			 direction = 1;
			 animLength = 500;
			 for (int i = 0; i < 23; i++) {
				 movements[i] = anims[animSelected][0][i];
				 movement2[i] = anims[animSelected][1][i];
				 //printf("a:\n%f", anims[animSelected][0][i], "\n");
			 }
			 movementFrame = 0; direction = 1; ILIKETOMoveITMoveIT(); redraw();
		 }
		 break;
	 case '6':
		 if (moving) {
			 nextAnim = 4;
		 }
		 else {
			 animSelected = 4;
			 direction = 1;
			 animLength = 500;
			 for (int i = 0; i < 23; i++) {
				 movements[i] = anims[animSelected][0][i];
				 movement2[i] = anims[animSelected][1][i];
				 //printf("a:\n%f", anims[animSelected][0][i], "\n");
			 }
			 movementFrame = 0; direction = 1; ILIKETOMoveITMoveIT(); redraw();
		 }
		 break;
	 case '7':
		 if (moving) {
			 nextAnim = 2;
		 }
		 else {
			 animSelected = 2;
			 direction = 1;
			 animLength = 500;
			 for (int i = 0; i < 23; i++) {
				 movements[i] = anims[animSelected][0][i];
				 movement2[i] = anims[animSelected][1][i];
				 //printf("a:\n%f", anims[animSelected][0][i], "\n");
			 }
			 movementFrame = 0; direction = 1; ILIKETOMoveITMoveIT(); redraw();
		 }
		 break;
	 case '8':
		 if (moving) {
			 nextAnim = 4;
		 }
		 else {
			 animSelected = 4;
			 direction = 1;
			 animLength = 500;
			 for (int i = 0; i < 23; i++) {
				 movements[i] = anims[animSelected][0][i];
				 movement2[i] = anims[animSelected][1][i];
				 //printf("a:\n%f", anims[animSelected][0][i], "\n");
			 }
			 movementFrame = 0; direction = 1; ILIKETOMoveITMoveIT(); redraw();
		 }
		 break;
	 case '9':
		 if (moving) {
			 nextAnim = 2;
		 }
		 else {
			 animSelected = 2;
			 direction = 1;
			 animLength = 500;
			 for (int i = 0; i < 23; i++) {
				 movements[i] = anims[animSelected][0][i];
				 movement2[i] = anims[animSelected][1][i];
				 //printf("a:\n%f", anims[animSelected][0][i], "\n");
			 }
			 movementFrame = 0; direction = 1; ILIKETOMoveITMoveIT(); redraw();
		 }
		 break;
	 case '0':
		 verts = _ce.eval_bezier(0.5f, _poly.vertexarray(), numRecursions);
		 _ce.setGuide(0.5f, verts, smoothShading);
		 _ce.build(1.0f); redraw();
		 break;
	 }
 }

   if (rd) redraw();
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd = true;
   const float incf = 0.01f;
   const float incr = GS_TORAD(2.5f);
   switch ( key )
    { case GLUT_KEY_LEFT:      side -= incf; break;
      case GLUT_KEY_RIGHT:     side += incf; break;
      case GLUT_KEY_UP:        bnf -= incf; break;
      case GLUT_KEY_DOWN:      bnf += incf; break;
      case GLUT_KEY_PAGE_UP:   _fovy -= incf; break;
      case GLUT_KEY_PAGE_DOWN: _fovy += incf; break;
      default: return; // return without rendering
	}
   if (rd) redraw(); // ask the window to be rendered when possible
 }

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   // The mouse is not used in this example.
   // Recall that a mouse click in the screen corresponds
   // to a whole line traversing the 3D scene.
 }

void AppWindow::glutMotion ( int x, int y )
 {
 }

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w = w; _h = h;
 }

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Build a cross with some lines (if not built yet):
   if ( _axis.changed ) // needs update
    { 
		_axis.build(1.0f); // axis has radius 1.0
    }

   // Define our scene transformation:
   GsMat rx, ry, stransf, 
	   rotHeadside, rotHeadup, 
	   rotRarm, rotLarm, rotRarm2, rotLarm2, rotRfore, rotLfore, rotRfore2, rotLfore2, rotRfore3, rotLfore3,
	   rotRleg, rotLleg, rotRleg2, rotLleg2, rotRcalf, rotLcalf, rotRfoot, rotLfoot,
	   rotBod, rotBod2,
	   rArmTrans, rForeTrans, lArmTrans, lForeTrans,
	   rLegTrans, lLegTrans, rCalfTrans, lCalfTrans, rFootTrans, lFootTrans,
	   hTrans, 
	   bTrans,
	   negrArmTrans, neglArmTrans, negrForeTrans, neglForeTrans,
	   negrLegTrans, neglLegTrans, negrCalfTrans, neglCalfTrans, negrFootTrans, neglFootTrans,
	   neghTrans, 
	   negbTrans, 
	   houseTrans,
	   moved;

   //x = red, y = green, z = blue
   rArmTrans.translation(0.02, -0.282, 0.01);
   negrArmTrans.translation(-0.02, 0.282, -0.01);
   rForeTrans.translation(0.028, -0.240, 0.019);
   negrForeTrans.translation(-0.028, 0.240, -0.019);

   lArmTrans.translation(-0.02, -0.282, 0.01);
   neglArmTrans.translation(0.02, 0.282, -0.01);
   lForeTrans.translation(-0.028, -0.240, 0.019);
   neglForeTrans.translation(0.028, 0.240, -0.019);

   rLegTrans.translation( 0.013, -0.202, 0);
   negrLegTrans.translation(-0.013, 0.202, 0);
   rCalfTrans.translation(0.009, -0.137, 0);
   negrCalfTrans.translation(-0.009, 0.137, 0);

   rFootTrans.translation(-0.006, -0.0415, 0.008);
   negrFootTrans.translation(0.006, 0.0415, -0.008);

   lLegTrans.translation(-0.013, -0.202, 0);
   neglLegTrans.translation(0.013, 0.202, 0);
   lCalfTrans.translation(-0.009, -0.137, 0);
   neglCalfTrans.translation(0.009, 0.137, 0);

   lFootTrans.translation(-0.006, -0.0415, -0.008);
   neglFootTrans.translation(0.006, 0.0415, 0.008);

   hTrans.translation(0, -0.32, 0);
   neghTrans.translation(0, 0.32, 0);

   bTrans.translation(0, -0.203, 0);
   negbTrans.translation(0, 0.203, 0);

   moved.translation(side, bodUD, bnf);

   //houseTrans.translation(0,0,-1);

   rx.rotx ( _rotx );
   ry.roty ( _roty );

   rotHeadside.roty(head);
   rotHeadup.rotx(head2);

   rotRarm.rotx(rArm);
   rotRarm2.rotz(rArm2);
   rotRfore.rotx(rFore);
   rotRfore2.rotz(rFore2);
   rotRfore3.roty(rFore3);

   rotLarm.rotx(lArm);
   rotLarm2.rotz(lArm2);
   rotLfore.rotx(lFore);
   rotLfore2.rotz(lFore2);

   rotRleg.rotx(rLeg);
   rotLleg.rotx(lLeg);
   rotRleg2.rotz(rLeg2);
   rotLleg2.rotz(lLeg2);
   rotRcalf.rotx(rCalf);
   rotLcalf.rotx(lCalf);

   rotRfoot.rotx(rFoot);
   rotLfoot.rotx(lFoot);

   rotBod.roty(bodi);
   rotBod2.rotx(bodFB);

   stransf = rx*ry; // set the scene transformation matrix

   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, persp, sproj;

   GsVec eye(0, 0, 2), center(0, 0, 0), up(0, 1, 0);
   camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix

   float aspect = 1.0f, znear = 0.1f, zfar = 50.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix
   //sproj = persp * camview; // set final scene projection

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
  // if (cameraView)
   {
	//	sproj = persp * camview; // set final scene projection
   }
	
  // else
   {
	   GsMat rotation, trans;
	   trans.translation(-side, 0, -bnf);
	   rotation.roty(-bodi);
	   sproj = persp * camview * rotation * trans;
   }

   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause our values to be transposed, and we will then have shin our 
   //  shaders vectors on the left side of a multiplication to a matrix.

   // Draw:
   if ( _viewaxis ) _axis.draw ( stransf, sproj );
   _floor.draw(stransf, sproj,_light,mat(GsVec(1,0,1),GsColor::black,GsColor::darkblue,GsColor::blue,float(.6)));
   GsMat bodBend = negbTrans*rotBod*rotBod2*bTrans;
   GsMat rightArmRaise = negrArmTrans*rotRarm*rotRarm2*rArmTrans;
   GsMat leftArmRaise = neglArmTrans*rotLarm*rotLarm2*lArmTrans;
   GsMat rightLegRaise = negrLegTrans*rotRleg*rotRleg2*rLegTrans;
   GsMat leftLegRaise = neglLegTrans*rotLleg*rotLleg2*lLegTrans;
   GsMat rightCalfRaise = negrCalfTrans*rotRcalf*rCalfTrans;
   GsMat leftCalfRaise = neglCalfTrans*rotLcalf*lCalfTrans;
   
   _bod.draw(stransf*moved*bodBend, sproj, _light, 3);
   _head.draw(stransf*moved*bodBend*neghTrans*rotHeadside*rotHeadup*hTrans, sproj, _light, 0);

   _rightArm.draw(stransf*moved*bodBend*rightArmRaise, sproj, _light, 0);
   _rightFore.draw(stransf*moved*bodBend*rightArmRaise*negrForeTrans*rotRfore*rotRfore2*rotRfore3*rForeTrans, sproj, _light, 0);

   _leftArm.draw(stransf*moved*bodBend*leftArmRaise, sproj, _light, 0);
   _leftFore.draw(stransf*moved*bodBend*leftArmRaise*neglForeTrans*rotLfore*rotLfore2*lForeTrans, sproj, _light, 0);
   
   _rightLeg.draw(stransf*moved*rotBod*rightLegRaise, sproj, _light, 0);
   _rightCalf.draw(stransf*moved*rotBod*rightLegRaise*rightCalfRaise, sproj, _light, 0);
   _rightFoot.draw(stransf*moved*rotBod*rightLegRaise*rightCalfRaise*negrFootTrans*rotRfoot*rFootTrans, sproj, _light, 3);

   _leftLeg.draw(stransf*moved*rotBod*leftLegRaise, sproj, _light, 0);
   _leftCalf.draw(stransf*moved*rotBod*leftLegRaise*leftCalfRaise, sproj, _light, 0);
   _leftFoot.draw(stransf*moved*rotBod*leftLegRaise*leftCalfRaise*neglFootTrans*rotLfoot*lFootTrans, sproj, _light, 3);

   /***********************SHADOWS*********************/
   _bod.draw(stransf*shadowMatrix()*moved*bodBend, sproj, _light, 2);
   _head.draw(stransf*shadowMatrix()*moved*bodBend*neghTrans*rotHeadside*rotHeadup*hTrans, sproj, _light, 2);

   _rightArm.draw(stransf*shadowMatrix()*moved*bodBend*rightArmRaise, sproj, _light, 2);
   _rightFore.draw(stransf*shadowMatrix()*moved*bodBend*rightArmRaise*negrForeTrans*rotRfore*rotRfore2*rForeTrans, sproj, _light, 2);

   _leftArm.draw(stransf*shadowMatrix()*moved*bodBend*leftArmRaise, sproj, _light, 2);
   _leftFore.draw(stransf*shadowMatrix()*moved*bodBend*leftArmRaise*neglForeTrans*rotLfore*rotLfore2*lForeTrans, sproj, _light, 2);

   _rightLeg.draw(stransf*shadowMatrix()*moved*rotBod*rightLegRaise, sproj, _light, 2);
   _rightCalf.draw(stransf*shadowMatrix()*moved*rotBod*rightLegRaise*rightCalfRaise, sproj, _light, 2);
   _rightFoot.draw(stransf*shadowMatrix()*moved*rotBod*rightLegRaise*rightCalfRaise*negrFootTrans*rotRfoot*rFootTrans, sproj, _light, 2);

   _leftLeg.draw(stransf*shadowMatrix()*moved*rotBod*leftLegRaise, sproj, _light, 2);
   _leftCalf.draw(stransf*shadowMatrix()*moved*rotBod*leftLegRaise*leftCalfRaise, sproj, _light, 2);
   _leftFoot.draw(stransf*shadowMatrix()*moved*rotBod*leftLegRaise*leftCalfRaise*neglFootTrans*rotLfoot*lFootTrans, sproj, _light, 2);

   //background.draw(stransf*houseTrans, sproj, _light, 4);

   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

void AppWindow::animStep(double frame1[], double frame2[], int direction) {

	//printf("dir:\n%f", direction, "\n");
	//moving = false;
		float deltaTime = glutGet(GLUT_ELAPSED_TIME) - frameStart;
		int totalTime = frameEnd - frameStart;
		double move[23] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		if (deltaTime / totalTime > 0.9) {
			if (direction == 0) {
				/*for (int i = 0; i < 23; i++) {
					double next = frame1[i];
					frame1[i] = frame2[i];
					frame2[i] = next;
				}
				frameStart = glutGet(GLUT_ELAPSED_TIME);
				frameEnd = frameStart + animLength;*/
				movementFrame++;
				moving = false;
			}
			else if (direction > 0) {
				//printf("next:\n%f\n frames:%f\n", frameStart, animFrames[animSelected]);
				if (movementFrame < animFrames[animSelected] - 1) {
					movementFrame++;
					frameStart = glutGet(GLUT_ELAPSED_TIME);
					frameEnd = frameStart + animLength;
					for (int i = 0; i < 23; i++) {
						movements[i] = movement2[i];
						movement2[i] = anims[animSelected][movementFrame][i];
					}
				}
				else {
					if (nextAnim > -1) {
						animSelected = nextAnim;
						nextAnim = -1;
						direction = 1;
						animLength = 500;
						for (int i = 0; i < 23; i++) {
							movements[i] = anims[animSelected][0][i];
							movement2[i] = anims[animSelected][1][i];
							//printf("a:\n%f", anims[animSelected][0][i], "\n");
						}
						movementFrame = 0; direction = 1; ILIKETOMoveITMoveIT(); redraw();
					}
					else {
						moving = false;
					}
				}
			}
		}
		for (int i = 0; i < 23; i++) {
			move[i] = frame1[i] + ((direction * deltaTime / (float)totalTime) * (frame2[i] - frame1[i]));
		}

		rArm = move[0];
		rArm2 = move[1];
		rFore = move[2];
		rFore2 = move[3];
		rFore3 = move[4];
		lArm = move[5];
		lArm2 = move[6];
		lFore = move[7];
		lFore2 = move[8];
		lFore3 = move[9];
		head = move[10];
		head2 = move[11];
		rLeg = move[12];
		rLeg2 = move[13];
		rCalf = move[14];
		rFoot = move[15];
		lLeg = move[16];
		lLeg2 = move[17];
		lCalf = move[18];
		lFoot = move[19];
		bodi = move[20];
		bodUD = move[21];
		bodFB = move[22];


}

void AppWindow::glutIdle() {
	//printf(".");

	if (moving) {// && frameEnd < glutGet(GLUT_ELAPSED_TIME)) {
		animStep(movements, movement2, direction);
	}


	/*if (moving) {// && frameEnd < glutGet(GLUT_ELAPSED_TIME)) {
		//printf("\nStart\n");
		float deltaTime = glutGet(GLUT_ELAPSED_TIME) - frameStart;
		int totalTime = frameEnd - frameStart;
		double move[23] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		if (deltaTime / totalTime > 0.9) {
			for (int i = 0; i < 23; i++) {
				double next = movements[i];
				movements[i] = movement2[i];
				movement2[i] = next;
			}
			frameStart = glutGet(GLUT_ELAPSED_TIME);
			frameEnd = frameStart + animLength;
		}
		for (int i = 0; i < 23; i++) {
			move[i] = movements[i] + ((deltaTime / (float)totalTime) * (movement2[i] - movements[i]));
		}

		rArm = move[0];
		rArm2 = move[1];
		rFore = move[2];
		rFore2 = move[3];
		rFore3 = move[4];
		lArm = move[5];
		lArm2 = move[6];
		lFore = move[7];
		lFore2 = move[8];
		lFore3 = move[9];
		head = move[10];
		head2 = move[11];
		rLeg = move[12];
		rLeg2 = move[13];
		rCalf = move[14];
		rFoot = move[15];
		lLeg = move[16];
		lLeg2 = move[17];
		lCalf = move[18];
		lFoot = move[19];
		bodi = move[20];
		bodUD = move[21];
		bodFB = move[22];
	} else {
		//moving = false;
		/*rArm = movements[0];
		rArm2 = movements[1];
		rFore = movements[2];
		rFore2 = movements[3];
		rFore3 = movements[4];
		lArm = movements[5];
		lArm2 = movements[6];
		lFore = movements[7];
		lFore2 = movements[8];
		lFore3 = movements[9];
		head = movements[10];
		head2 = movements[11];
		rLeg = movements[12];
		rLeg2 = movements[13];
		rCalf = movements[14];
		rFoot = movements[15];
		lLeg = movements[16];
		lLeg2 = movements[17];
		lCalf = movements[18];
		lFoot = movements[19];
		bodi = movements[20];
		bodUD = movements[21];
		bodFB = movements[22];*/
	//}
	redraw();
}

