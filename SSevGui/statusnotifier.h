/*
 * Copyright (C) 2015-2017 Symeon Huang <hzwhuang@gmail.com>
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

#ifndef STATUSNOTIFIER_H
#define STATUSNOTIFIER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>

class MainWindow;
class ConnectionItem;
class ServerAction;
class StatusNotifier : public QObject
{
    Q_OBJECT
public:
	StatusNotifier(MainWindow *w, bool startHiden, QObject *parent = 0);

	/**
	 * @brief 更新托盘菜单中的服务器列表
	 */
	void updateServerList();

public slots:
    void activate();
    void showNotification(const QString &);
    void onWindowVisibleChanged(bool visible);

private slots:
	void serverMenuClicked(bool checked);

private:
	QMenu systrayMenu;
	QMenu serverList_;
    QAction *minimiseRestoreAction;
    QSystemTrayIcon systray;
    MainWindow *window;
};

class ServerAction : public QAction {
	Q_OBJECT
public:
	ServerAction(QString text, QWidget* parent);

public slots:
	void onCurrentItemChanged(const ConnectionItem* newItem);
};

#endif // STATUSNOTIFIER_H