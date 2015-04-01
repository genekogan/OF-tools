#include "Sequencer.h"







Sequencer::Sequencer(string name, GuiWidget * panel, int numCols) : GuiElement(name)
{
    this->panel = panel;
    this->numCols = numCols;
    this->bpm = 120; // DEFAULT_BPM
    
    sequencerPanelMargin = GUI_DEFAULT_SEQUENCER_PANEL_MARGIN;
    sequencerWidth = GUI_DEFAULT_SEQUENCER_WIDTH;
    sequencerButtonHeight = GUI_DEFAULT_ELEMENT_HEIGHT;
    marginInner = GUI_DEFAULT_MARGIN_Y;
    lerpNumFrames = 1;
    
    //ofAddListener(panel->elementDeletedEvent, this, &Sequencer::eventElementDeleted);
    //getElementGroupsFromWidget(panel);
    
    ofAddListener(panel->newElementEvent, this, &Sequencer::newElementEvent);
    setupSequencer();   // setup seuquencer elements
    
    
    // MAIN PARAMS + DEFAULTS
    //setCollapsible(false);
    setSmooth(false);
    setAllSequencersActive(true);
    setBpm(bpm);
    setNumberColumns(numCols);
    
    GuiElement::setAutoUpdate(true);
    GuiElement::setAutoDraw(true);
}

Sequencer::~Sequencer()
{
    ofRemoveListener(clock.beatEvent, this, &Sequencer::eventBeat);
    ofRemoveListener(sBeatPm->sliderEvent, this, &Sequencer::eventBpm);
    ofRemoveListener(sNumCol->sliderEvent, this, &Sequencer::eventNumColumns);
    ofRemoveListener(panel->newElementEvent, this, &Sequencer::newElementEvent);
    
    delete tActive;
    delete tSmooth;
    delete sBeatPm;
    delete sNumCol;
    delete sLerpNf;
    delete bRandom;
    delete mChoose;
    
    
    
    /*
    for (auto s : sequencePairs) {
        delete s;
    }
    sequencePairs.clear();
    */
    
//    removeElementGroupsFromWidget(panel);
}

void Sequencer::setActive(bool active)
{
    this->active = active;
    active ? clock.start() : clock.stop();
}

void Sequencer::setSmooth(bool smooth)
{
    this->smooth = smooth;
}

void Sequencer::setBpm(int bpm)
{
    this->bpm = bpm;
    bpmInterval = 60000.0 / bpm;
    clock.setBpm(bpm);
    clock.setBeatPerBar(4);
}

void Sequencer::setNumberColumns(int numCols)
{
    this->numCols = numCols;
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            ofRectangle e = p->element->getRectangle();
            p->sequence->setNumberCells(numCols);
        }
    }
    //setupGuiPositions();
}

void Sequencer::saveSequencerSet(string sequencerName)
{
    if (sequencerName == "")
    {
        sequencerName = ofSystemTextBoxDialog("New sequencer set name:");
        if (sequencerName == "")   return;
    }
    
    SequencerSavedSet newSet(sequencerName, smooth, bpm, lerpNumFrames, numCols);
    
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            string sequenceName = p->element->getParent()->getName() + "/" + p->element->getName();
            newSet.addSequence(sequenceName, p->sequence->getAllValues(), p->sequence->getActive());
        }
    }
    sequencerSets[sequencerName] = newSet;
    mChoose->addToggle(sequencerName);
}

void Sequencer::loadSequencerSet(string sequencerName)
{
    if (sequencerSets.count(sequencerName) == 0)
    {
        ofLog(OF_LOG_ERROR, "Error: no sequencer set found named " + sequencerName);
        return;
    }
    
    setNumberColumns(sequencerSets[sequencerName].numCols);
    setSmooth(sequencerSets[sequencerName].smooth);
    setBpm(sequencerSets[sequencerName].bpm);
    lerpNumFrames = sequencerSets[sequencerName].lerpNumFrames;

    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            string sequenceName = p->element->getParent()->getName() + "/" + p->element->getName();
            if (sequencerSets[sequencerName].sequences.count(sequenceName) != 0)
            {
                p->sequence->setFromValues(sequencerSets[sequencerName].sequences[sequenceName]);
                p->sequence->setActive(sequencerSets[sequencerName].sequencesActive[sequenceName]);
            }

        }
    }
}

