#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLabel>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "dynamic_interaction_graph_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
	//azzeramento del seed random per non avere sempre lo stesso seed ad ogni esecuzione
	//permette di avere numeri semi-randomici plausibili.
	srand(time(NULL));
    MainWindow w;
	w.setFixedSize(w.getWindowX(), w.getWindowY());
	w.show();
	w.setWindowTitle("Dynamic interaction graph");

    return a.exec();
}
