#include "Subdivide.h"


Subdivision::Subdivision(int generation,
                         int x, int y,
                         int width, int height,
                         ofFloatColor *color,
                         ofFloatColor *varColor,
                         int *circleResolution,
                         bool *isLerp) {
    this->generation = generation;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;
    this->varColor = varColor;
    this->circleResolution = circleResolution;
    this->isLerp = isLerp;
    parent = NULL;
    subdivide();
}

Subdivision::Subdivision(int generation,
                         Subdivision *parent,
                         bool topleft,
                         int *circleResolution,
                         bool *isLerp) {
    this->generation = generation;
    this->parent = parent;
    this->topleft = topleft;
    this->circleResolution = circleResolution;
    this->isLerp = isLerp;
    color = new ofFloatColor();
    varColor = parent->varColor;
    offset = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
    subdivide();
    update();
}

void Subdivision::subdivide()
{
    if (generation > 0)
    {
        ratio = ofRandom(1);
        horizontal = ofRandom(1) < 0.5 ? true : false;
        child1 = new Subdivision(generation-1, this, true, circleResolution, isLerp);
        child2 = new Subdivision(generation-1, this, false, circleResolution, isLerp);
    }
}

void Subdivision::update()
{
    int r, g, b;
    if (parent == NULL) return;
    if (parent->horizontal)
    {
        x = parent->x + (topleft ? 0.0 : parent->width * parent->ratio);
        y = parent->y;
        width = parent->width * (topleft ? parent->ratio : 1.0 - parent->ratio);
        height = parent->height;
    }
    else
    {
        y = parent->y + (topleft ? 0.0 : parent->height * parent->ratio);
        x = parent->x;
        width = parent->width;
        height = parent->height * (topleft ? parent->ratio : 1.0-parent->ratio);
    }
    if (*isLerp)
    {
        r = ofClamp(ofLerp(255 * color->r, 255 * parent->color->r +
                           255 * varColor->r * 0.2 * offset.x, 0.2), 0, 255);
        g = ofClamp(ofLerp(255 * color->g, 255 * parent->color->g +
                           255 * varColor->g * 0.2 * offset.y, 0.2), 0, 255);
        b = ofClamp(ofLerp(255 * color->b, 255 * parent->color->b +
                           255 * varColor->b * 0.2 * offset.z, 0.2), 0, 255);
    }
    else
    {
        r = 255 * parent->color->r + 255 * varColor->r * offset.x;
        g = 255 * parent->color->g + 255 * varColor->g * offset.y;
        b = 255 * parent->color->b + 255 * varColor->b * offset.z;
    }
    color->set(ofColor(r, g, b, 255));
}

void Subdivision::draw(DrawStrategy drawStrategy)
{
    update();
    
    ofSetCircleResolution(*circleResolution);
    ofEnableSmoothing();
    
    if (generation == 0)
    {
        if (drawStrategy == RECTS) {
            drawRect();
        }
        else if (drawStrategy == DIAMONDS) {
            drawDiamond();
        }
        else if (drawStrategy == CIRCLES) {
            drawCircle();
        }
    }
    else
    {
        child1->draw(drawStrategy);
        child2->draw(drawStrategy);
    }
    
    ofDisableSmoothing();
}

void Subdivision::drawRect()
{
    ofSetColor(*color);
    ofFill();
    ofRect(x, y, width, height);
    ofNoFill();
    ofSetColor(0);
    ofSetLineWidth(1);
    ofRect(x, y, width, height);
}

void Subdivision::drawDiamond()
{
    ofSetColor(*color);
    ofFill();
    ofBeginShape();
        ofVertex(x, y+height/2);
        ofVertex(x+width/2, y);
        ofVertex(x+width, y+height/2);
        ofVertex(x+width/2, y+height);
    ofEndShape();
    ofNoFill();
    ofSetColor(0);
    ofSetLineWidth(1);
    ofBeginShape();
        ofVertex(x, y+height/2);
        ofVertex(x+width/2, y);
        ofVertex(x+width, y+height/2);
        ofVertex(x+width/2, y+height);
    ofEndShape();
}

void Subdivision::drawCircle()
{
    ofSetColor(*color);
    ofFill();
    ofEllipse(x+width/2, y+height/2, width, height);
    ofNoFill();
    ofSetColor(0);
    ofSetLineWidth(1);
    ofEllipse(x+width/2, y+height/2, width, height);
}

void Subdivide::setup(int width, int height, bool clearControls)
{
    Scene::setup(width, height, clearControls);

    control.addColor("color", &color);
    control.addColor("varColor", &varColor);
    control.addSlider("circleResolution", &circleResolution, 3, 72);
 
    control.addToggle("lerp", &isLerp);

    vector<string> drawTypes;
    drawTypes.push_back("Rects");
    drawTypes.push_back("Diamonds");
    drawTypes.push_back("Circles");
    control.addMenu("draw", drawTypes, this, &Subdivide::setDrawType);

    start = new Subdivision(9, 0, 0, width, height, &color, &varColor, &circleResolution, &isLerp);
}

void Subdivide::setDrawType(GuiElementEventArgs &e)
{
    drawStrategy = RECTS;
    if (e.name=="Rects") {
        drawStrategy = RECTS;
    }
    else if (e.name=="Diamonds") {
        drawStrategy = DIAMONDS;
    }
    else if (e.name=="Circles") {
        drawStrategy = CIRCLES;
    }
}

void Subdivide::update()
{
    
}

void Subdivide::draw(int x, int y)
{
    Scene::beginDraw(x, y);
    start->draw(drawStrategy);
    Scene::endDraw();
}
