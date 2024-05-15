#include <math.h>
#include <gl/glut.h>
#include <iostream>

#define PI           3.14159265358979323
#define COLORNUM     14
#define D2R          PI/180

using namespace std;

class Point3
{
public:
    float x, y, z;
    void set(float dx, float dy, float dz)
    {
        x = dx; y = dy; z = dz;
    }
    void set(Point3& p)
    {
        x = p.x; y = p.y; z = p.z;
    }
    Point3() { x = y = z = 0; }
    Point3(float dx, float dy, float dz)
    {
        x = dx; y = dy; z = dz;
    }

};

class Color3
{
public:
    float r, g, b;
    void set(float red, float green, float blue)
    {
        r = red; g = green; b = blue;
    }
    void set(Color3& c)
    {
        r = c.r; g = c.g; b = c.b;
    }
    Color3() { r = g = b = 0; }
    Color3(float red, float green, float blue)
    {
        r = red; g = green; b = blue;
    }

};

class Point2
{
public:
    Point2() { x = y = 0.0f; } // constructor 1
    Point2(float xx, float yy) { x = xx; y = yy; } // constructor 2
    void set(float xx, float yy) { x = xx; y = yy; }
    float getX() { return x; }
    float getY() { return y; }
    void draw() {
        glBegin(GL_POINTS);
        glVertex2f((GLfloat)x, (GLfloat)y);
        glEnd();
    }
private:
    float     x, y;
};

class Vector3
{
public:
    float    x, y, z;
    void set(float dx, float dy, float dz)
    {
        x = dx; y = dy; z = dz;
    }
    void set(Vector3& v)
    {
        x = v.x; y = v.y; z = v.z;
    }
    void flip()
    {
        x = -x; y = -y; z = -z;
    }
    void normalize();
    Vector3() { x = y = z = 0; }
    Vector3(float dx, float dy, float dz)
    {
        x = dx; y = dy; z = dz;
    }
    Vector3(Vector3& v)
    {
        x = v.x; y = v.y; z = v.z;
    }
    Vector3 cross(Vector3 b);
    float dot(Vector3 b);
};

