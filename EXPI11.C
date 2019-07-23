/* EXPI11.C     A SIMPLE REACTION TIME PROGRAM WHICH DISPLAYS ONE OF THREE
					STIMUI ON THE SCREEN.  THE CHOICE OF WHICH STIMUI IS DISPLAYED
					IS CHOICEN AT RANDOM BY THE GET_STIMUS() FUNCTION.
					THE USER MUST REACT TO A DOUBLE CHACTER STIMUIS WITHIN A TIME
					INTERIAL.
					THE STIMULI ARE CHACTERS WHICH ARE PLACE IN THE SAME
					POSITIONS. THE STIMULI ARE
					 (1)TWO CHACTER SPACED APART ON THE SAME LINE.
					 (2)ONE CHACTER PLACED IN THE SAME LEFT POSITION AS ONE OF THE
						 DOUBLE CHACTER STIMULI.
					 (3)ONE CHACTER PLACED IN THE SAME RIGHT POSITION AS ONE OF THE
						 DOUBLE CHACTER STIMUI.

Author Andrew S, Messser
LAST UPDATE  Nov 2, 1992 3:00 PM
Ver 1.00  Working
*/
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <graph.h>
#include <stdlib.h>      /* used only for rand() func */


/* The getrandom mac uses time.h to seed a real random number generator */

#define getrandom(min, max) ((rand() % (int)(((max)+1)-(min)))+(min))

/*                    FUNCTION PROTO TYPES             */


void help(int state);
void get_stimus(int test);
int testing_phase(void);
int exercise_phase(void);
void pauser(float max_hold);    /* A no frills pause function */
char *stimui_name(int t);
int save(int t);
int print_data(int t);
int data_screen(int t);
int data_menu(void);
void main_menu(void);

 struct default_config                    /* DEFAULT SETUP  */
	{
		float max_time;
		int   max_trial;
		char  ready_prompt[20];
		char  file_name[13];
		char  file_mode[2];
		char  subjest_name[12];
		float   inter_trial_intervial;
		char temp[12];
	}config = {3,32,"  ?", "results.dat", "a","unknown name", 2, "list"};



struct records
{
	char subject_name[12];
	char stimus_name[12];
	int  trial;
	float time;
	char truthness[10];
}recorder[100];

int limitcs2 = 8;
int limitcs1 =8;   /* limit for double star */
int limitlts = 8;
int limitrts = 8;


