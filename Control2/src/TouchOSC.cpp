#include "TouchOsc.h"



TouchOscLabel* TouchOscPage::addLabel(string name, float x, float y, float w, float h)
{
    TouchOscLabel *widget = new TouchOscLabel(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscTime* TouchOscPage::addTime(string name, float x, float y, float w, float h)
{
    TouchOscTime *widget = new TouchOscTime(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscBattery* TouchOscPage::addBattery(string name, float x, float y, float w, float h)
{
    TouchOscBattery *widget = new TouchOscBattery(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscFader* TouchOscPage::addFader(string name, float x, float y, float w, float h)
{
    TouchOscFader *widget = new TouchOscFader(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscRotary* TouchOscPage::addRotary(string name, float x, float y, float w, float h)
{
    TouchOscRotary *widget = new TouchOscRotary(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscLed* TouchOscPage::addLed(string name, float x, float y, float w, float h)
{
    TouchOscLed *widget = new TouchOscLed(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscButton* TouchOscPage::addButton(string name, float x, float y, float w, float h)
{
    TouchOscButton *widget = new TouchOscButton(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscToggle* TouchOscPage::addToggle(string name, float x, float y, float w, float h)
{
    TouchOscToggle *widget = new TouchOscToggle(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscEncoder* TouchOscPage::addEncoder(string name, float x, float y, float w, float h)
{
    TouchOscEncoder *widget = new TouchOscEncoder(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscXy* TouchOscPage::addXy(string name, float x, float y, float w, float h)
{
    TouchOscXy *widget = new TouchOscXy(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscMultiPush* TouchOscPage::addMultiPush(string name, float x, float y, float w, float h)
{
    TouchOscMultiPush *widget = new TouchOscMultiPush(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscMultiToggle* TouchOscPage::addMultiToggle(string name, float x, float y, float w, float h)
{
    TouchOscMultiToggle *widget = new TouchOscMultiToggle(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscMultiFader* TouchOscPage::addMultiFader(string name, float x, float y, float w, float h)
{
    TouchOscMultiFader *widget = new TouchOscMultiFader(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscMultiXy* TouchOscPage::addMultiXy(string name, float x, float y, float w, float h)
{
    TouchOscMultiXy *widget = new TouchOscMultiXy(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

TouchOscPage::TouchOscPage(string name, TouchOscColor color)
{
    this->name = name;
    this->defaultColor = color;
}

void TouchOscPage::setScale(float scaleX, float scaleY)
{
    this->scaleX = scaleX;
    this->scaleY = scaleY;
}

void TouchOscPage::setDefaultWidgetColor(TouchOscColor color)
{
    this->defaultColor = color;
}

string TouchOscPage::getXml()
{
    string b64name = base64_encode(reinterpret_cast<const unsigned char*>(name.c_str()), name.length());
    
    string xml;
    xml += "<tabpage name=\""+b64name+"\" scalef=\"0.0\" scalet=\"1.0\" >\n";
    for (int i=0; i<widgets.size(); i++) {
        widgets[i]->setScale(scaleX, scaleY);
        xml += widgets[i]->getXmlInner();
    }
    xml += "</tabpage>\n";
    return xml;
}

TouchOsc::TouchOsc()
{
    defaultColor = RED;
    customResolution = false;
}

TouchOscPage* TouchOsc::addPage(string name)
{
    TouchOscPage *newPage = new TouchOscPage(name, defaultColor);
    pages.push_back(newPage);
    return newPage;
}

void TouchOsc::addPage(TouchOscPage* newPage)
{
    pages.push_back(newPage);
}

string TouchOsc::getXml()
{
    string xml;
    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    if (customResolution)
        xml += "<layout version=\"13\" mode=\"3\" w=\""+ofToString((int)scaleX)+"\" h=\""+ofToString((int)scaleY)+"\" orientation=\"horizontal\">";
    else
        xml += "<layout version=\"13\" mode=\"0\" orientation=\"horizontal\">";
    for (int i=0; i<pages.size(); i++) {
        pages[i]->setScale(scaleX, scaleY);
        xml += pages[i]->getXml();
    }
    xml += "</layout>";
    return xml;
}

void TouchOsc::setDefaultColor(TouchOscColor color)
{
    this->defaultColor = color;
}

void TouchOsc::setScale(float scaleX, float scaleY)
{
    this->scaleX = scaleX;
    this->scaleY = scaleY;
    customResolution = true;
}

void TouchOsc::save(string name)
{
    ofFile file("index.xml", ofFile::WriteOnly);
    file << getXml();
    file.close();
    string cmd;
    cmd += "cd "+ofToString(ofToDataPath("")) + "; ";
    cmd += "zip -r "+name+".touchosc index.xml; ";
    cmd += "rm index.xml; ";
    cmd += "open "+name+".touchosc; ";
    ofSystem(cmd);
}

TouchOscPage::~TouchOscPage()
{
    for (int i=0; i<widgets.size(); i++) {
        delete widgets[i];
    }
}

TouchOsc::~TouchOsc()
{
    for (int i=0; i<pages.size(); i++) {
        delete pages[i];
    }
}