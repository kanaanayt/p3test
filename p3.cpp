#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

const int MAXSQUARES = 60;
const int MAXPLAYERS = 4;
const int MAXCARDS = 45;

////////////////////////////////////////////////////////
/* Colors */
////////////////////////////////////////////////////////

enum Color {
    BLUE,
    YELLOW,
    GREEN,
    RED,
    NONE
};

// Remember: by default, enumerations count from 0..N-1
const int MAXCOLORS = NONE+1;
const string colorNames[MAXCOLORS] = {"BLUE", "YELLOW", "GREEN", "RED", "NONE"};

////////////////////////////////////////////////////////
/* The Player */
////////////////////////////////////////////////////////

// The state of a single pawn
enum PawnState {
    STARTABLE,
    ON_BOARD,
    HOME
};

const int MAXSTATES = HOME+1;
const string stateNames[MAXSTATES] = {"Startable", "On board square", "Home" };

struct PawnLocation {
    PawnState   state;
    int         square;     // Only valid if state == ON_BOARD
};

const int NUMPAWNS = 4;
// A single player
struct Player {
    int               startSquare;      // Square player STARTS on
    int               homeSquare;       // This player's HOME square
    PawnLocation      pawns[NUMPAWNS];  // Where the pawns are
};

//////////////////////////////////////////////////////////
/* The Deck */
//////////////////////////////////////////////////////////

// The different types of cards.
enum CardType {
    START,
    FORWARD,
    BACKWARD,
    SWAP,
    SORRY
};

const int MAXTYPES = SORRY+1;
const string cardNames[MAXTYPES] = {"START", "FORWARD", "BACKWARD", "SWAP", "SORRY"};

// A single card
struct Card {
    CardType    type;
    int         value;
};

// A deck of cards
struct Deck {
    int         curCard;          // The index of the card on "top" of the deck
    int         numCards;         // The total number of cards
    Card        cards[MAXCARDS];
};

//////////////////////////////////////////////////////////
/* THE BOARD */
//////////////////////////////////////////////////////////

// The different kinds of squares.
enum SquareKind {
    REGULAR,
    BEGIN,      // Begins a slide
    END,        // Ends a slide
    STARTSQ,    // Start square for a player
    HOMESQ      // Home square for a player
};

const int MAXKINDS = HOMESQ+1;
const string kindNames[MAXKINDS] = {"REGULAR", "BEGIN", "END", "STARTSQ", "HOMESQ"};

struct Role {                 // The role played by a square
    SquareKind  kind;
    Color       color;      // NONE if kind is REGULAR
};

struct Pawn {
    Color color; // Color of this pawn, NONE if "no pawn here"
    int num; // Number of this pawn, if Color != NONE
};

// Note: Each square is guaranteed to play at most one slide role,
// and at most one start/stop role.
struct Square {
    Role        Slide;            // Slide start, end, or REGULAR
    Role        Ends;             // Player Start, Home, Or Regular
    Pawn        Occupant;         // Pawn on this square (if any)
};
  
// The board
struct Board {
    int         numSquares;             // How many squares total
    Square      squares[MAXSQUARES];
};
      
      
//////////////////////////////////////////////////////////
/* THE GAME */
//////////////////////////////////////////////////////////
      
struct Game {
    Board        board;
    Deck         deck;
    int          Numplayers;            // Total Number Of Players
    Player       Players[MAXPLAYERS];
    int          Rounds;                // Rounds Remaining To Play
    bool         Shuffle;
};

struct Outcome {
    bool         Canmove;         // True If A Move Exists, False If Forfiet
    int          Pawnnum;         // Which Of The Player'S Pawns To Move (If Any)
    Pawn         Opponent;        // Which opponent pawn is victim of swap/sorry (if any)

};
      
/////////////////////////////////////////////////////////////////
/// YOU MUST WRITE THESE
/////////////////////////////////////////////////////////////////
      
