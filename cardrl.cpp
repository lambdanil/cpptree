#include<iostream>
#include<sstream>
#include<time.h>
#include<chrono>
#include<algorithm>
#include<random>
#include<stdlib.h>
#include<string>
#include<cstdlib>
#include<string.h>
#include<vector>
#include<fstream>
#include "cpptree.h" // https://github.com/CuBeRJAN/cpptree
                     // it's not stealing if it's my own library

#define EFFECT_LENGTH 100


// This game is a Slay the Spire ripoff

using std::cout;    using std::cin;
using std::string;  using std::vector;

#include  <iterator>

struct {
    const string cyan = "\033[36m";
    const string magenta = "\033[35m";
    const string red = "\033[91m";
    const string gray = "\033[8m";
    const string green = "\033[92m";
    const string yellow = "\033[33m";
    const string end = "\033[0m";
} colors;

// randomization code here is copied from stackoverflow
template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}


// This part is copied code
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
void cls() {
    system("cls");
}
void game_quit() {
    cout << "Game over!\n";
    char x;
    cin >> x;
    exit(0);
}
int key_press() { // not working: F11 (-122, toggles fullscreen)
    KEY_EVENT_RECORD keyevent;
    INPUT_RECORD irec;
    DWORD events;
    while (true) {
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &irec, 1, &events);
        if (irec.EventType == KEY_EVENT && ((KEY_EVENT_RECORD&)irec.Event).bKeyDown) {
            keyevent = (KEY_EVENT_RECORD&)irec.Event;
            const int ca = (int)keyevent.uChar.AsciiChar;
            const int cv = (int)keyevent.wVirtualKeyCode;
            const int key = ca == 0 ? -cv : ca + (ca > 0 ? 0 : 256);
            switch (key) {
            case  -16: continue; // disable Shift
            case  -17: continue; // disable Ctrl / AltGr
            case  -18: continue; // disable Alt / AltGr
            case -220: continue; // disable first detection of "^" key (not "^" symbol)
            case -221: continue; // disable first detection of "`" key (not "`" symbol)
            case -191: continue; // disable AltGr + "#"
            case  -52: continue; // disable AltGr + "4"
            case  -53: continue; // disable AltGr + "5"
            case  -54: continue; // disable AltGr + "6"
            case  -12: continue; // disable num block 5 with num lock deactivated
            case   13: return  10; // enter
            case  -46: return 127; // delete
            case  -49: return 251; // ¹
            case    0: continue;
            case    1: continue; // disable Ctrl + a (selects all text)
            case    2: continue; // disable Ctrl + b
            case    3: continue; // disable Ctrl + c (terminates program)
            case    4: continue; // disable Ctrl + d
            case    5: continue; // disable Ctrl + e
            case    6: continue; // disable Ctrl + f (opens search)
            case    7: continue; // disable Ctrl + g
            //case    8: continue; // disable Ctrl + h (ascii for backspace)
            //case    9: continue; // disable Ctrl + i (ascii for tab)
            case   10: continue; // disable Ctrl + j
            case   11: continue; // disable Ctrl + k
            case   12: continue; // disable Ctrl + l
            //case   13: continue; // disable Ctrl + m (breaks console, ascii for new line)
            case   14: continue; // disable Ctrl + n
            case   15: continue; // disable Ctrl + o
            case   16: continue; // disable Ctrl + p
            case   17: continue; // disable Ctrl + q
            case   18: continue; // disable Ctrl + r
            case   19: continue; // disable Ctrl + s
            case   20: continue; // disable Ctrl + t
            case   21: continue; // disable Ctrl + u
            case   22: continue; // disable Ctrl + v (inserts clipboard)
            case   23: continue; // disable Ctrl + w
            case   24: continue; // disable Ctrl + x
            case   25: continue; // disable Ctrl + y
            case   26: continue; // disable Ctrl + z
            default: return key; // any other ASCII/virtual character
            }
        }
    }
}
#elif defined(__linux__)
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
void cls() {
    system("clear");
}
void game_quit() {
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
    cout << "Game over!\n";
    exit(0);
}
int key_press() { // not working: ¹ (251), num lock (-144), caps lock (-20), windows key (-91), kontext menu key (-93)
    struct termios term;
    tcgetattr(0, &term);
    while (true) {
        term.c_lflag &= ~(ICANON | ECHO); // turn off line buffering and echoing
        tcsetattr(0, TCSANOW, &term);
        int nbbytes;
        ioctl(0, FIONREAD, &nbbytes); // 0 is STDIN
        while (!nbbytes) {
            sleep(0.01);
            fflush(stdout);
            ioctl(0, FIONREAD, &nbbytes); // 0 is STDIN
        }
        int key = (int)getchar();
        if (key == 27 || key == 194 || key == 195) { // escape, 194/195 is escape for °ß´äöüÄÖÜ
            key = (int)getchar();
            if (key == 91) { // [ following escape
                key = (int)getchar(); // get code of next char after \e[
                if (key == 49) { // F5-F8
                    key = 62 + (int)getchar(); // 53, 55-57
                    if (key == 115) key++; // F5 code is too low by 1
                    getchar(); // take in following ~ (126), but discard code
                }
                else if (key == 50) { // insert or F9-F12
                    key = (int)getchar();
                    if (key == 126) { // insert
                        key = 45;
                    }
                    else { // F9-F12
                        key += 71; // 48, 49, 51, 52
                        if (key < 121) key++; // F11 and F12 are too low by 1
                        getchar(); // take in following ~ (126), but discard code
                    }
                }
                else if (key == 51 || key == 53 || key == 54) { // delete, page up/down
                    getchar(); // take in following ~ (126), but discard code
                }
            }
            else if (key == 79) { // F1-F4
                key = 32 + (int)getchar(); // 80-83
            }
            key = -key; // use negative numbers for escaped keys
        }
        term.c_lflag |= (ICANON | ECHO); // turn on line buffering and echoing
        tcsetattr(0, TCSANOW, &term);
        switch (key) {
        case  127: return   8; // backspace
        case  -27: return  27; // escape
        case  -51: return 127; // delete
        case -164: return 132; // ä
        case -182: return 148; // ö
        case -188: return 129; // ü
        case -132: return 142; // Ä
        case -150: return 153; // Ö
        case -156: return 154; // Ü
        case -159: return 225; // ß
        case -181: return 230; // µ
        case -167: return 245; // §
        case -176: return 248; // °
        case -178: return 253; // ²
        case -179: return 252; // ³
        case -180: return 239; // ´
        case  -65: return -38; // up arrow
        case  -66: return -40; // down arrow
        case  -68: return -37; // left arrow
        case  -67: return -39; // right arrow
        case  -53: return -33; // page up
        case  -54: return -34; // page down
        case  -72: return -36; // pos1
        case  -70: return -35; // end
        case    0: continue;
        case    1: continue; // disable Ctrl + a
        case    2: continue; // disable Ctrl + b
        case    3: continue; // disable Ctrl + c (terminates program)
        case    4: continue; // disable Ctrl + d
        case    5: continue; // disable Ctrl + e
        case    6: continue; // disable Ctrl + f
        case    7: continue; // disable Ctrl + g
        case    8: continue; // disable Ctrl + h
        //case    9: continue; // disable Ctrl + i (ascii for tab)
        //case   10: continue; // disable Ctrl + j (ascii for new line)
        case   11: continue; // disable Ctrl + k
        case   12: continue; // disable Ctrl + l
        case   13: continue; // disable Ctrl + m
        case   14: continue; // disable Ctrl + n
        case   15: continue; // disable Ctrl + o
        case   16: continue; // disable Ctrl + p
        case   17: continue; // disable Ctrl + q
        case   18: continue; // disable Ctrl + r
        case   19: continue; // disable Ctrl + s
        case   20: continue; // disable Ctrl + t
        case   21: continue; // disable Ctrl + u
        case   22: continue; // disable Ctrl + v
        case   23: continue; // disable Ctrl + w
        case   24: continue; // disable Ctrl + x
        case   25: continue; // disable Ctrl + y
        case   26: continue; // disable Ctrl + z (terminates program)
        default: return key; // any other ASCII character
        }
    }
}
#endif // Windows/Linux


