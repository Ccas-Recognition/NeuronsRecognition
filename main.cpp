#include <vector>
#include <iostream>
#include "ClassificationEstimations.h"

/**
 * Входная точка в программу.
 */


int main(int argc, char *argv[])
{
    using namespace std;
    using namespace cv;

    ClassificationEstimations estimations(1);

    if( argc < 3 )
    {
        cout << "Input: fg_path bg_path" << endl;
        return 1;
    }

    vector< Mat > fg_parts, bg_parts;
    estimations.getParts(fg_parts, argv[1]);
    estimations.getParts(bg_parts, argv[2]);

    cout << "First Kind Error: " << estimations.getError( fg_parts, 1 ) << endl;
    cout << "Second Kind Error: " << estimations.getError( bg_parts, 0 ) << endl;
    return 0;
}
