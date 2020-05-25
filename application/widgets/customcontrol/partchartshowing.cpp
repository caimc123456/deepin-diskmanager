/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     zhangkai <zhangkai@uniontech.com>
* Maintainer: zhangkai <zhangkai@uniontech.com>
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "partchartshowing.h"
#include <QDebug>
#include <QHBoxLayout>
PartChartShowing::PartChartShowing(QWidget *parent) : QWidget(parent)
{
//    tip = new ToolTip(this);
//    tip->hide();
}


void PartChartShowing::getData(const QString &totalsize, const QVector<double>sizeInfo, const QVector<QString> partNames, QVector<QColor>basecolor)
{
    int i = totalsize.lastIndexOf("G");
    total = totalsize.left(i).toDouble();
    if (total < 1) {
        total = total * 1024;
    }
    partsize = sizeInfo;
    color = basecolor;
    partname = partNames;
    total = static_cast<int>(total);
//    path = paintpath;
}

void PartChartShowing::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect;
    rect.setX(this->x());
    rect.setY(this->y());
    rect.setWidth(this->width());
    QRect paintRect = QRect(0, 10, rect.width(), 35);
    QPainterPath paintpath;
    const int radius = 8;

    paintpath.moveTo(paintRect.bottomRight() - QPoint(0, radius));
    paintpath.lineTo(paintRect.topRight() + QPoint(0, radius));
    paintpath.arcTo(QRect(QPoint(paintRect.topRight() - QPoint(radius * 2, 0)),
                          QSize(radius * 2, radius * 2)),
                    0, 90);
    paintpath.lineTo(paintRect.topLeft() + QPoint(radius, 0));
    paintpath.arcTo(QRect(QPoint(paintRect.topLeft()), QSize(radius * 2, radius * 2)), 90, 90);
    paintpath.lineTo(paintRect.bottomLeft() - QPoint(0, radius));
    paintpath.arcTo(QRect(QPoint(paintRect.bottomLeft() - QPoint(0, radius * 2)),
                          QSize(radius * 2, radius * 2)),
                    180, 90);
    paintpath.lineTo(paintRect.bottomRight() - QPoint(radius, 0));
    paintpath.arcTo(QRect(QPoint(paintRect.bottomRight() - QPoint(radius * 2, radius * 2)),
                          QSize(radius * 2, radius * 2)),
                    270, 90);

    QColor fillColor = QColor(this->palette().dark().color()); //this->palette().color(DPalette::Normal, DPalette::Highlight)
    painter.setBrush(QBrush(fillColor));
    painter.fillPath(paintpath, fillColor);
    painter.setPen(QPen(QColor(fillColor), 3));
    painter.drawLine(paintRect.topLeft().x() + radius, paintRect.topLeft().y() + radius, paintRect.topLeft().x() + radius + paintRect.width(), paintRect.bottomLeft().y() - radius);
    QColor tipColor = QColor(this->palette().dark().color());


    painter.setPen(tipColor);
    painter.drawRect(this->x(), this->y() + 50, 10, 10);

    painter.setPen(this->palette().text().color());
    QRect textRect(this->x() + 15, this->y() + 42, 80, 50);
    painter.drawText(textRect, tr("Free space"));


    QWidget::paintEvent(event);

    addPaint(&painter);
    painter.restore();


}

