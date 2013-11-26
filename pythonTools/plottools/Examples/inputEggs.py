import sys,os,copy


def singleMu_CR1MuonsNoMT2_MADGRAPH_InputEgg():
    '''set the egg'''
    D={}
    D['dataSample']='SingleMu'
    D['dataEstimation']='CR1Muons-NoMT2'
    D['dataTail']='NoTail'
    D['dataRWOptions']={'Merge':True,'RW':False}
    dataLumi=12216.611
    D['dataLumi']=dataLumi
    #
    #D['mcSamples']=['TTJetsMG','WJetsToLNu','SingleTop']
    D['mcSamples']=['SingleTop','TTJetsMG','WJetsToLNu']
    D['mcEstimation']='CR1Muons-NoMT2'
    D['mcTail']='NoTail'
    D['mcRWOptions']={'Merge':True,'RW':True,'dataLumi':dataLumi}
    #
    D['sigSamples']=[]
    D['sigEstimation']=''
    D['sigTail']=''
    D['sigRWOptions']={'Merge':True,'RW':False,'dataLumi':dataLumi}
    #D['retrievalFile']='/afs/naf.desy.de/user/e/eron/scratch/UserCode/RA4bHead/UserCode/DesySusy/ra4b_2012/treeSamples.py'
    D['retrievalFile']='treeSamples'

    D['plotDirectory']='metStudies/CR1Muons-NoMT2_NoTail/MADGRAPH'
    D['suffixList']=copy.copy( ['central','up','down',''])        
    
    return D

def singleMu_CR1MuonsNoMT2_POWHEG_InputEgg():   

    D={}
    D['dataSample']='SingleMu'
    D['dataEstimation']='CR1Muons-NoMT2'
    D['dataTail']='NoTail'
    D['dataRWOptions']={'Merge':True,'RW':False}
    dataLumi=12216.611
    D['dataLumi']=dataLumi
    #
    #
    #this is the order in which they will be plotted
    D['mcSamples']=['SingleTop','TTJetsMG','WJetsToLNu']    
    D['mcEstimation']='CR1Muons-NoMT2'
    D['mcTail']='NoTail'
    D['mcRWOptions']={'Merge':True,'RW':True,'dataLumi':dataLumi}
    #
    D['sigSamples']=[]
    D['sigEstimation']=''
    D['sigTail']=''
    D['sigRWOptions']={'Merge':True,'RW':False,'dataLumi':dataLumi}
    #
    D['retrievalFile']='treeSamples'
    D['plotDirectory']='metStudies/CR1Muons-NoMT2_NoTail/POWHEG'
    D['suffixList']=copy.copy( ['central','up','down',''])
        
    return D







def singleMu_CR1Muonsbaseline_MADGRAPH_InputEgg():
    '''set the egg'''
    D={}
    D['dataSample']='SingleMu'
    D['dataEstimation']='CR1Muons-baseline'
    D['dataTail']='ControlPlots'
    D['dataRWOptions']={'Merge':True,'RW':False}
    dataLumi=12216.611
    D['dataLumi']=dataLumi
    #
    #D['mcSamples']=['TTJetsMG','WJetsToLNu','SingleTop']
    D['mcSamples']=['SingleTop','TTJetsMG','WJetsToLNu']
    D['mcEstimation']='CR1Muons-baseline'
    D['mcTail']='ControlPlots'
    D['mcRWOptions']={'Merge':True,'RW':True,'dataLumi':dataLumi}
    #
    D['sigSamples']=[]
    D['sigEstimation']=''
    D['sigTail']=''
    D['sigRWOptions']={'Merge':True,'RW':False,'dataLumi':dataLumi}
    #D['retrievalFile']='/afs/naf.desy.de/user/e/eron/scratch/UserCode/RA4bHead/UserCode/DesySusy/ra4b_2012/treeSamples.py'
    D['retrievalFile']='treeSamples'
    D['plotDirectory']='metStudies/CR1Muons-baseline_ControlPlots/MADGRAPH'

    
    return D





