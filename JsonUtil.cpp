#include "JsonUtil.h"

#include <QStringList>
#include <QDateTime>
#include <QUrl>


QString JsonUtil::string(const QString& string)
{
    QString r = string;
    r.replace("\"", "\\\"");
    r.replace("\r\n", "\\n");
    r.replace("\n", "\\n");
    return QString("\"%1\"").arg(r);
}

QString JsonUtil::serialize(const QVariant& value, int indent)
{
    switch (value.type())
    {
        case QVariant::Invalid: return "\"<invalid>\"";
        case QVariant::Bool: return value.toBool() ? "true" : "false";
        case QVariant::Double: return QString::number(value.toDouble());
        case QVariant::Int: return QString::number(value.toInt());

        case QVariant::String: return string(value.toString());

        case QVariant::Time: return value.toTime().toString("\"hh:mm:ss\"");
        case QVariant::Date: return value.toDate().toString("\"yyyy-MM-dd\"");
        case QVariant::DateTime: return value.toDateTime().toString("\"yyyy-MM-dd hh:mm:ss\"");

        case QVariant::Url: return QString("\"%1\"").arg(value.toUrl().toString());

        case QVariant::List: {
            QVariantList childs = value.toList();
            QStringList str;
            foreach (const QVariant& child, childs)
                str.append(QString(indent+2, QChar(' ')) + serialize(child, indent + 2));
            return QString("[\n%1\n%2]")
                .arg(str.join(",\n"))
                .arg(QString(indent, QChar(' ')));
        }

        case QVariant::StringList: {
            QStringList vals = value.toStringList();
            for (int i = 0; i < vals.size(); i++)
                vals[i] = QString("%1").arg(string(vals[i]));
            return QString("[ %1 ]").arg(vals.join(", "));
        }

        case QVariant::Map: {
            QVariantMap childs = value.toMap();
            QStringList str;

            foreach (const QString& key, childs.keys())
            {
                QString keyCopy = key;
                str.append(QString("%1%2: %3")
                    .arg(QString(indent+2, ' '))
                    .arg(string(key))
                    .arg(serialize(childs.value(key), indent+2))
                );
            }

            return QString("{\n%1\n%2}")
                .arg(str.join(",\n"))
                .arg(QString(indent, ' '));
        }

        default: return "null /* unknown */";
    }
}
