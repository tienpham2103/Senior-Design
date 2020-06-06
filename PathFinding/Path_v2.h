#ifndef Path_H_INCLUDED
#define Path_H_INCLUDED

#include <string>
#include <vector>
#include <list>

class Path
{
public:
     Path(); //Constructor
     ~Path(void); //Destructor
     void MainLoop(void); //Main event loop
     void BuildGrid();
protected:
     void HandleEventOnDisplay();
     void HandleEventOnMouseLeftButtonDown(const double mousePosX, const double mousePosY);
     bool HandleEventOnNormalKeyPress(unsigned char key);
     static void CallbackEventOnDisplay();
     static void CallbackEventOnMouse(int button, int state, int x, int y);
     static void CallbackEventOnNormalKeyPress(unsigned char key, int x, int y);
     static void MousePosition(const int x, const int y, double *posX, double *posY);
     void DrawGrid();
     void ColorGrid();

private:
     std::vector<std::vector<int> > myGrid;
     std::list<int> traverseList;
     int lastCellX, lastCellY;
     bool firstPress = true;
     int direction = 1; // 1: moving right. 2: moving left
};

#endif
