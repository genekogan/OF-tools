#pragma once

#include "ofMain.h"
#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiConstants.h"
#include "GuiElement.h"


class GuiElementGroup
{
public:
    ~GuiElementGroup();
    void addElement(GuiElement * element);
    vector<GuiElement*> & getElements() {return elements;}
    
private:
    vector<GuiElement*> elements;
};


class GuiWidgetBase : public GuiElement
{
public:
    GuiWidgetBase();
    GuiWidgetBase(string name);
    virtual ~GuiWidgetBase();
    
    void initialize();
    
    void setParent(GuiWidgetBase *parent);
    GuiWidgetBase * getParent() {return parent;}
    bool getHasParent() {return hasParent;}
    
    void setName(string name);
    
    vector<GuiElementGroup*> & getElementGroups();
    void getAllElementGroups(vector<GuiElementGroup*> & allGroups);
    void getAllElements(vector<GuiElement*> & allElements);
    GuiElement * getElement(string name);
    void removeElement(string name);

    void setList(bool list);
    void setCollapsed(bool collapsed);
    
    bool & getList() {return list;}
    bool getCollapsed() {return hasParent ? collapsed || parent->getCollapsed() : collapsed;}
    bool isWidget() {return true;}
    
    virtual bool mouseMoved(int mouseX, int mouseY);
    virtual bool mousePressed(int mouseX, int mouseY);
    virtual bool mouseReleased(int mouseX, int mouseY);
    virtual bool mouseDragged(int mouseX, int mouseY);

    virtual bool keyPressed(int key);

    void update();
    void draw();
    
    void setHeaderColor(ofColor headerColor);
    void setHeaderHeight(int headerHeight);
    void setControllerHeight(int controllerHeight);
    void setMarginOuterX(int marginOuterX);
    void setMarginOuterY(int marginOuterY);
    void setMarginInner(int marginInner);

    ofColor getHeaderColor() {return headerColor;}
    int getHeaderHeight() {return headerHeight;}
    int getControllerHeight() {return controllerHeight;}
    int getMarginOuterX() {return marginOuterX;}
    int getMarginOuterY() {return marginOuterY;}
    int getMarginInner() {return marginInner;}

    virtual bool isMenu() {return false;}
    
    ofEvent<string> widgetChanged;
    ofEvent<GuiElement*> elementDeletedEvent;

    
protected:
    
    void setupElementGroup(GuiElementGroup * elementGroup);
    void setupGuiComponents();
    void scrollFocus(int scroll);
    
    // elements
    vector<GuiElementGroup*> elementGroups;
    GuiWidgetBase *parent;
    bool hasParent;
    
    // header
    ofRectangle headerRectangle;
    bool headerActive;
    bool list, collapsed;
    string header;
    float headerStringHeight;
    
    // style
    ofColor headerColor;
    int headerHeight;
    int controllerHeight;
    int marginInner;
    int marginOuterX;
    int marginOuterY;
};

