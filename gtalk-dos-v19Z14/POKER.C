#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <dir.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "jumptbl.h"
#include "glmdef.h"

struct startblock ourblock =
{
  LD_STARTID,                   /* startup string */
  0,                            /* glm version number */
  sizeof(struct startblock),    /* size of header */
  "MEBOT",                      /* name of glm */
  CAN_BE_SHARED,                /* shared or non-shared */
  0                             /* dummy location to */
                                /* fill in length */
};

#define NO_CARD -1
#define NUM_CARDS 52
#define MAX_PLAYERS 6

#define TOKEN_LENGTH 4
#define WORD_TOKEN 1
#define NUMBER_TOKEN 2
#define CARD_TOKEN 3
#define NUM_TOKENS 20


/* GAME RUN LEVELS */

#define NOT_RUNNING     0
#define DEAL_ROUND      1
#define ANTE_ROUND      2
#define FIRST_BET_ROUND 3
#define DISCARD_ROUND   4
#define FINAL_BET_ROUND 5
#define GAME_COMPLETE   6


char near *run_levels[GAME_COMPLETE+1] = { "wait","deal","ante","bet","discard","bet","complete" };
#define run_levels  (char far *)run_levels

char info_line[GLM_INFO_LINE_LEN+1];

const char private_poker_header[40];
const char public_poker_header[40];

signed char card_deck[NUM_CARDS];

char s[500];

struct players_struct
{
  unsigned long int money;
  unsigned long int credit;
  char handle[HANDLE_SIZE+1];
  int user_number;
  int portnum;
  unsigned int pid;
  signed char cards_in_hand[5];
  char have_cards;
  unsigned long int current_bet;
  int run_level;
  int warning_level;
  int anted;
  int fold;
  int playing;

} players[MAX_PLAYERS];

struct poker_stats_struct
{
  unsigned long int game_count;
  unsigned long int round_count;

} game_stats;


int bet_order[MAX_PLAYERS];

int num_players=0;

int game_run_level=NOT_RUNNING;
int server;
int poker_channel=2;
int first_better=0;
int current_better=0;



unsigned long int poker_pot;
unsigned long int bet_limit;
int broke_player;
unsigned long int poker_ante=5;
unsigned long int poker_timer;
unsigned long int current_bet;

char near *long_suit_names[4] = {"Clubs","Hearts","Diamonds","Spades"};

char near *long_card_names[13] = { "Two","Three","Four","Five",
                              "Six","Seven","Eight","Nine",
                              "Ten","Jack","Queen",
                              "King","Ace"};

char near *plural_long_card_names[13] = { "Twos","Threes","Fours","Fives",
                                          "Sixes","Sevens","Eights","Nines",
                                          "Tens","Jacks","Queens",
                                          "Kings","Aces"};

char near *short_suit_names[4] = {"C","|*f1|*h1H|*r1","|*f1|*h1D|*r1","S"};
char near *short_card_names[13] = { "2","3","4","5","6","7","8","9","10",
                               "J","Q","K","A"};

char near *tokens[NUM_TOKENS] =
  { "SEE", "CHE", "FOL", "BET",
    "RAI", "CAL", "CAR", "STA",
    "POT", "MON", "REM", "FOR",
    "JOI", "QUI", "ANT", "DEA",
    "AUT", "DIS", "HELP","HAND" };



    /* PROTOTYPES */

int number_players_in(void);
int player_number(int portnum);
void poker_handle_message(char *handle,char *string);
void deal_cards(void);
void shuffle_deck(void);
void private_poker_message(char *string,int portnum);
int card_less_than(int card,int card2);
void poker_message(char *string);
void deregister_player(int portnum);
void print_poker_cards(int portnum);
int get_token(char **token, int *type, long int *ret_value);
void announce_all_cards(void);
void poker_winner(int winner);

void reset_round_vars(void)
{
 int count;

 for (count=0;count<num_players;count++)
  {
   players[count].warning_level=0;

  }
  current_better = first_better;
  current_bet=0;

}


void initialize_game(void)
{
   int count,count2;
   int selected_player;
   int bet_selection[MAX_PLAYERS];
   int already_selected;


   current_bet=0;
   broke_player=0;

   for (count=0;count<num_players;count++)
    {
     players[count].playing=1;
     players[count].run_level=NOT_RUNNING;
     players[count].fold=0;
     players[count].have_cards=0;


	 if (players[count].money>players[count].credit)
	  { players[count].money -= players[count].credit;
		players[count].credit = 0;
	  }
	 else
	  {
		players[count].money  = 0;
		players[count].credit = 0;
	  }
    }
							 
    /* pick the first better */

   bet_limit=0;

    first_better++;
    if (first_better>num_players)
     first_better=0;
    current_better=first_better;

   reset_round_vars();

}

void start_game(int portnum)
{
    int player_no = player_number(portnum);


    if (player_no<0)
     {
      private_poker_message("Your not in the game, type \"/b poker join\" to join the game",portnum);
      return;
     }

    if (game_run_level!=NOT_RUNNING)
      {
      private_poker_message("This game is not yet over!",portnum);
      return;
      }

    if (num_players<2)
      {
       poker_message("Must have two players to play");
       return;
      }

    initialize_game();
    shuffle_deck();
    game_run_level = DEAL_ROUND;
}


void invite_portnum(int portnum)
{
  char s[35];
  sprintf(s,"/P%d help",tswitch);

  aput_append_into_buffer(portnum, poker_channel, 1,
      tswitch, portnum, 0, 4, private_poker_header, "Welcome to the poker channel. Type \"|*h1",s,"|*r1\" for help.");

  next_task();
}

void send_help(int portnum)
{
  private_poker_message("Help Requested.",portnum);
  aput_into_buffer(portnum,"BOT\\HELP\\POKER.HLP",250,13,0,0,0);
}


void ante_player(int portnum)
{
 int player_no = player_number(portnum);

 if (player_no<0)
   {
    private_poker_message("You must be in the game to ante",portnum);
    return;
   }

 if (game_run_level!=ANTE_ROUND)
   {
     private_poker_message("This is not time to Ante",portnum);
     return;
   }

 if (players[player_no].run_level!=ANTE_ROUND)
   {
     private_poker_message("This is not the time for you to Ante.",portnum);
     return;
   }


  players[player_no].money -= poker_ante;
  poker_pot += poker_ante;
  poker_handle_message(players[player_no].handle,"|*r1 has anted.");

  players[player_no].run_level=FIRST_BET_ROUND;

  print_poker_cards(portnum);

}


