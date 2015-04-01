#include "Sequencer.h"


Sequencer::SequenceElementPair::SequenceElementPair(GuiElement *element)
{
    this->element = element;
    sequence = new Sequence(element->getName(), 8);
    sequence->setAutoUpdate(false);
    sequence->setAutoDraw(false);
    sequence->setDiscrete(element->isDiscrete());
    ofAddListener(sequence->keyboardEvent, this, &SequenceElementPair::eventSetSequenceFromKey);
}

Sequencer::SequenceElementPair::~SequenceElementPair()
{
    delete sequence;
}

void Sequencer::SequenceElementPair::setElementFromSequence()
{
    if (element->getActive() && sequence->getActive()) {
        element->setValueFromSequence(*sequence);
    }
}

void Sequencer::SequenceElementPair::setSequenceFromElement(int column)
{
    if (element->getActive()) {
        element->setSequenceFromValue(*sequence, column);
    }
}

void Sequencer::SequenceElementPair::lerpWidgetToSequencer(int lerpNumFrames)
{
    if (element->getActive() && sequence->getActive()) {
        element->lerpTo(sequence->getValueAtCurrentCursor(), lerpNumFrames);
    }
}

void Sequencer::SequenceElementPair::eventSetSequenceFromKey(Sequence::SequenceKeyboardEventArgs & s)
{
    element->setSequenceFromExplicitValue(*sequence, s.column, s.value);
}

Sequencer::SequenceGroupPair::SequenceGroupPair(GuiElementGroup *elementGroup, GuiWidgetBase *parent)
{
    this->elementGroup = elementGroup;
    this->parent = parent;
    for (auto &e : elementGroup->getElements())
    {
        SequenceElementPair *newPair = new SequenceElementPair(e);
        elementGroup->addElement(newPair->getSequence());
        pairs.push_back(newPair);
        if (parent->isMenu() && !((GuiMenu *) parent)->getMultipleChoice()) {
            ofAddListener(newPair->getSequence()->elementEvent, this, &SequenceGroupPair::menuSelected);
        }
    }
}

Sequencer::SequenceGroupPair::~SequenceGroupPair()
{
    for (auto p : pairs) {
        delete p;
    }
    pairs.clear();
}

void Sequencer::SequenceGroupPair::menuSelected(GuiElementEventArgs &e)
{
    if (e.value == 0.0) return;
    for (auto p : pairs) {
        p->getSequence()->setValueAtCell(e.cell, p->getSequence()->getName() == e.name);
    }
}

void Sequencer::SequenceGroupPair::setNumberColumns(int numCols)
{
    this->numCols = numCols;
    for (auto p : pairs) {
        p->getSequence()->setSize(numCols);
    }
}

void Sequencer::SequenceGroupPair::setCursor(float cursor)
{
    for (auto p : pairs) {
        p->getSequence()->setCursor(cursor);
    }
}

void Sequencer::SequenceGroupPair::randomizeSequence(float density, float range)
{
    if (parent->isMenu() && !((GuiMenu *) parent)->getMultipleChoice())
    {
        for (int col = 0; col < numCols; col++)
        {
            int rowSelection = ofRandom(pairs.size());
            for (int row = 0; row < pairs.size(); row++) {
                pairs[row]->getSequence()->setValueAtCell(col, row == rowSelection);
            }
        }
    }
    else
    {
        for (auto p : pairs) {
            p->getSequence()->randomize(density, range);
        }
    }
}

void Sequencer::SequenceGroupPair::updateWidgetFromSequencer()
{
    if (parent->isMenu())
    {
        for (auto p : pairs)
        {
            if (p->getSequence()->getActive()) {
                ((GuiMenu *) parent)->setToggle(p->getSequence()->getName(), p->getSequence()->getValueAtCurrentIndex() == 1.0);
            }
        }
    }
    else
    {
        for (auto p : pairs) {
            p->setElementFromSequence();
        }
    }
}

