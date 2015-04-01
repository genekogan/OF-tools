#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // a widget is a collection of gui elements
    
    widget.setName("a widget");
    widget.setPosition(810, 10);

    // can add buttons and toggles with or without event notifiers
    widget.addButton("button", &button);
    widget.addButton("event", this, &ofApp::buttonEvent);
    widget.addToggle("toggle", &toggle);

    // text box
    widget.addTextBox("hello", this, &ofApp::textBoxEvent);
    
    // sliders
    widget.addSlider("float slider", &floatSlider, 0.0f, 10.0f);
    widget.addMultiSlider("vec4f slider", &vec4slider, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    
    // range sliders
    widget.addRangeSlider("range", &rangeLow, &rangeHigh, 0.0f, 10.0f);
    widget.addMultiRangeSlider("vec2range", &vec2RangeLow, &vec2RangeHigh, ofVec2f(0,0), ofVec2f(10, 20));

    // color widget
    // note that color must be an ofFloatColor, not ofColor
    widget.addColor("color", &color);

    // menu (single choice by default). toggle autoClose after a selection (default false)
    GuiMenu *menu = widget.addMenu("menu", this, &ofApp::menuSelect);
    menu->setAutoClose(true);
    menu->addToggle("new york");
    menu->addToggle("los angeles");
    menu->addToggle("chicago");

    // 2d pad
    Gui2dPad *pad = widget.add2dPad("pad", &padValue, ofPoint(0, 0), ofPoint(20, 12));
    pad->setDrawConnectedPoints(true);
    
    // a panel is a super-widget with some extra functionality, including
    // ability to embed widgets, a sequencer, presets, and osc functionality
    
    panel.setPosition(10, 10);
    panel.setName("a panel");
    
    // you can add gui elements to a panel just as you can to a widget.
    // these sliders also has an event notifier
    panel.addMultiSlider("slider w/ notifier", &vec2slider, ofVec2f(0, 0), ofVec2f(10, 10), this, &ofApp::sliderEvent);
    panel.addColor("color w/ notifier", &color2, this, &ofApp::colorEvent);

    // you can embed a widget inside a panel
    GuiWidget *innerWidget = panel.addWidget("widget inside panel"); 
    innerWidget->addButton("button", &button);
    innerWidget->addToggle("another toggle", this, &ofApp::panelToggleEvent);

    // menus can allow multiple choice
    GuiMenu *menu2 = innerWidget->addMenu("multi choice menu", this, &ofApp::multiChoiceMenuSelect, true);
    menu2->setMultipleChoice(true);
    menu2->addToggle("kamusta mundo");
    menu2->addToggle("xin chao the gioi");
    menu2->addToggle("namaste varlda");
    menu2->addToggle("suesday piphoplok");
    menu2->addToggle("suwati lok");

    // sliders can have different types
    panel.addSlider("float slider", &floatSlider, 0.0f, 10.0f);
    panel.addSlider("int slider", &intSlider, 5, 12);
    panel.addSlider("double slider", &doubleSlider, 5.0, 23.0);
    
    
    // initialize variables.
    // gui is linked to actual values and is auto-updating
    color2.set(0.5, 0.2, 0.9);
    padValue.set(5, 8);
    intSlider = 8;
    floatSlider = 8.5;
    rangeLow = 2.0;
    rangeHigh = 6.0;
    
    
    
    // misc notes -- uncomment to try
    
    // you can embed an existing widget into a panel:
    //panel.addWidget(&widget);
    
    // setAutoUpdate toggles automatic updating and mouse/keyboard interaction
    // if it is set to off, you are responsible for the update, mouseMoved,
    // mousePressed, mouseDragged, mouseReleased, keyPressed functions yourself.
    // setAutoDraw toggles automatic drawing -- if false, you are responsible for drawing widget
    //panel.setAutoUpdate(false);
    //panel.setAutoDraw(false);
    
    

    

    //ofExit();
    
}

//--------------------------------------------------------------
void ofApp::update(){

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString("click 'n' key while mouse hovering over 2d pad to add another point", 10, ofGetHeight() - 60);
    ofCircle(600, 500, 100);
}

//--------------------------------------------------------------
void ofApp::buttonEvent(GuiButtonEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "button event: " + e.button->getName() + " = " + ofToString(e.value));
}

//--------------------------------------------------------------
void ofApp::menuSelect(GuiMenuEventArgs & e)
{
//    ofLog(OF_LOG_NOTICE, "menu selection event: " + e.slider + " = " + ofToString(e.value));
}

//--------------------------------------------------------------
void ofApp::sliderEvent(GuiMultiSliderEventArgs<ofVec2f> & e)
{
    ofLog(OF_LOG_NOTICE, "slider event: " + e.slider->getName() + " = " + ofToString(e.value));
}

//--------------------------------------------------------------
void ofApp::colorEvent(GuiColorEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "color event: " + e.slider->getName() + " = " + ofToString(e.color));
}

//--------------------------------------------------------------
void ofApp::panelToggleEvent(GuiButtonEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "toggle event: " + e.button->getName() + " = " + ofToString(e.value));
}

//--------------------------------------------------------------
void ofApp::multiChoiceMenuSelect(GuiMenuEventArgs & e)
{
//    ofLog(OF_LOG_NOTICE, "panel menu event: " + e. + " = " + ofToString(e.value));
}

