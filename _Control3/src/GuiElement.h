#pragma once

#include "ofMain.h"
#include "GuiConstants.h"
#include "GuiBase.h"
#include "Sequencer.h"


//class Sequence;


class GuiElement : public GuiBase
{
public:
    GuiElement(string name);
    GuiElement();

    void setParent(GuiElement *parent);
    GuiElement * getParent() {return parent;}
    bool getHasParent() {return hasParent;}
    virtual bool isMultiElement() {return false;}

    virtual bool getCollapsed() {return false;}

    
    ///////
    virtual void lerpTo(float nextSliderValue, int lerpNumFrames) { }
    virtual void setValueFromSequence(Sequence &sequence) { }
    virtual void setSequenceFromValue(Sequence &sequence, int column) { }
    virtual void setSequenceFromExplicitValue(Sequence &sequence, int column, float value) { }
    ///////
    
    
    
    ////////
    
    virtual void getXml(ofXml &xml) { }
    virtual void setFromXml(ofXml &xml) { }
    ////////
    
    virtual void setupGuiPositions();
    void resetGuiPositions();
    
    
    
    /*
    Sequence *sequence;
    void setupSequencePosition()
    {
        sequence->setPosition(x + width + 4, y);
        sequence->setSize(400, height);
    }
    */
    
    
    
    
protected:
    
    void initialize();

    GuiElement *parent;
    bool hasParent;
    
};
