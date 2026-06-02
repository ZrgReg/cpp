#include <QApplication>

#include "FMain.h"
#include "WLog.h"
#include "_wgt/WImg/WImg.h"

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
WLog  *wgLog;
WImg  *wgImg;
FMain *fmMain;

// Главная функция. ------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    WLog _wgLog; wgLog = &_wgLog;
    WImg _wgImg; wgImg = &_wgImg;
    FMain _fmMain; fmMain = &_fmMain;
    fmMain->show();

    return app.exec();
}// main

//------------------------------------------------------------------------------
