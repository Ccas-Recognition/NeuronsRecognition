#include "neuron_recognition.h"

convex_hull :: convex_hull( vector< QPoint > &_init_points )
    : init_points( _init_points )
{
    length = init_points.size();
}

void convex_hull :: init_conv_hull( vector<QPoint> &_init_points )
{
    init_points = _init_points;
    length = init_points.size();
}

void convex_hull :: get_convex_hull()
{
    init_convex_hull();

    num_points *init_p = new num_points;
    for( unsigned int i = 0; i < length; ++i )
        init_p->push_back( i );

    result.push_back( top_point );

    num_points *left_points = whats_left( *init_p, top_point, right_point );
    if ( left_points != NULL ) {
        find_next_point( left_points, top_point, right_point );

        left_points->clear();
        delete left_points;
        left_points = NULL;
    }

    result.push_back( right_point );
    left_points = whats_left( *init_p, right_point, bottom_point );

    if ( left_points != NULL ) {
        find_next_point( left_points, right_point, bottom_point );

        left_points->clear();
        delete left_points;
        left_points = NULL;
    }

    result.push_back( bottom_point );
    left_points = whats_left( *init_p, bottom_point, left_point );

    if ( left_points != NULL ) {
        find_next_point( left_points, bottom_point, left_point );

        left_points->clear();
        delete left_points;
        left_points = NULL;
    }

    result.push_back( left_point );
    left_points = whats_left( *init_p, left_point, top_point );

    if ( left_points != NULL ) {
        find_next_point( left_points, left_point, top_point );

        left_points->clear();
        delete left_points;
        left_points = NULL;
    }

    //set_lines();

    //return result;
}

void convex_hull :: init_convex_hull()
{
    top_point = QPoint( 0, 0 );
    bottom_point = QPoint( 0, 10000 );
    left_point = QPoint( 10000, 0 );
    right_point = QPoint( 0, 0 );

    for ( unsigned int i = 0; i < length; ++i ){

        if ( init_points[ i ].x() >= right_point.x() )
            right_point = init_points[ i ];

        if ( init_points[ i ].x() <= left_point.x() )
            left_point = init_points[ i ];

        if ( init_points[ i ].y() <= bottom_point.y() )
            bottom_point = init_points[ i ];

        if ( init_points[ i ].y() >= top_point.y() )
            top_point = init_points[ i ];
    }
}

num_points *convex_hull :: whats_left( num_points& init_vec, QPoint& left_point, QPoint& right_point )
{
    unsigned int vec_size = init_vec.size();

    num_points *result_vec = new num_points;

    for ( unsigned int i = 0; i < vec_size; ++i ) {
        if ( vector_sign( init_points [ init_vec[ i ] ], left_point, right_point ) )
            result_vec->push_back( init_vec[ i ] );
    }

    if (result_vec->size() == 0) {
        delete result_vec;
        result_vec = NULL;
    }

    return result_vec;
}

int convex_hull :: vector_product( QPoint &new_point, QPoint &l_point, QPoint &r_point )
{
    return (int)(( new_point.x() - l_point.x() ) * ( r_point.y() - l_point.y() )
                 - ( new_point.y() - l_point.y() ) * ( r_point.x() - l_point.x()));
}

bool convex_hull :: vector_sign( QPoint& new_point, QPoint& l_point, QPoint& r_point )
{
    return ( vector_product( new_point, l_point, r_point ) < 0 );
}

