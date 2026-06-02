#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QStringList>
#include <QLibrary>
#include <QXmlStreamReader>
#include <QTextCodec>
#include <QFileDialog>
#include <QInputDialog>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMatrix>

#include "_lib/QrCodeGenerator/QrCode.hpp"

#include "_srv/Srv/tme.h"
#include "_srv/Srv/std.h"
#include "_srv/Srv/dbg.h"
#include "_srv/Srv/xml.h"
#include "_tst/XmlParse.h"

#include "ui_FMain.h"
#include "FMain.h"
#include "WLog.h"
#include "_wgt/WImg/WImg.h"

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

// Глобальные переменные. ------------------------------------------------------
//------------------------------------------------------------------------------
extern WLog *wgLog;
extern WImg *wgImg;

/* Дополнительные функции. ****************************************************/
/******************************************************************************/

// Отрисоака QR-кода. ----------------------------------------------------------
//------------------------------------------------------------------------------
void paintQR(QPainter &painter, const QSize sz, const QString &dta, QColor fg) {
    FNC << R"(/ bgn)";

    char *str = dta.toUtf8().data();
    qrcodegen::QrCode qr =
        qrcodegen::QrCode::encodeText(str, qrcodegen::QrCode::Ecc::LOW);

    const int s = (qr.size > 0 ? qr.size : 1);
    const double w = sz.width();
    const double h = sz.height();
    const double aspect = w/h;
    const double size = ((aspect > 1.0) ? h : w);
    const double scale = size/(s+2);

    painter.setPen(Qt::NoPen);
    painter.setBrush(fg);

    for(int y=0; y<s; y++) {
        for(int x=0; x<s; x++) {
            const int color = qr.getModule(x, y);  // 0 for white, 1 for black
            if(0 != color) {
                const double rx1 = (x+1)*scale, ry1 = (y+1)*scale;
                QRectF r(rx1, ry1, scale, scale);
                painter.drawRects(&r,1);
            }// if(0 != color)
        }// x
    }// y

    FNC << R"(\ end)";
}// paintQR

/* FMain **********************************************************************/
/******************************************************************************/

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::FMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FMain) {
    ui->setupUi(this);
    this->setMaximumHeight(0);
}// FMain

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
FMain::~FMain() {
    delete ui;
}// ~FMain

// Выход. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aExit_triggered() {
    if(QMessageBox::Yes == QMessageBox::question(
        this, "Подтверждение.", "Действительно выйти ?"
      , QMessageBox::Yes | QMessageBox::No ))
    { QApplication::quit(); }
}// on_aExit_triggered

// Проба -> Генерация QR-кода. -------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aGenQr_triggered() {
    QString txt = QInputDialog::getText(
        this, "Ведите текст для кодирования", "Текст:", QLineEdit::Normal
      , "Южно-эфиопский грач, увёл мышь за хобот на съезд ящериц." );
    QPainter pnt;
    QColor clr(Qt::black);
    QPixmap img(300, 300); img.fill();
    QSize sze(300, 300);

    pnt.begin(&img);
        paintQR(pnt, sze, txt, clr);
    pnt.end();

    wgImg->showPixmap(img);
}// on_aGenQr_triggered

// Генерация набора QR_кодов. --------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aGenQrSet_triggered() {
    FNC << "bgn";

    static const int LEN = 10, SZE = 480;
    static const QString PATH("d:/temp/13/1/%1.jpg");
    static const QString TEXT(
        "Южно-эфиопский грач, увёл мышь за хобот на съезд ящериц {%1}." );

    itm.clear();
    for(int i=0; i<LEN; i++){
        itm.append(QrItem(
            PATH.arg(STR(i)), TEXT.arg(STR(i)), SZE

          ,(i%3 == 0 ? MARGIN::NOT
          : i%3 == 1 ? MARGIN::YES : MARGIN::TRP)

          , Qt::transparent, Qt::white, Qt::black
        ));
    }// i

    QrGen *gen = new QrGen(this, itm);

    connect(gen, &QrGen::ready, this, &FMain::gen_complete);
    connect(gen, &QrGen::error, this, &FMain::gen_error);
    connect(gen, &QrGen::finished, gen, &QObject::deleteLater);

    this->bgn = SYSDATE;

    gen->start();

    FNC << "end";
}// on_aGenQrSet_triggered

