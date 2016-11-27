#pragma comment(lib, "glaux.lib")
#include <windows.h>
#include <stdlib.h> 
#include <time.h>
#include <GL/glut.h>
#include <math.h>
#include <GL/glaux.h>

GLfloat WinWid = 1300;
GLfloat WinHei = 700;
GLfloat Anglex = 0.0f, Angley = 0.0f, Anglez = 0.0f;
GLfloat X = 0.0f, Y = 0.0f, Z = 0.0f, sX = 1.0f, sY = 1.0f, sZ = 1.0f, yRot, xRot;
GLfloat lx = -0.0, ly = 0.0;
GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };// Фоновый слабобелый
GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };// Рассеяный среднебелый
GLfloat light_position[] = { 200, WinHei / 2, 50.0, 0.0 };// Расположение источника
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  spotDir[] = { 0.0f, 0.0f, -1.0f };

GLuint	textures[6];


typedef GLfloat GLTVector3[3];
typedef GLfloat GLTVector4[4];
typedef GLfloat GLTMatrix[16];

//void gltMakeShadowMatrix(GLTVector3 vPoints[3], GLTVector4 vLightPos, GLTMatrix destMat);
//void gltGetPlaneEquation(GLTVector3 vPoint1, GLTVector3 vPoint2, GLTVector3 vPoint3, GLTVector3 vPlane);
GLTMatrix shadowMat;


// Умножить вектор на скаляр
void gltScaleVector(GLTVector3 vVector, const GLfloat fScale)
{
	vVector[0] *= fScale; vVector[1] *= fScale; vVector[2] *= fScale;
}
//**********************************************************
GLfloat gltGetVectorLengthSqrd(const GLTVector3 vVector)
{
	return (vVector[0] * vVector[0]) + (vVector[1] * vVector[1]) + (vVector[2] * vVector[2]);
}
GLfloat gltGetVectorLength(const GLTVector3 vVector)
{
	return (GLfloat)sqrt(gltGetVectorLengthSqrd(vVector));
}

//**********************************************************
// Привести вектор к единичной длине (нормировать)
void gltNormalizeVector(GLTVector3 vNormal)
{
	GLfloat fLength = 1.0f / gltGetVectorLength(vNormal);
	gltScaleVector(vNormal, fLength);
}
// Вычесть один вектор из другого
void gltSubtractVectors(const GLTVector3 vFirst, const GLTVector3 vSecond, GLTVector3 vResult)
{
	vResult[0] = vFirst[0] - vSecond[0];
	vResult[1] = vFirst[1] - vSecond[1];
	vResult[2] = vFirst[2] - vSecond[2];
}

//**********************************************************
// Вычислить векторное произведение двух векторов
void gltVectorCrossProduct(const GLTVector3 vU, const GLTVector3 vV, GLTVector3 vResult)
{
	vResult[0] = vU[1] * vV[2] - vV[1] * vU[2];
	vResult[1] = -vU[0] * vV[2] + vV[0] * vU[2];
	vResult[2] = vU[0] * vV[1] - vV[0] * vU[1];
}


// Вычислить нормаль по трем точкам
void gltGetNormalVector(const GLTVector3 vP1, const GLTVector3 vP2, const GLTVector3 vP3, GLTVector3 vNormal)
{
	GLTVector3 vV1, vV2;

	gltSubtractVectors(vP2, vP1, vV1);
	gltSubtractVectors(vP3, vP1, vV2);

	gltVectorCrossProduct(vV1, vV2, vNormal);
	gltNormalizeVector(vNormal);
}

//**********************************************************
// Возвращает коэффициенты уравнения плоскости по трем точкам
void gltGetPlaneEquation(GLTVector3 vPoint1, GLTVector3 vPoint2, GLTVector3 vPoint3, GLTVector3 vPlane)
{
	// Вычислить вектор нормали
	gltGetNormalVector(vPoint1, vPoint2, vPoint3, vPlane);

	vPlane[3] = -(vPlane[0] * vPoint3[0] + vPlane[1] * vPoint3[1] + vPlane[2] * vPoint3[2]);
}


//**********************************************************
//**********************************************************
// Вычислуние матрицы преобразования тени.
// Входные параметры - координаты трех точек
// на плоскости (не лежащих на одной прямой) 
// и четырехмерный вектор - положения источника света
// Возвращаемое значение находится в destMat
//**********************************************************
//**********************************************************

