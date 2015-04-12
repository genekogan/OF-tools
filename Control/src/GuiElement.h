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
    
    virtual void getParameters(vector<ParameterBase*> & parameters) { }
    
    virtual void setValueFromSequence(Sequence &sequence) { }
    virtual void setSequenceFromValue(Sequence &sequence, int column) { }
    virtual void setSequenceFromExplicitValue(Sequence &sequence, int column, float value) { }
    
    virtual void getXml(ofXml &xml);
    virtual void setFromXml(ofXml &xml);
    
    virtual void setupGuiPositions();
    void resetGuiPositions();
    void setupDisplayString();

    string getAddress();
    bool getActive();
    void setActive(bool active);
    
protected:
    
    GuiElement *parent;
    bool hasParent;
    string display;
};
