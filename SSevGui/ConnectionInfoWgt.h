#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <QDialog>
#include "connection.h"

namespace Ui {
class ConnectionInfoWgt;
}

class ConnectionInfoWgt : public QDialog
{
	Q_OBJECT

public:
	explicit ConnectionInfoWgt(QWidget *parent = nullptr);
	~ConnectionInfoWgt();
	void setProfile(SQProfile profile);
	SQProfile profile();


private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();

private:
	Ui::ConnectionInfoWgt *ui;

	Connection* con_ = nullptr;
};

#endif // CONNECTIONINFO_H
