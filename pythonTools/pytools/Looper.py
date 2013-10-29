import sys,os
import commands as com


import ROOT
import copy


#
class Looper:
    """
    Loops recursively over a directory for a set of files.
    Once the files are initialized, calls to NextHisto return 
    a dictionary with the same histogram in all the files given.
    """
    
    
    def __init__(self):
        
        #dictionary of TFiles over which to loop
        self.LastTFile=0
        self.TFiles={}
        self.nextkey={}
        self.histoPack={}
        self.motherDir={}
        self.currentDir=0
        self.blackListDirectory=[]
        self.whiteListDirectory=[]
        self.blackListHisto=[]
        self.whiteListHisto=[]                
        self.currentPlotInWhiteList=0
        self.raiseIfNotFound=True
        
    def GetRelativePath(self,HistoPath):
        #remove everything from the ":" onwards
        position=HistoPath.find(':')
        HistoPath=HistoPath[position+1:]
        #add the histogram name
        return HistoPath
    #
    #
    def InitializeFiles(self,dictionaryFiles):
        """it takes a dictionary with file names as items, opens them, and
        builds a new dictionary with the opened corresponding TFiles"""
        for key,fileName in dictionaryFiles.iteritems():
            #
            out=com.getoutput('ls '+fileName)
            if out.find('No such file') != -1:
                raise NameError('in InitializeFiles, the file '+fileName+' does not exist')
            #
            self.TFiles[key]=ROOT.TFile(fileName,'READ')
            #last tfile

            self.LastTFile=self.TFiles[key]
            #print 'set lasttfile to ',self.LastTFile            
        return

    def ApplyPattern(self,histo,pattern):
        """checks whether the histogram agrees with the pattern given"""
        if len(pattern)==0:
            return True

        if histo==None or histo==0:
            raise NameError('histo is not present')
        
        OK=False

        #print 'the pattern to check is ',pattern
        if 'name_exact' in pattern:
            #print 'testing the histo ',histo.GetName(),
            #print 'what is going on ',histo.GetName()==pattern['name_exact']  
            OK=OK or histo.GetName()==pattern['name_exact']
            #print 'current OK value is ',OK
        if 'name_not_exact' in pattern:
            OK=OK or not histo.GetName()==pattern['name_not_exact']
        if 'name_contains' in pattern:
            OK=OK or histo.GetName().find(pattern['name_contains']) != -1
        if 'name_not_contains' in pattern:
            OK=OK or histo.GetName().find(pattern['name_not_contains']) == -1

        return OK
    #
    #
    #
    def GetHistoFromFiles(self,HistoPath):
        """gets the corresponding histo in all the files and returns
        a dictionary with them"""

        #
        self.histoPack.clear()        
        for key, files in self.TFiles.iteritems():
            #
            self.histoPack[key]=files.Get(HistoPath)
        #
    #







    def FindHistograms(self,dirName,histo,pattern={}):
        '''looks for the histogram histo inside of dirName
        it returns self.histoPack if the histograms were found
        otherwise it returns the string NotFound'''


        if str(histo).find('ROOT.TH')!= -1:
            histoName=histo.GetName()
        else:
            histoName=histo
        #
        #
        #
        if len(pattern)==0:
            pattern= {'name_exact':histoName}
            
        #print 'dirName = ',dirName
        #print 'lasttfile is ',self.LastTFile
        if dirName=='':
            dir=self.LastTFile
        else:
            dir=self.LastTFile.Get(dirName)
        #
        if str(dir).find('nil')!=-1:
            if self.raiseIfNotFound:
                raise NameError('the directory '+dirName+' was not found')
            else:
                return 'NotFound'
        #
        #print 'and the dirName is ',dirName        
        #print 'the dir is ',dir.GetPath()



        nextkey =ROOT.TIter(dir.GetListOfKeys())
        key=nextkey()        
        while str(key).find('nil') == -1:
            
            obj=dir.Get(key.GetName())
            if obj.IsA().InheritsFrom("TH1"):
                histofound=self.ApplyPattern(obj,pattern)
                if(histofound):
                    #get the rest from the other files
                    #
                    relHistoPath=self.GetRelativePath(dir.GetPath())
                    histoPath=relHistoPath+'/'+obj.GetName()
                    #print 'the histopath is ',histoPath
                    histoPack=self.GetHistoFromFiles(histoPath)
                    #
                    #print 'histograms found, returning ',self.histoPack

                    return self.histoPack
            key=nextkey()

        print 'in FindHistograms: '
        print '    the histogram ',histoName,' was not found in ',dir.GetPath()
        return 'NotFound'



    #def returnToMotherDir
    #
    #
    #
    def NextHisto(self,pattern={}):
        """where the magic takes place
        It moves to the next key in the iterator
        if pattern is given, it gets only
        the object that match the pattern"""

        
        dir=self.currentDir
        key=self.nextkey[dir.GetPath()]()
        #
        #
        if str(key).find('nil') != -1:
            #the last key has been found
            #jump to the mother directory and
            #resume the loop
            print 'Directory ',dir.GetPath(), 'was finished '
            #print dir.GetPath() , ' vs ' ,  self.motherDir
            if not dir.GetPath() in self.motherDir:
                return 'End','nopath'

            print ''
            print 'Moving to its mother directory: '
            print '    '+self.motherDir[dir.GetPath()].GetPath()
            print ''
            
            self.currentDir=self.motherDir[dir.GetPath()]
            #self.StartLoop(self.currentDir)
            return self.NextHisto(pattern)
        #
        #
        obj=dir.Get(key.GetName())
        if obj.IsA().InheritsFrom("TH1"):        
            #get the same object from the other files

            #print 'currently on ',obj.GetName()
            OK=self.ApplyPattern(obj,pattern)
            if not OK:
                #go to the next one
                return self.NextHisto(pattern)
            #
            #
            HistoPath=dir.GetPath()
            relHistoPath=self.GetRelativePath(HistoPath)
            #
            HistoPath=relHistoPath+'/'+obj.GetName()
            self.GetHistoFromFiles(HistoPath)
            #all the histos are now contained in
            #self.histoPack

            #print 'returning ',self.histoPack
            #raw_input('yo')
            return self.histoPack,relHistoPath[1:]
        
        elif obj.IsA().InheritsFrom("TDirectory"):
            #
            #print 'seeting mother dir ',obj.GetPath(), 'to ', self.currentDir


            ##IGNORE DIRECTORIES IN THE BLACK LIST
            if len(self.blackListDirectory)!= 0:
                if obj.GetName() in self.blackListDirectory:
                   #return to the mother dir
                   print 'ignoring the directory ',obj.GetPath()
                   print 'at this point dir is ',dir.GetPath()
                   self.currentDir=dir
                   return self.NextHisto(pattern)
               #
            #   
            ##IGNORE DIRECTORIES NOT IN THE WHITE LIST:
            if len(self.whiteListDirectory) != 0:   
                if  not obj.GetName() in self.whiteListDirectory:
                   #return to the mother dir
                   print 'ignoring the directory ',obj.GetPath()
                   print 'at this point dir is ',dir.GetPath()
                   self.currentDir=dir
                   return self.NextHisto(pattern)
               #
            #
            #
            self.motherDir[obj.GetPath()]=self.currentDir
            self.StartLoop(obj)
            return self.NextHisto(pattern)
    #

    def NextPlotInList(self,pattern={}):
        '''gets the next plot from the plotwhitelist and returns it'''

        if self.currentPlotInWhiteList == len(self.whiteListHisto) :
            #print 'end reached ',self.currentPlotInWhiteList,len(self.whiteListHisto)
            return 'End','nopath'


        #
        #
        dir=self.currentDir
        #print 'the current dir is ',dir.GetPath()
        HistoPath=dir.GetPath()
        relHistoPath=self.GetRelativePath(HistoPath)
        #

        newHisto=self.whiteListHisto[self.currentPlotInWhiteList]
        
        HistoPath=relHistoPath+'/'+newHisto

        #
        #
        self.GetHistoFromFiles(HistoPath)
        #all the histos are now contained in
        #self.histoPack
        
        #print 'returning ',self.histoPack
        #print 'nextplotinlist being called'
        self.currentPlotInWhiteList+=1
        #
        return self.histoPack,relHistoPath[1:]
    def Next():
        '''to be defined dynamically'''
        
        print 'Next is not yet defined '

    def StartLoop(self,dir):
        """loops recursively over the directory dir"""
        #
        #
        #dir.ls()
        self.currentDir=dir
        self.nextkey[dir.GetPath()] =ROOT.TIter(dir.GetListOfKeys())
        
    
    #
    def StartLoopFromString(self,indir):
        """loops recursively over the directory dir"""
        
        if indir=='':
            #print 'self.LastTfile is ',self.LastTFile
            dir=self.LastTFile
            #print 'the dir is thus ',dir.GetPath()
        else:
            dir=self.LastTFile.Get(indir)

        #the iterative function
        self.Next = self.NextHisto        
        self.StartLoop(dir)



    def LoopOverPlotList(self,startDir,plotList):
        """loops over the loop list"""

        if startDir=='':
            #print 'self.LastTfile is ',self.LastTFile
            dir=self.LastTFile
            #print 'the dir is thus ',dir.GetPath()
        else:
            dir=self.LastTFile.Get(startDir)

        self.whiteListHisto=copy.deepcopy(plotList)
        self.Next = self.NextPlotInList
        self.StartLoop(dir)
        

    def Start(self,startDir='',plotList=[]):
        '''starts from either a directory, or a plot list'''
        if len(plotList)==0:
            self.StartLoopFromString(startDir)
        else:
            self.LoopOverPlotList(startDir,plotList)
        
    def getCurrentPlotName(self):
        print 'the function getCurrentPlotName maybe doesnt work correctly? '
        return self.whiteListHisto[self.currentPlotInWhiteList-1]
        
