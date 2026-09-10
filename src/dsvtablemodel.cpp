#include "dsvtablemodel.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

DSVTableModel::DSVTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

void DSVTableModel::loadFile(const QString &filePath)
{
    beginResetModel();

    m_dsvIndex.clear();
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

    QChar delimiter = QLatin1Char(',');
    if (QFileInfo(filePath).suffix().compare(QStringLiteral("tsv"), Qt::CaseInsensitive) == 0)
    {
        delimiter = QLatin1Char('\t');
    }

    m_dsvIndex.build(reinterpret_cast<const char*>(m_mappedData), m_file.size(), delimiter);

    endResetModel();
}

void DSVTableModel::clear()
{
    beginResetModel();
    m_dsvIndex.clear();
    if (m_mappedData) {
        m_file.unmap(m_mappedData);
        m_mappedData = nullptr;
    }
    if(m_file.isOpen()) { m_file.close(); }

    endResetModel();
}

int DSVTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_dsvIndex.rowCount();
}

int DSVTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_dsvIndex.columnCount();
}

QVariant DSVTableModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal)
    {
        const QStringList &headers = m_dsvIndex.headers();
        if (section < headers.size())
            return headers.at(section);
        return {};
    }
    else
    {
        return section + 1;
    }
}

QVariant DSVTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    DSVIndex::FieldRef ref = m_dsvIndex.fieldAt(index.row(), index.column());
    if (!ref.isValid())
        return {};

    return QString::fromUtf8(m_mappedData ? reinterpret_cast<const char*>(m_mappedData) + ref.start
                                          : nullptr, ref.length).trimmed();
}
