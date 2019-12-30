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
        SinglePlayerMenuState,
        QuickBattleSetupState,
        ConquestModeSetup,
        ConquestOverviewState,
        ConquestBattleSetupState,
        ShopState,
        StoryModeSetupState,

        // CutScenes
        IntroCutSceneDialogState,
        cutScene1State,
        cutScene2State,
        cutScene3State,
        cutScene4State,
        cutScene5State,
        cutScene6State,
        cutScene7State,
        cutScene8State,
        cutScene9State,

        // Story Mode Levels
        Level1,
        Level2,
        Level3,
        Level4,
        Level5,
        Level6,
        Level7,
        Level8,
        Level9,
        Level10,

        // Dialogs
        IntroDialogState,
        Chapter1DialogState,
        Chapter2DialogState,
        Chapter3DialogState,
        Chapter4DialogState,
        Chapter5DialogState,
        Chapter6DialogState,
        Chapter7DialogState,
        Chapter8DialogState,
        Chapter9DialogState,


	};
}

#endif // STATEIDENTIFIERS_HPP
