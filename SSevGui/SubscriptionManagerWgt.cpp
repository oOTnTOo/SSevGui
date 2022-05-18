#include "SubscriptionManagerWgt.h"
#include "ui_SubscriptionManagerWgt.h"

#include <QMessageBox>
#include "BusView.h"

SubscriptionManagerWgt::SubscriptionManagerWgt(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SubscriptionManagerWgt)
{
	ui->setupUi(this);

	ui->splitter->setStretchFactor(0,2);
	ui->splitter->setStretchFactor(1,3);

	BusView::inst()->setting().readAirInfosIntoList(ui->listAir);

	connect(BusView::inst(),&BusView::sig_respParsed,this,&SubscriptionManagerWgt::onAirResponse);
}

SubscriptionManagerWgt::~SubscriptionManagerWgt()
{
	delete ui;
}

void SubscriptionManagerWgt::on_btnAdd_clicked() {
	//ui->btnAdd->setEnabled(false);
	BusView::inst()->updateAirport(ui->editAirLink->text());
}

void SubscriptionManagerWgt::on_btnDel_clicked() {
	QListWidgetItem* it = ui->listAir->takeItem(ui->listAir->currentRow());
	if(QMessageBox::Yes ==
	   QMessageBox::question(this,"Tips","Delete all connection in this airport?",QMessageBox::Yes|QMessageBox::No)){
		emit sig_DeleteAirportConnection(it->data(Qt::UserRole+1).value<AirportInfo>().url_);
	}
	delete it;

	BusView::inst()->setting().saveAirInfos(ui->listAir);
}

void SubscriptionManagerWgt::on_btnSave_clicked() {
	AirportInfo ai = ui->listAir->currentItem()->data(Qt::UserRole+1).value<AirportInfo>();
	AirportInfo before = ai;
	ai.url_ = ui->editAirLink->text();
	ui->listAir->currentItem()->setData(Qt::UserRole+1,QVariant::fromValue<AirportInfo>(ai));

	BusView::inst()->modifyAirUrlInMap(ai.url_, before.url_); // 更新Model里Map中的订阅Url缓存
	BusView::inst()->setting().saveAirInfos(ui->listAir);
}

void SubscriptionManagerWgt::onAirResponse(QList<SQProfile> profile, AirportInfo aInfo) {
	Q_UNUSED(profile)

	bool isDual = false;
	for (int i = 0; i < ui->listAir->count(); ++i) {
		AirportInfo ai = ui->listAir->item(i)->data(Qt::UserRole+1).value<AirportInfo>();
		if(ai.url_ != aInfo.url_)
			continue;

		ui->listAir->setCurrentRow(i);
		isDual = true;
	}
	if(!isDual) {
		QListWidgetItem* item = new QListWidgetItem(aInfo.name_, ui->listAir);
		item->setData(Qt::UserRole+1, QVariant::fromValue<AirportInfo>(aInfo));
	}
	ui->editAirName->setText(aInfo.name_);
	ui->editAirLink->setText(aInfo.url_);
	ui->labelTraffic->setText(QString("%1 / %2 GB").arg(aInfo.trafficUsed_).arg(aInfo.trafficTotal_));
	ui->labelExpiry->setText(aInfo.expiry_);

	BusView::inst()->setting().saveAirInfos(ui->listAir);
}

void SubscriptionManagerWgt::on_listAir_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous) {
	if(!current){
		ui->editAirLink->clear();
		ui->editAirName->clear();
		ui->labelTraffic->clear();
		ui->labelExpiry->clear();
		ui->btnDel->setEnabled(false);
		return;
	}

	const AirportInfo& ai = current->data(Qt::UserRole+1).value<AirportInfo>();

	ui->editAirLink->setText(ai.url_);
	ui->editAirName->setText(ai.name_);
	ui->labelTraffic->setText(QString("%1 / %2 GB").arg(ai.trafficUsed_).arg(ai.trafficTotal_));
	ui->labelExpiry->setText(ai.expiry_);

	ui->btnDel->setEnabled(true);
}
