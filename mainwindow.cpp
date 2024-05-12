#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvaswin.h"
#include "dialogclean.h"

#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QColor>

const int COLOR = 255;
const int ALPHA = 100;


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug ()<<QSslSocket::sslLibraryBuildVersionString();//依赖的ssl版本
    qDebug() << "Is Qt supported by OpenSSL? " << QSslSocket::supportsSsl();
    qDebug()<<QDir::tempPath();


    cw->drawColor = this->drawColor = QColor(255, 255, 255);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::ToolTip);//需要去掉标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);

    ui->point->setIcon(QIcon(":/point.png"));
    ui->pen->setIcon(QIcon(":/pen.png"));
    ui->eraser->setIcon(QIcon(":/eraser.png"));

    onSetToolbarPositon(2);

    connect(cw, SIGNAL(setToolBarPostion(int)), this, SLOT(onSetToolbarPositon(int)));
    connect(cw, SIGNAL(OnDrawCompleted()), this, SLOT(OnDrawCompleted()));
    connect(cw, SIGNAL(OnUpdateHistory()), this, SLOT(OnDrawCompleted()));


    connect(ui->point, &QPushButton::clicked, [&] {
        this->Point();
    });

    connect(ui->pen, &QPushButton::clicked, [&] {
        if (Mode == 1) {
            QColor c = QColorDialog::getColor(drawColor, this, tr("Select a color"));
            if(c.isValid()) cw->drawColor = drawColor = c;
        }
        this->Pen();
    });

    connect(ui->eraser, &QPushButton::clicked, [&] {
        if (Mode == 2) {
            DialogClean cn(this);
            cn.show();
            if (cn.exec() == QDialog::Accepted) {
                cw->history << QPair<int, Pages>{cw->page, cw->pages};
                cw->pages.replace(cw->page, QPair<QPoint,QList<QPair<QColor, QList<QPoint>>>>{cw->pages.at(cw->page).first,{{drawColor, QList<QPoint>() }}});
                cw->update();
                Pen();
            }
        }
        else {
            this->Eraser();
        }
    });

    connect(ui->quitButton, &QPushButton::clicked, [&] {
        if (lnbf_path != "") {QFile file(lnbf_path);
            file.open(QIODevice::ReadOnly);
            QTextStream in(&file);
            QString data;
            in>>data;
            Pages p;
            SAVE_OPEN_LNBF::load(data, p);
            if (p != cw->pages) {
                QMessageBox message(QMessageBox::Question, tr("Tip"), tr("You will exit!") + "\n" + tr("Before that, would you like to save the blackboard file?"),QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,this);
                message.setDefaultButton(QMessageBox::Cancel);
                message.button(QMessageBox::Yes)->setText(tr("Yes"));
                message.button(QMessageBox::No)->setText(tr("No"));
                message.button(QMessageBox::Cancel)->setText(tr("Cancel"));
                // ////////////////////
                int result = message.exec();
                switch(result){
                case QMessageBox::Yes: {
                    QFile file(lnbf_path); // 指定要写入的文件名
                    if (file.open(QIODevice::WriteOnly)) {
                        QTextStream out(&file); // 创建数据流对象
                        out << SAVE_OPEN_LNBF::save(cw->pages).toLocal8Bit(); // 序列化到文件中
                        file.close();
                    } else {
                        // 错误处理
                    }
                }
                case QMessageBox::No: {
                    this->cw->close();
                    this->close();
                    break;
                }
                }
            } else {
                this->cw->close();
                this->close();
            }
        } else if (cw->pages != Pages{{}}){
            QMessageBox message(QMessageBox::Question, tr("Tip"), tr("You will exit!") + "\n" + tr("Before that, would you like to save the blackboard file?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,this);
            message.setDefaultButton(QMessageBox::Cancel);
            message.button(QMessageBox::Yes)->setText(tr("Yes"));
            message.button(QMessageBox::No)->setText(tr("No"));
            message.button(QMessageBox::Cancel)->setText(tr("Cancel"));
            // ////////////////////
            int result = message.exec();
            switch(result){
            case QMessageBox::Yes: {
                QFileDialog fileDialog;
                QString path = fileDialog.getSaveFileName(this, tr("Save To"), ".", tr("LemonxNote Blackboard File") + "(*.lnbf)");
                if (path=="") { return; }
                QFile file(path); // 指定要写入的文件名
                if (file.open(QIODevice::WriteOnly)) {
                    file.write(SAVE_OPEN_LNBF::save(cw->pages).toLocal8Bit());
                    file.close();
                } else {
                    // 错误处理
                }
            }
            case QMessageBox::No: {
                this->cw->close();
                this->close();
                break;
            }
            }
        } else {
            this->cw->close();
            this->close();
        }
        });

    connect(ui->next_page, &QPushButton::clicked, [&]{
        cw->history<<QPair<int, Pages>{cw->page,cw->pages};
        emit cw->OnDrawCompleted();
        if(cw->pages.size()==cw->page+1) {  // 加页
            cw->pages<< QPair<QPoint, QList<QPair<QColor, QList<QPoint>>>> {QPoint(0,0), {{drawColor, QList<QPoint>() }}};
            cw->setPage(cw->page+1);
            cw->update();
            OnPageChanges();
        } else {      // 直接下一页
            cw->setPage(cw->page+1);
            cw->update();
            OnPageChanges();
        }
        qDebug()<<"Page: "<<cw->page<<", Total Page: "<<cw->pages.size();
    });

    connect(ui->back_page, &QPushButton::clicked, [&]{
        if(cw->page!=0) {      // 直接上一页
            cw->history<<QPair<int, Pages>{cw->page,cw->pages};
            emit cw->OnDrawCompleted();
            cw->setPage(cw->page-1);
            OnPageChanges();
            qDebug()<<"Page: "<<cw->page<<", Total Page: "<<cw->pages.size();
        }
    });

    connect(ui->undo, &QPushButton::clicked, [&]{
        if(cw->history.size()>0) {
            qDebug()<<"-Page: "<<cw->page<<", Total Page: "<<cw->pages.size();
            auto one_page = cw->history.takeAt(cw->history.size()-1);
            cw->page = one_page.first;
            cw->pages = one_page.second;
            cw->update();
            OnPageChanges();
            OnDrawCompleted();
        }
    });

    connect(ui->open_file, &QPushButton::clicked, [&]{
        QString file_name = QFileDialog::getOpenFileName(this, tr("Selete File"), ".", tr("LemonxNote Blackboard File")+"(*.lnbf)");
        if (file_name=="") { return; }
        if (cw->pages != Pages{{}}) {
            // Ask the user
            QMessageBox message(QMessageBox::Question, tr("Tip"), tr("If you open the file, this blackboard will be lost!"),QMessageBox::Yes | QMessageBox::No,this);
            message.setDefaultButton(QMessageBox::Cancel);
            message.button(QMessageBox::Yes)->setText(tr("Still"));
            message.button(QMessageBox::No)->setText(tr("Cancel"));
            int result = message.exec();
            if (result == QMessageBox::No) { return; }
        }
        QFile file(file_name);
        if (file.open(QIODevice::ReadOnly)) {
            try{
                QTextStream in(&file);
                QString data;
                in>>data;
                //qDebug()<<data;
                cw->page = 0;
                SAVE_OPEN_LNBF::load(data, cw->pages);
            } catch(char*err) {
                QMessageBox::critical(this, tr("Error"), tr("This file was broken!"));
                file.close();
                return;
            }
        } else {
            // 错误处理
            QMessageBox::critical(this, tr("Error"), tr("Couldn't open the file!"));
            file.close();
            return;
        }
        lnbf_path = file_name;
        cw->history = {};
        cw->update();
        OnPageChanges();
        OnDrawCompleted();
    });

    connect(ui->save_file, &QPushButton::clicked, [&]{
        if (lnbf_path != "") {
            QFile File(lnbf_path); // 指定要写入的文件名
            if (File.open(QIODevice::WriteOnly)) {
                File.write(SAVE_OPEN_LNBF::save(cw->pages).toLocal8Bit());
                File.close();
                OnDrawCompleted();
            } else {
                // 错误处理
            }
        } else if (cw->pages != Pages {{}}){
            QFileDialog fileDialog;
            QString path = fileDialog.getSaveFileName(this, tr("Save To"), ".", tr("LemonxNote Blackboard File") + "(*.lnbf)");
            if (path=="") { return; }
            QFile File(path);
            File.open(QFile::WriteOnly|QFile::Truncate);
            File.write(SAVE_OPEN_LNBF::save(cw->pages).toLocal8Bit());
            File.close();
            lnbf_path = path;
            OnDrawCompleted();
        }
    });

    connect(ui->settingButton, &QPushButton::clicked, [&]{
        QMessageBox *msg = new QMessageBox(this);
        msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Yes);
        msg->setIcon(QMessageBox::Information);
        msg->setWindowTitle(tr("About"));
        msg->setText(QString(tr("LemonxNote Version: ") + "v%1\n" +
                             tr("Is Qt supported by OpenSSL? ") + " %2\n" +
                             tr("Required OpenSSL version: ") + "%3\n" +
                             tr("Memory footprint: ") + "%4").arg(version)
                                                             .arg(QSslSocket::supportsSsl())
                                                             .arg(QSslSocket::sslLibraryBuildVersionString())
                                                             .arg(format_bytes(cw->getPagesSize())));
        msg->button(QMessageBox::Yes)->setText(tr("Yes"));
        msg->button(QMessageBox::Ok)->setText("  " + tr("Set Language") + "  ");
        QCheckBox *cb = new QCheckBox(msg);
        cb->setCheckState(Qt::Unchecked);
        cb->setText(tr("About") + " Qt");
        cb->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMenuButton));
        cb->setStyleSheet("QCheckBox{padding-left:-16px;}QCheckBox::indicator:unchecked{image:null;}QCheckBox::indicator:checked{image:null;}");
        connect(cb, &QCheckBox::clicked, [&]{
            msg->setWindowOpacity(0);
            cb->setCheckState(Qt::Unchecked);
            QMessageBox::aboutQt(msg, tr("About") + " Qt");
            msg->setWindowOpacity(1);
        });
        msg->setCheckBox(cb);
        if (msg->exec()==QMessageBox::Ok) {
            // Languages
            QDialog *dialog=new QDialog(this);
            //当主窗口不关闭，即不进行析构时，多次打开关闭对话框会导致内存泄漏
            dialog->setAttribute(Qt::WA_DeleteOnClose);//防止内存泄漏
            dialog->setWindowTitle(tr("Set Language"));
            dialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
            dialog->resize(250, 50);
            QGridLayout *gridLayout = new QGridLayout(dialog);
            QComboBox *qcb = new QComboBox(dialog);
            qcb->setMinimumSize(120, 20);
            qcb->addItems({"简体中文","English","Русский","한국어","Français"});
            qcb->setCurrentIndex(setting->value("language", QVariant(1)).toInt());
            connect(qcb, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](){
                setting->setValue("language", qcb->currentIndex());
                setLanguage(qcb->currentIndex());
                dialog->setWindowTitle(tr("Set Language"));
                ui->retranslateUi(this);
            });
            gridLayout->addWidget(qcb,0,0,1,1);
            dialog->exec();
        }
    });

    cw->show();

    this->Pen();



    connect(this->nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(checkUpdateWithData(QNetworkReply*)));
    nam->get(QNetworkRequest(QUrl("https://raw.gitcode.com/weixin_61221827/lemonx-note-api/raw/main/updateapi.json")));

    setLanguage(setting->value("language", QVariant(1)).toInt());
};

