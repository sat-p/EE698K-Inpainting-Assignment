top_builddir = ../..
include ${top_builddir}/makeinclude

WC = \
	ui/WC_MainWindow.fl \
	ui/WC_ImageBox.fl

WC_H = ${WC:ui/%.fl=include/%.h}
WC_SRC = ${WC:ui/%.fl=src/%.cxx}

all: ui
	${MAKE} -C src dep
	${MAKE} -C src obj
	${MAKE} -C src bin

ui: ${WC_H} ${WC_SRC}

include/%.h src/%.cxx: ui/%.fl
	cd src; \
	$(FLUID) -c \
		-o $(basename $(notdir $<)).cxx \
		-h ../include/$(basename $(notdir $<)).h \
		../$<

clean:
	${MAKE} -C src clean
	rm -rf *~ */*~