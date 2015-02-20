#include "Sequencer.h"


Sequencer::SequenceElementPair::SequenceElementPair(GuiElement *element, int numColumns)
{
    this->element = element;
    sequence = new Sequence(element->getName(), numColumns);
    sequence->setAutoUpdate(false);
    sequence->setAutoDraw(false);
    sequence->setDiscrete(element->isDiscrete());
    linkedToElement = true;
}

Sequencer::SequenceElementPair::SequenceElementPair(int numColumns)
{
    sequence = new Sequence("", numColumns);
    sequence->setAutoUpdate(false);
    sequence->setAutoDraw(false);
    linkedToElement = false;
}

void Sequencer::SequenceElementPair::updateElementFromSequencer()
{
    if (linkedToElement && sequence->getActive()) {
        element->setValueFromSequence(*sequence);
    }
}

void Sequencer::SequenceElementPair::lerpWidgetToSequencer(int lerpNumFrames)
{
    if (linkedToElement && sequence->getActive()) {
        element->lerpTo(sequence->getValueAtCurrentCursor(), lerpNumFrames);
    }
}

Sequencer::SequenceElementPair::~SequenceElementPair()
{
    delete sequence;
}

Sequencer::SequenceWidgetPair::SequenceWidgetPair(GuiWidget *widget, int numColumns)
{
    this->widget = widget;
    for (auto element : widget->getElements())
    {
        SequenceElementPair *elementPair = new SequenceElementPair(element, numColumns);
        elementPairs.push_back(elementPair);
    }
    
    if (widget->isMenu())
    {
        menu = ((GuiMenu *) widget);
        for (auto e : elementPairs) {
            ofAddListener(e->sequence->elementEvent, this, &SequenceWidgetPair::menuSequenceChanged);
        }
    }
}

Sequencer::SequenceWidgetPair::SequenceWidgetPair(int numColumns)
{
    SequenceElementPair *elementPair = new SequenceElementPair(numColumns);
    elementPairs.push_back(elementPair);
}

void Sequencer::SequenceWidgetPair::setNumberColumns(int numColumns)
{
    for (auto e : elementPairs) {
        e->sequence->setSize(numColumns);
    }
}

void Sequencer::SequenceWidgetPair::setCursor(float cursor)
{
    for (auto e : elementPairs) {
        e->sequence->setCursor(cursor);
    }
}

void Sequencer::SequenceWidgetPair::randomizeSequence(float density, float range)
{
    for (auto e : elementPairs) {
        e->sequence->randomize(density, range);
    }
}

void Sequencer::SequenceWidgetPair::updateWidgetFromSequencer()
{
    if (widget->isMenu())
    {
        if (menu->getMultipleChoice())
        {
            for (auto e : elementPairs)
            {
                if (e->sequence->getActive()) {
                    menu->setToggle(e->sequence->getName(), e->sequence->getValueAtCurrentIndex() == 1.0);
                }
            }
        }
        else
        {
            for (auto e : elementPairs)
            {
                if (e->sequence->getValueAtCurrentIndex() == 1.0 && e->sequence->getActive()) {
                    menu->setToggle(e->sequence->getName(), true);
                }
            }
        }
    }
    else
    {
        for (auto e : elementPairs) {
            e->updateElementFromSequencer();
        }
    }
}

void Sequencer::SequenceWidgetPair::lerpWidgetToSequencer(int lerpNumFrames)
{
    for (auto e : elementPairs) {
        e->lerpWidgetToSequencer(lerpNumFrames);
    }
}

void Sequencer::SequenceWidgetPair::update()
{
    for (auto e : elementPairs) {
        e->sequence->update();
    }
}

void Sequencer::SequenceWidgetPair::draw()
{
    for (auto e : elementPairs) {
        e->sequence->draw();
    }
}

void Sequencer::SequenceWidgetPair::mouseMoved(int mouseX, int mouseY)
{
    for (auto e : elementPairs) {
        e->sequence->mouseMoved(mouseX, mouseY);
    }
}

void Sequencer::SequenceWidgetPair::mousePressed(int mouseX, int mouseY)
{
    for (auto e : elementPairs) {
        e->sequence->mousePressed(mouseX, mouseY);
    }
    
}
void Sequencer::SequenceWidgetPair::mouseReleased(int mouseX, int mouseY)
{
    for (auto e : elementPairs) {
        e->sequence->mouseReleased(mouseX, mouseY);
    }
    
}
void Sequencer::SequenceWidgetPair::mouseDragged(int mouseX, int mouseY)
{
    for (auto e : elementPairs) {
        e->sequence->mouseDragged(mouseX, mouseY);
    }
}

