source /afs/naf.desy.de/group/cms/proof/PoD/PoD_env.sh
#source ./PoD_env.sh2
pod-server start
pod-submit -q proof.q -n 80 -r ge
