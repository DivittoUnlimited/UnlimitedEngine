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
        SinglePlayer,
        HostGame,
        JoinGame,
        DeathState,
        Pause,
        Settings,
        GameOver,
        Loading,
        HighScoreState,
        GetNameState,
        MessageBox,
        StartLevelState,
        NetworkPause
	};
}

#endif // STATEIDENTIFIERS_HPP
