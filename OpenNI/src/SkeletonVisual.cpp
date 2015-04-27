#include "SkeletonVisual.h"



void SkeletonVisual::setup(int width, int height)
{
    this->width = width;
    this->height = height;
}

void SkeletonVisual::update(OpenNI & openNi)
{
    ofClear(0, 0);
    
    int numTrackedUsers = openNi.getNumTrackedUsers();
    for (int i = 0; i < numTrackedUsers; i++)
    {
        ofxOpenNIUser * user = &openNi.getKinect().getTrackedUser(i);
        int id = user->getXnID();
        
        for (int j = 0; j < user->getNumJoints(); j++)
        {
            ofVec2f projectedJoint = openNi.getKinectProjectorToolkit().getProjectedPoint(user->getJoint((Joint) j).getWorldPosition());
            ofCircle(width * projectedJoint.x, height * projectedJoint.y, 10);
        }
        if (users.count(id) == 0)
        {
            Skeleton *newSkeleton = new Skeleton();
            newSkeleton->xnId = id;
            for (int j = 0; j < user->getNumJoints(); j++)
            {
                ofVec2f projectedJoint = openNi.getKinectProjectorToolkit().getProjectedPoint(user->getJoint((Joint) j).getWorldPosition());
                newSkeleton->points.push_back(ofVec2f(width * projectedJoint.x, height * projectedJoint.y));
            }
            newSkeleton->velocity.resize(newSkeleton->points.size());
            users[id] = newSkeleton;
        }
        else
        {
            for (int j = 0; j < user->getNumJoints(); j++)
            {
                ofVec2f projectedJoint = openNi.getKinectProjectorToolkit().getProjectedPoint(user->getJoint((Joint) j).getWorldPosition());
                users[id]->velocity[j].set(ofLerp(users[id]->velocity[j].x, width * projectedJoint.x - users[id]->points[j].x, 0.1),
                                           ofLerp(users[id]->velocity[j].y, height * projectedJoint.y - users[id]->points[j].y, 0.1));
                users[id]->points[j].set(width * projectedJoint.x, height * projectedJoint.y);
            }
        }
    }
    
}

void SkeletonVisual::draw()
{
    ofClear(0, 0);
    
    ofSetColor(255);
    
    map<int, Skeleton*>::iterator it = users.begin();
    for (; it !=users.end(); ++it) {
        
        for (auto p : it->second->points) {
            ofCircle(p.x, p.y, 4);
        }
        
        for (int j=0; j<it->second->points.size(); j++) {
            ofLine(it->second->points[j].x, it->second->points[j].y,
                   it->second->points[j].x + 10*it->second->velocity[j].x, it->second->points[j].y + 10*it->second->velocity[j].y);
        }
        
    }
    
    
}
