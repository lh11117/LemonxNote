#ifndef SAVE_OPEN_LNBF_H
#define SAVE_OPEN_LNBF_H

#include "canvaswin.h"
#include <QDomDocument>
#include <QXmlStreamWriter>

namespace SAVE_OPEN_LNBF {
    QString save(const Pages&);
    void load(const QString&, Pages&);
}

#endif // SAVE_OPEN_LNBF_H
