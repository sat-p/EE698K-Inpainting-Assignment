top_builddir = .
include $(top_builddir)/makeinclude

all:
	@echo "Please specify target: bin, install, uninstall, doc, or clean."

bin:
	$(MAKE) -C code all

install: install_bin

install_bin: bin
	$(INSTALL) -m 755 -D \
		$(top_builddir)/code/app/src/main \
		$(BIN_DIR)/EE604A-14610

uninstall:
	$(RM) \
		$(BIN_DIR)/EE604A-14610

doc:
	doxygen doxy.conf

clean:
	rm -f *~
	$(MAKE) -C code clean

.PHONY: all bin install \
	install_bin \
	uninstall doc clean