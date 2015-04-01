#pragma once

#include "ofMain.h"
#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiConstants.h"
#include "GuiElement.h"



class GuiElementGroup
{
public:
    GuiElementGroup(string name) {
        this->name = name;
    }
    GuiElementGroup() {
        this->name = "groupname";
    }
    ~GuiElementGroup();
    
    void addElement(GuiElement * element);
    void removeElement(string name);
    void clearToggles();
    
    string getName() {return name;}
    vector<GuiElement*> & getElements() {return elements;}
    
private:
    
    string name;
    vector<GuiElement*> elements;
};


class GuiWidgetBase : public GuiElement
{
public:
    GuiWidgetBase();
    GuiWidgetBase(string name);
    virtual ~GuiWidgetBase();
    void initialize();
    
    void setActive(bool active);
    
    virtual void savePreset(string path);
    virtual void loadPreset(string path);
    
    void setParent(GuiWidgetBase *parent);
    GuiWidgetBase * getParent() {return parent;}
    bool getHasParent() {return hasParent;}
    
    void setName(string name);
    void setAddress(string address);
    string getAddress() {return address;}

    virtual vector<ParameterBase*> getParameters();
    vector<GuiElementGroup*> & getElementGroups();
    void getAllElementGroups(vector<GuiElementGroup*> & allGroups);
    void getAllElements(vector<GuiElement*> & allElements);
    GuiElement * getElement(string name);
    void removeElement(string name);
    void clearElements();

    void setList(bool list);
    void setCollapsed(bool collapsed);
    
    bool & getList() {return list;}
    bool getCollapsed() {return hasParent ? collapsed || parent->getCollapsed() : collapsed;}
    bool isWidget() {return true;}
    
    void addBoundWidget(GuiWidgetBase *other);
    void removeBoundWidget(GuiWidgetBase *other);
    
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

    void setRectangle(ofRectangle rectangle);
    void setRectangle(int x, int y, int width, int height);

    virtual bool isMenu() {return false;}
    
    ofEvent<string> widgetChanged;
    ofEvent<GuiElement*> elementDeletedEvent;
    
protected:
    
    virtual void getXml(ofXml &xml);
    virtual void setFromXml(ofXml &xml);

    virtual void setupElementGroup(GuiElementGroup *elementGroup);
    virtual void setupGuiPositions();
    void scrollFocus(int scroll);
    
    // elements
    vector<ParameterBase*> parameters;
    vector<GuiElementGroup*> elementGroups;
    vector<GuiWidgetBase*> boundWidgets;
    GuiWidgetBase *parent;
    bool hasParent;
    string address;
    
    // header
    ofRectangle headerRectangle;
    bool headerActive;
    bool list, collapsed;
    string header;
    float headerStringHeight;
    ofPoint pMouse;
    ofPoint pPosition;
    
    // style
    ofColor headerColor;
    int headerHeight;
    int controllerHeight;
    int marginInner;
    int marginOuterX;
    int marginOuterY;
};

