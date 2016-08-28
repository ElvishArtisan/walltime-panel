## automake.am
##
## icons/automake.am
##
## (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
##
## Use automake to process this into a Makefile.in

install-exec-am:	
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps
	cp walltime-16x16.png $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/walltime-panel.png
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/22x22/apps
	cp walltime-22x22.png $(DESTDIR)@prefix@/share/icons/hicolor/22x22/apps/walltime-panel.png
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/32x32/apps
	cp walltime-32x32.png $(DESTDIR)@prefix@/share/icons/hicolor/32x32/apps/walltime-panel.png
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps
	cp walltime-48x48.png $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/walltime-panel.png


uninstall-local:	
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/walltime-panel.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/22x22/apps/walltime-panel.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/32x32/apps/walltime-panel.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/walltime-panel.png

EXTRA_DIST = settings.xpm\
             walltime_logo.xpm\
             walltime-16x16.xpm\
             walltime-16x16.png\
             walltime-22x22.png\
             walltime-32x32.png\
             walltime-48x48.ico\
             walltime-48x48.png\
             walltime-128x128.png\
             walltime-256x256.png\
             walltime-512x512.png

CLEANFILES = *~

MAINTAINERCLEANFILES = *~\
                       aclocal.m4\
                       configure\
                       Makefile.in
