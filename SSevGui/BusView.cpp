#include "BusView.h"
#include "sqprofile.h"
#include "Settings.h"
#include "sqprofile.h"

#include <QFile>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

BusView* BusView::inst() {
	static BusView bv;
	return &bv;
}

void BusView::init(ConnectionTableModel* model) {
	model_ = model;

	settings_.readProfile(model_);
	emit sig_modelItemChanged();
}

bool BusView::startProxy(ConnectionItem* connItem)
{
	const SQProfile& prof = connItem->connection()->profile();

	QStringList sl = {
		"-s",prof.serverAddress,
		"-p",QString::number(prof.serverPort),
		"-l",QString::number(prof.localPort),
		"-k",prof.password,
		"-m",prof.method
	};

	if(!prof.plugin.isEmpty()) {
		sl << "--plugin" << prof.plugin
		   << "--plugin-opts" << QString("\"%1\"").arg(prof.pluginOpt);
	}

	if(proc_.state() != QProcess::NotRunning){
		proc_.kill();
		qDebug() << proc_.waitForFinished(3000);
		curItem_->connection()->stop();
		curItem_ = nullptr;
		emit sig_currentItemChanged(nullptr);
	}
	if(!QFile::exists(qApp->applicationDirPath()+"/ss-local")) {
		emit sig_notifyText("ss-local not found, copy ss-local in program dir");
		return false;
	}
	proc_.start(qApp->applicationDirPath()+"/ss-local "+sl.join(" "));
	qDebug() << proc_.waitForStarted(3000) << proc_.arguments();

	curItem_ = connItem;
	connItem->connection()->start();
	emit sig_currentItemChanged(curItem_);
	return true;
}

bool BusView::stopProxy() {
	proc_.terminate();
	if(proc_.waitForFinished(10000)) {
		proc_.kill();
	}
	if(curItem_) {
		curItem_->connection()->stop();
		curItem_ = nullptr;
		emit sig_currentItemChanged(nullptr);
	}

	return true;
}

void BusView::updateAirport(QString url) {
	QNetworkRequest req(url);
	netMng_->get(req);
}

void BusView::updateAllAirport() {
	stopProxy();
	for(AirportInfo& ai : settings_.readAirInfos()) {
		updateAirport(ai.url_);
	}
}

const QMap<QString, QList<ConnectionItem*> >& BusView::connectionItems() const{
	return model_->connectionItems();
}

Settings& BusView::setting() {
	return settings_;
}

BusView::BusView(QObject *parent) : QObject(parent) {
	netMng_ = new QNetworkAccessManager(this);

	connect(&proc_,&QProcess::readyReadStandardOutput, [this](){qDebug() << QString::fromUtf8(proc_.readAllStandardOutput());});
	connect(netMng_,&QNetworkAccessManager::finished,this,&BusView::onSubsResponse);
}

BusView::~BusView() {
	proc_.terminate();
	if(proc_.waitForFinished(20000)) {
		proc_.kill();
	}
}

bool BusView::getSSDParseRes(QByteArray bytes, QList<SQProfile>& prfs, AirportInfo& ai) {
	bytes.replace('-', '+').replace('_', '/');
	int mod4 = bytes.count() % 4;
	if(mod4 > 0){
		bytes.append(4-mod4,'=');
	}

	QJsonParseError err;
	QJsonObject rootObj = QJsonDocument::fromJson(QByteArray::fromBase64(bytes),&err).object();
	if(err.error != QJsonParseError::NoError) {
		emit sig_notifyText(tr("%1 Error:\n %2")
							.arg(ai.url_)
							.arg(err.errorString()));
		return false;
	}
	QString airUrl = rootObj.value("url").toString();
	ai.url_ = airUrl;
	ai.name_ = rootObj.value("airport").toString();
	ai.expiry_ = rootObj.value("expiry").toString();
	ai.trafficTotal_ = rootObj.value("traffic_total").toDouble();
	ai.trafficUsed_ = rootObj.value("traffic_used").toDouble();

	for(QJsonValueRef v : rootObj.value("servers").toArray()) {
		QJsonObject o = v.toObject();
		SQProfile pro;
		pro.serverAddress = o.value("server").toString();
		pro.serverPort = o.value("port").toInt();
		pro.method = o.value("encryption").toString();
		pro.name = o.value("remarks").toString();
		pro.password = o.value("password").toString();
		pro.plugin = o.value("plugin").toString().replace("simple-obfs","obfs-local");
		pro.pluginOpt = o.value("plugin_options").toString();
		pro.timeout = 5000;

		pro.localAddress = "0.0.0.0";
		pro.localPort = 1088;

		pro.airportInfo_ = ai;

		prfs << pro;
	}

	return true;
}

void BusView::onSubsResponse(QNetworkReply* reply) {
	AirportInfo ai;
	ai.url_ = reply->request().url().toString();

	if(reply->error()!=QNetworkReply::NoError) {
		emit sig_notifyText(tr("%1 error: %2")
							.arg(ai.url_)
							.arg(reply->errorString()));
		return;
	}

	QString resp = QString::fromUtf8(reply->readAll());
	QStringList bl = resp.split("://");
	if(bl.count() < 2)
		emit sig_notifyText(tr("%1 error: return \n%2").arg(ai.url_).arg(resp));

	QList<SQProfile> prfs;

	if(bl.first() == "ssd") {
		getSSDParseRes(bl.last().toUtf8(), prfs, ai);
	} else {
		emit sig_notifyText(tr("Unsupport protocal"));
		return;
	}

	emit sig_respParsed(prfs, ai);
}
