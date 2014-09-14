//
//  BoostingClassifier.cpp
//  RayFeaturesProject
//
//  Created by Artem on 18.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#include <limits>
#include <numeric>
#include <functional>
#include <cmath>

#include "RayFeaturesEnumerator.h"
#include "BoostingClassifier.h"


class LabeledSampleElementCount : public std::unary_function<LabeledSampleElement_ptr, void>{
    const bool counted_label_;
    unsigned int count_;
public:
    LabeledSampleElementCount(bool counted_label) : counted_label_(counted_label), count_(0) {
        
    }
    void operator()(const LabeledSampleElement_ptr& sample_element_ptr)  {
        if (sample_element_ptr->label() == counted_label_) {
            ++count_;
        }
    }
    unsigned int result() const {
        return count_;
    }
};

void BoostingClassifier::initTraining() {
    this->isTrainded_ = false;
    
    
    this->buildIndex();
    
    unsigned int positive_count = std::for_each(training_sample_.begin(), training_sample_.end(), LabeledSampleElementCount(true)).result();
    unsigned int negative_count = std::for_each(training_sample_.begin(), training_sample_.end(), LabeledSampleElementCount(false)).result();
    
    assert(positive_count > 0);
    assert(negative_count > 0);
    
    float positive_weight =  1.0f / (2.0f * positive_count);
    float negative_weight =  1.0f / (2.0f * negative_count);
    
    
    for (std::vector<LabeledSampleElement_ptr>::iterator training_sample_it = training_sample_.begin(); training_sample_it != training_sample_.end(); ++training_sample_it) {
        
        if (training_sample_it->get()->label()) {
            training_sample_it->get()->weight = positive_weight;
        } else {
            training_sample_it->get()->weight = negative_weight;
        }       
    }
    
    weak_classifiers_.clear();
    weak_classifiers_.reserve(number_of_weak_classifiers_);
    
    weak_weights_.clear();
    weak_weights_.reserve(number_of_weak_classifiers_);
}

void BoostingClassifier::buildIndex() {
    LabeledSampleElement_ptr sample_element = training_sample_[0];
    RayFeaturesEnumerator features_enumerator((int) sample_element->features_ptr()->at(0).at(0).size(), (int) sample_element->features_ptr()->at(0).size(),(int)( 360 / sample_element->features_ptr()->at(0).at(0).at(0).size()), (int)sample_element->features_ptr()->size());
    std::cout << std::endl << "Building features index..." << std::endl;
    int64 t = cv::getTickCount();
    
    
    float ordered_weight = 0.0f;
    
    for (std::vector<LabeledSampleElement_ptr>::iterator training_sample_it = training_sample_.begin(); training_sample_it != training_sample_.end(); ++training_sample_it) {
        training_sample_it->get()->weight = ordered_weight;
        ordered_weight += 1.0f;
    }

    
    
    int features_count = features_enumerator.count();
    
    index_.resize(features_count);     
    RayFeatureType featureType = features_enumerator.first();
 
   
  
    
    int ten_percent_count = features_count / 10;
    
    for (int feature_no = 0; feature_no < features_count; ++feature_no) {
       
        assert(features_enumerator.fetureNo() == feature_no);
        index_[feature_no].resize(training_sample_.size());
        
        
               
        std::sort(training_sample_.begin(), training_sample_.end(), LabeledSampleElementLess(featureType));
     
       
        if ((feature_no + 1) % ten_percent_count == 0) {
            std::cout << (feature_no + 1) * 10 / ten_percent_count << " \% done, time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec";
            std::cout << std::endl;           
        }
        
           
      
       
    
        for (size_t ind = 0; ind < training_sample_.size(); ++ind) {
            index_[feature_no][ind] = static_cast<int>(training_sample_[ind].get()->weight);
        }       
        
        featureType = features_enumerator.next();
        
    }
    

    
      
    std::sort(training_sample_.begin(), training_sample_.end(), LabeledSampleElementWeightLess());
    
    std::cout << "Index building done, time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
}

