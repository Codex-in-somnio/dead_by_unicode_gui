#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include <QMessageBox>
#include <QMenu>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDebug>
#include <QLocale>
#include <map>

#define CONFIG_PATH "dbu_config.json"
#define VERSION_NUMBER "0.1.5"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inputWindow(new InputWindow),
    l10n(new Localization)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());

    QString langCode = QLocale::system().name().split("_").at(0).toLower();

    std::map<QString, QString> languages = l10n->getLanguages();
    if (languages.count(langCode) == 0) langCode = "en";

    for (auto &lang : languages)
    {
        this->ui->displayLanguageComboBox->addItem(lang.second, QVariant::fromValue(lang.first));
        if (lang.first == langCode) this->ui->displayLanguageComboBox->setCurrentText(lang.second);
    }

    l10n->setCurLangCode(langCode);
    inputWindow->setLocalizationContext(l10n);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/resources/dbu_icon_16.png"));
    trayIcon->setToolTip("Dead By Unicode");

    menu = new QMenu(this);

    ker = new KeyEventReceiver();
    ker->setCancelEvent(true);
    QObject::connect(ker, KeyEventReceiver::keyEventReceived, this, MainWindow::keyEventHandler);

    checkRegistry();

    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    readConfig();
    this->ui->keyLabel->setText(QKeySequence(hotKeyCode).toString());
    setup();
    if (this->ui->showConfigCheckBox->isChecked()) this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadUiText()
{
    menu->clear();

    menu->addAction(this->style()->standardIcon(QStyle::SP_FileIcon), l10n->getTrans("settings"), this, SLOT(show()));
    menu->addAction(this->style()->standardIcon(QStyle::SP_MessageBoxInformation), l10n->getTrans("about"), this, SLOT(showAbout()));
    menu->addAction(this->style()->standardIcon(QStyle::SP_TitleBarCloseButton), l10n->getTrans("exit"), this, SLOT(exit()));

    trayIcon->setContextMenu(menu);
    trayIcon->show();

    this->setWindowTitle(l10n->getTrans("settings"));
    this->ui->hotkeySettingsGroupBox->setTitle(l10n->getTrans("set_hot_key"));
    this->ui->keyInputButton->setText(l10n->getTrans("key_in"));
    this->ui->keyDelayLabel->setText(l10n->getTrans("key_delay") + ":");
    this->ui->keyDelayMsLabel->setText(l10n->getTrans("milliseconds"));
    this->ui->AutoEnterCheckBox->setText(l10n->getTrans("auto_enter"));
    this->ui->showConfigCheckBox->setText(l10n->getTrans("show_settings_on_start"));
    this->ui->displayLanguageLabel->setText(l10n->getTrans("language") + ":");
    this->ui->saveButton->setText(l10n->getTrans("save"));

    QFont font(l10n->getTrans("ui_font"));
    font.setPixelSize(12);
    QApplication::setFont(font);

    inputWindow->loadUiText();
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
    label->setText(l10n->getTrans("press_a_key"));
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
    QString langCode = this->ui->displayLanguageComboBox->currentData().toString();
    l10n->setCurLangCode(langCode);
    loadUiText();

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
        QMessageBox::warning(this, l10n->getTrans("error"), l10n->getTrans("register_hotkey_fail"));
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
        QMessageBox::warning(this, l10n->getTrans("error"), l10n->getTrans("parse_config_json_fail"));
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

    QString langCode = obj["display_language"].toString();
    auto languages = l10n->getLanguages();
    if (languages.count(langCode) > 0)
    {
        QString curLangName = languages[langCode];
        this->ui->displayLanguageComboBox->setCurrentText(curLangName);
    }

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
    obj.insert("display_language", this->ui->displayLanguageComboBox->currentData().toString());

    QJsonDocument doc(obj);

    QFile file;
    file.setFileName(CONFIG_PATH);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, l10n->getTrans("error"), l10n->getTrans("write_config_fail"));
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
        QMessageBox::warning(this, l10n->getTrans("error"), l10n->getTrans("open_hkey_fail").arg("HKEY_CURRENT_USER\\Control Panel\\Input Method") + " (" + errMessage + ")");
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
        QMessageBox::warning(this, l10n->getTrans("error"), l10n->getTrans("query_reg_value_fail").arg("EnableHexNumpad") + " (" + errMessage + ")");
    }
    else
    {
        if(QMessageBox::question(this, l10n->getTrans("question"), l10n->getTrans("question_enable_hex_numpad")) == QMessageBox::Yes)
        {
            DWORD regSetResult = RegSetValueExW(hKey, valueName, 0, REG_SZ, (const BYTE *) dataStr, dataStrSize);
            if (regSetResult == ERROR_SUCCESS)
            {
                QMessageBox::information(this, l10n->getTrans("info"), l10n->getTrans("set_reg_value_success"));
            }
            else
            {
                QString errMessage = systemErrorToQString(regSetResult);
                QMessageBox::warning(this, l10n->getTrans("error"), l10n->getTrans("set_reg_value_fail") + " (" + errMessage + ")");
            }
        }
    }
}

void MainWindow::showAbout()
{
    QString aboutText =
            "<b>Dead By Unicode</b><br>" +
            l10n->getTrans("author") + ": K9YYY<br>" +
            l10n->getTrans("release_version") + ": " + QString::fromLatin1(VERSION_NUMBER) + "<br>" +
            l10n->getTrans("build_datetime") + ": " + QString::fromLatin1(__DATE__) + ", " + QString::fromLatin1(__TIME__) + "<br>" \
            "<a href=\"" + l10n->getTrans("github_project_url") + "\">" + l10n->getTrans("github_project_page") + "</a>";

    QMessageBox::about(this, "关于", aboutText);
}

void MainWindow::on_saveButton_pressed()
{
    setup();
    writeConfig();
    QMessageBox::information(this, l10n->getTrans("info"), l10n->getTrans("config_saved"));
}
