#!/usr/bin/python
import sys, os
import commands as com
from SetUpSample import SetUpSampleAndScript


#
#
def SampleFromSubSamples(SubSampleList):
    """dont know yet """
    
def cleanUpDir(outdir):
    #out=com.getoutput("rm -f merge_script.o*")
    file=open("cleanUpDir","w")
    out=com.getoutput('chmod u+x cleanUpDir')
    print outdir
    line='rm -f merge_script.o*'
    file.write(line+'\n')
    file.close()
    out=com.getoutput("mv cleanUpDir "+outdir+"/")

def makeScriptFromInfoPack(Sample,Config,InfoPack,makeAllScript):
    Scripts=[]
    for pack in InfoPack:
        SubSample=pack[0]
        FilesDir=pack[1]
        nFiles=pack[2]
        script=SetUpSampleAndScript(Sample,SubSample,FilesDir,Config,nFiles)        
        cleanUpDir('./'+Sample+'/'+SubSample)
        Scripts.append(script)
    #
    
    if makeAllScript:
        allfile=open('runall_'+Sample+'_ALL','w')
        for scr in Scripts:
            line='./'+scr+' $1 $2'
            allfile.write(line+'\n')
            #
        #
        allfile.close()
        out=com.getoutput('chmod u+x runall_'+Sample+'_ALL')
        

def SingleMu():

    Sample='SingleMu'
    Config='config_DATA_RA4b.txt'
    InfoPack=[]
    #
    SubSample='Run2012A-13JulReReco'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/SingleMu/Run2012A-13Jul2012-v1/'
    #FilesDir='/scratch/hh/dust/naf/cms/user/eron/SingleMu/Run2012A-13Jul2012-v1/'
    nFiles=6
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='Run2012A-06AugReReco'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/SingleMu/Run2012A-recover-06Aug2012-v1/'
    #FilesDir='/scratch/hh/dust/naf/cms/user/eron/SingleMu/Run2012A-06AugReReco/'
    nFiles=6
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='Run2012B-13JulReReco'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/SingleMu/Run2012B-13Jul2012-v1/'
    #FilesDir='/scratch/hh/dust/naf/cms/user/eron/SingleMu/Run2012B-13JulReReco-v1/'
    nFiles=6
    InfoPack.append((SubSample,FilesDir,nFiles))    
    #
    SubSample='Run2012C-24Aug2012-v1'
    FilesDir='/afs/naf.desy.de/user/c/costanza/workdir/NTuple13_V1/MVAMET4/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_SingleMu_Run2012C24Aug_cfg/'
    #FilesDir='/scratch/hh/dust/naf/cms/user/eron/SingleMu/Run2012C-24Aug2012-v1/'
    nFiles=6
    InfoPack.append((SubSample,FilesDir,nFiles))    
    #
    SubSample='Run2012C-PromptReco-v2'
    FilesDir='/afs/naf.desy.de/user/c/costanza/workdir/NTuple13_V1/MVAMET4/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_SingleMu_Run2012CPromptv2_cfg/'
    #FilesDir='/scratch/hh/dust/naf/cms/user/eron/SingleMu/Run2012C-PromptReco-v2/'
    nFiles=4
    InfoPack.append((SubSample,FilesDir,nFiles))    
    #    
    SubSample='Run2012D-PromptReco-v1'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/SingleMu/Run2012D-PromptReco-v1/'
    nFiles=4
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================

