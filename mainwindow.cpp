#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include <QMessageBox>
#include <QMenu>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDebug>

#define CONFIG_PATH "dbu_config.json"
#define VERSION_NUMBER "0.1.1"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inputWindow(new InputWindow)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/resources/dbu_icon.ico"));
    trayIcon->setToolTip("Dead By Unicode");

    QMenu *menu = new QMenu(this);

    menu->addAction(this->style()->standardIcon(QStyle::SP_FileIcon), "设置", this, SLOT(show()));
    menu->addAction(this->style()->standardIcon(QStyle::SP_MessageBoxInformation), "关于", this, SLOT(showAbout()));
    menu->addAction(this->style()->standardIcon(QStyle::SP_TitleBarCloseButton), "退出", this, SLOT(exit()));

    trayIcon->setContextMenu(menu);
    trayIcon->show();

    ker = new KeyEventReceiver();
    ker->setCancelEvent(true);
    QObject::connect(ker, KeyEventReceiver::keyEventReceived, this, MainWindow::keyEventHandler);

    checkRegistry();

    readConfig();
    this->ui->keyLabel->setText(QKeySequence(hotKeyCode).toString());

    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    setup();
    if (this->ui->showConfigCheckBox->isChecked()) this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    MSG *msg = static_cast<MSG *>(message);
    if(msg->message == WM_HOTKEY)
    {
        HWND hWnd = (HWND) inputWindow->winId();
        SwitchToThisWindow(hWnd, true);
        inputWindow->show();
        inputWindow->setFocusToTextArea();
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}

void MainWindow::exit()
{
    QApplication::quit();
}

void MainWindow::on_keyInputButton_pressed()
{
    auto label = this->ui->keyLabel;
    label->setText("请按键");
    label->selectAll();
    label->setFocus();
    label->installEventFilter(ker);

}

void MainWindow::keyEventHandler(QKeyEvent *key)
{
    int keyCode = key->key();
    if (keyCode == Qt::Key_Alt || keyCode == Qt::Key_Shift || keyCode == Qt::Key_Control)
        return;
    auto label = this->ui->keyLabel;
    label->removeEventFilter(ker);
    label->setText(QKeySequence(key->key()).toString());
    hotKeyCode = key->key();
}

void MainWindow::setup()
{
    UnregisterHotKey(HWND(winId()), 0);

    unsigned int modifier = 0;

    if (this->ui->ctrlCheckBox->isChecked())
        modifier |= MOD_CONTROL;
    if (this->ui->altCheckBox->isChecked())
        modifier |= MOD_ALT;
    if (this->ui->shiftCheckBox->isChecked())
        modifier |= MOD_SHIFT;

    if(!RegisterHotKey(HWND(winId()), 0, modifier, hotKeyCode))
    {
        QMessageBox::warning(this, "错误", "绑定快捷键失败，可能指定的按键组合已经被其他程序绑定。");
    }

    inputWindow->setKeyDelay(this->ui->keyDelaySpinBox->value());
    inputWindow->setAutoEnter(this->ui->AutoEnterCheckBox->isChecked());
}

void MainWindow::readConfig()
{
    QFile file;
    file.setFileName(CONFIG_PATH);
    bool ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!ret)
    {
        file.close();
        return;
    }
    QByteArray content = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(content);
    if (doc.isNull() || !doc.isObject())
    {
        QMessageBox::warning(this, "错误", "读取配置文件时解析JSON失败。");
        qDebug(content.toStdString().c_str());
        return;
    }
    QJsonObject obj = doc.object();
    bool modCtrl = obj["mod_ctrl"].toBool();
    bool modAlt = obj["mod_alt"].toBool();
    bool modShift = obj["mod_shift"].toBool();

    this->ui->ctrlCheckBox->setChecked(modCtrl);
    this->ui->altCheckBox->setChecked(modAlt);
    this->ui->shiftCheckBox->setChecked(modShift);

    hotKeyCode = obj["hot_key_code"].toInt();

    this->ui->keyDelaySpinBox->setValue(obj["key_delay"].toInt());
    this->ui->AutoEnterCheckBox->setChecked(obj["auto_enter"].toBool());
    this->ui->showConfigCheckBox->setChecked(obj["show_config_on_start"].toBool());

    qDebug("Config read.");
}