void Sequencer::SequenceWidgetPair::menuSequenceChanged(GuiElementEventArgs & b)
{
    for (auto e : elementPairs)
    {
        if (e->sequence->getName() != b.name &&
            b.value == 1.0 &&
            menu->getMultipleChoice() == false) {
            e->sequence->setValueAtCell(b.cell, 0.0);
        }
    }
}

Sequencer::SequenceWidgetPair::~SequenceWidgetPair()
{
    for (auto e : elementPairs)
    {
        ofRemoveListener(e->sequence->elementEvent, this, &SequenceWidgetPair::menuSequenceChanged);
        delete e;
    }
    elementPairs.clear();
}

Sequencer::Sequencer(string name, GuiPanel * panel, int numColumns) : GuiWidget(name)
{
    this->panel = panel;
    this->numColumns = numColumns;
    vector<GuiWidget *> widgets = panel->getWidgets();
    for (auto widget : widgets)
    {
        SequenceWidgetPair *widgetPair = new SequenceWidgetPair(widget, numColumns);
        widgetPairs.push_back(widgetPair);
        ofAddListener(widget->widgetChanged, this, &Sequencer::eventWidgetCollapsed);
    }
    linkedToPanel = true;
    setupSequencer();
}

void Sequencer::setupGuiComponents()
{
    if (linkedToPanel) {
        setupGuiSequencesFromPanel();
    }
    else {
        //setupGuiSequencesManually();
    }
    
    setupGuiControls();
}

void Sequencer::setupGuiSequencesFromPanel()
{
    vector<GuiWidget *> widgets = panel->getWidgets();
    if (widgets.size() == 0)    return;
    ofRectangle widgetTop = widgets[0]->getRectangle();
    ofRectangle widgetBottom = widgets[widgets.size()-1]->getRectangle();
    
    sequenceRectangle.set(widgetTop.x + widgetTop.width + style.marginInner + seqPanelMarginLeft, widgetTop.y, sequencerWidth, widgetBottom.y + widgetBottom.height - widgetTop.y);
    controlRectangle.set(widgetTop.x + widgetTop.width + style.marginInner + seqPanelMarginLeft, widgetTop.y - seqHeaderHeight, sequenceRectangle.width, seqHeaderHeight);
    rectangle.set(controlRectangle.x, controlRectangle.y, controlRectangle.width, controlRectangle.height + sequenceRectangle.height);
    
    for (auto w : widgetPairs)
    {
        if (w->getWidget()->getCollapsed())
        {
            for (auto e : w->elementPairs) {
                e->sequence->setRectangle(0, 0, 0, 0);
            }
        }
        else
        {
            for (auto e : w->elementPairs)
            {
                ofRectangle elementRectangle = e->element->getRectangle();
                e->sequence->setRectangle(sequenceRectangle.x, elementRectangle.y, sequenceRectangle.width, elementRectangle.height);
            }
        }
    }
}

void Sequencer::setupSequencer()
{
    isList = false;
    collapsed = false;
    smooth = false;
    allSequencesActive = true;
    bpm = 120;
    lerpNumFrames = 1;
    randomDensity = 0.5;
    randomRange = 1.0;
    
    vector<string> choices;
    choices.push_back("seq 1");
    choices.push_back("seq 2");
    mChooseSeq = new GuiMenu("loasadd", choices, false, true);
    mChooseSeq->setCollapsed(true);
    mChooseSeq->getStyle().elementWidth = 100;
    mChooseSeq->setPosition(controlRectangle.x + 4, controlRectangle.y + style.marginInner - 2);
    
    tActive = new GuiToggle("active", &active, this, &Sequencer::eventActive);
    tSmooth = new GuiToggle("smooth", &smooth);
    bRandomize = new GuiButton("?", this, &Sequencer::eventRandomizeSequencer);
    sBpm = new GuiSlider<int>("bpm", &bpm, 1, 360);
    sNumColumns = new GuiSlider<int>("cols", &numColumns, 1, 64);
    sDensity = new GuiSlider<float>("density", &randomDensity, 0.0f, 1.0f);
    sRange = new GuiSlider<float>("range", &randomRange, 0.0f, 1.0f);
    sLerpFrames = new GuiSlider<int>("lerp", &lerpNumFrames, 1, 120);
    
    elements.clear();
    elements.push_back(tActive);
    elements.push_back(tSmooth);
    elements.push_back(sBpm);
    elements.push_back(sNumColumns);
    elements.push_back(sLerpFrames);
    elements.push_back(sDensity);
    elements.push_back(sRange);
    elements.push_back(bRandomize);
    elements.push_back(mChooseSeq);
    for (auto e : elements)
    {
        e->setAutoUpdate(false);
        e->setAutoDraw(false);
    }

    ofAddListener(clock.beatEvent, this, &Sequencer::eventBeat);
    ofAddListener(sBpm->elementEvent, this, &Sequencer::eventBpm);
    ofAddListener(sNumColumns->elementEvent, this, &Sequencer::eventNumColumns);
    
    setBpm(bpm);
    GuiElement::setAutoUpdate(true);
    GuiElement::setAutoDraw(true);
    setupGuiComponents();
}

