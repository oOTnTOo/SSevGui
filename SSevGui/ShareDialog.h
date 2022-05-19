#ifndef SHAREDIALOG_H
#define SHAREDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "DataDefine.h"

namespace Ui {
class ShareDialog;
}

class ShareDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ShareDialog(QWidget *parent = nullptr);
	~ShareDialog();

	void showServerPage(SQProfile& curProfile);
	void showAirPage();

private:
	Ui::ShareDialog *ui;

private slots:
	void onServersCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	void onAirsCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	void on_btnCopy_clicked();
};

#endif // SHAREDIALOG_H
