#ifndef KEYEVENTRECEIVER_H
#define KEYEVENTRECEIVER_H

#include <QKeyEvent>


class KeyEventReceiver : public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void keyEventReceived(QKeyEvent *listenForKey);

public:
    void setListenForKey(int listenForKey);
    void setCancelEvent(bool flag);

private:
    int listenForKey = -1;
    bool cancelEvent = false;
};

#endif // KEYEVENTRECEIVER_H
