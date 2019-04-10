# GCNv2 trained with more augmentations and larger batch
GCN_PATH=gcn2_aug.pt ./rgbd_gcn ../Vocabulary/GCNvoc.bin TUM3.yaml ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household/associations.txt

# GCNv2
./rgbd_gcn ../Vocabulary/GCNvoc.bin TUM3.yaml ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household/associations.txt

# Vanilla ORB
USE_ORB=1 ./rgbd_gcn ../Vocabulary/ORBvoc.bin TUM3.yaml ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household ~/Workspace/Datasets/TUM/freiburg3/rgbd_dataset_freiburg3_long_office_household/associations.txt
