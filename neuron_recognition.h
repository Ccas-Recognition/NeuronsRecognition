#ifndef NEURON_RECOGNITION_H
#define NEURON_RECOGNITION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
//#include <windows.h>
#include <deque>
#include "LabeledSampleElement.h"
#include "RayFeaturesPrecomputer.h"
#include "RayFeatureType.h"
#include "RayFeaturesEnumerator.h"
#include "BoostingClassifier.h"

#include "ClassifierCascade.h"
#include <vector>
#include <QPoint>
#include <QRect>
#include <cmath>
#include "classifier.h"

using namespace std;
using namespace cv;

typedef std::tr1::shared_ptr< RayFeatures > FEATS;
typedef vector< int > num_points;

class convex_hull
{
public:
	convex_hull() {}
    convex_hull( vector< QPoint >& );
    convex_hull( const convex_hull& orig ){
        this->result = orig.result;
        this->init_points = orig.init_points;
        this->length = orig.length;
    }
	~convex_hull() {
		result.clear();
		init_points.clear();
	}
    void init_conv_hull( vector< QPoint > & );
    void get_convex_hull();
	void correct_convex_hull( Mat& proc_image );

	double get_bond_length() {
		double answ = 0.0;
		for( int i = 0; i < result.size(); ++i ) {
			answ += euq_dist( result[ i ], result[ (i + 1) % result.size() ]);
		}
		return answ;
	}

    vector< QPoint >& get_conn_hull_points() {
        return result;
    }

    QPoint operator[]( int );
    convex_hull& operator=( const convex_hull& orig ) {
        this->result = orig.result;
        this->init_points = orig.init_points;
        this->length = orig.length;
        return *this;
    }

	bool is_empty() {
		if ( result.size() == 0 ) {
			return true;
		}
		else {
			return false;
		}
	}

	int size() {
		return result.size();
	}

	static int round( double value )
	{
		return ( int ) floor( value + 0.5 );
	}

private:
    vector< QPoint > init_points;
    QPoint top_point, bottom_point, left_point, right_point;
    unsigned int length;

    void init_convex_hull();
    num_points *whats_left( num_points&, QPoint& left_point, QPoint& right_point );
    void find_next_point( num_points *, QPoint& left_point, QPoint& right_point );

    int vector_product( QPoint& new_point, QPoint& l_point, QPoint& r_point );
    bool vector_sign( QPoint& new_point, QPoint& l_point, QPoint& r_point );

    void set_lines();
    vector< QPoint > result;

	static bool check_pix( double x, double y, Mat& image ) {
		if( image.ptr( (int) y )[ ( int ) x ] == 255 ) {
			return true;
		}
		else {
			return false;
		}
	}

	static bool check_pix( int x, int y, Mat& image ) {
		if( image.ptr( y )[ x ] == 255 ) {
			return true;
		}
		else {
			return false;
		}
	}

	static const int MIN_DIFF = 7;

	static void get_line( QPoint& p1, QPoint& p2, vector< QPoint >& line_points );
	static double euq_dist( QPoint& fir, QPoint& sec );
	double get_new_points( Mat& image, vector< QPoint >& line, vector< QPoint >& new_points );
};

struct neuron_props{
	double bright;
	double area;
	double perim;
	double form_fact;
};

enum ClassReq{ neuron, backGr };

class cluster {
public:
    cluster();
    cluster( const cluster &orig );
    virtual ~cluster();

    void set_point( int , int );
	void set_point( int pos, QPoint& new_point );

    QPoint operator[]( int );
    cluster& operator=(const cluster& );
    void operator+=( QPoint& margin );
    void clear();
    unsigned int length();
    QRect get_rect();

    void set_class( ClassReq label ) {
        class_label = label;
    }

	QPoint get_center() {
		int tmp_x = ( this->get_top_left().x() + this->get_bottom_right().x()) / 2;
		int tmp_y = ( this->get_top_left().y() + this->get_bottom_right().y()) / 2;
		return QPoint( tmp_x, tmp_y );
	}

    ClassReq get_label() {
        return class_label;
    }
    void culc_conn_hull( Mat& proc_image );

