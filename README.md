# GCNv2 SLAM

## Introduction
GCNv2 is a high-throughput variant of the Geometric Correspondence Network for performing RGB-D SLAM online on embedded platforms. We trained the binary descriptor in the same format with ORB (32 bytes) for the convenience of integration. In this implementation, we evaluate the motion estimation using a system built on top the [ORB-SLAM2], (https://github.com/raulmur/ORB_SLAM2). Thanks to the robustness of ORB-SLAM2, our system is able to achive reliable tracking perfomance on our drone platform in real-time. 

## Example
Online running performance with ORB and GCNv2 features:

ORB:

![](orb.gif)

GCNv2:

![](gcn.gif)

## Related Publications

* **[GCNv2: Efficient Correspondence Prediction for Real-Time SLAM](http://bsg.lericson.se/gcnv2.pdf)**, *J. Tang, L. Ericson, J. Folkesson and P. Jensfelt*, arxiv '19
* **[Geometric Correspondence Network for Camera Motion Estimation](http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=8260906&isnumber=8214927)**, *J. Tang, J. Folkesson and P. Jensfelt*, RA-L and ICRA '18

# Dependencies

## C++11 or C++0x Compiler
We use the new thread and chrono functionalities of C++11.

## Pytorch
We use [Pytorch](https://github.com/pytorch/pytorch) for deloying the GCNv2. Dowload and install instructions can be found at: https://pytorch.org/. 

## Pangolin
We use [Pangolin](https://github.com/stevenlovegrove/Pangolin) for visualization and user interface. Dowload and install instructions can be found at: https://github.com/stevenlovegrove/Pangolin.

## OpenCV
We use [OpenCV](http://opencv.org) to manipulate images and features. Dowload and install instructions can be found at: http://opencv.org. **Required at leat 2.4.3. Tested with OpenCV 2.4.11 and OpenCV 3.2**.

## Eigen3
Required by g2o (see below). Download and install instructions can be found at: http://eigen.tuxfamily.org. **Required at least 3.1.0**.

## DBoW2 and g2o (Included in Thirdparty folder)
We use modified versions of the [DBoW2](https://github.com/dorian3d/DBoW2) library to perform place recognition and [g2o](https://github.com/RainerKuemmerle/g2o) library to perform non-linear optimizations. Both modified libraries (which are BSD) are included in the *Thirdparty* folder.

# Preparation
Clone the code
```
git clone https://github.com/jiexiong2016/GCNv2_SLAM.git
```
Then build the project 
```
cd GCNv2_SLAM
./build.sh
```
Make sure to edit `build.sh` pointing to your local libtorch installation. Edit `run.sh` to check out how to run with GCNv2 or vanilla ORB.

# Demonstration video

[![YouTube video thumbnail](https://i.ytimg.com/vi/pz-gdnR9tAM/hqdefault.jpg)](https://www.youtube.com/watch?v=pz-gdnR9tAM)
