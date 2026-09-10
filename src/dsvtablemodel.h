#ifndef DSVTABLEMODEL_H
#define DSVTABLEMODEL_H

#include "dsvindex.h"
#include <QAbstractTableModel>
#include <QFile>

class DSVTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DSVTableModel(QObject *parent = nullptr);
    void loadCSV(const QString &filePath);
    void clear();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QFile m_file;
    uchar* m_mappedData = nullptr;
    DSVIndex m_dsvIndex;
};

#endif // DSVTABLEMODEL_H
