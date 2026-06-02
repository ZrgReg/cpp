// Îòëàäêà. --------------------------------------------------------------------
//------------------------------------------------------------------------------
void FMain::on_btDebug_clicked() {
    const QString msg = ": ÍÅÊÎÅ ÄÎÑÒÀÒÎ×ÍÎ ÄËÈÍÍÎÅ ÑÎÎÁÙÅÍÈÅ";
    static int cnt = 0; cnt++;

/**/
    QTextBrowser *browser = ui->tbLog;
    QString text;

/*
    text.append("<html><body>");
    text.append(
        "<style type='text/css'>"
//        "div { margin-bottom:" + QString::number(cnt) + "px; }"
        "hr { "
        "margin-bottom:" + QString::number(cnt) + "px; "
        "margin-top:" + QString::number(cnt) + "px; "
        "size:" + QString::number(cnt) + "px; "
        "}"

        "</style>" );
*/

    text.append(
QString("<div style='margin-bottom:0px'>")
      + "  <span>" + QString::number(cnt+0) + msg + "</span>" + "<br>"
      + "  <span>" + QString::number(cnt+0) + msg + "</span>"
      + "</div>"
    );

//    text.append("<div style='border-top: 1px solid black; width: 100%; margin: 0px;'>&nbsp;</div>");
//    text.append(R"(<hr size="1" style='margin-top:1; margin-bottom:1'>)");
//    text.append("<hr>");

    text.append(
QString("<div style='margin-bottom:0px'>")
      + "  <span>" + QString::number(cnt+1) + msg + "</span>" + "<br>"
      + "  <span>" + QString::number(cnt+1) + msg + "</span>"
      + "</div>"
    );

//    text.append("<div style='border-top: 1px solid black; width: 100%; margin: 0px;'>&nbsp;</div>");
//    text.append(R"(<hr size="1" style='margin-top:1; margin-bottom:1'>)");
//    text.append("<hr>");

    text.append(
QString("<div style='margin-bottom:0px'>")
      + "  <span>" + QString::number(cnt+2) + msg + "</span>" + "<br>"
      + "  <span>" + QString::number(cnt+2) + msg + "</span>"
      + "</div>"
    );

    text.append("</body></html>");
    browser->setHtml(text);
/**/


// style='padding-top:10px;'

/*
    if(FIRST_RUN) {
        ui->tbLog->append(
            "<style type='text/css'>p { margin-bottom:0px; }</style>"
        );
    }

    ui->tbLog->append(
QString("<p>") //+ QString::number(cnt) + msg
      + "  <span>" + QString::number(cnt) + msg + "</span>" + "<br>"
      + "  <span>" + QString::number(cnt) + msg + "</span>" + "<br>"
      + "</p>"
    );
*/

}// on_btDebug_clicked

//------------------------------------------------------------------------------

