//
//  RayFeatureValues.h
//  RayFeaturesProject
//
//  Created by Artem on 24.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#ifndef __RayFeaturesProject__RayFeatureValues__
#define __RayFeaturesProject__RayFeatureValues__

#include <iostream>
#include <vector>
#include <memory>
#include "RayFeatureType.h"

class RayFeatureValues {
public:
    unsigned int distance_squared;
    unsigned int norm_squared;
    int orientation; // angle in degrees between orientation and gradient orientation
    
    
    RayFeatureValues(unsigned int dist_sq = 0, unsigned int norm_sq = 0, int orient = 0) : distance_squared(dist_sq), norm_squared(norm_sq), orientation(orient) {
    }
    
    
    RayFeatureValues& operator=(const RayFeatureValues& rhs) {
        
        // Only do assignment if RHS is a different object from this.
        if (this != &rhs) {
            this->distance_squared = rhs.distance_squared;
            this->norm_squared = rhs.norm_squared;
            this->orientation = rhs.orientation;
        }
        
        return *this;
    }
    
    RayFeatureValues& operator+=(const RayFeatureValues& rhs) {
        this->distance_squared += rhs.distance_squared;
        this->norm_squared += rhs.norm_squared;
        this->orientation += rhs.orientation;
        
        return *this;
    }
    
    const RayFeatureValues operator+(const RayFeatureValues& other) const {
        return RayFeatureValues(*this) += other;
    }
};

// treshold, row, col, angle
typedef std::vector<std::vector<std::vector<std::vector<RayFeatureValues> > > > RayFeatures;

RayFeatureValues& GetFeatureValuesByInd(const std::tr1::shared_ptr<RayFeatures>& features_ptr, int threshold_index, int row, int col, int angle_index);

float GetFeatureValueByType(const std::tr1::shared_ptr<RayFeatures>& features_ptr, const RayFeatureType& feature_type);



#endif /* defined(__RayFeaturesProject__RayFeatureValues__) */
