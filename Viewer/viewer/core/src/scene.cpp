#include "scene.h"

Scene::Scene() : pselectedObject(0)  // inicialment l'objecte seleccionat és el 0
{}

Box Scene::boundingBox() const
{
    return pboundingBox;
}

const vector<Object>& Scene::objects() const
{
    return pobjects;
}

vector<Object>& Scene::objects()
{
    return pobjects;
}
    
void Scene::addObject(Object &o)
{
    pobjects.push_back(o);
    computeBoundingBox();
}

void Scene::clear()
{
    pobjects.clear();
}


void Scene::computeBoundingBox()
{
    if (pobjects.size())
    {
        pboundingBox = pobjects[0].boundingBox();
        for (unsigned int i=1; i<pobjects.size(); ++i)
            pboundingBox.expand(pobjects[i].boundingBox()); 
    }
}

int Scene::selectedObject() const 
{
    if (pobjects.size()) return pselectedObject;
    else return -1; 
}

void Scene::setSelectedObject(int index)
{
    pselectedObject = index;
}
    

