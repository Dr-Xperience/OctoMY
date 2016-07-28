#include "NodeAssociateStore.hpp"

#include "utility/Utility.hpp"

#include <QFile>
#include <QThreadPool>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QVariantMap>
#include <QVariantList>

NodeAssociateStore::NodeAssociateStore(QString fn, QObject *parent)
	: QObject(parent)
	, mReady(false)
	, mError(false)
	, mFilename(fn)
{
	if(QFile(mFilename).exists()){
		load();
	}
}



NodeAssociateStore::~NodeAssociateStore()
{
	save();
}


void NodeAssociateStore::bootstrap(bool inBackground)
{
	// QThreadPool takes ownership and deletes runnable automatically after completion
	if(inBackground){
		QThreadPool *tp=QThreadPool::globalInstance();
		if(0!=tp){
			const bool ret=tp->tryStart(new GenerateRunnable<NodeAssociateStore>(*this));
			if(ret){
				//qDebug()<<"NodeAssociateStore: Successfully started background thread";
				return;
			}
		}
		// Fall back to single threaded wday
		qDebug()<<"NodeAssociateStore: Falling back to serial bootstrap";
	}
	bootstrapWorker();
}


void NodeAssociateStore::bootstrapWorker()
{
	QFile f(mFilename);
	if(!f.exists()){
		qDebug()<<"NodeAssociateStore: no store file found, saving for first time";
		save();
	}
	load();
}


void NodeAssociateStore::load()
{
	//qDebug()<<"NodeAssociateStore: Loading from file";
	QJsonParseError jsonError;
	QByteArray raw=utility::fileToByteArray(mFilename);
	QJsonDocument doc = QJsonDocument::fromJson(raw, &jsonError);
	if (QJsonParseError::NoError != jsonError.error){
		qWarning() << "ERROR: Parsing json data: "<<jsonError.errorString()<< " for data "<<raw<<" from file "<<mFilename;
		mError=true;
	}
	else{
		//qDebug()<<"PARSED JSON: "<<doc.toJson();
		QVariantMap map = doc.object().toVariantMap();
		QVariantList remotes=map["participants"].toList();
		for(QVariantList::iterator b=remotes.begin(), e=remotes.end(); b!=e; ++b){
			QSharedPointer<NodeAssociate> peer=QSharedPointer<NodeAssociate>(new NodeAssociate((*b).toMap()));
			mPeers[peer->id()]=peer;
		}
		mReady=true;
	}
	emit discoveryStoreReady();
}

void NodeAssociateStore::save()
{
	qDebug()<<"NodeAssociateStore: Saving to file: "<<mFilename;
	QVariantMap map;
	map["createdTimeStamp"]=QDateTime::currentMSecsSinceEpoch();
	QVariantList remotes;
	for(QMap<QString, QSharedPointer<NodeAssociate> >::const_iterator b=mPeers.begin(), e=mPeers.end(); b!=e; ++b){
		remotes.push_back(b.value()->toVariantMap());
	}
	map["participants"]=remotes;
	QJsonDocument doc=QJsonDocument::fromVariant(map);
	utility::stringToFile(mFilename,doc.toJson());
}




bool NodeAssociateStore::hasParticipant(const QString &id)
{
	return (mPeers.find(id)!=mPeers.end());
}



QSharedPointer<NodeAssociate> NodeAssociateStore::getParticipant(const QString &id)
{
	if(hasParticipant(id)){
		return mPeers[id];
	}
	QSharedPointer<NodeAssociate> ret;
	return ret;
}


QSharedPointer<NodeAssociate> NodeAssociateStore::removeParticipant(const QString &id)
{
	QSharedPointer<NodeAssociate> ret;
	if(hasParticipant(id)){
		ret=mPeers[id];
		mPeers.remove(id);
	}
	return ret;
}



void NodeAssociateStore::setParticipant(QSharedPointer<NodeAssociate> participant)
{
	if(nullptr!=participant){
		auto id=participant->id();
		qDebug()<<"REGISTERING PARTICIPANT WITH ID: "<<id;
		mPeers[id]=participant;
	}
}


QMap<QString, QSharedPointer<NodeAssociate> > &NodeAssociateStore::getParticipants()
{
	return mPeers;
}
