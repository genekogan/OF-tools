#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    widget.setName("a widget");
    widget.setPosition(10, 10);
    widget.addButton("button", &button);
    widget.addButton("event", this, &ofApp::buttonEvent);
    widget.addToggle("toggle", &toggle);
    widget.addSlider("float slider", &floatSlider, 0.0f, 10.0f);
    widget.addSlider("vec4f slider", &vec4slider, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    widget.addRangeSlider("range", &rangeLow, &rangeHigh, 0.0f, 10.0f);
    widget.addRangeSlider("vec2range", &vec2RangeLow, &vec2RangeHigh, ofVec2f(0,0), ofVec2f(10, 20));
    widget.addColor("color", &color);
    GuiMenu *menu = widget.addMenu("menu", this, &ofApp::menuSelect);
    menu->setAutoClose(false);
    menu->addToggle("new york");
    menu->addToggle("los angeles");
    menu->addToggle("chicago");
    widget.add2dPad("pad", &padValue, ofPoint(0, 0), ofPoint(20, 12));


    
    panel.setPosition(230, 10);
    panel.setName("a panel");
    panel.addSlider("slider w/ notifier", &vec2slider, ofVec2f(0, 0), ofVec2f(10, 10), this, &ofApp::sliderEvent);
    panel.addColor("color w/ notifier", &color2, this, &ofApp::colorEvent);
    GuiWidget *innerWidget = panel.addWidget("inner widget");
    innerWidget->addButton("button", &button);
    innerWidget->addToggle("another toggle", this, &ofApp::panelToggleEvent);
    GuiMenu *menu2 = innerWidget->addMenu("mc menu", this, &ofApp::multiChoiceMenuSelect, true);
    menu2->addToggle("kamusta mundo");
    menu2->addToggle("xin chao the gioi");
    menu2->addToggle("namaste varlda");
    menu2->addToggle("suesday piphoplok");
    menu2->addToggle("suwati lok");
    panel.addSlider("float slider", &floatSlider, 0.0f, 10.0f);
    panel.addSlider("int slider", &intSlider, 5, 12);
    panel.addSlider("double slider", &doubleSlider, 5.0, 23.0);
    panel.addTextBox("my text", &textValue);
    
    
    
    // initialize variables.
    textValue = "hello world";
    color2.set(0.5, 0.2, 0.9);
    intSlider = 8;
    floatSlider = 8.5;
    rangeLow = 2.0;
    rangeHigh = 6.0;
    
    
    
    v = new Parameter<ofVec4f>("vec4", &vec4slider, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    v2 = new Parameter<ofVec4f>("vec4a", &vec4slider, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));

    
    osc.setupSender("localhost", 12346);
    osc.setupReceiver(12345);
    
    //osc.addParameterToSender(v);
    //osc.addParameterToReceiver(v2);
    
    //osc.addParametersToSender(panel.getParameters());

    vector<ParameterBase*> pm = panel.getParameters();
    for (auto p : pm) {
        cout << p->getName() << " ---- " << p->getOscAddress() << endl;
    }
    
    
    osc.addParameters(panel.getName(), panel.getParameters());
    osc.addParameters(widget.getName(), widget.getParameters());
    
    osc.addParameter(v);
    osc.addParameter(v2);
    
    osc.setPosition(450, 10);

}

//--------------------------------------------------------------
void ofApp::update(){

    osc.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
}

//--------------------------------------------------------------
void ofApp::buttonEvent(GuiElementEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "button event: " + e.name + " = " + ofToString(e.value));
}

//--------------------------------------------------------------
void ofApp::menuSelect(GuiElementEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "menu selection event: " + e.name + " = " + ofToString(e.value));
}

//--------------------------------------------------------------
void ofApp::sliderEvent(GuiElementEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "slider event: " + e.name + " = " + ofToString(e.value));
}

//--------------------------------------------------------------
void ofApp::colorEvent(GuiElementEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "color event: " + e.name + " = " + ofToString(e.value));
}

//--------------------------------------------------------------
void ofApp::panelToggleEvent(GuiElementEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "toggle event: " + e.name + " = " + ofToString(e.value));
}

//--------------------------------------------------------------
void ofApp::multiChoiceMenuSelect(GuiElementEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "panel menu event: " + e.name + " = " + ofToString(e.value));
}

