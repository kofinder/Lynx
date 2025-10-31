
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkStream.h"
#include "include/core/SkPaint.h"
#include <memory>

static std::unique_ptr<SkSurface> surface;
static SkCanvas* canvas = nullptr;

extern "C" void init_surface(int width, int height) {
    // SkImageInfo info = SkImageInfo::MakeN32Premul(width, height);
    // surface = SkSurface::MakeRaster(info);
    // canvas = surface->getCanvas();
    // canvas->clear(SK_ColorWHITE);
}

extern "C" void draw_circle(int x, int y, int r) {
    SkPaint paint;
    paint.setColor(SK_ColorBLUE);
    canvas->drawCircle(x, y, r, paint);
}

extern "C" void save_image(const char* path) {
    // auto image = surface->makeImageSnapshot();
    // SkFILEWStream out(path);
    // image->encodeToStream(&out, SkEncodedImageFormat::kPNG, 100);
}