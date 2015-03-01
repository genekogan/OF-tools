#include "GuiPanel.h"


GuiPanel::GuiPanel() : GuiWidget()
{
    marginOuterX = GUI_DEFAULT_PANEL_MARGIN_OUTER_X;
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
}

GuiPanel::~GuiPanel()
{
    if (bSeq) {
        delete sequencer;
    }
    delete tOsc;
    delete tSeq;
    delete tXml;
}

GuiWidget * GuiPanel::addWidget(string name)
{
    GuiElementGroup *elementGroup = new GuiElementGroup();
    GuiWidget * widget = new GuiWidget(name);
    widget->setParent(this);
    elementGroup->addElement(widget);
    setupElementGroup(elementGroup);
    ofAddListener(widget->widgetChanged, this, &GuiPanel::eventWidgetChanged);
    return widget;
}

void GuiPanel::addWidget(GuiWidget *widget)
{
    GuiElementGroup *elementGroup = new GuiElementGroup();
    widget->setParent(this);
    elementGroup->addElement(widget);
    setupElementGroup(elementGroup);
    ofAddListener(widget->widgetChanged, this, &GuiPanel::eventWidgetChanged);
}

void GuiPanel::enableSequencer()
{
    bSeq = true;
    sequencer = new Sequencer(getName()+" sequencer", this, GUI_DEFAULT_SEQUENCER_NUMCOLS);
    sequencer->setParent(this);
    sequencer->setAutoUpdate(false);
    sequencer->setAutoDraw(false);
}

void GuiPanel::disableSequencer()
{
    bSeq = false;
    delete sequencer;
}

void GuiPanel::setupGuiComponents()
{
    GuiWidget::setupGuiComponents();
    tOsc->setRectangle(rectangle.x + 8, rectangle.y + headerHeight + 5, 50, 15);
    tSeq->setRectangle(rectangle.x + 64, rectangle.y + headerHeight + 5, 50, 15);
    tXml->setRectangle(rectangle.x + 120, rectangle.y + headerHeight + 5, 50, 15);
}

void GuiPanel::update()
{
    GuiWidget::update();
    tOsc->update();
    tSeq->update();
    tXml->update();
    if (bSeq) {
        sequencer->update();
    }
}

void GuiPanel::draw()
{
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

        tOsc->draw();
        tSeq->draw();
        tXml->draw();

        if (bSeq) {
            sequencer->draw();
        }
    }
    
    ofPopStyle();
}

bool GuiPanel::mouseMoved(int mouseX, int mouseY)
{
    if (tOsc->mouseMoved(mouseX, mouseY)) return true;
    if (tSeq->mouseMoved(mouseX, mouseY)) return true;
    if (tXml->mouseMoved(mouseX, mouseY)) return true;
    if (bSeq) {
        if (sequencer->mouseMoved(mouseX, mouseY)) return true;
    }
    return GuiWidget::mouseMoved(mouseX, mouseY);
}

bool GuiPanel::mousePressed(int mouseX, int mouseY)
{
    if (tOsc->mousePressed(mouseX, mouseY)) return true;
    if (tSeq->mousePressed(mouseX, mouseY)) return true;
    if (tXml->mousePressed(mouseX, mouseY)) return true;
    if (bSeq) {
        if (sequencer->mousePressed(mouseX, mouseY)) return true;
    }
    return GuiWidget::mousePressed(mouseX, mouseY);
}

bool GuiPanel::mouseDragged(int mouseX, int mouseY)
{
    if (tOsc->mouseDragged(mouseX, mouseY)) return true;
    if (tSeq->mouseDragged(mouseX, mouseY)) return true;
    if (tXml->mouseDragged(mouseX, mouseY)) return true;
    if (bSeq) {
        if (sequencer->mouseDragged(mouseX, mouseY)) return true;
    }
    return GuiWidget::mouseDragged(mouseX, mouseY);
}

bool GuiPanel::mouseReleased(int mouseX, int mouseY)
{
    if (tOsc->mouseReleased(mouseX, mouseY)) return true;
    if (tSeq->mouseReleased(mouseX, mouseY)) return true;
    if (tXml->mouseReleased(mouseX, mouseY)) return true;
    if (bSeq) {
        if (sequencer->mouseReleased(mouseX, mouseY)) return true;
    }
    return GuiWidget::mouseReleased(mouseX, mouseY);
}

bool GuiPanel::keyPressed(int key)
{
    if (tOsc->keyPressed(key)) return true;
    if (tSeq->keyPressed(key)) return true;
    if (tXml->keyPressed(key)) return true;
    if (bSeq) {
        if (sequencer->keyPressed(key)) return true;
    }
    return GuiWidget::keyPressed(key);
}

void GuiPanel::eventWidgetChanged(string & s)
{
    setupGuiComponents();
}

void GuiPanel::eventToggleSequencer(GuiElementEventArgs &e)
{
    if (bSeq) {
        enableSequencer();
    }
    else {
        disableSequencer();
    }
}