// Проба. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTest_triggered() {
    FNC << R"(/ bgn)";

    int CNT = 1000, LNG = 500;

    QStringList bld;
    QString chr("абвгдеёжзиклмнопрстуфхцчшщъыьэюя.01234567890");

    for(int i=0; i<CNT; i++) {
        QString txt;
        for(int j=0; j<LNG; j++) { txt += chr[qrand()%chr.length()]; }
        bld.append(txt);
    }// i

    QDateTime bgn(QDateTime::currentDateTime());

    for(QString &string: bld) {
        char *str = string.toUtf8().data();
        qrcodegen::QrCode::encodeText(str, qrcodegen::QrCode::Ecc::LOW);
    }// str

    FNC << "| tme:" << bgn.msecsTo(QDateTime::currentDateTime()) << "msec";

    FNC << R"(\ end)";
}// on_aTest_triggered

// Генерация кодов завершена. --------------------------------------------------
//------------------------------------------------------------------------------
void FMain::gen_complete(QrGen *gen) {
    DBG << "Генерация кодов завершена за:" << bgn.msecsTo(SYSDATE) << " msec";
}// gen_complete

// Ошибка при генерации кодов. -------------------------------------------------
//------------------------------------------------------------------------------
void FMain::gen_error(QrGen::ERR, QString err) {
    DBG << QString("Ошибка: %1,при генерации кодов.").arg(err);
}// gen_error

// Многопоточная генерация QR-кодов. -------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aGenQrMlt_triggered() {
    FNC << "bgn";

    int LEN = 1000, SZE = 400, MRG = 0, CRE = 1;
    QStringList bld;
    QString PATH("d:/temp/13/1/%1.png");
    QString CODE = R"(<code text="%1" fnme="%2" size="%3" mrgn = "%4"/>)";
    QString TEXT =
        "Южно-эфиопский грач, увёл мышь за хобот на съезд ящериц {%1}.";

    for(int i=0; i<LEN; i++) {
        QString txt;
        for(int i=0; i<9; i++) { txt += TEXT.arg(STR(i)); }

        bld.append(CODE.arg(
            txt, PATH.arg(STR(i))
          , STR(SZE + ((SZE * (double)qrand() / (double)RAND_MAX) - (SZE/2)))
          , STR((i%3 == 0 ? 0: i%3 == 1 ? 1 : 2))
        ));
    }// i

/*
    QString xml = TAG("root", bld.join("\n"));
    std::string str = xml.toStdString();
*/
    QFile file(ui->edTask->text());
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        wgLog->add(QString("fle_err:") + "Not loaded task (*.xml).");
        return;
    }// if(!file.open(QIODevice::ReadOnly | QFile::Text))

    QTextStream inp(&file);
    std::string str = inp.readAll().toStdString();

    this->bgn = SYSDATE;

    mng.GenerateQR(str.c_str(), CRE, SKIP, SZE, MRG, 1, -1, 0);
    for(int i=0; i<mng.numErr(); i++)
        { FNC << "one:" << mng.lstErr("UTF-8")[i].text; }

    FNC << "Генерация кодов завершена за:"
        << bgn.msecsTo(SYSDATE) << " msec";

    if(mng.numErr() != 0) {
        const GenError *err = mng.lstErr("UTF-8");
        for(int i=0; i<mng.numErr(); i++) { FNC << "err:" << err[i].text; }
    }// if(mng.numErr() != 0)

    FNC << "end";
}// on_aGenQrMlt_triggered

// Вызов DLL. ------------------------------------------------------------------
//------------------------------------------------------------------------------

typedef int  (*VERSIOn)(void);
typedef void (*INITIAL)(void);
typedef void (*DESTROY)(void);
typedef int  (*EXECUTE)(const char *xml, int size, int threads, int mode);
typedef int  (*PERFORM)(const char *xml, int size, int margin, int threads, int mode);
typedef int  (*ADVANCE)(const char *xml, int size, int margin, int threads, int mode, int img_format, int img_quality, int err_correction);
typedef int  (*ERR_CNT)(void);
typedef void*(*ERR_TXT)(const char*);

