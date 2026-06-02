#ifndef WWS_H
#define WWS_H

#include <QMap>
#include <QWidget>
#include <QWebSocket>
#include <QMenu>


namespace Ui { class WWs; }

/* Веб сокет. *****************************************************************/
/******************************************************************************/
class WWs : public QWidget {
    Q_OBJECT

 public:
            ~WWs();
    explicit WWs(QWidget *prnt = nullptr);

    void add(QWebSocket *sckt);
    void del(QWebSocket *sckt);

 public slots:
   void on_ws_disconnect();
   void on_ws_txt_msg(QString msg);
   void on_ws_bin_msg(QByteArray msg);

 private slots:
   void on_btSend_clicked();
   void on_aDisconnect_triggered();
   void on_aHide_triggered();

private:
    Ui::WWs *ui;
    QMap<QVariant, QString> rsn;
    QMenu *menu = nullptr;

};// WWs

//------------------------------------------------------------------------------
#endif // WWS_H
