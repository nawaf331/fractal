#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h> 
//#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
//#define OS_WIN
//#endif
int width=600, height=600; // window size
int windowID;
int color=0,flag=0;
GLfloat minX = -2.2f, maxX = 0.8f, minY = -1.5f, maxY = 1.5; // complex plane boundaries
GLfloat stepX = (maxX - minX)/(GLfloat)width;
GLfloat stepY = (maxY - minY)/(GLfloat)height;
char titleSet[10][25]={"Mandelbrot fractal","Mandelbrot^3 fractal","Flower fractal","Star fractal","Julia Fractal","Sierpanski Gasket"};
GLfloat black[] = {0.0f, 0.0f, 0.0f}; // black color
const int paletteSize = 128;
GLfloat palette[paletteSize][3];
const GLfloat radius = 5.0f;
bool fullScreen=false;
int fracCount=0;
GLfloat juliaFactor=0.0;
GLfloat juliaSpecial=0.5;
GLfloat zoomFactor=0.1;
GLfloat theta=0.0;
int randm=0;

typedef float point[3];
point v[]={{-0.5,0.0,1.0},{-0.5,1.0,0.0},
           {-1.5,-0.5,0.0}, {0.5,-0.5,0.0}};

//#ifdef OS_WIN
//flag=0;
//#else

//#endif
//****************************************
void output(float x,float y,float z,void *font,char *string)
{
	char *c;
	glRasterPos3f(x,y,z);
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}
//***************************************
GLfloat* greenJulia(GLfloat u, GLfloat v){	//front page. though it is not green :P
	GLfloat re = u;
	GLfloat im = v;
	GLfloat frontFrac1=-0.1;		
	GLfloat frontFrac2=0.651;
	GLfloat tempRe=0.0;
	for(int i=0; i < paletteSize; i++){
		tempRe = re*re - im*im + frontFrac1+juliaFactor;			//z=z^2+(-0.1+0.651i)
		im = re * im * 2 + frontFrac2+juliaFactor;
		re = tempRe;
		if( (re*re + im*im) > radius ){
			return palette[i+35];
		}
	}
	return black;
}

//****************************************
GLfloat *mandelbrot(GLdouble u, GLdouble v)	//z=z^2+c 
{
	GLdouble re = u;
	GLdouble im = v;
	GLdouble tempRe=0.0;
	
	for(int i=0; i < paletteSize; i++)
		{
			tempRe =  re*re - im*im + u;
			im = re * im * 2 + v;
			re = tempRe;
			if( (re*re + im*im) > radius )
			{
				return palette[i];
			}
		}
		return black;
}

//*****************************************
GLfloat *mandelbrot3(GLdouble u, GLdouble v)		//z=z^3+c
{

	GLdouble re = u;
	GLdouble im = v;
	GLdouble tempRe=0.0;
	
	for(int i=0; i < paletteSize; i++)
		{
			tempRe = re*re*re - re*im*im - 2*re*im*im + u;
			im = re*re*im -im*im*im+ 2*re*re*im+ v;
			re = tempRe;
			if( (re*re + im*im) > radius )
			{
				return palette[i];
			}
		}
		return black;
}

//******************************************
GLfloat *starFractal(GLdouble u, GLdouble v)	//shape resembles to be that of a star
{
	GLdouble re = u;
	GLdouble im = v;
	GLdouble tempRe=0.0;
		GLfloat starFrac1=-0.1;
	GLfloat starFrac2=0.651;

	for(int i=0; i < paletteSize; i++)		//z=z^3+(-0.1+0.651i)
	{
		tempRe = re*re*re - re*im*im - 2*re*im*im -( re*re - im*im)+starFrac1+ juliaFactor*10;
		im = re*re*im -im*im*im+ 2*re*re*im -(re * im * 2) +starFrac2+juliaFactor*10;
			re = tempRe;


			if( (re*re + im*im) > radius )
			{
				return palette[(i+100)];		///for color variation add 100 to color index
			}
	}
		return black;
}

