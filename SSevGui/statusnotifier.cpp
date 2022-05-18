#include "statusnotifier.h"
#include "MainWindow.h"
#include <QApplication>
#include "BusView.h"
#ifdef Q_OS_LINUX
#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusPendingCall>
#endif

StatusNotifier::StatusNotifier(MainWindow *w, bool startHiden, QObject *parent) :
    QObject(parent),
    window(w)
{
	systray.setIcon(QIcon(":/icon/Resource/shadowsocks-qt5.png"));
	systray.setToolTip(QString("SSevGui"));
    connect(&systray, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason r) {
        if (r != QSystemTrayIcon::Context) {
            this->activate();
        }
    });
    minimiseRestoreAction = new QAction(startHiden ? tr("Restore") : tr("Minimise"), this);
    connect(minimiseRestoreAction, &QAction::triggered, this, &StatusNotifier::activate);
    systrayMenu.addAction(minimiseRestoreAction);
	systrayMenu.addSeparator();

	actStopProxy_ = systrayMenu.addAction(tr("Stop proxy"),BusView::inst(),&BusView::stopProxy);
	connect(BusView::inst(),&BusView::sig_currentItemChanged,this,&StatusNotifier::onCurrentItemChanged);
	actStopProxy_->setEnabled(false);

	serverList_.setTitle(tr("Server"));
	systrayMenu.addMenu(&serverList_);
	connect(BusView::inst(),&BusView::sig_modelItemChanged,this,&StatusNotifier::createServerList);

	systrayMenu.addMenu(w->subscriptionMenu());

	systrayMenu.addSeparator();
	systrayMenu.addAction(tr("Quit"), qApp, &QApplication::quit);
    systray.setContextMenu(&systrayMenu);
	systray.show();
}

void StatusNotifier::createServerList() {
	const QMap<QString, QList<ConnectionItem*>>& its = BusView::inst()->connectionItemsWithAir();
	QMap<QString, QList<ConnectionItem*>>::const_iterator i = its.constBegin();
	while(i!=its.constEnd()) {
		QMenu* m = new QMenu(&serverList_);
		const AirportInfo& ai = i.value().first()->connection()->profile().airportInfo_;
		m->setTitle(tr("[%1/%2 G %3 day] %4").arg(ai.trafficUsed_).arg(ai.trafficTotal_)
					.arg(QDateTime::currentDateTime()
						.daysTo(QDateTime::fromString(ai.expiry_,"yyyy-MM-dd hh:mm:ss")))
					.arg(ai.name_));
		for(ConnectionItem* ci: i.value()) {
			const SQProfile& s = ci->connection()->profile();
			QString lag = s.latency < 1 ? tr("Unknown") : QString::number(s.latency);
			//ServerAction* sa = new ServerAction(QString("[%1] %2 (%3:%4)").arg(lag).arg(s.name).arg(s.serverAddress).arg(s.serverPort),m);
			ServerAction* sa = new ServerAction(m);
			sa->setText(s.remake,s.serverAddress,s.serverPort,ci->data(4).toString());
			connect(sa,&ServerAction::triggered,this,&StatusNotifier::serverMenuClicked);
			connect(ci,&ConnectionItem::latencyChanged,sa,&ServerAction::onItemLatencyChanged);
			sa->setData(QVariant::fromValue<ConnectionItem*>(ci));
			m->addAction(sa);
		}
		serverList_.addMenu(m);
		++i;
	}
}

void StatusNotifier::activate()
{
    if (!window->isVisible() || window->isMinimized()) {
        window->showNormal();
        window->activateWindow();
        window->raise();
    } else {
        window->hide();
    }
}

void StatusNotifier::showNotification(const QString &msg)
{
#ifdef Q_OS_LINUX
    //Using DBus to send message.
    QDBusMessage method = QDBusMessage::createMethodCall("org.freedesktop.Notifications","/org/freedesktop/Notifications", "org.freedesktop.Notifications", "Notify");
    QVariantList args;
	args << QCoreApplication::applicationName() << quint32(0) << "SSevGui" << "SSevGui" << msg << QStringList () << QVariantMap() << qint32(-1);
    method.setArguments(args);
    QDBusConnection::sessionBus().asyncCall(method);
#else
    systray.showMessage("Shadowsocks-Qt5", msg);
#endif
}

void StatusNotifier::onWindowVisibleChanged(bool visible)
{
	minimiseRestoreAction->setText(visible ? tr("Minimise") : tr("Restore"));
}

void StatusNotifier::serverMenuClicked(bool checked) {
	QAction* na = qobject_cast<QAction*>(sender());
	if(!na)
		return;
	if(checked) {
		ConnectionItem* ci = na->data().value<ConnectionItem*>();
		BusView::inst()->startProxy(ci);
	} else {
		BusView::inst()->stopProxy();
	}
}

void StatusNotifier::onCurrentItemChanged(const ConnectionItem* newItem) {
	actStopProxy_->setEnabled(newItem);
}





















ServerAction::ServerAction(QWidget* parent) : QAction(parent) {
	setCheckable(true);
	connect(BusView::inst(),&BusView::sig_currentItemChanged,this,&ServerAction::onCurrentItemChanged);
}

void ServerAction::setText(const QString& name, const QString& address, quint16 port, QString lag) {
	QAction::setText(QString("[%1] %2 (%3:%4)").arg(lag).arg(name).arg(address).arg(port));
}

void ServerAction::onCurrentItemChanged(const ConnectionItem* newItem) {
	if(data().value<ConnectionItem*>() == newItem)
		setChecked(true);
	else
		setChecked(false);
}

void ServerAction::onItemLatencyChanged() {
	ConnectionItem* ci = qobject_cast<ConnectionItem*>(sender());
	if(nullptr == ci)
		return;

	const SQProfile& pr = ci->connection()->profile();
	setText(pr.remake,pr.serverAddress,pr.serverPort,ci->data(4).toString());
}
