####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = emoticon_nottysmile.png emoticon_smile.png emoticon_devil.png emoticon_angry.png emoticon_angry2.png emoticon_bigsm.png emoticon_blink.png emoticon_blush.png emoticon_cry.png emoticon_love.png emoticon_quest.png emoticon_sad.png emoticon_sungl.png emoticon_surp.png emoticon_tongue.png emoticon_yuck.png emoticon_straight.png emoticon_sleep.png emoticon_sarc.png emoticon_quiet.png emoticon_pissed.png emoticon_laugh.png emoticon_glass.png emoticon_clown.png emoticon_bye.png emoticon_angel.png 

####### kdevelop will overwrite this part!!! (end)############

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(kde_datadir)/kyim/pics/
	$(INSTALL_DATA) $(EXTRA_DIST) $(DESTDIR)$(kde_datadir)/kyim/pics/
	
uninstall-local:
	-rm -rf $(DESTDIR)$(kde_datadir)/kyim/pics