def singleMu_FromVariables_MADGRAPH_InputEgg(estimation,tail):
    '''set the egg'''
    D={}
    D['dataSample']='SingleMu'
    D['dataEstimation']=estimation
    D['dataTail']=tail
    D['dataRWOptions']={'Merge':True,'RW':False}
    dataLumi=12216.611
    D['dataLumi']=dataLumi
    #
    #D['mcSamples']=['TTJetsMG','WJetsToLNu','SingleTop']
    D['mcSamples']=['SingleTop','TTJetsMG','WJetsToLNu']
    D['mcEstimation']=estimation
    D['mcTail']=tail
    D['mcRWOptions']={'Merge':True,'RW':True,'dataLumi':dataLumi}
    #
    D['sigSamples']=[]
    D['sigEstimation']=''
    D['sigTail']=''
    D['sigRWOptions']={'Merge':True,'RW':False,'dataLumi':dataLumi}
    #D['retrievalFile']='/afs/naf.desy.de/user/e/eron/scratch/UserCode/RA4bHead/UserCode/DesySusy/ra4b_2012/treeSamples.py'
    D['retrievalFile']='treeSamples'
    #D['plotDirectory']='metStudies/CR1Muons-baseline_ControlPlots/MADGRAPH'
    D['plotDirectory']='metStudies/'+estimation+'_'+tail+'/MADGRAPH'
    D['plotWeightName']='MTMu'
    D['histoIntervalToWeight']=(50.,120.)    
    return D


def singleEl_FromVariables_MADGRAPH_InputEgg(estimation,tail):
    '''set the egg'''
    D={}
    D['dataSample']='SingleEl'
    D['dataEstimation']=estimation
    D['dataTail']=tail
    D['dataRWOptions']={'Merge':True,'RW':False}
    dataLumi=12216.611

    D['dataLumi']=dataLumi
    #
    #D['mcSamples']=['TTJetsMG','WJetsToLNu','SingleTop']
    D['mcSamples']=['SingleTop','TTJetsMG','WJetsToLNu']
    D['mcEstimation']=estimation
    D['mcTail']=tail
    D['mcRWOptions']={'Merge':True,'RW':True,'dataLumi':dataLumi}
    #
    D['sigSamples']=[]
    D['sigEstimation']=''
    D['sigTail']=''
    D['sigRWOptions']={'Merge':True,'RW':False,'dataLumi':dataLumi}
    D['retrievalFile']='treeSamples'
    D['plotDirectory']='metStudies/'+estimation+'_'+tail+'/MADGRAPH'
    #
    D['plotWeightName']='MTEl'
    D['histoIntervalToWeight']=(50.,120.)
    return D
















def singleMu_FromVariables_POWHEG_InputEgg(estimation,tail):
    
    D=singleMu_FromVariables_MADGRAPH_InputEgg(estimation,tail)
    #overwrite this
    D['mcSamples']=['SingleTop','TTJetsPWHG','WJetsToLNu'] 
    D['plotDirectory']='metStudies/'+estimation+'_'+tail+'/POWHEG'
    
    return D
def singleEl_FromVariables_POWHEG_InputEgg(estimation,tail):

    print 'calling this'

    D=singleEl_FromVariables_MADGRAPH_InputEgg(estimation,tail)
    #overwrite this
    D['mcSamples']=['SingleTop','TTJetsPWHG','WJetsToLNu'] 
    D['plotDirectory']='metStudies/'+estimation+'_'+tail+'/POWHEG'

    return D









def singleMu_CR1Muonsbaseline_POWHEG_InputEgg():   

    D={}
    D['dataSample']='SingleMu'
    D['dataEstimation']='CR1Muons-baseline'
    D['dataTail']='ControlPlots-Signal'
    D['dataRWOptions']={'Merge':True,'RW':False}
    dataLumi=12216.611
    D['dataLumi']=dataLumi
    #
    #
    #this is the order in which they will be plotted
    D['mcSamples']=['SingleTop','TTJetsPWHG','WJetsToLNu']    
    D['mcEstimation']='CR1Muons-baseline'
    D['mcTail']='ControlPlots-Signal'
    D['mcRWOptions']={'Merge':True,'RW':True,'dataLumi':dataLumi}
    #
    D['sigSamples']=''
    D['sigEstimation']=''
    D['sigTail']=''
    D['sigRWOptions']={'Merge':True,'RW':False,'dataLumi':dataLumi}
    #
    D['retrievalFile']='treeSamples'
    D['plotDirectory']='metStudies/CR1Muons-baseline_ControlPlots-Signal/POWHEG'
    #
    #
    return D



