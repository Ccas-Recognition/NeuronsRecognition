#include <neuron_recognition.h>

cluster :: cluster()
    : top_left( 10000, 10000 ), bottom_right( -10000, -10000 ), class_label( backGr ), pivot_koeff( 0.0 )
{
}

cluster :: cluster( const cluster &orig )
{
    points = orig.points;
    top_left = orig.top_left;
    bottom_right = orig.bottom_right;
	class_label = orig.class_label;
	pivot_koeff = orig.pivot_koeff;
	conv_hull = orig.conv_hull;
}

cluster :: ~cluster()
{
    points.clear();
}

void cluster :: set_point( int x, int y )
{
    points.push_back( QPoint( x, y ));

    if ( x < top_left.x() ) {
        top_left.setX( x );
    }
    if ( x > bottom_right.x() ) {
        bottom_right.setX( x );
    }

    if ( y < top_left.y() ) {
        top_left.setY( y );
    }
    if ( y > bottom_right.y() ) {
        bottom_right.setY( y );
    }
}

void cluster :: set_point( int pos, QPoint& new_point ) {
		int x = new_point.x(),
			y = new_point.y();

		if ( x < top_left.x() ) {
			top_left.setX( x );
		}
		if ( x > bottom_right.x() ) {
			bottom_right.setX( x );
		}

		if ( y < top_left.y() ) {
			top_left.setY( y );
		}
		if ( y > bottom_right.y() ) {
			bottom_right.setY( y );
		}
		points[ pos ] = new_point;
	}

QRect cluster :: get_rect()
{
    return QRect( top_left, bottom_right );
}

QPoint cluster :: operator[]( int pos )
{
    return points[ pos ];
}

cluster& cluster :: operator=( const cluster& orig )
{
    this->points = orig.points;
    this->bottom_right = orig.bottom_right;
    this->top_left = orig.top_left;
	this->conv_hull = orig.conv_hull;

    return *this;
}

void cluster :: operator+=( QPoint& margin )
{
	unsigned int cluster_len = points.size();

	for ( unsigned int i = 0; i < cluster_len; ++i ) {
		points[ i ] = points[ i ] + margin;
	}
	top_left = top_left + margin;
	bottom_right = bottom_right + margin;
}

void cluster :: clear()
{
    points.clear();
    top_left += QPoint( 10000, 10000 );
    bottom_right -= QPoint( 10000, 10000 );
}

unsigned int cluster :: length()
{
    return points.size();
}

void cluster :: culc_conn_hull( Mat& proc_image )
{
    conv_hull = convex_hull( this->points );
    conv_hull.get_convex_hull();
	conv_hull.correct_convex_hull( proc_image );
}

void cluster :: paint_conv_hull( Mat& proc_image )
{
	vector<QPoint>& tmp_points = conv_hull.get_conn_hull_points();

	for( int i = 0; i < tmp_points.size(); ++i ) {
		QPoint tmp_point = tmp_points[ i ];
		proc_image.ptr( tmp_point.y() )[ tmp_point.x() ] = 255;
	}
}

double cluster :: get_long_points( QPoint& begp, QPoint& endp )
{
	if ( conv_hull.is_empty() ) {
		begp = QPoint( 0, 0 );
		endp = QPoint( 0, 0 );
		return 0.0;
	}

	int conv_size = conv_hull.size();

	QPoint tmp_f = QPoint( 0, 0 ),
		   tmp_s = QPoint( 0, 0 );
	double max_dist = -100000.0;

	for ( int i = 0; i < conv_size - 1; ++i ) {
		tmp_f = conv_hull[ i ];
		for ( int j = i + 1; j < conv_size; ++j ) {
			tmp_s = conv_hull[ j ];
			int cur_dist = euq_dist( tmp_f, tmp_s );
			if ( cur_dist > max_dist ) {
				begp = tmp_f;
				endp = tmp_s;
				max_dist = cur_dist;
			}
		}
	}

	if ( begp.y() > endp.y()) {
		QPoint tmp_p = begp;
		begp = endp;
		endp = tmp_p;
	}

	return 0.0;
}