//***********************************
GLfloat *julia(GLdouble u, GLdouble v)		//julia set z=z^2+k where k is 0.36
{
	GLdouble re = u;
	GLdouble im = v;
	GLdouble tempRe=0.0;
	GLdouble juliaFrac=0.36;
	for(int i=0; i < paletteSize; i++)
		{
			tempRe =  re*re - im*im + juliaFrac+juliaFactor;
			im = re * im * 2 + juliaFrac+juliaFactor;
			re = tempRe;
			if( (re*re + im*im) > radius )
			{
				return palette[i];
			}
		}
		return black;
}

//*************************************
GLfloat *flower(GLdouble u, GLdouble v)		//z=z^2+C where C varies for each iteration of each point
{
	GLdouble re = u;
	GLdouble im = v;
	GLdouble tempRe=0.0;
	for(int i=0; i < paletteSize; i++)
		{
			tempRe =  re*re - im*im + re;
			im = re * im * 2 + im;
			re = tempRe;
			if( (re*re + im*im) > radius )
			{
				return palette[i+31];
			}
		}
		return black;
}

//***********************************
void drawFrontPage(){
	glPushMatrix();
	char name[10]="About Us";
	char desc[20]="Description";
	char help[10]="Help";
	char title[25]="Fractals are awesome!!";
	char isntit[25]="isn't it? :D";
	glColor3f(0.0,0.0,0.0);
	glTranslatef(-1.8,-0.5,0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.02,0.08,0.0);
	glVertex3f(0.02,-0.11,0.0);
	glVertex3f(0.65,-0.11,0.0);
	glVertex3f(0.65,0.08,0.0);
	glEnd();
	glColor3f(1.0,1.0,1.0);
	output(0.05,-0.05,0,GLUT_BITMAP_TIMES_ROMAN_24,desc);

	//help
	glTranslatef(0.0,-0.3,0.0);
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.0,0.08,0.0);
	glVertex3f(0.0,-0.11,0.0);
	glVertex3f(0.65,-0.11,0.0);
	glVertex3f(0.65,0.08,0.0);
	glEnd();
	glColor3f(1.0,1.0,1.0);
	output(0.20,-0.05,0,GLUT_BITMAP_TIMES_ROMAN_24,help);
	
	//printf("about us\n");
	glColor3f(0.0,0.0,0.0);
	glTranslatef(0.0,-0.3,0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.02,0.08,0.0);
	glVertex3f(0.02,-0.11,0.0);
	glVertex3f(0.65,-0.11,0.0);
	glVertex3f(0.65,0.08,0.0);
	glEnd();
	glColor3f(1.0,1.0,1.0);
	output(0.10,-0.05,0,GLUT_BITMAP_TIMES_ROMAN_24,name);
	
	glTranslatef(1.8,1.2,0.0);
	glColor3f(0.0,0.0,0.0);
	output(-1.2,1.2,0,GLUT_BITMAP_TIMES_ROMAN_24,title);
	output(-0.95,1.05,0,GLUT_BITMAP_TIMES_ROMAN_24,isntit);
	
	glPopMatrix();
}


void mymenu(int value) {
	if(value == 1)
		switch(fracCount)
		{
			case 0:
				printf("============= front page ================ \n");
				printf("A fractal is a mathematical set that has a fractal dimension that usually exceeds its topological dimension and may fall between the integers. Fractals are typically self-similar patterns, where self-similar means they are \"the same from near as from far\". Fractals may be exactly the same at every scale, or, they may be nearly the same at different scales. The definition of fractal goes beyond self-similarity per se to exclude trivial self-similarity and include the idea of a detailed pattern repeating itself.\n\n");	
				break;
			case 1:
				printf("====== Mandelbrot fractal ======\n");
				printf("The equation for this fractal is z=z^2+c\n\n");
				break;
			case 2:
				printf("====== Mandelbrot^3 ====== \n");
				printf("The equation for this fractal is z=z^3+c\n");
				break;
			case 3:
				printf("====== flower fractal ====== \n");
				printf("The equation for this fractal is z=z^3+c\n");
				break;
			case 4:
				printf("star fractal\n");
				break;
			case 5:
				printf("Julia factor\n");
				break;
		}
	else if(value == 2 )
	{
		fracCount=1;
		glutPostRedisplay();
	}
	else if(value == 3)
	{
		fracCount=2;
		glutPostRedisplay();
	}
	else if(value == 4)
	{
		fracCount=5;
		glutPostRedisplay();
	}
	else if(value == 10 )
	{
		system("gedit help");	
	}
	else if(value==99)
	{
		system("gedit about");	
	}

}

