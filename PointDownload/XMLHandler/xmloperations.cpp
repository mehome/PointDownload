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

#include "xmloperations.h"

XMLOperations::XMLOperations(QObject *parent) :
    QObject(parent)
{
    touchAll();
}

void XMLOperations::touchAll()
{
    touchConfigDir();
    touchDownloadDir();
    touchMainConfigFile();
    touchDownloadedConfigFile();
    touchDownloadingConfigFile();
    touchDownloadTrashConfigFile();
}

void XMLOperations::touchConfigDir()
{
    QDir localConfigDir;

    //如果路径不存在,则先新建文件夹,通常只会执行一次,即程序初次运行
    if (!localConfigDir.exists(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0) + "/.PointConfig"))
    {
        localConfigDir.mkpath(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0) + "/.PointConfig");
    }
}

void XMLOperations::touchDownloadDir()
{
    QDir localConfigDir;

    //如果路径不存在,则先新建文件夹,通常只会执行一次,即程序初次运行
    if (!localConfigDir.exists(QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).at(0) + "/PointDownload"))
    {
        localConfigDir.mkpath(QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).at(0) + "/PointDownload");
    }
}

void XMLOperations::touchMainConfigFile()
{
    QFile mainConfigFile(MAINCONFIGFILE_PATH);
    if (!mainConfigFile.exists())
    {
        if (!mainConfigFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return;
        }

        QDomDocument domDocument;

        QDomProcessingInstruction instruction;
        instruction=domDocument.createProcessingInstruction("xml","version=\'1.0\' encoding=\'UTF-8\'");

        domDocument.appendChild(instruction);

        //创建MainConfig根节点
        QDomElement mainConfigNode = domDocument.createElement("MainConfig");

        //将各个子节点添加到@mainConfigNode节点上
#ifdef Q_OS_LINUX
        mainConfigNode.appendChild(createChildElement("OperatingSystem", "Linux"));
#elif Q_OS_WIN
        mainConfigNode.appendChild(createChildElement("OperatingSystem", "Windows"));
#else
        mainConfigNode.appendChild(createChildElement("OperatingSystem", "UnSupport"));
#endif
        mainConfigNode.appendChild(createChildElement("Version", "1.0.0"));
        mainConfigNode.appendChild(createChildElement("DownloadSpeed", "2000"));
        mainConfigNode.appendChild(createChildElement("UploadSpeed", "500"));
        mainConfigNode.appendChild(createChildElement("WindowsSavePath",
                                                      QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).at(0) + "/PointDownload"));
        mainConfigNode.appendChild(createChildElement("LinuxSavePath",
                                                      QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).at(0) + "/PointDownload"));
        mainConfigNode.appendChild(createChildElement("Beep", "true"));
        mainConfigNode.appendChild(createChildElement("EnableUpload", "true"));
        mainConfigNode.appendChild(createChildElement("ThreadCount", "5"));
        mainConfigNode.appendChild(createChildElement("VideoDetectType", "mov;mkv;swf;flv;mp4;avi;rmvb;rm;3gp"));
        mainConfigNode.appendChild(createChildElement("AudioDetectType", "mp3;wma;flac;ape;wav;acc"));
        mainConfigNode.appendChild(createChildElement("MaxJobCount", "10"));
        mainConfigNode.appendChild(createChildElement("clipboard", "true"));
        mainConfigNode.appendChild(createChildElement("exitOnClose", "false"));
        mainConfigNode.appendChild(createChildElement("aria2Path", ""));
        mainConfigNode.appendChild(createChildElement("yougetPath", ""));

        //添加元素节点到父节点
        domDocument.appendChild(mainConfigNode);

        //写xml文件
        QTextStream textStream(&mainConfigFile);

        domDocument.save(textStream,4);

        mainConfigFile.close();
    }
}

void XMLOperations::touchDownloadedConfigFile()
{
    QFile downloadedFile(DOWNLOADEDFILE_PATH);
    if (!downloadedFile.exists())
    {
        if (!downloadedFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return;
        }

        QDomDocument domDocument;

        QDomProcessingInstruction instruction;
        instruction=domDocument.createProcessingInstruction("xml","version=\'1.0\' encoding=\'UTF-8\'");

        domDocument.appendChild(instruction);

        //创建MainConfig根节点
        QDomElement downloadedNode = domDocument.createElement("DownloadedList");
        //添加元素节点到父节点
        domDocument.appendChild(downloadedNode);

        //写xml文件
        QTextStream textStream(&downloadedFile);

        domDocument.save(textStream,4);

        downloadedFile.close();
    }
}

