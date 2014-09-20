#include "neuron_recognition.h"
#include <iostream>
#include <fstream>

ClassifierCascade *get_classifier( const char *classifier_filename ) {
    
    ClassifierCascade *cascade = new ClassifierCascade();

    cv :: FileStorage fs_cascade;
    fs_cascade.open( classifier_filename, cv :: FileStorage :: READ );

    RayFeaturesPrecomputer features_precomputer = 
                     RayFeaturesPrecomputer( 1, std :: vector< int >( 1, 255 ));
    features_precomputer.read( fs_cascade[ "features_precomputer" ]);
    
    cascade->read( fs_cascade[ "classifier_cascade" ]);
    fs_cascade.release();

    return cascade;
}

RayFeaturesPrecomputer *get_precomputer( const char *classifier_filename ) {
    
    cv :: FileStorage fs_cascade;
    fs_cascade.open( classifier_filename, cv :: FileStorage :: READ );

    RayFeaturesPrecomputer *features_precomputer = 
                        new RayFeaturesPrecomputer(1, std::vector<int>(1, 255));
    features_precomputer->read( fs_cascade[ "features_precomputer" ]);
    
    fs_cascade.release();
    
    return features_precomputer;
}

neuron_recognition :: neuron_recognition()
{
    proc = new preprocessing();
    main_classifier = new ann_classifier( "../Examples/ann_model" );
    cascade = get_classifier( "./ClassifCoeffs/classifier_1000_700_97_4.xml" );
    features_precomputer = get_precomputer( "./ClassifCoeffs/classifier_1000_700_97_4.xml" );
}

neuron_recognition :: neuron_recognition( const neuron_recognition &orig)
{
    orig.proc->copy( proc );
    clusters = orig.clusters;
    main_image = orig.main_image.clone();
}

neuron_recognition :: ~neuron_recognition()
{
    clusters.clear();
    main_image.release();
    delete main_classifier;
    delete proc;
}

vector< Mat >& neuron_recognition :: recognize( const char *input_file, Mat &proc_image, int type )
{
	vector< Mat > *answer = new vector< Mat >;

    if ( main_image.data ) {
        main_image.release();
        clusters.clear();
    }

	main_image = imread( input_file, 0 );
    if ( type == 1 ) {
		clusters = proc->get_conn( input_file, proc_image );
        imwrite( "bw.bmp", proc_image );
	}
	else if ( type == 2 ) {
		//proc->do_prep( input_file, proc_image );
		proc->aFilter(( char *) input_file );
        proc_image = imread( "dlls\\tmp.bmp", 0 );
		proc->thresholding( proc_image, 247 );
		//imwrite( "aFilterImage.bmp", proc_image );
		clusters = proc->get_conn( input_file, proc_image, true, false );
	}

    for( unsigned int i = 0; i < clusters.size(); ++i ) {
		QPoint begp, endp;
		/*double koeff = clusters[ i ].get_long_points( begp, endp );
		normalize_cluster.push_back( get_normalized_cluster( clusters[ i ], begp, endp ));

		Mat tmp_cluster = Mat( normalize_cluster[ i ].get_bottom_right().y() + 4, normalize_cluster[ i ].get_bottom_right().x() + 4, CV_8UC1, 255 );
		for ( unsigned int j = 0; j < normalize_cluster[ i ].length(); ++j ) {
			tmp_cluster.ptr( normalize_cluster[ i ][ j ].y() + 1 )[ normalize_cluster[ i ][ j ].x() + 1 ] = 0;
		}
		Mat se = getStructuringElement( cv :: MORPH_CROSS, cv :: Size( 3, 3 ));
		erode ( tmp_cluster, tmp_cluster, se );
		dilate( tmp_cluster, tmp_cluster, se );

		string file_name = "bwsamples\\norm_cluster" + int2str( i ) + ".bmp";
		imwrite(( const char * ) file_name.c_str(), tmp_cluster );
		answer->push_back( tmp_cluster );*/
		int rows = clusters[ i ].get_bottom_right().y() - clusters[ i ].get_top_left().y() + 3,
			cols = clusters[ i ].get_bottom_right().x() - clusters[ i ].get_top_left().x() + 3;
		int marg_x = clusters[ i ].get_top_left().x() - 1,
			marg_y = clusters[ i ].get_top_left().y() - 1;
		Mat tmp_cluster = Mat( rows, cols, CV_8UC1, 255 );
		for ( unsigned int j = 0; j < clusters[ i ].length(); ++j ) {
			tmp_cluster.ptr( clusters[ i ][ j ].y() - marg_y )[ clusters[ i ][ j ].x() - marg_x ] = 0;
		}
		/*string file_name = "bwsamples\\norm_cluster" + int2str( i ) + ".bmp";
		imwrite(( const char * ) file_name.c_str(), tmp_cluster );*/
		answer->push_back( tmp_cluster );
    }

//	for( int i = 0; i < clusters.size(); ++i ) {
//		clusters[ i ].paint_conv_hull( main_image );
//	}
//	imwrite( "conv_hull.bmp", main_image );
	return *answer;
}

