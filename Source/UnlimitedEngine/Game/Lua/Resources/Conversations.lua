-- Displayed in MessageBox starting at link 0. The link given with each answer tells where to direct
-- the conversation a link with a negative value ends the conversation.
-- As many characters can be used in dialog as well as response text, as well as any number of
-- responses can be given to the player to choose from. The only limits are how the game displays
-- this information (i.e Pokemon's Text box w/ 4 answers, Bioware's Dialog selection wheel, etc... ).

-- This file might become far to large to be managable. Breaking it up into seperate conversation files
-- will work as long as they are all referenced here.


local textures = require( "Game/Lua/Resources/Assets" ).Textures;

return {

	Map = {
                IntroCutSceneDialog = 0;
                Chapter1            = 1;
                Chapter2            = 2;
                Chapter3            = 3;
                Chapter4            = 4;
                Chapter5            = 5;
                Chapter6            = 6;
                Chapter7            = 7;
                Chapter8            = 8;
                Chapter9            = 9;
                Chapter10           = 10;
	};
	
        -- IntroCutSceneDialog
        IntroCutSceneDialog = {
                { -- Link 0
                        portrait = -1;
                        x = 0;
                        y = 0;
                        dialog = "Can you here me general?";
                        responses = {
                                {
                                        text = "What's going on?";
                                        link = 1;
                                }
                        };
                },
                { -- Link 1
                        portrait = textures.DemoGirl;
                        x = 100;
                        y = 50;
                        dialog = "Your troops were ambushed in the night, we found you barely alive amoung your men.";
                        responses = {
                                {
                                        text = "Any other Survivors?";
                                        link = 2;
                                },
                                {
                                        text = "Who attacked us!?";
                                        link = 3;
                                }
                        };
                },
                { -- Link 2
                        portrait = textures.DemoGirl;
                        x = 100;
                        y = 50;
                        dialog = "No, but we think we know who the attackers were. Our troops staging a counter attack.\nWe need you to command the troops.";
                        responses = {
                                {
                                        text = "I'm ready, Let's go crush those cowards!";
                                        link = -1;
                                }
                        };
                },
                { -- Link 3
                        portrait = textures.DemoGirl;
                        x = 100;
                        y = 50;
                        dialog = "We found the army that attacked you, and we are staging a counter offensive as we speak,\nWe need you to lead us in the battle.";
                        responses = {
                                {
                                        text = "Let's go crush those cowards!";
                                        link = -1;
                                }
                        };
                }
        };
        -- ======================================================================================
        -- ======================================================================================
        -- Chapter1
        Chapter1 = {
            { -- Link 0
                    portrait = textures.DemoGirl;
                    x = 100;
                    y = 50;
                    dialog = "You are about to enter level 2!";
                    responses = {
                            {
                                    text = "Well that's not a very good story...";
                                    link = 1;
                            }
                    };
            },
            { -- Link 1
                    portrait = textures.DemoGirl;
                    x = 100;
                    y = 50;
                    dialog = "Let's see you write a better story then!";
                    responses = {
                            {
                                    text = "Maybe i will...";
                                    link = -1;
                            }
                    };
            }
        };
        -- Chapter2
        Chapter2 = {
            { -- Link 0
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "You are about to enter level 3!";
                responses = {
                       {
                            text = "Well that's not a very good story...";
                            link = 1;
                       }
                };
            },
            { -- Link 1
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "Let's see you write a better story then!";
                responses = {
                        {
                            text = "Maybe i will...";
                            link = -1;
                        }
                };
            }
        };
        -- ========================================================================================
        -- ========================================================================================
        -- Chapter3
        Chapter3 = {
            { -- Link 0
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "You are about to enter level 4!";
                responses = {
                       {
                            text = "Well that's not a very good story...";
                            link = 1;
                       }
                };
            },
            { -- Link 1
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "Let's see you write a better story then!";
                responses = {
                        {
                            text = "Maybe i will...";
                            link = -1;
                        }
                };
            }
        };
        -- ========================================================================================
        -- ========================================================================================
        -- Chapter4
        Chapter4 = {
            { -- Link 0
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "You are about to enter level 5!";
                responses = {
                       {
                            text = "Well that's not a very good story...";
                            link = 1;
                       }
                };
            },
            { -- Link 1
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "Let's see you write a better story then!";
                responses = {
                        {
                            text = "Maybe i will...";
                            link = -1;
                        }
                };
            }
        };
        -- ========================================================================================
        -- ========================================================================================
        -- Chapter5
        Chapter5 = {
            { -- Link 0
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "You are about to enter level 6!";
                responses = {
                       {
                            text = "Well that's not a very good story...";
                            link = 1;
                       }
                };
            },
            { -- Link 1
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "Let's see you write a better story then!";
                responses = {
                        {
                            text = "Maybe i will...";
                            link = -1;
                        }
                };
            }
        };
        -- ========================================================================================
        -- ========================================================================================
        -- Chapter6
        Chapter6 = {
            { -- Link 0
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "You are about to enter level 7!";
                responses = {
                       {
                            text = "Well that's not a very good story...";
                            link = 1;
                       }
                };
            },
            { -- Link 1
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "Let's see you write a better story then!";
                responses = {
                        {
                            text = "Maybe i will...";
                            link = -1;
                        }
                };
            }
        };
        -- ========================================================================================
        -- ========================================================================================
        -- Chapter7
        Chapter7 = {
            { -- Link 0
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "You are about to enter level 8!";
                responses = {
                       {
                            text = "Well that's not a very good story...";
                            link = 1;
                       }
                };
            },
            { -- Link 1
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "Let's see you write a better story then!";
                responses = {
                        {
                            text = "Maybe i will...";
                            link = -1;
                        }
                };
            }
        };
        -- ========================================================================================
        -- ========================================================================================
        -- Chapter8
        Chapter8 = {
            { -- Link 0
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "You are about to enter level 9!";
                responses = {
                       {
                            text = "Well that's not a very good story...";
                            link = 1;
                       }
                };
            },
            { -- Link 1
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "Let's see you write a better story then!";
                responses = {
                        {
                            text = "Maybe i will...";
                            link = -1;
                        }
                };
            }
        };
        -- ========================================================================================
        -- ========================================================================================
        -- Chapter9
        Chapter9 = {
            { -- Link 0
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "You are about to enter level 10!";
                responses = {
                       {
                            text = "Well that's not a very good story...";
                            link = 1;
                       }
                };
            },
            { -- Link 1
                portrait = textures.DemoGirl;
                x = 100;
                y = 50;
                dialog = "Let's see you write a better story then!";
                responses = {
                        {
                            text = "Maybe i will...";
                            link = -1;
                        }
                };
            }
        };
        -- ========================================================================================
        -- ========================================================================================



};
