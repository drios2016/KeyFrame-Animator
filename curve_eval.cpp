
# include "curve_eval.h"

CurveEval::CurveEval() {
	_numpoints = 0;
}

void CurveEval::init(const GlProgram& prog) {
	_vsh.load_and_compile(GL_VERTEX_SHADER, "../vsh_mcol_flat.glsl");
	_fsh.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_flat.glsl");
	//_gouradshader.load_and_compile(GL_VERTEX_SHADER, "../vsh_smtl_gourad.glsl");
	//_fragsh2.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_gourad.glsl");
	_prog.init_and_link(_vsh, _fsh);
	//_prog.init_and_link(_gouradshader, _fragsh2);

	// Define buffers needed:
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	/*_prog.uniform_locations(10);
	// Define buffers needed:
	//set_program(prog);    // use base class GlObjects to keep program id
	//gen_vertex_arrays(1); // will use 1 vertex array
	//gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	//uniform_locations(10); // will send 2 variables: the 2 matrices below
	_prog.uniform_location(0, "vTransf");
	_prog.uniform_location(1, "vProj");
	_prog.uniform_location(2, "lPos");
	_prog.uniform_location(3, "la");
	_prog.uniform_location(4, "ld");
	_prog.uniform_location(5, "ls");
	_prog.uniform_location(6, "ka");
	_prog.uniform_location(7, "kd");
	_prog.uniform_location(8, "ks");
	_prog.uniform_location(9, "sh"); */
	_prog.uniform_locations(2); // will send 2 variables: the 2 matrices below
	_prog.uniform_location(0, "vTransf");
	_prog.uniform_location(1, "vProj");
}


void CurveEval::build(float r) {

	// send data to OpenGL buffers:

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	/*glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);*/
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 4 * sizeof(gsbyte), &C[0], GL_STATIC_DRAW);

	glBindVertexArray(0); // break the existing vertex array object binding.

						  // save size so that we can free our buffers and later draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.capacity(0); C.capacity(0); N.resize(0);
}

