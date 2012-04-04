#ifndef PARSER_H
#define PARSER_H

#include "component.h"
#include "graphicsscene.h"

#include <QString>

class Parser
{
public:
    Parser();
    static Component* toComponent(QString file_name);
    static void fromXML(QString file_name, GraphicsScene* scene);
    static void toMHS(QString file_name, GraphicsScene* scene);
    static void toXML(QString file_name, GraphicsScene* scene);

};

#endif // PARSER_H
