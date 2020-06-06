//SENIOR DESIGN

#include <Path.h>
#include <iostream>
#include <GL/glut.h>
#include <random>
#include <algorithm>
#include <fstream>
#include <cmath>

Path *m_graphics = NULL;

const float windowWidth = 600;
const float windowHeight = 800;
const int rows = 50;
const int columns = 50;

Path::Path(){

}

Path::~Path(void){
	
}

void Path::BuildGrid(){
	for (int i = 0; i < columns; i++)
		myGrid.push_back(std::vector<int>());
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			myGrid[i].push_back(0);
		}
	}
}

void Path::MainLoop(void){
	m_graphics = this;

	//Create window
	static int argc = 1;
	static char *args = (char*)"args";
	glutInit(&argc, &args);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Path Finding");

	glutDisplayFunc(CallbackEventOnDisplay);
	glutMouseFunc(CallbackEventOnMouse);
	glutKeyboardFunc(CallbackEventOnNormalKeyPress);

	//Enter main event loop
	glutMainLoop();     
}

void Path::CallbackEventOnDisplay(){
	if (m_graphics)
	{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, windowWidth, windowHeight, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_graphics->HandleEventOnDisplay();
	glutPostRedisplay();
	glutSwapBuffers();
	}
}

void Path::CallbackEventOnMouse(int button, int state, int x, int y){
	if (m_graphics && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		double mouseX, mouseY;
		MousePosition(x, y, &mouseX, &mouseY);
		m_graphics -> HandleEventOnMouseLeftButtonDown(mouseX, mouseY);
		glutPostRedisplay();
	}
}

void Path::CallbackEventOnNormalKeyPress(unsigned char key, int x, int y){
	if (m_graphics && m_graphics->HandleEventOnNormalKeyPress(key))
		glutPostRedisplay();
}

void Path::HandleEventOnDisplay(){
	DrawGrid();
	ColorGrid();
}

void Path::HandleEventOnMouseLeftButtonDown(const double mousePosX, const double mousePosY){
	double xCoastCell = (mousePosX * columns)/windowWidth;
	double yCoastCell = (mousePosY * rows)/windowHeight;
	//int coastCell = columns * (ceil(yCoastCell) - 1) + ceil(xCoastCell);
	int xCoast = (int) floor(xCoastCell);
	int yCoast = (int) floor(yCoastCell);
	myGrid[xCoast][yCoast] = 1;
}

bool Path::HandleEventOnNormalKeyPress(unsigned char key){
	if (key == 27)
	{
		exit(0);
		return true;
	}
	else if (key == 'a')
	{
		if (firstPress)
		{
			for (int i = rows - 1; i >= 0; i--)
			{
				if (myGrid[0][i] == 0)
				{
					myGrid[0][i] = 2;
					lastCellX = 0;
					lastCellY = i;
					traverseList.push_back(columns * lastCellY + lastCellX);
					break;
				}
			}
			firstPress = false;
		}
		else
		{
			if (direction == 1)
			{
				if (lastCellY < (rows - 1) && myGrid[lastCellX][lastCellY+1] == 0)
				{
					myGrid[lastCellX][lastCellY+1] = 2;
					lastCellY++;
					traverseList.push_back(columns * lastCellY + lastCellX);
				}
				else if (lastCellX < (columns - 1) && myGrid[lastCellX+1][lastCellY] == 0)
				{
					myGrid[lastCellX+1][lastCellY] = 2;
					lastCellX++;
					traverseList.push_back(columns * lastCellY + lastCellX);
				}
				else if (lastCellY > 0 && myGrid[lastCellX][lastCellY-1] == 0)
				{
					myGrid[lastCellX][lastCellY-1] = 2;
					lastCellY--;
					traverseList.push_back(columns * lastCellY + lastCellX);
					if (lastCellX == columns - 1)
						direction = 2;
				}
				else if (lastCellX > 0 && myGrid[lastCellX-1][lastCellY] == 0)
				{
					myGrid[lastCellX-1][lastCellY] = 2;
					lastCellX--;
					traverseList.push_back(columns * lastCellY + lastCellX);
				}
				else
				{
					if (!traverseList.empty())
						traverseList.pop_back();
					lastCellY = traverseList.back() / columns;
					lastCellX = traverseList.back() % columns;
					myGrid[lastCellX][lastCellY] = 4;
				}
			}	
			else
			{
				if (lastCellY < (rows - 1) && myGrid[lastCellX][lastCellY+1] == 0)
				{
					myGrid[lastCellX][lastCellY+1] = 2;
					lastCellY++;
					traverseList.push_back(columns * lastCellY + lastCellX);
				}
				else if (lastCellX > 0 && myGrid[lastCellX-1][lastCellY] == 0)
				{
					myGrid[lastCellX-1][lastCellY] = 2;
					lastCellX--;
					traverseList.push_back(columns * lastCellY + lastCellX);
				}
				else if (lastCellY > 0 && myGrid[lastCellX][lastCellY-1] == 0)
				{
					myGrid[lastCellX][lastCellY-1] = 2;
					lastCellY--;
					traverseList.push_back(columns * lastCellY + lastCellX);
					if (lastCellX == 0)
						direction = 1;
				}
				else if (lastCellX < (columns - 1) && myGrid[lastCellX+1][lastCellY] == 0)
				{
					myGrid[lastCellX+1][lastCellY] = 2;
					lastCellX++;
					traverseList.push_back(columns * lastCellY + lastCellX);
				}
				else
				{
					if (!traverseList.empty())
						traverseList.pop_back();
					lastCellY = traverseList.back() / columns;
					lastCellX = traverseList.back() % columns;
					myGrid[lastCellX][lastCellY] = 4;
				}
			}
		}
	}
	return true;
}

void Path::DrawGrid(){
	for (float i = 0.0; i <= rows; i++)
	{
		glBegin(GL_LINES);
		glLineWidth(3.0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.0, (windowHeight*i)/rows);
		glVertex2f(windowWidth, (windowHeight*i)/rows);
		glEnd();
	}
	for (float i = 0.0; i <= columns; i++)
	{
		glBegin(GL_LINES);
		glLineWidth(3.0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f((windowWidth*i)/columns, 0.0);
		glVertex2f((windowWidth*i)/columns, windowHeight);
		glEnd();
	}
}

void Path::ColorGrid(){
	for (float i = 0; i < rows; i++)
	{
		for (float j = 0; j < columns; j++)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (myGrid[i][j] == 0)
			{
				glColor3f(1.0f, 1.0f, 1.0f);
			}
			else if (myGrid[i][j] == 1)
			{
				glColor3f(0.0f, 1.0f, 0.0f);
			}
			else if (myGrid[i][j] == 2)
			{
				glColor3f(0.0f, 0.0f, 1.0f);
			}
			else if (myGrid[i][j] == 4)
			{
				glColor3f(0.76f, 0.05f, 0.9375f);
			}
			glBegin(GL_POLYGON);
			glVertex2d( (i * windowWidth) / columns, (j * windowHeight) / rows);
			glVertex2d( (i * windowWidth) / columns, ( (j + 1) * windowHeight) / rows);
			glVertex2d( ( (i + 1) * windowWidth) / columns, ( (j + 1) * windowHeight) / rows);
			glVertex2d( ( (i + 1) * windowWidth) / columns, (j * windowHeight) / rows);
			glEnd();
			
		}
	}
//Add purple color for cells that is traversed more than once
}

void Path::MousePosition(const int x, const int y, double *posX, double *posY){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, posX, posY, &posZ);
}

int main(){
	Path Path;
	Path.BuildGrid();
	Path.MainLoop();
	return 0;
}
