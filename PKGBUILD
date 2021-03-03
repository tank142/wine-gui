pkgname=wine-gui
pkgver=1
pkgrel=1
#pkgdesc=""
arch=('x86_64')
url="https://github.com/tank142/wine-gui"
license=('GPL3')
depends=('qt6-base' 'icoutils' 'imagemagick')
makedepends=('git')
source=('wine-gui::git+https://github.com/tank142/wine-gui.git')
sha256sums=('SKIP')

build() {
  cd "$srcdir/$pkgname"
  qmake
  make
}

package() {
  make -C "$pkgname" INSTALL_ROOT="$pkgdir/" install
}
