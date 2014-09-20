#include "myImage.h"

#include <QtGui/QImage>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QPixmap>
#include <QRect>
#include <QLine>
#include <QPoint>
#include <QVector>
#include <QRgb>
#include <QCursor>
#include <QDebug>
#include <cmath>
#include <fstream>

myImage :: myImage (QWidget *parent)
    : QWidget (parent), x (100), y (100), draw_flag (false), limit_flag (true), view_flag( true ),
      x_initial (0), y_initial (0), zoom_koeff (1.0), bot_x (0), bot_y (0), r (0), g (0), b (0)
{
    picture = new QImage ("bloody.bmp");

    main_image = new QImage (*picture);

    mask_image = new QImage (picture->width(), picture->height(), QImage :: Format_RGB32);
    for (int i = 0; i < picture->width(); ++i)
        for (int j = 0; j < picture->height(); ++j)
            mask_image->setPixel (i, j, qRgb (122, 122, 122));

    first_mid_x = mid_x = picture->width () / 2;
    first_mid_y = mid_y = picture->height () / 2;

    zoom_pict = *picture;

    view_mode_on = false;
    view_mode_drag = false;
}

void myImage::setDetectionData(DetectionData *data) {
    detectionData = data;

    delete picture;
    if (mask_image != NULL)
       delete mask_image;
    if (picture != main_image)
       delete main_image;

    picture = new QImage (data->filePath);
    main_image = new QImage (data->filePath);

    mask_image = new QImage (picture->width(), picture->height(), QImage :: Format_RGB32);
    for (int i = 0; i < picture->width(); ++i)
        for (int j = 0; j < picture->height(); ++j)
            mask_image->setPixel (i, j, qRgb (122, 122, 122));

    w = picture->width () * zoom_koeff;
    h = picture->height () * zoom_koeff;
    zoom_pict = picture->scaled (w, h);
    update();
}

QImage* myImage :: get_main_im()
{
    return main_image;
}

void myImage :: setDraw (bool flag)
{
    draw_flag = flag;
}

//________________________________Virtual functions________________________________//