void MainWindow::setLanguage(int lang){
    qApp->removeTranslator(m_pTranslator);
    m_pTranslator = new QTranslator;
    switch (lang) {
    case 0:
        m_pTranslator->load(":/languages/langChinese.qm");
        break;
    case 1:
        // 默认就是英语, 不用操作
        break;
    case 2:
        m_pTranslator->load(":/languages/langRussian.qm");
        break;
    case 3:
        m_pTranslator->load(":/languages/langKorean.qm");
        break;
    case 4:
        m_pTranslator->load(":/languages/langFrench.qm");
        break;
    }
    qApp->installTranslator(m_pTranslator);
}


void MainWindow::closeEvent(QCloseEvent* e) {
    e->accept();
    if(isUpdateDownloaded && update_exe_path!="" && install_bat_script_path!=""){
        QProcess process(this);
        process.startDetached(install_bat_script_path);
    }
}

void MainWindow::checkUpdateWithData(QNetworkReply* rep) {
    QString json = rep->readAll();
    QJsonParseError jsonError;
    // 将json解析为UTF-8编码的json文档，并从中创建一个QJsonDocument。
    // 如果解析成功，返回QJsonDocument对象，否则返回null
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &jsonError);
    // 判断是否解析失败
    if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
        qDebug() << "Error! Cannot check update! " << jsonError.error;
        return;
    }
    QJsonObject obj = doc.object();
    QString url = obj.value("download").toString(), newest_version = obj.value("version").toString();
    qDebug()<<"url = "<< url <<", newest version = "<< newest_version;
    //QMessageBox::about(this, "1", "url = "+ url +", newest version = "+ newest_version);

    bool isUpdateRequired = compareVersions(newest_version, version)/* 1: 需要更新; 0或-1: 不需要更新 */ == 1;

    if(isUpdateRequired) {
        // 下载更新
        connect(this->dl_nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(OnDownloadCompleted(QNetworkReply*)));
        QNetworkRequest request = QNetworkRequest(QUrl(url));
        request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3");
        dl_nam->get(request);
    }
}

