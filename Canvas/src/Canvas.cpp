#include "Canvas.h"


Canvas::~Canvas()
{
    // buggy..... needs fixing
    
    return;
    
    
    // delete modifiers
    modifiers.clear();
    vector<Modifier*>::iterator it = availableModifiers.begin();
    while (it != availableModifiers.end())
    {
        (*it)->setActive(false);
        delete *it;
        availableModifiers.erase(it);
    }
    
    // delete creators
    creator = NULL;
    map<string, Scene*>::iterator itc = creators.begin();
    while (itc != creators.end())
    {
        delete itc->second;
        creators.erase(itc);
    }
}

void Canvas::setup(string name, int width, int height)
{
    this->name = name;
    this->width = width;
    this->height = height;
    this->creator = NULL;

    createNewFbo();
    mapping.addQuad(width, height);
    
    // Control panel
    metaPanel.disableControlRow();
    metaPanel.setName("Canvas");
    metaPanel.setPosition(0, 0);

    menuView = metaPanel.addMenu("View modifier", this, &Canvas::eventViewLayer);
    menuView->addToggle("Creator");

    menuAddMod = metaPanel.addMenu("Add modifier", this, &Canvas::eventCreateModifier);
    menuAddMod->addToggle("Shader");
    menuAddMod->addToggle("PostProcessing");
    menuAddMod->addToggle("PostFx");
    menuAddMod->addToggle("PostGlitch");
    
    menuRemove = metaPanel.addMenu("Remove modifier", this, &Canvas::eventRemoveModifier);
    
    menuLoadPreset = metaPanel.addMenu("Load Preset", this, &Canvas::eventLoadPreset);
    metaPanel.addButton("Save Preset", this, &Canvas::eventSavePreset);
    refreshPresetMenu();
    
    widgetMapping = metaPanel.addWidget("Mapping");
    widgetMapping->addToggle("Enabled", &pmEnabled, this, &Canvas::eventToggleMapping);
    widgetMapping->addToggle("Calibrate", &pmCalibrating, this, &Canvas::eventToggleMappingCalibration)->setActive(pmEnabled);
    widgetMapping->addButton("Save", this, &Canvas::eventSaveCalibration)->setActive(pmEnabled);
    widgetMapping->addButton("Load", this, &Canvas::eventLoadCalibration)->setActive(pmEnabled);
    metaPanel.addToggle("Mask enabled", &maskEnabled);
    
    // Creator panel
    creatorPanel.setName("Creator");
    menuCreator = creatorPanel.addMenu("Creator", this, &Canvas::eventSelectCreator);
    
    setGuiDisplayMode(CONDENSED);
    
    
    
    /////////////
    
    maskShader.load("shaders_texture/standard.vert", "shaders_texture/alphamask.frag");
    mask.allocate(width,height);
    maskResult.allocate(width,height);
    maskResult.begin();
    ofClear(0, 0);
    maskResult.end();
}

void Canvas::update()
{
    checkFboSize();
    updatePanels();
    
    if (creator == NULL)    return;
    creator->update();
    
    fbo[0].begin();
    creator->draw(0, 0);
    fbo[0].end();
    
    for (int i = 0; i < modifiers.size(); i++)
    {
        fbo[i+1].begin();
        modifiers[i]->render(&fbo[i]);
        fbo[i+1].end();
    }
    
    if (maskEnabled)
    {
        mask.begin();
        ofClear(0, 0);
        ofSetColor(255);
        ofBeginShape();
        for (int i=0; i<150; i++)
        {
            float ang = ofMap(i, 0, 150, 0, TWO_PI);
            float rad = ofMap(ofNoise(i*0.1, ofGetElapsedTimef()), 0, 1, 200, 500);
            ofVertex(500 + rad * cos(ang), 400 + rad * sin(ang));
        }
        ofEndShape();
        mask.end();
    }
}

