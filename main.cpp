
#include <QApplication>
#include <QColorDialog>
#include <QStyleFactory>
#include <QSettings>
#include <QMessageBox>

void startError(QStringList inputArgs) {
    QMessageBox::critical(nullptr, "Invalid command line arguments", "<p><b>You used invalid command line arguments:</b></p><p><code>" + inputArgs.join(" ") + "</code></p><p><b>These are possible ways to start the app:</b></p><ul><li><code>-s \"styleName\"</code> or <code>-style \"styleName\"</code> to set a UI style</li><li><code>-allstyles</code> or <code>-a</code> to retrive a list of all existing styles.</li><ul>");
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QSettings set("ma7t3", "SimpleColorPicker");

    QStringList args = a.arguments();
    QStringList styles = QStyleFactory::keys();

    QString currentStyleName;

    switch(args.count()) {
        case 2:
            if(args[1] == "--allstyles" || args[1] == "-a") {
                QMessageBox::information(nullptr, "All available styles", "<p><b>These are all available styles:</b></p><ul><li>" + styles.join("</li><li>") + "</li><ul>");

            } else {
                QStringList argsW = args;
                argsW.remove(0);
                startError(argsW);
                return 1;
            } break;
        case 3:
            if(args[1] == "--style" || args[1] == "-s") {
                if(styles.contains(args[2])) {
                    currentStyleName = args[2];
                    set.setValue("lastStyle", currentStyleName);
                } else {
                    QMessageBox::warning(nullptr, "Style not found", "<p><b>The style was not found by the system:</b></p><p>" + args[2] + "</p>");
                    currentStyleName = set.value("lastStyle", "windowsvista").toString();
                }
            } else {
                QStringList argsW = args;
                argsW.remove(0);
                startError(argsW);
                return 1;
            }
    }

    if(currentStyleName.isEmpty())
        currentStyleName = set.value("lastStyle", "windowsvista").toString();

    QStyle *style = QStyleFactory::create(currentStyleName);

    if(style)
        a.setStyle(style);
    else {
        if(styles.contains("windowsvista")) {
            a.setStyle(QStyleFactory::create("windowsvista"));
        }
    }

    QColorDialog dlg;
    dlg.setOption(QColorDialog::NoButtons);
    dlg.setWindowIcon(QIcon(":/Icon.ico"));
    dlg.exec();
    return 0;
}