void myImage :: paintEvent (QPaintEvent *paintEvent)
{

   QPainter painter (this);

   def_pict = zoom_pict.copy (bot_x, bot_y, x, y);

   painter.setPen(QColor (255, 0, 0));

    painter.drawImage (0, 0, def_pict);

    int old_size = detectionData->old_rectangles.size ();

	QRect tmp_rect;
	int x = 0, y = 0;
	int width = 0, height = 0;

	painter.setPen(QColor (0, 0, 255));

	for (int i = 0; i < old_size; ++i) {
        x = int ((detectionData->old_rectangles[i].x () * zoom_koeff - bot_x));
        y = int ((detectionData->old_rectangles[i].y () * zoom_koeff - bot_y));
        width = int (detectionData->old_rectangles[i].width() * zoom_koeff);
        height = int (detectionData->old_rectangles[i].height () * zoom_koeff);
		tmp_rect = QRect (x, y, width, height);

		painter.drawRect (tmp_rect);
    }

   if ( view_flag ) {
	   if (draw_flag && limit_flag) {

		   int pre_width = int (abs (x_last - x_initial));
		   int pre_height = int (abs (y_last - y_initial));
		   if ( pre_width > pre_height ) {
			   pre_height = pre_width;
		   }
		   else {
			   pre_width = pre_height;
		   }

		   QRect rect;
		   rect = QRect (x_initial, y_initial, pre_width, pre_height);
           painter.drawRect (rect);
	   }
       int size = detectionData->rectangles.size ();

	   QRect tmp_rect;
	   int x = 0, y = 0;
	   int width = 0, height = 0;

	  /* for (int i = 0; i < size; ++i) {
           if ( detectionData->colors[ i ] == 1 ) {
               x = int ((detectionData->rectangles[i].x () * zoom_koeff - bot_x));
               y = int ((detectionData->rectangles[i].y () * zoom_koeff - bot_y));
               width = int (detectionData->rectangles[i].width() * zoom_koeff);
               height = int (detectionData->rectangles[i].height () * zoom_koeff);
			   tmp_rect = QRect (x, y, width, height);

			   painter.drawRect (tmp_rect);
		   }
	   }*/

	   for (int i = 0; i < size; ++i) {
           if ( detectionData->colors[ i ] == 2 ) {
			   painter.setPen(QColor (0, 255, 0));
               x = int ((detectionData->rectangles[i].x () * zoom_koeff - bot_x));
               y = int ((detectionData->rectangles[i].y () * zoom_koeff - bot_y));
               width = int (detectionData->rectangles[i].width() * zoom_koeff);
               height = int (detectionData->rectangles[i].height () * zoom_koeff);
			   tmp_rect = QRect (x, y, width, height);

			   painter.drawRect (tmp_rect);
		   }
           else if ( detectionData->colors[ i ] == 1 ) {
			   painter.setPen(QColor (255, 0, 0));
               x = int ((detectionData->rectangles[i].x () * zoom_koeff - bot_x));
               y = int ((detectionData->rectangles[i].y () * zoom_koeff - bot_y));
               width = int (detectionData->rectangles[i].width() * zoom_koeff);
               height = int (detectionData->rectangles[i].height () * zoom_koeff);
			   tmp_rect = QRect (x, y, width, height);

			   painter.drawRect (tmp_rect);
		   }
           else if ( detectionData->colors[ i ] == 3 ) {
			   painter.setPen(QColor (0, 0, 255));
               x = int ((detectionData->rectangles[i].x () * zoom_koeff - bot_x));
               y = int ((detectionData->rectangles[i].y () * zoom_koeff - bot_y));
               width = int (detectionData->rectangles[i].width() * zoom_koeff);
               height = int (detectionData->rectangles[i].height () * zoom_koeff);
			   tmp_rect = QRect (x, y, width, height);

			   painter.drawRect (tmp_rect);
		   }
	   }

	  /* painter.setPen(QColor (255, 0, 0 ));

	   for (int i = 0; i < size; ++i) {
           if ( detectionData->colors[ i ] == 1 ) {
               x = int ((detectionData->rectangles[i].x () * zoom_koeff - bot_x));
               y = int ((detectionData->rectangles[i].y () * zoom_koeff - bot_y));
               width = int (detectionData->rectangles[i].width() * zoom_koeff);
               height = int (detectionData->rectangles[i].height () * zoom_koeff);
			   tmp_rect = QRect (x, y, width, height);

			   painter.drawRect (tmp_rect);

		   }
	   }*/

	   painter.setPen( QColor( 0, 0, 255 ));

	   unsigned int lines_size = lines.size();
	   QPoint beg_point, end_point;

	   for( unsigned int i = 0; i < lines_size; ++i ) {
		   if ( i < lines_size - 1 ) {
			   x = ( int ) ( lines[ i ].x() * zoom_koeff - bot_x );
			   y = ( int ) ( lines[ i ].y() * zoom_koeff - bot_y );
			   beg_point = QPoint( x, y );
			   x = ( int ) ( lines[ i + 1 ].x() * zoom_koeff - bot_x );
			   y = ( int ) ( lines[ i + 1 ].y() * zoom_koeff - bot_y );
			   end_point = QPoint( x, y );
			   painter.drawLine( beg_point, end_point );
		   }
		   else {
			   if ( !limit_flag ) {
				   x = ( int ) ( lines[ i ].x() * zoom_koeff - bot_x );
				   y = ( int ) ( lines[ i ].y() * zoom_koeff - bot_y );
				   beg_point = QPoint( x, y );
				   end_point = QPoint( x_last, y_last );
				   painter.drawLine( beg_point, end_point );
			   }
			   else {
				   x = ( int ) ( lines[ i ].x() * zoom_koeff - bot_x );
				   y = ( int ) ( lines[ i ].y() * zoom_koeff - bot_y );
				   beg_point = QPoint( x, y );
				   x = ( int ) ( lines[ 0 ].x() * zoom_koeff - bot_x );
				   y = ( int ) ( lines[ 0 ].y() * zoom_koeff - bot_y );
				   end_point = QPoint( x, y );
				   painter.drawLine( beg_point, end_point );
			   }
		   }
       }
   }
   else {
	   painter.setPen( QColor( 0, 255, 0 ));

       unsigned int size = detectionData->boundaries.size();
	   int x_beg = 0, y_beg = 0,
		   x_end = 0, y_end = 0;

	   for (int i = 0; i < size; ++i) {
           if ( detectionData->colors[ i ] == 2 ) {
               unsigned int bond_len = detectionData->boundaries[ i ].size();
			   for( unsigned int j = 0; j < bond_len; ++j ) {
                    x_beg = ( int ) (  detectionData->boundaries[ i ][ j ].x() * zoom_koeff - bot_x );
                    y_beg = ( int ) (  detectionData->boundaries[ i ][ j ].y() * zoom_koeff - bot_y );
                    x_end = ( int ) (  detectionData->boundaries[ i ][ ( j + 1 ) % bond_len ].x() * zoom_koeff - bot_x );
                    y_end = ( int ) (  detectionData->boundaries[ i ][ ( j + 1 ) % bond_len ].y() * zoom_koeff - bot_y );

					painter.drawLine( x_beg, y_beg, x_end, y_end );
			   }
		   }
	   }
	   this->update();
   }
}

