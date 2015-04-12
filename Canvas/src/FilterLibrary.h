#pragma once

#include "ofxFilterLibrary.h"

#include "Control.h"
#include "Shader.h"
#include "Modifier.h"



class FilterLibrary : public Modifier
{
public:
    
    FilterLibrary() : Modifier()
    {
        type = "FilterLib";
    }
    
    void setActive(bool active)
    {
        this->active = active;
        panel.setActive(active);
    }
    
    void selectFilter(GuiMenuEventArgs &evt) {
        _currentFilter = evt.index;
    }
    
    void setup(int width, int height);
    void render(ofFbo *fbo);
    
    GuiPanel & getControl();
    
    int                         _currentFilter;
    vector<AbstractFilter *>    _filters;

    GuiPanel panel;
    
    
    
    //ofVideoGrabber              _video;
    
    ofFbo *fboInner;
};
