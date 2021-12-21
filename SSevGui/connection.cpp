#include "connection.h"
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
	stop();
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
	return profile_.name;
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


void Connection::onNewBytesTransmitted(const quint64 &b)
{
	profile_.currentUsage += b;
	profile_.totalUsage += b;
	emit dataUsageChanged(profile_.currentUsage, profile_.totalUsage);
}

void Connection::onLatencyAvailable(const int latency)
{
	profile_.latency = latency;
    emit latencyAvailable(latency);
}

void Connection::start() {
	running = true;
}

void Connection::stop() {
	running = false;
}
