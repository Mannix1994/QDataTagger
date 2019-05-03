#-------------------------------------------------
#
# Project created by QtCreator 2019-04-30T20:54:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QDataTagger
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    paintwidget.cpp \
    imagelist.cpp \
    convert.cpp \
    cv.cpp

HEADERS += \
        mainwindow.h \
    paintwidget.h \
    imagelist.h \
    convert.h \
    cv.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += /usr/local/opencv343/include/

#LIBS+=/usr/local/opencv343/lib/libopencv_features2d.so
#LIBS+=/usr/local/opencv343/lib/libopencv_ml.so
#LIBS+=/usr/local/opencv343/lib/libopencv_dpm.so
#LIBS+=/usr/local/opencv343/lib/libopencv_bgsegm.so
#LIBS+=/usr/local/opencv343/lib/libopencv_superres.so
#LIBS+=/usr/local/opencv343/lib/libopencv_freetype.so
#LIBS+=/usr/local/opencv343/lib/libopencv_hfs.so
#LIBS+=/usr/local/opencv343/lib/libopencv_plot.so
#LIBS+=/usr/local/opencv343/lib/libopencv_objdetect.so
#LIBS+=/usr/local/opencv343/lib/libopencv_stereo.so
#LIBS+=/usr/local/opencv343/lib/libopencv_face.so
#LIBS+=/usr/local/opencv343/lib/libopencv_xphoto.so
#LIBS+=/usr/local/opencv343/lib/libopencv_datasets.so
#LIBS+=/usr/local/opencv343/lib/libopencv_text.so
#LIBS+=/usr/local/opencv343/lib/libopencv_xfeatures2d.so
#LIBS+=/usr/local/opencv343/lib/libopencv_flann.so
#LIBS+=/usr/local/opencv343/lib/libopencv_img_hash.so
#LIBS+=/usr/local/opencv343/lib/libopencv_shape.so
#LIBS+=/usr/local/opencv343/lib/libopencv_xobjdetect.so
#LIBS+=/usr/local/opencv343/lib/libopencv_reg.so
#LIBS+=/usr/local/opencv343/lib/libopencv_rgbd.so
#LIBS+=/usr/local/opencv343/lib/libopencv_structured_light.so
#LIBS+=/usr/local/opencv343/lib/libopencv_calib3d.so
LIBS+=/usr/local/opencv343/lib/libopencv_imgcodecs.so
#LIBS+=/usr/local/opencv343/lib/libopencv_aruco.so
LIBS+=/usr/local/opencv343/lib/libopencv_highgui.so
LIBS+=/usr/local/opencv343/lib/libopencv_core.so
#LIBS+=/usr/local/opencv343/lib/libopencv_surface_matching.so
#LIBS+=/usr/local/opencv343/lib/libopencv_ximgproc.so
#LIBS+=/usr/local/opencv343/lib/libopencv_stitching.so
#LIBS+=/usr/local/opencv343/lib/libopencv_phase_unwrapping.so
#LIBS+=/usr/local/opencv343/lib/libopencv_videostab.so
LIBS+=/usr/local/opencv343/lib/libopencv_imgproc.so
#LIBS+=/usr/local/opencv343/lib/libopencv_dnn_objdetect.so
#LIBS+=/usr/local/opencv343/lib/libopencv_saliency.so
#LIBS+=/usr/local/opencv343/lib/libopencv_videoio.so
#LIBS+=/usr/local/opencv343/lib/libopencv_sfm.so
#LIBS+=/usr/local/opencv343/lib/libopencv_photo.so
#LIBS+=/usr/local/opencv343/lib/libopencv_bioinspired.so
#LIBS+=/usr/local/opencv343/lib/libopencv_video.so
#LIBS+=/usr/local/opencv343/lib/libopencv_optflow.so
#LIBS+=/usr/local/opencv343/lib/libopencv_tracking.so
#LIBS+=/usr/local/opencv343/lib/libopencv_fuzzy.so
#LIBS+=/usr/local/opencv343/lib/libopencv_dnn.so
#LIBS+=/usr/local/opencv343/lib/libopencv_ccalib.so
#LIBS+=/usr/local/opencv343/lib/libopencv_line_descriptor.so
#LIBS+=/usr/local/opencv343/lib/libopencv_hdf.so
