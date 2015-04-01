#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(5);
    
    // Toggles & buttons

    toggle = new GuiToggle("toggle bgColor", &myToggle);
    toggle->setPosition(10, 10);
    
    toggleNotify = new GuiToggle("toggle + notify", this, &ofApp::toggleNotifier);
    toggleNotify->setPosition(10, 40);
    toggleNotify->setSize(130, 35);
    
    
    button = new GuiButton("a button", this, &ofApp::buttonNotifier);
    button->setPosition(10, 105);

    
    // int + float sliders
    
    intSlider = new GuiSlider<int>("int slider", &intValue, 5, 10);
    intSlider->setSize(200, 40);
    intSlider->setPosition(300, 25);
    
    floatSlider = new GuiSlider<float>("float slider", &floatValue, 0.1, 0.5);
    floatSlider->setPosition(300, 70);
    
    // double slider with notifier
    
    doubleSlider = new GuiSlider<double>("double slider + notify", &doubleValue, 4.0, 9.2, this, &ofApp::sliderNotifier);
    doubleSlider->setPosition(300, 100);
    doubleSlider->setWidth(200);
    doubleSlider->setHeight(40);


    // Range slider
    
    rangeFloatSlider = new GuiRangeSlider<float>("range floats", &rangeLow, &rangeHigh, 0.0f, 10.0f);
    rangeFloatSlider->setPosition(640, 30);
    
    
    // Multi Sliders
    
    vec3Slider = new GuiMultiSlider<ofVec3f>("vec3 slider", &vec3val, ofVec3f(0, 0, 0), ofVec3f(2,5,3), this, &ofApp::vec3SliderNotifier);
    vec3Slider->setPosition(640, 70);
    vec3Slider->setCollapsible(true);

    
    pad = new Gui2dPad("hello", &mypt, ofPoint(0, 0), ofPoint(5, 3));
    pad->setPosition(410, 540);
    pad->setCollapsible(true);
    //pad->setWidth(500);
    //pad->setHeight(150);
    
    
    widget.setName("my widget");
    widget.setPosition(640, 250);
    /*
    widget.addSlider();
    widget.addToggle();
    widget.addMultiSlider();
    widget.addColor();
    widget.addMenu();
    widget.addMultiRangeSlider();
    */
    
    
/*
    // 2d pad
    
    pad = new Gui2dPad("2d pad", &padValue, ofPoint(0, 0), ofPoint(10, 20));
    pad->addPoint(&padValue2);
    pad->addPoint();
    pad->setRectangle(620, 420, 320, 320);
¨
    // Color picker
    
    colorPicker = new GuiColor("bg color", &bgColor);
    colorPicker->setRectangle(620, 200, 220, 40);

    // Menus
    
    vector<string> menuItems;
    menuItems.push_back("larry");
    menuItems.push_back("moe");
    menuItems.push_back("curly");
    
    menu = new GuiMenu("my menu", menuItems, this, &ofApp::menuNotifier);
    menu->setPosition(20, 200);
    menu->setAutoClose(true);

    menuMultiChoice = new GuiMenu("my menu mc", this, &ofApp::menuNotifierMultiChoice);
    menuMultiChoice->setPosition(320, 200);
    menuMultiChoice->setMultipleChoice(true);
    menuMultiChoice->addToggle("new york");
    menuMultiChoice->addToggle("los angeles");
    menuMultiChoice->addToggle("chicago");
    menuMultiChoice->addToggle("san franciso");
    menuMultiChoice->addToggle("philadelphia");
    menuMultiChoice->addToggle("boston");
    

    // some default values
    myToggle = true;
    rangeLow = 2.0f;
    rangeHigh = 5.0f;
    bgColor.set(0.5, 0.2, 0.1);
    padValue.set(0.3, 0.7);
 */
}

void ofApp::update()
{
    //cout << ofToString(vec3val) << endl;
    
    //vec3val.y = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 5);
    
    pad->update();
}

void ofApp::draw()
{
    if (myToggle)
    {
        ofBackground(bgColor);
    }
    else {
        ofBackground(100);
    }
    
    ofDrawBitmapString("float value "+ofToString(floatValue)+"\n"+
                       "int value "+ofToString(intValue)+"\n"+
                       "double value "+ofToString(doubleValue)+"\n"+
                       "range "+ofToString(rangeLow)+" - "+ofToString(rangeHigh)+"\n"+
                       "pad "+ofToString(padValue), 20, 400);

    pad->draw();

}

void ofApp::toggleNotifier(GuiButtonEventArgs & s)
{
    cout << "toggle notifier ! " << s.button->getName() << " : " << s.value << endl;
}

void ofApp::buttonNotifier(GuiButtonEventArgs & s)
{
    cout << "button notifier ! " << s.button->getName() << " : " << s.value << endl;
}

void ofApp::sliderNotifier(GuiSliderEventArgs<double> & s)
{
    cout << "slider "<<s.slider->getName()<< " : " << s.value << endl;
}

void ofApp::vec3SliderNotifier(GuiMultiSliderEventArgs<ofVec3f> & s)
{
    cout << "multislider "<<s.slider->getName()<<" "<<ofToString(s.value)<<endl;
}

void ofApp::menuNotifier(GuiButtonEventArgs & s)
{
    cout << "menu select:: "<<s.button->getName()<< " : " << s.value << endl;
}

void ofApp::menuNotifierMultiChoice(GuiButtonEventArgs & s)
{
    cout << "menu (multiple choice) select:: "<< s.button->getName() << " : " << s.value << endl;
}

ofApp::~ofApp()
{
    // don't forget to clean up
    /*
    delete button;
    delete toggle;
    delete toggleNotify;
    delete floatSlider;
    delete intSlider;
    delete doubleSlider;
    delete rangeFloatSlider;
    delete rangeIntSlider;
    delete menu;
    delete menuMultiChoice;
    delete colorPicker;
    delete pad;
     */
}
