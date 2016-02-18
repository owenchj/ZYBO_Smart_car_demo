TEMPLATE = app
TARGET = my_qt_camera
QT += core \
    gui\
    network
HEADERS += processImage.h \
    videodevice.h
SOURCES += processImage.cpp \
    videodevice.cpp \
    main.cpp
FORMS += 
RESOURCES += \
    icons/images/icons.qrc


QT += widgets
QT += core gui