bool neuron_recognition :: check_core( cluster& sample, cluster& cluster_tmp, Mat& bw_image )
{
	QPoint center = sample.get_center() + cluster_tmp.get_top_left() + QPoint( -1, -1 );

	int cent_x = center.x(),
		cent_y = center.y(),
		max_x = bw_image.cols, max_y = bw_image.rows;

	double norm = 0.0, amm = 0.0;

	for ( int y = cent_y - 2; y <= cent_y + 2; ++y ) {
		for ( int x = cent_x - 2; x <= cent_x + 2; ++x ) {
			if ( x >= 0 && x < max_x && y >= 0 && y < max_y ) {
				norm += 1.0;
				if ( bw_image.ptr( y )[ x ] == 255 ) {
					amm += 1.0;
				}
			}
		}
	}

	if ( ( amm / norm ) > 0.6 ) {
		return true;
	}
	else {
		return false;
	}
}

bool neuron_recognition :: isNeuron( QRect& rectangle )
{
    //imwrite( "./main_image.bmp", main_image );
    int x1 = rectangle.x();
    int y1 = rectangle.y();
    int x2 = rectangle.x() + rectangle.width();
    int y2 = rectangle.y() + rectangle.height();

    if( x1 < 0 || x1 >= main_image.cols || y1 < 0 || y1 >= main_image.rows )
        return false;
    if( x2 < 0 || x2 >= main_image.cols || y2 < 0 || y2 >= main_image.rows )
        return false;

    Mat resized_image = Mat( main_image, Rect( rectangle.x(), 
                                               rectangle.y(), 
                                               rectangle.width(), 
                                               rectangle.height()));
    resize( resized_image, resized_image, Size( 24, 24 ));
    //imwrite( "./resized.bmp", resized_image );
    
    FEATS features = features_precomputer->precompute( resized_image );
    LabeledSampleElement_ptr sample_element_ptr = 
          LabeledSampleElement_ptr( new LabeledSampleElement( features, true ));
    
    return cascade->classify( sample_element_ptr );
}

void neuron_recognition :: ray_classify( Mat& proc_image, vector<Mat>& samples, vector<QRect>&, vector<int>& colors )
{
    
    
}

