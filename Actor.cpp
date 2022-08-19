#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

actor::actor(int ID, double x, double y, int dir, int depth, double size, StudentWorld *world)
    : GraphObject(ID, x, y, dir, depth, size), aliveState(true)
{
    m_world = world;
}

blocking_Objects::blocking_Objects(int id, double x, double y, StudentWorld *world, int holdtype)
    : actor(id, x, y, 0, 2, 1, world),holdGoodies(holdtype)
{

}
Block::Block(double x, double y, StudentWorld *world, int holdtype)
    : blocking_Objects(IID_BLOCK, x, y, world, holdtype) 
{
}

Pipe::Pipe(double x, double y, StudentWorld *world, int holdtype)
    : blocking_Objects(IID_PIPE, x, y, world, holdtype) 
{
}

Portal::Portal(int id, double x, double y, StudentWorld *world)
    : actor(id, x, y, 0, 1, 1, world)
{
}

Flag::Flag(double x, double y, StudentWorld *world)
    : Portal(IID_FLAG, x, y, world)
{
}

Mario::Mario(double x, double y, StudentWorld *world)
    : Portal(IID_MARIO, x, y, world)
{
}

Peach::Peach(double x, double y, StudentWorld *world)
    : actor(IID_PEACH, x, y, 0, 0, 1, world)
{
    remaining_jump_distance = 0.0;
    jumpPower = false;
    hitpoint = 1;
    starPower = false;
    startime = 0;
    tempInvi = 0;
    shootPower = false;
    rechargeTime = 0;
}

goodies ::goodies(int ID, double x, double y, StudentWorld *world)
    : actor(ID, x, y, 0, 1, 1, world)
{
}
Mushroom ::Mushroom(double x, double y, StudentWorld *world)
    : goodies(IID_MUSHROOM, x, y, world)
{
}

Star ::Star(double x, double y, StudentWorld *world)
    : goodies(IID_STAR, x, y, world)
{
}

Flower ::Flower(double x, double y, StudentWorld *world)
    : goodies(IID_FLOWER, x, y, world)
{
}

FireBall ::FireBall(int id, double x, double y, int dir, StudentWorld *world)
    : actor(id, x, y, dir, 1, 1, world)
{
}

Peach_Object::Peach_Object(int id, double x, double y, int dir, StudentWorld *world)
    : FireBall(id, x, y, dir, world)
{
}

Peach_FireBall::Peach_FireBall(double x, double y, int dir, StudentWorld *world)
    : Peach_Object(IID_PEACH_FIRE, x, y, dir, world)
{
}

shell::shell(double x, double y, int dir, StudentWorld *world)
    : Peach_Object(IID_SHELL, x, y, dir, world)
{
}

Piranha_FireBall::Piranha_FireBall(double x, double y, int dir, StudentWorld *world)
    : FireBall(IID_PIRANHA_FIRE, x, y, dir, world)
{
}

Monster::Monster(int id, double x, double y, StudentWorld *world)
    : actor(id, x, y, 180 * randInt(0, 1), 0, 1, world)
{
}

Goomba::Goomba(double x, double y, StudentWorld *world)
    : Monster(IID_GOOMBA, x, y, world)
{
}

Piranha::Piranha(double x, double y, StudentWorld *world)
    : Monster(IID_PIRANHA, x, y, world), fire_delay(0)
{
}

Koopa::Koopa(double x, double y, StudentWorld *world)
    : Monster(IID_KOOPA, x, y, world)
{
}

void Peach::Power(int type, bool result)
{
    switch (type)
    {
    case 1: // flower
        shootPower = result;
        break;
    case 2: // mushroom
        jumpPower = result;
        break;
    case 3: // star
        starPower = result;
        if (result)
            startime = 150;
        else
            startime = 0;
        break;
    }
}

bool Peach::checkPower(int type)
{
    switch (type)
    {
    case 1: // flower
        return shootPower;
    case 2: // mushroom
        return jumpPower;
    case 3: // star
        return starPower;
    default:
        return false;
    }
}

void Peach::hitpoint2()
{
    hitpoint = 2;
}

