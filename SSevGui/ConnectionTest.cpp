#include "ConnectionTest.h"

ConnectionTest::ConnectionTest(QString addr, int port, QObject *parent) :addr_(addr), port_(port), QObject(parent)
{
	connect(&timeoutTimer_,&QTimer::timeout,this,&ConnectionTest::onTimeout);
	connect(&socket_,&QTcpSocket::connected,this,&ConnectionTest::onSocketConnected);
	connect(&socket_,
			QOverload<QTcpSocket::SocketError>::of
			(&QTcpSocket::error),
			this,
			&ConnectionTest::onSocketError);
}

void ConnectionTest::connectTest(int timeout) {
	socket_.connectToHost(addr_,port_);
	timeoutTimer_.start(timeout);
	lagTime_.start();
}

void ConnectionTest::onTimeout() {
	socket_.abort();
	emit sig_lagTestFinished(SQProfile::LATENCY_TIMEOUT, "Connect time out");
}

void ConnectionTest::onSocketConnected() {
	timeoutTimer_.stop();
	emit sig_lagTestFinished(lagTime_.elapsed(),"");
}

void ConnectionTest::onSocketError(QTcpSocket::SocketError error) {
	timeoutTimer_.stop();
	emit sig_lagTestFinished(SQProfile::LATENCY_TIMEOUT, socket_.errorString());
}
