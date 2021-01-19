#include "backend.h"

Backend::Backend(QObject *parent) : QObject(parent)
{
	_fetchKountdowns();
}

void Backend::_fetchKountdowns() {
	_kountdownArray.clear();
	QString jsonFilePath = QDir::homePath() + "/.local/share/DayKountdown/kountdowns.json";
	QFile inFile(jsonFilePath);
	if(inFile.exists()) {
		qDebug() << "Found kountdowns.json";
		inFile.open(QIODevice::ReadOnly | QIODevice::Text);
		QByteArray data = inFile.readAll();
		inFile.close();
		
		QJsonParseError errorPtr;
		QJsonDocument kountdownsDoc = QJsonDocument::fromJson(data, &errorPtr);
		if(kountdownsDoc.isNull())
			qDebug() << "Parse failed";
		QJsonObject rootObj = kountdownsDoc.object();
		QJsonArray kountdownsJsonArray = rootObj.value("kountdowns").toArray();
		
		/*
		* JSON Structure should be like so:
		* {
		* 		"kountdowns": [
		* 			{
		* 				"name": "kountdown1",
		* 				"description": "kountdown number one",
		* 				"date": QDateTime object
		* 			}
		* 		]
		* }
		* 
		*/
		
		int i = 0;
		foreach(const QJsonValue & kountdownJson, kountdownsJsonArray) {
			kountdown currKountdown;
			currKountdown.index = i;
			currKountdown.name = kountdownJson.toObject().value("name").toString();
			currKountdown.description = kountdownJson.toObject().value("description").toString();
			currKountdown.date = kountdownJson.toObject().value("date").toString();
			_kountdownArray.append(currKountdown);
			i++;
		}
	}
	else {
		qDebug() << "Didn't find kountdowns.json";
	}
}

QVariantList Backend::kountdownPopulator () {
	QVariantList kountdownsList;
	
	for(const kountdown & k : _kountdownArray) {
		kountdownsList << QVariant::fromValue(k);
	}
	
	return kountdownsList;
}
