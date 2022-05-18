#ifndef DATADEFINE_H
#define DATADEFINE_H

#include <QString>
#include <QDataStream>
#include <QDateTime>

struct AirportInfo {
	QString url_;		// 订阅地址
	QString name_;		// 名称
	QString expiry_;	// 过期时间
	float trafficTotal_;	// 总流量
	float trafficUsed_;	// 已用流量
};
Q_DECLARE_METATYPE(AirportInfo)

QDataStream& operator << (QDataStream &out, const AirportInfo &p);
QDataStream& operator >> (QDataStream &in, AirportInfo &p);

struct SQProfile
{
	SQProfile();
	SQProfile(const QString& uri); // Construct it using ss protocol

	bool autoStart;
	bool debug;
	quint16 serverPort;
	quint16 localPort;
	QString remake;
	QString serverAddress;
	QString localAddress;
	QString method;
	QString password;
	int timeout;
	int latency = 0;
	quint64 currentUsage;
	quint64 totalUsage;
	QDateTime lastTime;//last time this connection is used
	QDate nextResetDate;//next scheduled date to reset data usage
	bool httpMode;

	static const int LATENCY_TIMEOUT = -1;
	static const int LATENCY_ERROR = -2;
	static const int LATENCY_UNKNOWN = -3;
	static const int DEFAULT_PORT = 1088;
	static const int DEFAULT_TIMEOUT = 5000;

	QString plugin;
	QString pluginArg;
	QString pluginOpt;

	AirportInfo airportInfo_;

	bool operator ==(const SQProfile& other){
		return serverAddress==other.serverAddress
				&& serverPort==other.serverPort
				&& method == other.method
				&& password == other.password
				&& remake == other.remake;
	}

	bool isVaild() {
		return !serverAddress.isEmpty();
	}
};
Q_DECLARE_METATYPE(SQProfile)

QDataStream& operator << (QDataStream &out, const SQProfile &p);
QDataStream& operator >> (QDataStream &in, SQProfile &p);

#endif // DATADEFINE_H