void Sequencer::setupSequencer()
{
    tActive = new GuiToggle("on", &active, this, &Sequencer::eventActive);
    tSmooth = new GuiToggle("~", &smooth);
    bRandom = new GuiButton("?", this, &Sequencer::eventRandomizeSequencer);
    sBeatPm = new GuiSlider<int>("bpm", &bpm, 1, 360);
    sNumCol = new GuiSlider<int>("cols", &numCols, 1, 64);
    sLerpNf = new GuiSlider<int>("lerp", &lerpNumFrames, 1, 120);
    mChoose = new GuiMenu("choose", this, &Sequencer::eventSequencerMenuSelection, false, true);
    mChoose->addToggle("save new");
    mChoose->setCollapsed(true);
    
    tActive->setAutoUpdate(false);
    tActive->setAutoDraw(false);
    tSmooth->setAutoUpdate(false);
    tSmooth->setAutoDraw(false);
    bRandom->setAutoUpdate(false);
    bRandom->setAutoDraw(false);
    sBeatPm->setAutoUpdate(false);
    sBeatPm->setAutoDraw(false);
    sNumCol->setAutoUpdate(false);
    sNumCol->setAutoDraw(false);
    sLerpNf->setAutoUpdate(false);
    sLerpNf->setAutoDraw(false);
    mChoose->setAutoUpdate(false);
    mChoose->setAutoDraw(false);
    
    ofAddListener(clock.beatEvent, this, &Sequencer::eventBeat);
    ofAddListener(sBeatPm->sliderEvent, this, &Sequencer::eventBpm);
    ofAddListener(sNumCol->sliderEvent, this, &Sequencer::eventNumColumns);

    for (auto element : panel->getElementGroups()) {
        newElementEvent(element);
    }
}

void Sequencer::newElementEvent(GuiElement* & newElement)
{
    if (groups.count(newElement) == 0)
    {
        groups[newElement] = new ElementSequenceGroup(newElement);
        setupGuiPositions();
    }
}

void Sequencer::removeElementEvent(GuiElement* & element)
{
    if (groups.count(element) > 0)
    {
        delete groups[element];
        groups.erase(element);
        setupGuiPositions();
    }
}

void Sequencer::setupGuiPositions()
{
    ofRectangle panelRect = panel->getRectangle();
    rectangle.set(panelRect.x + panelRect.getWidth() + sequencerPanelMargin, panelRect.y, sequencerWidth, panelRect.getHeight());
    
    mChoose->setPosition(rectangle.x +   2, rectangle.y + marginInner);
    tActive->setPosition(rectangle.x + 107, rectangle.y + marginInner);
    tSmooth->setPosition(rectangle.x + 136, rectangle.y + marginInner);
    sBeatPm->setPosition(rectangle.x + 165, rectangle.y + marginInner);
    sLerpNf->setPosition(rectangle.x + 226, rectangle.y + marginInner);
    sNumCol->setPosition(rectangle.x + 287, rectangle.y + marginInner);
    bRandom->setPosition(rectangle.x + 348, rectangle.y + marginInner);
    mChoose->setSize(100, sequencerButtonHeight);
    tActive->setSize( 24, sequencerButtonHeight);
    tSmooth->setSize( 24, sequencerButtonHeight);
    sBeatPm->setSize( 56, sequencerButtonHeight);
    sLerpNf->setSize( 56, sequencerButtonHeight);
    sNumCol->setSize( 56, sequencerButtonHeight);
    bRandom->setSize( 14, sequencerButtonHeight);

    if (groups.size() > 0)
    {
        ofRectangle sequenceRect = groups.begin()->second->pairs[0]->sequence->getSequenceRectangle();
        rectSelectColumnMouseOver = -1;
        rectToggleAllSeqMouseOver = false;
        bToggleAllSequencers.set(rectangle.x, rectangle.y + sequencerButtonHeight + 2 * marginInner, sequenceRect.getHeight(), sequenceRect.getHeight());
        bColumnSelectors.resize(numCols);
        for (int i = 0; i < numCols; i++)
        {
            float x = ofMap(i + 0.15, 0, numCols, sequenceRect.x, sequenceRect.x + sequenceRect.width);
            bColumnSelectors[i].set(x, rectangle.y + sequencerButtonHeight + 2 * marginInner, sequencerWidth * 0.7 / numCols, sequenceRect.getHeight() * 0.7);
        }
    }

    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            ofRectangle e = p->element->getRectangle();
            p->sequence->setPosition(rectangle.x, e.y);
            p->sequence->setSize(sequencerWidth, e.height);
        }
    }
}

