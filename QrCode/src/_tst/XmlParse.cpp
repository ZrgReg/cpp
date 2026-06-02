// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "XmlParse.h"

#include "_srv/Srv/std.h"
#include "_srv/Srv/dbg.h"


// Константы. ------------------------------------------------------------------
//------------------------------------------------------------------------------
const QString ROOT = "root", CODE = "code";
const QString TEXT = "text", FNME = "fnme";

QString Q(const QString &str);

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
XmlParse::XmlParse(const QString &xml_doc) {
    xml.addData(xml_doc);

    if(xml.readNextStartElement() && xml.name() == ROOT) { parseRoot(); }
    if(xml.tokenType() == QXmlStreamReader::Invalid) { xml.readNext(); }
    if(xml.hasError()) { FNC << "err:" << xml.errorString(); }
}// XmlParse

// Разбор тега root. -----------------------------------------------------------
//------------------------------------------------------------------------------
void XmlParse::parseRoot(void) {
    if(!xml.isStartElement() || xml.name() != ROOT) { return; }

    while(xml.readNextStartElement()) {
        if(xml.name() == CODE) { parseCode(); }
        else { xml.skipCurrentElement(); }
    }// while(xml.readNextStartElement())
}// parseRoot

// Разбор тега code. -----------------------------------------------------------
//------------------------------------------------------------------------------
void XmlParse::parseCode(void) {
    if(!xml.isStartElement() || xml.name() != CODE) { return; }

    QXmlStreamAttributes attr = xml.attributes();
    if(attr.hasAttribute(TEXT) && attr.hasAttribute(FNME)) {
        FNC << "fnme:" << attr.value(FNME).toString()
            << "text:" << attr.value(TEXT).toString();
    } else {
        FNC << "err:" << QString("В теге <code> нет: %1").arg(
            (attr.hasAttribute(FNME) ? EMPTY_STR : Q(FNME))
          + (attr.hasAttribute(TEXT) ? EMPTY_STR : Q(TEXT)) );
    }// else

    xml.readNext();
}// parseCode

//------------------------------------------------------------------------------
