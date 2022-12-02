/*
 * CS4085: Skeleton for lab assignment.
 *
 * Copyright (C) 2008 Jacek Rosik <jacek.rosik@ul.ie>
 */


#ifndef __SHIP_H__
#define __SHIP_H__


#include <string>

#include "entity.h"
#include "world.h"

#define SHIP_SIZE 1.0

/**
 * Class representing the space ship.
 */
class Ship: public Entity {

public:
    enum DirectionButton {
        LEFT     = 0,
        TOP      = 1,
        RIGHT    = 2,
        BOTTOM   = 3,
    };

    enum Status {
        FREE                = 0, // no collision, no bouncing
        BOUNCING_HORIZONTAL = 1, // a horizontal collision was detected, no action taken yet
        BOUNCING_VERTICAL   = 2, // a vertical collision was detected, no action taken yet
    };

public:
    virtual ~Ship(void) {}

    static Ship *create(void);

    void manipulateDirection(DirectionButton directionButton);
    void speedUp(void);
    void slowDown(void);
    void crash(void);
    void updateStatus(void);
    void startBouncing(void);
    void stopBouncing(void);
    Status getStatus(void);
    void setStatus(Status newStatus);
    void stop(void);

    void translate(const osg::Vec3 trans);
    void rotate(double angle);

    const osg::Vec3 getPosition(void) const;
    double getOrientation(void) const;


protected:
    Ship(osg::MatrixTransform *visual);

    virtual void onUpdate(double time);

private:
    double _lastFrame;
    bool _isBouncing;
    double _speed;
    bool _directionButtons[4];
    Status _status;
    osg::Vec3 _direction;
    osg::Vec3 _velocity;    // = speed * direction
};


#endif /* __SHIP_H__ */


/* vi:set tw=78 sw=4 ts=4 et: */
