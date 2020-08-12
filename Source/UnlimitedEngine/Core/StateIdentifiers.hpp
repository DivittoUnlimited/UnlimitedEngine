#ifndef STATEIDENTIFIERS_HPP
#define STATEIDENTIFIERS_HPP

namespace States
{
	enum ID
	{
		None,
        SplashScreen,
		Title,
		Menu,
        SettingsState,
        BattleState,
        HostGame,
        JoinGame,
        DeathState,
        Pause,
        GameOver,
        Loading,
        HighScoreState,
        GetNameState,
        MessageBox,
        StartLevelState,
        NetworkPause,

        // Iconic States
        GameState // The main play field of the game

	};
}

#endif // STATEIDENTIFIERS_HPP
