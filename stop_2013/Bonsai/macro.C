{
  //#include <map>
  //#include <string>
  
  //std::map< std::string, bool>;
  //myMap["Selected"] = 1;
  
  //cout<<myMap["Selected"]<<endl;
  //std::map< std::string, bool>::const_iterator iter = myMap.begin();
  //cout<<"here1"<<endl;
  //cout<<(*iter).first<<endl;



  .L ../libRA4bDict.so;
  TFile *_file0 = TFile::Open("../SkimmingTrees/Data.root");
  skimmingTree.Process("BonsaiTSelector.C");
}