void FMain::on_aCallDll_triggered() {
    QLibrary lib;
    lib.setFileName("qr_gen.dll");

    DBG << "err:" << lib.errorString();

    VERSIOn version = (VERSIOn)lib.resolve("version");
    INITIAL initial = (INITIAL)lib.resolve("initial");
    DESTROY destroy = (DESTROY)lib.resolve("destroy");
    EXECUTE execute = (EXECUTE)lib.resolve("execute");
    PERFORM perform = (PERFORM)lib.resolve("perform");
    ADVANCE advance = (ADVANCE)lib.resolve("advance");
    ERR_CNT err_cnt = (ERR_CNT)lib.resolve("err_cnt");
    ERR_TXT err_txt = (ERR_TXT)lib.resolve("err_txt");

    DBG << "ver:" << version();

    FNC << "bgn";

    int LEN = 10, SZE = 400, MRG = 1;
    QStringList bld;
    QString PATH("d:/temp/13/1/%1.png");
    QString CODE = R"(<code text="%1" fnme="%2" size="%3" mrgn="%4"/>)";
    QString TEXT =
        "Южно-эфиопский грач, увёл мышь за хобот на съезд ящериц {%1}.";

    for(int i=0; i<LEN; i++) {
        bld.append(CODE.arg(
            TEXT.arg(STR(i)), PATH.arg(STR(i))
          , STR(SZE + ((SZE * (double)qrand() / (double)RAND_MAX) - (SZE/2)))
          , STR((i%3 == 0 ? 0: i%3 == 1 ? 1 : 2))
        ));
    }// i

    QString xml = TAG("root", bld.join("\n"));
    std::string str = xml.toStdString();

    this->bgn = SYSDATE;
    perform(str.c_str(), SZE, MRG, 2, SKIP);

    for(int i=0; i<err_cnt(); i++)
        { FNC << "one:" << ((GenError*)err_txt("UTF-8"))[i].text; }

    DBG << "Генерация кодов завершена за:"
        << bgn.msecsTo(SYSDATE) << " msec";

    destroy();

    FNC << "end";
}// on_aCallDll_triggered

// Печать. ---------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aPrint_triggered() {

}// on_aPrint_triggered

// Вызов qr_gen. ---------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aQrGen_triggered() {
    FNC << "bgn";

    FNC << "end";
}// on_aQrGen_triggered

// Проба парсера xml. ----------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aXmlParse_triggered() {


    XmlParse prs(R"(
    <root>
        <code fnme="FNME" text="TEXT"/>
        <code fnme="FNME" text="TEXT"/>
        <code/>
    </root>
    )");

}// on_aXmlParse_triggered


// Кнопка выбора файла задания. ------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btTask_clicked() {
    ui->edTask->setText(QFileDialog::getOpenFileName(
        this, "Открыть XML-файл с заданием.", EMPTY_STR, "*.xml" ));
}// on_btTask_clicked

