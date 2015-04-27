#pragma once

#include "ofMain.h"
#include "Base64.h"


enum TouchOscColor
{
    RED, GREEN, BLUE,
    YELLOW, PURPLE, GRAY,
    ORANGE, BROWN, PINK
};


class TouchOscWidget
{
public:
    TouchOscWidget(string name, float x, float y, float w, float h, TouchOscColor color)
    {
        this->name = name;
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        setColor(color);
        scaleX = 1.0;
        scaleY = 1.0;
        oscManual = false;
    }
    
    virtual void setOscAddress(string oscAddress)
    {
        this->oscAddress = oscAddress;
        oscManual = true;
    }
    
    virtual void setColor(TouchOscColor color)
    {
        if      (color == RED)      this->color = "red";
        else if (color == GREEN)    this->color = "green";
        else if (color == BLUE)     this->color = "blue";
        else if (color == YELLOW)   this->color = "yellow";
        else if (color == PURPLE)   this->color = "purple";
        else if (color == GRAY)     this->color = "gray";
        else if (color == ORANGE)   this->color = "orange";
        else if (color == BROWN)    this->color = "brown";
        else if (color == PINK)     this->color = "pink";
    }
    
    virtual void setScale(float scaleX, float scaleY)
    {
        this->scaleX = scaleX;
        this->scaleY = scaleY;
    }
    
    virtual string getXmlInner()
    {
        xml = "";
        string b64name = base64_encode(reinterpret_cast<const unsigned char*>(name.c_str()), name.length());
        xml += "<control name=\""+b64name+"\" ";
        xml += "type=\""+type+"\" ";
        xml += "x=\""+ofToString((int)(scaleX * x))+"\" y=\""+ofToString((int)(scaleY * y))+"\" ";
        xml += "w=\""+ofToString((int)(scaleX * w))+"\" h=\""+ofToString((int)(scaleY * h))+"\" color=\""+color+"\" ";
        getXml();
        xml += " >\n";
        xml += "</control>\n";
        return xml;
    }
    
    virtual void getXml()
    {
    };
    
    string xml;
    string type;
    string name;
    string oscAddress;
    string color;
    float x, y, w, h;
    float scaleX, scaleY;
    bool oscManual;
};


// labels, status displays

class TouchOscMetaWidget : public TouchOscWidget
{
public:
    TouchOscMetaWidget(string name, float x, float y, float w, float h, TouchOscColor color=RED, int textSize=14, bool background=false, bool outline=false) : TouchOscWidget(name, x, y, w, h, color)
    {
        setTextSize(textSize);
        setBackground(background);
        setOutline(outline);
    }
    
    virtual void getXml()
    {
        xml += "size=\""+ofToString(textSize)+"\" background=\""+background+"\" outline=\""+outline+"\" ";
    }
    
    virtual void setTextSize(int textSize) {this->textSize = textSize;}
    virtual void setBackground(bool background) {this->background = background ? "true" : "false";}
    virtual void setOutline(bool outline) {this->outline = outline ? "true" : "false";}
    
    string background, outline;
    int textSize;
};

class TouchOscLabel : public TouchOscMetaWidget
{
public:
    TouchOscLabel(string name, float x, float y, float w, float h, TouchOscColor color=RED, int textSize=14, bool background=false, bool outline=false) : TouchOscMetaWidget(name, x, y, w, h, color, textSize, background, outline)
    {
        label = name;
        type = (w >= h) ? "labelh" : "labelv";
    }
    
    virtual void getXml()
    {
        TouchOscMetaWidget::getXml();
        string labelb64 = base64_encode(reinterpret_cast<const unsigned char*>(label.c_str()), label.length());
        xml += "text=\""+labelb64+"\" ";
    }
    
    virtual void setType(int t) {type = (t == 0) ? "labelh" : "labelv";}
    virtual void setLabel(string label) {this->label = label;}
    virtual void setHorizontal() {type = "labelh";}
    virtual void setVertical() {type = "labelv";}
    
