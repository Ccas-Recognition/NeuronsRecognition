//
//  LabeledSampleElement.h
//  RayFeaturesProject
//
//  Created by Artem on 24.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#ifndef __RayFeaturesProject__LabeledSampleElement__
#define __RayFeaturesProject__LabeledSampleElement__

#include <iostream>

#include <iostream>
#include <vector>
#include <memory>

#include "RayFeatureValues.h"




class LabeledSampleElement {
    std::tr1::shared_ptr<RayFeatures> features_ptr_;
    bool label_;
public:
    float weight;
    
    LabeledSampleElement(const std::tr1::shared_ptr<RayFeatures>& features_ptr, bool label, float element_weight = 1.0f) :
    features_ptr_(features_ptr), label_(label), weight(element_weight) {
        
    }
    
    LabeledSampleElement& operator=(const LabeledSampleElement& rhs) {
        
        // Only do assignment if RHS is a different object from this.
        if (this != &rhs) {
            this->features_ptr_ = rhs.features_ptr_;
            this->label_ = rhs.label_;
            this->weight = rhs.weight;
        }
        
        return *this;
    }
    
    
    std::tr1::shared_ptr<RayFeatures> features_ptr() const {
        return features_ptr_;
    }
    
    void set_features_ptr(const std::tr1::shared_ptr<RayFeatures>& features_ptr) {
        features_ptr_ = features_ptr;
    }
    
    bool label() const {
        return label_;
    }
    
    void set_label(bool label) {
        label_ = label;
    }
    
    bool operator==(const LabeledSampleElement &other) const {
        return (features_ptr_.get() == other.features_ptr_.get()) && (this->label_ == other.label_);
    }
};

typedef std::tr1::shared_ptr<LabeledSampleElement> LabeledSampleElement_ptr;

class LabeledSampleElementLess : public std::binary_function<LabeledSampleElement_ptr, LabeledSampleElement_ptr, bool> {
    const RayFeatureType feature_type_;
public:
    LabeledSampleElementLess(const RayFeatureType& feature_type) : feature_type_(feature_type) {
        
    }
    bool operator()(const LabeledSampleElement_ptr& lhs, const LabeledSampleElement_ptr& rhs) const {        
        return GetFeatureValueByType(lhs->features_ptr(), feature_type_) < GetFeatureValueByType(rhs->features_ptr(), feature_type_);
    }
};

class LabeledSampleElementWeightLess : public std::binary_function<LabeledSampleElement_ptr, LabeledSampleElement_ptr, bool> {
public:
    bool operator()(const LabeledSampleElement_ptr& lhs, const LabeledSampleElement_ptr& rhs) const {
        return lhs->weight < rhs->weight;
    }
};

class LabeledSampleElementWeightSum : public std::unary_function<LabeledSampleElement_ptr, void>{
    const bool summarized_label_;
    float sum_;
public:
    LabeledSampleElementWeightSum(bool summarized_label) : summarized_label_(summarized_label), sum_(0.0f) {
        
    }
    void operator()(const LabeledSampleElement_ptr& sample_element_ptr)  {
        if (sample_element_ptr->label() == summarized_label_) {
            sum_ += sample_element_ptr->weight;
        }
    }
    float result() const {
        return sum_;
    }
};

class DivideLabeledSampleElementWeight : public std::unary_function<LabeledSampleElement_ptr, void>{
    const float divisor_;
public:
    DivideLabeledSampleElementWeight(float divisor) : divisor_(divisor) {
        
    }
    void operator()(LabeledSampleElement_ptr& sample_element_ptr)  {
        if (divisor_ > std::numeric_limits<float>::epsilon()) {
            sample_element_ptr->weight /= divisor_;
        }
    }
};



#endif /* defined(__RayFeaturesProject__LabeledSampleElement__) */