int last_stimus;     /* 11-2-1993  keeps track off last stimus */
int displace = 10;
int vertical = 0;
int stimus = 0;    /* Important gobal variable define stimui type */
char key= 0;             /* check key pressed */
/* *********************** Start Main Program  ********************* */
main(int argc, char *argv[])
{
char buf[70];         /* char array for printing */



time_t cstart, cend, fixtime;    /* For clock            */
int t, exit_value = 0;           /* real timer exit value for test_phase */
int value =0;

_setvideomode(_TEXTBW80);
_clearscreen(_GWINDOW);

if(argc == 2);
{
	 if(*argv[1]=='?' || argv[1] == "/?")
	 {
		help(2);
	 }
}

main_menu();
do
{
if(exit_value == 5)
{
	main_menu();
}
if(exit_value == 4)
{
	_clearscreen(_GCLEARSCREEN);
}
_displaycursor( _GCURSOROFF );
exercise_phase();

/* get the current time */
/* config.max_trial = config.max_trial - 1;  */
for(t=0; t <= config.max_trial -1 ; ++t)
{
_settextposition(12,35);
_outtext(config.ready_prompt);
pauser(config.inter_trial_intervial); /* pause afther removing stimui from screen */
key = 0;
exit_value = 0;


/* make sure no keys are being pressed */



_clearscreen(_GWINDOW);

	cstart = clock();    /* Use clock for timing to hundredths of seconds */

	exit_value = testing_phase( );
		fflush( stdin );
		key = 0;

	do
	 {

	  if(exit_value != -1)
	  {
		 if(kbhit())
			{

				key = getch();
				fflush( stdin );
				if(key == 32 && stimus != 2)
				 value = -3;   /* add 3/2/93 in place of exit_value = -2 */
				 fixtime = clock();

			}
	  }

		cend = clock();
	 /* make sure the right stimus and space bar press to exit trial */
	 }while ( exit_value !=-2 && exit_value != -1 && !(key == 32 && stimus == 2)  &&
	 ( ( ( (float)cend - cstart ) / CLK_TCK) < config.max_time  ) );

  recorder[t].time = (((float)cend - cstart) / CLK_TCK);

  if (recorder[t].time > config.max_time)
		{
			recorder[t].time = config.max_time;
		}

/* see if correct answer was given or incorrect  */
				if( key == 32 && stimus == 2)   /* space when double */

					strcpy(recorder[t].truthness , "Correct");
				else
				if( key != 32 && stimus == 2 && exit_value != -1)   /* space when double */
					strcpy(recorder[t].truthness , "Incorrect");

				else
				if( exit_value == -1)   /* space when double */
					strcpy(recorder[t].truthness , "Correct");

				else
				if( exit_value == -2) /* value when key press for single */
				strcpy(recorder[t].truthness , "Incorrect");

				/* added 3/2/93  */
				else
				if(exit_value != -1 && value == -3) /* value when key press for single */
				{
				 strcpy(recorder[t].truthness , "Incorrect");

				}

				else
					strcpy(recorder[t].truthness , "Correct");




	 sprintf( buf, "time:  %4.2f seconds  STIMULUS: %s",
		 recorder[t].time, stimui_name(t) );
	 /* _outtext(buf); */



}    /* end of a trial loop */

_displaycursor( _GCURSORON );
save(t);  /* save the data in a file */
exit_value = data_menu(); /* then give them a choices of what to do */
/* reset limit values  for another trial */
limitcs2 = 8;
limitcs1 =8;
limitlts = 8;
limitrts = 8;

}while(exit_value == 5 || exit_value == 4);     /* another? */
_setvideomode(_DEFAULTMODE);
help(1);

/* ************************** end of program ************************** */
}



int testing_phase(void)
{
/* define stimus here */

float holding, max_hold, hold_start;
int key;
max_hold = 1;  /* this holds the display of the stimuis */

get_stimus(1); /*  not a test */

hold_start = clock();
	fflush( stdin );
	key = 0;
    do {
	holding = clock();

	if(kbhit())
	{
		key = getch();
		/* space bar key pressed only when double star stimuii appears */
		if(  (key == 32 && stimus == 2))
		{
		  _clearscreen(_GWINDOW);
		  return(-1);
		}
		else
		  _clearscreen(_GWINDOW);
		  return(-2);
	}




    }while ( ( ( ( (float)holding - hold_start ) / CLK_TCK) < max_hold  )   );
    _clearscreen(_GWINDOW);

return(1);
 }



void get_stimus(int test)
{
/* define stimus here */
/* int stimus; */
/* _clearscreen(_GWINDOW); */
int counter = 0;
int check= 1;   /* just to see if should pick again if test is 1 */

srand( (unsigned)time(NULL) );
do {
	stimus = getrandom(1,4);        /* pick any # from 1 to 4  DEFIND STIMUS!*/

	if (stimus == last_stimus )
	{
	  for(counter = 0; counter <=20; counter++)
	  {
		stimus = getrandom(1,4);        /* pick any # from 1 to 4  DEFIND STIMUS!*/
				if(stimus !=last_stimus)
				{
					break;
				}

	  }
	}
	 if(last_stimus == 2 && (stimus ==2 || stimus == 4) )
	  {
			for(counter = 0; counter <=20; counter++)
			{
				stimus = getrandom(1,4);        /* pick any # from 1 to 4  DEFIND STIMUS!*/
				if(stimus !=2 || stimus !=4)
				{
					break;
				}
			}

	  }

			 if(stimus == 1 && test == 1 && limitlts != 0)
			 {
				limitlts = limitlts - 1;
				check = 0 ;
				break;
			 }

			 if(stimus == 2 && test == 1 && limitcs1 != 0)
			 {
				limitcs1 = limitcs1 - 1;
				check = 0 ;
				break;
			 }

			 if(stimus == 3 && test == 1 && limitrts != 0)
			 {
				limitrts = limitrts - 1;
				check = 0 ;
				break;
			 }
			 if(stimus == 4 && test == 1 && limitcs2 != 0)
			 {
				limitcs2 = limitcs2 - 1;
				check = 0 ;
				stimus = 2;
				break;
			 }

 }while(test == 1  && check == 1 );
last_stimus = stimus;            /* set to last stimus  */

/* Test subject here  return only when subject press esc */
	switch(stimus)
	{

		case 1:

			 _settextposition(12 + vertical,  40 - displace);
			 _outtext("*");
			 break;

		case 2:

			 _settextposition(12 + vertical, 40 - displace);
			 _outtext("*");
			 _settextposition(12 + vertical, 40 + displace);
			 _outtext("*");

			 break;

		case 3:

			 _settextposition(12 + vertical, 40 + displace);
			 _outtext("*");
			 break;

	}

}



