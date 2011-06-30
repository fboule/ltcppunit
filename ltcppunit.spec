Name:           ltcppunit
Version:        1.0
Release:        1%{?dist}
Summary:        Lightweight C++ Unit Testing

Group:          Development/Tools
License:        GPL
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
Lightweight C++ Unit Testing


%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc
/usr/include/ltcppunit/common.h
/usr/include/ltcppunit/ltcppunit.h
/usr/lib/libltcppunit-1.0.a
/usr/lib/libltcppunit-1.0.la
/usr/lib/libltcppunit-1.0.so
/usr/lib/libltcppunit-1.0.so.0
/usr/lib/libltcppunit-1.0.so.0.0.0




%changelog
