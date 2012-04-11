
#include <QApplication>
#include "MainWindow.hh"

int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(main);
  QApplication app(argc, argv);
  app.setApplicationName("Main");
  app.setOrganizationName("TradingCraft.com");
  // MainWindow will delete itself
  MainWindow* mw = new MainWindow();
  mw->show();
  return app.exec();
}