bool initGame(Game &game, int argc, char *argv[]);
// MODIFIES:      game
// EFFECTS:       reads the board and deck, as
//                specified on the command line, and initializes the Game
//                structure. Returns true on success, false on failure.
//                Should produce no output.
      
Outcome eval(const Game &game, Color player);
// REQUIRES:      game is well-formed
// EFFECTS:       evaluates the desired Outcome of "player" drawing the card
//                that is currently at the top of the deck.
//                Must follow the specification's movement rules, and MUST NOT
//                modify game. Should produce no output.
      
void apply(Game &game, Color player, Outcome move);
// REQUIRES:      game is well-formed
//                given the game state and the card that is
//                currently at the top of the deck.
// MODIFIES:      game
// EFFECTS:       applies the changes to game that result from player making move
//                Should produce the appropriate output for the *move*, but not
//                any output relating to drawing cards. Consumes the card
//                at the top of the deck, but does *not* shuffle/reset if
//                this is the last card.

int myMain(int argc, char *argv[]);
// REQUIRES:      argc and argv are the arguments to the program.
// EFFECTS:       performs the sorry simulation as specified.

//////////////////////////////////////////////////////////////////
/// SOME FUNCTIONS
//////////////////////////////////////////////////////////////////

void fileReader(Game &game);

void fileReader2(Game &game);

int howMany(int readerInt, string readerString);

void readTriplets(Game &game, int numSquares, string col2, int col1);

void readTriplets2(Game &game, int numSquares, string col2, int col1);

void initializeBoard(Game &game, string col2, int col1);

void initializeBoard2(Game &game, string col2, int col1);

SquareKind converterS(string question);

SquareKind converterSquareKind2(string question);
	
Color converterC(string question);

Color converterColor2(string question);

void deckReader(Game &game);

CardType converterCardType(string question);

void initializePlayers(Game &game, int inc, Color colorPlaceholder, SquareKind squareKindPlaceholder);

int main () {

    Game game;
    fileReader2(game);
    deckReader(game);
    
    return 0;
    
}

void fileReader(Game &game) {

    ifstream reader;
    reader.open("board.txt");
    
    // a couple agents and readers
    bool fileBeginning = 1;	
    int readerInt = 0;
    string readerString = "";
    int col1 = 0;
    string col2 = "";
    string col3 = "";
    int inc = 0;

    game.board.numSquares = howMany(readerInt, readerString);
    //    readTriplets(game, game.board.numSquares, readerString, readerInt);
    initializeBoard(game, readerString, readerInt);
    readTriplets2(game, game.board.numSquares,readerString, readerInt);
    
    reader >> readerString;
    while(!reader.eof()) {

	// cout << readerString << " is readerString";

	// READS IN numSquares
	if (readerString == "SQUARES" && fileBeginning) {

	    fileBeginning = 0;
	    reader >> readerInt;
	    game.board.numSquares = readerInt;
	    //      cout << game.board.numSquares
	    //      cout << readerInt;
	    //      cout << "is readerInt";

	}
    
	// READS IN TRIPLETS AND MAKES BOARD

	else {

	    reader >> col1 >> col2 >> col3;

	    if (inc == col1) {

	    }

	    else {

	    }
	}
    }
}

void fileReader2(Game &game) {
    ifstream reader;
    reader.open("board.txt");
    
    int readerInt = 0;
    string readerString = "";

    game.board.numSquares = howMany(readerInt, readerString);
    //    initializeBoard(game, readerString, readerInt);
    initializeBoard2(game, readerString, readerInt);
    readTriplets2(game, game.board.numSquares,readerString, readerInt);
}
int howMany(int readerInt, string readerString) {
    ifstream reader;
    reader.open("board.txt");
	   
    reader >> readerString;
    reader >> readerInt;

    return readerInt;
}

