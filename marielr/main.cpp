#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Application du qt_fr pour la traduction
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    if (translator.load ("qtbase_" + locale, QLibraryInfo::location (QLibraryInfo::TranslationsPath))) {
            app.installTranslator (&translator);
        }


    MainWindow w;
    w.show();

    return app.exec();
}
