#!/bin/zsh

DCHOME=/pnfs/desy.de/cms/tier2/store/user/fcostanz

NTUPLEVERSION=NTuple13_v1
NTUPLEHOME=$DCHOME/$NTUPLEVERSION

FILE=list.txt
if [ $1 ]
then
FILE=$1
fi

rm -f twiki.txt
touch twiki.txt

for DS GT cfg in `cat $FILE | awk '{print $3" "$4" "$5}'`
do
PD=`echo $DS | cut -f2 -d"/"`
SD=`echo $DS | cut -f3 -d"/"`
TD=`echo $DS | cut -f4 -d"/"`

GT=`echo $GT | sed 's/::All//g'`

jobFolder=`echo naf_$cfg | sed 's/.py//g'`

echo $DS $jobFolder

mkdir -p rootfiles/$PD/$SD
mv $jobFolder/*.root rootfiles/$PD/$SD/.

dcmkdir $NTUPLEHOME/$PD
dcmkdir $NTUPLEHOME/$PD/$SD

while ( . ./check.sh $PD/$SD; [ "$?" -ne 0 ] )
do

. ./cleanDir.sh $NTUPLEHOME/$PD/$SD

dcmkdir $NTUPLEHOME/$PD
dcmkdir $NTUPLEHOME/$PD/$SD

dcput rootfiles/$PD/$SD/ $NTUPLEHOME/$PD/$SD/
done

mkdir -p logfiles/$PD/$SD
cp $jobFolder/* logfiles/$PD/$SD/.


echo \| \^ \| `echo $cfg | sed 's/_cfg.py//g'` \| %Y% \| "=$NTUPLEHOME/$PD/$SD/=" \| \| \| $GT \| \<a href="https://cmsweb.cern.ch/das/request?input=dataset%3D%2F$PD%2F$SD%2F$TD&instance=cms_dbs_prod_global"\>link\</a\> \| $NTUPLEVERSION \| >> twiki.txt

done