def controlPlots_singleMu_CR1Muonsbaseline_POWHEG_InputEgg():   
    
    D={}
    D['dataSample']='SingleMu'
    D['dataEstimation']='CR1Muons-baseline'
    D['dataTail']='ControlPlots'
    D['dataRWOptions']={'Merge':True,'RW':False}
    dataLumi=12216.611
    D['dataLumi']=dataLumi
    #
    #
    #this is the order in which they will be plotted
    D['mcSamples']=['SingleTop','TTJetsPWHG','WJetsToLNu']    
    D['mcEstimation']='CR1Muons-baseline'
    D['mcTail']='ControlPlots'
    D['mcRWOptions']={'Merge':True,'RW':True,'dataLumi':dataLumi}
    #
    D['sigSamples']=''
    D['sigEstimation']=''
    D['sigTail']=''
    D['sigRWOptions']={'Merge':True,'RW':False,'dataLumi':dataLumi}
    #
    D['retrievalFile']='readySamples'
    D['plotDirectory']='controlPlots/CR1Muons-baseline_ControlPlots/POWHEG'
    D['firstDirectory']='ControlPlots'
    blackListDirectory=['Triggers','Signal_Electrons','Wide_Muons','Wide_Muons','IsoTracks','TauVeto','HT']
    #
    D['blackListDirectory']=copy.copy(blackListDirectory)

    return D







def controlPlots_singleMu_CR1Muonsbaseline_MADGRAPH_InputEgg():   

    D={}
    D['dataSample']='SingleMu'
    D['dataEstimation']='CR1Muons-baseline'
    D['dataTail']='ControlPlots'
    D['dataRWOptions']={'Merge':True,'RW':False}
    dataLumi=12216.611
    D['dataLumi']=dataLumi
    #
    #
    #this is the order in which they will be plotted
    D['mcSamples']=['SingleTop','TTJetsMG','WJetsToLNu']    
    D['mcEstimation']='CR1Muons-baseline'
    D['mcTail']='ControlPlots'
    D['mcRWOptions']={'Merge':True,'RW':True,'dataLumi':dataLumi}
    #
    D['sigSamples']=''
    D['sigEstimation']=''
    D['sigTail']=''
    D['sigRWOptions']={'Merge':True,'RW':False,'dataLumi':dataLumi}
    #
    D['retrievalFile']='readySamples'
    D['plotDirectory']='controlPlots/CR1Muons-baseline_ControlPlots/MADGRAPH'
    D['firstDirectory']='ControlPlots'
    blackListDirectory=['Triggers','Signal_Electrons','Wide_Muons','Wide_Muons','IsoTracks','TauVeto','HT']
    #
    D['blackListDirectory']=copy.copy(blackListDirectory)
    

    
    return D








def controlPlots_singleEl_FromVariables_POWHEG_InputEgg(estimation,tail):   
    
    D={}
    D['dataSample']='SingleEl'
    D['dataEstimation']=estimation
    D['dataTail']=tail
    D['dataRWOptions']={'Merge':True,'RW':False}
    dataLumi=12216.611
    D['dataLumi']=dataLumi
    #
    #
    #this is the order in which they will be plotted
    D['mcSamples']=['SingleTop','TTJetsPWHG','WJetsToLNu']    
    D['mcEstimation']=estimation
    D['mcTail']=tail
    D['mcRWOptions']={'Merge':True,'RW':True,'dataLumi':dataLumi}
    #
    D['sigSamples']=''
    D['sigEstimation']=''
    D['sigTail']=''
    D['sigRWOptions']={'Merge':True,'RW':False,'dataLumi':dataLumi}
    #
    D['retrievalFile']='readySamples'
    D['plotDirectory']='controlPlots/'+estimation+'_'+tail+'/MADGRAPH' 
    D['firstDirectory']='CutFlow'
    D['listOfPlots']=['global_flow_Out','selected_Muons_Out','Selected_Electron_Selection_Out','Good_Electron_Selection_Out']
    #
    blackListDirectory=['Triggers','Signal_Electrons','Wide_Muons','Wide_Muons','IsoTracks','TauVeto','HT']
    D['blackListDirectory']=copy.copy(blackListDirectory)
    #
    #
    #D['plotWeightName']='MTEl'
    #D['histoIntervalToWeight']=(50.,100.)


    return D