void neuron_recognition :: sample_classify( Mat& bw_image, vector< Mat >& samples, vector< QRect >& rectangles, vector< int >& colors )
{
    for( unsigned int i = 0; i < clusters.size(); ++i ) {
       rectangles.push_back( clusters[ i ].get_rect() );
    }
    correct_rectangles( rectangles );
    colors.assign( rectangles.size(), 1 );

	unsigned int samples_len = samples.size();

	for( unsigned int i = 0; i < samples.size(); ++i) {
		vector< cluster > sampl_clusters = proc->get_conn( NULL, samples[ i ], false );
		if ( clusters[ i ].get_top_left().y() < 12000 ) {
			if ( sampl_clusters.size() == 1 ) {
				QPoint center = sampl_clusters[ 0 ].get_center() + clusters[ i ].get_top_left() + QPoint( -1, -1 );
                                //Вставлять сюда
                                if ( isNeuron( rectangles[ i ]) && rectangles[ i ].width() > 30 ) {
                                    colors[ i ] = 2;
                                }
                                else {
                                    cerr << i << endl;
                                }
				/*if ( check_core( sampl_clusters[ 0 ], clusters[ i ], bw_image ) ) {
					colors[ i ] = 2;
				}*/
			}
			else if ( sampl_clusters.size() == 2 ) {
				colors[ i ] = 2;
			/*	vector< cluster > :: iterator k = sampl_clusters.begin();
				for( unsigned int j = 0; j < sampl_clusters.size(); ++j) {
					if( sampl_clusters[ j ].get_top_left() == QPoint( 0, 0 )) {
						sampl_clusters.erase( k );
						break;
					}
					else {
						k++;
					}
				}
				for( int n = 0; n < sampl_clusters[ 0 ].length(); ++n ) {
					samples[ i ].ptr( sampl_clusters[ 0 ][ n ].y())[ sampl_clusters[ 0 ][ n ].x() ] = 0;
				}
				Mat se = getStructuringElement( cv :: MORPH_ELLIPSE, cv :: Size( 3, 3 ));
				erode ( samples[ i ], samples[ i ], se );
				dilate( samples[ i ], samples[ i ], se );
				sampl_clusters.clear();
				sampl_clusters = proc->get_conn( NULL, samples[ i ], false );
				int num = 0;
				int max_size = -10000;
				for( int n = 0; n < sampl_clusters.size(); ++n ) {
					if ( sampl_clusters[ n ].length() > max_size ) {
						max_size = sampl_clusters[ n ].length();
						num = n;
					}
				}
				QPoint main_margin = clusters[ i ].get_top_left() + QPoint( -1, -1 );
				sampl_clusters[ num ] += main_margin;
				clusters[ i ] = sampl_clusters[ num ];*/
			}
			else if ( sampl_clusters.size() > 2 && sampl_clusters.size() < 10 ) {
				//imwrite("error.bmp", samples[ i ]);
				vector< cluster > :: iterator k = sampl_clusters.begin();
				for( unsigned int j = 0; j < sampl_clusters.size(); ++j) {
					if( sampl_clusters[ j ].get_top_left() == QPoint( 0, 0 )) {
						sampl_clusters.erase( k );
						break;
					}
					else {
						k++;
					}
				}
				divide_clusters( samples[ i ], sampl_clusters );
				//imwrite("main_error.bmp", samples[ i ]);

				vector< cluster > new_clusters = proc->get_conn( NULL, samples[ i ], true, false);
				QPoint main_margin = clusters[ i ].get_top_left() + QPoint( -1, -1 );
				for( int k = 0; k < new_clusters.size(); ++k ) {
					if( k == 0 ) {
						Mat tmp_clust_im = Mat( samples[ i ].rows, samples[ i ].cols, CV_8UC1, 255 );
						for ( unsigned int j = 0; j < new_clusters[ k ].length(); ++j ) {
							tmp_clust_im.ptr( new_clusters[ k ][ j ].y())[ new_clusters[ k ][ j ].x()] = 0;
						}
						//imwrite("error1.bmp", tmp_clust_im);
						samples[ i ] = tmp_clust_im;
						new_clusters[ k ] += main_margin;
						clusters[ i ] = new_clusters[ k ];
						rectangles[ i ] = clusters[ i ].get_rect();
						colors[ i ] = 2;
					}
					else {
						Mat tmp_clust_im = Mat( samples[ i ].rows, samples[ i ].cols, CV_8UC1, 255 );
						for ( unsigned int j = 0; j < new_clusters[ k ].length(); ++j ) {
							tmp_clust_im.ptr( new_clusters[ k ][ j ].y())[ new_clusters[ k ][ j ].x()] = 0;
						}
						//imwrite("error2.bmp", tmp_clust_im);
						samples.push_back( tmp_clust_im );
						new_clusters[ k ] += main_margin;
						clusters.push_back( new_clusters[ k ]);
						rectangles.push_back( new_clusters[ k ].get_rect());
						colors.push_back( 2 );
					}
				}
			}
			else {
				vector< cluster > :: iterator m = clusters.begin();
				vector< Mat > :: iterator n = samples.begin();
				vector< QRect > :: iterator p = rectangles.begin();

				clusters[ i ].clear();
				
				clusters.erase( m + i );
				samples.erase( n + i );
				rectangles.erase( p + i );
				--i;
			}
			for( unsigned int i = 0; i < sampl_clusters.size(); ++i ) {
				sampl_clusters[ i ].clear();
			}
		}
	}
	correct_rectangles( rectangles );
	//imwrite( "bw.bmp", bw_image );
	for ( unsigned int i = 0; i < clusters.size(); ++i ) {
		if( i == 119 ) {
			srand(213213);
		}
		//outF << i << '\t' << endl;
		clusters[ i ].culc_conn_hull( bw_image );
	}
}

