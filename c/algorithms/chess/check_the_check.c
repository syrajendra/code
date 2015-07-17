#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <fstream>

using namespace std;

#ifdef DEBUG
	#define TRACE(x)	x
#else
	#define TRACE(x)
#endif

#define ATTACK_CHECK(ch, attacker1, attacker2)	{\
			if (ch == '.') continue;			\
			else if (ch == attacker1 || ch == attacker2) { \
				TRACE(cout << "[attacker = " << ch << " ");	\
				return true;	\
			} \
			else break; } /* NO jumping over other piece	 */	\

#define NUM_OF_ROWS 8
#define NUM_OF_COLS 8
#define MAX_PIECES	16

typedef struct {
	char ch;
	int  x;
	int  y;
} piece_t;

typedef enum {
	NO,
	STRIGHT,
	DIAGONAL,
	CIRCLE,
	WRONG
} attact_type_t;

int (*is_opponent) (int c);
int (*is_associate)(int c);

class Attackers;

class Chess
{
private:
	Attackers 	*attacker;
	bool isblack;

public:
	char board[NUM_OF_ROWS][NUM_OF_COLS];
	piece_t black_pieces[MAX_PIECES];
	piece_t white_pieces[MAX_PIECES];
	int num_black;
	int num_white;

	Chess ();
	~Chess ();
	void display_board ();
	void clear_board ();
	void set_isblack(bool type);
	bool is_empty(int x, int y);
	bool is_safe(int x, int y);
	attact_type_t is_under_attack (int x, int y);
	bool check_for_check(int x, int y);
	bool check_for_checkmate(int x, int y);
	void find_piece_position(char piece, int *x, int *y);
	void update_all_piece_position();
};

class Attackers
{
public:
	Chess *chess;
	char stright_attackers[2];
	char diagonal_attackers[2];
	char circle_attacker;
	char diagonal_attacker_pawn;

	virtual bool is_pawn_attacking(int x, int y) = 0;
	bool is_stright_attack(int x, int y);
	bool is_diagonal_attack(int x, int y);
	bool is_circle_attack(int x, int y);
};

bool Attackers::is_stright_attack(int x, int y)
{
	// east attack
	for(int i = 1; i < (NUM_OF_COLS - y); i++) {
		char ch = chess->board[x][y+i];
		ATTACK_CHECK(ch, stright_attackers[0], stright_attackers[1]);
	}
	// west attack
	for(int i = 1; i <= y; i++) {
		char ch = chess->board[x][y-i];
		ATTACK_CHECK(ch, stright_attackers[0], stright_attackers[1]);
	}
	// NOrth attack
	for(int i = 1; i <= x; i++) {
		char ch = chess->board[x-i][y];
		ATTACK_CHECK(ch, stright_attackers[0], stright_attackers[1]);
	}
	// south attack
	for(int i = 1; i < (NUM_OF_ROWS - x); i++) {
		char ch = chess->board[x+i][y];
		ATTACK_CHECK(ch, stright_attackers[0], stright_attackers[1]);
	}
	return false;
}

bool Attackers::is_diagonal_attack (int x, int y)
{
	if(is_pawn_attacking(x, y)) return true;
	// NOrth-east attack
	for(int i = 1, j = 1; (i <= x) && (j < (NUM_OF_COLS - y)); i++, j++) {
		char ch = chess->board[x-i][y+j];
		ATTACK_CHECK(ch, diagonal_attackers[0], diagonal_attackers[1]);
	}
	// NOrth-west attack
	for(int i = 1, j = 1; (i <= x) && (j <= y); i++, j++) {
		char ch = chess->board[x-i][y-j];
		ATTACK_CHECK(ch, diagonal_attackers[0], diagonal_attackers[1]);
	}
	// south-west attack
	for(int i = 1, j = 1; (i < (NUM_OF_ROWS - x)) && (j <= y); i++, j++) {
		char ch = chess->board[x+i][y-j];
		ATTACK_CHECK(ch, diagonal_attackers[0], diagonal_attackers[1]);
	}
	// south-east attack
	for(int i = 1, j = 1; (i < (NUM_OF_ROWS - x)) && (j < (NUM_OF_COLS - y)); i++, j++) {
		char ch = chess->board[x+i][y+j];
		ATTACK_CHECK(ch, diagonal_attackers[0], diagonal_attackers[1]);
	}
	return false;
}

