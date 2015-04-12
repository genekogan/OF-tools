#include "ProjectionMapping.h"


Quad::Quad()
{
    selectedCorner = -1;
    debugging = false;
    debugMe = false;
}

void Quad::begin()
{
    quad.beginDraw();
}

void Quad::end()
{
    quad.endDraw();
    if (debugging) {
        drawDebug();
    }
}

void Quad::setDebug(bool debugging, bool debugMe)
{
    this->debugging = debugging;
    this->debugMe = debugMe;
}

void Quad::drawDebug()
{
    ofPushStyle();
    if (debugMe)
    {
        ofSetColor(0, 255, 0);
        ofSetLineWidth(4);
    }
    else
    {
        ofSetColor(255, 0, 0, 150);
        ofSetLineWidth(2);
    }
    ofNoFill();
    ofBeginShape();
    ofVertex(quad.getOutputPoint(0).x, quad.getOutputPoint(0).y);
    ofVertex(quad.getOutputPoint(1).x, quad.getOutputPoint(1).y);
    ofVertex(quad.getOutputPoint(2).x, quad.getOutputPoint(2).y);
    ofVertex(quad.getOutputPoint(3).x, quad.getOutputPoint(3).y);
    ofEndShape(close);
    ofPopStyle();
}

ofPoint Quad::getInputCorner(int idx)
{
    ofPoint pp = quad.getInputPoint(idx);
    return quad.getInputPoint(idx);
}

ofPoint Quad::getOutputCorner(int idx)
{
    ofPoint pp = quad.getOutputPoint(idx);
    return quad.getOutputPoint(idx);
}

void Quad::setInputCorner(int idx, int x, int y)
{
    quad.setInputPoint(idx, x, y);
}

void Quad::setOutputCorner(int idx, int x, int y)
{
    quad.setOutputPoint(idx, x, y);
}

void Quad::moveOutputCorner(int x, int y)
{
    if (selectedCorner == 4)
    {
        ofPoint diff = ofPoint(x-lastGrabbedPoint.x, y-lastGrabbedPoint.y);
        for (int i=0; i<4; i++)
        {
            ofPoint corner = getOutputCorner(i);
            setOutputCorner(i, corner.x+diff.x, corner.y+diff.y);
        }
        lastGrabbedPoint.set(x, y);
    }
    else if (selectedCorner >= 0)
    {
        quad.setOutputPoint(selectedCorner, x, y);
    }
}

void Quad::grabOutputCorner(int x, int y)
{
    selectedCorner = -1;
    int minDist = 25;
    for (int i=0; i<4; i++) {
        ofPoint c = quad.getOutputPoint(i);
        if (abs(ofDist(c.x, c.y, x, y)) < minDist) {
            selectedCorner = i;
            minDist = abs(ofDist(c.x, c.y, x, y));
        }
    }
    // if no corners found, see if grabbing the whole quad
    if (selectedCorner == -1) {
        ofPolyline poly;
        for (int i=0; i<4; i++)
            poly.addVertex(getOutputCorner(0));
        poly.addVertex(getOutputCorner(1));
        poly.addVertex(getOutputCorner(2));
        poly.addVertex(getOutputCorner(3));
        if (poly.inside(x, y))
        {
            selectedCorner = 4;
            lastGrabbedPoint.set(x, y);
        }
    }
}

void Quad::releaseOutputCorner()
{
    selectedCorner = -1;
}

ProjectionMapping::ProjectionMapping()
{
    selected = 0;
    debug = false;
    setMouseEventsAuto(true);
    mouseResolution.set(ofGetWidth(), ofGetHeight());
}

void ProjectionMapping::setMouseEventsAuto(bool mouseAuto)
{
    this->mouseAuto = mouseAuto;
    if (mouseAuto)
    {
        ofAddListener(ofEvents().mousePressed, this, &ProjectionMapping::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &ProjectionMapping::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &ProjectionMapping::mouseReleased);
        ofAddListener(ofEvents().keyPressed, this, &ProjectionMapping::keyPressed);
    }
    else
    {
        ofRemoveListener(ofEvents().mousePressed, this, &ProjectionMapping::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &ProjectionMapping::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &ProjectionMapping::mouseReleased);
        ofRemoveListener(ofEvents().keyPressed, this, &ProjectionMapping::keyPressed);
    }
}

void ProjectionMapping::setMouseResolution(int x, int y)
{
    mouseResolution.set(x, y);
}

void ProjectionMapping::addQuad(int width, int height)
{
    Quad *quad = new Quad();
    int initialOffset = 72 * quads.size();
    
    quad->setInputCorner(0, 0, 0);
    quad->setInputCorner(1, width, 0);
    quad->setInputCorner(2, width, height);
    quad->setInputCorner(3, 0, height);
    quad->setOutputCorner(0, initialOffset, initialOffset);
    quad->setOutputCorner(1, initialOffset + width, initialOffset);
    quad->setOutputCorner(2, initialOffset + width, initialOffset + height);
    quad->setOutputCorner(3, initialOffset, initialOffset + height);
    quads.push_back(quad);
}

