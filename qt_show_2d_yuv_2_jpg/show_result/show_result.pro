QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mypainter.cpp

HEADERS += \
    mainwindow.h \
    mypainter.h


FORMS += \
    mainwindow.ui

#INCLUDEPATH +=$$PWD/FFmpeg32/dev/include/

#LIBS += -L$$PWD/FFmpeg32/dev/lib  -lavcodec \

#LIBS += -L$$PWD/FFmpeg32/dev/lib -llibavdevice\

#LIBS += -L$$PWD/FFmpeg32/dev/lib -llibavfilter \

#LIBS += -L$$PWD/FFmpeg32/dev/lib -llibavformat \

#LIBS += -L$$PWD/FFmpeg32/dev/lib -llibpostproc \

#LIBS += -L$$PWD/FFmpeg32/dev/lib -llibswscale \

#LIBS += -L$$PWD/FFmpeg32/dev/lib -llibavutil \

#LIBS += -L$$PWD/FFmpeg32/dev/lib -llibswresample \

INCLUDEPATH += D:\opencv4\include
INCLUDEPATH += D:\opencv4\include\opencv
INCLUDEPATH += D:\opencv4\include\opencv2

LIBS += D:\opencv4\x86\mingw\lib\libopencv_calib3d400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_core400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_dnn400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_features2d400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_flann400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_highgui400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_imgcodecs400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_imgproc400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_ml400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_objdetect400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_photo400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_shape400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_stitching400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_superres400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_video400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_videoio400.dll.a
LIBS += D:\opencv4\x86\mingw\lib\libopencv_videostab400.dll.a




TRANSLATIONS += \
    show_result_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
