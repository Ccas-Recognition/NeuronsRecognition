//
//  BoostingClassifier.h
//  RayFeaturesProject
//
//  Created by Artem on 18.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#ifndef __RayFeaturesProject__BoostingClassifier__
#define __RayFeaturesProject__BoostingClassifier__

#include <iostream>
#include <vector>
#include <memory>
#include <deque>

#include "DecisionStump.h"
#include "LabeledSampleElement.h"


class BoostingClassifier {
    unsigned int number_of_weak_classifiers_;
    std::vector<DecisionStump> weak_classifiers_;
    std::vector<float> weak_weights_;
    
    std::vector<LabeledSampleElement_ptr> training_sample_;
    
    std::vector<std::vector<int> > index_;
    
    bool isTrainded_;
    
    void initTraining();
    void buildIndex();
    void performTrainingIteration(int iterationNo);
public:
    BoostingClassifier(unsigned int number_of_weak_classifiers = 0) : number_of_weak_classifiers_(number_of_weak_classifiers), weak_classifiers_(),  weak_weights_(), training_sample_(), index_(), isTrainded_(false) {
        
    }
    
    void copy_for_classification(BoostingClassifier* classifier) {
        *classifier = BoostingClassifier(this->number_of_weak_classifiers_);
        classifier->isTrainded_ = true;
        
        classifier->weak_classifiers_ =  std::vector<DecisionStump>(this->weak_classifiers_.begin(), this->weak_classifiers_.end());
        classifier->weak_weights_ = std::vector<float>(this->weak_weights_.begin(), this->weak_weights_.end());
     
    }
    
    // training function  - train number_of_weak_classifiers combination
    void train(const std::vector<LabeledSampleElement_ptr>& training_sample);
    
   
    void classify(const std::vector<LabeledSampleElement_ptr>& sample, std::deque<bool>* labels) const;
    void get_scores(const std::vector<LabeledSampleElement_ptr>& sample, std::vector<float>* scores) const;
    float get_score(const LabeledSampleElement_ptr& sample_element_ptr) const;
 
    
    // training function  - increase current number of weak classifiers by one
    void increase_number_of_weak_classifiers() {
        
        if (this->isTrainded_ && !training_sample_.empty()) {
            ++number_of_weak_classifiers_;
            this->performTrainingIteration(number_of_weak_classifiers_);     
        }
    }
    unsigned int get_number_of_weak_classifiers() const {
        return number_of_weak_classifiers_;
    }
    
    bool isTrainded() const {
        return isTrainded_;
    }
    
    float strong_threshold() const;
    
    void write(cv::FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" ;
        fs << "weak_weights" << "[";
        for(std::vector<float>::const_iterator it_w = weak_weights_.begin(); it_w != weak_weights_.end(); ++it_w) {
            fs << *it_w;
        }
        fs << "]";
        fs << "weak_classifiers" << "[";
        for(std::vector<DecisionStump>::const_iterator it_c = weak_classifiers_.begin(); it_c != weak_classifiers_.end(); ++it_c) {
            it_c->write(fs);
        }
        fs << "]";
        fs << "}";
    }
    
    
    void read(const cv::FileNode& node)                          //Read serialization for this class
    {
        node["weak_weights"] >> weak_weights_;
        
        cv::FileNode n = node["weak_classifiers"];                         // Read string sequence - Get node
        if (n.type() != cv::FileNode::SEQ)
        {
            std::cerr << "weak_classifiers is not a sequence! FAIL" << std::endl;
            return;
        }
        
        weak_classifiers_.clear();
        weak_classifiers_.reserve(n.size());
        
        cv::FileNodeIterator it_end = n.end(); // Go through the node
        for ( cv::FileNodeIterator it = n.begin(); it != it_end; ++it) {
            DecisionStump stump = DecisionStump();
            stump.read(*it);
            weak_classifiers_.push_back(stump);
        }
        
        number_of_weak_classifiers_ = (int) weak_classifiers_.size();
        isTrainded_ = true;
    }

    
};


#endif /* defined(__RayFeaturesProject__BoostingClassifier__) */
