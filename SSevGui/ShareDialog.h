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

	void showServerConfig(SQProfile& curProfile);

private:
	Ui::ShareDialog *ui;

private slots:
	void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	void on_btnCopy_clicked();
};

#endif // SHAREDIALOG_H
