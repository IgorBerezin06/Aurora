TARGET = ru.template.Chess

CONFIG += \
    auroraapp

PKGCONFIG += \

SOURCES += \
    src/boardmodel.cpp \
    src/main.cpp \

HEADERS += \
    src/boardmodel.h

DISTFILES += \
    rpm/ru.template.Chess.spec \

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.template.Chess.ts \
    translations/ru.template.Chess-ru.ts \

RESOURCES += \
    chess-resources.qrc
