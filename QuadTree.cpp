//
// Created by Mert on 3.03.2017.
//

#ifdef QUADTREE_H

#include "QuadTree.h"



template <typename T>
QuadTree<T>::QuadTree(vertex center, vertex range, unsigned bucketSize, unsigned depth){

	root = new QTNode<T>(center, range);
	maxDepth = depth;
	maxBucketSize = bucketSize;
}

template <typename T>
QuadTree<T>::~QuadTree(){
	delete root;
}

template <typename T>
void QuadTree<T>::insert(vertex v, T data){
	insert(v, data, root, 0);
}

template <typename T>
int QuadTree<T>::direction(const vertex& point, QTNode<T>* node){

	unsigned X = 0, Y = 0;
	X |= ((point.x >= node->center.x) << 1);
	Y |= ((point.y >= node->center.y) << 0);

	return (X | Y);
}


template <typename T>
QTNode<T>* QuadTree<T>::childNode(const vertex& v, QTNode<T>* node) {

	unsigned dir = direction(v, node);

	if (node->child[dir]) {
		return node->child[dir];
	}
	else {
		vertex r(node->range.x / 2.0, node->range.y / 2.0);
		node->child[dir] = new QTNode<T>(newCenter(dir, node), r);
		return node->child[dir];
	}

}


template <typename T>
vertex QuadTree<T>::newCenter(int direction, QTNode<T>* node) {

	vertex v(node->center.x, node->center.y);
	switch (direction){

	case LOWER_LEFT_QUAD:
		v.x -= node->range.x / 2.0;
		v.y -= node->range.y / 2.0;
		break;
	case UPPER_LEFT_QUAD:
		v.x -= node->range.x / 2.0;
		v.y += node->range.y / 2.0;
		break;
	case LOWER_RIGHT_QUAD:
		v.x += node->range.x / 2.0;
		v.y -= node->range.y / 2.0;
		break;
	case UPPER_RIGHT_QUAD:
		v.x += node->range.x / 2.0;
		v.y += node->range.y / 2.0;
		break;

	}
	return v;
}

template <typename T>
void QuadTree<T>::insert(vertex v, T data, QTNode<T>* node, unsigned depth){

	if(node->leaf){

		if(node->bucket.size() < maxBucketSize){
			node->bucket.push_back({ v,data });
		}else if(depth < maxDepth){
			node->leaf = false;
			insert(v, data, childNode(v, node), depth + 1);

			for(int i = 0; i < node->bucket.size(); ++i){
				insert(node->bucket[i].first, data, childNode(node->bucket[i].first, node), depth + 1);
			}

			node->bucket.clear();

		}


	}else{
		insert(v, data, childNode(v, node), depth + 1);
	}

}

template <typename T>
string QuadTree<T>::print(){
	stringstream ss;
	print(root, ss);
	return ss.str();
}


template <typename T>
void QuadTree<T>::print(QTNode<T>* node, stringstream& ss)
{

    for(int i = 0; i < 4 ; i++){

        if(node->child[i]){
           print(node->child[i],ss);
        }
    }
    if(node->leaf)
    ss << '{' << node->bucket[0].first.x << ',' << node->bucket[0].first.y << '}' << '\n';

    /*
	for (int i = 0; i < 4; ++i) {
		if (node->child[i]) {
			print(node->child[i], ss);
			for (int j = 0; j < 2 ; j++) {
                 ss << i << ',' << j << ' ' << &node->child[i] << "  " <<'{' << node->child[i]->bucket[j].first.x << ','
					<<  node->child[i]->bucket[j].first.y << '}' << ' ';

                //cout << "{" << node->bucket[i].first.x << "," << node->bucket[i].first.y << "}" << "  ";
			}
			ss << '\n';
		}
	}
	*/


	return;
}

#endif // QUADTREE_H