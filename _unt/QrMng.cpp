#include <functional>

#include "_srv/Srv/std.h"

#include "QrMng.h"


// Константы. ------------------------------------------------------------------
//------------------------------------------------------------------------------
static const QString ROOT = "root", CODE = "code";
static const QString TEXT = "text", FNME = "fnme";
static const QString MRGN = "mrgn", SIZE = "size";
static const QString MCLR = "mclr", FCLR = "fclr", PCLR = "pclr";

// Декорирование фигурными скобками. -------------------------------------------
//------------------------------------------------------------------------------
QString Q(const QString &str) { return QString("{") + str + QString("}"); }

// Очистка списка потоков генерации кодов. -------------------------------------
//------------------------------------------------------------------------------
void QrMng::CLEAR_GEN(void) {
    for(QrGen *gen : l_gen) { gen->stop(); }
    for(QrGen *gen : l_gen) { gen->wait(); }

    for(QrGen *gen : l_gen) { delete gen; }
    l_gen.clear();
}// CLEAR_GEN

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
QrMng::QrMng(QObject *parent) : QObject(parent) {

}// QrMng

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
QrMng::~QrMng(void) {
    delete[] err_out;
}

// Многопоточная генерация QR-кодов. -------------------------------------------
//------------------------------------------------------------------------------
int QrMng::GenerateQR(
    const char *xml_str, int thread_cnt, int mode
  , int img_size, int img_mrgn, int img_format, int img_quality
  , int err_corr )
{
    static const QString XML_ERR("Ошибка при разборке XML, причина: %1.");
    static const QString TSK_ERR("Ошибка описания задания, причина: %1.");

    this->mode = mode;
    delete[] err_out; err_out = nullptr;
    l_err.clear();

    // Подготовка набора списков.
    kit.clear();
    for(int i=0; i<thread_cnt; i++) { kit.push_back(QList<QrItem>()); }

    QList<QList<QrItem>>::iterator bgn = kit.begin(), end = kit.end();
    QList<QList<QrItem>>::iterator it_itm = bgn;

    auto ADD = [&](QrItem itm) -> void { // Попеременное добавление.
        it_itm->append(itm); it_itm++;
        if(it_itm == end) { it_itm = bgn; }
    };// ADD

    // Разбор XML.
    QXmlStreamReader xml(xml_str);

    // Тег: CODE.
    auto parseCode = [&](void) -> void {
        if(!xml.isStartElement() || xml.name() != CODE) { return; }

        bool size_ok(true), mrgn_ok(true);
        QXmlStreamAttributes attr = xml.attributes();
        if(attr.hasAttribute(TEXT) && attr.hasAttribute(FNME)) {
            int size = ( attr.hasAttribute(SIZE)
              ? attr.value(SIZE).toInt(&size_ok) : img_size );
            int mrgn = ( attr.hasAttribute(MRGN)
              ? attr.value(MRGN).toInt(&mrgn_ok) : img_mrgn );

            if(size_ok == false) { size = img_size; }
            if(mrgn_ok == false) { mrgn = img_mrgn; }
            if(size_ok == false || mrgn_ok == false) {
                l_err.append(InsError(TSK, TSK_ERR.arg(
                    "В теге <code> ошибка преобразования: %1").arg(
                        (size_ok ? EMPTY_STR : Q(SIZE))
                      + (mrgn_ok ? EMPTY_STR : Q(MRGN)) )));
            }// if(size_ok == false || mrgn_ok == false)

            ADD(QrItem(
                attr.value(FNME).asSTR, attr.value(TEXT).asSTR
              , size

              , mrgn == 0 ? MARGIN::NOT
              : mrgn == 2 ? MARGIN::TRP : MARGIN::YES

              //todo: Разобраться с прозрачностью.
              , Qt::white /* mrgn == 2 ? Qt::transparent : Qt::white*/

              , Qt::white, Qt::black
            ));
        } else {
            l_err.append(InsError(TSK, TSK_ERR.arg(
                "В теге <code> нет: %1").arg(
                   (attr.hasAttribute(FNME) ? EMPTY_STR : Q(FNME))
                  +(attr.hasAttribute(TEXT) ? EMPTY_STR : Q(TEXT)) )));
        }// else
    };// parseCode

    // Тег: ROOT.
    auto parseRoot = [&](void) -> void {
        if(!xml.isStartElement() || xml.name() != ROOT) { return; }

        while(xml.readNextStartElement()) {
            if(xml.name() == CODE) { parseCode(); }
            else                   { xml.skipCurrentElement(); }
            xml.readNext();
        }// while(xml.readNextStartElement())
    };// parseRoot

    // Разбор.
    if(xml.readNextStartElement() && xml.name() == ROOT) { parseRoot(); }
    if(xml.tokenType() == QXmlStreamReader::Invalid) { xml.readNext(); }

    // Обработка ошибок.
    if(this->mode == STOP && l_err.size() != 0) { return l_err.length(); }
    if(xml.hasError()) {
        l_err.append(InsError(XML, XML_ERR.arg(xml.errorString())));
        return l_err.length();
    }// if(xml.hasError())

    // Активация потоков.
    CLEAR_GEN();
    for(const QList<QrItem> &l_itm : this->kit) {
        QrGen *gen = new QrGen(
            this, l_itm
          , QImage::Format(img_format), img_quality
          , qrcodegen::QrCode::Ecc(err_corr) );
        connect(gen, &QrGen::error, this, &QrMng::error, Qt::DirectConnection);
        l_gen.append(gen);
    }// i
    for(QrGen *gen : l_gen) { gen->start(); }

    // Ожидание завершения потоков.
    for(QrGen *gen : l_gen) { gen->wait(); }

    return l_err.length();
}// GenerateQR

// Ошибка, при генерации QR-кода. ----------------------------------------------
//------------------------------------------------------------------------------
void QrMng::error(QrGen::ERR code, QString str) {
    l_err.append(InsError((code == QrGen::ERR::GEN ? GEN : SVE), str));
    if(this->mode == STOP) { CLEAR_GEN(); }
}// gen_error

// Генерация/возвращение списка ошибок. ----------------------------------------
//------------------------------------------------------------------------------
const GenError* QrMng::lstErr(const char *fmt) {
    if(err_out == nullptr) {
        err_out = new GenError[l_err.length()];

        int cnt = 0;
        for(const InsError &error : l_err) {
            GenError *cur = new GenError(error.code, error.text, fmt); /* !!! */
            err_out[cnt] = (*cur);
            cnt++;
        }// error

    }// if(err_out == nullptr)

    return err_out;
}// getLastErrors

//------------------------------------------------------------------------------



