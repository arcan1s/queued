# Maintainer: Evgeniy "arcanis" Alexeev <esalexeev@gmail.com>
# Author: Evgeniy "arcanis" Alexeev <esalexeev@gmail.com>

pkgname=queued
pkgver=1.2.0
pkgrel=1
pkgdesc="Daemon for starting jobs to queue of calculations"
arch=('any')
url="https://github.com/arcan1s/queued"
license=("GPLv3")
depends=('bash')
source=(https://github.com/arcan1s/queued/releases/download/V.${pkgver}/${pkgname}-${pkgver}-src.tar.xz)
md5sums=('a5e1af503128283cda85ca21d08b6853')
backup=('etc/queued.conf')

package()
{
  "${srcdir}/${pkgname}/install.sh" "${pkgdir}"
}