void end_game() {
    cls();
    cout << "You lost!\n";
    exit(0);
}

class card {
public:
    card(string n, string d, vector<int> vec, string ef, int c, int r, string cl, int tp) {
        name = n;
        type = tp;
        color = cl;
        rarity = r;
        desc = d;
        cost = c;
        values = vec;
        strcpy(effect, ef.c_str());
    }

    vector<int> values;
    int rarity; // 0 common, 1 uncommon, 2 rare, 3 very rare, 4 - upgraded cards
    int type; // 0 attack, 1 skill
    int cost;
    string color;
    string name;
    string desc;
    char effect[EFFECT_LENGTH];
};

// Player card piles
class pile {
public:
    vector<card> hand;
    vector<card> discard;
    vector<card> deck;
    vector<card> draw;
};

class player;
void discard_hand(player*, pile*);
void draw_hand(player*, pile*);
void check_bufferlen();

class player { // TODO: There surely is a cleaner way than having 20000 variables for status effects
public:
    string name;
    int vulnerable = 0;
    int drawcards = 4;
    int nlevel = 0; // level of enemies (not of descent!)
    int drawlimit = 10; // how many cards can be in hand at once
    int barricade = 0; // don't lose block for x turns
    int gold = 0;
    int level = 0; // each act has a number of levels, stored here
    int dont_discard_hand = 0; // don't discard hand for x turns
    int dont_draw = 0;
    int act = 0; // game is split into 3 acts, this stores the act number
    int poison = 0;
    int frail = 0; // weaken block cards
    int maxmana = 2;
    int mana;
    int weak = 0; // -1 weak each turn
    int hp = 25;
    int maxhp = 25;
    int block = 0;
    int strength = 0;
    bool confused = false; // confusion effect

