#pragma once
#ifdef QUADTREE_H

#include "QuadTree.h"

/*
QuadTree constructor.
@param center: QuadTree'nin merkez koordinatý
@param range: QuadTree'nin geniþliði
@param bucketSize:Bir yaprakta tutulabilcek obje sayýsý
@param depth:Agacýn gidebilceði maks derinlik
*/
template <typename T>
QuadTree<T>::QuadTree(point merkez, point genislik, unsigned kumeBoyutu, unsigned derinlik)
{
	root = new QTNode <T>(merkez, genislik);
	maxDerinlik = derinlik;
	maxKumeBoyutu = kumeBoyutu;
}

/*
Destructor. Rootu ve ona baðlý her þeyi sil
*/
template <typename T>
QuadTree<T>::~QuadTree()
{
	delete root;
}

/*
Public ekleme fonksiyonu
@param v : eklencek noktanýn koordinatý
@param data: Düðümde tutulacak bilgi
*/
template <typename T>
void QuadTree<T>::ekle(point v, T data)
{
	ekle(v, data, root, 0);
}

/*
Noktanýn düðümün hangi bölgesinde olduðunu bulan fonksiyon
@param point : Nokta
@param node : Düðüm
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
Düðümün çocuklarýný döndüren fonksiyon.Gelen noktaya göre o bölgede çocuðu varsa adresi döner yoksa oluþturup adresi döner
@param v : Noktanýn koordinatý
@param node : Düðüm
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
Düðümün çocuðunun merkezini hesaplayan fonksiyon
@param direction : Düðümün hangi çocuðu
@param node : Düðüm
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
Private ekleme fonksiyonu.Public ekleme fonksiyonundan çaðrýlýr.
@param v : Noktanýn koordinatlarý
@param data: Düðümde tutulacak bilgi (Her tipte olabilir)
@param node : Düðüm
@param depth: Düðümün derinliði
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
Bolge içerisinde ki düðümleri bulan fonksiyon
@param minXY : Aranan bölgenin minimum koordinatý
@param maxXY : Aranan bölgenin maksimum koordinatý
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
Bulunan noktalarý sonuca ekleyen fonksiyon
@param node : düðüm
@param &results : Noktalarýn ekleneceði liste
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
Noktanýn o bölge içerisinde kalýp kalmadýðýna bakan fonksiyon
@param point: Noktanýn koordinatlarý
@param minXY : Bölgenin minimum koordinatý
@param maxXY : Bölgenin maksimum koordinatý
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
Public çiz fonksiyonu.Diðer classlardan çaðrýlýr.Private Draw fonksiyonunu çaðýrýr
*/
template <typename T>
void QuadTree<T>::ciz()
{
	if(root){
		ciz(root);
	}
}


/*
Aðaç yapýsýnda dolu olan tüm düðümlerin karelerini çizer.
@param node : Çizilcek düðüm
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