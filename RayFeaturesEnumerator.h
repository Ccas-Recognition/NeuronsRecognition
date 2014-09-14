//
//  RayFeaturesEnumerator.h
//  RayFeaturesProject
//
//  Created by Artem on 25.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#ifndef __RayFeaturesProject__RayFeatureEnumerator__
#define __RayFeaturesProject__RayFeatureEnumerator__

#include <iostream>
#include <utility>
#include "RayFeatureType.h"

class RayFeaturesEnumerator {
    int width_;
    int height_;
    int angle_step_;
    int num_thresholds_;
    int feature_no_;
    
    RayFeatureType featureType_;
    
public:
    RayFeaturesEnumerator(int width, int height, int angle_step, int num_thresholds) :
    width_(width),
    height_(height),
    angle_step_(angle_step % 360),
    feature_no_(0),
    num_thresholds_(num_thresholds),
    featureType_(RayFeatureType(std::make_pair(0, 0), RF_DISTANCE_DIFFERENCE, 0, angle_step % 360, 0)) {
        
    }
    RayFeatureType first() const;
    RayFeatureType next();
    RayFeatureType last() const;
    void reInit() {
        feature_no_ = 0;
        featureType_ = this->first();
    }
    int fetureNo() const {
        return feature_no_;
    }
    int count() const;
};


#endif /* defined(__RayFeaturesProject__RayFeatureEnumerator__) */
