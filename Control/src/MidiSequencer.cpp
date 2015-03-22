#include "MidiSequencer.h"


MidiEventArgs::MidiEventArgs(int type, int note, int velocity)
{
    this->type = type;
    this->note = note;
    this->velocity = velocity;
}

MidiSequencer::MidiSequencerEvent::MidiSequencerEvent(int note, int velocity, int start, int end) : GuiElement("midiEvent")
{
    this->start = start;
    this->end = end;
    this->note = note;
    this->velocity = velocity;
    
    GuiElement::setAutoUpdate(false);
    GuiElement::setAutoDraw(false);
}

void MidiSequencer::MidiSequencerEvent::draw()
{
    ofRect(rectangle);
}

MidiSequencer::MidiSequencer(string name) : GuiWidgetBase(name)
{
    cout << "CREATE MIDI SEQUENCER " << name << " " << this <<endl;
    
    root = 0;
    key = 1;
    
    keyboardWidth = MIDISEQUENCER_DEFAULT_KEYBOARD_WIDTH;
    velocityHeight = MIDISEQUENCER_DEFAULT_VELOCITY_HEIGHT;
    toolbarHeight = MIDISEQUENCER_DEFAULT_TOOLBAR_HEIGHT;
    timeScrollHeight = MIDISEQUENCER_DEFAULT_TIMEBAR_HEIGHT;

    bpm = MIDISEQUENCER_DEFAULT_BPM;
    measures = MIDISEQUENCER_DEFAULT_MEASURES;
    noteMin = MIDISEQUENCER_DEFAULT_NOTE_MIN;
    noteMax = MIDISEQUENCER_DEFAULT_NOTE_MAX;
    start = MIDISEQUENCER_DEFAULT_START;
    end = MIDISEQUENCER_DEFAULT_END;

    colorForeground = MIDISEQUENCER_DEFAULT_COLOR_FOREGROUND;
    colorActive = MIDISEQUENCER_DEFAULT_COLOR_ACTIVE;
    colorOutline = MIDISEQUENCER_DEFAULT_COLOR_OUTLINE;

    setupKeys();
 
    newEvent = new MidiSequencerEvent(0, 0, 0, 0);
    selectedEvent = NULL;
    mouseOverEvent = NULL;
    mouseOverGrid = false;
    mouseOverKeyboard = false;
    mouseOverVelocity = false;
    
    string roots[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    rootSelect = new GuiMenu("select key", vector<string>(roots, roots + 12), this, &MidiSequencer::eventSelectRoot);
    rootSelect->setAutoUpdate(false);
    rootSelect->setAutoDraw(false);
    rootSelect->setAutoClose(true);
    rootSelect->setCollapsed(true);

    string keys[] = {"major", "minor"};
    keySelect = new GuiMenu("select key", vector<string>(keys, keys + 2), this, &MidiSequencer::eventSelectKey);
    keySelect->setAutoUpdate(false);
    keySelect->setAutoDraw(false);
    keySelect->setAutoClose(true);
    keySelect->setCollapsed(true);
    
    play = new GuiButton("play", this, &MidiSequencer::eventPlay);
    pause = new GuiButton("pause", this, &MidiSequencer::eventPause);
    stop = new GuiButton("stop", this, &MidiSequencer::eventStop);
    sBpm = new GuiSlider<int>("bpm", &bpm, 10, 1200, this, &MidiSequencer::eventSetBpm);
    sPeriod = new GuiSlider<int>("measures", &measures, 4, 100, this, &MidiSequencer::eventSetPeriod);
    rTime = new GuiRangeSlider<float>("", &start, &end, 0.0f, 1.0f, this, &MidiSequencer::eventSetTimeView);

    play->setParent(this);
    pause->setParent(this);
    stop->setParent(this);
    sBpm->setParent(this);
    sPeriod->setParent(this);
    rTime->setParent(this);
    play->setAutoUpdate(false);
    pause->setAutoUpdate(false);
    stop->setAutoUpdate(false);
    sBpm->setAutoUpdate(false);
    sPeriod->setAutoUpdate(false);
    rTime->setAutoUpdate(false);
    play->setAutoDraw(false);
    pause->setAutoDraw(false);
    stop->setAutoDraw(false);
    sBpm->setAutoDraw(false);
    sPeriod->setAutoDraw(false);
    rTime->setAutoDraw(false);
    
    shift = false;

    beat = 0;
    clock.setBeatPerBar(1);
    ofAddListener(clock.beatEvent, this, &MidiSequencer::eventBeat);

    setBpm(bpm);
    setPeriod(measures);
    
    setTimeDisplayRange(start, end);
    setNoteDisplayRange(noteMin, noteMax);
}

void MidiSequencer::setupKeys()
{
    int minor[] = {0, 2, 3, 5, 7, 8, 10};
    int major[] = {0, 2, 4, 5, 7, 9, 11};
    for (int i = 0; i < 12; i++)
    {
        rootIsMinor[i] = false;
        rootIsMajor[i] = false;
    }
    for (int i = 0; i < 7; i++)
    {
        rootIsMinor[minor[i]] = true;
        rootIsMajor[major[i]] = true;
    }
}

MidiSequencer::~MidiSequencer()
{
    for (auto m : events) {
        delete m;
    }
    events.clear();
    ofRemoveListener(clock.beatEvent, this, &MidiSequencer::eventBeat);
}

void MidiSequencer::setActive(bool active)
{
    this->active = active;
    active ? clock.start() : clock.stop();
}

void MidiSequencer::setNumberBeats(int numBeats)
{
    this->numBeats = numBeats;
    midiOnEvents.resize(numBeats);
    midiOffEvents.resize(numBeats);
    setupGuiPositions();
}

void MidiSequencer::setBpm(int bpm)
{
    this->bpm = bpm;
    clock.setBpm(bpm);
}

void MidiSequencer::setPeriod(int measures)
{
    this->measures = measures;
    setNumberBeats(measures * 16);
}

void MidiSequencer::setNoteDisplayRange(int noteMin, int noteMax)
{
    this->noteMin = noteMin;
    this->noteMax = noteMax;
    this->numRows = noteMax - noteMin + 1;
    keyboardSkip = ceil(numRows * 12 / rectKeyboard.height);
    setupGuiPositions();
}

void MidiSequencer::setTimeDisplayRange(float start, float end)
{
    this->start = ofClamp(start, 0.0, 1.0);
    this->end = ofClamp(end, 0.0, 1.0);
    setupGuiPositions();
}

void MidiSequencer::setMidiEventRectangle(MidiSequencerEvent *event)
{
    if (event->note < noteMin || event->note > noteMax ||
        (float) event->start / numBeats > end || (float) event->end / numBeats < start) {
        event->setRectangle(0, 0, 0, 0);
    }
    else
    {
        
        event->setRectangle(rectGrid.x + rectGrid.width * ofClamp((float) event->start / numBeats - start, 0.0, 1.0) / (end - start),
                            rectGrid.y + (noteMax - event->note) * rowHeight,
                            rectGrid.width * (min(end, (float) event->end / numBeats) - max(start, (float) event->start / numBeats)) / (end - start),
                            rowHeight);
    }
}

MidiSequencer::MidiSequencerEvent * MidiSequencer::addMidiEvent(int note, int velocity, int start, int end)
{
    MidiSequencerEvent *midiEvent = new MidiSequencerEvent(note, velocity, start, end);
    events.push_back(midiEvent);
    midiOnEvents[start].push_back(midiEvent);
    midiOffEvents[end].push_back(midiEvent);
    setMidiEventRectangle(midiEvent);
    return midiEvent;
}

void MidiSequencer::repositionMidiEvent(MidiSequencerEvent* event)
{
    for (int b = 0; b < midiOnEvents.size(); b++)
    {
        vector<MidiSequencerEvent*>::iterator it1 = midiOnEvents[b].begin();
        vector<MidiSequencerEvent*>::iterator it2 = midiOffEvents[b].begin();
        while (it1 != midiOnEvents[b].end()) {
            *it1 == event ? midiOnEvents[b].erase(it1) : ++it1;
        }
        while (it2 != midiOffEvents[b].end()) {
            *it2 == event ? midiOffEvents[b].erase(it2) : ++it2;
        }
    }
    midiOnEvents[event->start].push_back(event);
    midiOffEvents[event->end].push_back(event);
}

void MidiSequencer::removeEvent(MidiSequencerEvent *event, bool toDelete)
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
    
    if (toDelete) {
        delete event;
    }
}

