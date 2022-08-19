#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <list>
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  bool load();
  bool isBlockingAt(double x, double y) const;
  bool peachlap(double x, double y) const;
  bool DamageAt(double x, double y, int type);
  void peachpower(int type);
  bool checkStar() const;
  int checkPosition(int type) const; //type 1=x, type 2=y
  void setcomplete() {complete=true;}
  void setFinish() {mario_reached=true;}
  bool bonkit(double x, double y) const;
  void bonkpeach() const {peach_pointer->bonk();}
  void Release(int type,double x, double y, int dir=0);

private:
  list<actor*> members;
  int numofm;
  Level lev;
  Peach* peach_pointer;
  bool complete;
  bool mario_reached;
};

#endif // STUDENTWORLD_H_
