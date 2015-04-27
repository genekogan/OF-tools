#include "Sequencer.h"


Sequencer::SequenceElementPair::SequenceElementPair(GuiElement *element, int numCols)
{
    this->element = element;
    sequence = new Sequence(element->getAddress(), numCols);
    sequence->setAutoUpdate(false);
    sequence->setAutoDraw(false);
    sequence->setDiscrete(element->isDiscrete());
    ofAddListener(sequence->keyboardEvent, this, &SequenceElementPair::eventSetSequenceFromKey);
}

void Sequencer::SequenceElementPair::eventSetSequenceFromKey(Sequence::SequenceKeyboardEventArgs & evt)
{
    element->setSequenceFromExplicitValue(*sequence, evt.column, evt.value);
}

Sequencer::ElementSequenceGroup::ElementSequenceGroup(GuiElement *element, int numCols)
{
    this->element = element;
    if (element->isMultiElement())
    {
        for (auto e : ((GuiMultiElement *) element)->getElements()) {
            pairs.push_back(new SequenceElementPair(e, numCols));
        }
        if (isSingleChoiceMenu())
        {
            for (auto p : pairs) {
                ofAddListener(p->sequence->sequenceEvent, this, &ElementSequenceGroup::menuEvent);
            }
        }
    }
    else {
        pairs.push_back(new SequenceElementPair(element, numCols));
    }
}

bool Sequencer::ElementSequenceGroup::isSingleChoiceMenu()
{
    return (element->isMultiElement() &&
        ((GuiMultiElement *) element)->isMenu() &&
        !((GuiMenu *) element)->getMultipleChoice());
}