void MidiSequencer::removeSelectedEvents()
{
    vector<MidiSequencerEvent*> eventsToRemove;
    for (auto m : events)
    {
        if (m->getRectangle().getWidth() > 0)
        {
            if (selection.inside(m->getRectangle().x, m->getRectangle().y) ||
                selection.inside(m->getRectangle().x + m->getRectangle().width, m->getRectangle().y) ||
                selection.inside(m->getRectangle().x + m->getRectangle().width, m->getRectangle().y + m->getRectangle().height) ||
                selection.inside(m->getRectangle().x, m->getRectangle().y + m->getRectangle().height)) {
                eventsToRemove.push_back(m);
                removeEvent(m, false);
            }
        }
    }
    
    vector<MidiSequencerEvent*>::iterator itr = eventsToRemove.begin();
    while (itr != eventsToRemove.end())
    {
        delete *itr;
        eventsToRemove.erase(itr);
    }
    eventsToRemove.clear();
}

void MidiSequencer::eventBeat()
{
    beat = (beat + 1) % numBeats;
    
    if (midiOnEvents[beat].size() > 0)
    {
        for (auto m : midiOnEvents[beat])
        {
            MidiEventArgs args(1, m->note, m->velocity);
            ofNotifyEvent(midiEvent, args, this);
        }
    }
    
    if (midiOffEvents[beat].size() > 0)
    {
        for (auto m : midiOffEvents[beat])
        {
            MidiEventArgs args(0, m->note, m->velocity);
            ofNotifyEvent(midiEvent, args, this);
        }
    }
}

