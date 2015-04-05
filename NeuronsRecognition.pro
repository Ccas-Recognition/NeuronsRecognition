#
#-------------------------------------------------
# Project created by QtCreator 2012-04-04T00:14:59
#
#-------------------------------------------------

QT       += gui declarative sql
TARGET = NeuronsRecognition
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    myImage.cpp \
    preprocessing.cpp \
    neuron_recognition.cpp \
    cluster.cpp \
    gray_co_mat.cpp \
    hog_classifier.cpp \
    ann_classifier.cpp \
    convex_hull.cpp \
    BoostingClassifier.cpp \
    ClassifierCascade.cpp \
    DecisionStump.cpp \
    LabeledSampleElement.cpp \
    RayFeaturesEnumerator.cpp \
    RayFeaturesEvaluator.cpp \
    RayFeaturesPrecomputer.cpp \
    RayFeatureType.cpp \
    RayFeatureValues.cpp \
    TrainingSampleElement.cpp \
    ExcelFormat_src/BasicExcel.cpp \
    ExcelFormat_src/ExcelFormat.cpp \
    fileselector.cpp

HEADERS  += mainwindow.h \
    myImage.h \
    neuron_recognition.h \
    gray_co_mat.h \
    classifier.h \
    BoostingClassifier.h \
    ClassifierCascade.h \
    DecisionStump.h \
    LabeledSampleElement.h \
    RayFeaturesEnumerator.h \
    RayFeaturesEvaluator.h \
    RayFeaturesPrecomputer.h \
    RayFeatureType.h \
    RayFeatureValues.h \
    TrainingSampleElement.h \
    ExcelFormat_src/BasicExcel.hpp \
    ExcelFormat_src/ExcelFormat.h \
    fileselector.h \
    DetectionData.h

FORMS    += mainwindow.ui \
    fileselector.ui

RESOURCES += \
    sources.qrc

# configuration for mac os x
macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    QMAKE_CXXFLAGS+=-std=c++11
    QMAKE_CXXFLAGS+=-stdlib=libc++
    QMAKE_LFLAGS+=-std=c++11
    QMAKE_LFLAGS+=-stdlib=libc++

    INCLUDEPATH += /usr/local/include

    LIBS += -L/usr/local/lib \
            -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_objdetect \
            -lopencv_ml
}

#configuration for windows
win32 {
    INCLUDEPATH += $(OPENCV_DIR)/../../include/ \
                   $(OPENCV_DIR)/../../include/opencv \
                   $(OPENCV_DIR)/../../include/opencv2

    CONFIG(release, debug|release) {
    message("release compilation")
    LIBS += $(OPENCV_DIR)/lib/opencv_core249.lib \
            $(OPENCV_DIR)/lib/opencv_highgui249.lib \
            $(OPENCV_DIR)/lib/opencv_imgproc249.lib \
            $(OPENCV_DIR)/lib/opencv_objdetect249.lib \
            $(OPENCV_DIR)/lib/opencv_ml249.lib \
            $(OPENCV_DIR)/lib/opencv_features2d249.lib \
            $(OPENCV_DIR)/lib/opencv_calib3d249.lib \
            $(OPENCV_DIR)/lib/opencv_contrib249.lib \
            $(OPENCV_DIR)/lib/opencv_flann249.lib \
            $(OPENCV_DIR)/lib/opencv_gpu249.lib \
            $(OPENCV_DIR)/lib/opencv_legacy249.lib \
            $(OPENCV_DIR)/lib/opencv_nonfree249.lib \
            $(OPENCV_DIR)/lib/opencv_ocl249.lib \
            $(OPENCV_DIR)/lib/opencv_photo249.lib \
            $(OPENCV_DIR)/lib/opencv_stitching249.lib \
            $(OPENCV_DIR)/lib/opencv_superres249.lib \
            $(OPENCV_DIR)/lib/opencv_ts249.lib \
            $(OPENCV_DIR)/lib/opencv_video249.lib \
            $(OPENCV_DIR)/lib/opencv_videostab249.lib
    } else {
    message("debug compilation")
    LIBS += $(OPENCV_DIR)/lib/opencv_core249d.lib \
            $(OPENCV_DIR)/lib/opencv_highgui249d.lib \
            $(OPENCV_DIR)/lib/opencv_imgproc249d.lib \
            $(OPENCV_DIR)/lib/opencv_objdetect249d.lib \
            $(OPENCV_DIR)/lib/opencv_ml249d.lib \
            $(OPENCV_DIR)/lib/opencv_features2d249d.lib \
            $(OPENCV_DIR)/lib/opencv_calib3d249d.lib \
            $(OPENCV_DIR)/lib/opencv_contrib249d.lib \
            $(OPENCV_DIR)/lib/opencv_flann249d.lib \
            $(OPENCV_DIR)/lib/opencv_gpu249d.lib \
            $(OPENCV_DIR)/lib/opencv_legacy249d.lib \
            $(OPENCV_DIR)/lib/opencv_nonfree249d.lib \
            $(OPENCV_DIR)/lib/opencv_ocl249d.lib \
            $(OPENCV_DIR)/lib/opencv_photo249d.lib \
            $(OPENCV_DIR)/lib/opencv_stitching249d.lib \
            $(OPENCV_DIR)/lib/opencv_superres249d.lib \
            $(OPENCV_DIR)/lib/opencv_ts249d.lib \
            $(OPENCV_DIR)/lib/opencv_video249d.lib \
            $(OPENCV_DIR)/lib/opencv_videostab249d.lib
    }

    CONFIG += link_pkgconfig console
}
