#pragma comment(lib, "glaux.lib")
#include <windows.h>
#include <stdlib.h> 
#include <time.h>
#include <GL/glut.h>
#include <math.h>
#include <GL/glaux.h>

float width_win = 700;
float higth_win = 700;
float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
float pos[] = { width_win / 2, higth_win / 2, 250.0, 0.0 };
float pos2[] = { -100, 0, 500.0, 0.0 };
float angel = 0.0;
float rot = 0.0;
float scal = 0.0;
//float z_rot = 0.0;
float ambient[4] = { 0.5, 0.5, 0.5, 1 };
GLfloat front_color[] = { 0, 1, 0, 1 };
GLfloat back_color[] = { 0, 0, 1, 1 };

unsigned int textures[3];



void LoadTexrures(){

	AUX_RGBImageRec *texture1 = auxDIBImageLoadA("circles2.bmp"),
	*texture2 = auxDIBImageLoadA("456.bmp"),
	*texture3 = auxDIBImageLoadA("789.bmp");

	glGenTextures(2, &textures[0]);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture2->sizeX, texture2->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2->data);

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture3->sizeX, texture3->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture3->data);

}



struct Coord{
	int x;
	int y;
	int z;
};

struct Color {
	double red;
	double green;
	double blue;
};

void drawParallelogram(Color color, Coord start, int heightX, int heightY, int heightZ, bool needTexture) {

	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor3f(color.red, color.green, color.blue);

	needTexture = true;

	//грань 1
	//glColor3f(1, 1, 1);
	if (needTexture) {
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glEnable(GL_TEXTURE_2D);
	}
	glBegin(GL_QUADS);
	if (needTexture)
		glTexCoord2f(0.5, 0);
	glVertex3f(start.x, start.y, start.z);
	if (needTexture)
		glTexCoord2f(1, 1);
	glVertex3f(start.x, start.y + heightY, start.z);
	if (needTexture)
		glTexCoord2f(1, 1);
	glVertex3f(start.x + heightX, start.y + heightY, start.z);
	if (needTexture)
		glTexCoord2f(0, 0);
	glVertex3f(start.x + heightX, start.y, start.z);
	glEnd();
	if (needTexture)
		glDisable(GL_TEXTURE_2D);

	needTexture = false;

	//грань 2
	if (needTexture) {
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glEnable(GL_TEXTURE_2D);
	}
	glBegin(GL_QUADS);
	if (needTexture)
		glTexCoord2f(1, 0);
	glVertex3f(start.x, start.y, start.z + heightZ);
	if (needTexture)
		glTexCoord2f(1, 1);
	glVertex3f(start.x, start.y + heightY, start.z + heightZ);
	if (needTexture)
		glTexCoord2f(0, 1);
	glVertex3f(start.x, start.y + heightY, start.z);
	if (needTexture)
		glTexCoord2f(0, 0);
	glVertex3f(start.x, start.y, start.z);
	glEnd();
	if (needTexture)
		glDisable(GL_TEXTURE_2D);


	//грань 3
	if (needTexture) {
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glEnable(GL_TEXTURE_2D);
	}
	glBegin(GL_QUADS);
	if (needTexture) 
		glTexCoord2f(1, 0); 
	glVertex3f(start.x + heightX, start.y, start.z + heightZ);
	if (needTexture)
		glTexCoord2f(0, 1); 
	glVertex3f(start.x + heightX, start.y + heightY, start.z + heightZ);
	if (needTexture)
		glTexCoord2f(1, 0); 
	glVertex3f(start.x, start.y + heightY, start.z + heightZ);
	if (needTexture)
		glTexCoord2f(0, 0); 
	glVertex3f(start.x, start.y, start.z + heightZ);
	glEnd();
	if (needTexture)
		glDisable(GL_TEXTURE_2D);


//	glColor3f(0, 0, 1);
	//грань 4
	if (needTexture) {
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glEnable(GL_TEXTURE_2D);
	}
	glBegin(GL_QUADS);
	if (needTexture)
		glTexCoord2f(1, 1);
	glVertex3f(start.x + heightX, start.y, start.z);
	if (needTexture)
		glTexCoord2f(0, 1);
	glVertex3f(start.x + heightX, start.y + heightY, start.z);
	if (needTexture)
		glTexCoord2f(1, 0);
	glVertex3f(start.x + heightX, start.y + heightY, start.z + heightZ);
	if (needTexture)
		glTexCoord2f(0, 0);
	glVertex3f(start.x + heightX, start.y, heightZ);
	glEnd();
	if (needTexture)
		glDisable(GL_TEXTURE_2D);

	//грань 5
//	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(start.x + heightX, start.y + heightY, start.z);
	glVertex3f(start.x, start.y + heightY, start.z);
	glVertex3f(start.x, start.y + heightY, start.z + heightZ);
	glVertex3f(start.x + heightX, start.y + heightY, start.z + heightZ);
	glEnd();


	//грань 6
	//glColor3f(1, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(start.x + heightX, start.y, start.z);
	glVertex3f(start.x + heightX, start.y, start.z + heightZ);
	glVertex3f(start.x, start.y, start.z + heightZ);
	glVertex3f(start.x, start.y, start.z);
	glEnd();


	glPopMatrix();
}

