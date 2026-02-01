from CRABClient.UserUtilities import config
import time

config = config()


timestamp = time.strftime("%Y%m%d")
config.General.requestName = f"Muon2024_{DATASET_TAG}_{timestamp}"
config.General.workArea = "crab_projects_muon2024"
config.General.transferLogs = True
config.General.transferOutputs = True

config.JobType.pluginName = "Analysis"
config.JobType.psetName = "run3_2024_nano_data_cfg.py"
config.JobType.maxMemoryMB = 2500
#config.JobType.numCores = 8 

config.Data.inputDataset = "{INPUT_DATASET}" 
config.Data.outLFNDirBase = f"/store/group/cmst3/group/taug2/NanoAOD_Tianxiao/Muon2024/{DATASET_TAG}/"
config.Data.outputDatasetTag = "Run2024_NanoAODv15"
config.Data.inputDBS = "global"  
config.Data.splitting = "FileBased"
config.Data.unitsPerJob = 3 
config.Data.publication = False
config.Data.ignoreLocality = True

config.Site.storageSite = "T2_CH_CERN"  
config.Site.whitelist = ["T2_*", "T3_*"]