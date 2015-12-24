
#include "VirtualBase.hpp"
#include <QtCore>

namespace {


class RegisterAllQtCore {
public:
    RegisterAllQtCore() {
        cct::registerConverter<QAbstractAnimation,QObject>();
        cct::registerConverter<QAbstractEventDispatcher,QObject>();
        cct::registerConverter<QAbstractItemModel,QObject>();
        cct::registerConverter<QAbstractState,QObject>();
        cct::registerConverter<QAbstractTransition,QObject>();
        cct::registerConverter<QCoreApplication,QObject>();
        cct::registerConverter<QEventLoop,QObject>();
        cct::registerConverter<QFileSelector,QObject>();
        cct::registerConverter<QFileSystemWatcher,QObject>();
        cct::registerConverter<QIODevice,QObject>();
        cct::registerConverter<QItemSelectionModel,QObject>();
        cct::registerConverter<QLibrary,QObject>();
        cct::registerConverter<QMimeData,QObject>();
        cct::registerConverter<QObjectCleanupHandler,QObject>();
        cct::registerConverter<QPluginLoader,QObject>();
        cct::registerConverter<QSettings,QObject>();
        cct::registerConverter<QSharedMemory,QObject>();
        cct::registerConverter<QSignalMapper,QObject>();
        cct::registerConverter<QSocketNotifier,QObject>();
        cct::registerConverter<QThread,QObject>();
        cct::registerConverter<QThreadPool,QObject>();
        cct::registerConverter<QTimeLine,QObject>();
        cct::registerConverter<QTimer,QObject>();
        cct::registerConverter<QTranslator,QObject>();
        cct::registerConverter<QWinEventNotifier,QObject>();

        cct::registerVirtualConverter<QObject,QAbstractAnimation>();
        cct::registerVirtualConverter<QObject,QAbstractEventDispatcher>();
        cct::registerVirtualConverter<QObject,QAbstractItemModel>();
        cct::registerVirtualConverter<QObject,QAbstractState>();
        cct::registerVirtualConverter<QObject,QAbstractTransition>();
        cct::registerVirtualConverter<QObject,QCoreApplication>();
        cct::registerVirtualConverter<QObject,QEventLoop>();
        cct::registerVirtualConverter<QObject,QFileSelector>();
        cct::registerVirtualConverter<QObject,QFileSystemWatcher>();
        cct::registerVirtualConverter<QObject,QIODevice>();
        cct::registerVirtualConverter<QObject,QItemSelectionModel>();
        cct::registerVirtualConverter<QObject,QLibrary>();
        cct::registerVirtualConverter<QObject,QMimeData>();
        cct::registerVirtualConverter<QObject,QObjectCleanupHandler>();
        cct::registerVirtualConverter<QObject,QPluginLoader>();
        cct::registerVirtualConverter<QObject,QSettings>();
        cct::registerVirtualConverter<QObject,QSharedMemory>();
        cct::registerVirtualConverter<QObject,QSignalMapper>();
        cct::registerVirtualConverter<QObject,QSocketNotifier>();
        cct::registerVirtualConverter<QObject,QThread>();
        cct::registerVirtualConverter<QObject,QThreadPool>();
        cct::registerVirtualConverter<QObject,QTimeLine>();
        cct::registerVirtualConverter<QObject,QTimer>();
        cct::registerVirtualConverter<QObject,QTranslator>();
        cct::registerVirtualConverter<QObject,QWinEventNotifier>();

    }
};

RegisterAllQtCore static_install_;

}



