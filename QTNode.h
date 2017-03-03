#include <cstdlib>
#include <vector>
#include "Vertex.h"

using namespace std;

template <class T>
class QTNode{

public:

    vertex center, range;


    bool leaf;
    QTNode* child[4];


    vector < pair<vertex, T> > bucket;


    QTNode <T>(vertex newCenter, vertex newRange) {

        for (int i = 0; i < 4; i++)
            child[i] = NULL;

        center = newCenter;
        range = newRange;
        leaf = true;

    }


    ~QTNode()
    {
        for (int i = 0; i < 4; i++) {
            if (child[i])
                delete child[i];
        }

    }

};