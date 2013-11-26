#!/usr/python


import sys

sys.path.append('../../')

from plottools.Plot_RootObjects import *
from plottools.Plot_RebinHistogram import *
from plottools.Plot_PlottingStyles import *
from plottools.Plot_SetRootObjectsOptions import *
from plottools.Plot_BuildDictStructure import *
from pytools.Looper import Looper
import plottools.Plot_DataBkgSigPlot as DBSPlot
from ROOT import TColor



def plotColorProperties():
 
    fileNameList=['4-TTbar','1-BosonJets','3-TopJets','2-DiBoson','TDR4','TDR5','TDR6','TDR8']
    colorList=[1,1,1,1,1,3,4,6]
    widthList=[1,1,1,1,3,3,3,3]
    styleList=[1,1,1,1,2,2,2,2]
    fillList=[TColor.kBlue,TColor.kOrange,TColor.kCyan,TColor.kRed,0,0,0,0]
    
    lineColor=dict(zip(fileNameList,colorList))
    lineWidth=dict(zip(fileNameList,widthList))
    lineStyle=dict(zip(fileNameList,styleList))
    histoFill=dict(zip(fileNameList,fillList))

    colorProperties={'lineColor':lineColor,'lineWidth':lineWidth,
                     'lineStyle':lineStyle,'histoFill':histoFill}

    return colorProperties

    
def getColorProperties(colorProperties,tagname):
    lineColor=colorProperties['lineColor'][tagname]
    lineWidth=colorProperties['lineWidth'][tagname]
    lineStyle=colorProperties['lineStyle'][tagname]
    histoFill=colorProperties['histoFill'][tagname]            
    return lineColor,lineWidth,lineStyle,histoFill




if __name__=='__main__':




    SCALE_FACTOR=5/10000.0
    #SCALE_FACTOR=1.0
    
    Ana='SingleS'
    base='karim/'+Ana+'/'+Ana+'_'
    puList=['50PU','140PU']
    


    for pu in puList:
     
        fileList=[]
        signalList=[]
        fileList.append(base+pu+'_TTbar_his.root')
        fileList.append(base+pu+'_BosonJets_his.root')
        fileList.append(base+pu+'_TopJets_his.root')
        fileList.append(base+pu+'_DiBoson_his.root')
        signalList.append(base+pu+'_TDR4_his.root')
        signalList.append(base+pu+'_TDR5_his.root')
        signalList.append(base+pu+'_TDR6_his.root')
        signalList.append(base+pu+'_TDR8_his.root')
        #
        #
        #
        #DEFINE THE NAMES FOR THE FILES
        fileNameList=['4-TTbar','1-BosonJets','3-TopJets','2-DiBoson']
        signalNameList=['TDR4','TDR5','TDR6','TDR8']
        #
        #
        #CONSTRUCT A DICIONARY FROM TWO LISTS
        fileDictionary=dict(zip(fileNameList,fileList))
        signalDictionary=dict(zip(signalNameList,signalList))
        #
        #
        #DEFINE THE HISTROGRAM LIST
        histogramList=['hLepPtM','hAllHT','hMET','hMT2W','hMT2Wpre','hMT']
        #
        #
        #============================================
        #CONSTRUCT THE LOOPER AND INITIALIZE THEM
        loop=Looper()
        loop.InitializeFiles(fileDictionary)
        #START TAKES TWO ARGUMENTS
        #1) startDir is the path of the top directory in which to start
        #2) plotList is a white list of the histograms over which to loop
        loop.Start(startDir='',plotList=histogramList)
        #loop.Start()

        #
        sigloop=Looper()
        sigloop.InitializeFiles(signalDictionary)

        #=============================================
        #
        #
        #DEFINE THE COLOR PROPERTIES
        colorProperties=plotColorProperties()
        #DEFINE THE PLOT STRUCTURE
        stackPlotStructure=BuildDictStructure(fileDictionary)
        signalPlotStructure=BuildDictStructure(signalDictionary)





        #
        #
        #
        #NOW LOOP!
        histoPack=True
        signalPack=True
        while histoPack:
            #
            histoPack,pathName  = loop.Next()
            if histoPack=='End':
                break

            #print 'histoPack ',histoPack
            #raw_input('')


            currentPlotName=loop.getCurrentPlotName()
            #
            signalPack=sigloop.FindHistograms(pathName,currentPlotName)
            #
            #
            #
            #
            #
            #SET THE PROPERTIES OF THE MC PLOT            
            for histoKey,histoObj in histoPack.iteritems():
                #
                if str(histoPack[histoKey]).find('nil')!=-1:
                    print 'the histogram for the key ',histoKey, ' is missing'
                    print 'an exception is about to happen!'
                #
                lineColor,lineWidth,lineStyle,fillColor=getColorProperties(colorProperties,histoKey)
                #
                #INSERT THEM INTO THE PLOT STRUCTURE
                stackPlotStructure[histoKey][0]=histoObj.Clone()
                stackPlotStructure[histoKey][1]['lineColor']=lineColor
                stackPlotStructure[histoKey][1]['lineWidth']=lineWidth
                stackPlotStructure[histoKey][1]['lineStyle']=lineStyle
                stackPlotStructure[histoKey][1]['fillColor']=fillColor
                stackPlotStructure[histoKey][1]['NoErrors']=True
                stackPlotStructure[histoKey][1]['rebin']=True
                stackPlotStructure[histoKey][1]['Scale']=SCALE_FACTOR

                #print 'the very cmplicated stucture is.....',stackPlotStructure
                #SET THE PROPERTIES OF THE SIGNALS
            for histoKey,histoObj in signalPack.iteritems():
            #
                if str(signalPack[histoKey]).find('nil')!=-1:
                    print 'the histogram for the key ',histoKey, ' is missing'
                    print 'an exception is about to happen!'
                #
                lineColor,lineWidth,lineStyle,fillColor=getColorProperties(colorProperties,histoKey)
                #
                #INSERT THEM INTO THE PLOT STRUCTURE
                signalPlotStructure[histoKey][0]=histoObj.Clone()
                signalPlotStructure[histoKey][1]['lineColor']=lineColor
                signalPlotStructure[histoKey][1]['lineWidth']=lineWidth
                signalPlotStructure[histoKey][1]['lineStyle']=lineStyle
                signalPlotStructure[histoKey][1]['fillColor']=fillColor
                signalPlotStructure[histoKey][1]['NoErrors']=True                    
                signalPlotStructure[histoKey][1]['rebin']=True
                signalPlotStructure[histoKey][1]['Scale']=SCALE_FACTOR
                #GENERAL PROPERTIES OF THE PLOT
                #getting the predefined style
            #
            #
            #
            #
            #
            PlotProperties=Style_ControlPlots()        

            #set the name of the saved plot
            outPlotName=histoObj.GetName()
            PlotProperties['outPlotName']=outPlotName+'_'+pu
            #
            #raw_input('at the end!')
            #
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
            #print 'feeding ',stackPlotStructure
            InputList=['SimpleStack',PlotProperties,{},stackPlotStructure,signalPlotStructure]
            newPlot=DBSPlot.DataBkgSigPlot(InputList)
            newPlot.suffixList=['.pdf']
            #=====DRAW IT
            #
            #
            newPlot.Draw()
            #
            #
            #
            #
            #
            #
            #









