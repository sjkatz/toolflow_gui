#ifndef PARSER_H
#define PARSER_H

#include "component.h"

#include <QString>

class Parser
{
public:
    Parser();
    static Component* toComponent(QString file_name);
};

#endif // PARSER_H