bool mkdir(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists()) return true;
    else return dir.mkdir(fullPath);
}

void MainWindow::OnDownloadCompleted(QNetworkReply* rep) {
    if (rep->error()) { qDebug()<<"Download failed!"; return; }
    qDebug()<<"The download was successful.";
    QDir temp_path(QDir::tempPath());
    temp_path = QDir(temp_path.path() + "/LemonxNoteUpdate");
    if(!mkdir(temp_path.path())) { qDebug()<<temp_path.path()<<" Cloudn't create dir!"; return; }
    update_exe_path = temp_path.path() + "/update.exe";
    QFile f(update_exe_path);
    f.open(QIODevice::ReadWrite);
    QByteArray data = rep->readAll();
    f.write(data);
    qDebug() << "Write " << data.size() << " bytes";
    f.close();
    install_bat_script_path = temp_path.path() + "/install_update.bat";
    QFile install_bat(install_bat_script_path);
    install_bat.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream out(&install_bat);
    out<<"@echo off"
      <<"\n"
      <<"chcp 65001"
      <<"\n"
      <<QString("\"%1\" /VERYSILENT \"/DIR=%2\"").arg(update_exe_path).arg(QDir::currentPath())
      <<"\n"
      <<QString("del \"./update.exe\" /f /q").arg(update_exe_path)
      <<"\n"
      <<QString("del \"./install_update.bat\" /f /q").arg(install_bat_script_path)
      <<"\n"
      <<"exit";
    rep->deleteLater();
    isUpdateDownloaded = true;
}


