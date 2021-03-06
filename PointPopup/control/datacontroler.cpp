/***********************************************************************
*PointDownload
*Copyright (C) 2014  PointTeam
*
* Author:     Match <2696627729@qq.com>
* Maintainer: Match <2696627729@qq.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/


#include "datacontroler.h"
#include <QtQml>

DataControler::DataControler(QObject *parent) :
    QObject(parent)
{
    //import时使用Singleton.DataControler，在获取内容或调用函数时使用PEventFilter
    qmlRegisterSingletonType<DataControler>("Singleton.DataControler", 1, 0, "DataControler", dataObj);

    startMainProgram();

    fileURL = "";
    fileNameList = "";
    fileSavePath = gXMLOpera.getMainConfig().linuxSavePath;
    freeSpace = getLinuxFreeSpace(fileSavePath);
    maxThread = gXMLOpera.getMainConfig().defaultThreadCount;
    maxSpeed = gXMLOpera.getMainConfig().downloadSpeed;

    toolsType = "aria2";

    yougetProcess = NULL;
}

//保证单例对象先于qml组件之前构建
DataControler * DataControler::dataControler = new DataControler();
DataControler * DataControler::getInstance()
{
    return dataControler;
}

void DataControler::selectSavePath(QString buttonName)
{
    if (buttonName == "Default")
    {
        setFileSavePath(gXMLOpera.getMainConfig().linuxSavePath);
        setFreeSpace(getLinuxFreeSpace(fileSavePath));
    }
    else if (buttonName == "Desktop")
    {
        setFileSavePath(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).at(0));
        setFreeSpace(getLinuxFreeSpace(fileSavePath));
    }
    else
    {
        QString tmpPath =  QFileDialog::getExistingDirectory(0,
                                                          tr("Open Folder"),
                                                          gXMLOpera.getMainConfig().linuxSavePath,
                                                          QFileDialog::ShowDirsOnly);
        if (tmpPath != "")
        {
            setFileSavePath(tmpPath);
            setFreeSpace(getLinuxFreeSpace(fileSavePath));
        }
    }
}

void DataControler::sendToMainServer(QString threads, QString speed, QString savePath)
{
    if (checkIsInDownloading(fileURL))
    {
        return;//no need to do anything
    }
    else if (checkIsInDownloaded(fileURL))
    {
        //delete xml item
        QFile::remove(gXMLOpera.getDownloadedNode(fileURL).savePath + "/"
                      + gXMLOpera.getDownloadedNode(fileURL).name);
        gXMLOpera.removeDownloadedFileNode(fileURL);

    }
    else if (checkIsInDownloadTrash(fileURL))
    {
        gXMLOpera.removeDownloadTrashFileNode(fileURL);
    }

    //info: toolsType?:?fileNameList?:?URL?:?RedirectURL?:?iconName?:?savePath?:?threadCount?:?maxSpeed
    QString info = toolsType + "?:?"
            + fileNameList + "?:?"
            + fileURL + "?:?"
            + redirectURL + "?:?"
            + "qrc:/images/right/filetype/" +getIconName() + "?:?"
            + savePath + "?:?"
            + threads + "?:?"
            + speed;

    localSocket->write(info.toStdString().c_str());
    localSocket->flush();

    qApp->quit();
}

void DataControler::getURLFromBrowser(QString URLInfo)
{
    if (URLInfo.contains("?:?"))
    {
        setFileURL(URLInfo.split("?:?").at(0));
        redirectURL = fileURL;
        toolsType = URLInfo.split("?:?").at(1);

        if(toolsType == "youget")
        {
            QTimer::singleShot(100,this,SLOT(getURLInfoFromYouget()));
        }
        else
        {
            //此处会导致警告：QString::arg: Argument missing: 无法解析res_nclose中的符号“res_nclose”：libresolv.so.2,
            //获取http、ftp、Bt等类型的信息
            //延迟时间，防止qml组件未初始化前就发送了信号
            QTimer::singleShot(100,this,SLOT(getURLInfo()));
        }
    }
}

QString DataControler::getFileURL()
{
    return fileURL;
}

QString DataControler::getFileNameList()
{
    return fileNameList;
}

QString DataControler::getMaxSpeed()
{
    return maxSpeed;
}

QString DataControler::getFreeSpace()
{
    return freeSpace;
}

QString DataControler::getFileSavePath()
{
    return fileSavePath;
}

QString DataControler::getMaxThread()
{
    return maxThread;
}

void DataControler::setFileURL(QString URL)
{
    fileURL = URL;
    emit sFileURLChange();
}

void DataControler::setFileNameList(QString nameList)
{
    fileNameList = nameList;
    emit sFileNameListChange();
}

void DataControler::setFileSavePath(QString path)
{
    fileSavePath = path;
    emit sFileSavePathChange();
}

void DataControler::setMaxSpeed(QString speed)
{
    maxSpeed = speed;
    emit sMaxSpeedChange();
}

void DataControler::setMaxThread(QString count)
{
    maxThread = count;
    emit sMaxThreadChange();
}

void DataControler::setFreeSpace(QString space)
{
    freeSpace = space;
    emit sFreeSpaceChange();
}


QString DataControler::getLinuxFreeSpace(QString path)
{
    struct statfs lv_fs;

    if(statfs(path.toUtf8().data(),&lv_fs) < 0)
    {
        return "";
    }
    else
    {
        double freeSize = lv_fs.f_bfree * lv_fs.f_bsize; // Byte
        if (freeSize > 1024 * 1024 * 1024)//GB
            return QString::number(freeSize / 1024 / 1024 / 1024,'f',1) + "GB";
        else if (freeSize > 1024 * 1024)//MB
            return QString::number(freeSize / 1024 / 1024,'f',1) + "MB";
        else if (freeSize > 1024)//KB
            return QString::number(freeSize/ 1024,'f',1) + "KB";
        else
            return QString::number(freeSize,'f',1) + "B";
    }
}

void DataControler::getURLInfoFromYouget()
{

    //该指针指向另外一个被启动的程序，所以 绝对不能被delete
    yougetProcess = new QProcess(this);
    connect(yougetProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(getYougetFeedBack()));
    connect(yougetProcess,SIGNAL(readyReadStandardError()),
            this,SLOT(getYougetError()));

    QStringList arguments;
    arguments << "-i";
    arguments << fileURL;
    yougetProcess->start("you-get",arguments);

}

void DataControler::getURLInfo()
{
    if (fileURL.contains("http://") || fileURL.contains("https"))
    {
        //必须要执行getHttpFileTypeSize函数，保证如果有重定向可以根据重定向后再去取名字
        QString tmpNameList = getHttpFileTypeSize(fileURL) + QString("@");
        QString tmpName =  getHttpFtpFileName(redirectURL);
        tmpNameList += tmpName;

        setFileNameList(tmpNameList);
        emit sFnishGetAllInfo();
    }
    else if(fileURL.contains("ftp://"))
    {
        //必须要执行getHttpFileTypeSize函数，保证如果有重定向可以根据重定向后再去取名字
        QString tmpNameList = getFtpFileTypeSize(fileURL) + "@" + getHttpFtpFileName(redirectURL);
        setFileNameList(tmpNameList);
        emit sFnishGetAllInfo();
    }
    else if (fileURL.contains(".torrent"))
    {

    }
}

void DataControler::getYougetFeedBack()
{
    QString outPut = QString(yougetProcess->readAllStandardOutput());
     int titleIndex = outPut.indexOf("Title:");
     int typeIndex = outPut.indexOf("Type:");
     int sizeIndex = outPut.indexOf("Size:");
     QString tmpTitle = outPut.mid(titleIndex + 12,typeIndex - titleIndex - 13);
     fileType = outPut.mid(outPut.indexOf("(") + 1,outPut.indexOf(")") - outPut.indexOf("(") - 1);
     QString tmpSize = outPut.mid(sizeIndex + 12,outPut.lastIndexOf("(") - sizeIndex - 17);

     //youget返回大小都以MiB计算,要转换成B
     tmpSize = QString::number(qint64(tmpSize.toDouble() * 1024 * 1024));

     setFileNameList(fileType + "@" + tmpSize + "@" + tmpTitle);
     emit sFnishGetAllInfo();
}

void DataControler::getYougetError()
{
    qDebug() << "++++++++++++++++++++++++++++++++++++++++++++++++++";
    qDebug() << yougetProcess->readAllStandardError();
    qDebug() << "++++++++++++++++++++++++++++++++++++++++++++++++++";
}

bool isConnected = false;
void DataControler::mainProgramStarted()
{
    //要保证主程序启动才连接到主程序
    if (!isConnected)
    {
        localSocket = new QLocalSocket();
        connectToMainProgram();
        isConnected = true;
    }
}

QString DataControler::getHttpFileTypeSize(QString URL)
{
    qint64 totalSize = -1;
    fileType = "";
    QNetworkAccessManager * tmpManager = new QNetworkAccessManager;
    QNetworkRequest headReq(URL);
    headReq.setRawHeader("User-Agent", "");  //Content-Length

    QNetworkReply*  headReply = NULL;
    bool connectError = false;
    int tryTimes = 1;
    //如果失败,连接尝试1次;
    do{
        connectError = false;
        headReply =  tmpManager->head(headReq);
        if(!headReply)
        {
            connectError = true;
            continue;
        }
        QEventLoop loop;
        connect(headReply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        connectError = (headReply->error() != QNetworkReply::NoError);
        if(connectError)
        {
            qDebug()<<"connect failed!";
            qDebug() << headReply->errorString();
        }
        headReply->deleteLater();
    } while (connectError && --tryTimes);

    int statusCode = headReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

//    qDebug() << statusCode;
    if(statusCode == 302)
    {
        QUrl newUrl = headReply->header(QNetworkRequest::LocationHeader).toUrl();
        if(newUrl.isValid())
        {
//            qDebug()<<"Redirect："<<newUrl;
            URL = newUrl.toString();
            redirectURL = URL;
            return getHttpFileTypeSize(URL);
        }
    }

    else
    {
        totalSize = headReply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
        fileType =  headReply->header(QNetworkRequest::ContentTypeHeader).toString();
    }
    headReply->deleteLater();
    delete tmpManager;

    return fileType + "@" + QString::number(totalSize);

}

QString DataControler::getFtpFileTypeSize(QString URL)
{
    return "unknown@unknown";
}

QString DataControler::getIconName()
{
    QString headerString = fileType.mid(0, fileType.indexOf("/"));
    QString behindString = fileType.mid(fileType.indexOf("/") + 1, fileType.length());

    QString iconName  = "noicon";
    if (headerString == "application")
    {
        if (behindString.contains("pdf"))
        {
            //MIME类型@资源文件别名
            iconName = "pdf";
        }
        else if (behindString.contains("msword"))
            iconName = "ms-doc";
        else if (behindString.contains("excel"))
            iconName = "ms-xls";
        else if (behindString.contains("powerpoint"))
            iconName = "ms-ppt";
        else if (behindString.contains("octet-stream"))
            iconName = "binary";
        else if (behindString.contains("x-") ||
                 behindString.contains("zip") ||
                 behindString.contains("tar") ||
                 behindString.contains("rar"))
            iconName = "archive";
        else
            iconName = "noicon";
    }
    else if (behindString.contains("adobe"))
        iconName = "adobe";
    else if (headerString == "image")
        iconName = "image";
    else if (headerString == "audio")
        iconName = "audio";
    else if (headerString == "video")
        iconName = "video";
    else if (headerString == "text" && behindString == "plain")
        iconName = "text";
    else
        iconName = "noicon";

    return iconName;
}

QString DataControler::getHttpFtpFileName(QString URL)
{
    QRegExp rx;
    rx.setPatternSyntax(QRegExp::RegExp);
    rx.setCaseSensitivity(Qt::CaseSensitive); //大小写敏感
    rx.setPattern(QString("[^/]*$"));
    int pos = URL.indexOf(rx);

    if ( pos >= 0 )
    {
        QString tmpStr = rx.capturedTexts().at(0);
        rx.setPattern(QString("\.[^?@#$%&]+"));
        tmpStr.indexOf(rx);
        QString tmpName = rx.capturedTexts().at(0);

        if (tmpName == "")
            return "UnknownName";
        else
            return tmpName;
    }
    else
        return "";
}


void DataControler::startMainProgram()
{
    //每次启动前先尝试启动主程序
    QObject * mparent;
    QStringList arguments;
    arguments << "-c";

    //该指针指向另外一个被启动的程序，所以 绝对不能被delete
    QProcess * myProcess = new QProcess(mparent);
    myProcess->start(MAIN_PROGRAM_PATH,arguments);
    mainProgramStarted();
//    connect(myProcess,SIGNAL(started()),this,SLOT(mainProgramStarted()));
}

void DataControler::connectToMainProgram()
{
    // 服务端的serverNewConnectionHandler成员方法将被调用
    localSocket->connectToServer("PointURLServer");
    if (localSocket->waitForConnected())
    {
        qDebug() << "connect to main program success";
        return;
        // TODO:
    }
    else
    {
        //不成功就循环地尝试连接，这里要保证此函数在主程序启动后再调用
        connectToMainProgram();
    }
}

bool DataControler::checkIsInDownloading(QString URL)
{
    QList<SDownloading> tmpList = gXMLOpera.getDownloadingNodes();

    for (int i = 0; i < tmpList.count(); i ++)
    {
        if (tmpList.at(i).URL == URL)//if exit,when user click ok button ,this window will just close and do nothing
        {
            return true;
        }
    }

    return false;//at the end got nothing will return false
}

bool DataControler::checkIsInDownloaded(QString URL)
{
    QList<SDownloaded> tmpList = gXMLOpera.getDownloadedNodes();

    for (int i = 0; i < tmpList.count(); i ++)
    {
        if (tmpList.at(i).URL == URL)//if exit,when user click ok button ,this window will just close and do nothing
        {
            return true;
        }
    }

    return false;//at the end got nothing will return false
}

bool DataControler::checkIsInDownloadTrash(QString URL)
{
    QList<SDownloadTrash> tmpList = gXMLOpera.getDownloadTrashNodes();

    for (int i = 0; i < tmpList.count(); i ++)
    {
        if (tmpList.at(i).URL == URL)//if exit,when user click ok button ,this window will just close and do nothing
        {
            return true;
        }
    }

    return false;//at the end got nothing will return false
}