void Sequencer::ElementSequenceGroup::randomizeSequencer()
{
    if (isSingleChoiceMenu())
    {
        int numToggles = pairs.size();
        if (numToggles == 0) return;
        int numCols = pairs[0]->sequence->getSize();
        for (int col = 0; col < numCols; col++)
        {
            int idxRandomCell = ofRandom(numToggles);
            for (int t = 0; t < numToggles; t++) {
                pairs[t]->sequence->setValueAtCell(col, t == idxRandomCell, false);
            }
        }
    }
    else
    {
        for (auto p : pairs) {
            p->sequence->randomize(0.5, 1.0);
        }
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

void Sequencer::SequencerSavedSet::addSequence(string sequenceGroupName, string sequenceName, vector<float> sequence, bool active)
{
    if (sequenceGroups.count(sequenceGroupName) == 0) {
        sequenceGroups[sequenceGroupName] = SequencerSavedSetGroup();
    }
    sequenceGroups[sequenceGroupName].addSequence(sequenceName, sequence, active);
}

void Sequencer::SequencerSavedSet::SequencerSavedSetGroup::addSequence(string sequenceName, vector<float> sequence, bool active)
{
    sequences[sequenceName] = sequence;
    sequencesActive[sequenceName] = active;
}

Sequencer::Sequencer(string name, GuiWidget * panel, int numCols) : GuiElement(name)
{
    this->panel = panel;
    this->numCols = numCols;
    this->bpm = GUI_DEFAULT_SEQUENCER_BPM;
    this->sequencerPanelMargin = GUI_DEFAULT_SEQUENCER_PANEL_MARGIN;
    this->sequencerWidth = GUI_DEFAULT_SEQUENCER_WIDTH;
    this->sequencerButtonHeight = GUI_DEFAULT_ELEMENT_HEIGHT;
    this->marginInner = GUI_DEFAULT_MARGIN_Y;
    this->lerpNumFrames = 1;
    
    ofAddListener(panel->newElementEvent, this, &Sequencer::addElement);
    ofAddListener(panel->removeElementEvent, this, &Sequencer::removeElement);

    setupSequencer();
    setActive(false);
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
    ofRemoveListener(((GuiSlider<int> *) buttons[3])->sliderEvent, this, &Sequencer::eventBpm);
    ofRemoveListener(((GuiSlider<int> *) buttons[4])->sliderEvent, this, &Sequencer::eventNumColumns);
    ofRemoveListener(panel->newElementEvent, this, &Sequencer::addElement);
    ofRemoveListener(panel->removeElementEvent, this, &Sequencer::removeElement);

    for (auto b : buttons) {
        delete b;
    }
    buttons.clear();

    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    while (it != groups.end())
    {
        delete it->second;
        ++it;
    }
    groups.clear();
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

void Sequencer::setLerpNumFrames(int lerpNumFrames)
{
    this->lerpNumFrames = lerpNumFrames;
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
    setupGuiPositions();
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
        string sequenceGroupName = it->second->element->getAddress();
        
        for (auto &p : it->second->pairs)
        {
            string sequenceName = p->element->getAddress();
            newSet.addSequence(sequenceGroupName, sequenceName, p->sequence->getAllValues(), p->sequence->getActive());
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
        string sequenceGroupName = it->second->element->getAddress();
        for (auto &p : it->second->pairs)
        {
            string sequenceName = p->element->getAddress();
            if (sequencerSets[sequencerName].sequenceGroups.count(sequenceGroupName) != 0 &&
                sequencerSets[sequencerName].sequenceGroups[sequenceGroupName].sequences.count(sequenceName) != 0)
            {
                p->sequence->setFromValues(sequencerSets[sequencerName].sequenceGroups[sequenceGroupName].sequences[sequenceName]);
                p->sequence->setActive(sequencerSets[sequencerName].sequenceGroups[sequenceGroupName].sequencesActive[sequenceName]);
            }
        }
    }
}

void Sequencer::setupSequencer()
{
    mChoose = new GuiMenu("choose", this, &Sequencer::eventSequencerMenuSelection, false, true);
    mChoose->setCollapsible(true);
    mChoose->setCollapsed(true);
    mChoose->setAutoUpdate(false);
    mChoose->setAutoDraw(false);
    mChoose->addToggle("save new");
 
    buttons.push_back(new GuiToggle("on", &active, this, &Sequencer::eventActive));
    buttons.push_back(new GuiToggle("~", &smooth));
    buttons.push_back(new GuiButton("?", new bool(false), this, &Sequencer::eventRandomizeSequencer));
    buttons.push_back(new GuiSlider<int>("bpm", &bpm, 1, 360));
    buttons.push_back(new GuiSlider<int>("cols", &numCols, 1, 64));
    buttons.push_back(new GuiSlider<int>("lerp", &lerpNumFrames, 1, 120));
    
    for (auto b : buttons)
    {
        b->setParent(this);
        b->setAutoUpdate(false);
        b->setAutoDraw(false);
    }
    buttons.push_back(mChoose);
    
    ofAddListener(clock.beatEvent, this, &Sequencer::eventBeat);
    ofAddListener(((GuiSlider<int> *) buttons[3])->sliderEvent, this, &Sequencer::eventBpm);
    ofAddListener(((GuiSlider<int> *) buttons[4])->sliderEvent, this, &Sequencer::eventNumColumns);

    for (auto element : panel->getElementGroups()) {
        addElement(element);
    }
}

void Sequencer::addElement(GuiElement* & newElement)
{
    if (groups.count(newElement) == 0)
    {
        groups[newElement] = new ElementSequenceGroup(newElement, numCols);
        setupGuiPositions();
    }
}

void Sequencer::removeElement(GuiElement* & element)
{
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    while (it != groups.end())
    {
        if (it->first == element ||
            (it->second->element->getHasParent() && (it->second->element->getParent() == element)))
        {
            delete it->second;
            groups.erase(it++);
            setupGuiPositions();
        }
        else
        {
            vector<SequenceElementPair*>::iterator itp = it->second->pairs.begin();
            while (itp != it->second->pairs.end())
            {
                if ((*itp)->element->getHasParent() && ((*itp)->element->getParent() == element))
                {
                    delete *itp;
                    it->second->pairs.erase(itp++);
                    setupGuiPositions();
                }
                else {
                    ++itp;
                }
            }
            ++it;
        }
    }
}

void Sequencer::setupGuiPositions()
{
    ofRectangle panelRect = panel->getRectangle();
    rectangle.set(panelRect.x + panelRect.getWidth() + sequencerPanelMargin, panelRect.y, sequencerWidth, panelRect.getHeight());
    
    buttons[6]->setPosition(rectangle.x +   2, rectangle.y + marginInner);
    buttons[0]->setPosition(rectangle.x + 107, rectangle.y + marginInner);
    buttons[1]->setPosition(rectangle.x + 136, rectangle.y + marginInner);
    buttons[3]->setPosition(rectangle.x + 165, rectangle.y + marginInner);
    buttons[5]->setPosition(rectangle.x + 226, rectangle.y + marginInner);
    buttons[4]->setPosition(rectangle.x + 287, rectangle.y + marginInner);
    buttons[2]->setPosition(rectangle.x + 348, rectangle.y + marginInner);
    buttons[6]->setSize(100, sequencerButtonHeight);
    buttons[0]->setSize( 24, sequencerButtonHeight);
    buttons[1]->setSize( 24, sequencerButtonHeight);
    buttons[3]->setSize( 56, sequencerButtonHeight);
    buttons[5]->setSize( 56, sequencerButtonHeight);
    buttons[4]->setSize( 56, sequencerButtonHeight);
    buttons[2]->setSize( 14, sequencerButtonHeight);

    if (groups.size() > 0)
    {
        ofRectangle sequenceRect = ofRectangle(rectangle.x + GUI_DEFAULT_ELEMENT_HEIGHT, 0, sequencerWidth - GUI_DEFAULT_ELEMENT_HEIGHT, GUI_DEFAULT_ELEMENT_HEIGHT);
        int columnRowY = rectangle.y + 8 + sequencerButtonHeight + 2 * marginInner;
        rectSelectColumnMouseOver = -1;
        rectToggleAllSeqMouseOver = false;
        bToggleAllSequencers.set(rectangle.x, columnRowY, sequenceRect.height, sequenceRect.height);
        bColumnSelectors.resize(numCols);
        for (int i = 0; i < numCols; i++)
        {
            float x = ofMap(i + 0.15, 0, numCols, sequenceRect.x, sequenceRect.x + sequenceRect.width);
            bColumnSelectors[i].set(x, columnRowY, sequencerWidth * 0.7 / numCols, sequenceRect.height * 0.7);
        }
    }

    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            if (p->element->getCollapsed())
            {
                p->sequence->setRectangle(0, 0, 0, 0);
            }
            else {
                ofRectangle e = p->element->getRectangle();
                p->sequence->setPosition(rectangle.x, e.y);
                p->sequence->setSize(sequencerWidth, e.height);
            }
        }
    }
}

void Sequencer::setAllSequencersActive(bool allSequencesActive)
{
    this->allSequencesActive = allSequencesActive;    
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
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        it->second->randomizeSequencer();
    }
}