void convex_hull :: find_next_point( num_points *p_points, QPoint &left_point, QPoint &right_point )
{
    int osn_x = ( int ) ( right_point.x() - left_point.x() );
    int osn_y = ( int ) ( right_point.y() - left_point.y() );
    int new_x = 0, new_y = 0;
    double osn_len = sqrt( osn_x * osn_x * 1.0 + osn_y * osn_y * 1.0 );
    double tmp_len = 0.0, ortho_len = 0.0, new_vec_len = 0.0, cos_val = 0.0, sin_val = 0.0;
    int new_point_num = 0;
    unsigned int points_length = p_points->size();

    for ( unsigned int i = 0; i < points_length; ++i ) {

        new_x = init_points[ (*p_points)[ i ] ].x() - left_point.x();
        new_y = init_points[ (*p_points)[ i ] ].y() - left_point.y();
        new_vec_len = sqrt( new_x * new_x * 1.0 + new_y * new_y * 1.0 );
        sin_val = ( new_x * osn_x * 1.0  + new_y * osn_y * 1.0 ) / ( osn_len * new_vec_len );
        cos_val = sqrt ( 1.0 - sin_val * sin_val );
        tmp_len = new_vec_len * cos_val;

        if ( tmp_len > ortho_len ) {
            ortho_len = tmp_len;
            new_point_num = (*p_points)[ i ];
        }
    }

    num_points *left_points = whats_left( *p_points, left_point, init_points[ new_point_num ]);

    if ( left_points != NULL ) {
        find_next_point( left_points, left_point, init_points[ new_point_num ]);

        left_points->clear();
        delete left_points;
        left_points = NULL;
    }

    result.push_back( init_points[ new_point_num ] );
    left_points = whats_left( *p_points, init_points[ new_point_num ], right_point );

    if ( left_points != NULL ) {
        find_next_point( left_points, init_points[ new_point_num ], right_point );

        left_points->clear();
        delete left_points;
        left_points = NULL;
    }
}

void convex_hull :: set_lines()
{
    unsigned int res_length = result.size();

    double x1 = 0.0, x2 = 0.0, y1 = 0.0, y2 = 0.0;
    double k = 0.0, b = 0.0, tan_val = 0.0;
    int beg_x = 0, end_x = 0, beg_y = 0, end_y = 0, x = 0, y = 0;

    for ( unsigned int i = 0; i < res_length; ++i ) {
        x1 = result[ i ].x() * 1.0;
        y1 = result[ i ].y() * 1.0;

        x2 = result[( i + 1 ) % res_length ].x() * 1.0;
        y2 = result[( i + 1 ) % res_length ].y() * 1.0;

        k = ( y2 - y1 ) / ( x2 - x1 );
        b = y1 - k * x1;

        if( x2 == x1 ) {
            if ( y1 < y2 ) {
                beg_y = y1 + 1;
                end_y = y2 - 1;
            }
            else {
                beg_y = y2 + 1;
                end_y = y1 - 1;
            }

            for ( y = beg_y; y <= end_y; ++y ) {
                x = ( unsigned int ) x1;
                result.push_back( QPoint( x, y ));
            }
        }
        else {
            tan_val = fabs( y2 - y1 ) / fabs( x2 - x1 );
            if ( tan_val < 1.0 ) {

                if ( x1 < x2 ) {
                    beg_x = x1 + 1;
                    end_x = x2 - 1;
                }
                else {
                    beg_x = x2 + 1;
                    end_x = x1 - 1;
                }

                for ( x = beg_x; x <= end_x; ++x ) {
                    y = ( unsigned int ) round( k * x + b );
                    result.push_back( QPoint( x, y ));
                }
            }
            else {
                if ( y1 < y2 ) {
                    beg_y = y1 + 1;
                    end_y = y2 - 1;
                }
                else {
                    beg_y = y2 + 1;
                    end_y = y1 - 1;
                }

                for ( y = beg_y; y <= end_y; ++y ) {
                    x = ( unsigned int ) round(( y - b ) / k );
                    result.push_back( QPoint( x, y ));
                }
            }
        }
    }
}

QPoint convex_hull :: operator[]( int pos )
{
    return result[ pos ];
}