Vector3 Vector3::cross(Vector3 b)
{
    Vector3 c(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
    return c;
}

float Vector3::dot(Vector3 b)
{
    return x * b.x + y * b.y + z * b.z;
}

void Vector3::normalize()
{
    float temp = sqrt(x * x + y * y + z * z);
    x = x / temp;
    y = y / temp;
    z = z / temp;
}

class VertexID
{
public:
    int       vertIndex;
    int       colorIndex;
};

class Face
{
public:
    int         nVerts;
    VertexID*   vert;
    Vector3     facenorm;

    Face()
    {
        nVerts = 0;
        vert = NULL;
    }
    ~Face()
    {
        if (vert != NULL)
        {
            delete[] vert;
            vert = NULL;
        }
        nVerts = 0;
    }

    void setIndices4(int v0, int v1, int v2, int v3)
	{
		nVerts = 4;
		vert = new VertexID[nVerts];
		vert[0].vertIndex = v0;
		vert[1].vertIndex = v1;
		vert[2].vertIndex = v2;
		vert[3].vertIndex = v3;
	}

	void setIndices3(int v0, int v1, int v2)
	{
		nVerts = 3;
		vert = new VertexID[nVerts];
		vert[0].vertIndex = v0;
		vert[1].vertIndex = v1;
		vert[2].vertIndex = v2;
	}
};

class Mesh
{
public:
    int         numVerts;
    Point3*     pt;

    int         numFaces;
    Face*       face;

public:
    Mesh()
    {
        numVerts = 0;
        pt = NULL;
        numFaces = 0;
        face = NULL;
    }
    ~Mesh()
    {
        if (pt != NULL)
        {
            delete[] pt;
        }
        if (face != NULL)
        {
            delete[] face;
        }
        numVerts = 0;
        numFaces = 0;
    }

    void CreateCuboid(float fLength, float fHeight, float fWidth);
    void CreateShape2(int nSegment, float fWidth, float fHeight, float fLength, float fRadius1, float fRadius2, float fRadius3);
    void CreateShape3(float fLength, float fRadius0, float fRadius1, float fHeight0, float fHeight1, int p);
    void CreateShape1(float fWidth, float fHeight, float fLength, float fLength1);
    void CreateCylinder(int nSegment, float fHeight, float fRadius, bool c);
    void SetColor(int colorIdx);
    void DrawWireframe();
    void DrawColor();
    void CalculateFacesNorm();
    void setupMaterial(float ambient[], float diffuse[], float specular[], float shiness);
    void Draw();

};

float ColorArr[COLORNUM][3] = {{1.0, 0.0, 0.0},	{0.0, 1.0, 0.0},{0.019, 0.0, 1.0},{1.0, 1.0, 0.32},{1.0, 0.02, 0.22},{0.02, 1.0, 1.0},
							{0.3, 0.32, 0.3},{0.57, 0.53, 0.5},{0.92, 0.091, 0.9},{1.0, 0.01, 0.02},{0.2, 1.0, 0.04}, {0.03, 0.02, 1.0},{0.451, 0.142, 0.91}, {0.3, 0.1, 0.01}};


void Mesh::CreateCylinder(int nSegment, float fHeight, float fRadius, bool c)
{
    numVerts = nSegment * 2 + 2;
    pt = new Point3[numVerts];

    int     i, idx;
    float   fAngle = 2 * PI / nSegment;
    float   x, y, z;

    pt[0].set(0, fHeight / 2, 0);
    pt[numVerts - 1].set(0, -fHeight / 2, 0);

    for (int i = 0; i < nSegment; i++)
    {
        x = fRadius * cos(fAngle * i);
        z = fRadius * sin(fAngle * i);
        y = fHeight / 2;
        pt[i + 1].set(x, y, z);
        pt[i + 1 + nSegment].set(x, -y, z);
    }

    if (!c)
        numFaces = nSegment * 3;
    else
        numFaces = 2;

    face = new Face[numFaces];

    idx = 0;

    for (i = 0; i < nSegment; i++)
    {
        if (c)
        {
            face[idx].nVerts = nSegment;
            face[idx].vert = new VertexID[face[idx].nVerts];
            for (int j = 0; j < nSegment; j++) face[idx].vert[j].vertIndex = nSegment - j;
            idx++;
            break;
        }
        else
            face[idx++].setIndices3(0, (i < nSegment - 1)? i + 2 : 1, i + 1);
    }

    for (i = 0; i < nSegment; i++)
    {
        if (c)
        {
            face[idx].nVerts = nSegment;
            face[idx].vert = new VertexID[face[idx].nVerts];
            for (int j = 0; j < nSegment; j++) face[idx].vert[j].vertIndex = j + 1 + nSegment;
            idx++;
            break;
        }
        else
            face[idx++].setIndices3(numVerts - 1, i + 1 + nSegment, (i < nSegment - 1) ? i + 2 + nSegment : 1 + nSegment);
    }

    if (!c) {
        for (i = 0; i < nSegment; i++) face[idx++].setIndices4(i + 1, (i < nSegment - 1)? i + 2 : 1, (i < nSegment - 1)? i + 2 + nSegment : 1 + nSegment, i + 1 + nSegment);
    }
}

void Mesh::CreateShape1(float fLength, float fHeight, float fWidth, float fWidth0)
{
	numVerts = 8;
	pt = new Point3[numVerts];

	pt[0].set(-fLength / 2, fHeight / 2, fWidth0 / 2);
    pt[1].set(fLength / 2, -fHeight / 2, -fWidth / 2);
	pt[2].set(fLength / 2, fHeight / 2, -fWidth0 / 2);
	pt[3].set(-fLength / 2, -fHeight / 2, fWidth / 2);
    pt[4].set(-fLength / 2, fHeight / 2, -fWidth0 / 2);
	pt[5].set(fLength / 2, -fHeight / 2, fWidth / 2);
	pt[6].set(fLength / 2, fHeight / 2, fWidth0 / 2);
	pt[7].set(-fLength / 2, -fHeight / 2, -fWidth / 2);

	numFaces = 6;
	face = new Face[numFaces];

    face[0].setIndices4(6, 5, 1, 2);
    face[1].setIndices4(4, 2, 1, 7);
    face[2].setIndices4(0, 4, 7, 3);
    face[3].setIndices4(0, 3, 5, 6);
    face[4].setIndices4(1, 5, 3, 7);
    face[5].setIndices4(2, 4, 0, 6);
}

void Mesh::CreateShape2(int nSegment, float fWidth, float fHeight, float fLength, float fRadius1, float fRadius2, float fRadius3)
{
    numVerts = 10*(nSegment + 1) + 10;
    pt = new Point3[numVerts];

    int i;
    int idx = 0;
    float x, z;

    float d0 = PI/nSegment;
    float d1 = 0.5*PI/nSegment;

    for (int i = 1; i <= nSegment + 1; i++)
    {
        x = fWidth/2 + fRadius1*sin(PI - d0*(i - 1));
        z = fRadius1*cos(PI - d0*(i - 1));

        if (i == 1) pt[0].set(fWidth/2, fHeight/2, 0);
        pt[i].set(x, fHeight/2, z);

        if (i == 1) pt[5*nSegment + 10].set(fWidth/2, -fHeight/2, 0);
        pt[i + 5*nSegment + 10].set(x, -fHeight/2, z);

        x = -fWidth/2 - fRadius1*sin(d0*(i - 1));
        z = fRadius1*cos(d0*(i - 1));

        if (i == 1) pt[nSegment + 2].set(-fWidth/2, fHeight/2, 0);
        pt[i + nSegment + 2].set(x, fHeight/2, z);

        if (i == 1) pt[6*nSegment + 12].set(-fWidth/2, -fHeight/2, 0);
        pt[i + 6*nSegment + 12].set(x, -fHeight/2, z);

        x = fRadius3*sin(-PI/2 + d0*(i - 1));
        z = fLength + fRadius3*cos(-PI/2 + d0*(i - 1));

        if (i == 1) pt[2*nSegment + 4].set(0, fHeight/2, fLength);
        pt[i + 2*nSegment + 4].set(x, fHeight/2, z);

        if (i == 1) pt[7*nSegment + 14].set(0, -fHeight/2, fLength);
        pt[i + 7*nSegment + 14].set(x, -fHeight/2, z);

        x = -(fRadius2 + fRadius3) + fRadius2*cos(d1*(i - 1));
        z = fRadius1 + fRadius2 + fRadius2*cos(PI/2 + d1*(i - 1));

        if (i == 1) pt[3*nSegment + 6].set(-fRadius3, fHeight/2, fRadius1);
        pt[i + 3*nSegment + 6].set(x, fHeight/2, z);

        if (i == 1) pt[8*nSegment + 16].set(-fRadius3, -fHeight/2, fRadius1);
        pt[i + 8*nSegment + 16].set(x, -fHeight/2, z);

        x = fRadius2 + fRadius3 + fRadius2*cos(PI/2 + d1*(i - 1));
        z = fRadius1 + fRadius2 - fRadius2*sin(PI/2 + d1*(i - 1));

        if (i == 1) pt[4*nSegment + 8].set(fRadius3, fHeight/2, fRadius1);
        pt[i + 4*nSegment + 8].set(x, fHeight/2, z);

        if (i == 1) pt[9*nSegment + 18].set(fRadius3, -fHeight/2, fRadius1);
        pt[i + 9*nSegment + 18].set(x, -fHeight/2, z);
    }

    numFaces = 9 + 15*nSegment;
    face = new Face[numFaces];

    for(i = 0; i < nSegment; i++)  face[idx++].setIndices3(nSegment + 2, nSegment + i + 4, nSegment + i + 3);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices3(2*(nSegment + 2) , 2*(nSegment + 2) + i + 1, 2*(nSegment + 3) + i);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices3(7*nSegment + 14, 7*nSegment + i + 16, 7*nSegment + i + 15);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices3(i + 8*nSegment + 18, 8*nSegment + 16, i + 8*nSegment + 17);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices3(i + 3*(nSegment + 2) + 2, i + 3*nSegment + 7, 3*(nSegment + 2));
    for(i = 0; i < nSegment; i++) face[idx++].setIndices3(9*nSegment + 18, 9*nSegment + 19 + i, 9*nSegment + 20 + i);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices3(5*(nSegment + 2), 5*(nSegment + 2) + i + 1, 5*(nSegment + 2) + i + 2);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices3(0, i + 2, i + 1);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices3(6*(nSegment + 2), 6*(nSegment + 2) + i + 1, 6*(nSegment + 2) + i + 2);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices3(4*nSegment + i + 9, 8 + 4*nSegment, 4*nSegment + i + 10);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices4(2 + i, 5*nSegment + 12 + i, 5*nSegment + 11 + i, 1 + i);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices4(6*nSegment + 14 + i, 6*nSegment + 13 + i, nSegment + 3 + i, nSegment + 4 + i);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices4(3*nSegment + 8 + i, 8*nSegment + 18 + i, 8*nSegment + 17 + i, 3*nSegment + 7 + i);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices4(4*nSegment + 10 + i, 9*nSegment + 20 + i, 9*nSegment + 19 + i, 4*nSegment + 9 + i);
    for(i = 0; i < nSegment; i++) face[idx++].setIndices4(2*(nSegment + 3) + i, 2*(nSegment + 2) + 1 + i, 7*nSegment + 15 + i, 7*nSegment + 16 + i);

    face[idx++].setIndices4(13 + 7*nSegment, 11 + 5*nSegment, 11 + 6*nSegment, 13 + 6*nSegment);
    face[idx++].setIndices4(nSegment + 1, 1, 2*nSegment + 3, nSegment + 3);
    face[idx++].setIndices4(nSegment + 1, 4*(nSegment + 1) + 5, 9*(nSegment + 1) + 10, 11 + 6*nSegment);
    face[idx++].setIndices4(10*nSegment + 19, 5*nSegment + 9, 3*nSegment + 5, 8*nSegment + 15);
    face[idx++].setIndices4(3*(nSegment + 1) + 4, 2*(nSegment + 1) + 3, 7*(nSegment + 1) + 8, 8*(nSegment + 1) + 9);
    face[idx++].setIndices4(nSegment + 3, 6*(nSegment + 1) + 7, 9*(nSegment + 1) + 8, 4*(nSegment + 1) + 3);
    face[idx++].setIndices4(4*(nSegment + 1) + 4, 3*(nSegment + 1) + 3, 2*(nSegment + 1) + 3, 3*(nSegment + 1) + 2);
    face[idx++].setIndices4(16 + 8*nSegment, 18 + 9*nSegment, 15 + 8*nSegment, 15 + 7*nSegment);
    face[idx++].setIndices4(3 + 2*nSegment, 1, 11 + 5*nSegment, 13 + 7*nSegment);
}

void Mesh::CreateShape3(float fLength, float fRadius0, float fRadius1, float fHeight0, float fHeight1, int p)
{
    numVerts = 20;
    pt = new Point3[numVerts];

    int idx;
    float a = fLength/2, b = fRadius1/2, c = fRadius0/2, d = (fRadius1 - fRadius0)/(2*tan(PI/p));

    pt[0].set(-a, 0, -b);
    pt[1].set(a, 0, -c);
    pt[2].set(-a, 0, -c);
    pt[3].set(-a, 0, c);
    pt[4].set(a, 0, c);
    pt[5].set(-a, 0, b);
    pt[6].set(a - d, 0, -b);
    pt[7].set(a - d, 0, b);
    pt[8].set(-a, fHeight1, -c);
    pt[9].set(-a, fHeight1, c);
    pt[10].set(a, fHeight1, c);
    pt[11].set(-a, fHeight1, b);
    pt[12].set(-a, fHeight1, -b);
    pt[13].set(a, fHeight1, -c);
    pt[14].set(a - d, fHeight1, -b);
    pt[15].set(a - d, fHeight1, b);
    pt[16].set(-a, fHeight0, -c);
    pt[17].set(a, fHeight0, -c);
    pt[18].set(a, fHeight0, c);
    pt[19].set(-a, fHeight0, c);

    numFaces = 16;
    face = new Face[numFaces];

    idx = 0;

    face[idx++].setIndices4(16, 19, 18, 17);
    face[idx++].setIndices4(1, 4, 3, 2);
    face[idx++].setIndices4(6, 14, 13, 1);
    face[idx++].setIndices4(4, 7, 15, 10);
    face[idx++].setIndices4(0, 12, 14, 6);
    face[idx++].setIndices4(7, 15, 11, 5);
    face[idx++].setIndices4(0, 2, 8, 12);
    face[idx++].setIndices4(3, 5, 11, 9);
    face[idx++].setIndices4(2, 3, 19, 16);
    face[idx++].setIndices4(1, 17, 18, 4);
    face[idx++].setIndices4(10, 18, 19, 9);
    face[idx++].setIndices4(13, 8, 16, 17);
    face[idx++].setIndices4(0, 6, 1, 2);
    face[idx++].setIndices4(3, 4, 7, 5);
    face[idx++].setIndices4(12, 8, 13, 14);
    face[idx++].setIndices4(9, 11, 15, 10);
}

void Mesh::SetColor(int colorIdx)
{
    for (int f = 0; f < numFaces; f++)
    {
        for (int v = 0; v < face[f].nVerts; v++)
        {
            face[f].vert[v].colorIndex = colorIdx;
        }
    }
}

void Mesh::DrawWireframe()
{
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (int f = 0; f < numFaces; f++)
    {
        glBegin(GL_POLYGON);
        for (int v = 0; v < face[f].nVerts; v++)
        {
            int iv = face[f].vert[v].vertIndex;
            glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);
}

void Mesh::DrawColor()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int f = 0; f < numFaces; f++)
    {
        glBegin(GL_POLYGON);
        for (int v = 0; v < face[f].nVerts; v++)
        {
            int iv = face[f].vert[v].vertIndex;
            int ic = face[f].vert[v].colorIndex;

            ic = f % COLORNUM;

            glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]);
            glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
        }
        glEnd();
    }
}

