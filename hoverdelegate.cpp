#include "hoverdelegate.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTableView>
#include <QApplication>

HoverDelegate::HoverDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void HoverDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    
    // If this is the hovered cell, fill with red background
    if (index == hoveredIndex) {
        painter->save();
        painter->fillRect(option.rect, QColor(255, 0, 0, 50)); // Semi-transparent red
        painter->restore();
    }
    
    // Draw the rest of the cell normally
    QStyledItemDelegate::paint(painter, opt, index);
}

bool HoverDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    switch (event->type()) {
        case QEvent::MouseMove: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (option.rect.contains(mouseEvent->pos())) {
                if (hoveredIndex != index) {
                    QModelIndex oldHovered = hoveredIndex;
                    hoveredIndex = index;
                    
                    // Force redraw of both old and new hovered cells
                    if (auto view = qobject_cast<QTableView*>(parent())) {
                        if (oldHovered.isValid()) {
                            view->viewport()->update(view->visualRect(oldHovered));
                        }
                        view->viewport()->update(view->visualRect(hoveredIndex));
                    }
                }
            }
            break;
        }
        case QEvent::Leave: {
            if (hoveredIndex.isValid()) {
                QModelIndex oldHovered = hoveredIndex;
                hoveredIndex = QModelIndex();
                
                // Force redraw when mouse leaves
                if (auto view = qobject_cast<QTableView*>(parent())) {
                    view->viewport()->update(view->visualRect(oldHovered));
                }
            }
            break;
        }
        default:
            break;
    }
    
    return QStyledItemDelegate::editorEvent(event, model, option, index);
} 