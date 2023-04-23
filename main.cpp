#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "curl_utils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    get_response_from_ai_us("http:", "fs", "fsef");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "OpenAi-Client_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