void Sequencer::ElementSequenceGroup::menuEvent(GuiSequenceEventArgs & evt)
{
    if (evt.value > 0.5)
    {
        for (auto p : pairs) {
            p->sequence->setValueAtCell(evt.cell, p->sequence == evt.sequence, false);
        }
    }
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
            if (p->element->getActive() && p->sequence->getActive()) {
                p->element->lerpTo(p->sequence->getValueAtCurrentCursor(), lerpNumFrames);
            }
        }
    }
}

void Sequencer::setColumnToCurrentValues(int column)
{
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs) {
            p->element->setSequenceFromValue(*p->sequence, column);
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
            if (!smooth && p->element->getActive() && p->sequence->getActive()) {
                p->element->setValueFromSequence(*p->sequence);
            }
        }
    }
}

void Sequencer::update()
{
    if (active && smooth)
    {
        cursor = currentColumn + (ofGetElapsedTimeMillis() - bpmTime) / bpmInterval;
        map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
        for (; it != groups.end(); ++it)
        {
            for (auto &p : it->second->pairs)
            {
                p->sequence->setCursor(cursor);
                if (p->element->getActive() && p->sequence->getActive()) {
                    p->element->setValueFromSequence(*p->sequence);
                }
            }
        }
    }

    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            p->sequence->update();
        }
    }
    
    for (auto b : buttons) {
        b->update();
    }
}

void Sequencer::draw()
{
    ofSetColor(colorBackground, 50);
    ofFill();
    ofRect(rectangle);
    
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            if (p->element->getActive()) {
                p->sequence->draw();
            }
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

    for (auto b : buttons) {
        if (b->getName() == "choose") {
            cout << b->getCollapsed() << " " << endl;
        }
        b->draw();
    }
    mChoose->draw();
}

bool Sequencer::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
    
    for (auto b : buttons) {
        b->mouseMoved(mouseX, mouseY);
    }
    
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            if (p->element->getActive()) {
                p->sequence->mouseMoved(mouseX, mouseY);
            }
        }
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
    for (auto b : buttons) {
        if (b->mousePressed(mouseX, mouseY)) return true;
    }
    
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            if (p->element->getActive()) {
                if (p->sequence->mousePressed(mouseX, mouseY)) return true;
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
    for (auto b : buttons) {
        if (b->mouseDragged(mouseX, mouseY)) return true;
    }
    
    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            if (p->element->getActive()) {
                if (p->sequence->mouseDragged(mouseX, mouseY)) return true;
            }
        }
    }
    return false;
}

bool Sequencer::mouseReleased(int mouseX, int mouseY)
{
    for (auto b : buttons) {
        if (b->mouseReleased(mouseX, mouseY)) return true;
    }

    map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto &p : it->second->pairs)
        {
            if (p->element->getActive()) {
                if (p->sequence->mouseReleased(mouseX, mouseY)) return true;
            }
        }
    }
    return false;
}