void gltMakeShadowMatrix(GLTVector3 vPoints[3], GLTVector4 vLightPos, GLTMatrix destMat)
{
	GLTVector4 vPlaneEquation;
	GLfloat dot;
	gltGetPlaneEquation(vPoints[0], vPoints[1], vPoints[2], vPlaneEquation);
	// Вычисляет скалярное произведение направляющего вектора плоскости
	// и вектора положения источника света
	dot = vPlaneEquation[0] * vLightPos[0] +
		vPlaneEquation[1] * vLightPos[1] +
		vPlaneEquation[2] * vLightPos[2] +
		vPlaneEquation[3] * vLightPos[3];
	// Формируем матрицу проекции
	// Первый столбец
	destMat[0] = dot - vLightPos[0] * vPlaneEquation[0];
	destMat[4] = 0.0f - vLightPos[0] * vPlaneEquation[1];
	destMat[8] = 0.0f - vLightPos[0] * vPlaneEquation[2];
	destMat[12] = 0.0f - vLightPos[0] * vPlaneEquation[3];
	// Второй столбец
	destMat[1] = 0.0f - vLightPos[1] * vPlaneEquation[0];
	destMat[5] = dot - vLightPos[1] * vPlaneEquation[1];
	destMat[9] = 0.0f - vLightPos[1] * vPlaneEquation[2];
	destMat[13] = 0.0f - vLightPos[1] * vPlaneEquation[3];
	// Третий столбец
	destMat[2] = 0.0f - vLightPos[2] * vPlaneEquation[0];
	destMat[6] = 0.0f - vLightPos[2] * vPlaneEquation[1];
	destMat[10] = dot - vLightPos[2] * vPlaneEquation[2];
	destMat[14] = 0.0f - vLightPos[2] * vPlaneEquation[3];
	// Четвертый столбец
	destMat[3] = 0.0f - vLightPos[3] * vPlaneEquation[0];
	destMat[7] = 0.0f - vLightPos[3] * vPlaneEquation[1];
	destMat[11] = 0.0f - vLightPos[3] * vPlaneEquation[2];
	destMat[15] = dot - vLightPos[3] * vPlaneEquation[3];
}
//**********************************************************



void LoadGLTextures(){

	AUX_RGBImageRec *texture1 = auxDIBImageLoadA("circles3.bmp"),
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

	//грань 1

	if (needTexture) {
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glEnable(GL_TEXTURE_2D);
	}
	glBegin(GL_QUADS);
	if (needTexture)
		glTexCoord2f(1, 0);
	glVertex3f(start.x, start.y, start.z);
	if (needTexture)
		glTexCoord2f(1, 1); 
	glVertex3f(start.x, start.y + heightY, start.z);
	if (needTexture)
		glTexCoord2f(0, 1);
	glVertex3f(start.x + heightX, start.y + heightY, start.z);
	if (needTexture)
		glTexCoord2f(0, 0);
	glVertex3f(start.x + heightX, start.y, start.z);
	glEnd();
	if (needTexture)
		glDisable(GL_TEXTURE_2D);

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
		glTexCoord2f(1, 1); 
	glVertex3f(start.x + heightX, start.y + heightY, start.z + heightZ);
	if (needTexture)
		glTexCoord2f(0, 1);
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
		glTexCoord2f(1, 0);
	glVertex3f(start.x + heightX, start.y, start.z);
	if (needTexture)
		glTexCoord2f(1, 1);
	glVertex3f(start.x + heightX, start.y + heightY, start.z);
	if (needTexture)
		glTexCoord2f(0, 1);
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

void Dalek(int nShadow, GLenum order) {



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float size = 50;
	glEnable(GL_DEPTH_TEST);//проверка глубины
	GLUquadricObj *quadObj;
	quadObj = gluNewQuadric();

	glPushMatrix();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(order);

	glEnd();

	glDisable(GL_CULL_FACE);

	//	glBindTexture(GL_TEXTURE_2D, textures[2]);
	// Рисуем плоскость, имитирующую землю
	glBegin(GL_QUADS);
	glColor3d(150, 155, 155);
	//	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(800.0f, -250.0f, -800.0f);
	//	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-800.0f, -250.0f, -800.0f);
	//	glColor3d(222, 184, 135);
	//	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-800.0f, -250.0f, 800.0f);
	//	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(800.0f, -250.0f, 800.0f);
	glEnd();

	//	glBindTexture(GL_TEXTURE_2D, textures[2]);

	//glEnable(GL_DEPTH_TEST);//проверка глубины
	// Каким цветом рисовать
	if (nShadow == 0)  // Рисовать 
		glColor3d(0.7, 0.7, 0.7);
	else        // Рисовать черным цветом тень 
		glColor3ub(0, 0, 0);



	glTranslatef(X, Y, Z);
	glRotatef(Anglex, 1.0, 0.0, 0.0);
	glRotatef(Angley, 0.0, 1.0, 0.0);
	glRotatef(Anglez, 0.0, 0.0, 1.0);
	glTranslatef(-X, -Y, -Z);

	glScalef(sX, sY, sZ);

	glTranslatef(X, 0.0, 0.0);
	glTranslatef(0.0, Y, 0.0);
	glTranslatef(0.0, 0.0, Z);



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
	glTranslatef(0, 1, 0);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, -400, 0);
	glVertex3f(0, 400, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 1);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0, 0, -400);
	glVertex3f(0, 0, 400);
	glEnd();
	glPopMatrix();

	//glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();

}


