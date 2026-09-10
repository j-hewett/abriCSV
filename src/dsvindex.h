#ifndef DSVINDEX_H
#define DSVINDEX_H

#include <QList>
#include <QChar>

class DSVIndex
{
public:
    struct FieldRef
    {
        qint64 start = -1;
        qint64 length = 0;

        bool isValid() const { return start >= 0; }
    };

    DSVIndex() = default;

    void build(const char* data, qint64 size, QChar delim);
    void clear();

    int rowCount() const;
    int columnCount() const;
    const QStringList& headers() const;

    FieldRef fieldAt(int row, int column) const;

private:
    qint64 scanField(qint64 start, qint64 limit, QChar delim) const;
    QStringList parseLine(const char *data, qint64 lineStart, qint64 lineEnd) const;
    FieldRef findField(qint64 rowStart, int column) const;

private:
    const char* m_data = nullptr;
    qint64 m_size = 0;
    QChar m_delimiter = ',';
    QList<qint64> m_rowOffsets;
    QStringList m_headers;
};

#endif // DSVINDEX_H
