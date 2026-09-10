#include "dsvindex.h"
#include <qtypes.h>

void DSVIndex::build(const char* data, qint64 size, QChar delim)
{
    clear();

    m_data = data;
    m_size = size;
    m_delimiter = delim;

    if (!m_data || m_size <= 0)
        return;

    qint64 lineStart = 0;
    bool first = true;
    bool inQuotes = false;

    for (qint64 i = 0; i < m_size; i++)
    {
        char c = m_data[i];
        if (c == '"')
        {
            inQuotes = !inQuotes;
        }
        else if (c == '\n' && !inQuotes)
        {
            if (first)
            {
                m_headers = parseLine(m_data, lineStart, i);
                first = false;
            }
            m_rowOffsets.push_back(i + 1);
            lineStart = i + 1;
        }
    }
}

void DSVIndex::clear()
{
    m_data = nullptr;
    m_size = 0;
    m_rowOffsets.clear();
    m_headers.clear();
}

int DSVIndex::rowCount() const
{
    return static_cast<int>(m_rowOffsets.size());
}

int DSVIndex::columnCount() const
{
    return static_cast<int>(m_headers.size());
}

const QStringList& DSVIndex::headers() const
{
    return m_headers;
}

DSVIndex::FieldRef DSVIndex::fieldAt(int row, int column) const
{
    if (row < 0 || row >= rowCount())
        return {};

    if (column < 0 || column >= columnCount())
        return {};

    return findField(m_rowOffsets[row], column);
}

qint64 DSVIndex::scanField(qint64 start, qint64 limit, QChar delim) const
{
    bool inQuotes = false;
    for (qint64 i = start; i < limit; i++) {
        const char c = m_data[i];
        if (c == '"')
            inQuotes = !inQuotes;
        else if ((c == delim || c == '\n') && !inQuotes)
            return i;
    }
    return limit;
}

QStringList DSVIndex::parseLine(const char *data, qint64 lineStart, qint64 lineEnd) const
{
    QStringList fields;
    qint64 fieldStart = lineStart;
    while (fieldStart <= lineEnd) {
        const qint64 sepPos = scanField(fieldStart, lineEnd, m_delimiter);
        fields << QString::fromUtf8(data + fieldStart, sepPos - fieldStart).remove(QLatin1Char('"'));
        fieldStart = sepPos + 1;
    }
    return fields;
}

DSVIndex::FieldRef DSVIndex::findField(qint64 rowStart, int column) const
{
    qint64 fieldStart = rowStart;
    for (int i = 0; i <= column; i++) {
        const qint64 sepPos = scanField(fieldStart, m_size, m_delimiter);
        if (i == column) {
            qint64 fieldEnd = sepPos;
            if (fieldEnd > fieldStart && m_data[fieldEnd - 1] == '\r')
                fieldEnd--;
            return FieldRef{fieldStart, fieldEnd - fieldStart};
        }
        fieldStart = sepPos + 1;
    }
    return {};
}