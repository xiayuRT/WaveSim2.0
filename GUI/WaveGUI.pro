QT += quick
QT += widgets
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../common/src/Jonswap.cpp \
        ../common/src/basemotor.cpp \
        ../common/src/cartthread.cpp \
        ../common/src/cartmotor.cpp \
        ../GUI/src/main.cpp \
        ../GUI/src/maincontrol.cpp \
        ../common/src/spi.cpp

HEADERS += \
        ../common/inc/Jonswap.hpp \
        ../common/inc/basemotor.hpp \
        ../common/inc/cartmotor.hpp \
        ../common/inc/cartthread.hpp \
        ../GUI/inc/maincontrol.hpp \
        ../common/inc/spi.hpp



RESOURCES += resource/qml.qrc

LIBS += -lsFoundation20 -lpthread -lpigpio

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += \
        ../external/inc-pub \
        ../common/inc \
        /inc

