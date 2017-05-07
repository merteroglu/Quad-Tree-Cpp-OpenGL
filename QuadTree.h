/**
QuadTree.h
Aðaç yapýsýnýn oluþturulduðu class. Aðaca ekleme arama iþlemleri bu classta yapýlýr
**/

#ifndef QUADTREE_H
#define QUADTREE_H

#include <cstdlib>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>

#include "QTNode.h"
#include "Point.h"

using namespace std;

#define SOL_ALT 0
#define SOL_UST 1
#define SAG_ALT 2
#define SAG_UST 3

enum bolge_durumu
{
	bolgede_degil,
	kismen,
	bolgede
};

template <typename T>
class QuadTree
{
public:

	QuadTree <T>(point merkez, point genislik, unsigned kumeBoyutu = 1, unsigned derinlik = 16);
	~QuadTree();

	void 	ekle(point v, T data);
	void 	ciz();
	vector <pair <point, T> > bolgedekiObjeleriBul(point minXY, point maxXY);

private:

	QTNode<T>* cocukDugumBul(const point& v, QTNode<T>* node);
	point   yeniMerkezBul(int bolgeNo, QTNode <T>* node);
	int 	bolgeNoBul(const point& point, QTNode <T>* node);
	void 	ekle(point v, T data, QTNode<T>* node, unsigned derinlik);
	void 	ciz(QTNode<T>* node);
	void	noktalariSonucaEkle(QTNode<T>* node, vector <pair <point, T> >& sonuc);
	bool    noktaBolgedeMi(const point& p, const point& minXY, const point& maxXY);
	bolge_durumu bolgeDurumunuBul(const point& merkez, const point& genislik, const point& minXY, const point& maxXY);

	QTNode<T>* root;
	unsigned maxDerinlik, maxKumeBoyutu;
};


#include "QuadTree.cpp"
#endif //#ifdef QUADTREE_H