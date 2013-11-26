import os,sys
import commands as com
from ROOT import ROOT, TFile, TCanvas, THStack,TLegend,gPad,gROOT
import plottools.Plot_RootObjects as rootobj
import plottools.Plot_SetRootObjectsOptions as setopt
import plottools.Plot_PlottingStyles as styles
import plottools.Plot_RebinHistogram as rebin
import plottools.Plot_DataBkgSigPlot as DBSPlot
import pytools.Looper as looper
import pytools.CheckDirectory as checkdir
from plottools.Plot_BuildDictStructure import BuildDictStructure
#import RA4b_TreeSamples as treesamples
from pytools.retrieveDataAndMC import RetrieveDataMCSignal
from pytools import hookDebugger
from inputEggs import *
#import plottools.extractScaleFactorFromPeak as peakscale
import pytools.weightFromHistos as rw
import copy




def makePlots(InputEgg):

    #raw_input('attach!')
    #============DATA
    DataMcSampleList=[]
    dataSample,mcSampleDict,sigSampleDict=RetrieveDataMCSignal(InputEgg)
    #xraw_input('what')
    #
    #
    #
    #OPEN THE ROOT FILES===============================
    print 'using the data file ',dataSample.RootFile
    #
    #
    #Attach the TFile next to the Sample in the dictionary
    #
    print 'the mcsample dict is here'
    print mcSampleDict
    #raw_input('mcsampledict')
    for key, sample in mcSampleDict.iteritems():
        sample.append(TFile(sample[0].RootFile,'READ'))
        #
    for key, sample in sigSampleDict.iteritems():
        sample.append(TFile(sample[0].RootFile,'READ'))
        #        

        
    #GET THE PLOT STRUCTURE
    dataPlotStructure=BuildDictStructure(dataSample)
    mcPlotStructure=BuildDictStructure(mcSampleDict)
    signalPlotStructure=BuildDictStructure(sigSampleDict)
    #
    #=======================================================




    #==========START LOOPING OVER THE HISTOGRAMS WITH THE LOOPER TOOL
    #
    #
    plotsDirectory=InputEgg['plotDirectory']
    #
    #
    #
    #
    dataLoop=looper.Looper()
    dataLoop.InitializeFiles({'data':dataSample.RootFile})
    listOfPlots=InputEgg.get('listOfPlots',[])
    firstDirectory=InputEgg.get('firstDirectory','')
    #
    dataLoop.blackListDirectory=InputEgg.get('blackListDirectory',[])
    dataLoop.whiteListDirectory=InputEgg.get('whiteListDirectory',[])    
    #
    #
    #
    dataLoop.StartFrom(firstDirectory,listOfPlots)
    #if listOfPlots != None:
    #    dataLoop.LoopOverPlotList(firstDirectory,plotList)
    #else:
    #    dataLoop.StartLoopFromString(firstDirectory)
    #
    #
    #
    

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
    #print 'mcPlotStructure ',mcPlotStructure
    #raw_input('mcPlotStructure ')
    
    mcLoop.InitializeFiles(mcInitializeDict)
    #
    #
    #
    #INITALIZE THE NAME OF THE VARIABLE TO OBTAIN FURTHER WEIGHTS
    plotWeightName=InputEgg.get('plotWeightName','noPlot')
    plotIntervalToWeight=InputEgg.get('histoIntervalToWeight','noInterval')
    #
    #
    #
    #
    #
    #
    #
    #
    #
    #========START THE LOOP
    dataHistoDict=''
    continueLoop=True
    ansAll=False
    oldPath='oldPath'
    while continueLoop:
        #
        #
        #=========GET THE NEXT HISTOGRAM IN THE DATA FILE
        dataHistoDict, dataRelPath=dataLoop.Next({})
        #print 'dataHistoDict is now ',dataHistoDict
        #
        #

        if dataHistoDict =='End':
            print 'LOOP FINISHED!'

            print 'the plots were saved in the directory '
            print '   ',InputEgg['plotDirectory']

            break
        #
        #
        #
        #IGNORE TH2?
        doTH2=InputEgg.get('doTH2',False)
        if not doTH2:
            if dataHistoDict['data'].IsA().InheritsFrom('TH2'):
                continue
        #
        #
        #
        #
        #
        dataHisto=dataHistoDict['data']
        print 'the entries of ',dataHisto.GetName(), ' is ',dataHisto.GetEntries()
        if  dataHisto.GetEntries()!=0:
            #
            #
            #
            #
            currentDataPath=dataLoop.currentDir.GetPath()
            whereSemiColon=currentDataPath.find(':')
            currentDataPath=currentDataPath[whereSemiColon+2:]
            whereLastSlash=currentDataPath.rfind('/')
            dirToSaveThePlot=plotsDirectory
            if whereLastSlash != -1:
                lastDir=currentDataPath[whereLastSlash+1:]
                dirToSaveThePlot=plotsDirectory+'/'+lastDir


            #
            #
            #
            #
            suffixList=InputEgg.get('suffixList',[''])
            #======PLOTTING FOR THE THREE SMEARING METHODS
            #for suffix in ('central','up','down',''):
            for suffix in suffixList:
                
                print 'currently on ',suffix
                
                if suffix!='':
                    dirInMC='jetRes/'+suffix
                else:

                    dirInMC=currentDataPath

                #
                #
                #
                #
                carryOn=False
                for key,list in mcInitializeDict.iteritems():
                    if dirInMC!='':
                        testDirF=TFile(list,'READ')
                        testDir=testDirF.Get(dirInMC)
                        if str(testDir).find('nil') != -1:
                            print 'the directory ',dirInMC, 'does not exist...'
                            print 'continue  (y)?'
                            if not ansAll:
                                ans=raw_input('   yes to continue for this histo, \'all\' to continue for all histos  ')
                                if ans =='all':
                                    ansAll=True
                                    carryOn=True
                                elif ans !='yes':
                                    sys.exit()
                            else:
                                carryOn=True
                                
                        testDirF.Close()
                    break
                if carryOn: continue
                #
                #
                #
                #
                #
                #
                searchPattern= {'name_exact':dataHisto.GetName()}
                #
                #
                #====GET A DICTIONARY WITH HISTOS THAT ARE CALLED LIKE
                #====THE DATA HISTO + PATTERN
                mcHistosDict=mcLoop.FindHistograms(dirInMC,dataHisto,searchPattern)
                if mcHistosDict == 'NotFound':
                    raise NameError('the histograms '+dataHisto.GetName()+' were not found in'+mcHistosDict)

                #
                #
                #
                #
                #
                #build the dictionaries to retrieve extra weights
                if plotWeightName!='noPlot' and currentDataPath!=oldPath:
                    
                    dataWeightDict=dataLoop.FindHistograms(currentDataPath,plotWeightName)
                    mcWeightDict=mcLoop.FindHistograms(currentDataPath,plotWeightName)
                    print 'dataWeightDict ',dataWeightDict
                    print 'mcWeightDict ',mcWeightDict
                        
                    #
                    extraWeight=rw.GetWeightFromDicts(dataWeightDict,mcWeightDict,plotIntervalToWeight)
                    print 'the extraWeight is ',extraWeight
                    #raw_input()

                else:
                    extraWeight=1.0
                                    
                #
                #
                #
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
                dataList[0]=dataClone
                dataList[1]=styles.Style_DataHistogram()
                #set the properties of the mc histos
                for mcKey,mcHisto in mcHistosDict.iteritems():
                    #
                    #
                    newPlot=mcHistosDict[mcKey].Clone()
                    newPlot.Scale(extraWeight)
                    #
                    #
                    mcPlotStructure[mcKey][0]=newPlot
                    mcPlotStructure[mcKey][1]['lineColor']=setopt.GetColor(mcKey)
                    mcPlotStructure[mcKey][1]['fillColor']=setopt.GetColor(mcKey)
                    mcPlotStructure[mcKey][1]['NoErrors']=True
                #
                #
                #
                #GENERAL PROPERTIES OF THE PLOT
                
                PlotProperties=styles.Style_ControlPlots(InputEgg.get('dataLumi',''))
                PlotProperties['xtitle_ratioplot']=rootobj.GetUnits(dataClone.GetName())
                #THE NAME OF THE .pdf FILE SHOULD BE THE SAME ONE AS THE PLOT NAME
                if suffix != '':
                    outPlotName=dataClone.GetName()+'_'+suffix
                else:
                    outPlotName=dataClone.GetName()
                #
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
                print 'the plot name is ',outPlotName
                newPlot.Draw()                
                #=====PUT THE PLOTS IN THE CORRESPONDING DIR:
                
                checkdir.CheckDirectory(dirToSaveThePlot)
                #
                for suf in newPlot.suffixList:
                    os.system('mv '+outPlotName+suf+' '+dirToSaveThePlot)

                oldPath=currentDataPath                    
        else:
            pass
            #print 'LOOP FINISHED '
            #break




