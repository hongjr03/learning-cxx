#include "../exercise.h"
#include <string.h>
// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        shape[0] = shape_[0], shape[1] = shape_[1], shape[2] = shape_[2], shape[3] = shape_[3];
        size = shape[0] * shape[1] * shape[2] * shape[3];
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法

        // for (int s1 = 0; s1 < shape[0]; s1++) {
        //     int others_s1 = others.shape[0] == 1 ? 0 : s1;
        //     for (int s2 = 0; s2 < shape[1]; s2++) {
        //         int others_s2 = others.shape[1] == 1 ? 0 : s2;
        //         for (int s3 = 0; s3 < shape[2]; s3++) {
        //             int others_s3 = others.shape[2] == 1 ? 0 : s3;
        //             for (int s4 = 0; s4 < shape[3]; s4++) {
        //                 int others_s4 = others.shape[3] == 1 ? 0 : s4;
        //                 int idx = s1 * shape[1] * shape[2] * shape[3] +
        //                           s2 * shape[2] * shape[3] +
        //                           s3 * shape[3] +
        //                           s4;
        //                 int others_idx = others_s1 * others.shape[1] * others.shape[2] * others.shape[3] +
        //                                  others_s2 * others.shape[2] * others.shape[3] +
        //                                  others_s3 * others.shape[3] +
        //                                  others_s4;
        //                 data[idx] += others.data[others_idx];
        //             }
        //         }
        //     }
        // }

        int size = shape[0] * shape[1] * shape[2] * shape[3];

        for (int i = 0; i < size; i++) {
            int idx[4] = {0, 0, 0, 0};
            int j = i;
            for (int k = 3; k >= 0; k--) {
                idx[k] = j % shape[k];
                j /= shape[k];
            }

            int others_idx[4] = {0, 0, 0, 0};
            for (int k = 0; k < 4; k++) {
                others_idx[k] = others.shape[k] == 1 ? 0 : idx[k];
            }

            int idx_ = idx[0] * shape[1] * shape[2] * shape[3] +
                       idx[1] * shape[2] * shape[3] +
                       idx[2] * shape[3] +
                       idx[3];
            int others_idx_ = others_idx[0] * others.shape[1] * others.shape[2] * others.shape[3] +
                              others_idx[1] * others.shape[2] * others.shape[3] +
                              others_idx[2] * others.shape[3] +
                              others_idx[3];
            data[idx_] += others.data[others_idx_];
        }

        return *this;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
