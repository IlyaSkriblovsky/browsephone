#pragma once

#include <QString>
#include <QVariant>

class JsonUtil
{
    public:
        static QString serialize(const QVariant& value, int indent = 0);
        static QString string(const QString& str);
};
