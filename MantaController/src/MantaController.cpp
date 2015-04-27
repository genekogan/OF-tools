#include "MantaController.h"



MantaController::MantaController() : ofxManta()
{
    featureValueSelected = false;
    featureVelocitySelected = false;
    velocityLerpRate = 0.1;
    drawHelperLabel = true;
    //viewParameters = false;         // ???
    
    
    panel.setPosition(10, ofGetHeight()-35);
    panel.addSlider("pad1", getPadRef(0, 0), 0.0f, 196.0f);
}

MantaController::~MantaController()
{
    close();
}

bool MantaController::setup()
{
    ofxManta::setup();
    setMouseActive(true);
    setVisible(true);
}

void MantaController::setVisible(bool visible)
{
    this->visible = visible;
    setMouseActive(visible);
}

void MantaController::setMouseActive(bool mouseActive)
{
    this->mouseActive = mouseActive;
    if (mouseActive)
    {
        ofAddListener(ofEvents().mousePressed, this, &MantaController::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &MantaController::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &MantaController::mouseReleased);
        ofAddListener(ofEvents().keyPressed, this, &MantaController::keyPressed);
        ofAddListener(ofEvents().keyReleased, this, &MantaController::keyReleased);
    }
    else
    {
        ofRemoveListener(ofEvents().mousePressed, this, &MantaController::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &MantaController::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &MantaController::mouseReleased);
        ofRemoveListener(ofEvents().keyPressed, this, &MantaController::keyPressed);
        ofRemoveListener(ofEvents().keyReleased, this, &MantaController::keyReleased);
    }
}

