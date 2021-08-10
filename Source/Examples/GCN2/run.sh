# Run GCN, removed resolution requirment. The current model was trained with resolution 320x240 as input. Use other resolution may affect the actual performance. Ideally, GCNv2 should be trained/finetuned in desired resolution.
GCN_PATH=gcn2_320x240.pt ./rgbd_gcn ../Vocabulary/GCNvoc.bin TUM3_small.yaml ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household/associations.txt

# 640x480 resolution example
FULL_RESOLUTION=1 GCN_PATH=gcn2_640x480.pt ./rgbd_gcn ../Vocabulary/GCNvoc.bin TUM3.yaml ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household/associations.txt

# Reproduce results in comparison with ORB as in our paper, will enable NN_ONLY and use 320x240 resolution.

# GCNv2
# NN_ONLY=1 GCN_PATH=gcn2_320x240.pt ./rgbd_gcn ../Vocabulary/GCNvoc.bin TUM3_small.yaml ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household/associations.txt

# Vanilla ORB
# NN_ONLY=1 USE_ORB=1 ./rgbd_gcn ../Vocabulary/ORBvoc.bin TUM3_small.yaml ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household/associations.txt
