#pragma once

#include "ofMain.h"
#include "MantaController.h"
#include "AudioUnits.h"



class MantaAudioUnitController : public MantaController
{
public:
    
    struct MantaParameterMapping
    {
        AudioUnitInstrumentParameter *parameter;
        float min, max;
        int mantaIdx;
        GuiToggle *toggle;
        
        MantaParameterMapping(AudioUnitInstrumentParameter *parameter, GuiToggle *toggle)
        {
            this->parameter = parameter;
            this->min = parameter->min;
            this->max = parameter->max;
            this->toggle = toggle;
            mantaIdx = -1;
        }

    };
    
    void blah() {
        
        vector<int> selection = getPadSelection();
        for (auto s : selection) {
            cout << "Select " << s << endl;
            
            midiMap[s] = s;
        }
        
    }
    
    
    MantaAudioUnitController();
    ~MantaAudioUnitController();
    
    void setup(AudioUnitInstrument *instrument);
    
    
    void mapPadToParameter(int idx, AudioUnitInstrumentParameter*);
    
    void selectParameterMapGroup(GuiMenuEventArgs &evt);
    void selectParameterMapEntry(GuiMenuEventArgs &evt);
    
    void eventSetRange(GuiRangeSliderEventArgs<float> &evt) {
        
    }
    
    void PadEvent(ofxMantaEvent & evt);
    void SliderEvent(ofxMantaEvent & evt);
    void ButtonEvent(ofxMantaEvent & evt);
    void PadVelocityEvent(ofxMantaEvent & evt);
    void ButtonVelocityEvent(ofxMantaEvent & evt);

    
protected:
    
    AudioUnitInstrument *instrument;
    

    
    
//    map<GuiToggle*, AudioUnitInstrumentParameter*> allParameters;

    
    
    map<GuiToggle*, MantaParameterMapping*> allParameters;
    
    
    
    
    map<int, MantaParameterMapping*> parameterMap;
    map<int, int> midiMap;

    
    
    
    
    GuiMenu *parameterGroupSelector;
    GuiTextBox *bActiveMantaElement;
    GuiTextBox *bActiveParameter;
    GuiRangeSlider<float> *tActiveRange;
    map<string, GuiMenu*> subMenus;

    GuiPanel *panel;
    
    
    int assignPadIdx;
    MantaParameterMapping *selectedInstrument;
    bool assignArmed;
};