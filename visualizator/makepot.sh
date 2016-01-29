if [ -z "$1" ]; then
	echo "uso: $0 <version>";
	exit 1;
fi

cd lang && xgettext -s -d visualizator -L C++ --keyword=_ --keyword=_Std --copyright-holder="MetaEmotion S.L" --package-name="Ginkgo CADx Visualizator"  --package-version="$1" --msgid-bugs-address=ginkgo@metaemotion.com -o visualizator.pot `find .. -type f |grep -i 'cpp\|\.c$' |grep -v .svn`
