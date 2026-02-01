import os
import shutil

datasets = {
    "/Muon0/Run2024C-MINIv6NANOv15-v1/MINIAOD": "Run2024C_Muon0_v1",
    "/Muon1/Run2024C-MINIv6NANOv15-v1/MINIAOD": "Run2024C_Muon1_v1",
    "/Muon0/Run2024D-MINIv6NANOv15-v1/MINIAOD": "Run2024D_Muon0_v1",
    "/Muon1/Run2024D-MINIv6NANOv15-v1/MINIAOD": "Run2024D_Muon1_v1",
    "/Muon0/Run2024E-MINIv6NANOv15-v1/MINIAOD": "Run2024E_Muon0_v1",
    "/Muon1/Run2024E-MINIv6NANOv15-v1/MINIAOD": "Run2024E_Muon1_v1",
    "/Muon0/Run2024F-MINIv6NANOv15-v1/MINIAOD": "Run2024F_Muon0_v1",
    "/Muon1/Run2024F-MINIv6NANOv15-v1/MINIAOD": "Run2024F_Muon1_v1",
    "/Muon0/Run2024G-MINIv6NANOv15-v1/MINIAOD": "Run2024G_Muon0_v1",
    "/Muon1/Run2024G-MINIv6NANOv15-v2/MINIAOD": "Run2024G_Muon1_v2",
    "/Muon0/Run2024H-MINIv6NANOv15-v1/MINIAOD": "Run2024H_Muon0_v1",
    "/Muon1/Run2024H-MINIv6NANOv15-v2/MINIAOD": "Run2024H_Muon1_v2",
    "/Muon0/Run2024I-MINIv6NANOv15-v1/MINIAOD": "Run2024I_Muon0_v1",
    "/Muon0/Run2024I-MINIv6NANOv15_v2-v1/MINIAOD": "Run2024I_Muon0_v2",
    "/Muon1/Run2024I-MINIv6NANOv15-v1/MINIAOD": "Run2024I_Muon1_v1",
    "/Muon1/Run2024I-MINIv6NANOv15_v2-v1/MINIAOD": "Run2024I_Muon1_v2",
}

for dataset_path, dataset_tag in datasets.items():
    cfg_file = f"crab_muon_2024_{dataset_tag}.py"
    shutil.copy("submit_crab_Muon.py", cfg_file)
    with open(cfg_file, "r") as f:
        content = f.read()
    content = content.replace("{INPUT_DATASET}", dataset_path)
    content = content.replace("{DATASET_TAG}", dataset_tag)
    with open(cfg_file, "w") as f:
        f.write(content)
    
    submit_code = os.system(f"crab submit {cfg_file}")