void Canvas::updatePanels()
{
    if (!creatorPanel.getAutoUpdate()) {
        creatorPanel.update();
    }
    for (auto m : modifiers)
    {
        if (!m->getControl().getAutoUpdate()) {
            m->getControl().update();
        }
    }
}

void Canvas::draw(int x, int y)
{
    if (pmEnabled)
    {
        mapping.begin(0);
        if (maskEnabled)
        {
            maskResult.begin();
            ofClear(0, 0);
            maskShader.begin();
            maskShader.setUniformTexture("maskTex", mask.getTextureReference(), 1);

            fbo[fbo.size()-1].draw(0, 0);
            
            maskShader.end();
            maskResult.end();

            maskResult.draw(0, 0);
        }
        else
        {
            fbo[fbo.size()-1].draw(0, 0);
        }
        mapping.end(0);
    }
    else
    {
        fbo[fbo.size()-1].draw(x, y);
    }
}

void Canvas::addCreator(Scene *creator)
{
    creators[creator->getName()] = creator;
    creator->setup(width, height, true);
    creator->getControl().setAutoDraw(false);
    creator->getControl().setAutoUpdate(false);
    creator->getControl().setActive(false);
    creator->getControl().disableControlRow();
    creatorPanel.addWidget(&creator->getControl());
    menuCreator->addToggle(creator->getName());
    if (creators.size() == 1) {
        menuCreator->setToggle(creator->getName(), true);
    }
}

Modifier * Canvas::addModifier(string type)
{
    vector<Modifier*>::iterator it = availableModifiers.begin();
    while (it != availableModifiers.end())
    {
        if (((*it)->getType() == type) && !(*it)->getActive()) {
            return initializeModifier(*it, false);
        }
        else {
            ++it;
        }
    }
    if      (type == "Shader")          return initializeModifier(new ShaderModifier(), true);
    else if (type == "PostProcessing")  return initializeModifier(new PostProcessingMod(), true);
    else if (type == "PostFX")          return initializeModifier(new PostFX(), true);
    else if (type == "PostGlitch")      return initializeModifier(new PostGlitch(), true);
}

Modifier * Canvas::initializeModifier(Modifier *modifier, bool toSetup)
{
    if (toSetup)
    {
        int index = 0;
        for (auto m : availableModifiers) {
            index += (m->getType() == modifier->getType());
        }
        modifier->setIndex(index);
        modifier->setup(width, height);
        availableModifiers.push_back(modifier);
    }
    modifiers.push_back(modifier);
    modifier->setActive(true);

    resetGuiPositions();
    menuRemove->addToggle(modifier->getDisplayText());
    menuView->addToggle(modifier->getDisplayText())->setValue(true, true);;
    
    return modifier;
}

void Canvas::selectCreator(string name)
{
    if (creators.count(name) != 0) {
        menuCreator->setToggle(name, true);
    }
}

void Canvas::switchCreator(string name)
{
    if (creators.count(name) != 0)
    {
        if (creator != NULL) {
            creator->getControl().setActive(false);
        }
        creator = creators[name];
        creator->getControl().setActive(true);
    }
}

void Canvas::eventToggleMapping(GuiButtonEventArgs &evt)
{
    widgetMapping->getElement("Calibrate")->setActive(pmEnabled);
    widgetMapping->getElement("Save")->setActive(pmEnabled);
    widgetMapping->getElement("Load")->setActive(pmEnabled);
}

void Canvas::eventToggleMappingCalibration(GuiButtonEventArgs &evt)
{
    mapping.setDebug(pmCalibrating);
    mapping.setMouseEventsAuto(pmCalibrating);
}

void Canvas::eventSaveCalibration(GuiButtonEventArgs &evt)
{
    mapping.savePreset("Mapping.xml");
}

void Canvas::eventLoadCalibration(GuiButtonEventArgs &evt)
{
    mapping.loadPreset("Mapping.xml");
}

