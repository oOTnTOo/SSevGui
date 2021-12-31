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

	bool startProxy(ConnectionItem* connItem);
	bool stopProxy();
	void updateAirport(QString url);
	void updateAllAirport();

	void testServer(SQProfile* profile);

	/**
	 * @brief 获取服务器信息列表
	 * @return 返回服务器列表，并归档，注意，不可修改，所有服务器信息的改动需通过model
	 */
	const QMap<QString, QList<ConnectionItem*> >& connectionItems() const;

	Settings& setting();
	AirportInfo airportInfo(QString airUrl) const;

private:
	explicit BusView(QObject *parent = nullptr);
	~BusView();

	bool getSSDParseRes(QByteArray bytes, QList<SQProfile>& profiles, AirportInfo& airInfo);

	QProcess proc_;
	Settings settings_;

	ConnectionTableModel *model_ = nullptr;
	ConnectionItem* curItem_ = nullptr;

	QNetworkAccessManager* netMng_;

signals:
	void sig_notifyText(QString errorString);
	void sig_respParsed(QList<SQProfile> pros, AirportInfo airInfo);
	void sig_modelItemChanged();
	void sig_currentItemChanged(ConnectionItem* newItem);

private slots:
	void onSubsResponse(QNetworkReply* reply);
};

#endif // BUSVIEW_H
