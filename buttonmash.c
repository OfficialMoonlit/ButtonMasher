#include <psx.h>
#include <stdio.h>

unsigned int prim_list[0x4000];
unsigned short padbuf;

volatile int display_is_old = 1;
volatile int time_counter = 0;
volatile int button_counter = 0;
int  dbuf=0;

void prog_vblank_handler() {
	display_is_old = 1;
	time_counter++;
}

int main() {
	printf("PSX init...\n");
	PSX_Init();
	printf("PSX init done.\n");
	printf("GS Init...\n");
	GsInit();
	printf("PSX init done.\n");
	GsSetList(prim_list);
	printf("Clearing VRAM...\n");
	GsClearMem();
	printf("VRAM cleared.\n");
	printf("Setting graphics mode: NTSC...\n");
	GsSetVideoMode(320, 240, EXAMPLES_VMODE);
	printf("Now rendering for NTSC.\n");
	printf("Loading default font...\n");
	GsLoadFont(768, 4, 768, 256);
	printf("Font loaded.\n");
	printf("Init variables...\n");
	SetVBlankHandler(prog_vblank_handler);
	volatile int button_counter = 0;
	volatile int button_score = 0;
	volatile int ready = 1;
	printf("Variables ready.\n");
	printf("Starting main loop...\n");
	while(1) {
		//printf("Loop!\n");
		
		if(display_is_old)	{
			//printf("Rendering...\n");
			dbuf=!dbuf;
			GsSetDispEnvSimple(0, dbuf ? 0 : 256);
			GsSetDrawEnvSimple(0, dbuf ? 256 : 0, 320, 240);
			
			GsPrintFont(5, 5, "Button Masher 1.0 by Moonlit");
			GsPrintFont(5, 25, "When the timer runs down,");
			GsPrintFont(5, 35, "press X to test your reflexes!");
				
			//GsPrintFont(220, 5, "%d", time_counter);
			
			if(!ready){
				button_counter = 0;
				//printf("\n");
				//ready = 1;
			};
						
			if(button_counter >= 60){
				//printf("Countdown: 3\n");
				GsPrintFont(5, 80, "3");
			};
			
			if(button_counter >= 120){
				//printf("Countdown: 2\n");
				GsPrintFont(5, 90, "2");
			};
			
			if(button_counter >= 180){
				//printf("Countdown: 1\n");
				GsPrintFont(5, 100, "1");
			};

			if(button_counter >= 240){
				//printf("Countdown: Go!\n");
				GsPrintFont(5, 110, "Go!");
			};
			
			if(button_counter == 820){
				printf("No button pressed, player was too slow.\n");
				button_counter = 0;
				GsPrintFont(5, 180, "Too slow!");
				GsPrintFont(5, 200, "Press O to reset and try again!");
				ready = 0;
			};
			
			//printf("Reading joypad...\n");
			PSX_ReadPad(&padbuf, NULL);
			if(padbuf & PAD_LEFT)  GsSetVideoMode(640, 240, EXAMPLES_VMODE);;
			if(padbuf & PAD_RIGHT)  GsSetVideoMode(640, 480, EXAMPLES_VMODE);;
			if(padbuf & PAD_UP)  GsSetVideoMode(320, 240, EXAMPLES_VMODE);;
			if(padbuf & PAD_DOWN)  GsSetVideoMode(320, 480, EXAMPLES_VMODE);;
			if(padbuf & PAD_TRIANGLE)  GsPrintFont(70, 180, "TRIANGLE");;
			if(padbuf & PAD_SQUARE)  GsPrintFont(70, 180, "SQUARE");;
			if(padbuf & PAD_CIRCLE){
				printf("Player pressed circle.\n");	
				button_counter = 0;
				button_score = 0;
				GsSortCls(0,0,0);
				ready = 1;
			};
			if(padbuf & PAD_CROSS && ready){
				printf("Player pressed cross.\n");	
				button_score = button_counter;
				if(button_score > 240){
					GsPrintFont(5, 180, "Your reaction time was %d ms!", ((button_score - 240) * 100) / 6);
					GsPrintFont(5, 200, "Press O to reset and try again!");
					button_counter = 0;
					ready = 0;
				}
				else{
					GsPrintFont(5, 180, "Hey, no cheating!");
					GsPrintFont(5, 200, "Press O to reset and try again!");
					button_counter = 0;
					ready = 0;
				};
			}
			else{
				button_counter++;
			};
			
			
			
			GsDrawList();
			while(GsIsDrawing());

			display_is_old=0;
		}
	}

	return 0;
}
