#include "HubMain.hpp"

#include "hub/HubWindow.hpp"
#include "hub/Hub.hpp"
#include "basic/LogHandler.hpp"

#include <QDebug>
#include <QTimer>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "3d/PoseView.hpp"
HubMain::HubMain(int argc, char *argv[]):
	QObject(0)
  , sm(0)
  , ret(EXIT_SUCCESS)
  , argc(argc)
  , argv(argv)
  , app(0)
{
	qsrand(QDateTime::currentMSecsSinceEpoch());
	setObjectName("HubMain");
	LogHandler::setLogging(true);
	QCommandLineParser parser;
	parser.setApplicationDescription("Robust real-time communication and control library for robots");
	parser.addHelpOption();
	parser.addVersionOption();

	QCommandLineOption localHostOption(QStringList() <<  "l" << "local-host", QCoreApplication::translate("main", "Select server host to listen."), QCoreApplication::translate("main", "local-host"));
	parser.addOption(localHostOption);

	QCommandLineOption localPortOption(QStringList() <<  "p" << "local-port", QCoreApplication::translate("main", "Select server port to listen."), QCoreApplication::translate("main", "local-port"));
	parser.addOption(localPortOption);

	QCommandLineOption remoteHostOption(QStringList() <<  "r" << "remote-host", QCoreApplication::translate("main", "Select remote host to target."), QCoreApplication::translate("main", "remote-host"));
	parser.addOption(remoteHostOption);

	QCommandLineOption remotePortOption(QStringList() <<  "o" << "remote-port", QCoreApplication::translate("main", "Select remote port to target."), QCoreApplication::translate("main", "remote-port"));
	parser.addOption(remotePortOption);

	QCommandLineOption headlessOption(QStringList() <<  "h" << "head-less", QCoreApplication::translate("main", "Don't display GUI"), QCoreApplication::translate("main", "head-less"));
	parser.addOption(headlessOption);

	// Process the actual command line arguments given by the user
	QStringList arguments;
	for(int i=0;i<argc;++i){
		arguments<<argv[i];
	}
	parser.process(arguments);

	const bool headless=parser.isSet(headlessOption);



	app=(headless?(new QCoreApplication(argc, argv)):(new QApplication(argc, argv)));
	qDebug()<<(headless?"HEADLESS":"GUI ENABLED");
	Hub *hub=new Hub(parser);
	if(!headless){
		HubWindow *w=new HubWindow (hub,0);
		w->show();
	}


	//	app=new QApplication(argc, argv); PoseView widget; widget.show();

	if(0!=app){

		QSurfaceFormat fmt;
		fmt.setDepthBufferSize(24);
		/*
		if (QCoreApplication::arguments().contains(QStringLiteral("--multisample")))
			fmt.setSamples(4);
		if (QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"))) {
			fmt.setVersion(3, 2);
			fmt.setProfile(QSurfaceFormat::CoreProfile);
		}*/
		QSurfaceFormat::setDefaultFormat(fmt);

		Q_INIT_RESOURCE(style);
		Q_INIT_RESOURCE(fonts);
		Q_INIT_RESOURCE(icons);
		Q_INIT_RESOURCE(qfi);
		Q_INIT_RESOURCE(3d);
		QTimer::singleShot(0, this, SLOT(run()));
		ret=app->exec();
		qDebug()<<QFileInfo( QCoreApplication::applicationFilePath()).fileName() << " done, quitting";
	}
	else{
		qWarning()<<"ERROR: no app, quitting";
	}
}

HubMain::~HubMain(){
}


void HubMain::run(){
	sm=new StyleManager;
	if(0!=sm){
		sm->apply();
	}
}


int main(int argc, char *argv[]){
	HubMain m(argc,argv);
}