void Sequencer::setAllSequencersActive(bool allSequencesActive)
{
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            if (p->element->getActive()) {
                p->sequence->setActive(allSequencesActive);
            }
        }
    }
}

void Sequencer::randomizeSequencer()
{
    
}

void Sequencer::selectColumn(int column)
{
    setActive(false);
    currentColumn = column;
    cursor = currentColumn;

    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            p->sequence->setCursor(cursor);
            if (p->element->getActive()) {
                //p->element->lerpWidgetToSequencer(lerpNumFrames);
            }
        }
    }
}

void Sequencer::setColumnToCurrentValues(int column)
{
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            if (p->element->getActive()) {
                //p->element->setSequenceFromElement(column);
            }
        }
    }
}

void Sequencer::next()
{
    bpmTime = ofGetElapsedTimeMillis();
    currentColumn = fmodf(currentColumn + 1.0f, (float) numCols);
    cursor = currentColumn;
    
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            p->sequence->setCursor(cursor);
            if (!smooth) {
                // update widget from seq
                //            p->updateWidgetFromSequencer();
            }
        }
    }
}








void Sequencer::eventBpm(GuiSliderEventArgs<int> &s)
{
    setBpm(bpm);
}

void Sequencer::eventNumColumns(GuiSliderEventArgs<int> &s)
{
    setNumberColumns(numCols);
}

void Sequencer::eventActive(GuiButtonEventArgs &b)
{
    setActive(b.value);
}

void Sequencer::eventWidgetCollapsed(string &s)
{
    setupGuiPositions();
}

void Sequencer::eventRandomizeSequencer(GuiButtonEventArgs &b)
{
    randomizeSequencer();
}

void Sequencer::eventSelectColumn(GuiSliderEventArgs<int> & b)
{
    selectColumn(b.value);
}

void Sequencer::eventBeat()
{
    next();
}

void Sequencer::eventSequencerMenuSelection(GuiMenuEventArgs & evt)
{
    if (evt.toggle->getName() == "save new") {
        //saveSequencerSet();
    }
    else {
        //loadSequencerSet(evt.toggle->getName());
    }
}








void Sequencer::update()
{
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it) {
        it->second->update();
    }
    
    tActive->update();
    tSmooth->update();
    bRandom->update();
    sBeatPm->update();
    sNumCol->update();
    sLerpNf->update();
    mChoose->update();
}

void Sequencer::draw()
{
    ofSetColor(colorBackground, 50);
    ofFill();
    ofRect(rectangle);
    
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it) {
        it->second->draw();
    }
    
    for (int i=0; i<bColumnSelectors.size(); i++)
    {
        ofSetColor(GUI_DEFAULT_SEQUENCER_COLOR_ACTIVE, ofClamp(ofMap(abs(cursor - i), 0, 1, 255, 0), 0, 255));
        ofFill();
        ofRect(bColumnSelectors[i]);
        rectSelectColumnMouseOver == i ? ofSetColor(colorActive) : ofSetColor(colorOutline);
        ofNoFill();
        ofRect(bColumnSelectors[i]);
    }
    rectToggleAllSeqMouseOver ? ofFill() : ofNoFill();
    allSequencesActive ? ofSetColor(GUI_DEFAULT_SEQUENCER_COLOR_ACTIVE) : ofSetColor(GUI_DEFAULT_SEQUENCER_COLOR_INACTIVE);
    ofCircle(bToggleAllSequencers.x + bToggleAllSequencers.width / 2, bToggleAllSequencers.y + bToggleAllSequencers.width / 3, bToggleAllSequencers.width / 3);

    tActive->draw();
    tSmooth->draw();
    bRandom->draw();
    sBeatPm->draw();
    sNumCol->draw();
    sLerpNf->draw();
    mChoose->draw();
}

