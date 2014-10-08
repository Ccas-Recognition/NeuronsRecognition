#
#-------------------------------------------------
# Project created by QtCreator 2012-04-04T00:14:59
#
#-------------------------------------------------

QT       += core gui sql
TARGET = NeuronsRecognition
TEMPLATE = app

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
QMAKE_CXXFLAGS+=-std=c++11
QMAKE_CXXFLAGS+=-stdlib=libc++
QMAKE_LFLAGS+=-std=c++11
QMAKE_LFLAGS+=-stdlib=libc++


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

INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_objdetect \
        -lopencv_ml

