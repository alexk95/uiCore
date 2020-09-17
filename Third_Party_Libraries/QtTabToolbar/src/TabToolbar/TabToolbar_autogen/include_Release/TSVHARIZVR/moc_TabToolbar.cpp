/****************************************************************************
** Meta object code from reading C++ file 'TabToolbar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TabToolbar/TabToolbar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TabToolbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_tt__TabToolbar_t {
    QByteArrayData data[16];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_tt__TabToolbar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_tt__TabToolbar_t qt_meta_stringdata_tt__TabToolbar = {
    {
QT_MOC_LITERAL(0, 0, 14), // "tt::TabToolbar"
QT_MOC_LITERAL(1, 15, 9), // "Minimized"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 9), // "Maximized"
QT_MOC_LITERAL(4, 36, 17), // "SpecialTabClicked"
QT_MOC_LITERAL(5, 54, 12), // "StyleChanged"
QT_MOC_LITERAL(6, 67, 12), // "FocusChanged"
QT_MOC_LITERAL(7, 80, 8), // "QWidget*"
QT_MOC_LITERAL(8, 89, 3), // "old"
QT_MOC_LITERAL(9, 93, 3), // "now"
QT_MOC_LITERAL(10, 97, 10), // "TabClicked"
QT_MOC_LITERAL(11, 108, 5), // "index"
QT_MOC_LITERAL(12, 114, 17), // "CurrentTabChanged"
QT_MOC_LITERAL(13, 132, 6), // "HideAt"
QT_MOC_LITERAL(14, 139, 7), // "HideTab"
QT_MOC_LITERAL(15, 147, 7) // "ShowTab"

    },
    "tt::TabToolbar\0Minimized\0\0Maximized\0"
    "SpecialTabClicked\0StyleChanged\0"
    "FocusChanged\0QWidget*\0old\0now\0TabClicked\0"
    "index\0CurrentTabChanged\0HideAt\0HideTab\0"
    "ShowTab"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_tt__TabToolbar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    0,   71,    2, 0x06 /* Public */,
       5,    0,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   73,    2, 0x08 /* Private */,
      10,    1,   78,    2, 0x08 /* Private */,
      12,    1,   81,    2, 0x08 /* Private */,
      13,    1,   84,    2, 0x08 /* Private */,
      13,    0,   87,    2, 0x28 /* Private | MethodCloned */,
      14,    1,   88,    2, 0x08 /* Private */,
      15,    1,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 7,    8,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void tt::TabToolbar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TabToolbar *_t = static_cast<TabToolbar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Minimized(); break;
        case 1: _t->Maximized(); break;
        case 2: _t->SpecialTabClicked(); break;
        case 3: _t->StyleChanged(); break;
        case 4: _t->FocusChanged((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 5: _t->TabClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->CurrentTabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->HideAt((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->HideAt(); break;
        case 9: _t->HideTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->ShowTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TabToolbar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TabToolbar::Minimized)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TabToolbar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TabToolbar::Maximized)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TabToolbar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TabToolbar::SpecialTabClicked)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TabToolbar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TabToolbar::StyleChanged)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject tt::TabToolbar::staticMetaObject = {
    { &QToolBar::staticMetaObject, qt_meta_stringdata_tt__TabToolbar.data,
      qt_meta_data_tt__TabToolbar,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *tt::TabToolbar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *tt::TabToolbar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_tt__TabToolbar.stringdata0))
        return static_cast<void*>(this);
    return QToolBar::qt_metacast(_clname);
}

int tt::TabToolbar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void tt::TabToolbar::Minimized()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void tt::TabToolbar::Maximized()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void tt::TabToolbar::SpecialTabClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void tt::TabToolbar::StyleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