void Sequencer::setupGuiControls()
{
    mChooseSeq->setPosition(controlRectangle.x + 4, controlRectangle.y + style.marginInner - 2);
    tActive->setRectangle(controlRectangle.x + 4, controlRectangle.y + style.marginInner + 20, 56, 14);

    sBpm->setRectangle(controlRectangle.x + 120, controlRectangle.y + style.marginInner, 100, 14);
    sLerpFrames->setRectangle(controlRectangle.x + 120, controlRectangle.y + style.marginInner + 20, 100, 14);

    sNumColumns->setRectangle(controlRectangle.x + 225, controlRectangle.y + style.marginInner, 80, 14);
    tSmooth->setRectangle(controlRectangle.x + 225, controlRectangle.y + style.marginInner + 20, 80, 14);

    sDensity->setRectangle(controlRectangle.x + 310, controlRectangle.y + style.marginInner, 100, 14);
    sRange->setRectangle(controlRectangle.x + 310, controlRectangle.y + style.marginInner + 20, 82, 14);
    bRandomize->setRectangle(controlRectangle.x + 396, controlRectangle.y + style.marginInner + 20, 14, 14);
    
    rectSelectColumnMouseOver = -1;
    rectToggleAllSeqMouseOver = false;
    rectTopSeq = widgetPairs[0]->elementPairs[0]->sequence->getSequenceRectangle();
    rectToggleAllSeq.set(sequenceRectangle.x + 1, rectTopSeq.y - rectTopSeq.height, rectTopSeq.height * 0.7, rectTopSeq.height * 0.7);
    rectSelectColumn.resize(numColumns);
    for (int i = 0; i < rectSelectColumn.size(); i++)
    {
        float x = ofMap(i + 0.1, 0, numColumns, rectTopSeq.x, rectTopSeq.x + rectTopSeq.width);
        rectSelectColumn[i].set(x, rectTopSeq.y - rectTopSeq.height, 0.7 * rectTopSeq.width / numColumns, rectTopSeq.height * 0.7);
    }
}

void Sequencer::update()
{
    GuiWidget::update();
    if (active && smooth)
    {
        cursor = currentColumn + (ofGetElapsedTimeMillis() - bpmTime) / bpmInterval;
        for (auto w : widgetPairs)
        {
            w->setCursor(cursor);
            w->updateWidgetFromSequencer();
        }
    }
    for (auto w : widgetPairs) {
        w->update();
    }
}

void Sequencer::draw()
{
    GuiWidget::draw();
    for (auto w : widgetPairs) {
        w->draw();
    }
    drawControls();
}

void Sequencer::drawControls()
{
    for (int i=0; i<rectSelectColumn.size(); i++)
    {
        ofSetColor(GUI_DEFAULT_SEQUENCER_COLOR_ACTIVE, ofClamp(ofMap(abs(cursor - i), 0, 1, 255, 0), 0, 255));
        ofFill();
        ofRect(rectSelectColumn[i]);
        rectSelectColumnMouseOver == i ? ofSetColor(style.colorActive) : ofSetColor(GUI_DEFAULT_SEQUENCER_COLOR_ACTIVE);
        ofNoFill();
        ofRect(rectSelectColumn[i]);
    }
    rectToggleAllSeqMouseOver ? ofFill() : ofNoFill();
    allSequencesActive ? ofSetColor(GUI_DEFAULT_SEQUENCER_COLOR_ACTIVE) : ofSetColor(GUI_DEFAULT_SEQUENCER_COLOR_INACTIVE);
    ofCircle(rectToggleAllSeq.x + rectToggleAllSeq.width / 2, rectToggleAllSeq.y + rectToggleAllSeq.width / 2, rectToggleAllSeq.width / 2);
}

