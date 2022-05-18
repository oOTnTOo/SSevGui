#include "Settings.h"
#include <QDebug>

Settings::Settings(QObject *parent) : QObject(parent) {
	settings_.setPath(QSettings::IniFormat,QSettings::UserScope,"SSevGUI-Qt5");
	qDebug() << settings_.fileName();
	qRegisterMetaTypeStreamOperators<SQProfile>("SQProfile");
}

void Settings::readProfile(ConnectionTableModel* model) {
	int size = settings_.beginReadArray("Profile");
	for (int i = 0; i < size; ++i) {
		settings_.setArrayIndex(i);
		QVariant value = settings_.value("SQProfile");
		SQProfile profile = value.value<SQProfile>();
		Connection *con = new Connection(profile, this);
		model->appendConnection(con);
	}
	settings_.endArray();
}

void Settings::saveAllProfile(const ConnectionTableModel& model) {
	if(model.rowCount() < 1)
		settings_.remove("Profile");
	settings_.beginWriteArray("Profile");
	for (int i = 0; i < model.rowCount(); ++i) {
		settings_.setArrayIndex(i);
		Connection *con = model.getItem(i)->connection();
		QVariant value = QVariant::fromValue<SQProfile>(con->profile());
		settings_.setValue("SQProfile", value);
	}
	settings_.endArray();
}

//void Settings::saveProfile(int index, SQProfile profile) {
//	settings_.beginWriteArray("Profile");
//	QVariant value = QVariant::fromValue<SQProfile>(profile);
//	settings_.setValue("SQProfile", value);
//	settings_.endArray();
//}

void Settings::readAirInfosIntoList(QListWidget* listWgt) {
	int count = settings_.beginReadArray("Airport");
	for (int i = 0; i < count; ++i) {
		settings_.setArrayIndex(i);
		AirportInfo ai;
		ai.url_ = settings_.value("url").toString();
		ai.name_ = settings_.value("name").toString();
		ai.expiry_ = settings_.value("expiry").toString();
		ai.trafficTotal_ = settings_.value("trafficTotal").toDouble();
		ai.trafficUsed_ = settings_.value("trafficUsed").toDouble();

		QListWidgetItem* item = new QListWidgetItem(ai.name_,listWgt);
		item->setData(Qt::UserRole+1,QVariant::fromValue<AirportInfo>(ai));
	}
	settings_.endArray();
}

void Settings::saveAirInfos(const QListWidget* listWgt) {
//	if(listWgt->count() < 1)
	settings_.remove("Airport");
	settings_.beginWriteArray("Airport");
	for(int i = 0; i < listWgt->count(); ++i){
		settings_.setArrayIndex(i);
		AirportInfo airInfos = listWgt->item(i)->data(Qt::UserRole+1).value<AirportInfo>();
		settings_.setValue("url",airInfos.url_);
		settings_.setValue("name",airInfos.name_);
		settings_.setValue("trafficTotal",airInfos.trafficTotal_);
		settings_.setValue("trafficUsed",airInfos.trafficUsed_);
		settings_.setValue("expiry",airInfos.expiry_);
	}
	settings_.endArray();
}

QList<AirportInfo> Settings::readAirInfos() {
	int count = settings_.beginReadArray("Airport");
	QList<AirportInfo> ais;
	for (int i = 0; i < count; ++i) {
		settings_.setArrayIndex(i);
		AirportInfo ai;
		ai.url_ = settings_.value("url").toString();
		ai.name_ = settings_.value("name").toString();
		ai.expiry_ = settings_.value("expiry").toString();
		ai.trafficTotal_ = settings_.value("trafficTotal").toDouble();
		ai.trafficUsed_ = settings_.value("trafficUsed").toDouble();
		ais << ai;
	}
	settings_.endArray();

	return ais;
}
