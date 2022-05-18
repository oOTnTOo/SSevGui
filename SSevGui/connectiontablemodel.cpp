#include "connectiontablemodel.h"

ConnectionTableModel::ConnectionTableModel(QObject *parent) :
	QAbstractTableModel(parent) {
	header_ << tr("Name") << tr("Server") << tr("Airport") << tr("Status") << tr("Latency")
			<< tr("Local Port");
}

ConnectionTableModel::~ConnectionTableModel()
{}

const QMap<QString, QList<ConnectionItem*> >& ConnectionTableModel::connectionItemsWithAir() const {
	//const QMap<QString, QList<ConnectionItem*> >* t = &mapAirItems_;
	return mapAirItems_;
}

const QList<ConnectionItem*>& ConnectionTableModel::connectionItems() const {
	return items;
}

ConnectionItem *ConnectionTableModel::getItem(const int &row) const
{
    return items.at(row);
}

int ConnectionTableModel::rowCount(const QModelIndex &) const
{
    return items.count();
}

int ConnectionTableModel::columnCount(const QModelIndex &) const
{
	return header_.count();
}

QVariant ConnectionTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    ConnectionItem *item = getItem(index.row());
	return item->data(index.column(), role);
}

QVariant ConnectionTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical || role != Qt::DisplayRole) {
        return QVariant();
    }

	return header_.value(section);
}

QModelIndex ConnectionTableModel::index(int row, int column, const QModelIndex &) const
{
    if (row < 0 || row >= items.size()) {
        return QModelIndex();
    } else {
        ConnectionItem* item = items.at(row);
        return createIndex(row, column, item);//column is ignored (all columns have the same effect)
    }
}

bool ConnectionTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || count <= 0 || count + row > items.count()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    items.erase(items.begin() + row, items.begin() + row + count);
    endRemoveRows();
	return true;
}

void ConnectionTableModel::removeAirsConnection(QString airportUrl) {
	for(int i=items.count()-1;i>-1;--i) {
		if(items.at(i)->connection()->profile().airportInfo_.url_ == airportUrl) {
			beginRemoveRows(QModelIndex(),i,i);
			ConnectionItem* ci = items.takeAt(i);
			delete ci;
			endRemoveRows();
		}
	}

	mapAirItems_.remove(airportUrl);
}

bool ConnectionTableModel::move(int row, int target, const QModelIndex &parent)
{
    if (row < 0 || row >= rowCount() || target < 0 || target >= rowCount() || row == target) {
        return false;
    }

    //http://doc.qt.io/qt-5/qabstractitemmodel.html#beginMoveRows
    int movTarget = target;
    if (target - row > 0) {
        movTarget++;
    }
    beginMoveRows(parent, row, row, parent, movTarget);
    items.move(row, target);
    endMoveRows();
	return true;
}

void ConnectionTableModel::clearItems() {
	beginResetModel();
	qDeleteAll(items);
	items.clear();
	endResetModel();
}

bool ConnectionTableModel::appendConnection(Connection *con, const QModelIndex &parent)
{
    ConnectionItem* newItem = new ConnectionItem(con, this);
    connect(newItem, &ConnectionItem::message, this, &ConnectionTableModel::message);
    connect(newItem, &ConnectionItem::stateChanged, this, &ConnectionTableModel::onConnectionStateChanged);
    connect(newItem, &ConnectionItem::latencyChanged, this, &ConnectionTableModel::onConnectionLatencyChanged);
    connect(newItem, &ConnectionItem::dataUsageChanged, this, &ConnectionTableModel::onConnectionDataUsageChanged);
    beginInsertRows(parent, items.count(), items.count());
    items.append(newItem);
    endInsertRows();

	mapAirItems_[con->profile().airportInfo_.url_.isEmpty() ? "UnKnown" : con->profile().airportInfo_.url_] << newItem;

	return true;
}

bool ConnectionTableModel::appendProfiles(const QList<SQProfile>& pros) {
	beginResetModel();
	for (const SQProfile& pro : pros) {
		Connection* con = new Connection(pro,this);
		ConnectionItem* newItem = new ConnectionItem(con, this);
		newItem->connection()->setParent(newItem);
		connect(newItem, &ConnectionItem::message, this, &ConnectionTableModel::message);
		connect(newItem, &ConnectionItem::stateChanged, this, &ConnectionTableModel::onConnectionStateChanged);
		connect(newItem, &ConnectionItem::latencyChanged, this, &ConnectionTableModel::onConnectionLatencyChanged);
		connect(newItem, &ConnectionItem::dataUsageChanged, this, &ConnectionTableModel::onConnectionDataUsageChanged);
		items << newItem;

		mapAirItems_[pro.airportInfo_.url_.isEmpty() ? "UnKnown" : pro.airportInfo_.url_] << newItem;
	}
	endResetModel();
	return true;
}

void ConnectionTableModel::disconnectConnectionsAt(const QString &addr, quint16 port)
{
    bool anyAddr = (addr.compare("0.0.0.0") == 0);
	for (ConnectionItem* i : items) {
		Connection *con = i->connection();
		if (con->isRunning() && con->profile().localPort == port) {
			if ((con->profile().localAddress == addr) ||
				(con->profile().localAddress.compare("0.0.0.0") == 0) ||
                anyAddr) {
                con->stop();
            }
        }
	}
}

void ConnectionTableModel::modifyAirUrl(const QString& newUrl, const QString& oldUrl) {
	mapAirItems_[newUrl] = qMove(mapAirItems_[oldUrl]);
	mapAirItems_.remove(oldUrl);
}

void ConnectionTableModel::testAllLatency()
{
	for (ConnectionItem* i : items) {
        i->testLatency();
	}
}

void ConnectionTableModel::addAirConnection(QList<SQProfile> profiles, AirportInfo airInfo) {
	if(mapAirItems_.contains(airInfo.url_)) {
		removeAirsConnection(airInfo.url_);
	}

	appendProfiles(profiles);
}

void ConnectionTableModel::onConnectionStateChanged(bool running)
{
    ConnectionItem *item = qobject_cast<ConnectionItem*>(sender());
    int row = items.indexOf(item);
    emit dataChanged(this->index(row, 0),
					 this->index(row, header_.count() - 1));
    emit rowStatusChanged(row, running);
}

void ConnectionTableModel::onConnectionLatencyChanged()
{
    ConnectionItem *item = qobject_cast<ConnectionItem*>(sender());
    int row = items.indexOf(item);
	emit dataChanged(this->index(row, 4), this->index(row, 4));
}

void ConnectionTableModel::onConnectionDataUsageChanged()
{
    ConnectionItem *item = qobject_cast<ConnectionItem*>(sender());
    int row = items.indexOf(item);
	emit dataChanged(this->index(row, 5), this->index(row, 6));
}
