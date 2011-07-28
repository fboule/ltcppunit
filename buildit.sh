#!/bin/bash

make dist
st=$?
[ $st -ne 0 ] && exit $st
mv ltcppunit-1.0.tar.gz ~/rpmbuild/SOURCES/
cd ~/rpmbuild
rpmbuild -ba ~/Projects/GIT/ltcppunit.git/ltcppunit.spec
sudo yum reinstall RPMS/i686/ltcppunit-1.0-1.fc15.i686.rpm RPMS/i686/ltcppunit-devel-1.0-1.fc15.i686.rpm -Cy