void Mesh::Draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int f = 0; f < numFaces; f++)
    {
        glBegin(GL_POLYGON);
        for (int v = 0; v < face[f].nVerts; v++)
        {
            int iv = face[f].vert[v].vertIndex;

            glNormal3f(face[f].facenorm.x, face[f].facenorm.y, face[f].facenorm.z);
            glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
        }
        glEnd();
    }
}

void Mesh::CalculateFacesNorm()
{
    float p[3];
	for (int f = 0; f < numFaces; f++)
	{
		p[0] = p[1] = p[2] = 0;
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int iv = face[f].vert[v].vertIndex;
			int iv_next = face[f].vert[(v + 1) % face[f].nVerts].vertIndex;
			p[0] += (pt[iv].y - pt[iv_next].y) * (pt[iv].z + pt[iv_next].z);
			p[1] += (pt[iv].z - pt[iv_next].z) * (pt[iv].x + pt[iv_next].x);
			p[2] += (pt[iv].x - pt[iv_next].x) * (pt[iv].y + pt[iv_next].y);
		}
		face[f].facenorm.set(p[0], p[1], p[2]);
		face[f].facenorm.normalize();
	}
}

void Mesh::setupMaterial(float ambient[], float diffuse[], float specular[], float shiness)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiness);
}

