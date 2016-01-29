#!/bin/bash
# This scripts build the extension descriptors
SRCDIR="$1"
DLLPATH="$2"
INFPATH="$3"
CORE_VERSION=$4
CORE_SUBVERSION=$5

cd "`dirname $0`"

if [ -z ${DLLPATH} ] || [ -z ${INFPATH} ] || [ -z ${CORE_VERSION} ] || [ -z ${CORE_SUBVERSION} ]; then
	echo "Uso: $0 <srcdir> <dll_path> <inf_path> <ginkgo_version> <ginkgo_release>"
	exit 1
fi
if ! [ -d "${SRCDIR}" ]; then
	echo "srcdir not found"
	exit 1
fi

DLLFILE=`basename ${DLLPATH}`

LICENSE_FILE="`find "${SRCDIR}" -maxdepth 2 -name 'license.h' | sed '1q'`"

if ! [ -f "${LICENSE_FILE}" ]; then
	echo "license file not found"
	exit 1
fi

(echo "DLL=${DLLFILE}" > "${INFPATH}") || exit 1
(cat "$LICENSE_FILE" | grep 'EXT_' | sed "s/[^\(EXT_\)]*EXT_\([^ \t]*\)[  \t]*/\1=/" | while read line
do
	KEY="${line%%=*}"
	VAL="${line##*=}"
	VAL=`echo $VAL | sed 's/_Std("\([^"]*\).*/\1/;s/^"\$[^:]*:[ \t]*\([^ \t\$]*\).*/\1/;s/^"\(.*\)"$/\1/'`
	echo "$KEY=$VAL"
done
) | grep "^SID\|^PROVIDER\|^DESCRIPTION\|^VERSION\|^SUBVERSION\|^RELEASE\|^BUILD\|^CODENAME\|^UPDATEURL"  >> "${INFPATH}"
echo "CORE_VERSION=${CORE_VERSION}" >> "${INFPATH}"
echo "CORE_SUBVERSION=${CORE_SUBVERSION}" >> "${INFPATH}"

exit 0


