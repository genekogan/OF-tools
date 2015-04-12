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
#include "FilterLibrary.h"

#include "ProjectionMapping.h"



class Canvas
{
public:
    enum CanvasView { CONDENSED, EXPANDED_ATTACHED, EXPANDED_DETACHED };
    
    ~Canvas();
    void setup(string name, int width, int height);
    void update();
    void draw(int x, int y);
    
    void addCreator(Scene *creator);
    void selectCreator(string name);

    Modifier * addShaderModifier() {return addModifier("Shader");}
    Modifier * addPostProcessing() {return addModifier("PostProcessing");}
    Modifier * addPostFX() {return addModifier("PostFX");}
    Modifier * addPostGlitch() {return addModifier("PostGlitch");}
    Modifier * addFilterLibrary() {return addModifier("FilterLib");}

    void loadPreset(string path);
    void savePreset(string name);
    
    void setGuiDisplayMode(CanvasView canvasView);
    void toggleGuiVisible();
    
    GuiPanel & getControl() {return metaPanel;}
    GuiPanel & getCreatorPanel() {return creatorPanel;}
    
private:
    
    void createNewFbo();
    void checkFboSize();
    void updatePanels();
    void refreshPresetMenu();
    void resetGuiPositions();
    void switchCreator(string name);

    Modifier * addModifier(string type);
    Modifier * initializeModifier(Modifier *modifier, bool toSetup);

    void eventViewLayer(GuiMenuEventArgs &evt);
    void eventSelectCreator(GuiMenuEventArgs &e);
    void eventCreateModifier(GuiMenuEventArgs &evt);
    void eventRemoveModifier(GuiMenuEventArgs &evt);
    void eventLoadPreset(GuiMenuEventArgs &evt);
    void eventSavePreset(GuiButtonEventArgs &evt);
    void eventToggleMapping(GuiButtonEventArgs &evt);
    void eventToggleMappingCalibration(GuiButtonEventArgs &evt);
    void eventSaveCalibration(GuiButtonEventArgs &evt);
    void eventLoadCalibration(GuiButtonEventArgs &evt);
    
    void setFromXml(ofXml &xml);
    void getXml(ofXml &xml);
    
    CanvasView canvasView;
    bool guiVisible;
    
    string name;
    int width, height;

    map<string, Scene*> creators;
    Scene *creator;
    vector<Modifier*> availableModifiers;
    vector<Modifier*> modifiers;
    vector<ofFbo> fbo;

    GuiPanel creatorPanel, metaPanel;
    GuiMenu *menuCreator;
    GuiMenu *menuLoadPreset;
    GuiMenu *menuAddMod;
    GuiMenu *menuRemove;
    GuiMenu *menuView;
    GuiWidget *widgetMapping;
    
    ProjectionMapping mapping;
    bool pmEnabled;
    bool pmCalibrating;
    
    ofFbo mask;
    ofFbo maskResult;
    ofShader maskShader;
    bool maskEnabled;
};

