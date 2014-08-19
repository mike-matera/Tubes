/*
 * Show.h
 *
 *  Created on: Jul 30, 2014
 *      Author: cgerstle
 */

#ifndef SHOW_H_
#define SHOW_H_

#include <vector>

namespace tv {

class Show {
public:
    Show(int sec, bool push);
    virtual ~Show();
    void PushLayer(const char *layer);

    std::vector<char*> layers;
    int seconds;
    bool push;
};

} /* namespace tv */

#endif /* SHOW_H_ */
