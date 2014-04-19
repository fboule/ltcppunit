Summary:        Lightweight C++ Unit Testing
License:        GPL
Name:           ltcppunit
Version:        1.0
Release:        1%{?dist}
Source:         %{name}-%{version}.tar.gz
Group:          Development/Tools

BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires:  libtool
BuildRequires:  gcc-c++

%description
Lightweight C++ Unit Testing

%prep
%setup -q

%build
./configure
make

%install
make install prefix=$RPM_BUILD_ROOT/usr

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/include/ltcppunit/common.h
/usr/include/ltcppunit/ltcppunit.h
/usr/lib/lib%{name}-%{version}.a
/usr/lib/lib%{name}-%{version}.la
/usr/lib/lib%{name}-%{version}.so
/usr/lib/lib%{name}-%{version}.so.0
/usr/lib/lib%{name}-%{version}.so.0.0.0

%changelog
