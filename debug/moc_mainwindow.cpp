/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Thu 8. Mar 10:09:33 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      35,   11,   11,   11, 0x08,
      58,   11,   11,   11, 0x08,
      83,   11,   11,   11, 0x08,
     108,   11,   11,   11, 0x08,
     132,  126,   11,   11, 0x08,
     174,  168,   11,   11, 0x08,
     199,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0actionOpen_Triggered()\0"
    "actionWire_Triggered()\0actionSelect_Triggered()\0"
    "actionDelete_Triggered()\0refreshTreeView()\0"
    "index\0treeView_doubleClicked(QModelIndex)\0"
    "value\0slider_valueChanged(int)\0"
    "scene_selectionChanged()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: actionOpen_Triggered(); break;
        case 1: actionWire_Triggered(); break;
        case 2: actionSelect_Triggered(); break;
        case 3: actionDelete_Triggered(); break;
        case 4: refreshTreeView(); break;
        case 5: treeView_doubleClicked((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 6: slider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: scene_selectionChanged(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