int exercise_phase(void)
{
	int key;
	_settextposition(13,8);
	_outtext("Press any key to start practing or press esc to RUN Experiment now\n");
	do
	{
	}while( !kbhit() );   /* wait for respondence */
	fflush( stdin );
	_clearscreen(_GWINDOW);
	get_stimus(0);   /* get the first stimui */
	do
  {

		if(kbhit())
		{
			key = getch();
			/*  go on for double star and space press only */
			if(  (key == 32 && stimus == 2))
			{
			 _clearscreen(_GWINDOW);
			 pauser(config.inter_trial_intervial);
			 get_stimus(0);
			}
			/* go on and get next stimui when stimus is not double star
			and key press is not space or esc */
			if(  (key != 32 && key != 27 && stimus != 2))
			{
			 _clearscreen(_GWINDOW);
			 pauser(config.inter_trial_intervial);
			 get_stimus(0);
			}

		 /* esc key pressed */
			if(  (key == 27 ))
			{
			_clearscreen(_GWINDOW);
			pauser(config.inter_trial_intervial);
			return(1);
			}

		}

    }while(1);
}


void pauser(float max_hold)
{
float hold_start, holding;
hold_start = clock();

	 do {
	holding = clock();               /* current time */

	 }while ( ( ( ( (float)holding - hold_start ) / CLK_TCK) < max_hold  )   );
	 _clearscreen(_GWINDOW);

}

/* defind stimus name for a sp trial */
char *stimui_name(int t)
{


	switch(stimus)
	{

		case 1:
			strcpy(recorder[t].stimus_name, "LEFT STAR");
			break;
		case 2:
			strcpy(recorder[t].stimus_name, "DOUBLE STAR");
			break;

		case 3:
			strcpy(recorder[t].stimus_name, "RIGHT STAR");
			break;
	}

return(recorder[t].stimus_name);
}

int save(int t)   /* SAVE A DATA FILE IN CURRENT DIRECTORY  */
{
	FILE *fp;          /* fp is the file pointer                */

	int i;            /* loop and array counter                */
	int dc;           /* check input data for errors           */
	if((fp=fopen(config.file_name, config.file_mode ))==NULL)
	{
		/* error message if data file cannot be open */
		printf(" disk error!! \n");
		return(1);

	}
	/* IF FILE IS FOUND READ IN DATA UNTIL END OF FILE IS REACHED  */
	for(i=0; i<t; i++)
	{
		fprintf(fp, "%5d %12s %4.2f %12s %10s\n", i+1, recorder[i].stimus_name,
									recorder[i].time,
									config.subjest_name,
									recorder[i].truthness
									);
	}
	fclose(fp);       /* close data file  */
	return(0);
}    /* END OF SAVE DATA FILE  */

