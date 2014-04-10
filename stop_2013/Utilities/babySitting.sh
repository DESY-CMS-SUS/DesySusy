for JOBDIR in `cat workdir.txt`
  do
  echo $JOBDIR
  rm -f help.txt status.txt

  OUTNAME=`grep output_file $JOBDIR/share/crab.cfg | sed 's/output_file//g' | sed 's/=//g' | sed 's/ //g' | sed 's/.root//g'`
  echo "OUTNAME = $OUTNAME"
  if [[ -z $OUTNAME ]]
      then      
      continue
  fi
  
  BASEDIR=`grep user_remote_dir $JOBDIR/share/crab.cfg | sed 's/user_remote_dir//g' | sed 's/=//g' | sed 's/ //g'`
  echo "BASEDIR = $BASEDIR"
  if [[ -z $BASEDIR ]]
      then      
      continue
  fi

  crab -status -c $JOBDIR > status.txt

  skipline=-1
  dcls $DC_HOME/$BASEDIR > help.txt
  while read line
    do
    if [[ -n `echo $line | grep "Jobs Cleared"` ]]
	then
	echo "cleared Jobs!"
	skipline=2
    fi
    if [[ skipline -eq 0 ]]
	then
	CLEAREDJOBS=`echo $line | sed 's/List of jobs://g' | sed 's/ //g'`
	if [[ -n $CLEAREDJOBS ]]
	    then
	    for subStr in `echo $CLEAREDJOBS | sed 's/,/\n/g'`
	      do
	      i=`echo $subStr | cut -f1 -d'-'`
	      j=`echo $subStr | cut -f2 -d'-'`
	      for ((k = $i; k <= $j; k++))
		do
		for file in `grep ${OUTNAME}_${k}_ help.txt | sort`
		  do
		  echo removing $file
		  dcdel $DC_HOME/$BASEDIR/$file
		done
	      done
	    done
	    echo killing $CLEAREDJOBS
	    crab -kill $CLEAREDJOBS -c $JOBDIR
	    echo resubmitting $CLEAREDJOBS
	    crab -resubmit $CLEAREDJOBS -c $JOBDIR
	fi
	break
    fi
    skipline=`expr $skipline - 1`
  done < status.txt


  NJOBS=`grep "Total Jobs" status.txt | sed 's/Total Jobs//g' | sed 's/crab://g' | sed 's/ //g'`
  echo "NJOBS = $NJOBS"

  crab -getoutput 1-$NJOBS -c $JOBDIR

  rm -f help.txt
  dcls $DC_HOME/$BASEDIR > help.txt
  for ((i = 1; i < `expr $NJOBS + 1`; i++))
    do
    NFILES=`grep ${OUTNAME}_${i}_ help.txt | wc -l`
    if [[ $NFILES -gt 1 ]]
	then
	for file in `grep ${OUTNAME}_${i}_ help.txt | sort`
	  do
	  echo removing $file
	  dcdel $DC_HOME/$BASEDIR/$file
	done
    fi
  done

  rm -f help.txt
  dcls $DC_HOME/$BASEDIR > help.txt
  MISSINGJOBS=""
  for ((i = 1; i < `expr $NJOBS + 1`; i++))
    do
    NFILES=`grep ${OUTNAME}_${i}_ help.txt | wc -l`
    if [[ $NFILES -eq 0 ]]
	then
	MISSINGJOBS_OLD=$MISSINGJOBS
	MISSINGJOBS=`echo "$MISSINGJOBS_OLD","$i"`
    fi
  done

  MISSINGJOBS=`echo "$MISSINGJOBS" | sed 's/^,//' | sed 's/ //g'`
  if [[ -n $MISSINGJOBS ]]
      then
      echo resubmitting $MISSINGJOBS
      crab -resubmit $MISSINGJOBS -c $JOBDIR
  fi

  crab -status -c $JOBDIR
  echo NFILES = `dcls $DC_HOME/$BASEDIR | grep .root | wc -l`
done