void Sequencer::SequenceGroupPair::lerpWidgetToSequencer(int lerpNumFrames)
{
    for (auto p : pairs) {
        p->lerpWidgetToSequencer(lerpNumFrames);
    }
}

Sequencer::SequencerSavedSet::SequencerSavedSet(string name, bool smooth, int bpm, int lerpNumFrames, int numCols)
{
    this->name = name;
    this->smooth = smooth;
    this->bpm = bpm;
    this->lerpNumFrames = lerpNumFrames;
    this->numCols = numCols;
}

void Sequencer::SequencerSavedSet::addSequence(string sequenceName, vector<float> sequence, bool active)
{
    sequences[sequenceName] = sequence;
    sequencesActive[sequenceName] = active;
}



//////////////////////////////////////////////////
//  Sequencer

Sequencer::Sequencer(string name, GuiWidgetBase * panel, int numCols) : GuiWidgetBase(name)
{
    this->panel = panel;
    this->numCols = numCols;
    
    sequencerPanelMargin = GUI_DEFAULT_SEQUENCER_PANEL_MARGIN;
    sequencerWidth = GUI_DEFAULT_SEQUENCER_WIDTH;
    lerpNumFrames = 1;

    ofAddListener(panel->elementDeletedEvent, this, &Sequencer::eventElementDeleted);
    //getElementGroupsFromWidget(panel);
    setupSequencer();
    
    setList(false);
    setSmooth(false);
    setAllSequencersActive(true);
    setBpm(120);
    setNumberColumns(numCols);
    
    GuiElement::setAutoUpdate(true);
    GuiElement::setAutoDraw(true);
}

Sequencer::~Sequencer()
{
    ofRemoveListener(clock.beatEvent, this, &Sequencer::eventBeat);
    ofRemoveListener(sBeatPm->elementEvent, this, &Sequencer::eventBpm);
    ofRemoveListener(sNumCol->elementEvent, this, &Sequencer::eventNumColumns);
    
    delete tActive;
    delete tSmooth;
    delete sBeatPm;
    delete sNumCol;
    delete sLerpNf;
    delete bRandom;
    delete mChoose;
    
    for (auto s : sequencePairs) {
        delete s;
    }
    sequencePairs.clear();
    
    removeElementGroupsFromWidget(panel);
}

void Sequencer::resetSequencerElements()
{
    
    // TODO :: unsafe, can happen multiple times?
    
    
    cout << "RESET " << endl;
    getElementGroupsFromWidget(panel);
    setupGuiPositions();
}

void Sequencer::getElementGroupsFromWidget(GuiWidgetBase * widget)
{
    vector<GuiElementGroup*> &groups = widget->getElementGroups();
    for (auto group : groups)
    {
        GuiElementGroup * newGroup = new GuiElementGroup();
        vector<GuiElement*> & elements = group->getElements();
        for (auto &element : elements)
        {
            if (element->isWidget()) {
                getElementGroupsFromWidget((GuiWidgetBase*) element);
            }
            else {
                newGroup->addElement(element);
            }
        }
        if (newGroup->getElements().size() > 0)
        {
            SequenceGroupPair *newPair = new SequenceGroupPair(newGroup, widget);
            newPair->setNumberColumns(numCols);
            sequencePairs.push_back(newPair);
        }
    }
    ofAddListener(widget->widgetChanged, this, &Sequencer::eventWidgetCollapsed);
}

