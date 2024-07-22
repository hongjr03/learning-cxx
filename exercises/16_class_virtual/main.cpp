#include "../exercise.h"

// READ: 虚函数 <https://zh.cppreference.com/w/cpp/language/virtual>

struct A {
    virtual char virtual_name() const {
        return 'A';
    }
    char direct_name() const {
        return 'A';
    }
};
struct B : public A {
    // READ: override <https://zh.cppreference.com/w/cpp/language/override>
    char virtual_name() const override {
        return 'B';
    }
    char direct_name() const {
        return 'B';
    }
};
struct C : public B {
    // READ: final <https://zh.cppreference.com/w/cpp/language/final>
    char virtual_name() const final {
        return 'C';
    }
    char direct_name() const {
        return 'C';
    }
};
struct D : public C {
    char direct_name() const {
        return 'D';
    }
};

int main(int argc, char **argv) {
    constexpr auto MSG = "Replace '?' with its correct name.";

    A a;
    B b;
    C c;
    D d;

    ASSERT(a.virtual_name() == 'A', MSG);
    ASSERT(b.virtual_name() == 'B', MSG);
    ASSERT(c.virtual_name() == 'C', MSG);
    ASSERT(d.virtual_name() == 'C', MSG); // 继承自 C
    ASSERT(a.direct_name() == 'A', MSG);
    ASSERT(b.direct_name() == 'B', MSG);
    ASSERT(c.direct_name() == 'C', MSG);
    ASSERT(d.direct_name() == 'D', MSG);

    A &rab = b;
    B &rbc = c;
    C &rcd = d;

    ASSERT(rab.virtual_name() == 'B', MSG); // 虚函数表从 A 开始，B 重写了 A 的虚函数
    ASSERT(rbc.virtual_name() == 'C', MSG); // C 重写了 B 的虚函数
    ASSERT(rcd.virtual_name() == 'C', MSG); // D 继承自 C
    ASSERT(rab.direct_name() == 'A', MSG); // 虽然 rab 是 B 类型，但是调用的是 A 的方法
    ASSERT(rbc.direct_name() == 'B', MSG); // 同上
    ASSERT(rcd.direct_name() == 'C', MSG);

    A &rac = c;
    B &rbd = d;

    ASSERT(rac.virtual_name() == 'C', MSG); // C 重写了 B 的虚函数
    ASSERT(rbd.virtual_name() == 'C', MSG); // D 继承自 C
    ASSERT(rac.direct_name() == 'A', MSG); // 调用的是 A 的方法
    ASSERT(rbd.direct_name() == 'B', MSG);

    A &rad = d;

    ASSERT(rad.virtual_name() == 'C', MSG); // D 继承自 C
    ASSERT(rad.direct_name() == 'A', MSG); // 调用的是 A 的方法

    return 0;
}

// READ: 扩展阅读 - 纯虚、抽象 <https://zh.cppreference.com/w/cpp/language/abstract_class>
// READ: 扩展阅读 - 虚继承 <https://zh.cppreference.com/w/cpp/language/derived_class>