void solidCylinder(GLUquadric *qobj, GLdouble baseRadius, GLdouble topRadius,
	GLdouble height, GLint slices, GLint stacks) {
	gluCylinder(qobj, baseRadius, topRadius, height, slices, stacks);
	glRotatef(180, 1, 0, 0);
	gluDisk(qobj, 0.0f, baseRadius, slices, 1);
	glRotatef(180, 1, 0, 0);
	glTranslatef(0.0f, 0.0f, height);
	gluDisk(qobj, 0.0f, topRadius, slices, 1);
	glTranslatef(0.0f, 0.0f, -height);
}

void Draw_sh() {



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float size = 50;
	glEnable(GL_DEPTH_TEST);//проверка глубины
	GLUquadricObj *quadObj;
	quadObj = gluNewQuadric();

	// основание(фигура 2)
	Coord startFig2 = { 0, -200, 0 };
	Color color = { 0.3, 0.3, 0.3 };
	int lengthFig2X = 300, lengthFig2Y = 20, lengthFig2Z = 300;
	drawParallelogram(color, startFig2, lengthFig2X, lengthFig2Y, lengthFig2Z, false);

	// параллелограм над подставкой(фигура 3)
	color = { 0.8, 0, 0 };
	
	int marginRelativeFig2X = 50, marginRelativeFig2Z = 20;
	Coord startFig3 = {
		startFig2.x + marginRelativeFig2X,
		startFig2.y + lengthFig2Y,
		startFig2.z + marginRelativeFig2Z
	};
	int lengthFig3X = lengthFig2X - 2 * marginRelativeFig2X;
	int lengthFig3Y = 150;
	int lengthFig3Z = lengthFig2Z - 2 * marginRelativeFig2Z;
	drawParallelogram(color, startFig3, lengthFig3X, lengthFig2Y, lengthFig3Z, false);
	

	// параллелограм над параллелограмом над подставкой(фигура 4)
	color = { 0.7, 0, 0 };
	int marginRelativeFig3X = 20;
	Coord startFig4 = {
		startFig3.x + marginRelativeFig3X,
		startFig3.y + lengthFig2Y,
		startFig3.z
	};
	int lengthFig4X = lengthFig3X - marginRelativeFig3X;
	int lengthFig4Y = 125;
	int lengthFig4Z = lengthFig3Z;
	
	drawParallelogram(color, startFig4, lengthFig4X, lengthFig4Y, lengthFig4Z, true);
	

	// фигура 5
	color = { 0.6, 0, 0 };
	int marginRelativeFig4X = 20;
	Coord startFig5 = {
		startFig4.x + marginRelativeFig4X,
		startFig4.y + lengthFig4Y,
		startFig4.z
	};
	int lengthFig5X = lengthFig4X - marginRelativeFig4X;
	int lengthFig5Y = 80;
	int lengthFig5Z = lengthFig3Z;
	drawParallelogram(color, startFig5, lengthFig5X, lengthFig5Y, lengthFig5Z, true);

	//фигура 6(взбивалка для яиц)
	color = { 0.83, 0.83, 0.83 };
	int lengthFig6X = 100;
	int lengthFig6Y = 20;
	int lengthFig6Z = 50;
	int marginRelativeFig5Y = (lengthFig5Y - lengthFig6Y) / 2;
	int marginRelativeFig5Z = 40;
	Coord startFig6 = {
		startFig5.x - lengthFig6X,
		startFig5.y + marginRelativeFig5Y,
		startFig5.z + marginRelativeFig5Z
	};
	drawParallelogram(color, startFig6, lengthFig6X, lengthFig6Y, lengthFig6Z, false);

	//фигура 7(ручка вантуза)
	color = { 0.73, 0.73, 0.73 };
	int lengthFig7X = 130;
	int lengthFig7Y = 30;
	int lengthFig7Z = 50;
	int marginRelativeFig5Y_2 = (lengthFig5Y - lengthFig6Y) / 2;
	int marginRelativeFig5Z_2 = marginRelativeFig5Z + lengthFig6Z + 80;
	Coord startFig7 = {
		startFig5.x - lengthFig7X,
		startFig5.y + marginRelativeFig5Y_2,
		startFig5.z + marginRelativeFig5Z_2
	};
	drawParallelogram(color, startFig7, lengthFig7X, lengthFig7Y, lengthFig7Z, false);

	//фигура 8(конец вантуза)
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(startFig7.x, startFig7.y + lengthFig7Y / 2, startFig7.z + lengthFig7Z / 2);
	glScalef(1, 2.5, 2.5);
	gluSphere(quadObj, 10, lengthFig7Y, lengthFig7Z);
	glPopMatrix();

	//фигура 9
	glTranslatef(0, 0, 0);
	color = { 0.8, 0.8, 0.8 };
	int marginFig9RelativeFig5X = 10;
	int marginFig9RelativeFig5Z = 20;
	int lengthFig9X = lengthFig5X - marginFig9RelativeFig5X * 2;
	int lengthFig9Y = 10;
	int lengthFig9Z = lengthFig5Z - marginFig9RelativeFig5Z * 2;
	Coord startFig9 = {
		startFig5.x + marginFig9RelativeFig5X,
		startFig5.y + lengthFig5Y,
		startFig5.z + marginFig9RelativeFig5Z
	};
	drawParallelogram(color, startFig9, lengthFig9X, lengthFig9Y, lengthFig9Z, false);

	//фигура 10
	color = { 0, 0, 0 };
	int marginFig10RelativeFig5X = 20;
	int marginFig10RelativeFig5Z = 30;
	int lengthFig10X = lengthFig5X - marginFig10RelativeFig5X * 2;
	int lengthFig10Y = 5;
	int lengthFig10Z = lengthFig5Z - marginFig10RelativeFig5Z * 2;
	Coord startFig10 = {
		startFig5.x + marginFig10RelativeFig5X,
		startFig9.y + lengthFig9Y,
		startFig5.z + marginFig10RelativeFig5Z
	};
	drawParallelogram(color, startFig10, lengthFig10X, lengthFig10Y, lengthFig10Z, false);

	//фигура 11
	color = { 0.8, 0.8, 0.8 };
	int marginFig11RelativeFig5X = marginFig9RelativeFig5X;
	int marginFig11RelativeFig5Z = marginFig9RelativeFig5Z;
	int lengthFig11X = lengthFig9X;
	int lengthFig11Y = lengthFig9Y;
	int lengthFig11Z = lengthFig9Z;
	Coord startFig11 = {
		startFig9.x,
		startFig10.y + lengthFig10Y,
		startFig9.z
	};
	drawParallelogram(color, startFig11, lengthFig11X, lengthFig11Y, lengthFig11Z, false);

	//фигура 12
	color = { 0, 0, 0 };
	int marginFig12RelativeFig5X = marginFig10RelativeFig5X;
	int marginFig12RelativeFig5Z = marginFig10RelativeFig5Z;
	int lengthFig12X = lengthFig10X;
	int lengthFig12Y = lengthFig10Y;
	int lengthFig12Z = lengthFig10Z;
	Coord startFig12 = {
		startFig10.x,
		startFig11.y + lengthFig11Y,
		startFig10.z
	};
	drawParallelogram(color, startFig12, lengthFig12X, lengthFig12Y, lengthFig12Z, false);

	//фигура 13
	color = { 0.8, 0.8, 0.8 };
	int marginFig13RelativeFig5X = marginFig9RelativeFig5X;
	int marginFig13RelativeFig5Z = marginFig9RelativeFig5Z;
	int lengthFig13X = lengthFig9X;
	int lengthFig13Y = lengthFig9Y;
	int lengthFig13Z = lengthFig9Z;
	Coord startFig13 = {
		startFig9.x,
		startFig12.y + lengthFig12Y,
		startFig9.z
	};
	drawParallelogram(color, startFig13, lengthFig13X, lengthFig13Y, lengthFig13Z, false);

	//фигура 14(основание головы)
	color = { 0.8, 0, 0 };
	int lengthFig14X = lengthFig5X;
	int lengthFig14Y = 50;
	int lengthFig14Z = lengthFig5Z;
	Coord startFig14 = {
		startFig5.x,
		startFig13.y + lengthFig13Y,
		startFig5.z
	};
	drawParallelogram(color, startFig14, lengthFig14X, lengthFig14Y, lengthFig14Z, false);

	//фигура 15(полусфера головы)
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(quadObj, GL_TRUE);
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(
		startFig14.x + lengthFig14X / 2,
		startFig14.y + lengthFig14Y,
		startFig14.z + lengthFig14Z / 2
		);
	glScalef(1, 0.8, 1.7);
	GLdouble eq[4];
	eq[0] = 0;
	eq[1] = startFig14.y + lengthFig14Y + 100;
	eq[2] = 0;
	eq[3] = 0.0f;

	int lengthFig15X = lengthFig5X / 2;
	int lengthFig15Y = lengthFig15X;
	int lengthFig15Z = lengthFig5Z;
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eq);
	gluSphere(quadObj, lengthFig15X, lengthFig15Y, lengthFig15Z);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//фигура 16(ножка глаза)
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	Coord startFig16 = {
		startFig14.x + 10,
		startFig14.y + lengthFig14Y,
		startFig14.z + lengthFig14Z / 2
	};
	glTranslatef(startFig16.x, startFig16.y, startFig16.z);
	int lengthFig16X = 140;
	glRotated(-90, 0, 1, 0);
	gluCylinder(quadObj, 10, 10, lengthFig16X, 100, 100);
	glPopMatrix();

	//фигура 17(полусфера глаза)
	glPushMatrix();
	glColor3f(0, 0, 0);
	int lengthFig17X = 20;
	int lengthFig17Y = 30;
	int lengthFig17Z = 30;
	Coord startFig17 = {
		startFig16.x - lengthFig16X - lengthFig17X + 2,
		startFig16.y,
		startFig16.z
	};
	glTranslatef(startFig17.x, startFig17.y, startFig17.z);
	glScalef(1, 1, 1);
	//m_qObj = gluNewQuadric();

	eq[0] = startFig16.x;
	eq[1] = 0;
	eq[2] = 0;
	eq[3] = 0.0f;


	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eq);
	gluSphere(quadObj, lengthFig17X, lengthFig17Y, lengthFig17Z);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	//фигура 18(яблоко глаза)
	glPushMatrix();
	glColor3f(0, 0, 1);
	int lengthFig18X = 10;
	Coord startFig18 = {
		startFig17.x + lengthFig18X,
		startFig17.y,
		startFig17.z
	};
	glTranslatef(startFig18.x, startFig18.y, startFig18.z);

	glRotated(-90, 0, 1, 0);
	solidCylinder(quadObj, 10, 10, lengthFig18X, 100, 100);
	glPopMatrix();


	////оси
	glPushMatrix();
	glTranslatef(0, 0, 0);

	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-600, 0, 0);
	glVertex3f(400, 0, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, -400, 0);
	glVertex3f(0, 400, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0, 0, -400);
	glVertex3f(0, 0, 400);
	glEnd();
	glPopMatrix();

	//glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();

}







