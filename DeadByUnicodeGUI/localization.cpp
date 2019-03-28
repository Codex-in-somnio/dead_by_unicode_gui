#include "localization.h"
#include <QFile>
#include <QMessageBox>

Localization::Localization()
{
    QFile file;
    file.setFileName("l10n.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        file.setFileName(":/resources/l10n.json");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(nullptr, "Error", "Failed to open localization file.");
            return;
        }
    }
    QByteArray content = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(content);
    if (doc.isNull() || !doc.isObject())
    {
        QMessageBox::critical(nullptr, "Error", "Failed to parse localization file.");
        return;
    }
    l10nJsonObj = doc.object();
}


QString Localization::getTrans(QString msg, QString langCode)
{
    if (langCode == nullptr)
        langCode = curLangCode;
    if (l10nJsonObj.contains(langCode))
    {
        auto messagesObj = l10nJsonObj[langCode].toObject()["messages"].toObject();
        if (messagesObj.contains(msg))
            return messagesObj[msg].toString();
        else if (langCode.compare("en") != 0)
            return getTrans(msg, "en");
        else
            return msg;
    }
    else if (l10nJsonObj.contains("en"))
    {
        return getTrans(msg, "en");
    }
    else
    {
        return msg;
    }
}

void Localization::setCurLangCode(QString langCode)
{
    curLangCode = langCode;
}

map<QString, QString> Localization::getLanguages()
{
    map<QString, QString> ret;
    for (int i = 0; i < l10nJsonObj.count(); ++i)
    {
        QString code = l10nJsonObj.keys().at(i);
        QString name = l10nJsonObj[code].toObject()["language_name"].toString();
        ret[code] = name;
    }
    return ret;
}

QString Localization::getFontName()
{
    QString fontName = l10nJsonObj[curLangCode].toObject()["ui_font"].toString();
    if (fontName == "") fontName = "Sans Serif";
    return fontName;
}
