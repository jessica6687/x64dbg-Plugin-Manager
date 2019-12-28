// Copyright (c) 2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "consoleoutput.h"
#include "../global.h"
#include "../utils.h"
#include "../createmoduleprocess.h"
#include "../getfilefromserverprocess.h"
#include "../installmoduleprocess.h"
#include "../removemoduleprocess.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(X_ORGANIZATIONNAME);
    QCoreApplication::setOrganizationDomain(X_ORGANIZATIONDOMAIN);
    QCoreApplication::setApplicationName(X_APPLICATIONNAME);
    QCoreApplication::setApplicationVersion(X_APPLICATIONVERSION);

    QCoreApplication app(argc, argv);

    ConsoleOutput consoleOutput;
    QCommandLineParser parser;
    QString sDescription;
    sDescription.append(QString("%1 v%2\n").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION));
    sDescription.append(QString("%1\n").arg("Copyright(C) 2019 hors<horsicq@gmail.com> Web: http://ntinfo.biz"));
    parser.setApplicationDescription(sDescription);
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("plugin","The plugin to open.");

    QCommandLineOption clSetGlobalRootPath  (QStringList()<<"g"<<"setglobalrootpath",   "Set a global root path<path>.",                "path");
    QCommandLineOption clCreatePlugin       (QStringList()<<"c"<<"createplugin",        "Create a plugin<name>.",                       "name");
    QCommandLineOption clSetRootPath        (QStringList()<<"r"<<"setrootpath",         "Set a root path<path>.",                       "path");
    QCommandLineOption clSetName            (QStringList()<<"n"<<"setname",             "Set a name of plugin<name>.",                  "name");
    QCommandLineOption clSetVersion         (QStringList()<<"V"<<"setversion",          "Set a version of plugin<version>.",            "version");
    QCommandLineOption clSetDate            (QStringList()<<"d"<<"setdate",             "Set a date of plugin<date>.",                  "date");
    QCommandLineOption clSetAuthor          (QStringList()<<"a"<<"setauthor",           "Set an author of plugin<author>.",             "author");
    QCommandLineOption clSetBugreport       (QStringList()<<"b"<<"setbugreport",        "Set a bugreport of plugin<bugreport>.",        "bugreport");
    QCommandLineOption clSetInfo            (QStringList()<<"I"<<"setinfo",             "Set an info of plugin<info>.",                 "info");

    parser.addOption(clSetGlobalRootPath);
    parser.addOption(clCreatePlugin);
    parser.addOption(clSetRootPath);
    parser.addOption(clSetName);
    parser.addOption(clSetVersion);
    parser.addOption(clSetDate);
    parser.addOption(clSetAuthor);
    parser.addOption(clSetBugreport);
    parser.addOption(clSetInfo);

    parser.process(app);

    if(parser.isSet(clCreatePlugin))
    {
        Utils::MDATA mdata={};

        mdata.sBundleFileName=parser.value(clCreatePlugin);

        if(mdata.sBundleFileName!="")
        {
            mdata.sBundleFileName+=".x64dbg.zip";
        }

        mdata.sRoot=parser.value(clSetRootPath);
        mdata.sName=parser.value(clSetName);
        mdata.sCurrentVersion=parser.value(clSetVersion);
        mdata.sCurrentDate=parser.value(clSetDate);
        mdata.sAuthor=parser.value(clSetAuthor);
        mdata.sBugreport=parser.value(clSetBugreport);
        mdata.sInfo=parser.value(clSetInfo);

        QString sErrorString;

        if(Utils::checkMData(&mdata,&sErrorString))
        {
            CreateModuleProcess createModuleProcess;
            QObject::connect(&createModuleProcess,SIGNAL(infoMessage(QString)),&consoleOutput,SLOT(infoMessage(QString)));
            QObject::connect(&createModuleProcess,SIGNAL(errorMessage(QString)),&consoleOutput,SLOT(errorMessage(QString)));
            createModuleProcess.setData(&mdata);
            createModuleProcess.process();
        }
        else
        {
            consoleOutput.errorMessage(sErrorString);
        }
    }

    parser.showHelp();
    Q_UNREACHABLE();

    return 0;
}
