#include "GuiColor.h"


GuiColor::GuiColor(string name, Parameter<ofFloatColor> *parameter) : GuiElement(name)
{
    this->parameter = parameter;
    setupColor();
}

GuiColor::GuiColor(string name, ofFloatColor *color) : GuiElement(name)
{
    parameter = new Parameter<ofFloatColor>(name, color, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1));
    setupColor();
}

void GuiColor::setupColor()
{
    channels.push_back(new GuiSlider<float>(name+".r", &parameter->getReference()->r, parameter->getMin().r, parameter->getMax().r));
    channels.push_back(new GuiSlider<float>(name+".g", &parameter->getReference()->g, parameter->getMin().g, parameter->getMax().g));
    channels.push_back(new GuiSlider<float>(name+".b", &parameter->getReference()->b, parameter->getMin().b, parameter->getMax().b));
    channels.push_back(new GuiSlider<float>(name+".a", &parameter->getReference()->a, parameter->getMin().a, parameter->getMax().a));
}

void GuiColor::setAutoUpdate(bool autoUpdate)
{
    this->autoUpdate = autoUpdate;
    GuiElement::setAutoUpdate(autoUpdate);
    for (auto c : channels) {
        c->setAutoUpdate(autoUpdate);
    }
}

void GuiColor::setAutoDraw(bool autoDraw)
{
    this->autoDraw = autoDraw;
    GuiElement::setAutoDraw(autoDraw);
    for (auto c : channels) {
        c->setAutoDraw(autoDraw);
    }
}

void GuiColor::update()
{
    for (auto c : channels)
    {
        c->update();
        c->setColorFore(parameter->get());
    }
}

void GuiColor::setupGuiComponents()
{
    for (int i=0; i<channels.size(); i++)
    {
        channels[i]->setRectangle(rectangle.x,
                                 rectangle.y + i * (GUI_ELEMENT_HEIGHT + GUI_MARGIN_BETWEEN),
                                 GUI_WIDTH - 2*GUI_MARGIN_INNER - 2*GUI_MARGIN_OUTER,
                                 GUI_ELEMENT_HEIGHT);
    }
}

void GuiColor::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
    for (auto c : channels) {
        c->mouseMoved(mouseX, mouseY);
    }
}

void GuiColor::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (active)
    {
        for (auto c : channels) {
            c->mousePressed(mouseX, mouseY);
        }
    }
}

void GuiColor::mouseReleased(int mouseX, int mouseY)
{
    GuiElement::mouseReleased(mouseX, mouseY);
    for (auto c : channels) {
        c->mouseReleased(mouseX, mouseY);
    }
}

void GuiColor::mouseDragged(int mouseX, int mouseY)
{
    GuiElement::mouseDragged(mouseX, mouseY);
    for (auto c : channels) {
        c->mouseDragged(mouseX, mouseY);
    }
}

void GuiColor::draw()
{
    ofPushMatrix();
	ofPushStyle();
    for (auto c : channels)
    {
        c->draw();
    }
	ofPopStyle();
	ofPopMatrix();
}


GuiColorEntry::GuiColorEntry(string name, Parameter<ofFloatColor> *parameter) : GuiWidgetBase(name)
{
    this->parameter = parameter;
    setupColor();
}

GuiColorEntry::GuiColorEntry(string name, ofFloatColor *color) : GuiWidgetBase(name)
{
    parameter = new Parameter<ofFloatColor>(name, color, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1));
    setupColor();
}

void GuiColorEntry::setupColor()
{
    color = new GuiColor(name, parameter);
    elements.push_back(color);
    setupGuiComponents();
    setAutoUpdate(false);
    setAutoDraw(false);
    isList = true;
    collapsed = false;
}

ofPoint GuiColorEntry::getGuiElementSize()
{
    return ofPoint(GUI_WIDTH - 2*GUI_MARGIN_OUTER - 2*GUI_MARGIN_INNER, 4 * (GUI_ELEMENT_HEIGHT + GUI_MARGIN_BETWEEN));
}
