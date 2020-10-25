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
    QByteArrayData data[18];
    char stringdata0[184];
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
QT_MOC_LITERAL(6, 67, 10), // "tabClicked"
QT_MOC_LITERAL(7, 78, 5), // "index"
QT_MOC_LITERAL(8, 84, 17), // "currentTabChanged"
QT_MOC_LITERAL(9, 102, 12), // "FocusChanged"
QT_MOC_LITERAL(10, 115, 8), // "QWidget*"
QT_MOC_LITERAL(11, 124, 3), // "old"
QT_MOC_LITERAL(12, 128, 3), // "now"
QT_MOC_LITERAL(13, 132, 10), // "TabClicked"
QT_MOC_LITERAL(14, 143, 17), // "CurrentTabChanged"
QT_MOC_LITERAL(15, 161, 6), // "HideAt"
QT_MOC_LITERAL(16, 168, 7), // "HideTab"
QT_MOC_LITERAL(17, 176, 7) // "ShowTab"

    },
    "tt::TabToolbar\0Minimized\0\0Maximized\0"
    "SpecialTabClicked\0StyleChanged\0"
    "tabClicked\0index\0currentTabChanged\0"
    "FocusChanged\0QWidget*\0old\0now\0TabClicked\0"
    "CurrentTabChanged\0HideAt\0HideTab\0"
    "ShowTab"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_tt__TabToolbar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,
       4,    0,   81,    2, 0x06 /* Public */,
       5,    0,   82,    2, 0x06 /* Public */,
       6,    1,   83,    2, 0x06 /* Public */,
       8,    1,   86,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    2,   89,    2, 0x08 /* Private */,
      13,    1,   94,    2, 0x08 /* Private */,
      14,    1,   97,    2, 0x08 /* Private */,
      15,    1,  100,    2, 0x08 /* Private */,
      15,    0,  103,    2, 0x28 /* Private | MethodCloned */,
      16,    1,  104,    2, 0x08 /* Private */,
      17,    1,  107,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 10,   11,   12,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,

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
        case 4: _t->tabClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->currentTabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->FocusChanged((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 7: _t->TabClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->CurrentTabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->HideAt((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->HideAt(); break;
        case 11: _t->HideTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->ShowTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
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
        {
            using _t = void (TabToolbar::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TabToolbar::tabClicked)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TabToolbar::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TabToolbar::currentTabChanged)) {
                *result = 5;
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
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
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

// SIGNAL 4
void tt::TabToolbar::tabClicked(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void tt::TabToolbar::currentTabChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
