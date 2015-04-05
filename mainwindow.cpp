#include "mainwindow.h"
#include "myImage.h"
#include <QRect>
#include <QFileDialog>
#include <QDir>
#include <QChar>
#include <QDateTime>
#include <QPushButton>
#include <QSize>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QKeyEvent>
#include <QDebug>
#include "ExcelFormat_src/BasicExcel.hpp"

#include <fstream>
#include <stdio.h>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <QDirIterator>


#define linear_size 0.01859358

MainWindow :: MainWindow (QWidget *parent) :
    QMainWindow (parent)
{
    setDetectionData(NULL);
    setupUi (this);
    mainImage = new myImage (Ui_MainWindow :: imageWidget);
//	mainImage->setVisible( false );

    connect (actionOpen_File, SIGNAL (triggered ()), this, SLOT (open_file ()));
    //connect (buttonRight, SIGNAL (clicked ()), mainImage, SLOT (move_right ()));
   // connect (buttonLeft, SIGNAL (clicked ()), mainImage, SLOT (move_left ()));
   // connect (buttonUp, SIGNAL (clicked ()), mainImage, SLOT (move_up ()));
    //connect (buttonDown, SIGNAL (clicked ()), mainImage, SLOT (move_down ()));
    connect (actionSave_File, SIGNAL (triggered ()), this, SLOT (save_file ()));
    connect( actionRun, SIGNAL( triggered()), this, SLOT( proc_image() ));
    connect( actionCulc_Stat, SIGNAL( triggered()), mainImage, SLOT( set_flag() ));
    connect( actionChange_view, SIGNAL( triggered()), mainImage, SLOT( change_view()));
	//connect( actionReadData, SIGNAL( triggered()), this, SLOT( read_data()));

    mainImage->setFocus();

    recognizer = new neuron_recognition();

    //DEBUG//
//    QString s = "ImageExamples/main_image.bmp";
//    addFile(s);

//    setDetectionData(& (detectionMap[s.toStdString()]));
//    process_file(s);
}

void MainWindow :: setDetectionData(DetectionData *data){
   detectionData = data;
   if (data != NULL)
   {
       file_name = data->filePath;
       mainImage->setDetectionData(data);
   }
}

MainWindow :: ~MainWindow ()
{
    //delete recognizer; //DEBUG !!!
    //delete mainImage; //DEBUG !!!
    //delete ui; //DEBUG !!!
}

void MainWindow :: resizeEvent (QResizeEvent *)
{
    int width = Ui_MainWindow :: imageWidget->width ();
    int height = Ui_MainWindow :: imageWidget->height ();

    //buttonLeft->setGeometry (10, height / 2, 40, 80);
    //buttonUp->setGeometry (width / 2 - 50, 10, 80, 40);
    //buttonDown->setGeometry (width / 2 - 50, height - 42, 80, 40);
    //buttonRight->setGeometry (width - 80, height / 2, 40, 80);

    mainImage->setGeometry(0, 0, width, height);
}

//______________________SLOTS______________________//


void MainWindow::addFile(QString path) {
    Ui_MainWindow::listWidget->addItem(path);
    detectionMap[path.toStdString()] = DetectionData(path);
}

void MainWindow :: open_file ()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
           this,
           tr("Open Images"),
           QDir::currentPath(),
           tr("Image Files (*.png *.bmp *.jpg)") );

   if( !filenames.isEmpty() )
   {
     qDebug( filenames.join(",").toAscii() );
     for (int i=0; i< filenames.length(); i+=1) {
         addFile(filenames[i]);
     }
   }


    /*QFileDialog* lFileDlg  = new QFileDialog (NULL);
    lFileDlg->setOptions(0);
    lFileDlg->setWindowTitle(tr("Open image"));
    lFileDlg->setFilter(tr("Image Files (*.png *.bmp *.jpg)"));
    if (!lFileDlg->exec() || !lFileDlg->selectedFiles().size())
    {
        delete lFileDlg;
        return;
    }
    QString filepath = lFileDlg->selectedFiles().at(0);
    delete lFileDlg;*/

    //process_file( filepath );
}

void MainWindow :: process_file( QString filepath )
{
    file_name = filepath;

    if (!file_name.isEmpty()) {
        detectionData->rectangles.clear();
        detectionData->old_rectangles.clear();
        detectionData->colors.clear();
        QString postfix = file_name;
        postfix.remove( 0, postfix.length() - 3 );
        mainImage->load_image(file_name);
        mainImage->update ();
    }
}

void MainWindow :: save_file ()
{
#if 0
    save_to_file();
#else
    save_to_xls();
#endif
}

