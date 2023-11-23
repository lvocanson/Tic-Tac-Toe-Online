#pragma once

enum GameModeType
{
    CLASSIC,
    FAST
};

struct GameMode 
{
    bool IsTimerOn;
    float PlayerMoveLimitTime;
    unsigned int AlignmentGoal;
    unsigned int TotalRow;
    unsigned int TotalColumn;
};

static const GameMode GAMEMODE_CLASSIC = {false, 0, 3, 3, 3};
static const GameMode GAMEMODE_FAST = {true, 3.f, 3, 3, 3};

class GameSettings
{
public:

    GameSettings();
    ~GameSettings();

    void SetGameMode(GameMode gameMode) { m_ActualGameMode = gameMode; }

    unsigned int GetAlignmentGoal() const { return m_ActualGameMode.AlignmentGoal; }
    unsigned int GetTotalRow() const { return m_ActualGameMode.TotalRow; }
    unsigned int GetTotalColumn() const { return m_ActualGameMode.TotalColumn; }
    float GetPlayerMoveLimitTime() const { return m_ActualGameMode.PlayerMoveLimitTime; }
    bool IsTimerOn() const { return m_ActualGameMode.IsTimerOn; }

private:
    GameMode m_ActualGameMode;

};
