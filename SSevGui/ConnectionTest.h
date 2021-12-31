#ifndef CONNECTIONTEST_H
#define CONNECTIONTEST_H

#include <QObject>
#include <QHostInfo>
#include <QTimer>
#include <QTcpSocket>

#include "connectionitem.h"

/**
 * @brief 延迟测试
 */
class ConnectionTest : public QObject {
	Q_OBJECT
public:
	explicit ConnectionTest(QString address, int port, QObject *parent = nullptr);

	void connectTest(int timeout);

private:
	QString addr_;
	int port_;
	QTime lagTime_;
	QTimer timeoutTimer_;
	QTcpSocket socket_;

private slots:
	void onTimeout();
	void onSocketConnected();
	void onSocketError(QTcpSocket::SocketError error);

signals:
	void sig_lagTestFinished(int lag, QString errContent);
};

#endif // CONNECTIONTEST_H
