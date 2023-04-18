#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QFileDialog>
#include <QLabel>


//class QGraphicsScene;

class Widget : public QWidget
{
    Q_OBJECT

public slots:

    void _transform();
    void LoadButtonClicked();
    void ShowImage();
    void SaveImage();
//    void Search();
//    void onOkButtonClicked();

public:

    Widget(QWidget *parent = nullptr);
    ~Widget();

    QSize size;

    int _width = 800;
    int _height = 500;

    QLabel* width_height;
    QLabel* label;

//    QLabel* info;

    QString _Width ;
    QString file_name;
    QString _targetBrightness;

    QPixmap QPixmap_filteredImage;
    QPixmap SavePixmap;
    QPixmap imgs;
    QImage _image;
    QImage m_filteredImage;

    QGraphicsScene *scene_for_view = nullptr;
    QGraphicsView * view = nullptr;
};

#endif // WIDGET_H
