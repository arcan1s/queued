#!/bin/bash

ARCHIVE="queued"
[ -d ${ARCHIVE} ] && rm -rf ${ARCHIVE}

mkdir -p ${ARCHIVE}/usr/{bin,lib/systemd/system}
mkdir -p ${ARCHIVE}/usr/share/man/{man1,man5}
mkdir -p ${ARCHIVE}/etc

cp sources/queued ${ARCHIVE}/usr/bin/
cp sources/add_queued ${ARCHIVE}/usr/bin/
cp sources/queued.service ${ARCHIVE}/usr/lib/systemd/system/
cp sources/queued.conf ${ARCHIVE}/etc/
cp sources/*.1 ${ARCHIVE}/usr/share/man/man1/
cp sources/*.5 ${ARCHIVE}/usr/share/man/man5/
cp install.sh ${ARCHIVE}/
cp {AUTHORS,COPYING} ${ARCHIVE}/

VERSION=$(grep Version sources/queued | awk '{printf $5;}')
tar -cf - ${ARCHIVE} | xz -9 -c - > ${ARCHIVE}-${VERSION}-src.tar.xz
rm -r ${ARCHIVE}

# update PKGBUILD
MD5SUMS=$(md5sum ${ARCHIVE}-${VERSION}-src.tar.xz | awk '{print $1}')
sed -i "s/md5sums=('[0-9a-f]\{32\}')/md5sums=('${MD5SUMS}')/" PKGBUILD
sed -i "s/pkgver=[0-9.]\{5\}/pkgver=${VERSION}/" PKGBUILD
