#!/bin/zsh

FILE=DC.txt
if [ $1 ]
then
FILE=$1
fi

red='\e[0;31m'
green='\e[0;32m'
NC='\e[0m' # No Color


for dir in `cat $FILE | awk -F "," '{print $11}'`
  do
  rm -f help.txt help2.txt sort.txt sort2.txt

  echo $dir
  echo $dir | grep -q "pnfs"
  if [[ $? -eq 0 ]]
      then
      dcls $dir > help.txt
  else
      ls $dir/*.root > help.txt
  fi
  touch help2.txt
  
  for rootfile in `cat help.txt`
    do
    break
  done
  echo $rootfile
  echo $rootfile | grep -q cfg
  if [ $? -eq 0 ]
      then
      continue
  fi
  
  for rootfile in `cat help.txt`
    do
    echo $rootfile| rev | cut -f 3- -d"_" | rev >> help2.txt
  done

  sort help2.txt > sort.txt
  sort -u help2.txt > sort2.txt

  if [[ `sort -u help2.txt | wc -l` -ne `more help.txt | wc -l` ]]
      then
      echo -e "${red}Houston, we've got a problem${NC}" $dir
      diff sort.txt sort2.txt
  else
      echo -e "${green}Kein Problem Digga!!${NC}"
  fi
done

rm -f help.txt help2.txt sort.txt sort2.txt