bool Attackers::is_circle_attack (int x, int y)
{
	char ch[8] = { '.' };
	// NOrth-NOrth-east attack
	if (x-2 >= 0 && y+1 < NUM_OF_COLS) ch[0] = chess->board[x-2][y+1];
	// east-NOrth-east attack
	if (x-1 >= 0 && y+2 < NUM_OF_COLS) ch[1] = chess->board[x-1][y+2];

	// NOrth-NOrth-west attack
	if (x-2 >= 0 && y-1 >= 0) ch[2] = chess->board[x-2][y-1];
	// west-NOrth-west attack
	if (x-1 >= 0 && y-2 >= 0) ch[3] = chess->board[x-1][y-2];

	// west-south-west attack
	if (x+1 < NUM_OF_ROWS && y-2 >= 0) ch[4] = chess->board[x+1][y-2];
	// south-south-west attack
	if (x+2 < NUM_OF_ROWS && y-1 >= 0) ch[5] = chess->board[x+2][y-1];

	// east-south-east attack
	if (x+1 < NUM_OF_ROWS && y+2 < NUM_OF_COLS) ch[6] = chess->board[x+1][y+2];
	// south-south-east attack
	if (x+2 < NUM_OF_ROWS && y+1 < NUM_OF_COLS) ch[7] = chess->board[x+2][y+1];

	for (int i=0; i<8; i++) {
		if (ch[i] == circle_attacker) {
			TRACE(cout << "[attacker = " << circle_attacker << " ");
			return true;
		}
	}
	return false;
}

class BlackAttackers : public Attackers
{
public:
	BlackAttackers(Chess *ptr);
	bool is_pawn_attacking(int x, int y);
	~BlackAttackers() {}
};

class WhiteAttackers : public Attackers
{
public:
	WhiteAttackers(Chess *ptr);
	bool is_pawn_attacking(int x, int y);
	~WhiteAttackers() {}
};


WhiteAttackers::WhiteAttackers(Chess *ptr)
{
	chess = ptr;
	stright_attackers[0] 	= 'R';
	stright_attackers[1] 	= 'Q';
	diagonal_attackers[0]	= 'B';
	diagonal_attackers[1]	= 'Q';
	diagonal_attacker_pawn  = 'P';
	circle_attacker			= 'N';
}

bool WhiteAttackers::is_pawn_attacking (int x, int y)
{
	if (x+1 < NUM_OF_ROWS && y-1 >= 0 && chess->board[x+1][y-1] == diagonal_attacker_pawn ||
		x+1 < NUM_OF_ROWS && y+1 < NUM_OF_COLS && chess->board[x+1][y+1] == diagonal_attacker_pawn) {
		TRACE(cout << "[attacker = P ");
		return true;
	}
	return false;
}

BlackAttackers::BlackAttackers(Chess *ptr)
{
	chess = ptr;
	stright_attackers[0] 	= 'r';
	stright_attackers[1] 	= 'q';
	diagonal_attackers[0]	= 'b';
	diagonal_attackers[1]	= 'q';
	diagonal_attacker_pawn  = 'p';
	circle_attacker			= 'n';
}

bool BlackAttackers::is_pawn_attacking (int x, int y)
{
	if (x-1 >= 0 && y-1 >= 0 && chess->board[x-1][y-1] == diagonal_attacker_pawn ||
		x-1 >= 0 && y+1 < NUM_OF_COLS &&  chess->board[x-1][y+1] == diagonal_attacker_pawn) {
		TRACE(cout << "[attacker = p ");
		return true;
	}
	return false;
}

void Chess::clear_board()
{
	for (int i = 0 ; i < NUM_OF_ROWS ; i++ )
		for (int j = 0 ; j < NUM_OF_COLS ; j++ )
			board[i][j] = '.';
}

void Chess::set_isblack (bool type)
{
	isblack = type;
	if (isblack) {
		attacker = new WhiteAttackers(this);
		is_opponent = &isupper;
		is_associate= &islower;
	} else {
		attacker = new BlackAttackers(this);
		is_opponent = &islower;
		is_associate= &isupper;
	}
}

Chess::~Chess()
{
	delete attacker;
}

Chess::Chess()
{
	attacker = NULL;
	num_white= 0;
	num_black= 0;
	clear_board();
}

