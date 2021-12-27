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

#include <cassert>
#include "renderer/alpha_blend.hpp"
#include "renderer/bitmap.hpp"
#include "renderer/canvas.hpp"
#include "renderer/text_renderer.hpp"

namespace aribcaption {

Canvas::Canvas(Bitmap& target_bitmap) : bitmap_(target_bitmap) {}

Canvas::~Canvas() = default;

void Canvas::SetTextRenderer(TextRenderer& text_renderer) {
    text_renderer_ = &text_renderer;
}

void Canvas::ClearColor(ColorRGBA color) {
    for (int y = 0; y < bitmap_.height(); y++) {
        ColorRGBA* line_begin = bitmap_.GetPixelAt(0, y);
        alphablend::FillLine(line_begin, color, bitmap_.width());
    }
}

void Canvas::ClearRect(ColorRGBA color, const Rect& rect) {
    Rect clipped = Rect::ClipRect(bitmap_.GetRect(), rect);

    if (clipped.width() <= 0 || clipped.height() <= 0) {
        return;
    }

    auto line_width = static_cast<size_t>(clipped.width());

    for (int y = clipped.top; y < clipped.bottom; y++) {
        ColorRGBA* line_begin = bitmap_.GetPixelAt(clipped.left, y);
        alphablend::FillLine(line_begin, color, line_width);
    }
}

void Canvas::DrawRect(ColorRGBA fg_color, const Rect& rect) {
    Rect clipped = Rect::ClipRect(bitmap_.GetRect(), rect);

    if (clipped.width() <= 0 || clipped.height() <= 0) {
        return;
    }

    auto line_width = static_cast<size_t>(clipped.width());

    for (int y = clipped.top; y < clipped.bottom; y++) {
        ColorRGBA* line_begin = bitmap_.GetPixelAt(clipped.left, y);
        alphablend::BlendColorToLine(line_begin, fg_color, line_width);
    }
}

void Canvas::DrawBitmap(const Bitmap& bmp, const Rect& rect) {
    assert(bmp.width() == rect.width() && bmp.height() == rect.height());

    Rect clipped = Rect::ClipRect(bitmap_.GetRect(), rect);

    if (clipped.width() <= 0 || clipped.height() <= 0) {
        return;
    }

    int clip_x_offset = clipped.left - rect.left;
    int clip_y_offset = clipped.top - rect.top;
    auto line_width = static_cast<size_t>(clipped.width());

    for (int y = clipped.top; y < clipped.bottom; y++) {
        ColorRGBA* dest_begin = bitmap_.GetPixelAt(clipped.left, y);
        const ColorRGBA* src_begin = bmp.GetPixelAt(clip_x_offset, clip_y_offset + y - clipped.top);
        alphablend::BlendLine(dest_begin, src_begin, line_width);
    }
}

void Canvas::DrawBitmap(const Bitmap& bmp, int target_x, int target_y) {
    Rect rect{target_x, target_y, target_x + bmp.width(), target_y + bmp.height()};

    DrawBitmap(bmp, rect);
}

auto Canvas::DrawChar(uint32_t ucs4, CharStyle style, ColorRGBA color, ColorRGBA stroke_color,
                      float stroke_width, int char_width, int char_height, int x, int y,
                      std::optional<UnderlineInfo> underline_info) -> TextRenderStatus {
    assert(text_renderer_);
    return text_renderer_->DrawChar(ucs4, style, color, stroke_color,
                                    stroke_width,
                                    char_width,
                                    char_height,
                                    bitmap_,
                                    x,
                                    y,
                                    underline_info);
}

}  // namespace aribcaption
