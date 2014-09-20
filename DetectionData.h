#ifndef DETECTIONDATA_H
#define DETECTIONDATA_H

#include <vector>
#include <QRect>
#include <QPoint>

using std::vector;

struct DetectionData {
    vector< QRect > rectangles;
    vector< QRect > old_rectangles;
    vector< int > colors;
    vector< vector< QPoint > > boundaries;
};

#endif // DETECTIONDATA_H
