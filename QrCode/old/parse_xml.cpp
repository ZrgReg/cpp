// Проба. ----------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_aTest_triggered() {
/*
    const GenError *err = mng.lstErr();
    for(int i=0; i < mng.numErr(); i++)
        { DBG << "num:" << i << ", err:" << err[i].text; }
*/

/*
    QFile fle("d:/temp/16/enc.txt");
    QString str(" -=В чащах юга жил бы цитрус=- ");
    QByteArray arr_win = QTextCodec::codecForName("Windows-1251")
        ->fromUnicode(str);
    QByteArray arr_utf = QTextCodec::codecForName("UTF-8")
        ->fromUnicode(str);
    const char *win = arr_win.constData();
    const char *utf = arr_utf.constData();

    FNC << "open:" << fle.open(QIODevice::WriteOnly);

    fle.write(win);
    fle.write(utf);
*/
//    QString hdr(R"(<?xml version="1.0" encoding="windows-1251"?>)");
    QByteArray enc;
    QString hdr(EMPTY_STR);
//    QString txt(R"(<root><code one="раз" two="2"/><code/></root>)");
    QString txt(R"(<t><k><code one="раз" two="2"/><code/></k></t>)");


    QXmlStreamReader xml(
        QString(QTextCodec::codecForName("Windows-1251")->fromUnicode(hdr))
      + QString(QTextCodec::codecForName("Windows-1251")->fromUnicode(txt))
    );

    static const QString ROOT = "root", CODE = "code";
    static const QString ONE = "one", TWO = "two";

    while(!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument) {
            FNC << "enc:" << xml.documentEncoding();
            enc = NVL(xml.documentEncoding().toString(), "UTF-8").toLocal8Bit();
            continue;
        }
        if(token == QXmlStreamReader::StartElement) {
            if(ROOT == xml.name()) { FNC << ROOT; }
            if(CODE == xml.name()) {
                FNC << CODE << "col:" << xml.columnNumber();

                QXmlStreamAttributes attr = xml.attributes();
                if(attr.hasAttribute(ONE)) {
                    FNC << ONE << attr.value(ONE);

                    FNC << ONE << QString(QTextCodec::codecForName("Windows-1251")
                        ->toUnicode(attr.value(ONE).toLocal8Bit()) );
                }
                if(attr.hasAttribute(TWO)) { FNC << TWO << attr.value(TWO); }

            }// if(CODE == xml.name())
        }// if(tolen == QXmlStreamReader::StartElement)

    }// while(!prs.atEnd() && !xml.hasError())

    FNC << "has:" << xml.hasError();

}// on_aTest_triggered