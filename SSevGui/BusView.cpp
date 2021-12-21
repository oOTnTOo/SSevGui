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
}

void BusView::startProxy(SQProfile prof) {
	//SQProfile prof = model_->getItem(ui->tableServers->currentIndex().row())->connection()->profile();

	//QString str = QString("-s %1 -p %2 -l %3 -k %4 -m %5 --plugin obfs-local --plugin-opts \"\"")
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
//		if(!prof.pluginArg.isEmpty())
//			sl << QString("")
	}

	if(proc_.state() != QProcess::NotRunning){
		proc_.kill();
		qDebug() << proc_.waitForFinished(3000);
	}
	if(!QFile::exists(qApp->applicationDirPath()+"/ss-local")) {
		qDebug() << "ss-local not found";
		return;
	}
	proc_.start(qApp->applicationDirPath()+"/ss-local "+sl.join(" "));
	qDebug() << proc_.waitForStarted(3000) << proc_.arguments();
}

void BusView::updateAirport(QString url) {
	QNetworkRequest req(url);
	netMng_->get(req);
}

void BusView::updateAllAirport() {
	for(QString& au : model_->airportUrls()) {
		updateAirport(au);
	}
}

Settings& BusView::setting() {
	return settings_;
}

BusView::BusView(QObject *parent) : QObject(parent) {
	netMng_ = new QNetworkAccessManager(this);

	//connect(&proc_,&QProcess::readyReadStandardOutput, [this](){qDebug() << QString::fromUtf8(proc_.readAllStandardOutput());});
	connect(netMng_,&QNetworkAccessManager::finished,this,&BusView::onSubsResponse);
}

BusView::~BusView() {

}

bool BusView::getSSDParseRes(QByteArray bytes, QList<SQProfile>& prfs, AirportInfo& ai) {
	bytes.replace('-', '+').replace('_', '/');
	int mod4 = bytes.count() % 4;
	if(mod4 > 0){
		bytes.append(4-mod4,'=');
	}

	QJsonObject rootObj = QJsonDocument::fromJson(QByteArray::fromBase64(bytes)).object();
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
		pro.plugin = o.value("plugin").toString();
		pro.pluginOpt = o.value("plugin_options").toString();

		pro.localAddress = "0.0.0.0";
		pro.localPort = 1088;

		pro.airportInfo_ = ai;

		prfs << pro;
	}

	return true;
}

void BusView::onSubsResponse(QNetworkReply* reply) {
	if(reply->error()!=QNetworkReply::NoError) {
		emit sig_notifyText(reply->errorString());
		return;
	}
	QString resp = QString::fromUtf8(reply->readAll());
	QStringList bl = resp.split("://");

	QList<SQProfile> prfs;
	AirportInfo ai;

	if(bl.first() == "ssd") {
		getSSDParseRes(bl.last().toUtf8(), prfs, ai);
	}

	emit sig_respParsed(prfs, ai);
}
