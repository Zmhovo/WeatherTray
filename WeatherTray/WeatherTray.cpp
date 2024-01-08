#include "WeatherTray.h"

WeatherTray::WeatherTray(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);          //���ô����ޱ߿�
    setFixedSize(width(), height());

    mExitMenu = new QMenu(this);
    mExitAct = new QAction();

    mExitAct->setText("�˳�Exit");
    mExitAct->setIcon(QIcon(":/res/close.png"));

    mExitMenu->addAction(mExitAct);

    connect(mExitAct, &QAction::triggered, this, [=] {
        qApp->exit(0);
        });
}

WeatherTray::~WeatherTray()
{
}

void WeatherTray::onReplied(QNetworkReply* reply)
{
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() != QNetworkReply::NoError || status_code != 200)
    {
        QMessageBox::warning(this, "����", "��������ʧ��", QMessageBox::Ok);
    }
    else
    {
        QByteArray byteArray = reply->readAll();
    }

    reply->deleteLater();
}

void WeatherTray::contextMenuEvent(QContextMenuEvent* event)          //�����Ҽ��˵�
{
    mExitMenu->exec(QCursor::pos());

    event->accept();
}

void WeatherTray::mousePressEvent(QMouseEvent* event)
{
    mOffset = event->globalPos() - this->pos();
}

void WeatherTray::mouseMoveEvent(QMouseEvent* event)
{
    this->move(event->globalPos() - mOffset);
}
