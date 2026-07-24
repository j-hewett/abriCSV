#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H

#include "csvindex.h"
#include <QAbstractTableModel>
#include <QFile>

class CSVTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CSVTableModel(QObject *parent = nullptr);

    void loadCSV(const QString &filePath);

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:


    QFile m_file;
    uchar* m_mappedData = nullptr;
    CSVIndex m_csvIndex;

};

#endif // CSVTABLEMODEL_H
