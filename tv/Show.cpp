/*
 * Show.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cgerstle
 */

#include "Show.h"

namespace tv {

Show::Show(int sec, Programs::Colorspace colorspace, bool push) {
    this->seconds = sec;
    this->push = push;
    this->colorspace = colorspace;
}

Show::~Show() {
}

void Show::PushLayer(const char *layer) {
    layers.push_back((char *)layer);
}

} /* namespace tv */
