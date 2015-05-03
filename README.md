New repository of tools and templates to replace [OF-Tools-And-Templates](https://github.com/genekogan/OF-tools-and-templates) which is no longer being maintained.  Documentation scant, more coming soon.

### Tools

[Control](https://github.com/genekogan/OF-tools/tree/master/Control)
 - GUI and parameter tracker
 - manages OSC connections, and auto-generates synced TouchOSC layouts
 - modulate parameters with sequencer
 - save and load presets

[Visuals](https://github.com/genekogan/OF-tools/tree/master/Visuals) &  [Canvas](https://github.com/genekogan/OF-tools/tree/master/Canvas)
 - visual content, shader collection, media player/manager, scene changer
 - manages a sequence of layers which create and modulate visual content inside a quad
 - simple projection mapping w/ gui
 - integration with [Syphon](http://syphon.v002.info/) and [ScreenGrab](https://github.com/genekogan/ofxScreenGrab) for grabbing visual content from outside the app
 - save and load presets of Canvas layer sequences and parameters

[OpenNi](https://github.com/genekogan/OF-tools/tree/master/OpenNi)
 - common kinect routines/templates
 - track contours/blobs and skeleton features, create masks and visual content based on the data
 - calibrate to a projector with [ofxKinectProjectorToolkit](https://github.com/genekogan/ofxKinectProjectorToolkit)
 - track statistical features with [ofxKinectFeatures](https://github.com/asarasua/ofxKinectFeatures)

[AudioUnit](https://github.com/genekogan/OF-tools/tree/master/AudioUnit), [AbletonLive](https://github.com/genekogan/OF-tools/tree/master/AbletonLive) [SuperCollider](https://github.com/genekogan/OF-tools/tree/master/SuperCollider)
 - sound modules for controlling [OSX AudioUnits](https://github.com/admsyn/ofxAudioUnit), [Ableton Live](https://www.ableton.com/), and [SuperCollider](supercollider.sourceforge.net)
 - main contribution is automatically parsing relevant parameters (for Ableton or an audio unit) and providing a GUI and interface to control them, give data to user

[Algorithms](https://github.com/genekogan/OF-tools/tree/master/Algorithms)
 - point generators (evenly spaced, circle packing, delaunay)
 - recursively subdivide a 2d polygon
 - time-based behaviors

LeapMotion
 - (tbd) derive high-level features from [LeapMotion data](https://www.leapmotion.com/)

[MantaController](https://github.com/genekogan/OF-tools/tree/master/Manta)
 - interface with the [Manta](www.snyderphonics.com/products.htm) touch controller


#### Usage

To use these in an OF project, the best place to put this repo is in your OF root folder, so that it is at the same level from your root folder as addons.

Many of the tools and templates require additional addons to function. This is a complete list of addons used in any of the templates, though not all templates require all addons.

Addons
 - [ofxSecondWindow](https://github.com/genekogan/ofxSecondWindow)
 - [ofxCv](https://github.com/kylemcdonald/ofxCv)
 - [ofxOpenNi](https://github.com/gameoverhack/ofxOpenNI)
 - [ofxKinectProjectorToolkit](https://github.com/genekogan/ofxKinectProjectorToolkit)
 - [ofxKinectFeatures](https://github.com/asarasua/ofxKinectFeatures)
 - [ofxLearn](https://github.com/genekogan/ofxLearn)
 - [ofxSelfOrganizingMap](https://github.com/genekogan/ofxSelfOrganizingMap)
 - [ofxGMM](https://github.com/genekogan/ofxGMM)
<!-- - [ofxHistogram](https://github.com/genekogan/ofxHistogram) -->
<!-- - [ofxSpreadsheet](https://github.com/genekogan/ofxSpreadsheet) -->
<!-- - [ofxTouchOsc](https://github.com/genekogan/ofxTouchOsc) -->
<!-- - [ofxSequencer](https://github.com/genekogan/ofxSequencer) -->
 - [ofxBpm](https://github.com/mirrorboy714/ofxBpm)
 - [ofxConvexHull](https://github.com/genekogan/ofxConvexHull)
 - [ofxSyphon](https://github.com/astellato/ofxSyphon)
 - [ofxAudioUnit](https://github.com/admsyn/ofxAudioUnit)
 - [ofxSuperCollider](http://www.erase.net/projects/ofxSuperCollider/)
<!-- - [ofxLeapMotion2](https://github.com/genekogan/ofxLeapMotion2) -->
<!-- - [ofxTwistedRibbon](https://github.com/tado/ofxTwistedRibbon) -->
 - [ofxBlurShader](https://github.com/companje/ofxBlurShader)
 - [ofxDelaunay](https://github.com/obviousjim/ofxDelaunay)
 - [ofxPostGlitch](https://github.com/maxillacult/ofxPostGlitch)
 - [ofxPostProcessing](https://github.com/neilmendoza/ofxPostProcessing)
 - [ofxFx](https://github.com/patriciogonzalezvivo/ofxFX)
 - [ofxFluid](https://github.com/patriciogonzalezvivo/ofxFluid)
 - [ofxManta](https://github.com/genekogan/ofxManta/tree/master/src)

### Misc

Notes:
 - various assets, like openni config/lib files excluded from the repo to save space.  will make a python script for automating placing them into individual apps

