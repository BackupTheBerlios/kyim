####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = ding.wav door.wav knock.wav buzz.wav 

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(kde_datadir)/kyim/sounds/
	$(INSTALL_DATA) $(srcdir)/ding.wav $(DESTDIR)$(kde_datadir)/kyim/sounds/ding.wav
	$(mkinstalldirs) $(DESTDIR)$(kde_datadir)/kyim/sounds/
	$(INSTALL_DATA) $(srcdir)/door.wav $(DESTDIR)$(kde_datadir)/kyim/sounds/door.wav
	$(mkinstalldirs) $(DESTDIR)$(kde_datadir)/kyim/sounds/
	$(INSTALL_DATA) $(srcdir)/knock.wav $(DESTDIR)$(kde_datadir)/kyim/sounds/knock.wav
	$(mkinstalldirs) $(DESTDIR)$(kde_datadir)/kyim/sounds/
	$(INSTALL_DATA) $(srcdir)/buzz.wav $(DESTDIR)$(kde_datadir)/kyim/sounds/buzz.wav

uninstall-local:
	-rm -f $(DESTDIR)$(kde_datadir)/kyim/sounds/ding.wav
	-rm -f $(DESTDIR)$(kde_datadir)/kyim/sounds/door.wav
	-rm -f $(DESTDIR)$(kde_datadir)/kyim/sounds/knock.wav
	-rm -f $(DESTDIR)$(kde_datadir)/kyim/sounds/buzz.wav

####### kdevelop will overwrite this part!!! (end)############
