#ifndef TEXTAREA_H
#define TEXTAREA_H

#include <QPlainTextEdit>
#include "inputwindow.h"

class TextArea : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit TextArea();

private slots:
    void keyPressEvent(QKeyEvent *event);
};

#endif // TEXTAREA_H
