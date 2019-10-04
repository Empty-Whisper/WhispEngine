#include "GameObject.h"
#include "Application.h"


GameObject::GameObject()
{
	/*glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n_points, points, GL_STATIC_DRAW);

	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * n_index * 3, index, GL_STATIC_DRAW);*/
}


GameObject::~GameObject()
{
}

void GameObject::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, n_index * 3, GL_UNSIGNED_SHORT, NULL);
}
