//Engenharia Informática 3º Ano
//Disciplina: Computação Grafica
//Pedro Miguel Ferreira Ribeiro a37556
//Fernando Pinto a37536
//Paulo Gabriel 37555

#include <stdlib.h>
#include <stdio.h> // Uso da SOIL lib para load de texturas >> http://www.lonesock.net/soil.html
#include "soil\SOIL.h"
#include <SOIL.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#define _CRT_SECURE_NO_WARNINGS
#define BETWEEN(value, min, max) (value < max && value > min)

//Varios
GLuint texture[20];
GLfloat largura = 1920.0f, altura = 1080.0f;

//Personagem
GLfloat WS = 120.00f, HS = 120.00f;
GLfloat x_obj = 180, y_obj = 80, VG = 0.5;

//Gameplay
GLfloat velocity=0.05;
GLint timer = 0;
GLint state = 0;
GLint l = 5;
GLfloat gravity = 0.5;
GLfloat time = 0;
GLint color_obj = 1;
GLfloat last_gravity = 0;
GLfloat last_velocity = 0;
GLint score=0;

//Knifes
GLfloat x_faca =20000, x_faca2 = 20000, x_faca3 = 20000;
GLfloat y_faca = 200.00f, y_faca2 = 200.00f, y_faca3 = 200.00f;