def SingleElectron():

    Sample='SingleElectron'
    Config='config_DATA_RA4b.txt'
    InfoPack=[]
    #
    SubSample='Run2012A-13JulReReco'
    FilesDir='/afs/naf.desy.de/user/l/lobanov/dust/SUSY/nTupler13/data/SingleElectron/Run2012A-13Jul2012-v1/'
    nFiles=6
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='Run2012A-06AugReReco'
    FilesDir='/afs/naf.desy.de/user/l/lobanov/dust/SUSY/nTupler13/data/SingleElectron/Run2012A-recover-06Aug2012-v1/'
    nFiles=6
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='Run2012B-13JulReReco'
    FilesDir='/afs/naf.desy.de/user/l/lobanov/dust/SUSY/nTupler13/data/SingleElectron/Run2012B-13Jul2012-v1/'
    nFiles=10
    InfoPack.append((SubSample,FilesDir,nFiles))    
    #
    SubSample='Run2012C-24Aug2012-v1'
    FilesDir='/afs/naf.desy.de/user/l/lobanov/dust/SUSY/nTupler13/data/SingleElectron/Run2012C-24Aug2012-v1/'
    nFiles=10
    InfoPack.append((SubSample,FilesDir,nFiles))    
    #
    SubSample='Run2012C-PromptReco-v2'
    FilesDir='/afs/naf.desy.de/user/l/lobanov/dust/SUSY/nTupler13/data/SingleElectron/Run2012C-PromptReco-v2/'
    nFiles=4
    InfoPack.append((SubSample,FilesDir,nFiles))    
    #    
    SubSample='Run2012D-PromptReco-v1'
    FilesDir='/afs/naf.desy.de/user/l/lobanov/dust/SUSY/nTupler13/data/SingleElectron/Run2012D-PromptReco-v1/'
    nFiles=2
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================


def TTJetsPOWHEG():
    Sample='TTJetsPOWHEG'
    Config='config_MC_RA4b.txt'
    InfoPack=[]
    #
    SubSample='DR53-v1'    
    nFiles=8
    FilesDir='/afs/naf.desy.de/user/c/costanza/workdir/NTuple13_V1/MVAMET4/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_TTJetsPowhegv1_cfg/'
    InfoPack.append((SubSample,FilesDir,nFiles))    
    #
    SubSample='DR53-v2'
    nFiles=8
    FilesDir='/afs/naf.desy.de/user/c/costanza/workdir/NTuple13_V1/MVAMET4/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_TTJetsPowhegv2_cfg/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================

                    
def TTJetsMG():

    Sample='TTJetsMG'
    Config='config_MC_RA4b.txt'
    InfoPack=[]
    #
    SubSample='FullyHad'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TTJets_HadronicMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A_ext-v1/'
    nFiles=3
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='SemiLept'
    #FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/TTJets_SemiLepttMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A_ext-v1'
    FilesDir='/scratch/hh/dust/naf/cms/user/eron/TTJetsMG/Semi-Lept/'
    nFiles=3
    InfoPack.append((SubSample,FilesDir,nFiles))        
    #
    SubSample='DiLept'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TTJets_FullLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2/'
    nFiles=3
    InfoPack.append((SubSample,FilesDir,nFiles))        
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================
    

def WJetsToLNu():
    Sample='WJetsToLNu'
    Config='config_MC_RA4b.txt'
    InfoPack=[]
    #
    SubSample='Inclusive'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    nFiles=1
    InfoPack=[]
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================

    
    #
    InfoPack=[]
    SubSample='1Jet'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    nFiles=2
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='2Jets'
    FilesDir='/afs/naf.desy.de/user/c/costanza/workdir/NTuple13_V1/MVAMET4/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_W2JetsToLNu_cfg/'
    nFiles=6
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='3Jets'
    FilesDir='/afs/naf.desy.de/user/c/costanza/workdir/NTuple13_V1/MVAMET4/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_W3JetsToLNu_cfg/'
    nFiles=6
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='4Jets'
    FilesDir='/afs/naf.desy.de/user/c/costanza/workdir/NTuple13_V1/MVAMET4/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_W4JetsToLNu_cfg/'
    nFiles=6
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================


def SingleTop():
    Sample='SingleTop'
    Config='config_MC_RA4b.txt'
    nFiles=6    
    InfoPack=[]
    #
    SubSample='TBarToDilepton-tW-ch'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TBarToDilepton_tW-channel-DR_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TBarToLeptons-s-ch'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TBarToLeptons_s-channel_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TBarToLeptons-t-ch'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TBarToLeptons_t-channel_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TBarToThadWlep-tW-ch'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TBarToThadWlep_tW-channel-DR_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TBarToTlepWhad-tW-ch'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TBarToTlepWhad_tW-channel-DR_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TToDilepton-tW-ch'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TToDilepton_tW-channel-DR_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TToLeptons-s-ch'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TToLeptons_s-channel_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TToLeptons-t-ch'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TToLeptons_t-channel_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TToThadWlep-tW-ch'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TToThadWlep_tW-channel-DR_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TToTlepWhad-tW-ch'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v1/mc/TToTlepWhad_tW-channel-DR_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================    