    void begin_turn(pile* pl_cards) {
        pl_discard_hand(pl_cards);
        if (!dont_draw)
            draw_hand(this, pl_cards); // draw new cards
        hp -= poison; // apply poison
        if (hp < 1) end_game(); // end game if hp <= 0
        mana = maxmana; // refresh mana
    }

    void take_damage(int dmg) {
        int rdmg = dmg;
        if (vulnerable) rdmg *= 1.5;
        int obl = block;
        if (rdmg >= block) {
            block = 0;
            hp -= rdmg - obl;
        }
        else
            block -= rdmg;
    }


    // piercing damage
    void take_damage_forced(int dmg) {
        int rdmg = dmg;
        if (vulnerable) rdmg *= 1.5;
        hp -= rdmg;
    }

    int mult_dmg_from(int dmg) {
        if (weak) return (dmg + (dmg * (0.2 * strength))) * 0.6;
        return (dmg + (dmg * (0.2 * strength)));
    }

    int mult_dmg_to(int dmg) {
        if (vulnerable) return dmg * 1.5;
        return dmg;
    }

    void addblock(int blc) {
        if (!frail)
            block += blc;
        else
            block += blc * 0.7;
    }

    void remove_mana(int n) {
        mana -= n;
    }

    void clear_block() {
        if (!barricade) block = 0;
    }

    int mult_block(int blc) {
        if (frail)
            blc *= 0.7;
        return blc;
    }

    void decrease_counters() {
        if (poison)
            take_damage_forced(poison);
        clear_block();
        if (poison) poison--;
        if (vulnerable) vulnerable--;
        if (barricade) barricade--;
        if (weak) weak--;
        if (dont_discard_hand) dont_discard_hand--;
        if (dont_draw) dont_draw--;
    }

    void pl_discard_hand(pile* plc) {
        if (!barricade)
            discard_hand(this, plc);
    }
};

void shuffle_stringvec(vector<string>*);

class enemy;
void eval_effect(char effect[EFFECT_LENGTH], player* plr, enemy* enemy, pile* pl_pile);
class enemy {
public:
    string name;
    int poison = 0;
    int maxmana = 4; // Enemies have mana, may be useful some time
    int mana;
    int hp;
    int maxhp;
    int vulnerable = 0;
    int barricade = 0;
    int weak = 0;
    int block = 0;
    int strength = 0;
    int level;
    int frail = 0; // Lower block effectiveness
    string intention;
    vector<string> actions;
    int intention_counter_max;
    int intention_counter = 0;

    enemy(string n, int mhp, int l, vector<string> ac) {
        name = n;
        maxhp = mhp;
        level = l;
        actions = ac;
        intention_counter_max = ac.size();
        hp = maxhp;
    }


    int mult_dmg_from(int dmg) {
        if (weak) return (dmg + (dmg * (0.2 * strength))) * 0.6;
        return (dmg + (dmg * (0.2 * strength)));
    }

    int mult_dmg_to(int dmg) {
        if (vulnerable) return dmg * 1.5;
        return dmg;
    }

    void clear_block() {
        if (!barricade) block = 0;
    }

    int mult_block(int blc) {
        if (frail)
            blc *= 0.7;
        return blc;
    }

    void take_damage(int dmg) {
        int rdmg = dmg;
        if (vulnerable) rdmg *= 1.5;
        int obl = block;
        if (rdmg >= block) {
            block = 0;
            hp -= rdmg - obl;
        }
        else
            block -= rdmg;
    }

    // piercing damage
    void take_damage_forced(int dmg) {
        int rdmg = dmg;
        if (vulnerable) rdmg *= 1.5;
        hp -= rdmg;
    }

    bool check_hp() {
        if (hp < 1) {
            return false;
        }
        return true;
    }

    void addblock(int blc) {
        if (!frail)
            block += blc;
        else
            block += blc * 0.7;
    }

    void remove_mana(int n) {
        mana -= n;
    }

    string get_intention() {
        if (intention_counter >= intention_counter_max - 1) {
            shuffle_stringvec(&actions);
            intention_counter = 0;
        }
        else {
            intention_counter++;
        }
        intention = actions.at(intention_counter);
        return intention;
    }

    void commit_intention(player* pl, pile* plc) {
        char ef[EFFECT_LENGTH];
        strcpy(ef, intention.c_str());
        eval_effect(ef, pl, this, plc);
    }