void XMLOperations::touchDownloadingConfigFile()
{
    QFile downloadingFile(DOWNLOADINGFILE_PATH);
    if (!downloadingFile.exists())
    {
        if (!downloadingFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return;
        }

        QDomDocument domDocument;

        QDomProcessingInstruction instruction;
        instruction=domDocument.createProcessingInstruction("xml","version=\'1.0\' encoding=\'UTF-8\'");

        domDocument.appendChild(instruction);

        //创建MainConfig根节点
        QDomElement downloadingNode = domDocument.createElement("DownloadingList");
        //添加元素节点到父节点
        domDocument.appendChild(downloadingNode);

        //写xml文件
        QTextStream textStream(&downloadingFile);

        domDocument.save(textStream,4);

        downloadingFile.close();
    }
}

void XMLOperations::touchDownloadTrashConfigFile()
{
    QFile downloadTrashFile(DOWNLOADTRASHFILE_PATH);
    if (!downloadTrashFile.exists())
    {
        if (!downloadTrashFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return;
        }

        QDomDocument domDocument;

        QDomProcessingInstruction instruction;
        instruction=domDocument.createProcessingInstruction("xml","version=\'1.0\' encoding=\'UTF-8\'");

        domDocument.appendChild(instruction);

        //创建MainConfig根节点
        QDomElement downloadTrashNode = domDocument.createElement("DownloadTrashList");
        //添加元素节点到父节点
        domDocument.appendChild(downloadTrashNode);

        //写xml文件
        QTextStream textStream(&downloadTrashFile);

        domDocument.save(textStream,4);

        downloadTrashFile.close();
    }
}

void XMLOperations::setConfigDomNodeValue(QDomDocument &domDoc, QString tagName, QString nodeValue)
{
    if (domDoc.elementsByTagName(tagName).count() > 0
            && domDoc.elementsByTagName(tagName).item(0).toElement().text() != nodeValue
            && nodeValue != "")
    {
        domDoc.elementsByTagName(tagName).at(0).toElement().firstChild().setNodeValue(nodeValue);
    }
}

void XMLOperations::setDownloadingNodeValue(QDomDocument &domDoc, QString URL, QString tagName, QString nodeValue)
{
    QDomNodeList tmpList = domDoc.elementsByTagName("File");
    for (int i = 0; i < tmpList.count(); i ++)
    {
        //contentNode:子节点File下面包含的内容节点列表
        QDomNodeList contentNode = tmpList.item(i).toElement().childNodes();
        QDomNode tmpURLNode;
        QDomNode tmpTargeNode;
        for (int j = 0; j < contentNode.count(); j ++)
        {
            QString tmpTagName = contentNode.item(j).nodeName();
            if (tmpTagName == "URL")
                tmpURLNode = contentNode.item(j);
            else if (tmpTagName == tagName)
                tmpTargeNode = contentNode.item(j);
            else
                continue;
        }
        if (tmpURLNode.toElement().text() == URL && nodeValue != "")
        {
            tmpTargeNode.toElement().firstChild().setNodeValue(nodeValue);
        }
    }
}

void XMLOperations::setDownloadingThreadNodeValue(QDomDocument &domDoc, QString URL, QList<SDownloadThread> nodeValue)
{
    QDomNodeList tmpList = domDoc.elementsByTagName("File");
    for (int i = 0; i < tmpList.count(); i ++)
    {
        //contentNode:子节点File下面包含的内容节点列表
        QDomNodeList contentNode = tmpList.item(i).toElement().childNodes();
        QDomNode tmpURLNode;
        QDomNode tmpTargeNode;
        for (int j = 0; j < contentNode.count(); j ++)
        {
            QString tmpTagName = contentNode.item(j).nodeName();
            if (tmpTagName == "URL")
                tmpURLNode = contentNode.item(j);
            else if (tmpTagName == "Threads")
                tmpTargeNode = contentNode.item(j);
            else
                continue;
        }
        if (tmpURLNode.toElement().text() == URL)
        {
            //Thread list
            QDomNodeList threadNodeList = tmpTargeNode.toElement().childNodes();
            if (threadNodeList.count() != nodeValue.count())
                return;
            for (int x = 0; x < threadNodeList.count(); x ++)
            {
                QDomNodeList targetNodeList = threadNodeList.item(x).toElement().childNodes();
                for (int y = 0; y < targetNodeList.count(); y ++)
                {
                    if (targetNodeList.item(y).toElement().nodeName() == "CompleteBlockCount"
                            && nodeValue.at(x).completedBlockCount != "")
                        targetNodeList.item(y).toElement().firstChild().setNodeValue(nodeValue.at(x).completedBlockCount);
                }
            }
            break;
        }
    }
}

