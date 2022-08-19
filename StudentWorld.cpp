#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <sstream>
#include <list>
using namespace std;

GameWorld *createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), lev(assetPath)
{
    complete = false;
    mario_reached = false;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

bool StudentWorld::load()
{
    ostringstream oss;
    int n = getLevel();
    /*if (n < 10)
        oss << "Level0" << n << ".txt";
    else
        oss << "Level" << n << ".txt";*/

    oss.fill('0');
    oss<<"Level"<<setw(2)<<n<< ".txt";
    string level_file = oss.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found || result == Level::load_fail_file_not_found) // load false;
        return false;
    // load success
    return true;
}

bool StudentWorld::isBlockingAt(double x, double y) const
{
    for (list<actor *>::const_iterator p = members.begin(); p != members.end(); p++)
    {
        if ((*(*p)).blockingOrNot() && (*(*p)).isOverlap(x, y))
            return true;
    }
    return false;
}

bool StudentWorld::DamageAt(double x, double y, int type) // type 1 won't attack peach, type 2 will
{
    for (list<actor *>::const_iterator p = members.begin(); p != members.end(); p++)
    {
        if ((*(*p)).DamagableOrNot() && (*(*p)).isOverlap(x, y) && (*(*p)).aliveOrNot())
        {
            if (type == 1)
            {
                if ((*p) != peach_pointer) // don't attack peach
                {
                    (*(*p)).getDamage(); // inform it being damaged
                    increaseScore(100);
                    return true;
                }
            }
            else // type 2
            {
                if ((*p) == peach_pointer) // just attack peach(piranha fireball)
                {
                    (*(*p)).getDamage(); // inform it being damaged
                    return true;
                }
            }
        }
    }
    return false;
}

bool StudentWorld::bonkit(double x, double y) const // won't bonk peach
{
    for (list<actor *>::const_iterator p = members.begin(); p != members.end(); p++)
    {
        if ((*(*p)).isOverlap(x, y) && (*p) != peach_pointer)
        {
            (*(*p)).bonk();
            return true;
        }
    }
    return false;
}

void StudentWorld::Release(int type, double x, double y, int dir)
{
    switch (type)
    {
    case 1: // flower
        members.push_back(new Flower(x, y, this));
        numofm++;
        break;
    case 2: // mushroom
        members.push_back(new Mushroom(x, y, this));
        numofm++;
        break;
    case 3: // star
        members.push_back(new Star(x, y, this));
        numofm++;
        break;
    case 4: // peach fireball
    {
        members.push_back(new Peach_FireBall(x, y, dir, this));
        numofm++;
        break;
    }
    case 5: // Piranha firball
    {
        members.push_back(new Piranha_FireBall(x, y, dir, this));
        numofm++;
        break;
    }
    case 6: // shell
    {
        members.push_back(new shell(x, y, dir, this));
        numofm++;
        break;
    }
    default:
        break;
    }
}

bool StudentWorld::peachlap(double x, double y) const
{
    return peach_pointer->isOverlap(x, y);
}

void StudentWorld::peachpower(int type)
{
    switch (type)
    {
    case 1: // flower
        peach_pointer->Power(1, true);
        break;
    case 2: // mushroom
        peach_pointer->Power(2, true);
        break;
    case 3:
        peach_pointer->Power(3, true);
        break;
    case 4:
        peach_pointer->hitpoint2(); // set hit point to 2;
    }
}

bool StudentWorld::checkStar() const
{
    return peach_pointer->checkPower(3); // 3 for star
}

int StudentWorld::checkPosition(int type) const // type 1=x, type 2=y
{
    switch (type)
    {
    case 1: // x
        return peach_pointer->getX();
        break;

    case 2: // y
        return peach_pointer->getY();
        break;
    default:
        return 0;
        break;
    }
}

int StudentWorld::init()
{
    if (!load()) // level_map loading error
        return GWSTATUS_LEVEL_ERROR;

    // loading successfully
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            Level::GridEntry ge = lev.getContentsOf(i, j);
            double x = i * SPRITE_WIDTH;
            double y = j * SPRITE_HEIGHT;
            switch (ge)
            {
            case Level::block:
                members.push_back(new Block(x, y, this, 0));
                numofm++;
                break;
            case Level::flower_goodie_block:
                members.push_back(new Block(x, y, this, 1));
                numofm++;
                break;
            case Level::mushroom_goodie_block:
                members.push_back(new Block(x, y, this, 2));
                numofm++;
                break;
            case Level::star_goodie_block:
                members.push_back(new Block(x, y, this, 3));
                numofm++;
                break;
            case Level::pipe:
                members.push_back(new Pipe(x, y, this, 0));
                numofm++;
                break;
            case Level::peach:
                peach_pointer = new Peach(x, y, this);
                members.push_back(peach_pointer);
                numofm++;
                break;
            case Level::flag:
                members.push_back(new Flag(x, y, this));
                numofm++;
                break;
            case Level::goomba:
                members.push_back(new Goomba(x, y, this));
                numofm++;
                break;
            case Level::piranha:
                members.push_back(new Piranha(x, y, this));
                numofm++;
                break;
            case Level::koopa:
                members.push_back(new Koopa(x, y, this));
                numofm++;
                break;
            case Level::mario:
                members.push_back(new Mario(x, y, this));
                numofm++;
                break;
            default:
                break;
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    // decLives();
    // return GWSTATUS_PLAYER_DIED;
    list<actor *> died;
    for (list<actor *>::iterator p = members.begin(); p != members.end(); p++)
    {
        if ((*(*p)).aliveOrNot())
        {
            (*(*p)).doSomething();
            if (!peach_pointer->aliveOrNot()) // if died
            {
                playSound(SOUND_PLAYER_DIE);
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
        if (complete)
        {
            complete = false;
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
        if (mario_reached)
        {
            playSound(SOUND_GAME_OVER);
            return GWSTATUS_PLAYER_WON;
        }
    }

    for (list<actor *>::iterator p = members.begin(); p != members.end(); p++)
    {
        if (!(*(*p)).aliveOrNot())
        {
            died.push_back(*p);
            p = members.erase(p);
        }
    }

    for (list<actor *>::iterator p = died.begin(); p != died.end(); ) // delete died actors
    {
        delete (*p);
        p = died.erase(p);
    }

    ostringstream oss;
    oss.fill('0');
    oss << "Lives: " << getLives() << "  Level: " << setw(2)<<getLevel() << "  Points: " << setw(6)<<getScore() << " ";
    if (peach_pointer->checkPower(1))
        oss << " ShootPower!";
    if (peach_pointer->checkPower(2))
        oss << " JumpPower!";
    if (peach_pointer->checkPower(3))
        oss << " StarPower!";
    setGameStatText(oss.str());

    // not complete
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (list<actor *>::iterator p = members.begin(); p != members.end();)
    {
        delete (*p);
        p = members.erase(p);
    }
}
