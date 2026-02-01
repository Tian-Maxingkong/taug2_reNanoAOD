from CRABClient.UserUtilities import config

config = config()

config.General.requestName = "DYto2Mu-2Jets_MLL-50_RunIII2024Summer24MiniAODv6"
config.General.workArea = "crab_projects"
config.General.transferLogs = True
config.General.transferOutputs = True

config.JobType.pluginName = "Analysis"
config.JobType.psetName = "run3_2024_nano_mc_cfg.py"
config.JobType.maxMemoryMB = 2500
# config.JobType.numCores = 8 

config.Data.inputDataset = "/DYto2Mu-2Jets_Bin-MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/RunIII2024Summer24MiniAODv6-150X_mcRun3_2024_realistic_v2-v6/MINIAODSIM"
config.Data.outLFNDirBase = "/store/group/cmst3/group/taug2/NanoAOD_Tianxiao/"
config.Data.outputDatasetTag = "RunIII2024Summer24_NanoAODv15"  
config.Data.inputDBS = "global"
config.Data.splitting = "FileBased"
config.Data.unitsPerJob = 3  
config.Data.publication = False
config.Data.ignoreLocality = True

config.Site.storageSite = "T2_CH_CERN"
config.Site.whitelist = ["T2_*","T3_*"]

