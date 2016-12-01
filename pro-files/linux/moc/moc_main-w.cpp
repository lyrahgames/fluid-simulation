/****************************************************************************
** Meta object code from reading C++ file 'main-w.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/main-w.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main-w.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainW_t {
    QByteArrayData data[19];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainW_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainW_t qt_meta_stringdata_MainW = {
    {
QT_MOC_LITERAL(0, 0, 5), // "MainW"
QT_MOC_LITERAL(1, 6, 7), // "repaint"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 9), // "set_ready"
QT_MOC_LITERAL(4, 25, 2), // "in"
QT_MOC_LITERAL(5, 28, 13), // "init_rand_pos"
QT_MOC_LITERAL(6, 42, 12), // "gen_rand_pos"
QT_MOC_LITERAL(7, 55, 9), // "loop_slot"
QT_MOC_LITERAL(8, 65, 8), // "set_grid"
QT_MOC_LITERAL(9, 74, 1), // "n"
QT_MOC_LITERAL(10, 76, 10), // "clear_slot"
QT_MOC_LITERAL(11, 87, 22), // "set_rand_pos_size_slot"
QT_MOC_LITERAL(12, 110, 1), // "i"
QT_MOC_LITERAL(13, 112, 17), // "gen_rand_pos_slot"
QT_MOC_LITERAL(14, 130, 9), // "play_slot"
QT_MOC_LITERAL(15, 140, 18), // "set_wave_damp_slot"
QT_MOC_LITERAL(16, 159, 3), // "val"
QT_MOC_LITERAL(17, 163, 15), // "set_wave_c_slot"
QT_MOC_LITERAL(18, 179, 16) // "set_wave_dt_slot"

    },
    "MainW\0repaint\0\0set_ready\0in\0init_rand_pos\0"
    "gen_rand_pos\0loop_slot\0set_grid\0n\0"
    "clear_slot\0set_rand_pos_size_slot\0i\0"
    "gen_rand_pos_slot\0play_slot\0"
    "set_wave_damp_slot\0val\0set_wave_c_slot\0"
    "set_wave_dt_slot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainW[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       3,    1,   85,    2, 0x0a /* Public */,
       3,    0,   88,    2, 0x2a /* Public | MethodCloned */,
       5,    0,   89,    2, 0x0a /* Public */,
       6,    0,   90,    2, 0x0a /* Public */,
       7,    0,   91,    2, 0x08 /* Private */,
       8,    1,   92,    2, 0x08 /* Private */,
      10,    0,   95,    2, 0x08 /* Private */,
      11,    1,   96,    2, 0x08 /* Private */,
      13,    0,   99,    2, 0x08 /* Private */,
      14,    0,  100,    2, 0x08 /* Private */,
      15,    1,  101,    2, 0x08 /* Private */,
      17,    1,  104,    2, 0x08 /* Private */,
      18,    1,  107,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   16,
    QMetaType::Void, QMetaType::Double,   16,
    QMetaType::Void, QMetaType::Double,   16,

       0        // eod
};

void MainW::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainW *_t = static_cast<MainW *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->repaint(); break;
        case 1: _t->set_ready((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->set_ready(); break;
        case 3: _t->init_rand_pos(); break;
        case 4: _t->gen_rand_pos(); break;
        case 5: _t->loop_slot(); break;
        case 6: _t->set_grid((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->clear_slot(); break;
        case 8: _t->set_rand_pos_size_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->gen_rand_pos_slot(); break;
        case 10: _t->play_slot(); break;
        case 11: _t->set_wave_damp_slot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->set_wave_c_slot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->set_wave_dt_slot((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainW::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainW.data,
      qt_meta_data_MainW,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainW::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainW::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainW.stringdata0))
        return static_cast<void*>(const_cast< MainW*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainW::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
