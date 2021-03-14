pkgname=wine-gui-qt5
pkgver=0.12alpha
pkgrel=1
pkgdesc="GUI for managing Windows programs under linux"
provides=(wine-gui)
conflicts=(wine-gui)
arch=('x86_64')
url="https://github.com/tank142/wine-gui"
license=('GPL3')
depends=('qt5-base' 'winetricks' 'icoutils' 'imagemagick')
makedepends=('git')
source=('wine-gui::git+https://github.com/tank142/wine-gui.git#branch=qt5')
sha256sums=('SKIP')

build() {
  mkdir "$srcdir/wine-gui/gui/build"
  cd "$srcdir/wine-gui/gui/build"
  qmake-qt5 ..
  make
}

package() {
  cd "$srcdir/wine-gui/gui/build"
  make -C ./ INSTALL_ROOT="$pkgdir/" install
}