void neuron_recognition :: divide_clusters( Mat& proc_image, vector< cluster >& white_areas )
{
	unsigned int areas_len = white_areas.size();

	Mat main_tmp_image = proc_image.clone();
	for( int i = 0; i < proc_image.cols; ++i ) {
		for( int j = 0; j < proc_image.rows; ++j ) {
			proc_image.ptr( j )[ i ] = 255;
		}
	}

	for( unsigned int i = 0; i < areas_len; ++i ) {
		for( unsigned int j = 0; j < white_areas[ i ].length(); ++j ) {
			main_tmp_image.ptr( white_areas[ i ][ j ].y() )[ white_areas[ i ][ j ].x() ] = 0;
		}
	}

	double k = 0.0, b = 0.0;
	QPoint mid_point = QPoint( 0, 0 );

	for( unsigned int i = 0; i < areas_len; ++i ) {
		Mat tmp_image = main_tmp_image.clone();
		for( unsigned int j = 0; j < areas_len; ++j ) {
			if ( i == j ) {
				continue;
			}
			mid_point = get_koeffs( white_areas[ i ], white_areas[ j ], k, b );
			if ( k == 0.0 && b == 0.0 ) {
				set_line( tmp_image, mid_point);
			}
			else {
				set_line( tmp_image, k, b );
			}
		}
		get_area( tmp_image, proc_image, QPoint( white_areas[ i ][ 0 ]));
	}
}

void neuron_recognition :: get_area( Mat& proc_image, Mat& output_image, QPoint seed )
{
	int width = proc_image.cols,
		height = proc_image.rows;
	int **cluster_mat = new int*[ proc_image.cols ];
    for( int i = 0; i < proc_image.cols; ++i)
        cluster_mat[ i ] = new int[ proc_image.rows ];
	for( int i = 0; i < width; ++i )
        for( int j = 0; j < height; ++j )
            cluster_mat[ i ][ j ] = -1;
	int _x = 0, _y = 0;

	cluster query_vec;

	cluster_mat[ seed.x() ][ seed.y() ] = 1;
	query_vec.set_point( seed.x(), seed.y() );

	for( unsigned int k = 0; k < query_vec.length(); ++k ) {
        _x = query_vec[ k ].x();
        _y = query_vec[ k ].y();

        if(( _x - 1 >= 0 ) && ( cluster_mat[ _x - 1 ][ _y ] == -1 ) && preprocessing :: check_diff( proc_image, _x, _y, 1 )) {
            cluster_mat[ _x - 1 ][ _y ] = 1;
            query_vec.set_point( _x - 1, _y );
        }

        if(( _x + 1 < width ) && ( cluster_mat[ _x + 1 ][ _y ] == -1 ) && preprocessing :: check_diff( proc_image, _x, _y, 2 )) {
            cluster_mat[ _x + 1 ][ _y ] = 1;
            query_vec.set_point( _x + 1, _y );
        }

        if(( _y - 1 >= 0 ) && ( cluster_mat[ _x ][ _y - 1 ] == -1 ) && preprocessing :: check_diff( proc_image, _x, _y, 3 )) {
            cluster_mat[ _x ][ _y - 1] = 1;
            query_vec.set_point( _x, _y - 1 );
        }

        if(( _y + 1 < height ) && ( cluster_mat[ _x ][ _y + 1 ] == -1 ) && preprocessing :: check_diff( proc_image, _x, _y, 4 )) {
            cluster_mat[ _x ][ _y + 1 ] = 1;
            query_vec.set_point( _x , _y + 1 );
        }
    }

	for( unsigned int i = 0; i < query_vec.length(); ++i ) {
		output_image.ptr( query_vec[ i ].y())[ query_vec[ i ].x()] = 0;
	}
	query_vec.clear();
}

