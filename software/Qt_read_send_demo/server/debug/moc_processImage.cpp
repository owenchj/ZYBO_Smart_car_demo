/****************************************************************************
** Meta object code from reading C++ file 'processImage.h'
**
** Created: Thu Jul 31 11:58:37 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../processImage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'processImage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ProcessImage[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      43,   39,   13,   13, 0x08,
      66,   13,   13,   13, 0x08,
      86,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ProcessImage[] = {
    "ProcessImage\0\0paintEvent(QPaintEvent*)\0"
    "err\0display_error(QString)\0"
    "processConnection()\0processdata()\0"
};

const QMetaObject ProcessImage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ProcessImage,
      qt_meta_data_ProcessImage, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ProcessImage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ProcessImage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ProcessImage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProcessImage))
        return static_cast<void*>(const_cast< ProcessImage*>(this));
    return QWidget::qt_metacast(_clname);
}

int ProcessImage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: paintEvent((*reinterpret_cast< QPaintEvent*(*)>(_a[1]))); break;
        case 1: display_error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: processConnection(); break;
        case 3: processdata(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