void MidiSequencer::eventSelectRoot(GuiElementEventArgs &e)
{
    root = e.cell;
}

void MidiSequencer::eventSelectKey(GuiElementEventArgs &e)
{
    key = e.name == "major" ? 1 : 0;
}

void MidiSequencer::eventPlay(GuiElementEventArgs &e)
{
    if (active) {
        beat = 0;
    }
    else {
        setActive(true);
    }
}

void MidiSequencer::eventPause(GuiElementEventArgs &e)
{
    setActive(!active);
}

void MidiSequencer::eventStop(GuiElementEventArgs &e)
{
    setActive(false);
    beat = 0;
    for (int i = 0; i < 127; i++)
    {
        MidiEventArgs args(0, i, 127);
        ofNotifyEvent(midiEvent, args, this);
    }
}

void MidiSequencer::eventSetBpm(GuiElementEventArgs &e)
{
    setBpm(bpm);
}

void MidiSequencer::eventSetPeriod(GuiElementEventArgs &e)
{
    setPeriod(measures);
}

void MidiSequencer::eventSetTimeView(GuiElementEventArgs &e)
{
    setTimeDisplayRange(start, end);
}

void MidiSequencer::draw()
{
    ofSetColor(colorBackground);
    ofRect(rectangle);

    drawGrid();
    drawKeyboard();
    drawVelocity();
    drawToolbar();
}