    string label;
};

class TouchOscBattery : public TouchOscMetaWidget
{
public:
    TouchOscBattery(string name, float x, float y, float w, float h, TouchOscColor color=RED, int textSize=14, bool background=false, bool outline=false) : TouchOscMetaWidget(name, x, y, w, h, color, textSize, background, outline)
    {
        type = (w >= h) ? "batteryh" : "batteryv";
    }
    
    virtual void getXml()
    {
        TouchOscMetaWidget::getXml();
    }
    
    virtual void setType(int t) {type = (t == 0) ? "batteryh" : "batteryv";}
    virtual void setHorizontal() {type = "batteryh";}
    virtual void setVertical() {type = "batteryv";}
};

class TouchOscTime : public TouchOscMetaWidget
{
public:
    TouchOscTime(string name, float x, float y, float w, float h, TouchOscColor color=RED, int textSize=14, bool background=false, bool outline=false, bool show_seconds=false) : TouchOscMetaWidget(name, x, y, w, h, color, textSize, background, outline)
    {
        setShowSeconds(show_seconds);
        type = (w >= h) ? "timeh" : "timev";
    }
    
    virtual void getXml()
    {
        TouchOscMetaWidget::getXml();
        xml += "seconds=\""+show_seconds+"\" ";
    }
    
    virtual void setType(int t) {type = (t == 0) ? "timeh" : "timev";}
    virtual void setShowSeconds(bool show_seconds) {this->show_seconds = show_seconds ? "true" : "false";}
    virtual void setHorizontal() {type = "timeh";}
    virtual void setVertical() {type = "timev";}
    
    string show_seconds;
};


// widgets with numerical values

class TouchOscNumericWidget : public TouchOscWidget
{
public:
    TouchOscNumericWidget(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0) : TouchOscWidget(name, x, y, w, h, color)
    {
        this->min = min;
        this->max = max;
    }
    
    virtual void getXml()
    {
        xml += "scalef=\""+ofToString(min)+"\" scalet=\""+ofToString(max)+"\" ";
        if (oscManual) {
            string oscAddressb64 = base64_encode(reinterpret_cast<const unsigned char*>(oscAddress.c_str()), oscAddress.length());
            xml += "osc_cs=\""+oscAddressb64+"\" ";
        }
    }
    
    virtual void setRange(float min, float max) { this->min = min; this->max = max; }
    virtual void setMin(float min) { this->min = min; }
    virtual void setMax(float max) { this->max = max; }
    
    float min, max;
};

class TouchOscFader : public TouchOscNumericWidget
{
public:
    TouchOscFader(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0, bool inverted=false, bool centered=false, bool responseRelative=false) : TouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        setInverted(inverted);
        setCentered(centered);
        setResponseRelative(responseRelative);
        type = (w >= h) ? "faderh" : "faderv";
    }
    
    virtual void getXml()
    {
        TouchOscNumericWidget::getXml();
        xml += "response=\""+response+"\" inverted=\""+inverted+"\" centered=\""+centered+"\" ";
    }
    
    virtual void setType(int t) {type = (t == 0) ? "faderh" : "faderv";}
    virtual void setHorizontal() {type="faderh";}
    virtual void setVertical() {type="faderv";}
    virtual void setInverted(bool inverted) { this->inverted = inverted ? "true" : "false"; }
    virtual void setCentered(bool centered) { this->centered = centered ? "true" : "false"; }
    virtual void setResponseRelative(bool responseRelative) { this->response = responseRelative ? "relative" : "absolute"; }
    
    string inverted, centered, response;
};

class TouchOscRotary : public TouchOscFader
{
public:
    TouchOscRotary(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0, bool inverted=false, bool centered=false, bool responseRelative=false, bool norollover=false) : TouchOscFader(name, x, y, w, h, color, min, max, inverted, centered, responseRelative)
    {
        setNoRollover(norollover);
        type = (w >= h) ? "rotaryh" : "rotaryv";
    }
    
