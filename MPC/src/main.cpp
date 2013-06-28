#include <QApplication>
#include "allapplicationdialog.h"
#include "start.h"


int main(int argc, char ** argv)
{
    QApplication app( argc, argv );
    app.setOrganizationName("MPC");
    app.setOrganizationDomain("MPC.googlecode.com");
    app.setApplicationName("MPC");

    QSettings settings;
    settings.beginGroup("MainWindow");
    QString language = settings.value("language").toString();
    settings.endGroup();
    QTranslator translator;
    if (language == "")
        translator.load(":/translations/MPC_" + QLocale::system().name());
    else
        translator.load(":/translations/MPC_" + language);
    app.installTranslator(&translator);
    QTranslator translatorQt;
    translatorQt.load( ":/translations/Qt_"+language+".qm" );
    if (translatorQt.isEmpty())
        translatorQt.load( QLibraryInfo::location( QLibraryInfo::TranslationsPath) + "/qt_"+QLocale::system().name()+".qm" );
    if (!translatorQt.isEmpty())
        app.installTranslator( &translatorQt );

    //start * newstart = new start();
    //newstart->show();

    //MainWindow win;
    //win.show();
    //win.threadsDetailTableWidget->hide();


    AllApplicationDialog *hello = new AllApplicationDialog();
    hello->show();

    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    return app.exec();
}

