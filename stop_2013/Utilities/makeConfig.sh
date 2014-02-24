#!/bin/zsh

FILE=DC.txt
if [ $1 ]
then
FILE=$1
fi

rm -f pu_config.txt para_config.txt twiki.txt SetUpSamples.txt

touch pu_config.txt
touch para_config.txt
touch twiki.txt
touch SetUpSamples.txt

./dofast getpileupinflust
./dofast writeParaConfig

oldSample=dummy

for DS GT Sample Subsample xs dir NTuple Status type in `cat $FILE | awk -F "," '{print $1" "$3" "$6" "$7" "$5" "$8" "$10" "$11" "$2}'`
  do
  echo $Sample $Subsample
   
  PD=`echo $DS | cut -f2 -d"/"`
  SD=`echo $DS | cut -f3 -d"/"`
  TD=`echo $DS | cut -f4 -d"/"`
  
  NTries=`./getpileupinflust $Sample $Subsample $dir ./`
  NTries=`echo $NTries | grep % | cut -f2 -d"%"`
  
  echo $NTries  
  ./writeParaConfig $Sample $Subsample ./ $NTries $xs 1

  echo $dir | grep -q "/pnfs/desy.de/cms/tier2/store/user/"
  if [ $? -eq 0 ]
      then
      NFiles=`dcls $dir | grep .root | wc -l`
  else
      NFiles=`ls $dir | grep .root | wc -l`
  fi
  NFilesJob=`expr 250000 \* $NFiles`
  NFilesJob=`expr $NFilesJob / $NTries`
  if [[ $NFilesJob -eq 0 ]]
      then
      NFilesJob=1
  fi
  
  echo $Sample | grep -q $oldSample
  if [ $? -eq 0 ]
      then
      SampleForTwiki=\^
  else
      SampleForTwiki=\!$Sample
      echo -e "def ${Sample}():" >> SetUpSamples.txt
      echo -e "\tSample='$Sample'" >> SetUpSamples.txt
      echo $type | grep -q Data
      if [ $? -eq 0 ]
	  then
	  echo -e "\tConfig='config_DATA_RA4b.txt'" >> SetUpSamples.txt
      else
	  echo -e "\tConfig='config_MC_RA4b.txt'" >> SetUpSamples.txt
      fi
      echo -e "\tnFiles=$NFilesJob" >> SetUpSamples.txt
      echo -e "\tInfoPack='[]'" >> SetUpSamples.txt
  fi
  
  echo -e "\tSubSample='$Subsample'" >> SetUpSamples.txt
  echo -e "\tFileDir='$dir'" >> SetUpSamples.txt
  echo -e "\tInfoPack.append((SubSample,FilesDir,nFiles))" >> SetUpSamples.txt
  echo -e "'#'"  >> SetUpSamples.txt
  
  echo \| $SampleForTwiki \| \!$Subsample \| %Y% \| "=$dir=" \| $xs  \| $NTries \| $GT \| \<a href="https://cmsweb.cern.ch/das/request?input=dataset%3D%2F$PD%2F$SD%2F$TD&instance=cms_dbs_prod_global"\>link\</a\> \| $NTuple \| >> twiki.txt

  oldSample=$Sample
  
done

echo -e "\tInfoPack.append((SubSample,FilesDir,nFiles))" >> SetUpSamples.txt


#DS,type,GT,NJobs,xs,Sample,SubSample,Dir,NTupler,NTupleVersion,Status
