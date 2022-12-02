/*
 * CS4085: Skeleton for lab assignment.
 *
 * Copyright (C) 2008 Jacek Rosik <jacek.rosik@ul.ie>
 */


#include <cassert>
#include <cmath>

#include <osg/Geode>
#include <osg/Quat>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>

#include "ship.h"
#include <iostream>

#define DIR_CHANGE 0.2
#define DIR_LIMIT 1


// ?
struct BoundingBox
{
    float halfLengthX;
    float halfLengthY;
    float halfLengthZ;
    float offsetX;
    float offsetY;
    float offsetZ;
};

Ship::Ship(osg::MatrixTransform *visual):
    Entity(visual),
    _lastFrame(0.0),
    _velocity(0.0, 0.0, 0.0),
    _direction(1.0, 0.0, 0.0),
    _speed(0),
    _isBouncing(false)
{
    // no active direction at beginning
    _directionButtons[LEFT]     = false;
    _directionButtons[TOP]      = false;
    _directionButtons[RIGHT]    = false;
    _directionButtons[BOTTOM]   = false;
}


Ship *
Ship::create()
{
    osg::ref_ptr<osg::MatrixTransform> visual = new osg::MatrixTransform();

    /* Create visual representation of the ship.  */
    osg::ref_ptr<osg::Shape> shape = new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), SHIP_SIZE);
    //shape->setRotation(osg::Quat(0.5 * M_PI, osg::Vec3(1.0, 0.0, 0.0)));

    osg::ref_ptr<osg::Geode>    geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(shape.get()));
    visual->addChild(geode.get());

    return new Ship(visual.get());
}

/*
    1,0,-1 possible values on x,y,z of _direction
*/
void
Ship::manipulateDirection(DirectionButton db)
{
    switch(db){
        case LEFT:
        /*
            if (_directionButtons[RIGHT]){
                _directionButtons[RIGHT] = false;
            }
            else{
                _directionButtons[LEFT] = true;
            }*/

            if (_direction.x() > -DIR_LIMIT){
                _direction += osg::Vec3(-DIR_CHANGE, 0.0, 0.0);
            }
            break;
        case TOP:
            /*
            if (_directionButtons[BOTTOM]){
                _directionButtons[BOTTOM] = false;
            }
            else{
                _directionButtons[TOP] = true;
            }*/

            if (_direction.y() < DIR_LIMIT){
                _direction += osg::Vec3(0.0, DIR_CHANGE, 0.0);
            }
            break;
        case RIGHT:
            /*
            if (_directionButtons[LEFT]){
                _directionButtons[LEFT] = false;
            }
            else{
                _directionButtons[RIGHT] = true;
            }
*/
            if (_direction.x() < DIR_LIMIT){
                _direction += osg::Vec3(DIR_CHANGE, 0.0, 0.0);
            }
            break;
        case BOTTOM:
  /*
            if (_directionButtons[TOP]){
                _directionButtons[TOP] = false;
            }
            else{
                _directionButtons[BOTTOM] = true;
            }
*/
            if (_direction.y() > -DIR_LIMIT){
                _direction += osg::Vec3(0.0, -DIR_CHANGE, 0.0);
            }
            break;
    }
}

void
Ship::crash(void)
{
    _direction *= -1;
    //_speed = 0;
}

void
Ship::startBouncing(void)
{
    _isBouncing = true;
}

void
Ship::stopBouncing(void)
{
    _isBouncing = false;
}

void
Ship::speedUp(void)
{
    if (_speed < 50){
        _speed += 5;
    }
}

void
Ship::slowDown(void)
{
    if (_speed > -50){
        _speed -= 5;
    }
}
//bool
/*
void
Ship::activateD(Engine engine, bool activate)
{
    // Enable/Disable given engine.
    assert(engine >= LEFT_ENGINE && engine <= BOTTOM_ENGINE);
    _engines[engine] = activate;
}

bool
Ship::getEngineStatus(Engine engine)
{
    assert(engine >= LEFT_ENGINE && engine <= BOTTOM_ENGINE);
    return _engines[engine];
}*/


void
Ship::translate(const osg::Vec3 translation)
{
    osg::Matrix matrix;
    matrix.makeTranslate(translation);
    _visual->postMult(matrix);
}


void
Ship::rotate(double angle)
{
    osg::Matrix matrix;
    matrix.makeRotate(angle, osg::Vec3(0.0, 0.0, 1.0));
    _visual->postMult(matrix);
}

const osg::Vec3
Ship::getPosition(void) const
{
    osg::Vec3 trans, vdummy;
    osg::Quat qdummy;
    _visual->getMatrix().decompose(trans, qdummy, vdummy, qdummy);

    return trans;
}


double
Ship::getOrientation(void) const
{
    double angle;
    osg::Vec3 vdummy;
    osg::Quat rot, qdummy;
    _visual->getMatrix().decompose(vdummy, rot, vdummy, qdummy);
    rot.getRotate(angle, vdummy);

    return angle;
}

void
Ship::onUpdate(double time)
{
    if (doesCrash()){
        printf("Crsh\n");
        crash();
    }
    else{
        printf("No crsh\n");
    }


    double delta = time - _lastFrame;
    _lastFrame = time;

    /*
    // Get the force vector first.    //!!!!
    osg::Vec3 force(0.0, 0.0, 0.0);
    if (_directions[LEFT])
        force += osg::Vec3(5.0, 0.0, 0.0);
    if (_directions[TOP_ENGINE])
        force += osg::Vec3(0.0, -5.0, 0.0);
    if (_directions[RIGHT_ENGINE])
        force += osg::Vec3(-5.0, 0.0, 0.0);
    if (_directions[BOTTOM_ENGINE])
        force += osg::Vec3(0.0, 5.0, 0.0);
    force *= _power;
    */

    /* Update ship's position.  */
    osg::Vec3 position = getPosition();
    float limit = WORLD_SIZE / 2 + SHIP_SIZE;
    float teleport = WORLD_SIZE + 2 * SHIP_SIZE;


    // teleport logic
    //std::cout << position.x() << " " << position.y() << std::endl;
    if (position.x() < -limit){
        //std::cout << " off left " << std::endl;
        translate(osg::Vec3(teleport, 0.0, 0.0));
    }
    if (position.x() > limit){
        //std::cout << " off right " << std::endl;
        translate(osg::Vec3(-teleport, 0.0, 0.0));
    }
    if (position.y() < -limit){
        //std::cout << " off bottom " << std::endl;
        translate(osg::Vec3(0.0, teleport, 0.0));
    }
    if (position.y() > limit){
        //std::cout << " off top " << std::endl;
        translate(osg::Vec3(0.0, -teleport, 0.0));
    }

    // move logic
    // v = d / t => d = v * t
    translate(_velocity * delta);

    if (_direction == osg::Vec3(0.0, 0.0, 0.0)){
        _velocity = osg::Vec3(0.0, 0.0, 0.0);
    }
    else{
        //osg::Vec3 normalized_direction = _direction / _direction.length();
        //_velocity = normalized_direction * _speed;
        _velocity = _direction * _speed;
    }



    /* Caluclate ship's new velocity.  */
    // _velocity += force * delta; velocity modification moved to speedUp and slowDown
    //printf("VELOCITY: %f %f %f\n", _velocity[0], _velocity[1], _velocity[2]);
}


/* vi:set tw=78 sw=4 ts=4 et: */
