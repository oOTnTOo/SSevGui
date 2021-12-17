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

	BusView::inst()->setting().readProfile(model_);

	connect(ui->tableServers,&QTableView::customContextMenuRequested,
			[this](const QPoint &pos){
		//this->checkCurrentIndex(ui->tableServers->indexAt(pos));
		ui->menuConnection->popup(ui->tableServers->viewport()->mapToGlobal(pos));
	});
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
		BusView::inst()->setting().saveProfile(*model_);
	}
	delete ciw;
}

void MainWindow::on_actionConnect_triggered() {
	BusView::inst()->startProxy(model_->getItem(ui->tableServers->currentIndex().row())->connection()->profile());
}

void MainWindow::on_actionClear_triggered() {
	if(QMessageBox::Yes == QMessageBox::question(this,"Are you sure?","Clear list? sure?")) {
		model_->clearItems();
		BusView::inst()->setting().saveProfile(*model_);
	}
}

void MainWindow::on_actionManager_triggered() {
	SubscriptionManagerWgt* smw = new SubscriptionManagerWgt(this);
	smw->setAttribute(Qt::WA_DeleteOnClose);
	smw->show();
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

void MainWindow::on_tableServers_doubleClicked(const QModelIndex &index) {
	Connection* conn = model_->getItem(proxyModel_->mapToSource(index).row())->connection();
	ConnectionInfoWgt* ciw = new ConnectionInfoWgt(this);
	ciw->setProfile(conn->profile());
	if(QDialog::Accepted == ciw->exec()) {
		conn->setProfile(ciw->profile());
		BusView::inst()->setting().saveProfile(*model_);
	}
	delete ciw;
}
