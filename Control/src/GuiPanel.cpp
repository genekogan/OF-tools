#include "GuiPanel.h"


GuiPanel::GuiPanel() : GuiWidget()
{
    marginX = GUI_DEFAULT_PANEL_MARGIN_OUTER_X;
    controlRow = false;
    sequencerMade = false;
    hasParent = false;
    bOsc = false;
    bSeq = false;
    bXml = false;
    setCollapsible(true);
    enableControlRow();
    
    
    
    /////
    //createSequencer();
}

GuiPanel::~GuiPanel()
{
    if (controlRow) {
        disableControlRow();
    }
}

void GuiPanel::setPosition(int x, int y)
{
    GuiElement::setPosition(x, y);;
    if (controlRow) {
        meta->setPosition(rectangle.x + rectangle.width + 4, rectangle.y);
    }
}

void GuiPanel::enableControlRow()
{
    if (!controlRow)
    {
        controlRow = true;
        controllerHeight = GUI_DEFAULT_PANEL_CONTROLLER_HEIGHT;
        
        tOsc = new GuiToggle("osc", &bOsc);
        tSeq = new GuiToggle("seq", &bSeq, this, &GuiPanel::eventToggleSequencer);
        tXml = new GuiToggle("xml", &bXml);
        tOsc->setParent(this);
        tSeq->setParent(this);
        tXml->setParent(this);
        tOsc->setAutoUpdate(false);
        tSeq->setAutoUpdate(false);
        tXml->setAutoUpdate(false);
        tOsc->setAutoDraw(false);
        tSeq->setAutoDraw(false);
        tXml->setAutoDraw(false);
        
        
        
        
        
        
        ///////
        //
        //
        //
        //addBoundWidget(sequencer);
        
        meta = new GuiWidget("Meta");
        meta->setParent(this);
        meta->setPosition(rectangle.x + rectangle.width + 4, rectangle.y);
        meta->setAutoUpdate(false);
        meta->setAutoDraw(false);
        
        meta->addButton("save me", this, &GuiPanel::savePresetPrompt);
        meta->addButton("load me", this, &GuiPanel::loadPresetPrompt);
    }
}

void GuiPanel::disableControlRow()
{
    if (controlRow)
    {
        controlRow = false;
        controllerHeight = 0;
        
        ///////
        //
        //
        //
        //removeBoundWidget(sequencer);
        
        bOsc = false;
        bSeq = false;
        bXml = false;
        
        delete tOsc;
        delete tSeq;
        delete tXml;
        
        delete meta;
        
        if (sequencerMade) {
            delete sequencer;
        }
    }
}

void GuiPanel::setupGuiPositions()
{
    GuiWidget::setupGuiPositions();
    if (controlRow)
    {
        if (active)
        {
            int width = rectangle.width / 3.0;
            tOsc->setPosition(rectangle.x + 4, rectangle.y + headerHeight + 5);
            tSeq->setPosition(rectangle.x + 4 + width, rectangle.y + headerHeight + 5);
            tXml->setPosition(rectangle.x + 4 + 2 * width, rectangle.y + headerHeight + 5);
            tOsc->setSize(width - 8, 15);
            tSeq->setSize(width - 8, 15);
            tXml->setSize(width - 8, 15);
        }
        else
        {
            tOsc->setRectangle(0, 0, 0, 0);
            tSeq->setRectangle(0, 0, 0, 0);
            tXml->setRectangle(0, 0, 0, 0);
        }
    }
    
    if (sequencerMade) {
        sequencer->setupGuiPositions();
    }
}

void GuiPanel::update()
{
    if (!active) {
        return;
    }
    
    GuiWidget::update();
    if (controlRow)
    {
        tOsc->update();
        tSeq->update();
        tXml->update();
    }
    if (sequencerMade) {
        sequencer->update();
    }
    if (bXml) {
        meta->update();
    }
}

void GuiPanel::draw()
{    
    if (!active) {
        return;
    }

    ofPushStyle();
    ofSetLineWidth(1);
    ofSetCircleResolution(16);

    GuiWidget::draw();
    
    if (!getCollapsed())
    {
        ofPushStyle();
        ofSetColor(255, 50);
        ofRect(rectangle.x, rectangle.y + headerHeight, rectangle.width, controllerHeight);
        ofPopStyle();
        
        if (controlRow)
        {
            tOsc->draw();
            tSeq->draw();
            tXml->draw();
        }
        
        if (bSeq && sequencerMade) {
            sequencer->draw();
        }
        
        if (bXml) {
            meta->draw();
        }
    }
    
    ofPopStyle();
}

