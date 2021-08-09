#include <time.h>
#include <string>
#include <iostream>

#include "ORBVocabulary.h"

using namespace std;

bool load_as_text(ORB_SLAM2::ORBVocabulary* voc, const std::string infile) {
    clock_t tStart = clock();
    bool res = voc->loadFromTextFile(infile);
    printf("Loading fom text: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return res;
}

//void load_as_xml(ORB_SLAM2::ORBVocabulary* voc, const std::string infile) {
//    clock_t tStart = clock();
//    voc->load(infile);
//    printf("Loading fom xml: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
//}

void load_as_binary(ORB_SLAM2::ORBVocabulary* voc, const std::string infile) {
    clock_t tStart = clock();
    voc->loadFromBinaryFile(infile);
    printf("Loading fom binary: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

//void save_as_xml(ORB_SLAM2::ORBVocabulary* voc, const std::string outfile) {
//    clock_t tStart = clock();
//    voc->save(outfile);
//    printf("Saving as xml: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
//}

void save_as_text(ORB_SLAM2::ORBVocabulary* voc, const std::string outfile) {
    clock_t tStart = clock();
    voc->saveToTextFile(outfile);
    printf("Saving as text: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

void save_as_binary(ORB_SLAM2::ORBVocabulary* voc, const std::string outfile) {
    clock_t tStart = clock();
    voc->saveToBinaryFile(outfile);
    printf("Saving as binary: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}


int main(int argc, char **argv) {
    cout << "BoW load/save benchmark" << endl;
    ORB_SLAM2::ORBVocabulary* voc = new ORB_SLAM2::ORBVocabulary();

    // DEFAULT_ORB_VOCABULARY stores where the vocabulary is installed
    // 
    
    load_as_text(voc, DEFAULT_ORB_VOCABULARY);
    save_as_binary(voc, DEFAULT_BINARY_ORB_VOCABULARY);
    cout << DEFAULT_BINARY_ORB_VOCABULARY << endl;

    return 0;
}
