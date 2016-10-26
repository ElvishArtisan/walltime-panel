## automake.am
##
## Top level automake.am
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2015 Fred Gleason <fredg@paravelsystems.com>
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

SUBDIRS = icons\
          src\
          xdg

rpm:	dist
	cp walltime-panel-$(VERSION).tar.gz $(HOME)/rpmbuild/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux walltime-panel.spec
	mv $(HOME)/rpmbuild/RPMS/$(ARCH)/walltime-panel-*.rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/SRPMS/walltime-panel-*.src.rpm $(top_srcdir)/
	rm $(HOME)/rpmbuild/SOURCES/walltime-panel-$(VERSION).tar.gz
	rm -rf $(HOME)/rpmbuild/BUILD/walltime-panel-$(VERSION)

install-win32:	
	cp src/walltime-panel.exe .
	cp $(MINGW32_SYS_ROOT)/bin/QtCore4.dll .
	cp $(MINGW32_SYS_ROOT)/bin/QtGui4.dll .
	cp $(MINGW32_SYS_ROOT)/bin/QtNetwork4.dll .
	cp $(MINGW32_SYS_ROOT)/bin/libgcc_s_sjlj-1.dll .
	cp $(MINGW32_SYS_ROOT)/bin/libpng16-16.dll .
	cp $(MINGW32_SYS_ROOT)/bin/libstdc++-6.dll .
	cp $(MINGW32_SYS_ROOT)/bin/libwinpthread-1.dll .
	cp $(MINGW32_SYS_ROOT)/bin/zlib1.dll .


EXTRA_DIST = autogen.sh\
             get_distro.sh\
             get_target.sh\
             walltime-panel.ism\
             walltime-panel.spec.in\
             COPYING.rtf\
             PACKAGE_VERSION

CLEANFILES = *~\
             *.deb\
             *.dll\
             *.exe\
             *.mo\
             *.rpm\
             *.tar.gz\
             *.tgz\
             *.zip

DISTCLEANFILES = walltime-panel.spec\
                 config.guess\
                 config.sub\
                 ltmain.sh

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       config.guess\
                       config.status\
                       config.sub\
                       configure\
                       depcomp\
                       install-sh\
                       libtool\
                       ltmain.sh\
                       Makefile.in\
                       missing\
                       mkinstalldirs