void PartChartShowing::addPaint(QPainter *painter)
{

    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing);
    QRect rect;
    rect.setX(this->x());
    rect.setY(this->y());
    rect.setWidth(this->width());
    QRect paintRect = QRect(0, 10, rect.width(), 35);
    QPainterPath path0, path1, path2, path3, path4, path5, path6, path7, path8, path9, path10, path11, path12;
    QVector<QPainterPath>path{path0, path1, path2, path3, path4, path5, path6, path7, path8, path9, path10, path11, path12};
    QPainterPath paintpath;
    const int radius = 8;
    double sum = 0.00;
    if (flag == 1) {
        painter->setPen(QColor(this->palette().highlight().color()));
        painter->drawRoundedRect(paintRect, 8, 8);
        flag = 0;
    }

    for (int i = 0; i < partsize.size(); i++) {
//        sum = sum + partsize.at(i);
        qDebug() << sum;
        qDebug() << partsize.at(i);
        sum = static_cast<int>(sum + partsize.at(i));
        if (i == 0 && sum >= total) {
            painter->setPen(QColor(this->palette().highlight().color()));
            painter->setBrush(color.at(0));
            painter->drawRoundedRect(paintRect, 8, 8);

        }
        if (i == 0) {
            if (static_cast<int>(sum) < static_cast<int>(total)) {
                path[0].moveTo(paintRect.topLeft() + QPoint(radius, 0));
                path[0].arcTo(QRect(QPoint(paintRect.topLeft()), QSize(radius * 2, radius * 2)), 90, 90);
                path[0].lineTo(paintRect.bottomLeft() - QPoint(0, radius));
                path[0].arcTo(QRect(QPoint(paintRect.bottomLeft() - QPoint(0, radius * 2)),
                                    QSize(radius * 2, radius * 2)),
                              180, 90);
                qDebug() << partsize.at(i) << total << partsize.at(i) / total << (static_cast<int>((partsize.at(i) / total)));
                path[0].lineTo(paintRect.bottomLeft() + QPoint(((partsize.at(i) / total)*paintRect.width() + radius), 0));
                path[0].lineTo(paintRect.topLeft() + QPoint(((partsize.at(i) / total)*paintRect.width() + radius), 0));
                path[0].lineTo(paintRect.topLeft() + QPoint(radius, 0));
            }
            if (static_cast<int>(sum) >= static_cast<int>(total)) {
                path[0].moveTo(paintRect.bottomRight() - QPoint(0, radius));
                path[0].lineTo(paintRect.topRight() + QPoint(0, radius));
                path[0].arcTo(QRect(QPoint(paintRect.topRight() - QPoint(radius * 2, 0)),
                                    QSize(radius * 2, radius * 2)),
                              0, 90);
                path[0].lineTo(paintRect.topLeft() + QPoint(radius, 0));
                path[0].arcTo(QRect(QPoint(paintRect.topLeft()), QSize(radius * 2, radius * 2)), 90, 90);
                path[0].lineTo(paintRect.bottomLeft() - QPoint(0, radius));
                path[0].arcTo(QRect(QPoint(paintRect.bottomLeft() - QPoint(0, radius * 2)),
                                    QSize(radius * 2, radius * 2)),
                              180, 90);
                path[0].lineTo(paintRect.bottomRight() - QPoint(radius, 0));
                path[0].arcTo(QRect(QPoint(paintRect.bottomRight() - QPoint(radius * 2, radius * 2)),
                                    QSize(radius * 2, radius * 2)),
                              270, 90);

            }
            painter->fillPath(path[0], QBrush(color.at(0)));
            if (number == 0) {
                painter->setBrush(QBrush(color.at(0)));
                painter->setPen(this->palette().color(DPalette::Normal, DPalette::Highlight));
                painter->drawPath(path[0]);
                number = -1;
                flag = 0;
            }

        } else if (sum < total && i > 0) {
            path[i].moveTo(path[i - 1].currentPosition() + QPoint(((partsize.at(i - 1) / total)*paintRect.width()), 0));
            path[i].lineTo(path[i - 1].currentPosition() + QPoint(((partsize.at(i - 1) / total)*paintRect.width() + (partsize.at(i) / total)*paintRect.width()), 0));
            path[i].lineTo(path[i - 1].currentPosition() + QPoint(((partsize.at(i - 1) / total)*paintRect.width() + (partsize.at(i) / total)*paintRect.width()), paintRect.height() - 1));
            path[i].lineTo(path[i - 1].currentPosition() + QPoint(((partsize.at(i - 1) / total)*paintRect.width()), paintRect.height() - 1));
            path[i].lineTo(path[i - 1].currentPosition() + QPoint(((partsize.at(i - 1) / total)*paintRect.width()), 0));
            QColor fillcolor;
            if (i > color.size() - 1) {
                fillcolor = color.at(i % (color.size() - 1) - 1);
            } else {
                fillcolor = color.at(i);
            }
            painter->fillPath(path[i], QBrush(fillcolor));
            if (number > 0 && number != -1 && number == i) {
                painter->setBrush(fillcolor);
                painter->setPen(this->palette().color(DPalette::Normal, DPalette::Highlight));
                painter->drawPath(path[number]);
                flag = 0;
            }
        } else if (sum >= total) {
            path[partsize.size() - 1].moveTo(path[partsize.size() - 2].currentPosition() + QPoint(((partsize.at(partsize.size() - 2) / total)*paintRect.width()), 0));
            path[partsize.size() - 1].lineTo(paintRect.topRight() - QPoint(radius, 0));
            path[partsize.size() - 1].arcTo(QRect(QPoint(paintRect.topRight() - QPoint(radius * 2, 0)),
                                                  QSize(radius * 2, radius * 2)),
                                            90, -90);
            path[partsize.size() - 1].lineTo(paintRect.bottomRight() - QPoint(0, radius));

            path[partsize.size() - 1].arcTo(QRect(QPoint(paintRect.bottomRight() - QPoint(radius * 2, radius * 2)),
                                                  QSize(radius * 2, radius * 2)),
                                            0, -90);
            path[partsize.size() - 1].lineTo(path[partsize.size() - 2].currentPosition() + QPoint(((partsize.at(partsize.size() - 2) / total))*paintRect.width(), paintRect.height() - 1));
            path[partsize.size() - 1].lineTo(path[partsize.size() - 2].currentPosition() + QPoint(((partsize.at(partsize.size() - 2) / total))*paintRect.width(), 0));
            QColor fillcolor;
            if (i > color.size() - 1) {
                fillcolor = color.at(i % (color.size() - 1) - 1);
            } else {
                fillcolor = color.at(i);
            }
            painter->fillPath(path[partsize.size() - 1], QBrush(fillcolor));
            if (number == partsize.size() - 1) {
                painter->setPen(this->palette().color(DPalette::Normal, DPalette::Highlight));
                painter->setBrush(fillcolor);
                painter->drawPath(path[partsize.size() - 1]);
                number = -1;
                qDebug() << path[partsize.size() - 1].currentPosition().x() << "yes";

            }
            qDebug() << number << flag;
            qDebug() << path[partsize.size() - 1].currentPosition().x();
        }
        allpath = path;
//        qDebug() << allpath.size() << partsize.size();
    }
    if (flag == 3) {

        painter->setPen(this->palette().color(DPalette::Normal, DPalette::Highlight));
        //                paintpath.moveTo()
        paintpath.moveTo(paintRect.bottomRight() - QPoint(0, radius));
        paintpath.lineTo(paintRect.topRight() + QPoint(0, radius));
        paintpath.arcTo(QRect(QPoint(paintRect.topRight() - QPoint(radius * 2, 0)),
                              QSize(radius * 2, radius * 2)),
                        0, 90);
        paintpath.lineTo(QPoint(allpath[partsize.size() - 1].currentPosition().x() + (partsize.at(partsize.size() - 1) / total * this->width()), 10));
        paintpath.lineTo(QPoint(allpath[partsize.size() - 1].currentPosition().x() + (partsize.at(partsize.size() - 1) / total * this->width()), paintRect.height() + 9));
        paintpath.lineTo(paintRect.bottomRight() - QPoint(radius, 0));
        paintpath.arcTo(QRect(QPoint(paintRect.bottomRight() - QPoint(radius * 2, radius * 2)),
                              QSize(radius * 2, radius * 2)),
                        270, 90);
        painter->drawPath(paintpath);
        flag = 0;
    }

}

void PartChartShowing::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    int i ;
    if (partsize.size() == 0) {
        if (event->button() == Qt::LeftButton) {
//            qDebug() << partsize.size();
            if (x > 0 && x < this->width() && y > 10 && y < 45) {
                flag = 1;
                qDebug() << flag << event->pos();
                update();
            }

        }
    } else if (partsize.size() > 0) {
        if (event->button() == Qt::LeftButton) {
            for (i = 0; i < partsize.size(); i++) {
                int width = ((partsize.at(i) / total) * this->width());
                if ((x > allpath[i].currentPosition().x() && x < (allpath[i].currentPosition().x() + width) && y > 10 && y < 45) || (i == 0 && y > 10 && y < 45)) {
                    qDebug() << "121212121";
                    flag = 2;
                    number = i;
                    update();
                }
            }
            int width = ((partsize.at(partsize.size() - 1) / total) * this->width());
            //空闲分区
            if (x > allpath[partsize.size() - 1].currentPosition().x() + width && x < this->width() && y > 10 && y < 45 && static_cast<int>(sum) < static_cast<int>(total)) {
                flag = 3;
                i = -1;
                qDebug() << "23232323";
                number = 100;
                update();
            }
        }
    }

    emit sendFlag(flag, number, x);




}

