#include "parser.h"

#include <QFileInfo>
#include <QDomDocument>

Parser::Parser()
{
}

Component* Parser::toComponent(QString file_name)
{
    QDomDocument doc("mydocument");
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
        return 0;
    if (!doc.setContent(&file)) {
        file.close();
        return 0;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();
    QDomNodeList nodeList = docElem.elementsByTagName("pin");
    QDomElement rect = docElem.elementsByTagName("rect").at(0).toElement();
    Component* comp1 = new Component(docElem.attribute("name"));
    QRectF rectf(rect.attribute("x").toInt()/10,rect.attribute("y").toInt()/10,rect.attribute("width").toInt()/10,rect.attribute("height").toInt()/10);
    comp1->setRect(rectf);
    //scene->addItem(comp1);
    for(int i=0; i < nodeList.count(); i++)
    {
        QDomNode node = nodeList.at(i);
        QDomElement e = node.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            QDomAttr pol =  e.attributeNode("polarity");
            QDomAttr x =  e.attributeNode("x");
            QDomAttr y =  e.attributeNode("y");
            QDomAttr name =  e.attributeNode("name");
            if(pol.value().compare("Input",Qt::CaseInsensitive) == 0){
                Port* port = new Port( name.value());
                comp1->addPort(*port, (x.value().toInt()+30)/10,y.value().toInt()/10);
            }
            else{
                Port* port = new Port( name.value(),Port::OUT);
                comp1->addPort(*port, (x.value().toInt()-35)/10,y.value().toInt()/10);
            }
        }
    }
    return comp1;
}
