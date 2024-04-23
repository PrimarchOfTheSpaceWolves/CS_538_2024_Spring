#include "PotatoForwardEngine.hpp" 
 
PotatoForwardEngine::PotatoForwardEngine(int windowWidth, int windowHeight) : PotatoRenderEngine(windowWidth, windowHeight) { 
    // For now, generate simple fan 
    PolyMesh *m = generateTestFan(Vec3f(windowWidth/2.0f, windowHeight/2.0f, 0.0f), 
                                    windowHeight/3.0f, GEO_FAN_BLADE_CNT); 
    allMeshes.push_back(m); 

    for(int i = 0; i < MAX_THREAD_CNT; i++) {
        allThreads.push_back(new thread(&PotatoForwardEngine::threadWorkerFunc, this));
    }
} 

void PotatoForwardEngine::threadWorkerFunc() {
    while(stillWorking) {
        
        if(rowsToDo.size() > 0) {
            rowListLock.lock();
            if(rowsToDo.size() > 0) {
                int row = rowsToDo[rowsToDo.size()-1];
                rowsToDo.pop_back();
                cout << "Thread working on " << row << endl;
            }
            rowListLock.unlock();
        }
        else {
            this_thread::sleep_for(chrono::milliseconds(50));
        }        
    }
}
 
PotatoForwardEngine::~PotatoForwardEngine() { 
    stillWorking = false;
    for(int i = 0; i < allThreads.size(); i++) {
        allThreads[i]->join();
        delete allThreads[i];
    }
    allThreads.clear();

    // Clean up meshes 
    for(int i = 0; i < allMeshes.size(); i++) { 
        delete allMeshes.at(i); 
    } 
    allMeshes.clear(); 
 
    allFragments.clear(); 
} 
 
void PotatoForwardEngine::mergeFragments(vector<Fragment> &fragList, Image<Vec3f> *drawBuffer) { 
    // For now, just blindly write all fragments to buffer 
    for(int i = 0; i < fragList.size(); i++) { 
        Fragment f = fragList.at(i); 
        drawBuffer->setPixel(f.pos.x, f.pos.y, Vec3f(f.color)); 
    } 
} 
 
void PotatoForwardEngine::renderToDrawBuffer(Image<Vec3f> *drawBuffer) { 
 
    rowListLock.lock();
    for(int i = 0; i < 10; i++) {
        rowsToDo.push_back(i);
    }
    rowListLock.unlock();

    // Clear list of fragments 
    allFragments.clear(); 
 
    for(int i = 0; i < allMeshes.size(); i++) { 
        PolyMesh *mesh = allMeshes.at(i); 
 
        // Get fragments for inside of polygons 
        fillTriangles(mesh, allFragments); 
 
        // Get fragments for lines 
        drawLines(mesh, allFragments, DRAW_LINES_AS_WIREFRAME); 
    } 
 
    // Merge fragments 
    mergeFragments(allFragments, drawBuffer); 

    while(rowsToDo.size() != 0) {
        this_thread::sleep_for(chrono::milliseconds(20));
    }
} 
 
