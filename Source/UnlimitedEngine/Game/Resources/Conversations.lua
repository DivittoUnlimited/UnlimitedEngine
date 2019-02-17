return {
	Map = {
		ProffessorDialog1 = 0;
	};
	
	ProffessorDialog1 = {
                { -- Link 0
                        dialog = "Hello There! You look new around here!";
			responses = {
                                {
					text = "Yes, where am i?";
					link = 1;
                                },
				{
					text = "No way! I come here all the time!";
					link = 2;
                                }
                        };
		},
		{ -- Link 1
                        dialog = "This is Greenville, All sorts of strange things happen here.\nThese portals just apeared! Want to check them out?";
			responses = {
                                {
					text = "Sounds great!";
                                        link = 3;
                                },
				{
					text = "No thanks, that sounds WAAAY to dangerous!";
					link = 4;
                                }
                        };
		},
                { -- Link 2
                        dialog = "Really? Well i can't always be right, with the way things always change around here, who knows.\nHave you scene these crazy warp portals?\nWant to try them out with me?";
			responses = {
                                {
                                        text = "Wow those are new!";
                                        link = 3;
                                },
				{
					text = "Actually I invented those.";
					link = 5;
                                }
                        };
		},
                { -- Link 3
                        dialog = "Awesome follow me!";
			responses = {
                                {
                                        text = "Continue...";
                                        link = -1;
                                }
                        };
		},
                { -- Link 4
                        dialog = "It's not dangerous at all watch this!";
			responses = {
                                {
                                        text = "Continue...";
                                        link = -1;
                                }
                        };
                },
                { -- Link 5
                        dialog = "What!? yeah right. Suite yourself i wont show you around.";
                        responses = {
                                {
                                        text = "Continue...";
                                        link = -1;
                                }
                        };
                }
	};
	
};
