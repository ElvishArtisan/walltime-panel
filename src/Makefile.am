## automake.am
##
## Makefile for the walltime-panel applet
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
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -Wno-strict-aliasing -fPIC @QT4_CFLAGS@
MOC = @QT4_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	@MOC@ $< -o $@


bin_PROGRAMS = walltime-panel

dist_walltime_panel_SOURCES = cmdswitch.cpp cmdswitch.h\
                              config.cpp config.h\
                              profile.cpp profile.h\
                              walltime-panel.cpp walltime-panel.h

nodist_walltime_panel_SOURCES = moc_walltime-panel.cpp

walltime_panel_LDADD = @GUILIBS@
walltime_panel_LDFLAGS = @GUIFLAGS@

CLEANFILES = *~\
             moc_*\
             *.obj\
             *.idb\
             *.pdb\
             *ilk
MAINTAINERCLEANFILES = *~\
                       Makefile.in
