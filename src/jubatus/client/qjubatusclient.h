/* Copyright (c) 2012 Silk Project.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Silk nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SILK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef QJUBATUSCLIENT_H
#define QJUBATUSCLIENT_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "jubatus_global.h"

namespace jubatus {
    namespace client {
        namespace common {
            struct datum;
            class client;
        }
    }
}

class JUBATUS_EXPORT QJubatusClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(double timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
public:
    explicit QJubatusClient(QObject *parent = 0);

    const QString &host() const;
    int port() const;
    const QString &name() const;
    double timeout() const;

public slots:
    void setHost(const QString &host);
    void setPort(int port);
    void setName(const QString &name);
    void setTimeout(double timeout);

signals:
    void hostChanged(const QString &host);
    void portChanged(int port);
    void nameChanged(const QString &name);
    void timeoutChanged(double timeout);

    void error(const QString &message);

protected:
    template <class T>
    T *client() {
        if (!m_client) {
            m_client = new T(convert(host()), port(), convert(name()), timeout());
        }
        return static_cast<T *>(m_client);
    }

    std::string convert(const QString &data) const;
    QString convert(const std::string &data) const;
    std::vector<std::string> convert(const QStringList &data) const;
    QStringList convert(const std::vector<std::string> &data) const;
    jubatus::client::common::datum convert(const QVariantMap &data) const;
    QVariantMap convert(const jubatus::client::common::datum &data) const;
    std::vector<jubatus::client::common::datum> convert(const QList<QVariantMap> &data) const;
    QList<QVariantMap> convert(const std::vector<jubatus::client::common::datum> &data) const;

private:
    class Private;
    Private *d;

    jubatus::client::common::client *m_client;
};

#define EXEC_JUBATUS_COMMAND(COMMAND) \
    try { \
        for (int i = 0; i < 3; i++) { \
            try { \
                COMMAND; \
                break; \
            } catch (msgpack::rpc::connection_closed_error &e) { \
                client()->get_client().close(); \
            } catch (msgpack::rpc::system_error &e) { \
                client()->get_client().close(); \
            } catch (msgpack::rpc::timeout_error &e) { \
                client()->get_client().close(); \
            } \
            ::sleep(1); \
        } \
    } catch(msgpack::rpc::rpc_error &e) { \
        emit error(QString::fromUtf8(e.what())); \
    }

#endif // QJUBATUSCLIENT_H