bool Sequencer::mouseMoved(int mouseX, int mouseY)
{
    tActive->mouseMoved(mouseX, mouseY);
    tSmooth->mouseMoved(mouseX, mouseY);
    bRandom->mouseMoved(mouseX, mouseY);
    sBeatPm->mouseMoved(mouseX, mouseY);
    sNumCol->mouseMoved(mouseX, mouseY);
    sLerpNf->mouseMoved(mouseX, mouseY);
    mChoose->mouseMoved(mouseX, mouseY);
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it) {
        it->second->mouseMoved(mouseX, mouseY);
    }
    rectToggleAllSeqMouseOver = bToggleAllSequencers.inside(mouseX, mouseY);
    rectSelectColumnMouseOver = -1;
    for (int i = 0; i < bColumnSelectors.size(); i++)
    {
        if (bColumnSelectors[i].inside(mouseX, mouseY)) {
            rectSelectColumnMouseOver = i;
        }
    }
    return mouseOver;
}

bool Sequencer::mousePressed(int mouseX, int mouseY)
{
    if      (tActive->mousePressed(mouseX, mouseY)) return true;
    else if (tSmooth->mousePressed(mouseX, mouseY)) return true;
    else if (bRandom->mousePressed(mouseX, mouseY)) return true;
    else if (sBeatPm->mousePressed(mouseX, mouseY)) return true;
    else if (sNumCol->mousePressed(mouseX, mouseY)) return true;
    else if (sLerpNf->mousePressed(mouseX, mouseY)) return true;
    else if (mChoose->mousePressed(mouseX, mouseY)) return true;
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it) {
        if (it->second->mousePressed(mouseX, mouseY)) return true;
    }
    if (rectToggleAllSeqMouseOver)
    {
        setAllSequencersActive(!allSequencesActive);
        return true;
    }
    else if (rectSelectColumnMouseOver != -1)
    {
        selectColumn(rectSelectColumnMouseOver);
        return true;
    }
    return false;
}

bool Sequencer::mouseDragged(int mouseX, int mouseY)
{
    if      (tActive->mouseDragged(mouseX, mouseY)) return true;
    else if (tSmooth->mouseDragged(mouseX, mouseY)) return true;
    else if (bRandom->mouseDragged(mouseX, mouseY)) return true;
    else if (sBeatPm->mouseDragged(mouseX, mouseY)) return true;
    else if (sNumCol->mouseDragged(mouseX, mouseY)) return true;
    else if (sLerpNf->mouseDragged(mouseX, mouseY)) return true;
    else if (mChoose->mouseDragged(mouseX, mouseY)) return true;
    
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it) {
        if (it->second->mouseDragged(mouseX, mouseY)) return true;
    }
    return false;
}

bool Sequencer::mouseReleased(int mouseX, int mouseY)
{
    if      (tActive->mouseReleased(mouseX, mouseY)) return true;
    else if (tSmooth->mouseReleased(mouseX, mouseY)) return true;
    else if (bRandom->mouseReleased(mouseX, mouseY)) return true;
    else if (sBeatPm->mouseReleased(mouseX, mouseY)) return true;
    else if (sNumCol->mouseReleased(mouseX, mouseY)) return true;
    else if (sLerpNf->mouseReleased(mouseX, mouseY)) return true;
    else if (mChoose->mouseReleased(mouseX, mouseY)) return true;
    
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it) {
        if (it->second->mouseReleased(mouseX, mouseY)) return true;
    }
    return false;
}

bool Sequencer::keyPressed(int key)
{
    if      (tActive->keyPressed(key)) return true;
    else if (tSmooth->keyPressed(key)) return true;
    else if (bRandom->keyPressed(key)) return true;
    else if (sBeatPm->keyPressed(key)) return true;
    else if (sNumCol->keyPressed(key)) return true;
    else if (sLerpNf->keyPressed(key)) return true;
    else if (mChoose->keyPressed(key)) return true;
    
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it) {
        if (it->second->keyPressed(key)) return true;
    }
    return false;
}



