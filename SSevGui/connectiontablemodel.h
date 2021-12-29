/*
 * Copyright (C) 2015-2016 Symeon Huang <hzwhuang@gmail.com>
 *
 * shadowsocks-qt5 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * shadowsocks-qt5 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libQtShadowsocks; see the file LICENSE. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef CONNECTIONTABLEMODEL_H
#define CONNECTIONTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "connectionitem.h"
#include "DataDefine.h"

class ConnectionTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit ConnectionTableModel(QObject *parent = nullptr);
    ~ConnectionTableModel();

	const QMap<QString, QList<ConnectionItem*> >& connectionItems() const;
    ConnectionItem *getItem(const int &row) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex()) Q_DECL_OVERRIDE;
	void removeAirsConnection(QString airportUrl);
    bool move(int row, int target, const QModelIndex &parent = QModelIndex());
	void clearItems();

    bool appendConnection(Connection *con, const QModelIndex &parent = QModelIndex());
	bool appendProfiles(const QList<SQProfile>& pros);

    void disconnectConnectionsAt(const QString &addr, quint16 port);

	QStringList header_;

public slots:
    void testAllLatency();
	void addAirConnection(QList<SQProfile> profiles, AirportInfo airInfo);

signals:
    void message(const QString &);
    void rowStatusChanged(int row, bool running);

private:
    QList<ConnectionItem*> items;

	// 该Map只有索引功能并不维持Url为最新，最新Url在配置文件中
	QMap<QString, QList<ConnectionItem*>> mapAirItems_;
    static QString convertLatencyToString(const int latency);

private slots:
    void onConnectionStateChanged(bool running);
    void onConnectionLatencyChanged();
    void onConnectionDataUsageChanged();
};

#endif // CONNECTIONTABLEMODEL_H