#include "MantaAudioUnitController.h"


MantaAudioUnitController::MantaAudioUnitController() : MantaController() {
    
    ofAddListener(padEvent, this, &MantaAudioUnitController::PadEvent);
    ofAddListener(sliderEvent, this, &MantaAudioUnitController::SliderEvent);
    ofAddListener(buttonEvent, this, &MantaAudioUnitController::ButtonEvent);
    ofAddListener(padVelocityEvent, this, &MantaAudioUnitController::PadVelocityEvent);
    ofAddListener(buttonVelocityEvent, this, &MantaAudioUnitController::ButtonVelocityEvent);

    //   ofAddListener(clickE, <#ListenerClass *listener#>, <#void (ListenerClass::*listenerMethod)(const void *, ArgumentsType &)#>)
 
    panel = new GuiPanel();
    panel->setName("Manta + Audio Unit");
    panel->setPosition(750, 100);
}

MantaAudioUnitController::~MantaAudioUnitController()
{
    
}

void MantaAudioUnitController::setup(AudioUnitInstrument *instrument)
{
    this->instrument = instrument;
    MantaController::setup();
    
    panel->disableControlRow();
    panel->setName("Manta -> Parameter");
    bActiveMantaElement = panel->addTextBox("None", new string("none"));
    bActiveParameter = panel->addTextBox("Parameter", new string("none"));
    tActiveRange = panel->addRangeSlider("Map", new float(0.25f), new float(0.75f), 0.0f, 1.0f, this, &MantaAudioUnitController::eventSetRange);
    parameterGroupSelector = panel->addMenu("Select group", this, &MantaAudioUnitController::selectParameterMapGroup);
    
    GuiMenu *wGroup;
    int clumpID = -1;
    vector<AudioUnitInstrumentParameter*> parameters = instrument->getParameters();
    for (auto p : parameters)
    {
        if (p->clumpId != clumpID)
        {
            if (clumpID != -1)  wGroup->setCollapsed(true);
            clumpID = p->clumpId;
            wGroup = panel->addMenu(p->name, this, &MantaAudioUnitController::selectParameterMapEntry, true, false);
            wGroup->setActive(false);
            parameterGroupSelector->addToggle(wGroup->getName());
        }
        GuiToggle *toggle = wGroup->addToggle(p->name, new bool(false));
        allParameters[toggle] = new MantaParameterMapping(p, toggle);
        subMenus[p->name] = wGroup;
    }
    wGroup->setCollapsed(true);
}

void MantaAudioUnitController::selectParameterMapGroup(GuiMenuEventArgs &evt)
{
    map<string, GuiMenu*>::iterator it = subMenus.begin();
    for (; it != subMenus.end(); ++it) {
        it->second->setActive(false);
    }
    subMenus[evt.toggle->getName()]->setActive(true);
    subMenus[evt.toggle->getName()]->setCollapsed(false);
}

void MantaAudioUnitController::selectParameterMapEntry(GuiMenuEventArgs &evt)
{
    bActiveParameter->setValue(evt.toggle->getName());
    selectedInstrument = allParameters[evt.toggle];
    if (selectedInstrument->mantaIdx != -1) {
        bActiveMantaElement->setValue("Pad "+ofToString(selectedInstrument->mantaIdx));
    }
    assignArmed = true;
}

void MantaAudioUnitController::mapPadToParameter(int idx, AudioUnitInstrumentParameter* parameter)
{
    
    
    
    
    
}

void MantaAudioUnitController::PadEvent(ofxMantaEvent & evt)
{
    if (assignArmed)
    {
        parameterMap[evt.id] = selectedInstrument;
        selectedInstrument->mantaIdx = evt.id;
        bActiveMantaElement->setValue("Pad "+ofToString(evt.id));
        assignArmed = false;
    }
    else
    {
        if (parameterMap.count(evt.id) != 0) {
            parameterMap[evt.id]->parameter->setValue(ofMap(evt.value, 0, 196, parameterMap[evt.id]->min, parameterMap[evt.id]->max));
        }
    }
}

void MantaAudioUnitController::SliderEvent(ofxMantaEvent & evt)
{

}

void MantaAudioUnitController::ButtonEvent(ofxMantaEvent & evt)
{

}

void MantaAudioUnitController::PadVelocityEvent(ofxMantaEvent & evt)
{
    if (midiMap.count(evt.id) != 0)
    {
        int note = 120; // midiMap[evt.id];
        if (evt.value == -1) {
            instrument->getAudioUnitSampler()->midiNoteOff(note, 120);
        }
        else {
            instrument->getAudioUnitSampler()->midiNoteOn(note, evt.value);
        }
    }
}

void MantaAudioUnitController::ButtonVelocityEvent(ofxMantaEvent & evt)
{

}