void MainWindow::onSetToolbarPositon(int left_or_right){
    // left_or_right: 1: left
    //                2: right
    QScreen* primaryScreen = QGuiApplication::primaryScreen();
    // 获取屏幕的分辨率
    QRect screenGeometry = primaryScreen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    // 设置位置
    if (left_or_right==1) {
        this->resize(this->width(), screenHeight);
        this->move(0, 0);
    } else if (left_or_right == 2) {
        this->resize(this->width(), screenHeight);
        this->move(screenWidth - this->width(), 0);
    }
}

void MainWindow::OnDrawCompleted(){
    cw->update();
    //qDebug()<<"Page histories: "<<cw->history.size();
    ui->undo->setIcon(QIcon(":/undo.png"));
    ui->save_file->setIcon(QIcon(":/save.png"));
    QString qss = "background-color: rgba(0, 0, 0, 50);"
            "border-radius: 35px;"
            "padding: 5px;";
    ui->undo->setStyleSheet(qss);
    ui->save_file->setStyleSheet(qss);
    if(cw->history.size()>0){
        tintButtonBackground(ui->undo, QColor(COLOR, COLOR, COLOR), QSize(25,25));
    } else {
        tintButtonBackground(ui->undo, QColor(COLOR, COLOR, COLOR, 30), QSize(25,25));
    }
    if (lnbf_path=="") {
        if (cw->pages != Pages {{}}){

        } else {
            tintButtonBackground(ui->save_file, QColor(COLOR, COLOR, COLOR, 30), QSize(30,30));
        }
    } else {
        QFile file(lnbf_path);
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        QString data;
        in>>data;
        Pages p;
        SAVE_OPEN_LNBF::load(data, p);
        if (p == cw->pages) {
            tintButtonBackground(ui->save_file, QColor(COLOR, COLOR, COLOR, 30), QSize(30,30));
        } else {
            tintButtonBackground(ui->save_file, QColor(COLOR, COLOR, COLOR), QSize(30,30));
        }
    }
    if(cw->history.size()>50){
        cw->history.takeAt(0);
    }
    if(cw->history.size()>1 && cw->getPagesSize()>1024*1024*20){
        cw->history.takeAt(0);
    }
}
void MainWindow::OnPageChanges(){
    ui->page->setText(QString("%1/%2").arg(cw->page+1).arg(cw->pages.size()));
    ui->next_page->setIcon(QIcon(":/next_page.png"));
    ui->back_page->setIcon(QIcon(":/back_page.png"));
    QString qss = "background-color: rgba(0, 0, 0, 50);"
            "border-radius: 35px;"
            "padding: 5px;";
    ui->next_page->setStyleSheet(qss);
    ui->back_page->setStyleSheet(qss);
    if(cw->page!=0){
        tintButtonBackground(ui->next_page, QColor(COLOR, COLOR, COLOR), QSize(35, 35));
        tintButtonBackground(ui->back_page, QColor(COLOR, COLOR, COLOR), QSize(35, 35));
    } else {
        tintButtonBackground(ui->next_page, QColor(COLOR, COLOR, COLOR), QSize(35, 35));
        tintButtonBackground(ui->back_page, QColor(COLOR, COLOR, COLOR, 30), QSize(35, 35));
    }

    if(cw->page+1==cw->pages.size()){
        ui->next_page->setIcon(QIcon(":/new_page.png"));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::OnModeChange() {
    if (Mode != 0) {
    } else {
    }
}

void MainWindow::InitIcons(){
    ui->point->setIcon(QIcon(":/point.png"));
    ui->pen->setIcon(QIcon(":/pen.png"));
    ui->eraser->setIcon(QIcon(":/eraser.png"));
    ui->next_page->setIcon(QIcon(":/next_page.png"));
    ui->back_page->setIcon(QIcon(":/back_page.png"));
    ui->quitButton->setIcon(QIcon(":/quit.png"));
    ui->undo->setIcon(QIcon(":/undo.png"));
    ui->open_file->setIcon(QIcon(":/file.png"));
    ui->save_file->setIcon(QIcon(":/save.png"));
    ui->settingButton->setIcon(QIcon(":/setting.png"));
    QString qss = "background-color: rgba(0, 0, 0, 50);"
            "border-radius: 35px;"
            "padding: 5px;";
    ui->point->setStyleSheet(qss);
    ui->pen->setStyleSheet(qss);
    ui->eraser->setStyleSheet(qss);
    ui->quitButton->setStyleSheet(qss);
    ui->next_page->setStyleSheet(qss);
    ui->back_page->setStyleSheet(qss);
    ui->undo->setStyleSheet(qss);
    ui->open_file->setStyleSheet(qss);
    ui->save_file->setStyleSheet(qss);
    ui->settingButton->setStyleSheet(qss);
    ui->page->setStyleSheet(qss + "color: #fff; font: 15pt \"Consolas\";");
    //tintButtonBackground(ui->next_page, QColor(COLOR, COLOR, COLOR), QSize(35, 35));
    //tintButtonBackground(ui->back_page, QColor(COLOR, COLOR, COLOR, 30), QSize(35, 35));
    tintButtonBackground(ui->quitButton, QColor(255, 0, 0), QSize(25,25));
    tintButtonBackground(ui->open_file, QColor(COLOR, COLOR, COLOR), QSize(30,30));
    tintButtonBackground(ui->save_file, QColor(COLOR, COLOR, COLOR, 30), QSize(30,30));
    tintButtonBackground(ui->settingButton, QColor(COLOR, COLOR, COLOR), QSize(30,30));
    OnPageChanges();
    OnDrawCompleted();
}


void MainWindow::Point()
{
    this->InitIcons();
    tintButtonBackground(ui->point, QColor(COLOR, COLOR, COLOR));
    tintButtonBackground(ui->pen, QColor(COLOR, COLOR, COLOR, ALPHA));
    tintButtonBackground(ui->eraser, QColor(COLOR, COLOR, COLOR, ALPHA));
    cw->Mode = Mode = 0;
    OnModeChange();
}

void MainWindow::Pen()
{
    this->InitIcons();
    tintButtonBackground(ui->point, QColor(COLOR, COLOR, COLOR, ALPHA));
    tintButtonBackground(ui->pen, this->drawColor);
    tintButtonBackground(ui->eraser, QColor(COLOR, COLOR, COLOR, ALPHA));
    cw->Mode = Mode = 1;
    OnModeChange();
}

void MainWindow::Eraser()
{
    this->InitIcons();
    tintButtonBackground(ui->point, QColor(COLOR, COLOR, COLOR, ALPHA));
    tintButtonBackground(ui->pen, QColor(COLOR, COLOR, COLOR, ALPHA));
    tintButtonBackground(ui->eraser, QColor(COLOR, COLOR, COLOR));
    cw->Mode = Mode = 2;
    OnModeChange();
}


int compareVersions(const QString& v1, const QString& v2)
{
    /* 比较版本号 */
    QVersionNumber version1 = QVersionNumber::fromString(v1);
    QVersionNumber version2 = QVersionNumber::fromString(v2);

    // 首先比较主版本号
    if (version1.majorVersion() < version2.majorVersion())
        return -1;
    else if (version1.majorVersion() > version2.majorVersion())
        return 1;

    // 如果主版本号相同，则比较次版本号
    if (version1.minorVersion() < version2.minorVersion())
        return -1;
    else if (version1.minorVersion() > version2.minorVersion())
        return 1;

    // 如果次版本号也相同，则比较补丁版本号
    if (version1.microVersion() < version2.microVersion())
        return -1;
    else if (version1.microVersion() > version2.microVersion())
        return 1;

    return 0; // 版本号完全相同
}


void tintButtonBackground(QPushButton *button, const QColor &color, const QSize &padding)
{
    if (!button) return;

    // 获取按钮的尺寸
    QSize buttonSize = button->size();

    // 加载按钮当前的图标（如果有的话）到QPixmap
    QIcon buttonIcon = button->icon();
    QPixmap pixmap = buttonIcon.pixmap(buttonSize);

    if (pixmap.isNull()) {
        return;
    }

    // 使用 QPainter 对象来修改pixmap，首先按比例缩放且保持图像质量
    QPixmap scaledPixmap = pixmap.scaled(buttonSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPainter painter(&scaledPixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(scaledPixmap.rect(), color);
    painter.end();

    // 将着色且按比例缩放后的图片设置为按钮的图标
    QIcon coloredIcon(scaledPixmap);
    button->setIcon(coloredIcon);
    button->setIconSize(buttonSize - padding); // 确保图标大小与按钮匹配，尽管按比例缩放，但仍设置以覆盖所有情况

}
