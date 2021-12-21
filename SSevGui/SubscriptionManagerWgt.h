#ifndef SUBSCRIPTIONMANAGERWGT_H
#define SUBSCRIPTIONMANAGERWGT_H

#include <QDialog>

#include "sqprofile.h"
#include "DataDefine.h"

namespace Ui {
class SubscriptionManagerWgt;
}

class QListWidgetItem;
class SubscriptionManagerWgt : public QDialog
{
	Q_OBJECT

public:
	explicit SubscriptionManagerWgt(QWidget *parent = nullptr);
	~SubscriptionManagerWgt();

private:
	Ui::SubscriptionManagerWgt *ui;

private slots:
	void on_btnAdd_clicked();
	void on_btnDel_clicked();
	void onAirResponse(QList<SQProfile> profile, AirportInfo airportInfo);
	void on_listAir_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

signals:
	void sig_DeleteAirportConnection(QString Url);
};

#endif // SUBSCRIPTIONMANAGERWGT_H