void MantaController::update()
{
    if (!connected)
    {
        connected = getConnected();
        return;
    }
    
    // pad velocities
    int idx = 0;
    for (int row=0; row<6; row++)
    {
        for (int col=0; col<8; col++)
        {
            padVelocity[row][col] = ofLerp(padVelocity[row][col], getPad(row, col) - prevPad[row][col], 0.1);
            prevPad[row][col] = getPad(row, col);
            idx++;
        }
    }
    // slider velocities
    for (int i=0; i<2; i++)
    {
        sliderVelocity[i] = ofLerp(sliderVelocity[i], getSlider(i) - prevSlider[i], velocityLerpRate);
        prevSlider[i] = getSlider(i);
    }
    // button velocities
    for (int i=0; i<4; i++)
    {
        buttonVelocity[i] = ofLerp(buttonVelocity[i], getButton(i) - prevButton[i], velocityLerpRate);
        prevButton[i] = getButton(i);
    }
    
    // finger stats
    float _padSum = 0;
    float _padAverage = 0;
    float _width = 0;
    float _height = 0;
    float _whRatio = 0;
    float _numPads = 0;
    
    fingers.clear();
    fingerValues.clear();
    ofPoint fingersMin = ofPoint(1, 1);
    ofPoint fingersMax = ofPoint(0, 0);
    float currentValue;
    for (int row=0; row<6; row++)
    {
        for (int col=0; col<8; col++)
        {
            currentValue = getPad(row, col);
            if (currentValue > 0)
            {
                ofPoint fingerPos = getPositionAtPad(row, col);
                fingers.push_back(fingerPos);
                fingerValues.push_back(currentValue);
                _numPads+=1.0;
                _padSum += currentValue;
                if (fingerPos.x > fingersMax.x)   fingersMax.x = fingerPos.x;
                if (fingerPos.x < fingersMin.x)   fingersMin.x = fingerPos.x;
                if (fingerPos.y > fingersMax.y)   fingersMax.y = fingerPos.y;
                if (fingerPos.y < fingersMin.y)   fingersMin.y = fingerPos.y;
            }
        }
    }
    
    _padAverage = fingers.size() > 0 ? _padSum / _numPads : 0.0;
    
    float _perimeter = 0.0;
    float _averageInterFingerDistance = 0.0;
    
    // stats on finger groups
    if (fingers.size() < 2)
    {
        _width = 0;
        _height = 0;
        _whRatio = 0;
        _perimeter = 0;
        _averageInterFingerDistance = 0;
        fingersHull.resize(0);
        fingersHullNormalized.resize(0);
    }
    else if (fingers.size() == 2)
    {
        _width = fingersMax.x - fingersMin.x;
        _height = fingersMax.y - fingersMin.y;
        _whRatio = _width / (1.0 + _height);
        
        _perimeter = (pow(fingers[0].x - fingers[1].x, 2)+
                      pow(fingers[0].y - fingers[1].y, 2));
        
        _averageInterFingerDistance = _perimeter;
        fingersHull.resize(0);
        fingersHullNormalized.resize(0);
    }
    else
    {
        _width = fingersMax.x - fingersMin.x;
        _height = fingersMax.y - fingersMin.y;
        _whRatio = _width / _height;
        
        fingersHull = convexHull.getConvexHull(fingers);
        fingersHullNormalized.resize(fingersHull.size());
        for (int i=0; i<fingersHull.size(); i++)
        {
            fingersHullNormalized[i].x = (fingersHull[i].x - fingersMin.x) / (fingersMax.x - fingersMin.x);
            fingersHullNormalized[i].y = (fingersHull[i].y - fingersMin.y) / (fingersMax.y - fingersMin.y);
        }
        for (int i=0; i<fingersHull.size()-1; i++)
        {
            _perimeter += (pow(fingersHull[i].x - fingersHull[(i+1)].x, 2)+
                           pow(fingersHull[i].y - fingersHull[(i+1)].y, 2));
        }
        _averageInterFingerDistance = _perimeter / (float) (fingersHull.size()-1);
    }
    
    numPadsVelocity = ofLerp(numPadsVelocity, _numPads-numPads, velocityLerpRate);
    perimeterVelocity = ofLerp(perimeterVelocity, _perimeter-perimeter, velocityLerpRate);
    averageInterFingerDistanceVelocity = ofLerp(averageInterFingerDistanceVelocity, _averageInterFingerDistance-averageInterFingerDistance, velocityLerpRate);
    
    padSumVelocity = ofLerp(padSumVelocity, _padSum-padSum, velocityLerpRate);
    padAverageVelocity = ofLerp(padAverageVelocity, _padAverage-padAverage, velocityLerpRate);
    
    widthVelocity = ofLerp(widthVelocity, _width-padWidth, velocityLerpRate);
    heightVelocity = ofLerp(heightVelocity, _height-padHeight, velocityLerpRate);
    whRatioVelocity = ofLerp(whRatioVelocity, _whRatio-whRatio, velocityLerpRate);
    
    padWidth = _width;
    padHeight = _height;
    whRatio = _whRatio;
    perimeter = _perimeter;
    averageInterFingerDistance = _averageInterFingerDistance;
    padSum = _padSum;
    padAverage = _padAverage;
    numPads = _numPads;
    
    // centroid and weighted centroid
    ofPoint _centroid, _weightedCentroid;
    for (int i=0; i<fingers.size(); i++)
    {
        _centroid += fingers[i];
        _weightedCentroid += (fingers[i] * fingerValues[i] / padSum);
    }
    _centroid /= _numPads;
    
    centroidVelocityX = ofLerp(centroidVelocityX, _centroid.x-centroidX, velocityLerpRate);
    centroidVelocityY = ofLerp(centroidVelocityY, _centroid.y-centroidY, velocityLerpRate);
    weightedCentroidVelocityX = ofLerp(weightedCentroidVelocityX, _weightedCentroid.x-weightedCentroidX, velocityLerpRate);
    weightedCentroidVelocityY = ofLerp(weightedCentroidVelocityY, _weightedCentroid.y-weightedCentroidY, velocityLerpRate);
    
    centroidX = _centroid.x;
    centroidY = _centroid.y;
    weightedCentroidX = _weightedCentroid.x;
    weightedCentroidY = _weightedCentroid.y;
}

void MantaController::draw(int x, int y, int width)
{
    if (!visible)   return;
    this->x = x;
    this->y = y;
    this->width = width;
    
    mainDrawRect = ofRectangle(x, y, width, width * 310.0 / 400.0);
    //paramSwitchDrawRect = ofRectangle(x+mainDrawRect.getWidth()-17, y+2, 14, 14);
    
    //if (viewParameters)
    {
        //drawParametersSelector();
        //drawParametersSelectorButton();
    }
    //else
    {
        ofxManta::draw(x, y, width);
        if (drawHelperLabel && mainDrawRect.inside(ofGetMouseX(), ofGetMouseY()))
        {
            ofPushStyle();
            ofSetColor(255, 125, 0);
            ofDrawBitmapString("Click select. CMD multiple. SHIFT velocity",
                               x+3, y+mainDrawRect.getHeight()*0.22);
            ofPopStyle();
        }
        //drawParametersSelectorButton();
    }
    
    if (px != x || py != y || pwidth != width)
    {
        px = x;
        py = y;
        pwidth = width;
        if (mouseActive) {
            setMouseResponders();
        }
    }
}

