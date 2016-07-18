#ifndef MODELBASE_H
#define MODELBASE_H

#include "JObject.h"

#define DECLARE_MODEL_BEGIN(modelclass, baseclass) \
\
class modelclass : public baseclass \
{ \
public: \
    explicit modelclass(QObject *parent = 0) : baseclass(parent){ \
        id(-1); \
    } \
    virtual ~modelclass() { \
        for( auto model = modelsMap.begin(); model != modelsMap.end(); model++) {\
            delete (*model).second; \
        } \
        modelsMap.clear(); \
    } \
\
    inline bool isValid(){ return this != &modelclass::invalidModel; } \
\
    static inline modelclass * getModel(const int id) { \
        auto model = modelsMap.find(id); \
        if(model != modelsMap.end()) \
            return (*model).second; \
        return &modelclass::invalidModel; \
    } \
\
    static inline modelclass * getModel(const QString & name){ \
        for( auto model = modelsMap.begin(); model != modelsMap.end(); model++) {\
            if((*model).second->name() == name) \
                return (*model).second; \
        } \
        return &modelclass::invalidModel; \
    } \
\
    static inline void addModel(const int id, modelclass * model) { \
        modelsMap.insert(std::map<const int, modelclass*>::value_type(id, model)); \
    } \
\
    static inline std::map<const int, modelclass*> & models(){ return modelsMap; } \
    static modelclass invalidModel; \
\
private: \
    static std::map<const int, modelclass*> modelsMap; \
\
public slots: \
    MetaPropertyPublicSet(int, id) \
    MetaPropertyPublicSet(QString, name)


#define DECLARE_MODEL_END(modelclass) \
};

#define IMPLMENT_MODEL(modelclass) \
modelclass modelclass::invalidModel; \
std::map<const int, modelclass*> modelclass::modelsMap;

#endif // MODELBASE_H
