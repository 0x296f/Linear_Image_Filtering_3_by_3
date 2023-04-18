#include "widget.h"

#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPixmap>
#include <QMainWindow>
#include <QFileDialog>
#include <map>
#include <qDebug>
#include <QLineEdit>
#include <QtNetwork/QNetworkAccessManager>
#include <QMessageBox>
#include <QDesktopServices>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtMath>


void Widget::SaveImage()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png *.xpm *.jpg)"));
    QPixmap_filteredImage.save(filePath);
}

void QGraphicsSceneSettings(QGraphicsView* view, int width, int height){
    view->setFixedSize(width, height);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void createButton(QPushButton *button, int width, int height , QGridLayout* body, int a ,int b,int c, int d){
    button->setFixedSize(width, height);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    body->addWidget(button, a, b, c, d);
}

QImage filterImage(QImage inputImage)
{
    int width = inputImage.width();
    int height = inputImage.height();

    QImage outputImage(width, height, QImage::Format_RGB32);

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            QRgb pixel00 = inputImage.pixel(x - 1, y - 1);
            QRgb pixel01 = inputImage.pixel(x, y - 1);
            QRgb pixel02 = inputImage.pixel(x + 1, y - 1);
            QRgb pixel10 = inputImage.pixel(x - 1, y);
            QRgb pixel11 = inputImage.pixel(x, y);
            QRgb pixel12 = inputImage.pixel(x + 1, y);
            QRgb pixel20 = inputImage.pixel(x - 1, y + 1);
            QRgb pixel21 = inputImage.pixel(x, y + 1);
            QRgb pixel22 = inputImage.pixel(x + 1, y + 1);

            int red = (qRed(pixel00) + qRed(pixel01) + qRed(pixel02) +
                       qRed(pixel10) + qRed(pixel11) + qRed(pixel12) +
                       qRed(pixel20) + qRed(pixel21) + qRed(pixel22)) / 9;
            int green = (qGreen(pixel00) + qGreen(pixel01) + qGreen(pixel02) +
                         qGreen(pixel10) + qGreen(pixel11) + qGreen(pixel12) +
                         qGreen(pixel20) + qGreen(pixel21) + qGreen(pixel22)) / 9;
            int blue = (qBlue(pixel00) + qBlue(pixel01) + qBlue(pixel02) +
                        qBlue(pixel10) + qBlue(pixel11) + qBlue(pixel12) +
                        qBlue(pixel20) + qBlue(pixel21) + qBlue(pixel22)) / 9;

            outputImage.setPixel(x, y, qRgb(red, green, blue));
        }
    }
    return outputImage;
}

void Widget::_transform()
{
      QPixmap_filteredImage = QPixmap::fromImage(filterImage(_image));

      scene_for_view->clear();
      QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap_filteredImage); // создаем тут QGraphicsPixmapItem, и не забываем очистить scen перед закрытием ПО
      scene_for_view->addItem(item);
      view->setScene(scene_for_view);
      view->setRenderHint(QPainter::Antialiasing); // сглаживание
      view->setDragMode(QGraphicsView::ScrollHandDrag); // включаем режим перетаскивания

      scene_for_view->setSceneRect(0, 0,_width , _height); //  размер сцены
}

void Widget::LoadButtonClicked()
{
    file_name = QFileDialog::getOpenFileName(this);
    imgs.load(file_name);
    _image.load(file_name);
}

void Widget::ShowImage()
{
    scene_for_view->clear();
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(imgs); // создаем тут QGraphicsPixmapItem, и не забываем очистить scen перед закрытием ПО
    scene_for_view->addItem(item);
    view->setScene(scene_for_view);

    view->setRenderHint(QPainter::Antialiasing); // сглаживание
    view->setDragMode(QGraphicsView::ScrollHandDrag); // включаем режим перетаскивания

    scene_for_view->setSceneRect(0, 0,_width , _height); //  размер сцены

    view->fitInView(scene_for_view->sceneRect(), Qt::KeepAspectRatio);// масштабируем сцену

     size = imgs.size();
    _width = size.width();
    _height = size.height();
    width_height->setText( "X:" + QString::number(_width)+ "  Y:" + QString::number(_height) );
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout* body = new QGridLayout(this);

    QPushButton* open = new QPushButton("Open" );
    QPushButton* save = new QPushButton("Save");
    QPushButton* show = new QPushButton("Show");
    QPushButton* okButton = new QPushButton("OK");
    QPushButton* search = new QPushButton("Search");
    QPushButton* transform = new QPushButton("Transform");

    label = new QLabel(this);
//    QLineEdit* lineEdit = new QLineEdit(this);
//    info = new QLabel(this);
    width_height = new QLabel(this);

    createButton(save, 75 ,30, body , 1, 2, 1, 1);
    createButton(show, 75 ,30, body , 1, 3, 1, 1);
    createButton(open, 75 ,30, body , 2, 2, 1, 1);
    createButton(transform, 75 ,30, body , 2, 3, 1, 1);

//    search->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    search->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    search->setFixedSize(75, 30);
//    body->addWidget(search, 5, 2, 1, 1);

//    okButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    okButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    okButton->setFixedSize(75, 30);
//    body->addWidget(okButton, 6, 3, 1, 1);

//    body->addWidget(lineEdit,6 , 2 ,1 ,1 );
//    lineEdit->setFixedSize(75, 30);

//    body->addWidget(info, 6 , 1,1 , 1);
    label->setPixmap(imgs);

    body->addWidget(width_height, 7 , 1,1 , 1);

    connect(open, SIGNAL(clicked()), this, SLOT(LoadButtonClicked()));
    connect(show, SIGNAL(clicked()), this, SLOT(ShowImage()));
    connect(save, SIGNAL(clicked()), this, SLOT(SaveImage()));
//    connect(search, SIGNAL(clicked()), this, SLOT(Search()));
//    connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
    connect(transform, SIGNAL(clicked()), this, SLOT(_transform()));

    view = new QGraphicsView(this);
    scene_for_view = new QGraphicsScene(view);
    view->setScene(scene_for_view);
    scene_for_view->setSceneRect(0, 0, 600, 700);
    QGraphicsSceneSettings(view, 1024, 768);
    QRectF rect = view->sceneRect();

    body->addWidget(view, 1, 1, 5, 1);
    setLayout(body);

}

Widget::~Widget()
{
    scene_for_view->clear();
}

//void Widget::onOkButtonClicked()
//{
//       QLineEdit *lineEdit = findChild<QLineEdit*>(); // не знаю как работает, это я просто украл
//       _targetBrightness = lineEdit->text();
//}

//void Widget::Search()
//{
//    int targetBrightness = _targetBrightness.toInt();
//    QPoint targetPixel(-1, -1);

//    for (int i = 0; i < _image.width(); i++)
//    {
//        for (int j = 0; j < _image.height(); j++)
//        {
//            QRgb color = _image.pixel(i, j);
//            int brightness = qGray(color);

//            if (brightness == targetBrightness)
//            {
//                targetPixel.setX(i);
//                targetPixel.setY(j);
//                qDebug() << "Target pixel found at:" << targetPixel;
//            }

//        }
//    }

//}




