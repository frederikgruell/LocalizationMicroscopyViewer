/****************************************************************************
** Meta object code from reading C++ file 'locmicview.h'
**
** Created: Tue Aug 30 12:04:56 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/locmicview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'locmicview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LocMicView[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   12,   11,   11, 0x05,
      40,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,   11,   11,   11, 0x0a,
      70,   11,   11,   11, 0x0a,
      80,   11,   11,   11, 0x0a,
      93,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LocMicView[] = {
    "LocMicView\0\0enabled\0zoomInEnabled(bool)\0"
    "zoomOutEnabled(bool)\0zoomIn()\0zoomOut()\0"
    "actualSize()\0zoomToFit()\0"
};

const QMetaObject LocMicView::staticMetaObject = {
    { &QScrollArea::staticMetaObject, qt_meta_stringdata_LocMicView,
      qt_meta_data_LocMicView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LocMicView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LocMicView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LocMicView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LocMicView))
        return static_cast<void*>(const_cast< LocMicView*>(this));
    return QScrollArea::qt_metacast(_clname);
}

int LocMicView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: zoomInEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: zoomOutEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: zoomIn(); break;
        case 3: zoomOut(); break;
        case 4: actualSize(); break;
        case 5: zoomToFit(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void LocMicView::zoomInEnabled(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LocMicView::zoomOutEnabled(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
