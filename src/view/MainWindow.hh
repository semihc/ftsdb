#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QSharedPointer>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include "FtsDb.hh"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
class QDockWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow();
  MainWindow(const QString &fileName);
  virtual ~MainWindow();

 protected:
  void closeEvent(QCloseEvent *event);

 private slots:
  void newFile();
  void open();
  bool save();
  bool saveAs();
  void about();
  void documentWasModified();
  void dbList();

 private:
  void init();
  void createDocks();
  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();
  void readSettings();
  void writeSettings();
  bool maybeSave();
  void loadFile(const QString &fileName);
  bool saveFile(const QString &fileName);
  void setCurrentFile(const QString &fileName);
  QString strippedName(const QString &fullFileName);
  MainWindow *findMainWindow(const QString &fileName);

  QTextEdit *textEdit;
  QDockWidget *dockWidget;
  QString curFile;
  bool isUntitled;
  
  QMenu *fileMenu;
  QMenu *editMenu;
  QMenu *helpMenu;
  QToolBar *fileToolBar;
  QToolBar *editToolBar;
  QAction *newAct;
  QAction *openAct;
  QAction *saveAct;
  QAction *saveAsAct;
  QAction *closeAct;
  QAction *exitAct;
  QAction *cutAct;
  QAction *copyAct;
  QAction *pasteAct;
  QAction *dbListAct;
  QAction *aboutAct;
  QAction *aboutQtAct;

  QAbstractItemModel* m_dbModel;
  QAbstractItemView* m_dbView;
  QSharedPointer<TC::FtsDb> m_ftsdb;
};

#endif
