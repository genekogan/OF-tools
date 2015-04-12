#include "GuiMultiRangeSlider.h"


GuiMultiRangeSliderBase::GuiMultiRangeSliderBase(string name) : GuiMultiElement(name)
{
    marginY = GUI_DEFAULT_MARGIN_Y_INNER;
    setHeader(getName());
}
