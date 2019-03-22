#include "keyeventreceiver.h"

bool KeyEventReceiver::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()==QEvent::KeyPress)
    {
        QKeyEvent *key = static_cast<QKeyEvent*>(event);
        if (listenForKey == key->key() || listenForKey == -1)
        {
            emit keyEventReceived(key);
            if (cancelEvent)
                return true;
            else
                return QObject::eventFilter(obj, event);
        }
    }
    return QObject::eventFilter(obj, event);
}

void KeyEventReceiver::setListenForKey(int key)
{
    listenForKey = key;
}

void KeyEventReceiver::setCancelEvent(bool flag)
{
    cancelEvent = flag;
}