void Sequencer::removeElementGroupsFromWidget(GuiWidgetBase * widget)
{
    vector<GuiElementGroup*> &groups = widget->getElementGroups();
    for (auto group : groups)
    {
        vector<GuiElement*> & elements = group->getElements();
        for (auto &element : elements)
        {
            if (element->isWidget()) {
                removeElementGroupsFromWidget((GuiWidgetBase*) element);
            }
        }
    }
    ofRemoveListener(widget->widgetChanged, this, &Sequencer::eventWidgetCollapsed);
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
    
    GuiElementGroup *elementGroup = new GuiElementGroup();
    elementGroup->addElement(tActive);
    elementGroup->addElement(tSmooth);
    elementGroup->addElement(sBeatPm);
    elementGroup->addElement(sNumCol);
    elementGroup->addElement(sLerpNf);
    elementGroup->addElement(bRandom);
    elementGroup->addElement(mChoose);
    setupElementGroup(elementGroup);
    
    ofAddListener(clock.beatEvent, this, &Sequencer::eventBeat);
    ofAddListener(sBeatPm->elementEvent, this, &Sequencer::eventBpm);
    ofAddListener(sNumCol->elementEvent, this, &Sequencer::eventNumColumns);
}

void Sequencer::setupGuiPositions()
{
    setupSequencesFromPanel();
    
    mChoose->setRectangle(rectangle.x +   2, rectangle.y + marginInner,   100, 15);
    tActive->setRectangle(rectangle.x + 107, rectangle.y + marginInner+2,  24, 15);
    tSmooth->setRectangle(rectangle.x + 136, rectangle.y + marginInner+2,  24, 15);
    sBeatPm->setRectangle(rectangle.x + 165, rectangle.y + marginInner+2,  56, 15);
    sLerpNf->setRectangle(rectangle.x + 226, rectangle.y + marginInner+2,  56, 15);
    sNumCol->setRectangle(rectangle.x + 287, rectangle.y + marginInner+2,  56, 15);
    bRandom->setRectangle(rectangle.x + 348, rectangle.y + marginInner+2,  14, 15);
    
    if (sequencePairs.size() > 0)
    {
        ofRectangle rectTopSeq = sequencePairs[0]->getElementPairs()[0]->getSequence()->getSequenceRectangle();
        rectSelectColumnMouseOver = -1;
        rectToggleAllSeqMouseOver = false;
        bToggleAllSequencers.set(rectangle.x, rectTopSeq.y - rectTopSeq.height, rectTopSeq.height, rectTopSeq.height);
        bColumnSelectors.resize(numCols);
        for (int i = 0; i < numCols; i++)
        {
            float x = ofMap(i + 0.15, 0, numCols, rectTopSeq.x, rectTopSeq.x + rectTopSeq.width);
            bColumnSelectors[i].set(x, rectTopSeq.y - rectTopSeq.height, rectTopSeq.width * 0.7 / numCols, rectTopSeq.height * 0.7);
        }
    }
}

void Sequencer::setupSequencesFromPanel()
{
    ofRectangle panelRect = panel->getRectangle();
    rectangle.set(panelRect.x + panelRect.width + sequencerPanelMargin, panelRect.y, sequencerWidth, panelRect.height);
    
    for (auto groupPair : sequencePairs)
    {
        for (auto elementPair : groupPair->getElementPairs())
        {
//            cout << "========== " << endl;
//            cout << "seq " << groupPair->getParent()->getName() << " -> " << elementPair->getElement()->getName() << endl;
            
            if (groupPair->getParent()->getCollapsed() ||
                !groupPair->getParent()->getActive() ||
                !elementPair->getElement()->getActive() ||
                !elementPair->getElement()->getParent()->getActive())
            {
                elementPair->getSequence()->setRectangle(0, 0, 0, 0);
//                cout << " -- disappear"<<endl;
            }
            else
            {
                ofRectangle elementRectangle = elementPair->getElement()->getRectangle();
                elementPair->getSequence()->setRectangle(rectangle.x, elementRectangle.y, sequencerWidth, elementRectangle.height);
//                cout << " -- Set seq rect " << elementPair->getElement()->getName() <<"  : " << ofToString(elementPair->getSequence()->getRectangle()) << endl;
            }
        }
    }
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
    for (auto p : sequencePairs) {
        p->setNumberColumns(numCols);
    }
    setupGuiPositions();
}

