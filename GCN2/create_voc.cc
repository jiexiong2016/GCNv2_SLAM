
#include <iostream>
#include <vector>

#include <torch/script.h> // One-stop header.
#include <torch/torch.h>

// DBoW2
#include "DBoW2.h"
#include <DUtils/DUtils.h>
#include <DUtilsCV/DUtilsCV.h> // defines macros CVXX
#include <DVision/DVision.h>

// OpenCV
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace DBoW2;
using namespace DUtils;
using namespace std;

void nms(cv::Mat det, cv::Mat desc, std::vector<cv::KeyPoint>& pts, cv::Mat& descriptors,
        int border, int dist_thresh, int img_width, int img_height){

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
                pts.push_back(cv::KeyPoint(pts_raw[select_ind], 1.0f));

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


void loadFeatures(vector<vector<cv::Mat > > &features, cv::Mat descriptors);
void changeStructure(const cv::Mat &plain, vector<cv::Mat> &out);
void createVocabularyFile(OrbVocabulary &voc, std::string &fileName, const vector<vector<cv::Mat > > &features);

// ----------------------------------------------------------------------------

int main()
{


    torch::DeviceType device_type;
    device_type = torch::kCUDA;
    torch::Device device(device_type);

    std::shared_ptr<torch::jit::script::Module> module = torch::jit::load("/home/t/Dropbox/gcn2.pt");
    //std::shared_ptr<torch::jit::script::Module> module = torch::jit::load("/home/t/Dropbox/gcn_tiny.pt");

    vector<cv::String> fn;
    glob("/home/t/Workspace/GCNV2/SUN3D/mono1/*.png", fn, false);

    size_t count = fn.size(); 

    vector<vector<cv::Mat > > features;
   
    int index = 0;
    for (size_t i=0; i<count; i++)
    {
        index++;
        
        if (index>0)
        {
            
            std::cout << fn[i] << std::endl;
            cv::Mat image_now = cv::imread(fn[i],CV_LOAD_IMAGE_UNCHANGED);

            cv::Mat img1;
            image_now.convertTo(img1, CV_32FC1, 1.f / 255.f , 0);
            
            int img_width = 320;
            int img_height = 240;

            auto img1_tensor = torch::CPU(torch::kFloat32).tensorFromBlob(img1.data, {1, img_height, img_width, 1});
            img1_tensor = img1_tensor.permute({0,3,1,2});
            auto img1_var = torch::autograd::make_variable(img1_tensor, false).to(device);

            std::vector<torch::jit::IValue> inputs;
            inputs.push_back(img1_var);
            auto output = module->forward(inputs).toTuple();

            auto pts  = output->elements()[0].toTensor().to(torch::kCPU).squeeze();
            auto desc = output->elements()[1].toTensor().to(torch::kCPU).squeeze();

            cv::Mat pts_mat(cv::Size(3, pts.size(0)), CV_32FC1, pts.data<float>());
            cv::Mat desc_mat(cv::Size(32, pts.size(0)), CV_8UC1, desc.data<unsigned char>());

            int border = 8;
            int dist_thresh = 4;

            std::vector<cv::KeyPoint> keypoints;
            cv::Mat descriptors;
            nms(pts_mat, desc_mat, keypoints, descriptors, border, dist_thresh, img_width, img_height);
            loadFeatures(features, descriptors);
            
        }
    }

    cout << "... Extraction done!" << endl;

    // define vocabulary
    const int nLevels = 6;
    const int k = 10; // branching factor
    const WeightingType weight = TF_IDF;
    const ScoringType score = L1_NORM;
    OrbVocabulary voc(k, nLevels, weight, score);

    std::string vocName = "vocGCN.bin";
    createVocabularyFile(voc, vocName, features);

    cout << "--- THE END ---" << endl;

    return 0;
}

// ----------------------------------------------------------------------------

void loadFeatures(vector<vector<cv::Mat > > &features, cv::Mat descriptors)
{


    features.push_back(vector<cv::Mat >());
    changeStructure(descriptors, features.back());
  
}

// ----------------------------------------------------------------------------

void changeStructure(const cv::Mat &plain, vector<cv::Mat> &out)
{
  out.resize(plain.rows);

  for(int i = 0; i < plain.rows; ++i)
  {
    out[i] = plain.row(i);
  }
}

// ----------------------------------------------------------------------------

void createVocabularyFile(OrbVocabulary &voc, std::string &fileName, const vector<vector<cv::Mat > > &features)
{

  cout << "> Creating vocabulary. May take some time ..." << endl;
  voc.create(features);
  cout << "... done!" << endl;

  cout << "> Vocabulary information: " << endl
  << voc << endl << endl;

  // save the vocabulary to disk
  cout << endl << "> Saving vocabulary..." << endl;
  voc.saveToBinaryFile(fileName);
  cout << "... saved to file: " << fileName << endl;
}
// ----------------------------------------------------------------------------
