#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QListWidget>
#include <QSettings>
#include "connectiontablemodel.h"
#include "DataDefine.h"

class Settings : public QObject
{
	Q_OBJECT
public:
	explicit Settings(QObject *parent = nullptr);

	void readProfile(ConnectionTableModel* model);
	void saveAllProfile(const ConnectionTableModel& model);
	void saveProfile(int index, SQProfile profile);

	void readAirInfosIntoList(QListWidget* listWgt);
	void saveAirInfos(const QListWidget* listWgt);
	QList<AirportInfo> readAirInfos();

private:
	QSettings settings_;

signals:

public slots:
};

#endif // SETTINGS_H