void readTriplets(Game &game, int numSquares, string col2, int col1) {
    ifstream reader;
    reader.open("board.txt");

    string col3;
    int inc = 0;

    reader >> col2;
    reader >> col1;

    reader >> col1 >> col2 >> col3;
    while(inc < numSquares) {
       	if(col1 == inc) {
	    if(col2 == "BEGIN" || col2 == "END") {	    
		game.board.squares[inc].Slide.kind = converterS(col2);
		game.board.squares[inc].Slide.color = converterC(col3);
		cout << game.board.squares[inc].Slide.kind << "is the kind " << endl;
		cout << game.board.squares[inc].Slide.color << " is the color " << endl;
	    }	    
	    else if(col2 == "HOMESQ" || col2 == "STARTSQ") {
      		game.board.squares[inc].Ends.kind = converterS(col2);
		game.board.squares[inc].Ends.color = converterC(col3);
	    }
	    cout << "I shoul'dn't be in here" << endl;
	    reader >> col1 >> col2 >> col3;
	    if(col1 == inc) inc = inc - 1;
	}
	else {
	    game.board.squares[inc].Slide.kind = REGULAR;
	    game.board.squares[inc].Ends.kind = REGULAR;
	    game.board.squares[inc].Slide.color = NONE;
	    game.board.squares[inc].Ends.color = NONE;   	    
	}	
       	inc = inc + 1;
    }
}

void readTriplets2(Game &game, int numSquares, string col2, int col1) {

    ifstream reader;
    reader.open("board.txt"); // contains information about the squares of the board

    string col3; // placeholder of color of square
    int inc = 0;
    int oldcol1 = 100; // large and random number
    SquareKind squareKindPlaceholder;
    Color colorPlaceholder;

    reader >> col2; // placeholder of SquareKind
    reader >> col1; // placeholder of square number

    reader >> col1 >> col2 >> col3;
    while(!reader.eof()) {
	// some thought processes
	// for this increment please execute these commands. A mismatch means: change the increment.
       	if(col1 == inc) {
	    if(col2 == "BEGIN" || col2 == "END") {	    
		game.board.squares[inc].Slide.kind = converterSquareKind2(col2);
		game.board.squares[inc].Slide.color = converterColor2(col3);
	    }	    
	    else if(col2 == "HOMESQ" || col2 == "STARTSQ") {
		squareKindPlaceholder = converterSquareKind2(col2);
		colorPlaceholder = converterColor2(col3);

      		game.board.squares[inc].Ends.kind = squareKindPlaceholder;
		game.board.squares[inc].Ends.color = colorPlaceholder;
      		initializePlayers(game, inc, colorPlaceholder, squareKindPlaceholder);
	    }
	    oldcol1 = col1;
	    reader >> col1 >> col2 >> col3;
	    if(oldcol1 == col1) {
		inc = inc - 1;
	    }
	}
       	inc = inc + 1;
    }    
}

void initializeBoard(Game &game, string col2, int col1) {

    ifstream reader;
    reader.open("board.txt");

    string col3;
    int inc = 0;
    int oldcol1 = 100; // a random and large number
    
    // some temporary placeholders
    reader >> col2;
    reader >> col1;

    reader >> col1;
    while(!reader.eof()) {
	// some thought processes
	// for the current set of read data please execute these commands.
	while(inc < col1) {
	    if(inc == oldcol1) {
	    }
	    else {
		game.board.squares[inc].Slide.kind = REGULAR;
		game.board.squares[inc].Ends.kind = REGULAR;
		game.board.squares[inc].Slide.color = NONE;
		game.board.squares[inc].Ends.color = NONE;
	    }	    
	    inc = inc + 1;
	    oldcol1 = col1;	 
	}
	reader >> col2 >> col3 >> col1;
    }
}

