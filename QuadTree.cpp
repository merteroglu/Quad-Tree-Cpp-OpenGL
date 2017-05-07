#pragma once
#ifdef QUADTREE_H

#include "QuadTree.h"

/*
QuadTree constructor.
@param center: QuadTree'nin merkez koordinat�
@param range: QuadTree'nin geni�li�i
@param bucketSize:Bir yaprakta tutulabilcek obje say�s�
@param depth:Agac�n gidebilce�i maks derinlik
*/
template <typename T>
QuadTree<T>::QuadTree(point merkez, point genislik, unsigned kumeBoyutu, unsigned derinlik)
{
	root = new QTNode <T>(merkez, genislik);
	maxDerinlik = derinlik;
	maxKumeBoyutu = kumeBoyutu;
}

/*
Destructor. Rootu ve ona ba�l� her �eyi sil
*/
template <typename T>
QuadTree<T>::~QuadTree()
{
	delete root;
}

/*
Public ekleme fonksiyonu
@param v : eklencek noktan�n koordinat�
@param data: D���mde tutulacak bilgi
*/
template <typename T>
void QuadTree<T>::ekle(point v, T data)
{
	ekle(v, data, root, 0);
}

/*
Noktan�n d���m�n hangi b�lgesinde oldu�unu bulan fonksiyon
@param point : Nokta
@param node : D���m
*/
template <typename T>
int QuadTree<T>::bolgeNoBul(const point& point, QTNode<T>* node)
{
	unsigned X = 0, Y = 0;
	X |= ((point.x >= node->merkez.x) << 1);
	Y |= ((point.y >= node->merkez.y) << 0);
	return (X | Y);
}

/*
D���m�n �ocuklar�n� d�nd�ren fonksiyon.Gelen noktaya g�re o b�lgede �ocu�u varsa adresi d�ner yoksa olu�turup adresi d�ner
@param v : Noktan�n koordinat�
@param node : D���m
*/
template <typename T>
QTNode<T>* QuadTree<T>::cocukDugumBul(const point& v, QTNode<T>* node)
{
	unsigned dir = bolgeNoBul(v, node);
	if (node->child[dir]) {
		return node->child[dir];
	}else {
		point r(node->genislik.x / 2.0, node->genislik.y / 2.0);
		node->child[dir] = new QTNode<T>(yeniMerkezBul(dir, node), r);
		return node->child[dir];
	}
}

/*
D���m�n �ocu�unun merkezini hesaplayan fonksiyon
@param direction : D���m�n hangi �ocu�u
@param node : D���m
*/
template <typename T>
point QuadTree<T>::yeniMerkezBul(int bolgeNo, QTNode<T>* node)
{
	point v(node->merkez.x, node->merkez.y);
	switch (bolgeNo) {
	case SOL_ALT:
		v.x -= node->genislik.x / 2.0;
		v.y -= node->genislik.y / 2.0;
		break;
	case SOL_UST:
		v.x -= node->genislik.x / 2.0;
		v.y += node->genislik.y / 2.0;
		break;
	case SAG_ALT:
		v.x += node->genislik.x / 2.0;
		v.y -= node->genislik.y / 2.0;
		break;
	case SAG_UST:
		v.x += node->genislik.x / 2.0;
		v.y += node->genislik.y / 2.0;
		break;
	}
	return v;
}

/*
Private ekleme fonksiyonu.Public ekleme fonksiyonundan �a�r�l�r.
@param v : Noktan�n koordinatlar�
@param data: D���mde tutulacak bilgi (Her tipte olabilir)
@param node : D���m
@param depth: D���m�n derinli�i
*/
template <typename T>
void QuadTree<T>::ekle(point v, T data, QTNode<T>* node, unsigned derinlik)
{
	if (node->yaprak) {
		if (node->kume.size() < maxKumeBoyutu) {
			node->kume.push_back({ v, data });
		}else if (derinlik < maxDerinlik) {
			node->yaprak = false;
			ekle(v, data, cocukDugumBul(v, node), derinlik + 1);
			for (int i = 0; i < node->kume.size(); ++i) {
				ekle(node->kume[i].first, data, cocukDugumBul(node->kume[i].first, node), derinlik + 1);
			}
			node->kume.clear();
		}
	}else{
		ekle(v, data, cocukDugumBul(v, node), derinlik + 1);
	}
}

