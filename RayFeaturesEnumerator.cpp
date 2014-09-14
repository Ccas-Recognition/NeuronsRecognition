//
//  RayFeaturesEnumerator.cpp
//  RayFeaturesProject
//
//  Created by Artem on 25.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#include "RayFeaturesEnumerator.h"


RayFeatureType RayFeaturesEnumerator::next() {
    //RayFeatureType featureType;
    featureType_.coords.first += 2;
    if (featureType_.coords.first >= width_) {
        featureType_.coords.first = 0;
        
        featureType_.coords.second += 2;
        if (featureType_.coords.second >= height_) {
            featureType_.coords.second = 0;
            
            
            if (featureType_.type_name == RF_DISTANCE_DIFFERENCE) {
                featureType_.theta_2 += angle_step_;
                
                if (featureType_.theta_2 >= 360) {
                    featureType_.theta_1 += angle_step_;
                    featureType_.theta_2 = featureType_.theta_1 + angle_step_;
                    
                    if (featureType_.theta_2 >= 360) {
                        featureType_.theta_1 = 0;
                        featureType_.theta_2 = angle_step_;
                        featureType_.type_name = RF_DISTANCE;
                    }
                    
                }
                
            } else {
                featureType_.theta_1 += angle_step_;
                if (featureType_.theta_1 >= 360) {
                    featureType_.theta_1 = 0;
                    if (featureType_.type_name == RF_DISTANCE) {
                        featureType_.type_name = RF_NORM;
                    } else if (featureType_.type_name == RF_NORM) {
                        featureType_.type_name = RF_ORIENTATION;
                    } else if (featureType_.type_name == RF_ORIENTATION) {
                        featureType_.threshold_index += 1;
                        if (featureType_.threshold_index >= num_thresholds_) {
                            featureType_ = this->last();
                        } else {
                            featureType_.type_name = this->first().type_name;
                            featureType_.theta_1 = this->first().theta_1;
                            featureType_.theta_2 = this->first().theta_2;
                        }                       
                    }
                }
            }
            
            
        }
    }
    
    ++feature_no_;
    
    return featureType_;
}



RayFeatureType RayFeaturesEnumerator::first() const {
    return RayFeatureType(std::make_pair(0, 0), RF_DISTANCE_DIFFERENCE, 0, angle_step_, 0);
}

RayFeatureType RayFeaturesEnumerator::last() const {
    return RayFeatureType(std::make_pair(-1, -1), RF_DISTANCE_DIFFERENCE, 0, 0, 0);
}

int RayFeaturesEnumerator::count() const {
    return num_thresholds_ * width_ / 2 * height_ / 2 * (360 / angle_step_) * (3 * 2 +  360 / angle_step_ - 1) / 2;
}