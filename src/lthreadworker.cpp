#include "lthreadworker.h"



LThreadWorker::LThreadWorker(QObject *parent)
    : m_parent{parent},
      m_thread{nullptr},
      m_autoDelete(false)
{
}



bool LThreadWorker::isThreadRunning() const
{
    return m_thread != nullptr && m_thread->isRunning();
}



void LThreadWorker::startThread(const QString &name)
{
    if (m_thread == nullptr)
        m_thread = new QThread(m_parent);

    if (!name.isEmpty())
        m_thread->setObjectName(name);

    moveToThread(m_thread);

    connect(m_thread, &QThread::finished, this, [this] {
        emit threadFinished();
        if (m_autoDelete)
            deleteLater();
    });

    m_thread->start();

    emit threadStarted();
}



void LThreadWorker::stopThread()
{
    if (m_thread == nullptr)
        return;

    m_thread->quit();
    m_thread->wait();
}



void LThreadWorker::terminateThread()
{
    if (m_thread == nullptr)
        return;

    m_thread->terminate();
    m_thread->wait();

    emit threadTerminated();
}