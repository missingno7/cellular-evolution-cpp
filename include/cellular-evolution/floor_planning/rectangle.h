#pragma once
//
// Created by jiri on 14/11/19.
//

class Rectangle {
public:
    void flip()
    {
        int tmp=x2;
        x2=x1+(y2-y1);
        y2=y1+(tmp-x1);
    }

    void copyTo(Rectangle *rec){
        rec->x1=x1;
        rec->x2=x2;
        rec->y1=y1;
        rec->y2=y2;
    }

    int x1;
    int y1;
    int x2;
    int y2;
};
