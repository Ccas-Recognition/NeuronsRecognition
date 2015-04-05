#ifndef DETECTIONDATA_H
#define DETECTIONDATA_H

#include <vector>
#include <QRect>
#include <QPoint>
#include <QString>

using std::vector;

struct DetectionData {
    vector< QRect > rectangles;
    vector< QRect > old_rectangles;
    vector< int > colors;
    vector< vector< QPoint > > boundaries;
    vector< QPoint > lines;

    QString filePath;

    DetectionData(QString q=""): filePath(q) {}
};

#endif // DETECTIONDATA_H