    void begin_turn() {
        if (barricade) barricade--;
        if (poison)
            take_damage_forced(poison);
        else block = 0;
    }

    void decrease_counters() {
        clear_block();
        if (poison) poison--;
        if (vulnerable) vulnerable--;
        if (barricade) barricade--;
        if (weak) weak--;
    }

};


void discard_from_hand(pile* pl_cards, int index);
void exhaust_from_hand(pile* pl_cards, int index);

void buffer_send(string tosend) {
    cout << tosend;
}

string msgbuffer; // global variable!!
void buffer_queue(string q) {
    check_bufferlen();
    msgbuffer += q + "\n";
}

char get_onechar() {
    char ret;
    cin >> ret;
    return ret;
}

int select_from_hand(pile* plc, string msg) {
    buffer_send(msg);
    char inp = key_press();
    return (inp - '0' - 1);
}

void buffer_flush() {
    msgbuffer = "";
}

void check_bufferlen() {
    int lines = 0;
    for (unsigned long int i = 0; i < msgbuffer.length(); i++) {
        if (msgbuffer.at(i) == '\n') lines++;
    }
    int it = msgbuffer.length() - 2;
    bool lessline = false;
    bool r = false;
    while (lines > 4) {
        r = true;
        if (msgbuffer.at(it) == '\n') lessline = true;
        msgbuffer.erase(msgbuffer.begin() + it, msgbuffer.end());
        if (lessline) lines--;
        lessline = false;
        it--;
    }
    if (r)
        msgbuffer += '\n';
}

// Evaluate effect of card
// d = dmg
// p = pois
// b = block
// 92d3b3p means 11 damage, 3 block and 3 poison
// [b]lock, [d]amage enemy, dis[c]ard other cards, [p]oison
// [h]eal, e[x]haust other card(s), [H]eal enemy, [D]amage player
// [B]lock enemy, add [s]trength, add [S]trength to enemy
// [w]eaken player, [W]eaken enemy, gain [m]ana
// draw a [C]ard, enemy [l]ose str, player [L]ose str

// there are 2 letters at the end, the second is a condition!
// w means do only if enemy is [w]eak etc.
// [a]lways
void eval_effect(char effect[EFFECT_LENGTH], player* plr, enemy* en, pile* pl_pile) {
    check_bufferlen();
    int tmpnum = 0;
    for (int i = 0; i < EFFECT_LENGTH - 1; i++) {
        if (effect[i] == '\0') break;

        if (isdigit(effect[i])) tmpnum += (effect[i] - '0');
        // Here we check the conditions (in a very ugly way)
        else if (effect[i + 1] == '\0' || (isdigit(effect[i - 1]) && (
            (effect[i + 1] == 'a') || // "always" condition i.e. no condition
            (effect[i + 1] == 'w' && en->weak) || // check for enemy weaken condition
            (effect[i + 1] == 'W' && plr->weak)
            ))) {
            switch (effect[i]) {
                case 'd':
                    en->take_damage(plr->mult_dmg_from(tmpnum));
                    buffer_queue(colors.red + "You hit for " + std::to_string(plr->mult_dmg_from(tmpnum)) + " damage" + colors.end); tmpnum = 0;
                    break;
                case 'D':
                    plr->take_damage(en->mult_dmg_from(tmpnum));
                    buffer_queue(colors.red + "You take " + std::to_string(en->mult_dmg_from(tmpnum)) + " damage" + colors.end); tmpnum = 0;
                    break;
                case 'b':
                    plr->addblock(tmpnum);
                    buffer_queue(colors.cyan + "You gain " + std::to_string(tmpnum) + " block" + colors.end); tmpnum = 0;
                    break;
                case 'B':
                    en->addblock(tmpnum);
                    buffer_queue(colors.cyan + "Enemy gains " + std::to_string(tmpnum) + " block" + colors.end); tmpnum = 0;
                    break;
                case 's':
                    plr->strength += tmpnum;
                    buffer_queue(colors.magenta + "You gain " + std::to_string(tmpnum) + " strength" + colors.end); tmpnum = 0;
                    break;
                case 'S':
                    en->strength += tmpnum;
                    buffer_queue(colors.magenta + "Enemy gains " + std::to_string(tmpnum) + " strength" + colors.end); tmpnum = 0;
                    break;
                case 'w':
                    plr->weak += tmpnum + 1; // +1 because weaken gets removed at the start of player turn
                    buffer_queue(colors.magenta + "Enemy weakens you for " + std::to_string(tmpnum) + " more turn(s)" + colors.end); tmpnum = 0;
                    break;
                case 'W':
                    en->weak += tmpnum + 1; // +1 because weaken gets removed at start of enemy turn
                    buffer_queue(colors.magenta + "You weaken enemy for " + std::to_string(tmpnum + 1) + " more turn(s)" + colors.end); tmpnum = 0;
                    break;
                case 'l':
                    en->strength -= tmpnum;
                    buffer_queue(colors.red + "Enemy loses " + std::to_string(tmpnum + 1) + " strength" + colors.end); tmpnum = 0;
                    break;
                case 'L':
                    plr->strength -= tmpnum;
                    buffer_queue(colors.red + "You lose " + std::to_string(tmpnum + 1) + " strength" + colors.end); tmpnum = 0;
                    break;
                case 'm':
                    plr->mana += 1; // mana is allowed over maxmana counter
                    tmpnum = 0;
                    break;
                case 'c':
                    for (int i = 0; i < tmpnum; i++) {
                        if (pl_pile->hand.size() > 0) {
                            int choice = select_from_hand(pl_pile, "Select a card to discard: ");
                            discard_from_hand(pl_pile, choice);
                        }
                        else break;
                    }
                    tmpnum = 0;
                break;
            }
        }
        else tmpnum = 0;
    }
}