int print_data(int t)   /* Print DATA on printer  */
{
	int i;            /* loop and array counter                */
	int dc;           /* check input data for errors           */
	if((stdprn)==NULL)
	{
		/* error message if data file cannot be open */
		printf(" printer error!! \n");
		return(2);

	}
	/* IF printer IS ok READ IN put out DATA UNTIL END OF FILE IS REACHED  */
	for(i=0; i<t+1; i++)
	{
		fprintf(stdprn, "%5d %12s %4.2f %12s %10s\n", i+1, recorder[i].stimus_name,
									recorder[i].time,
									config.subjest_name,
									recorder[i].truthness
									);
	}

	return(0);
}    /* END OF print data  */
/*                   DISPLAY DATA ON SCREEN    */
int data_screen(int t)
{
	int i;            /* loop and array counter                */
	char buff[20];

	_clearscreen(_GCLEARSCREEN);
	sprintf(buff, "%s %s", config.temp, config.file_name);
	system(buff);
	/*
	for(i=0; i<t+1; i++)
	{

		fprintf(stdout, "%5d %12s %4.2f %12s %10s\n", i+1, recorder[i].stimus_name,
									recorder[i].time,
									config.subjest_name,
									recorder[i].truthness
									);
	}
	*/
	return(0);
}    /* END OF print data  */



int data_menu(void)
{
  int answer = 0;
  char anw[13];
  char buffer[80];
  do{
	   _clearscreen(_GWINDOW);
		_settextposition(5,5);
		printf("At this point you may choose any of the following options\n" );
		printf("by just typing in the number of the selection\n");
		printf("                   ***  DATA OPTIONS    ***    \n\n");
		printf("TO PRINT DATA TO PRINTER..............ENTER THE NUMBER...(1) \n");
		printf("TO DISPLAY DATA ON SCREEN.............ENTER THE NUMBER...(2) \n");
		printf("TO  RENAME DATA FILE..................ENTER THE NUMBER...(3) \n\n");
		printf("              *** PROGRAM CONTROL OPTIONS  *** \n\n");
		printf("TO RUN EXPERIMENT WITH SAME OPTIONS...ENTER THE NUMBER...(4) \n");
		printf("TO RUN EXPERIMENT WITH NEW OPTIONS....ENTER THE NUMBER...(5) \n");
		printf("TO EXIT THE PROGRAM AND RETURN TO DOS ENTER THE NUMBER...(6) \n");
		printf("MAKE YOUR SELECTION BY PRESSING THE CORRECT NUMBER KEY THEN ");
		printf("PRESS ENTER\n");
		printf("WAITING FOR YOUR SELECTION: ");
		scanf("%d", &answer);
		switch(answer)
		{
		case 1:
			 print_data(config.max_trial);
			 break;
		case 2:
			 data_screen(config.max_trial);
			 break;
		case 3:
			printf("enter a new file name : ");
			scanf("%s", &anw);
			strcpy(config.file_name, anw);
			save(config.max_trial+1);
			break;
		}
	}while(answer <= 3);

return(answer);
/* answer will only be 4, 5 and 6 */
}

