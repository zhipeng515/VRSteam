#ifndef MODELBASE_H
#define MODELBASE_H

#include "JObject.h"
#include <QFile>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>

#define DECLARE_MODEL_BEGIN(modelclass, baseclass) \
\
class modelclass : public baseclass \
{ \
public: \
    explicit modelclass(QObject *parent = 0) : baseclass(parent){} \
\
    inline bool isValid(){ return this != &modelclass::invalidModel; } \
\
    static inline modelclass * getModel(const int id) { \
        auto model = modelsMap.find(id); \
        if(model != modelsMap.end()) \
            return model.value(); \
        return &modelclass::invalidModel; \
    } \
\
    static inline modelclass * getModel(const QString & name){ \
        for( auto model = modelsMap.begin(); model != modelsMap.end(); model++) {\
            if(model.value()->name() == name) \
                return model.value(); \
        } \
        return &modelclass::invalidModel; \
    } \
\
    static inline void addModel(const int id, modelclass * model) { \
        modelsMap.insert(id, model); \
    } \
\
    static inline void removeModel(const int id) { \
        auto model = modelsMap.find(id); \
        if(model != modelsMap.end()) { \
            delete model.value(); \
            modelsMap.erase(model); \
        } \
    } \
\
    static inline void removeAllModels() { \
        qDeleteAll(modelsMap); \
        modelsMap.clear(); \
    } \
    static inline void saveModels(const QString & localFile) { \
        QByteArray jsonByteArray("{\n"); \
        for( auto model = modelsMap.begin(); model != modelsMap.end(); model++) {\
            jsonByteArray.append("\"" + QString::number(model.key()) + "\"" + ": "); \
            jsonByteArray.append(model.value()->exportToJson()); \
            jsonByteArray.append(",\n"); \
        } \
        if(jsonByteArray.endsWith(",\n")) \
            jsonByteArray.remove(jsonByteArray.count()-2, 2); \
        jsonByteArray.append('}'); \
        QFile jsonFile(localFile); \
        jsonFile.open(QIODevice::WriteOnly); \
        jsonFile.write(jsonByteArray); \
        jsonFile.close(); \
    } \
    static inline void loadModels(const QString & localFile) { \
        QFile jsonFile(localFile); \
        jsonFile.open(QIODevice::ReadOnly); \
        QByteArray jsonByteArray = jsonFile.readAll(); \
        jsonFile.close(); \
\
        QJsonParseError error; \
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonByteArray, &error); \
        if (error.error == QJsonParseError::NoError) { \
            if (jsonDocument.isObject()) { \
                QVariantMap jsonMap = jsonDocument.object().toVariantMap(); \
                for(auto json = jsonMap.begin(); json != jsonMap.end(); json++) \
                { \
                    modelclass * model = new modelclass; \
                    model->importFromVariant(json.value()); \
                    modelclass::addModel(json.key().toInt(), model); \
                } \
            } \
        } \
    } \
\
    static inline QMap<int, modelclass*> & models(){ return modelsMap; } \
    static modelclass invalidModel; \
\
private: \
    static QMap<int, modelclass*> modelsMap;

#define DECLARE_MODEL_END(modelclass) \
};

#define IMPLMENT_MODEL(modelclass) \
modelclass modelclass::invalidModel; \
QMap<int, modelclass*> modelclass::modelsMap;

#endif // MODELBASE_H