// convert effect number to int (991 to 19 etc.)
int efnum_to_int(string efnum) {
    int ret = 0;
    for (long unsigned int i = 0; i < efnum.size(); i++) {
        ret += (int)(efnum[i] - '0');
    }
    return ret;
}

// convert int to effect number (19 to 991 etc.)
string int_to_efnum(int real) {
    //string ri = std::to_string(real);
    string concat = "";
    for (int i = 0; i < ((real - (real % 9)) / 9); i++) {
        concat += "9";
    }
    concat += std::to_string(real % 9);
    return concat;
}

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine shuffle_myseed(seed);

// shuffle deck
void shuffle_deck(vector<card>* dc) {
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(dc->begin(), dc->end(), shuffle_myseed);
}

void shuffle_stringvec(vector<string>* dc) {
    std::shuffle(dc->begin(), dc->end(), shuffle_myseed);
}

// initialize some stuff at the beginning of a fight
void start_fight(player* pl, pile* pl_cards) {
    pl->weak = 0;
    pl->poison = 0;
    pl->block = 0;
    pl->barricade = 0;
    pl_cards->hand.clear();
    pl_cards->discard.clear();
    pl_cards->draw = pl_cards->deck;
    shuffle_deck(&pl_cards->draw);
}

// draw a full hand of cards
void draw_hand(player* pl, pile* pl_cards) {
    for (int i = 0; i < pl->drawcards; i++) {
        if (pl_cards->draw.size() > 0) {
            pl_cards->hand.push_back(pl_cards->draw.at(0));
            pl_cards->draw.erase(pl_cards->draw.begin());
        }
        else if (pl_cards->discard.size() > 0) {
            pl_cards->draw = pl_cards->discard;
            shuffle_deck(&pl_cards->draw);
            pl_cards->hand.push_back(pl_cards->draw.at(0));
            pl_cards->draw.erase(pl_cards->draw.begin());
            pl_cards->discard.erase(pl_cards->discard.begin(), pl_cards->discard.end());
        }
        else break;
    }
}

// make intention into readable string
string enemy_intention_to_string(player*pl, enemy* en) {
    string intend = en->intention;
    int tmpnum = 0;
    string ret = "";
    for (int i = 0; i < EFFECT_LENGTH - 1; i++) {
        if (intend[i] == '\0') break;
        if (isdigit(intend[i])) tmpnum += (intend[i] - '0');
        else {                                                           // multiply enemy damage by player taken damage
            if (intend[i] == 'D') { ret += ("Attack for " + std::to_string(pl->mult_dmg_to(en->mult_dmg_from(tmpnum))) + " damage. || "); tmpnum = 0; }
            else if (intend[i] == 'B') { ret += ("Apply " + std::to_string(en->mult_block(tmpnum)) + " block. || "); tmpnum = 0; }
            else if (intend[i] == 'S') { ret += ("Gain " + std::to_string(tmpnum) + " strength. || "); tmpnum = 0; }
            else if (intend[i] == 'w') { ret += ("Weaken " + std::to_string(tmpnum) + " turns. || "); tmpnum = 0; }
        }
    }

    ret.erase(ret.end() - 3, ret.end());
    return ret;
}

// discard entire hand
void discard_hand(player* pl, pile* pl_cards) {
    while (pl_cards->hand.size() > 0) {
        pl_cards->discard.push_back(pl_cards->hand.at(0));
        pl_cards->hand.erase(pl_cards->hand.begin());
    }
}

// Check if element is present in vector
bool is_in_vector(vector<int> vec, int c) {
    bool found = false;
    for (int i = 0; i < vec.size(); i++) {
        if (vec.at(i) == c) {
            found = true;
            break;
        }
    }
    return found;
}

