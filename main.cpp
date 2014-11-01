#include <QtGui/QApplication>
#include "mainwindow.h"
#include "fileselector.h"
#include "SlidingWindow.h"
/**
 * Входная точка в программу.
 */
int main(int argc, char *argv[])
{
#if WIN32
    system("del /Q dump");
#endif
    int start_x = 150;
    int start_y = 100;

    Mat image = imread( "ImageExamples/Tv244_part.jpg", 0 );
    unsigned int _sizes[] = {24, 36, 48, 60};
    vector< unsigned int > sizes( sizeof(_sizes)/sizeof(unsigned int) );
    copy(_sizes, _sizes + (sizeof(_sizes)/sizeof(unsigned int)), sizes.begin());

    ImageRecognition::SlidingWindowFragmentation fragmentation( image, 24, 4, sizes );

    ImageRecognition::ResponsePlaceholder<uchar> response;
    vector< Rect_<int> > rects;
    fragmentation.FindMaximaWindows( rects, response );

    Mat output_image = imread( "ImageExamples/Tv244_part.jpg" );
    for(int i=0; i<rects.size(); ++i)
    {
        rectangle(output_image, rects[i], Scalar(0, 0, 255), 1);
    }
    imwrite( "dump/rects.jpg" ,output_image );

    return 0;
    QApplication a(argc, argv);
    MainWindow w;
    w.move(QPoint(start_x, start_y));
    w.show();

    FileSelector f;
    f.setMainWindow(&w);
    f.move(QPoint(start_x + w.width() + 30, start_y));
    f.show();
    
    return a.exec();
}
