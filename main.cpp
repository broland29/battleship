/*
 * CS4085: Skeleton for lab assignment.
 *
 * Copyright (C) 2008 Jacek Rosik <jacek.rosik@ul.ie>
 */


#include <cstdlib>

#include <osg/Group>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include "ship.h"
#include "shipcontroller.h"
#include "world.h"

#include <iostream>

struct Barrier
{
    float halfLengthX;
    float halfLengthY;
    float halfLengthZ;
    float offsetX;
    float offsetY;
    float offsetZ;
};

static int noOfBarriers = 0;
static struct Barrier barriers[100];    // not very nice

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"

    static int l_cppfunction(lua_State *L) {
        double arg = luaL_checknumber(L,1);
        lua_pushnumber(L, arg * 0.5);
        return 1;
    }

    static int cppAddBarrier(lua_State *L) {
        /*
        std::cout << "In cppAddBarrier" << std::endl;
        std::cout << lua_tonumber(L, 1) << std::endl;
        std::cout << lua_tonumber(L, 2) << std::endl;
        std::cout << lua_tonumber(L, 3) << std::endl;
        std::cout << lua_tonumber(L, 4) << std::endl;
        std::cout << lua_tonumber(L, 5) << std::endl;
        std::cout << lua_tonumber(L, 6) << std::endl;
        */

        struct Barrier barrier;
        barrier.halfLengthX = lua_tonumber(L, 1);
        barrier.halfLengthY = lua_tonumber(L, 2);
        barrier.halfLengthZ = lua_tonumber(L, 3);
        barrier.offsetX = lua_tonumber(L, 4);
        barrier.offsetY = lua_tonumber(L, 5);
        barrier.offsetZ = lua_tonumber(L, 6);

        barriers[noOfBarriers++] = barrier;

        return 0;   // no values pushed on stack
    }


}


// store ship when initialized
osg::ref_ptr<Ship> ship;

float shipX, shipY, shipW, shipH, barX, barY, barW, barH;
// https://developer.mozilla.org/en-US/docs/Games/Techniques/2D_collision_detection
bool Ship::doesCrash(){
    //osg::Vec3 position = Ship.getPosition();
    // lots of reverse engineering going on here
    Ship *_ship = ship.get();
    osg::Vec3 shipPosition = _ship->getPosition();
    //printf("VELOCITY: %f %f %f\n", shipPosition[0], shipPosition[1], shipPosition[2]);
    World *world = World::instance();

    // we only work with x, y
    // coordinates of ship's bounding box
    // x, y, z: middle of the bounding box
    shipX = shipPosition[0]; //+ SHIP_SIZE / 2;
    shipY = shipPosition[1]; //+ SHIP_SIZE / 2;
    // width, height of ship's bounding box
    shipW = SHIP_SIZE;
    shipH = SHIP_SIZE;

    printf("x: %f | y:%f | ", shipX, shipY);

    for(int i = 0; i < noOfBarriers; i++){
        // coordinates of barrier
        // x, y, z: middle of the barrier
        barX = barriers[i].offsetX - barriers[i].halfLengthX;
        barY = barriers[i].offsetY - barriers[i].halfLengthY;
        // width, height of ship's bounding box
        barW = barriers[i].halfLengthX * 2;
        barH = barriers[i].halfLengthY * 2;

        if(
            shipX < barX + barW  &&
            shipX + shipW > barX &&
            shipY < barY + barH  &&
            shipH + shipY > barY
        ){
            if (_ship->_isBouncing){
                _ship->startBouncing();
                return false;
            }
            _ship->startBouncing();
            return true;    // cont
        }
    }

    _ship->stopBouncing();
    return false;
}


/**
 * Initialise the display window camera and projections.
 */
void
initViewer(osgViewer::Viewer &viewer)
{
    /* This will set up a square window (app runs fullscreen by default).  */
    viewer.setUpViewInWindow(0, 0, WINDOW_SIZE, WINDOW_SIZE);

    /* This will create rendering context windows camreras etc... */
    viewer.realize();

    /* Set up the camera that it is showing the whole world and does not
     * react to window resizes. The camera is looking at XY plane from above.  */
    osg::ref_ptr<osg::Camera> camera =  viewer.getCamera();
    camera->setProjectionResizePolicy(osg::Camera::FIXED);
    camera->setProjectionMatrixAsOrtho2D(-0.5 * WORLD_SIZE, 0.5 * WORLD_SIZE,
                                         -0.5 * WORLD_SIZE, 0.5 * WORLD_SIZE);
    camera->setViewMatrixAsLookAt(osg::Vec3(0.0, 0.0, 0.0),
                                  osg::Vec3(0.0, 0.0, -1.0),
                                  osg::Vec3(0.0, 1.0, 0.0));
    /* This will disable the manipulator which is useless for us.  */
    camera->setAllowEventFocus(false);
}



