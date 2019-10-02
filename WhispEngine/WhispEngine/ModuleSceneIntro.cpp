#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "Imgui/imgui.h"
#include "par_shapes.h" //https://prideout.net/shapes

#include "MathGeoLib/include/MathGeoLib.h"
#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/lib_x86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "MathGeoLib/lib_x86/Release/MathGeoLib.lib")
#endif


ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	App->camera->Move(vec3(5.0f, 3.0f, 5.0f));
	App->camera->LookAt(vec3(0.f, 0.f, 0.f));


	// Generate a sphere from a subdivided icosahedron, which produces a nicer
	// distribution of triangles, but no texture coordinates.
	//par_shapes_mesh* par_shapes_create_subdivided_sphere(int nsubdivisions);

	// Piramid
	/*int m = 0;
	int n = 2;*/

	//float piramid_vertices[54 * 3]{
	//	m,0,m,
	//	n,0,m,
	//	n,0,n,

	//	n,0,n,
	//	m,0,n,
	//	m,0,m,
	//	//---
	//	n,0,m,
	//	m,0,m,
	//	n/2, n, n/2,
	//	//---
	//	n,0,n,
	//	n,0,m,
	//	n / 2, n, n / 2,
	//	//---
	//	n,0,n,
	//	n / 2, n, n / 2,
	//	m,0,n,
	//	//---
	//	m,0,n,
	//	n / 2, n, n / 2,
	//	m,0,m
	//};

	// Cube
	int i = 0;
	int j = 2;

	float cube_vertices[8*3]{
		j,j,i, //v0
		i,j,i, //v1
		i,i,i, //v2
		j,i,i, //v3

		i,i,j, //v4
		i,j,j, //v5
		j,j,j, //v6
		j,i,j, //v7
	};
	//float cube_vertices[54*3]{
	//	i,i,i,
	//	i,j,i,
	//	j,i,i,

	//	j,i,i,
	//	i,j,i,
	//	j,j,i,
	//	//---
	//	i,i,j,
	//	j,i,j,
	//	i,j,j,

	//	j,i,j,
	//	j,j,j,
	//	i,j,j,
	//	//---
	//	i,j,i,
	//	i,j,j,
	//	j,j,i,

	//	j,j,i,
	//	i,j,j,
	//	j,j,j,
	//	//---
	//	i,i,i,
	//	j,i,i,
	//	i,i,j,

	//	j,i,i,
	//	j,i,j,
	//	i,i,j,
	//	//---
	//	i,i,i,
	//	i,i,j,
	//	i,j,j,

	//	i,i,i,
	//	i,j,j,
	//	i,j,i,
	//	//---
	//	j,i,i,
	//	j,j,j,
	//	j,i,j,

	//	j,i,i,
	//	j,j,i,
	//	j,j,j
	//};
	uint cube_index[36]{
		0,1,2, 2,3,0,   // 36 of indices
		0,3,4, 4,5,0,
		0,5,6, 6,1,0,
		1,6,7, 7,2,1,
		7,4,3, 3,2,7,
		4,7,6, 6,5,4
	};

	


	glGenBuffers(1, (GLuint*) &index_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*cube_num_index, cube_index, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(vao));
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*cube_num_vertices * 3, cube_vertices, GL_STATIC_DRAW);
	
	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{	
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, vao);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//// … draw other buffers
	//glDrawArrays(GL_TRIANGLES, 0, piramid_num_vertices);
	//glDisableClientState(GL_VERTEX_ARRAY);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, cube_num_index, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBegin(GL_LINES);

	for (int i = 0; i <= 100; ++i)
	{
		
		glVertex3f(i, 0, 0);
		glVertex3f(i, 0, 100);

		glVertex3f(100, 0, i);
		glVertex3f(0, 0, i);


		glColor3f(0.f, 1.f, 1.f);

	}	
	glEnd();


	//par_shapes_mesh* par_shapes_create_parametric_sphere(int slices, int stacks);

	return UPDATE_CONTINUE;
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}