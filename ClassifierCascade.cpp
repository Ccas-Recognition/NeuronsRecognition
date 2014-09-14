//
//  ClassifierCascade.cpp
//  RayFeaturesProject
//
//  Created by Artem on 26.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#include "ClassifierCascade.h"


void ClassifierCascade::calculate_false_positive_and_detection_rates(const std::vector<LabeledSampleElement_ptr>& test_sample, float *false_positive_rate, float *detection_rate) const {
    std::deque<bool> labels;
    this->classify(test_sample, &labels);
    
    int number_of_positives = 0;
    int number_of_negatives = 0;
    int number_of_detected_positives = 0;
    int number_of_false_positives = 0;
    
    size_t index = 0;
    for (std::deque<bool>::iterator it_l = labels.begin(); it_l != labels.end(); ++it_l, ++index) {
        if (test_sample[index].get()->label()) {
            ++number_of_positives;
            if (*it_l) {
                ++number_of_detected_positives;
            }
        } else {
            ++number_of_negatives;
            if (*it_l) {
                ++number_of_false_positives;
            }
        }
    }
    
    *false_positive_rate = static_cast<float>(number_of_false_positives) / static_cast<float>(number_of_negatives);
    *detection_rate = static_cast<float>(number_of_detected_positives) / static_cast<float>(number_of_positives);
}