void MantaController::drawStats(int x, int y, int w)
{
    int h = w * 310.0 / 400.0;
    statsDrawRect = ofRectangle(x, y, w, h);
    
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(x, y);
    
    ofSetColor(0);
    ofFill();
    ofRect(0, 0, w, h);
    
    // draw convex hull
    ofNoFill();
    ofSetColor(0, 255, 0);
    ofSetLineWidth(1);
    ofBeginShape();
    for (int i=0; i<fingersHull.size(); i++)
    {
        float x = ofMap(fingersHull[i].x, 0, 1, 0, w);
        float y = ofMap(fingersHull[i].y, 0, 1, 0, h);
        ofVertex(x, y);
    }
    ofEndShape();
    
    // draw fingers
    ofFill();
    ofSetColor(255, 0, 0);
    ofSetLineWidth(0);
    for (int i=0; i<fingers.size(); i++)
    {
        float x = ofMap(fingers[i].x, 0, 1, 0, w);
        float y = ofMap(fingers[i].y, 0, 1, 0, h);
        float r = ofMap(fingerValues[i], 0, 196, 0, 10);
        ofCircle(x, y, r);
    }
    
    // draw centroids
    float cx = ofMap(centroidX, 0, 1, 0, w);
    float cy = ofMap(centroidY, 0, 1, 0, h);
    float wcx = ofMap(weightedCentroidX, 0, 1, 0, w);
    float wcy = ofMap(weightedCentroidY, 0, 1, 0, h);
    ofNoFill();
    ofSetColor(150);
    ofSetLineWidth(2);
    ofLine(cx-4, cy-4, cx+4, cy+4);
    ofLine(cx+4, cy-4, cx-4, cy+4);
    ofSetColor(255);
    ofLine(wcx-4, wcy-4, wcx+4, wcy+4);
    ofLine(wcx+4, wcy-4, wcx-4, wcy+4);
    
    // draw stats
    ofSetColor(255);
    ofDrawBitmapString("pad sum", 3, 12);
    ofDrawBitmapString("pad avg", 3, 28);
    ofDrawBitmapString("perimeter", 3, 44);
    ofDrawBitmapString("bw fingers", 3, 60);
    ofSetColor(0, 255, 0);
    ofSetLineWidth(0);
    ofFill();
    ofRect(75,  1, ofClamp(ofMap(padSum, 0, 1024, 0, w-80), 0, w-80), 14);
    ofRect(75, 17, ofClamp(ofMap(padAverage, 0, 196, 0, w-80), 0, w-80), 14);
    ofRect(75, 33, ofClamp(ofMap(perimeter, 0, 2, 0, w-80), 0, w-80), 14);
    ofRect(75, 49, ofClamp(ofMap(averageInterFingerDistance, 0, 1, 0, w-80), 0, w-80), 14);
    
    ofPopMatrix();
    ofPopStyle();
}