void myImage :: SetViewMode(bool value)
{
    if(view_mode_on == value)
        return;
    //qDebug() << "SetViewMode " << value;

    if(value == true && draw_flag == true)
        mouseReleaseEvent(0);
    if(value == true)
        this->setCursor(QCursor(Qt::OpenHandCursor));
    else
        this->setCursor(QCursor(Qt::ArrowCursor));

    view_mode_on = value;
}

void myImage :: resizeEvent (QResizeEvent *event)
{
    x = this->width ();
    y = this->height ();

    bot_x = mid_x - (x / 2);
    bot_y = mid_y - (y / 2);
}

void myImage :: mouseDoubleClickEvent( QMouseEvent *mouseEvent )
{
    if(view_mode_on)
        return;

    int x_init = int ((x_initial + bot_x) / zoom_koeff);
    int y_init = int ((y_initial + bot_y) / zoom_koeff);

    del_rect (x_init, y_init);
}

void myImage :: SaveInitialCoordinates()
{
    initial_mid_x = mid_x;
    initial_mid_y = mid_y;
    initial_first_mid_x = first_mid_x;
    initial_first_mid_y = first_mid_y;
    initial_bot_x = bot_x;
    initial_bot_y = bot_y;

}

void myImage :: mousePressEvent (QMouseEvent* mouseEvent)
{

    x_initial = x_last = int (mouseEvent->x ());
    y_initial = y_last = int (mouseEvent->y ());

    int x_init = int ((x_initial + bot_x) / zoom_koeff);
    int y_init = int ((y_initial + bot_y) / zoom_koeff);

    if(view_mode_on)
    {
        view_mode_drag = true;

        SaveInitialCoordinates();
        this->setCursor(QCursor(Qt::ClosedHandCursor));

        return;
    }

    draw_flag = true;

    if (!limit_flag) {
        if( mouseEvent->button() == Qt :: LeftButton ) {
            lines.push_back( QPoint( x_init, y_init ));
        }
        else {
            limit_flag = !limit_flag;
            release_rectangles();
        }
    }

    this->update ();
}

void myImage :: wheelEvent(QWheelEvent* event)
{
    zoom( (event->delta() > 0)? 1.2f:(1.0f/1.2f), float(event->x() ), float(event->y() ) );
}

