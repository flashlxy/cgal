%define internal_release 0
%if %{internal_release}
%define tarball_name CGAL-3.2-I-%{internal_release}
%define release %{internal_release}
%define CGAL_DIR %{_libdir}/CGAL-3.2-I
%else
%define tarball_name CGAL-3.2
%define release 1
%define CGAL_DIR %{_libdir}/CGAL-3.2
%endif
%define boost_version 1.32

Summary: Computational Geometry Algorithms Library
Name: CGAL
Version: 3.2
Release: %{release}
License: QPL/LGPL
URL: http://www.cgal.org/
Group: System Environment/Libraries
Source: %{tarball_name}.tar.gz
Source1:%{name}-%{version}-doc_pdf.tar.gz
Source2:%{name}-%{version}-doc_html.tar.gz
Patch0: CGAL.fix_perl_path.patch
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

#Prefix: %{CGAL_DIR}

Prereq: /sbin/ldconfig
Prereq: fileutils

# Required packages.
Requires: boost >= %boost_version
Requires: gmp
Requires: qt >= 3.0
Requires: libstdc++


BuildRequires: gmp-devel
BuildRequires: boost-devel >= %boost_version
BuildRequires: gcc-c++
BuildRequires: libstdc++-devel
BuildRequires: qt-devel >= 3.0



%description
Libraries for CGAL applications.
CGAL is a collaborative effort of several sites in Europe and
Israel. The goal is to make the most important of the solutions and
methods developed in computational geometry available to users in
industry and academia in a C++ library. The goal is to provide easy
access to useful, reliable geometric algorithms.

%package devel
Group: Development/Libraries
Summary: Development files and tools for %name applications
Requires: boost-devel >= %boost_version, gmp-devel
Requires: %{name}  = %{version}-%{release}

%package demo
Group: Development/Libraries
Summary: demo of %name algorithms.
Requires: %{name}-devel  = %{version}-%{release}

%package doc
Group: Documentation
Summary: HTML and PDF documentation for developing with %name

%description devel
The %{name}-devel package provides the headers files and tools you may need to 
develop applications using %name.

%description doc
The %{name}-doc package provides the html and pdf documentation of %name.

%description demo
The %{name}-demo package provides some demos of %name algorithms.(to be compiled)

%prep
%setup -n %{name}-%{version}
%setup -D -T -a 1
%setup -D -T -a 2

%patch0 -p1
 
%build
rm -rf $RPM_BUILD_ROOT
./install_cgal -ni g++  --prefix ${RPM_BUILD_ROOT}%{CGAL_DIR} --CUSTOM_CXXFLAGS "$RPM_OPT_FLAGS"

%install
CGAL_OS=`./install_cgal -os g++`
MAKEFILE=makefile_${CGAL_OS}
ln -s $MAKEFILE ${RPM_BUILD_ROOT}%{CGAL_DIR}/make/makefile
sed -i "s,$RPM_BUILD_ROOT,,g;" ${RPM_BUILD_ROOT}%{CGAL_DIR}/make/$MAKEFILE

MAKEFILE=%{CGAL_DIR}/make/makefile
cd $RPM_BUILD_ROOT
mkdir -p ./etc/profile.d
cat > ./etc/profile.d/cgal.sh <<EOF
if [ -z "\$CGAL_MAKEFILE" ] ; then
	CGAL_MAKEFILE="$MAKEFILE"
	CGAL_DIR="%{CGAL_DIR}"
fi
export CGAL_MAKEFILE CGAL_DIR
EOF

cat > ./etc/profile.d/cgal.csh <<EOF

if ( \$?CGAL_MAKEFILE ) then
         exit
endif
setenv CGAL_MAKEFILE "$MAKEFILE"
setenv CGAL_DIR "%{CGAL_DIR}"
EOF
chmod 755 ./etc/profile.d/cgal.*sh
%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%dir %{CGAL_DIR}
%dir %{CGAL_DIR}/lib
%dir %{CGAL_DIR}/lib/*
%{CGAL_DIR}/lib/*/libCGAL.so
%doc LICENSE*
%doc README

%files devel
%defattr(-,root,root,-)
%{CGAL_DIR}/include
%{CGAL_DIR}/lib/*/*.a
%{CGAL_DIR}/make
%{CGAL_DIR}/bin
/etc/profile.d/cgal.*
%doc LICENSE*
%doc README

%files doc
%defattr(-,root,root,-)
%doc doc_html
%doc doc_pdf
%doc LICENSE*

%files demo
%defattr(-,root,root,-)
%doc demo
%doc LICENSE*
%doc README

%changelog
* Fri Mar 10 2006 Naceur MESKINI <nmeskini@sophia.inria.fr>
- adding new sub-packages doc(pdf&html) and demo.
- add internal_release 395flag. 
* Thu Mar 09 2006 Naceur MESKINI <nmeskini@sophia.inria.fr>
- cleanup a specfile.
