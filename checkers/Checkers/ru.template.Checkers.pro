TARGET = ru.template.Checkers

CONFIG += \
    auroraapp

PKGCONFIG += \

SOURCES += \
    src/main.cpp \
    src/CheckersModel.cpp

HEADERS += \
    src/CheckersModel.h

DISTFILES += \
    qml/pages/CheckersModeSelection.qml \
    rpm/ru.template.Checkers.spec

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.template.Checkers.ts \
    translations/ru.template.Checkers-ru.ts
