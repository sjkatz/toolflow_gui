#include "parser.h"

#include <QFileInfo>
#include <QDebug>
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

    QDomElement docElem = doc.documentElement();
    Component* comp1 = new Component(docElem.attribute("name"));

    //###########################################################
    //RECT
    //###########################################################
    QDomElement rect = docElem.elementsByTagName("rect").at(0).toElement();
    QRectF rectf(rect.attribute("x").toInt()/10,rect.attribute("y").toInt()/10,rect.attribute("width").toInt()/10,rect.attribute("height").toInt()/10);
    comp1->setRect(rectf);

    //###########################################################
    //ATTR
    //###########################################################
    QDomNodeList attrList = docElem.elementsByTagName("attr");
    QMap<QString,QVariant> data;
    for(int i=0; i < attrList.count(); i++)
    {
        QDomNode node = attrList.at(i);
        QDomElement e = node.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            data[e.attribute("name")] = e.attribute("value");
        }
    }
    comp1->setData(data);

    //###########################################################
    //PIN
    //###########################################################
    QDomNodeList nodeList = docElem.elementsByTagName("pin");
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
            Port::DataType type;
            if(name.value().contains(QRegExp("\\(\\d*:\\d*\\)")))
                type = Port::STD_LOGIC_VECTOR;
            else
                type = Port::STD_LOGIC;
            if(pol.value().compare("Input",Qt::CaseInsensitive) == 0){
                Port* port = new Port( name.value(), Port::IN, type);
                comp1->addPort(*port, (x.value().toInt()+30)/10,y.value().toInt()/10);
            }
            else{
                Port* port = new Port( name.value(),Port::OUT, type);
                comp1->addPort(*port, (x.value().toInt()-35)/10,y.value().toInt()/10);
            }
        }
    }
    return comp1;
}

void Parser::fromXML(QString file_name, GraphicsScene* scene)
{
    QDomDocument doc("mydocument");
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    //###########################################################
    //COMPONENTS
    //###########################################################
    QDomNodeList compList = docElem.elementsByTagName("Component");
    for(int i=0; i < compList.count(); i++)
    {
        QDomNode node = compList.at(i);
        QDomElement e = node.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            QString x =  e.attribute("x");
            QString y =  e.attribute("y");
            QString name =  e.attribute("name");
            QString id =  e.attribute("id");
            Component* comp1 = new Component(name,QList<Port*>(),id);
            QDomElement rect = e.elementsByTagName("Rect").at(0).toElement();
            QRectF rectf(rect.attribute("x").toInt(),rect.attribute("y").toInt(),rect.attribute("width").toInt(),rect.attribute("height").toInt());
            comp1->setRect(rectf);

            QDomElement properties = e.elementsByTagName("Properties").at(0).toElement();
            QDomNodeList propertyList = properties.elementsByTagName("Property");
            QMap<QString,QVariant> data;
            for(int i=0; i < propertyList.count(); i++)
            {
                QDomNode node = propertyList.at(i);
                QDomElement e = node.toElement(); // try to convert the node to an element.
                if(!e.isNull()) {
                    data[e.attribute("name")] = e.attribute("value");
                }
            }
            comp1->setData(data);

            QDomElement ports = e.elementsByTagName("Ports").at(0).toElement();
            QDomNodeList portList = ports.elementsByTagName("Port");
            for(int i=0; i < portList.count(); i++)
            {
                QDomNode node = portList.at(i);
                QDomElement e = node.toElement(); // try to convert the node to an element.
                if(!e.isNull()) {
                    QString x =  e.attribute("x");
                    QString y =  e.attribute("y");
                    QString name =  e.attribute("name");
                    QString data_type =  e.attribute("data_type");
                    QString direction =  e.attribute("direction");
                    QString id =  e.attribute("id");

                    Port::DataType type;
                    if(name.contains(QRegExp("\\(\\d*:\\d*\\)")))
                        type = Port::STD_LOGIC_VECTOR;
                    else
                        type = Port::STD_LOGIC;
                    if(direction.compare("IN",Qt::CaseInsensitive) == 0){
                        Port* port = new Port( name, Port::IN, type,0,0,id);
                        comp1->addPort(*port, (x.toInt()),y.toInt());
                    }
                    else{
                        Port* port = new Port( name,Port::OUT, type,0,0,id);
                        comp1->addPort(*port, (x.toInt()),y.toInt());
                    }

                }
            }

            scene->addComponent(comp1);
            //comp1->translate(x.toFloat(),y.toFloat());
            comp1->setPos(x.toFloat(),y.toFloat());
            comp1->scale(5,5);
        }
    }

    //###########################################################
    //WIRES
    //###########################################################
    QDomNodeList wireList = docElem.elementsByTagName("Wire");
    for(int i=0; i < wireList.count(); i++)
    {
        QDomNode node = wireList.at(i);
        QDomElement e = node.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            QString c_src =  e.attribute("c_src");
            QString c_dest =  e.attribute("c_dest");
            QString p_src =  e.attribute("p_src");
            QString p_dest =  e.attribute("p_dest");
            QString id =  e.attribute("id");
            if(Component* comp_src = scene->findComponent(c_src))
                if(Component* comp_dest = scene->findComponent(c_dest))
                    if(Port* port_src = comp_src->findPort(p_src))
                        if(Port* port_dest = comp_dest->findPort(p_dest)){
                            Wire* wire = new Wire(port_src, port_dest,id);
                            scene->addWire(wire);

                        }
        }
    }
}

