#ifndef SLIDINGWINDOW_H
#define SLIDINGWINDOW_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sstream>
namespace ImageRecognition
{

using namespace std;
using namespace cv;

template<typename TPixel>
struct ResponsePlaceholder
{
    float operator()( const Mat& part ) const
    {
        GaussianBlur( part, part, Size(3,3), 0, 0, BORDER_DEFAULT );

        /// Convert it to gray
        //cvtColor( part, part, CV_RGB2GRAY );


        /// Generate grad_x and grad_y
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        Mat grad;

        int scale = 1;
        int delta = 0;

        /// Gradient X
        //Scharr( part, grad_x, CV_16S, 1, 0, scale, delta, BORDER_DEFAULT );
        Sobel( part, grad_x, CV_16S, 1, 0, 3, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( grad_x, abs_grad_x );

        /// Gradient Y
        //Scharr( part, grad_y, CV_16S, 0, 1, scale, delta, BORDER_DEFAULT );
        Sobel( part, grad_y, CV_16S, 0, 1, 3, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( grad_y, abs_grad_y );

        /// Total Gradient (approximate)
        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
        //qDebug() << grad.type() << endl;
        double res = 0.0;
        for(int y=0; y<part.rows; ++y)
        {
            for(int x=0; x<part.cols; ++x)
            {
                res += abs(grad.at<uchar>(y, x));
            }
        }
        res /= part.rows*part.cols;

#if 0
        static int image_count = 1;
        stringstream ss1, ss2;
        ss1 << "dump/part_" << image_count << "_" << res <<  ".png";
        ss2 << "dump/part_" << image_count << "_" << res <<  "_.png";
        imwrite( ss1.str(), grad );
        imwrite( ss2.str(), part );
        ++image_count;
#endif
        return res;
    }
};

class SlidingWindowFragmentation
{
    int standartWindowSize;
    int standartWindowOffset;
    Size standartWindow;
    vector<float> scales;

    Mat baseImage;

    static std::string int2str( int n )
    {
        std::stringstream ss;
        ss << n;
        return ss.str();
    }

public:
    SlidingWindowFragmentation( Mat _image, unsigned int _standartWindowSize,
                                unsigned int _standartWindowOffset, const unsigned int sizes[] )
    {
        standartWindowSize = _standartWindowSize;
        standartWindowOffset = _standartWindowOffset;

        standartWindow = Size( standartWindowSize, standartWindowSize );

        float ratio1 = standartWindowSize/float(sizes[0]);
        int h = int(_image.rows*ratio1);
        int w = int(_image.cols*ratio1);
        baseImage = Mat( h, w, _image.type() );
        resize(_image, baseImage, baseImage.size() );

        int sizes_count = sizeof(sizes)/sizeof(unsigned int);
        scales.resize( sizes_count );
        scales[0] = 1.0f;
        for( int i=1; i<sizes_count; ++i )
            scales[i] = float(sizes[i]*ratio1)/sizes[i-1];
    }

    template<class ResponseFunctor>
    void FindMaximaWindows( vector< Rect_<int> > &maxima, int nmsLength, const ResponseFunctor *pResponce)
    {

    }

    template<class ResponseFunctor>
    Mat MakeResponseImage( unsigned int scale_index, const ResponseFunctor *pResponce) const
    {
        if(scale_index >= scales.size())
            throw "Index Out of Range";

        int w = int(baseImage.cols*scales[ scale_index ]);
        int h = int(baseImage.rows*scales[ scale_index ]);
        Mat image = Mat( h, w, baseImage.type() );
        resize( baseImage, image, image.size() );

        int response_width = w/standartWindowOffset - standartWindowSize/standartWindowOffset - 1;
        int response_height = h/standartWindowOffset - standartWindowSize/standartWindowOffset - 1;
        Mat responseImage = Mat( response_height, response_width, CV_32FC1 );
        float maxResponse = 0.0f;
        for( int i=0; i<responseImage.rows; ++i )
        {
            int y = i*standartWindowOffset;

            for( int j=0; j<responseImage.cols; ++j )
            {
                int x = j*standartWindowOffset;
                Mat part = image(Range(y, y + standartWindowSize), Range(x, x + standartWindowSize));
                //qDebug() << part.rows << " " << part.cols << endl;
#if 0
                string filename = "dump/part" + int2str(i) + "_" + int2str(j) + ".png";
                imwrite(filename, part);
#endif
                responseImage.at<float>(i, j) = (*pResponce)(part);
                maxResponse = max(maxResponse, responseImage.at<float>(i, j));
            }
        }
        responseImage /= maxResponse;
        responseImage *= 255;

        Mat responseBorderedImage;
        copyMakeBorder( responseImage, responseBorderedImage,
                        standartWindowSize/2, standartWindowSize/2, standartWindowSize/2, standartWindowSize/2,
                        BORDER_CONSTANT, Scalar ( 0, 0, 0 ) );

        return responseBorderedImage;
    }
};

}

#endif // SLIDINGWINDOW_H
