//
//  RayFeatureType.cpp
//  RayFeaturesProject
//
//  Created by Artem on 18.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#include "RayFeatureType.h"

RayFeatureType::RayFeatureType(std::pair<int, int> coordinates, RayFeatureTypeName feature_type_name, int theta1, int theta2, int thresh_ind) : coords(coordinates), type_name(feature_type_name), theta_1(theta1 % 360), theta_2(theta2 % 360), threshold_index(thresh_ind) {
    
}

bool RayFeatureType::operator==(const RayFeatureType &other) const {
    if (other.type_name == RF_DISTANCE_DIFFERENCE) {
         return (this->coords == other.coords) && (this->type_name == other.type_name) && (this->theta_1 == other.theta_1) && (this->threshold_index == other.threshold_index) && (this->theta_2 == other.theta_2);
    } else {
         return (this->coords == other.coords) && (this->type_name == other.type_name) && (this->theta_1 == other.theta_1)  && (this->threshold_index == other.threshold_index);
    }
}

bool RayFeatureType::operator!=(const RayFeatureType &other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const RayFeatureType& featureType) {
    os << "f(): ";
    switch (featureType.type_name) {
        case RF_DISTANCE:
            os << "RF_DISTANCE";
            break;
        case RF_NORM:
            os << "RF_NORM";
            break;
        case RF_ORIENTATION:
            os << "RF_ORIENTATION";
            break;
        case RF_DISTANCE_DIFFERENCE:
            os << "RF_DISTANCE_DIFFERENCE";
            break;
        default:
            break;
    }
    
    os << " with x = " << featureType.coords.first << " y = " << featureType.coords.second;
    os << " theta_1 = " << featureType.theta_1;
    
    if (featureType.type_name == RF_DISTANCE_DIFFERENCE) {
        os << " theta_2 = " << featureType.theta_2;
    }
    
    os << " threshold_index = " << featureType.threshold_index;
    
    return os;
}