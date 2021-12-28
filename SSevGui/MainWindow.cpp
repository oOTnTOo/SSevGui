#include "MainWindow.h"
#include "ConnectionInfoWgt.h"
#include "SubscriptionManagerWgt.h"
#include "BusView.h"
#include <QLocalSocket>
#include <QDebug>
#include <QMessageBox>

#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	instanceRunning(false),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);

	sntfier_ = new StatusNotifier(this,false,this);

	model_ = new ConnectionTableModel(ui->tableServers);
	proxyModel_ = new QSortFilterProxyModel(ui->tableServers);
	proxyModel_->setSourceModel(model_);
	proxyModel_->setSortRole(Qt::EditRole);
	proxyModel_->setFilterCaseSensitivity(Qt::CaseInsensitive);
	proxyModel_->setFilterKeyColumn(-1);
	ui->tableServers->setModel(proxyModel_);

	BusView::inst()->init(model_);

	connect(BusView::inst(),&BusView::sig_respParsed,this,&MainWindow::onSubsParsed);
	connect(BusView::inst(),&BusView::sig_notifyText,sntfier_,&StatusNotifier::showNotification);
	connect(BusView::inst(),&BusView::sig_currentItemChanged,ui->tableServers,&QTableView::reset);

	//ui->statusbar->showMessage(tr("Caution: Linux need modify socks5 proxy setting manully."));
}

MainWindow::~MainWindow() {
	delete ui;
}


void MainWindow::initSingleInstance()
{
	const QString serverName = QCoreApplication::applicationName();
	QLocalSocket socket;
	socket.connectToServer(serverName);
	if (socket.waitForConnected(500)) {
		instanceRunning = true;
//		if (configHelper->isOnlyOneInstance()) {
//			qWarning() << "An instance of ss-qt5 is already running";
//		}
		QByteArray username = qgetenv("USER");
		if (username.isEmpty()) {
			username = qgetenv("USERNAME");
		}
		socket.write(username);
		socket.waitForBytesWritten();
		return;
	}

	/* Can't connect to server, indicating it's the first instance of the user */
	instanceServer = new QLocalServer(this);
	instanceServer->setSocketOptions(QLocalServer::WorldAccessOption);
	connect(instanceServer, &QLocalServer::newConnection,
			this, &MainWindow::onSingleInstanceConnect);
	if (instanceServer->listen(serverName)) {
		/* Remove server in case of crashes */
		if (instanceServer->serverError() == QAbstractSocket::AddressInUseError &&
				QFile::exists(instanceServer->serverName())) {
			QFile::remove(instanceServer->serverName());
			instanceServer->listen(serverName);
		}
	}
}

void MainWindow::updateTabelServersMenu(const QModelIndex &index) {
	ui->actionConnect->setText(model_->getItem(index.row())->connection()->isRunning()
		? tr("Disconnect") : tr("Connect"));
}

void MainWindow::hideEvent(QHideEvent* event) {
	QMainWindow::hideEvent(event);
	sntfier_->onWindowVisibleChanged(false);
}

void MainWindow::showEvent(QShowEvent* event) {
	QMainWindow::showEvent(event);
	sntfier_->onWindowVisibleChanged(true);
	setFocus();
}

void MainWindow::closeEvent(QCloseEvent *event){
	QMainWindow::closeEvent(event);
	BusView::inst()->stopProxy();
	return;

	if(event->spontaneous()) {
		event->ignore();
		hide();
	} else {
		QMainWindow::closeEvent(event);
	}
}

void MainWindow::on_actionManually_triggered() {
	ConnectionInfoWgt* ciw = new ConnectionInfoWgt(this);
	if(QDialog::Accepted == ciw->exec()) {
		Connection* conn = new Connection(ciw->profile(), model_);
		model_->appendConnection(conn);
		BusView::inst()->setting().saveAllProfile(*model_);
		emit BusView::inst()->sig_modelItemChanged();
	}
	delete ciw;
}

void MainWindow::on_actionConnect_triggered() {
	QModelIndex idx = ui->tableServers->currentIndex();
	if(!idx.isValid()) {
		return;
	}
	ui->actionConnect->setEnabled(false);

	ConnectionItem* conn = model_->getItem(proxyModel_->mapToSource(idx).row());
	if(conn->connection()->isRunning())
		BusView::inst()->stopProxy();
	else
		BusView::inst()->startProxy(conn);

	ui->actionConnect->setEnabled(true);
}

void MainWindow::on_actionClear_triggered() {
	if(QMessageBox::Yes == QMessageBox::question(this,"Are you sure?","Clear list? sure?")) {
		model_->clearItems();
		BusView::inst()->setting().saveAllProfile(*model_);
	}
}

void MainWindow::on_actionManager_triggered() {
	SubscriptionManagerWgt* smw = new SubscriptionManagerWgt(this);
	connect(smw,&SubscriptionManagerWgt::sig_DeleteAirportConnection,this,&MainWindow::onDeleteAirport);
	smw->setAttribute(Qt::WA_DeleteOnClose);
	smw->show();
}

void MainWindow::on_actionUpdate_triggered() {
	BusView::inst()->updateAllAirport();
}

void MainWindow::on_menuConnection_aboutToShow() {
	QModelIndex idx = ui->tableServers->currentIndex();
	if(!idx.isValid())
		return;
	idx = proxyModel_->mapToSource(idx);
	updateTabelServersMenu(idx);
}

void MainWindow::on_tableServers_customContextMenuRequested(const QPoint& pos) {
	QModelIndex idx = proxyModel_->mapToSource(ui->tableServers->indexAt(pos));
	if(!idx.isValid())
		return;

	ui->menuConnection->popup(ui->tableServers->viewport()->mapToGlobal(pos));
}

void MainWindow::onSingleInstanceConnect() {
	QLocalSocket *socket = instanceServer->nextPendingConnection();
	if (!socket) {
		return;
	}

	if (socket->waitForReadyRead(1000)) {
		QByteArray username = qgetenv("USER");
		if (username.isEmpty()) {
			username = qgetenv("USERNAME");
		}

		QByteArray recvUsername = socket->readAll();
		if (recvUsername == username) {
			// Only show the window if it's the same user
			show();
		} else {
			qWarning("Another user is trying to run another instance of ss-qt5");
		}
	}
	socket->deleteLater();
}


bool MainWindow::isInstanceRunning() {
	return instanceRunning;
}

QMenu* MainWindow::subscriptionMenu() {
	return ui->menuSubscription;
}

void MainWindow::on_tableServers_doubleClicked(const QModelIndex &index) {
	Connection* conn = model_->getItem(proxyModel_->mapToSource(index).row())->connection();
	ConnectionInfoWgt* ciw = new ConnectionInfoWgt(this);
	ciw->setProfile(conn->profile());
	if(QDialog::Accepted == ciw->exec()) {
		conn->setProfile(ciw->profile());
		BusView::inst()->setting().saveAllProfile(*model_);
	}
	delete ciw;
}

void MainWindow::on_editFilter_editingFinished() {
	proxyModel_->setFilterWildcard(ui->editFilter->text());
}

void MainWindow::onSubsParsed(QList<SQProfile> profiles, AirportInfo airInfo) {
	model_->addAirConnection(profiles,airInfo);
	emit BusView::inst()->sig_modelItemChanged();
	BusView::inst()->setting().saveAllProfile(*model_);
	sntfier_->showNotification(QString(tr("%1 updated")).arg(airInfo.name_));
}

void MainWindow::onDeleteAirport(QString airUrl) {
	model_->removeAirsConnection(airUrl);
	BusView::inst()->setting().saveAllProfile(*model_);
}
