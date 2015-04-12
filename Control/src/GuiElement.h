#pragma once

#include "ofMain.h"
#include "ofBitmapFont.h"
#include "GuiConstants.h"
#include "GuiBase.h"
#include "Sequence.h"


class Sequence;


class GuiElement : public GuiBase
{
public:
    
    
    virtual void getParameters(vector<ParameterBase*> & parameters) { }
    
    
    GuiElement(string name);
    GuiElement();

    void setParent(GuiElement *parent);
    GuiElement * getParent() {return parent;}
    bool getHasParent() {return hasParent;}
    virtual bool isMultiElement() {return false;}
    virtual bool getCollapsed();
    virtual bool isDiscrete() {return false;}

    void setMouseOver(bool mouseOver);
    
    virtual void lerpTo(float nextSliderValue, int lerpNumFrames) { }
    
    virtual void setValueFromSequence(Sequence &sequence) { }
    virtual void setSequenceFromValue(Sequence &sequence, int column) { }
    virtual void setSequenceFromExplicitValue(Sequence &sequence, int column, float value) { }
    
    virtual void getXml(ofXml &xml);
    virtual void setFromXml(ofXml &xml);
    
    virtual void setupGuiPositions();
    void resetGuiPositions();
    
    
    string display;
    
    void setupDisplayString()
    {
        display = name;
        int displayWidth = ofBitmapStringGetBoundingBox(display, 0, 0).width;
        while (displayWidth > getWidth())
        {
            display = display.substr(0, display.length()-1);
            displayWidth = ofBitmapStringGetBoundingBox(display, 0, 0).width;
        }
    }
    

    
    string getAddress() {
        return hasParent ? parent->getAddress() + "/" + getName() : getName();
    }
    
    bool getActive() {
        return hasParent ? parent->getActive() && active : active;
    }
    
    void setActive(bool active)
    {
        GuiBase::setActive(active);
        resetGuiPositions();
    }
    
protected:
    
    GuiElement *parent;
    bool hasParent;
};
