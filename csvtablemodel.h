#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H

#include <QAbstractTableModel>

class CSVTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CSVTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int extracted() const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void loadCSV(const QString &filePath);

private:
    QList<QStringList> m_data;
    QStringList m_headers;
    int m_rowCount = 0;
    int m_columnCount = 0;
};

#endif // CSVTABLEMODEL_H
