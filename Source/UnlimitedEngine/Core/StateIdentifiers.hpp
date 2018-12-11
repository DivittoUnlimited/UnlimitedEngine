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
        Game,
        DeathState,
        Pause,
        Settings,
        GameOver,
        Loading,
        HighScoreState,
        GetNameState,
        StartLevelState
	};
}

#endif // STATEIDENTIFIERS_HPP
