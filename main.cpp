// QuadTreeDeneme.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <conio.h>
#include "QuadTree.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

QuadTree <int>* qtree;

long double randomFloat() {
    long double x = ((long double)rand() / (long double)1000);
    return x;
}


int main()
{
    cout << "#### Paxwel QuadTree ####\n";
    srand(time(0));

    vertex origin(0, 0);
    vertex axis(1024.0, 1024.0);
    static int bucketSize = 1;


    qtree = new QuadTree<int>(origin, axis, 1,16);


/*
	for (int i = 0; i < 3; i++) {
        vertex newpoint(rand()%50,rand()%50);
		qtree->insert(newpoint, 1);
	}
*/
    /*
780,1010
910,1000
850,880
860,810
100,150
310,400
400,800
600,620
512,512
0,1024
0,0
1024,0
1024,1024
     */

    vertex nok1(780,1010);
    vertex nok2(910,1000);
    vertex nok3(850,880);
    vertex nok4(860,810);
    vertex nok5(100,150);
    vertex nok6(310,400);
    vertex nok7(400,800);
    vertex nok8(600,620);
    vertex nok9(512,512);
    vertex nok10(0,1024);
    vertex nok11(0,0);
    vertex nok12(1024,0);
    vertex nok13(1024,1024);

    qtree->insert(nok1,1);
    qtree->insert(nok2,1);
    qtree->insert(nok3,1);
    qtree->insert(nok4,1);
    qtree->insert(nok5,1);
    /*
    qtree->insert(nok6,1);
    qtree->insert(nok7,1);
    qtree->insert(nok8,1);
    qtree->insert(nok9,1);
    qtree->insert(nok10,1);
    qtree->insert(nok11,1);
    qtree->insert(nok12,1);
    qtree->insert(nok13,1);
*/
    cout << qtree->print();


    _getch();
    return 0;
}
