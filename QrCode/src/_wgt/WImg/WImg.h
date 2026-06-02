#ifndef WIMG_H
#define WIMG_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QWidget>

// NAMESPACE. ------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace Ui { class WImg; }

/* Вывод изображения. *********************************************************/
/******************************************************************************/
class WImg : public QWidget {
    Q_OBJECT

public:
    explicit WImg(QWidget *parent = 0);
            ~WImg();

    void setPixmap(const QPixmap &img);
    void showPixmap(const QPixmap &img) { setPixmap(img); show(); }

private:
    Ui::WImg *ui;
};

#endif // WIMG_H
