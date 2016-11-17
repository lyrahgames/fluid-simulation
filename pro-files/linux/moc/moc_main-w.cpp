/****************************************************************************
** Meta object code from reading C++ file 'main-w.h'
**
** Created: Thu Nov 17 14:24:12 2016
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/main-w.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main-w.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainW[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x0a,
      20,   17,    6,    6, 0x0a,
      36,    6,    6,    6, 0x2a,
      48,    6,    6,    6, 0x0a,
      64,    6,    6,    6, 0x0a,
      81,   79,    6,    6, 0x08,
      97,   95,    6,    6, 0x08,
     125,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainW[] = {
    "MainW\0\0repaint()\0in\0set_ready(bool)\0"
    "set_ready()\0init_rand_pos()\0gen_rand_pos()\0"
    "n\0set_grid(int)\0i\0set_rand_pos_size_slot(int)\0"
    "gen_rand_pos_slot()\0"
};

void MainW::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainW *_t = static_cast<MainW *>(_o);
        switch (_id) {
        case 0: _t->repaint(); break;
        case 1: _t->set_ready((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->set_ready(); break;
        case 3: _t->init_rand_pos(); break;
        case 4: _t->gen_rand_pos(); break;
        case 5: _t->set_grid((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->set_rand_pos_size_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->gen_rand_pos_slot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainW::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainW::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainW,
      qt_meta_data_MainW, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainW::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainW::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainW::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainW))
        return static_cast<void*>(const_cast< MainW*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainW::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
