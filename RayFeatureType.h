//
//  RayFeatureType.h
//  RayFeaturesProject
//
//  Created by Artem on 18.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#ifndef __RayFeaturesProject__RayFeatureType__
#define __RayFeaturesProject__RayFeatureType__

#include <iostream>
#include <opencv2/core/core.hpp>

enum RayFeatureTypeName {
    RF_DISTANCE_DIFFERENCE = 0,
    RF_DISTANCE = 1,
    RF_ORIENTATION = 2,
    RF_NORM = 3
};

class RayFeatureType {
public:
    RayFeatureType(std::pair<int, int> coordinates = std::make_pair(0, 0), RayFeatureTypeName feature_type_name = RF_DISTANCE_DIFFERENCE, int theta1 = 30, int theta2 = 0, int thresh_ind = 0);
    std::pair<int, int> coords;
    RayFeatureTypeName type_name;
    int theta_1;
    int theta_2;
    int threshold_index;
    
    bool operator==(const RayFeatureType &other) const;
    bool operator!=(const RayFeatureType &other) const;
    
    
    void write(cv::FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" ;
        fs << "x" << coords.first;
        fs << "y" << coords.second;
        fs << "type_name" << type_name;
        fs << "theta_1" << theta_1;
        fs << "theta_2" << theta_2;
        fs << "threshold_index" << threshold_index;
        fs << "}";
    }
    
    void read(const cv::FileNode& node)                          //Read serialization for this class
    {
        
        coords.first = (int)node["x"];
        coords.second = (int)node["y"];
        type_name = (RayFeatureTypeName)((int)node["type_name"]);
        theta_1 = (int)node["theta_1"];
        theta_2 = (int)node["theta_2"];
        threshold_index = (int)node["threshold_index"];
    }
    
    
};

std::ostream& operator<<(std::ostream& os, const RayFeatureType& featureType);


#endif /* defined(__RayFeaturesProject__RayFeatureType__) */