/*
Bolge i�erisinde ki d���mleri bulan fonksiyon
@param minXY : Aranan b�lgenin minimum koordinat�
@param maxXY : Aranan b�lgenin maksimum koordinat�
*/
template <typename T>
vector <pair <point, T> > QuadTree<T>::bolgedekiObjeleriBul(point minXY, point maxXY)
{
	vector <pair <point, T> > sonuclar;
	queue <QTNode<T>*> nodes;
	nodes.push(root);

	while (!nodes.empty()) {
		QTNode<T>* top = nodes.front();
		if (top->yaprak) {
			bolge_durumu durum = bolgeDurumunuBul(top->merkez, top->genislik, minXY, maxXY);
			switch (durum) {
			case bolgede:
				sonuclar.insert(sonuclar.end(), top->kume.begin(), top->kume.end());
				break;
			case  kismen:
				for (int i = 0; i < top->kume.size(); ++i) {
					if (noktaBolgedeMi(top->kume[i].first, minXY, maxXY)) {
						sonuclar.push_back(top->kume[i]);
					}
				}
				break;
			case bolgede_degil:
				break;
			}
		}else{
			for (int i = 0; i < 4; ++i) {
				if (top->child[i]) {
					bolge_durumu durum = bolgeDurumunuBul(top->child[i]->merkez, top->child[i]->genislik, minXY, maxXY);
					switch (durum) {
					case bolgede:
						noktalariSonucaEkle(top->child[i], sonuclar);
						break;
					case kismen:
						nodes.push(top->child[i]);
						break;
					case bolgede_degil:
						break;
					}
				}
			}
		}
		nodes.pop();
	}
	return sonuclar;
}

/*
Bulunan noktalar� sonuca ekleyen fonksiyon
@param node : d���m
@param &results : Noktalar�n eklenece�i liste
*/
template <typename T>
void QuadTree<T>::noktalariSonucaEkle(QTNode<T>* node, vector <pair <point, T> >& sonuc)
{
	if (node->yaprak) {
		sonuc.insert(sonuc.end(), node->kume.begin(), node->kume.end());
	}
	else {
		for (int i = 0; i < 4; ++i) {
			if (node->child[i]) {
				noktalariSonucaEkle(node->child[i], sonuc);
			}
		}
	}
}

/*
Noktan�n o b�lge i�erisinde kal�p kalmad���na bakan fonksiyon
@param point: Noktan�n koordinatlar�
@param minXY : B�lgenin minimum koordinat�
@param maxXY : B�lgenin maksimum koordinat�
*/
template <typename T>
bool QuadTree<T>::noktaBolgedeMi(const point& p, const point& minXY, const point& maxXY)
{
	if ((p.x >= minXY.x) && (p.x < maxXY.x) && (p.y >= minXY.y) && (p.y < maxXY.y)) {
		return true;
	}
	else {
		return false;
	}
}


template <typename T>
bolge_durumu QuadTree<T>::bolgeDurumunuBul(const point& merkez, const point& genislik, const point& minXY, const point& maxXY)
{
	int yakinlik = 0;
	yakinlik += noktaBolgedeMi({ merkez.x - genislik.x, merkez.y - genislik.y }, minXY, maxXY);
	yakinlik += noktaBolgedeMi({ merkez.x - genislik.x, merkez.y + genislik.y }, minXY, maxXY);
	yakinlik += noktaBolgedeMi({ merkez.x + genislik.x, merkez.y - genislik.y }, minXY, maxXY);
	yakinlik += noktaBolgedeMi({ merkez.x + genislik.x, merkez.y + genislik.y }, minXY, maxXY);

	if (yakinlik == 4) {
		return bolgede;
	}
	else if (yakinlik > 0) {
		return kismen;
	}
	else {
		point nodeMin(merkez.x - genislik.x, merkez.y - genislik.y);
		point nodeMax(merkez.x + genislik.x, merkez.y + genislik.y);
		yakinlik += noktaBolgedeMi(minXY, nodeMin, nodeMax);
		yakinlik += noktaBolgedeMi({ minXY.x, maxXY.y }, nodeMin, nodeMax);
		yakinlik += noktaBolgedeMi(maxXY, nodeMin, nodeMax);
		yakinlik += noktaBolgedeMi({ maxXY.x, minXY.y }, nodeMin, nodeMax);
		if (yakinlik > 0) {
			return kismen;
		}
	}
	return bolgede_degil;
}

/*
Public �iz fonksiyonu.Di�er classlardan �a�r�l�r.Private Draw fonksiyonunu �a��r�r
*/
template <typename T>
void QuadTree<T>::ciz()
{
	if(root){
		ciz(root);
	}
}


/*
A�a� yap�s�nda dolu olan t�m d���mlerin karelerini �izer.
@param node : �izilcek d���m
*/
template <typename T>
void QuadTree<T>::ciz(QTNode<T>* node)
{
	
	glBegin (GL_LINE_LOOP);

	glVertex2f (node->merkez.x + node->genislik.x, node->merkez.y + node->genislik.y);
	glVertex2f (node->merkez.x + node->genislik.x, node->merkez.y - node->genislik.y);
	glVertex2f (node->merkez.x - node->genislik.x, node->merkez.y - node->genislik.y);
	glVertex2f (node->merkez.x - node->genislik.x, node->merkez.y + node->genislik.y);
	
	glEnd();
	
	for (int i = 0; i < 4; ++i) {
		if (node->child[i]) {
			ciz(node->child[i]);
		}
	}
}

#endif