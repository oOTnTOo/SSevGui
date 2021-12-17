#ifndef DATADEFINE_H
#define DATADEFINE_H

#include <QString>

struct AirportInfo {
	QString url_;		// 订阅地址
	QString name_;		// 名称
	QString expiry_;	// 过期时间
	float trafficTotal_;	// 总流量
	float trafficUsed_;	// 已用流量
};
Q_DECLARE_METATYPE(AirportInfo);

#endif // DATADEFINE_H