GLfloat* calculateColor(GLfloat u, GLfloat v){
	switch(fracCount)
	{
		case 0: 
					juliaSpecial=0.5;
					return greenJulia(u,v);
					break;
		case 1:

			juliaSpecial=0.0;
			return mandelbrot(u,v);
					break;
		case 2:

			//color=0;
			juliaSpecial=0.5;
			return mandelbrot3(u,v);
			break;
		case 3:
			//	printf("Flower\n");
				juliaSpecial=0.0;
				//color=0.0;
				return flower(u,v);
				break;
		case 4:
			//printf("Star\n");
			juliaSpecial=0.9;
			//color=45;
			return starFractal(u,v);
			break;
		case 5: 
			//printf("Julia\n");
			//color=0;
			juliaSpecial=0.5;
			return julia(u,v);
					break;
		default: 
			//printf("default\n");
			//juliaSpecial=0.0;
			fracCount=0;
			glutPostRedisplay();
			//return mandelbrot(u,v);
			break;

	}
}

//****************************************
void triangle(point a,point b,point c)
{
	glBegin(GL_POLYGON);
	  glVertex3fv(a);
	  glVertex3fv(b);
	  glVertex3fv(c);
	glEnd();
}

void divide_triangle(point a,point b,point c,int m)
{
	point v1,v2,v3;
	int j;
	if(m>0)
	{
		for(j=0;j<3;j++)
			v1[j]=(a[j]+b[j])/2;
		for(j=0;j<3;j++)
			v2[j]=(a[j]+c[j])/2;
		for(j=0;j<3;j++)
			v3[j]=(c[j]+b[j])/2;
		divide_triangle(a,v1,v2,m-1);
		divide_triangle(c,v2,v3,m-1);
		divide_triangle(b,v3,v1,m-1);
	}
	else(triangle(a,b,c));
}

void tetrahedron(int m)
{
	glColor3fv(palette[color]);
	divide_triangle(v[0],v[1],v[2],m);
	glColor3fv(palette[color+6]);
	divide_triangle(v[3],v[2],v[1],m);
	glColor3fv(palette[color+12]);
	divide_triangle(v[0],v[3],v[1],m);
	glColor3fv(palette[color+18]);
	divide_triangle(v[0],v[2],v[3],m);
}

//****************************************
void repaint() {// function called to repaint the window
	GLfloat* pixelColor;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen buffer
	if(fracCount<6){
	glBegin(GL_POINTS); // start drawing in single pixel mode
	for(GLfloat y = maxY; y >= minY; y -= stepY){
		for(GLfloat x = minX+juliaSpecial; x <= maxX+juliaSpecial; x += stepX){
			pixelColor=calculateColor(x,y);
			glColor3fv(pixelColor+color); // set color
			glVertex3f(x-juliaSpecial, y, 0.0f); // put pixel on screen (buffer) - [ 1 ]
			//printf("%d\t%d\n",stepX,stepY);
		}
	}
	glEnd(); // end drawing
	}
	else {
		switch(fracCount)
		{
			case 6:
				//glPushMatrix();
				tetrahedron(5);
				//glPopMatrix();
				break;
		}
	}
	if(fracCount==0){
		drawFrontPage();
	}
	else
	{
		glPushMatrix();
		char title[25]="";
		strcpy(title,titleSet[fracCount-1]);
		glColor3f(0.2,0.2,0.2);
		output(-1.1,1.2,0,GLUT_BITMAP_TIMES_ROMAN_24,title);
		printf("%s\n",title);
		glPopMatrix();
	}
	glutSwapBuffers(); // swap the buffers - [ 2 ]
}

