#ifndef _MYIMAGE_H
#define	_MYIMAGE_H

#include <QtGui/QWidget>
#include <opencv2/core/core.hpp>
#include <vector>
#include "DetectionData.h"

using namespace std;
using namespace cv;

class QImage;
class QPixmap;
class QRect;

/**
 *Класс, описывающий вывод изображения на главное окно приложения.
 */
class myImage : public QWidget {

    Q_OBJECT

public:
	//Конструктор класса.
    myImage (QWidget *parent = 0);

    void setDraw ( bool );
	//Функции, реализующие загрузку разных структур данных для вывода на главное окно приложения.
    void load_image (QString& );
    void load_image (QImage *);
    void load_image( Mat& );
    vector< QPoint >& get_lines();

    void SetViewMode(bool value);

    QImage *get_main_im ();
    QImage *get_mask_im ();

    void setDetectionData(DetectionData * data);

signals:
    void set_rect_to_table ();

private slots:
    void zoom(float, float, float);

    void update_data ();

    void set_white_color ();
    void set_black_color ();

    void set_flag ();
	void change_view();
private:
	//Исходное изображение.
    QImage *picture;
	//Масштабированное изображение.
    QPixmap *map_picture;

	//Вспомогательные структуры данных.
    QImage *mask_image;
    QPixmap *mask_map;

    QImage *main_image;

    QImage def_pict;
    QImage zoom_pict;

    int r, g, b;
    int x, y;
    bool draw_flag;
    bool limit_flag;
	bool view_flag;

    bool view_mode_drag;
    bool view_mode_on;
    int initial_mid_x, initial_mid_y, initial_first_mid_x, initial_first_mid_y;
    int initial_bot_x, initial_bot_y;

    int w, h;
    DetectionData * detectionData;
    vector< QPoint > lines;

    int x_initial, y_initial, x_last, y_last;

    double zoom_koeff;
    int mid_x, mid_y, first_mid_x, first_mid_y;
    int bot_x, bot_y;
    int old_height, old_width;

    static const int step = 20;

    void del_rect (int , int );
    void change_color( int, int );

    static void Mat2QImage( Mat& , QImage* & );
    static double vect_prod( QPoint , QPoint , QPoint );
    void release_rectangles();

protected:
    void paintEvent (QPaintEvent *paintEvent);
    void resizeEvent (QResizeEvent *);

    void mousePressEvent (QMouseEvent *mouseEvent);
    void mouseDoubleClickEvent( QMouseEvent *mouseEvent );
    void mouseReleaseEvent (QMouseEvent *mouseEvent);
    void mouseMoveEvent (QMouseEvent *mouseEvent);
    void wheelEvent(QWheelEvent* event);

    void SaveInitialCoordinates();
};

#endif	/* MYIMAGE_H */