void MantaController::setMouseResponders()
{
    float sliderSizeX = 0.65 * getDrawWidth();
    float sliderSizeY = 0.05 * getDrawHeight();
    float buttonSize = 1.5 * 0.02 * getDrawWidth();
    
    ofRectangle sliderPosition0(x + 0.08 * getDrawWidth(), y + 0.05 * getDrawHeight(), sliderSizeX, sliderSizeY);
    ofRectangle sliderPosition1(x + 0.05 * getDrawWidth(), y + 0.13 * getDrawHeight(), sliderSizeX, sliderSizeY);
    ofRectangle buttonPosition0(x + 0.8 * getDrawWidth() - 0.5 * buttonSize, y + 0.075 * getDrawHeight() - 0.5 * buttonSize, buttonSize, buttonSize);
    ofRectangle buttonPosition1(x + 0.9 * getDrawWidth() - 0.5 * buttonSize, y + 0.075 * getDrawHeight() - 0.5 * buttonSize, buttonSize, buttonSize);
    ofRectangle buttonPosition2(x + 0.85 * getDrawWidth() - 0.5 * buttonSize, y + 0.155 * getDrawHeight() - 0.5 * buttonSize, buttonSize, buttonSize);
    ofRectangle buttonPosition3(x + 0.95 * getDrawWidth() - 0.5 * buttonSize, y + 0.155 * getDrawHeight() - 0.5 * buttonSize, buttonSize, buttonSize);
    
    sliderPositions[0] = sliderPosition0;
    sliderPositions[1] = sliderPosition1;
    buttonPositions[0] = buttonPosition0;
    buttonPositions[1] = buttonPosition1;
    buttonPositions[2] = buttonPosition2;
    buttonPositions[3] = buttonPosition3;
    
    for (int row=0; row<6; row++)
    {
        for (int col=0; col<8; col++)
        {
            int rx = x + ofMap(col + 0.5, 0, 8, 0.01 * getDrawWidth(), 0.94 * getDrawWidth());
            int ry = y + ofMap(row + 0.5, 6, 0, 0.24 * getDrawHeight(), 0.97 * getDrawHeight());
            int size = getDrawWidth() / 11.0;
            if (row %2 != 0)  rx += 0.93 * getDrawWidth() / 16.0;
            ofRectangle padPosition(rx - 0.5 * size, ry - 0.5*size, size, size);
            padPositions[row * 8 + col] = padPosition;
        }
    }
}

int MantaController::getSizeSelection()
{
    return getPadSelection(0).size() + getPadSelection(1).size() +
    getSliderSelection(0).size() + getSliderSelection(1).size() +
    getButtonSelection(0).size() + getButtonSelection(1).size();
}

void MantaController::getMantaElementsInBox(int x, int y)
{
    clearSelection(selection);
    dragPoint2 = ofPoint(x, y);
    ofRectangle rect = ofRectangle(min(dragPoint1.x, dragPoint2.x), min(dragPoint1.y, dragPoint2.y),
                                   abs(dragPoint1.x - dragPoint2.x), abs(dragPoint1.y - dragPoint2.y));
    for (int i=0; i<48; i++)
    {
        if (rect.inside(padPositions[i].x, padPositions[i].y) ||
            rect.inside(padPositions[i].x+ padPositions[i].width, padPositions[i].y) ||
            rect.inside(padPositions[i].x+ padPositions[i].width, padPositions[i].y+ padPositions[i].height) ||
            rect.inside(padPositions[i].x, padPositions[i].y+ padPositions[i].height)) {
            int row = floor(i / 8);
            int col = i % 8;
            addPadToSelection(row, col, selection);
        }
    }
    for (int i=0; i<2; i++)
    {
        if (rect.inside(sliderPositions[i].x, sliderPositions[i].y) ||
            rect.inside(sliderPositions[i].x+ sliderPositions[i].width, sliderPositions[i].y) ||
            rect.inside(sliderPositions[i].x+ sliderPositions[i].width, sliderPositions[i].y+sliderPositions[i].height) ||
            rect.inside(sliderPositions[i].x, sliderPositions[i].y+ sliderPositions[i].height)) {
            addSliderToSelection(i, selection);
        }
    }
    for (int i=0; i<2; i++)
    {
        if (rect.inside(buttonPositions[i].x, buttonPositions[i].y) ||
            rect.inside(buttonPositions[i].x+ buttonPositions[i].width, buttonPositions[i].y) ||
            rect.inside(buttonPositions[i].x+ buttonPositions[i].width, buttonPositions[i].y+ buttonPositions[i].height) ||
            rect.inside(buttonPositions[i].x, buttonPositions[i].y+ buttonPositions[i].height)) {
            addButtonToSelection(i, selection);
        }
    }
}

ofPoint MantaController::getPositionAtPad(int row, int col)
{
    return ofPoint(ofMap(row % 2 != 0 ? col+0.5 : col, 0, 9, 0, 1),
                   ofMap(row + 0.5, 0, 7, 1, 0));
}

void MantaController::setPadSelection(vector<int> idx, int selection)
{
    clearPadSelection();
    for (int i = 0; i < idx.size(); i++)
    {
        int row = floor(idx[i] / 8);
        int col = idx[i] % 8;
        addPadToSelection(row, col, selection);
    }
}

