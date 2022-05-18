#include "connection.h"
#include "ConnectionTest.h"
#include <QHostInfo>
#include <QHostAddress>

Connection::Connection(QObject *parent) :
    QObject(parent),
    running(false)
{}

Connection::Connection(const SQProfile &_profile, QObject *parent) :
	Connection(parent)
{
	profile_ = _profile;
}

Connection::Connection(QString uri, QObject *parent) :
	Connection(parent)
{
	profile_ = SQProfile(uri);
}

Connection::~Connection()
{
}

void Connection::setProfile(SQProfile profile) {
	profile_ = profile;
}

const SQProfile& Connection::profile() const
{
	return profile_;
}

const QString& Connection::name() const
{
	return profile_.remake;
}

bool Connection::isValid() const
{
	if (profile_.serverAddress.isEmpty() || profile_.localAddress.isEmpty() || profile_.timeout < 1) {
        return false;
    }
    else {
        return true;
    }
}

const bool &Connection::isRunning() const
{
	return running;
}

void Connection::latencyTest() {
	ConnectionTest* ct = new ConnectionTest(profile_.serverAddress,profile_.serverPort,this);
	connect(ct,&ConnectionTest::sig_lagTestFinished,this,&Connection::onTestedLatency);
	ct->connectTest(profile_.timeout);
}


void Connection::onNewBytesTransmitted(const quint64 &b)
{
	profile_.currentUsage += b;
	profile_.totalUsage += b;
	emit dataUsageChanged(profile_.currentUsage, profile_.totalUsage);
}

void Connection::onTestedLatency(int lag, QString error) {
	profile_.latency = lag;
	emit latencyTested(lag, error);
	sender()->deleteLater();
}

void Connection::start() {
	running = true;

#ifdef Q_OS_LINUX
	QString cmd = QString("gsettings set org.gnome.system.proxy.socks host '%1'").arg(profile_.localAddress);
	system(cmd.toUtf8().constData());
	cmd = QString("gsettings set org.gnome.system.proxy.socks port '%1'").arg(profile_.localPort);
	system(cmd.toUtf8().constData());
	system("gsettings set org.gnome.system.proxy mode 'manual'");
#endif
}

void Connection::stop() {
	running = false;

#ifdef Q_OS_LINUX
	system("gsettings set org.gnome.system.proxy mode 'none'");
#endif
}
