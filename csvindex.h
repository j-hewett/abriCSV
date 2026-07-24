#ifndef CSVINDEX_H
#define CSVINDEX_H

#include <QList>

class CSVIndex
{
public:
    struct FieldRef
    {
        qint64 start = -1;
        qint64 length = 0;

        bool isValid() const { return start >= 0; }
    };

    CSVIndex() = default;

    void build(const char* data, qint64 size);
    void clear();

    int rowCount() const;
    int columnCount() const;
    const QStringList& headers() const;

    FieldRef fieldAt(int row, int column) const;

private:
    static QStringList parseLine(const char* data, qint64 lineStart, qint64 lineEnd);
    FieldRef findField(qint64 rowStart, int column) const;

private:
    const char* m_data = nullptr;
    qint64 m_size = 0;

    QList<qint64> m_rowOffsets;
    QStringList m_headers;
};

#endif // CSVINDEX_H
