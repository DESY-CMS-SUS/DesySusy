

import os,sys
import commands as com
from pytools.SampleClasses import Sample  

def SingleMu_CR1_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    Samp.AddRootFile('SingleMu/Run2012A-13JulReReco/SingleMu_Run2012A-13JulReReco_CR1_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012A-06AugReReco/SingleMu_Run2012A-06AugReReco_CR1_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012B-13JulReReco/SingleMu_Run2012B-13JulReReco_CR1_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012C-24Aug2012-v1/SingleMu_Run2012C-24Aug2012-v1_CR1_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012C-PromptReco-v2/SingleMu_Run2012C-PromptReco-v2_CR1_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp

def TTJetsMG_CR1_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #print 'currently in ',com.getoutput('pwd')
    #raw_input('')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/SemiLept/TTJetsMG_SemiLept_CR1_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/DiLept/TTJetsMG_DiLept_CR1_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/FullyHad/TTJetsMG_FullyHad_CR1_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp    

def WJetsToLNuJets_CR1_NoTail(options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/1Jet/WJetsToLNu_1Jet_CR1_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/2Jets/WJetsToLNu_2Jets_CR1_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/3Jets/WJetsToLNu_3Jets_CR1_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/4Jets/WJetsToLNu_4Jets_CR1_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp    

def SingleTop_CR1_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/SingleTop/'
    Samp.AddRootFile(topdir+'TToTlepWhad-tW-ch/SingleTop_TToTlepWhad-tW-ch_CR1_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-t-ch/SingleTop_TBarToLeptons-t-ch_CR1_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToTlepWhad-tW-ch/SingleTop_TBarToTlepWhad-tW-ch_CR1_NoTail.root')
    Samp.AddRootFile(topdir+'TToLeptons-t-ch/SingleTop_TToLeptons-t-ch_CR1_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToThadWlep-tW-ch/SingleTop_TBarToThadWlep-tW-ch_CR1_NoTail.root')
    Samp.AddRootFile(topdir+'TToThadWlep-tW-ch/SingleTop_TToThadWlep-tW-ch_CR1_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToDilepton-tW-ch/SingleTop_TBarToDilepton-tW-ch_CR1_NoTail.root')
    Samp.AddRootFile(topdir+'TToDilepton-tW-ch/SingleTop_TToDilepton-tW-ch_CR1_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-s-ch/SingleTop_TBarToLeptons-s-ch_CR1_NoTail.root')
    Samp.AddRootFile(topdir+'TToLeptons-s-ch/SingleTop_TToLeptons-s-ch_CR1_NoTail.root')    
    Samp.ReweightAndMerge(options)
    return Samp    













def SingleMu_CR1Muons_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    Samp.AddRootFile('SingleMu/Run2012A-13JulReReco/SingleMu_Run2012A-13JulReReco_CR1Muons_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012A-06AugReReco/SingleMu_Run2012A-06AugReReco_CR1Muons_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012B-13JulReReco/SingleMu_Run2012B-13JulReReco_CR1Muons_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012C-24Aug2012-v1/SingleMu_Run2012C-24Aug2012-v1_CR1Muons_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012C-PromptReco-v2/SingleMu_Run2012C-PromptReco-v2_CR1Muons_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp

def TTJetsMG_CR1Muons_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #print 'currently in ',com.getoutput('pwd')
    #raw_input('')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/SemiLept/TTJetsMG_SemiLept_CR1Muons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/DiLept/TTJetsMG_DiLept_CR1Muons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/FullyHad/TTJetsMG_FullyHad_CR1Muons_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp    

    
def WJetsToLNu_CR1Muons_NoTail(options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/1Jet/WJetsToLNu_1Jet_CR1Muons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/2Jets/WJetsToLNu_2Jets_CR1Muons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/3Jets/WJetsToLNu_3Jets_CR1Muons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/4Jets/WJetsToLNu_4Jets_CR1Muons_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp

def WJetsToLNuInclusive_CR1Muons_NoTail(options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/Inclusive/WJetsToLNu_Inclusive_CR1Muons_NoTail.root')
    Samp.ReweightAndMerge(options)    
    return Samp


def SingleTop_CR1Muons_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/SingleTop/'
    Samp.AddRootFile(topdir+'TToTlepWhad-tW-ch/SingleTop_TToTlepWhad-tW-ch_CR1Muons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-t-ch/SingleTop_TBarToLeptons-t-ch_CR1Muons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToTlepWhad-tW-ch/SingleTop_TBarToTlepWhad-tW-ch_CR1Muons_NoTail.root')
    Samp.AddRootFile(topdir+'TToLeptons-t-ch/SingleTop_TToLeptons-t-ch_CR1Muons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToThadWlep-tW-ch/SingleTop_TBarToThadWlep-tW-ch_CR1Muons_NoTail.root')
    Samp.AddRootFile(topdir+'TToThadWlep-tW-ch/SingleTop_TToThadWlep-tW-ch_CR1Muons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToDilepton-tW-ch/SingleTop_TBarToDilepton-tW-ch_CR1Muons_NoTail.root')
    Samp.AddRootFile(topdir+'TToDilepton-tW-ch/SingleTop_TToDilepton-tW-ch_CR1Muons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-s-ch/SingleTop_TBarToLeptons-s-ch_CR1Muons_NoTail.root')
    Samp.AddRootFile(topdir+'TToLeptons-s-ch/SingleTop_TToLeptons-s-ch_CR1Muons_NoTail.root')    
    Samp.ReweightAndMerge(options)
    return Samp


def TTJetsPWHG_CR1Muons_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsPWHG/'    
    Samp.AddRootFile(topdir+'v1/TTJetsPWHG_v1_CR1Muons_NoTail.root')
    Samp.AddRootFile(topdir+'v2/TTJetsPWHG_v2_CR1Muons_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp    








def SingleMu_CRFMuons_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    Samp.AddRootFile('SingleMu/Run2012A-13JulReReco/SingleMu_Run2012A-13JulReReco_CRFMuons_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012A-06AugReReco/SingleMu_Run2012A-06AugReReco_CRFMuons_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012B-13JulReReco/SingleMu_Run2012B-13JulReReco_CRFMuons_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012C-24Aug2012-v1/SingleMu_Run2012C-24Aug2012-v1_CRFMuons_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012C-PromptReco-v2/SingleMu_Run2012C-PromptReco-v2_CRFMuons_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp

def TTJetsMG_CRFMuons_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #print 'currently in ',com.getoutput('pwd')
    #raw_input('')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/SemiLept/TTJetsMG_SemiLept_CRFMuons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/DiLept/TTJetsMG_DiLept_CRFMuons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/FullyHad/TTJetsMG_FullyHad_CRFMuons_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp    

    
def WJetsToLNu_CRFMuons_NoTail(options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/1Jet/WJetsToLNu_1Jet_CRFMuons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/2Jets/WJetsToLNu_2Jets_CRFMuons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/3Jets/WJetsToLNu_3Jets_CRFMuons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/4Jets/WJetsToLNu_4Jets_CRFMuons_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp

def WJetsToLNuInclusive_CRFMuons_NoTail(options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/Inclusive/WJetsToLNu_Inclusive_CRFMuons_NoTail.root')
    Samp.ReweightAndMerge(options)    
    return Samp


def SingleTop_CRFMuons_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/SingleTop/'
    Samp.AddRootFile(topdir+'TToTlepWhad-tW-ch/SingleTop_TToTlepWhad-tW-ch_CRFMuons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-t-ch/SingleTop_TBarToLeptons-t-ch_CRFMuons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToTlepWhad-tW-ch/SingleTop_TBarToTlepWhad-tW-ch_CRFMuons_NoTail.root')
    Samp.AddRootFile(topdir+'TToLeptons-t-ch/SingleTop_TToLeptons-t-ch_CRFMuons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToThadWlep-tW-ch/SingleTop_TBarToThadWlep-tW-ch_CRFMuons_NoTail.root')
    Samp.AddRootFile(topdir+'TToThadWlep-tW-ch/SingleTop_TToThadWlep-tW-ch_CRFMuons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToDilepton-tW-ch/SingleTop_TBarToDilepton-tW-ch_CRFMuons_NoTail.root')
    Samp.AddRootFile(topdir+'TToDilepton-tW-ch/SingleTop_TToDilepton-tW-ch_CRFMuons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-s-ch/SingleTop_TBarToLeptons-s-ch_CRFMuons_NoTail.root')
    Samp.AddRootFile(topdir+'TToLeptons-s-ch/SingleTop_TToLeptons-s-ch_CRFMuons_NoTail.root')    
    Samp.ReweightAndMerge(options)
    return Samp    














def SingleEl_CR1Electrons_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    Samp.AddRootFile('SingleEl/Run2012A-13JulReReco/SingleEl_Run2012A-13JulReReco_CR1Electrons_NoTail.root')
    Samp.AddRootFile('SingleEl/Run2012A-06AugReReco/SingleEl_Run2012A-06AugReReco_CR1Electrons_NoTail.root')
    Samp.AddRootFile('SingleEl/Run2012B-13JulReReco/SingleEl_Run2012B-13JulReReco_CR1Electrons_NoTail.root')
    Samp.AddRootFile('SingleEl/Run2012C-24Aug2012-v1/SingleEl_Run2012C-24Aug2012-v1_CR1Electrons_NoTail.root')
    Samp.AddRootFile('SingleEl/Run2012C-PromptReco-v2/SingleEl_Run2012C-PromptReco-v2_CR1Electrons_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp

def TTJetsMG_CR1Electrons_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #print 'currently in ',com.getoutput('pwd')
    #raw_input('')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/SemiLept/TTJetsMG_SemiLept_CR1Electrons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/DiLept/TTJetsMG_DiLept_CR1Electrons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/FullyHad/TTJetsMG_FullyHad_CR1Electrons_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp    

    
def WJetsToLNu_CR1Electrons_NoTail(options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/1Jet/WJetsToLNu_1Jet_CR1Electrons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/2Jets/WJetsToLNu_2Jets_CR1Electrons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/3Jets/WJetsToLNu_3Jets_CR1Electrons_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/4Jets/WJetsToLNu_4Jets_CR1Electrons_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp

def WJetsToLNuInclusive_CR1Electrons_NoTail(options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/Inclusive/WJetsToLNu_Inclusive_CR1Electrons_NoTail.root')
    Samp.ReweightAndMerge(options)    
    return Samp


def SingleTop_CR1Electrons_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/SingleTop/'
    Samp.AddRootFile(topdir+'TToTlepWhad-tW-ch/SingleTop_TToTlepWhad-tW-ch_CR1Electrons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-t-ch/SingleTop_TBarToLeptons-t-ch_CR1Electrons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToTlepWhad-tW-ch/SingleTop_TBarToTlepWhad-tW-ch_CR1Electrons_NoTail.root')
    Samp.AddRootFile(topdir+'TToLeptons-t-ch/SingleTop_TToLeptons-t-ch_CR1Electrons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToThadWlep-tW-ch/SingleTop_TBarToThadWlep-tW-ch_CR1Electrons_NoTail.root')
    Samp.AddRootFile(topdir+'TToThadWlep-tW-ch/SingleTop_TToThadWlep-tW-ch_CR1Electrons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToDilepton-tW-ch/SingleTop_TBarToDilepton-tW-ch_CR1Electrons_NoTail.root')
    Samp.AddRootFile(topdir+'TToDilepton-tW-ch/SingleTop_TToDilepton-tW-ch_CR1Electrons_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-s-ch/SingleTop_TBarToLeptons-s-ch_CR1Electrons_NoTail.root')
    Samp.AddRootFile(topdir+'TToLeptons-s-ch/SingleTop_TToLeptons-s-ch_CR1Electrons_NoTail.root')    
    Samp.ReweightAndMerge(options)
    return Samp


def TTJetsPWHG_CR1Electrons_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsPWHG/'    
    Samp.AddRootFile(topdir+'v1/TTJetsPWHG_v1_CR1Electrons_NoTail.root')
    Samp.AddRootFile(topdir+'v2/TTJetsPWHG_v2_CR1Electrons_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp    


















def SingleMu_CR1Muonsbaseline_NoTail(options):

    Samp=Sample()
    Samp.isData=True
    Samp.AddRootFile('SingleMu/Run2012A-13JulReReco/SingleMu_Run2012A-13JulReReco_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012A-06AugReReco/SingleMu_Run2012A-06AugReReco_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012B-13JulReReco/SingleMu_Run2012B-13JulReReco_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012C-24Aug2012-v1/SingleMu_Run2012C-24Aug2012-v1_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile('SingleMu/Run2012C-PromptReco-v2/SingleMu_Run2012C-PromptReco-v2_CR1Muons-baseline_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp

def TTJetsMG_CR1Muonsbaseline_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    #print 'currently in ',com.getoutput('pwd')
    #raw_input('')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/SemiLept/TTJetsMG_SemiLept_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/DiLept/TTJetsMG_DiLept_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/FullyHad/TTJetsMG_FullyHad_CR1Muons-baseline_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp    

    
def WJetsToLNu_CR1Muonsbaseline_NoTail(options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/1Jet/WJetsToLNu_1Jet_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/2Jets/WJetsToLNu_2Jets_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/3Jets/WJetsToLNu_3Jets_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/4Jets/WJetsToLNu_4Jets_CR1Muons-baseline_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp

def WJetsToLNuInclusive_CR1Muonsbaseline_NoTail(options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/Inclusive/WJetsToLNu_Inclusive_CR1Muons-baseline_NoTail.root')
    Samp.ReweightAndMerge(options)    
    return Samp


def SingleTop_CR1Muonsbaseline_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/SingleTop/'
    Samp.AddRootFile(topdir+'TToTlepWhad-tW-ch/SingleTop_TToTlepWhad-tW-ch_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-t-ch/SingleTop_TBarToLeptons-t-ch_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToTlepWhad-tW-ch/SingleTop_TBarToTlepWhad-tW-ch_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile(topdir+'TToLeptons-t-ch/SingleTop_TToLeptons-t-ch_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToThadWlep-tW-ch/SingleTop_TBarToThadWlep-tW-ch_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile(topdir+'TToThadWlep-tW-ch/SingleTop_TToThadWlep-tW-ch_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToDilepton-tW-ch/SingleTop_TBarToDilepton-tW-ch_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile(topdir+'TToDilepton-tW-ch/SingleTop_TToDilepton-tW-ch_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-s-ch/SingleTop_TBarToLeptons-s-ch_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile(topdir+'TToLeptons-s-ch/SingleTop_TToLeptons-s-ch_CR1Muons-baseline_NoTail.root')    
    Samp.ReweightAndMerge(options)
    return Samp


def TTJetsPWHG_CR1Muonsbaseline_NoTail(options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsPWHG/'    
    Samp.AddRootFile(topdir+'v1/TTJetsPWHG_v1_CR1Muons-baseline_NoTail.root')
    Samp.AddRootFile(topdir+'v2/TTJetsPWHG_v2_CR1Muons-baseline_NoTail.root')
    Samp.ReweightAndMerge(options)
    return Samp    













def SingleMu_CR1Muonsbaseline_ControlPlots(options):

    Samp=Sample()
    Samp.isData=True
    Samp.AddRootFile('SingleMu/Run2012A-13JulReReco/SingleMu_Run2012A-13JulReReco_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile('SingleMu/Run2012A-06AugReReco/SingleMu_Run2012A-06AugReReco_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile('SingleMu/Run2012B-13JulReReco/SingleMu_Run2012B-13JulReReco_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile('SingleMu/Run2012C-24Aug2012-v1/SingleMu_Run2012C-24Aug2012-v1_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile('SingleMu/Run2012C-PromptReco-v2/SingleMu_Run2012C-PromptReco-v2_CR1Muons-baseline_ControlPlots.root')
    Samp.ReweightAndMerge(options)
    return Samp

def TTJetsMG_CR1Muonsbaseline_ControlPlots(options):

    Samp=Sample()
    Samp.isData=False
    #print 'currently in ',com.getoutput('pwd')
    #raw_input('')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/SemiLept/TTJetsMG_SemiLept_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/DiLept/TTJetsMG_DiLept_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/FullyHad/TTJetsMG_FullyHad_CR1Muons-baseline_ControlPlots.root')
    Samp.ReweightAndMerge(options)
    return Samp    

    
def WJetsToLNu_CR1Muonsbaseline_ControlPlots(options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/1Jet/WJetsToLNu_1Jet_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/2Jets/WJetsToLNu_2Jets_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/3Jets/WJetsToLNu_3Jets_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/4Jets/WJetsToLNu_4Jets_CR1Muons-baseline_ControlPlots.root')
    Samp.ReweightAndMerge(options)
    return Samp

def WJetsToLNuInclusive_CR1Muonsbaseline_ControlPlots(options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/Inclusive/WJetsToLNu_Inclusive_CR1Muons-baseline_ControlPlots.root')
    Samp.ReweightAndMerge(options)    
    return Samp


def SingleTop_CR1Muonsbaseline_ControlPlots(options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/SingleTop/'
    Samp.AddRootFile(topdir+'TToTlepWhad-tW-ch/SingleTop_TToTlepWhad-tW-ch_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-t-ch/SingleTop_TBarToLeptons-t-ch_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile(topdir+'TBarToTlepWhad-tW-ch/SingleTop_TBarToTlepWhad-tW-ch_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile(topdir+'TToLeptons-t-ch/SingleTop_TToLeptons-t-ch_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile(topdir+'TBarToThadWlep-tW-ch/SingleTop_TBarToThadWlep-tW-ch_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile(topdir+'TToThadWlep-tW-ch/SingleTop_TToThadWlep-tW-ch_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile(topdir+'TBarToDilepton-tW-ch/SingleTop_TBarToDilepton-tW-ch_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile(topdir+'TToDilepton-tW-ch/SingleTop_TToDilepton-tW-ch_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-s-ch/SingleTop_TBarToLeptons-s-ch_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile(topdir+'TToLeptons-s-ch/SingleTop_TToLeptons-s-ch_CR1Muons-baseline_ControlPlots.root')    
    Samp.ReweightAndMerge(options)
    return Samp


def TTJetsPWHG_CR1Muonsbaseline_ControlPlots(options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsPWHG/'    
    Samp.AddRootFile(topdir+'v1/TTJetsPWHG_v1_CR1Muons-baseline_ControlPlots.root')
    Samp.AddRootFile(topdir+'v2/TTJetsPWHG_v2_CR1Muons-baseline_ControlPlots.root')
    Samp.ReweightAndMerge(options)
    return Samp    








def SingleMu_FromVariables(estimation,tail,options):

    Samp=Sample()
    Samp.isData=True
    Samp.AddRootFile('SingleMu/Run2012A-13JulReReco/SingleMu_Run2012A-13JulReReco_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('SingleMu/Run2012A-06AugReReco/SingleMu_Run2012A-06AugReReco_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('SingleMu/Run2012B-13JulReReco/SingleMu_Run2012B-13JulReReco_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('SingleMu/Run2012C-24Aug2012-v1/SingleMu_Run2012C-24Aug2012-v1_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('SingleMu/Run2012C-PromptReco-v2/SingleMu_Run2012C-PromptReco-v2_'+estimation+'_'+tail+'.root')
    Samp.ReweightAndMerge(options)
    return Samp


def SingleEl_FromVariables(estimation,tail,options):

    Samp=Sample()
    Samp.isData=True
    Samp.AddRootFile('SingleEl/Run2012A-13JulReReco/SingleEl_Run2012A-13JulReReco_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('SingleEl/Run2012A-06AugReReco/SingleEl_Run2012A-06AugReReco_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('SingleEl/Run2012B-13JulReReco/SingleEl_Run2012B-13JulReReco_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('SingleEl/Run2012C-24Aug2012-v1/SingleEl_Run2012C-24Aug2012-v1_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('SingleEl/Run2012C-PromptReco-v2/SingleEl_Run2012C-PromptReco-v2_'+estimation+'_'+tail+'.root')
    Samp.ReweightAndMerge(options)
    return Samp


def TTJetsMG_FromVariables(estimation,tail,options):

    Samp=Sample()
    Samp.isData=False
    #print 'currently in ',com.getoutput('pwd')
    #raw_input('')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/SemiLept/TTJetsMG_SemiLept_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/DiLept/TTJetsMG_DiLept_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsMG/FullyHad/TTJetsMG_FullyHad_'+estimation+'_'+tail+'.root')
    Samp.ReweightAndMerge(options)
    return Samp    

    
def WJetsToLNu_FromVariables(estimation,tail,options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/1Jet/WJetsToLNu_1Jet_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/2Jets/WJetsToLNu_2Jets_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/3Jets/WJetsToLNu_3Jets_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/4Jets/WJetsToLNu_4Jets_'+estimation+'_'+tail+'.root')
    Samp.ReweightAndMerge(options)
    return Samp

def WJetsToLNuInclusive_FromVariables(estimation,tail,options):
    Samp=Sample()
    Samp.isData=False
    Samp.AddRootFile('/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/WJetsToLNu/Inclusive/WJetsToLNu_Inclusive_'+estimation+'_'+tail+'.root')
    Samp.ReweightAndMerge(options)    
    return Samp


def SingleTop_FromVariables(estimation,tail,options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/SingleTop/'
    Samp.AddRootFile(topdir+'TToTlepWhad-tW-ch/SingleTop_TToTlepWhad-tW-ch_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-t-ch/SingleTop_TBarToLeptons-t-ch_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile(topdir+'TBarToTlepWhad-tW-ch/SingleTop_TBarToTlepWhad-tW-ch_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile(topdir+'TToLeptons-t-ch/SingleTop_TToLeptons-t-ch_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile(topdir+'TBarToThadWlep-tW-ch/SingleTop_TBarToThadWlep-tW-ch_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile(topdir+'TToThadWlep-tW-ch/SingleTop_TToThadWlep-tW-ch_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile(topdir+'TBarToDilepton-tW-ch/SingleTop_TBarToDilepton-tW-ch_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile(topdir+'TToDilepton-tW-ch/SingleTop_TToDilepton-tW-ch_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile(topdir+'TBarToLeptons-s-ch/SingleTop_TBarToLeptons-s-ch_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile(topdir+'TToLeptons-s-ch/SingleTop_TToLeptons-s-ch_'+estimation+'_'+tail+'.root')    
    Samp.ReweightAndMerge(options)
    return Samp


def TTJetsPWHG_FromVariables(estimation,tail,options):

    Samp=Sample()
    Samp.isData=False
    topdir='/scratch/hh/dust/naf/cms/user/eron/RA4b/nTuple13_v1/TTJetsPWHG/'    
    Samp.AddRootFile(topdir+'v1/TTJetsPWHG_v1_'+estimation+'_'+tail+'.root')
    Samp.AddRootFile(topdir+'v2/TTJetsPWHG_v2_'+estimation+'_'+tail+'.root')
    Samp.ReweightAndMerge(options)
    return Samp    














if __name__=='__main__':

    options={}
    #options['RW']=False
    #options['Merge']=True
    #mysamp=SingleEl_CR1_NoTail(options)
    

    #options={}
    #options['RW']=True
    #options['Merge']=True
    #options['dataLumi']=[10000.0]
    #mysamp=TTJetsMG_CR1_NoTail(options)
    
    options={'RW':True,'Merge':True,'dataLumi':[10000.0]}
    myInstance=SingleTop_CR1Muonsbaseline_ControlPlots(options)

    print 'the merged root file is ',myInstance.RootFile
    #

    
