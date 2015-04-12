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
        
    loadLutLookup();

    active = 0;
    pass = true;
}

void PostFX::loadLutLookup()
{
    ofDirectory dir;
    dir.allowExt("cube");
	dir.listDir("LUTs/");
	dir.sort();
    
    lutChoices.push_back("None");
    for (int i=0; i<dir.size(); i++)    lutChoices.push_back(dir.getPath(i));
    string fxStr[] = { "none", "bloom", "gaussian_blur", "bokeh",
        "glow", "blur", "median", "oldtv", "inverse",
        "barrelChromaAb", "chromaAb", "chromaGlitch", "grayscale" };
    vector<string> fxChoices(fxStr, fxStr + sizeof(fxStr) / sizeof(fxStr[0]));
    
    for (int i=0; i<lutChoices.size(); i++)
    {
        ofxLUT *newLut = new ofxLUT();
        newLut->allocate(width, height);
        newLut->loadLUT(lutChoices[i]);
        lut.push_back(newLut);
    }
    
    panel.setName("postFx");
    GuiMenu *menuLut = panel.addMenu("LUT", lutChoices, this, &PostFX::chooseLut);
    GuiMenu *menuFx = panel.addMenu("Fx", fxChoices, this, &PostFX::chooseFx);
    
    panel.addSlider("radius", &radius, -15.0f, 15.0f);
    panel.addSlider("fade", &fade, -1.0f, 1.0f);
    
    menuLut->setToggle("None", true);
    menuFx->setToggle("none", true);    
}

void PostFX::chooseLut(GuiMenuEventArgs &e)
{
    active = e.index;
    pass = active == 0;
}

void PostFX::setLutLookup(vector<ofxLUT*> lut)
{
    this->lut=lut;
}

void PostFX::chooseFx(GuiMenuEventArgs &e)
{
    selection = e.toggle->getName();
}

void PostFX::render(ofFbo *fbo)
{
    if (!pass)
    {
        lut[active]->setTexture(fbo->getTextureReference());
        lut[active]->update();
    }
    
    if (selection == "none")
    {
        if (pass)   fbo->draw(0, 0);
        else        lut[active]->draw();
    }
    else if (selection == "bloom")
    {
        if (pass)   bloom << fbo->getTextureReference();
        else        bloom << *lut[active];
        bloom.update();
        bloom.draw();
    }
    else if (selection == "gaussian_blur")
    {
        gaussianBlur.setRadius(radius);
        if (pass)   gaussianBlur << fbo->getTextureReference();
        else        gaussianBlur << *lut[active];
        gaussianBlur.update();
        gaussianBlur.draw();
    }
    else if (selection == "bokeh")
    {
        bokeh.setRadius(radius);
        if (pass)   bokeh << fbo->getTextureReference();
        else        bokeh << *lut[active];
        bokeh.update();
        bokeh.draw();
    }
    else if (selection == "glow")
    {
        glow.setRadius(radius);
        if (pass)   glow << fbo->getTextureReference();
        else        glow << *lut[active];
        glow.update();
        glow.draw();
    }
    else if (selection == "blur")
    {
        blur.setFade(fade);
        if (pass)   blur << fbo->getTextureReference();
        else        blur << *lut[active];
        blur.update();
        blur.draw();
    }
    else if (selection == "median")
    {
        if (pass)   median << fbo->getTextureReference();
        else        median << *lut[active];
        median.update();
        median.draw();
    }
    else if (selection == "oldtv")
    {
        if (pass)   oldtv << fbo->getTextureReference();
        else        oldtv << *lut[active];
        oldtv.update();
        oldtv.draw();
    }
    else if (selection == "inverse")
    {
        if (pass)   inverse << fbo->getTextureReference();
        else        inverse << *lut[active];
        inverse.update();
        inverse.draw();
    }
    else if (selection == "barrelChromaAb")
    {
        if (pass)   barrelChromaAb << fbo->getTextureReference();
        else        barrelChromaAb << *lut[active];
        barrelChromaAb.update();
        barrelChromaAb.draw();
    }
    else if (selection == "chromaAb")
    {
        if (pass)   chromaAb << fbo->getTextureReference();
        else        chromaAb << *lut[active];
        chromaAb.update();
        chromaAb.draw();
    }
    else if (selection == "chromaGlitch")
    {
        if (pass)   chromaGlitch << fbo->getTextureReference();
        else        chromaGlitch << *lut[active];
        chromaGlitch.update();
        chromaGlitch.draw();
    }
    else if (selection == "grayscale")
    {
        if (pass)   grayscale << fbo->getTextureReference();
        else        grayscale << *lut[active];
        grayscale.update();
        grayscale.draw();
    }
}