unsigned long int read_amount(char *string)
{
 unsigned long int amount1 = 0l;
 unsigned long int amount2 = 0l;

 while ((*string==' ') && (*string))
  string++;

 for (;;)
 {
   if ((*string<'0') || (*string>'9'))
     return (amount1);

   amount2=((amount1*10) + (*string-'0'));

   if (amount2<amount1) /* if it flipped over return */
     return amount1;

   amount1=amount2;

   string++;

 }



}



int check_bet_round(int portnum)
{
  int player_no = player_number(portnum);

 if (player_no<0)
  {
    private_poker_message("You must be in the game to bet",portnum);
	return 0;
  }

  if ((game_run_level==FIRST_BET_ROUND) || (game_run_level==FINAL_BET_ROUND))
  {

    if (players[player_no].fold)
	 {
	   private_poker_message("You folded, you're no longer in this hand.",portnum);
	   return 0;
	}

	if (players[player_no].run_level==NOT_RUNNING)
	 {
	   private_poker_message("You're not in this hand.",portnum);
	   return 0;
	 }

    if (!players[player_no].warning_level)
     {
	   private_poker_message("It is not your turn to bet.",portnum);
	   return 0;
     }

	 return 1;

  }
  else
    private_poker_message("This is not the time to bet.",portnum);

  return 0;
}



void make_bet(int portnum,char *string)
{
 int player_no = player_number(portnum);
 char s[200];
 unsigned long int amount=read_amount(string);

 if (!check_bet_round(portnum))
  return;



     if (amount<players[player_no].current_bet)
     {
      sprintf(s,"You have already bet %lu.",players[player_no].current_bet);
      private_poker_message(s,portnum);
      return;
     }

    if (players[player_no].money<(amount - players[player_no].current_bet))
     {
      sprintf(s,"You only have %lu, you can't afford that bet",players[player_no].money);
      private_poker_message(s,portnum);
      return;
     }

     if ((amount)<current_bet)
     {
      sprintf(s,"You must at least see the current bet of %lu.",current_bet);
      private_poker_message(s,portnum);
      return;
     }

     if ((!amount) && (broke_player) && (game_run_level==FINAL_BET_ROUND) && (players[player_no].money))
      {
        private_poker_message("Sorry, a player will go broke this hand, you must bet at least 1.",portnum);
        return;
      }

     if ((amount>bet_limit) && (amount!=current_bet) && (bet_limit))
	 {
	   if (current_bet>bet_limit)
		 sprintf(s,"Sorry, you may only see the current bet of %lu.",current_bet);
	   else
		 sprintf(s,"Sorry, a bet limit of %lu is imposed.",bet_limit);

	   private_poker_message(s,portnum);
	   return;
	 }


      players[player_no].warning_level=0;
      players[player_no].run_level = game_run_level + 1;

     if ((!current_bet) && (!amount))
     {
       poker_handle_message(players[player_no].handle,"|*r1 checks.");

       return;
     }

     if (amount==current_bet)
     {
      sprintf(s,"|*r1 sees the current bet of %lu.",current_bet);
      poker_handle_message(players[player_no].handle,s);
      players[player_no].money-=(amount - players[player_no].current_bet);
      poker_pot+=(amount-players[player_no].current_bet);
      players[player_no].current_bet = amount;
      if (!players[player_no].money)
       broke_player=1;
      return;
     }



     if (!current_bet)
       sprintf(s,"|*r1 has set the bet to %lu.",(unsigned long int)amount);
     else
       sprintf(s,"|*r1 raises %lu to make the bet %lu.",(unsigned long int)(amount-current_bet),(unsigned long int)amount);

      poker_handle_message(players[player_no].handle,s);

      current_bet = amount;
      players[player_no].money -= (amount - players[player_no].current_bet);
      poker_pot += (amount - players[player_no].current_bet);
      players[player_no].current_bet = amount;

      if (!players[player_no].money)
       broke_player=1;
      return;


}

void see_bet(int portnum)
{
 int player_no = player_number(portnum);
 char s[100];
 unsigned long int amount_in_question;

 if (!check_bet_round(portnum))
  return;

  if (!current_bet)
   {
     private_poker_message("There is no current bet to see, use \"bet 0\" to check",portnum);
     return;
   }

  amount_in_question = (current_bet - players[player_no].current_bet);

  players[player_no].warning_level=0;
  players[player_no].run_level = game_run_level + 1;

 if ((amount_in_question)>players[player_no].money)
  {
	bet_limit = 50;

    players[player_no].credit += (amount_in_question - players[player_no].money) + 1;

    sprintf(s,"|*r1 is lent %lu so he can see the bet.",players[player_no].credit);
    poker_handle_message(players[player_no].handle,s);
    sprintf(s,"Bet limit of %lu imposed for |*r1",bet_limit);
    poker_handle_message(s,players[player_no].handle);

	players[player_no].money = 1;
    players[player_no].current_bet = current_bet;

	poker_pot +=amount_in_question;

  }
 else
 {

     sprintf(s,"|*r1 pays %lu to see the current bet of %lu.",amount_in_question,current_bet);
     poker_handle_message(players[player_no].handle,s);

	 players[player_no].money -= amount_in_question;
	 poker_pot				  += amount_in_question;

	 players[player_no].current_bet = current_bet;
 }

}


void fold_player(int portnum)
{
 int player_no = player_number(portnum);


 if (player_no<0)
  {
    private_poker_message("You are not in the game.",portnum);
    return;
  }

 if (game_run_level==NOT_RUNNING)
  {
    private_poker_message("The game has not yet begun.",portnum);
    return;
  }

 if ((players[player_no].fold) || (players[player_no].run_level==NOT_RUNNING))
  {
    private_poker_message("You are not in this game.",portnum);
    return;
  }
 players[player_no].fold=1;
 players[player_no].run_level = NOT_RUNNING;
 players[player_no].playing=0;

 poker_handle_message(players[player_no].handle,"|*r1 folds.");

}

int find_card_in_hand(int player_no,int card)
{
 int count;
 for(count=0;count<5;count++)
   if (players[player_no].cards_in_hand[count]==card)
     return count;

 return -1;

}

int check_for_blank_line(char *string)
{
  while (*string)
   if (*string!=' ')
     return 0;
   else
     string++;

   return 1;
}