// Генерация кодов по заданию (lib). -------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aGenToTask_triggered() {
    QLibrary lib;

    QString fnme = ui->edTask->text();
    int size = ui->spSize->value();
    int mrgn = ui->spMargin->value();
    int ncor = ui->spCore->value();
    int mode = ui->spMode->value();
    int ifmt = ui->spFormat->value();
    int iqlt = ui->spQuality->value();
    int corr = ui->spCorr->value();

    wgLog->show();
    wgLog->add(EMPTY_STR);
    wgLog->add("-----------------------------------");

    QFile file(fnme);
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        wgLog->add(QString("fle_err:") + "Not loaded task (*.xml).");
        return;
    }// if(!file.open(QIODevice::ReadOnly | QFile::Text))

    lib.setFileName("qr_gen.dll");

    if(!lib.load()) {
//        wgLog->add(QString("dll_err:") + "Not loaded (qr_gen.dll).");
        wgLog->add(QString("dll_err:") + lib.errorString());
        return;
    }// if(!lib.isLoaded())

    VERSIOn version = (VERSIOn)lib.resolve("version");
    INITIAL initial = (INITIAL)lib.resolve("initial");
    DESTROY destroy = (DESTROY)lib.resolve("destroy");
    EXECUTE execute = (EXECUTE)lib.resolve("execute");
    PERFORM perform = (PERFORM)lib.resolve("perform");
    ADVANCE advance = (ADVANCE)lib.resolve("advance");
    ERR_CNT err_cnt = (ERR_CNT)lib.resolve("err_cnt");
    ERR_TXT err_txt = (ERR_TXT)lib.resolve("err_txt");

    wgLog->add(
        QString("| ncor:") + QString::number(ncor)
      + QString(", size:") + QString::number(size)
      + QString(", ifmt:") + QString::number(ifmt)
      + QString(", iqlt:") + QString::number(iqlt)
      + QString(", corr:") + QString::number(corr) );
    wgLog->add(QString("dll_ver:") + QString::number(version()));
    wgLog->add(QString("tsl_fle:") + fnme);

    QTextStream inp(&file);
    std::string str = inp.readAll().toStdString();

    QDateTime bgn(QDateTime::currentDateTime());
    advance(str.c_str(), size, mrgn, ncor, mode, ifmt, iqlt, corr);
    QDateTime end(QDateTime::currentDateTime());

    qint64 msec = bgn.msecsTo(end);
    wgLog->add(
        QString("tsk_tme:")
      + QTime::fromMSecsSinceStartOfDay(msec).toString("HH:mm:ss")
      + " (" + STR(msec) + "msec" + ")" );

    for(int i=0; i<err_cnt(); i++) {
        wgLog->add(QString("tsk_err:") + ((GenError*)err_txt("UTF-8"))[i].text);
    }// i

    destroy();
    lib.unload();
}// on_aGenToTask_triggered

// Выполнить. ------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_pushButton_clicked() { ui->aGenToTask->triggered(); }

// Генерация кодов по заданию (lib). -------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aGenToTaskS_triggered() {
    QLibrary lib;

    QString fnme = ui->edTask->text();
    int size = ui->spSize->value();
    int mrgn = ui->spMargin->value();
    int ncor = ui->spCore->value();
    int mode = ui->spMode->value();
    int ifmt = ui->spFormat->value();
    int iqlt = ui->spQuality->value();
    int corr = ui->spCorr->value();

    wgLog->show();
    wgLog->add(EMPTY_STR);
    wgLog->add("-----------------------------------");

    QFile file(fnme);
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        wgLog->add(QString("fle_err:") + "Not loaded task (*.xml).");
        return;
    }// if(!file.open(QIODevice::ReadOnly | QFile::Text))

    wgLog->add(
        QString("| ncor:") + QString::number(ncor)
      + QString(", size:") + QString::number(size)
      + QString(", ifmt:") + QString::number(ifmt)
      + QString(", iqlt:") + QString::number(iqlt)
      + QString(", corr:") + QString::number(corr) );
    wgLog->add(QString("tsl_fle:") + fnme);

    QTextStream inp(&file);
    std::string str = inp.readAll().toStdString();

    QDateTime bgn(QDateTime::currentDateTime());

//typedef int  (*ADVANCE)(const char *xml, int size, int margin, int threads, int mode, int img_format, int img_quality, int err_correction);
//advance(str.c_str(), size, mrgn, ncor, mode, ifmt, iqlt, corr);
    mng.GenerateQR(str.c_str(), ncor, mode, size, mrgn, ifmt, iqlt, corr);

    QDateTime end(QDateTime::currentDateTime());

    qint64 msec = bgn.msecsTo(end);
    wgLog->add(
        QString("tsk_tme:")
      + QTime::fromMSecsSinceStartOfDay(msec).toString("HH:mm:ss")
      + " (" + STR(msec) + "msec" + ")" );

    if(mng.numErr() != 0) {
        const GenError *err = mng.lstErr("UTF-8");
        for(int i=0; i<mng.numErr(); i++)
            { wgLog->add(QString("tsk_err:") + err[i].text); }
    }// if(mng.numErr() != 0)

}// on_aGenToTaskS_triggered

//------------------------------------------------------------------------------
