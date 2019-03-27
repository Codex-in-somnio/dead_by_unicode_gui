#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QAction>
#include "inputwindow.h"
#include "keyeventreceiver.h"
#include "localization.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
    Ui::MainWindow *ui;
    InputWindow *inputWindow;
    QSystemTrayIcon *trayIcon;
    KeyEventReceiver *ker;
    int hotKeyCode = 0x44; // default to 'D'
    Localization *l10n;
    QMenu *menu;
    void setup();
    void readConfig();
    void writeConfig();
    void checkRegistry();
    void loadUiText();

private slots:
    void exit();
    void keyEventHandler(QKeyEvent *key);
    void showAbout();
    void on_keyInputButton_pressed();
    void on_saveButton_pressed();
};

#endif // MAINWINDOW_H