void discard(int portnum,char *string)
{
   int player_no = player_number(portnum);
   int count,count2,temp;
   char *command=string+1;
   int bad_format=0;
   int type=0;
   char s[150];
   long int value;
   char discard_positions[5];
   int num_tokens=0;


   if (player_no<0)
     {
        private_poker_message("You must be in the game to discard.",portnum);
        return;
     }
   if (game_run_level!=DISCARD_ROUND)
     {
        private_poker_message("This is not the time to discard.",portnum);
        return;
     }
   if (players[player_no].run_level!=DISCARD_ROUND)
     {
        private_poker_message("You have already discarded.",portnum);
        return;
     }

   if (check_for_blank_line(string))
     {
        poker_handle_message(players[player_no].handle,"|*r1 is discarding no cards.");
        players[player_no].run_level = FINAL_BET_ROUND;
        players[player_no].warning_level=0;
        return;
     }

   while ((get_token(&command, &type, &value)) && num_tokens<5)
   {

      if (type==NUMBER_TOKEN)
        {
         if ((value<1) || (value>5))
           bad_format=1;
         else
           discard_positions[num_tokens] = (value-1);
        }
      else
      if (type==CARD_TOKEN)
        {
         temp = find_card_in_hand(player_no,value);
         if (temp==-1)
           bad_format=1;
         else
            discard_positions[num_tokens] = temp;
        }
      else
        bad_format=1;


      if (bad_format)
       {
         private_poker_message("Bad format in discard request, please retry.",portnum);
         return;
       }

      num_tokens++;
      type=0;
     }


    if (!num_tokens)
     {
      private_poker_message("Bad discard request:",portnum);
      private_poker_message(string,portnum);
      return;
     }

    if (num_tokens==5)
     {
      private_poker_message("You may not discard all your cards.",portnum);
      return;
     }

    for (count=0;count<num_tokens;count++)
      for (count2=count+1;count2<num_tokens;count2++)
        if (discard_positions[count]==discard_positions[count2])
          {
            private_poker_message("Confusing discard request, please retry.",portnum);
            return;
          }


    /* put stuff in to check for no ace and 4 discards */


    for (count=0;count<num_tokens;count++)
      players[player_no].cards_in_hand[discard_positions[count]] = NO_CARD;

    if (num_tokens==1)
      strcpy(s,"|*r1 will be discarding 1 card.");
    else
      sprintf(s,"|*r1 will be discarding %d cards.",num_tokens);

    poker_handle_message(players[player_no].handle,s);


    players[player_no].run_level = FINAL_BET_ROUND;
    players[player_no].warning_level=0;
    return;

};




void end_game(void)
{
   int count;


   for (count=0;count<num_players;count++)
    {
     players[count].playing=0;
     players[count].run_level=NOT_RUNNING;
     players[count].fold=0;
     players[count].have_cards=0;
    }

}

void empty_deck(void)
{
  int count;
  for (count=0;count<NUM_CARDS;count++)
    card_deck[count] = NO_CARD;
}

void private_poker_message(char *string,int portnum)
{
    char s[20];
    sprintf(s,"Port: [%d] ",portnum);
    print_string("(private_poker_message): <PokerBot>: ");
    print_string(s);
    print_str_cr(string);

    aput_append_into_buffer(portnum, poker_channel, 1,
                 tswitch, portnum, 0, 2, private_poker_header,string);
    next_task();
}

void poker_message(char *string)
{

    print_string("(poker_message): --> PokerBot: ");
    print_str_cr(string);

    aput_append_into_buffer(server, poker_channel, 0,
                 poker_channel, tswitch, 0, 2, (char far *)public_poker_header,(char far *)string);

    next_task();
}

void poker_handle_message(char *handle,char *string)
{

    print_string("(poker_message): --> PokerBot: ");
    print_str_cr(string);

    aput_append_into_buffer(server, poker_channel, 0,
             poker_channel, tswitch, 0, 5, (char far *)public_poker_header,(char far *)handle,"|*r1",(char far *)string,"|*r1");

    next_task();
}



int process_ante_round(void)
{
    int count;
    int waiting_for_players=0;
    unsigned int time_elapsed;
//    char s[150];

    for(count=0;count<num_players;count++)
     if ((players[count].have_cards) && (!players[count].fold) && (players[count].playing))
     {

        if ((players[count].run_level==DEAL_ROUND))
         {

           if (players[count].money<poker_ante)
               {
                 poker_handle_message(players[count].handle,"|*r1 is being forced out for not being able to afford the ante.");
                 deregister_player(players[count].portnum);
               }
           else
               {
                 waiting_for_players=1;
                 players[count].warning_level=0;
                 poker_timer=dans_counter;
                 players[count].run_level=ANTE_ROUND;
                 sprintf(s,"Type \"/b %s ante\" to ante. Current Ante is %lu",(char far *)"poker",poker_ante);
                 private_poker_message(s,players[count].portnum);
               }
         }
        else
        if ((players[count].run_level==ANTE_ROUND))
          {
            waiting_for_players=1;

            if (dans_counter<poker_timer)
              time_elapsed =  (unsigned int)0xFFFF - (poker_timer-dans_counter);
            else
              time_elapsed = dans_counter - poker_timer;

            if (((time_elapsed/18) > 60) && !players[count].warning_level)
              {
               private_poker_message("Please Ante or Quit the game (you have 45 seconds)",players[count].portnum);
               players[count].warning_level=1;
              }

            if (((time_elapsed/18) > 105) && (players[count].warning_level>0))
               {
                 poker_handle_message(players[count].handle,"|*r1 is being kicked out for not responding");
                 deregister_player(players[count].portnum);
               }
          }
     }

    if (waiting_for_players)
       return 0;


    return 1;    /* continue to next round */
}


int process_bets(int next_round)
{
 int count;
 int round_complete=1;
 int first_bets_in=1;
 unsigned int elapsed_time;
 char s[150];


 for (count=0;count<num_players;count++)
  if (!((players[count].fold) || (players[count].run_level==NOT_RUNNING)))
    {
    if ((players[count].run_level!=next_round))
     {
      first_bets_in=0;
      players[count].current_bet=0;
      round_complete=0;
     }

    if (players[count].current_bet!=current_bet)
       round_complete=0;

    }


 if (round_complete)
  return 1;

 if (current_better>=num_players)
   current_better=0;

 while ((players[current_better].fold) || players[current_better].run_level==NOT_RUNNING ||
          ((players[current_better].run_level==next_round)
              && (players[current_better].current_bet==current_bet)) )
 {
   current_better++;
   if (current_better>=num_players)
    current_better=0;
 }

   if (current_better>=num_players)
    current_better=0;
 count = current_better;

 if (!players[count].warning_level)
  {
   poker_timer = dans_counter;

   if ((count==first_better) && (!first_bets_in))
    strcpy(s,"|*r1 your first to place a bet.");
   else
    strcpy(s,"|*r1 your next to place a bet.");

   poker_handle_message(players[count].handle,s);
   players[count].warning_level=1;
  }
 else
  {
   if (dans_counter<poker_timer)
     elapsed_time = 0xFFFF - (poker_timer - dans_counter);
   else
     elapsed_time = dans_counter - poker_timer;

   if ((elapsed_time/18)>120)
    {
         poker_handle_message(players[count].handle,"|*r1 is being kicked out for not responding");
         deregister_player(players[count].portnum);
    }

   if (((elapsed_time/18)>60) && (players[count].warning_level<2))
    {
     players[count].warning_level=2;
     private_poker_message("Please Bet or Quit the game (you have 60 seconds)",players[count].portnum);
    }

  }

  return 0;
}

