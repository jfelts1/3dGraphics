#include "disk.h"

extern GLuint program;
extern GLuint gouraudProgram;

void Disk::initializeCone(const float yValue)
{
	//top point of cone
	points[index][0] = 0.0;
	points[index][1] = yValue;
	points[index][2] = 0.0;
	//points[index][3] = 1.0;

	normals[index][0] = 0.0;
	normals[index][1] = 0.0;
	normals[index][2] = 0.0;

	index++;

	float theta;
	size_t tIndices = 0;

	for (size_t i = 0; i < NumConePoints; ++i, index++)
	{
		theta = static_cast<float>(i*20.0f*kPI / 180.0f);

		points[index][0] = 10 * cos(theta);
		points[index][1] = yValue;
		points[index][2] = 10 * -sin(theta);
		//points[index][3] = 1.0;

		normals[index][0] = 0.0;
		normals[index][1] = 0.0;
		normals[index][2] = 0.0;

		if (i <= (NumConePoints - 2))
		{
			indices[tIndices] = 0u;
			tIndices++;
			indices[tIndices] = index;
			tIndices++;
			indices[tIndices] = index + 1;
			tIndices++;
		}
		//last triangle
		else
		{
			indices[tIndices] = 0u;
			tIndices++;
			indices[tIndices] = index;
			tIndices++;
			indices[tIndices] = 1u;
			tIndices++;
		}
	}
	calculateNormals();
}

void Disk::calculateNormals()
{
	std::vector<glm::vec3> vertices;
	for (auto& p : points)
	{
		vertices.emplace_back(p);
	}
	std::vector<int> ind;
	for (auto& i : indices)
	{
		ind.emplace_back(i);
	}

	for (size_t i = 0;i < points.size();i++)
	{
		std::vector<glm::vec3> normalsVec = getAdjacentTriangleNormals(i, vertices, ind);
		normals[i] = computeVertexNormal(normalsVec);
		//printf("%llu: %f,%f,%f\n",i,normals[i][0],normals[i][1],normals[i][2]);
		normalsVec.clear();
	}
}

void Disk::renderInit()
{
	GLuint offset = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(points), points.data());
	offset += sizeof(points);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(normals), normals.data());

	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	// attribute indices
	vertexPosition = static_cast<GLuint>(glGetAttribLocation(program, "VertexPosition"));
	glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(vertexPosition);

	vertexNormal = static_cast<GLuint>(glGetAttribLocation(program, "VertexNormal"));
	glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(sizeof(points)));
	glEnableVertexAttribArray(vertexNormal);

	vertexPosition = static_cast<GLuint>(glGetAttribLocation(gouraudProgram, "VertexPosition"));
	glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(vertexPosition);

	vertexNormal = static_cast<GLuint>(glGetAttribLocation(gouraudProgram, "VertexNormal"));
	glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(sizeof(points)));
	glEnableVertexAttribArray(vertexNormal);
}

void Disk::display() const
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);


	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, nullptr);
}