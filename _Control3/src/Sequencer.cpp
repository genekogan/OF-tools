#include "Sequencer.h"




Sequencer::Sequencer(string name, GuiWidget * panel, int numCols) : GuiElement(name)
{
    this->panel = panel;
    this->numCols = numCols;
    
    sequencerPanelMargin = GUI_DEFAULT_SEQUENCER_PANEL_MARGIN;
    sequencerWidth = GUI_DEFAULT_SEQUENCER_WIDTH;
    lerpNumFrames = 1;
    
    //ofAddListener(panel->elementDeletedEvent, this, &Sequencer::eventElementDeleted);
    //getElementGroupsFromWidget(panel);
    setupSequencer();
    
    //setCollapsible(false);
    setSmooth(false);
    setAllSequencersActive(true);
    setBpm(120);
    setNumberColumns(numCols);
    
    GuiElement::setAutoUpdate(true);
    GuiElement::setAutoDraw(true);
}

Sequencer::~Sequencer()
{
//    ofRemoveListener(clock.beatEvent, this, &Sequencer::eventBeat);
//    ofRemoveListener(sBeatPm->elementEvent, this, &Sequencer::eventBpm);
//    ofRemoveListener(sNumCol->elementEvent, this, &Sequencer::eventNumColumns);
    
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









void Sequencer::setupSequencer()
{
    
}

void Sequencer::setupGuiPositions()
{
    
    for (auto p : sequencePairs) {
        
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
    //for (auto p : sequencePairs) {
    //    p->setNumberColumns(numCols);
    //}
    setupGuiPositions();
}









void Sequencer::setAllSequencersActive(bool allSequencesActive)
{/*
    this->allSequencesActive = allSequencesActive;
    for (auto p : sequencePairs)
    {
        for (auto &e : p->getElementPairs())
        {
            if (e->getElement()->getActive()) {
                e->getSequence()->setActive(allSequencesActive);
            }
        }
    }*/
}

void Sequencer::randomizeSequencer()
{
    
}

void Sequencer::selectColumn(int column)
{
    setActive(false);
    currentColumn = column;
    cursor = currentColumn;
    /*
    for (auto p : sequencePairs)
    {
        p->setCursor(cursor);
        for (auto &e : p->getElementPairs())
        {
            if (e->getElement()->getActive()) {
                e->lerpWidgetToSequencer(lerpNumFrames);
            }
        }
    }*/
}

void Sequencer::setColumnToCurrentValues(int column)
{
    /*
    for (auto p : sequencePairs)
    {
        for (auto &e : p->getElementPairs())
        {
            if (e->getElement()->getActive()) {
                e->setSequenceFromElement(column);
            }
        }
    }*/
}

void Sequencer::next()
{
    bpmTime = ofGetElapsedTimeMillis();
    currentColumn = fmodf(currentColumn + 1.0f, (float) numCols);
    cursor = currentColumn;
    /*
    for (auto p : sequencePairs)
    {
        p->setCursor(cursor);
        if (!smooth) {
            p->updateWidgetFromSequencer();
        }
    }*/
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
//    selectColumn(ofToFloat(b.));
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

}

void Sequencer::draw()
{

}