void MainWindow::writeConfig()
{
    QJsonObject obj;
    obj.insert("mod_ctrl", this->ui->ctrlCheckBox->isChecked());
    obj.insert("mod_alt", this->ui->altCheckBox->isChecked());
    obj.insert("mod_shift", this->ui->shiftCheckBox->isChecked());
    obj.insert("hot_key_code", this->hotKeyCode);
    obj.insert("key_delay", this->ui->keyDelaySpinBox->value());
    obj.insert("auto_enter", this->ui->AutoEnterCheckBox->isChecked());
    obj.insert("show_config_on_start", this->ui->showConfigCheckBox->isChecked());

    QJsonDocument doc(obj);

    QFile file;
    file.setFileName(CONFIG_PATH);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "错误", "写入配置文件失败。");
        return;
    }
    file.write(doc.toJson());
    file.close();
}

QString systemErrorToQString(DWORD error)
{
    QString ret = QString::number(error);
    LPWSTR messageBuffer = NULL;
    DWORD size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, (LPWSTR) &messageBuffer, 0, NULL);
    if (size != 0)
        ret += " - " + QString::fromWCharArray(messageBuffer);
    LocalFree(messageBuffer);
    ret = ret.replace("\r\n", "");
    return ret;
}


void MainWindow::checkRegistry()
{
    LPCWSTR path = L"Control Panel\\Input Method";
    LPCWSTR valueName = L"EnableHexNumpad";
    HKEY hKey;
    LSTATUS result = RegOpenKeyEx(HKEY_CURRENT_USER, path, 0, KEY_READ | KEY_SET_VALUE, &hKey);
    if (result != ERROR_SUCCESS)
    {
        QString errMessage = systemErrorToQString(result);
        QMessageBox::warning(this, "错误", "无法判断是否已启用十六进制小键盘输入：打开“HKEY_CURRENT_USER\\Control Panel\\Input Method”失败。（" + errMessage + "）");
        return;
    }

    DWORD type;
    LPCWSTR dataStr = L"1";
    DWORD dataStrSize = (wcslen(dataStr) + 1) * sizeof (wchar_t);
    DWORD rSize = dataStrSize;
    wchar_t rData[dataStrSize];
    result = RegQueryValueExW(hKey, valueName, NULL, &type, (LPBYTE) rData, &rSize);

    if (result == ERROR_SUCCESS && type == REG_SZ && wcscmp(rData, dataStr) == 0)
    {
        return;
    }
    else if (result != ERROR_SUCCESS && result != ERROR_FILE_NOT_FOUND && result != ERROR_MORE_DATA)
    {
        QString errMessage = systemErrorToQString(result);
        QMessageBox::warning(this, "错误", "无法判断是否已启用十六进制小键盘输入:请求“EnableHexNumpad”失败。（" + errMessage + "）");
    }
    else
    {
        if(QMessageBox::question(this, "提示", "检测到未启用十六进制小键盘输入，是否立即设置启用？") == QMessageBox::Yes)
        {
            DWORD regSetResult = RegSetValueExW(hKey, valueName, 0, REG_SZ, (const BYTE *) dataStr, dataStrSize);
            if (regSetResult == ERROR_SUCCESS)
            {
                QMessageBox::information(this, "提示", "已写入注册表项。需要重新登录Windows账号使设置生效。");
            }
            else
            {
                QString errMessage = systemErrorToQString(regSetResult);
                QMessageBox::warning(this, "错误", "写入注册表项失败。（" + errMessage + "）");
            }
        }
    }
}

void MainWindow::showAbout()
{
    QString aboutText =
            "<b>Dead By Unicode</b><br>" \
            "作者：K9YYY<br>" \
            "Release 版本：" + QString::fromLatin1(VERSION_NUMBER) + "<br>" \
            "编译时间：" + QString::fromLatin1(__DATE__) + ", " + QString::fromLatin1(__TIME__) + "<br>" \
            "<a href=\"https://github.com/k9yyy/dead_by_unicode_gui\">GitHub项目页</a>";

    QMessageBox::about(this, "关于", aboutText);
}

void MainWindow::on_saveButton_pressed()
{
    setup();
    writeConfig();
    QMessageBox::information(this, "信息", "已保存设置。");
}
