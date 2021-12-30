#ifndef SERVERSWINDOW_H
#define SERVERSWINDOW_H

#include <QMainWindow>
#include <QLocalServer>
#include <QCloseEvent>
#include <QProcess>
#include "statusnotifier.h"

#include <QSortFilterProxyModel>
#include "connectiontablemodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	bool isInstanceRunning();
	QMenu* subscriptionMenu();

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
	void updateTabelServersMenu(const QModelIndex &modelIndex);

	void hideEvent(QHideEvent *event) override;
	void showEvent(QShowEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

private slots:
	void on_actionManually_triggered();
	void on_actionConnect_triggered();
	void on_actionClear_triggered();
	void on_actionManager_triggered();
	void on_actionUpdate_triggered();
	void on_actionQuit_triggered();

	void on_menuConnection_aboutToShow();

	void on_tableServers_customContextMenuRequested(const QPoint &pos);
	void on_tableServers_doubleClicked(const QModelIndex &index);

	void on_editFilter_editingFinished();

	void onSubsParsed(QList<SQProfile> profiles, AirportInfo airInfo);
	void onDeleteAirport(QString airUrl);
};
#endif // SERVERSWINDOW_H