int process_first_bets(void)
{
    return (process_bets(DISCARD_ROUND));
}



int process_discard_round(void)
{
    int count;
    int waiting_for_players=0;
    unsigned int time_elapsed;
    char s[150];

    for(count=0;count<num_players;count++)
     {

        if ((players[count].run_level==DISCARD_ROUND))
         {
             waiting_for_players=1;
             poker_timer=dans_counter;

             if (dans_counter<poker_timer)
               time_elapsed =  (unsigned int)0xFFFF - (poker_timer-dans_counter);
             else
               time_elapsed = dans_counter - poker_timer;


             if (((time_elapsed/18) > 60) && !players[count].warning_level)
               {
                 private_poker_message("Please Discard or Quit the game (you have 45 seconds)",players[count].portnum);
                 players[count].warning_level=1;
               }

             if (((time_elapsed/18) > 105) && (players[count].warning_level>0))
               {
                 poker_handle_message(players[count].handle,"|*r1 is being kicked out for not responding");
                 deregister_player(players[count].portnum);
               }
          }
     }

    if (waiting_for_players)
       return 0;


    return 1;    /* continue to next round */
}


void replace_discards(void)
{
  int num_cards_in_hand=0;
  int count_player=0;
  int temp;
  int tries;

  g_delay(10);

  for (num_cards_in_hand=0;num_cards_in_hand<5;num_cards_in_hand++)
   {

    for (count_player=0;count_player<num_players;count_player++)
    if (players[count_player].cards_in_hand[num_cards_in_hand]==NO_CARD)
        {
           tries=0;

           do
           {
            temp = rand() % NUM_CARDS;

            if ((tries % 30) == 29)
              srand((unsigned) dans_counter);

           }  while ((card_deck[temp]==NO_CARD) && (tries++)<140);

           if (card_deck[temp]==NO_CARD)
             { poker_message("Having trouble dealing cards (random problems)");
               tries=0;
               do
               { temp++;
                 if (temp>=52) temp=0;
                 tries++;
               } while ((card_deck[temp]==NO_CARD) && tries<60);

               if (card_deck[temp]==NO_CARD)
                {
                 poker_message("The Card Deck is corrupted, ending game.");
                 game_run_level = GAME_COMPLETE;
                }

             }


           players[count_player].cards_in_hand[num_cards_in_hand] = card_deck[temp];

           card_deck[temp] = NO_CARD;

           players[count_player].have_cards=1;

        }

   }
   g_delay(10);

}

int process_final_bets(void)
{
    return (process_bets(GAME_COMPLETE));
}


int has_card_with_value(int player_no,signed char value)
{
  int count;

  if ((value<0) || (value>12))
   return 0;

  for(count=0;count<5;count++)
    if ((players[player_no].cards_in_hand[count] % 13) == value)
      return 1;

  return 0;
}

int find_high_card(int player_no)
{
   int count;
   int high_card=NO_CARD;

   if ((player_no<0) || (player_no>=num_players))
     return NO_CARD;

   for (count=0;count<5;count++)
      if (card_less_than(high_card,players[player_no].cards_in_hand[count]))
        high_card = players[player_no].cards_in_hand[count];

   return (high_card);

}

int is_a_flush(int player_no)
{
  signed char suit;
  int count;

  suit = (players[player_no].cards_in_hand[0] / 13);

  for (count=1;count<5;count++)
     if (suit != (players[player_no].cards_in_hand[count] / 13))
       return NO_CARD;

  return suit;
}

int is_a_straight(int player_no)
{
  signed char high_card_value = (find_high_card(player_no) % 13);
  int count;

  if (high_card_value<5)
    return NO_CARD;


  for (count=1;count<5;count++)
    if (!has_card_with_value(player_no,(high_card_value-count)))
      return NO_CARD;

  return high_card_value;


}


int n_of_a_kind(int player_no,int n,int not_of_value)
{
    int count,count2;
    int test_value;
    int num_of_this_kind;

    for (count=0;count<(6-n);count++)
     if ((test_value = (players[player_no].cards_in_hand[count] % 13)) != not_of_value)
        {
         num_of_this_kind=1;

         for (count2=count+1;count2<5;count2++)
            if (test_value == (players[player_no].cards_in_hand[count2] % 13))
              num_of_this_kind++;

         if (num_of_this_kind==n)
           return  (test_value);

        }

    return NO_CARD;


}

int card_less_than(int card,int card2)
{
    int value  = card  % 13;
    int value2 = card2 % 13;
    int suit   = card  / 13;
    int suit2  = card2 / 13;

    if (value<value2)
     return 1;
    if (value>value2)
     return 0;
     
    if (suit<suit2)
     return 1;
    else
     return 0;

}

struct hand_classification_struct
{
 signed char straight;
 signed char flush;
 signed char four_of_a_kind;
 signed char three_of_a_kind;
 signed char two_of_a_kind;
 signed char two_of_a_kind2;
 signed char high_card;
};


