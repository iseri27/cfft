src := cfft.sh

PREFIX=$$HOME/.local

all: ${src}
	mkdir -p ${PREFIX}/bin
	cp -f cfft.sh ${PREFIX}/bin/cfft
	chmod +x ${PREFIX}/bin/cfft

install: all

uninstall:
	rm -f ${PREFIX}/bin/cfft