void MidiSequencer::drawGrid()
{
    if (active)
    {
        float t = (float) beat / numBeats;
        if (t > end || t < start) {
            setTimeDisplayRange(t, t + end - start);
        }
    }
    
    ofSetLineWidth(1);
    ofSetColor(225);
    ofRect(rectGrid);
    
    ofSetColor(250);
    for (int r = 0; r < numRows; r++)
    {
        if ((key == 0 && rootIsMinor[(root + noteMax - r) % 12]) ||
            (key == 1 && rootIsMajor[(root + noteMax - r) % 12])) {
            ofRect(rectGrid.x, rectGrid.y + r * rowHeight, rectGrid.width, rowHeight);
        }
    }
    
    ofSetColor(50, 120);
    for (int c = floor(start * numBeats); c < ceil(end * numBeats); c++)
    {
        float x = ofClamp(ofMap((float) c / numBeats, start, end, rectGrid.x, rectGrid.x + rectGrid.width), rectGrid.x, rectGrid.x + rectGrid.width);
        ofSetLineWidth(c % 64 == 0 ? 4 : (c % 16 == 0 ? 3 : (c % 4 == 0 ? 2 : 1)));
        ofLine(x, rectGrid.y, x, rectGrid.y + rectGrid.height);
    }
    ofSetLineWidth(1);
    for (int r = 0; r <= numRows; r++)
    {
        (root + 1 + noteMax - r) % 12 == 0 ? ofSetColor(255, 0, 0, 150) : ofSetColor(50, 120);
        ofLine(rectGrid.x, rectGrid.y + r * rowHeight, rectGrid.x + rectGrid.width, rectGrid.y + r * rowHeight);
    }
    
    ofSetColor(colorForeground);
    for (auto m : events) {
        m->draw();
    }
    
    if (selection.getWidth() > 0)
    {
        ofSetColor(colorActive);
        ofNoFill();
        ofSetLineWidth(2);
        for (auto m : events)
        {
            if (m->getRectangle().getWidth() > 0)
            {
                if (selection.inside(m->getRectangle().x, m->getRectangle().y) ||
                    selection.inside(m->getRectangle().x + m->getRectangle().width, m->getRectangle().y) ||
                    selection.inside(m->getRectangle().x + m->getRectangle().width, m->getRectangle().y + m->getRectangle().height) ||
                    selection.inside(m->getRectangle().x, m->getRectangle().y + m->getRectangle().height)) {
                    m->draw();
                }
            }
        }
    }
    else if (selectedEvent != NULL)
    {
        ofSetColor(colorActive);
        ofNoFill();
        ofSetLineWidth(2);
        selectedEvent->draw();
    }
    
    if (mouseDragging && mouseOverEvent == NULL)
    {
        ofSetColor(colorActive);
        ofFill();
        newEvent->draw();
    }
    else if (mouseOverEvent != NULL)
    {
        ofSetColor(0, 200);
        if (mouseOverEventMode == LEFT) {
            ofLine(mouseOverEvent->getRectangle().x - 5, mouseOverEvent->getRectangle().y - 2, mouseOverEvent->getRectangle().x - 5, mouseOverEvent->getRectangle().y + mouseOverEvent->getRectangle().height + 2);
        }
        else if (mouseOverEventMode == RIGHT) {
            ofLine(mouseOverEvent->getRectangle().x + mouseOverEvent->getRectangle().width + 5, mouseOverEvent->getRectangle().y - 2, mouseOverEvent->getRectangle().x + mouseOverEvent->getRectangle().width + 5, mouseOverEvent->getRectangle().y + mouseOverEvent->getRectangle().height + 2);
        }
        else if (mouseOverEventMode == TOP || mouseOverEventMode == BOTTOM)
        {
            ofSetColor(colorOutline);
            ofSetLineWidth(2);
            ofLine(mouseOverEvent->getRectangle().x, mouseOverEvent->getRectangle().y + (127.0 - mouseOverEvent->velocity) * mouseOverEvent->getRectangle().height / 127.0, mouseOverEvent->getRectangle().x + mouseOverEvent->getRectangle().width, mouseOverEvent->getRectangle().y + (127.0 - mouseOverEvent->velocity) * mouseOverEvent->getRectangle().height / 127.0);
        }
    }
    
    float bt = ((float) beat / numBeats - start) / (end - start);
    if (bt >= 0.0 && bt < 1.0)
    {
        ofSetColor(0);
        ofSetLineWidth(4);
        ofLine(rectGrid.x + rectGrid.width * bt, rectGrid.y, rectGrid.x + rectGrid.width * bt, rectGrid.y + rectGrid.height);
    }
}

void MidiSequencer::drawKeyboard()
{
    ofSetColor(0);
    ofRect(rectKeyboard);
    
    ofSetColor(100);
    ofSetLineWidth(1);
    for (int r = 0; r < numRows; r++)
    {
        if ((key == 0 && rootIsMinor[(root + noteMax - r) % 12]) ||
            (key == 1 && rootIsMajor[(root + noteMax - r) % 12])) {
            ofSetColor(200);
        }
        else {
            ofSetColor(150);
        }
        ofFill();
        ofRect(rectKeyboard.x, rectKeyboard.y + r * rowHeight, rectKeyboard.width, rowHeight);
        ofSetColor(0, 180);
        ofLine(rectKeyboard.x, rectKeyboard.y + (r + 1) * rowHeight, rectKeyboard.x + rectKeyboard.width, rectKeyboard.y + (r + 1) * rowHeight);
        ofSetColor(0);
        if (r % keyboardSkip == 0) {
            ofDrawBitmapString(ofToString(noteMax - r), rectKeyboard.x + 8, rectKeyboard.y + (r + 1) * rowHeight - 1);
        }
    }
    
    if (noteSelected && selectedNote >= noteMin && selectedNote <= noteMax)
    {
        ofSetColor(colorActive);
        ofFill();
        ofRect(rectKeyboard.x, rectKeyboard.y + (noteMax - selectedNote) * rowHeight, rectKeyboard.width, rowHeight);
        ofSetColor(colorText);
        ofDrawBitmapString(ofToString(selectedNote), rectKeyboard.x + 8, rectKeyboard.y + (noteMax - selectedNote + 1) * rowHeight - 1);
    }
}