bool GuiPanel::mouseMoved(int mouseX, int mouseY)
{
    if (controlRow)
    {
        if (tOsc->mouseMoved(mouseX, mouseY)) return true;
        if (tSeq->mouseMoved(mouseX, mouseY)) return true;
        if (tXml->mouseMoved(mouseX, mouseY)) return true;
    }
    if (bSeq) {
        if (sequencer->mouseMoved(mouseX, mouseY)) return true;
    }
    if (bXml) {
        if (meta->mouseMoved(mouseX, mouseY)) return true;
    }
    return GuiWidget::mouseMoved(mouseX, mouseY);
}

bool GuiPanel::mousePressed(int mouseX, int mouseY)
{
    if (controlRow)
    {
        if (tOsc->mousePressed(mouseX, mouseY)) return true;
        if (tSeq->mousePressed(mouseX, mouseY)) return true;
        if (tXml->mousePressed(mouseX, mouseY)) return true;
    }
    if (bSeq) {
        if (sequencer->mousePressed(mouseX, mouseY)) return true;
    }
    if (bXml) {
        if (meta->mousePressed(mouseX, mouseY)) return true;
    }
    return GuiWidget::mousePressed(mouseX, mouseY);
}

bool GuiPanel::mouseDragged(int mouseX, int mouseY)
{
    if (controlRow)
    {
        if (tOsc->mouseDragged(mouseX, mouseY)) return true;
        if (tSeq->mouseDragged(mouseX, mouseY)) return true;
        if (tXml->mouseDragged(mouseX, mouseY)) return true;
    }
    if (bSeq) {
        if (sequencer->mouseDragged(mouseX, mouseY)) return true;
    }
    if (bXml) {
        if (meta->mouseDragged(mouseX, mouseY)) return true;
    }
    return GuiWidget::mouseDragged(mouseX, mouseY);
}

bool GuiPanel::mouseReleased(int mouseX, int mouseY)
{
    if (controlRow)
    {
        if (tOsc->mouseReleased(mouseX, mouseY)) return true;
        if (tSeq->mouseReleased(mouseX, mouseY)) return true;
        if (tXml->mouseReleased(mouseX, mouseY)) return true;
    }
    if (bSeq) {
        if (sequencer->mouseReleased(mouseX, mouseY)) return true;
    }
    if (bXml) {
        if (meta->mouseReleased(mouseX, mouseY)) return true;
    }
    return GuiWidget::mouseReleased(mouseX, mouseY);
}

bool GuiPanel::keyPressed(int key)
{
    if (controlRow)
    {
        if (tOsc->keyPressed(key)) return true;
        if (tSeq->keyPressed(key)) return true;
        if (tXml->keyPressed(key)) return true;
    }
    if (bSeq) {
        if (sequencer->keyPressed(key)) return true;
    }
    if (bXml) {
        if (meta->keyPressed(key)) return true;
    }
    return GuiWidget::keyPressed(key);
}

void GuiPanel::eventToggleSequencer(GuiButtonEventArgs &e)
{
    if (!sequencerMade) {
        createSequencer();
    }
}

void GuiPanel::getXml(ofXml &xml)
{
    GuiWidget::getXml(xml);
    if (sequencerMade) {
        sequencer->getXml(xml);
    }
}

void GuiPanel::setFromXml(ofXml &xml)
{
    GuiWidget::setFromXml(xml);
    
    
    if (xml.exists("Sequencer"))
    {
        if (!sequencerMade) {
            createSequencer();
        }
        sequencer->setFromXml(xml);
    }
    

    
    //////
    /*
    if (xml.exists("Sequencer"))
    {
        xml.setTo("Sequencer");
        if (!sequencerMade) {
            createSequencer();
        }
        sequencer->setFromXml(xml);
        xml.setToParent();
    }
    else {
        ofLog(OF_LOG_ERROR, "No sequencer found in preset");
    }
     */

}