	void paint_conv_hull( Mat& proc_image );
	convex_hull& get_conn_hull() {
		return conv_hull;
	}
	double get_long_points( QPoint& begp, QPoint& endp );

	QPoint get_top_left() {
		return top_left;
	}

	double get_bond_length() {
		if( conv_hull.size() > 0 ) {
			return conv_hull.get_bond_length();
		}
		else {
			return 30;
		}
	}

	QPoint get_bottom_right() {
		return bottom_right;
	}
	void refresh_boundaries(){
		bottom_right = QPoint( -10000, -10000 );
		top_left = QPoint( 10000, 10000);
	}

	static double euq_dist( QPoint& fir, QPoint& sec ) {
		double answ = ( fir.x() - sec.x() ) * ( fir.x() - sec.x() ) * 1.0;
		answ += ( fir.y() - sec.y() ) * ( fir.y() - sec.y() ) * 1.0;
		return sqrt( answ );
	}
private:
    vector< QPoint > points;
    QPoint top_left,
           bottom_right;
    ClassReq class_label;
    convex_hull conv_hull;
	double pivot_koeff;
};

class preprocessing
{
public:
    preprocessing();
    preprocessing( int _smooth_radius, double _subtr_val, double _thresh_coeff );
    preprocessing( const preprocessing& orig );
     virtual ~preprocessing();
	void aFilter( char *image_name );
    void do_prep( const char * , Mat& );
    vector< cluster >& get_conn( const char * , Mat& , bool prepr = true, bool post = true );
	void thresholding( Mat& , int thresh = -1 );

    void classifier_prep( const char *, Mat& );
    void classifier_prep( Mat& );

    void copy( preprocessing *);

	static bool check_diff( Mat& , int x, int y, int direction );
private:
    Mat    main_image;
    int	   smooth_radius;
    double subtr_val;
    double thresh_coeff;
    static const int iter_num = 20;

    static uchar clamp( double value, uchar min, uchar max );

    void load_image( const char * );
    void unsharped_mask( Mat& );
    void close_reconstruction( Mat& );

    void reconstruction( Mat&, Mat& );
    void artems_filter( Mat& );

    double get_contrast();
    static double get_threshold( Mat& );
};

class neuron_recognition
{
public:
    neuron_recognition();
    neuron_recognition( const neuron_recognition& orig );
    virtual ~neuron_recognition();
    vector< Mat >& recognize( const char * , Mat& , int type = 1 );

    void ray_classify( Mat& proc_image, vector< Mat >& samples, vector< QRect >&, vector< int >& colors );
    void sample_classify( Mat& proc_image, vector< Mat >& samples, vector< QRect >&, vector< int >& colors );
    void classify( vector< QRect >&, vector< int >& colors );
    vector< cluster > get_clusters() {
        return clusters;
    }

	vector< vector< QPoint> >& get_bounds();
	
	neuron_props get_props( int num )
	{
		neuron_props answ;
		answ.area  = ( double ) clusters[ num ].length();
		answ.perim = clusters[ num ].get_bond_length();
		answ.form_fact = answ.perim / ( 2 * 3.14 ) - 1;
		return answ;
	}

	static int round( double value )
	{
		return ( int ) floor( value + 0.5 );
	}

private:
    Mat main_image;
    preprocessing *proc;
    classifier *main_classifier;
    ClassifierCascade *cascade;
    RayFeaturesPrecomputer *features_precomputer;

    vector< cluster > clusters;
	vector< cluster > normalize_cluster;

    static void correct_rectangles( vector< QRect >& rectangles );

    cluster get_normalized_cluster( cluster& priv, QPoint a, QPoint b );
    void divide_clusters( Mat& proc_image, vector< cluster >& white_areas );
    static QPoint get_koeffs( cluster& , cluster& , double& k, double& b );
    static string int2str( int num );
    static void set_line( Mat& proc_image, double k, double b );
    static void set_line( Mat& proc_image, QPoint& );
    static void get_area( Mat& proc_image, Mat& output_image, QPoint seed );
    bool check_core( cluster& sample, cluster& cluster_tmp, Mat& bw_image );
    bool isNeuron( QRect& );
};

#endif // NEURON_RECOGNITION_H
