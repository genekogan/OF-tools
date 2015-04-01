#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "GuiSlider.h"
//#include "Sequence.h"



class GuiMultiElement : public GuiElement
{
public:
    
    GuiMultiElement(string name);
    virtual ~GuiMultiElement();
    
    virtual bool isMultiElement() {return true;}
    
    vector<GuiElement*> & getElements() {return elements;}
    
    bool removeElement(string name);
    void clearElements();
    
    void setHeader(string header);
    void setCollapsible(bool collapsible);
    
    bool getCollapsed();
    void setCollapsed(bool collapsed);

    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    virtual void update();
    virtual void draw();
    
    ///////
    void getXml(ofXml &xml)
    {
        //xml.addValue("Name", getName());
        //xml.addValue<float>("Value", getValue());
    }
    void setFromXml(ofXml &xml)
    {
        //setValue(xml.getValue<float>("Value"));
    }
    
    //////
    

protected:
    
    void setupGuiPositions();
    virtual void initializeElement(GuiElement *element);
    
    vector<GuiElement*> elements;

    // header
    ofRectangle headerRectangle;
    string header;
    bool headerActive;
    bool collapsible, collapsed;
    float headerStringHeight;
    bool draggingWidget;
    
    // style
    ofColor headerColor;
    int headerHeight;
    int controllerHeight;
    int marginX;
    int marginY;
};