void Sequencer::randomizeSequencer()
{
    for (auto p : sequencePairs) {
        p->randomizeSequence(0.5, 1.0);
    }
}

void Sequencer::setAllSequencersActive(bool allSequencesActive)
{
    this->allSequencesActive = allSequencesActive;
    for (auto p : sequencePairs)
    {
        for (auto &e : p->getElementPairs())
        {
            if (e->getElement()->getActive()) {
                e->getSequence()->setActive(allSequencesActive);
            }
        }
    }
}

void Sequencer::selectColumn(int column)
{
    setActive(false);
    currentColumn = column;
    cursor = currentColumn;
    
    for (auto p : sequencePairs)
    {
        p->setCursor(cursor);
        for (auto &e : p->getElementPairs())
        {
            if (e->getElement()->getActive()) {
                e->lerpWidgetToSequencer(lerpNumFrames);
            }
        }
    }
}

void Sequencer::setColumnToCurrentValues(int column)
{
    for (auto p : sequencePairs)
    {
        for (auto &e : p->getElementPairs())
        {
            if (e->getElement()->getActive()) {
                e->setSequenceFromElement(column);
            }
        }
    }
}

void Sequencer::next()
{
    bpmTime = ofGetElapsedTimeMillis();
    currentColumn = fmodf(currentColumn + 1.0f, (float) numCols);
    cursor = currentColumn;
    
    for (auto p : sequencePairs)
    {
        p->setCursor(cursor);
        if (!smooth) {
            p->updateWidgetFromSequencer();
        }
    }
}

bool Sequencer::mouseMoved(int mouseX, int mouseY)
{
    GuiWidgetBase::mouseMoved(mouseX, mouseY);
    for (auto s : sequencePairs)
    {
        for (auto e : s->getElementPairs()) {
            if (e->getSequence()->mouseMoved(mouseX, mouseY)) return true;
        }
    }
    
    rectToggleAllSeqMouseOver = bToggleAllSequencers.inside(mouseX, mouseY);
    rectSelectColumnMouseOver = -1;
    for (int i = 0; i < bColumnSelectors.size(); i++)
    {
        if (bColumnSelectors[i].inside(mouseX, mouseY))
        {
            rectSelectColumnMouseOver = i;
            return true;
        }
    }
    return false;
}

