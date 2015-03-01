#include "ofApp.h"


void ofApp::setup()
{
    // Toggles & buttons

    toggle = new GuiToggle("toggle bgColor", &myToggle);
    toggle->setPosition(10, 10);
    
    toggleNotify = new GuiToggle("toggle + notify", this, &ofApp::toggleNotifier);
    toggleNotify->setRectangle(10, 50, 130, 30);
    
    button = new GuiButton("a button", this, &ofApp::buttonNotifier);
    button->setPosition(10, 105);
    
    // int + float sliders
    
    intSlider = new GuiSlider<int>("int slider", &intValue, 5, 10);
    intSlider->setRectangle(ofRectangle(300, 10, 200, 40));
    
    floatSlider = new GuiSlider<float>("float slider", &floatValue, 0.1, 0.5);
    floatSlider->setPosition(300, 70);
    
    // double slider with notifier
    
    doubleSlider = new GuiSlider<double>("double slider + notify", &doubleValue, 4.0, 9.2, this, &ofApp::sliderNotifier);
    doubleSlider->setRectangle(ofRectangle(300, 100, 200, 40));

    // Range slider
    
    rangeFloatSlider = new GuiRangeSlider<float>("range floats", &rangeLow, &rangeHigh, 0.0f, 10.0f);
    rangeFloatSlider->setRectangle(640, 30, 200, 20);
    
    // 2d pad
    
    pad = new Gui2dPad("2d pad", &padValue, ofPoint(0, 0), ofPoint(10, 20));
    pad->setRectangle(620, 420, 180, 180);

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
}

void ofApp::update()
{
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
}

void ofApp::toggleNotifier(GuiElementEventArgs & s)
{
    cout << "toggle notifier ! " << s.name << " : " << s.value << endl;
}

void ofApp::buttonNotifier(GuiElementEventArgs & s)
{
    cout << "button notifier ! " << s.name << " : " << s.value << endl;
}

void ofApp::sliderNotifier(GuiElementEventArgs & s)
{
    cout << "slider "<<s.name<< " : " << s.value << endl;
}

void ofApp::menuNotifier(GuiElementEventArgs & s)
{
    cout << "menu select:: "<<s.name<< " : " << s.value << endl;
}

void ofApp::menuNotifierMultiChoice(GuiElementEventArgs & s)
{
    cout << "menu (multiple choice) select:: "<< s.name << " : " << s.value << endl;
}

ofApp::~ofApp()
{
    // don't forget to clean up
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
}
