#ifndef Q_APLAYER_HEADER
#define Q_APLAYER_HEADER

#include <QObject>
#include <QString>
#include <QMediaPlayer>

/* Проигрываеие аудио. ********************************************************/
/******************************************************************************/
class APlayer : public QObject {
    Q_OBJECT

  public:
    enum class ERR { NOT, UNC, INP };

    explicit APlayer(QObject *parent = 0);
            ~APlayer();

    bool isPlayerAvailable() const;

    void play(QUrl url);
    void stop();

    void pause();
    void tempo(qreal rate);

    void setVolume(int volume);

  private:
    QMediaPlayer *player;

  private slots:
//    void durationChanged(qint64 duration);
//    void positionChanged(qint64 progress);

};// APlayer

//------------------------------------------------------------------------------
#endif // Q_APLAYER_HEADER
