#include "gray_co_mat.h"
#include <string.h>
#include <cmath>
#include <stdlib.h>
#include <fstream>

gray_co_mat :: gray_co_mat (  int _offset[], int _num_levels, bool _symmetric )
    : num_levels( _num_levels ), symmetric( _symmetric )
{
    offset[ 0 ] = _offset[ 0 ];
    offset[ 1 ] = _offset[ 1 ];
    if ( symmetric == true ) {
        symm_offset[ 0 ] = -_offset[ 0 ];
        symm_offset[ 1 ] = -_offset[ 1 ];
    }

    gray_mat = new double* [ num_levels ];
    for( int i = 0; i < num_levels; ++i ) {
        gray_mat[ i ] = new double[ num_levels ];
        memset( gray_mat[ i ], 0.0, num_levels * sizeof( double ));
    }
}

void gray_co_mat :: load_image( unsigned int _width, unsigned int _height, int **_image )
{
    width = _width;
    height = _height;
    image = _image;

    if ( gray_mat != NULL ) {
        for( int i = 0; i < num_levels; ++i ) {
            memset( gray_mat[ i ], 0, num_levels * sizeof( double ));
        }
    }
}

gray_co_mat :: gray_co_mat( const gray_co_mat& orig )
{
}

gray_co_mat :: ~gray_co_mat()
{
    image = NULL;
    for( int i = 0; i < num_levels; ++i ) {
        delete[] gray_mat[ i ];
    }

    delete gray_mat;
}

gray_props *gray_co_mat :: culc_matrix()
{
    int norm_sum = culc_offset_mat( offset );

    if ( symmetric == true ) {
        culc_offset_mat( symm_offset );
        norm_sum *= 2;
    }
//
   // std :: ofstream outF( "output" );
    for( int i = 0; i < num_levels; ++i ) {
        for( int j = 0; j < num_levels; ++j ) {
            gray_mat[ j ][ i ] = gray_mat[ j ][ i ] / ( 1.0 * norm_sum );
  //          outF << gray_mat[ j ][ i ] << '\t';
        }
    //    outF << std :: endl;
    }

    return culc_props();
}

int gray_co_mat :: culc_offset_mat( int* offset )
{
    int norm_sum = 0;
    int first_pix_val = 0, second_pix_val = 0;
    int scale_first_val = 0, scale_second_val = 0;
    int neib_x = 0, neib_y = 0;
   // std :: ofstream outF( "gray_matrix" );

    for( unsigned int j = 0; j < height; ++j ) {
        for( unsigned int i = 0; i < width; ++i ) {
            first_pix_val = image[ i ][ j ];
           // outF << scale( first_pix_val ) + 1 << '\t';

            neib_x = i + offset[ 0 ];
            neib_y = j + offset[ 1 ];
            if( neib_x >= 0 && neib_x < width && neib_y >= 0 && neib_y < height ) {
                ++norm_sum;
                second_pix_val = image[ neib_x ][ neib_y ];

                scale_first_val = scale( first_pix_val );
                scale_second_val = scale( second_pix_val );
                gray_mat[ scale_first_val ][ scale_second_val ] =
                        gray_mat[ scale_first_val ][ scale_second_val ] + 1.0;
            }
        }
        //outF << std :: endl;
    }

    return norm_sum;
}

int gray_co_mat :: scale( int val )
{
    double tmp_res = val * ( num_levels - 1 ) / 255.0 ;
    return ( int ) tmp_res;
}

double gray_co_mat :: get_entropy()
{
    double result = 0.0;

    for ( unsigned int j = 0; j < num_levels; ++j ) {
        for ( unsigned int i = 0; i < num_levels; ++i) {
            if ( gray_mat[ i ][ j ] != 0.0 ) {
                result -= gray_mat[ i ][ j ] * log( gray_mat[ i ][ j ]) * 1.0;
            }
        }
    }

    return result;
}

double gray_co_mat :: get_contrast()
{
    double result = 0.0;

    for ( unsigned int j = 0; j < num_levels; ++j ) {
        for ( unsigned int i = 0; i < num_levels; ++i) {
            result += ( i - j ) * ( i - j ) * gray_mat[ i ][ j ] * 1.0;
        }
    }
    
    return result;
}

double gray_co_mat :: get_corr()
{
    return 0.0;
}

double gray_co_mat :: get_hom()
{
    double result = 0.0;

    for ( unsigned int j = 0; j < num_levels; ++j ) {
        for ( unsigned int i = 0; i < num_levels; ++i) {
            result += gray_mat[ i ][ j ] / ( 1.0 + abs(( int )( i - j )) * 1.0 );
        }
    }

    return result;
}

double gray_co_mat :: get_energy()
{
    double result = 0.0;

    for ( unsigned int j = 0; j < num_levels; ++j ) {
        for ( unsigned int i = 0; i < num_levels; ++i) {
            result += gray_mat[ i ][ j ] * gray_mat[ i ][ j ];
        }
    }

    return result;
}


gray_props *gray_co_mat :: culc_props()
{
    gray_props *main_props = new gray_props();

    main_props->contrast = get_contrast();
    main_props->correlation = get_corr();
    main_props->energy = get_energy();
    main_props->homogeneity = get_hom();
    main_props->entropy = get_entropy();
    
    return main_props;
}