void neuron_recognition :: set_line( Mat& proc_image, QPoint& midp )
{
	int x = midp.x(), y = 0, end_y = proc_image.rows;
	for ( y = 0; y < end_y; ++y ) {
		proc_image.ptr( x )[ y ] = 255;
	}
}

void neuron_recognition :: set_line( Mat& proc_image, double k, double b )
{
	int x = 0, y = 0,
		end_x = proc_image.cols, 
		end_y = proc_image.rows;

	if( k == 0 ) {
        y = round( b );

		for( x = 0; x < end_x; ++x) {
			proc_image.ptr( y )[ x ] = 255;
		}
    }
    else {
		if ( fabs( k ) < 1 ) {
			for( x = 0; x < end_x; ++x ) {
				y = round( k * x + b );
				if ( y >=0 && y < end_y ) {
					proc_image.ptr( y )[ x ] = 255;
				}
			}
		}
		else {
			for( y = 0; y < end_y; ++y ) {
				x = round(( y * 1.0 - b ) / k );
				if ( x >= 0 && x < end_x ) {
					proc_image.ptr( y )[ x ] = 255;
				}
			}
		}
    }
}

QPoint neuron_recognition :: get_koeffs( cluster& fir_cl, cluster& sec_cl, double& k, double& b )
{
	QPoint answ = QPoint( 0, 0 );
	double min_dist = 10000.0;
	unsigned int fir_len = fir_cl.length(),
				 sec_len = sec_cl.length();
	QPoint firp( 0, 0 ), secp( 0, 0 ), main_firp( 0, 0 ), main_secp( 0, 0 );

	for( unsigned int i = 0; i < fir_len; ++i ) {
		firp = fir_cl[ i ];
		for ( unsigned int j = 0; j < sec_len; ++j ) {
			secp = sec_cl[ j ];
			double tmp_dist = cluster :: euq_dist( firp, secp );
			if ( tmp_dist < min_dist ) {
				min_dist = tmp_dist;
				answ = QPoint( round(( firp.x() + secp.x()) / 2 ) , round(( firp.y() + secp.y()) / 2 ));
				main_firp = firp;
				main_secp = secp;
			}
		}
	}

	if ( main_firp.y() != main_secp.y() ) {
		k = ( -1.0 ) * ( main_secp.x() - main_firp.x()) / ( main_secp.y() - main_firp.y());
		b = answ.y() * 1.0 - k * answ.x();
	}
	return answ;
}

void neuron_recognition :: classify( vector< QRect >& rectangles, vector< int >& colors )
{
    unsigned int len = clusters.size();

    for( unsigned int i = 0; i < len; ++i ) {
        rectangles.push_back( clusters[ i ].get_rect() );
    }
    correct_rectangles( rectangles );
    colors.assign( rectangles.size(), 1 );

	//Mat prepr_image = imread( "tmp.bmp" );
    Mat prepr_image = main_image.clone();
    proc->classifier_prep( prepr_image );

    vector< float > answ;
    main_classifier->predict_rows( prepr_image, rectangles, answ );

    for( unsigned int i = 0; i < len; ++i ) {
        if( answ[ i ] > 0 ) {
            colors[ i ] = 2;
        }
    }
    answ.clear();	
}

