#ifndef XMLPARSE_H
#define XMLPARSE_H

// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QXmlStreamReader>


// Парсинг XML. ----------------------------------------------------------------
//------------------------------------------------------------------------------
class XmlParse {

public:
    XmlParse(const QString &xml_doc);

private:
    QXmlStreamReader xml;

    void parseRoot(void);
    void parseCode(void);

};// XmlParse

//------------------------------------------------------------------------------

#endif // XMLPARSE_H