QDomElement XMLOperations::createChildElement(QString tagName, QString tagValue)
{
    QDomDocument domDoc;
    QDomElement tmpElement = domDoc.createElement(tagName);
    QDomText tmpText = domDoc.createTextNode(tagValue);
    tmpElement.appendChild(tmpText);

    return tmpElement;
}

QDomElement XMLOperations::createThreadElement(QList<SDownloadThread> threadList)
{
    QDomDocument domDoc;
    QDomElement threadsNode = domDoc.createElement("Threads");

    for (int i = 0; i < threadList.count(); i ++)
    {
        QDomElement threadNode = domDoc.createElement("Thread");
        threadNode.appendChild(createChildElement("StartBlockIndex", threadList.at(i).startBlockIndex));
        threadNode.appendChild(createChildElement("EndBlockIndex", threadList.at(i).endBlockIndex));
        threadNode.appendChild(createChildElement("CompleteBlockCount", threadList.at(i).completedBlockCount));

        threadsNode.appendChild(threadNode);
    }

    return threadsNode;
}

QDomNode XMLOperations::getMatchFileNode(QDomDocument &domDoc,QString URL)
{
    QDomNodeList tmpList = domDoc.elementsByTagName("File");
    for (int i = 0; i < tmpList.count(); i ++)
    {
        QDomNodeList contentList = tmpList.item(i).toElement().childNodes();
        for (int j = 0; j < contentList.count(); j ++)
        {
            QString tmpNodeName = contentList.item(j).nodeName();
            QString tmpNodeText = contentList.item(j).toElement().text();
            if (tmpNodeName == "URL" && tmpNodeText == URL)
            {
                return tmpList.item(i);
            }
        }
    }
}


QDomDocument XMLOperations::getDocument(QString path)
{
    QDomDocument domDocument;
    QFile downloadingFile(path);
    if (downloadingFile.open(QIODevice::ReadOnly))
    {
        domDocument.setContent(&downloadingFile);
    }

    downloadingFile.close();

    return domDocument;
}

bool XMLOperations::writeMainConfigFile(SMainConfig mainConfig)
{
    QDomDocument domDocument;
    QFile mainConfigFile(MAINCONFIGFILE_PATH);
    if (mainConfigFile.open(QIODevice::ReadOnly))
    {
        // 此处需做错误判断
        if (!domDocument.setContent(&mainConfigFile))
            return false;
    }
    else
        return false;

    mainConfigFile.close();

    //传递的值与原来相同或者为空则不写入
    setConfigDomNodeValue(domDocument, "DownloadSpeed", mainConfig.downloadSpeed);
    setConfigDomNodeValue(domDocument, "UploadSpeed", mainConfig.uploadSpeed);
    setConfigDomNodeValue(domDocument, "WindowsSavePath", mainConfig.windowsSavePath);
    setConfigDomNodeValue(domDocument, "LinuxSavePath", mainConfig.linuxSavePath);
    setConfigDomNodeValue(domDocument, "Beep", mainConfig.beep);
    setConfigDomNodeValue(domDocument, "EnableUpload", mainConfig.enableUpload);
    setConfigDomNodeValue(domDocument, "ThreadCount", mainConfig.defaultThreadCount);
    setConfigDomNodeValue(domDocument, "VideoDetectType", mainConfig.videoDetectType);
    setConfigDomNodeValue(domDocument, "AudioDetectType", mainConfig.audioDetectType);
    setConfigDomNodeValue(domDocument, "MaxJobCount", mainConfig.maxJobCount);
    setConfigDomNodeValue(domDocument, "clipboard", mainConfig.clipboard);
    setConfigDomNodeValue(domDocument, "exitOnClose", mainConfig.exitOnClose);
    setConfigDomNodeValue(domDocument, "aria2Path", mainConfig.exitOnClose);
    setConfigDomNodeValue(domDocument, "yougetPath", mainConfig.exitOnClose);


    //写xml文件
    if (!mainConfigFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }
    QTextStream textStream(&mainConfigFile);

    domDocument.save(textStream,4);

    mainConfigFile.close();

    return true;
}

