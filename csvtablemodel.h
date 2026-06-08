#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H

#include <QObject>
#include <QAbstractItemModel>

class CSVTableModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CSVTableModel(QWidget *parent = nullptr);
    ~CSVTableModel();

private:
    int m_rowCount;
    int m_columnCount;

public:
    int rowCount(const QModelIndex&) const override
    {
        return m_rowCount;
    }
    int columnCount(const QModelIndex&) const override
    {
        return m_columnCount;
    }

};

#endif // CSVTABLEMODEL_H