void BoostingClassifier::performTrainingIteration(int iterationNo) {
    
    if (!this->training_sample_.empty()) {
        // Select the best weak classifier with respect to the weighted error
        std::cout << std::endl << "Selecting the best " << iterationNo << " weak classifier..." << std::endl;
        int64 t = cv::getTickCount();
        
        
        LabeledSampleElement_ptr sample_element = training_sample_[0];
        
        RayFeaturesEnumerator features_enumerator((int) sample_element->features_ptr()->at(0).at(0).size(), (int) sample_element->features_ptr()->at(0).size(),(int)( 360 / sample_element->features_ptr()->at(0).at(0).at(0).size()), (int)sample_element->features_ptr()->size());
        
        std::cout << "feartureType Count = " << features_enumerator.count() <<std::endl;
        
        DecisionStump minErrorStump = DecisionStump();
        float minError = 1.0;
        RayFeatureType featureType = features_enumerator.first();
         
        while (featureType != features_enumerator.last()) {
            DecisionStump stump = DecisionStump(featureType);
            
            //float error = stump.train(&training_sample_);
            
            float error = stump.train_with_index(&training_sample_, index_[features_enumerator.fetureNo()]);
            
            if (error < minError) {
                minError = error;
                minErrorStump = stump;
            }
            
            featureType = features_enumerator.next();
        }
        
        std::cout << "Selecting done, time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
        
        weak_classifiers_.push_back(minErrorStump);
        weak_weights_.push_back( logf(1.0f / minError) / logf( 2 ));
        
        std::cout << "Best classifier error = " << minError << std::endl;
        std::cout << minErrorStump << std::endl;
        
        // Update the weights (downweight the examples, that are classified correctly)
        
        float downweight_factor = minError / (1.0f - minError);
        
        if (minError > 0) {
            for (std::vector<LabeledSampleElement_ptr>::iterator it_s = training_sample_.begin(); it_s < training_sample_.end(); ++it_s)
            {
                
                if (it_s->get()->label() == minErrorStump.decision(it_s->get()->features_ptr())) {
                    it_s->get()->weight *= downweight_factor;
                }
            }
        }
        
        std::deque<bool> answers;
        
        this->classify(training_sample_, &answers);
        
        size_t ind = 0;
        int error_count = 0;
        for (std::vector<LabeledSampleElement_ptr>::iterator it_s = training_sample_.begin(); it_s < training_sample_.end(); ++it_s, ++ind)
        {
            if (it_s->get()->label() != answers.at(ind)) {
                ++error_count;
            }
        }
        
        std::cout << "Boosting error = " << static_cast<float>(error_count) / training_sample_.size() << " count = " << error_count << std::endl;
         
        
    }
     
}

float BoostingClassifier::strong_threshold() const {
    return std::accumulate(weak_weights_.begin(), weak_weights_.end(), 0.0f) / 2.0f;
}

void BoostingClassifier::classify(const std::vector<LabeledSampleElement_ptr>& sample, std::deque<bool>* labels) const {
    *labels = std::deque<bool>(sample.size(), false);
    
    float strong_threshold = this->strong_threshold();
    
    size_t ind = 0;
    for ( std::vector<LabeledSampleElement_ptr>::const_iterator it_s = sample.begin(); it_s < sample.end(); ++it_s, ++ind)
    {
        float score = 0.0f;
        std::vector<float>::const_iterator it_w = weak_weights_.begin();
        for (std::vector<DecisionStump>::const_iterator it_weak = weak_classifiers_.begin(); it_weak < weak_classifiers_.end(); ++it_weak, ++it_w) {
            if (it_weak->decision(it_s->get()->features_ptr())) {
                score += *it_w;
            }
        }
        
        labels->at(ind) = (score >= strong_threshold);
    }

}

void BoostingClassifier::get_scores(const std::vector<LabeledSampleElement_ptr>& sample, std::vector<float>* scores) const {
    *scores = std::vector<float>(sample.size(), 0.0);
        
    size_t ind = 0;
    for ( std::vector<LabeledSampleElement_ptr>::const_iterator it_s = sample.begin(); it_s < sample.end(); ++it_s, ++ind)
    {
        float score = 0.0f;
        std::vector<float>::const_iterator it_w = weak_weights_.begin();
        for (std::vector<DecisionStump>::const_iterator it_weak = weak_classifiers_.begin(); it_weak < weak_classifiers_.end(); ++it_weak, ++it_w) {
            if (it_weak->decision(it_s->get()->features_ptr())) {
                score += *it_w;
            }
        }
        
        scores->at(ind) = score;
    }

}

float BoostingClassifier::get_score(const LabeledSampleElement_ptr& sample_element_ptr) const {
    float score = 0.0f;
    std::vector<float>::const_iterator it_w = weak_weights_.begin();
    for (std::vector<DecisionStump>::const_iterator it_weak = weak_classifiers_.begin(); it_weak < weak_classifiers_.end(); ++it_weak, ++it_w) {
        if (it_weak->decision(sample_element_ptr->features_ptr())) {
            score += *it_w;
        }
    }
    
    return score;
}



void BoostingClassifier::train(const std::vector<LabeledSampleElement_ptr>& training_sample) {
    
    this->training_sample_ = training_sample;
    
    if (!this->training_sample_.empty()) {
        this->initTraining();
        
        for (int iterationNo = 0; iterationNo < number_of_weak_classifiers_ ; ++iterationNo) {
            this->performTrainingIteration(iterationNo + 1);
        }
    }
    
    this->isTrainded_ = true;
     
}



