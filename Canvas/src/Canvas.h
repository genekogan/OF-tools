#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "Shader.h"
#include "Control.h"



#include "Modifier.h"
#include "ShaderModifier.h"
#include "PostGlitch.h"
#include "PostProcessing.h"
#include "PostFX.h"




class Canvas
{
public:
    
    void setup(string name, int width, int height)
    {
        this->name = name;
        this->width = width;
        this->height = height;
        creator = NULL;
        
        panel.setName(name);
        menuCreator = panel.addMenu("Creator", this, &Canvas::selectCreator);

        createNewFbo();
    }
    
    void selectCreator(GuiElementEventArgs &e)
    {
        if (creator != NULL)
        {
//            panel.removeElement(creator->getControl().getName());

            //creator->getControl().setAutoDraw(false);
            //creator->getControl().setAutoUpdate(false);
          
            creator->getControl().setActive(false);
        }
        
        creator = creators[e.name];
        
        creator->getControl().setActive(true);
        
        //creator->getControl().setAutoDraw(true);
        //creator->getControl().setAutoUpdate(true);
        
        

    }
    
    void update()
    {
        if (creator == NULL)    return;
        
        creator->update();
        
        fbo[0].begin();
        creator->draw(0, 0);
        fbo[0].end();
        
        for (int i = 1; i < fbo.size(); i++)
        {
            fbo[i].begin();
            modifiers[i-1]->render(&fbo[i-1]);
            fbo[i].end();
        }
    }
    
    void draw(int x, int y)
    {
        fbo[fbo.size()-1].draw(x, y);
    }
    
    void addCreator(Scene *creator)
    {
        creators[creator->getName()] = creator;
        creator->setup(width, height, true);
        
        creator->getControl().setAutoDraw(false);
        creator->getControl().setAutoUpdate(false);
        
        
        panel.addWidget(&creator->getControl());
        
        creator->getControl().setActive(false);
        
        
        
        //panel.addBoundWidget(&creator->getControl());

        menuCreator->addToggle(creator->getName());
        
        if (creators.size() == 1) {
            GuiElementEventArgs evt(creator->getName(), 0, 0);
            selectCreator(evt);
            //this->creator = creators[creator->getName()];
        }
    }
    
    void addModifier()
    {
        Modifier *modifier = new ShaderModifier();
        modifier->setup(width, height);
        
        if (modifiers.size() > 0) {
            //modifiers[modifiers.size()-1]->shader->getControl().addBoundWidget(&modifier->shader->getControl());
            modifiers[modifiers.size()-1]->getControl().addBoundWidget(&modifier->getControl());
        }
        else {
            panel.addBoundWidget(&modifier->getControl());
            //creator->getControl().addBoundWidget(&modifier->getControl());
        }
        
        modifiers.push_back(modifier);
        createNewFbo();
    }
    
    void addPostGlitch()
    {
        Modifier *modifier = new PostGlitch();
        modifier->setup(width, height);
        
        if (modifiers.size() > 0) {
            //modifiers[modifiers.size()-1]->shader->getControl().addBoundWidget(&modifier->shader->getControl());
            modifiers[modifiers.size()-1]->getControl().addBoundWidget(&modifier->getControl());
        }
        else {
            //panel.addBoundWidget(&modifier->shader->getControl());
            creator->getControl().addBoundWidget(&modifier->getControl());
        }
        
        modifiers.push_back(modifier);
        createNewFbo();
    }

    void addPostProcessing()
    {
        Modifier *modifier = new PostProcessingMod();
        modifier->setup(width, height);
        
        if (modifiers.size() > 0) {
            //modifiers[modifiers.size()-1]->shader->getControl().addBoundWidget(&modifier->shader->getControl());
            modifiers[modifiers.size()-1]->getControl().addBoundWidget(&modifier->getControl());
        }
        else {
            //panel.addBoundWidget(&modifier->shader->getControl());
            creator->getControl().addBoundWidget(&modifier->getControl());
        }
        
        modifiers.push_back(modifier);
        createNewFbo();
    }

    void addPostFX()
    {
        Modifier *modifier = new PostFX();
        modifier->setup(width, height);
        
        if (modifiers.size() > 0) {
            //modifiers[modifiers.size()-1]->shader->getControl().addBoundWidget(&modifier->shader->getControl());
            modifiers[modifiers.size()-1]->getControl().addBoundWidget(&modifier->getControl());
        }
        else {
            //panel.addBoundWidget(&modifier->shader->getControl());
            creator->getControl().addBoundWidget(&modifier->getControl());
        }
        
        modifiers.push_back(modifier);
        createNewFbo();
    }

    
    void createNewFbo()
    {
        ofFbo newFbo;
        newFbo.allocate(width, height);
        newFbo.begin();
        ofClear(0, 0);
        newFbo.end();
        fbo.push_back(newFbo);
    }
    
    string name;
    int width, height;
    GuiPanel panel;
    GuiMenu *menuCreator;

    map<string, Scene*> creators;
    vector<Modifier*> modifiers;
    vector<ofFbo> fbo;
    
    Scene *creator;
};

