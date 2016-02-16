# include "sofloor.h"


void Sofloor::SoFloor()
{
	_numpoints = 0;
}

void Sofloor::init()
{
	_vshgou.load_and_compile(GL_VERTEX_SHADER, "../vsh_mcol_gouraud.glsl");
	_fshgou.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_gouraud.glsl");
	_proggouraud.init_and_link(_vshgou, _fshgou);

	_proggouraud.init_and_link(_vshgou, _fshgou);
	// Define buffers needed:
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	_proggouraud.uniform_locations(10); // will send 2 variables: the 2 matrices below
	_proggouraud.uniform_location(0, "vTransf");
	_proggouraud.uniform_location(1, "vProj");
	_proggouraud.uniform_location(2, "lPos");
	_proggouraud.uniform_location(3, "la");
	_proggouraud.uniform_location(4, "ld");
	_proggouraud.uniform_location(5, "ls");
	_proggouraud.uniform_location(6, "ka");
	_proggouraud.uniform_location(7, "kd");
	_proggouraud.uniform_location(8, "ks");
	_proggouraud.uniform_location(9, "sh");
}

void Sofloor::build()
{
	P.push_back(GsVec(-1, 0, -1));
	P.push_back(GsVec(-1, 0, 1));
	P.push_back(GsVec(1, 0, -1));

	P.push_back(GsVec(1, 0, 1));
	P.push_back(GsVec(-1, 0, 1));
	P.push_back(GsVec(1, 0, -1));
	
	C.push_back(GsVec(0, 0, 1));
	C.push_back(GsVec(0, 0, 1));
	C.push_back(GsVec(0, 0, 1));
	C.push_back(GsVec(0, 0, 1));
	C.push_back(GsVec(0, 0, 1));
	C.push_back(GsVec(0, 0, 1));
	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, C.size() * 3 * sizeof(float), &C[0], GL_STATIC_DRAW);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); C.resize(0);
}

void Sofloor::draw(const GsMat& tr, const GsMat& pr, const light&l, const mat& m)
{
	// Draw Lines:
	glUseProgram(_proggouraud.id);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glUniformMatrix4fv(_proggouraud.uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(_proggouraud.uniloc[1], 1, GL_FALSE, pr.e);

	float f[4]; // we convert below our color values to be in [0,1]   
	glUniform3fv(_proggouraud.uniloc[2], 1, l.pos.e);
	glUniform4fv(_proggouraud.uniloc[3], 1, l.amb.get(f));
	glUniform4fv(_proggouraud.uniloc[4], 1, l.dif.get(f));
	glUniform4fv(_proggouraud.uniloc[5], 1, l.spe.get(f));
	glUniform4fv(_proggouraud.uniloc[6], 1, m.amb.get(f));
	glUniform4fv(_proggouraud.uniloc[7], 1, m.dif.get(f));
	glUniform4fv(_proggouraud.uniloc[8], 1, m.spe.get(f));
	glUniform1fv(_proggouraud.uniloc[9], 1, &m.shine);

	glDrawArrays(GL_TRIANGLES, 0, _numpoints);

	//draw the things
}
