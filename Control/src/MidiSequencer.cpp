#include "MidiSequencer.h"



bool MidiSequencerEvent::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
    
}

bool MidiSequencerEvent::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    
}

bool MidiSequencerEvent::mouseDragged(int mouseX, int mouseY)
{
    GuiElement::mouseDragged(mouseX, mouseY);
    
}

bool MidiSequencerEvent::mouseReleased(int mouseX, int mouseY)
{
    GuiElement::mouseReleased(mouseX, mouseY);

}


MidiSequencer::MidiSequencer(string name) : GuiWidgetBase(name)
{
    setRectangle(100, 100, 500, 400);
    
    rows = 24;

    rowHeight = rectangle.height / rows;
    
    newEvent = new MidiSequencerEvent(0, 0, 0, 0);
    
    
    period = 10000;
}

MidiSequencer::~MidiSequencer()
{
    for (auto m : events) {
        delete m;
    }
    events.clear();
}

void MidiSequencer::update()
{
    time = ofGetElapsedTimeMillis();
    
    tt  = fmod((double) time / period, 1.0);
    
    //cout << "t is " << time << " " << tt << endl;
    
    for (auto m : events) {
        //m->checkNote(tt);
        
        
        if (!m->expired) {
            if (tt >= m->t1 && tt <= m->t2) {
                cout << "FIRE NOTE " << m->row << " " <<m->velocity << endl;
                m->on = true;
                GuiMidiEventArgs args(1, m->row, m->velocity);
                ofNotifyEvent(midiEvent, args, this);
            }
            else {
                if (m->on) {
                    cout << "DONE " << m->row << " " <<m->velocity << endl;
                    
                    m->on = false;
                    m->expired = true;
                    GuiMidiEventArgs args(0, m->row, m->velocity);
                    ofNotifyEvent(midiEvent, args, this);
                }
            }
        }

    }
}

void MidiSequencer::draw()
{
    ofSetColor(50, 0, 0);
    ofRect(rectangle);
    
    ofSetColor(0, 255, 0);
    for (auto m : events) {
        m->draw();
    }
    
    
    if (mouseDragging) {
        ofSetColor(0, 0, 255);
        newEvent->draw();
        
    }
    
    ofSetColor(255);
    ofLine(tt * rectangle.width + rectangle.x, rectangle.y, tt * rectangle.width + rectangle.x, rectangle.y + rectangle.height);
    
}

void MidiSequencer::setupGuiComponents()
{
}

bool MidiSequencer::mouseMoved(int mouseX, int mouseY)
{
    GuiWidgetBase::mouseMoved(mouseX, mouseY);
    
    
    
    return false;
}

bool MidiSequencer::mousePressed(int mouseX, int mouseY)
{
    if (GuiWidgetBase::mousePressed(mouseX, mouseY)) return true;
    if (mouseOver)
    {
        newEvent->setRow(floor((mouseY - rectangle.y) / (rectangle.height / rows)));
        newEvent->setBeginning((mouseX - rectangle.x) / rectangle.width);
        newEvent->setEnd(newEvent->getBeginning());
        
        
        newEvent->setRectangle(rectangle.x + newEvent->t1 * rectangle.width,
                               rectangle.y + newEvent->row * rowHeight,
                               (newEvent->t2 - newEvent->t1) * rectangle.width,
                               rowHeight);

    }
    return false;
}

bool MidiSequencer::mouseDragged(int mouseX, int mouseY)
{
    GuiWidgetBase::mouseDragged(mouseX, mouseY);
    if (mouseDragging)
    {
        newEvent->setEnd((mouseX - rectangle.x) / rectangle.width);
        
        newEvent->setRectangle(rectangle.x + newEvent->t1 * rectangle.width,
                               rectangle.y + newEvent->row * rowHeight,
                               (newEvent->t2 - newEvent->t1) * rectangle.width,
                               rowHeight);

        
    }
    return false;
}

bool MidiSequencer::mouseReleased(int mouseX, int mouseY)
{
    if (GuiWidgetBase::mouseReleased(mouseX, mouseY)) return true;
    
    MidiSequencerEvent *midiEvent = new MidiSequencerEvent(newEvent->getRow(), newEvent->getBeginning(), newEvent->getEnd(), newEvent->getVelocity());
    events.push_back(midiEvent);
    midiEvent->setRectangle(rectangle.x + midiEvent->t1 * rectangle.width,
                            rectangle.y + midiEvent->row * rowHeight,
                            (midiEvent->t2 - midiEvent->t1) * rectangle.width,
                            rowHeight);
    return false;
}