GLfloat redrgb[] = { 1.0f, 0.0f , 0.0f, 1.0f };
GLfloat bluergb[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat greenrgb[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat yellowrgb[] = { 1.0f, 0.65f, 0.46f, 1.0f };

float  camera_angle = -45;
float  camera_height = 2.4;
float  camera_dis = 4;

float  camera_X, camera_Y, camera_Z;
float  lookAt_X = 0, lookAt_Y = 0.1, lookAt_Z = 0;

int  screenWidth = 1200;
int  screenHeight = 600;

bool   modeortho = false;
bool   modewire = false;

Mesh   chotcao, chot, contruot, thanhlienket, chuT;
Mesh   cmauvat[4], mauvat[4];

float ANG = 0;
float ANG1 = 0;
float ANG2 = 0;

float lchuthap = 1, ranh0 = 0.11, ranh1 = 0.22, caoct0 = 0.09, caoct1 = 0.18;
float lconchay = 0.22, hconchay = caoct1 - caoct0, rconchay = ranh0;
float hchotcao = 0.05, rchotcao = 0.04;
float hchotthap = 0.07, rchotthap = rchotcao;
float LA = 0.734;
float llk = LA + ranh0, caolk = 0.06, daylk = 0.16, noclk = ranh0;
float wt = 1, ht = 0.22, lt = 0.94, rad1 = 0.13, rad2 = 0.07, rad3 = 0.16;

float deg[4][4], rad[4][4];

int nSegment = 36;

GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat shiness = 90.0;

void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
	{
	case '1':
		ANG2 += 3;
		break;

	case '2':
		ANG2 -= 3;
		break;

	case '3':
		ANG1 += 3;
        break;

    case '4':
		ANG1 -= 3;
        break;

    case '5':
		ANG += 3; if (ANG > 360) ANG -= 360;
        break;

    case '6':
		ANG -= 3; if (ANG < -360) ANG += 360;
        break;

	case '+':
		camera_dis = camera_dis + 0.06; break;

	case '-':
		camera_dis = camera_dis - 0.06; break;

	case 'w':

	case 'W':
		modewire = !modewire; break;

	case 'v':

	case 'V':
		modeortho = !modeortho;
		break;

	default:
		break;
	}

    glutPostRedisplay();
}

void mySpecialKeyboard(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        camera_height += 0.04;
        break;

    case GLUT_KEY_DOWN:
        camera_height -= 0.04;
        break;

    case GLUT_KEY_RIGHT:
        camera_angle -= 1.75;
        break;

    case GLUT_KEY_LEFT:
        camera_angle += 1.75;
        break;

    default:
        break;
    }

    glutPostRedisplay();
}