// get indexes of matching substrings
vector<int> get_substring_index(string data, string sub) {
    bool f;
    vector<int> ret;
    for (int i = 0; i < data.size() - sub.size(); i++) {
        f = true;
        for (int j = 0; j < sub.size(); j++) {
            if (data.at(i + j) != sub.at(j)) f = false;
        }
        if (f)
            ret.push_back(i);
    }
    return ret;
}

// Get card description
string get_card_desc(player* pl, enemy* en, card cr) {
    vector<int> vecdmg;
    vector<int> vecblc;
    string desc = cr.desc;
    vecdmg = get_substring_index(cr.desc, "_d_");
    vecblc = get_substring_index(cr.desc, "_b_");
    vector<int> vec = vecdmg;
    vec.insert(vec.begin(), vecblc.begin(), vecblc.end());
    std::sort(vec.begin(), vec.end());
    for (int i = vec.size()-1; i >= 0; i--) {
        if (is_in_vector(vecdmg, vec.at(i))) {
            desc.insert(vec.at(i)+3, std::to_string(en->mult_dmg_to(pl->mult_dmg_from(cr.values.at(i))))); // Multiply damage
        }
        if (is_in_vector(vecblc, vec.at(i))) {
            desc.insert(vec.at(i)+3, std::to_string(pl->mult_block(cr.values.at(i)))); // Multiply block
        }
    }
    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < 3; j++) {
            if (!isdigit(desc.at(vec.at(i))))
                desc.erase(desc.begin() + vec.at(i), desc.begin() + vec.at(i)+1);
        }
    }
    return desc;
}

// Prints the entire game screen
// This function is really ugly
void print_game(player* pl, pile* pl_cards, enemy* en) {
    cls();
    vector<card> tmpdraw = pl_cards->draw;
    vector<int> vdmg;
    vector<int> vblc;
    vector<string> draw_descs;
    for (long unsigned int i = 0; i < pl_cards->hand.size(); i++) {
         draw_descs.push_back(get_card_desc(pl, en, pl_cards->hand.at(i)));
    }
    cout << colors.green << "Act: " << pl->act + 1 << "/3" << "\t\t\t" << "Level: " << pl->level + 1 << colors.red
        << "\t\t\tDeck: " << pl_cards->deck.size() << " cards";
    cout << colors.yellow << "\t\t\tGold: " << pl->gold << colors.end << "\n";
    cout << string(111, '-') << "\n";
    cout << colors.yellow << pl->name << " the ironclad\t\t\t\t\t\t\t\t\t\t" << en->name << colors.end << std::endl;
    cout << "Draw pile: " << colors.yellow << pl_cards->draw.size() << " cards\t\t\t\t\t\t\t\t\t\t" << colors.end << "Discard pile: "
        << colors.yellow << pl_cards->discard.size() << " cards\n" << colors.end;
    cout << "HP: " << colors.green << pl->hp << colors.end << "\t\t\t\t\t\t\t\t\t\t\t\tEnemy HP: " << colors.green << en->hp << colors.end << "\n";
    cout << "Block: " << colors.cyan << pl->block << colors.end << "\t\t\t\t\t\t\t\t\t\t\tEnemy block: " << colors.cyan << en->block << colors.end << "\n";
    cout << "Mana: " << colors.magenta << pl->mana << colors.end << "\t\t\t\t\t\t\t\t\t\t\t\tEnemy intent: " << colors.magenta << enemy_intention_to_string(pl, en) << colors.end << "\n";
    cout << string(111, '-') << "\n";
    string mydesc;
    int cnt = 0; // Count number of cards so that message buffer is always at the same height
    for (unsigned long int i = 0; i < pl_cards->hand.size(); i++) {
        cnt++;
        mydesc = draw_descs.at(i);
        while (mydesc.length() < 60) mydesc += " ";
        cout << pl_cards->hand.at(i).color << "(" << i + 1 << ") " << pl_cards->hand.at(i).name << colors.end << "\t\t\t:: "
            << mydesc << colors.magenta
            << "\t" << ":: Mana cost: " << pl_cards->hand.at(i).cost << colors.end << std::endl;
    }
    cout << "\n\n\n";
    cout << "weak: " << colors.magenta << pl->weak << colors.end << "\t\tstr: " << colors.red << pl->strength << colors.end
        << "\t\tpoison: " << colors.green << pl->poison << colors.end << "\n";
    cout << "enemy weak: " << colors.magenta << en->weak << colors.end << "\tenemy str: "
        << colors.red << en->strength << colors.end << "\tenemy poison: " << colors.green << en->poison << colors.end << "\n";

    cout << string(pl->drawlimit - cnt, '\n') << msgbuffer << "\n";
}

// Start of turn function
void start_turn(player* pl, pile* pl_cards) {
    pl->begin_turn(pl_cards);
}

