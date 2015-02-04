#pragma once

static int GUI_WIDTH = 200;

static int GUI_MARGIN_OUTER = 4;
static int GUI_MARGIN_INNER = 6;
static int GUI_MARGIN_BETWEEN = 2;

static int GUI_HEADER_HEIGHT = 16;

static int GUI_ELEMENT_HEIGHT = 14;

static int GUI_SEQUENCER_WIDTH = 200;

static ofColor GUI_COLOR_ACTIVE = ofColor(120);
static ofColor GUI_COLOR_INACTIVE = ofColor(80);




static ofColor GUI_DEFAULT_COLOR_BACK = ofColor(0);
static ofColor GUI_DEFAULT_COLOR_FORE = ofColor(140);
static ofColor GUI_DEFAULT_COLOR_ACTIVE = ofColor(255, 255, 0);
static ofColor GUI_DEFAULT_COLOR_TEXT = ofColor(255);
static float GUI_DEFAULT_LINE_WIDTH_ACTIVE = 1;


struct GuiStyle
{
    float h;
};



struct GuiButtonEventArgs
{
    string name;
    bool value;
    GuiButtonEventArgs(string name, bool value)
    {
        this->name = name;
        this->value = value;
    }
};
