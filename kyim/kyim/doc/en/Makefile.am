
# This is where it will all be installed
datadir = $(kde_htmldir)/en/kyim

html: index.docbook
	ksgml2html index.sgml en

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datadir)
	$(INSTALL_DATA) index.html $(DESTDIR)$(datadir)/index.html
	$(INSTALL_DATA) index-1.html $(DESTDIR)$(datadir)/index-1.html
	$(INSTALL_DATA) index-2.html $(DESTDIR)$(datadir)/index-2.html
	$(INSTALL_DATA) index-3.html $(DESTDIR)$(datadir)/index-3.html
	$(INSTALL_DATA) index-4.html $(DESTDIR)$(datadir)/index-4.html
	$(INSTALL_DATA) index-5.html $(DESTDIR)$(datadir)/index-5.html
	$(INSTALL_DATA) logotp3.png $(DESTDIR)$(datadir)/logotp3.png

uninstall-local:
	-rm -f $(DESTDIR)$(datadir)/index.html
	-rm -f $(DESTDIR)$(datadir)/index-1.html
	-rm -f $(DESTDIR)$(datadir)/index-2.html
	-rm -f $(DESTDIR)$(datadir)/index-3.html
	-rm -f $(DESTDIR)$(datadir)/index-4.html
	-rm -f $(DESTDIR)$(datadir)/index-5.html
	-rm -f $(DESTDIR)$(datadir)/logotp3.png