bool Sequencer::mousePressed(int mouseX, int mouseY)
{
    if (GuiWidgetBase::mousePressed(mouseX, mouseY)) return true;
    if (mouseOver)
    {
        for (auto s : sequencePairs)
        {
            for (auto e : s->getElementPairs()) {
                if (e->getSequence()->mousePressed(mouseX, mouseY)) return true;
            }
        }
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
    GuiWidgetBase::mouseDragged(mouseX, mouseY);
    if (mouseDragging)
    {
        for (auto s : sequencePairs)
        {
            for (auto e : s->getElementPairs()) {
                if (e->getSequence()->mouseDragged(mouseX, mouseY)) return true;
            }
        }
    }
    return false;
}

bool Sequencer::mouseReleased(int mouseX, int mouseY)
{
    if (GuiWidgetBase::mouseReleased(mouseX, mouseY)) return true;
    for (auto s : sequencePairs)
    {
        for (auto e : s->getElementPairs()) {
            if (e->getSequence()->mouseReleased(mouseX, mouseY)) return true;
        }
    }
    return false;
}

bool Sequencer::keyPressed(int key)
{
    if (GuiWidgetBase::keyPressed(key)) return true;
    if (mouseOver)
    {
        for (auto s : sequencePairs)
        {
            for (auto e : s->getElementPairs()) {
                if (e->getSequence()->keyPressed(key)) return true;
            }
        }
        
        if (key == OF_KEY_LEFT)
        {
            selectColumn((currentColumn + numCols - 1) % numCols);
            return true;
        }
        else if (key == OF_KEY_RIGHT)
        {
            selectColumn((currentColumn + 1) % numCols);
            return true;
        }
        else if (key == 48)
        {
            selectColumn(0);
            return true;
        }
        else if (key >= 49 && key <= 57)
        {
            selectColumn(key-49);
            return true;
        }
        else if (key == OF_KEY_RETURN && rectSelectColumnMouseOver != -1)
        {
            setColumnToCurrentValues(rectSelectColumnMouseOver);
            return true;
        }
    }
    return false;
}

void Sequencer::update()
{
    GuiWidgetBase::update();
    
    if (active && smooth)
    {
        cursor = currentColumn + (ofGetElapsedTimeMillis() - bpmTime) / bpmInterval;
        for (auto s : sequencePairs)
        {
            s->setCursor(cursor);
            s->updateWidgetFromSequencer();
        }
    }
}

void Sequencer::draw()
{
    GuiWidgetBase::draw();
    
    for (auto s : sequencePairs)
    {
        for (auto e : s->getElementPairs()) {
            e->getSequence()->draw();
        }
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
        
    mChoose->draw();
}

void Sequencer::saveSequencerSet(string sequencerName)
{
    if (sequencerName == "")
    {
        sequencerName = ofSystemTextBoxDialog("New sequencer set name:");
        if (sequencerName == "")   return;
    }

    SequencerSavedSet newSet(sequencerName, smooth, bpm, lerpNumFrames, numCols);
    for (auto s : sequencePairs)
    {
        for (auto &e : s->getElementPairs())
        {
            string sequenceName = s->getParent()->getName() + "/" + e->getElement()->getName();
            newSet.addSequence(sequenceName, e->getSequence()->getAllValues(), e->getSequence()->getActive());
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
    
    for (auto s : sequencePairs)
    {
        for (auto &e : s->getElementPairs())
        {
            string sequenceName = s->getParent()->getName() + "/" + e->getElement()->getName();
            if (sequencerSets[sequencerName].sequences.count(sequenceName) != 0)
            {
                e->getSequence()->setFromValues(sequencerSets[sequencerName].sequences[sequenceName]);
                e->getSequence()->setActive(sequencerSets[sequencerName].sequencesActive[sequenceName]);
            }
        }
    }
}

void Sequencer::eventBpm(GuiElementEventArgs &s)
{
    setBpm(bpm);
}

void Sequencer::eventNumColumns(GuiElementEventArgs &s)
{
    setNumberColumns(numCols);
}

void Sequencer::eventActive(GuiElementEventArgs &b)
{
    setActive(b.value);
}

void Sequencer::eventWidgetCollapsed(string &s)
{
    setupGuiPositions();
}

void Sequencer::eventRandomizeSequencer(GuiElementEventArgs &b)
{
    randomizeSequencer();
}

void Sequencer::eventSelectColumn(GuiElementEventArgs & b)
{
    selectColumn(ofToFloat(b.name));
}

void Sequencer::eventBeat()
{
    next();
}

void Sequencer::eventSequencerMenuSelection(GuiElementEventArgs & evt)
{
    if (evt.name == "save new") {
        saveSequencerSet();
    }
    else {
        loadSequencerSet(evt.name);
    }
}




//////////////////
/////
// TODO: fix this

void Sequencer::eventElementDeleted(GuiElement * &elementToDelete)
{
    vector<SequenceGroupPair*>::iterator itg = sequencePairs.begin();
    while (itg != sequencePairs.end())
    {
        vector<SequenceElementPair*>::iterator ite = (*itg)->getElementPairs().begin();
        while (ite != (*itg)->getElementPairs().end())
        {
            if ((*ite)->getElement() == elementToDelete)
            {
                delete *ite;
                (*itg)->getElementPairs().erase(ite);
            }
            else {
                ++ite;
            }
        }
        ++itg;
    }
    setupGuiPositions();
}
