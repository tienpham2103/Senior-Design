#ifndef SIMPLEDRAWING_H_INCLUDED
#define SIMPLEDRAWING_H_INCLUDED

#include <string>
#include <vector>

class SimpleDrawing
{
public:
     SimpleDrawing(); //Constructor
     ~SimpleDrawing(void); //Destructor
     void ProcessVector();
     bool GetFileContent(std::string fileName, std::vector<std::string> & vecOfStr);
     void MainLoop(void); //Main event loop
protected:
     void HandleEventOnDisplay();
     static void CallbackEventOnDisplay();
     void ColorDepth();
     std::vector<float> depthVector;
     std::vector<float> xVector;
     std::vector<float> yVector;
     float * HSL_To_RGB(float hue, float sat, float light);
};

#endif
