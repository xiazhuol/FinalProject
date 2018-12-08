#include "stdafx.h"
#include <fstream>      // For input streams from files
#include <string>       // For the string type
#include <sstream>      // For streams from strings
#include "Mesh.h"
using namespace std;


CMesh::CMesh(void)
{
}


CMesh::~CMesh(void)
{
}


void CMesh::InitGL()
{
    for(PTV v=m_triangles.begin();  v!=m_triangles.end();  v++)
    {
		normalArray.push_back(m_normals[v->n]);
		vertexArray.push_back(m_vertices[v->v]);
		texArray.push_back(m_tvertices[v->t]);
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexArray.size()*sizeof(glm::vec3), 
		&vertexArray[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &m_normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
	glBufferData(GL_ARRAY_BUFFER, normalArray.size()*sizeof(glm::vec3),
		&normalArray[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &m_texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_texVBO);
	glBufferData(GL_ARRAY_BUFFER, texArray.size()*sizeof(glm::vec2),
		&texArray[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void CMesh::RenderGL()
{
	glUseProgram(m_program);
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexArray.size());
}

void CMesh::AddTriangleVertex(int v, int n, int t)
{
    TV tv;
    tv.v = v;
    tv.n = n;
    tv.t = t;
    m_triangles.push_back(tv);
}

//
// Name :         CMesh::AddFlatQuad()
// Description :  Add a quadrangle to the triangle mesh.
//
void CMesh::AddFlatQuad(int a, int b, int c, int d, int n)
{
	
	 // First triangle
    AddTriangleVertex(a, n, -1);
    AddTriangleVertex(b, n, -1);
    AddTriangleVertex(c, n, -1);

    // Second triangle
    AddTriangleVertex(a, n, -1);
    AddTriangleVertex(c, n, -1);
    AddTriangleVertex(d, n, -1);
}


void CMesh::AddQuad(int a, int b, int c, int d)
{
	 // First triangle
    AddTriangleVertex(a, a, -1);
    AddTriangleVertex(b, b, -1);
    AddTriangleVertex(c, c, -1);

    // Second triangle
    AddTriangleVertex(a, a, -1);
    AddTriangleVertex(c, c, -1);
    AddTriangleVertex(d, d, -1);
}


//
// Name :         CMesh::ComputeSmoothNormals()
// Description :  Compute a normal for each vertex that is the
//                average of the surface normals incident on the 
//                vertex.
//
void CMesh::ComputeSmoothNormals(void)
{
	m_normals.resize(m_vertices.size());
    for(unsigned int i=0;  i<m_vertices.size();  i++)
        m_normals[i] = glm::vec3(0, 0, 0);

	 for(PTV v=m_triangles.begin();  v!=m_triangles.end();  )
    {
        // Collect up the vertices of a triangle...
        int a = v->v;
        v++;
        int b = v->v;
        v++;
        int c = v->v;
        v++;

        // Surface normal
        glm::vec3 normal = glm::cross(m_vertices[b] - m_vertices[a], 
                                 m_vertices[c] - m_vertices[a]);
        normal *=1/normal.length();

        // Add to the incident vertices normals
        m_normals[a] += normal;
        m_normals[b] += normal;
        m_normals[c] += normal;

		 // Normalize the normals
    for(unsigned int i=0;  i<m_vertices.size();  i++)
        m_normals[i] *= 1/m_normals[i].length();
}
}

void CMesh::LoadOBJ(const char* filename)
{
	 ifstream str(filename);
    if(!str)
    {
        AfxMessageBox(L"File not found");
        return;
    }

    string line;
    while(getline(str, line))
    {
        istringstream lstr(line);

        string code;
        lstr >> code;
        if(code == "v") 
        {
            double x, y, z;
            lstr >> x >> y >> z;
            AddVertex(glm::vec3(x, y, z));
        }
        else if(code == "vn")
        {
			double a, b, c;
            lstr >> a >> b >> c;
            AddNormal(glm::vec3(a, b, c));	
        }
        else if(code == "vt")
        {
			double s, t;
            lstr >> s >> t;
            AddTexCoord(glm::vec2(s, t));
        }
        else if(code == "f")
        {
			for(int i=0;  i<3;  i++)
            {
                char slash;
                int v, t, n;
                lstr >> v >> slash >> t >> slash >> n;
                AddTriangleVertex(v-1, n-1, t-1);
            }
        }

    }
}

void CMesh::CreateCube()
{
		AddVertex(glm::vec3(-0.5, -0.5, 0.5));
		AddVertex(glm::vec3(-0.5, 0.5, 0.5));
		AddVertex(glm::vec3(0.5, 0.5, 0.5));
		AddVertex(glm::vec3(0.5, -0.5, 0.5));
		AddVertex(glm::vec3(-0.5, -0.5, -0.5));
		AddVertex(glm::vec3(-0.5, 0.5, -0.5));
		AddVertex(glm::vec3(0.5, 0.5, -0.5));
		AddVertex(glm::vec3(0.5, -0.5, -0.5));

		AddNormal(glm::vec3(1, 0, 0));
		AddNormal(glm::vec3(-1, 0, 0));
		AddNormal(glm::vec3(0, 1, 0));
		AddNormal(glm::vec3(0, -1, 0));
		AddNormal(glm::vec3(0, 0, 1));
		AddNormal(glm::vec3(0, 0, -1));

		AddTexCoord(glm::vec2(0, 0));
		AddTexCoord(glm::vec2(1, 0));
		AddTexCoord(glm::vec2(0, 1));
		AddTexCoord(glm::vec2(1, 1));

		AddTriangleVertex(1, 5, 0);
		AddTriangleVertex(0, 5, 1);
		AddTriangleVertex(3, 5, 2);
		AddTriangleVertex(1, 5, 0);
		AddTriangleVertex(3, 5, 2);
		AddTriangleVertex(2, 5, 3);
		AddTriangleVertex(2, 0, 0);
		AddTriangleVertex(3, 0, 1);
		AddTriangleVertex(7, 0, 2);
		AddTriangleVertex(2, 0, 0);
		AddTriangleVertex(7, 0, 2);
		AddTriangleVertex(6, 0, 3);
		AddTriangleVertex(3, 3, 0);
		AddTriangleVertex(0, 3, 1);
		AddTriangleVertex(4, 3, 2);
		AddTriangleVertex(3, 3, 0);
		AddTriangleVertex(4, 3, 2);
		AddTriangleVertex(7, 3, 3);
		AddTriangleVertex(6, 2, 0);
		AddTriangleVertex(5, 2, 1);
		AddTriangleVertex(1, 2, 2);
		AddTriangleVertex(6, 2, 0);
		AddTriangleVertex(1, 2, 2);
		AddTriangleVertex(2, 2, 3);
		AddTriangleVertex(4, 5, 0);
		AddTriangleVertex(5, 5, 1);
		AddTriangleVertex(6, 5, 2);
		AddTriangleVertex(4, 5, 0);
		AddTriangleVertex(6, 5, 2);
		AddTriangleVertex(7, 5, 3);
		AddTriangleVertex(5, 1, 0);
		AddTriangleVertex(4, 1, 1);
		AddTriangleVertex(0, 1, 2);
		AddTriangleVertex(5, 1, 0);
		AddTriangleVertex(0, 1, 2);
		AddTriangleVertex(1, 1, 3);
}



void CMesh::CreateLetter1() 
{
	AddVertex(glm::vec3(0, 12, 7));//b
	AddVertex(glm::vec3(0, 14, 7));//a
	AddVertex(glm::vec3(10, 14, 7));//d
	AddVertex(glm::vec3(10, 12, 7));//c
	
	/*AddVertex(glm::vec3(0., 2, 7));//e
	AddVertex(glm::vec3(10, 2, 7));//f
	AddVertex(glm::vec3(0, 0, 7));//g
	AddVertex(glm::vec3(10, 0, 7));//h
	AddVertex(glm::vec3(8, 12, 7));//i
	AddVertex(glm::vec3(2, 2, 7));//j*/
	AddVertex(glm::vec3(0, 12, 4));//b1
	AddVertex(glm::vec3(0, 14, 4));//a1
	AddVertex(glm::vec3(10, 14, 4));//d1
	AddVertex(glm::vec3(10, 12, 4));//c1
	
	/*AddVertex(glm::vec3(0., 2, 4));//e1
	AddVertex(glm::vec3(10, 2, 4));//f1
	AddVertex(glm::vec3(0, 0, 4));//g1
	AddVertex(glm::vec3(10, 0, 4));//h1
	AddVertex(glm::vec3(8, 12, 4));//i1
	AddVertex(glm::vec3(2, 2, 4));//j1*/

	AddNormal(glm::vec3(1, 0, 0));
	AddNormal(glm::vec3(-1, 0, 0));
	AddNormal(glm::vec3(0, 1, 0));
	AddNormal(glm::vec3(0, -1, 0));
	AddNormal(glm::vec3(0, 0, 1));
	AddNormal(glm::vec3(0, 0, -1));

	AddTexCoord(glm::vec2(0, 0));
	AddTexCoord(glm::vec2(1, 0));
	AddTexCoord(glm::vec2(0, 1));
	AddTexCoord(glm::vec2(1, 1));


	AddTriangleVertex(1, 5, 0);
	AddTriangleVertex(0, 5, 1);
	AddTriangleVertex(3, 5, 2);
	AddTriangleVertex(1, 5, 0);
	AddTriangleVertex(3, 5, 2);
	AddTriangleVertex(2, 5, 3);
	AddTriangleVertex(2, 0, 0);
	AddTriangleVertex(3, 0, 1);
	AddTriangleVertex(7, 0, 2);
	AddTriangleVertex(2, 0, 0);
	AddTriangleVertex(7, 0, 2);
	AddTriangleVertex(6, 0, 3);
	AddTriangleVertex(3, 3, 0);
	AddTriangleVertex(0, 3, 1);
	AddTriangleVertex(4, 3, 2);
	AddTriangleVertex(3, 3, 0);
	AddTriangleVertex(4, 3, 2);
	AddTriangleVertex(7, 3, 3);
	AddTriangleVertex(6, 2, 0);
	AddTriangleVertex(5, 2, 1);
	AddTriangleVertex(1, 2, 2);
	AddTriangleVertex(6, 2, 0);
	AddTriangleVertex(1, 2, 2);
	AddTriangleVertex(2, 2, 3);
	AddTriangleVertex(4, 5, 0);
	AddTriangleVertex(5, 5, 1);
	AddTriangleVertex(6, 5, 2);
	AddTriangleVertex(4, 5, 0);
	AddTriangleVertex(6, 5, 2);
	AddTriangleVertex(7, 5, 3);
	AddTriangleVertex(5, 1, 0);
	AddTriangleVertex(4, 1, 1);
	AddTriangleVertex(0, 1, 2);
	AddTriangleVertex(5, 1, 0);
	AddTriangleVertex(0, 1, 2);
	AddTriangleVertex(1, 1, 3);
	/*glEnable(GL_CULL_FACE);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	glNormal3d(0., 0., 1.);



	Z_front(); // Front

	glDisable(GL_CULL_FACE);



	glEnable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glNormal3d(1., 0., 0.);

	glNormal3d(-1., 0., 0);



	z_side(); // side
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glNormal3d(0., 0, -1.);
	z_back(); // back
	glDisable(GL_TEXTURE_2D);*/
	
}
void CMesh::CreateLetter2()
{
	
	AddVertex(glm::vec3(2, 2, 7));//j
	AddVertex(glm::vec3(0., 2, 7));//e
	AddVertex(glm::vec3(8, 12, 7));//i
	AddVertex(glm::vec3(10, 12, 7));//c
	AddVertex(glm::vec3(2, 2, 4));//j1
	AddVertex(glm::vec3(0., 2, 4));//e1
	AddVertex(glm::vec3(8, 12, 4));//i1
	AddVertex(glm::vec3(10, 12, 4));//c1
	

	AddNormal(glm::vec3(1, 0, 0));
	AddNormal(glm::vec3(-1, 0, 0));
	AddNormal(glm::vec3(0, 1, 0));
	AddNormal(glm::vec3(0, -1, 0));
	AddNormal(glm::vec3(0, 0, 1));
	AddNormal(glm::vec3(0, 0, -1));

	AddTexCoord(glm::vec2(0, 0));
	AddTexCoord(glm::vec2(1, 0));
	AddTexCoord(glm::vec2(0, 1));
	AddTexCoord(glm::vec2(1, 1));


	AddTriangleVertex(1, 5, 0);
	AddTriangleVertex(0, 5, 1);
	AddTriangleVertex(3, 5, 2);
	AddTriangleVertex(1, 5, 0);
	AddTriangleVertex(3, 5, 2);
	AddTriangleVertex(2, 5, 3);
	AddTriangleVertex(2, 0, 0);
	AddTriangleVertex(3, 0, 1);
	AddTriangleVertex(7, 0, 2);
	AddTriangleVertex(2, 0, 0);
	AddTriangleVertex(7, 0, 2);
	AddTriangleVertex(6, 0, 3);
	AddTriangleVertex(3, 3, 0);
	AddTriangleVertex(0, 3, 1);
	AddTriangleVertex(4, 3, 2);
	AddTriangleVertex(3, 3, 0);
	AddTriangleVertex(4, 3, 2);
	AddTriangleVertex(7, 3, 3);
	AddTriangleVertex(6, 2, 0);
	AddTriangleVertex(5, 2, 1);
	AddTriangleVertex(1, 2, 2);
	AddTriangleVertex(6, 2, 0);
	AddTriangleVertex(1, 2, 2);
	AddTriangleVertex(2, 2, 3);
	AddTriangleVertex(4, 5, 0);
	AddTriangleVertex(5, 5, 1);
	AddTriangleVertex(6, 5, 2);
	AddTriangleVertex(4, 5, 0);
	AddTriangleVertex(6, 5, 2);
	AddTriangleVertex(7, 5, 3);
	AddTriangleVertex(5, 1, 0);
	AddTriangleVertex(4, 1, 1);
	AddTriangleVertex(0, 1, 2);
	AddTriangleVertex(5, 1, 0);
	AddTriangleVertex(0, 1, 2);
	AddTriangleVertex(1, 1, 3);


}
void CMesh::CreateLetter3()
{
	
	AddVertex(glm::vec3(0., 2, 7));//e
	AddVertex(glm::vec3(0, 0, 7));//g
	AddVertex(glm::vec3(10, 0, 7));//h
	AddVertex(glm::vec3(10, 2, 7));//f
	AddVertex(glm::vec3(0., 2, 4));//e1
	AddVertex(glm::vec3(0, 0, 4));//g1
	AddVertex(glm::vec3(10, 0, 4));//h1
	AddVertex(glm::vec3(10, 2, 4));//f1
	

	

	AddNormal(glm::vec3(1, 0, 0));
	AddNormal(glm::vec3(-1, 0, 0));
	AddNormal(glm::vec3(0, 1, 0));
	AddNormal(glm::vec3(0, -1, 0));
	AddNormal(glm::vec3(0, 0, 1));
	AddNormal(glm::vec3(0, 0, -1));

	AddTexCoord(glm::vec2(0, 0));
	AddTexCoord(glm::vec2(1, 0));
	AddTexCoord(glm::vec2(0, 1));
	AddTexCoord(glm::vec2(1, 1));


	AddTriangleVertex(1, 5, 0);
	AddTriangleVertex(0, 5, 1);
	AddTriangleVertex(3, 5, 2);
	AddTriangleVertex(1, 5, 0);
	AddTriangleVertex(3, 5, 2);
	AddTriangleVertex(2, 5, 3);
	AddTriangleVertex(2, 0, 0);
	AddTriangleVertex(3, 0, 1);
	AddTriangleVertex(7, 0, 2);
	AddTriangleVertex(2, 0, 0);
	AddTriangleVertex(7, 0, 2);
	AddTriangleVertex(6, 0, 3);
	AddTriangleVertex(3, 3, 0);
	AddTriangleVertex(0, 3, 1);
	AddTriangleVertex(4, 3, 2);
	AddTriangleVertex(3, 3, 0);
	AddTriangleVertex(4, 3, 2);
	AddTriangleVertex(7, 3, 3);
	AddTriangleVertex(6, 2, 0);
	AddTriangleVertex(5, 2, 1);
	AddTriangleVertex(1, 2, 2);
	AddTriangleVertex(6, 2, 0);
	AddTriangleVertex(1, 2, 2);
	AddTriangleVertex(2, 2, 3);
	AddTriangleVertex(4, 5, 0);
	AddTriangleVertex(5, 5, 1);
	AddTriangleVertex(6, 5, 2);
	AddTriangleVertex(4, 5, 0);
	AddTriangleVertex(6, 5, 2);
	AddTriangleVertex(7, 5, 3);
	AddTriangleVertex(5, 1, 0);
	AddTriangleVertex(4, 1, 1);
	AddTriangleVertex(0, 1, 2);
	AddTriangleVertex(5, 1, 0);
	AddTriangleVertex(0, 1, 2);
	AddTriangleVertex(1, 1, 3);

	
}