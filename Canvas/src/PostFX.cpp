//
//  PostFX.cpp
//  template_simple
//
//  Created by Eugene Kogan on 3/23/15.
//
//

#include "PostFX.h"


void PostFX::setup(int width, int height)
{
    Modifier::setup(width, height);
    
    bloom.allocate(width, height);
    gaussianBlur.allocate(width, height);
    bokeh.allocate(width, height);
    glow.allocate(width, height);
    blur.allocate(width, height);
    median.allocate(width, height);
    oldtv.allocate(width, height);
    inverse.allocate(width, height);
    barrelChromaAb.allocate(width, height);
    chromaAb.allocate(width, height);
    chromaGlitch.allocate(width, height);
    grayscale.allocate(width, height);
    
    
    dir.allowExt("cube");
	dir.listDir("LUTs/");
	dir.sort();
    
    lutChoices.push_back("None");
    for (int i=0; i<dir.size(); i++) {
        lutChoices.push_back(dir.getPath(i));
    }
    string fxStr[] = { "none", "bloom", "gaussian_blur", "bokeh",
        "glow", "blur", "median", "oldtv", "inverse",
        "barrelChromaAb", "chromaAb", "chromaGlitch", "grayscale" };
    vector<string> fxChoices(fxStr, fxStr + sizeof(fxStr) / sizeof(fxStr[0]));
    
    
    for (int i=0; i<lutChoices.size(); i++) {
        ofxLUT *newLut = new ofxLUT();
        newLut->allocate(width, height);
        newLut->loadLUT(lutChoices[i]);
        lut.push_back(newLut);
    }
    
    active = 0;
    
    panel.setName("postFx");
    panel.addMenu("LUT", lutChoices, this, &PostFX::chooseLut);
    panel.addMenu("Fx", fxChoices, this, &PostFX::chooseFx);
    
    panel.addSlider("radius", &radius, -15.0f, 15.0f);
    panel.addSlider("fade", &fade, -1.0f, 1.0f);
    
    GuiElementEventArgs args("None", 0, 0);
    chooseFx(args);
    
    pass = true;

    
}

void PostFX::chooseLut(GuiElementEventArgs &e) {
    if (e.name == "None") {
        pass = true;
    }
    else {
        for (int i=0; i<lutChoices.size(); i++) {
            if (lutChoices[i] == e.name) {
                active = i;
            }
        }
        pass = false;
    }
}

//-----------
void PostFX::chooseFx(GuiElementEventArgs &e) {
    selection = e.name;
}