void create()
{
    deg[0][0] = 4;
    deg[0][1] = 360.0/4;
    deg[0][2] = -lchuthap/2 - ranh0/(2*tan(PI/4));
    deg[0][3] = 360.0/8;

    deg[1][0] = 5;
    deg[1][1] = 360.0/5;
    deg[1][2] = -lchuthap/2 - ranh0/(2*tan(PI/5));
    deg[1][3] = 360.0/5;

    deg[2][0] = 6;
    deg[2][1] = 360.0/6;
    deg[2][2] = -lchuthap/2 - ranh0/(2*tan(PI/6));
    deg[2][3] = 360.0/12;

    deg[3][0] = 8;
    deg[3][1] = 360.0/8;
    deg[3][2] = -lchuthap/2 - ranh0/(2*tan(PI/8));
    deg[3][3] = 360.0/16;

    rad[0][0] = 0;
    rad[0][1] = 0;
    rad[0][2] = 0;
    rad[0][3] = 0;

    rad[1][0] = 90;
    rad[1][1] = -11.0/4;
    rad[1][2] = 0;
    rad[1][3] = 11.0/4;

    rad[2][0] = 90;
    rad[2][1] = 11.0/4;
    rad[2][2] = 0;
    rad[2][3] = 11.0/4;

    rad[3][0] = -90;
    rad[3][1] = 11.0/4;
    rad[3][2] = 0;
    rad[3][3] = -11.0/4;

    contruot.CreateShape1(lconchay, hconchay, ranh0, ranh0);
    contruot.CalculateFacesNorm();

    chotcao.CreateCylinder(nSegment, hchotcao, rchotcao, false);
    chotcao.CalculateFacesNorm();

    chot.CreateCylinder(nSegment, hchotthap, rchotthap, false);
    chot.CalculateFacesNorm();

    thanhlienket.CreateShape1(llk, caolk, daylk, noclk);
    thanhlienket.CalculateFacesNorm();

    chuT.CreateShape2(nSegment, wt, ht, lt, rad1, rad2, rad3);
    chuT.CalculateFacesNorm();

    cmauvat[0].CreateCylinder(4, caoct0, ranh0/(sin(PI/4)*2), true);
    cmauvat[0].CalculateFacesNorm();

    cmauvat[1].CreateCylinder(5, caoct0, ranh0/(sin(PI/5)*2), true);
    cmauvat[1].CalculateFacesNorm();

    cmauvat[2].CreateCylinder(6, caoct0, ranh0/(sin(PI/6)*2), true);
    cmauvat[2].CalculateFacesNorm();

    cmauvat[3].CreateCylinder(8, caoct0, ranh0/(sin(PI/8)*2), true);
    cmauvat[3].CalculateFacesNorm();

    mauvat[0].CreateShape3(lchuthap, ranh0, ranh1, caoct0, caoct1, 4);
    mauvat[0].CalculateFacesNorm();

    mauvat[1].CreateShape3(lchuthap, ranh0, ranh1, caoct0, caoct1, 5);
    mauvat[1].CalculateFacesNorm();

    mauvat[2].CreateShape3(lchuthap, ranh0, ranh1, caoct0, caoct1, 6);
    mauvat[2].CalculateFacesNorm();

    mauvat[3].CreateShape3(lchuthap, ranh0, ranh1, caoct0, caoct1, 8);
    mauvat[3].CalculateFacesNorm();
}