void Parser::toMHS(QString file_name, GraphicsScene* scene)
{
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "PARAMETER VERSION = 2.1.0" << "\n";

//    foreach( Wire* wire, scene->wires())
//    {
//        out << "PORT " << wire->source()->name() << " to " << wire->destination()->name() << "\n";
//    }

    foreach( Component* comp, scene->components())
    {
        out << "\n" << "######################################################################" << "\n";
        out << "BEGIN " << comp->name() << "\n";
        out << "\t" << "PARAMETER INSTANCE = " << comp->name() << comp->id() << "\n";
        out << "\t" << "PARAMETER HW_VER = 1.00.a" << "\n";

        QMapIterator<QString, QVariant> i(comp->data());
        while (i.hasNext()) {
            i.next();
            out << "\t" << "PARAMETER " << i.key() << " = " << i.value().toString() << "\n";
        }

        foreach( Port* port, comp->ports())
        {
            foreach( Wire* wire, scene->wires())
            {
                if (wire->source() == port || wire->destination() == port)
                    out << "\t" << "PORT " << port->name() << " == " << wire->id() << "\n";
            }
        }
        out << "END " << "\n";
    }

    file.close();
}

void Parser::toXML(QString file_name, GraphicsScene* scene)
{
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QDomDocument doc("MyML");
    QDomElement root = doc.createElement("Design");
    doc.appendChild(root);

    //###########################################################
    //WIRES
    //###########################################################
    foreach( Wire* wire, scene->wires())
    {
        QDomElement wire_elem = doc.createElement("Wire");
        root.appendChild(wire_elem);

        wire_elem.setAttribute("c_src",wire->source()->parentComponentId());
        wire_elem.setAttribute("c_dest",wire->destination()->parentComponentId());
        wire_elem.setAttribute("p_src",wire->source()->id());
        wire_elem.setAttribute("p_dest",wire->destination()->id());
        wire_elem.setAttribute("id",wire->id());
    }

    //###########################################################
    //COMPONENTS
    //###########################################################
    foreach( Component* comp, scene->components())
    {
        QDomElement comp_elem = doc.createElement("Component");
        root.appendChild(comp_elem);

        comp_elem.setAttribute("name",comp->name());
        comp_elem.setAttribute("id",comp->id());
        comp_elem.setAttribute("x",comp->scenePos().x());
        comp_elem.setAttribute("y",comp->scenePos().y());

        QDomElement rect_elem = doc.createElement("Rect");
        comp_elem.appendChild(rect_elem);

        rect_elem.setAttribute("x",comp->rect().x());
        rect_elem.setAttribute("y",comp->rect().y());
        rect_elem.setAttribute("width",comp->rect().width());
        rect_elem.setAttribute("height",comp->rect().height());

        if(!comp->data().isEmpty())
        {
            QDomElement properties_elem = doc.createElement("Properties");
            comp_elem.appendChild(properties_elem);

            QMapIterator<QString, QVariant> i(comp->data());
            while (i.hasNext()) {
                i.next();

                QDomElement property_elem = doc.createElement("Property");
                properties_elem.appendChild(property_elem);

                property_elem.setAttribute("name",i.key());
                property_elem.setAttribute("value",i.value().toString());
            }
        }

        if(!comp->ports().isEmpty())
        {
            QDomElement ports_elem = doc.createElement("Ports");
            comp_elem.appendChild(ports_elem);

            foreach( Port* port, comp->ports())
            {
                QDomElement port_elem = doc.createElement("Port");
                ports_elem.appendChild(port_elem);

                port_elem.setAttribute("name",port->name());
                port_elem.setAttribute("direction",port->directionString());
                port_elem.setAttribute("data_type",port->dataTypeString());
                port_elem.setAttribute("id",port->id());
                port_elem.setAttribute("x",port->pos().x());
                port_elem.setAttribute("y",port->pos().y());
            }
        }
    }

    QString xml = doc.toString();
    file.write(xml.toAscii());
    file.close();
}
