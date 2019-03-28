#ifndef LOCALIZATION_H
#define LOCALIZATION_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <map>

using namespace std;

class Localization
{
public:
    Localization();
    void setCurLangCode(QString langCode);
    map<QString, QString> getLanguages();
    QString getTrans(QString msg, QString langCode = nullptr);
    QString getFontName();

private:
    QJsonObject l10nJsonObj;
    QString curLangCode = "en";
};

#endif // LOCALIZATION_H
