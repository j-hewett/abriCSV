#include "csvtablemodel.h"
#include <QFile>
#include <QTextStream>

CSVTableModel::CSVTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

void CSVTableModel::loadCSV(const QString &filePath)
{
    beginResetModel();

    m_csvIndex.clear();
    if (m_mappedData) {
        m_file.unmap(m_mappedData);
        m_mappedData = nullptr;
    }
    m_file.close();

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

    m_csvIndex.build(reinterpret_cast<const char*>(m_mappedData), m_file.size());

    endResetModel();
}

int CSVTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_csvIndex.rowCount();
}

int CSVTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_csvIndex.columnCount();
}

QVariant CSVTableModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal)
    {
        const QStringList &headers = m_csvIndex.headers();
        if (section < headers.size())
            return headers.at(section);
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

    CSVIndex::FieldRef ref = m_csvIndex.fieldAt(index.row(), index.column());
    if (!ref.isValid())
        return {};

    return QString::fromUtf8(m_mappedData ? reinterpret_cast<const char*>(m_mappedData) + ref.start
                                          : nullptr, ref.length).trimmed();
}