// draw will be called everytime we need to display this object:
void CurveEval::draw(GsMat& tr, GsMat& pr, const light& l, const mat & m)
{
	// Prepare program:
	glUseProgram(_prog.id);
	glUniformMatrix4fv(_prog.uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(_prog.uniloc[1], 1, GL_FALSE, pr.e);

	// Draw:
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/*glUniformMatrix4fv(_prog.uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(_prog.uniloc[1], 1, GL_FALSE, pr.e);

	float f[4]; // we convert below our color values to be in [0,1]   
	glUniform3fv(_prog.uniloc[2], 1, l.pos.e);
	l.amb.get(f);
	glUniform4fv(_prog.uniloc[3], 1, f);
	l.dif.get(f);
	glUniform4fv(_prog.uniloc[4], 1, f);
	l.spe.get(f);
	glUniform4fv(_prog.uniloc[5], 1, f);
	m.amb.get(f);
	glUniform4fv(_prog.uniloc[6], 1, f);
	m.dif.get(f);
	glUniform4fv(_prog.uniloc[7], 1, f);
	m.spe.get(f);
	glUniform4fv(_prog.uniloc[8], 1, f);
	glUniform1fv(_prog.uniloc[9], 1, &m.shine);*/

	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
}



GsArray<GsVec> CurveEval::eval_bezier(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions) {

	Q.capacity(0);
	if (t < 0.001) {
		for (int i = 0; i < ctrlpnts.size() - 1; i++) {
			Q.push(ctrlpnts.get(i));
		}
	}
	else if (t > 0.999) {
		Q.push(ctrlpnts.get(0));
		for (int i = 0; i < ctrlpnts.size() - 1; i++) {
			Q.push(ctrlpnts.get(i + 1));
		}
	} else {
		Q.push(ctrlpnts.get(0));
		for (int n = 0; n < ctrlpnts.size() - 1; n++) {
			subdivBez(t, ctrlpnts.get(n), ctrlpnts.get(n + 1), ctrlpnts, numRecursions);
		}
		/*for (int i = 0; i < ctrlpnts.size() - 1; i++) {
			GsVec legLen = ctrlpnts.get(i + 1) - ctrlpnts.get(i);
			Q.push(ctrlpnts.get(i) + legLen * t);
		}*/
		Q.push(ctrlpnts.get(ctrlpnts.size() - 1));
	}
	return Q;
}

void CurveEval::subdivBez(float t, GsVec a, GsVec b, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	GsVec legLen = t * (b - a);
	legLen += a;
	if (numRecursions > 0) {
		subdivBez(t, a, legLen, ctrlpnts, (numRecursions - 1));
	}
	Q.push(legLen);
	if (numRecursions > 0) {
		subdivBez(t, legLen, b, ctrlpnts, (numRecursions - 1));
	}
	
	//Q.push(ctrlpnts.get(ctrlpnts.size() - 1));
}

GsArray<GsVec> CurveEval::eval_lagrange(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions) {

	GsArray<GsVec> R;
	R.capacity(0);
	S.capacity(0);
	//R.push(ctrlpnts.get(0));
	float y = 1, nextY = .0f;
	//std::cout << "size: " << ctrlpnts.size() << "\n";
	for (int i = 0; i < ctrlpnts.size() - 1; i++) {
		subdivGrange(t, ctrlpnts.get(i), ctrlpnts.get(i + 1), ctrlpnts, numRecursions);/*
		R.push(ctrlpnts.get(i));
		float x = ctrlpnts.get(i).x + t * (ctrlpnts.get(i + 1).x - ctrlpnts.get(i).x);
		for (int j = 0; j < ctrlpnts.size(); j++) {
			//y = t * (ctrlpnts.get(j).y - ctrlpnts.get(j + 1).y);
			for (int k = 0; k < ctrlpnts.size(); k++) {
				if (j == k) {
					y *= ctrlpnts.get(j).y;
				} else {
					y *=  (x - ctrlpnts.get(k).x) / (ctrlpnts.get(j).x - ctrlpnts.get(k).x);
				}
			}
			nextY += y;
			std::cout << "nextY: " << nextY << "; ";
			y = 1;
		}
		// = t * (ctrlpnts.get(i + 1).y - ctrlpnts.get(i).y);
		R.push(GsVec(x, nextY, .0f));
		nextY = .0f;

		R.push(GsVec(ctrlpnts.get(i + 1).x, ctrlpnts.get(i + 1).y, .0f));*/
	}
	//R.push(ctrlpnts.get(ctrlpnts.size() - 1));

	//S.capacity(0);
	//S = R;
	return S;
}
void CurveEval::subdivGrange(float t, GsVec a, GsVec b, const GsArray<GsVec> &ctrlpnts, int numIt) {
	float y = 1, nextY = .0f;
	float x = a.x + t * (b.x - a.x);
	for (int j = 0; j < ctrlpnts.size(); j++) {
		//y = t * (ctrlpnts.get(j).y - ctrlpnts.get(j + 1).y);
		for (int k = 0; k < ctrlpnts.size(); k++) {
			if (j == k) {
				y *= ctrlpnts.get(j).y;
			}
			else {
				y *= (x - ctrlpnts.get(k).x) / (ctrlpnts.get(j).x - ctrlpnts.get(k).x);
			}
		}
		nextY += y;
		//std::cout << "Y: " << y << "; ";
		//std::cout << "nextY: " << nextY << "; ";
		y = 1;
	}
	if (numIt == 0) {
		if (a.y != 0) {
			S.push(a);
		}
	}
	else {
		subdivGrange(t, a, GsVec(x, nextY, .0f), ctrlpnts, (numIt - 1));
	}
	// = t * (ctrlpnts.get(i + 1).y - ctrlpnts.get(i).y);
	S.push(GsVec(x, nextY, .0f));
	nextY = .0f;

	if (numIt == 0) {
		if (b.y != 0) {
			S.push(b);
		}
	}
	else {
		subdivGrange(t, GsVec(x, nextY, .0f), b, ctrlpnts, (numIt - 1));
	}
}


GsArray<GsVec> CurveEval::eval_bspline(float t, int k, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	B.capacity(0);
	int n = ctrlpnts.size() - 1;
	GsVec newpoint;// = ctrlpnts.get(0);// GsVec(0.0, 0.0, 0.0);
	float increment = t;
	increment /= (numRecursions + 1);
	for (float point = 2; point <= n + 1; point += increment) {
		newpoint =  GsVec(0.0, 0.0, 0.0);
		float sum = 0;
		for (int i = 0; i < n + 1; i++)
		{
			newpoint += ctrlpnts(i)*subdivBSpline(point, i, 3);
		}
		B.push(newpoint);
	}
	//sum all
	//B.remove(0);
	return B;
}
/*
void CurveEval::subdivBSpline(float t, GsVec a, GsVec b, const GsArray<GsVec> &ctrlpnts, int degree, int numIt) {
	float y = 1, nextY = .0f;
	float x = a.x + t * (b.x - a.x);
	for (int i = 0; i < ctrlpnts.size(); i++) {
		
	}
}*/
float CurveEval::subdivBSpline(float location, int currentloc, int degree) {
	float ui = currentloc;//current location on the line(the point we are currently doing)[0,1,2,3,ect...]
	float u = location;

	if (degree == 1)
	{
		if (ui <= u && u < ui + 1)
			return 1.0f;//changed from void return
		else {
			return 0;
		}
	}
	else {
		return (((u - ui) / (degree - 1))*subdivBSpline(location, int(currentloc), degree - 1) + ((ui + degree - u) / (degree - 1))*subdivBSpline(location, int(currentloc + 1), degree - 1));
	}
}

GsArray<GsVec> CurveEval::eval_crspline(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	int n = ctrlpnts.size() - 1;
	CR.capacity(0);
	//CR.push(ctrlpnts.get(0));
	GsArray<GsVec> temp = ctrlpnts, temp2;
		for (int i = 1; i < temp.size() - 1; i++) {
			//subdivCRSpline(t, ctrlpnts.get(i), ctrlpnts.get(i + 1), ctrlpnts, numRecursions);
			temp2.push(subdivCRSpline(t, i, temp, numRecursions));
		}
		temp2.remove(0);
		temp2.remove(temp2.size() - 1);
	while (numRecursions-- > 0) {
		temp.capacity(0);
		for (int i = 1; i < temp2.size(); i++) {
			temp.push(temp2.get(i - 1));
			temp.push((temp2.get(i) - temp2.get(i - 1)) * t + temp2.get(i - 1));
			temp.push(temp2.get(i));
		}
		temp2.capacity(0);
		temp2.push(temp);
	}
	//CR.push(ctrlpnts.get(ctrlpnts.size() - 1));
	//CR.push(ctrlpnts.get(1));
	CR.push(temp2);
	//CR.push(ctrlpnts.get(ctrlpnts.size() - 2));
	//CR.remove(0);
	//CR.remove(CR.size() - 1);

	return CR;
}

GsArray<GsVec> CurveEval::subdivCRSpline(float t, int p, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	//std::cout << p << ", ";
	GsVec Ip = (ctrlpnts[p + 1] - ctrlpnts[p - 1]) / 2;
	GsArray<GsVec> newCR;
	newCR.push(ctrlpnts[p] - (Ip / 3));
	newCR.push(ctrlpnts[p]);
	newCR.push(ctrlpnts[p] + (Ip / 3));
	return newCR;
}


GsArray<GsVec> CurveEval::eval_bospline(float t, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	BO.capacity(0);
	int n = ctrlpnts.size() - 1;
	GsArray<GsVec> temp;
	GsVec u0, u1, u2, v0, v1;
	float d0, d1;
	for (int p = 0; p < ctrlpnts.size(); p++) {
		u1 = ctrlpnts[p];
		u1.normalize();
		if (p > 0) {
			u0 = ctrlpnts[p - 1];
			u0.normalize();
			d0 = sqrt((ctrlpnts[p] - ctrlpnts[p - 1]).norm2());
			v0 = (ctrlpnts[p] - ctrlpnts[p - 1]) / d0;
		}
		else {
			d0 = 0;
		}
		if (p < ctrlpnts.size() - 1) {
			u2 = ctrlpnts[p + 1];
			u2.normalize();
			d1 = sqrt((ctrlpnts[p + 1] - ctrlpnts[p]).norm2());
			v1 = (ctrlpnts[p + 1] - ctrlpnts[p]) / d1;
		}
		else {
			d1 = 0;
		}
		GsVec vPrime = ((d1 * v0) + (d0 * v1)) / (d1 + d0);
		if (p > 0 && p < ctrlpnts.size() - 1) {
			temp.push(ctrlpnts.get(p) - (vPrime * d0 / 3));
		//}
		//if (p < ctrlpnts.size() - 1) {
			temp.push(ctrlpnts.get(p));
			temp.push(ctrlpnts.get(p) + (vPrime * d1 / 3));
		}
	}
	GsArray<GsVec> temp2;
	temp.remove(0);
	temp.remove(temp.size() - 1);
	while (numRecursions-- > 0) {
		temp2.capacity(0);
		for (int i = 1; i < temp.size(); i++) {
			temp2.push(temp.get(i - 1));
			temp2.push((temp.get(i) - temp.get(i - 1)) * t + temp.get(i - 1));
			temp2.push(temp.get(i));
		}
		temp.capacity(0);
		temp.push(temp2);
	}
	//BO = temp;
	//temp.remove(0);
	//temp.remove(temp.size() - 1);
	/*temp2.capacity(0);
	while (numRecursions-- >= 0)do {
		for (int i = 0; i < temp.size(); i++) {
			temp2.push(subdivBOSpline(t, i, temp, numRecursions));
		}
		temp.capacity(0);
		temp.push(temp2);
		temp2.capacity(0);
	} ;*/
	//BO = temp;
	//temp.remove(0);
	//temp.remove(temp.size() - 1);
	BO = temp;
	return BO;
}
GsArray<GsVec> CurveEval::subdivBOSpline(float t, int p, const GsArray<GsVec> &ctrlpnts, int numRecursions) {
	GsArray<GsVec> temp;
	GsVec u0, u1, u2, v0, v1;
	float d0, d1;
	u1 = ctrlpnts[p];
	u1.normalize();
	if (p > 0) {
		u0 = ctrlpnts[p - 1];
		u0.normalize();
		d0 = sqrt((ctrlpnts[p] - ctrlpnts[p - 1]).norm2());
		v0 = (ctrlpnts[p] - ctrlpnts[p - 1]) / d0;
	} else {
		d0 = 0;
	}
	if (p < ctrlpnts.size() - 1) {
		u2 = ctrlpnts[p + 1];
		u2.normalize();
		d1 = sqrt((ctrlpnts[p + 1] - ctrlpnts[p]).norm2());
		v1 = (ctrlpnts[p + 1] - ctrlpnts[p]) / d1;
	} else {
		d1 = 0;
	}
	GsVec vPrime = ((d1 * v0) + (d0 * v1)) / (d1 + d0);
	if (p > 0 && p < ctrlpnts.size() - 1) {
		temp.push(ctrlpnts.get(p) - (vPrime * d0 / 3));
	//}
	//if (p < ctrlpnts.size() - 1) {
		temp.push(ctrlpnts.get(p));
		temp.push(ctrlpnts.get(p) + (vPrime * d1 / 3));
	}
	return temp;
}

GsArray<GsVec> CurveEval::setGuide(float t, const GsArray<GsVec> &ctrlpnts, bool smoothShading) {
	P.capacity(0); C.capacity(0); N.resize(0);
	//setGuide(t, ctrlpnts);
	
	for (int i = 0; i < ctrlpnts.size() - 1; i++) {
		P.push(ctrlpnts.get(i));	C.push(GsColor::cyan);
		GsVec n = ctrlpnts.get(i);
		n.normalize();
		P.push(GsVec(ctrlpnts.get(i).x, ctrlpnts.get(i).y, -1));	C.push(GsColor::cyan);
		GsVec o = GsVec(ctrlpnts.get(i).x, ctrlpnts.get(i).y, -1);
		o.normalize();
		P.push(ctrlpnts.get(i + 1));	C.push(GsColor::cyan);
		GsVec m = ctrlpnts.get(i + 1);
		m.normalize();
		if (smoothShading) {
			N.push_back(n);
			N.push_back(o);
			N.push_back(m);
		}
		else {
			N.push_back(n);
			N.push_back(n);
			N.push_back(n);
		}
		P.push(ctrlpnts.get(i + 1));	C.push(GsColor::magenta);
		P.push(GsVec(ctrlpnts.get(i + 1).x, ctrlpnts.get(i + 1).y, -10));	C.push(GsColor::magenta);
		n = GsVec(ctrlpnts.get(i + 1).x, ctrlpnts.get(i + 1).y, -10);
		n.normalize();
		P.push(GsVec(ctrlpnts.get(i).x, ctrlpnts.get(i).y, -10));	C.push(GsColor::magenta);
		o = GsVec(ctrlpnts.get(i).x, ctrlpnts.get(i).y, -10);
		o.normalize();
		if (smoothShading) {
			N.push_back(m);
			N.push_back(n);
			N.push_back(o);
		}
		else {
			N.push_back(m);
			N.push_back(m);
			N.push_back(m);
		}
	}
	return Q;
}