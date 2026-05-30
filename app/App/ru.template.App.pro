TARGET = ru.template.App

CONFIG += \
    auroraapp

PKGCONFIG += \

SOURCES += \
    src/main.cpp \
    src/boardmodel.cpp \
    src/CheckersModel.cpp

HEADERS += \
    src/boardmodel.h \
    src/CheckersModel.h

DISTFILES += \
    rpm/ru.template.App.spec

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.template.App.ts \
    translations/ru.template.App-ru.ts