void myImage :: mouseReleaseEvent (QMouseEvent *mouseEvent)
{
    if(view_mode_on)
    {
        view_mode_drag = false;
        this->setCursor(QCursor(Qt::OpenHandCursor));

        return;
    }

    if(draw_flag == false)
        return;
    draw_flag = false;

    if (limit_flag) {

        QRect rect;
        int x_init = int ((x_initial + bot_x) / zoom_koeff);
        int y_init = int ((y_initial + bot_y) / zoom_koeff);
        int pre_width = int (abs (x_last - x_initial) / zoom_koeff);
        int pre_height = int (abs (y_last - y_initial) / zoom_koeff);
		if ( pre_width > pre_height ) {
			pre_height = pre_width;
		}
		else {
			pre_width = pre_height;
		}

        rect = QRect (x_init, y_init, pre_width, pre_height);

        if ((pre_width == 0 || pre_height == 0) && ( mouseEvent != 0 ) && ( mouseEvent->button() == Qt :: RightButton )) {
			del_rect (x_init, y_init);
        }
        else if((pre_width == 0 || pre_height == 0) && ( mouseEvent != 0 ) && ( mouseEvent->button() == Qt :: LeftButton )) {
			change_color( x_init, y_init );
		}
        else {
            detectionData->colors.push_back( 3 );
            detectionData->rectangles.push_back (rect);
        }
    }

    this->update ();
}

void myImage :: mouseMoveEvent (QMouseEvent *mouseEvent)
{
    x_last = mouseEvent->x ();
    y_last = mouseEvent->y ();

    if(view_mode_drag)
    {
        float step_x = float(-x_last + x_initial);
        float step_y = float(-y_last + y_initial);

        mid_y = initial_mid_y + step_y;
        bot_y = initial_bot_y + step_y;
        first_mid_y = initial_first_mid_y + step_y / zoom_koeff;

        mid_x = initial_mid_x + step_x;
        bot_x = initial_bot_x + step_x;
        first_mid_x = initial_first_mid_x + step_x / zoom_koeff;
    }

    if (!limit_flag) {

        /*if (draw_flag) {
            int x_init = int ((x_last + bot_x) / zoom_koeff);
            int y_init = int ((y_last + bot_y) / zoom_koeff);

            for (int i = x_init - 1; i < x_init + 1; ++i)
                for (int j = y_init - 1; j < y_init + 1; ++j) {
                    picture->setPixel(i, j, qRgb(r, g, b));
                    mask_image->setPixel(i, j, qRgb(r, g, b));
                }

            int pen_step = int (1 * zoom_koeff);
            for (int i = x_last + bot_x - pen_step; i < x_last + bot_x + pen_step; ++i)
                for (int j = y_last + bot_y - pen_step; j < y_last + bot_y + pen_step; ++j)
                    zoom_pict.setPixel(i, j, qRgb(r, g, b));
        }*/
    }
    this->update ();
}

double myImage :: vect_prod( QPoint p1, QPoint p2, QPoint cur_p )
{
    return (( p2.x() - p1.x()) * ( cur_p.y() - p1.y()) - ( p2.y() - p1.y()) * ( cur_p.x() - p1.x()));
}

