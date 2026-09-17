#ifndef COLUMNFITTER_H
#define COLUMNFITTER_H

#include <QObject>
#include <QTableView>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QResizeEvent>
#include <QTimer>
#include <QSet>
#include <vector>
#include <algorithm>

class ColumnFitter : public QObject
{
public:
    explicit ColumnFitter(QTableView *view)
        : QObject(view), m_view(view)
    {
        QHeaderView *header = m_view->horizontalHeader();

        // only query visible rows
        header->setResizeContentsPrecision(0);

        m_view->viewport()->installEventFilter(this);

        connect(header, &QHeaderView::sectionResized, this,
                [this](int logicalIndex, int, int newSize) {
                    if (!m_applying && newSize > 0)
                        m_pinned.insert(logicalIndex);
                });

        if (auto *model = m_view->model()) {
            connect(model, &QAbstractItemModel::modelReset,        this, &ColumnFitter::invalidate);
        }
        schedule();
    }

    void clearManualWidths()
    {
        m_pinned.clear();
        schedule();
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (obj == m_view->viewport() && event->type() == QEvent::Resize) {
            auto *re = static_cast<QResizeEvent *>(event);
            schedule();
        }
        return QObject::eventFilter(obj, event);
    }

private:
    void invalidate()
    {
        m_idealsDirty = true;
        schedule();
    }

    void schedule()
    {
        if (m_pending)
            return;
        m_pending = true;
        QTimer::singleShot(0, this, [this] {
            m_pending = false;
            fit();
        });
    }

    void recomputeIdeals()
    {
        QAbstractItemModel *model = m_view->model();
        QHeaderView *header = m_view->horizontalHeader();
        auto *itemView = static_cast<QAbstractItemView *>(m_view);

        const int columnCount = model ? model->columnCount() : 0;
        m_ideal.assign(columnCount, 0);
        for (int c = 0; c < columnCount; ++c) {
            if (header->isSectionHidden(c) || m_pinned.contains(c))
                continue;
            m_ideal[c] = std::max(itemView->sizeHintForColumn(c),
                                  header->sectionSizeHint(c));
        }
        m_idealsDirty = false;
    }

    void fit()
    {
        QAbstractItemModel *model = m_view->model();
        if (!model)
            return;

        const int columnCount = model->columnCount();
        if (m_idealsDirty || static_cast<int>(m_ideal.size()) != columnCount)
            recomputeIdeals();

        QHeaderView *header = m_view->horizontalHeader();
        const int base = header->defaultSectionSize();

        int available = m_view->viewport()->width();
        std::vector<int> autoCols;
        std::vector<int> needy;
        for (int c = 0; c < columnCount; ++c) {
            if (header->isSectionHidden(c))
                continue;
            if (m_pinned.contains(c)) {
                available -= header->sectionSize(c);
                continue;
            }
            autoCols.push_back(c);
            if (m_ideal[c] > base)
                needy.push_back(c);
        }
        if (autoCols.empty())
            return;

        std::vector<int> width(columnCount, base);
        int extra = available - base * static_cast<int>(autoCols.size());

        while (extra > 0 && !needy.empty()) {
            const int n = static_cast<int>(needy.size());
            const int share = extra / n;
            const int remainder = extra % n;

            std::vector<int> stillNeedy;
            int used = 0;
            for (int k = 0; k < n; ++k) {
                int c = needy[k];
                int give = share + (k < remainder ? 1 : 0);
                int g = std::min(give, m_ideal[c] - width[c]);
                width[c] += g;
                used += g;
                if (width[c] < m_ideal[c])
                    stillNeedy.push_back(c);
            }

            if (used == 0)
                break;
            extra -= used;
            needy.swap(stillNeedy);
        }

        m_applying = true;   // don't treat our own resizes as user pins
        for (int c : autoCols)
            if (header->sectionSize(c) != width[c])
                header->resizeSection(c, width[c]);
        m_applying = false;
    }

    QTableView *m_view;
    std::vector<int> m_ideal;
    QSet<int> m_pinned;
    bool m_idealsDirty = true;
    bool m_pending = false;
    bool m_applying = false;
};

#endif // COLUMNFITTER_H