void MantaController::setSliderSelection(vector<int> idx, int selection)
{
    clearSliderSelection();
    for (int i = 0; i < idx.size(); i++)
    {
        addSliderToSelection(idx[i], selection);
    }
}

void MantaController::setButtonSelection(vector<int> idx, int selection)
{
    clearButtonSelection();
    for (int i = 0; i < idx.size(); i++)
    {
        addButtonToSelection(idx[i], selection);
    }
}

void MantaController::markPad(int row, int col, bool mark)
{
    setLedManual(true);
    setPadLedState(row, col, mark ? Manta::Red : Manta::Off);
    setLedManual(false);
}

void MantaController::markSlider(int index, int column)
{
    setLedManual(true);
    setSliderLedState(index, Manta::Amber, column);
    setLedManual(false);
}

void MantaController::markButton(int index, bool mark)
{
    setLedManual(true);
    setButtonLedState(index, mark ? Manta::Red : Manta::Off);
    setLedManual(false);
}

void MantaController::markAllPads(bool mark)
{
    setLedManual(true);
    for (int r=0; r<6; r++)
    {
        for (int c=0; c<8; c++) {
            setPadLedState(r, c, mark ? Manta::Red : Manta::Off);
        }
    }
    setLedManual(false);
}

void MantaController::markAllSliders(int column)
{
    setLedManual(true);
    for (int i=0; i<2; i++) {
        setSliderLedState(i, Manta::Amber, column);
    }
    setLedManual(false);
}

void MantaController::markAllButtons(bool mark)
{
    setLedManual(true);
    for (int i=0; i<4; i++) {
        setButtonLedState(i, mark ? Manta::Red : Manta::Off);
    }
    setLedManual(false);
}

void MantaController::mousePressed(ofMouseEventArgs &evt)
{
    if (!mouseActive ||
        !mainDrawRect.inside(evt.x, evt.y))  return;

    
    float h = width * 310.0 / 400.0;
    /*
    for (int i = 0; i < features.size(); i++)
    {
        ofRectangle pRect(x + 8,   y + ofMap(i, 0, 12, 15, min((int) h, 240)), 120, 16);
        ofRectangle vRect(x + 145, y + ofMap(i, 0, 12, 15, min((int) h, 240)), 120, 16);
        if (pRect.inside(evt.x, evt.y))
        {
            //MantaElement mantaEvt(features[i].type, 0, 0);
            //selectedFeature = features[i];
            featureValueSelected = true;
            featureVelocitySelected = false;
            //ofNotifyEvent(clickEvent, mantaEvt, this);
            return;
        }
        else if (vRect.inside(evt.x, evt.y))
        {
            //MantaElement mantaEvt(features[i].type, 0, 1);
            //selectedFeature = features[i];
            featureValueSelected = false;
            featureVelocitySelected = true;
            //ofNotifyEvent(clickEvent, mantaEvt, this);
            return;
        }
        //MantaElement mantaEvt;
        //ofNotifyEvent(clickEvent, mantaEvt, this);
        }
    }*/
    //else if (!shift)
    if (!shift)
    {
        dragging = true;
        dragPoint1 = ofPoint(evt.x, evt.y);
        dragPoint2 = dragPoint1;
        clearSelection(0);
        clearSelection(1);
        return;
    }
}

void MantaController::mouseDragged(ofMouseEventArgs &evt)
{
    if (!mouseActive || !dragging || //viewParameters ||
        !mainDrawRect.inside(evt.x, evt.y))  return;
    getMantaElementsInBox(evt.x, evt.y);
}

