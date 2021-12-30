#include "MainWindow.h"

#include <signal.h>
#include <QApplication>
#include <QTranslator>

static void onSignalRecv(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		qApp->quit();
	} else {
		qWarning("Unhandled signal %d", sig);
	}
}

void setupApplication(QApplication &a)
{
	signal(SIGINT, onSignalRecv);
	signal(SIGTERM, onSignalRecv);

	QApplication::setFallbackSessionManagementEnabled(false);

	a.setApplicationName(QString("SSevGUI-Qt5"));
	a.setApplicationDisplayName(QString("SSevGUI-Qt5"));
	a.setOrganizationDomain("SSevGUI");
	a.setOrganizationName("SSevGUI");
	//a.setApplicationVersion(APP_VERSION);

#ifdef Q_OS_WIN
	if (QLocale::system().country() == QLocale::China) {
		a.setFont(QFont("Microsoft Yahei", 9, QFont::Normal, false));
	}
	else {
		a.setFont(QFont("Segoe UI", 9, QFont::Normal, false));
	}
#endif
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
	QIcon::setThemeName("Breeze");
#endif

	QTranslator *zhcn = new QTranslator(&a);
	zhcn->load(qApp->applicationDirPath()+"/zh_CN.qm");
	a.installTranslator(zhcn);
}

int main(int argc, char *argv[]) {
	QApplication::setFallbackSessionManagementEnabled(false);

	QApplication a(argc, argv);
	setupApplication(a);

	MainWindow w;

	if (w.isInstanceRunning()) {
		return -1;
	}

	w.show();
	return a.exec();
}
