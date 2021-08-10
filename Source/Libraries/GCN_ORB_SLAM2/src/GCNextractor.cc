/**
* This file is part of ORB-SLAM2.
* This file is based on the file orb.cpp from the OpenCV library (see BSD license below).
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/
/**
* Software License Agreement (BSD License)
*
*  Copyright (c) 2009, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
*/


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>

#include "GCNextractor.h"


using namespace cv;
using namespace std;

namespace ORB_SLAM2
{

void nms(cv::Mat det, cv::Mat desc, std::vector<cv::KeyPoint>& pts, cv::Mat& descriptors,
        int border, int dist_thresh, int img_width, int img_height, float ratio_width, float ratio_height){

    std::vector<cv::Point2f> pts_raw;

    for (int i = 0; i < det.rows; i++){

        int u = (int) det.at<float>(i, 0);
        int v = (int) det.at<float>(i, 1);
        // float conf = det.at<float>(i, 2);

        pts_raw.push_back(cv::Point2f(u, v));
    }

    cv::Mat grid = cv::Mat(cv::Size(img_width, img_height), CV_8UC1);
    cv::Mat inds = cv::Mat(cv::Size(img_width, img_height), CV_16UC1);

    grid.setTo(0);
    inds.setTo(0);

    for (int i = 0; i < pts_raw.size(); i++)
    {   
        int uu = (int) pts_raw[i].x;
        int vv = (int) pts_raw[i].y;

        grid.at<char>(vv, uu) = 1;
        inds.at<unsigned short>(vv, uu) = i;
    }
    
    cv::copyMakeBorder(grid, grid, dist_thresh, dist_thresh, dist_thresh, dist_thresh, cv::BORDER_CONSTANT, 0);

    for (int i = 0; i < pts_raw.size(); i++)
    {   
        int uu = (int) pts_raw[i].x + dist_thresh;
        int vv = (int) pts_raw[i].y + dist_thresh;

        if (grid.at<char>(vv, uu) != 1)
            continue;

        for(int k = -dist_thresh; k < (dist_thresh+1); k++)
            for(int j = -dist_thresh; j < (dist_thresh+1); j++)
            {
                if(j==0 && k==0) continue;

                grid.at<char>(vv + k, uu + j) = 0;
                
            }
        grid.at<char>(vv, uu) = 2;
    }

    size_t valid_cnt = 0;
    std::vector<int> select_indice;

    for (int v = 0; v < (img_height + dist_thresh); v++){
        for (int u = 0; u < (img_width + dist_thresh); u++)
        {
            if (u -dist_thresh>= (img_width - border) || u-dist_thresh < border || v-dist_thresh >= (img_height - border) || v-dist_thresh < border)
            continue;

            if (grid.at<char>(v,u) == 2)
            {
                int select_ind = (int) inds.at<unsigned short>(v-dist_thresh, u-dist_thresh);
                pts.push_back(cv::KeyPoint(pts_raw[select_ind].x * ratio_width, pts_raw[select_ind].y * ratio_height, 1.0f));

                select_indice.push_back(select_ind);
                valid_cnt++;
            }
        }
    }
    
    descriptors.create(select_indice.size(), 32, CV_8U);

    for (int i=0; i<select_indice.size(); i++)
    {
        for (int j=0; j<32; j++)
        {
            descriptors.at<unsigned char>(i, j) = desc.at<unsigned char>(select_indice[i], j);
        }
    }
}



GCNextractor::GCNextractor(int _nfeatures, float _scaleFactor, int _nlevels,
			   int _iniThFAST, int _minThFAST): FeatureExtractor(_nfeatures, _scaleFactor, _nlevels, _iniThFAST, _minThFAST)
{
    const char *net_fn = getenv("GCN_PATH");

    // If a network isn't defined, pick the default which matches the resolution
    if (net_fn == nullptr)
      {
	if (getenv("FULL_RESOLUTION") == nullptr)
	  {
	    net_fn = "gcn2_320x240.pt";
	  }
	else
	  {
	    net_fn = "gcn2_640x480.pt";
	  }
      }
    string networkName = string(DEFAULT_GCN_SCRIPT_DIR) + string("/") + string(net_fn);
    cout << "Loading " << networkName << endl;
    module = torch::jit::load(networkName);
    cout << "Loaded" << endl;
}

void GCNextractor::operator()( InputArray _image, InputArray _mask, vector<KeyPoint>& _keypoints, OutputArray _descriptors)
{ 

    torch::DeviceType device_type;
    device_type = torch::kCPU;
    torch::Device device(device_type);

    if(_image.empty())
        return;

    Mat image = _image.getMat();
    assert(image.type() == CV_8UC1 );

    cv::Mat img;
    image.convertTo(img, CV_32FC1, 1.f / 255.f , 0);

    int img_width = 320;
    int img_height = 240;

    int border = 8;
    int dist_thresh = 4;

    if (getenv("FULL_RESOLUTION") != nullptr)
    {
        img_width = 640;
        img_height = 480;

        border = 16;
        dist_thresh = 8;
    }
    
    float ratio_width = float(img.cols) / float(img_width);
    float ratio_height = float(img.rows) / float(img_height);
    
    cv::resize(img, img, cv::Size(img_width, img_height));

    auto img_tensor = torch::from_blob(img.data, {1, img_height, img_width, 1}).to(device);
    img_tensor = img_tensor.permute({0,3,1,2});
    auto img_var = torch::autograd::make_variable(img_tensor, false).to(device);

    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(img_var);
    auto output = module.forward(inputs).toTuple();

    auto pts  = output->elements()[0].toTensor().to(torch::kCPU).squeeze();
    auto desc = output->elements()[1].toTensor().to(torch::kCPU).squeeze();

    cv::Mat pts_mat(cv::Size(3, pts.size(0)), CV_32FC1, pts.data_ptr<float>());
    cv::Mat desc_mat(cv::Size(32, pts.size(0)), CV_8UC1, desc.data_ptr<unsigned char>());

    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    nms(pts_mat, desc_mat, keypoints, descriptors, border, dist_thresh, img_width, img_height, ratio_width, ratio_height);

    _keypoints.insert(_keypoints.end(), keypoints.begin(), keypoints.end());
    
    int nkeypoints = keypoints.size();
    _descriptors.create(nkeypoints, 32, CV_8U);
    descriptors.copyTo(_descriptors.getMat());

    // std::cout << descriptors << std::endl;
    // std::cout << pts.size(0) << std::endl;
    // std::cout << keypoints.size() << std::endl;
    // cv::waitKey();

}

} //namespace ORB_SLAM