void initializeBoard2(Game &game, string col2, int col1) {

    ifstream reader;
    reader.open("board.txt");

    string col3;
    int inc = 0;
    
    // some temporary placeholders
    reader >> col2;
    reader >> col1;

    reader >> col1;
    while(!reader.eof()) {
	// some thought processes
	// for the current set of read data, and the current inc, please execute these commands.
	if (inc == col1) {
	    if(!reader.eof()) { // makes sure the file is intact
		reader >> col2 >> col3 >> col1;
		// the following for loop covers repitions in square numbers
		for (int inc2 = 0; inc2 < 4; inc2 = inc2 + 1) {
		    if (inc == col1) {
			reader >> col2 >> col3 >> col1;
		    }
		}
	    }
	}
	else {
	    // for this increment, inc must be smaller than col1, please execute these commands
	    game.board.squares[inc].Slide.kind = REGULAR;
	    game.board.squares[inc].Ends.kind = REGULAR;
	    game.board.squares[inc].Slide.color = NONE;
	    game.board.squares[inc].Ends.color = NONE;
	}
	inc = inc + 1;
    }
}

SquareKind converterS(string question) {
    
    SquareKind reg = REGULAR;
    SquareKind beg = BEGIN;
    SquareKind en = END;
    SquareKind st = STARTSQ;
    SquareKind ho = HOMESQ;

    if(question == "BEGIN") {
	return beg;
    }
    else if(question == "END") {
	return en;
    }
    else if(question == "STARTSQ") {
	return st;
    }
    else if(question == "HOMESQ") {
	return ho;
    }

}

SquareKind converterSquareKind2(string question) {
    if(question == "BEGIN") {
	return BEGIN;
    }
    else if(question == "END") {
	return END;
    }
    else if(question == "STARTSQ") {
	return STARTSQ;
    }
    else if(question == "HOMESQ") {
	return HOMESQ;
    }
}

Color converterC(string question) {

    Color bl = BLUE;
    Color ye = YELLOW;
    Color gr = GREEN;
    Color re = RED;

    if(question == "BLUE") {
	return bl;
    }
    if(question == "YELLOW") {
	return ye;
    }
    if(question == "GREEN") {
	return gr;
    }
    if(question == "RED") {
	return re;
    }
}

Color converterColor2(string question) {
    if(question == "BLUE") {
	return BLUE;
    }
    if(question == "YELLOW") {
	return YELLOW;
    }
    if(question == "GREEN") {
	return GREEN;
    }
    if(question == "RED") {
	return RED;
    }
}

void deckReader(Game &game) {
    ifstream reader;
    reader.open("deck.txt");

    int num = 0;
    reader >> num;
    game.deck.numCards = num;

    string col1 = "";
    int inc = 0;
    int col2 = 0;
    reader >> col1 >> col2;
    while(!reader.eof()) {
	if(col1 == "START" || col1 =="SWAP" || col1 == "SORRY") {
	    game.deck.cards[inc].type = converterCardType(col1);
	}
	if(col1 == "BACKWARD" || col1 == "FORWARD") {
	    game.deck.cards[inc].type  = converterCardType(col1);
	    game.deck.cards[inc].value = col2;
	}
	reader >> col1 >> col2;
	inc = inc + 1;
     }
}

CardType converterCardType(string question) {
    if(question == "START") return START;
    if(question == "FORWARD") return FORWARD;
    if(question == "BACKWARD") return BACKWARD;
    if(question == "SWAP") return SWAP;
    if(question == "SORRY") return SORRY;
}

Outcome eval(const Game &game, Color player) {
    
}

void initializePlayers(Game &game, int inc, Color colorPlaceholder, SquareKind squareKindPlaceholder) {
    if (squareKindPlaceholder == STARTSQ) {
	game.Players[colorPlaceholder].startSquare = inc;
    }
    else if (squareKindPlaceholder == HOMESQ) {
	game.Players[colorPlaceholder].homeSquare = inc;
    }
    for (int inc2 = 0; inc2 < 4; inc2 = inc2 + 1) {
	game.Players[colorPlaceholder].pawns[inc2].state = STARTABLE;	
    }
}
