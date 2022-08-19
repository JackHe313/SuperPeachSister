#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class actor : public GraphObject
{
public:
    actor(int ID, double x, double y, int dir = 0, int depth = 0, double size = 1.0, StudentWorld *world = nullptr);
    ~actor(){};
    StudentWorld *getworld() const { return m_world; }
    virtual void doSomething() = 0;
    virtual bool blockingOrNot() const = 0;
    virtual bool DamagableOrNot() const =0;
    bool isOverlap(double x, double y) const;
    virtual void bonk() = 0;
    void setDeath(){aliveState=false;}
    bool aliveOrNot() {return aliveState;}
    virtual void getDamage() {setDeath();} //overide by koopa which will also introduce a shell, and peach

private:
    StudentWorld *m_world;
    bool aliveState;
};

class Peach : public actor
{
public:
    Peach(double x, double y, StudentWorld *world);
    void doSomething();
    bool blockingOrNot() const { return false; }
    bool DamagableOrNot() const { return true; }
    void bonk();
    void Power(int type, bool result);
    bool checkPower(int type);
    void hitpoint2();
    void getDamage(){bonk();}

private:
    double remaining_jump_distance;
    bool jumpPower;
    bool starPower;
    bool shootPower;
    int hitpoint;
    int startime;
    int tempInvi;
    int rechargeTime;
};
class blocking_Objects: public actor
{
public:
    blocking_Objects(int id, double x, double y, StudentWorld *world, int holdtype);
    void doSomething(){};
    bool blockingOrNot() const { return true; }
    bool DamagableOrNot() const { return false; }
    void bonk();
private:
    int holdGoodies;
};

class Block : public blocking_Objects
{
public:
    Block(double x, double y, StudentWorld *world, int holdtype);
};

class Pipe : public blocking_Objects
{
public:
    Pipe(double x, double y, StudentWorld *world, int holdtype);
};

class Portal : public actor
{
public:
    Portal(int id, double x, double y, StudentWorld *world);
    virtual void doSomething()=0;
    bool blockingOrNot() const { return false; }
    bool DamagableOrNot() const { return false; }
    void bonk(){};
};

class Flag: public Portal
{
public:
    Flag(double x, double y, StudentWorld *world);
    void doSomething();
};

class Mario: public Portal
{
public:
    Mario(double x, double y, StudentWorld *world);
    void doSomething();
};

class goodies: public actor
{
public:
    goodies(int Id, double x, double y, StudentWorld *world);
    virtual void doSomething();
    bool blockingOrNot() const { return false; }
    bool DamagableOrNot() const { return false; }
    void bonk(){}; // A goodie object does nothing when bonked
};

class Mushroom : public goodies
{
public:
    Mushroom(double x, double y, StudentWorld *world) ;
    void doSomething();   
};

class Star : public goodies
{
public:
    Star(double x, double y, StudentWorld *world);
    void doSomething();   
};

class Flower : public goodies
{
public:
    Flower(double x, double y, StudentWorld *world);
    void doSomething();
};

class FireBall: public actor
{
public:
    FireBall(int id, double x, double y, int dir, StudentWorld *world);
    virtual void doSomething();
    bool blockingOrNot() const { return false; }
    bool DamagableOrNot() const { return false; }
    void bonk(){}; // A goodie object does nothing when bonked
};

class Peach_Object: public FireBall
{
public: 
    Peach_Object(int id, double x, double y, int dir, StudentWorld *world);
    void doSomething();
};

class Piranha_FireBall : public FireBall
{
public:
    Piranha_FireBall(double x, double y, int dir, StudentWorld *world);
    void doSomething();
};

class shell: public Peach_Object
{
public:
    shell(double x, double y, int dir, StudentWorld *world);
};

class Peach_FireBall: public Peach_Object
{
public:
    Peach_FireBall(double x, double y, int dir, StudentWorld *world);
};

class Monster: public actor
{
public:
    Monster(int id, double x, double y, StudentWorld *world) ;
    virtual void doSomething();
    bool blockingOrNot() const { return false; }
    bool DamagableOrNot() const { return true; }
    virtual void bonk(); 
};

class Goomba: public Monster
{
public:
    Goomba(double x, double y, StudentWorld *world) ;
    void doSomething();
};

class Piranha: public Monster
{
public:
    Piranha(double x, double y, StudentWorld *world);
    void doSomething();
private:
    int fire_delay;
};

class Koopa: public Monster
{
public:
    Koopa(double x, double y, StudentWorld *world);
    void doSomething();
    void bonk();
    void getDamage();
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