/**
 * Create the game world and underlying scene graph.
 */
void
initGameWorld(osgViewer::Viewer &viewer, char fileName[])
{
    World *world = World::instance();

    /* Attach the scene graph to viewer.  */
    viewer.setSceneData(world->getSceneGraph());

    /* Add our space ship to the world.  */
    //osg::ref_ptr<Ship> ship = Ship::create();
    ship = Ship::create();
    world->addEntity(ship.get());

    /* Set up keboard event handler to controll the ship.  */
    viewer.addEventHandler(new ShipController(ship.get()));


    // very good advice here:
    //http://rubenlaguna.com/wp/2012/11/26/first-steps-lua-cplusplus-integration/

    /* lua_pcall arguments:
        - lua state
        - number of arguments
        - number of return values
        - extended debug info (not used)
    */

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_loadfile(L, "setup.lua")){
        std::cerr << "Loading lua file failed:" << std::endl;
        std::cerr << "   " << lua_tostring(L, -1) << std::endl;
        lua_pop(L,1);
        lua_close(L);
        return;
    }

    // also part of initialization (?)
    if (lua_pcall(L, 0, LUA_MULTRET, 0)) {
        std::cerr << "Loading lua file failed (2):" << std::endl;
        std::cerr << "   " << lua_tostring(L, -1) << std::endl;
        lua_pop(L,1);
        lua_close(L);
        return;
    }


/*    if (lua_pcall(L, 0, LUA_MULTRET, 0)) {   // pcall: lua state, number of arguments, number of return values, extended debug info (not used)
        std::cerr << "Error (2):" << std::endl;
        std::cerr << lua_tostring(L, -1) << std::endl;
        lua_pop(L,1);
    }

    lua_getglobal(L, "reply");
    char const *luaReply = lua_tostring(L,-1);  // free? error handle?
    lua_pop(L,1);
    std::cout << "Reply: " << luaReply << std::endl;

    lua_getglobal(L, "myluafunction");
    lua_pushstring(L, "fileName");
    lua_pcall(L, 1, 1, 0);
    std::cout << "Lua return value: " << lua_tostring(L, -1) << std::endl;
    lua_pop(L,1);

    lua_pushcfunction(L,l_cppfunction);
    lua_setglobal(L, "cppfunction");

    lua_getglobal(L, "myfunction");
    lua_pushnumber(L, 5);
    lua_pcall(L, 1, 1, 0);
    std::cout << "The return value of the function was " << lua_tonumber(L, -1) << std::endl;
    lua_pop(L,1);

*/


    // register function cppAddBarrier
    lua_pushcfunction(L, cppAddBarrier);
    lua_setglobal(L, "cppAddBarrier");

    // call lua function parseFile
    if (lua_getglobal(L, "parseFile") == LUA_TFUNCTION){

        // transmit file name
        lua_pushstring(L, fileName);
        lua_setglobal(L, "fileName");


        if (lua_pcall(L,0, LUA_MULTRET, 0) == LUA_OK) {
            std::cout << "Pushed fileName on Lua stack with success." << std::endl;
        }
        else {
            std::cerr << "Pushed fileName on Lua stack with failure." << std::endl;
            std::cerr << "   " << lua_tostring(L, -1) << std::endl;     // print error message
            lua_pop(L,1);                                               // pop error message
        }

        // error handling for this pcall returned "error in error handling" and other very interesting messages
        lua_pcall(L, 0, 1, 0);

        int returnValue = lua_tointeger(L, -1); // already "over" the return value in stack
        if (returnValue == 0){
            std::cout << "Lua function returned with success." << std::endl;
        }
        else {
            std::cout << "Lua function returned with failure." << std::endl;
        }

        // pop return value from stack
        lua_pop(L,1);
    }
    else {
        std::cerr << "Lua function not found." << std::endl;
    }

    for(int i = 0; i < noOfBarriers; i++){
        world->addBarrier(barriers[i].halfLengthX, barriers[i].halfLengthY, barriers[i].halfLengthZ,
            barriers[i].offsetX, barriers[i].offsetY, barriers[i].offsetZ);
    }

    lua_close(L);
}


/**
 * MAIN
 */
int
main(int argc, char *argv[])
{
    /* Create and initialise the viewer.  */
    osgViewer::Viewer viewer;
    initViewer(viewer);

    /* Create the scene.  */
    if (argc == 1){
        std::cerr << "No file name specified." << std::endl;
    }
    initGameWorld(viewer, argv[1]);

    /* Enter the event processing loop.  */
    return  viewer.run();
}


/* vi:set tw=78 sw=4 ts=4 et: */
