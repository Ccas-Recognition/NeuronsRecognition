#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include <vector>
#include <QRect>

#include "gray_co_mat.h"

using namespace std;
using namespace cv;
/*
 *Базовый класс для классификаторов.
 */
class classifier
{
public:
    classifier() {}
    classifier( const char *file_name ) {}
    classifier( const classifier& orig ) {}
    virtual ~classifier() {}

    virtual void set_params( const char *file_name ) {}
    virtual void set_params( vector< double>& coeffs ) {}

    virtual void train( const char *file_name ){}

    virtual bool detect_window( Mat& window ) { return true; }
    virtual float predict_row( CvMat *row, CvMat* &answ ) { return 0.0; }
    virtual void predict_rows( Mat& proc_image, vector< QRect >& rectangles, vector< float >& answ ) {}
protected:
    void get_train_data( const char *train_data ) {}
};

/**
 *Класс, реализующий классификацию на основе HOG-признаков.
 */
class hog_classifier : public classifier
{
public:
	//Конструктор класса.
    hog_classifier();
    hog_classifier( const classifier& orig );
    virtual ~hog_classifier();
	//Функция, реализующая классификацию квадратной области.
    bool detect_window( Mat& );
private:
    HOGDescriptor *descr;
    float threshold;
};
/**
 *Класс, реализующий классификацию на основе признаков, посчитанных с помощью матрицы смежности серого цвета.
 */
class ann_classifier : public classifier
{
public:
	//Конструктор класса.
    ann_classifier();
    ann_classifier( const char *file_name );
    ann_classifier( CvMat *inputLayers );
    ann_classifier( const ann_classifier& orig );
    virtual ~ann_classifier();
	//Функция, реализующая обучение классификатора.
    void train( const char *train_data );
	//Функция, реализующая классификацию квадратной области по вектору признаков.
    float predict_row( CvMat *row, CvMat* &answ );
	//Функция, реализующая классификацию квадратных областей по вектору признаков.
    void predict_rows( Mat& proc_image, vector< QRect >& rectangles, vector< float >& answ );
protected:
    int get_train_data( const char *train_data, CvMat* &inputs, CvMat* &outputs );
private:
    CvANN_MLP *neuro_net;
    static const int WIN_SIZE = 30;
};

#endif // CLASSIFIER_H
