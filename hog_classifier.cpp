#include "classifier.h"
#include <fstream>

hog_classifier :: hog_classifier()
    : classifier()
{
    descr = new HOGDescriptor( Size( 32, 32 ), Size( 16, 16 ), Size( 16, 16 ), Size( 8, 8 ), 9 );

    vector< float > coeffs;
    float tmp_val = 0.0;
    ifstream inpF( "..\\Examples\\coeffs" );

    inpF >> threshold;
    while( !inpF.eof() ) {
        inpF >> tmp_val;
        coeffs.push_back( tmp_val );
    }

    descr->setSVMDetector( coeffs );

    inpF.close();
    coeffs.clear();
}

hog_classifier :: hog_classifier( const classifier &orig )
{

}

hog_classifier :: ~hog_classifier()
{
    delete descr;
}

bool hog_classifier :: detect_window( Mat& proc_image )
{
    vector< Point > loc;
    descr->detect( proc_image, loc, threshold, Size( 16, 16 ), Size( 0, 0 ));
    if ( loc.size() > 0 ) {
        return true;
    }
    else {
        return false;
    }
}
