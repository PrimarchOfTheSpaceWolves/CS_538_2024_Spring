#pragma once 
 
#include "PotatoRenderEngine.hpp" 
#include "Rasterize.hpp" 
#include <thread>
using namespace std; 
using namespace potato; 
using namespace tinyxml2; 
 
class PotatoForwardEngine : public PotatoRenderEngine { 
    private: 
        // List of triangle meshes 
        vector<PolyMesh*> allMeshes; 
        vector<Fragment> allFragments; 

        const int MAX_THREAD_CNT = 5;
        vector<thread*> allThreads;
        bool stillWorking = true;
        vector<int> rowsToDo;
        mutex rowListLock;

        void threadWorkerFunc();
 
        // Merge fragments 
        void mergeFragments(vector<Fragment> &fragList, Image<Vec3f> *drawBuffer); 
 
    public: 
        PotatoForwardEngine(int windowWidth, int windowHeight); 
        virtual ~PotatoForwardEngine(); 
        virtual void renderToDrawBuffer(Image<Vec3f> *drawBuffer); 
}; 