void Sequencer::mouseMoved(int mouseX, int mouseY)
{
    GuiWidget::mouseMoved(mouseX, mouseY);
    for (auto w : widgetPairs) {
        w->mouseMoved(mouseX, mouseY);
    }
    
    rectToggleAllSeqMouseOver = rectToggleAllSeq.inside(mouseX, mouseY);
    rectSelectColumnMouseOver = -1;
    for (int i = 0; i < rectSelectColumn.size(); i++)
    {
        if (rectSelectColumn[i].inside(mouseX, mouseY))
        {
            rectSelectColumnMouseOver = i;
            break;
        }
    }
}

void Sequencer::mousePressed(int mouseX, int mouseY)
{
    GuiWidget::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        for (auto w : widgetPairs) {
            w->mousePressed(mouseX, mouseY);
        }
    }
    if (rectToggleAllSeqMouseOver) {
        toggleSequencersActive();
    }
    else if (rectSelectColumnMouseOver != -1) {
        selectColumn(rectSelectColumnMouseOver);
    }
}

void Sequencer::mouseReleased(int mouseX, int mouseY)
{
    GuiWidget::mouseReleased(mouseX, mouseY);
    for (auto w : widgetPairs) {
        w->mouseReleased(mouseX, mouseY);
    }
}

void Sequencer::mouseDragged(int mouseX, int mouseY)
{
    GuiWidget::mouseDragged(mouseX, mouseY);
    if (mouseDragging)
    {
        for (auto w : widgetPairs) {
            w->mouseDragged(mouseX, mouseY);
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

void Sequencer::setNumberColumns(int numColumns)
{
    this->numColumns = numColumns;
    for (auto w : widgetPairs) {
        w->setNumberColumns(numColumns);
    }
    setupGuiComponents();
}

void Sequencer::randomizeSequencer()
{
    for (auto w : widgetPairs)
    {
        if (w->getWidget()->isMenu()) {
            randomizeSequencerForMenu(w);
        }
        else {
            w->randomizeSequence(randomDensity, randomRange);
        }
    }
}

void Sequencer::randomizeSequencerForMenu(SequenceWidgetPair * w)
{
    if (((GuiMenu *) w->getWidget())->getMultipleChoice()) {
        w->randomizeSequence(randomDensity, randomRange);
    }
    else
    {
        for (int col = 0; col < numColumns; col++)
        {
            int idx = ofRandom(w->elementPairs.size());
            for (int i = 0; i < w->elementPairs.size(); i++) {
                w->elementPairs[i]->sequence->setValueAtCell(col, i == idx);
            }
        }
    }
}

void Sequencer::toggleSequencersActive()
{
    allSequencesActive = !allSequencesActive;
    for (auto w : widgetPairs) {
        for (auto e : w->elementPairs) {
            e->sequence->setActive(allSequencesActive);
        }
    }
}

void Sequencer::selectColumn(int column)
{
    setActive(false);
    currentColumn = column;
    cursor = currentColumn;
    for (auto w : widgetPairs)
    {
        w->setCursor(cursor);
        w->lerpWidgetToSequencer(lerpNumFrames);
    }
}

void Sequencer::next()
{
    bpmTime = ofGetElapsedTimeMillis();
    currentColumn = fmodf(currentColumn + 1.0f, (float) numColumns);
    cursor = currentColumn;
    for (auto w : widgetPairs)
    {
        w->setCursor(cursor);
        if (!smooth) {
            w->updateWidgetFromSequencer();
        }
    }
}

void Sequencer::eventBpm(GuiElementEventArgs &s)
{
    setBpm(bpm);
}

void Sequencer::eventNumColumns(GuiElementEventArgs &s)
{
    setNumberColumns(numColumns);
}

void Sequencer::eventActive(GuiElementEventArgs &b)
{
    setActive(b.value);
}

void Sequencer::eventWidgetCollapsed(string &s)
{
    setupGuiComponents();
}

void Sequencer::eventRandomizeSequencer(GuiElementEventArgs &b)
{
    if (b.value == 1.0) // todo: shouldn't need to check
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

Sequencer::~Sequencer()
{
    for (auto w : widgetPairs)
    {
        ofRemoveListener(w->getWidget()->widgetChanged, this, &Sequencer::eventWidgetCollapsed);
        delete w;
    }
    setActive(false);
    delete tActive;
    delete tSmooth;
    delete sBpm;
    delete sNumColumns;
    delete sLerpFrames;
    delete sDensity;
    delete sRange;
    delete bRandomize;
    delete mChooseSeq;
}