/****************************************************************************
** Meta object code from reading C++ file 'main-w.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/main-w.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main-w.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainW_t {
    QByteArrayData data[13];
    char stringdata[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainW_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainW_t qt_meta_stringdata_MainW = {
    {
QT_MOC_LITERAL(0, 0, 5),
QT_MOC_LITERAL(1, 6, 7),
QT_MOC_LITERAL(2, 14, 0),
QT_MOC_LITERAL(3, 15, 9),
QT_MOC_LITERAL(4, 25, 2),
QT_MOC_LITERAL(5, 28, 13),
QT_MOC_LITERAL(6, 42, 12),
QT_MOC_LITERAL(7, 55, 9),
QT_MOC_LITERAL(8, 65, 8),
QT_MOC_LITERAL(9, 74, 1),
QT_MOC_LITERAL(10, 76, 22),
QT_MOC_LITERAL(11, 99, 1),
QT_MOC_LITERAL(12, 101, 17)
    },
    "MainW\0repaint\0\0set_ready\0in\0init_rand_pos\0"
    "gen_rand_pos\0loop_slot\0set_grid\0n\0"
    "set_rand_pos_size_slot\0i\0gen_rand_pos_slot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainW[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x0a /* Public */,
       3,    1,   60,    2, 0x0a /* Public */,
       3,    0,   63,    2, 0x2a /* Public | MethodCloned */,
       5,    0,   64,    2, 0x0a /* Public */,
       6,    0,   65,    2, 0x0a /* Public */,
       7,    0,   66,    2, 0x08 /* Private */,
       8,    1,   67,    2, 0x08 /* Private */,
      10,    1,   70,    2, 0x08 /* Private */,
      12,    0,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,

       0        // eod
};

void MainW::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainW *_t = static_cast<MainW *>(_o);
        switch (_id) {
        case 0: _t->repaint(); break;
        case 1: _t->set_ready((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->set_ready(); break;
        case 3: _t->init_rand_pos(); break;
        case 4: _t->gen_rand_pos(); break;
        case 5: _t->loop_slot(); break;
        case 6: _t->set_grid((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->set_rand_pos_size_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->gen_rand_pos_slot(); break;
        default: ;
        }
    }
}

const QMetaObject MainW::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainW.data,
      qt_meta_data_MainW,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainW::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainW::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainW.stringdata))
        return static_cast<void*>(const_cast< MainW*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainW::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
