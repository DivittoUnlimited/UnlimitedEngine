-- Displayed in MessageBox starting at link 0. The link given with each answer tells where to direct the conversation a link with a negative value ends the conversation.
-- As many characters can be used in dialog as well as response text, as well as any number of responses can be given to the player to choose from. The only limits are how the game displays
-- this information (i.e Pokemon's Text box w/ 4 answers, Bioware's Dialog selection wheel, etc... ).

return {
	Map = {
                CowboyBobIntro = 0;
	};
	
        CowboyBobIntro = {
                { -- Link 0
                        dialog = "WWWeeeeelllll' Howdy Partner!!!";
			responses = {
                                {
                                        text = "Well' Howdy!";
                                        link = -1;
                                },
				{
                                        text = "You talkin' to me?";
                                        link = 1;
                                }
                        };
		},
		{ -- Link 1
                        dialog = "yes sir' my names Bob an' i reckon' you look a little lost there patner.";
			responses = {
                                {
                                        text = "I ain't lost i jus' don't like yer face!";
                                        link = 2;
                                },
				{
                                        text = "Actually patner you migh' be right.";
                                        link = 3;
                                }
                        };
		},
                { -- Link 2
                        dialog = "Is tha' so? Draw! ya yella' belly!";
			responses = {
                                {
                                        text = "...";
                                        link = -1;
                                }
                        };
		},
                { -- Link 3
                        dialog = "Well follo' me!";
			responses = {
                                {
                                        text = "Continue...";
                                        link = -1;
                                }
                        };
                }
	};
	
};
