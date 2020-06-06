#ifndef Path_H_INCLUDED
#define Path_H_INCLUDED

#include <string>
#include <vector>

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
     static void CallbackEventOnNormalKeyPress(unsigned char key);
     static void MousePosition(const int x, const int y, double *posX, double *posY);
     void DrawGrid();
     void DrawCoast();

private:
     std::vector<std::vector<int> > myGrid;
     int direction = 1;
};

#endif
