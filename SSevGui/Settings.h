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
	void saveProfile(const ConnectionTableModel& model);

	void readAirInfos(QListWidget* listWgt);
	void saveAirInfos(const QListWidget* listWgt);

private:
	QSettings settings_;

signals:

public slots:
};

#endif // SETTINGS_H
