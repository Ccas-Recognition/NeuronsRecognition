#ifndef GRAY_CO_MAT_H
#define	GRAY_CO_MAT_H
/**
 * Structure with properties of gray-level co-occurrence matrix.
 */
struct gray_props {

    gray_props( double ent = 0.0, double corr = 0.0, double contr = 0.0,
            double hom = 0.0, double en = 0.0 )
        : entropy( ent ), correlation( corr ), contrast( contr ), homogeneity( hom ),
            energy( en ) {}

    gray_props( const gray_props& orig )
    {
        this->entropy = orig.entropy;
        this->correlation = orig.correlation;
        this->contrast = orig.contrast;
        this->homogeneity = orig.homogeneity;
        this->energy = orig.energy;
    }
    /**
     * Entropy is a statistical measure of randomness that can be
     * used to characterize the texture of the input image.  Entropy is defined
     * as -sum( p[ i ] * log2( p[ i ])) where p contains the histogram.
     */
    double entropy;
    /**
     * Statistical measure of how correlated a pixel is to its
     * neighbor over the whole image. Range = [-1 1].
     * Correlation is 1 or -1 for a perfectly positively or
     * negatively correlated image. Correlation is NaN for a
     * constant image.
     */
    double correlation;
    /**
     * The intensity contrast between a pixel and its neighbor
     * over the whole image. Range = [0 (width-1)^2].
     * Contrast is 0 for a constant image.
     */
    double contrast;
    /**
     * closeness of the distribution of elements in the GLCM to
     * the GLCM diagonal. Range = [0 1]. Homogeneity is 1 for
     * a diagonal GLCM
     */
    double homogeneity;
    /**
     * summation of squared elements in the GLCM. Range = [0 1].
     * Energy is 1 for a constant image
     */
    double energy;
};

/**
 *
 * @param _offset
 * @param _num_levels
 * @param _symmetric
 *
 * Gray level co-occurance matrix class.
 * Imlements the getting properties of matrix.
 */
class gray_co_mat {
public:
    gray_co_mat( int _offset[], int _num_levels = 16, bool _symmetric = true );
    gray_co_mat( const gray_co_mat& orig );
    virtual ~gray_co_mat();
    /**
     * Load data.
     * @param _width
     * @param _height
     * @param _image
     */
    void load_image( unsigned int _width, unsigned int _height, int **_image );
    /**
     * Function calculates the main normalize matrix.
     */
    gray_props *culc_matrix();
    /**
     * Culculates properties of main gray mtrix.
     * @return properties
     */
    gray_props *culc_props();

    static const int NUM_FEATS = 5;
private:
    /**
     * Geometrical sizes of image.
     */
    unsigned int width, height;
    /**
     * Input gray-level matrix.
     */
    int **image;
    /**
     * Main gray-level co-occurrence matrix.
     */
    double **gray_mat;
    /**
     * A Boolean that creates a GLCM where the ordering of
     * values in the pixel pairs is not considered. For
     * example, when calculating the number of times the
     * value 1 is adjacent to the value 2, GRAYCOMATRIX
     * counts both 1,2 and 2,1 pairings, if 'Symmetric' is
     * set to true. When 'Symmetric' is set to false,
     * GRAYCOMATRIX only counts 1,2 or 2,1, depending on the
     * value of 'offset'. The GLCM created in this way is
     * symmetric across its diagonal, and is equivalent to
     * the GLCM described by Haralick (1973).
     */
    bool symmetric;
    /**
     * A 2-vector of offsets specifying the distance
     * between the pixel-of-interest and its neighbor.
     */
    int offset[ 2 ];
    int symm_offset[ 2 ];
    /**
     * An integer specifying the number of gray levels to use when
     * scaling the grayscale values in I. For example, if
     * 'NumLevels' is 8, GRAYCOMATRIX scales the values in I so
     * they are integers between 1 and 8.  The number of gray levels
     * determines the size of the gray-level co-occurrence matrix
     */
    int num_levels;
    /**
     * Function culculates grayco matrix and returns
     * coefficient of normalization.
     * @param offset
     * @return coefficient of normalization
     */
    int culc_offset_mat( int *offset );

    int scale( int val );

    /*_________Calculation of properties______________*/
    double get_entropy();
    double get_contrast();
    double get_corr();
    double get_hom();
    double get_energy();
};

#endif	/* GRAY_CO_MAT_H */

