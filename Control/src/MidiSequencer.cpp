#include "MidiSequencer.h"



MidiSequencerEvent::MidiSequencerEvent(int row, int velocity, int start, int end) : GuiElement("")
{
    this->row = row;
    this->velocity = velocity;
    this->start = start;
    this->end = end;
    
    setAutoDraw(false);
    setAutoUpdate(false);
}

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

void MidiSequencerEvent::draw()
{
    ofRect(rectangle);
}


MidiSequencer::MidiSequencer(string name) : GuiWidgetBase(name)
{    
    newEvent = new MidiSequencerEvent(0, 0, 0, 0);
    selectedEvent = NULL;
    
    setNumberRows(24);
    beat = 0;
    
    clock.setBeatPerBar(1);
    
    ofAddListener(clock.beatEvent, this, &MidiSequencer::eventBeat);
    setActive(true);
    
    period = 36.0f;
    dt = 0.1f;
    
    clock.setBpm(60.0f / dt);

    setNumberBeats(period / dt);
}

void MidiSequencer::setNumberRows(int rows)
{
    this->rows = rows;
    setupGuiComponents();
}

void MidiSequencer::setNumberBeats(int numBeats)
{
    this->numBeats = numBeats;
    midiOnEvents.resize(numBeats);
    midiOffEvents.resize(numBeats);
    setupGuiComponents();
}

MidiSequencer::~MidiSequencer()
{
    for (auto m : events) {
        delete m;
    }
    events.clear();
    ofRemoveListener(clock.beatEvent, this, &MidiSequencer::eventBeat);
}

void MidiSequencer::update()
{

}

void MidiSequencer::setActive(bool active)
{
    this->active = active;
    active ? clock.start() : clock.stop();
}

void MidiSequencer::addMidiEvent(int row, int velocity, int start, int end)
{
    MidiSequencerEvent *midiEvent = new MidiSequencerEvent(row, velocity, start, end);
    events.push_back(midiEvent);
    midiOnEvents[start].push_back(midiEvent);
    midiOffEvents[end].push_back(midiEvent);
    setMidiEventRectangle(midiEvent);
}

void MidiSequencer::removeEvent(MidiSequencerEvent *event)
{
    for (int b = 0; b < midiOnEvents.size(); b++)
    {
        vector<MidiSequencerEvent*>::iterator it = midiOnEvents[b].begin();
        while (it != midiOnEvents[b].end())
        {
            if (*it == event) {
                midiOnEvents[b].erase(it);
            }
            else {
                ++it;
            }
        }
    }
    
    for (int b = 0; b < midiOffEvents.size(); b++)
    {
        vector<MidiSequencerEvent*>::iterator it = midiOffEvents[b].begin();
        while (it != midiOffEvents[b].end())
        {
            if (*it == event) {
                midiOffEvents[b].erase(it);
            }
            else {
                ++it;
            }
        }
    }
    
    vector<MidiSequencerEvent*>::iterator it = events.begin();
    while (it != events.end())
    {
        if (*it == event) {
            events.erase(it);
        }
        else {
            ++it;
        }
    }

    delete event;
}

void MidiSequencer::eventBeat()
{
    beat = (beat + 1) % numBeats;
    
    if (midiOnEvents[beat].size() > 0)
    {
        for (auto m : midiOnEvents[beat])
        {
            GuiMidiEventArgs args(1, m->getRow(), m->getVelocity());
            ofNotifyEvent(midiEvent, args, this);
        }
    }
    if (midiOffEvents[beat].size() > 0)
    {
        for (auto m : midiOffEvents[beat])
        {
            GuiMidiEventArgs args(0, m->getRow(), m->getVelocity());
            ofNotifyEvent(midiEvent, args, this);
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
    
    if (selectedEvent != NULL) {
        ofSetColor(255, 255, 0);
        selectedEvent->draw();
    }
    
    if (mouseDragging)
    {
        ofSetColor(0, 0, 255);
        newEvent->draw();
    }
    
    ofSetColor(255);
    ofLine(((float) beat / numBeats) * rectangle.width + rectangle.x, rectangle.y, ((float) beat / numBeats) * rectangle.width + rectangle.x, rectangle.y + rectangle.height);
}

void MidiSequencer::setupGuiComponents()
{
    rowHeight = rectangle.height / rows;
    
    for (auto m : events) {
        setMidiEventRectangle(m);
    }
}

void MidiSequencer::setMidiEventRectangle(MidiSequencerEvent *event)
{
    event->setRectangle(rectangle.x + event->getStart() * rectangle.width / numBeats,
                        rectangle.y + event->getRow() * rowHeight,
                        rectangle.width * (event->getEnd() - event->getStart()) / numBeats,
                        rowHeight);
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
        newEvent->setStart((mouseX - rectangle.x) * numBeats / rectangle.width);
        newEvent->setEnd(newEvent->getStart());
        setMidiEventRectangle(newEvent);
    }
    return false;
}

bool MidiSequencer::mouseDragged(int mouseX, int mouseY)
{
    GuiWidgetBase::mouseDragged(mouseX, mouseY);
    if (mouseDragging)
    {
        newEvent->setEnd((mouseX - rectangle.x) * numBeats / rectangle.width);
        setMidiEventRectangle(newEvent);
    }
    return false;
}

bool MidiSequencer::mouseReleased(int mouseX, int mouseY)
{
    if (GuiWidgetBase::mouseReleased(mouseX, mouseY)) return true;
    if (newEvent->getStart() != newEvent->getEnd()) {
        addMidiEvent(newEvent->getRow(), newEvent->getVelocity(), newEvent->getStart(), newEvent->getEnd());
    }
    else
    {
        for (auto m : events)
        {
            if (m->getRectangle().inside(mouseX, mouseY)) {
                selectedEvent = m;
            }
        }
    }
    return false;
}

bool MidiSequencer::keyPressed(int key)
{
    if (GuiWidgetBase::keyPressed(key)) return true;
    if (key == OF_KEY_BACKSPACE) {
        if (selectedEvent != NULL) {
            removeEvent(selectedEvent);
            selectedEvent = NULL;
        }
        return true;
    }
    return false;
}