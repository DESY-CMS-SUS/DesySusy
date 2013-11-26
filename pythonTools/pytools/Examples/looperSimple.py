

import sys
sys.path.append('../')

from Looper import Looper






#INFO
#this illustrates the possible uses of the Looper class
#The class is used to loop recursively over histograms in a root
#file, as well as other possibilities explained further down.

dataFile='testData.root'
ttjetsFile='testTTbar.root'
qcdFile='testQCD.root'



#DATA STUFF
#================
dataLoop=Looper() #instance of the class
dataDictionary={}
dataDictionary['data']=dataFile
dataLoop.InitializeFiles(dataDictionary) #this has to be a dictionary!



#MC STUFF
#================
mcLoop=Looper()
mcDictionary={'ttjets':ttjetsFile,'qcd':qcdFile}
mcLoop.InitializeFiles(mcDictionary)



#POSSIBLE USAGE
#====================
#1. start over the top dir and iterate recursively
dataLoop.Start()


#2. Start over a certain directory
##########dataLoop.Start(startDir='FirstDirectory')


#3. Use only historgams in list and ignore the rest
##########myHistoList = ['TD_h1','TD_h3']
##########dataLoop.Start(plotList = myHistoList)

#4. Use only directories in a white list
#########directoryList = 'FirstDirectory/F_SubDirectory','ThirdDirectory']
#########dataLoop.whiteListDirectory = directoryList
#########dataLoop.Start()


#5. Ignore directories in a black list
#########directoryList = 'FirstDirectory/F_SubDirectory','ThirdDirectory']
#########dataLoop.blackListDirectory = directoryList
#########dataLoop.Start()



while True:
    
    histoPack,pathName = dataLoop.Next()

    #histoPack is a dictionary with:
    #1 a string as the key
    #2 the histogram as the value
    #pathName is the path inside the file where the
    #current plot is

    
    if histoPack=='End': break
    
    print 'the histoPack is ',histoPack
    print 'the histoName is ',histoPack['data'].GetName()
    print 'currently on the path... ',pathName
    print
    print

    mcPack = mcLoop.FindHistograms(pathName,histoPack['data'])


    print 'the mcPack is ',mcPack
    
    raw_input('pausing...')