void ProjectionMapping::mousePressed(ofMouseEventArgs &e)
{
    if (!debug) return;
    float x = ofMap(e.x, 0, ofGetWidth(), 0, mouseResolution.x);
    float y = ofMap(e.y, 0, ofGetHeight(), 0, mouseResolution.y);
    quads[selected]->grabOutputCorner(x, y);
}

void ProjectionMapping::mouseDragged(ofMouseEventArgs &e)
{
    if (!debug) return;
    float x = ofMap(e.x, 0, ofGetWidth(), 0, mouseResolution.x);
    float y = ofMap(e.y, 0, ofGetHeight(), 0, mouseResolution.y);
    quads[selected]->moveOutputCorner(x, y);
}

void ProjectionMapping::mouseReleased(ofMouseEventArgs &e)
{
    if (!debug) return;
    quads[selected]->releaseOutputCorner();
}

void ProjectionMapping::keyPressed(ofKeyEventArgs &e)
{
    if (debug)
    {
        if (e.key == OF_KEY_LEFT) {
            toggleSelectedQuad(-1);
        }
        else if (e.key == OF_KEY_RIGHT) {
            toggleSelectedQuad(+1);
        }
        else if (e.key == 'S') {
            savePreset("ProjectionMapping_Preset.xml");
        }
        else if (e.key == 'L') {
            loadPreset("ProjectionMapping_Preset.xml");
        }
    }
}

void ProjectionMapping::setDebug(bool debug)
{
    this->debug = debug;
    for (int i=0; i<quads.size(); i++) {
        quads[i]->setDebug(debug, i==selected);
    }
}

void ProjectionMapping::toggleDebug()
{
    setDebug(!debug);
}

void ProjectionMapping::toggleSelectedQuad(int shift)
{
    quads[selected]->setDebug(debug, false);
    selected = (selected+quads.size()+shift)%quads.size();
    quads[selected]->setDebug(debug, true);
}

void ProjectionMapping::loadPreset(string path)
{
    ofXml xml;
    xml.load(path);
    setFromXml(xml);
}

void ProjectionMapping::savePreset(string path)
{
    ofXml xml;
    getXml(xml);
    xml.save(path);
}

void ProjectionMapping::getXml(ofXml &xml)
{
    xml.addChild("Quads");
    xml.setTo("Quads");
    for (int i=0; i<quads.size(); i++)
    {
        ofXml quadXml;
        quadXml.addChild("Quad");
        quadXml.setTo("Quad");
        quadXml.addValue("iX0", quads[i]->getInputCorner(0).x);
        quadXml.addValue("iY0", quads[i]->getInputCorner(0).y);
        quadXml.addValue("iX1", quads[i]->getInputCorner(1).x);
        quadXml.addValue("iY1", quads[i]->getInputCorner(1).y);
        quadXml.addValue("iX2", quads[i]->getInputCorner(2).x);
        quadXml.addValue("iY2", quads[i]->getInputCorner(2).y);
        quadXml.addValue("iX3", quads[i]->getInputCorner(3).x);
        quadXml.addValue("iY3", quads[i]->getInputCorner(3).y);
        quadXml.addValue("oX0", quads[i]->getOutputCorner(0).x);
        quadXml.addValue("oY0", quads[i]->getOutputCorner(0).y);
        quadXml.addValue("oX1", quads[i]->getOutputCorner(1).x);
        quadXml.addValue("oY1", quads[i]->getOutputCorner(1).y);
        quadXml.addValue("oX2", quads[i]->getOutputCorner(2).x);
        quadXml.addValue("oY2", quads[i]->getOutputCorner(2).y);
        quadXml.addValue("oX3", quads[i]->getOutputCorner(3).x);
        quadXml.addValue("oY3", quads[i]->getOutputCorner(3).y);
        xml.addXml(quadXml);
    }
    xml.setToParent();
}

void ProjectionMapping::setFromXml(ofXml &xml)
{
    if(xml.exists("Quads"))
    {
        xml.setTo("Quads[0]");
        do {
            if (xml.getName() == "Quads" && xml.setTo("Quad[0]"))
            {
                int idx = 0;
                do {
                    if (quads.size() < idx+1) {
                        addQuad(ofGetWidth(), ofGetHeight());
                    }
                    quads[idx]->setInputCorner(0, xml.getValue<int>("iX0"), xml.getValue<int>("iY0"));
                    quads[idx]->setInputCorner(1, xml.getValue<int>("iX1"), xml.getValue<int>("iY1"));
                    quads[idx]->setInputCorner(2, xml.getValue<int>("iX2"), xml.getValue<int>("iY2"));
                    quads[idx]->setInputCorner(3, xml.getValue<int>("iX3"), xml.getValue<int>("iY3"));
                    quads[idx]->setOutputCorner(0, xml.getValue<int>("oX0"), xml.getValue<int>("oY0"));
                    quads[idx]->setOutputCorner(1, xml.getValue<int>("oX1"), xml.getValue<int>("oY1"));
                    quads[idx]->setOutputCorner(2, xml.getValue<int>("oX2"), xml.getValue<int>("oY2"));
                    quads[idx]->setOutputCorner(3, xml.getValue<int>("oX3"), xml.getValue<int>("oY3"));
                    idx++;
                }
                while (xml.setToSibling());
                xml.setToParent();
            }
        }
        while (xml.setToSibling());
        xml.setToParent();
    }
}