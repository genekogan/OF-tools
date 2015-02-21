#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Toggles & buttons
    
    
    toggle = new GuiToggle("toggle bgColor", &myToggle);
    toggle->setPosition(10, 10);
    
    toggleNotify = new GuiToggle("toggle + notify", this, &ofApp::toggleNotifier);
    toggleNotify->setRectangle(10, 50, 130, 30);
    
    button = new GuiButton("a button", this, &ofApp::buttonNotifier);
    button->setPosition(10, 105);

    
    // Sliders
    
    intSlider = new GuiSlider<int>("int slider", &intValue, 5, 10);
    intSlider->setRectangle(ofRectangle(300, 10, 200, 40));
    
    floatSlider = new GuiSlider<float>("float slider", &floatValue, 0.1, 0.5);
    floatSlider->setPosition(300, 80);
    
    doubleSlider = new GuiSlider<double>("double slider", &doubleValue, 4.0, 9.2);
    doubleSlider->setRectangle(ofRectangle(300, 150, 200, 40));

    
    // Color picker
    
    colorPicker = new GuiColor("my color", &bgColor);
    colorPicker->setPosition(620, 200);

    
    // Menu
    
    vector<string> menuItems;
    menuItems.push_back("hello world");
    menuItems.push_back("menu items");
    menuItems.push_back("another item");
    menuItems.push_back("ipsum lorem");
    
    menu = new GuiMenu("my menu", menuItems, this, &ofApp::menuNotifier);
    menu->setPosition(20, 200);
    menu->setAutoClose(true);

    menuMultiChoice = new GuiMenu("my menu mc", menuItems, this, &ofApp::menuNotifierMultiChoice);
    menuMultiChoice->setPosition(320, 200);
    menuMultiChoice->setMultipleChoice(true);
    
    
    
    myToggle = true;
    bgColor.set(0.5, 0.2, 0.1);
     
}

//--------------------------------------------------------------
void ofApp::update(){
    floatValue = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0.1, 0.5);
    bgColor.set(ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, 1), bgColor.g, bgColor.b);
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (myToggle) {
        ofBackground(bgColor);
    }
    else {
        ofBackground(100);
    }
    
    ofDrawBitmapString("float value "+ofToString(floatValue)+"\n"+
                       "int value "+ofToString(intValue)+"\n"+
                       "double value "+ofToString(doubleValue)+"\n", 20, 400);
}

//--------------------------------------------------------------
ofApp::~ofApp() {
    delete button;
    delete toggle;
    delete toggleNotify;
    delete floatSlider;
    delete intSlider;
    delete doubleSlider;
    delete menu;
    delete menuMultiChoice;
}
