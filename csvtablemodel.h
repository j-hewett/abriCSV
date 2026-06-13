#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H

#include <QAbstractTableModel>
#include <QFile>

class CSVTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CSVTableModel(QObject *parent = nullptr);

    void loadCSV(const QString &filePath);
    QStringList parseCSVLine(QString& line);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    qint64 findField(qint64 rowStart, int column, qint64& fieldEnd) const;


    QFile m_file;
    uchar* m_mappedData = nullptr;
    qint64 m_fileSize = 0;
    QVector<qint64> m_rowOffsets;

    QStringList m_headers;

};

#endif // CSVTABLEMODEL_H
