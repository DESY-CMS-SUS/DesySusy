





from ROOT import TFile, TH1D




#if __name__=='__main__':

def makeFile(name,lumi=1.0):

    f=TFile(name,'RECREATE')



    h1=TH1D('h1','h1',50,-10.,10.)
    h1.FillRandom('gaus',int(10000*lumi))

    h2=TH1D('h2','h2',50,-10.,10.)
    h2.FillRandom('gaus',int(50000*lumi))

    h3=TH1D('h3','h3',100,-10.,10.)
    h3.FillRandom('gaus',int(50000*lumi))

    h4=TH1D('h4','h4',100,-10.,10.)
    h4.FillRandom('gaus',int(50000*lumi))

    h5=TH1D('h5','h5',100,-10.,10.)
    h5.FillRandom('gaus',int(50000*lumi))

    h6=TH1D('h6','h6',100,-10.,10.)
    h6.FillRandom('gaus',int(50000*lumi))        

    h7=TH1D('h7','h7',100,-10.,10.)
    h7.FillRandom('gaus',int(50000*lumi))

    p1=TH1D('p1','p1',100,-10.,10.)
    p1.FillRandom('gaus',int(50000*lumi))

    p2=TH1D('p2','p2',100,-10.,10.)
    p2.FillRandom('gaus',int(50000*lumi))

    p3=TH1D('p3','p3',100,-10,10.)
    p3.FillRandom('gaus',int(50000*lumi))        
    

    h1.Write()
    h2.Write()
    h3.Write()
    h4.Write()
    h5.Write()
    h6.Write()
    h7.Write()
    p1.Write()
    p2.Write()
    p3.Write()    



    
    f1=f.mkdir('FirstDirectory')
    f1.cd()

    h1=TH1D('FD_h1','h1',50,-10.,10.)
    h1.FillRandom('gaus',int(10000*lumi))
    h2=TH1D('FD_h2','h2',50,-10.,10.)
    h2.FillRandom('gaus',int(50000*lumi))
    h3=TH1D('FD_h3','h3',100,-10.,10.)
    h3.FillRandom('gaus',int(50000*lumi))

    h1.Write()
    h2.Write()
    h3.Write()


    f2=f1.mkdir('F_SubDirectory')
    f2.cd()

    h1=TH1D('FD_h1','h1',50,-10.,10.)
    h1.FillRandom('gaus',int(10000*lumi))
    h2=TH1D('FD_h2','h2',50,-10.,10.)
    h2.FillRandom('gaus',int(50000*lumi))
    h3=TH1D('FD_h3','h3',100,-10.,10.)
    h3.FillRandom('gaus',int(50000*lumi))

    h1.Write()
    h2.Write()
    h3.Write()

    

    f.cd()    
    f1=f.mkdir('SecondDirectory')
    f1.cd()
    h1=TH1D('SD_h1','h1',50,-10.,10.)
    h1.FillRandom('gaus',int(10000*lumi))
    h2=TH1D('SD_h2','h2',50,-10.,10.)
    h2.FillRandom('gaus',int(50000*lumi))
    h3=TH1D('SD_h3','h3',100,-10.,10.)
    h3.FillRandom('gaus',int(50000*lumi))

    h1.Write()
    h2.Write()
    h3.Write()

    f.cd()    
    f1=f.mkdir('ThirdDirectory')
    f1.cd()
    h1=TH1D('TD_h1','h1',50,-10.,10.)
    h1.FillRandom('gaus',int(10000*lumi))
    h2=TH1D('TD_h2','h2',50,-10.,10.)
    h2.FillRandom('gaus',int(50000*lumi))
    h3=TH1D('TD_h3','h3',100,-10.,10.)
    h3.FillRandom('gaus',int(50000*lumi))            


    h1.Write()
    h2.Write()
    h3.Write()
    



if __name__=='__main__':

    
    makeFile('testData.root',1.0)
    makeFile('testTTbar.root',0.7)
    makeFile('testQCD.root',0.3)    
    
    