void MidiSequencer::drawVelocity()
{
    ofSetColor(255);
    ofRect(rectVelocity);
    
    for (int c = floor(start * numBeats); c < ceil(end * numBeats); c++)
    {
        float x = ofClamp(ofMap((float) c / numBeats, start, end, rectVelocity.x, rectVelocity.x + rectVelocity.width), rectVelocity.x, rectVelocity.x + rectVelocity.width);
        for (int i = 0; i < midiOnEvents[c].size(); i++)
        {
            midiOnEvents[c][i] == selectedEvent ? ofSetColor(colorActive, 150) : ofSetColor(colorForeground, 150);
            float height = ofMap(midiOnEvents[c][i]->velocity, 0, 127, 0, rectVelocity.height);
            ofRect(x, rectVelocity.y + rectVelocity.height, colWidth, -height);
        }
    }
}

void MidiSequencer::drawToolbar()
{
    if (ofGetKeyPressed(OF_KEY_TAB)) {
        cout << "start/end " << start << "/" << end << ", rows " << numRows << ", numBeats " << numBeats << " rowHeight/colWidth " << rowHeight << "," <<colWidth << endl;
    }

    // draw toolbar
    ofSetColor(0, 0, 200);
    ofRect(rectToolbar);
    ofSetColor(0, 100, 200);
    ofRect(rectTimebar);
    
    ofSetColor(colorText);
    ofDrawBitmapString(name, rectangle.x + 10, rectangle.y + 20);

    keySelect->update();
    rootSelect->update();
    play->update();
    pause->update();
    stop->update();
    sBpm->update();
    sPeriod->update();
    rTime->update();

    play->draw();
    pause->draw();
    stop->draw();
    sBpm->draw();
    sPeriod->draw();
    rTime->draw();
    keySelect->draw();
    rootSelect->draw();
}

bool MidiSequencer::mouseMoved(int mouseX, int mouseY)
{
    GuiWidgetBase::mouseMoved(mouseX, mouseY);
    mouseOverGrid = rectGrid.inside(mouseX, mouseY);
    mouseOverKeyboard = rectKeyboard.inside(mouseX, mouseY);
    mouseOverVelocity = rectVelocity.inside(mouseX, mouseY);
    
    rootSelect->mouseMoved(mouseX, mouseY);
    keySelect->mouseMoved(mouseX, mouseY);
    play->mouseMoved(mouseX, mouseY);
    pause->mouseMoved(mouseX, mouseY);
    stop->mouseMoved(mouseX, mouseY);
    sBpm->mouseMoved(mouseX, mouseY);
    sPeriod->mouseMoved(mouseX, mouseY);
    rTime->mouseMoved(mouseX, mouseY);
    
    if (!mouseDragging)
    {
        mouseOverEvent = NULL;
        for (auto m : events)
        {
            if (m->getRectangle().inside(mouseX, mouseY))
            {
                mouseOverEvent = m;
                mouseOverEventMode = MIDDLE;
                float eventMargin = min(2.0, (m->end - m->start) / 3.0);
                if ((mouseX - m->getRectangle().getX()) / colWidth < eventMargin) {
                    mouseOverEventMode = LEFT;
                }
                else if ((m->getRectangle().getX() + m->getRectangle().getWidth() - mouseX) / colWidth < eventMargin) {
                    mouseOverEventMode = RIGHT;
                }
                else if ((mouseY - m->getRectangle().getY()) / m->getRectangle().getHeight() < 0.25) {
                    mouseOverEventMode = TOP;
                }
                else if ((m->getRectangle().getY() + m->getRectangle().getHeight() - mouseY) / m->getRectangle().getHeight() < 0.25) {
                    mouseOverEventMode = BOTTOM;
                }
                return true;
            }
        }
    }
    return false;
}

