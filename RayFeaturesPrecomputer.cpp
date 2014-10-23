//
//  RayFeaturesPrecomputer.cpp
//  RayFeaturesProject
//
//  Created by Artem on 11.09.12.
//  Copyright (c) 2012 Artem. All rights reserved.
//

#include <math.h>
#include "RayFeaturesPrecomputer.h"

#define M_PI 3.14159265358979323846


void RayFeaturesPrecomputer::precompute_in_direction(const cv::Mat& grad_x, const cv::Mat& grad_y, const cv::Mat& edges, size_t threshold_ind, int direction,  std::shared_ptr<RayFeatures> features_ptr) const {
    cv::Mat mask = cv::Mat::zeros(edges.size(), CV_8U);
    
    cv::Rect imgRect = cv::Rect(cv::Point(0, 0), edges.size());
    
    int angle = direction % 360;
             
    std::vector<cv::Point> start_points;
    start_points.reserve(edges.size().width + edges.size().height);
    
       
    if ((angle < 90) || (angle > 270) ) {
        for (int row = 0; row < edges.rows; row += 1) {
            start_points.push_back(cv::Point(0, row));
        }    
    }
    if ((0 < angle) && (angle < 180)) {
        for (int col = 0; col <  edges.cols; col += 1) {
            start_points.push_back(cv::Point(col, 0));
        }
    }
    if ((90 < angle) && (angle < 270)) {
        for (int row = 0; row < edges.rows; row += 1) {
            start_points.push_back(cv::Point(edges.cols - 1, row));
        }
    }
    if ((180 < angle) && (angle < 360)) {
        for (int col = 0; col <  edges.cols; col += 1) {
            start_points.push_back(cv::Point(col, edges.rows - 1));
        }
    }
    
        
    std::vector<cv::Point>(start_points.begin(), start_points.end()).swap(start_points);
     
    
    float angle_sin = sinf(angle * M_PI / 180.0);
    float angle_cos = cosf(angle * M_PI / 180.0);
    float shift_step = std::max(fabsf(angle_sin), fabsf(angle_cos));
    
    int angle_ind = angle / angle_step_;

    
    
    for (std::vector<cv::Point>::const_iterator start_pt_it = start_points.begin(); start_pt_it != start_points.end(); ++start_pt_it) {
        float shift = 0.0;
        cv::Point pt = *start_pt_it;
        RayFeatureValues feature_values = RayFeatureValues(0, 0, 0);
    
        cv::Point last_edge_pt = pt;
        
        while(imgRect.contains(pt)) {
            if (edges.at<uchar>(pt) > 0) {
                last_edge_pt = pt;
                feature_values.distance_squared = 0;
                feature_values.norm_squared = grad_x.at<short>(pt) * grad_x.at<short>(pt) + grad_y.at<short>(pt) * grad_y.at<short>(pt);
                
                assert(feature_values.norm_squared > 0);
                
                int gradient_angle = 360 + atan2(( float )grad_y.at<short>(pt), ( float )grad_x.at<short>(pt)) * 180 / M_PI;
                
                feature_values.orientation = (gradient_angle - angle + 360) % 360;
                
                assert((0 <= feature_values.orientation) && (feature_values.orientation < 360));
            } else {
                feature_values.distance_squared = (pt.x - last_edge_pt.x) * (pt.x - last_edge_pt.x) + (pt.y - last_edge_pt.y) * (pt.y - last_edge_pt.y);
            }
            
            
        
            
            if (mask.at<uchar>(pt) == 0) {
                features_ptr->at(threshold_ind).at(pt.y).at(pt.x).at(angle_ind) = feature_values;
            } else {
                features_ptr->at(threshold_ind).at(pt.y).at(pt.x).at(angle_ind) += feature_values;
            }
            
            
            mask.at<uchar>(pt) += 1;
            shift += shift_step;
            pt.x = cvRound(start_pt_it->x + shift * angle_cos);
            pt.y = cvRound(start_pt_it->y + shift * angle_sin);            
        }
    }
    
    // normalize features, that were calculated more than once
    
    double overhead = 0.0;
    
    for (int row = 0; row < mask.rows; ++row) {
        for (int col = 0; col < mask.cols; ++col) {
            
            double mask_val = static_cast<double>(mask.at<uchar>(row, col));
            overhead += mask_val;
            
            if (mask.at<uchar>(row, col) > 1) {                
                features_ptr->at(threshold_ind).at(row).at(col).at(angle_ind).distance_squared = cvRound(features_ptr->at(threshold_ind).at(row).at(col).at(angle_ind).distance_squared / mask_val);
                features_ptr->at(threshold_ind).at(row).at(col).at(angle_ind).norm_squared = cvRound(features_ptr->at(threshold_ind).at(row).at(col).at(angle_ind).norm_squared / mask_val);
                features_ptr->at(threshold_ind).at(row).at(col).at(angle_ind).orientation = cvRound(features_ptr->at(threshold_ind).at(row).at(col).at(angle_ind).orientation / mask_val);
            }
            
        
        }
    }
     
    
    //std::cout << angle << " direction overhead = " << overhead / (mask.size().width * mask.size().height) << std::endl;
    
   
    assert(cv::countNonZero(mask) == mask.size().width * mask.size().height);
         
}

