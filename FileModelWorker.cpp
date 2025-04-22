#include "FileModelWorker.h"

void FileModelWorker::process(){
    QDir dir(m_path);
    QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot,
                                            QDir::Name | QDir::DirsFirst);
    emit finished(list);
}