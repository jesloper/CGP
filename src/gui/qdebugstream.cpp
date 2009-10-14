#include "qdebugstream.h"
#include "streambuf"
std::streamsize QDebugStream::xsputn(const char *p, std::streamsize n) {
    QMutexLocker locker(m_mutex);
    m_string.append(p, p + n);
    //uint pos = 0;
    /*while (pos != std::string::npos) {
        pos = m_string.find('\n');
        if (pos != std::string::npos) {
            std::string tmp(m_string.begin(), m_string.begin() + pos);
            m_text.append(tmp.c_str()).append("\n");
            m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
        }
    }
    */
    m_text.append(m_string.c_str());
    m_string.clear();
    if(log_window){
        QCoreApplication::postEvent(this, new QEvent(QEvent::User));
    }else{
        (*m_fileStream) << m_text;
    }
    return n;
}

/**
  * Deals with posted events. Actually updates the log window
  */
bool QDebugStream::event(QEvent* event) {
    if (event->type() == QEvent::User) {
        {
            QMutexLocker locker(m_mutex);
            if (!m_text.isEmpty()){
                log_window->append(m_text);
            }
            m_text.clear();
        }
        event->accept();
        return true;
    }
    return false;
}

std::streambuf::int_type QDebugStream::overflow(int_type v) {
    QMutexLocker locker(m_mutex);

   // if (v == '\n') {
        m_text.append(m_string.c_str()).append("\n");
        m_string.erase(m_string.begin(), m_string.end());
    //} else {
    //    m_string += v;
   // }
    if(log_window){
        QCoreApplication::postEvent(this, new QEvent(QEvent::User));
    }
    return v;
}
