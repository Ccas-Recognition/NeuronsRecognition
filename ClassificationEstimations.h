#ifndef CLASSIFICATIONESTIMATIONS_H
#define CLASSIFICATIONESTIMATIONS_H

#include <vector>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

class ClassifierCascade;
class RayFeaturesPrecomputer;

class ClassificationEstimations
{
    ClassifierCascade *cascade;
    RayFeaturesPrecomputer *features_precomputer;
    int sparce_number;
public:
    ClassificationEstimations(int _sparce_number = 1);
    void getParts( vector<cv::Mat> &parts, string xml_path );
    int classify( const cv::Mat& part );
    float getError( const vector<cv::Mat> &parts, int answer );

};

#endif // CLASSIFICATIONESTIMATIONS_H
