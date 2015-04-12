#include "Agents.h"


void Agents::setup(int width, int height, bool clearControls)
{
    Scene::setup(width, height, clearControls);

    numAgents = 16;
    wrapping = true;
    debug = false;
    speed = 1.0;
    size = 200;
    color1 = ofFloatColor(1);
    color2 = ofFloatColor(1);
    fillAlpha = 25;
    strokeAlpha = 25;
    lineWidth = 1.0;
    cycle = 100;
    density = 0.7;
    drawLines = false;
    drawTriangles = true;

    control.addSlider("numAgents and some other stuff", &numAgents, 1, 360);
    control.addSlider("speed", &speed, -5.0f, 5.0f);
    control.addSlider("size and some other stuff", &size, 30, 800);
    control.addColor("colorLines", &color1);
    control.addColor("colorTri", &color2);
    control.addSlider("fillAlpha and some other stuff", &fillAlpha, 0, 255);
    control.addSlider("strokeAlpha and some other stuff", &strokeAlpha, 0, 255);
    control.addSlider("lineWidth and some other stuff", &lineWidth, 0.0f, 5.0f);
    control.addSlider("cycle and some other stuff", &cycle, 4, 1000);
    control.addSlider("density", &density, 0.0f, 1.0f);
    control.addToggle("wrap", &wrapping);
    control.addToggle("debug and some other stuff", &debug);
    control.addToggle("lines", &drawLines);
    control.addToggle("triangles", &drawTriangles);
    
    update();
}

void Agents::update()
{
    while (agents.size() < numAgents) {
        addNewAgent();
    }
    while (agents.size() > numAgents) {
        agents.pop_back();
    }
    
    for (int i=0; i<agents.size(); i++)
    {
        agents[i].setSpeed(speed);
        agents[i].setSize(size);
        agents[i].update();
        if (wrapping) {
            agents[i].wrap();
        }
    }
    for (int i=0; i<agents.size(); i++)
    {
        for (int j=i+1; j<agents.size(); j++)
        {
            bool intersecting = agents[i].isIntersecting(agents[j]);
            if (intersecting)
            {
                agents[i].addIntersectingAgent(&agents[j]);
                agents[j].addIntersectingAgent(&agents[i]);
            }
        }
    }
}

void Agents::refresh()
{
    ofBackground(0);
}

void Agents::addNewAgent()
{
    Agent agent;
    agent.setup(ofRandom(0, width), ofRandom(0, height), ofRandom(1), width, height);
    agents.push_back(agent);
}

void Agents::drawDebug()
{
    ofBackground(0);
    for (int i=0; i<agents.size(); i++) {
        agents[i].draw();
    }
}

void Agents::draw(int x, int y)
{
    Scene::beginDraw(x, y);
    
    if (debug) {
        drawDebug();
        return;
    }
    
    // draw lines between all intersecting agents
    if (drawLines)
    {
        for (int i=0; i<agents.size(); i++)
        {
            for (int j=0; j<agents[i].getIntersecting().size(); j++)
            {
                Agent *a = agents[i].getIntersecting()[j];
                float t = (float) ((ofGetFrameNum() + a->tOffset) % cycle) / cycle;
                if (t < density)
                {
                    ofSetColor(color1, strokeAlpha);
                    ofSetLineWidth(lineWidth);
                    ofLine(agents[i].pos.x, agents[i].pos.y, a->pos.x, a->pos.y);
                }
            }
        }   
    }
    
    // when intersecting exactly with two agents, draw triangle
    if (drawTriangles)
    {
        for (int i=0; i<agents.size(); i++)
        {
            if (agents[i].getIntersecting().size() == 2)
            {
                Agent *a1 = agents[i].getIntersecting()[0];
                Agent *a2 = agents[i].getIntersecting()[1];
                float t = (float) ((ofGetFrameNum() + a1->tOffset + a2->tOffset) % cycle) / cycle;
                if (t < density)
                {
                    ofSetColor(color2, fillAlpha);
                    ofFill();
                    ofTriangle(agents[i].pos.x, agents[i].pos.y,
                               a1->pos.x, a1->pos.y, a2->pos.x, a2->pos.y);
                    
                    ofSetColor(0, strokeAlpha);
                    ofNoFill();
                    ofTriangle(agents[i].pos.x, agents[i].pos.y,
                               a1->pos.x, a1->pos.y, a2->pos.x, a2->pos.y);
                }
                
            }
        }        
    }
    
    Scene::endDraw();
}

void Agent::setup(int x, int y, float baseRad, int width, int height)
{
    this->width = width;
    this->height = height;
    pos.set(x, y);
    vel.set(ofRandom(-1, 1), ofRandom(-1, 1));
    this->baseRad = baseRad;
    size = 50.0;
    speed = 1.0;
    rad = baseRad * size;
    tOffset = (int) ofRandom(1000);
}

void Agent::setSpeed(float speed)
{
    this->speed = speed;
}

void Agent::setSize(float size)
{
    this->size = size;
}

void Agent::update()
{
    intersecting.clear();
    rad = baseRad * size;
    pos += (speed * vel);
}

bool Agent::isIntersecting(Agent &other)
{
    bool isIntersecting = false;
    if ((pos.x - other.pos.x)*(pos.x - other.pos.x) +
        (pos.y - other.pos.y)*(pos.y - other.pos.y) <
        (rad + other.rad)*(rad + other.rad)) {
        isIntersecting = true;
    }
    return isIntersecting;
}

vector<Agent *> Agent::getIntersecting()
{
    return intersecting;
}

void Agent::addIntersectingAgent(Agent *other)
{
    intersecting.push_back(other);
}

void Agent::wrap()
{
    if      (pos.x < 0)             pos.set(width, pos.y);
    else if (pos.x > width)         pos.set(0, pos.y);
    if      (pos.y < 0)             pos.set(pos.x , height);
    else if (pos.y > height)        pos.set(pos.x, 0);
}

void Agent::draw()
{
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofSetColor(255);
    ofNoFill();
    ofEllipse(0, 0, 2*rad, 2*rad);
    ofRotate(ofRadToDeg(atan(vel.y / vel.x) + (vel.x < 0 ? PI : 0)), 0, 0, 1);
    ofLine(0, 0, rad, 0);
    ofPopMatrix();
}
