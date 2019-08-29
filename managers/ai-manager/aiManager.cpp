#include "aiManager.h"

AIManager::AIManager(vector<TeamRobot*> _team): team(_team)
{}

void AIManager::readAIData(const AIDataManagerPackage &package)
{
    AIRobotPackage robot;
    int robots = package.team_size();
    for(int i = 0 ; i < robots ; i++) {
        robot = package.team(i);
        team[robot.id()]->setAIData(robot);
    }
}
