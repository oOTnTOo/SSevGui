#ifndef TOOLS_H
#define TOOLS_H

#include "QString"
#include "DataDefine.h"

class Tools {
public:
	static QString makeServerShareUrl(const SQProfile profile);
	static QImage makeQR(QString content);
};

#endif // TOOLS_H
