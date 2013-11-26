


#PERFORMS CONTROL PLOTS

import os,sys
import commands as com


#MOVE THE PATH TWO DIRECTORIES UP

sys.path.append('../../')
from ROOT import ROOT, TFile, TCanvas, THStack,TLegend,gPad,gROOT,SetOwnership
import plottools.Plot_RootObjects as rootobj
import plottools.Plot_SetRootObjectsOptions as setopt
import plottools.Plot_PlottingStyles as styles
import plottools.Plot_RebinHistogram as rebin
import plottools.Plot_DataBkgSigPlot as DBSPlot
import pytools.Looper as looper
import pytools.CheckDirectory as checkdir
from plottools.Plot_BuildDictStructure import *
from pytools.retrieveSamples import *
import readySamples as samples
import copy


    


def makePlots(dataSample,mcSampleDict,signalSampleDict,plotsDirectory):
    '''make the control plots with this data sample, the dictionary of
    montecarlo samples and the signal samples
    What samples are used specifically is given on the arguments'''
   


    #===================================================
    #
    #
    #
    #OPEN THE ROOT FILES===============================
    print 'using the data file ',dataSample.RootFile
    #
    dataTFile=TFile(dataSample.RootFile,"READ")
    #
    #
    #Attach the TFile next to the Sample in the dictionary
    #
    for key, sample in mcSampleDict.iteritems():
        sample.append(TFile(sample[0].RootFile,'READ'))
    #
    #
    #GET THE PLOT STRUCTURE
    dataPlotStructure=BuildDictStructure(dataSample)
    mcPlotStructure=BuildDictStructure(mcSampleDict)
    signalPlotStructure=BuildDictStructure(signalSampleDict)
    #
    #=======================================================



    #==========START LOOPING OVER THE HISTOGRAMS WITH THE LOOPER TOOL
    #
    #
    #plotsDirectory='metStudies/dataMcPlots/default'
    #
    #
    #
    dataLoop=looper.Looper()
    dataLoop.InitializeFiles({'data':dataSample.RootFile})
    dataLoop.blackListDirectory.append('Before_CutFlow')
    dataLoop.blackListDirectory.append('Triggers')
    dataLoop.StartLoopFromString('ControlPlots')
    
    #build the looper over the monte carlo samples
    mcLoop=looper.Looper()
    #
    #
    #
    #========INITIALIZE THE FILES OVER WHICH TO LOOP
    mcInitializeDict={}
    for key,list in mcSampleDict.iteritems():
        print 'with key ', key
        print 'and files ',list[0].RootFile
        mcInitializeDict[key]=list[0].RootFile
    #
    mcLoop.InitializeFiles(mcInitializeDict)
    #
    #
    #
    #
    #========START THE LOOP
    dataHistoDict=''
    continueLoop=True
    while continueLoop:
        #
        #
        #=========GET THE NEXT HISTOGRAM IN THE DATA FILE
        dataHistoDict=dataLoop.NextHisto({})
        #
        #
        if dataHistoDict != 'End':
            dataHisto=dataHistoDict['data']
            
            #=====LOOK FOR THE CORRESPONDING HISTOGRAM IN THE MC
            #
            #
            searchPattern= {'name_exact':dataHisto.GetName()}
            #
            #
            #====GET A DICTIONARY WITH HISTOS THAT ARE CALLED LIKE
            #====THE DATA HISTO+A SUFFIX
            currentDataPath=dataLoop.currentDir.GetPath()
            whereSemiColon=currentDataPath.find(':')+1
            currentDataPath=currentDataPath[whereSemiColon:]
            #raw_input('')
            mcHistosDict=mcLoop.FindHistograms(currentDataPath,dataHisto,searchPattern)
            if mcHistosDict == 'NotFound':
                raise NameError('the histograms correspoding to '+dataHisto.GetName()+' were not found ')

            whereLastSlash=currentDataPath.rfind('/')
            lastDir=currentDataPath[whereLastSlash+1:]
            #
            #
            #
            #============NOW LET'S START PLOTTING=================
            #
            #
            #
            #SET THE PROPERTIES OF THE DATA PLOT
            dataKey=dataSample.SampleName                    
            dataList = dataPlotStructure[dataKey]
            dataClone=dataHisto.Clone()
            SetOwnership(dataClone,False)
            dataList[0]=dataClone
            dataList[1]=styles.Style_DataHistogram()
            #set the properties of the mc histos
            #dataHisto.Draw()
            #gPad.Update()
            #raw_input('')
            #
            #
            #SET THE PROPERTIES OF THE MC PLOT            
            for mcKey,mcHisto in mcHistosDict.iteritems():
                #
                if str(mcHistosDict[mcKey]).find('nil')!=-1:
                    print 'the histogram for the key ',mcKey, ' is missing'
                    print 'an exception is about to happen'
                #
                mcPlotStructure[mcKey][0]=mcHistosDict[mcKey].Clone()
                mcPlotStructure[mcKey][1]['lineColor']=setopt.GetColor(mcKey)
                mcPlotStructure[mcKey][1]['fillColor']=setopt.GetColor(mcKey)
                mcPlotStructure[mcKey][1]['NoErrors']=True
                #mcPlotStructure[mcKey][0].Draw()
                #gPad.Update()
                #raw_input('')
            #
            #
            #
            #GENERAL PROPERTIES OF THE PLOT
            #getting the predefined style
            PlotProperties=styles.Style_ControlPlots(dataSample.SampleLumi)
            PlotProperties['xtitle_ratioplot']=rootobj.GetUnits(dataClone.GetName())
            #THE NAME OF THE .pdf FILE SHOULD BE THE SAME ONE AS THE PLOT NAME
            outPlotName=dataClone.GetName()
            PlotProperties['outPlotName']=outPlotName
            #======================================
            #
            #
            #
            #
            #
            #      CREATE THE PLOT
            #
            #
            #
            #
            #
            #
            #========================================
            #Lets create the input list for the class that we are using
            InputList=['StackWithRatio',PlotProperties,dataPlotStructure,mcPlotStructure,signalPlotStructure]
            newPlot=DBSPlot.DataBkgSigPlot(InputList)
            newPlot.suffixList=['.gif','.pdf']
            #=====DRAW IT
            #
            #
            newPlot.Draw()
            #
            #
            #
            #     PUT THE PLOTS IN THE CORRESPONDING DIR:
            checkdir.CheckDirectory(plotsDirectory+'/'+lastDir)
            #
            for suf in newPlot.suffixList:
                os.system('mv '+outPlotName+suf+' '+plotsDirectory+'/'+lastDir)
        else:
            print 'LOOP FINISHED '
            break
        #




