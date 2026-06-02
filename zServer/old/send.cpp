// Чтение сокета. --------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::read_socket(void) {
//    static QByteArray OU(" <<< "), TO(" >>> "), OK("<ok> DEFAULT </ok>");
    static QByteArray OU(R"( / )"), TO(R"( \ )"), OK("<ok> DEFAULT </ok>");

    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

//    QDataStream out(socket);
/**/
    QTextStream out(socket); out.setAutoDetectUnicode(true);
/**/

    QString read = QString::fromUtf8(socket->readAll());
    int pos = read.indexOf('/');

    QString type = read.left(pos).trimmed();
    QString addr = read.mid(pos, read.indexOf("HTTP/") - pos).trimmed();
    QString path = addr.left(addr.indexOf('?')).mid(1).trimmed();

    auto SEND = [&](const QString &log, const QByteArray &ans, const QColor &clr)
     -> void {
/*
        this->log(TO + type + ", path:" + path, clr);
        this->log(OU + log, clr);
        out << ans; socket->waitForBytesWritten(); socket->disconnectFromHost();
*/

/*
QString("<div style='margin-bottom:5px'>")
      + "  <span>" + QString::number(cnt+0) + msg + "</span>" + "<br>"
      + "  <span>" + QString::number(cnt+0) + msg + "</span>"
      + "</div>"
*/


/**/
        QString tme = "[" + QTime::currentTime().toString() + "]";
        QString msg =
            "<div style='margin-bottom:0px; color:" + clr.name() + "'>"
            "  <span>" + tme + OU + ESCPG(log) + "</span>" + "<br>"
            "  <span>" + tme + TO + type + ", path:" + path + "</span>" + "<br>"
            "</div>";

        this->log_html(msg);

        out << ans;
        socket->waitForBytesWritten();
        socket->close();
//        socket->disconnectFromHost();
/**/

    };// SEND

    for(WHandler *handler: hdl) {
        if(0 == QString::compare(path, handler->path, Qt::CaseInsensitive)) {
            SEND(handler->plane_text(), handler->answer(), handler->color);
            return;
        }// if(0 == QString::compare(path, handler->path, Qt::CaseInsensitive))
    }// handler

    SEND(OK, OK, Qt::darkRed);
}// read_socket