void Chess::display_board ()
{
	for (int i = 0 ; i < NUM_OF_ROWS ; i++ ) {
		for (int j = 0 ; j < NUM_OF_COLS ; j++ ) {
			printf(" %c", board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

template<size_t N >
int read_input_file (Chess (&cb)[N])
{
	string line;
	int cnt = 0;
	ifstream fs ("input.txt");
	if (fs.is_open()) {
		cb[cnt].clear_board();
		int i = 0;
		while (getline (fs, line)) {
			if (!line.size()) {
				cnt++;
				cb[cnt].clear_board();
				i = 0;
				continue;// skip line
			} else if (line[0] == '#') continue;
			if (i < NUM_OF_ROWS && line.size() == NUM_OF_COLS) {
				for (int j = 0 ; j < NUM_OF_COLS ; j++) {
					cb[cnt].board[i][j] = line[j];
				}
			} else {
				cout << "Error: WRONG input line. [" << line << "] Exiting..." << endl;
				exit (1);
			}
			i++;
		}
		fs.close();
	} else cout << "Error: Failed to open file" << endl;
	return cnt;
}

attact_type_t Chess::is_under_attack (int x, int y)
{
	if( x >= 0 && x < NUM_OF_ROWS &&
		y >= 0 && y < NUM_OF_COLS) {
		if (attacker->is_stright_attack (x, y)) {
			TRACE(cout << ": stright attack @ " << x << ":" << y << "] ");
			return STRIGHT;
		} else if (attacker->is_diagonal_attack(x, y)) {
			TRACE(cout << ": diagonal attack @ " << x << ":" << y << "] ");
			return DIAGONAL;
		} else if (attacker->is_circle_attack (x, y)) {
			TRACE(cout << ": circle attack @ " << x << ":" << y << "] ");
			return CIRCLE;
		} else {
			TRACE(cout << "[under NO attack @ " << x << ":" << y << "] ");
			return NO;
		}
	} else 	return WRONG;
	return NO;
}

void Chess::find_piece_position(char piece, int *x, int *y)
{
	set_isblack(islower(piece) ? true : false);
	TRACE(cout << "Piece [" << piece << "] is " << (isblack ? "black" : "white") << endl);
	for (int i = 0 ; i < NUM_OF_ROWS ; i++ ) {
		for (int j = 0 ; j < NUM_OF_COLS ; j++ ) {
			if (piece == board[i][j]) {
				*x = i;
				*y = j;
				return;
			}
		}
	}
}

void Chess::update_all_piece_position()
{
	for (int i = 0 ; i < NUM_OF_ROWS ; i++ ) {
		for (int j = 0 ; j < NUM_OF_COLS ; j++ ) {
			if (board[i][j] == '.') continue;
			if(islower(board[i][j])) {
				black_pieces[num_black].x   = i;
				black_pieces[num_black].y   = j;
				black_pieces[num_black++].ch= board[i][j];
			} else {
				white_pieces[num_white].x   = i;
				white_pieces[num_white].y   = j;
				white_pieces[num_white++].ch= board[i][j];
			}
		}
	}
}

bool Chess::is_empty(int x, int y)
{
	return (board[x][y] == '.') ? true : false;
}

bool Chess::is_safe(int x, int y)
{
	if (is_empty(x, y)) return true;
	if (is_opponent(board[x][y])) return true;
	return false;
}

bool Chess::check_for_checkmate(int x, int y)
{
	attact_type_t at;
	if (NO == (at = is_under_attack(x, y))) 	return false;
	if ((is_safe(x, y+1))   &&
		(NO == is_under_attack(x, y+1)))		return false; //east
	if ((is_safe(x, y-1))   &&
		(NO == is_under_attack(x, y-1)))		return false; // west
	if ((is_safe(x-1, y))   &&
		(NO == is_under_attack(x-1, y)))		return false; // north
	if ((is_safe(x+1, y))   &&
		(NO == is_under_attack(x+1, y)))		return false; // south
	if ((is_safe(x-1, y+1)) &&
		(NO == is_under_attack(x-1, y+1)))		return false; // north-east
	if ((is_safe(x-1, y-1)) &&
		(NO == is_under_attack(x-1, y-1))) 		return false; // north-west
	if ((is_safe(x+1, y-1)) &&
		(NO == is_under_attack(x+1, y-1))) 		return false; // south-west
	if ((is_safe(x+1, y+1)) &&
		(NO == is_under_attack(x+1, y+1))) 		return false; // south-east
	if (at == CIRCLE) return true; // Knight is attacker
	update_all_piece_position ();
	//if (at == STRIGHT) {
	//
	//} else if (at == DIAGONAL) {
	//
	//}
	//  is valid move == king attcked place
	//  return false;
	return true;
}

bool Chess::check_for_check(int x, int y)
{
	if(is_under_attack(x, y)) return true;
	return false;
}

void check_kings (int game, Chess &cb)
{
	cout << "Game " << game << ": " << endl;
	int x = 0, y = 0;

	cb.find_piece_position ('k', &x, &y);
	cout << "black king ";
	if (cb.check_for_check(x, y)) cout << "is in check" << endl;
	else cout << "is in NO check" << endl;

	cout << "black king ";
	if (cb.check_for_checkmate(x, y)) cout << "is in checkmate" << endl;
	else cout << "is in NO checkmate situation" << endl;

	cb.find_piece_position ('K', &x, &y);
	cout << "white king ";
	if (cb.check_for_check(x, y)) cout << "is in check" << endl;
	else cout << "is in NO check" << endl;

	cout << "white king ";
	if (cb.check_for_checkmate(x, y)) cout << "is in checkmate" << endl;
	else cout << "is in NO checkmate situation" << endl;

	cout << endl;
}

int main (int argc, char *argv[])
{
	//cout << "Chess Board" << endl;
	Chess cb[100];
	//cb[0].display_board ();
	cout << "\nProgram to check if king is under attack" << endl;
	int cnt = read_input_file (cb);
	for (int i = 0; i < cnt; i++) {
		check_kings(i, cb[i]);
		cb[i].display_board();
		cout << endl;
	}
	return 0;
}
