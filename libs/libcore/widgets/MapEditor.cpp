#include "MapEditor.hpp"
#include "ui_MapEditor.h"

#include "../libmap/MapControl.hpp"
#include "../libmap/OSMMapAdapter.hpp"
#include "../libmap/OpenAerialMapAdapter.hpp"
#include "../libmap/MapLayer.hpp"
#include "../libmap/GoogleMapAdapter.hpp"
#include "../libmap/LineString.hpp"



#include <QGeoPositionInfoSource>

MapEditor::MapEditor(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::MapEditor)
	, gps(QGeoPositionInfoSource::createDefaultSource(this))
{
	ui->setupUi(this);
	if (0 != gps) {
		gps->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
		if(0!=gps){
			if(!connect(gps, SIGNAL(positionUpdated(QGeoPositionInfo)),this,SLOT(onPositionUpdated(QGeoPositionInfo)))){
				qWarning()<<"ERROR: Could not connect";
			}
		}
	}
	else{
		ui->toolButtonCenter->setEnabled(false);
	}
	prepareMap();
}

MapEditor::~MapEditor()
{
	delete ui;
}


void MapEditor::prepareMap(){
	qmapcontrol::MapControl *mc=ui->widgetMap;
	if(0!=mc){
		mc->showScale(true);
		QDir dir("./map.cache/");
		if (!dir.exists()) dir.mkpath(".");
		mc->enablePersistentCache ( dir,8192);
		// create MapAdapter to get maps from
		//		qmapcontrol::MapAdapter* mapadapter = new qmapcontrol::OSMMapAdapter();
		//qmapcontrol::MapAdapter* mapadapter = new qmapcontrol::OpenAerialMapAdapter();
		//qmapcontrol::TileMapAdapter* mapadapter = new qmapcontrol::TileMapAdapter("tile.openstreetmap.org", "/%1/%2/%3.png", 256, 0, 17);
		qmapcontrol::TileMapAdapter* mapadapter = new qmapcontrol::GoogleMapAdapter(qmapcontrol::GoogleMapAdapter::satellite);



		//qmapcontrol::TileMapAdapter* mapadapter = new qmapcontrol::TileMapAdapter("cache.kartverket.no/grunnkart/wmts", "/%1/%2/%3.png", 256, 0, 17);

		// create a map layer with the mapadapter
		qmapcontrol::Layer* l = new qmapcontrol::MapLayer("Custom Layer", mapadapter);
		// add Layer to the MapControl
		mc->addLayer(l);

		// create a LineString
		QList<qmapcontrol::Point*> points;

		points.append(new qmapcontrol::Point( 5.456635,60.384571, "HQ"));
		points.append(new qmapcontrol::Point(  5.456249,60.384317, "Launch point"));

		// A QPen also can use transparency
		QPen* linepen = new QPen(QColor(0, 0, 255, 100));
		linepen->setWidth(5);
		// Add the Points and the QPen to a LineString
		qmapcontrol::LineString* ls = new qmapcontrol::LineString(points, "Launch vector", linepen);

		// Add the LineString to the layer
		l->addGeometry(ls);
		homeMap();
	}
}





void MapEditor::homeMap() {
	qmapcontrol::MapControl *mc=ui->widgetMap;
	if(0!=mc){
		//qDebug()<<"HOME";
		QList<QPointF> londalen;
		londalen <<QPointF(5.452844, 60.385883);
		londalen <<QPointF(5.457945, 60.380353);
		mc->setViewAndZoomIn ( londalen) ;
	}
}


void MapEditor::onPositionUpdated(QGeoPositionInfo pi){
	if(pi.isValid() ){
		QGeoCoordinate c=pi.coordinate();
		if(c.isValid()){
			qmapcontrol::MapControl *mc=ui->widgetMap;
			if(0!=mc){
				QPointF p(c.longitude(), c.latitude());
				//qDebug()<<"CENTER";
				QList<QPointF> center;
				center <<p;
				center <<p;
				mc->setViewAndZoomIn ( center) ;
			}
		}
	}
}

void MapEditor::on_toolButtonHome_clicked(){
	ui->toolButtonCenter->setChecked(false);
	homeMap();
}

void MapEditor::on_toolButtonCenter_toggled(bool checked)
{
	if(checked){
		gps->startUpdates();
	}
	else{
		gps->stopUpdates();
	}
}