void Keyboard(unsigned char key, int x, int y){

	switch (key){
		//вращение
	case 'd':
		angel = 0;
		angel++;
		glRotatef(angel, 0.0, 1.0, 0.0);
		glutPostRedisplay();
		break;
	case 'a':
		angel = 0;
		angel--;
		glRotatef(angel, 0.0, 1.0, 0.0);
		glutPostRedisplay();
		break;
	case 'w':
		angel = 0;
		angel--;
		glRotatef(angel, 1.0, 0.0, 0.0);
		glutPostRedisplay();
		break;
	case 's':
		angel = 0;
		angel++;
		glRotatef(angel, 1.0, 0.0, 0.0);
		glutPostRedisplay();
		break;
	case 'x':
		angel = 0;
		angel--;
		glRotatef(angel, 0.0, 0.0, 1.0);
		glutPostRedisplay();
		break;
	case 'z':
		angel = 0;
		angel++;
		glRotatef(angel, 0.0, 0.0, 1.0);
		glutPostRedisplay();
		break;
		//движение по осям
	case '6':
		rot = 0;
		rot++;
		glTranslatef(rot, 0.0, 0.0);
		glutPostRedisplay();
		break;
	case '4':
		rot = 0;
		rot--;
		glTranslatef(rot, 0.0, 0.0);
		glutPostRedisplay();
		break;
	case '8':
		rot = 0;
		rot++;
		glTranslatef(0.0, rot, 0.0);
		glutPostRedisplay();
		break;
	case '2':
		rot = 0;
		rot--;
		glTranslatef(0.0, rot, 0.00);
		glutPostRedisplay();
		break;
	case '9':
		rot = 0;
		rot--;
		glTranslatef(0.0, 0.0, rot);
		glutPostRedisplay();
		break;
	case '1':
		rot = 0;
		rot++;
		glTranslatef(0.0, 0.0, rot);
		glutPostRedisplay();
		break;

	}
}

void SKeyboard(int key, int x, int y){
	switch (key){
	case GLUT_KEY_UP:

		scal = 1.0;
		scal += 0.05;
		glScalef(scal, scal, scal);
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:

		scal = 1.0;
		scal -= 0.05;
		glScalef(scal, scal, scal);
		glutPostRedisplay();
		break;
	}

}



void Init(){
	LoadTexrures();

	glClearColor(180.0 / 255.0, 243.0 / 255.0, 246.0 / 255.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width_win / 2, width_win / 2, -higth_win / 2, higth_win / 2, -250, 250);
	glMatrixMode(GL_MODELVIEW);


}


int main(int argc, char**argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width_win, higth_win);

	glutCreateWindow("Лабораторная работа 3");
	glutDisplayFunc(Draw_sh);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_COLOR_MATERIAL);
	Init();
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SKeyboard);
	glScalef(1, 1, 0.5);
	glutMainLoop();
	return 0;

}