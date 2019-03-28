#include "inputwindow.h"
#include "ui_inputwindow.h"
#include "keyeventreceiver.h"

InputWindow::InputWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

    KeyEventReceiver *kerEsc = new KeyEventReceiver();
    KeyEventReceiver *kerEnter = new KeyEventReceiver();
    ui->inputTextArea->installEventFilter(kerEsc);
    ui->inputTextArea->installEventFilter(kerEnter);
    QObject::connect(kerEsc, KeyEventReceiver::keyEventReceived, this, InputWindow::hide);
    QObject::connect(kerEnter, KeyEventReceiver::keyEventReceived, this, InputWindow::keyEventHandler);
    kerEsc->setListenForKey(Qt::Key_Escape);
    kerEnter->setListenForKey(Qt::Key_Return);
    kerEnter->setCancelEvent(true);

    hexinput = new HexInput();
}

InputWindow::~InputWindow()
{
    delete ui;
}

void InputWindow::loadUiText()
{
    this->ui->enterButton->setText(l10n->getTrans("enter") + " (Enter)");
    this->ui->hideButton->setText(l10n->getTrans("cancel") + " (Esc)");

    QFont font(l10n->getFontName());
    font.setPixelSize(12);
    this->ui->enterButton->setFont(font);
    this->ui->hideButton->setFont(font);
    font.setPixelSize(14);
    this->ui->inputTextArea->setFont(font);
}

void InputWindow::setFocusToTextArea()
{
    ui->inputTextArea->setFocus();
    ui->inputTextArea->selectAll();
}

void InputWindow::on_enterButton_pressed()
{
    sendText();
}

void InputWindow::on_hideButton_pressed()
{
    this->hide();
}

void InputWindow::sendText()
{
    QString msg = this->ui->inputTextArea->toPlainText();
    this->hide();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    if (autoEnter) msg += "\n";
    hexinput->enter_string(msg);
}

void InputWindow::keyEventHandler(QKeyEvent *key)
{
    if (key->key() == Qt::Key_Return)
        sendText();
}

void InputWindow::setKeyDelay(int delay)
{
    hexinput->set_key_delay(delay);
}

void InputWindow::setAutoEnter(bool flag)
{
    autoEnter = flag;
}

void InputWindow::setLocalizationContext(Localization *l10n)
{
    this->l10n = l10n;
}
