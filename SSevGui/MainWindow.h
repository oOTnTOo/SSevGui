#ifndef SERVERSWINDOW_H
#define SERVERSWINDOW_H

#include <QMainWindow>
#include <QLocalServer>
#include <QCloseEvent>
#include <QSortFilterProxyModel>
#include <QProcess>
#include "connectiontablemodel.h"
#include "statusnotifier.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	bool isInstanceRunning();

private slots:
	void onSingleInstanceConnect();

private:
	Ui::MainWindow *ui;

	ConnectionTableModel *model_;
	QSortFilterProxyModel *proxyModel_;
	StatusNotifier* sntfier_ = nullptr;

	QLocalServer* instanceServer;
	bool instanceRunning;

	void initSingleInstance();

	void hideEvent(QHideEvent *event) override;
	void showEvent(QShowEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

private slots:
	void on_actionManually_triggered();
	void on_actionConnect_triggered();
	void on_actionClear_triggered();
	void on_actionManager_triggered();

	void on_tableServers_doubleClicked(const QModelIndex &index);
};
#endif // SERVERSWINDOW_H
