/*
 * Show.h
 *
 *  Created on: Jul 30, 2014
 *      Author: cgerstle
 */

#ifndef SHOW_H_
#define SHOW_H_

#include <vector>
#include "Programs/programs.h"

namespace tv {

class Show {
public:
    Show(int sec, Programs::Colorspace colorspace, bool push);
    virtual ~Show();
    void PushLayer(const char *layer);

    std::vector<char*> layers;
    int seconds;
    bool push;
    Programs::Colorspace colorspace;
};

} /* namespace tv */

#endif /* SHOW_H_ */
