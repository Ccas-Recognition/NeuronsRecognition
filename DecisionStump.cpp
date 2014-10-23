//
//  DecisionStump.cpp
//  RayFeaturesProject
//
//  Created by Artem on 18.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#include <algorithm>
#include <limits>
#include <numeric>
#include <functional>
#include "DecisionStump.h"




float DecisionStump::train(std::vector<LabeledSampleElement_ptr>* training_sample_ptr) {
    // Normalize weights
    float positive_sum = std::for_each(training_sample_ptr->begin(), training_sample_ptr->end(), LabeledSampleElementWeightSum(true)).result();
    float negative_sum = std::for_each(training_sample_ptr->begin(), training_sample_ptr->end(), LabeledSampleElementWeightSum(false)).result();
    
    float total_sum = positive_sum + negative_sum;
    
    std::for_each(training_sample_ptr->begin(), training_sample_ptr->end(), DivideLabeledSampleElementWeight(total_sum));    
    positive_sum /= total_sum;
    negative_sum /= total_sum;
    total_sum = positive_sum + negative_sum;
    
    //Sorting features by value
    
    LabeledSampleElementLess compare_less = LabeledSampleElementLess(this->featureType_);
    
    std::sort(training_sample_ptr->begin(), training_sample_ptr->end(), compare_less);
  
    float min_error = total_sum;
    this->threshold_ = GetFeatureValueByType(training_sample_ptr->at(0)->features_ptr(), this->featureType_);
    this->polarity_ = 1;
    
    std::vector<LabeledSampleElement_ptr>::iterator curr_thresh_sample_it = training_sample_ptr->begin();
    std::vector<LabeledSampleElement_ptr>::iterator next_thresh_sample_it = training_sample_ptr->end();
    
    
    float current_positive_sum = 0.0f;
    float current_negative_sum = 0.0f;
    
    while (curr_thresh_sample_it != training_sample_ptr->end()) {
        next_thresh_sample_it = std::upper_bound(curr_thresh_sample_it, training_sample_ptr->end(), *curr_thresh_sample_it, compare_less);
        
        current_positive_sum += std::for_each(curr_thresh_sample_it, next_thresh_sample_it, LabeledSampleElementWeightSum(true)).result();
        current_negative_sum += std::for_each(curr_thresh_sample_it, next_thresh_sample_it, LabeledSampleElementWeightSum(false)).result();
        
        
      
        if (next_thresh_sample_it == training_sample_ptr->end()) {
            break;
        } else {
            curr_thresh_sample_it = next_thresh_sample_it;
        }
               
        if ((current_negative_sum + positive_sum - current_positive_sum) < min_error) {
            min_error = current_negative_sum + positive_sum - current_positive_sum;
            this->polarity_ = 1;
            this->threshold_ = GetFeatureValueByType(next_thresh_sample_it->get()->features_ptr(), this->featureType_);
        }
        
        if ((current_positive_sum + negative_sum - current_negative_sum) < min_error) {
            min_error = current_positive_sum + negative_sum - current_negative_sum;
            this->polarity_ = -1;
            this->threshold_ = GetFeatureValueByType(next_thresh_sample_it->get()->features_ptr(), this->featureType_);
        }
    }
    
    return min_error;
}

float DecisionStump::train_with_index(std::vector<LabeledSampleElement_ptr>* training_sample_ptr, const std::vector<int>& feature_index) {
    // Normalize weights
    float positive_sum = std::for_each(training_sample_ptr->begin(), training_sample_ptr->end(), LabeledSampleElementWeightSum(true)).result();
    float negative_sum = std::for_each(training_sample_ptr->begin(), training_sample_ptr->end(), LabeledSampleElementWeightSum(false)).result();
    
    float total_sum = positive_sum + negative_sum;
    
    std::for_each(training_sample_ptr->begin(), training_sample_ptr->end(), DivideLabeledSampleElementWeight(total_sum));
    positive_sum /= total_sum;
    negative_sum /= total_sum;
    total_sum = positive_sum + negative_sum;
    
    //Sorting features by value
    LabeledSampleElementLess compare_less = LabeledSampleElementLess(this->featureType_);
    
    
    //Instead of sorting assert right order
    
    assert(training_sample_ptr->size() == feature_index.size());
    
    for (size_t ind = 1; ind < training_sample_ptr->size(); ++ind) {
        assert(!compare_less(training_sample_ptr->at(feature_index[ind]), training_sample_ptr->at(feature_index[ind - 1])));
    }
    
    
    
    
    float min_error = total_sum;
    this->threshold_ = GetFeatureValueByType(training_sample_ptr->at(feature_index[0])->features_ptr(), this->featureType_);
    this->polarity_ = 1;
    
    
    int curr_thresh_sample_ind = 0;
    int next_thresh_sample_ind = static_cast<int>(feature_index.size());
    
 
    
    
    float current_positive_sum = 0.0f;
    float current_negative_sum = 0.0f;
    
    while (curr_thresh_sample_ind < static_cast<int>(feature_index.size())) {
        
        // searcing for the upper bound
        
        int first = curr_thresh_sample_ind;
        int last = static_cast<int>(feature_index.size());
        
        int count = last - first;
        
        while (count > 0) {
            int step = count / 2;
            int middle = first + step;
            if (!compare_less(training_sample_ptr->at(feature_index[curr_thresh_sample_ind]), training_sample_ptr->at(feature_index[middle]))) {
                first = middle + 1;
                count -= step + 1;
            } else {
                count = step;
            }
        }
        
        next_thresh_sample_ind = first;
        
        for (int ind = curr_thresh_sample_ind; ind < next_thresh_sample_ind; ++ind) {
            if (training_sample_ptr->at(feature_index[ind])->label()) {
                current_positive_sum += training_sample_ptr->at(feature_index[ind])->weight;
            } else {
                current_negative_sum += training_sample_ptr->at(feature_index[ind])->weight;
            }
        }
      
        
       
        
        
        
        if (next_thresh_sample_ind == static_cast<int>(feature_index.size())) {
            break;
        } else {
            curr_thresh_sample_ind = next_thresh_sample_ind;
        }
        
        if ((current_negative_sum + positive_sum - current_positive_sum) < min_error) {
            min_error = current_negative_sum + positive_sum - current_positive_sum;
            this->polarity_ = 1;
            this->threshold_ = GetFeatureValueByType(training_sample_ptr->at(feature_index[curr_thresh_sample_ind])->features_ptr(), this->featureType_);
        }
        
        if ((current_positive_sum + negative_sum - current_negative_sum) < min_error) {
            min_error = current_positive_sum + negative_sum - current_negative_sum;
            this->polarity_ = -1;
            this->threshold_ = GetFeatureValueByType(training_sample_ptr->at(feature_index[curr_thresh_sample_ind])->features_ptr(), this->featureType_);
        }
    }
    
    return min_error;

}

bool DecisionStump::decision(std::shared_ptr<RayFeatures> features_ptr) const {
    float val = GetFeatureValueByType(features_ptr, this->featureType_);
    
    return (polarity_ * val < polarity_ * threshold_);
}

std::ostream& operator<<(std::ostream& os, const DecisionStump& decision_stump) {
    os << "DecisionStump : " << decision_stump.polarity() << " * f() < " << decision_stump.polarity() << " * " << decision_stump.threshold() << std::endl;
    os << decision_stump.featureType();
    
    return os;
}