    virtual void getXml()
    {
        TouchOscFader::getXml();
        xml += "norollover=\""+norollover+"\" ";
    }
    
    virtual void setHorizontal() {type="rotaryh";}
    virtual void setVertical() {type="rotaryv";}
    virtual void setNoRollover(bool norollover) {this->norollover = (norollover) ? "true" : "false"; }
    
    string norollover;
};

class TouchOscLed : public TouchOscNumericWidget
{
public:
    TouchOscLed(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0) : TouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        type = "led";
    }
    
    virtual void getXml()
    {
        TouchOscNumericWidget::getXml();
    }
};

class TouchOscButton : public TouchOscNumericWidget
{
public:
    TouchOscButton(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0, bool local_off=false, bool velocity=false) : TouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        setLocalOff(local_off);
        setVelocity(velocity);
        type = "push";
    }
    
    virtual void getXml()
    {
        TouchOscNumericWidget::getXml();
        xml += "local_off=\""+local_off+"\" velocity=\""+velocity+"\" ";
    }
    
    virtual void setLocalOff(bool local_off) {this->local_off = local_off ? "true" : "false";}
    virtual void setVelocity(bool velocity) {this->velocity = velocity ? "1" : "0";}
    
    string velocity, local_off;
};

class TouchOscToggle : public TouchOscNumericWidget
{
public:
    TouchOscToggle(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0, bool local_off=false) : TouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        setLocalOff(local_off);
        type = "toggle";
    }
    
    virtual void getXml()
    {
        TouchOscNumericWidget::getXml();
        xml += "local_off=\""+local_off+"\" ";
    }
    
    virtual void setLocalOff(bool local_off) {this->local_off = local_off ? "true" : "false";}
    
    string local_off;
};

class TouchOscEncoder : public TouchOscNumericWidget
{
public:
    TouchOscEncoder(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0, bool local_off=false) : TouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        type = "encoder";
    }
    
    virtual void getXml()
    {
        TouchOscNumericWidget::getXml();
    }
};


class TouchOscXy : public TouchOscNumericWidget
{
public:
    TouchOscXy(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0, bool inverted_x=false, bool inverted_y=false) : TouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        setInvertedX(inverted_x);
        setInvertedY(inverted_y);
        type = "xy";
    }
    
    virtual void getXml()
    {
        TouchOscNumericWidget::getXml();
        xml += "inverted_x=\""+ofToString(inverted_x)+"\" inverted_y=\""+ofToString(inverted_y)+"\" ";
    }
    
    virtual void setInvertedX(bool inverted_x) { this->inverted_x = inverted_x ? "true" : "false"; }
    virtual void setInvertedY(bool inverted_y) { this->inverted_y = inverted_y ? "true" : "false"; }
    
    string inverted_x, inverted_y;
};


class TouchOscMultiPush : public TouchOscNumericWidget
{
public:
    TouchOscMultiPush(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0, int number_x=2, int number_y=2, bool local_off=false) : TouchOscNumericWidget(name, x, y, w, h, color, min, max) {
        setLocalOff(local_off);
        setNumberX(number_x);
        setNumberY(number_y);
        type = "multipush";
    }
    
    virtual void getXml()
    {
        TouchOscNumericWidget::getXml();
        xml += "number_x=\""+ofToString(number_x)+"\" number_y=\""+ofToString(number_y)+"\" local_off=\""+local_off+"\" ";
    }
    
    virtual void setLocalOff(bool local_off) {this->local_off = local_off ? "true" : "false";}
    virtual void setNumberX(int number_x) {this->number_x = number_x;}
    virtual void setNumberY(int number_y) {this->number_y = number_y;}
    
    int number_x, number_y;
    string local_off;
};

class TouchOscMultiToggle : public TouchOscMultiPush
{
public:
    TouchOscMultiToggle(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0, int number_x=2, int number_y=2, bool local_off=false, bool ex_mode=false) : TouchOscMultiPush(name, x, y, w, h, color, min, max)
    {
        setExclusive(ex_mode);
        type = "multitoggle";
    }
    
