#include "csvtablemodel.h"
#include <QFile>
#include <QTextStream>

CSVTableModel::CSVTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

void CSVTableModel::loadCSV(const QString &filePath)
{
    beginResetModel();
    if (m_mappedData) {
        m_file.unmap(m_mappedData);
        m_mappedData = nullptr;
    }
    m_file.close();

    m_rowOffsets.clear();
    m_headers.clear();
    m_fileSize = 0;

    m_file.setFileName(filePath);

    if (!m_file.open(QIODevice::ReadOnly))
    {
        endResetModel();
        return;
    }

    m_mappedData = m_file.map(0, m_file.size());
    if (!m_mappedData)
    {
        endResetModel();
        return;
    }

    m_fileSize = m_file.size();

    bool first = true;
    for (qint64 i = 0; i < m_fileSize; i++)
    {
        char c = m_mappedData[i];
        if (c == '\n')
        {
            if(first)
            {
                QString line = QString::fromUtf8(reinterpret_cast<const char*>(m_mappedData), i);
                m_headers = parseCSVLine(line);
                first = false;
            }
            m_rowOffsets.push_back(i + 1);
        }
    }

    endResetModel();
}

QStringList CSVTableModel::parseCSVLine(QString& line)
{
    QStringList fields;
    QString current;

    // don't treat commas inside quotes as separators
    // this logic strips quotes from content - check if desired behaviour
    bool inQuotes = false;

    for (const QChar c : line)
    {
        if (c == '"')
        {
            inQuotes = !inQuotes;
            continue;
        }
        if (c == ',' && !inQuotes)
        {
            fields << current; current.clear();
            continue;
        }
        current += c;
    }

    fields.append(current);
    return fields;
}

qint64 CSVTableModel::findField(qint64 rowStart, int column, qint64& fieldEnd) const
{
    bool inQuotes = false;
    int separatorCount = 0;
    qint64 fieldStart = rowStart;

    for (qint64 i = rowStart; i < m_fileSize; i++)
    {
        char c = m_mappedData[i];

        if (c == '"')
        {
            inQuotes = !inQuotes;
        }
        else if ((c == ',' || c == '\n') && !inQuotes)
        {
            if (separatorCount == column)
            {
                fieldEnd = i;
                if (fieldEnd > fieldStart && m_mappedData[fieldEnd - 1] == '\r')
                    fieldEnd--;
                return fieldStart;
            }
            fieldStart = i + 1;
            separatorCount++;
        }
    }
    return -1; // field not found
}

int CSVTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_rowOffsets.size();
}

int CSVTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_headers.size();
}

QVariant CSVTableModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal)
    {
        if (section < m_headers.size())
            return m_headers.at(section);
        return {};
    }
    else
    {
        return section + 1;
    }
}

QVariant CSVTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();
    if (index.row() >= m_rowOffsets.size())
        return {};

    qint64 rowStart = m_rowOffsets[index.row()];
    int separatorCount = 0;

    qint64 fieldEnd = 0;
    qint64 fieldStart = findField(rowStart, index.column(), fieldEnd);
    if (fieldStart == -1)
        return {};
    return QString::fromUtf8(
               reinterpret_cast<const char*>(m_mappedData + fieldStart),
               fieldEnd - fieldStart).trimmed();
}