#define STRAIGHT_FLUSH     0
#define FOUR_OF_A_KIND     1
#define FULL_HOUSE         2
#define FLUSH              3
#define STRAIGHT           4
#define THREE_OF_A_KIND    5
#define DBL_TWO_OF_A_KIND  6
#define TWO_OF_A_KIND      7
#define HIGH_CARD          8
#define NUMBER_OF_COMBINATIONS 9



  struct hand_classification_struct winning_hand[NUMBER_OF_COMBINATIONS] =
    {  { 1      , 1       , NO_CARD, NO_CARD, NO_CARD, NO_CARD, 1       },  /* straight-flush    =  0 */
       { NO_CARD, NO_CARD , 1      , NO_CARD, NO_CARD, NO_CARD, NO_CARD },  /* 4 of a kind       =  1 */
       { NO_CARD, NO_CARD , NO_CARD, 1      , 1      , NO_CARD, NO_CARD },  /* full house        =  2 */
       { NO_CARD, 1       , NO_CARD, NO_CARD, NO_CARD, NO_CARD, 1       },  /* flush             =  3 */
       { 1      , NO_CARD , NO_CARD, NO_CARD, NO_CARD, NO_CARD, 1       },  /* straight          =  4 */
       { NO_CARD, NO_CARD , NO_CARD, 1      , NO_CARD, NO_CARD, NO_CARD },  /* three of a kind   =  5 */
       { NO_CARD, NO_CARD , NO_CARD, NO_CARD, 1      , 1      , NO_CARD },  /* dbl two of a kind =  6 */
       { NO_CARD, NO_CARD , NO_CARD, NO_CARD, 1      , NO_CARD, NO_CARD },  /* two of a kind     =  7 */
       { NO_CARD, NO_CARD , NO_CARD, NO_CARD, NO_CARD, NO_CARD, 1       }}; /* high card         =  8 */


int player_with_hand(struct hand_classification_struct *everyone,struct hand_classification_struct *winning_hand)
{
  signed char high_card = NO_CARD;
  int high_player = NO_CARD;
  struct hand_classification_struct *this_player;
  struct hand_classification_struct  *the_high_player;
  int has_hand;
  int count;


  for (count=0;count<num_players;count++)
    if ((!players[count].fold) && (players[count].run_level==GAME_COMPLETE))
     {
        has_hand=1;

        this_player = &everyone[count];
        if (high_player!=NO_CARD)
          the_high_player = &everyone[high_player];

        if (winning_hand->straight!=NO_CARD)
         {
           if (this_player->straight==NO_CARD)
            has_hand=0;
           else
            {
               if (high_player!=NO_CARD)
                    if (card_less_than(this_player->straight,the_high_player->straight))
                       has_hand=0;
            }

         }

        if ((winning_hand->flush!=NO_CARD) && has_hand)
         {
           if (this_player->flush==NO_CARD)
            has_hand=0;
           else
            {
               if (high_player!=NO_CARD)
                    if (this_player->flush < the_high_player->flush)
                       has_hand=0;
            }
         }

        if ((winning_hand->four_of_a_kind!=NO_CARD)  && has_hand)
         {
           if (this_player->four_of_a_kind==NO_CARD)
            has_hand=0;
           else
            {
               if (high_player!=NO_CARD)
                    if (this_player->four_of_a_kind < the_high_player->four_of_a_kind)
                       has_hand=0;
            }


         }

        if ((winning_hand->three_of_a_kind!=NO_CARD) && has_hand)
         {
           if (this_player->three_of_a_kind==NO_CARD)
            has_hand=0;
           else
            {
               if (high_player!=NO_CARD)
                    if (this_player->three_of_a_kind < the_high_player->three_of_a_kind)
                       has_hand=0;
            }

         }

        if ((winning_hand->two_of_a_kind!=NO_CARD) && has_hand)
         {
           if (this_player->two_of_a_kind==NO_CARD)
            has_hand=0;
           else
            {
               if (high_player!=NO_CARD)
                    if (this_player->two_of_a_kind < the_high_player->two_of_a_kind)
                       has_hand=0;
            }

         }

        if ((winning_hand->two_of_a_kind2!=NO_CARD) && has_hand)
         {
           if (this_player->two_of_a_kind2==NO_CARD)
            has_hand=0;
           else
            {
               if (high_player!=NO_CARD)
                    if (this_player->two_of_a_kind2 < the_high_player->two_of_a_kind2)
                       has_hand=0;
            }

         }

        if ((winning_hand->high_card!=NO_CARD) && has_hand)
         {
           if (high_player!=NO_CARD)
                if (card_less_than(this_player->high_card,the_high_player->high_card))
                   has_hand=0;

         }

         if ((has_hand))
          {
            high_player = count;
            high_card   = this_player->high_card;
           }


     }

     return (high_player);
}




void check_for_winner(void)
{
  int count,count2;
  char s2[70];
  char s3[70];
  int high_card=1;
  struct hand_classification_struct player_classification[MAX_PLAYERS];
  int winner=-1;


  poker_message("Revealing the players cards:");


  for (count=0;count<num_players;count++)
    if ((!players[count].fold) && (players[count].run_level!=NOT_RUNNING))
      {
        *s=0; /* clear the string */

        for (count2=0;count2<5;count2++)
          {

            sprintf(s2," %2s-%s ",
                         (char far *)short_card_names[((unsigned char) players[count].cards_in_hand[count2]) % 13],
                         (char far *)short_suit_names[((unsigned char) players[count].cards_in_hand[count2]) / 13]);
            strcat((char far *)s,(char far *)s2);
          }
        poker_handle_message(players[count].handle,s);
      }


  for (count=0;count<num_players;count++)
    if ((!players[count].fold) && (players[count].run_level!=NOT_RUNNING))
        {
          player_classification[count].straight        = is_a_straight(count);
          player_classification[count].flush           = is_a_flush(count);
          player_classification[count].high_card       = find_high_card(count);
          player_classification[count].four_of_a_kind  = n_of_a_kind(count,4,NO_CARD);
          player_classification[count].three_of_a_kind = n_of_a_kind(count,3,NO_CARD);
          player_classification[count].two_of_a_kind   = n_of_a_kind(count,2,player_classification[count].three_of_a_kind % 13);
          if (player_classification[count].three_of_a_kind == NO_CARD)
              player_classification[count].two_of_a_kind2  = n_of_a_kind(count,2,player_classification[count].two_of_a_kind % 13);
          else
              player_classification[count].two_of_a_kind2 = NO_CARD;

        }

  /* check for winning hands in order now */
  count=0;

  while ((winner = player_with_hand(player_classification,&winning_hand[count])) == NO_CARD)
   count++;

  sprintf(s2,"%s|*r1 wins with ",players[winner].handle);

  switch (count)
   {
     case STRAIGHT_FLUSH:
             if ((player_classification[winner].high_card % 13) == 12)
              {
               strcpy(s,"a Royal Flush");
               high_card=0;
              }
             else
              strcpy(s,"a Straight Flush");

             break;
     case STRAIGHT:
             strcpy(s,"a Straight");
             break;
     case FOUR_OF_A_KIND:
             high_card=0;
             sprintf(s,"Four %s",
                         (char far *)plural_long_card_names[((unsigned char) player_classification[winner].four_of_a_kind) % 13]);
             break;
     case FLUSH:
             sprintf(s,"a %s Flush",
                         (char far *)long_suit_names[((unsigned char) player_classification[winner].flush) / 13]);
             break;
     case FULL_HOUSE:
             high_card=0;
             strcpy(s,"a Full House");
             break;
     case THREE_OF_A_KIND:
             high_card=0;
             sprintf(s,"Three %s",
                         (char far *)plural_long_card_names[((unsigned char) player_classification[winner].three_of_a_kind) % 13]);
             break;
     case DBL_TWO_OF_A_KIND:
             high_card=0;
             sprintf(s,"Two Pair, %s and %s",
                         (char far *)plural_long_card_names[((unsigned char) player_classification[winner].two_of_a_kind) % 13],
                         (char far *)plural_long_card_names[((unsigned char) player_classification[winner].two_of_a_kind2) % 13]);
             break;
     case TWO_OF_A_KIND:
             high_card=0;
             sprintf(s,"a Pair of %s",
                         (char far *)plural_long_card_names[((unsigned char) player_classification[winner].two_of_a_kind) % 13]);
             break;
     case HIGH_CARD:
             high_card=0;
             sprintf(s,"a %s of %s high",
                         (char far *)long_card_names[((unsigned char) player_classification[winner].high_card) % 13],
                         (char far *)long_suit_names[((unsigned char) player_classification[winner].high_card) / 13]);
             break;



   }


  if (high_card)
     sprintf(s3,", %s-%s high.",
             (char far *)long_card_names[((unsigned char) player_classification[winner].high_card) % 13],
             (char far *)long_suit_names[((unsigned char) player_classification[winner].high_card) / 13]);
  else
     strcpy(s3,".");



   aput_append_into_buffer(server, poker_channel, 0,
                 poker_channel, tswitch, 0, 4, public_poker_header,s2,s,s3);




  poker_winner(winner);
}

