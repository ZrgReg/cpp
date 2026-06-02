#ifndef FMAIN_H
#define FMAIN_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QMainWindow>
#include <QDateTime>
#include <QPair>
#include <QList>

#include "_unt/QrGen.h"
#include "_unt/QrMng.h"

namespace Ui { class FMain; }

// Главная форма. --------------------------------------------------------------
//------------------------------------------------------------------------------
class FMain : public QMainWindow {
    Q_OBJECT

public:
    explicit FMain(QWidget *parent = 0);
            ~FMain();

    QDateTime bgn;

private slots:
    void gen_complete(QrGen *gen);
    void gen_error(QrGen::ERR code, QString err);

    void on_aExit_triggered();
    void on_aGenQr_triggered();
    void on_aGenQrSet_triggered();
    void on_aTest_triggered();
    void on_aGenQrMlt_triggered();
    void on_aCallDll_triggered();
    void on_aPrint_triggered();
    void on_aQrGen_triggered();
    void on_aXmlParse_triggered();
    void on_aGenToTask_triggered();
    void on_btTask_clicked();

    void on_pushButton_clicked();

    void on_aGenToTaskS_triggered();

private:
    Ui::FMain *ui;
    QList<QrItem> itm;
    QrMng mng;

};// FMain

//------------------------------------------------------------------------------
#endif // FMAIN_H