void myImage :: release_rectangles()
{
    unsigned int lines_size = lines.size();

    for( unsigned int i = 0; i < detectionData->rectangles.size(); ++i ) {
        int cur_x = ( detectionData->rectangles[ i ].x() + detectionData->rectangles[ i ].right()) / 2,
            cur_y = ( detectionData->rectangles[ i ].y() + detectionData->rectangles[ i ].bottom()) / 2;
        QPoint cur_p = QPoint( cur_x, cur_y );
        vector< QLine > tmp_lines;

        for( unsigned int j = 0; j < lines_size; ++j ) {
            if ( j < lines_size - 1  ) {
                if((cur_x >= lines[ j ].x() && cur_x <= lines[ j + 1 ].x()) || (cur_x <= lines[ j ].x() && cur_x >= lines[ j + 1 ].x())) {
                    tmp_lines.push_back( QLine( lines[ j ], lines[ j + 1 ]));
                }
            }
            else {
                if(( cur_x >= lines[ j ].x() && cur_x <= lines[ 0 ].x()) || ( cur_x <= lines[ j ].x() && cur_x >= lines[ 0 ].x())) {
                    tmp_lines.push_back( QLine( lines[ j ], lines[ 0 ]));
                }
            }
        }

        int n = 0,
            m = 0;
        int pre_y = 0;

        for( unsigned int k = 0; k < tmp_lines.size(); ++k ) {
            double koeff = ( tmp_lines[ k ].dy() * 1.0 / tmp_lines[ k ].dx() );
            double b =  tmp_lines[ k ].y2() * 1.0 - tmp_lines[ k ].x2() * koeff;
            int tmp_y =  ( int )( koeff * cur_x + b );
            if( tmp_y < cur_y && tmp_y != pre_y ) {
                //if ( vect_prod( tmp_lines[ k ].p1(), tmp_lines[ k ].p2(), cur_p ) * vect_prod( tmp_lines[ k ].p1(), tmp_lines[ k ].p2(), QPoint( cur_x, -10000 )) < 0 ) {
                    ++n;
                //}
            }
            else if ( tmp_y > cur_y && tmp_y != pre_y ) {
                ++m;
            }
            else if ( tmp_y == cur_y ) {
                n = 1;
                m = 1;
                break;
            }
            pre_y = tmp_y;
        }
        if ( n == 0 || m  == 0 ) {
            detectionData->rectangles.erase( detectionData->rectangles.begin() + i );
            detectionData->colors.erase( detectionData->colors.begin() + i );
            --i;
        }
        tmp_lines.clear();
    }
}

vector< QPoint >& myImage :: get_lines()
{
    return lines;
}

void myImage :: del_rect (int x, int y)
{
    int len = detectionData->rectangles.size ();

    for (int i = 0; i < len; i++) {

        if (x >= detectionData->rectangles[i].x () &&  x <= detectionData->rectangles[i].x () + detectionData->rectangles[i].width ())
            if (y >= detectionData->rectangles[i].y () &&  y <= detectionData->rectangles[i].y () + detectionData->rectangles[i].height ()) {
                detectionData->rectangles.erase (detectionData->rectangles.begin () + i);
                detectionData->colors.erase ( detectionData->colors.begin () + i );
                break;
            }
    }
}

void myImage :: change_view()
{
	view_flag = !view_flag;
}

void myImage :: change_color( int x, int y )
{
    int len = detectionData->rectangles.size ();

    for (int i = 0; i < len; i++) {

        if (x >= detectionData->rectangles[i].x () &&  x <= detectionData->rectangles[i].x () + detectionData->rectangles[i].width ()) {
             if (y >= detectionData->rectangles[i].y () &&  y <= detectionData->rectangles[i].y () + detectionData->rectangles[i].height ()) {
                 if ( detectionData->colors[ i ] == 1 ) {
                     detectionData->colors[ i ] = 3;
                 }
                 else {
                     detectionData->colors[ i ] = 1;
                 }
                break;
             }
        }
    }
}

void myImage :: load_image (QString& file_name) {

     delete picture;
     if (mask_image != NULL)
        delete mask_image;
     if (picture != main_image)
        delete main_image;

     lines.clear();

     picture = new QImage (file_name);
     main_image = new QImage (file_name);

     mask_image = new QImage (picture->width(), picture->height(), QImage :: Format_RGB32);
     for (int i = 0; i < picture->width(); ++i)
         for (int j = 0; j < picture->height(); ++j)
             mask_image->setPixel (i, j, qRgb (122, 122, 122));

//     std :: ofstream out ("out.txt");
//     out << file_name.toStdString () << std :: endl;

     zoom_pict = *picture;
     zoom_koeff = 1.0;

     mid_x = first_mid_x = picture->width () / 2;
     mid_y = first_mid_y = picture->height () / 2;

     bot_x = first_mid_x - (x / 2);
     bot_y = first_mid_y - (y / 2);

     this->update ();
}

