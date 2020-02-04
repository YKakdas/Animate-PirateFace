#include <iostream>
#include "Angel.h"
#include <Math.h>

using namespace Angel;

typedef vec4 color4;
typedef vec4 point4;

const int NumVertices = 93;
const double PI = 3.141592653589793238463;

const GLint width = 500;
const GLint height = 500;

point4 points[NumVertices];
color4 colors[NumVertices];

point4 circle[30];
color4 color = { 1.0,0.0,0.0,1.0 };
point4 center = { 0.0,0.0,0.0,1.0 };

GLfloat radius = 0.3;

/* vertices of LShape
	1---2
	 - -
	 - -
	 - -3
	 -------------4
	0-------------5
*/

int Index = 0;

void triangle(int b, int c) {
	colors[Index] = color; points[Index] = center; Index++;
	colors[Index] = color; points[Index] = circle[b %30]; Index++;
	colors[Index] = color; points[Index] = circle[c%30]; Index++;
}

void fill() {
	for (int i = 0; i < 30; i++)
	{
		float angle = 2 * PI * i / 30;
		point4 point;
		point.x = center.x + cos(angle) * radius;
		point.y = center.y + sin(angle) * radius;
		point.z = 0.6;
		circle[i] = point;
	}

	for (int i = 0; i <= 29; i++) {
		triangle(i , i + 1);
	}
}

void init()
{
	fill();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glutSwapBuffers();
}

void myMouse(int btn, int state, int x, int y) {
	
}

void myKeyboard(unsigned char key, int x, int y) {
	
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("LShape");
	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}