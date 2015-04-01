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
    
    /*
    virtual void setupSequences()
    {
        return;
        
        cout << "Setujp " << getName() << endl;
        
        for (int i = 0; i < elements.size(); i++)
        {
            if (elements[i]->isMultiElement()) {
                ((GuiMultiElement *) elements[i])->setupSequences();
            }
            else
            {
                Sequence *sequence = new Sequence(elements[i]->getName(), 8);
                sequence->setAutoUpdate(false);
                sequence->setAutoDraw(false);
                sequence->setDiscrete(false);       // inherit true in button/menu
                sequences.push_back(sequence);
            }
        }
        hasSequencer = true;            // necesary?
        
        setupSequencePositions();  // not here... in setupguipos
    }
    
    virtual void setupSequencePositions()
    {
        return;
        
        for (int i = 0; i < elements.size(); i++)
        {
            if (elements[i]->isMultiElement()) {
                ((GuiMultiElement *) elements[i])->setupSequencePositions();
            }
            else
            {
                ofRectangle e = elements[i]->getRectangle();
                sequences[i]->setPosition(e.x + e.width + 8, e.y);
                sequences[i]->setSize(320, e.height);
            }
        }
    }

    
    virtual void drawSequences()
    {
        return;
        
        //cout << "draw "<<getName()<< " " <<hasSequencer << endl;
        if (hasSequencer)
        {
            for (int i = 0; i < elements.size(); i++)
            {
                if (elements[i]->isMultiElement()) {
                    ((GuiMultiElement *) elements[i])->drawSequences();
                }
                else {
                    sequences[i]->draw();
                }
            }
        }
    }
    
    virtual void updateSequences()
    {
        return;
        
        if (hasSequencer)
        {
            for (int i = 0; i < elements.size(); i++)
            {
                if (elements[i]->isMultiElement()) {
                    ((GuiMultiElement *) elements[i])->updateSequences();
                }
                else {
                    sequences[i]->update();
                }
            }
        }
    }
    
    
    
    vector<Sequence*> & getSequences() {return sequences;}
    
    
    vector<Sequence*> sequences;
    bool hasSequencer = false;

    */
    
    ofEvent<GuiElement*> newElementEvent;
    // eventNewElement, eventDeleteElement,
    // event(widget changed), event(active?)
    

protected:
    
    void setupGuiPositions();
    virtual void initializeElement(GuiElement *element, bool sendNotification=true);
    
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
