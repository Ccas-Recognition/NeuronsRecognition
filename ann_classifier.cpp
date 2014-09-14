#include "classifier.h"

#include <fstream>
#include <iostream>

ann_classifier :: ann_classifier()
{
    neuro_net = new CvANN_MLP();

    CvMat *inputLayers = cvCreateMat( 4, 1, CV_32SC1 );

    //cvGetRows(inputlLayers, &neuralLayers1, 0, 4);

    cvSet1D(inputLayers, 0, cvScalar(5));
    cvSet1D(inputLayers, 1, cvScalar(7));
    cvSet1D(inputLayers, 2, cvScalar(5));
    cvSet1D(inputLayers, 3, cvScalar(1));

    neuro_net->create( inputLayers );
}

ann_classifier :: ann_classifier(const char *file_name)
{
    neuro_net = new CvANN_MLP();
    neuro_net->load( file_name );
}

ann_classifier :: ann_classifier( CvMat *inputLayers )
    : classifier()
{
    neuro_net = new CvANN_MLP();
    neuro_net->create( inputLayers );
}

ann_classifier :: ann_classifier( const ann_classifier& orig )
{
}

ann_classifier :: ~ann_classifier()
{
    //delete neuro_net; //DEBUG !!!
}

int ann_classifier::get_train_data( const char *train_data, CvMat* &inputs, CvMat* &outputs )
{
    int num_features = 0, num_samples = 0;
    double cur_val = 0.0;
    vector< double > tmp_storage;
    ifstream InpF( train_data );
    InpF >> num_features;

    while( !InpF.eof() ) {
        InpF >> cur_val;
        tmp_storage.push_back( cur_val );
    }
    tmp_storage.pop_back();
    InpF.close();

    num_samples = ( int ) tmp_storage.size() / num_features;

    inputs = cvCreateMat( num_samples, num_features - 1, CV_32FC1 );
    outputs = cvCreateMat( num_samples, 1, CV_32FC1 );

    int k = 0;
    for( int i = 0; i < num_samples; ++i ) {
        float *inp_ptr = ( float *)(inputs->data.ptr + i * inputs->step );

        for( int j = 0; j < num_features - 1; ++j ) {
            inp_ptr[ j ] = tmp_storage[ k++ ];
        }

        cvSet1D( outputs, i, cvScalar( tmp_storage[ k++ ]));
    }

    tmp_storage.clear();

    return num_samples;
}

void ann_classifier :: train( const char *train_data )
{
    CvMat *inputs;
    CvMat *outputs;
    int num_samples = get_train_data( train_data, inputs, outputs );
    CvMat* sampleWts = cvCreateMat( num_samples, 1, CV_32FC1 );
    for( int i = 0; i < num_samples; ++i ) {
        cvSet1D( sampleWts, i, cvScalar( 1 ));
    }

    int iter = neuro_net->train( inputs, outputs, sampleWts, 0,
                                CvANN_MLP_TrainParams( cvTermCriteria(
                                                        CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,
                                                        100000,
                                                        0.000012 ),
                                                        CvANN_MLP_TrainParams :: BACKPROP, 0.01, 0.05 ) );
    neuro_net->save("ann_model");
    cout << iter << endl;
}

float ann_classifier :: predict_row( CvMat *row, CvMat* &answ )
{
    neuro_net->predict( row, answ );
    return 0.0;
}

void ann_classifier :: predict_rows( Mat& proc_image, vector< QRect >& rectangles, vector< float >& answ )
{
    Mat tmp_image;
    unsigned int len = rectangles.size();
    int x      = 0,
        y      = 0,
        width  = 0,
        height = 0;

    int **image = new int*[ WIN_SIZE ];
    for( int i = 0; i < WIN_SIZE; ++i ) {
        image[ i ] = new int[ WIN_SIZE ];
    }
    CvMat *inputs = cvCreateMat( len, gray_co_mat :: NUM_FEATS, CV_32FC1 );
    CvMat *outputs = cvCreateMat( len, 1, CV_32FC1 );


    int offset[ 2 ] = { 1, 0 };
    gray_co_mat *main_mat = new gray_co_mat( offset, 16, true );
    int k = 0;

    for( unsigned int i = 0; i < len; ++i ) {
        rectangles[ i ].getRect( &x, &y, &width, &height);
        if ( x >= 0 && y >= 0 && x + width < proc_image.cols && y + height < proc_image.rows ) {
            tmp_image = Mat( proc_image, Rect( x, y, width, height ));

            if ( tmp_image.cols != WIN_SIZE && tmp_image.rows != WIN_SIZE ) {
                resize( tmp_image, tmp_image, Size( WIN_SIZE, WIN_SIZE ));
            }

            for( unsigned int j = 0; j < WIN_SIZE; ++j ) {
                uchar *tmp_image_data = tmp_image.ptr( j );
                for( unsigned int i = 0; i < WIN_SIZE; ++i ) {
                    image[ i ][ j ] = ( int )tmp_image_data[ i ];
                }
            }

            main_mat->load_image( WIN_SIZE, WIN_SIZE, image );
            gray_props *stat_props = main_mat->culc_matrix();
            inputs->data.fl[ k++ ] = stat_props->contrast;
            inputs->data.fl[ k++ ] = stat_props->correlation;
            inputs->data.fl[ k++ ] = stat_props->energy;
            inputs->data.fl[ k++ ] = stat_props->entropy;
            inputs->data.fl[ k++ ] = stat_props->homogeneity;
            delete stat_props;
        }
    }

    neuro_net->predict( inputs, outputs );
    for( unsigned int i = 0; i < len; ++i ) {
        answ.push_back( outputs->data.fl[ i ] );
    }

    cvReleaseMat( &inputs );
    cvReleaseMat( &outputs );
    tmp_image.release();

    for( int i = 0; i < WIN_SIZE; ++i )
        delete[] image[ i ];

    delete[] image;
}

























