#ifndef LOCALIZATION_H
#define LOCALIZATION_H
#include <map>
#include <QString>

using namespace std;

class Localization
{
public:
    Localization();
    void setCurLangCode(QString langCode);
    map<QString, QString> getLanguages();
    QString getTrans(QString msg, QString langCode = nullptr);

private:
    map<QString, map<QString, QString>> m;
    QString curLangCode = "en";
};

#endif // LOCALIZATION_H