GLvoid loadTextures() { // load das texturas usando a soil lib >> http://www.lonesock.net/soil.html
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.6f, 0.80f, 0.98f, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenTextures(20, texture);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture[1] = SOIL_load_OGL_texture
	(
		"textures/salsicha.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture[2] = SOIL_load_OGL_texture
	(
		"textures/pickle.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture[3] = SOIL_load_OGL_texture
	(
		"textures/faca5.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glBindTexture(GL_TEXTURE_2D, texture[4]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture[4] = SOIL_load_OGL_texture
	(
		"textures/faca4.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glBindTexture(GL_TEXTURE_2D, texture[5]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture[5] = SOIL_load_OGL_texture
	(
		"textures/faca2.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glBindTexture(GL_TEXTURE_2D, texture[6]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture[6] = SOIL_load_OGL_texture
	(
		"textures/faca3.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glBindTexture(GL_TEXTURE_2D, texture[7]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture[7] = SOIL_load_OGL_texture
	(
		"textures/faca1.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);


	glBindTexture(GL_TEXTURE_2D, texture[8]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture[8] = SOIL_load_OGL_texture
	(
		"textures/game_paused.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glBindTexture(GL_TEXTURE_2D, texture[9]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture[9] = SOIL_load_OGL_texture
	(
		"textures/game_over.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	glBindTexture(GL_TEXTURE_2D, texture[10]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture[10] = SOIL_load_OGL_texture
	(
		"textures/background.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	
}

GLvoid DrawObjects(GLfloat x, GLfloat y, GLint f) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[f]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(x, y);
	glTexCoord2d(1, 0); glVertex2f(x + WS, y);
	glTexCoord2d(1, 1); glVertex2f(x + WS, y + HS);
	glTexCoord2d(0, 1); glVertex2f(x, y + HS);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

GLvoid drawBackground() {
	GLint Jlargura = glutGet(GLUT_WINDOW_WIDTH);
	GLint Jaltura = glutGet(GLUT_WINDOW_HEIGHT); 

	glEnable(GL_TEXTURE_2D); //inicia
	glBindTexture(GL_TEXTURE_2D, texture[10]); // Ativação da textura(Imagem)
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);//red, green, blue, and alpha values.
	glBegin(GL_QUADS);// Limitação dos seguintes vertices vertices
	glTexCoord2d(0, 0); glVertex2f(0.0f, 0.0f);
	glTexCoord2d(1, 0); glVertex2f(Jlargura, 0.0f);
	glTexCoord2d(1, 1); glVertex2f(Jlargura, Jaltura);
	glTexCoord2d(0, 1); glVertex2f(0.0f, Jaltura);
	glEnd();

	glDisable(GL_TEXTURE_2D); // termina
}

GLvoid DrawKnifes(GLfloat x, GLfloat y,GLint k) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[k]);
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2d(0, 0); glVertex2f((x - 40.0f), y);
	glTexCoord2d(1, 0); glVertex2f(x + 40.0f, y);
	glTexCoord2d(1, 1); glVertex2f(x + 40.0f, y + 80.0f);
	glTexCoord2d(0, 1); glVertex2f(x - 40.0f, y + 80.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

GLvoid drawPause() {

	GLint Jlargura = glutGet(GLUT_WINDOW_WIDTH);
	GLint Jaltura = glutGet(GLUT_WINDOW_HEIGHT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[8]);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f((Jlargura / 2.f) - 400.0f, (Jaltura / 2.f) - 200.0f);
	glTexCoord2d(1, 0); glVertex2f((Jlargura / 2.f) + 400.0f, (Jaltura / 2.f) - 200.0f);
	glTexCoord2d(1, 1); glVertex2f((Jlargura / 2.f) + 400.0f, (Jaltura / 2.f) + 200.0f);
	glTexCoord2d(0, 1); glVertex2f((Jlargura / 2.f) - 400.0f, (Jaltura / 2.f) + 200.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

GLvoid drawGameOver() {

	GLint Jlargura = glutGet(GLUT_WINDOW_WIDTH);
	GLint Jaltura = glutGet(GLUT_WINDOW_HEIGHT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[9]);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f((Jlargura / 2.f) - 400.0f, (Jaltura / 2.f) - 200.0f);
	glTexCoord2d(1, 0); glVertex2f((Jlargura / 2.f) + 400.0f, (Jaltura / 2.f) - 200.0f);
	glTexCoord2d(1, 1); glVertex2f((Jlargura / 2.f) + 400.0f, (Jaltura / 2.f) + 200.0f);
	glTexCoord2d(0, 1); glVertex2f((Jlargura / 2.f) - 400.0f, (Jaltura / 2.f) + 200.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

GLvoid Encolhe() {
	if(state==1){
	HS = 100;
	WS = 100;	
	x_faca  += -(1);
	x_faca2 += -(1);
	x_faca3 += -(1);
	}
}

GLvoid startGame() {
	GLint Jlargura = glutGet(GLUT_WINDOW_WIDTH);
	GLint Jaltura = glutGet(GLUT_WINDOW_HEIGHT);
	score = 0;
	state = 1;
	time = 0;
	VG = 0.5;
	velocity = -(0.30);
	x_faca = Jlargura/2;
	x_faca2 = (Jlargura/2)+400;
	x_faca3 = (Jlargura/2)+800;
}

GLvoid ReSpwan() {
	GLint Jlargura = glutGet(GLUT_WINDOW_WIDTH);
	GLint Jaltura = glutGet(GLUT_WINDOW_HEIGHT);

	if (x_faca <= 0) { x_faca = (Jlargura); printf("Respawn Faca 1\n"); if (l < 6)l++; score += 10;}
	if (x_faca2 <= 0) { x_faca2 = (Jlargura); printf("Respawn Faca 2\n"); score += 10;}
	if (x_faca3 <= 0) {
		x_faca3 = (Jlargura); printf("Respawn Faca 3\n");
		if(velocity>=(-1.25))velocity += -(0.05);
		if(VG<=1.5)VG += 0.1;		
		score += 10;
	}
}

GLvoid Gravity() {
	
	if (y_faca2 <= 100)  { gravity =  VG; }
	if (y_faca3 <= 100)  { gravity =  VG; }
	if (y_faca2 >= 350) { gravity = -VG; }
	if (y_faca3 >= 350) { gravity = -VG; }
	y_faca2 += gravity;
	y_faca3 += gravity;
	
	
}

GLvoid Pause() {
	state = 4;
	drawPause();
}
GLvoid unPause() {
	state = 1;
}
GLvoid Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Desenhar BackGround
	glPushMatrix();
	glLoadIdentity();
	drawBackground();
	glPopMatrix();

	//Desenhar Objeto(Salchicha/Mr.Pickle)
	glPushMatrix();
	glLoadIdentity();
	DrawObjects(x_obj, y_obj, color_obj);
	glPopMatrix();

	//Desenhar Facas
	for (int k = 2; k < l; k++) {
		glPushMatrix();
		glLoadIdentity();
		if (k == 3)	 DrawKnifes(x_faca, y_faca, k);
		if (k == 4)  DrawKnifes(x_faca2, y_faca2, k);
		if (k == 5) { DrawKnifes(x_faca3, y_faca3, k); }
		glPopMatrix();
	}






	if (state == 1) {
		x_faca += velocity;
		x_faca2 += velocity;
		x_faca3 += velocity;
		ReSpwan();
		Gravity();
	}
	if (state == 0) { drawPause(); }
	if (state == 3) { drawGameOver(); }
	if (state == 4) { Pause(); drawPause(); }

	//HitBox
	//a Textura(Imagem do objeto) tem uma parte trasparente, dificil de Fazer uma HitBox completamente certa
	if (BETWEEN(x_faca, (x_obj + 40), (x_obj + WS - 40))) {
		if ((HS + 60) >= y_faca) state = 3;
	}
	if (BETWEEN(x_faca2, (x_obj + 40), (x_obj + WS - 40))) {
		if ((HS + 60) >= (y_faca2 + 30)) state = 3;
	}
	if (BETWEEN(x_faca3, (x_obj + 40), (x_obj + WS - 40))) {
		if ((HS + 60) >= (y_faca3 + 30)) state = 3;
	}

	//https://cboard.cprogramming.com/game-programming/40704-how-do-i-display-ints-value-opengl.html
	//http://www.cplusplus.com/forum/beginner/84151/

	char s[256];
	s[0] = score;
	sprintf_s(s, "The value of n is %d", score);
	glColor3f(0, 0, 0);
	glRasterPos2f(1260, 730);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s);

	glPopMatrix();
	glutSwapBuffers();
	glutPostRedisplay();

}


GLvoid HandleKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 49:
		if (state == 4) { color_obj = 1; state = 0;}
		break;
	case 50:
		if (state == 4){color_obj = 2; state = 0;}
		break;
	case 83:
		if (state != 1)state = 4; break;
	case 32:
		if (state == 3) { state = 0; x_faca = 20000, x_faca2 = 20000, x_faca3 = 20000;}
		else if (state == 4) state = 1;
		else if (state != 1)startGame();
		else if (state == 1) Encolhe();
		break;
	case 81:
		exit(0);
		break;
	case (80):
		if (state == 1) state = 4;
		else if (state == 4)state = 1;
		break;
	}
	

	glutPostRedisplay();
}

GLvoid keyUp(unsigned char key, int x, int y) {
	if (key == 32) {
		if (state == 1) { HS = 160; WS = 160; }
	}
}

GLvoid ChangeSize(GLsizei w, GLsizei h) {

	glViewport(0, 0, w, h); //https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glViewport.xml

	GLint Jlargura = glutGet(GLUT_WINDOW_WIDTH);
	GLint Jaltura = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

		glOrtho(0.0f, Jlargura, 0.0f, Jaltura, 1.0, -1.0); //https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glOrtho.xml

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_ALPHA);
	glutInitWindowSize(1360, 760);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow("Corre Salsisha Corre:");
	loadTextures();
	glutDisplayFunc(Display);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(HandleKeyboard);
	glutKeyboardUpFunc(keyUp);
	glutFullScreen();           // making the window full screen
	glutMainLoop();
}