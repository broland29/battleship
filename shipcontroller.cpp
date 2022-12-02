/*
 * CS4085: Skeleton for lab assignment.
 *
 * Copyright (C) 2008 Jacek Rosik <jacek.rosik@ul.ie>
 */


#include <osg/Node>
#include <osg/NodeVisitor>
#include <osgUtil/UpdateVisitor>

#include "ship.h"

#include "shipcontroller.h"
#include <iostream>



ShipController::ShipController(Ship *ship):
    _ship(ship)
{
   assert(ship != NULL);
}

ShipController::~ShipController(void)
{
}


bool
ShipController::handle(const osgGA::GUIEventAdapter &event, osgGA::GUIActionAdapter &action)
{
    /* Only handle KEYDOWN/UP events.  */
    if (!(event.getEventType() & (osgGA::GUIEventAdapter::KEYDOWN | osgGA::GUIEventAdapter::KEYUP)))
        return false;

    /* Activate the engine one key down event.  */
//    bool activate = event.getEventType() == osgGA::GUIEventAdapter::KEYDOWN;

   // std::cout << _ship->getEngineStatus(Ship::RIGHT_ENGINE) << std::endl;

    /* Check which key was pressed and activate the correspoinding engine.  */
    switch (event.getKey()) {
        case osgGA::GUIEventAdapter::KEY_Left:
            _ship->manipulateDirection(Ship::DirectionButton::LEFT);
            break;
        case osgGA::GUIEventAdapter::KEY_Up:
            _ship->manipulateDirection(Ship::DirectionButton::TOP);
            break;
        case osgGA::GUIEventAdapter::KEY_Right:
            _ship->manipulateDirection(Ship::DirectionButton::RIGHT);
            break;
        case osgGA::GUIEventAdapter::KEY_Down:
            _ship->manipulateDirection(Ship::DirectionButton::BOTTOM);
            break;
        case osgGA::GUIEventAdapter::KEY_KP_Add:
        case osgGA::GUIEventAdapter::KEY_Plus:
            std::cout << "+" << std::endl;
            _ship->speedUp();
            break;
        case osgGA::GUIEventAdapter::KEY_KP_Subtract:
        case osgGA::GUIEventAdapter::KEY_Minus:
            std::cout << "-" << std::endl;
            _ship->slowDown();
            break;
        case osgGA::GUIEventAdapter::KEY_C:
            std::cout << "-" << std::endl;
            _ship->stop();
            break;
    }

    return false;
}


/* vi:set tw=78 sw=4 ts=4 et: */