bool XMLOperations::writeDownloadingConfigFile(SDownloading downloading)
{
    QDomDocument domDocument;
    QFile downloadingFile(DOWNLOADINGFILE_PATH);
    if (downloadingFile.open(QIODevice::ReadOnly))
    {
        // 此处需做错误判断
        if (!domDocument.setContent(&downloadingFile))
        {
            qDebug() << "setContent err...";
            return false;
        }
    }
    else
    {
        qDebug() << "opening xml file err";
        return false;
    }

    downloadingFile.close();

    //传递的值与原来相同或者为空则不写入,只有以下的值能修改
    setDownloadingNodeValue(domDocument, downloading.URL,"Name", downloading.name);
    setDownloadingNodeValue(domDocument, downloading.URL,"JobMaxSpeed", downloading.jobMaxSpeed);
    setDownloadingNodeValue(domDocument, downloading.URL,"EnableUpload", downloading.enableUpload);
    setDownloadingNodeValue(domDocument, downloading.URL, "ReadySize", downloading.readySize);
    setDownloadingNodeValue(domDocument, downloading.URL, "State", downloading.state);//2014.4.7add
    setDownloadingNodeValue(domDocument, downloading.URL, "AverageSpeed", downloading.averageSpeed);//2014.5.2add
    setDownloadingNodeValue(domDocument, downloading.URL, "LastModifyTime", downloading.lastModifyTime);//2014.5.2add
    setDownloadingNodeValue(domDocument, downloading.URL,"AutoOpenFolder", downloading.autoOpenFolder);
    setDownloadingThreadNodeValue(domDocument, downloading.URL, downloading.threadList);//only set CompleteBlockCount

    setDownloadingNodeValue(domDocument, downloading.URL,"RedirectURL", downloading.redirectRUL);  //20140521 zwy
    //写xml文件
    if (!downloadingFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "open file to write err";
        return false;
    }
    QTextStream textStream(&downloadingFile);

    domDocument.save(textStream,4);

    downloadingFile.close();

    return true;
}

