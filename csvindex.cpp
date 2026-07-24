#include "csvindex.h"

void CSVIndex::build(const char* data, qint64 size)
{
    clear();

    m_data = data;
    m_size = size;

    if (!m_data || m_size <= 0)
        return;

    qint64 lineStart = 0;
    bool first = true;

    for (qint64 i = 0; i < m_size; i++)
    {
        // does not handle '\n' inside quotes

        char c = m_data[i];
        if (c == '\n')
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

void CSVIndex::clear()
{
    m_data = nullptr;
    m_size = 0;
    m_rowOffsets.clear();
    m_headers.clear();
}

int CSVIndex::rowCount() const
{
    return static_cast<int>(m_rowOffsets.size());
}

int CSVIndex::columnCount() const
{
    return m_headers.size();
}

const QStringList& CSVIndex::headers() const
{
    return m_headers;
}

CSVIndex::FieldRef CSVIndex::fieldAt(int row, int column) const
{
    if (row < 0 || static_cast<size_t>(row) >= m_rowOffsets.size())
        return {};

    if (column < 0)
        return {};

    return findField(m_rowOffsets[row], column);
}

QStringList CSVIndex::parseLine(const char* data, qint64 lineStart, qint64 lineEnd)
{
    QStringList fields;
    QByteArray current;
    bool inQuotes = false;

    for (qint64 i = lineStart; i < lineEnd; i++)
    {
        char c = data[i];

        if (c == '"')
        {
            inQuotes = !inQuotes;
            continue;
        }
        if (c == ',' && !inQuotes)
        {
            fields << QString::fromUtf8(current);
            current.clear();
            continue;
        }
        current += c;
    }
    fields << QString::fromUtf8(current);

    return fields;
}

CSVIndex::FieldRef CSVIndex::findField(qint64 rowStart, int column) const
{
    bool inQuotes = false;
    int separatorCount = 0;
    qint64 fieldStart = rowStart;

    for (qint64 i = rowStart; i < m_size; i++)
    {
        char c = m_data[i];

        if (c == '"')
        {
            inQuotes = !inQuotes;
        }
        else if ((c == ',' || c == '\n') && !inQuotes)
        {
            if (separatorCount == column)
            {
                qint64 fieldEnd = i;
                if (fieldEnd > fieldStart && m_data[fieldEnd - 1] == '\r')
                    fieldEnd--;
                return FieldRef{fieldStart, fieldEnd - fieldStart};
            }
            fieldStart = i + 1;
            separatorCount++;
        }
    }

    return {};
}