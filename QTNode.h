/*
Quadtree de ki her bir düðümün classý
Düðümlerde merkez ve geniþlik bilgileri tutulur
*/
#pragma once

#include <cstdlib>
#include <vector>
#include "Point.h"
using namespace std;

template <class T>
class QTNode {

public:

	QTNode <T>(point yeniMerkez, point yeniGenislik) {
		for (int i = 0; i < 4; i++)
			child[i] = NULL;
		
		merkez = yeniMerkez;
		genislik = yeniGenislik;
		yaprak = true;
	}

	~QTNode() { 
		for (int i = 0; i < 4; ++i) 
			if (child[i]) { 
				delete child[i]; 
			} 
		}

	point merkez, genislik;

	
	bool yaprak;
	QTNode* child[4];

	
	vector <pair <point, T> > kume;

};