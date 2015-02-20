#pragma once

//#include "ofMain.h"


static int GUI_WIDTH = 200;

static int GUI_MARGIN_OUTER = 4;
static int GUI_MARGIN_INNER = 6;
static int GUI_MARGIN_BETWEEN = 2;

static int GUI_HEADER_HEIGHT = 48;
static int GUI_LIST_HEADER_HEIGHT = 16;

static int GUI_ELEMENT_HEIGHT = 14;

static int GUI_SEQUENCER_WIDTH = 200;

static ofColor GUI_COLOR_ACTIVE = ofColor(120);
static ofColor GUI_COLOR_INACTIVE = ofColor(80);






//////

static int      GUI_DEFAULT_PANEL_WIDTH         = 200;

//static int      GUI_DEFAULT_HEADER_HEIGHT       = 48;

//static int      GUI_DEFAULT_ELEMENT_HEIGHT      = 14;

static int      GUI_DEFAULT_SEQUENCER_WIDTH     = 200;






//static int      GUI_DEFAULT_ELEMENT_WIDTH       = 200;
static int      GUI_DEFAULT_LIST_HEADER_HEIGHT  = 16;



//////////

static int      GUI_DEFAULT_MARGIN_OUTER        = 4;
static int      GUI_DEFAULT_MARGIN_INNER        = 6;
static int      GUI_DEFAULT_MARGIN_BETWEEN      = 2;

static ofColor  GUI_DEFAULT_COLOR_BACK          = ofColor(0);
static ofColor  GUI_DEFAULT_COLOR_FORE          = ofColor(140);
static ofColor  GUI_DEFAULT_COLOR_ACTIVE        = ofColor(255, 255, 0);
static ofColor  GUI_DEFAULT_COLOR_TEXT          = ofColor(255);

static float    GUI_DEFAULT_LINE_WIDTH_ACTIVE   = 1;

////////

static int      GUI_DEFAULT_ELEMENT_WIDTH       = GUI_DEFAULT_PANEL_WIDTH - 2 * GUI_DEFAULT_MARGIN_INNER - 2 * GUI_DEFAULT_MARGIN_OUTER;
static int      GUI_DEFAULT_ELEMENT_HEIGHT      = 14;




struct GuiStyle
{
    int marginInner;
    int marginOuter;
    int marginBetween;
    
    ofColor colorBackground;
    ofColor colorForeground;
    ofColor colorActive;
    ofColor colorText;
    
    float lineWidthActive;
    
    GuiStyle()
    {
        marginInner     = GUI_DEFAULT_MARGIN_INNER;
        marginOuter     = GUI_DEFAULT_MARGIN_OUTER;
        marginBetween   = GUI_DEFAULT_MARGIN_BETWEEN;
        
        colorBackground = GUI_DEFAULT_COLOR_BACK;
        colorForeground = GUI_DEFAULT_COLOR_FORE;
        colorActive     = GUI_DEFAULT_COLOR_ACTIVE;
        colorText       = GUI_DEFAULT_COLOR_TEXT;
        
        lineWidthActive = GUI_DEFAULT_LINE_WIDTH_ACTIVE;
    }
};


struct GuiStyleWidget : public GuiStyle
{
    int elementWidth;
    int elementHeight;
    int listHeaderHeight;
    
    GuiStyleWidget() : GuiStyle()
    {
        elementWidth = GUI_DEFAULT_ELEMENT_WIDTH;
        elementHeight = GUI_DEFAULT_ELEMENT_HEIGHT;
        listHeaderHeight = GUI_DEFAULT_LIST_HEADER_HEIGHT;
    }
};


struct GuiButtonEventArgs
{
    string name;
    bool value;
    int cell;
    
    GuiButtonEventArgs(string name, int cell, bool value)
    {
        this->name = name;
        this->cell = cell;
        this->value = value;
    }
};


struct GuiSliderEventArgs
{
    string name;
    float value;
    
    GuiSliderEventArgs(string name, float value)
    {
        this->name = name;
        this->value = value;
    }
};

