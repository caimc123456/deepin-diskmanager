#include "mainwindow.h"
#include "centerwidget.h"
#include "titlewidget.h"
#include "customcontrol/bufferwin.h"
#include "partedproxy/dmdbushandler.h"
#include <QTimer>
#include <DTitlebar>
#include <QApplication>
#include <QDesktopWidget>
#include <DWidgetUtil>
#include <QRect>
#include <QDebug>

#include "common.h"

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
{
    m_handler = DMDbusHandler::instance(this);
    initUi();
    initConnection();
    m_handler->getDeviceinfo(); //call after initUi
    QRect rect = QApplication::desktop()->geometry();
    setMinimumSize(rect.width() * 3 / 5, rect.height() * 3 / 5);
}

MainWindow::~MainWindow()
{
    if (nullptr != m_bufferwin) {
        m_bufferwin->deleteLater();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_central->HandleQuit();
    DMainWindow::closeEvent(event);
}

void MainWindow::initUi()
{
    m_bufferwin = new BufferWin;
    m_spinner = new DSpinner(this);
    m_spinner->setWindowFlags(Qt::SplashScreen);
    m_spinner->setWindowModality(Qt::ApplicationModal);
    m_spinner->setAttribute(Qt::WA_TranslucentBackground, true);
    m_spinner->setGeometry(100, 100, 100, 100);
    m_central = new CenterWidget(this);
    setCentralWidget(m_central);
    titlebar()->setIcon(QIcon::fromTheme(app_name));
    titlebar()->setTitle("");
    // titlebar()->setMenu(m_central->titleMenu());
    titlebar()->addWidget(m_central->titlewidget(), Qt::AlignCenter);
}

void MainWindow::initConnection()
{
    connect(m_handler, &DMDbusHandler::sigShowSpinerWindow, this, &MainWindow::slotshowSpinerWindow);
}

void MainWindow::slotHandleQuitAction()
{
    //ToDo judge exit or no
    qDebug() << __FUNCTION__;
}

void MainWindow::slotshowSpinerWindow(bool bshow)
{
    if (bshow) {
        // m_bufferwin->Start();
        m_spinner->start();
        Dtk::Widget::moveToCenter(m_spinner);
        m_spinner->show();
    } else {
        // m_bufferwin->Stop();
        m_spinner->stop();
        m_spinner->hide();
        raise();
        activateWindow();
    }
}