void XMLOperations::insertDownloadedNode(SDownloaded tmpStruct)
{
    QDomDocument domDocument;
    QFile downloadedFile(DOWNLOADEDFILE_PATH);
    if (downloadedFile.open(QIODevice::ReadOnly))
    {
        // 此处需做错误判断
        if (!domDocument.setContent(&downloadedFile))
            return;
    }
    else
        return;

    downloadedFile.close();

    QDomElement fileElement = domDocument.createElement("File");

    fileElement.appendChild(createChildElement("Name", tmpStruct.name));
    fileElement.appendChild(createChildElement("URL", tmpStruct.URL));
    fileElement.appendChild(createChildElement("DLToolsType", tmpStruct.dlToolsType));
    fileElement.appendChild(createChildElement("CompleteDate", tmpStruct.completeDate));
    fileElement.appendChild(createChildElement("Size", tmpStruct.Size));
    fileElement.appendChild(createChildElement("Path", tmpStruct.savePath));
    fileElement.appendChild(createChildElement("Exist", tmpStruct.exist));
    fileElement.appendChild(createChildElement("IconPath",tmpStruct.iconPath));

    QDomElement rootElement = domDocument.documentElement();
    rootElement.appendChild(fileElement);

    //写xml文件
    if (!downloadedFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    QTextStream textStream(&downloadedFile);

    domDocument.save(textStream,4);

    downloadedFile.close();
}

void XMLOperations::insertDownloadingNode(SDownloading tmpStruct)
{
    QDomDocument domDocument;
    QFile downloadingFile(DOWNLOADINGFILE_PATH);
    if (downloadingFile.open(QIODevice::ReadOnly))
    {
        // 此处需做错误判断
        if (!domDocument.setContent(&downloadingFile))
        {
             return;
        }
    }
    else
        return;

    downloadingFile.close();

    QDomElement fileElement = domDocument.createElement("File");

    fileElement.appendChild(createChildElement("Name", tmpStruct.name));
    fileElement.appendChild(createChildElement("JobMaxSpeed", tmpStruct.jobMaxSpeed));
    fileElement.appendChild(createChildElement("SavePath", tmpStruct.savePath));
    fileElement.appendChild(createChildElement("EnableUpload", tmpStruct.enableUpload));
    fileElement.appendChild(createChildElement("URL", tmpStruct.URL));
    fileElement.appendChild(createChildElement("RedirectURL", tmpStruct.redirectRUL));
    fileElement.appendChild(createChildElement("DLToolsType", tmpStruct.dlToolsType));
    fileElement.appendChild(createChildElement("BlockCount", tmpStruct.blockCount));
    fileElement.appendChild(createChildElement("BlockSize", tmpStruct.blockSize));
    fileElement.appendChild(createChildElement("TotalSize", tmpStruct.totalSize));
    fileElement.appendChild(createChildElement("ReadySize", tmpStruct.readySize));
    fileElement.appendChild(createChildElement("State", tmpStruct.state));//2014.4.7add
    fileElement.appendChild(createChildElement("AverageSpeed", tmpStruct.averageSpeed));//2014.4.7add
    fileElement.appendChild(createChildElement("LastModifyTime", tmpStruct.lastModifyTime));//2014.4.7add
    fileElement.appendChild(createChildElement("AutoOpenFolder", tmpStruct.autoOpenFolder));
    fileElement.appendChild(createChildElement("IconPath",tmpStruct.iconPath));
    fileElement.appendChild(createThreadElement(tmpStruct.threadList));

    QDomElement rootElement = domDocument.documentElement();
    rootElement.appendChild(fileElement);

    //写xml文件
    if (!downloadingFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    QTextStream textStream(&downloadingFile);

    domDocument.save(textStream,4);

    downloadingFile.close();
}

void XMLOperations::insertDownloadTrash(SDownloadTrash tmpStruct)
{
    QDomDocument domDocument;
    QFile downloadTrashFile(DOWNLOADTRASHFILE_PATH);
    if (downloadTrashFile.open(QIODevice::ReadOnly))
    {
        // 此处需做错误判断
        if (!domDocument.setContent(&downloadTrashFile))
            return;
    }
    else
        return;

    downloadTrashFile.close();

    QDomElement fileElement = domDocument.createElement("File");

    fileElement.appendChild(createChildElement("Name", tmpStruct.name));
    fileElement.appendChild(createChildElement("URL", tmpStruct.URL));
    fileElement.appendChild(createChildElement("DLToolsType", tmpStruct.dlToolsType));
    fileElement.appendChild(createChildElement("TotalSize", tmpStruct.totalSize));
    fileElement.appendChild(createChildElement("IconPath",tmpStruct.iconPath));

    QDomElement rootElement = domDocument.documentElement();
    rootElement.appendChild(fileElement);

    //写xml文件
    if (!downloadTrashFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    QTextStream textStream(&downloadTrashFile);

    domDocument.save(textStream,4);

    downloadTrashFile.close();
}

void XMLOperations::removeDownloadedFileNode(QString URL)
{
    QDomDocument domDocument;
    QFile downloadedFile(DOWNLOADEDFILE_PATH);
    if (downloadedFile.open(QIODevice::ReadOnly))
    {
        // 此处需做错误判断
        if (!domDocument.setContent(&downloadedFile))
            return;
    }
    else
        return;

    downloadedFile.close();

    domDocument.documentElement().removeChild(getMatchFileNode(domDocument,URL));

    //写xml文件
    if (!downloadedFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    QTextStream textStream(&downloadedFile);
    domDocument.save(textStream,4);
    downloadedFile.close();

}

void XMLOperations::removeDownloadingFileNode(QString URL)
{
    if (!urlExit(URL,"ing"))
        return;

    QDomDocument domDocument;
    QFile downloadingFile(DOWNLOADINGFILE_PATH);
    if (downloadingFile.open(QIODevice::ReadOnly))
    {
        // 此处需做错误判断
        if (!domDocument.setContent(&downloadingFile))
        {
            qDebug() << "remove: setContent err";
            return;
        }
    }
    else
    {
        qDebug() << "remove: open file err";
        return;
    }

    downloadingFile.close();

    domDocument.documentElement().removeChild(getMatchFileNode(domDocument,URL));

    //写xml文件
    if (!downloadingFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "remove: open file err";
        return;
    }
    QTextStream textStream(&downloadingFile);
    domDocument.save(textStream,4);
    downloadingFile.close();
}

void XMLOperations::removeDownloadTrashFileNode(QString URL)
{
    QDomDocument domDocument;
    QFile downloadTrashFile(DOWNLOADTRASHFILE_PATH);
    if (downloadTrashFile.open(QIODevice::ReadOnly))
    {
        // 此处需做错误判断
        if (!domDocument.setContent(&downloadTrashFile))
            return;
    }
    else
        return;

    downloadTrashFile.close();
    qDebug() << "remove trash item";

    domDocument.documentElement().removeChild(getMatchFileNode(domDocument,URL));

    //写xml文件
    if (!downloadTrashFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    QTextStream textStream(&downloadTrashFile);
    domDocument.save(textStream,4);
    downloadTrashFile.close();
}

bool XMLOperations::urlExit(QString URL, QString type)
{
    QDomNodeList tmpList;

    if (type == "ing")
        tmpList = getDocument(DOWNLOADINGFILE_PATH).elementsByTagName("File");
    else
        tmpList = getDocument(DOWNLOADEDFILE_PATH).elementsByTagName("File");

    for (int i = 0; i < tmpList.count(); i ++)
    {

        //contentNode:子节点File下面包含的内容节点列表
        QDomNodeList contentNode = tmpList.item(i).toElement().childNodes();
        QDomNode tmpURLNode;
        for (int j = 0; j < contentNode.count(); j ++)
        {
            QString tmpTagName = contentNode.item(j).nodeName();
            if (tmpTagName == "URL")
            {
                tmpURLNode = contentNode.item(j);
                break;
            }
        }

        if (tmpURLNode.toElement().text() == URL)
            return true;
    }

    return false;
}

QList<SDownloaded> XMLOperations::getDownloadedNodes()
{
    QList<SDownloaded> tmpNodeList;

    QDomDocument domDocument;
    QFile * downloadedFile = new QFile(DOWNLOADEDFILE_PATH);
    if (downloadedFile->open(QIODevice::ReadOnly))
    {
        domDocument.setContent(downloadedFile); // 此处需做错误判断
    }
    downloadedFile->close();

    // 以下代码为解析XML文件中的内容
    QDomNodeList fileNodeList = domDocument.elementsByTagName("File");
    //循环读取所有的File节点
    for (int i = 0; i < fileNodeList.count(); i++)
    {
        SDownloaded tmpNodeStruct;
        //contentNode:子节点File下面包含的内容节点列表
        QDomNodeList contentNode = fileNodeList.item(i).toElement().childNodes();
        //循环读取一个File节点下的所有信息并保存
        for (int j = 0; j < contentNode.count(); j ++)
        {
            QString tagName = contentNode.item(j).nodeName();
            QString nodeValue = contentNode.item(j).toElement().text();

            if (tagName == "Name")
                tmpNodeStruct.name = nodeValue;
            else if (tagName == "CompleteDate")
                tmpNodeStruct.completeDate = nodeValue;
            else if (tagName == "Size")
                tmpNodeStruct.Size = nodeValue;
            else if (tagName == "Path")
                tmpNodeStruct.savePath = nodeValue;
            else if (tagName == "URL")
                tmpNodeStruct.URL = nodeValue;
            else if (tagName == "DLToolsType")
                tmpNodeStruct.dlToolsType = nodeValue;
            else if (tagName == "Exist")
                tmpNodeStruct.exist = nodeValue;
            else if (tagName == "IconPath")
                tmpNodeStruct.iconPath = nodeValue;
            else
                continue;
        }

        tmpNodeList.append(tmpNodeStruct);
    }

    return tmpNodeList;
}

QList<SDownloading> XMLOperations::getDownloadingNodes()
{
    QList<SDownloading> tmpNodeList;

    QDomDocument domDocument;
    QFile * downloadingFile = new QFile(DOWNLOADINGFILE_PATH);
    if (downloadingFile->open(QIODevice::ReadOnly))
    {
        domDocument.setContent(downloadingFile); // 此处需做错误判断
    }
    downloadingFile->close();

    // 以下代码为解析XML文件中的内容
    QDomNodeList fileNodeList = domDocument.elementsByTagName("File");
    //循环读取所有的File节点
    for (int i = 0; i < fileNodeList.count(); i++)
    {
        SDownloading tmpNodeStruct;
        //contentNode:子节点File下面包含的内容节点列表
        QDomNodeList contentNode = fileNodeList.item(i).toElement().childNodes();
        //循环读取一个File节点下的所有信息并保存
        for (int j = 0; j < contentNode.count(); j ++)
        {
            QString tagName = contentNode.item(j).nodeName();
            QString nodeValue = contentNode.item(j).toElement().text();

            if (tagName == "Name")
                tmpNodeStruct.name = nodeValue;
            else if (tagName == "JobMaxSpeed")
                tmpNodeStruct.jobMaxSpeed = nodeValue;
            else if (tagName == "SavePath")
                tmpNodeStruct.savePath = nodeValue;
            else if (tagName == "EnableUpload")
                tmpNodeStruct.enableUpload = nodeValue;
            else if (tagName == "URL")
                tmpNodeStruct.URL = nodeValue;
            else if (tagName == "RedirectURL")
                tmpNodeStruct.redirectRUL = nodeValue;
            else if (tagName == "DLToolsType")
                tmpNodeStruct.dlToolsType = nodeValue;
            else if (tagName == "BlockCount")
                tmpNodeStruct.blockCount = nodeValue;
            else if (tagName == "BlockSize")
                tmpNodeStruct.blockSize = nodeValue;
            else if (tagName == "TotalSize")
                tmpNodeStruct.totalSize = nodeValue;
            else if (tagName == "ReadySize")
                tmpNodeStruct.readySize = nodeValue;
            else if (tagName == "State")
                tmpNodeStruct.state = nodeValue;    //2014.4.07
            else if (tagName == "AverageSpeed")
                tmpNodeStruct.averageSpeed = nodeValue;
            else if (tagName == "LastModifyTime")
                tmpNodeStruct.lastModifyTime = nodeValue;
            else if (tagName == "AutoOpenFolder")
                tmpNodeStruct.autoOpenFolder = nodeValue;
            else if (tagName == "IconPath")
                tmpNodeStruct.iconPath = nodeValue;
            else if (tagName == "Threads")
            {
                QList<SDownloadThread> threadList;
                //Thread list
                QDomNodeList threadNodeList = contentNode.item(j).toElement().childNodes();
                for (int x = 0; x < threadNodeList.count(); x ++)
                {
                    SDownloadThread tmpThreadStruct;
                    QDomNodeList targetNodeList = threadNodeList.item(x).toElement().childNodes();
                    for (int y = 0; y < targetNodeList.count(); y ++)
                    {
                        QString tagName = targetNodeList.item(y).toElement().nodeName();
                        QString nodeValue = targetNodeList.item(y).toElement().text();

                        if (tagName == "CompleteBlockCount")
                            tmpThreadStruct.completedBlockCount = nodeValue;
                        else if (tagName == "StartBlockIndex")
                            tmpThreadStruct.startBlockIndex = nodeValue;
                        else if (tagName == "EndBlockIndex")
                            tmpThreadStruct.endBlockIndex = nodeValue;
                    }

                    threadList.append(tmpThreadStruct);
                }

                tmpNodeStruct.threadList = threadList;
            }
            else
                continue;
        }
        tmpNodeList.append(tmpNodeStruct);

    }
    return tmpNodeList;
}

QList<SDownloadTrash> XMLOperations::getDownloadTrashNodes()
{
    QList<SDownloadTrash> tmpNodeList;

    QDomDocument domDocument;
    QFile * downloadedFile = new QFile(DOWNLOADTRASHFILE_PATH);
    if (downloadedFile->open(QIODevice::ReadOnly))
    {
        domDocument.setContent(downloadedFile); // 此处需做错误判断
    }
    downloadedFile->close();

    // 以下代码为解析XML文件中的内容
    QDomNodeList fileNodeList = domDocument.elementsByTagName("File");
    //循环读取所有的File节点
    for (int i = 0; i < fileNodeList.count(); i++)
    {
        SDownloadTrash tmpNodeStruct;
        //contentNode:子节点File下面包含的内容节点列表
        QDomNodeList contentNode = fileNodeList.item(i).toElement().childNodes();
        //循环读取一个File节点下的所有信息并保存
        for (int j = 0; j < contentNode.count(); j ++)
        {
            QString tagName = contentNode.item(j).nodeName();
            QString nodeValue = contentNode.item(j).toElement().text();

            if (tagName == "Name")
                tmpNodeStruct.name = nodeValue;
            else if (tagName == "TotalSize")
                tmpNodeStruct.totalSize = nodeValue;
            else if (tagName == "URL")
                tmpNodeStruct.URL = nodeValue;
            else if (tagName == "DLToolsType")
                tmpNodeStruct.dlToolsType = nodeValue;
            else if (tagName == "IconPath")
                tmpNodeStruct.iconPath = nodeValue;
            else
                continue;
        }

        tmpNodeList.append(tmpNodeStruct);
    }

    return tmpNodeList;
}

SMainConfig XMLOperations::getMainConfig()
{
    SMainConfig tmpConfig;

    QDomDocument domDocument;
    QFile * mainConfigFile = new QFile(MAINCONFIGFILE_PATH);
    if (mainConfigFile->open(QIODevice::ReadOnly))
    {
        // 此处需做错误判断
        if (!domDocument.setContent(mainConfigFile))
            return tmpConfig;
    }
    else
        return tmpConfig;

    mainConfigFile->close();

    // 以下代码为解析XML文件中的内容
    tmpConfig.operatingSysType = domDocument.elementsByTagName("OperatingSystem").item(0).toElement().text();
    tmpConfig.version = domDocument.elementsByTagName("Version").item(0).toElement().text();
    tmpConfig.downloadSpeed = domDocument.elementsByTagName("DownloadSpeed").item(0).toElement().text();
    tmpConfig.uploadSpeed = domDocument.elementsByTagName("UploadSpeed").item(0).toElement().text();
    tmpConfig.windowsSavePath = domDocument.elementsByTagName("WindowsSavePath").item(0).toElement().text();
    tmpConfig.linuxSavePath = domDocument.elementsByTagName("LinuxSavePath").item(0).toElement().text();
    tmpConfig.beep = domDocument.elementsByTagName("Beep").item(0).toElement().text();
    tmpConfig.enableUpload = domDocument.elementsByTagName("EnableUpload").item(0).toElement().text();
    tmpConfig.defaultThreadCount = domDocument.elementsByTagName("ThreadCount").item(0).toElement().text();
    tmpConfig.videoDetectType = domDocument.elementsByTagName("VideoDetectType").item(0).toElement().text();
    tmpConfig.audioDetectType = domDocument.elementsByTagName("AudioDetectType").item(0).toElement().text();
    tmpConfig.maxJobCount = domDocument.elementsByTagName("MaxJobCount").item(0).toElement().text();
    tmpConfig.clipboard = domDocument.elementsByTagName("clipboard").item(0).toElement().text();
    tmpConfig.exitOnClose = domDocument.elementsByTagName("exitOnClose").item(0).toElement().text();
    tmpConfig.aria2Path = domDocument.elementsByTagName("aria2Path").item(0).toElement().text();
    tmpConfig.yougetPath = domDocument.elementsByTagName("yougetPath").item(0).toElement().text();

    return tmpConfig;
}

SDownloaded XMLOperations::getDownloadedNode(QString URL)
{
    SDownloaded tmpNode;
    QList<SDownloaded> tmpList = getDownloadedNodes();
    for (int i = 0; i < tmpList.count(); i ++)
    {
        if (tmpList.at(i).URL == URL)
        {
            tmpNode = tmpList.at(i);
            break;
        }
    }

    return tmpNode;
}

SDownloading XMLOperations::getDownloadingNode(QString URL)
{
    SDownloading tmpNode;
    QList<SDownloading> tmpList = getDownloadingNodes();
    for (int i = 0; i < tmpList.count(); i ++)
    {
        if (tmpList.at(i).URL == URL)
        {
            tmpNode = tmpList.at(i);
            break;
        }
    }
    return tmpNode;
}

SDownloadTrash XMLOperations::getDownloadTrashNode(QString URL)
{
    SDownloadTrash tmpNode;
    QList<SDownloadTrash> tmpList = getDownloadTrashNodes();
    for (int i = 0; i < tmpList.count(); i ++)
    {
        if (tmpList.at(i).URL == URL)
        {
            tmpNode = tmpList.at(i);
            break;
        }
    }
    return tmpNode;
}