void ClassifierCascade::train(const std::vector<LabeledSampleElement_ptr>& training_sample, const std::vector<LabeledSampleElement_ptr>& test_sample) {
    const float min_stage_detection_rate = std::pow(goal_detection_rate_, 1.0f / number_of_stages_);
    const float max_stage_false_positive_rate = std::pow(goal_false_posotive_rate_, 1.0f / number_of_stages_);
        
    float detection_rate = 1.0f;
    float false_positive_rate = 1.0f;
    
    float last_detection_rate = detection_rate;
    float last_false_positive_rate = false_positive_rate;
    
    int cascade_stage = 0;

    std::vector<LabeledSampleElement_ptr> stage_training_sample = std::vector<LabeledSampleElement_ptr>(training_sample.begin(), training_sample.end());
    std::vector<float> scores;
    
    std::cout << std::endl << "Training cascade ..." << std::endl;
    int64 t_total = cv::getTickCount();
    
     while ((false_positive_rate > goal_false_posotive_rate_) && (cascade_stage < number_of_stages_)) {
         std::cout << std::endl << "Training cascade at " << cascade_stage << " stage..." << std::endl;
         std::cout << "Goals d. rate = " << min_stage_detection_rate * last_detection_rate << " fp. rate = " << max_stage_false_positive_rate * last_false_positive_rate  << std::endl << std::endl;
         int64 t = cv::getTickCount();
         
         int number_of_boosting_iterations = 2;
         BoostingClassifier boosting_classifier = BoostingClassifier(number_of_boosting_iterations);
         
         
         while (false_positive_rate > max_stage_false_positive_rate * last_false_positive_rate) {
             if (!boosting_classifier.isTrainded()) {
                 boosting_classifier.train(stage_training_sample);
                 this->cascaded_classifiers_.push_back(BoostingClassifier());
                 boosting_classifier.copy_for_classification(&cascaded_classifiers_.back());
                 cascaded_thresholds_.push_back(boosting_classifier.strong_threshold());
             } else {
                 boosting_classifier.increase_number_of_weak_classifiers();
                 boosting_classifier.copy_for_classification(&cascaded_classifiers_.at(cascade_stage));
                 cascaded_thresholds_.at(cascade_stage) = boosting_classifier.strong_threshold();
             }
             
            
             
             this->calculate_false_positive_and_detection_rates(test_sample, &false_positive_rate, &detection_rate);
            
            
             
             if (detection_rate < min_stage_detection_rate*last_detection_rate) {
                 boosting_classifier.get_scores(test_sample, &scores);
                 std::sort(scores.begin(), scores.end(), std::greater<float>());
                 
                 std::vector<float>::iterator first = std::lower_bound(scores.begin(), scores.end(), cascaded_thresholds_.at(cascade_stage), std::greater<float>());
                 std::cout << "Strong threshold = " << cascaded_thresholds_.at(cascade_stage) << std::endl;
                 assert(first != scores.end());
                  
               
                 std::cout << "Lower bound = " << *first << std::endl;
                 
                 std::vector<float>::iterator last = scores.end();
                 
                 std::vector<float>::iterator::difference_type count, step;
                 
                 count = std::distance(first, last);
                 
                 while (count > 0) {
                     std::vector<float>::iterator it = first;
                     step = count / 2;
                     std::advance(it, step);
                     
                     cascaded_thresholds_.at(cascade_stage) = *it;
                     this->calculate_false_positive_and_detection_rates(test_sample, &false_positive_rate, &detection_rate);
                     
                     if (detection_rate < min_stage_detection_rate * last_detection_rate) {
                         first = ++it;
                         count -= step + 1;
                     } else {
                         count = step;
                     }
                    
                 }
                 
                 assert(first != scores.end());
                 
                 std::cout << "New thresh = " << *first << std::endl;
                 cascaded_thresholds_.at(cascade_stage) = *first;
                 this->calculate_false_positive_and_detection_rates(test_sample, &false_positive_rate, &detection_rate);
                 
                 std::cout << "Current d. rate = " << detection_rate << " fp. rate = " << false_positive_rate << std::endl;
                 assert(detection_rate >= min_stage_detection_rate*last_detection_rate);
                 
                 
                 
             }
                         
             
         }
         
         std::cout << std::endl << "Training at stage " << cascade_stage << " done, time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
         ++cascade_stage;
         last_false_positive_rate = false_positive_rate;
         last_detection_rate = detection_rate;
         
         
         if ((false_positive_rate > goal_false_posotive_rate_) && (cascade_stage < number_of_stages_)) {
             std::vector<LabeledSampleElement_ptr> next_stage_training_sample;
             next_stage_training_sample.reserve(stage_training_sample.size());
             
             for (std::vector<LabeledSampleElement_ptr>::const_iterator it = stage_training_sample.begin(); it < stage_training_sample.end(); ++it) {
                 if (it->get()->label() || this->classify(*it)) {
                     next_stage_training_sample.push_back(*it);
                 }
             }
             stage_training_sample = std::vector<LabeledSampleElement_ptr>(next_stage_training_sample.begin(), next_stage_training_sample.end());
         }


     }
    
    std::cout << std::endl << "Training cascade done, time: " << ((cv::getTickCount() - t_total) / cv::getTickFrequency()) << " sec" << std::endl;
    if (cascade_stage == number_of_stages_) {
        std::cout << "Number of stages riched " << number_of_stages_ << std::endl;
    } else {
        std::cout << "Learning goals RICHED!!!" << std::endl;
        
    }

}

bool ClassifierCascade::classify(const LabeledSampleElement_ptr& sample_element_ptr) const {
    bool label = true;
    for (size_t classifier_ind = 0; classifier_ind < cascaded_classifiers_.size(); ++classifier_ind) {
        label = cascaded_classifiers_[classifier_ind].get_score(sample_element_ptr) >= cascaded_thresholds_[classifier_ind];
        if (!label) {
            break;
        }
    }
    return label;
}


void ClassifierCascade::classify(const std::vector<LabeledSampleElement_ptr>& sample, std::deque<bool>* labels) const {
    *labels = std::deque<bool>(sample.size(), false);
    
    size_t ind = 0;
    for (std::vector<LabeledSampleElement_ptr>::const_iterator it_s = sample.begin(); it_s != sample.end(); ++it_s, ++ind) {
        labels->at(ind) = this->classify(*it_s);
    }
    
}
