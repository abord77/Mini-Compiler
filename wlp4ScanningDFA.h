#ifndef __WLP4SCANNINGDFA_H__
#define __WLP4SCANNINGDFA_H__
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include "scanningDFA.h"

class Wlp4ScanningDFA : public ScanningDFA {
    const std::string wlp4TokenDFA = 
".ALPHABET \n\
!-~ \n\
.STATES \n\
start \n\
LPAREN! \n\
RPAREN! \n\
LBRACE! \n\
RBRACE! \n\
BECOMES! \n\
EQ! \n\
NE! \n\
saw_exMark \n\
LT! \n\
GT! \n\
LE! \n\
GE! \n\
PLUS! \n\
MINUS! \n\
STAR! \n\
SLASH! \n\
PCT! \n\
COMMA! \n\
SEMI! \n\
LBRACK! \n\
RBRACK! \n\
AMP! \n\
seen_i! \n\
IF! \n\
seen_in! \n\
INT! \n\
seen_w! \n\
seen_wa! \n\
seen_wai! \n\
WAIN! \n\
seen_wh! \n\
seen_whi! \n\
seen_whil! \n\
WHILE! \n\
seen_e! \n\
seen_el! \n\
seen_els! \n\
ELSE! \n\
seen_p! \n\
seen_pr! \n\
seen_pri! \n\
seen_prin! \n\
seen_print! \n\
seen_printl! \n\
PRINTLN! \n\
seen_pu! \n\
seen_put! \n\
seen_putc! \n\
seen_putch! \n\
seen_putcha! \n\
PUTCHAR! \n\
seen_g! \n\
seen_ge! \n\
seen_get! \n\
seen_getc! \n\
seen_getch! \n\
seen_getcha! \n\
GETCHAR! \n\
seen_r! \n\
seen_re! \n\
seen_ret! \n\
seen_retu! \n\
seen_retur! \n\
RETURN! \n\
seen_N! \n\
seen_NU! \n\
seen_NUL! \n\
NULL! \n\
seen_n! \n\
seen_ne! \n\
NEW! \n\
seen_d! \n\
seen_de! \n\
seen_del! \n\
seen_dele! \n\
seen_delet! \n\
DELETE! \n\
ID! \n\
NUM_0! // seen 0 \n\
NUM_1! // seen 1-9 \n\
NUM_2! // seen [1-9][0-9]* \n\
saw_/ \n\
COMMENT! \n\
.TRANSITIONS \n\
start           (               LPAREN \n\
start           )               RPAREN \n\
start           {               LBRACE \n\
start           }               RBRACE \n\
start           =               BECOMES \n\
BECOMES         =               EQ \n\
start           !               saw_exMark \n\
saw_exMark      =               NE \n\
start           <               LT \n\
start           >               GT \n\
LT              =               LE \n\
GT              =               GE \n\
start           +               PLUS \n\
start           -               MINUS \n\
start           *               STAR \n\
start           /               SLASH \n\
start           %               PCT \n\
start           ,               COMMA \n\
start           ;               SEMI \n\
start           [               LBRACK \n\
start           ]               RBRACK \n\
start           &               AMP \n\
start           0               NUM_0 \n\
start           1-9             NUM_1 \n\
NUM_1           0-9             NUM_2 \n\
NUM_2           0-9             NUM_2 \n\
start           a-c f h j-m o q s-v x-z A-M O-Z  ID \n\
ID              a-z A-Z 0-9     ID \n\
start           d               seen_d \n\
seen_d          e               seen_de \n\
seen_d          a-d f-z A-Z 0-9 ID \n\
seen_de         l               seen_del \n\
seen_de         a-k m-z A-Z 0-9 ID \n\
seen_del        e               seen_dele \n\
seen_del        a-d f-z A-Z 0-9 ID \n\
seen_dele       t               seen_delet \n\
seen_dele       a-s u-z A-Z 0-9 ID \n\
seen_delet      e               DELETE \n\
seen_delet      a-d f-z A-Z 0-9 ID \n\
DELETE          a-z A-Z 0-9     ID \n\
start           e               seen_e \n\
seen_e          l               seen_el \n\
seen_e          a-k m-z A-Z 0-9 ID \n\
seen_el         s               seen_els \n\
seen_el         a-r t-z A-Z 0-9 ID \n\
seen_els        e               ELSE \n\
seen_els        a-d f-z A-Z 0-9 ID \n\
ELSE            a-z A-Z 0-9     ID \n\
start           g               seen_g \n\
seen_g          e               seen_ge \n\
seen_g          a-d f-z A-Z 0-9 ID \n\
seen_ge         t               seen_get \n\
seen_ge         a-s u-z A-Z 0-9 ID \n\
seen_get        c               seen_getc \n\
seen_get        a-b d-z A-Z 0-9 ID \n\
seen_getc       h               seen_getch \n\
seen_getc       a-g i-z A-Z 0-9 ID \n\
seen_getch      a               seen_getcha \n\
seen_getch      b-z A-Z 0-9     ID \n\
seen_getcha     r               GETCHAR \n\
seen_getcha     a-q s-z A-Z 0-9 ID \n\
GETCHAR         a-z A-Z 0-9     ID \n\
start           i               seen_i \n\
seen_i          f               IF \n\
seen_i          a-e g-m o-z A-Z 0-9 ID \n\
IF              a-z A-Z 0-9     ID \n\
seen_i          n               seen_in \n\
seen_i          a-e g-m o-z A-Z 0-9 ID \n\
seen_in         t               INT \n\
seen_in         a-s u-z A-Z 0-9 ID \n\
INT             a-z A-Z 0-9     ID \n\
start           n               seen_n \n\
seen_n          e               seen_ne \n\
seen_n          a-d f-z A-Z 0-9 ID \n\
seen_ne         w               NEW \n\
seen_ne         a-v x-z A-Z 0-9 ID \n\
NEW             a-z A-Z 0-9     ID \n\
start           p               seen_p \n\
seen_p          r               seen_pr \n\
seen_p          a-q s-t v-z A-Z 0-9 ID \n\
seen_pr         i               seen_pri \n\
seen_pr         a-h j-z A-Z 0-9 ID \n\
seen_pri        n               seen_prin \n\
seen_pri        a-m o-z A-Z 0-9 ID \n\
seen_prin       t               seen_print \n\
seen_prin       a-s u-z A-Z 0-9 ID \n\
seen_print      l               seen_printl \n\
seen_print      a-k m-z A-Z 0-9 ID \n\
seen_printl     n               PRINTLN \n\
seen_printl     a-m o-z A-Z 0-9 ID \n\
PRINTLN         a-z A-Z 0-9     ID \n\
seen_p          u               seen_pu \n\
seen_p          a-q s-t v-z A-Z 0-9 ID \n\
seen_pu         t               seen_put \n\
seen_pu         a-s u-z A-Z 0-9 ID \n\
seen_put        c               seen_putc \n\
seen_put        a-b d-z A-Z 0-9 ID \n\
seen_putc       h               seen_putch \n\
seen_putc       a-g i-z A-Z 0-9 ID \n\
seen_putch      a               seen_putcha \n\
seen_putch      b-z A-Z 0-9     ID \n\
seen_putcha     r               PUTCHAR \n\
seen_putcha     a-q s-z A-Z 0-9 ID \n\
PUTCHAR         a-z A-Z 0-9     ID \n\
start           r               seen_r \n\
seen_r          e               seen_re \n\
seen_r          a-d f-z A-Z 0-9 ID \n\
seen_re         t               seen_ret \n\
seen_re         a-s u-z A-Z 0-9 ID \n\
seen_ret        u               seen_retu \n\
seen_ret        a-t v-z A-Z 0-9 ID \n\
seen_retu       r               seen_retur \n\
seen_retu       a-q s-z A-Z 0-9 ID \n\
seen_retur      n               RETURN \n\
seen_retur      a-m o-z A-Z 0-9 ID \n\
RETURN          a-z A-Z 0-9     ID \n\
start           w               seen_w \n\
seen_w          a               seen_wa \n\
seen_w          b-g i-z A-Z 0-9 ID \n\
seen_wa         i               seen_wai \n\
seen_wa         a-h j-z A-Z 0-9 ID \n\
seen_wai        n               WAIN \n\
seen_wai        a-m o-z A-Z 0-9 ID \n\
WAIN            a-z A-Z 0-9     ID \n\
seen_w          h               seen_wh \n\
seen_w          b-g i-z A-Z 0-9 ID \n\
seen_wh         i               seen_whi \n\
seen_wh         a-h j-z A-Z 0-9 ID \n\
seen_whi        l               seen_whil \n\
seen_whi        a-k m-z A-Z 0-9 ID \n\
seen_whil       e               WHILE \n\
seen_whil       a-d f-z A-Z 0-9 ID \n\
WHILE           a-z A-Z 0-9     ID \n\
start           N               seen_N \n\
seen_N          U               seen_NU \n\
seen_N          a-z A-T V-Z 0-9 ID \n\
seen_NU         L               seen_NUL \n\
seen_NU         a-z A-K M-Z 0-9 ID \n\
seen_NUL        L               NULL \n\
seen_NUL        a-z A-K M-Z 0-9 ID \n\
NULL            a-z A-Z 0-9     ID \n\
SLASH           /               COMMENT \n\
COMMENT         !-~             COMMENT";

    bool isChar(std::string s);
    bool isInRange(std::string s);
    void initAlphabet(std::istringstream &dfa) override;
    void initStates(std::istringstream &dfa) override;
    void initTransitions(std::istringstream &dfa) override;

    public:
        Wlp4ScanningDFA();
        void init();
        bool findTransition(std::string &currentState, char transitionLetter) override;
        int findState(std::string currentState) override;
        std::pair<std::string, stateType> getState(int index) override;
};

#endif
