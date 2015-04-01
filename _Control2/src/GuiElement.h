#pragma once

#include "ofMain.h"
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
    virtual bool getCollapsed() {return false;}
    virtual bool isDiscrete() {return false;}

    void setMouseOver(bool mouseOver);
    
    virtual void lerpTo(float nextSliderValue, int lerpNumFrames) { }
    virtual void setValueFromSequence(Sequence &sequence) { }
    virtual void setSequenceFromValue(Sequence &sequence, int column) { }
    virtual void setSequenceFromExplicitValue(Sequence &sequence, int column, float value) { }
    
    virtual void getXml(ofXml &xml) { }
    virtual void setFromXml(ofXml &xml) { }
    
    virtual void setupGuiPositions();
    void resetGuiPositions();
    
    
    /*
    
    ////====================================================
    
    virtual void setupSequences()
    {
        return;
        
        Sequence *sequence = new Sequence(getName(), 8);
        sequence->setAutoUpdate(false);
        sequence->setAutoDraw(false);
        sequence->setDiscrete(false);       // inherit true in button/menu
        sequences.push_back(sequence);
        hasSequencer = true;
        setupSequencePositions();
    }
    
    virtual void drawSequences()
    {
        return;
        
        if (hasSequencer)
        {
            for (auto s : sequences) {
                s->draw();
            }
        }
    }
    
    virtual void updateSequences()
    {
        return;
        
        if (hasSequencer)
        {
            for (auto s : sequences) {
                s->update();
            }
        }
    }
    
    virtual void setupSequencePositions()
    {
        return;
        
        sequences[0]->setPosition(x + width + 4, y);
        sequences[0]->setSize(320, height);
    }
    
    vector<Sequence*> sequences;
    bool hasSequencer = false;
    
    
    */
    
    
protected:
    
    void initialize();

    GuiElement *parent;
    bool hasParent;
    
};
