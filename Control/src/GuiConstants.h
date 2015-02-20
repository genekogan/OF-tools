#pragma once



// SEQUENCER STYLE STRUCT


static const float sequencerWidth = 480;
static const float seqHeaderHeight = 56;
static const float seqPanelMarginLeft = 4;

static ofColor GUI_DEFAULT_SEQUENCER_COLOR_INACTIVE = ofColor(200, 10, 10);
static ofColor GUI_DEFAULT_SEQUENCER_COLOR_ACTIVE = ofColor(10, 200, 10);
static ofColor GUI_DEFAULT_SEQUENCER_COLOR_FORE = ofColor(255);





static int      GUI_DEFAULT_HEADER_HEIGHT       = 50;
static int      GUI_DEFAULT_LIST_HEADER_HEIGHT  = 16;
static ofColor  GUI_DEFAULT_HEADER_COLOR        = ofColor(20, 20, 200);

static int      GUI_DEFAULT_ELEMENT_WIDTH       = 200;
static int      GUI_DEFAULT_ELEMENT_HEIGHT      = 14;

static int      GUI_DEFAULT_MARGIN_INNER        = 2;
static int      GUI_DEFAULT_MARGIN_BETWEEN      = 4;

static ofColor  GUI_DEFAULT_COLOR_BACK          = ofColor(0);
static ofColor  GUI_DEFAULT_COLOR_FORE          = ofColor(120);
static ofColor  GUI_DEFAULT_COLOR_ACTIVE        = ofColor(200, 200, 20);
static ofColor  GUI_DEFAULT_COLOR_TEXT          = ofColor(255);

static float    GUI_DEFAULT_LINE_WIDTH_ACTIVE   = 0.75;


struct GuiStyle
{
    int elementWidth;
    int elementHeight;
    
    int marginInner;
    int marginBetween;
    
    ofColor colorBackground;
    ofColor colorForeground;
    ofColor colorActive;
    ofColor colorText;
    
    GuiStyle()
    {
        elementWidth    = GUI_DEFAULT_ELEMENT_WIDTH;
        elementHeight   = GUI_DEFAULT_ELEMENT_HEIGHT;
        
        marginInner     = GUI_DEFAULT_MARGIN_INNER;
        marginBetween   = GUI_DEFAULT_MARGIN_BETWEEN;
        
        colorBackground = GUI_DEFAULT_COLOR_BACK;
        colorForeground = GUI_DEFAULT_COLOR_FORE;
        colorActive     = GUI_DEFAULT_COLOR_ACTIVE;
        colorText       = GUI_DEFAULT_COLOR_TEXT;
    }
};


struct GuiElementEventArgs
{
    string name;
    float value;
    int cell;
    
    GuiElementEventArgs(string name, int cell, float value)
    {
        this->name = name;
        this->cell = cell;
        this->value = value;
    }
};