//****************************************
void reshape (int w, int h){ // function called when window size is changed
	width=w;
	height=h;
	stepX = (maxX-minX)/(GLfloat)width; // calculate new value of step along X axis
	stepY = (maxY-minY)/(GLfloat)height; // calculate new value of step along Y axis
	glViewport (0, 0, (GLsizei)w, (GLsizei)h); // set new dimension of viewable screen
	glutPostRedisplay(); // repaint the window
}

//****************************************
void keyFunction(unsigned char key, int x, int y){ // function to handle key pressing
	switch(key){
case 'F': // pressing F is turning on/off fullscreen mode
case 'f':
	if(fullScreen){
		glutReshapeWindow(width,height); // sets default window size
		GLsizei windowX = (glutGet(GLUT_SCREEN_WIDTH)-width)/2;
		GLsizei windowY = (glutGet(GLUT_SCREEN_HEIGHT)-height)/2;
		glutPositionWindow(windowX, windowY); // centers window on the screen
		fullScreen = false;
	}
	else{
		fullScreen = true;
		glutFullScreen(); // go to fullscreen mode
	}
	glutPostRedisplay();
	break;
case 'k': 
	juliaFactor+=0.0001;
	//juliaFactor+=0.01;
	glLoadIdentity();
	glOrtho(minX, maxX, minY, maxY, ((GLdouble)-1), (GLdouble)1);
	glutPostRedisplay();
	break;
case 'l': juliaFactor-=0.0001;
	glutPostRedisplay();
	break;
case 'c': color++;
	glutPostRedisplay();
	break;
case 'C':color--;
	glutPostRedisplay();
	break;
case 'm': 
	minX = -2.2f, maxX = 0.8f, minY = -1.5f, maxY = 1.5; 
	stepX = (maxX - minX)/(GLfloat)width;
	stepY = (maxY - minY)/(GLfloat)height;
	glLoadIdentity();
	glOrtho(minX, maxX, minY, maxY, ((GLdouble)-1), (GLdouble)1);
	fracCount++;
	glutPostRedisplay();
	break;
case 'n':
	minX = -2.2f, maxX = 0.8f, minY = -1.5f, maxY = 1.5; // complex plane boundaries
	stepX = (maxX - minX)/(GLfloat)width;
	stepY = (maxY - minY)/(GLfloat)height;
	fracCount--;
	glLoadIdentity();
	glOrtho(minX, maxX, minY, maxY, ((GLdouble)-1), (GLdouble)1);
	glutPostRedisplay();
	break;
case 'z':
case '+':
		printf("Zooming in\n");
		minX+=zoomFactor;
		maxX-=zoomFactor;
		minY+=zoomFactor;
		maxY-=zoomFactor;
		stepX = (maxX-minX)/(GLfloat)width; // calculate new value of step along X axis
		stepY = (maxY-minY)/(GLfloat)height; // calculate new value of step along Y axis
		//glViewport (0, 0, (GLsizei)width, (GLsizei)height); // set new dimension of viewable screen
		glLoadIdentity();
		glOrtho(minX, maxX, minY, maxY, ((GLfloat)-1), (GLfloat)1);
		glutPostRedisplay(); // repaint the window
		break;
case 'Z':
case '-':
		printf("Zooming out\n");
		minX-=zoomFactor;
		maxX+=zoomFactor;
		minY-=zoomFactor;
		maxY+=zoomFactor;
		stepX = (maxX-minX)/(GLfloat)width; // calculate new value of step along X axis
		stepY = (maxY-minY)/(GLfloat)height; // calculate new value of step along Y axis
		//glViewport (0, 0, (GLsizei)width, (GLsizei)height); // set new dimension of viewable screen
		glLoadIdentity();
		glOrtho(minX, maxX, minY, maxY, ((GLfloat)-1), (GLfloat)1);
		glutPostRedisplay(); // repaint the window
		break;
case '<' : 
		theta+=0.2;
		srand(time(NULL));
		randm=rand()%3;
		glClearColor(1.0,1.0,1.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		if(randm==0)
			glRotatef(theta,1.0,0.0,0.0);
		else if(randm==1)
			glRotatef(theta,0.0,1.0,0.0);
		else
			glRotatef(theta,0.0,0.0,1.0);
		glutPostRedisplay();
		break;
case 27 : // escape key - close the program
	glutDestroyWindow(windowID);
	exit(0);
	break;
	}
}

//***************************************************
void mouseFunction(int button,int state,int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(fracCount==0 && flag==0)
		{
			printf("x = %d,y = %d\n",x,y);
				//**************** check if the user clicks on any button
		
			if((x>85 && x<210) && y>385 && y<420)
			{
				system("gedit description");	
			}
			else if((x>85 && x<210) && y>445 && y<480)
			{
				system("gedit help");	
			}
			else if((x>85 && x<210) && y>500 && y<555)
			{
				system("gedit about");	
			}
			
		}
		else { //if(fracCount%100!=99){
			
		printf("Zooooooooming area \n");
		GLdouble centreX=minX+stepX*x;
		GLdouble centreY=minY+stepY*y;

		minX=centreX - (maxX-minX)/2.0f;
		maxX=centreX + (maxX-minX)/2.0f;
		minY=centreY - (maxY-minY)/2.0f;
		maxY=centreY + (maxY-minY)/2.0f;
		stepX = (maxX-minX)/(GLfloat)width; // calculate new value of step along X axis
		stepY = (maxY-minY)/(GLfloat)height; // calculate new value of step along Y axis
		
		glLoadIdentity();
		glOrtho(minX, maxX, minY, maxY, ((GLdouble)-1), (GLdouble)1);
		glutPostRedisplay();
		}
	}
	else
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glutCreateMenu(mymenu); // single menu, no need for id
		glutAddMenuEntry("What is this", 1);
		glutAddMenuEntry("Mandelbrot Fractal",2);
		glutAddMenuEntry("Mandelbrot^3 fractal",3);
		glutAddMenuEntry("Julia Fractal",4);
		glutAddMenuEntry("How do i use this",10);
		glutAddMenuEntry("About Us", 99);
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}
	else 
	if(button == GLUT_MIDDLE_BUTTON && state==GLUT_DOWN)
	{
		printf("\nPan");
		GLdouble centreX=minX+stepX*x;
		GLdouble centreY=minY+stepY*y;

		minX=centreX - (maxX-minX)/2.0f;
		maxX=centreX + (maxX-minX)/2.0f;
		minY=centreY - (maxY-minY)/2.0f;
		maxY=centreY + (maxY-minY)/2.0f;
		stepX = (maxX-minX)/(GLfloat)width; // calculate new value of step along X axis
		stepY = (maxY-minY)/(GLfloat)height; // calculate new value of step along Y axis
		
		glLoadIdentity();
		glOrtho(minX, maxX, minY, maxY, ((GLdouble)-1), (GLdouble)1);
		glutPostRedisplay();
	}
}

