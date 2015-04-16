#include "GuiPanel.h"


GuiPanel::GuiPanel() : GuiWidget()
{
    marginX = GUI_DEFAULT_PANEL_MARGIN_OUTER_X;
    controlRow = false;
    sequencerMade = false;
    oscManagerMade = false;
    hasParent = false;
    bOsc = false;
    bSeq = false;
    bXml = false;
    setCollapsible(true);
    enableControlRow();
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
        
        tOsc = new GuiToggle("osc", &bOsc, this, &GuiPanel::eventToggleOscManager);
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
        if (oscManagerMade) {
            delete oscManager;
        }
    }
}

void GuiPanel::createSequencer()
{
    sequencer = new Sequencer(getName()+" sequencer", this, GUI_DEFAULT_SEQUENCER_NUMCOLS);
    sequencer->setParent(this);
    sequencer->setAutoUpdate(false);
    sequencer->setAutoDraw(false);
    sequencerMade = true;
}

void GuiPanel::createOscManager(OscManager *osc)
{
    oscManager = new OscManagerPanel(getName()+" oscManager", this, osc);
    oscManager->setParent(this);
    oscManager->setAutoUpdate(false);
    oscManager->setAutoDraw(false);
    oscManagerMade = true;
    setupGuiPositions();
}

void GuiPanel::setupGuiPositions()
{
    GuiWidget::setupGuiPositions();
    if (controlRow)
    {
        if (active)
        {
            if (oscManagerMade)
            {
                int width = rectangle.width / 3.0;
                tOsc->setPosition(rectangle.x + 4, rectangle.y + headerHeight + 5);
                tSeq->setPosition(rectangle.x + 4 + width, rectangle.y + headerHeight + 5);
                tXml->setPosition(rectangle.x + 4 + 2 * width, rectangle.y + headerHeight + 5);
                tOsc->setSize(width - 8, 15);
                tSeq->setSize(width - 8, 15);
                tXml->setSize(width - 8, 15);
            }
            else {
                int width = rectangle.width / 2.0;
                tOsc->setRectangle(0, 0, 0, 0);
                tSeq->setPosition(rectangle.x + 4, rectangle.y + headerHeight + 5);
                tXml->setPosition(rectangle.x + 4 + width, rectangle.y + headerHeight + 5);
                tSeq->setSize(width - 8, 15);
                tXml->setSize(width - 8, 15);
            }
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
    if (oscManagerMade) {
        oscManager->setupGuiPositions();
    }
}

void GuiPanel::addElementToTouchOscLayout(TouchOscPage *page, float *y)
{
}

void GuiPanel::update()
{
    if (!active) {
        return;
    }
    
    GuiWidget::update();
    if (controlRow)
    {
        if (oscManagerMade) tOsc->update();
        tSeq->update();
        tXml->update();
    }
    if (sequencerMade) {
        sequencer->update();
    }
    if (oscManagerMade) {
        oscManager->update();
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
            if (oscManagerMade) tOsc->draw();
            tSeq->draw();
            tXml->draw();
        }
        
        if (bOsc && oscManagerMade) {
            oscManager->draw();
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
    if (bOsc) {
        if (oscManager->mouseMoved(mouseX, mouseY)) return true;
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
    if (bOsc) {
        if (oscManager->mousePressed(mouseX, mouseY)) return true;
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
    if (bOsc) {
        if (oscManager->mouseDragged(mouseX, mouseY)) return true;
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
    if (bOsc) {
        if (oscManager->mouseReleased(mouseX, mouseY)) return true;
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
    if (bOsc) {
        if (oscManager->keyPressed(key)) return true;
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

void GuiPanel::eventToggleOscManager(GuiButtonEventArgs &e)
{

}

void GuiPanel::savePresetPrompt(GuiButtonEventArgs &e)
{
    savePreset("/Users/Gene/Desktop/testXml.xml");
}

void GuiPanel::loadPresetPrompt(GuiButtonEventArgs &e)
{
    loadPreset("/Users/Gene/Desktop/testXml.xml");
}

void GuiPanel::savePreset(string path)
{
    ofXml xml;
    xml.addChild("Preset");
    xml.setTo("Preset");
    getXml(xml);
    xml.save(path);
}

void GuiPanel::loadPreset(string path)
{
    ofXml xml;
    xml.load(path);
    xml.setTo("Preset");
    setFromXml(xml);
}

void GuiPanel::saveSequencerToXml(ofXml &xml)
{
    if (sequencerMade)
    {
        ofXml xmlSequencer;
        xmlSequencer.addChild("Sequencer");
        xmlSequencer.setTo("Sequencer");
        sequencer->getXml(xmlSequencer);
        xml.addXml(xmlSequencer);
    }
}

void GuiPanel::loadSequencerFromXml(ofXml &xml)
{
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
    else {
        ofLog(OF_LOG_ERROR, "No sequencer found in preset");
    }
}

