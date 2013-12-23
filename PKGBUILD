# Maintainer: Evgeniy "arcanis" Alexeev <esalexeev@gmail.com>
# Author: Evgeniy "arcanis" Alexeev <esalexeev@gmail.com>

pkgname=queued
pkgver=1.1.0
pkgrel=1
pkgdesc="Daemon for starting jobs to queue of calculations"
arch=('any')
url="https://github.com/arcan1s/queued"
license=("GPLv3")
depends=('bash')
source=(https://github.com/arcan1s/queued/releases/download/V.${pkgver}/${pkgname}-${pkgver}-src.tar.xz)
md5sums=('0832ae610567e0aed4dccea02408e5a3')
backup=('etc/queued.conf')

package()
{
  "${srcdir}/${pkgname}/install.sh" "${pkgdir}"
}
