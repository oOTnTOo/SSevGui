#ifndef BUSVIEW_H
#define BUSVIEW_H

#include <QObject>
#include <QProcess>

#include "Settings.h"
#include "DataDefine.h"

class SQProfile;
class QNetworkAccessManager;
class QNetworkReply;

class BusView : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(BusView)

public:
	static BusView* inst();

	void startProxy(SQProfile profile);
	void getSubsContent(QString url);
	Settings& setting();
	AirportInfo airportInfo(QString airUrl) const;



private:
	explicit BusView(QObject *parent = nullptr);
	~BusView();

	QProcess proc_;
	Settings settings_;

	QNetworkAccessManager* netMng_;

signals:
	void sig_respParsed(QList<SQProfile> pros, AirportInfo airInfo);

private slots:
	void onSubsResponse(QNetworkReply* reply);
};

#endif // BUSVIEW_H
