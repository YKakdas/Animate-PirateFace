#include "PirateFace.h"

void quad(int a, int b, int c) {
	pirateFaceVertices.push_back(pirateFaceVertices[a]);
	pirateFaceVertices.push_back(pirateFaceVertices[b]);
	pirateFaceVertices.push_back(pirateFaceVertices[c]);
}

void fillPointsOfPirateFace() {
	for (int i = 0; i < pirateFace.numberOfVerticesOfCircle; i++)
	{
		float angle = 2 * PI * i / pirateFace.numberOfVerticesOfCircle;
		point3 point;
		point.x = pirateFace.centerOfFace.x + cos(angle) * pirateFace.faceRadius;
		point.y = pirateFace.centerOfFace.y + sin(angle) * pirateFace.faceRadius;
		point.z = 0.0;
		pirateFaceVertices.push_back(point);
	}

	quad(40, 140, 145);
	quad(145, 35, 40);

	for (int i = 0; i < pirateFace.numberOfVerticesOfCircle; i++)
	{
		float angle = 2 * PI * i / pirateFace.numberOfVerticesOfCircle;
		point3 point;
		point.x = -pirateFace.centerOfEye.x + cos(angle) * pirateFace.eyeRadius;
		point.y = pirateFace.centerOfEye.y + sin(angle) * pirateFace.eyeRadius;
		point.z = 0.0;
		pirateFaceVertices.push_back(point);
	}

	for (int i = 0; i < pirateFace.numberOfVerticesOfCircle;)
	{
		pirateFaceVertices.push_back(pirateFace.centerOfEye);
		float angle = 2 * PI * i / pirateFace.numberOfVerticesOfCircle;
		point3 point;
		point.x = pirateFace.centerOfEye.x + cos(angle) * pirateFace.eyeRadius;
		point.y = pirateFace.centerOfEye.y + sin(angle) * pirateFace.eyeRadius;
		point.z = 0.0;
		pirateFaceVertices.push_back(point);
		i++;
		angle = 2 * PI * i / pirateFace.numberOfVerticesOfCircle;
		point.x = pirateFace.centerOfEye.x + cos(angle) * pirateFace.eyeRadius;
		point.y = pirateFace.centerOfEye.y + sin(angle) * pirateFace.eyeRadius;
		point.z = 0.0;
		pirateFaceVertices.push_back(point);
		if (i == 135) {
			pirateFaceVertices.push_back({ 0.155,0.12,0.0 });
			pirateFaceVertices.push_back({ 0.155,0.2,0.0 });
			pirateFaceVertices.push_back({ 0.105,0.2,0.0 });
			pirateFaceVertices.push_back({ 0.105,0.2,0.0 });
			pirateFaceVertices.push_back({ 0.105,0.12,0.0 });
			pirateFaceVertices.push_back({ 0.155,0.12,0.0 });
		}
	}
	
	for (int i = pirateFace.numberOfVerticesOfSmile+3; i < pirateFace.numberOfVerticesOfSmile*2-2; i++)
	{
		float angle = 2 * PI * i / (pirateFace.numberOfVerticesOfSmile * 2);
		point3 point;
		point.x = pirateFace.centerOfSmile.x + cos(angle) * pirateFace.smileRadius;
		point.y = pirateFace.centerOfSmile.y + sin(angle) * pirateFace.smileRadius;
		point.z = 0.0;
		pirateFaceVertices.push_back(point);
	}
}

void init()
{
	fillPointsOfPirateFace();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*pirateFaceVertices.size() + sizeof(vec3),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*pirateFaceVertices.size(), pirateFaceVertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*pirateFaceVertices.size(), sizeof(vec3), color);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(point3)*pirateFaceVertices.size()));

	uniformScalePos = glGetUniformLocation(program, "scale");
	uniformTranslatePos = glGetUniformLocation(program, "translate");
	uniformRotatePos = glGetUniformLocation(program, "rotate");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform3fv(uniformScalePos, 1, scaleVec);
	glUniform3fv(uniformTranslatePos, 1, translateVec);
	glUniform3fv(uniformRotatePos, 1, rotateVec);
	glDrawArrays(GL_LINE_LOOP, 0, pirateFace.numberOfVerticesOfCircle);
	glDrawArrays(GL_TRIANGLES, pirateFace.numberOfVerticesOfCircle, pirateFace.numberOfVerticesOfBandanna);
	glDrawArrays(GL_LINE_LOOP, pirateFace.numberOfVerticesOfCircle + pirateFace.numberOfVerticesOfBandanna,
		pirateFace.numberOfVerticesOfCircle);
	glDrawArrays(GL_TRIANGLES, pirateFace.numberOfVerticesOfCircle * 2 + pirateFace.numberOfVerticesOfBandanna,
		pirateFace.numberOfVerticesOfCircle*3+ pirateFace.numberOfVerticesOfBandanna);	
	glDrawArrays(GL_LINE_STRIP, pirateFace.numberOfVerticesOfCircle * 5 + pirateFace.numberOfVerticesOfBandanna*2,
		pirateFace.numberOfVerticesOfSmile-5);
	glutSwapBuffers();
}


GLfloat theta = 0.0;
void animate(int id) {
	
	if (theta < -360) {
		return;
	}

	scaleVec = { 0.5,0.5,0.5 };
	rotateVec = { 0.0,0.0,theta };
	translateVec = { translateAmount,0.0,0.0 };

	translateAmount += 0.15;
	theta -= 360 / n;

	glutPostRedisplay();

	glutTimerFunc(700, animate, 0);
}

void myKeyboard(unsigned char key, int x, int y) {
	if (key == 'a') {
		glutTimerFunc(700, animate, 0);
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pirate Face");
	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutKeyboardFunc(myKeyboard);
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}