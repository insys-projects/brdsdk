#!/bin/bash

#
# Если мы вызываем этот скрипт для сборки под ARM,
# например из среды buildroot, то необходимо в
# в качестве параметра передать путь к скрипту:
#
# source ${HOME}/brdsdk/sdkenv.sh ${HOME}/brdsdk
#
# В остальных случаях использовать стандартно:
#
# source ${HOME}/brdsdk/sdkenv.sh
#

SETTINGS_FILE='sdkenv.sh'
SCRIPT_LOC="./"

if [ $# != 0 ]; then
        SCRIPT_LOC="$1"
else
        #  SCRIPT_LOC should point to script location
        if [ "$0" == "ksh" ]; then
                SCRIPT_LOC_TMP_UNI=`readlink -f ${BRD_ARG_}`
        else
                SCRIPT_LOC_TMP_UNI=$BASH_SOURCE
        fi
        SCRIPT_LOC_TMP_UNI=${SCRIPT_LOC_TMP_UNI%/*}
        if [ "$SCRIPT_LOC_TMP_UNI" != "" ]; then
                if [ "$SCRIPT_LOC_TMP_UNI" == "sdkenv.sh" ]; then
                        SCRIPT_LOC_TMP_UNI="./"
                fi
                SCRIPT_LOC_TMP_UNI=`readlink -f ${SCRIPT_LOC_TMP_UNI}`
                if [ $? == 0 ]; then
                        SCRIPT_LOC=${SCRIPT_LOC_TMP_UNI}
                fi
        fi
        unset SCRIPT_LOC_TMP_UNI
fi


export BRDSDKDIR=`readlink -f "${SCRIPT_LOC}"`
export BRDSDKLIB=${BRDSDKDIR}/lib

export LD_LIBRARY_PATH=${GIPCYLIB}:${BARDYLIB}:${BRDSDKLIB}:${LD_LIBRARY_PATH}
export PS1="\[\e[32;1m\][brdsdk]\[\e[0m\]:\w> "