void Peach::doSomething()
{
    int key;
    StudentWorld *w = getworld();

    if (!aliveOrNot()) // dead
        return;

    if (starPower) // have invincibility
    {
        startime--; // decrement star reaminingtime
        if (startime == 0)
            starPower = false;
    }
    if (tempInvi > 0) // have temporary invincibility
        tempInvi--;

    if (rechargeTime > 0)
        rechargeTime--;

    w->bonkit(getX(), getY());

    if (remaining_jump_distance > 0) // if jumping right now
    {
        if (!w->isBlockingAt(getX(), getY() + 4))
        {
            moveTo(getX(), getY() + 4);
            remaining_jump_distance--;
        }
        else
        {
            remaining_jump_distance = 0;
            w->bonkit(getX(), getY() + 4);
        }
    }
    else // not jumping thus check whether is falling
    {
        if (!w->isBlockingAt(getX(), getY()) && !w->isBlockingAt(getX(), getY() - 1) && !w->isBlockingAt(getX(), getY() - 2) && !w->isBlockingAt(getX(), getY() - 3)) // no block below
            moveTo(getX(), getY() - 4);
    }

    if (w->getKey(key))
    {
        // user hit a key
        switch (key)
        {
        case KEY_PRESS_LEFT:
            setDirection(180);
            // determine whether blocking the way
            if (!w->isBlockingAt(getX() - 4, getY()))
                moveTo(getX() - 4, getY());
            break;
        case KEY_PRESS_RIGHT:
            setDirection(0);
            // determine whether blocking the way
            if (!w->isBlockingAt(getX() + 4, getY()))
                moveTo(getX() + 4, getY());
            break;
        case KEY_PRESS_UP:
            if (w->isBlockingAt(getX(), getY() - 1))
            {
                if (jumpPower)
                    remaining_jump_distance = 12;
                else
                    remaining_jump_distance = 8;
                w->playSound(SOUND_PLAYER_JUMP);
            }
            break;
        case KEY_PRESS_SPACE:
            if (shootPower && rechargeTime == 0) // if have shoot power and not in reloading time
            {
                w->playSound(SOUND_PLAYER_FIRE);
                rechargeTime = 8;
                int nx = getX();
                if (getDirection() == 0) // face toward right
                    nx += 4;
                else // face left
                    nx -= 4;
                if (!w->isBlockingAt(nx, getY()))
                {
                    w->Release(4, nx, getY(), getDirection());
                }
            }
        }
    }
}

void Peach::bonk()
{
    StudentWorld *w = getworld();
    if ((!starPower) && tempInvi == 0) // no invincibility
    {
        hitpoint--;
        tempInvi = 10;
        if (shootPower)
            shootPower = false;
        if (jumpPower)
            jumpPower = false;

        if (hitpoint >= 1)
            w->playSound(SOUND_PLAYER_HURT);
        else // died
            setDeath();
    }
}

void Flag::doSomething()
{
    StudentWorld *w = getworld();
    if (w->peachlap(getX(), getY()))
    {
        w->increaseScore(1000);
        w->setcomplete();
    }
}

void Mario::doSomething()
{
    StudentWorld *w = getworld();
    if (w->peachlap(getX(), getY()))
    {
        w->increaseScore(1000);
        w->setFinish();
    }
}

void goodies::doSomething()
{
    StudentWorld *w = getworld();
    if (!w->isBlockingAt(getX(), getY() - 2)) // if no object block it falling two blocks down
        moveTo(getX(), getY() - 2);
    if (getDirection() == 0) // right
    {
        if (w->isBlockingAt(getX() + 2, getY()))
        {
            setDirection(180);
            return;
        }
        else
            moveTo(getX() + 2, getY());
    }
    else // left
    {
        if (w->isBlockingAt(getX() - 2, getY()))
        {
            setDirection(0);
            return;
        }
        else
            moveTo(getX() - 2, getY());
    }
}

