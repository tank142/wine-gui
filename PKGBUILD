pkgname=wine-gui
pkgver=0.13alpha
pkgrel=1
pkgdesc="GUI for managing Windows programs under linux"
arch=('x86_64')
url="https://github.com/tank142/wine-gui"
license=('GPL3')
depends=('qt6-base' 'winetricks' 'icoutils' 'imagemagick')
makedepends=('git')
source=('wine-gui::git+https://github.com/tank142/wine-gui.git#branch=qt6')
sha256sums=('SKIP')

build() {
  rm -rf "$srcdir/wine-gui/src/build"
  mkdir "$srcdir/wine-gui/src/build"
  cd "$srcdir/wine-gui/src/build"
  qmake-qt6 ..
  make
}

package() {
  cd "$srcdir/wine-gui/src/build"
  make -C ./ INSTALL_ROOT="$pkgdir/" install
}
