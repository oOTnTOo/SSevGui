#include "ShareDialog.h"
#include "ui_ShareDialog.h"
#include <QClipboard>

#include "BusView.h"
#include "Tools.h"

ShareDialog::ShareDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ShareDialog)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
}

ShareDialog::~ShareDialog()
{
	delete ui;
}

void ShareDialog::showServerConfig(SQProfile& curProfile) {
	QListWidgetItem* cur=nullptr;
	for(ConnectionItem* ci : BusView::inst()->connectionItems()) {
		SQProfile p = ci->connection()->profile();
		QListWidgetItem* it = new QListWidgetItem(p.remake+" "+p.serverAddress, ui->listWidget);
		it->setData(Qt::UserRole+1, Tools::makeServerShareUrl(p));
		if(curProfile.isVaild() && (p==curProfile))
			cur = it;
	}
	if(cur)
		ui->listWidget->setCurrentItem(cur);
	else if(ui->listWidget->count() > 0)
		ui->listWidget->setCurrentRow(0);
}

void ShareDialog::on_listWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous) {
	ui->editUrl->setText(current->data(Qt::UserRole+1).toString());
	ui->editUrl->setCursorPosition(0);
}

void ShareDialog::on_btnCopy_clicked() {
	qApp->clipboard()->setText(ui->editUrl->text());
	ui->editUrl->selectAll();
}
