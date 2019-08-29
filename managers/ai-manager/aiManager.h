#ifndef AIMANAGER_H
#define AIMANAGER_H
#include "messages_ai_data_manager.pb.h"
#include "definitions.h"
#include "teamRobot.h"

class AIManager
{
private:
    vector<TeamRobot*> team;

public:
    AIManager(vector<TeamRobot*> _team);

    void readAIData(const AIDataManagerPackage& package);
};

#endif // AIMANAGER_H