bool MidiSequencer::mousePressed(int mouseX, int mouseY)
{
    if (GuiWidgetBase::mousePressed(mouseX, mouseY)) return true;
    if (mouseOver)
    {
        if      (rootSelect->mousePressed(mouseX, mouseY)) return true;
        else if (keySelect->mousePressed(mouseX, mouseY)) return true;
        else if (play->mousePressed(mouseX, mouseY)) return true;
        else if (pause->mousePressed(mouseX, mouseY)) return true;
        else if (stop->mousePressed(mouseX, mouseY)) return true;
        else if (sBpm->mousePressed(mouseX, mouseY)) return true;
        else if (sPeriod->mousePressed(mouseX, mouseY)) return true;
        else if (rTime->mousePressed(mouseX, mouseY)) return true;

        selection.set(0, 0, 0, 0);
        pMouse.set(mouseX, mouseY);
        pNoteMin = noteMin;
        pNoteMax = noteMax;
        pStart = start;
        pEnd = end;
        
        if (mouseOverGrid)
        {
            if (mouseOverEvent != NULL)
            {
                pMouseOverEventNoteVelocity.set(mouseOverEvent->note, mouseOverEvent->velocity);
                pMouseOverEventStartEnd.set(mouseOverEvent->start, mouseOverEvent->end);
            }
            else
            {
                newEvent->note = noteMax - floor((mouseY - rectGrid.y) / rowHeight);
                newEvent->start = floor(numBeats * (start + (float) (mouseX - rectGrid.x) * (end - start) / rectGrid.width));
                newEvent->end = newEvent->start;
                setMidiEventRectangle(newEvent);
            }
            return true;
        }
        else if (mouseOverKeyboard)
        {
            if (!noteSelected)
            {
                noteSelected = true;
                selectedNote = noteMax - floor((mouseY - rectGrid.y) / rowHeight);
                MidiEventArgs args(1, selectedNote, 127);
                ofNotifyEvent(midiEvent, args, this);
            }
            return true;
        }
    }
    return false;
}

bool MidiSequencer::mouseDragged(int mouseX, int mouseY)
{
    GuiWidgetBase::mouseDragged(mouseX, mouseY);
    if (mouseDragging)
    {
        if      (rootSelect->mouseDragged(mouseX, mouseY)) return true;
        else if (keySelect->mouseDragged(mouseX, mouseY)) return true;
        else if (play->mouseDragged(mouseX, mouseY)) return true;
        else if (pause->mouseDragged(mouseX, mouseY)) return true;
        else if (stop->mouseDragged(mouseX, mouseY)) return true;
        else if (sBpm->mouseDragged(mouseX, mouseY)) return true;
        else if (sPeriod->mouseDragged(mouseX, mouseY)) return true;
        else if (rTime->mouseDragged(mouseX, mouseY)) return true;

        if (mouseOverGrid)
        {
            if (ofGetKeyPressed(OF_KEY_SHIFT))
            {
                float nextRange = ofClamp(ofMap(mouseX - pMouse.x, -rectGrid.width, rectGrid.width, pEnd - pStart - 1.0, pEnd - pStart + 1.0), 0.001, 1.0);
                float nextEnd = ofClamp(pStart + nextRange, pStart, 1.0);
                float nextStart = ofClamp(nextEnd - nextRange, 0.0, pStart);
                setTimeDisplayRange(nextStart, nextEnd);
            }
            else if (ofGetKeyPressed(OF_KEY_COMMAND))
            {
                selection.set(pMouse.x, pMouse.y, ofGetMouseX() - pMouse.x, ofGetMouseY() - pMouse.y);
            }
            else if (mouseOverEvent != NULL)
            {
                if (mouseOverEventMode == MIDDLE)
                {
                    int diffX = (mouseX - pMouse.x) / colWidth;
                    mouseOverEvent->start = pMouseOverEventStartEnd.x + diffX;
                    mouseOverEvent->end = pMouseOverEventStartEnd.y + diffX;
                    mouseOverEvent->note = pMouseOverEventNoteVelocity.x - (mouseY - pMouse.y) / rowHeight;
                    repositionMidiEvent(mouseOverEvent);
                }
                else if (mouseOverEventMode == LEFT)
                {
                    mouseOverEvent->start = pMouseOverEventStartEnd.x + (mouseX - pMouse.x) / colWidth;
                    repositionMidiEvent(mouseOverEvent);
                }
                else if (mouseOverEventMode == RIGHT)
                {
                    mouseOverEvent->end = pMouseOverEventStartEnd.y + (mouseX - pMouse.x) / colWidth;
                    repositionMidiEvent(mouseOverEvent);
                }
                else if (mouseOverEventMode == TOP || mouseOverEventMode == BOTTOM)
                {
                    mouseOverEvent->velocity = ofClamp(pMouseOverEventNoteVelocity.y + 10.0 * ((pMouse.y - mouseY) / rowHeight), 0, 127);
                }
                setMidiEventRectangle(mouseOverEvent);
                return true;
            }
            else
            {
                newEvent->end = floor(numBeats * (start + (mouseX - rectGrid.x) * (end - start) / rectGrid.width));
                setMidiEventRectangle(newEvent);
                return true;
            }
        }
        else if (mouseOverKeyboard)
        {
            if (ofGetKeyPressed(OF_KEY_SHIFT))
            {
                //int range = ofClamp(127 * (mouseY - rectKeyboard.y) / rectKeyboard.height, 1, 127);
                int range = ofClamp((pNoteMax - pNoteMin) * (1.0 + 0.01 * (mouseY - pMouse.y)), 1, 127);
                setNoteDisplayRange(pNoteMin - 0.5 * (range - (pNoteMax - pNoteMin)),
                                    pNoteMax + 0.5 * (range - (pNoteMax - pNoteMin)));
            }
            else
            {
                int diff = ofClamp((mouseY - pMouse.y) / rowHeight, -pNoteMin, 127 - pNoteMax);
                setNoteDisplayRange(pNoteMin + diff, pNoteMax + diff);
            }
            return true;
        }
    }
    return false;
}