void convex_hull :: get_line( QPoint& p1, QPoint& p2, vector< QPoint >& line_points )
{
	if ( line_points.size() > 0 ) {
		line_points.clear();
	}
	int x1 = p1.x(), y1 = p1.y(), x2 = p2.x(), y2 = p2.y();

	double k = ( y2 - y1 ) * 1.0 / ( x2 - x1 );
	double b = y1 * 1.0 - k * x1;
	int beg_x = 0, beg_y = 0, end_x = 0, end_y = 0;
	int x = 0, y = 0;

	if( x2 == x1 ) {
		int koeff = 1;
		beg_y = y1;
		end_y = y2;
		if ( y1 > y2 ) {
			koeff = -1;
		}

		x = x1;
		for ( y = beg_y; y != end_y + koeff; y += koeff ) {
			line_points.push_back( QPoint( x, y ));
		}
	}
	else {
		if ( fabs( k ) < 1.0 ) {
			int koeff = 1;
			beg_x = x1;
			end_x = x2;
			if ( x1 > x2 ) {
				koeff = -1;
			}

			for ( x = beg_x; x != end_x + koeff; x += koeff ) {
				y = round( k * x + b );
				line_points.push_back( QPoint( x, y ));
			}
		}
		else {
			int koeff = 1;
			beg_y = y1;
			end_y = y2;
			if ( y1 > y2 ) {
				koeff = -1;
			}

			for ( y = beg_y; y != end_y + koeff; y += koeff ) {
				x = round(( y - b ) / k);
				line_points.push_back( QPoint( x, y ));
			}
		}
	}
}

double convex_hull :: euq_dist( QPoint& fir, QPoint& sec )
{
	double answ = ( fir.x() - sec.x() ) * ( fir.x() - sec.x() ) * 1.0;
	answ += ( fir.y() - sec.y() ) * ( fir.y() - sec.y() ) * 1.0;
	return std :: sqrt( answ );
}

