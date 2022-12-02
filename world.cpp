/*
 * CS4085: Skeleton for lab assignment.
 *
 * Copyright (C) 2008 Jacek Rosik <jacek.rosik@ul.ie>
 */


#include <osg/Group>

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>

#include "entity.h"
#include "world.h"


World::World(void):
    _root(new osg::Group())
{
}


World::~World(void)
{
}


/**
 * Add entity to the world.
 *
 * The world teakes the ownership of the entity and is responsible for
 * destroying it.
 */
void
World::addEntity(Entity *entity)
{
    _entities.push_back(entity);

    /* Add the visual representation to the scene graph.  */
    _root->addChild(entity->getVisual());
}

// http://code.google.com/p/osgworks/source/browse/trunk/tests/shapes/shapes.cpp?r=415 - this link does not work
// and
// http://osgqsg.googlecode.com/svn-history/r2/trunk/Examples/Picking/PickingMain.cpp - nope, neither does this
void
World::addBarrier(float hx, float hy, float hz, float ox, float oy, float oz)
{
  osg::Box * box = new osg::Box();
  box->setHalfLengths( osg::Vec3(hx, hy, hz) );
  osg::ShapeDrawable * shape = new osg::ShapeDrawable( box );

  osg::Geode * geode = new osg::Geode();
  geode->addDrawable( shape );

  osg::MatrixTransform * transform = new osg::MatrixTransform();
  transform->addChild( geode );
  osg::Matrix m;
  m.makeTranslate( ox, oy, oz ); // offset from centre
  transform->setMatrix( m );

  _root->addChild(transform);
}

/* vi:set tw=78 sw=4 ts=4 et: */