void poker_winner(int winner)
{
  char s[120];

  if (poker_pot)
   {
    sprintf(s,"|*r1 wins with %lu in the pot.",poker_pot);
    poker_handle_message(players[winner].handle,s);
   }
  else
   poker_message("No money in the pot");


  if (players[winner].credit)
   {
    if (poker_pot<=players[winner].credit)
	 {
		poker_pot=0;
		players[winner].money = 0;
	 }
	else
	  poker_pot -= players[winner].credit;

	players[winner].credit=0;
   }

  players[winner].money+=poker_pot;

  poker_pot=0;
  game_run_level=GAME_COMPLETE;

}

void print_roll_over_message(void)
{
 char s[150];

 sprintf(s,"No final bet, the pot (%lu) stays and we'll deal again.",poker_pot);
 poker_message(s);

}

int number_players_in(void)
{
 int count;
 int number_playing=0;

        for (count=0;count<num_players;count++)
         if (!players[count].fold)
           { number_playing++;
           }

 return (number_playing);
}

void process_game_level(void)
{
    int number_playing=0;
    int count;
    int a_player;



    if (game_run_level!=NOT_RUNNING)
      {

        if (num_players)
            for (count=0;count<num_players;count++)
             if ((!players[count].fold) && (players[count].playing))
               { number_playing++;
                 a_player=count;
               }


        if (number_playing==1)
         {
            poker_message("Only one player left. This game is over.");
            poker_winner(a_player);
            game_run_level=NOT_RUNNING;
         }

        if (!number_playing)
        {
          poker_pot=0;
          game_run_level=NOT_RUNNING;
        }

      }

    /* check for empty game here */
    /* check for forceouts after a 1 minute time limit here */

    switch (game_run_level)
     {

            case NOT_RUNNING:       break;


            case DEAL_ROUND:        deal_cards();
                                    game_run_level = ANTE_ROUND;
                                    reset_round_vars();
                                    break;

            case ANTE_ROUND:        if (process_ante_round())
                                      {
                                       game_run_level = FIRST_BET_ROUND;
                                       poker_message("Everyone has anted, now for some betting.");
                                       reset_round_vars();
                                      }
                                    break;

            case FIRST_BET_ROUND:   if (process_first_bets())
                                      {
                                       game_run_level = DISCARD_ROUND;
                                       poker_message("All bets are in, now discard.");
                                       reset_round_vars();
                                      }
                                    break;

            case DISCARD_ROUND:     if (process_discard_round())
                                     {
                                     game_run_level = FINAL_BET_ROUND;

                                     replace_discards();
                                     poker_message("Discarded cards replaced, now it's time for the final bets.");
                                     g_delay(5);

                                     announce_all_cards();
                                     reset_round_vars();
                                     }
                                    break;

            case FINAL_BET_ROUND:   if (process_final_bets())
                                    {
                                      game_run_level = GAME_COMPLETE;
                                      if (!current_bet)
                                        {
                                            print_roll_over_message();
                                            game_run_level = NOT_RUNNING;
                                        }
                                      reset_round_vars();
                                    }
                                    break;

            case GAME_COMPLETE:     check_for_winner();
                                    end_game();
                                    game_run_level = NOT_RUNNING;
                                    break;

     }


}

void shuffle_deck(void)
{
   int count=0;
   int loc;
   int count2;

   empty_deck();
   srand((unsigned) dans_counter);

   poker_message("Shuffling...");

   for (count=0;count<NUM_CARDS;count++)
   {
      count2=0;

      do
      { loc = rand() % NUM_CARDS;
        count2++; }
      while ((card_deck[loc]!=NO_CARD) && (count2<52));

      if (card_deck[loc] != NO_CARD)
       {
         loc=0;
         while (card_deck[loc]!=NO_CARD)
          loc++;
       }
      if ((loc>=0) && (loc<52))
        card_deck[loc] = count;
      else
        print_str_cr("It's screwed dave");

   }

   print_str_cr("Deck Shuffled");
}

/* example ST_COPY
  st_copy((void far *)&a_user,(sizeof(struct user_data)),USER_LINES,0);

   */

void deregister_player(int portnum)
{

   int count=0;
   int count2;
   char s[120];

   while (count<num_players)
    {
     if (players[count].portnum == portnum)
       {
         sprintf(s,"Player (#%02d):%s|*r1 has left the game",portnum,players[count].handle);
         poker_message(s);

         count2=count+1;
         while (count2<num_players)
          { players[count2-1] = players[count2];
            count2++;
          }
         num_players--;


       }
       count++;
    }

    if (first_better>num_players)
     first_better=0;
}

