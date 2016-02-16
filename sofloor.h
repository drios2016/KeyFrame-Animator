

// Ensure the header file is included only once in multi-file projects
#ifndef SO_FLOOR_H
#define SO_FLOOR_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
#include "light.h"
#include "material.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class Sofloor : public GlObjects
{
private:
	GlShader _vshgou, _fshgou, _vshphong, _fshphong;
	GlProgram _proggouraud, _progphong;
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec> C; // normals
	int _numpoints;
public:
	void SoFloor();
	void init();
	void build();
	void draw(const GsMat& tr, const GsMat& pr, const light& l, const mat & m);
};



#endif // SO_AXIS_H