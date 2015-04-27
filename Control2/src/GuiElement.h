#pragma once

#include "ofMain.h"
#include "ofBitmapFont.h"
#include "GuiConstants.h"
#include "GuiBase.h"
#include "Sequence.h"


class Sequence;
class TouchOscPage;

class GuiElement : public GuiBase
{
public:
    GuiElement(string name);
    GuiElement();
    virtual ~GuiElement();
    
    void setParent(GuiElement *parent);
    bool getHasParent() {return hasParent;}
    GuiElement * getParent() {return parent;}

    virtual bool isMultiElement() {return false;}
    virtual bool isDiscrete() {return false;}

    virtual bool getCollapsed();
    void setMouseOver(bool mouseOver);

    string getAddress();
    bool getActive();
    void setActive(bool active);
    
    virtual void getParameters(vector<ParameterBase*> & parameters) { }
    
    virtual void lerpTo(float nextSliderValue, int lerpNumFrames) { }
    virtual void setValueFromSequence(Sequence &sequence) { }
    virtual void setSequenceFromValue(Sequence &sequence, int column) { }
    virtual void setSequenceFromExplicitValue(Sequence &sequence, int column, float value) { }
    
    virtual void getXml(ofXml &xml);
    virtual void setFromXml(ofXml &xml);
    
    virtual void addElementToTouchOscLayout(TouchOscPage *page, float *y);
    virtual void updateParameterOscAddress();

    virtual string getOscAddress() {return getAddress();}
    virtual void sendOsc(ofxOscMessage &msg) { }
    virtual void receiveOsc(ofxOscMessage &msg) { }
    virtual bool valueChanged() { return false; }
    
protected:

    virtual void setupGuiPositions();
    void resetGuiPositions();
    void setupDisplayString();

    GuiElement *parent;
    bool hasParent;
    string display;
};