def DiBoson():
    Sample='DiBoson'
    Config='config_MC_RA4b.txt'
    nFiles=6
    InfoPack=[]
    #
    SubSample='ZZJetsTo4'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/ZZJetsTo4L_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='WZJetsTo3LNu'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='WWJetsTo2L2Nu'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='ZZJetsTo2L2Nu'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v3/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='ZZJetsTo2L2Q'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='WZJetsTo2L2Q'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='WGstarToLNu2E'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/WGstarToLNu2E_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='WGstarToLNu2Mu'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/WGstarToLNu2Mu_TuneZ2star_7TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='WGstarToLNu2Tau'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/WGstarToLNu2Tau_TuneZ2star_7TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================    

    
def TriBoson():
    Sample='TriBoson'
    Config='config_MC_RA4b.txt'
    nFiles=6
    InfoPack=[]
    #
    SubSample='ZZZNoGstarJets'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/ZZZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='WWWJets'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/WWWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='WWZNoGstarJets'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/WWZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='WZZNoGstarJets'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/WZZNoGstarJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='WWGJets'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/WWGJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================    
    

def TTV():
    Sample='TTV'
    Config='config_MC_RA4b.txt'
    nFiles=6
    InfoPack=[]
    #    
    SubSample='TTZJets'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/TTZJets_8TeV-madgraph_v2/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TTWJets'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/TTWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TTGJets'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/TTGJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TTWWJets'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/TTWWJets_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='TBZToLL'
    FilesDir='/pnfs/desy.de/cms/tier2/store/user/fcostanz/NTuple13_v1/TBZToLL_4F_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================


def QCD():
    Sample='QCD'
    Config='config_MC_RA4b.txt'
    nFiles=6
    InfoPack=[]
    #    
    SubSample='HTbinned-100To250'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/QCD_HT-100To250_TuneZ2star_8TeV-madgraph-pythia/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='HTbinned-250To500'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/QCD_HT-250To500_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='HTbinned-500To1000'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/QCD_HT-500To1000_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='HTbinned-1000ToInf'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/QCD_HT-1000ToInf_TuneZ2star_8TeV-madgraph-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='MuEnriched'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/nTuple13_v2/mc/QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v3/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='EMEnriched-20To30'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/QCD_Pt_20_30_EMEnriched_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='EMEnriched-30To80'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/is/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_QCD_Pt30To80EMEnriched_cfg'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='EMEnriched-80To170'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/is/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_QCD_Pt80To170EMEnriched_cfg'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='EMEnriched-170To250'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/is/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_QCD_Pt170To250EMEnriched_cfg'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='EMEnriched-250To350'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/is/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_QCD_Pt250To350EMEnriched_cfg'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #    
    SubSample='EMEnriched-350ToInf'
    FilesDir='/scratch/hh/dust/naf/cms/user/sahin/is/CMSSW_5_3_3_patch2/src/SUSYBSMAnalysis/SusyCAF/test/naf_QCD_Pt350ToInfEMEnriched_cfg'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================

def T2ttPoints():
    Sample='T2ttPoints'
    Config='config_MC_RA4b.txt'
    nFiles=12
    InfoPack=[]
    
    SubSample='Stop400-LSP150'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/SMS-T2tt_2J_mStop-400_mLSP-150_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='Stop500-LSP300'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/SMS-T2tt_2J_mStop-500_mLSP-300_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    SubSample='Stop600-LSP50'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/SMS-T2tt_2J_mStop-600_mLSP-50_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #    
    SubSample='Stop750-LSP25'
    FilesDir='/scratch/hh/dust/naf/cms/user/costanza/store/NTuple13_v2/SMS-T2tt_2J_mStop-750_mLSP-25_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/'
    InfoPack.append((SubSample,FilesDir,nFiles))
    #
    #
    makeScriptFromInfoPack(Sample,Config,InfoPack,True)        
    #=======================================



    
if __name__=='__main__':

    SingleMu()
    SingleElectron()
    TTJetsPOWHEG() 
    #TTJetsMG()
    WJetsToLNu()
    SingleTop()
    DiBoson()
    TriBoson()
    TTV()
    QCD()
    T2ttPoints()
