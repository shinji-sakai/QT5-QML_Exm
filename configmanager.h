#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "configreader.h"
#include <QtCore>

class ConfigManager : public QObject, public ConfigReader
{
    Q_OBJECT
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
public:
    ConfigManager();

signals:
    void languageChanged();

public slots:
    // Load / Save external slots
    void loadConfig(QString fileName);
    void saveConfig(QString fileName);

    // Methods for language
    QString language();
    void setLanguage(QString lang);

    // Get key and category list
    QStringList keys();
    QStringList categories();

    // Get name and description
    QString getName(QString fieldName);
    QString getDescription(QString fieldName);

    // Get / Set value
    QJsonObject getKeyValue(QString fieldName);
    void setKeyValue(QString fieldName, QVariant value);
};

#endif // CONFIGMANAGER_H
