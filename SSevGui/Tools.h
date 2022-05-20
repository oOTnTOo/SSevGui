#ifndef TOOLS_H
#define TOOLS_H

#include "QString"
#include "DataDefine.h"

class Tools {
public:
	static QString makeServerShareUrl(const SQProfile profile);
	static QImage makeQR(QString content);

	static bool parseLegacyURL(const QString& ssURL, SQProfile& profile);
	static bool parseSIP002URL(const QString& ssURL, SQProfile& profile);

	static QByteArray safeTobase64(const QByteArray& safeBase64);
	static QByteArray short2base64(const QByteArray& shortBase64);
};

#endif // TOOLS_H
