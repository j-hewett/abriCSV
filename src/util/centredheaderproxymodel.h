#ifndef CENTREDHEADERPROXYMODEL_H
#define CENTREDHEADERPROXYMODEL_H

#include <QIdentityProxyModel>

class CentredHeaderProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    using QIdentityProxyModel::QIdentityProxyModel;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (role == Qt::TextAlignmentRole && orientation == Qt::Horizontal) {
            return QVariant(Qt::AlignCenter);
        }
        return QIdentityProxyModel::headerData(section, orientation, role);
    }
};

#endif // CENTREDHEADERPROXYMODEL_H