void MainWindow :: save_to_file(  )
{
    /*QDir *save_pos_dir = new QDir ();
    QDir *save_neg_dir = new QDir ();
    string system_query = "dlls\\aFilter.exe ";
    system_query += file_name.toStdString();
    system( system_query.c_str() );


    QString image_name = file_name.remove( file_name.length() - 4, 4 );
    image_name.remove( 0, image_name.length() - 4 );
    QString dir_pos_name =  image_name + "_pos";
    QString dir_neg_name = image_name + "_neg";
    save_pos_dir->mkdir ( dir_pos_name );
    save_neg_dir->mkdir ( dir_neg_name );
    int n = 1, m = 1;

    file_name = "tmp.bmp";
    unsigned int len = detectionData->rectangles.size();
    for( unsigned int i = 0; i < len; ++i ) {
        if( detectionData->colors[ i ] == 2 ) {

            QString output_file_name = dir_pos_name + "\\" + dir_pos_name + int2str( n ).c_str() + ".jpg";
            QImage tmp_image = QImage( file_name ).copy( detectionData->rectangles[ i ]).scaled( 30, 30 );
            tmp_image.save( output_file_name );
            ++n;
        }
        else {

            QString output_file_name = dir_neg_name + "\\" + dir_neg_name + int2str( m ).c_str() + ".jpg";
            QImage tmp_image = QImage( file_name ).copy( detectionData->rectangles[ i ]).scaled( 30, 30 );
            tmp_image.save( output_file_name );
            ++m;
        }
    }*/
    QDir *save_dir = new QDir();
    save_dir->mkdir( "Neurons_coordinates" );
    QDateTime date_time;

    QFileInfo file_info(file_name);
    QString tmp_name = file_info.baseName();

    string out_file_name = "Neurons_coordinates\\" + tmp_name.toStdString() + '_' + date_time.currentDateTime().toString( QString( "dd_MM_yyyy" )).toStdString() + ".txt";
    ofstream OutF( out_file_name.c_str());
    OutF << detectionData->rectangles.size() << endl;
    for ( unsigned int i = 0; i < detectionData->rectangles.size(); ++i )
    {
        if(detectionData->colors[ i ] == 1)
        {
            int x = (detectionData->rectangles[ i ].x() + detectionData->rectangles[ i ].right()) / 2;
            int y = (detectionData->rectangles[ i ].y() + detectionData->rectangles[ i ].bottom()) / 2;
            OutF << "coordinates : " << "x:" << x << " y:" << y << endl;
            neuron_props tmp_props = recognizer->get_props( i );
            OutF << "Area:" <<  tmp_props.area << endl;
            OutF << "Perimeter:" <<  tmp_props.perim << endl;
            OutF << "Form_factor:" <<  tmp_props.form_fact << endl;
            OutF << "Automatic_detection: true" << endl;
            OutF << "===============================================" << endl;
        }
        else if( detectionData->colors[ i ] == 3)
        {
            int x = (detectionData->rectangles[ i ].x() + detectionData->rectangles[ i ].right()) / 2;
            int y = (detectionData->rectangles[ i ].y() + detectionData->rectangles[ i ].bottom()) / 2;
            OutF << "coordinates : " << "x:" << x << " y:" << y << endl;
            OutF << "Automatic_detection: false" << endl;
            OutF << "===============================================" << endl;
        }

    }

    OutF.close();
    delete save_dir;
}

void MainWindow :: save_to_xls( )
{
    QDir *save_dir = new QDir();
    save_dir->mkdir( "Neurons_coordinates" );
    QDateTime date_time;

    QFileInfo file_info(file_name);
    QString tmp_name = file_info.baseName();

    string out_file_name = "Neurons_coordinates\\" + tmp_name.toStdString() + '_' + date_time.currentDateTime().toString( QString( "dd_MM_yyyy" )).toStdString() + ".xls";

    QFile::remove(QString( out_file_name.c_str() ) );

    using namespace YExcel;
    BasicExcel e;
    e.New(1);
    BasicExcelWorksheet* sheet = e.GetWorksheet("Sheet1");
    sheet->Rename(tmp_name.toStdString().c_str());
    sheet->Cell(0, 0)->SetString("Neuron ID");
    sheet->Cell(0, 1)->SetString("pos X");
    sheet->Cell(0, 2)->SetString("pos Y");
    sheet->Cell(0, 3)->SetString("Area");
    sheet->Cell(0, 4)->SetString("Perimeter");
    sheet->Cell(0, 5)->SetString("Formfactor");
    //sheet->Cell(0, 6)->SetString("Automatic Detection");

    unsigned int index = 1;
    for ( unsigned int i = 0; i < detectionData->rectangles.size(); ++i )
    {
        if(detectionData->colors[ i ] == 1)
        {
            sheet->Cell( index, 0)->SetInteger(index);

            int x = (detectionData->rectangles[ i ].x() + detectionData->rectangles[ i ].right()) / 2;
            int y = (detectionData->rectangles[ i ].y() + detectionData->rectangles[ i ].bottom()) / 2;
            sheet->Cell( index, 1)->SetInteger(x);
            sheet->Cell( index, 2)->SetInteger(y);

            neuron_props tmp_props = recognizer->get_props( i );
            sheet->Cell( index, 3)->SetDouble(tmp_props.area);
            sheet->Cell( index, 4)->SetDouble(tmp_props.perim);
            sheet->Cell( index, 5)->SetDouble(tmp_props.form_fact);
            //sheet->Cell( index, 6)->SetString("True");
            ++index;
        }
        else if( detectionData->colors[ i ] == 3)
        {
            sheet->Cell( index, 0)->SetInteger(index);

            int x = (detectionData->rectangles[ i ].x() + detectionData->rectangles[ i ].right()) / 2;
            int y = (detectionData->rectangles[ i ].y() + detectionData->rectangles[ i ].bottom()) / 2;
            sheet->Cell( index, 1)->SetInteger(x);
            sheet->Cell( index, 2)->SetInteger(y);

            //neuron_props tmp_props = recognizer->get_props( i ); //DEBUG
            sheet->Cell( index, 3)->SetString("-");
            sheet->Cell( index, 4)->SetString("-");
            sheet->Cell( index, 5)->SetString("-");
            //sheet->Cell( index, 6)->SetString("False");
            ++index;
        }

    }

    delete save_dir;

    e.SaveAs( out_file_name.c_str() );
}


