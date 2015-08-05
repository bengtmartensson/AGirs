#ifndef RENDERER_H
#define	RENDERER_H

#include "IrSignal.h"

class Renderer {
public:
    Renderer() {};
    //Renderer(const Renderer& orig);
    virtual ~Renderer() {};
    virtual const IrSignal* toSignal() const = 0;
private:

};

#endif	/* RENDERER_H */

