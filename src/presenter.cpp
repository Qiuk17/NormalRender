#include "presenter.h"

void BmpPresenter::init(int w, int h) {
    pImage = new Image(w, h);
}

void BmpPresenter::setPixel(int x, int y, const Color& color) {
    pImage->SetPixel(x, pImage->Height() - 1 - y, color.toVec());
}

void BmpPresenter::finish(const char* argv) {
    pImage->SaveBMP(argv);
    delete pImage;
}
