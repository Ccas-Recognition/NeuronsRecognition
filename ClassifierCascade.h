//
//  ClassifierCascade.h
//  RayFeaturesProject
//
//  Created by Artem on 26.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#ifndef __RayFeaturesProject__ClassifierCascade__
#define __RayFeaturesProject__ClassifierCascade__

#include <iostream>
#include <deque>
#include <utility>
#include <vector>
#include <opencv2/core/core.hpp>
#include "BoostingClassifier.h"

class ClassifierCascade {
    float goal_false_posotive_rate_;
    float goal_detection_rate_;
    int number_of_stages_;
    std::vector<BoostingClassifier> cascaded_classifiers_;
    std::vector<float> cascaded_thresholds_;

   
    
    void calculate_false_positive_and_detection_rates(const std::vector<LabeledSampleElement_ptr>& test_sample, float *false_positive_rate, float *detection_rate) const;
    
    
public:
    ClassifierCascade(float goal_false_posotive_rate = 1e-05f, float goal_detection_rate = 0.9f, int number_of_stages = 10)
    : goal_false_posotive_rate_(goal_false_posotive_rate), goal_detection_rate_(goal_detection_rate), number_of_stages_(number_of_stages), cascaded_classifiers_(), cascaded_thresholds_() {
        
    }
   
    void train(const std::vector<LabeledSampleElement_ptr>& training_sample, const std::vector<LabeledSampleElement_ptr>& test_sample);
    
    void classify(const std::vector<LabeledSampleElement_ptr>& sample, std::deque<bool>* labels) const;
    bool classify(const LabeledSampleElement_ptr& sample_element_ptr) const;
    
    
    void write(cv::FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" ;
        fs << "goal_false_posotive_rate" << goal_false_posotive_rate_;
        fs << "goal_detection_rate" << goal_detection_rate_;
        fs << "number_of_stages" << number_of_stages_;
        fs << "boosting_classifiers" << "[";
        for(std::vector<BoostingClassifier>::const_iterator it_c = cascaded_classifiers_.begin(); it_c != cascaded_classifiers_.end(); ++it_c) {
            it_c->write(fs);
        }
        fs << "]";
        fs << "thresholds" << "[";
        for(std::vector<float>::const_iterator it_t = cascaded_thresholds_.begin(); it_t != cascaded_thresholds_.end(); ++it_t) {
            fs << *it_t;
        }
        fs << "]";
        fs << "}";
    }
    
    void read(const cv::FileNode& node)                          //Read serialization for this class
    {
        node["goal_false_posotive_rate"] >> goal_false_posotive_rate_;
        node["goal_detection_rate"] >> goal_detection_rate_;
        node["number_of_stages"] >> number_of_stages_;
        
        cv::FileNode n = node["boosting_classifiers"];                         // Read string sequence - Get node
        if (n.type() != cv::FileNode::SEQ)
        {
            std::cerr << "boosting_classifiers is not a sequence! FAIL" << std::endl;
            return;
        }
        
        std::vector<BoostingClassifier> boosting_classifiers;
        boosting_classifiers.reserve(n.size());
        
        cv::FileNodeIterator it_end = n.end(); // Go through the node
        for ( cv::FileNodeIterator it = n.begin(); it != it_end; ++it) {
            BoostingClassifier boosting_classifier = BoostingClassifier();
            boosting_classifier.read(*it);
            boosting_classifiers.push_back(boosting_classifier);
        }
        
        cascaded_classifiers_ = boosting_classifiers;
        
        
        std::vector<float> thresholds;
        node["thresholds"] >> thresholds;
        
        
        cascaded_thresholds_ = thresholds;        
        
    }


    
};



#endif /* defined(__RayFeaturesProject__ClassifierCascade__) */