    virtual void getXml()
    {
        TouchOscMultiPush::getXml();
        xml += "ex_mode=\""+ex_mode+"\" ";
    }
    
    virtual void setExclusive(bool ex_mode) {this->ex_mode = ex_mode ? "true" : "false";}
    
    string local_off, ex_mode;
};

class TouchOscMultiFader : public TouchOscNumericWidget
{
public:
    TouchOscMultiFader(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0, int number=2, bool inverted=false, bool centered=false) : TouchOscNumericWidget(name, x, y, w, h, color, min, max)
    {
        setNumber(number);
        setInverted(inverted);
        setCentered(centered);
        type = (w >= h) ? "multifaderh" : "multifaderv";
    }
    
    virtual void getXml()
    {
        TouchOscNumericWidget::getXml();
        xml += "number=\""+ofToString(number)+"\" inverted=\""+inverted+"\" centered=\""+centered+"\" ";
    }
    
    virtual void setType(int t) {type = (t == 0) ? "multifaderh" : "multifaderv";}
    virtual void setNumber(int number) {this->number = number;}
    virtual void setInverted(bool inverted) { this->inverted = inverted ? "true" : "false"; }
    virtual void setCentered(bool centered) { this->centered = centered ? "true" : "false"; }
    
    int number;
    string inverted, centered;
};

class TouchOscMultiXy : public TouchOscXy
{
public:
    TouchOscMultiXy(string name, float x, float y, float w, float h, TouchOscColor color=RED, float min=0.0, float max=1.0, bool inverted_x=false, bool inverted_y=false) : TouchOscXy(name, x, y, w, h, color, min, max)
    {
        type = "multixy";
    }
    
    virtual void getXml()
    {
        TouchOscXy::getXml();
    }
};



class TouchOscPage
{
public:
    ~TouchOscPage();
    TouchOscPage(string name, TouchOscColor color=YELLOW);
    void setScale(float scaleX, float scaleY);
    void setDefaultWidgetColor(TouchOscColor color);
    string getXml();
    
    TouchOscLabel* addLabel(string name, float x, float y, float w, float h);
    TouchOscTime* addTime(string name, float x, float y, float w, float h);
    TouchOscBattery* addBattery(string name, float x, float y, float w, float h);
    TouchOscFader* addFader(string name, float x, float y, float w, float h);
    TouchOscRotary* addRotary(string name, float x, float y, float w, float h);
    TouchOscLed* addLed(string name, float x, float y, float w, float h);
    TouchOscButton* addButton(string name, float x, float y, float w, float h);
    TouchOscToggle* addToggle(string name, float x, float y, float w, float h);
    TouchOscEncoder* addEncoder(string name, float x, float y, float w, float h);
    TouchOscXy* addXy(string name, float x, float y, float w, float h);
    TouchOscMultiPush* addMultiPush(string name, float x, float y, float w, float h);
    TouchOscMultiToggle* addMultiToggle(string name, float x, float y, float w, float h);
    TouchOscMultiFader* addMultiFader(string name, float x, float y, float w, float h);
    TouchOscMultiXy* addMultiXy(string name, float x, float y, float w, float h);
    
    vector<TouchOscWidget *> & getWidgets() {return widgets;}
    
private:
    
    vector<TouchOscWidget *> widgets;
    string name;
    float scaleX, scaleY;
    TouchOscColor defaultColor;
};


class TouchOsc
{
public:
    TouchOsc();
    ~TouchOsc();

    TouchOscPage* addPage(string name);
    void addPage(TouchOscPage* newPage);
    void setDefaultColor(TouchOscColor color);
    void setScale(float scaleX, float scaleY);
    void save(string name);
    string getXml();
    
private:
    vector<TouchOscPage *> pages;
    float scaleX, scaleY;
    TouchOscColor defaultColor;
    bool customResolution;
};

