# Author: Evgeniy "arcanis" Alexeev <esalexeev@gmail.com>
# Maintainer: Evgeniy "arcanis" Alexeev <esalexeev@gmail.com>

pkgname=queued
pkgver=1.1.0
pkgrel=1
pkgdesc="Daemon for starting jobs to queue of calculations"
arch=('any')
url="https://github.com/arcan1s/queued"
license=("GPL")
makedeps=('git')
source=(https://github.com/arcan1s/queued/releases/download/V.${pkgver}/${pkgname}-${pkgver}-src.tar.xz)
md5sums=('c7f710d8779e7cd780f7e8043735cae7')
backup=('etc/queued.conf')

package()
{
  # daemon
  install -D -m755 "${srcdir}/${pkgname}/usr/bin/queued" "${pkgdir}/usr/bin/queued"
  
  # service
  install -D -m644 "${srcdir}/${pkgname}/usr/lib/systemd/system/queued.service" \
                   "${pkgdir}/usr/lib/systemd/system/queued.service"
  install -D -m644 "${srcdir}/${pkgname}/etc/queued.conf" \
                   "${pkgdir}/etc/queued.conf"
}
