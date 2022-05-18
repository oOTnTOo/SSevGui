#include "ConnectionInfoWgt.h"
#include "ui_ConnectionInfoWgt.h"

ConnectionInfoWgt::ConnectionInfoWgt(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConnectionInfoWgt) {
	//setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
	setWindowFlag(Qt::WindowCloseButtonHint);
	ui->setupUi(this);

	QStringList first={"aes","camellia"};
	QStringList middle={"128","192","256"};
	QStringList last={"gcm","cfb","ctr"};

	for (QString& fir : first) {
		for (QString& mid : middle) {
			if(fir=="camellia"){
				// camellia 只会cfb结束，不必遍历最后三位
				ui->comboEncryMethod->addItem(QString("%1-%2-%3").arg(fir).arg(mid).arg("cfb"));
				continue;
			}
			for (QString& la : last) {
				ui->comboEncryMethod->addItem(QString("%1-%2-%3").arg(fir).arg(mid).arg(la));
			}
		}
	}
}

ConnectionInfoWgt::~ConnectionInfoWgt()
{
	delete ui;
}

void ConnectionInfoWgt::setProfile(SQProfile profile) {
	ui->editName->setText(profile.remake);
	ui->editRemoteAddr->setText(profile.serverAddress);
	ui->editRemotePort->setText(QString::number(profile.serverPort));
	ui->editPwd->setText(profile.password);
	ui->editLocalAddr->setText(profile.localAddress);
	ui->editLocalPort->setText(QString::number(profile.localPort));
	profile.httpMode ? ui->radioHttp->setChecked(true) : ui->radioSocks->setChecked(true);
	ui->comboEncryMethod->setCurrentText(profile.method);
	ui->editPlugin->setText(profile.plugin);
	ui->editPlugOpt->setText(profile.pluginOpt);
	ui->editPlugArg->setText(profile.pluginArg);
}

SQProfile ConnectionInfoWgt::profile() {
	SQProfile sqp;

	sqp.autoStart = false;
	sqp.debug = false;
	sqp.remake = ui->editName->text();
	sqp.serverAddress = ui->editRemoteAddr->text();
	sqp.serverPort = ui->editRemotePort->text().toInt();
	sqp.password = ui->editPwd->text();
	sqp.localAddress = ui->editLocalAddr->text();
	sqp.localPort = ui->editLocalPort->text().toInt();
	sqp.httpMode = ui->radioHttp->isChecked();
	sqp.method = ui->comboEncryMethod->currentText();
	sqp.plugin = ui->editPlugin->text();
	sqp.pluginOpt = ui->editPlugOpt->text();
	sqp.pluginArg = ui->editPlugArg->text();

	return sqp;
}

void ConnectionInfoWgt::on_buttonBox_accepted() {
	accept();
}

void ConnectionInfoWgt::on_buttonBox_rejected() {
	close();
}