void deregister_player_verbose(int portnum)
{

   int count=0;
   int count2;
   int found_player=0;
   char s[120];

   while (count<num_players)
    {
     if (players[count].portnum == portnum)
       {
         sprintf(s,"Player (#%02d):%s|*r1 has left the game",portnum,players[count].handle);
         poker_message(s);
         found_player=1;

         count2=count+1;
         while (count2<num_players)
          { players[count2-1] = players[count2];
            count2++;
          }
         num_players--;


       }
       count++;
    }

    if (found_player)
     {
        if (first_better>num_players)
         first_better=0;
     }
    else
     private_poker_message("You're not in the game!",portnum);
}


void register_player(int portnum)
{
  struct user_data a_user;
  struct ln_type a_line;
  int player_no = player_number(portnum);

  char s[150];

  if (player_no!=-1)
   {
    private_poker_message("You are already in the game!",portnum);
    return;
   }

  if (num_players>=MAX_PLAYERS)
     {
       private_poker_message("No room in the game\007",portnum);
       return;
     }




  if (!st_copy((void far *)&a_line,(sizeof(struct user_data)),LINE_STATUS,portnum))
    return;

  if (a_line.mainchannel!=poker_channel)
    {
       sprintf(s,"<PokerBot>: You must be on the poker channel%c%c              Current Channel: %d",13,10,poker_channel);
       aput_into_buffer(portnum,s,
         0, 1, tswitch, portnum, 0);
       return;
    }

  st_copy((void far *)&a_user,(sizeof(struct user_data)),USER_LINES,portnum);



  players[num_players].user_number = a_user.number;
  strncpy(players[num_players].handle,a_user.handle,HANDLE_SIZE);
  players[num_players].handle[HANDLE_SIZE]=0;
  players[num_players].portnum = portnum;
  players[num_players].pid = 0;
  players[num_players].credit=0;
  players[num_players].have_cards=0;
  players[num_players].playing=0;
  players[num_players].run_level = NOT_RUNNING;


  players[num_players].money=1000;

  /* load his money here */


  sprintf(s,"|*r1 has joined the game with %lu money",players[num_players].money);
  poker_handle_message(players[num_players].handle,s);
  num_players++;

}

int compare_list(char near *list[], int length, char **cvalue)
{
  int count;

  for (count=0;count<length;count++)
  {
   if (!strncmp(*cvalue,list[count],strlen(list[count])))
   {
     (*cvalue) += strlen(list[count]);
     return (count);
   }
  }
  return (-1);
}

int get_token(char **token, int *type, long int *ret_value)
{
  char token_buf[TOKEN_LENGTH+1];
  int count = 0;
  char *copy = token_buf;
  long int value = -1;

  while (**token == ' ') (*token)++;
  if (!(**token)) return (0);

  while ((count < TOKEN_LENGTH) && ((**token) != ' ') && (**token))
  {
    *copy++ = (((**token)>='a') && ((**token)<='z')) ?
       (*(*token)++ - ' ') : (*(*token)++);
    count++;
  }
  while (((**token) != ' ') && (**token)) (*token)++;
  *copy = 0;
  copy = token_buf;
  value = compare_list(tokens,NUM_TOKENS,&copy);
  if (value != -1)
  {
    if ((*type) && (*type != WORD_TOKEN)) return (0);
    *type = WORD_TOKEN;
    *ret_value = value;
    return (1);
  }
  value = compare_list(short_card_names,13,&copy);
  if (value != -1)
  {
    int card = value;

    if (*copy == '-') copy++;
    value = compare_list(short_suit_names,4,&copy);
    if (value != -1)
    {
      if ((*type) && (*type != CARD_TOKEN)) return (0);
      *type = CARD_TOKEN;
      *ret_value = card + (value * 13);
      return (1);
    }
  }
  copy = token_buf;
  value = 0;
  while ((*copy >= '0') && (*copy <= '9'))
    value = (value * 10) + (*copy++ - '0');
  if ((*copy) || ((*type) && (*type != NUMBER_TOKEN))) return (0);
  *type = NUMBER_TOKEN;
  *ret_value = value;
  return (1);
}

int player_number(portnum)
{
 int count=0;

 while (count<num_players)
  {
    if (players[count].portnum==portnum)
     return (count);
    else
    count++;

  }
  return -1;
}



void print_poker_cards_for_player(int player_no)
{
  int count;
  char s[200];
  char s2[40];


  if ((player_no<0) || (player_no)>=num_players)
   return;

  if (game_run_level==NOT_RUNNING)
   {
	private_poker_message("The game has not yet begun.",players[player_no].portnum);
    return;
   }

  if (!players[player_no].have_cards)
    { private_poker_message("You have no cards",players[player_no].portnum);
      return;
    }

  if (players[player_no].fold)
   {
	private_poker_message("You have already folded.",players[player_no].portnum);
	return;
   }

  if (players[player_no].run_level==NOT_RUNNING)
   {
	private_poker_message("You are not in this round of play.",players[player_no].portnum);
	return;
   }

  if (players[player_no].run_level<FIRST_BET_ROUND)
    {
     private_poker_message("You must ante first.",players[player_no].portnum);
     return;
    }
  if ((players[player_no].run_level!=DISCARD_ROUND) && (game_run_level==DISCARD_ROUND))
    {
       private_poker_message("You must wait for all others to discard.",players[player_no].portnum);
       return;
    }

  strcpy(s,"Your Cards: ");
  for (count=0;count<5;count++)
   {
     sprintf(s2,"%d) %s-%s  ",count+1,
                         (char far *)short_card_names[((unsigned char) players[player_no].cards_in_hand[count]) % 13],
                         (char far *)short_suit_names[((unsigned char) players[player_no].cards_in_hand[count]) / 13]);
     strcat(s,s2);
   }

   private_poker_message(s,players[player_no].portnum);

}


void print_poker_cards(int portnum)
{
 int player_no = player_number(portnum);

 if (player_no<0)
  return;
 print_poker_cards_for_player(player_no);
}

void announce_all_cards(void)
{
 int count;

 print_str_cr("Announcing Cards");

 for (count=0;count < num_players;count++)
  if (!(players[count].fold) && (players[count].run_level!=NOT_RUNNING))
    print_poker_cards_for_player(count);
}