if __name__=='__main__':

    #
    #
    doMADGRAPH=False
    if doMADGRAPH:
        InputEgg=singleMu_FromVariables_MADGRAPH_InputEgg('CR1Muons-baseline','ControlPlots')
        makePlots(InputEgg)
        
        InputEgg=singleMu_FromVariables_MADGRAPH_InputEgg('CR1Muons-baseline','ControlPlots-BtagMET50')
        makePlots(InputEgg)
        
        InputEgg=singleMu_FromVariables_MADGRAPH_InputEgg('CR1Muons-baseline','ControlPlots-BtagMET100')
        makePlots(InputEgg)

        InputEgg=singleEl_FromVariables_MADGRAPH_InputEgg('CR1Electrons-baseline','ControlPlots')
        makePlots(InputEgg)
        
        InputEgg=singleEl_FromVariables_MADGRAPH_InputEgg('CR1Electrons-baseline','ControlPlots-BtagMET50')
        makePlots(InputEgg)
        
        InputEgg=singleEl_FromVariables_MADGRAPH_InputEgg('CR1Electrons-baseline','ControlPlots-BtagMET100')
        makePlots(InputEgg)        
    #
    #
    #
    #
    #
    #
    doPOWHEG=True
    if doPOWHEG:
        #InputEgg=singleMu_FromVariables_POWHEG_InputEgg('CR1Muons-baseline','ControlPlots')
        #makePlots(InputEgg)
        
        #InputEgg=singleMu_FromVariables_POWHEG_InputEgg('CR1Muons-baseline','ControlPlots-BtagMET50')
        #makePlots(InputEgg)
        
        #InputEgg=singleMu_FromVariables_POWHEG_InputEgg('CR1Muons-baseline','ControlPlots-BtagMET100')
        #makePlots(InputEgg)

        #InputEgg=singleEl_FromVariables_POWHEG_InputEgg('CR1Electrons-baseline','ControlPlots')
        #makePlots(InputEgg)
        
        #InputEgg=singleEl_FromVariables_POWHEG_InputEgg('CR1Electrons-baseline','ControlPlots-BtagMET50')
        #makePlots(InputEgg)
        
        #InputEgg=singleEl_FromVariables_POWHEG_InputEgg('CR1Electrons-baseline','ControlPlots-BtagMET100')
        #makePlots(InputEgg)

        InputEgg=controlPlots_singleEl_FromVariables_POWHEG_InputEgg('CR1Electrons-baseline','ControlPlots')
        makePlots(InputEgg)                
    #
    #
    



    
    #InputEgg=controlPlots_singleMu_CR1Muonsbaseline_POWHEG_InputEgg()    
    #makePlots(InputEgg)
    #
    #InputEgg=singleMu_CR1Muonsbaseline_POWHEG_InputEgg()    
    #makePlots(InputEgg)
    #
    #InputEgg=singleMu_CR1Muonsbaseline_MADGRAPH_InputEgg()    
    #makePlots(InputEgg)

    
    
