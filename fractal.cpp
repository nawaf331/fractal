#include<stdio.h>
#include <stdlib.h>
#include <GL/glut.h> // include GLUT library header


int width=600, height=600; // window size
int windowID;
int color=0;

GLfloat minX = -2.2f, maxX = 0.8f, minY = -1.5f, maxY = 1.5; // complex plane boundaries
GLfloat stepX = (maxX - minX)/(GLfloat)width;
GLfloat stepY = (maxY - minY)/(GLfloat)height;



GLfloat black[] = {0.0f, 0.0f, 0.0f}; // black color
const int paletteSize = 128;
GLfloat palette[paletteSize][3];
const GLfloat radius = 5.0f;
bool fullScreen=false;
int fracCount=0;
GLfloat juliaFactor=0.0;
GLfloat juliaSpecial=0.5;
GLfloat zoomFactor=0.1;
//****************************************
GLfloat* greenJulia(GLfloat u, GLfloat v){
	GLfloat re = u;
	GLfloat im = v;
	GLfloat juliaFrac01=-0.1;
	GLfloat juliaFrac02=0.651;
	GLfloat tempRe=0.0;
	for(int i=0; i < paletteSize; i++){
		tempRe = re*re - im*im + juliaFrac01+juliaFactor;
		im = re * im * 2 + juliaFrac02+juliaFactor;
		re = tempRe;
		if( (re*re + im*im) > radius ){
			return palette[i+35];
		}
	}
	return black;
}

GLfloat *mandelbrot(GLdouble u, GLdouble v)
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

GLfloat *mandelbrot3(GLdouble u, GLdouble v)
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

GLfloat *starFractal(GLdouble u, GLdouble v)
{
		GLdouble re = u;
	GLdouble im = v;
	GLdouble tempRe=0.0;
		GLfloat juliaFrac01=-0.1;
	GLfloat juliaFrac02=0.651;

	for(int i=0; i < paletteSize; i++)
	{
		tempRe = re*re*re - re*im*im - 2*re*im*im -( re*re - im*im)+juliaFrac01+ juliaFactor*10;
		im = re*re*im -im*im*im+ 2*re*re*im -(re * im * 2) +juliaFrac02+juliaFactor*10;
			re = tempRe;


			if( (re*re + im*im) > radius )
			{
				return palette[(i+100)];		///for color variation add 45 to color index
			}
	}
		return black;
}

GLfloat *julia(GLdouble u, GLdouble v)
{
	GLdouble re = u;
	GLdouble im = v;
	GLdouble tempRe=0.0;
	GLdouble juliaFrac1=0.36;
	GLdouble juliaFrac2=0.36;
	for(int i=0; i < paletteSize; i++)
		{
			tempRe =  re*re - im*im + juliaFrac1+juliaFactor;
			im = re * im * 2 + juliaFrac2+juliaFactor;
			re = tempRe;
			if( (re*re + im*im) > radius )
			{
				return palette[i];
			}
		}
		return black;
}

GLfloat *flower(GLdouble u, GLdouble v)
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
				//color=32;
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
			juliaSpecial=0.0;
			
			return mandelbrot(u,v);
			break;

	}
}

//****************************************
void repaint() {// function called to repaint the window
	GLfloat* pixelColor;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen buffer
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
	glutSwapBuffers(); // swap the buffers - [ 2 ]
}

//****************************************
void reshape (int w, int h){ // function called when window size is changed
	stepX = (maxX-minX)/(GLfloat)w; // calculate new value of step along X axis
	stepY = (maxY-minY)/(GLfloat)h; // calculate new value of step along Y axis
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
		if(fracCount==0)
		{
			printf("dinka chika\n");
		}
		else {
			
		printf("Zooooooooming area\n");
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
		if(fracCount==0)
		{
			printf("dinka\n");
		}
		else{
		printf("function yet to be implemented\n");
		}
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
			minX = -2.2f, maxX = 0.8f, minY = -1.5f, maxY = 1.5; 
			stepX = (maxX - minX)/(GLfloat)width;
			stepY = (maxY - minY)/(GLfloat)height;
			glLoadIdentity();
			glOrtho(minX, maxX, minY, maxY, ((GLdouble)-1), (GLdouble)1);
			fracCount++;
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			minX = -2.2f, maxX = 0.8f, minY = -1.5f, maxY = 1.5; 
			stepX = (maxX - minX)/(GLfloat)width;
			stepY = (maxY - minY)/(GLfloat)height;
			glLoadIdentity();
			glOrtho(minX, maxX, minY, maxY, ((GLdouble)-1), (GLdouble)1);
			fracCount--;
			glutPostRedisplay();
			break;
		case GLUT_KEY_UP:
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
			printf("Help meee\n");
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
	windowID = glutCreateWindow("---Fractal---");

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