void print_poker_status(int portnum)
{
  int count;
  char far *header;
  char s2[10];
  unsigned long int sum;
  char s3[120];

  if (portnum==server)
    header = (char far *)public_poker_header;
  else
    header = (char far *)private_poker_header;

  if (!num_players)
   {

        aput_append_into_buffer(portnum, poker_channel, 1,
                 tswitch, portnum, 0, 2, header,"No Current Players");
        aput_append_into_buffer(portnum, poker_channel, 1,
                 tswitch, portnum, 0, 2, header,info_line);

        return;
   }

  for (count=0;count<num_players;count++)
    {

        sprintf(s2,"#%02d:",players[count].portnum);

        if (players[count].credit)
         {
           if (players[count].credit > players[count].money)
              sum = players[count].credit - players[count].money;
           else
              sum = players[count].money  - players[count].credit;

           sprintf(s,"|*r1 with -%-9lu money  at run_level:%s",sum,
                  run_levels[players[count].run_level]);
         }
        else
           sprintf(s,"|*r1 with %-9lu money  at run_level:%s",players[count].money,
                  run_levels[players[count].run_level]);

        aput_append_into_buffer(portnum, poker_channel, 1,
                 tswitch, portnum, 0, 4, header,s2,players[count].handle,s);
    }

  if (poker_pot)
   {
      sprintf(s,"Current Pot: %lu    Current Bet: %lu",poker_pot,current_bet);
      aput_append_into_buffer(portnum,poker_channel, 1,
                 tswitch, portnum, 0, 2, header, s);
   }
}

void interpret_command(char *string, int portnum,int in_public)
{
  int type,result;
  long int value;
  char *command = string;
  char s[80];

    if (portnum==tswitch)
      return;

    type = 0;

    if ((result = get_token(&command,&type,&value))!=0)
      sprintf(s,"Token %d/%d",type,value);
      else
      sprintf(s,"Bad Token %d/%d",type,value);

    print_str_cr(s);


    if ((result && (type!=WORD_TOKEN)) || (!result))
     {
      private_poker_message("Unrecognized command",portnum);
      return;
     }


    if ((result) && (type==WORD_TOKEN))
      switch(value)
       {

		 case 0:  /* SEE the current bet */
         case 5:  /* CALL the current bet */
			  see_bet(portnum);
			  break;

		 case 2:	/* FOLD */
              fold_player(portnum);
              break;

         case 3:    /* BET money */
              make_bet(portnum,command);
              break;

         case 19:   /* show HAND */
         case 6:    /* show CARDS */
              print_poker_cards(portnum);
              break;

         case 7:    /* STATUS requested */
              if (in_public)
                print_poker_status(server);
              else
                print_poker_status(portnum);
              break;

         case 12:   /* JOIN game */
              register_player(portnum);
              break;

         case 13:   /* QUIT the game */
              deregister_player_verbose(portnum);
              break;

         case 14:   /* ANTE player */
              ante_player(portnum);
              break;

         case 15:   /* DEAL cards */
              start_game(portnum);
              break;

         case 17:   /* DIScard */
              discard(portnum,command);
              break;
         case 18:
              send_help(portnum);
              break;

         default:
              private_poker_message("Command not yet implemented.",portnum);
              break;

         break;

       }

}



int interpret_abuf(void)
{
  char s[421];
  char *start;
  int sentby, channel, type, temp1, temp2, temp3;

  process_game_level();

  if (aget_abuffer(&sentby,&channel,s,&type,&temp1,&temp2,&temp3))
  {
    s[420]=0;
    switch (type)
    {
      case 0:                                           /* Normal Message */
              if (channel!=poker_channel)
                break; /* throw it out if it's not on our channel */

              start=s;
              while ( (*start) && !( (*start=='-') && ( (*(start+1)=='P') || (*(start+1) == 'p')) ) )
                start++;

              if (!*start)
                break;

              while ((*start) && (*start!=' '))
                  start++;

              interpret_command(start,temp2,1);
              break;

      case 1: if (*s == '*') return (0);                /* PRIVATE MESSAGE */
              interpret_command(s,temp1,0);
              break;

      case 3: if (temp3 == 1)                           /* Login/Logout Message */
               {
                 sprintf(s,"Poker on T%d",poker_channel);
                 private_poker_message(s,temp2);
               }
              if (temp3 == 2) deregister_player(temp2);
              break;

      case 4:                                           /* Multiple Channel Message */
              if (((temp1==2) || (temp1==1)) && (channel==poker_channel))
                 deregister_player(temp3);
              if (temp1==3)
                 invite_portnum(temp3);

              break;

    } // end switch

  }
  else  /* (no messages waiting, then sleep a while ) */
    g_delay(9);

  process_game_level();

  return (1);
}

int running=1;


void exit_poker_bot(void)
{
    running=0;
}



int far ginsu_main(void)
{
  int type,temp1,temp2,temp3,channel,sentby;
  char real_name[15];

  server = get_server();
  initabuffer(2048);
  register_bot("Poker");
  get_registered_bot_name_for_myself(real_name,14);

  sprintf(info_line,"Come to Channel: %d",poker_channel);
  change_my_info_line(info_line);
  if (!change_my_channel(2))
    return (1);

  sprintf((char far *)private_poker_header,"|*h1P|*r1#%02d<T%d:%s> ",tswitch,poker_channel,(char far *)real_name);
  sprintf((char far *)public_poker_header,"#%02d<T%d:%s> ",tswitch,poker_channel,real_name);

  sprintf(s,"\007--> PokerBot is running on Channel %d, come join the fun.",poker_channel);
  broadcast_message(s);

  while (running)
  {
    running = interpret_abuf();
    next_task();
  }

  unregister_bot_myself();
  poker_message("Bot Shutting Down.");
  return (1);
}



/* game play routines */


void deal_cards(void)
{
  int num_cards_in_hand=0;
  int count_player=0;
  int temp;
  int tries;

  poker_message("Dealing...");
  g_delay(10);

  for (num_cards_in_hand=0;num_cards_in_hand<5;num_cards_in_hand++)
   {
        for (count_player=0;count_player<num_players;count_player++)
        if (players[count_player].playing)
            {
               tries=0;

               do
                   {
                    temp = rand() % NUM_CARDS;

                    if (tries>30)
                      srand((unsigned) dans_counter);

                   }  while ((card_deck[temp]==NO_CARD) && (tries++)<100);

               if (card_deck[temp]==NO_CARD)
                     { poker_message("Having trouble dealing cards (random problems)");
                       tries=0;
                       do
                       { temp++;
                         if (temp>=52) temp=0;
                         tries++;
                       } while ((card_deck[temp]==NO_CARD) && tries<60);

                       if (card_deck[temp]==NO_CARD)
                        {
                         poker_message("The Card Deck is corrupted, exiting bot");
                         exit_poker_bot();
                        }

                     }
               players[count_player].cards_in_hand[num_cards_in_hand] = card_deck[temp];
               card_deck[temp] = NO_CARD;
               players[count_player].have_cards=1;
               players[count_player].run_level = DEAL_ROUND;
            }

   }

   g_delay(10);
}

