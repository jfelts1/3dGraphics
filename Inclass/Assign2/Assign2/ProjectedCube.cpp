#include "ProjectedCube.h"

using namespace glm;
using std::function;
using std::pair;
using std::cout;
using std::cin;
using std::endl;

// Choose whether the back face will be enabled or not
inline void cullToggle()
{
	if(cull)
	{
		cout << "Cull enabled" << endl;
		glEnable(GL_CULL_FACE);
	}
	else
	{
		cout << "Cull disabled" << endl;
		glDisable(GL_CULL_FACE);
	}
}

// Choose whether to draw in wireframe mode or not
inline void lineToggle()
{
	if(line)
	{
		cout << "Line enabled" << endl;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		cout << "Line disabled" << endl;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

inline void orthgraphicToggle()
{
	if(orthographic)
	{
        cout << "Orthographic enabled" << endl;
        view_matrix = lookAt(eye, center, vec3(0.0f, 1.0f, 0.0f));
        projection_matrix = ortho(-1.0f, 1.0f, -1.0f, 1.0f, zNear, zFar);
	}
	else
	{
        cout << "Orthographic disabled" << endl;
        view_matrix = lookAt(eye, center, vec3(0.0f, 1.0f, 0.0f));
        projection_matrix = frustum(-1.0f, 1.0f, -1.0f, 1.0f, zNear, zFar);
	}
}

inline void isometricToggle()
{
	if(isometric)
	{
        orthographic = true;
        orthgraphicToggle();
		cout << "Isometric enabled" << endl;
        view_matrix = lookAt(vec3(2.0f,2.0f,2.0f), center, vec3(0.0f, 1.0f, 0.0f));
	}
}

inline void dimetricToggle()
{
	if(dimetric)
	{
        orthographic = true;
        orthgraphicToggle();
		cout << "Dimetric enabled" << endl;
        view_matrix = lookAt(vec3(2.0f,1.0f,2.0f), center, vec3(0.0f, 1.0f, 0.0f));
	}
}

inline void trimetricToggle()
{
	if(trimetric)
	{
        orthographic = true;
        orthgraphicToggle();
		cout << "Trimetric enabled" << endl;
        view_matrix = lookAt(vec3(1.0f,2.0f,3.0f), center, vec3(0.0f, 1.0f, 0.0f));
	}
}

inline void onePointToggle()
{
	if(onePoint)
	{
        orthographic = false;
        orthgraphicToggle();
        cout << "One Point enabled" << endl;
        view_matrix = lookAt(vec3(0.0f,0.0f,2.0f), center, vec3(0.0f, 1.0f, 0.0f));
	}
}

inline void twoPointToggle()
{
	if (twoPoint)
	{
        orthographic = false;
        orthgraphicToggle();
		cout << "Two Point enabled" << endl;
        view_matrix = lookAt(vec3(2.0f,0.0f,2.0f), center, vec3(0.0f, 1.0f, 0.0f));
	}
}

inline void threePointToggle()
{
	if (threePoint)
	{
        orthographic = false;
        orthgraphicToggle();
		cout << "Three Point enabled" << endl;
        view_matrix = lookAt(vec3(2.0f,2.0f,2.0f), center, vec3(0.0f, 1.0f, 0.0f));
	}
}

void Initialize(void)
{
	initCubeLocs();

	GLuint offset = 0;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);
	glGenBuffers(1, &cube_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices) + sizeof(cube_normals), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(cube_vertices), cube_vertices);
	offset += sizeof(cube_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(cube_normals), cube_normals);


	glGenBuffers(1, &cube_ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(loc);

	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(sizeof(cube_vertices)));
	glEnableVertexAttribArray(normal_loc);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}

void Display(void)
{
    //glFrontFace(GL_CW);
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Setup

	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

	cullToggle();
	lineToggle();
	orthgraphicToggle();
	isometricToggle();
	dimetricToggle();
	trimetricToggle();
	onePointToggle();
	twoPointToggle();
	threePointToggle();

	// Setup view matrix

    //view_matrix = lookAt(eye, center, vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(static_cast<GLint>(view_matrix_loc), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&view_matrix[0]));


	// Choose the type of Projection

    glUniformMatrix4fv(
                static_cast<GLint>(projection_matrix_loc), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&projection_matrix[0]));
	mat4 model_matrix;
	glUniformMatrix4fv(static_cast<GLint>(matrix_loc), 1, GL_FALSE, reinterpret_cast<GLfloat*>(&model_matrix[0]));


	glBindVertexArray(cube_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	aspect = float(width) / float(height);
}

void printMatrix(mat4& mat)
{
	for(int i =0;i<4;i++)
	{
		for(int j =0;j<4;j++)
		{
			printf("%f ",mat[i][j]);
		}
		printf("\n");
	}
}

// ReSharper disable once CppParameterNeverUsed
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 'o':
		orthographic = !orthographic;
		break;
	case 's':
		line = !line;
		break;
	case 'c':
		cull = !cull;
		break;
	
	case 'i':
		// implement isometric view
		isometric = !isometric;
		break;
	case 'd':
		//implement dimetric view
		dimetric = !dimetric;
		break;
	case 't':
		// implement trimetric view
		trimetric = !trimetric;
		break;
	case '1':
		//implement one-point perspective 
		onePoint = !onePoint;
		break;
	case '2':
		//implement two-point perspective
		twoPoint = !twoPoint;
		break;
	case '3':
		// implement three point perspective
		threePoint = !threePoint;
		break;

	}
	glutPostRedisplay();
}

void initAttributeIndices()
{
	loc = static_cast<GLuint>(glGetAttribLocation(program, "position"));
	normal_loc = static_cast<GLuint>(glGetAttribLocation(program, "normal"));
	projection_matrix_loc = static_cast<GLuint>(glGetUniformLocation(program, "projection_matrix"));
}

void initUniformIndices()
{
	view_matrix_loc = static_cast<GLuint>(glGetUniformLocation(program, "view_matrix"));
	matrix_loc = static_cast<GLuint>(glGetUniformLocation(program, "model_matrix"));
}

void initCubeLocs()
{
	program = Utils::initShaders("shader_projected.vs", "shader_projected.fs");
	initAttributeIndices();
	initUniformIndices();
}

/*********/
int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;
    glutInitContextVersion(3,3);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(SCREEN_SIZE_X, SCREEN_SIZE_Y);

	glutCreateWindow("ProjectedCube");

	if (glewInit())
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << endl;
	}

	Initialize();
	cout << glGetString(GL_VERSION) << endl;
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return EXIT_SUCCESS;
}




/*************/