void Mushroom::doSomething()
{
    StudentWorld *w = getworld();
    if (w->peachlap(getX(), getY())) // overlap with peach
    {
        w->increaseScore(75);
        w->peachpower(2); // give jumppower
        w->peachpower(4); // set hitpoint 2;
        setDeath();
        w->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    else
        goodies::doSomething();
}

void Star::doSomething()
{
    StudentWorld *w = getworld();
    if (w->peachlap(getX(), getY())) // overlap with peach
    {
        w->increaseScore(100);
        w->peachpower(3); // give starpower
        w->peachpower(4); // set hitpoint 2;
        setDeath();
        w->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    else
        goodies::doSomething();
}

void Flower::doSomething()
{
    StudentWorld *w = getworld();
    if (w->peachlap(getX(), getY())) // overlap with peach
    {
        w->increaseScore(50);
        w->peachpower(1); // give shootpower
        w->peachpower(4); // set hitpoint 2;
        setDeath();
        w->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    else
        goodies::doSomething();
}

bool actor::isOverlap(double x, double y) const
{
    if ((x + SPRITE_WIDTH - 1 >= this->getX() && x + SPRITE_WIDTH - 1 <= this->getX() + SPRITE_WIDTH - 1) || (x >= this->getX() && x <= this->getX() + SPRITE_WIDTH - 1)) // in width range
    {
        if ((y + SPRITE_HEIGHT - 1 >= this->getY() && y + SPRITE_HEIGHT - 1 <= this->getY() + SPRITE_HEIGHT - 1) || (y >= this->getY() && y <= this->getY() + SPRITE_HEIGHT - 1))
            return true;
    }
    return false;
}

void blocking_Objects::bonk()
{
    StudentWorld *w = getworld();
    if (holdGoodies != 0)
    {
        w->Release(holdGoodies, getX(), getY() + 8);
        w->playSound(SOUND_POWERUP_APPEARS);
    }
    else
    {
        w->playSound(SOUND_PLAYER_BONK);
    }
    holdGoodies = 0;
}

void Peach_Object::doSomething()
{
    StudentWorld *w = getworld();
    if (w->DamageAt(getX(), getY(), 1)) // if true, set the itme to death;  type=1 for not hurting peach
    {
        setDeath();
        return;
    }
    else
        FireBall::doSomething(); // move;
}
void Piranha_FireBall::doSomething()
{
    StudentWorld *w = getworld();
    if (w->DamageAt(getX(), getY(), 2)) // if true, set the itme to death;  type=2 for just hurting peach
    {
        setDeath();
        return;
    }
    else
        FireBall::doSomething(); // move;
}

void FireBall::doSomething()
{
    StudentWorld *w = getworld();
    if (!w->isBlockingAt(getX(), getY() - 2)) // if no object block it falling two blocks down
        moveTo(getX(), getY() - 2);
    if (getDirection() == 0) // right
    {
        if (w->isBlockingAt(getX() + 2, getY())) // touch the wall
        {
            setDeath();
            return;
        }
        else
            moveTo(getX() + 2, getY());
    }
    else // left
    {
        if (w->isBlockingAt(getX() - 2, getY())) // touch the wall
        {
            setDeath();
            return;
        }
        else
            moveTo(getX() - 2, getY());
    }
}

void Monster::doSomething() // attack peach and move
{
    // alive
    StudentWorld *w = getworld();
    if (w->peachlap(getX(), getY())) // if overlap with peach
    {
        w->bonkpeach();
        return;
    }

    if (getDirection() == 0) // right
    {
        if (w->isBlockingAt(getX() + 1, getY()))
            setDirection(180);
        else if (!w->isBlockingAt(getX() + 1 + SPRITE_WIDTH - 1, getY() - 1)) // move right, check if its right side above some block
            setDirection(180);

        if (getDirection() == 0) // right
        {
            if (!w->isBlockingAt(getX() + 1, getY()) ) // movable
                moveTo(getX() + 1, getY());
            else
                return;
        }
        else // left
        {
            if (!w->isBlockingAt(getX() - 1, getY())) // movable
                moveTo(getX() - 1, getY());
            else
                return;
        }
    }
    else // left
    {
        if (w->isBlockingAt(getX() - 1, getY()))
            setDirection(0);
        else if (!w->isBlockingAt(getX() - 1 - SPRITE_WIDTH + 1, getY() - 1)) // move right, check if its right side above some block
            setDirection(0);

        if (getDirection() == 0) // right
        {
            if (!w->isBlockingAt(getX() + 1, getY())) // movable
                moveTo(getX() + 1, getY());
            else
                return;
        }
        else // left
        {
            if (!w->isBlockingAt(getX() - 1, getY())) // movable
                moveTo(getX() - 1, getY());
            else
                return;
        }
    }
    return;
}

void Goomba::doSomething()
{
    if (!aliveOrNot())
        return;
    else
        Monster::doSomething();
}

void Koopa::doSomething()
{
    if (!aliveOrNot())
        return;
    else
        Monster::doSomething();
}

void Piranha::doSomething()
{
    if (!aliveOrNot())
        return;
    increaseAnimationNumber();

    StudentWorld *w = getworld();
    if (w->peachlap(getX(), getY())) // if overlap with peach
    {
        w->bonkpeach();
        return;
    }

    if (w->checkPosition(2) <= (getY() + SPRITE_HEIGHT * 1.5) && w->checkPosition(2) >= (getY() - SPRITE_HEIGHT * 1.5)) // in the same level
    {
        if (w->checkPosition(1) < getX()) // left
            setDirection(180);
        else
            setDirection(0);

        if (fire_delay > 0)
        {
            fire_delay--;
            return;
        }
        else
        {
            if ((w->checkPosition(1) - getX() >= 0 && w->checkPosition(1) - getX() < 8 * SPRITE_WIDTH) || (getX() - w->checkPosition(1) >= 0 && getX() - w->checkPosition(1) < 8 * SPRITE_WIDTH)) // in shooting range
            {
                w->Release(5, getX(), getY(), getDirection());
                w->playSound(SOUND_PIRANHA_FIRE);
                fire_delay = 40;
            }
        }
    }
    else
        return;
}

void Monster::bonk()
{
    StudentWorld *w = getworld();
    if (w->checkStar())
    {
        w->playSound(SOUND_PLAYER_KICK);
        w->increaseScore(100);
        setDeath();
    }
}

void Koopa::bonk()
{
    StudentWorld *w = getworld();
    if (w->checkStar())
    {
        w->playSound(SOUND_PLAYER_KICK);
        w->increaseScore(100);
        setDeath();
        w->Release(6, getX(), getY(), getDirection());
    }
}

void Koopa::getDamage()
{
    StudentWorld *w = getworld();
    w->increaseScore(100);
    setDeath();
    w->Release(6, getX(), getY(), getDirection());
}

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