void drawtile(float size, float x, float z)
{
    glColor3f(0.9, 0.375, 0.375);
    glBegin(GL_QUADS);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x + 0.25*size, 0, z);
        glVertex3f(x, 0, z  + 0.25*size);
        glVertex3f(x - 0.25*size, 0, z);
    glEnd();

    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z);
        glVertex3f(x - 0.5*size, 0, z - 0.25*size);
        glVertex3f(x - 0.5*size, 0, z - 0.5*size);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.5*size);
        glVertex3f(x - 0.5*size, 0, z - 0.5*size);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.5*size);
        glVertex3f(x + 0.25*size, 0, z - 0.5*size);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z);
    glEnd();

    glPushMatrix();
    glScalef(-1, 0, -1);
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z);
        glVertex3f(x - 0.5*size, 0, z - 0.25*size);
        glVertex3f(x - 0.5*size, 0, z - 0.5*size);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.5*size);
        glVertex3f(x - 0.5*size, 0, z - 0.5*size);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.5*size);
        glVertex3f(x + 0.25*size, 0, z - 0.5*size);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glScalef(1, 0, -1);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z);
        glVertex3f(x - 0.5*size, 0, z - 0.25*size);
        glVertex3f(x - 0.5*size, 0, z - 0.5*size);
    glEnd();

    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.5*size);
        glVertex3f(x - 0.5*size, 0, z - 0.5*size);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.5*size);
        glVertex3f(x + 0.25*size, 0, z - 0.5*size);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glScalef(-1, 0, 1);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z);
        glVertex3f(x - 0.5*size, 0, z - 0.25*size);
        glVertex3f(x - 0.5*size, 0, z - 0.5*size);
    glEnd();

    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.5*size);
        glVertex3f(x - 0.5*size, 0, z - 0.5*size);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.5*size);
        glVertex3f(x + 0.25*size, 0, z - 0.5*size);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex3f(x, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, z - 0.25*size);
        glVertex3f(x - 0.25*size, 0, 0 + z);
    glEnd();
    glPopMatrix();
}

