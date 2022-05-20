#include "Tools.h"
#include "3rd/qzxing/QZXing"
#include <QDebug>

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

bool Tools::parseLegacyURL(const QString& ssURL, SQProfile& profile) {
	QRegExp legacyUrlFinder(R"(ss://([A-Za-z0-9+-/=_]+)(?:#(\S+))?)");
	QRegExp detailFinder(R"(^(.*):(.*)@(.*):(.*)$)");
	if(legacyUrlFinder.indexIn(ssURL) < 0)
		return false;

	profile.remake = legacyUrlFinder.cap(2);

	QByteArray det = short2base64(legacyUrlFinder.cap(1).toUtf8());

	if(detailFinder.indexIn(det) < 0)
		return false;

	profile.method = detailFinder.cap(1);
	profile.password = detailFinder.cap(2);
	profile.serverAddress = detailFinder.cap(3);
	profile.serverPort = detailFinder.cap(4).toInt();

	return true;
}

bool Tools::parseSIP002URL(const QString& ssURL, SQProfile& profile) {
	QRegExp urlFinder(R"(ss://(.*)@(.*):([0-9]*)\/\?plugin=(.*)#(.*))");
	if(urlFinder.indexIn(ssURL) < 0)
		return false;

	QList<QByteArray> userInfo = QByteArray::fromBase64(short2base64(urlFinder.cap(1).toUtf8())).split(':');
	profile.method = userInfo.first();
	profile.password = userInfo.last();
	profile.serverAddress = urlFinder.cap(2);
	profile.serverPort = urlFinder.cap(3).toInt();
	profile.remake = QByteArray::fromPercentEncoding(urlFinder.cap(5).toUtf8());
	QList<QByteArray> pp = QByteArray::fromPercentEncoding(urlFinder.cap(4).toUtf8()).split(';');
	profile.plugin = pp.first();
	pp.removeFirst();
	profile.pluginOpt = pp.join(';');

	return true;
}

QByteArray Tools::safeTobase64(const QByteArray& safeBase64) {
	QByteArray bytes = safeBase64;
	bytes.replace('-', '+').replace('_', '/');
	int mod4 = bytes.count() % 4;
	if(mod4 > 0){
		bytes.append(4-mod4,'=');
	}

	return bytes;
}

QByteArray Tools::short2base64(const QByteArray& shortBase64) {
	QByteArray det = shortBase64;
	int mod = (det.count()%4);

	return det.append(mod ? 4-mod:mod, '=');
}
