/**
* This file is part of ORB-SLAM2.
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


#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>
#include<iomanip>

#include<opencv2/core/core.hpp>

#include"System.h"

using namespace std;

void LoadImages(const string &strSequence, vector<string> &vstrImageFilenames,
                vector<double> &vTimestamps);

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << endl << "Usage: ./mono_kitti path_to_settings path_to_sequence" << endl;
        return 1;
    }

    // Retrieve paths to images
    vector<string> vstrImageFilenames;
    vector<double> vTimestamps;
    LoadImages(string(argv[2]), vstrImageFilenames, vTimestamps);

    int nImages = vstrImageFilenames.size();

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    string settingsFile = string(DEFAULT_SETTINGS_DIRECTORY) + string("/") + string(argv[1]);
    ORB_SLAM2::System SLAM(DEFAULT_BINARY_ORB_VOCABULARY, settingsFile, ORB_SLAM2::System::MONOCULAR,
                           true);//binary readin
//    ORB_SLAM2::System SLAM(DEFAULT_ORB_VOCABULARY,settingsFile,ORB_SLAM2::System::MONOCULAR,true);// original txt vocabulary file read in

    // ORB_SLAM2::System SLAM(DEFAULT_ORB_VOCABULARY,argv[1],ORB_SLAM2::System::MONOCULAR,true);

    // Vector for tracking time statistics
    vector<float> vTimesTrack;
    vTimesTrack.resize(nImages);

    cout << endl << "-------" << endl;
    cout << "Start processing sequence ..." << endl;
    cout << "Images in the sequence: " << nImages << endl << endl;

    int main_error = 0;
    std::thread runthread([&]() {  // Start in new thread
        // Main loop
        cv::Mat im;
        for (int ni = 0; ni < nImages; ni++) {
            // Read image from file
            im = cv::imread(vstrImageFilenames[ni], cv::IMREAD_UNCHANGED);
            double tframe = vTimestamps[ni];

            if (im.empty()) {
                cerr << endl << "Failed to load image at: " << vstrImageFilenames[ni] << endl;
                return 1;
            }

            chrono::steady_clock::time_point t1 = chrono::steady_clock::now();

            // Pass the image to the SLAM system
            SLAM.TrackMonocular(im, tframe);

            chrono::steady_clock::time_point t2 = chrono::steady_clock::now();

            double ttrack = chrono::duration_cast<chrono::duration<double> >(t2 - t1).count();

            vTimesTrack[ni] = ttrack;

            // Wait to load the next frame
            double T = 0;
            if (ni < nImages - 1)
                T = vTimestamps[ni + 1] - tframe;
            else if (ni > 0)
                T = tframe - vTimestamps[ni - 1];

            if (ttrack < T)
                this_thread::sleep_for(chrono::duration<double>(T - ttrack));
            //            usleep((T-ttrack)*1e6);
        }
    });
    SLAM.StartViewer();

    cout << "Viewer started, waiting for thread." << endl;
    runthread.join();
    if (main_error != 0)
        return main_error;
    cout << "Tracking thread joined..." << endl;

    // Stop all threads
    SLAM.Shutdown();

    // Tracking time statistics
    sort(vTimesTrack.begin(), vTimesTrack.end());
    float totaltime = 0;
    for (int ni = 0; ni < nImages; ni++) {
        totaltime += vTimesTrack[ni];
    }
    cout << "-------" << endl << endl;
    cout << "median tracking time: " << vTimesTrack[nImages / 2] << endl;
    cout << "mean tracking time: " << totaltime / nImages << endl;

    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

    return 0;
}

void LoadImages(const string &strPathToSequence, vector<string> &vstrImageFilenames, vector<double> &vTimestamps) {
    ifstream fTimes;
    string strPathTimeFile = strPathToSequence + "/times.txt";
    fTimes.open(strPathTimeFile.c_str());
    while (!fTimes.eof()) {
        string s;
        getline(fTimes, s);
        if (!s.empty()) {
            stringstream ss;
            ss << s;
            double t;
            ss >> t;
            vTimestamps.push_back(t);
        }
    }

    string strPrefixLeft = strPathToSequence + "/image_0/";

    const int nTimes = vTimestamps.size();
    vstrImageFilenames.resize(nTimes);

    for (int i = 0; i < nTimes; i++) {
        stringstream ss;
        ss << setfill('0') << setw(6) << i;
        vstrImageFilenames[i] = strPrefixLeft + ss.str() + ".png";
    }
}
