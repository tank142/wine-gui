QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000   # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dirWidget.cpp \
    dircopy.cpp \
    dirwork.cpp \
    filedesktop.cpp \
    filedesktopwidget.cpp \
    fileregistry.cpp \
    fileshell.cpp \
    findlink.cpp \
    iconextract.cpp \
    main.cpp \
    mainwindow.cpp \
    openexe.cpp \
    prefixadd.cpp \
    prefixdel.cpp \
    prefixtab.cpp \
    prefixtable.cpp \
    settings.cpp \
    settings_storage_slots.cpp \
    shell.cpp \
    shelloutput.cpp \
    shortcutread.cpp \
    sizewin.cpp \
    storageread.cpp \
    tabdll.cpp \
    tabdxsettings.cpp \
    tablabels.cpp \
    tabsettings.cpp \
    tabsettingsregkey.cpp \
    tabtools.cpp

HEADERS += \
    dirWidget.h \
    dircopy.h \
    dirwork.h \
    filedesktop.h \
    filedesktopwidget.h \
    fileregistry.h \
    fileshell.h \
    findlink.h \
    iconextract.h \
    main_target.h \
    mainwindow.h \
    openexe.h \
    prefixadd.h \
    prefixdel.h \
    prefixtab.h \
    prefixtable.h \
    settings.h \
    settings_storage_slots.h \
    shell.h \
    shelloutput.h \
    shortcutread.h \
    sizewin.h \
    storageread.h \
    tabdll.h \
    tabdxsettings.h \
    tablabels.h \
    tabsettings.h \
    tabsettingsregkey.h \
    tabtools.h

FORMS +=

TRANSLATIONS += \
    wine-gui_ru_RU.ts wine-gui_en_US.ts

LIBS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    wine-gui_ru_RU.ts wine-gui_en_US.ts

RESOURCES += \
    icons/icons.qrc
DEPLOYMENTFOLDERS = icons
unix:!macx {
    TARGET = wine-gui
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
    target.path =  $$quote($$PREFIX/bin)
    desktop.files = icons/wine-gui.desktop
    desktop.path =  $$quote($$PREFIX/share/applications)
    icon_hdd.files = icons/hdd.png
    icon_hdd.path =  $$quote($$PREFIX/share/wine-gui/icons)
    icon_m2_ssd.files = icons/m2_ssd.png
    icon_m2_ssd.path =  $$quote($$PREFIX/share/wine-gui/icons)
    icon_pcie_ssd.files = icons/pcie_ssd.png
    icon_pcie_ssd.path =  $$quote($$PREFIX/share/wine-gui/icons)
    icon_raid.files = icons/raid.png
    icon_raid.path =  $$quote($$PREFIX/share/wine-gui/icons)
    icon_ssd.files = icons/ssd.png
    icon_ssd.path =  $$quote($$PREFIX/share/wine-gui/icons)
    icon_wine.files = icons/wine.svg
    icon_wine.path =  $$quote($$PREFIX/share/icons/hicolor/scalable/apps)

    wine-gui_ru_RU.files = wine-gui_ru_RU.qm
    wine-gui_ru_RU.path =  $$quote($$PREFIX/share/wine-gui/lang)
    wine-gui_en_US.files = wine-gui_en_US.qm
    wine-gui_en_US.path =  $$quote($$PREFIX/share/wine-gui/lang)
    INSTALLS += target desktop icon_hdd icon_m2_ssd icon_pcie_ssd icon_raid icon_ssd icon_wine \
    wine-gui_ru_RU wine-gui_en_US
}
