#include "Tools.h"
#include "3rd/qzxing/QZXing"

QString Tools::makeServerShareUrl(const SQProfile profile) {
	QByteArray ssurl;
	if(profile.plugin.isEmpty()) {
		ssurl = QString("%1:%2@%3:%4")
				.arg(profile.method)
				.arg(profile.password)
				.arg(profile.serverAddress)
				.arg(profile.serverPort).toUtf8().toBase64();
	} else {
		QByteArray sBase64 = QString("%1:%2")
				.arg(profile.method)
				.arg(profile.password)
				.toUtf8().toBase64().replace('+','-').replace('/','_').replace('=',' ').trimmed();
		QString pluPart = profile.plugin;

		if(!profile.pluginOpt.isEmpty()) {
			pluPart += ";" + profile.pluginOpt;
		}

		ssurl = QString("%1@%2:%3/?plugin=%4")
				.arg(QString::fromUtf8(sBase64))
				.arg(profile.serverAddress)
				.arg(profile.serverPort)
				.arg(QString::fromUtf8(pluPart.toUtf8().toPercentEncoding()))
				.toUtf8();
	}

	if(!profile.remake.isEmpty()) {
		ssurl += "#" + profile.remake.toUtf8().toPercentEncoding();
	}

	return "ss://" + ssurl;
}

QImage Tools::makeQR(QString content) {
	return QZXing::encodeData(content);
}