std::shared_ptr<RayFeatures> RayFeaturesPrecomputer::precompute(const cv::Mat& image) const {
     
     
    assert(angle_step_ % 360 != 0);
    assert((image.channels() == 3) || (image.channels() == 1));
    assert(thresholds_.size() > 0);
     
    
    cv::Mat gray_image;
    
    if (image.channels() == 3) {
        cv::cvtColor(image, gray_image, CV_BGR2GRAY);
    } else { // (image.channels() == 1)
        gray_image = image;
    }
    
    
    // initialization
     
      
    std::shared_ptr<RayFeatures> features_ptr(new RayFeatures());
     
   
    features_ptr->resize(thresholds_.size());
    
    for (size_t thresh_ind = 0; thresh_ind < this->thresholds_.size(); ++thresh_ind) {
        features_ptr->at(thresh_ind).resize(gray_image.rows);
        for (int row = 0; row < gray_image.rows; ++row) {
            features_ptr->at(thresh_ind).at(row).resize(gray_image.cols);
            for (int col = 0; col < gray_image.cols; ++col) {
                features_ptr->at(thresh_ind).at(row).at(col).resize(360 / (angle_step_ % 360));
            }
        }

    }
    
   
    
     
        
    cv::blur(gray_image, gray_image, cv::Size(3,3));
    
    cv::Mat gradient_x, gradient_y;
    
    cv::Sobel(gray_image, gradient_x, CV_16S, 1, 0, 3);
    cv::Sobel(gray_image, gradient_y, CV_16S, 0, 1, 3);
    
    /*
    double lowThreshold = 20.0;
    double ratio = 4;
  
    cv::Canny(gray_image, edges, lowThreshold, ratio * lowThreshold, 3);
     */
    
    cv::Mat magnitude, gradient_x_f, gradient_y_f;
    
    gradient_x.convertTo(gradient_x_f, CV_32F);
    gradient_y.convertTo(gradient_y_f, CV_32F);
    
    cv::magnitude(gradient_x_f, gradient_y_f, magnitude);
    
    
    for (size_t thresh_ind = 0; thresh_ind < this->thresholds_.size(); ++thresh_ind) {
        cv::Mat edges;
        edges = magnitude > this->thresholds_[thresh_ind];
        
        /*
        cv::namedWindow("edges");
        cv::imshow("edges", edges);
        
        cv::waitKey(0);
        
        cv::destroyWindow("edges");
         */
        
        for (int direction = 0; direction < 360; direction += (angle_step_ % 360)) {
            this->precompute_in_direction(gradient_x, gradient_y, edges, thresh_ind, direction, features_ptr);
        }
    }
    
    
   
    
   
    
    
    
     
     
     return features_ptr;
    
        
     
}