//освещение
void SetLightDalekLight()
{
	glDisable(GL_DITHER);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	// Настройка источника света
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	// Включаем поддержку согласования цветов
	glEnable(GL_COLOR_MATERIAL);
	// Режим согласования цветов назначаем для фонового и рассеянного
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);// Начальное значение диффузного материала
	glMateriali(GL_FRONT, GL_SHININESS, 28);    // Яркий блик
	// Любые три точки на плоскости, имитирующей землю
	GLTVector3 points[3] = { { -30.0f, -149.0f, -20.0f },
	{ -30.0f, -149.0f, 20.0f },
	{ 40.0f, -149.0f, 20.0f } };
	// Вычисление матрицы проекции тени на плоскость основания (землю)
	gltMakeShadowMatrix(points, light_position, shadowMat);
}

void Display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetLightDalekLight();
	// Ориентируем прожектор
	glEnable(GL_LIGHTING);;
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glClear(GL_DEPTH_BUFFER_BIT);
	glPushAttrib(0xffffffff);
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPopAttrib();
	Dalek(0, GL_CCW);

	



	glutSwapBuffers();
	glFlush();
}

void Timer(int){
	glutPostRedisplay();
	glutTimerFunc(10, Timer, 0);
}

void Keyboard(unsigned char key, int x, int y){
	switch (key)
	{
	case 'a': Angley++;
		break;
	case 'd': Angley--;
		break;
	case 'w': Anglex++;
		break;
	case 's': Anglex--;
		break;
	case 'q': Anglez++;
		break;
	case 'e': Anglez--;
		break;
	case '+':
		if (sX < 2.2){
			sX += 0.1f; sY += 0.1f; sZ += 0.1f;
		}
		break;
	case '-':
		if (sX > 0){
			sX -= 0.1f; sY -= 0.1f; sZ -= 0.1f;
		}
		break;

	case 'g':
		light_position[0] += 0.5;
		lx += 0.5;
		if (lx >= 0){
			light_position[1] -= 0.1;
			ly -= 0.1;
		}
		else{
			light_position[1] += 0.1;
			ly += 0.1;
		}
		break;
	case 'f':
		light_position[0] -= 0.5;
		lx -= 0.5;
		if (lx <= 0){
			light_position[1] -= 0.1;
			ly -= 0.1;
		}
		else{
			light_position[1] += 0.1;
			ly += 0.1;
		}
		break;

	default:
		break;
	}
}

void SKeyboard(int key, int x, int y){
	switch (key)
	{
	case GLUT_KEY_LEFT: X -= 2;
		break;
	case GLUT_KEY_RIGHT: X += 2;
		break;
	case GLUT_KEY_UP:
		Y += 2;
		break;
	case GLUT_KEY_DOWN: if (Y >= -55) Y -= 2;
		break;
	case GLUT_KEY_PAGE_UP: Z += 2;
		break;
	case GLUT_KEY_PAGE_DOWN: Z -= 2;
		break;
	default:
		break;
	}
}

void Reshape(int width, int height){
	SetLightDalekLight();
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glEnable(GL_DEPTH_TEST);  // Включить тест глубины
	glClearColor(180.0 / 255.0, 243.0 / 255.0, 246.0 / 255.0, 1.0);// Цвет фона окна
	// Предотвращаем деление на нуль
	if (height == 0)
		height = 1;
	// Устанавливаем поле просмотра с размерами окна
	glViewport(0, 0, width, height);
	// Устанавливает матрицу преобразования в режим проецирования
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Устанавливаем размеры отсекающего объема
	GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;// Для соблюдения пропорций
	gluPerspective(60.0f, aspectRatio, 1.0f, 500.0f);    // Отсекающая перспектива
	// Восстановливает матрицу преобразования в исходный режим вида
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Отодвинем сцену в отрицательную сторону оси 0z
	glTranslatef(0.0f, 0.0f, -100.0f);
}




int main(int argc, char**argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WinWid, WinHei);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Лаботаторная работа 4");
	// Загрузка текстур
	LoadGLTextures();
	glEnable(GL_TEXTURE_2D);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutTimerFunc(10, Timer, 0);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SKeyboard);
	glScalef(1, 1, 0.5);
	glutMainLoop();
	return 0;
}