bool MidiSequencer::mouseReleased(int mouseX, int mouseY)
{
    if (GuiWidgetBase::mouseReleased(mouseX, mouseY)) return true;
    if (mouseOver)
    {
        if      (rootSelect->mouseReleased(mouseX, mouseY)) return true;
        else if (keySelect->mouseReleased(mouseX, mouseY)) return true;
        else if (play->mouseReleased(mouseX, mouseY)) return true;
        else if (pause->mouseReleased(mouseX, mouseY)) return true;
        else if (stop->mouseReleased(mouseX, mouseY)) return true;
        else if (sBpm->mouseReleased(mouseX, mouseY)) return true;
        else if (sPeriod->mouseReleased(mouseX, mouseY)) return true;
        else if (rTime->mouseReleased(mouseX, mouseY)) return true;

        if (mouseOverGrid)
        {
            if (newEvent->start != newEvent->end)
            {
                selectedEvent = addMidiEvent(newEvent->note, 127, newEvent->start, newEvent->end);
                newEvent->end = 0;
                newEvent->start = 0;
            }
            else
            {
                selectedEvent = NULL;
                for (auto m : events)
                {
                    if (m->getRectangle().inside(mouseX, mouseY)) {
                        selectedEvent = m;
                    }
                }
            }
            return true;
        }
        if (noteSelected)
        {
            MidiEventArgs args(0, selectedNote, 127);
            ofNotifyEvent(midiEvent, args, this);
            noteSelected = false;
            return true;
        }
    }
    return false;
}