void eval_card(player* pl, pile* pl_pile, enemy* en, card crd) {
    pl->mana -= crd.cost;
    print_game(pl, pl_pile, en);
    char tmpef[EFFECT_LENGTH];
    strcpy(tmpef, crd.effect);
    tmpef[strlen(tmpef) - 1] = '\0'; // We need to remove the last char of the string, since it stores return value of card and is not part of effect
    eval_effect(tmpef, pl, en, pl_pile);
}

// discard x card from hand
void discard_from_hand(pile* pl_cards, int index) {
    pl_cards->discard.push_back(pl_cards->hand.at(index));
    pl_cards->hand.erase(pl_cards->hand.begin() + index);
}

// same thing but exhaust
void exhaust_from_hand(pile* pl_cards, int index) {
    pl_cards->hand.erase(pl_cards->hand.begin() + index);
}

// last char is return value ([D]iscard, [E]xhaust, [R]eturn to hand) -- NOT IN THIS FUNC ANYMORE
// activate card from hand
void play_card_from_hand(player* pl, pile* pl_cards, enemy* en, int index) {
    if (pl->mana >= pl_cards->hand.at(index).cost) {
        card cr = pl_cards->hand.at(index); // Card has to be discarded / exhausted before effect is evaluated, so we make a copy we use to evaluate
        cr = pl_cards->hand.at(index);
        if (pl_cards->hand.at(index).effect[strlen(pl_cards->hand.at(index).effect) - 1] == 'D') // Discard card
            discard_from_hand(pl_cards, index);
        else if (pl_cards->hand.at(index).effect[strlen(pl_cards->hand.at(index).effect) - 1] == 'E') // Exhaust card
            exhaust_from_hand(pl_cards, index);
        eval_card(pl, pl_cards, en, cr);
    }
    else {
        buffer_queue("Not enough mana to play card.");
    }
}

void create_fight(player* pl, pile* plc, enemy* en_main) {
    start_fight(pl, plc);
    bool fight = true;
    char choice = '0';
    while (fight) {
        start_turn(pl, plc);
        en_main->get_intention();
        en_main->decrease_counters();
        pl->decrease_counters();
        print_game(pl, plc, en_main);
        while (choice != 'q') {
            choice = key_press();
            if (choice > '0' && choice < (char)('1' + plc->hand.size()))
                play_card_from_hand(pl, plc, en_main, (int)(choice - '0') - 1);
            if (!en_main->check_hp()) {
                cout << colors.red << "You kill the " << en_main->name << "!\n" << colors.end;
                fight = false;
                break;
            }
            print_game(pl, plc, en_main);
        }
        en_main->begin_turn();
        en_main->commit_intention(pl, plc);
        choice = '0';
    }
}

// TODO: * read from some kind of proper database
//       * balance the game properly
void init_game(vector<enemy>* env, vector<card>* crds) {
    // +100 to level is elite
    // Levels 0-2 are all act one, just different difficulties
    // Name, HP, level, effects
    env->push_back(enemy("Goblin", 20, 0, { "6Da","5Ba" }));
    env->push_back(enemy("Goblin", 30, 1, { "8Da","9Ba","9Da" }));
    env->push_back(enemy("Cultist", 30, 1, { "8Da","7Ba","2Sa" }));
    env->push_back(enemy("Cultist", 30, 2, { "12Da","10Ba","2Sa" }));
    env->push_back(enemy("Hobgoblin", 30, 1, { "8Da","7Ba","2wa" }));
    env->push_back(enemy("Strong goblin", 70, 101, { "93Da","91Ba","93Da","93Da","91Ba","6Sa" }));

    // Add after all non-upgraded cards!
    // name - desc - effect - mana - rarity - color - type (0 attack, 1 skill)
    // '+' after card name means upgraded ! each upgraded card has to follow this naming !
    // TODO: shuffle curse, vulnerable, can't draw more this turn
    crds->push_back(card("Strike", "Deal _d_ damage", {5}, "5daD", 1, 0, colors.red, 0));
    crds->push_back(card("Defend", "Get _b_ block", {5}, "5baD", 1, 0, colors.cyan, 1));
    crds->push_back(card("Iron mask", "Get _b_ block and discard another card", {10}, "91ba1caD", 1, 1, colors.cyan, 1));
    crds->push_back(card("Fear strike", "Deal _d_ damage and apply 1 weak", {3}, "3da1WaD", 0, 0, colors.magenta, 0));
    crds->push_back(card("Instinct", "Deal _d_ damage, if enemy is weak gain 1 mana", {4}, "4d1mwD", 1, 1, colors.red, 0));
    crds->push_back(card("Pain", "Deal _d_ damage and apply 2 weak", {12}, "93da1WaD", 1, 1, colors.red, 0));
    crds->push_back(card("Shockwave", "Deal _d_ damage and gain _b_ block", {4,4}, "4da4baD", 1, 1, colors.cyan, 1));
    crds->push_back(card("Clean strike", "Deal _d_ damage and draw one card", {6}, "6da1CaD", 1, 1, colors.red, 0));
    crds->push_back(card("Boomerang", "Deal _d_ damage 3 times and take 3 damage", {3}, "3da3da3da3DaD", 1, 1, colors.red, 0));
    crds->push_back(card("Claw", "Deal _d_ damage twice", {4},  "4da4daD", 1, 1, colors.red, 0));
    crds->push_back(card("Sacrifice", "Gain 2 mana, lose 3 health", {}, "2ma3DaD", 1, 1, colors.magenta, 1));
    crds->push_back(card("Crack the sky", "Deal _d_ damage, exhaust", {25}, "997daE", 1, 1, colors.red, 0));
    crds->push_back(card("Disarm", "Enemy loses 4 strength", {}, "4laD", 1, 1, colors.red, 0));
}

