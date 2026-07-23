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

