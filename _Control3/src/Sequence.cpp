#include "Sequence.h"


Sequence::SequenceKeyboardEventArgs::SequenceKeyboardEventArgs(int column, float value)
{
    this->column = column;
    this->value = value;
}

Sequence::Sequence(string name, int numCells) : GuiBase(name)
{
    setSize(numCells);
    setupSequence();
}

Sequence::Sequence(string name) : GuiBase(name)
{
    setSize(1);
    setupSequence();
}

Sequence::~Sequence()
{
    values.clear();
    setActive(false);
}

void Sequence::setupSequence()
{
    setActive(true);
    colorForeground = GUI_DEFAULT_SEQUENCER_COLOR_FOREGROUND;
    colorOutline = GUI_DEFAULT_SEQUENCER_COLOR_OUTLINE;
    mouseOverActive = false;
    mouseOverSequencer = false;
}

void Sequence::setupGuiPositions()
{
    activeRectangle = ofRectangle(rectangle.x, rectangle.y + rectangle.height / 8, rectangle.height - rectangle.height / 4, rectangle.height - rectangle.height / 4);
    sequenceRectangle = ofRectangle(rectangle.x + rectangle.height, rectangle.y, rectangle.width - rectangle.height, rectangle.height);
    cellWidth = (float) sequenceRectangle.width / numCells;
}

void Sequence::setActive(bool active)
{
    this->active = active;
}

void Sequence::setDiscrete(bool discrete)
{
    this->discrete = discrete;
}

void Sequence::setSize(int numCells)
{
    this->numCells = numCells;
    values.resize(numCells);
}

void Sequence::setCursor(float cursor)
{
    this->cursor = fmodf(cursor, (float) numCells);
    cursorLerp = cursor - floor(cursor);
}

void Sequence::setValueAtCell(int idx, float value)
{
    if (idx >= numCells)
    {
        ofLog(OF_LOG_ERROR, "Warning: no cell at index "+ofToString(idx));
        return;
    }
    values[idx] = value;
}

void Sequence::setFromValues(vector<float> values)
{
    this->values = values;
    numCells = values.size();
}

void Sequence::randomize(float density, float range)
{
    for (int i = 0; i < numCells; i++) {
        values[i] = discrete ? ofRandom(1) < density : ofLerp(0.5 - 0.5 * range, 0.5 + 0.5 * range, ofRandom(1));
    }
}

float Sequence::getValueAtIndex(int idx)
{
    return values[idx];
}

float Sequence::getValueAtCurrentIndex()
{
    return values[floor(cursor)];
}

float Sequence::getValueAtCursor(float cursor_)
{
    int idx1 = floor(cursor_);
    float cursorLerp_ = cursor_ - floor(cursor_);
    return ofLerp(values[idx1], values[(1+idx1) % values.size()], cursorLerp_);
}

float Sequence::getValueAtCurrentCursor()
{
    int idx1 = floor(cursor);
    return ofLerp(values[idx1], values[(1+idx1) % values.size()], cursorLerp);
}

bool Sequence::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
    mouseOverActive = activeRectangle.inside(mouseX, mouseY);
    mouseOverSequencer = sequenceRectangle.inside(mouseX, mouseY);
    if (mouseOverSequencer)
    {
        activeCell = floor((float) (ofGetMouseX() - sequenceRectangle.x) / cellWidth);
    }
    return mouseOver || mouseOverActive || mouseOverSequencer;
}

bool Sequence::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        if (mouseOverSequencer)
        {
            if (discrete)
            {
                values[activeCell] = (1.0 - values[activeCell]) > 0.5;
//                GuiElementEventArgs args(name, activeCell, values[activeCell]);
//                ofNotifyEvent(elementEvent, args, this);
            }
            else {
                mousePos.set(mouseX, mouseY);
            }
            return true;
        }
        else if (mouseOverActive)
        {
            setActive(!active);
            return true;
        }
    }
    return false;
}

bool Sequence::mouseReleased(int mouseX, int mouseY)
{
    GuiElement::mouseReleased(mouseX, mouseY);
    return mouseOver || mouseOverActive || mouseOverSequencer;
}

bool Sequence::mouseDragged(int mouseX, int mouseY)
{
    GuiElement::mouseDragged(mouseX, mouseY);
    if (mouseDragging && !discrete)
    {
        values[activeCell] = ofClamp(values[activeCell] - 0.005 * (mouseY - mousePos.y), 0, 1);
        mousePos.set(mouseX, mouseY);
    }
    return mouseOver || mouseOverActive || mouseOverSequencer;
}

bool Sequence::keyPressed(int key)
{
    GuiElement::keyPressed(key);
    if (mouseOverSequencer)
    {
        if (key == 46 || (key >= 48 && key <= 57))
        {
            if (editing) {
                editingValue += key;
            }
            else
            {
                editing = true;
                editingValue = key;
            }
            return true;
        }
        else if (key == ' ')
        {
            if (getDiscrete())
            {
                setValueAtCell(activeCell, 1.0 - values[activeCell]);
//                GuiElementEventArgs args(name, activeCell, values[activeCell]);
//                ofNotifyEvent(elementEvent, args, this);
            }
        }
        else if (key == OF_KEY_RETURN)
        {
            SequenceKeyboardEventArgs args(activeCell, ofToFloat(editingValue));
            ofNotifyEvent(keyboardEvent, args, this);
            editingValue = "";
            return true;
        }
    }
    return false;
}

void Sequence::update()
{
    
}

void Sequence::draw()
{
    ofPushStyle();
    
    active ? ofSetColor(GUI_DEFAULT_SEQUENCER_COLOR_ACTIVE) : ofSetColor(GUI_DEFAULT_SEQUENCER_COLOR_INACTIVE);
    ofFill();
    ofCircle(activeRectangle.x + activeRectangle.width / 2 + 1, activeRectangle.y + activeRectangle.height / 2, activeRectangle.width / 2);
    
    if (mouseOverActive)
    {
        ofSetColor(editing ? GUI_DEFAULT_COLOR_ACTIVE_EDIT : colorActive);
        ofNoFill();
        ofSetLineWidth(2);
        ofCircle(activeRectangle.x + activeRectangle.width / 2, activeRectangle.y + activeRectangle.height / 2, activeRectangle.width / 2);
        ofSetLineWidth(1);
    }
    
    if (active)
    {
        for (int i = 0; i < numCells; i++)
        {
            ofFill();
            ofSetColor(colorBackground);
            ofRect(sequenceRectangle.x + i * cellWidth,
                   sequenceRectangle.y,
                   cellWidth,
                   sequenceRectangle.height);
            
            ofSetColor(colorForeground);
            ofRect(sequenceRectangle.x + cellWidth * (i + 0.5 * (1.0 - values[i])),
                   sequenceRectangle.y + sequenceRectangle.height * (0.5 * (1.0 - values[i])),
                   cellWidth * values[i],
                   sequenceRectangle.height * values[i]);
            
            ofNoFill();
            ofSetColor(colorOutline);
            ofRect(sequenceRectangle.x + i * cellWidth, sequenceRectangle.y, cellWidth, sequenceRectangle.height);
        }
        
        if (mouseOverSequencer)
        {
            ofSetColor(colorActive);
            ofNoFill();
            ofSetLineWidth(2);
            ofRect(sequenceRectangle.x + activeCell * cellWidth, sequenceRectangle.y, cellWidth, sequenceRectangle.height);
            ofSetLineWidth(1);
        }
    }
    
    ofPopStyle();
}