void main_menu(void)
{
	char buffer[80];
	int selection = 10;
  /*   float  max_time;
		int   max_trial;
		char  ready_prompt[20];
		char  file_name[13];
		char  file_mode[2];
		char  subjest_name[12];
  */
  do{


		switch(selection)
		{
		case 1:
			_settextposition(5,60);
			_outtext("ÛÛÛÛÛ");
			_settextposition(5,60);
			scanf("%f", &config.max_time);
			break;

		case 2:
			_settextposition(6,60);
			_outtext("ÛÛÛÛÛ");
			_settextposition(6,60);
		  /* scanf("%d", &config.max_trial); DON'T TAKE INPUT HERE */
			break;

		case 3:
			_settextposition(7,60);
			_outtext("ÛÛÛÛÛÛÛÛÛÛÛÛÛ");
			_settextposition(7,60);
			scanf("%s", &buffer);
			strcpy(config.file_name, buffer);
			break;
		case 4:

			_settextposition(8,60);
			_outtext("ÛÛÛÛÛÛÛÛÛÛÛÛ");
			_settextposition(8,60);
			scanf("%s", &buffer);
			strcpy(config.file_mode, buffer);
			break;

		case 5:
			_settextposition(9,60);
			_outtext("ÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");
			_settextposition(9,60);
			scanf("%s", &buffer);
			strcpy(config.subjest_name, buffer);
			break;

		case 6:
			_settextposition(10,60);
			_outtext("ÛÛÛÛÛÛÛÛÛÛÛÛ");
			_settextposition(10,60);
			scanf("%s", &buffer);
			strcpy(config.ready_prompt, buffer);
			break;

		case 7:
			_settextposition(11,60);
			_outtext("ÛÛÛÛÛÛÛÛÛÛÛÛ");
			_settextposition(11,60);
			scanf("%f", &config.inter_trial_intervial);
			break;

		case 8:
			_settextposition(12,60);
			_outtext("ÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");
			_settextposition(12,60);
			scanf("%s", &buffer);
			strcpy(config.temp, buffer);
			break;


	}

  _clearscreen(_GCLEARSCREEN);
  _settextposition(3,30);
  sprintf(buffer, "PARAMETER MENU ");
  _outtext(buffer);

  _settextposition(5,5);
  sprintf(buffer, "1 MAXIMUM TIME TO RESPONED TO A STIMUII                 %2.2f",
						config.max_time);
  _outtext(buffer);

  _settextposition(6,5);
  sprintf(buffer, "2 THE NUMBER OF TRIALS IS SET AT                       %d",
						config.max_trial);
  _outtext(buffer);

  _settextposition(7,5);
  sprintf(buffer, "3 DATA WILL BY SAVE IN THE FILE NAMED                  %s",
						config.file_name);
  _outtext(buffer);

  _settextposition(8,5);
  sprintf(buffer, "4 THE FILE MODE IS SET AT  (a)ppend or (w)rite over    (%s)",
						config.file_mode);
  _outtext(buffer);
  _settextposition(9,5);
  sprintf(buffer, "5 THE SUBJECT NAME IS                                  %s",
  config.subjest_name);
  _outtext(buffer);

  _settextposition(10,5);
  sprintf(buffer, "6 THE READY PROMPT IS                                  %s",
  config.ready_prompt);
  _outtext(buffer);

  _settextposition(18,5);
  sprintf(buffer, "PLEASE ENTER THE NUMBER OF SELECTION YOU WANT THEN HIT ENTER \n ");
  _outtext(buffer);

  _settextposition(11,5);
  sprintf(buffer, "7 THE INTER TRIAL INTERVAL IS SET AT:                   %4.2f",
  config.inter_trial_intervial);
  _outtext(buffer);

  _settextposition(12,5);
  sprintf(buffer, "8 THE NAME OF THE FILE EDITOR IS :                     %s",
  config.temp);
  _outtext(buffer);

  _settextposition(13,5);
  sprintf(buffer, "9 EXIT PROGRAM");
  _outtext(buffer);

  _settextposition(15,5);
  sprintf(buffer, "10 RUN PROGRAM ");
  _outtext(buffer);

  _settextposition(18,70);
  scanf("%d", &selection);
  if(selection == 9)
  {
  exit(1);
 _setvideomode(_DEFAULTMODE);
  }
 }while(selection <= 9 );
 _clearscreen(_GCLEARSCREEN);
}


void help(int state)
{

	_settextposition(7,15);
	printf("±±±±±±±±±±±±±±±±±±±±±±±±EXPI±±±±±±±±±±±±±±±±±±±±±±±");
	_settextposition(8,15);
	printf("±                                                 ±");
	_settextposition(9,15);
	printf("±  By Andrew Messer                               ±");
	_settextposition(10,15);
	printf("±  This program is written for Beth Levinstein    ±");
	_settextposition(11,15);
	printf("±  Copyright (c) 1993  *Paradigm Technologies*    ±");
	_settextposition(12,15);
	printf("±  PO Box 627; Flushing, NY 11352                 ±");
	_settextposition(13,15);
	printf("±  Call (718) 886-3782 for Support                ±");
	_settextposition(14,15);
	printf("±                                                 ±");
	_settextposition(15,15);
	printf("±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±");
	_settextposition(20,1);

	if(state = 1)
	{
		exit(1);
	}
	else
	{
		return;
	}
}
