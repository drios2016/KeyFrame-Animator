
// Ensure the header file is included only once in multi-file projects
#ifndef SO_POLY_H
#define SO_POLY_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoPoly : public GlObjects
 { private :
	 GlShader _vsh, _fsh;
	 GlProgram _prog;
    GsArray<GsVec>   V; // polyline coordinates
	GsVec W[4][4];
    GsArray<GsColor> C; // colors
    GsColor _color, _selcolor;
    int _selection;
    int _polysize, _fullsize;
    float _selr;
    bool _uptodate;
    void make_selection ();

   public :
    SoPoly ();
    void changed () { _uptodate=false; }
    const GsArray<GsVec>& vertexarray() { return V; }
    GsVec* vertices() { return V.pt(); }
	GsVec* vertexdim() { return *W; }
    int num_vertices () const { return V.size(); }
    void add ( const GsVec& p, int pos=-1 );
	void addToArr(const GsArray<GsVec>& p);
    void move ( int pos, const GsVec& p );
    void del ( int pos );
    int selection ();
    void select ( int i, float r );
    void init ( const GlProgram& prog, const GsColor& c, const GsColor& selc );
    void draw ( GsMat& tr, GsMat& pr );
 };

#endif // SO_POLY_H
