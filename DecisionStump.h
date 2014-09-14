//
//  DecisionStump.h
//  RayFeaturesProject
//
//  Created by Artem on 18.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#ifndef __RayFeaturesProject__DecisionStump__
#define __RayFeaturesProject__DecisionStump__

#include <iostream>
#include <memory>
#include <utility>
#include <opencv2/core/core.hpp>
#include "LabeledSampleElement.h"
#include "RayFeatureType.h"
#include "RayFeatureValues.h"

class DecisionStump {
    RayFeatureType featureType_;
    float threshold_;
    int polarity_;
public:
    DecisionStump(const RayFeatureType& featureType = RayFeatureType(), float threshold = 0.0, int polarity = 1) : featureType_(featureType), threshold_(threshold), polarity_(polarity) {
        
    }
    int polarity() const {
        return polarity_;
    }
    float threshold() const {
        return threshold_;
    }
    RayFeatureType featureType() const {
        return featureType_;
    }
    
    // Train desicion stump, returns weighted error
    // training sample will generally be shuffled duiring training, weights will be normalized
    float train(std::vector<LabeledSampleElement_ptr>* training_sample_ptr);
    
    // Train desicion stump, returns weighted error
    // weights will be normalized, feature_index contains feature sorting order
    float train_with_index(std::vector<LabeledSampleElement_ptr>* training_sample_ptr, const std::vector<int>& feature_index);
    
    bool decision(std::tr1::shared_ptr<RayFeatures> features_ptr) const;
    
    
    void write(cv::FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" ;
        fs << "featureType";
        featureType_.write(fs);
        fs << "threshold" << threshold_;
        fs << "polarity" << polarity_;
        fs << "}";
    }
    
    void read(const cv::FileNode& node)                          //Read serialization for this class
    {
        featureType_.read(node["featureType"]);
        threshold_ = (float)node["threshold"];
        polarity_ = (int)node["polarity"];
        
    }
    
};

std::ostream& operator<<(std::ostream& os, const DecisionStump& decision_stump);

#endif /* defined(__RayFeaturesProject__DecisionStump__) */
