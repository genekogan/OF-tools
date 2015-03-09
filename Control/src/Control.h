#pragma once


#include "ofMain.h"

#include "Parameter.h"

#include "GuiConstants.h"

#include "GuiElement.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiRangeSlider.h"
#include "Gui2dPad.h"

#include "GuiWidgetBase.h"

#include "GuiColor.h"
#include "GuiMenu.h"

#include "Bpm.h"
#include "Sequence.h"
#include "Sequencer.h"
#include "MidiSequencer.h"

#include "GuiWidget.h"
#include "GuiPanel.h"


/*
 
TO-DO
=====
saving sequencer sets
examples
 - widget
 - panel
 - sequencer
object management
 - dynamically adding/removing widgets/elements
 - shared_ptr (http://www.umich.edu/~eecs381/handouts/C++11_smart_ptrs.pdf)
specific GuiElementEventArgs *L, M
subclass accessors
---------
examples
 - customized menu
style customization (color, size, font)
sortable menu widget
keyboard editing
slider warp
non-linked sequencer


OSC MANAGER
===========
 - osc in, osc out
 - generate touch osc layouts
 - osc summary interface
 
 
PRESETS
=======
 - parameters load/save xml
 - save/load interface
 
 
 
*/