void myImage :: load_image (QImage *new_image)
{
    delete picture;
    if (mask_image != NULL)
        delete mask_image;
    delete main_image;
    picture = new_image;
    main_image = new_image;

    mask_image = new QImage (picture->width(), picture->height(), QImage :: Format_RGB32);

    for (int i = 0; i < picture->width(); ++i)
        for (int j = 0; j < picture->height(); ++j)
            mask_image->setPixel (i, j, qRgb (122, 122, 122));

    zoom_pict = *picture;
    zoom_koeff = 1.0;

    mid_x = first_mid_x = picture->width () / 2;
    mid_y = first_mid_y = picture->height () / 2;

    bot_x = first_mid_x - (x / 2);
    bot_y = first_mid_y - (y / 2);

    this->update ();
}

void myImage :: Mat2QImage( Mat& proc_image, QImage* &new_image )
{
    int im_width  = proc_image.cols,
        im_height = proc_image.rows;

    new_image = new QImage( im_width, im_height, QImage :: Format_RGB32 );

    uchar *proc_image_data = NULL;
    int tmp_val = 0;

    for( int j = 0; j < im_height; ++j ) {
        proc_image_data = proc_image.ptr( j );

        for( int i = 0; i < im_width; ++i ) {
            tmp_val = ( int ) proc_image_data[ i ];
            new_image->setPixel( QPoint( i, j ), qRgb( tmp_val, tmp_val, tmp_val ));
        }
    }
}

void myImage :: load_image( Mat& new_image )
{
    QImage *new_qimage;
    Mat2QImage( new_image, new_qimage );
    load_image( new_qimage );
}

//_________________________SLOTS_________________________//

void myImage :: set_white_color()
{
    r = 255, g = 255, b = 255;
}

void myImage :: set_black_color()
{
    r = 0, g = 0, b = 0;
}

void myImage :: zoom ( float k, float pos_x, float pos_y )
{
    float image_x1 = (pos_x + bot_x)/zoom_koeff;
    float image_y1 = (pos_y + bot_y)/zoom_koeff;

    if(zoom_koeff*k > 2.0f  )
    {
        zoom_koeff = 2.0f;
        return;
    }
    if( zoom_koeff*k < 0.1f )
    {
        zoom_koeff = 0.1f;
        return;
    }

    zoom_koeff *= k;

    w = picture->width () * zoom_koeff;
    h = picture->height () * zoom_koeff;

    mid_x = first_mid_x * zoom_koeff;
    mid_y = first_mid_y * zoom_koeff;

    bot_x = mid_x - (x / 2);
    bot_y = mid_y - (y / 2);

    zoom_pict = picture->scaled (w, h);

    float image_x2 = (pos_x + bot_x)/zoom_koeff;
    float image_y2 = (pos_y + bot_y)/zoom_koeff;

    first_mid_x -= image_x2 - image_x1;
    first_mid_y -= image_y2 - image_y1;

    mid_x = first_mid_x * zoom_koeff;
    mid_y = first_mid_y * zoom_koeff;

    bot_x = mid_x - (x / 2);
    bot_y = mid_y - (y / 2);

    x_initial = x_last = int (pos_x);
    y_initial = y_last = int (pos_y);

    SaveInitialCoordinates();

    this->update ();
}

void myImage :: update_data ()
{
    this->update ();
}

void myImage :: set_flag ()
{
    limit_flag = !limit_flag;
}