void sannha()
{
    glPushMatrix();
    glTranslatef(0, -2, 0);

    for (int row = -25; row <= 25; row++) {
        for (int col = -25; col <= 25; col++) {
            drawtile(0.75, 0.75*col, 0.75*row);
        }
    }

    glPopMatrix();
}

void redstar(int idx)
{
    mauvat[idx].setupMaterial(ambient, redrgb, specular, shiness);
    glPushMatrix();
    glTranslatef(0, caoct0/2, 0);
    glRotatef(deg[idx][3], 0, 1, 0);
    if (modewire == true)
        cmauvat[idx].DrawWireframe();
    else
        cmauvat[idx].Draw();
    glPopMatrix();

    for (int i = 0; i < deg[idx][0]; i++) {
        glPushMatrix();
        glRotatef(-deg[idx][1]*i, 0, 1, 0);
        glTranslatef(deg[idx][2], 0, 0);
        if (modewire == true)
            mauvat[idx].DrawWireframe();
        else
            mauvat[idx].Draw();
        glPopMatrix();
    }
}

void render()
{
    glPushMatrix();

    glRotatef(ANG1, 1, 0, 0);
    glRotatef(ANG2, 0, 1, 0);
    glColor3f(0, 0, 1);

    redstar(0);

    glPushMatrix();
    glTranslatef(LA*cos(ANG*D2R), caoct0 + hconchay/2, 0);
    if (modewire == true) {
        contruot.DrawWireframe();
    }
    else {
        contruot.setupMaterial(ambient, bluergb, specular, shiness);
        contruot.Draw();
    }
    glTranslatef(-LA*cos(ANG*D2R), 0, LA*sin(ANG*D2R));
    glRotatef(90, 0, 1, 0);
    if (modewire == true) {
        contruot.DrawWireframe();
    }
    else {
        contruot.setupMaterial(ambient, bluergb, specular, shiness);
        contruot.Draw();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(LA/2*cos(ANG*D2R), caoct1 + caolk/2, LA/2*sin(ANG*D2R));
    glRotatef(ANG, 0, 1, 0);
    if (modewire == true) {
        thanhlienket.DrawWireframe();
    }
    else {
        thanhlienket.setupMaterial(ambient, greenrgb, specular, shiness);
        thanhlienket.Draw();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(LA/2*cos(ANG*D2R), caoct1 + caolk + hchotcao/2, LA/2*sin(ANG*D2R));
    if (modewire == true) {
        chotcao.DrawWireframe();
    }
    else {
        chotcao.setupMaterial(ambient, yellowrgb, specular, shiness);
        chotcao.Draw();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(LA*cos(ANG*D2R), caoct1 + hchotthap/2, 0);
    if (modewire == true) {
        chot.DrawWireframe();
    }
    else {
        chot.setupMaterial(ambient, yellowrgb, specular, shiness);
        chot.Draw();
    }

    glTranslatef(-LA*cos(ANG*D2R), 0, LA*sin(ANG*D2R));
    if (modewire == true) {
        chot.DrawWireframe();
    }
    else {
        chot.setupMaterial(ambient, yellowrgb, specular, shiness);
        chot.Draw();
    }
    glPopMatrix();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.75, 0, -2.75);
    glRotatef(90, 1, 0, 0);
    if (modewire == true) {
        chuT.DrawWireframe();
    }
    else {
        chuT.setupMaterial(ambient, redrgb, specular, shiness);
        chuT.Draw();
    }
    glPopMatrix();

    for (int i = 1; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(rad[i][1], rad[i][2], rad[i][3]);
        glRotatef(rad[i][0], 1, 0, 0);
        redstar(i);
        glPopMatrix();
    }

    glDisable(GL_LIGHTING);
    sannha();
    glEnable(GL_LIGHTING);
}

void myInit()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glEnable(GL_NORMALIZE);
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, screenWidth, screenHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    (modeortho) ? glOrtho(-2.5, 2.5, -1.25, 1.25, -5.0, 5.0) : gluPerspective(45.0, (float)screenWidth / (float)screenHeight, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float nx, ny, nz;

    if (modeortho) {
        camera_X = 0;
        camera_Y = 1;
        camera_Z = 0;

        nx = 0; ny = 0; nz = -1;
    }
    else {
        camera_X = camera_dis * sin(camera_angle * D2R);
        camera_Y = camera_height;
        camera_Z = camera_dis * cos(camera_angle * D2R);

        nx = 0; ny = 1; nz = 0;
    }

    gluLookAt(camera_X, camera_Y, camera_Z, lookAt_X, lookAt_Y, lookAt_Z, nx, ny, nz);

    GLfloat light_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

    GLfloat light_Position0[] = { -4.5f, 12.0f, -4.5f, 0.0f };
    GLfloat light_Position1[] = { 4.5f, 12.0f, 4.5f, 0.0f };

    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_Specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_Position0);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_Diffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_Ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_Specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_Position1);

    glEnable(GL_LIGHT0);

    render();

    glFlush();

    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    cout << "1, 2      : Xoay hinh chu thap xung quanh truc Y cuc bo\n";
    cout << "3, 4      : Xoay hinh chu thap xung quanh truc X cuc bo\n";
    cout << "5, 6      : Dich chuyen thanh truot\n";
    cout << "W, w  : Chuyen doi qua lai giua che do khung day va to mau\n";
    cout << "V, v  : Chuyen doi qua lai giua hai che do nhin\n";
    cout << "+     : Tang khoang cach camera\n";
    cout << "-     : Giam khoang cach camera\n";
    cout << "up arrow  : Tang chieu cao camera\n";
    cout << "down arrow: Giam chieu cao camera\n";
    cout << "<-        : Quay camera theo chieu kim dong ho\n";
    cout << "->        : Quay camera nguoc chieu kim dong ho\n";

    glutInit(&argc, (char**)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tu Mai The Nhan - 2114277");

    myInit();

    create();

    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecialKeyboard);
    glutMainLoop();

    return 0;
}