bool Sequencer::keyPressed(int key)
{    
    if (mouseOver)
    {
        for (auto b : buttons) {
            if (b->keyPressed(key)) return true;
        }
        
        map<GuiElement*,ElementSequenceGroup*>::iterator it = groups.begin();
        for (; it != groups.end(); ++it)
        {
            for (auto &p : it->second->pairs)
            {
                if (p->element->getActive()) {
                    if (p->sequence->keyPressed(key)) return true;
                }
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

void Sequencer::eventRandomizeSequencer(GuiButtonEventArgs &b)
{
    randomizeSequencer();
}

void Sequencer::eventSelectColumn(GuiSliderEventArgs<int> & b)
{
    selectColumn(b.value);
}

void Sequencer::eventSequencerMenuSelection(GuiMenuEventArgs & evt)
{
    evt.toggle->getName() == "save new" ? saveSequencerSet() : loadSequencerSet(evt.toggle->getName());
}

void Sequencer::eventBeat()
{
    next();
}

void Sequencer::getXml(ofXml &xml)
{
    ofXml xmlSequencer;
    xmlSequencer.addChild("Sequencer");
    xmlSequencer.setTo("Sequencer");
    
    ofXml xmlSequencerSets;
    xmlSequencerSets.addChild("SequencerSets");
    xmlSequencerSets.setTo("SequencerSets");
    
    map<string, SequencerSavedSet>::iterator it = sequencerSets.begin();
    for (; it != sequencerSets.end(); ++it)
    {
        ofXml xmlSequencerSet;
        xmlSequencerSet.addChild("SequencerSet");
        xmlSequencerSet.setTo("SequencerSet");
        
        xmlSequencerSet.addValue<string>("Name", it->second.name);
        xmlSequencerSet.addValue<int>("Bpm", it->second.bpm);
        xmlSequencerSet.addValue<bool>("Smooth", it->second.smooth);
        xmlSequencerSet.addValue<int>("Lerp", it->second.lerpNumFrames);
        xmlSequencerSet.addValue<int>("NumCols", it->second.numCols);
        
        map<string, SequencerSavedSet::SequencerSavedSetGroup>::iterator itg = it->second.sequenceGroups.begin();
        for (; itg != it->second.sequenceGroups.end(); ++itg)
        {
            ofXml xmlSequenceGroup;
            xmlSequenceGroup.addChild("SequenceGroup");
            xmlSequenceGroup.setTo("SequenceGroup");
            xmlSequenceGroup.addValue("Name", itg->first);
            
            map<string, vector<float> >::iterator its = itg->second.sequences.begin();
            for (; its != itg->second.sequences.end(); ++its)
            {
                ofXml xmlSequence;
                xmlSequence.addChild("Sequence");
                xmlSequence.setTo("Sequence");
                xmlSequence.addValue("Name", its->first);
                xmlSequence.addValue<vector<float> >("Values", its->second);
                xmlSequence.addValue<bool>("Active", itg->second.sequencesActive.count(its->first) > 0 ? itg->second.sequencesActive[its->first] : true);
                xmlSequenceGroup.addXml(xmlSequence);
            }
            
            xmlSequencerSet.addXml(xmlSequenceGroup);
        }
        xmlSequencerSets.addXml(xmlSequencerSet);
    }
    
    ofXml xmlCurrent;
    xmlCurrent.addChild("Current");
    xmlCurrent.setTo("Current");
    xmlCurrent.addValue<string>("Name", getName());
    xmlCurrent.addValue<bool>("Active", getActive());
    xmlCurrent.addValue<int>("Bpm", getBpm());
    xmlCurrent.addValue<bool>("Smooth", getSmooth());
    xmlCurrent.addValue<int>("Lerp", getLerpNumFrames());
    xmlCurrent.addValue<int>("NumCols", getNumberColumns());
    
    map<GuiElement*,ElementSequenceGroup*>::iterator itg = groups.begin();
    for (; itg != groups.end(); ++itg)
    {
        ofXml xmlSequenceGroup;
        xmlSequenceGroup.addChild("SequenceGroup");
        xmlSequenceGroup.setTo("SequenceGroup");
        xmlSequenceGroup.addValue<string>("Name", itg->second->element->getAddress());
        for (auto &p : itg->second->pairs)
        {
            ofXml xmlSequence;
            xmlSequence.addChild("Sequence");
            xmlSequence.setTo("Sequence");
            xmlSequence.addValue<string>("Name", p->element->getAddress());
            xmlSequence.addValue<vector<float> >("Values", p->sequence->getAllValues());
            xmlSequence.addValue<bool>("Active", p->sequence->getActive());
            xmlSequenceGroup.addXml(xmlSequence);
        }
        xmlCurrent.addXml(xmlSequenceGroup);
    }
    
    xmlSequencer.addXml(xmlSequencerSets);
    xmlSequencer.addXml(xmlCurrent);
    
    xml.addXml(xmlSequencer);
}

void Sequencer::setFromXml(ofXml &xml)
{
    sequencerSets.clear();
    mChoose->clearToggles();
    
    if (xml.exists("Sequencer"))
    {
        xml.setTo("Sequencer");
        if (xml.exists("SequencerSets"))
        {
            xml.setTo("SequencerSets");
            if (xml.exists("SequencerSet[0]"))
            {
                xml.setTo("SequencerSet[0]");
                do {
                    string sequencerName = xml.getValue<string>("Name");
                    SequencerSavedSet newSet(sequencerName,
                                             xml.getValue<bool>("Smooth"),
                                             xml.getValue<int>("Bpm"),
                                             xml.getValue<int>("Lerp"),
                                             xml.getValue<int>("NumCols"));
                    
                    if (xml.exists("SequenceGroup[0]"))
                    {
                        xml.setTo("SequenceGroup[0]");
                        do {
                            string sequenceGroupName = xml.getValue<string>("Name");
                            if (xml.exists("Sequence[0]"))
                            {
                                xml.setTo("Sequence[0]");
                                do {
                                    string sequenceName = xml.getValue<string>("Name");
                                    bool sequenceActive = xml.getValue<bool>("Active");
                                    string valueStringXml = xml.getValue<string>("Values");
                                    vector<string> valueString = ofSplitString(valueStringXml.substr(1, valueStringXml.size()-2), ",");
                                    vector<float> sequenceValues;
                                    for (auto vs : valueString) sequenceValues.push_back(ofToFloat(vs));
                                    newSet.addSequence(sequenceGroupName, sequenceName, sequenceValues, sequenceActive);
                                }
                                while(xml.setToSibling());
                                xml.setToParent();
                            }
                        }
                        while(xml.setToSibling());
                        xml.setToParent();
                    }
                    sequencerSets[sequencerName] = newSet;
                    mChoose->addToggle(sequencerName);
                }
                while(xml.setToSibling());
                xml.setToParent();
            }
            xml.setToParent();
        }
        
        if (xml.exists("Current"))
        {
            xml.setTo("Current");
            
            setName(xml.getValue<string>("Name"));
            setActive(xml.getValue<bool>("Active"));
            setBpm(xml.getValue<int>("Bpm"));
            setSmooth(xml.getValue<bool>("Smooth"));
            setNumberColumns(xml.getValue<int>("NumCols"));
            lerpNumFrames = xml.getValue<int>("Lerp");
            
            if (xml.exists("SequenceGroup[0]"))
            {
                xml.setTo("SequenceGroup[0]");
                do {
                    string sequenceGroupName = xml.getValue<string>("Name");
                    if (xml.exists("Sequence[0]"))
                    {
                        xml.setTo("Sequence[0]");
                        do {
                            string sequenceName = xml.getValue<string>("Name");
                            bool sequenceActive = xml.getValue<bool>("Active");
                            
                            string valueStringXml = xml.getValue<string>("Values");
                            vector<string> valueString = ofSplitString(valueStringXml.substr(1, valueStringXml.size()-2), ",");
                            vector<float> sequenceValues;
                            for (auto vs : valueString) sequenceValues.push_back(ofToFloat(vs));
                            
                            map<GuiElement*,ElementSequenceGroup*>::iterator itg = groups.begin();
                            for (; itg != groups.end(); ++itg)
                            {
                                string currentSequenceGroupName = itg->second->element->getAddress();
                                for (auto &p : itg->second->pairs)
                                {
                                    string currentSequenceName = p->element->getAddress();
                                    if (currentSequenceGroupName == sequenceGroupName && currentSequenceName == sequenceName)
                                    {
                                        p->sequence->setFromValues(sequenceValues);
                                        p->sequence->setActive(sequenceActive);
                                    }
                                }
                            }
                        }
                        while(xml.setToSibling());
                        xml.setToParent();
                    }
                }
                while(xml.setToSibling());
                xml.setToParent();
            }
            xml.setToParent();
        }
        xml.setToParent();
    }
}
