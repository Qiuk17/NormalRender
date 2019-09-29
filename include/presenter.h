#ifndef _PRESENTER_H
#define _PRESENTER_H

#include <image.hpp>
#include "color.h"

class Presenter {
public:
    virtual void init(int w, int h) {}
    virtual void setPixel(int x, int y, const Color& color) = 0;
    virtual void finish(const char* argv) {}
};

class BmpPresenter : public Presenter {
public:
    void init(int w, int h) override;
    void setPixel(int x, int y, const Color& color) override;
    void finish(const char* argv) override;
private:
    Image* pImage;
};

#endif