//****************************************
void specialKeyFunction(int key, int x, int y){ // function to handle key pressing
	switch(key)
	{
		case GLUT_KEY_RIGHT:
			color=0;
			minX = -2.2f, maxX = 0.8f, minY = -1.5f, maxY = 1.5; 
			stepX = (maxX - minX)/(GLfloat)width;
			stepY = (maxY - minY)/(GLfloat)height;
			glLoadIdentity();
			glOrtho(minX, maxX, minY, maxY, ((GLdouble)-1), (GLdouble)1);
			fracCount++;
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			color=0;
			minX = -2.2f, maxX = 0.8f, minY = -1.5f, maxY = 1.5; 
			stepX = (maxX - minX)/(GLfloat)width;
			stepY = (maxY - minY)/(GLfloat)height;
			glLoadIdentity();
			glOrtho(minX, maxX, minY, maxY, ((GLdouble)-1), (GLdouble)1);
			fracCount--;
			glutPostRedisplay();
			break;
		case GLUT_KEY_UP:
		if(fracCount<6) {
			printf("Zooming in\n");
			minX+=zoomFactor;
			maxX-=zoomFactor;
			minY+=zoomFactor;
			maxY-=zoomFactor;
			stepX = (maxX-minX)/(GLfloat)width; // calculate new value of step along X axis
			stepY = (maxY-minY)/(GLfloat)height; // calculate new value of step along Y axis
		//glViewport (0, 0, (GLsizei)width, (GLsizei)height); // set new dimension of viewable screen
			glLoadIdentity();
			glOrtho(minX, maxX, minY, maxY, ((GLfloat)-1), (GLfloat)1);
			glutPostRedisplay(); // repaint the window
		}
		else {
			theta+=0.2;
			glutPostRedisplay();
		}
			break;
		case GLUT_KEY_DOWN:
			printf("Zooming out\n");
			minX-=zoomFactor;
			maxX+=zoomFactor;
			minY-=zoomFactor;
			maxY+=zoomFactor;
			stepX = (maxX-minX)/(GLfloat)width; // calculate new value of step along X axis
			stepY = (maxY-minY)/(GLfloat)height; // calculate new value of step along Y axis
			//glViewport (0, 0, (GLsizei)width, (GLsizei)height); // set new dimension of viewable screen
			glLoadIdentity();
			glOrtho(minX, maxX, minY, maxY, ((GLfloat)-1), (GLfloat)1);
			glutPostRedisplay(); // repaint the window
			break;
		case GLUT_KEY_HOME:
			minX = -2.2f, maxX = 0.8f, minY = -1.5f, maxY = 1.5; 
			stepX = (maxX - minX)/(GLfloat)width;
			stepY = (maxY - minY)/(GLfloat)height;
			glLoadIdentity();
			glOrtho(minX, maxX, minY, maxY, ((GLdouble)-1), (GLdouble)1);
			fracCount=0;
			color=0;
			glutPostRedisplay();
			break;
		case GLUT_KEY_F1:
			system("gedit help");
			break;
		case GLUT_KEY_F10:
			if(fullScreen){
				glutReshapeWindow(width,height); // sets default window size
				GLsizei windowX = (glutGet(GLUT_SCREEN_WIDTH)-width)/2;
				GLsizei windowY = (glutGet(GLUT_SCREEN_HEIGHT)-height)/2;
				glutPositionWindow(windowX, windowY); // centers window on the screen
				fullScreen = false;
			}
			else{
				fullScreen = true;
				glutFullScreen(); // go to fullscreen mode
			}
			glutPostRedisplay();
			break;
	}
}
//****************************************
void createPalette(){
	for(int i=0; i < 32; i++){
		palette[i][0] = (8*i)/(GLfloat)255;
		palette[i][1] = (128-4*i)/(GLfloat)255;
		palette[i][2] = (255-8*i)/(GLfloat)255;
	}
	for(int i=0; i < 32; i++){
		palette[32+i][0] = (GLfloat)1;
		palette[32+i][1] = (8*i)/(GLfloat)255;
		palette[32+i][2] = (GLfloat)0;
	}
	for(int i=0; i < 32; i++){
		palette[64+i][0] = (128-4*i)/(GLfloat)255;
		palette[64+i][1] = (GLfloat)1;
		palette[64+i][2] = (8*i)/(GLfloat)255;
	}
	for(int i=0; i < 32; i++){
		palette[96+i][0] = (GLfloat)0;
		palette[96+i][1] = (255-8*i)/(GLfloat)255;
		palette[96+i][2] = (8*i)/(GLfloat)255; 
	}
}

//****************************************
int main(int argc, char** argv){
	glutInit(&argc, argv);
	createPalette();
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	GLsizei windowX = (glutGet(GLUT_SCREEN_WIDTH)-width)/2;
	GLsizei windowY = (glutGet(GLUT_SCREEN_HEIGHT)-height)/2;
	glutInitWindowPosition(windowX, windowY);
	glutInitWindowSize(width, height);
	windowID = glutCreateWindow("Matrix Fractal Zoomer");

	glViewport (0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(minX, maxX, minY, maxY, ((GLfloat)-1), (GLfloat)1);

	// set the event handling methods
	glutDisplayFunc(repaint);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyFunction);
	glutSpecialFunc(specialKeyFunction);
	glutMouseFunc(mouseFunction);
	glutMainLoop();

	return 0;
}

//#endif
