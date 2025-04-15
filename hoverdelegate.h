#ifndef HOVERDELEGATE_H
#define HOVERDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QStyleOptionViewItem>

class HoverDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit HoverDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    QModelIndex hoveredIndex;
};

#endif // HOVERDELEGATE_H 