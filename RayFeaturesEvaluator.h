//
//  RayFeaturesEvaluator.h
//  RayFeaturesProject
//
//  Created by Artem on 18.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#ifndef __RayFeaturesProject__RayFeaturesEvaluator__
#define __RayFeaturesProject__RayFeaturesEvaluator__

#include <iostream>
#include <memory>
#include "RayFeaturesPrecomputer.h"



class RayFeaturesEvaluator {
    std::shared_ptr<RayFeatures> features_ptr_;
public:
    RayFeaturesEvaluator(std::shared_ptr<RayFeatures> features_ptr) : features_ptr_(features_ptr) {
    }
    
};

#endif /* defined(__RayFeaturesProject__RayFeaturesEvaluator__) */
