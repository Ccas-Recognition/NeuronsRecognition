//
//  RayFeatureValues.cpp
//  RayFeaturesProject
//
//  Created by Artem on 24.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#include <limits>
#include "RayFeatureValues.h"


RayFeatureValues& GetFeatureValuesByInd(const std::shared_ptr<RayFeatures>& features_ptr, int threshold_index, int row, int col, int angle_index) {
    
    assert((threshold_index >= 0) && (threshold_index < (*features_ptr).size()));
    assert((row >= 0) && (row < (*features_ptr)[0].size()));
    assert((col >= 0) && (col < (*features_ptr)[0][0].size()));
    assert((angle_index >= 0) && (angle_index < (*features_ptr)[0][0][0].size()));
    
    RayFeatureValues& featureVals = (*features_ptr)[threshold_index][row][col][angle_index];
    return featureVals;
}

float GetFeatureValueByType(const std::shared_ptr<RayFeatures>& features_ptr, const RayFeatureType& feature_type) {
    
    assert((360 % (int)(*features_ptr)[0][0][0].size()) == 0);
    
    int angle_step = 360 / (int)(*features_ptr)[0][0][0].size();
        
    RayFeatureValues featureVals_1 = GetFeatureValuesByInd(features_ptr, feature_type.threshold_index, feature_type.coords.second, feature_type.coords.first, feature_type.theta_1 / angle_step);
    
    switch (feature_type.type_name) {
        case RF_DISTANCE:
            return sqrtf(featureVals_1.distance_squared);
            break;
        case RF_NORM:
            return sqrtf(featureVals_1.norm_squared);
            break;
        case RF_ORIENTATION:
            return cosf(featureVals_1.orientation);
            break;
        case RF_DISTANCE_DIFFERENCE: {
            RayFeatureValues featureVals_2 = GetFeatureValuesByInd(features_ptr, feature_type.threshold_index, feature_type.coords.second, feature_type.coords.first, feature_type.theta_2 / angle_step);
            
            if (featureVals_1.distance_squared > 0) {
                float val_1 = sqrtf(featureVals_1.distance_squared);
                float val_2 = sqrtf(featureVals_2.distance_squared);
                return (val_1 - val_2) / val_1;
            } else {
                if (featureVals_2.distance_squared > 0) {
                    return -std::numeric_limits<float>::max();
                } else {
                    return std::numeric_limits<float>::max();
                }
            }
            break;
        }
        default:
            assert(-1);
            break;
    }

}