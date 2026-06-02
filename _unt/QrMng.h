#ifndef QRMNG_H
#define QRMNG_H

#include <QObject>
#include <QTextCodec>
#include <QByteArray>
#include <QXmlStreamReader>

#include "QrGen.h"

enum ExecError { STOP=1, SKIP=2 };
enum ErrorCode { XML=1, GEN=2, TSK=3, SVE=4};

// Структура хранения ошибки (возвращаемая). -----------------------------------
//------------------------------------------------------------------------------
struct GenError {
    int code;
    char *text = nullptr;

    GenError(void) {}
    GenError(int code, const QString &str, const char *enc): code(code) {
        QByteArray arr = QTextCodec::codecForName(enc)->fromUnicode(str);

        text = new char[arr.size() + 1]; text[arr.size()] = '\0';
        strcpy(text, arr.constData());
    }// Error

   ~GenError() { delete[] text; }
};// GenError

// Структура хранения ошибок (внутренняя). -------------------------------------
//------------------------------------------------------------------------------
struct InsError {
    int code; QString text;
    InsError(int code, const QString &text): code(code), text(text) {}
};// InsError

// Управление генерацией Qr-кодов. ---------------------------------------------
//------------------------------------------------------------------------------
class QrMng : public QObject {
    Q_OBJECT

  public:
    QList<QList<QrItem>> kit;

    explicit QrMng(QObject *parent = 0);
            ~QrMng(void);

    int GenerateQR(
        const char *xml_str, int thread_cnt, int mode
      , int img_size, int img_mrgn, int img_format, int img_quality
      , int err_corr );

    const GenError* lstErr(const char *fmt = "UTF-8");
    int             numErr(void) { return l_err.length(); }

  private:
    int mode;
    QList<QrGen*> l_gen;
    QList<InsError> l_err;
    GenError *err_out = nullptr;

    void CLEAR_GEN(void);

  private slots:
    void error(QrGen::ERR code, QString str);

};

//------------------------------------------------------------------------------
#endif // QRMNG_H
