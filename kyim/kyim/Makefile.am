# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = libyahoo2 doc pics sounds

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kyim

# set the include path for X, qt and KDE
INCLUDES = $(all_includes) -I./libyahoo2

# the library search path. 
kyim_LDFLAGS = $(KDE_RPATH) $(all_libraries) -L./libyahoo2

# the libraries to link against.
kyim_LDADD   = $(LIB_KFILE) -lyahoo2

# and #define for DATADIR path
AM_CPPFLAGS = -DDATADIR=\"$(DESTDIR)$(kde_datadir)/kyim\" -DUSE_STRUCT_CALLBACKS

# which sources should be compiled for kyim
kyim_SOURCES = AddBuddyImpl.cpp	      AddedMessageImpl.cpp   ChatBase.cpp\
ChatSession.cpp		      ChatWindowImpl.cpp     ConfAddInviteImpl.cpp\
ConfInvitationImpl.cpp	      ConferenceSession.cpp  ConferenceWindowImpl.cpp\
Icon.cpp\
IncomingFileTransferImpl.cpp  KYahooIface.cpp	     KYahooIface_skel.cpp\
KYahooIface_stub.cpp	      NewMailImpl.cpp	     OfflineMsgImpl.cpp\
OfflinesViewImpl.cpp	      Parse.cpp		     ProxyAuthImpl.cpp\
RemoveBuddyImpl.cpp	      SetupImpl.cpp	     StartChatDialog.cpp\
dialogs.cpp	     main.cpp\
process_packet.cpp	      statusIcons.cpp	     utils.cpp\
IMState.cpp \
AddBuddy.ui       ConfInvitation.ui    IncomingFileTransfer.ui  ProxyAuth.ui\
AddedMessage.ui   ConferenceWindow.ui  NewMail.ui               RemoveBuddy.ui\
ChatWindow.ui     OfflineMsg.ui            Setup.ui\
ConfAddInvite.ui  OfflinesView.ui          StartChat.ui\
BuddyList.ui emoticon.cpp

# these are the headers for your project that won't be installed
noinst_HEADERS = AddBuddyImpl.h		    AddedMessageImpl.h	 ChatBase.h\
ChatSession.h		    ChatWindowImpl.h	 ConfAddInviteImpl.h\
ConfInvitationImpl.h	    ConferenceSession.h  ConferenceWindowImpl.h\
Icon.h\
IncomingFileTransferImpl.h  KYahooIface.h	 KYahooIface_stub.h\
NewMailImpl.h		    OfflineMsgImpl.h	 OfflinesViewImpl.h\
Parse.h			    ProxyAuthImpl.h	 RemoveBuddyImpl.h\
SetupImpl.h		    StartChatDialog.h\
dialogs.h		    emoticons.h		 kyim.h\
kyimicons.h		    process_packet.h	 statusIcons.h\
utils.h IMState.h BuddyStatus.h emoticon.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO 

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(kde_icondir)/hicolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/kyim-hi-32x32.png $(DESTDIR)$(kde_icondir)/hicolor/32x32/apps/kyim.png
	$(mkinstalldirs) $(DESTDIR)$(kde_icondir)/hicolor/48x48/apps/
	$(INSTALL_DATA) $(srcdir)/kyim-hi-48x48.png $(DESTDIR)$(kde_icondir)/hicolor/48x48/apps/kyim.png
	$(mkinstalldirs) $(DESTDIR)$(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/kyim-lo-16x16.png $(DESTDIR)$(kde_icondir)/locolor/16x16/apps/kyim.png
	$(mkinstalldirs) $(DESTDIR)$(kde_icondir)/locolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/kyim-lo-32x32.png $(DESTDIR)$(kde_icondir)/locolor/32x32/apps/kyim.png

uninstall-local:
	-rm -f $(DESTDIR)$(kde_icondir)/hicolor/32x32/apps/kyim.png
	-rm -f $(DESTDIR)$(kde_icondir)/hicolor/48x48/apps/kyim.png
	-rm -f $(DESTDIR)$(kde_icondir)/locolor/16x16/apps/kyim.png
	-rm -f $(DESTDIR)$(kde_icondir)/locolor/32x32/apps/kyim.png


messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kyim.pot

# this is where the desktop file will go 
desktopdir   = $(kde_appsdir)/Internet
desktop_DATA = kyim.desktop
