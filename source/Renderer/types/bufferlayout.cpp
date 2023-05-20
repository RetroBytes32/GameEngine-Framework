#include "bufferlayout.h"

Vertex::Vertex() {
    x=0;  y=0;  z=0;
    r=0;  g=0;  b=0;
    nx=0, ny=0, nx=0;
    u=0;  v=0;
    lr=0; lg=0; lb=0;
    return;
}

Vertex::Vertex(float xx, float yy, float zz, float rr, float gg, float bb, float uu, float vv) {
    x=xx;  y=yy;  z=zz;
    r=rr;  g=gg;  b=bb;
    nx=0, ny=0, nx=0;
    u=uu;  v=vv;
    lr=1; lg=1; lb=1;
    return;
}

void Vertex::operator= (const Vertex& vertex) {
    x = vertex.x;    // Position
    y = vertex.y;
    z = vertex.z;
    r = vertex.r;    // Color
    g = vertex.g;
    b = vertex.b;
    nx = vertex.nx;  // Normals
    ny = vertex.ny;
    nz = vertex.nz;
    u = vertex.u;    // Texture UV
    v = vertex.v;
    lr = vertex.lr;  // Lighting color
    lg = vertex.lg;
    lb = vertex.lb;
    return;
}

Index::Index(unsigned int value) {
    
    index = value;
    return;
}
