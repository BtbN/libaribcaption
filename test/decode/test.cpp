/*
 * Copyright (C) 2021 magicxqq <xqq@xqq.im>. All rights reserved.
 *
 * This file is part of libaribcaption.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <cstdint>
#include "context.hpp"
#include "caption.hpp"
#include "decoder.hpp"

inline constexpr uint8_t test_data1[] = {
    0x80, 0xff, 0xf0, 0x04, 0x00, 0x00, 0x00, 0x4e, 0x3f, 0x00, 0x00, 0x4a, 0x1f, 0x20, 0x00, 0x00, 0x01, 0x0c,
    0x1f, 0x20, 0x00, 0x00, 0x3f, 0x9b, 0x37, 0x20, 0x53, 0x9b, 0x31, 0x37, 0x30, 0x3b, 0x33, 0x30, 0x20, 0x5f,
    0x9b, 0x36, 0x32, 0x30, 0x3b, 0x34, 0x38, 0x30, 0x20, 0x56, 0x1d, 0x61, 0x9b, 0x33, 0x36, 0x3b, 0x33, 0x36,
    0x20, 0x57, 0x9b, 0x34, 0x20, 0x58, 0x9b, 0x32, 0x34, 0x20, 0x59, 0x8a, 0x87, 0x90, 0x20, 0x44, 0x90, 0x51,
    0x9b, 0x31, 0x37, 0x30, 0x3b, 0x35, 0x30, 0x39, 0x20, 0x61, 0x7d, 0x7a, 0x21, 0x41, 0x4f, 0xf1
};
inline constexpr uint8_t test_data2[] = {
    0x80, 0xff, 0xf0, 0x04, 0x00, 0x00, 0x03, 0x4c, 0x3f, 0x00, 0x03, 0x48, 0x1f, 0x20, 0x00, 0x00, 0x01, 0x0c,
    0x1f, 0x30, 0x00, 0x02, 0x9e, 0x03, 0x41, 0x21, 0x01, 0x01, 0x02, 0x12, 0x24, 0x00, 0x00, 0xf0, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0xf0, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00,
    0x00, 0xf0, 0x0f, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00,
    0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00,
    0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00,
    0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00,
    0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00,
    0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00,
    0x0f, 0x00, 0xf0, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00,
    0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x41, 0x22, 0x01, 0x01, 0x02,
    0x12, 0x24, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0,
    0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f,
    0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f,
    0x00, 0xf0, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00,
    0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00,
    0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00,
    0xf0, 0x0f, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f,
    0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0,
    0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00,
    0xf0, 0x00, 0x41, 0x23, 0x01, 0x01, 0x02, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x00, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x00, 0x00, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x0f, 0xff, 0xff, 0x0f, 0x00,
    0x00, 0x00, 0xf0, 0x0f, 0x0f, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x0f, 0x00, 0x0f, 0x0f, 0x00,
    0x00, 0x00, 0xf0, 0x0f, 0x0f, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0xff, 0xff, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x0f, 0x0f, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0xf0, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0xff, 0x00, 0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xf0, 0x00, 0xff, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0x00, 0x00, 0x9a, 0x9b, 0x37, 0x20, 0x53,
    0x9b, 0x31, 0x37, 0x30, 0x3b, 0x33, 0x30, 0x20, 0x5f, 0x9b, 0x36, 0x32, 0x30, 0x3b, 0x34, 0x38, 0x30, 0x20,
    0x56, 0x1d, 0x61, 0x9b, 0x33, 0x36, 0x3b, 0x33, 0x36, 0x20, 0x57, 0x9b, 0x34, 0x20, 0x58, 0x9b, 0x32, 0x34,
    0x20, 0x59, 0x89, 0x87, 0x90, 0x20, 0x44, 0x90, 0x51, 0x9b, 0x35, 0x31, 0x30, 0x3b, 0x34, 0x31, 0x39, 0x20,
    0x61, 0x1b, 0x2a, 0x20, 0x41, 0x1b, 0x7d, 0xa1, 0x8a, 0x0e, 0x50, 0x35, 0x50, 0x35, 0x43, 0x1b, 0x29, 0x4a,
    0x0e, 0x21, 0x89, 0xa2, 0x88, 0x90, 0x20, 0x40, 0x83, 0x9b, 0x33, 0x37, 0x30, 0x3b, 0x34, 0x34, 0x39, 0x20,
    0x61, 0x1b, 0x29, 0x30, 0x0e, 0x37, 0x69, 0x2d, 0x24, 0x73, 0x20, 0x6a, 0x6a, 0x41, 0x68, 0x89, 0x9b, 0x33,
    0x33, 0x30, 0x3b, 0x35, 0x30, 0x39, 0x20, 0x61, 0x1b, 0x29, 0x4a, 0x0e, 0x28, 0x8a, 0x0f, 0x47, 0x72, 0x35,
    0x34, 0x31, 0x21, 0xa3, 0x21, 0x39, 0x44, 0x33, 0x89, 0x0e, 0x29, 0xa1, 0x8a, 0x1b, 0x29, 0x30, 0x0e, 0x22,
    0x43, 0x0f, 0x21, 0x44, 0x89, 0xa2, 0x3d, 0xe2
};

auto decoder_callback = [](std::unique_ptr<aribcaption::Caption> caption) -> void {
    printf("%s\n", caption->text.c_str());
};

int main(int argc, const char* argv[]) {
    aribcaption::Context context;
    context.SetLogcatCallback([](aribcaption::LogLevel level, const char* message) {
        if (level == aribcaption::LogLevel::kError) {
            fprintf(stderr, "%s\n", message);
        } else {
            printf("%s\n", message);
        }
    });

    aribcaption::Decoder decoder(context);
    decoder.Initialize();

    auto status = decoder.Decode(test_data2, sizeof(test_data2), 0, decoder_callback);
    printf("DecodeStatus: %d\n", status);

    status = decoder.Decode(test_data1, sizeof(test_data1), 0, decoder_callback);
    printf("DecodeStatus: %d\n", status);

    return 0;
}
