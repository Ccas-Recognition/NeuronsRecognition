#include <QFileInfo>
#include <QDir>

#include "ClassificationEstimations.h"

#include "LabeledSampleElement.h"
#include "RayFeaturesPrecomputer.h"
#include "RayFeatureType.h"
#include "RayFeaturesEnumerator.h"
#include "BoostingClassifier.h"
#include "ClassifierCascade.h"
#include "neuron_recognition.h"

using namespace cv;

ClassifierCascade *get_classifier2( const char *classifier_filename ) {

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

RayFeaturesPrecomputer *get_precomputer2( const char *classifier_filename ) {

    cv :: FileStorage fs_cascade;
    fs_cascade.open( classifier_filename, cv :: FileStorage :: READ );

    RayFeaturesPrecomputer *features_precomputer =
                        new RayFeaturesPrecomputer(1, std::vector<int>(1, 255));
    features_precomputer->read( fs_cascade[ "features_precomputer" ]);

    fs_cascade.release();

    return features_precomputer;
}


ClassificationEstimations::ClassificationEstimations(int _sparce_number)
{
    sparce_number = _sparce_number;
    cascade = get_classifier2( "./ClassifCoeffs/classifier_1000_700_97_4.xml" );
    features_precomputer = get_precomputer2( "./ClassifCoeffs/classifier_1000_700_97_4.xml" );
}


void ClassificationEstimations::getParts( vector<Mat> &parts, string xml_path )
{
    cv::FileStorage storage( xml_path, FileStorage::READ);

    QFileInfo info( xml_path.c_str() );
    string dir_path = info.absoluteDir().absolutePath().toStdString();

    FileNode images;
    images = storage["images"];
    cout << "Parts Size:" << images.size() << endl;
    parts.reserve( images.size() );
    int index = 0;
    for( FileNodeIterator it = images.begin(); it != images.end(); ++it, ++index )
    {
        if(index%1000 == 0)
            cout << "index: " << index << endl;

       if( index%sparce_number != 0)
           continue;

       string part_path = ( dir_path + "/" + string(*it));
       Mat part_image = imread( part_path );
       resize( part_image, part_image, Size( 24, 24 ));
       parts.push_back(part_image);
#if 0
       imshow("image_part", part_image);
       waitKey(0);
#endif
       //cout << part_path;
    }
}

int ClassificationEstimations::classify( const Mat& part )
{
    FEATS features = features_precomputer->precompute( part );
    LabeledSampleElement_ptr sample_element_ptr =
          LabeledSampleElement_ptr( new LabeledSampleElement( features, true ));

    return int( cascade->classify( sample_element_ptr ) );
}

float ClassificationEstimations::getError( const vector<Mat> &parts, int answer )
{
    int error_count = 0;

    for( unsigned int i=0; i<parts.size(); ++i )
    {
        if( classify(parts[i]) != answer )
        {
            ++error_count;
#if 0
            {
                stringstream filename;
                filename << "dump/error_image_" << answer << "_" << error_count << ".png";
                cout << filename.str() << endl;
                imwrite(filename.str(), parts[i]);
            }
#endif
        }
    }
    return float(error_count)/parts.size();
}