void MainWindow :: keyPressEvent(QKeyEvent *keyevent)
{
    if(keyevent->isAutoRepeat())
        return;
    //static int index = 1;
    //qDebug() << "keyPressEvent " << index++;
    if(keyevent->key() == Qt::Key_Space)
        mainImage->SetViewMode(true);
    if(keyevent->key() == Qt::Key_Escape)
        QApplication::quit();
}

void MainWindow :: keyReleaseEvent(QKeyEvent *keyevent)
{
    if(keyevent->isAutoRepeat())
        return;

    //static int index = 1;
    //qDebug() << "keyReleaseEvent " << index++;
    if(keyevent->key() == Qt::Key_Space)
        mainImage->SetViewMode(false);
}

void MainWindow :: read_data()
{
	QString tmp_name = file_name;
	int k = 1;
    for ( int i = tmp_name.length() - 1; i >= 0; --i ) {
        if ( tmp_name[ i ] == '.' ) {
            break;
        }
        else {
            ++k;
        }
    }
	if( k < tmp_name.length() ) {
        tmp_name = tmp_name.remove( file_name.length() - k, k );
        int k = 1;
        for ( int i = tmp_name.length() - 1; i >= 0; --i ) {
            if ( tmp_name[ i ] == '/' || tmp_name[ i ] == '\\' ) {
                break;
            }
            else {
                ++k;
            }
        }
        tmp_name = tmp_name.remove( 0, tmp_name.length() - k + 1 );
	}
	tmp_name = "Data\\" + tmp_name + ".txt";

	ifstream hand_data(( const char *)tmp_name.toStdString().c_str());

	int x = 0, y = 0, h = 0, w = 0;
    while( !hand_data.eof() ) {
		hand_data >> x >> y >> w >> h;
		if ( w > 0 && h > 0 ) {
            detectionData->old_rectangles.push_back( QRect( x, y, w, h ));
		}
    }

	/*Mat proc_image;
    vector< Mat > neurons_samples = recognizer->recognize(( const char * )file_name.toStdString().c_str(), proc_image );
	*/
	mainImage->update();
}

void MainWindow :: proc_image()
{
    Mat proc_image;
    vector< Mat > neurons_samples = recognizer->recognize(( const char * )file_name.toStdString().c_str(), proc_image );
    recognizer->sample_classify( proc_image, neurons_samples, detectionData->rectangles, detectionData->colors );
    detectionData->boundaries = recognizer->get_bounds();
    //recognizer->classify( detectionData->rectangles, detectionData->colors );
	/*string system_query = "dlls\\aFilter.exe ";
	system_query += file_name.toStdString();
	system( system_query.c_str() );*/

    if( proc_image.data ) {
       // mainImage->load_image( proc_image );
        mainImage->update ();
        proc_image.release();
    }
    /* //DEBUG !!!
    ofstream outF( "/media/DATA/Repos/Matlab/Neurons/Images_test/results" );
    for( unsigned int i = 0; i < detectionData->rectangles.size(); ++i ) {
        outF << detectionData->rectangles[ i ].x() << '\t' << detectionData->rectangles[ i ].y() << '\t' <<
                detectionData->rectangles[ i ].width() << '\t' << detectionData->rectangles[ i ].height() << endl;
    }

    outF.close();*/
}

int MainWindow :: str2int( char *str_num )
{
    int res = 0;
    for ( int i = 0; str_num[ i ] != '\0'; ++i ) {
        res = res * 10 + static_cast< int >( str_num[ i ] ) - 48;
    }

    return res;
}

string MainWindow :: int2str( int num )
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

void MainWindow::on_actionOpen_folder_triggered()
{
    QString dirname = QFileDialog::getExistingDirectory(
            this,
            tr("Select a Directory"),
            QDir::currentPath() );

    if( !dirname.isNull() )
    {
        QDirIterator it(dirname, QStringList() << "*.jpg" << "*.png" << "*.bmp", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            addFile(it.next());
            // qDebug() << it.next();
        }
      // qDebug( dirname.toAscii() );
    }
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QString path = current->text();
    setDetectionData(& (detectionMap[path.toStdString()]));

    setFocus();
}

void MainWindow::on_splitter_splitterMoved(int pos, int index)
{
    resizeEvent(NULL);
    //mainImage->paintEvent(NULL);
}

void MainWindow::updateImage()
{
    resizeEvent(NULL);
}
