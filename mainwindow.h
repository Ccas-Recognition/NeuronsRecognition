#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <string>
#include "ui_mainwindow.h"
#include "neuron_recognition.h"
#include "DetectionData.h"


class myImage;
class QRect;
class QString;
class QPushButton;

using namespace std;
using namespace cv;
/**
 *В данном классе реализована отрисовка основных элементов главного окна приложения
 */
class MainWindow : public QMainWindow, public Ui :: MainWindow
{
    Q_OBJECT

public:
	//Конструктор класса.
    explicit MainWindow( QWidget *parent = NULL );
    ~MainWindow();

    static int str2int( char *str_num );
    static string int2str( int num );
    void process_file ( QString );

    void setDetectionData(DetectionData * data);
private slots:
	//Функция, реализующая выбор и загрузку изображения в программу.
    void open_file ();
	//Фуекция, реализиющая сохранение выходных данных на жесткий диск.
    void save_file ();
	//Фуекция, запускающая основной метод программы.
    void proc_image();

	void read_data();

protected:
    void resizeEvent( QResizeEvent * );
    void keyPressEvent(QKeyEvent *keyevent);
    void keyReleaseEvent(QKeyEvent *keyevent);

    void save_to_file( );
    void save_to_xls( );
private:
    Ui :: MainWindow *ui;

    neuron_recognition *recognizer;

    myImage *mainImage;
    QString file_name;
    DetectionData * detectionData;
};

#endif // MAINWINDOW_H
