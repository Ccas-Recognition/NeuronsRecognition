//
//  RayFeaturesPrecomputer.h
//  RayFeaturesProject
//
//  Created by Artem on 11.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#ifndef __RayFeaturesProject__RayFeaturesPrecomputer__
#define __RayFeaturesProject__RayFeaturesPrecomputer__

#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include "RayFeatureValues.h"


class RayFeaturesPrecomputer {
    int angle_step_;
    std::vector<int> thresholds_;
    void precompute_in_direction(const cv::Mat& grad_x, const cv::Mat& grad_y, const cv::Mat& edges, size_t threshold_ind, int direction,  std::tr1::shared_ptr<RayFeatures> features_ptr) const;
public:
    RayFeaturesPrecomputer(int angle_step = 30, const std::vector<int>& thresholds = std::vector<int>(1, 80)) : angle_step_(angle_step % 360) , thresholds_(thresholds) {
        
        for (size_t ind = 0; ind < this->thresholds_.size(); ++ind) {
            thresholds_[ind] = thresholds_[ind] % 360;
        }
        
    }
    
    int angle_step() const {
        return angle_step_;
    }
    
    std::vector<int> thresholds() const {
        return thresholds_;
    }
    
    void write(cv::FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" ;
        fs << "angle_step" << angle_step_;
        fs << "thresholds" << "[";        
        for(std::vector<int>::const_iterator it_t = thresholds_.begin(); it_t != thresholds_.end(); ++it_t) {
            fs << *it_t;
        }
        fs << "]";
        fs << "}";
    }
    
    void read(const cv::FileNode& node)                          //Read serialization for this class
    {
        
        angle_step_ = (int)node["angle_step"];
        cv::FileNode n = node["thresholds"];                         // Read string sequence - Get node
        if (n.type() == cv::FileNode::INT) {
            thresholds_ = std::vector<int>(1, 0);
            thresholds_[0] = (int)node["thresholds"];
        } else {
            if (n.type() != cv::FileNode::SEQ)
            {
                std::cerr << "thresholds is not a sequence! FAIL" << std::endl;
                return;
            }
            
            thresholds_.clear();
            thresholds_.reserve(n.size());
            
            cv::FileNodeIterator it_end = n.end(); // Go through the node
            for ( cv::FileNodeIterator it = n.begin(); it != it_end; ++it) {
                thresholds_.push_back((int)*it);
            }

        }        
    }

    
    std::tr1::shared_ptr<RayFeatures> precompute(const cv::Mat& image) const;
};

#endif /* defined(__RayFeaturesProject__RayFeaturesPrecomputer__) */
