//
// Created by Mert on 3.03.2017.
//

#pragma once

class vertex {

public:

    long double x;
    long double y;

    vertex() {
        x = 0;
        y = 0;
    }

    vertex(long double newX, long double newY) {
        x = newX;
        y = newY;
    }

    ~vertex()
    {

    }

    bool operator == (vertex v){
        return ((x == v.x)&&(y == v.y));
    }


};