void MantaController::mouseReleased(ofMouseEventArgs &evt)
{
    //if (viewParameters) return;
    dragging = false;
    
    if (!mouseActive ||
        !mainDrawRect.inside(evt.x, evt.y) ||
        ofDist(dragPoint1.x, dragPoint1.y, dragPoint2.x, dragPoint2.y) > 1 ) {
        return;
    }
    
    for (int i=0; i<2; i++)
    {
        if (sliderPositions[i].inside(evt.x, evt.y))
        {
            if (!shift) clearSelection(selection);
            addSliderToSelection(i, selection);
            //MantaElement mantaEvt(SLIDER, i, selection);
            //ofNotifyEvent(clickEvent, mantaEvt, this);
            return;
        }
    }
    for (int i=0; i<4; i++)
    {
        if (buttonPositions[i].inside(evt.x, evt.y))
        {
            if (!shift) clearSelection(selection);
            addButtonToSelection(i, selection);
            //MantaElement mantaEvt(BUTTON, i, selection);
            //ofNotifyEvent(clickEvent, mantaEvt, this);
            return;
        }
    }
    for (int i=0; i<48; i++)
    {
        if (padPositions[i].inside(evt.x, evt.y))
        {
            if (!shift) clearSelection(selection);
            addPadToSelection(floor(i / 8), i % 8, selection);
            //MantaElement mantaEvt(PAD, i, selection);
            //ofNotifyEvent(clickEvent, mantaEvt, this);
            return;
        }
    }
    
    //MantaElement mantaEvt;
    //ofNotifyEvent(clickEvent, mantaEvt, this);
}

void MantaController::keyPressed(ofKeyEventArgs &e)
{
    if (e.key == OF_KEY_SHIFT) {
        //selection = 1;
    }
    else if (e.key == OF_KEY_COMMAND) {
        //shift = true;
    }
}

void MantaController::keyReleased(ofKeyEventArgs &e)
{
    if (e.key == OF_KEY_SHIFT) {
        //selection = 0;
    }
    else if (e.key == OF_KEY_COMMAND) {
        //shift = false;
    }
}











/*
 MantaController::MantaController()
 {
 features.push_back(MantaTypeFeature(NUM_PADS, "num pads"));
 features.push_back(MantaTypeFeature(PAD_SUM, "pad sum"));
 features.push_back(MantaTypeFeature(PAD_AVG, "pad average"));
 features.push_back(MantaTypeFeature(CENTROID_X, "centroidX"));
 features.push_back(MantaTypeFeature(CENTROID_Y, "centroidY"));
 features.push_back(MantaTypeFeature(W_CENTROID_X, "wCentroidX"));
 features.push_back(MantaTypeFeature(W_CENTROID_Y, "wCentroidY"));
 features.push_back(MantaTypeFeature(AVG_FING_DIST, "avg fing dist"));
 features.push_back(MantaTypeFeature(PERIMETER, "perimeter"));
 features.push_back(MantaTypeFeature(WIDTH, "width"));
 features.push_back(MantaTypeFeature(HEIGHT, "height"));
 features.push_back(MantaTypeFeature(WH_RATIO, "w/h ratio"));
 selectedFeature = features[0];
 featureValueSelected = false;
 featureVelocitySelected = false;
 velocityLerpRate = 0.1;
 drawHelperLabel = true;
 viewParameters = false;
 
 panel.setName("Manta");
 panel.addSlider("pad1", &getAverageInterFingerDistance(), 0.0f, 10.0f);
 
 }
 
 
 void MantaController::drawParametersSelector()
 {
 float h = width * 310.0 / 400.0;
 ofSetColor(0);
 ofFill();
 ofRect(x, y, width, h);
 
 for (int i=0; i<features.size(); i++)
 {
 ofRectangle pRect(x + 8,   y + ofMap(i, 0, features.size(), 15, min((int) h, 240)), 120, 16);
 ofRectangle vRect(x + 145, y + ofMap(i, 0, features.size(), 15, min((int) h, 240)), 120, 16);
 
 if (featureValueSelected && (selectedFeature.type == features[i].type))
 {
 ofSetColor(0, 100, 0);
 ofFill();
 }
 else
 {
 ofSetColor(0, 200, 0);
 ofNoFill();
 }
 ofRect(pRect);
 
 if (featureVelocitySelected && (selectedFeature.type == features[i].type))
 {
 ofSetColor(0, 100, 0);
 ofFill();
 }
 else
 {
 ofSetColor(0, 200, 0);
 ofNoFill();
 }
 ofRect(vRect);
 
 ofSetColor(255);
 ofDrawBitmapString(features[i].name,  pRect.x+3, pRect.y + 12);
 ofDrawBitmapString("velocity", vRect.x+3, vRect.y + 12);
 }
 }
 
 void MantaController::drawParametersSelectorButton()
 {
 ofPushStyle();
 ofNoFill();
 ofRect(paramSwitchDrawRect);
 ofDrawBitmapString("P", paramSwitchDrawRect.getX()+3, paramSwitchDrawRect.getY()+12);
 ofPopStyle();
 }
 
 
 */