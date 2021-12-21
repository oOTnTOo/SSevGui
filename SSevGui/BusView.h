#ifndef BUSVIEW_H
#define BUSVIEW_H

#include <QObject>
#include <QProcess>
#include <QSortFilterProxyModel>

#include "connectiontablemodel.h"
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

	void init(ConnectionTableModel* model);

	void startProxy(SQProfile profile);
	void updateAirport(QString url);
	void updateAllAirport();

	Settings& setting();
	AirportInfo airportInfo(QString airUrl) const;


private:
	explicit BusView(QObject *parent = nullptr);
	~BusView();

	bool getSSDParseRes(QByteArray bytes, QList<SQProfile>& profiles, AirportInfo& airInfo);

	QProcess proc_;
	Settings settings_;

	ConnectionTableModel *model_ = nullptr;
	//QSortFilterProxyModel *proxyModel_ = nullptr;

	QNetworkAccessManager* netMng_;

signals:
	void sig_notifyText(QString errorString);
	void sig_respParsed(QList<SQProfile> pros, AirportInfo airInfo);

private slots:
	void onSubsResponse(QNetworkReply* reply);
};

#endif // BUSVIEW_H