void Canvas::eventCreateModifier(GuiMenuEventArgs &evt)
{
    if      (evt.index == 0) addShaderModifier();
    else if (evt.index == 1) addPostProcessing();
    else if (evt.index == 2) addPostFX();
    else if (evt.index == 3) addPostGlitch();
    evt.toggle->setValue(false, false);
}

void Canvas::eventRemoveModifier(GuiMenuEventArgs &evt)
{
    menuRemove->removeToggle(evt.toggle->getName());
    menuView->removeToggle(evt.toggle->getName());
    modifiers[evt.index]->setActive(false);
    modifiers.erase(modifiers.begin() + evt.index);
    resetGuiPositions();
}

void Canvas::eventLoadPreset(GuiMenuEventArgs &evt)
{
    loadPreset("presets/"+evt.toggle->getName());
}

void Canvas::eventSavePreset(GuiButtonEventArgs &evt)
{
    string name = ofSystemTextBoxDialog("name preset");
    savePreset(name);
}

void Canvas::eventSelectCreator(GuiMenuEventArgs &e)
{
    switchCreator(e.toggle->getName());
}

void Canvas::eventViewLayer(GuiMenuEventArgs &evt)
{
    if (canvasView != CONDENSED)    return;

    creatorPanel.setAutoUpdate(false);
    creatorPanel.setAutoDraw(false);
    for (auto m : modifiers)
    {
        m->getControl().setAutoUpdate(false);
        m->getControl().setAutoDraw(false);
    }
    if (evt.toggle->getName() == "Creator")
    {
        creatorPanel.setAutoUpdate(true);
        creatorPanel.setAutoDraw(true);
    }
    else
    {
        modifiers[evt.index-1]->getControl().setAutoUpdate(true);
        modifiers[evt.index-1]->getControl().setAutoDraw(true);
    }
}

void Canvas::createNewFbo()
{
    int idx = fbo.size();
    fbo.resize(fbo.size() + 1);
    fbo[idx].allocate(width, height);
    fbo[idx].begin();
    ofClear(0, 0);
    fbo[idx].end();
}

void Canvas::checkFboSize()
{
    while (modifiers.size() + 1 > fbo.size()) {
        createNewFbo();
    }
    if (fbo.size() > modifiers.size() + 1) {
        fbo.resize(modifiers.size() + 1);
    }
}

void Canvas::refreshPresetMenu()
{
    ofDirectory dir;
    dir.allowExt("xml");
    dir.open("presets/");
    dir.listDir();
    vector<string> presetList;
    for(int i = 0; i < dir.size(); i++)
    {
        presetList.push_back(dir.getName(i));
        menuLoadPreset->addToggle(dir.getName(i));
    }
}

void Canvas::resetGuiPositions()
{
    menuView->setActive(canvasView == CONDENSED);
    
    if (canvasView == CONDENSED)
    {
        metaPanel.attachWidget(&creatorPanel);
        creatorPanel.setAutoUpdate(false);
        creatorPanel.setAutoDraw(false);
        for (auto m : modifiers)
        {
            metaPanel.attachWidget(&m->getControl());
            m->getControl().setAutoUpdate(false);
            m->getControl().setAutoDraw(false);
        }
    }
    else if (canvasView == EXPANDED_ATTACHED)
    {
        metaPanel.attachWidget(&creatorPanel);
        creatorPanel.setAutoUpdate(true);
        creatorPanel.setAutoDraw(true);
        if (modifiers.size() > 0)
        {
            creatorPanel.attachWidget(&modifiers[0]->getControl());
            for (int i=1; i<modifiers.size(); i++) {
                modifiers[i-1]->getControl().attachWidget(&modifiers[i]->getControl());
            }
        }
    }
    else if (canvasView == EXPANDED_DETACHED)
    {
        creatorPanel.setPosition(200, 0);
        creatorPanel.setAutoUpdate(true);
        creatorPanel.setAutoDraw(true);
        if (modifiers.size() > 0)
        {
            modifiers[0]->getControl().setPosition(400, 0);
            for (int i=1; i<modifiers.size(); i++) {
                modifiers[i]->getControl().setPosition((i+2) * 200, 0);
            }
        }
    }
}