double convex_hull :: get_new_points( Mat& proc_image, vector< QPoint >& line, vector< QPoint >& new_points )
{
	double max_diff = -1000.0,
		   tmp_diff = 0.0;
	if( new_points.size() > 0 ) {
		new_points.clear();
	}
	int x1 = line[ 0 ].x(), y1 = line[ 0 ].y(),
		x2 = line[ line.size() - 1 ].x(), y2 = line[ line.size() - 1 ].y(),
		cur_x = 0, cur_y = 0,
		tmp_x = 0, tmp_y = 0;
	int ort_koeff = 1;

	new_points.push_back( line[ 0 ]);

	if ( x1 == x2 ) {

		int vect_prod = ort_koeff * ( y2 - y1 );
		ort_koeff = ( vect_prod < 0 ) ? -1 : 1;

		for ( unsigned int i = 1; i < line.size() - 1; ++i ) {
			tmp_diff = 0.0;
			tmp_x = line[ i ].x();
			tmp_y = line[ i ].y();
			while( 1 ) {
				if ( check_pix( tmp_x, tmp_y, proc_image ) ) {
					tmp_diff += 1.0;
					tmp_x += ort_koeff;
				}
				else {
					break;
				}
			}
			new_points.push_back( QPoint( tmp_x, tmp_y ));
			max_diff = ( tmp_diff > max_diff ) ? tmp_diff : max_diff;
		}
	}
	else if ( y1 == y2 ) {
		int vect_prod = ( -1 )* ort_koeff * ( x2 - x1 );
		ort_koeff = ( vect_prod < 0 ) ? -1 : 1;

		for ( unsigned int i = 1; i < line.size() - 1; ++i ) {
			tmp_diff = 0.0;
			tmp_x = line[ i ].x();
			tmp_y = line[ i ].y();
			while( 1 ) {
				if ( check_pix( tmp_x, tmp_y, proc_image ) ) {
					tmp_diff += 1.0;
					tmp_y += ort_koeff;
				}
				else {
					break;
				}
			}
			new_points.push_back( QPoint( tmp_x, tmp_y ));
			max_diff = ( tmp_diff > max_diff ) ? tmp_diff : max_diff;
		}
	}
	else {
		double k_ort = ( -1.0 ) * ( x2 - x1 ) / ( y2 - y1 );

		if ( fabs( k_ort ) > 1 ) {
			double b_tmp = ( x1 * ( x2 - x1 ) + y1 * ( y2 - y1 )) * 1.0 / ( y2 - y1 );
			double vect_prod = ( ( y1 + ort_koeff - b_tmp ) * 1.0 / k_ort - x1 ) * ( y2 - y1 ) - ( x2 - x1 ) * ort_koeff;
			ort_koeff = ( vect_prod < 0 ) ? -1 : 1;

			for ( unsigned int i = 1; i < line.size() - 1; ++i ) {
				tmp_diff = 0.0;
				tmp_x = line[ i ].x();
				tmp_y = line[ i ].y();
				b_tmp = ( tmp_x * ( x2 - x1 ) + tmp_y * ( y2 - y1 )) * 1.0 / ( y2 - y1 );
				while( 1 ) {
					if ( check_pix( tmp_x, tmp_y, proc_image ) ) {
						tmp_diff += 1.0;
						tmp_y += ort_koeff;
						tmp_x = ( int ) ((tmp_y - b_tmp ) * 1.0 / k_ort );
					}
					else {
						break;
					}
				}
				new_points.push_back( QPoint( tmp_x, tmp_y ));
				max_diff = ( tmp_diff > max_diff ) ? tmp_diff : max_diff;
			}
		}
		else {
			double b_tmp = ( x1 * ( x2 - x1 ) + y1 * ( y2 - y1 )) * 1.0 / ( y2 - y1 );
			double vect_prod = (  ort_koeff ) * ( y2 - y1 ) - ( x2 - x1 ) * ( k_ort * ( x1 + ort_koeff) + b_tmp - y1 );
			ort_koeff = ( vect_prod < 0 ) ? -1 : 1;

			for ( unsigned int i = 1; i < line.size() - 1; ++i ) {
				tmp_diff = 0.0;
				tmp_x = line[ i ].x();
				tmp_y = line[ i ].y();
				b_tmp = ( tmp_x * ( x2 - x1 ) + tmp_y * ( y2 - y1 )) * 1.0 / ( y2 - y1 );
				while( 1 ) {
					if ( check_pix( tmp_x, tmp_y, proc_image ) ) {
						tmp_diff += 1.0;
						tmp_x += ort_koeff;
						tmp_y = ( int ) ( k_ort * tmp_x + b_tmp );
					}
					else {
						break;
					}
				}
				new_points.push_back( QPoint( tmp_x, tmp_y ));
				max_diff = ( tmp_diff > max_diff ) ? tmp_diff : max_diff;
			}
		}
	}
	new_points.push_back( line[ line.size() - 1 ]);
	return max_diff;
}

void convex_hull :: correct_convex_hull( Mat& proc_image )
{
	unsigned int res_length = result.size();
	vector< QPoint > line_points;
	vector< QPoint > new_points;
	vector< QPoint > new_conv_hull;
	double diff = 0.0;

	for( int i = 0; i < result.size(); ++i ) {
		if ( result[ i ] != result[ ( i + 1 ) % result.size() ]) {
			get_line( result[ i ], result[ ( i + 1 ) % result.size() ], line_points );
			diff = get_new_points( proc_image, line_points, new_points );
			if ( diff < MIN_DIFF ) {
				for( int j = 0; j < new_points.size() - 1; ++j ) {
					new_conv_hull.push_back( new_points[ j ]);
				}
			}
			else {
				for( int j = 0; j < line_points.size() - 1; ++j ) {
					new_conv_hull.push_back( line_points[ j ]);
				}
			}
		}
		else {
			result.erase( result.begin() + i );
			--i;
		}
	}

	result.clear();
	for( int i = 0; i < new_conv_hull.size(); ++i ) {
		result.push_back( new_conv_hull[ i ]);
	}
	//set_lines();
	new_conv_hull.clear();
}