def Plot_SingleMu_CR1Muonsbaseline_ControlPlots():


    '''retrieves the samples that correspond to the CR1Muons baseline calculation

    The samples are retrieved through calls to the function pytools.retrieveSamples

    '''

    mcSampleDict={}
    signalSampleDict={}
    #
    #
    #===MERGE AND REWEIGHT OPTIONS
    dataMergeRWOptions={}
    mcMergeRWOptions={}    
    dataMergeRWOptions['Merge']=True
    dataMergeRWOptions['ForceMerge']=True
    dataMergeRWOptions['RW']=False
    mcMergeRWOptions['Merge']=True
    mcMergeRWOptions['RW']=True
    mcMergeRWOptions['ForceRW']=True


    
    singlemu=retrieveSamples(['SingleMu','CR1Muonsbaseline','ControlPlots'],dataMergeRWOptions)
    singlemu.SampleLumi=12216.611
    #singlemu.SampleLumi=6897
    print 'the data file is ',singlemu.RootFile
    
    mcMergeRWOptions['dataLumi']=[singlemu.SampleLumi]
    #
    #
    #
    #===MC SAMPLES
    ttjets=retrieveSamples(['TTJetsMG','CR1Muonsbaseline','ControlPlots'],mcMergeRWOptions)
    singletop=retrieveSamples(['SingleTop','CR1Muonsbaseline','ControlPlots'],mcMergeRWOptions)
    wjets =retrieveSamples(['WJetsToLNuJets','CR1Muonsbaseline','ControlPlots'],mcMergeRWOptions)
    #ttjetspowheg=retrieveSamples(['TTJetsPWHG','CR1Muonsbaseline','ControlPlots'],mcMergeRWOptions)
    #wjets =retrieveSamples(['WJetsToLNuInclusive','CR1Muons','ControlPlots'],mcMergeRWOptions)    
    
    mcSampleDict['2-ttjets']=[ttjets] #this has to be a list
    #mcSampleDict['2-ttjets-PWHG']=[ttjetspowheg] #this has to be a list    
    mcSampleDict['1-singletop']=[singletop]
    mcSampleDict['3-wjets']=[wjets]
    #
    plotsDirectory='controlPlots/MADGRAPH/CR1Muonsbaseline'
    #plotsDirectory='controlPlots/POWHEG/CR1Muons-baseline'    
    #
    #
    makePlots(singlemu,mcSampleDict,signalSampleDict,plotsDirectory)