vector<card> cards; // another global variable...
std::vector<enemy> enemies;
void upgrade_card(pile* plc, int index) {
    for (long unsigned int i = 0; i < cards.size(); i++) {
        if (cards.at(i).name == plc->deck.at(index).name + "+")
            plc->deck.at(index) = cards.at(i);
    }
}

// Pick random enemy from act
enemy pick_enemy(player* pl) {
    enemy en = *select_randomly(enemies.begin(), enemies.end());
    while (en.level != pl->nlevel)
        en = *select_randomly(enemies.begin(), enemies.end());
    return en;
}

bool one_chance_in(int max) {
    if (rand() % (max - 1 + 1) + 1) return true;
    else return false;
}

card select_random_card() {
    card cr = *select_randomly(cards.begin(), cards.end());
    while (true) {
        if (cr.rarity == 0) // don't select started cards
            cr = *select_randomly(cards.begin(), cards.end());
        if (cr.rarity == 2) {
            if (!one_chance_in(3))
                cr = *select_randomly(cards.begin(), cards.end());
            else break;
        }
        if (cr.rarity == 3) {
            if (one_chance_in(3))
                cr = *select_randomly(cards.begin(), cards.end());
            else break;
        }
    }
    return cr;
}

// Pick random elite enemy from act
enemy pick_elite_enemy(player* pl) {
    enemy en = *select_randomly(enemies.begin(), enemies.end());
    while (en.level != pl->nlevel + 100) // +100 is elites
        en = *select_randomly(enemies.begin(), enemies.end());
    return en;
}

// Create deck at the beginning of the game
pile create_deck(vector<card> crds) {
    pile pl_pile;
    for (int i = 0; i < 3; i++)
        pl_pile.deck.push_back(crds.at(0)); // strike
    for (int i = 0; i < 3; i++)
        pl_pile.deck.push_back(crds.at(1)); // defend
    pl_pile.deck.push_back(crds.at(2)); // iron mask
    pl_pile.deck.push_back(crds.at(3)); // fear strike
    return pl_pile;
}

// Create player at the beginning of the game
player create_player() {
    player pl;
    cout << "Enter your name:\n";
    cin >> pl.name;
    return pl;
}

// Split a string into a vector of strings by char split
vector<string> split_string(string data, char split) {
    vector<string> v;
    string tmp;
    std::stringstream ss(data);
    while(getline(ss, tmp, split)){
        v.push_back(tmp);
    }
    return v;
}

// same thing but get integer vector returned
vector<int> split_string_get_int(string data, char split) {
    vector<int> v;
    string tmp;
    std::stringstream ss(data);
    while(getline(ss, tmp, split)){
        v.push_back(stoi(tmp));
    }
    return v;
}

vector_tree<string> read_tree_from_file(string filename, int offset){
	std::ifstream file;
	file.open(filename);
	string line;

    for (int i = 0; i < offset; i++) getline(file, line);

    vector<int> relations;
    vector<string> names;

	getline(file, line);
    relations = split_string_get_int(line, '^');
	getline(file, line);
    names = split_string(line, '^');

    vector_tree<string> ret;
    ret.setVectors(relations, names);
	file.close();
    return ret;
}

void random_encounter() {
    // do a random encounter
}

// TODO: get card after battle
//       get special card after elite
// encounters, shops
int main() {
    srand(time(NULL));
    init_game(&enemies, &cards); // global variables

    // Initialize player and deck
    player pl = create_player();
    pile pl_pile = create_deck(cards);
    // Initialize enemy
    enemy en_main = pick_enemy(&pl);

    while (pl.act != 2) {
        while (pl.level != 10) {
            create_fight(&pl, &pl_pile, &en_main);
            getchar();
            getchar();
            en_main = pick_enemy(&pl);
            pl.level++;
            if (pl.level == 2 || pl.level == 6)
                pl.nlevel++;
        }
        pl.act++;
    }
}
