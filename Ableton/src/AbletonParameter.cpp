#include "AbletonParameter.h"


AbletonParameter::AbletonParameter(string name, int track, int device, int parameter, float value_, ofxOscSender * sender)
{
    this->name = name;
    this->track = track;
    this->device = device;
    this->parameter = parameter;
    this->sender = sender;
    value = new float(value_);
}

AbletonParameter::~AbletonParameter()
{
    //ofRemoveListener(slider->sliderEvent, this, &AbletonParameter::eventSlider);
    //delete slider;
}

void AbletonParameter::setRange(float min, float max)
{
    this->min = min;
    this->max = max;
}

void AbletonParameter::setValue(float value_)
{
    *value = value_;
}

void AbletonParameter::createSlider(GuiWidget *widget)
{
    slider = widget->addSlider(name, value, min, max, this, &AbletonParameter::eventSlider);
}

void AbletonParameter::eventSlider(GuiSliderEventArgs<float> &evt)
{
    ofxOscMessage msg;
    msg.setAddress("/live/device");
    msg.addIntArg(track);
    msg.addIntArg(device);
    msg.addIntArg(parameter);
    msg.addFloatArg(*value);
    sender->sendMessage(msg);
}