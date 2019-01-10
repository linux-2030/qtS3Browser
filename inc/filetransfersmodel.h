#ifndef FILETRANSFERSMODEL_H
#define FILETRANSFERSMODEL_H

#include "inc/logmgr.h"
#include <QObject>
#include <QMap>

class FileTransfersModel : public QObject
{
    Q_OBJECT
public:
    enum class TransferMode { upload, download };
    Q_ENUMS(TransferMode)
    // --------------------------------------------------------------------------
    explicit FileTransfersModel(QObject *parent = nullptr);
    // --------------------------------------------------------------------------
    Q_INVOKABLE inline int getTransfersNumQML() const { return transfers.size(); }
    // --------------------------------------------------------------------------
    Q_INVOKABLE inline QList<QString> getTransfersKeysQML() const { return transfers.keys(); }
    // --------------------------------------------------------------------------
    Q_INVOKABLE QString getTransferSrcPathQML(const int idx) const {
        if(transfers.size() > 0 && idx < transfers.size() && idx >= 0) {
            return transfers.values().at(idx).at(0);
        }
        return "";
    }
    // --------------------------------------------------------------------------
    Q_INVOKABLE QString getTransferDstPathQML(const int idx) const {
        if(transfers.size() > 0 && idx < transfers.size() && idx >= 0) {
            return transfers.values().at(idx).at(1);
        }
        return "";
    }
    // --------------------------------------------------------------------------
    Q_INVOKABLE TransferMode getTransferModeQML(const int idx) const {
        if(modes.size() > 0 && idx < modes.size() && idx >= 0) {
            return modes.values().at(idx);
        }
        return TransferMode::download;
    }
    // --------------------------------------------------------------------------
    Q_INVOKABLE QString getTransferIconQML(const QString &fileName) const {
        if(modes[fileName] == TransferMode::download) {
            return "32_download_icon.png";
        }
        return "32_upload_icon.png";
    }
    // --------------------------------------------------------------------------
    Q_INVOKABLE void addTransferToQueueQML(const QString &fileName, const QString &src, const QString &dst);
    // --------------------------------------------------------------------------
    Q_INVOKABLE void removeTransferQML(const QString &fileName);
    // --------------------------------------------------------------------------
    Q_INVOKABLE void removeTransferQML(const int idx);
    // --------------------------------------------------------------------------
    Q_SIGNAL void addTransferProgressSignal(const QString key,
                                            const unsigned long current,
                                            const unsigned long total);
    // --------------------------------------------------------------------------
    Q_SLOT void addTransferProgressSlot(const QString key, const unsigned long current, const unsigned long total);
    // --------------------------------------------------------------------------
    Q_INVOKABLE static int getTransferProgressNum() {
        std::lock_guard<std::mutex> lock(mut);
        return transfersProgress.size();
    }
    // --------------------------------------------------------------------------
    Q_INVOKABLE static unsigned long getTransfersCopiedBytes(const QString& key) {
        //LogMgr::debug(Q_FUNC_INFO);
        if(!key.isEmpty() && transfersProgress.contains(key)) {
            return transfersProgress[key].first();
        }
        return 0;
    }
    // --------------------------------------------------------------------------
    Q_INVOKABLE static unsigned long getTransfersTotalBytes(const QString& key) {
        //LogMgr::debug(Q_FUNC_INFO);
        if(!key.isEmpty() && transfersProgress.contains(key)) {
            return transfersProgress[key].last();
        }
        return 0;
    }
    // --------------------------------------------------------------------------
    Q_INVOKABLE static QString getTransfersProgressKey(const int idx);
    // --------------------------------------------------------------------------
    Q_INVOKABLE static void clearTransfersProgress();
private:
    static QMap<QString, QStringList> transfers;
    static QMap<QString, TransferMode> modes;
    static QMap<QString, QList<unsigned long>> transfersProgress;
    static std::mutex mut;
};

#endif // FILETRANSFERSMODEL_H