bool MidiSequencer::keyPressed(int key)
{
    if (rootSelect->keyPressed(key)) return true;
    else if (keySelect->keyPressed(key)) return true;
    else if (play->keyPressed(key)) return true;
    else if (pause->keyPressed(key)) return true;
    else if (stop->keyPressed(key)) return true;
    else if (sBpm->keyPressed(key)) return true;
    else if (sPeriod->keyPressed(key)) return true;
    else if (rTime->keyPressed(key)) return true;

    else if (mouseOverGrid)
    {
        if (key == OF_KEY_BACKSPACE)
        {
            if (selection.getWidth() > 0) {
                removeSelectedEvents();
            }
            if (selectedEvent != NULL)
            {
                removeEvent(selectedEvent);
                selectedEvent = NULL;
            }
            return true;
        }
        else if (key == OF_KEY_UP)
        {
            if (selectedEvent != NULL)
            {
                if (selectedEvent->note == 127) return true;
                selectedEvent->note = selectedEvent->note + 1;
                setMidiEventRectangle(selectedEvent);
            }
            else
            {
                if (noteMin == 127 || noteMax == 127) return true;
                setNoteDisplayRange(noteMin + 1, noteMax + 1);
            }
            return true;
        }
        else if (key == OF_KEY_DOWN)
        {
            if (selectedEvent != NULL)
            {
                if (selectedEvent->note == 0) return true;
                selectedEvent->note = selectedEvent->note - 1;
                setMidiEventRectangle(selectedEvent);
            }
            else
            {
                if (noteMin == 0 || noteMax == 0) return true;
                setNoteDisplayRange(noteMin - 1, noteMax - 1);
            }
            return true;
        }
        else if (key == OF_KEY_LEFT)
        {
            if (selectedEvent != NULL)
            {
                if (selectedEvent->start == 0 || selectedEvent->end == 0) return true;
                selectedEvent->start = selectedEvent->start - 1;
                selectedEvent->end = selectedEvent->end - 1;
                repositionMidiEvent(selectedEvent);
                setMidiEventRectangle(selectedEvent);
            }
            return true;
        }
        else if (key == OF_KEY_RIGHT)
        {
            if (selectedEvent != NULL)
            {
                if (selectedEvent->start == numBeats || selectedEvent->end == numBeats) return true;
                selectedEvent->start = selectedEvent->start + 1;
                selectedEvent->end = selectedEvent->end + 1;
                repositionMidiEvent(selectedEvent);
                setMidiEventRectangle(selectedEvent);
            }
            return true;
        }
        else if (key == ' ')
        {
            int note = noteMax + 1 - (ofGetMouseY() - rectGrid.y) / rowHeight;
            int start = floor(numBeats * (start + (ofGetMouseX() - rectGrid.x) * (end - start) / rectGrid.width));
            selectedEvent = addMidiEvent(note, 127, start, start + 1);
            return true;
        }
    }
    return false;
}

void MidiSequencer::setupGuiPositions()
{
    rectToolbar.set(rectangle.x, rectangle.y, rectangle.width, toolbarHeight);
    rootSelect->setRectangle(rectToolbar.x + 110, rectToolbar.y + 5, 100, 14);
    keySelect->setRectangle(rectToolbar.x + 230, rectToolbar.y + 5, 100, 14);
    play->setRectangle(rectToolbar.x + 350, rectToolbar.y + 8, 60, 14);
    pause->setRectangle(rectToolbar.x + 420, rectToolbar.y + 8, 60, 14);
    stop->setRectangle(rectToolbar.x + 490, rectToolbar.y + 8, 60, 14);
    sBpm->setRectangle(rectToolbar.x + 560, rectToolbar.y + 8, 100, 14);
    sPeriod->setRectangle(rectToolbar.x + 670, rectToolbar.y + 8, 100, 14);
    
    if (collapsed)
    {
        rectTimebar.set(0, 0, 0, 0);
        rTime->setRectangle(0, 0, 0, 0);
        rectGrid.set(0, 0, 0, 0);
        rectVelocity.set(0, 0, 0, 0);
        rectKeyboard.set(0, 0, 0, 0);
    }
    else
    {
        rectTimebar.set(rectangle.x + keyboardWidth + 2 * marginInner, rectangle.y + toolbarHeight + marginInner, rectangle.width - keyboardWidth - 3 * marginInner, timeScrollHeight);
        rTime->setRectangle(rectTimebar.x, rectTimebar.y + 2, rectTimebar.width, rectTimebar.height - 4);
        rectGrid.set(rectangle.x + keyboardWidth + 2 * marginInner, rectangle.y + timeScrollHeight + toolbarHeight + marginInner, rectangle.width - keyboardWidth - 3 * marginInner, rectangle.height - velocityHeight - timeScrollHeight - toolbarHeight - 3 * marginInner);
        rectVelocity.set(rectangle.x + keyboardWidth + 2 * marginInner, rectangle.y + rectangle.height - velocityHeight - marginInner, rectangle.width - keyboardWidth - 3 * marginInner, velocityHeight);
        rectKeyboard.set(rectangle.x + marginInner, rectangle.y + timeScrollHeight + toolbarHeight + marginInner, keyboardWidth, rectangle.height - velocityHeight - timeScrollHeight - toolbarHeight - 3 * marginInner);
    }

    colWidth = rectGrid.width / (numBeats * (end - start));
    rowHeight = rectGrid.height / numRows;
    
    cout << "row height is " << rowHeight << " : " << rectGrid.height << "/ "<<numRows << endl;
    
    for (auto m : events) {
        setMidiEventRectangle(m);
    }
    setMidiEventRectangle(newEvent);
}

