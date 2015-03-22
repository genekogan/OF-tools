#pragma once


// ELEMENT

static int      GUI_DEFAULT_CHAR_WIDTH          = 8;

static int      GUI_DEFAULT_ELEMENT_WIDTH       = 200;
static int      GUI_DEFAULT_ELEMENT_HEIGHT      = 14;

static ofColor  GUI_DEFAULT_COLOR_BACKGROUND    = ofColor(0);
static ofColor  GUI_DEFAULT_COLOR_FOREGROUND    = ofColor(120);
static ofColor  GUI_DEFAULT_COLOR_OUTLINE       = ofColor(255);
static ofColor  GUI_DEFAULT_COLOR_ACTIVE        = ofColor(200, 200, 20);
static ofColor  GUI_DEFAULT_COLOR_ACTIVE_EDIT   = ofColor(20, 220, 20);
static ofColor  GUI_DEFAULT_COLOR_TEXT          = ofColor(255);


// WIDGET

static int      GUI_DEFAULT_HEADER_HEIGHT       = 20;
static int      GUI_DEFAULT_CONTROLLER_HEIGHT   = 0;
static int      GUI_DEFAULT_MARGIN_OUTER_X      = 6;
static int      GUI_DEFAULT_MARGIN_OUTER_Y      = 6;
static int      GUI_DEFAULT_MARGIN_INNER        = 3;

static ofColor  GUI_DEFAULT_HEADER_COLOR        = ofColor(0, 0, 255);


// COLOR WIDGET

static ofColor  GUI_DEFAULT_COLOR_TEXT_LIGHT    = ofColor(255);
static ofColor  GUI_DEFAULT_COLOR_TEXT_DARK     = ofColor(85);


// PANEL

static int      GUI_DEFAULT_PANEL_MARGIN_OUTER_X        = 2;
static int      GUI_DEFAULT_PANEL_CONTROLLER_HEIGHT     = 24;


// SEQUENCER

static int      GUI_DEFAULT_SEQUENCER_NUMCOLS           = 8;

static int      GUI_DEFAULT_SEQUENCER_PANEL_MARGIN      = 4;
static int      GUI_DEFAULT_SEQUENCER_WIDTH             = 368;

static ofColor  GUI_DEFAULT_SEQUENCER_COLOR_INACTIVE    = ofColor(200, 10, 10);
static ofColor  GUI_DEFAULT_SEQUENCER_COLOR_ACTIVE      = ofColor(10, 200, 10);
static ofColor  GUI_DEFAULT_SEQUENCER_COLOR_FOREGROUND  = ofColor(255);
static ofColor  GUI_DEFAULT_SEQUENCER_COLOR_OUTLINE     = ofColor(150);


// MIDI SEQUENCER

static int      MIDISEQUENCER_DEFAULT_WIDTH             = 720;
static int      MIDISEQUENCER_DEFAULT_HEIGHT            = 400;

static int      MIDISEQUENCER_DEFAULT_KEYBOARD_WIDTH    = 40;
static int      MIDISEQUENCER_DEFAULT_VELOCITY_HEIGHT   = 40;
static int      MIDISEQUENCER_DEFAULT_TOOLBAR_HEIGHT    = 30;
static int      MIDISEQUENCER_DEFAULT_TIMEBAR_HEIGHT    = 16;

static int      MIDISEQUENCER_DEFAULT_BPM               = 360;
static int      MIDISEQUENCER_DEFAULT_MEASURES          = 12;
static int      MIDISEQUENCER_DEFAULT_NOTE_MIN          = 60;
static int      MIDISEQUENCER_DEFAULT_NOTE_MAX          = 78;
static float    MIDISEQUENCER_DEFAULT_START             = 0.0;
static float    MIDISEQUENCER_DEFAULT_END               = 0.25;

static ofColor  MIDISEQUENCER_DEFAULT_COLOR_FOREGROUND  = ofColor(20, 20, 150);
static ofColor  MIDISEQUENCER_DEFAULT_COLOR_OUTLINE     = ofColor(220, 80, 50);
static ofColor  MIDISEQUENCER_DEFAULT_COLOR_ACTIVE      = ofColor(160, 240, 20);


// OSC MANAGER

static int      GUI_DEFAULT_OSCMANAGER_WIDTH            = 480;