def Plot_SingleEl_CR1Electrons_ControlPlots():


    '''retrieves the samples that correspond to the CR1Electrons baseline calculation

    The samples are retrieved through calls to the function pytools.retrieveSamples

    '''
    
    mcSampleDict={}
    signalSampleDict={}
    #
    #
    #===MERGE AND REWEIGHT OPTIONS
    dataMergeRWOptions={}
    mcMergeRWOptions={}    
    dataMergeRWOptions['Merge']=True
    dataMergeRWOptions['ForceMerge']=True
    dataMergeRWOptions['RW']=False
    mcMergeRWOptions['Merge']=True
    mcMergeRWOptions['RW']=True
    mcMergeRWOptions['ForceRW']=True

    
    singleel=retrieveSamples(['SingleEl','CR1Electrons','ControlPlots'],dataMergeRWOptions)
    singleel.SampleLumi=12216.611
    #singleel.SampleLumi=6897
    print 'the data file is ',singleel.RootFile
    
    mcMergeRWOptions['dataLumi']=[singleel.SampleLumi]
    #
    #
    #
    #===MC SAMPLES
    ttjets=retrieveSamples(['TTJetsMG','CR1Electrons','ControlPlots'],mcMergeRWOptions)
    singletop=retrieveSamples(['SingleTop','CR1Electrons','ControlPlots'],mcMergeRWOptions)
    wjets =retrieveSamples(['WJetsToLNuJets','CR1Electrons','ControlPlots'],mcMergeRWOptions)
    #ttjetspowheg=retrieveSamples(['TTJetsPWHG','CR1Electrons','ControlPlots'],mcMergeRWOptions)
    #wjets =retrieveSamples(['WJetsToLNuInclusive','CR1Electrons','ControlPlots'],mcMergeRWOptions)    
    
    mcSampleDict['2-ttjets']=[ttjets] #this has to be a list
    #mcSampleDict['2-ttjets-PWHG']=[ttjetspowheg] #this has to be a list    
    mcSampleDict['1-singletop']=[singletop]
    mcSampleDict['3-wjets']=[wjets]
    #
    plotsDirectory='controlPlots/MADGRAPH/CR1Electrons'
    #plotsDirectory='controlPlots/POWHEG/CR1Electrons'    
    #
    #
    makePlots(singleel,mcSampleDict,signalSampleDict,plotsDirectory)



#def PlotASample(dataName,mcNames, Estimation,Tail, ,dataMergeRWOptions,mcMergeRWOptions)



def controlPlots():
    
    Plot_SingleMu_CR1Muonsbaseline_ControlPlots()
    #Plot_SingleEl_CR1Electrons_ControlPlots()





if __name__=='__main__':

    controlPlots()


