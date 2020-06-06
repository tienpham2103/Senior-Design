//SENIOR DESIGN

#include <SimpleDrawing.h>
#include <iostream>
#include <GL/glut.h>
#include <random>
#include <algorithm>
#include <fstream>

SimpleDrawing *m_graphics = NULL;
const float windowWidth = 400;
const float windowHeight = 800;
const int mapWidth = 300;
const int mapHeight = 300;
const float light = 0.5;
const float saturation = 1;
std::vector<std::string> vecOfStr;

float minDepth = 2; //centimeters
float maxDepth = 130; //centimeters
float minHue = 0;
float maxHue = 250;

SimpleDrawing::SimpleDrawing(){

}
//////////////////////////////////////////////////////////independent research
SimpleDrawing::~SimpleDrawing(void){
	
}

void SimpleDrawing::MainLoop(void){
	m_graphics = this;

	//Create window
	static int argc = 1;
	static char *args = (char*)"args";
	glutInit(&argc, &args);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Simple Drawing");

	glutDisplayFunc(CallbackEventOnDisplay);
	//glutSpecialFunc(HandleEventOnSpecialKeyPressed);
	//glutKeyboardFunc(HandleEventOnKeyPressed);

	//Enter main event loop
	glutMainLoop();     
}

void SimpleDrawing::CallbackEventOnDisplay(){
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

void SimpleDrawing::HandleEventOnDisplay(){
	ColorDepth();
}

void SimpleDrawing::ColorDepth(){
	std::vector<float>::iterator it;
	int count = 0; //count = current cell to color
	for (it = depthVector.begin(); it != depthVector.end(); it++)
	{
		int nrCol, nrRow;
		nrCol = count%mapWidth + 1;
		nrRow = count/mapWidth + 1;

		float red, green, blue;
		if (*it < 0)
		{red = 1; green = 1; blue = 1;}
		else{		
			float hue = (((*it - minDepth) * (maxHue - minHue)) / (maxDepth - minDepth)) + minHue;
			float *rgbVal;
			rgbVal = HSL_To_RGB(hue, saturation, light);
			red = *rgbVal;
			green = *(rgbVal + 1);
			blue = *(rgbVal + 2);
		}

		glColor3f(red, green, blue);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
		glVertex2f(windowWidth*(nrCol - 1)/mapWidth, windowHeight*(nrRow - 1)/mapHeight);
		glVertex2f(windowWidth*(nrCol - 1)/mapWidth, windowHeight*nrRow/mapHeight);
		glVertex2f(windowWidth*nrCol/mapWidth, windowHeight*nrRow/mapHeight);
		glVertex2f(windowWidth*nrCol/mapWidth, windowHeight*(nrRow - 1)/mapHeight);
		glEnd();
		count++;
	}
	
	
	/*std::vector<float>::iterator it;
	for (it = depthVector.begin(); it!= depthVector.end(); it++)
	{
		if (*it > maxDepth)
			*it = maxDepth;
		float hue = (((*it - minDepth) * (maxHue - minHue)) / (maxDepth - minDepth)) + minHue;
		float *rgbVal;
		rgbVal = HSL_To_RGB(hue, saturation, light);
		float red = *rgbVal;
		float green = *(rgbVal + 1);
		float blue = *(rgbVal + 2);
		glColor3f(red, green, blue);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
	}*/
}

void SimpleDrawing::ProcessVector(){
	//Sample depthVector:

	//Random depthVector:
	std::vector<int> indexVector;
	std::mt19937 rng(time(0));
	std::uniform_int_distribution<int> gen(0, mapWidth*mapHeight-1);

	for (int i = 0; i < mapWidth*mapHeight; i++)
	{
		depthVector.push_back(-1);
		indexVector.push_back(i);
	}

	std::vector<int>::iterator ite;
	for (ite = indexVector.begin(); ite != indexVector.end(); ite++)
	{
		int current = *ite;
		if (current / mapWidth == 0 || current / mapWidth == mapHeight - 1 || current % mapWidth == 0 || current % mapWidth == mapWidth - 1)
		{
			depthVector[current] = 0;
			indexVector.erase(ite--);
		}
	}

	for (int i = 0; i < 5000; i++)
	{
		int r;
		std::vector<int>::iterator position;
		do {
			r = gen(rng); 
			position = std::find(indexVector.begin(), indexVector.end(), r);
		} while ((r / mapWidth == 0 || r / mapWidth == mapHeight - 1 || r % mapWidth == 0 || r % mapWidth == mapWidth - 1) || (position == indexVector.end()));
		depthVector[r] = 1;
		position = std::find(indexVector.begin(), indexVector.end(), r);
		indexVector.erase(position);
	}

	int currentValue = 0;
	while (!indexVector.empty())
	{
		currentValue++;
		for (ite = indexVector.begin(); ite != indexVector.end(); ite++)
		{
			if (depthVector[*ite + 1] == currentValue || depthVector[*ite - 1] == currentValue || depthVector[*ite + mapWidth] == currentValue || depthVector[*ite - mapWidth] == currentValue)
			{
				depthVector[*ite] = currentValue + 1;
				indexVector.erase(ite--);
			}
		}
	}	

	//Random debris
	std::uniform_int_distribution<int> gen2(0, mapWidth*mapHeight-1); //random from 0 to mapWidth*mapHeight-1
	for (int i = 0; i < 30; i++) //Random 30 debris
	{
		int r;
		do {r = gen2(rng);} while (r / mapWidth == 0 || r / mapWidth == mapHeight - 1 || r % mapWidth == 0 || r % mapWidth == mapWidth - 1);
		depthVector[r] = -depthVector[r];
		depthVector[r+mapWidth] = -depthVector[r+mapWidth];
		depthVector[r+1] = -depthVector[r+1];
		depthVector[r+1+mapWidth] = -depthVector[r+1+mapWidth];	
	}


/*	int count = 0;
	for (std::string & line : vecOfStr)
	{
		if (count%3 == 0)
			{xVector.push_back(std::stof(line)); count++;}
		else if (count%3 == 1)
			{yVector.push_back(std::stof(line)); count++;}
		else {depthVector.push_back(std::stof(line)); count++;}
	}
*/
}

float * SimpleDrawing::HSL_To_RGB(float hue, float sat, float light){
	float temp1, temp2, temp3, tempR, tempG, tempB;
	static float RGB[3] = {0.0, 0.0, 0.0};
	if (light < 0.5)
	{
		temp1 = light*(1.0 + sat);
	}
	else {temp1 = light + sat - light*sat;}
	temp2 = 2*light - temp1;
	temp3 = hue/360.0;

	tempR = temp3 + 0.333;
	if (tempR > 1) {tempR = tempR - 1;}
	tempG = temp3;
	tempB = temp3 - 0.333;
	if (tempB < 0) {tempB = tempB + 1;}
	
	while (true)
	{
		if (6*tempR < 1)
		{
			RGB[0] = temp2+(temp1-temp2)*6*tempR;
			break;
		}
		else if (2*tempR < 1)
		{
			RGB[0] = temp1;
			break;
		}
		else if (3*tempR < 2)
		{
			RGB[0] = temp2+(temp1-temp2)*(2.0/3.0 - tempR)*6;
			break;
		}
		else
		{
			RGB[0] = temp2;
			break;
		}
	}
	while (true)
	{
		if (6*tempG < 1)
		{
			RGB[1] = temp2+(temp1-temp2)*6*tempG;
			break;
		}
		else if (2*tempG < 1)
		{
			RGB[1] = temp1;
			break;
		}
		else if (3*tempG < 2)
		{
			RGB[1] = temp2+(temp1-temp2)*(2.0/3.0 - tempG)*6;
			break;
		}
		else
		{
			RGB[1] = temp2;
			break;
		}
	}
	while (true)
	{
		if (6*tempB < 1)
		{
			RGB[2] = temp2+(temp1-temp2)*6*tempB;
			break;
		}
		else if (2*tempB < 1)
		{
			RGB[2] = temp1;
			break;
		}
		else if (3*tempB < 2)
		{
			RGB[2] = temp2+(temp1-temp2)*(2.0/3.0 - tempB)*6;
			break;
		}
		else
		{
			RGB[2] = temp2;
			break;
		}
	}
	RGB[0] = RGB[0]*255;
	RGB[1] = RGB[1]*255;
	RGB[2] = RGB[2]*255;
	return RGB;
}

bool SimpleDrawing::GetFileContent(std::string fileName, std::vector<std::string> & vecOfStr){
	// Open the File
	std::ifstream in(fileName.c_str());
 
	// Check if object is valid
	if(!in)
	{
		std::cerr << "Cannot open the File : "<<fileName<<std::endl;
		return false;
	}
 
	std::string str;
	// Read the next line from File untill it reaches the end.
	while (std::getline(in, str))
	{
		// Line contains string of length > 0 then save it in vector
		if(str.size() > 0)
			vecOfStr.push_back(str);
	}
	//Close The File
	in.close();
	return true;
}

int main(){
	SimpleDrawing simpleDrawing;
	
	//bool result = simpleDrawing.GetFileContent(/*file name*/, vecOfStr);
	//if (result)
	{
		simpleDrawing.ProcessVector();
		simpleDrawing.MainLoop();
	}
	return 0;
}