void Canvas::loadPreset(string path)
{
    ofXml xml;
    xml.load(path);
    setFromXml(xml);
    resetGuiPositions();
}

void Canvas::savePreset(string name)
{
    ofXml xml;
    getXml(xml);
    xml.save("presets/"+name+".xml");
    menuLoadPreset->addToggle(name+".xml");
}

void Canvas::setFromXml(ofXml &xml)
{
    xml.setTo("Preset");
    xml.setTo("Creator");
    selectCreator(xml.getValue<string>("Selection"));
    creatorPanel.setFromXml(xml);
    xml.setToParent();
    
    for (auto m : modifiers) {
        m->setActive(false);
    }
    modifiers.clear();
    menuRemove->clearToggles();
    menuView->clearToggles();
    menuView->addToggle("Creator");
    
    xml.setTo("Modifiers");
    if (xml.exists("Modifier[0]"))
    {
        xml.setTo("Modifier[0]");
        do {
            string type = xml.getValue<string>("Type");
            Modifier *modifier;
            if      (type == "Shader")          modifier = addShaderModifier();
            else if (type == "PostProcessing")  modifier = addPostProcessing();
            else if (type == "PostFX")          modifier = addPostFX();
            else if (type == "PostGlitch")      modifier = addPostGlitch();
            modifier->getControl().setFromXml(xml);
        }
        while(xml.setToSibling());
    }
    xml.setToParent();
    
    xml.setTo("Mapping");
    pmEnabled = xml.getValue<bool>("Enabled");
    if (xml.exists("Quads")) {
        mapping.setFromXml(xml);
    }
    
    xml.setToParent();
    xml.setToParent();
    
    GuiButtonEventArgs args(NULL, false);
    eventToggleMapping(args);
}

void Canvas::getXml(ofXml &xml)
{
    xml.addChild("Preset");
    xml.setTo("Preset");
    
    ofXml xmlCreator;
    xmlCreator.addChild("Creator");
    xmlCreator.setTo("Creator");
    xmlCreator.addValue("Selection", creator->getName());
    creatorPanel.getXml(xmlCreator);
    xml.addXml(xmlCreator);
    
    ofXml xmlModifiers;
    xmlModifiers.addChild("Modifiers");
    xmlModifiers.setTo("Modifiers");
    for (int m = 0; m < modifiers.size(); m++)
    {
        ofXml xmlModifier;
        xmlModifier.addChild("Modifier");
        xmlModifier.setTo("Modifier");
        xmlModifier.addValue("Type", modifiers[m]->getType());
        modifiers[m]->getControl().getXml(xmlModifier);
        xmlModifiers.addXml(xmlModifier);
    }
    xml.addXml(xmlModifiers);

    ofXml xmlMapping;
    xmlMapping.addChild("Mapping");
    xmlMapping.setTo("Mapping");
    xmlMapping.addValue("Enabled", pmEnabled);
    mapping.getXml(xmlMapping);
    xml.addXml(xmlMapping);
    
    xml.setToParent();
}

void Canvas::setGuiDisplayMode(CanvasView canvasView)
{
    this->canvasView = canvasView;
    resetGuiPositions();
}

void Canvas::toggleGuiVisible()
{
    guiVisible = !guiVisible;
    metaPanel.setAutoUpdate(guiVisible);
    metaPanel.setAutoDraw(guiVisible);
    creatorPanel.setAutoUpdate(guiVisible);
    creatorPanel.setAutoDraw(guiVisible);
    for (auto m : modifiers)
    {
        m->getControl().setAutoUpdate(guiVisible);
        m->getControl().setAutoDraw(guiVisible);
    }
}
