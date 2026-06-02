// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <functional>

#include <QListWidget>
#include <QListWidgetItem>
#include <QStringBuilder>

#include "env.h" // Глобальная среда приложения.

#include "dbg.h"
#include "lst.h"

#include "WWs.h"
#include "ui_WWs.h"


// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
WWs::WWs(QWidget *prnt) : QWidget(prnt), ui(new Ui::WWs) {

    // Внешний вид.
    ui->setupUi(this);

    // Инициализация.
    this->rsn = MAP(QList<QPair<QVariant, QString>> {
        { QWebSocketProtocol::CloseCodeNormal, "CloseCodeNormal" }
      , { QWebSocketProtocol::CloseCodeGoingAway, "CloseCodeGoingAway" }
      , { QWebSocketProtocol::CloseCodeProtocolError, "CloseCodeProtocolError" }
      , { QWebSocketProtocol::CloseCodeDatatypeNotSupported, "CloseCodeDatatypeNotSupported" }
      , { QWebSocketProtocol::CloseCodeReserved1004, "CloseCodeReserved1004" }
      , { QWebSocketProtocol::CloseCodeMissingStatusCode, "CloseCodeMissingStatusCode" }
      , { QWebSocketProtocol::CloseCodeAbnormalDisconnection, "CloseCodeAbnormalDisconnection" }
      , { QWebSocketProtocol::CloseCodeWrongDatatype, "CloseCodeWrongDatatype" }
      , { QWebSocketProtocol::CloseCodePolicyViolated, "CloseCodePolicyViolated" }
      , { QWebSocketProtocol::CloseCodeTooMuchData, "CloseCodeTooMuchData" }
      , { QWebSocketProtocol::CloseCodeMissingExtension, "CloseCodeMissingExtension" }
      , { QWebSocketProtocol::CloseCodeBadOperation, "CloseCodeBadOperation" }
      , { QWebSocketProtocol::CloseCodeTlsHandshakeFailed, "CloseCodeTlsHandshakeFailed" }
    });

    // Меню.
    this->menu = new QMenu(this);
        this->menu->addAction(ui->aColor);
        this->menu->addAction(ui->aDisconnect);
        this->menu->addSeparator(); //---------------
        this->menu->addAction(ui->aHide);
    ui->btMenu->setMenu(this->menu);

}// WWs

// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
WWs::~WWs() {
    delete ui; FNC << "end";
}//~WWs

// Потеря соединения. ----------------------------------------------------------
//------------------------------------------------------------------------------
void WWs::on_ws_disconnect() {
    QWebSocket *sckt = qobject_cast<QWebSocket*>(sender());
    del(sckt);
    E::Main->post(
        "Подключение WS разорвано: {"
      % sckt->origin() % ": " % sckt->resourceName() %  "}");
}// on_ws_disconnect

// Приём текстового сообщения. -------------------------------------------------
//------------------------------------------------------------------------------
void WWs::on_ws_txt_msg(QString msg) {
    QWebSocket *sckt = qobject_cast<QWebSocket*>(sender());
    E::Main->post("WS (" % sckt->origin() % ") txt: ", msg );

    FNC << sckt->peerName() << sckt->resourceName();
}// on_ws_txt_msg

// Приём двоичного сообщения. --------------------------------------------------
//------------------------------------------------------------------------------
void WWs::on_ws_bin_msg(QByteArray msg) {
    QWebSocket *sckt = qobject_cast<QWebSocket*>(sender());
    E::Main->post("WS (" % sckt->origin() % ") bin: ", msg );
}// on_ws_bin_msg

// Добавить под наблюдение. ----------------------------------------------------
//------------------------------------------------------------------------------
void WWs::add(QWebSocket *sckt) {
    QListWidgetItem *item = new QListWidgetItem(sckt->origin());
        item->setData(Qt::UserRole, QVariant::fromValue(sckt));
    ui->lw->addItem(item);
}// add

// Удалить из под наблюдения. --------------------------------------------------
//------------------------------------------------------------------------------
void WWs::del(QWebSocket *sckt) {

    for(int i=0; i<ui->lw->count(); i++) {
        QListWidgetItem *item = ui->lw->item(i);
        if(item && item->data(Qt::UserRole).value<QWebSocket*>() == sckt )
            { ui->lw->takeItem(i); sckt->deleteLater(); }
    }// i

}// del

// Отправить сообщение по WS. --------------------------------------------------
//------------------------------------------------------------------------------
void WWs::on_btSend_clicked() {
    QListWidgetItem *item = ui->lw->currentItem();
    if(item) {
        item->data(Qt::UserRole).value<QWebSocket*>()
            ->sendTextMessage(ui->teSend->toPlainText());
    }// if(item)
}// on_btSend_clicked

// Разорвать соединение. -------------------------------------------------------
//------------------------------------------------------------------------------
void WWs::on_aDisconnect_triggered() {

    QListWidgetItem *item = ui->lw->currentItem();
    if(item && E::Sel->sel(&this->rsn) == QDialog::Accepted) {
        item->data(Qt::UserRole).value<QWebSocket*>()->close(
            static_cast<QWebSocketProtocol::CloseCode>(E::Sel->ret().asINT)
          , "z-srv force disconnect" );
    }// if(item)

}// on_aDisconnect_triggered

// Скрыть. ---------------------------------------------------------------------
//------------------------------------------------------------------------------
void WWs::on_aHide_triggered() { this->hide(); }

//------------------------------------------------------------------------------

