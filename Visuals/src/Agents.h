#pragma once

#include "ofMain.h"
#include "Scene.h"


enum AgentsDrawType { LINES, TRIANGLES };


class Agent
{    
public:
    void setup(int x, int y, float baseRad, int width, int height);
    void setSpeed(float speed);
    void setSize(float size);
    void update();
    void wrap();
    void draw();
    void addIntersectingAgent(Agent *other);
    bool isIntersecting(Agent &other);
    vector<Agent *> getIntersecting();

    float rad, baseRad, speed, size;
    int tOffset;
    ofVec2f pos, vel;
    int width, height;
    vector<Agent *> intersecting;
};


class Agents : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
    void setup(int width, int height);
    void draw(int x, int y);
    
protected:
    
    void setDrawingMode(string & s);
    void addNewAgent();
    void refresh();
    void drawDebug();

    vector<Agent> agents;
    
    int numAgents;
    bool wrapping;
    bool debug;
    float speed;
    int size;
    ofFloatColor color1;
    ofFloatColor color2;
    int fillAlpha;
    int strokeAlpha;
    float lineWidth;
    int cycle;
    float density;
    bool drawLines;
    bool drawTriangles;
};
