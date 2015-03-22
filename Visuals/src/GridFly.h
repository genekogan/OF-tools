#include "ofMain.h"
#include "Scene.h"


class GridFly : public Scene
{
public:
    ~GridFly();
    void setup();
    void update();
    void draw();
    
    void setup(int width, int height);
    void draw(int x, int y);
    
private:
    
    void addRow();
    
    ofMesh mesh;
    ofEasyCam cam;
    
    vector<ofVec3f> *vertices;
    float minY, dy, y, Z;
    ofPoint camPosition = ofPoint(0, 100);
    
    int nx;
    int ny;
    ofVec2f length;
    float margin;
    ofVec2f noiseFactor;
    float speed;
    float angle;
    ofVec3f axis;
    ofFloatColor color;

};


/*
 - random shapes floating around with random data
 - 3d shapes / obj from thingiverse
*/