void neuron_recognition :: correct_rectangles( vector<QRect> &rectangles )
{
    int top_x = 0, top_y = 0,
        bot_x = 0, bot_y = 0;

    unsigned int len = rectangles.size();
    for( unsigned int i = 0; i < len; ++i ) {

        top_x = rectangles[ i ].topLeft().x();
        top_y = rectangles[ i ].topLeft().y();
        bot_x = rectangles[ i ].bottomRight().x();
        bot_y = rectangles[ i ].bottomRight().y();
        //TODO Должна быть проверка на выход за пределы картинки
        if(( bot_x - top_x ) > ( bot_y - top_y )) {
            top_y = ( bot_y + top_y - bot_x + top_x ) / 2;
            bot_y = top_y + bot_x - top_x;
        }
        else {
            top_x = ( top_x + bot_x - bot_y + top_y ) / 2;
            bot_x = top_x + bot_y - top_y;
        }
        rectangles[ i ] = QRect( QPoint( top_x, top_y ), QPoint( bot_x, bot_y ));
    }
}

cluster neuron_recognition :: get_normalized_cluster( cluster& priv, QPoint a, QPoint b )
{
	QPoint top_left = priv.get_top_left(),
		   bottom_right = priv.get_bottom_right();
	a = a - top_left;
	b = b - top_left;

	cluster answer = priv;
	double cos_angle = ( b.x() - a.x()) * 1.0 / cluster :: euq_dist( a, b );
	double sin_angle = ( b.y() - a.y()) * 1.0 / cluster :: euq_dist( a, b );
	//double k_tan	 = ( b.y() - a.y()) * 1.0 / ( b.x() - a.x());
	answer.refresh_boundaries();

	for ( unsigned int i = 0; i < answer.length(); ++i ) {
		QPoint tmp_point = answer[ i ] - top_left;
		double tmp_x = ( double ) tmp_point.x(),
			   tmp_y = ( double ) tmp_point.y();
		tmp_point = QPoint( round( tmp_x * cos_angle + tmp_y * sin_angle ), round(( -1.0 ) * tmp_x * sin_angle + tmp_y * cos_angle ));
		answer.set_point( i, tmp_point );
	}

	QPoint new_top_left = answer.get_top_left();
	QPoint margin = QPoint( 1, 1 );
	if ( new_top_left.x() != 0 ) {
		margin.setX( ( -1 ) * new_top_left.x() + 1 );
	}
	if ( new_top_left.y() != 0 ) {
		margin.setY( ( -1 ) * new_top_left.y() + 1 );
	}
	answer += margin;

	return answer;
}

string neuron_recognition :: int2str( int num )
{
    string answ = "";

    if ( num == 0 ) {
        answ = "0";
    }
    else {
        string temp= "";

        while ( num > 0 )
        {
            temp += num % 10 + 48;
            num /= 10;
        }

        for ( int i = 0; i < temp.length(); ++i )
            answ += temp[ temp.length() - i - 1 ];
    }

    return answ;
}

vector< vector< QPoint> >& neuron_recognition :: get_bounds()
{
	vector< vector< QPoint > > *answer = new vector< vector< QPoint > >;
	unsigned int clust_len = clusters.size();

	for( unsigned int i = 0; i < clust_len; ++i ) {
		vector< QPoint > tmp_bounds;
		convex_hull& tmp_hull = clusters[ i ].get_conn_hull();
		for( unsigned int j = 0; j < tmp_hull.size(); ++j ) {
			tmp_bounds.push_back( tmp_hull[ j ] );
		}
		answer->push_back( tmp_bounds );
	}

	return *answer;
}
