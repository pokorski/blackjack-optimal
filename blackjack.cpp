#include <bits/stdc++.h>
using namespace std;

const int VALUES[] = {2,3,4,5,6,7,8,9,10,10,10,10,11};

double value_fn(char card) {  // input should be 2,3,4,5,6,7,8,9,T,J,Q,K or A
    if (card == 'A') return 11;
    if ((card == 'T') || (card == 'J') || (card == 'Q') || (card == 'K')) return 10;
    return card - '0';
}

double dealer_check(int my_points, int dealer_points, int dealer_can_reduce) {
    if (my_points > 21) return -1.0;
    if (dealer_points > 21) {
        if (dealer_can_reduce > 0) return dealer_check(my_points, dealer_points - 10, dealer_can_reduce - 1);
        return 1.0;
    }
    if (dealer_points >= 17) {
        if (my_points > dealer_points) return 1.0;
        if (my_points < dealer_points) return -1.0;
        return 0.0;
    }
    double result = 0.0;
    for (int r = 0; r < 13; r++)
        result += dealer_check(my_points, dealer_points + VALUES[r], dealer_can_reduce + ((r == 12) ? 1 : 0)) / 13.0;
    return result;
}

double best_strategy(int my_points, bool can_reduce, bool can_split, bool can_double, int dealer_points, bool dealer_can_reduce, bool printing) {
    if (my_points > 21) {
        if (!can_reduce) return -1.0;
        return best_strategy(my_points - 10, false, false, can_double, dealer_points, dealer_can_reduce, false);
    }
    if ((my_points == 21) && (can_double)) {
        double value_if_blackjack = 0.0;
        for (int r = 0; r < 13; r++) {
            int ace_points = dealer_points + VALUES[r];
            if ((ace_points > 21) && (dealer_can_reduce || (r == 12))) ace_points -= 10;
            value_if_blackjack += ((ace_points < 21) ? 1.5 : 0.0) / 13.0;
        }
        if (printing)
            cout << "blackjack EV: " << fixed << setprecision(3) << value_if_blackjack << "\n";
        return value_if_blackjack;
    }

    double best_result = std::numeric_limits<double>::min();

    // hit
    double value_if_hit = 0.0;
    for (int r = 0; r < 13; r++)
        value_if_hit += best_strategy(my_points + VALUES[r], can_reduce || (r == 12), false, false, dealer_points, dealer_can_reduce, false) / 13.0;
    best_result = max(best_result, value_if_hit);
    if (printing)
        cout << "if hit we get EV: " << fixed << setprecision(3) << value_if_hit << "\n";

    // stand
    double value_if_stand = dealer_check(my_points, dealer_points, dealer_can_reduce);
    best_result = max(best_result, value_if_stand);
    if (printing)
        cout << "if stand we get EV: " << fixed << setprecision(3) << value_if_stand << "\n";

    // split
    double value_if_split = 0.0;
    if (can_split) {
        value_if_split = 2.0 * best_strategy(my_points / 2, can_reduce, false, false, dealer_points, dealer_can_reduce, false);
        best_result = max(best_result, value_if_split);
        if (printing)
            cout << "if split we get EV: " << fixed << setprecision(3) << value_if_split << "\n";
    }

    // double
    double value_if_double = 0.0;
    if (can_double) {
        for (int r = 0; r < 13; r++) {
            int ace_points = my_points + VALUES[r];
            if ((ace_points > 21) && (can_reduce || (r == 12))) ace_points -= 10;
            value_if_double += 2.0 * dealer_check(ace_points, dealer_points, dealer_can_reduce) / 13.0;
        }
        best_result = max(best_result, value_if_double);
        if (printing)
            cout << "if double we get EV: " << fixed << setprecision(3) << value_if_double << "\n";
    }

    return best_result;
}

int main()
{
    string my_cards;
    cin >> my_cards;  // for example "JQ" or "T7" (no spaces, at least two cards)

    char dealer_card;
    cin >> dealer_card;  // for example "A" or "3" (just one character)

    int my_points = 0;
    bool can_reduce = false;
    for (char card : my_cards) {
        my_points += value_fn(card);
        can_reduce |= (card == 'A');
        if (my_points > 21) { my_points -= 10; can_reduce = false; }
    }
    bool can_split = ((my_cards.length() == 2) && (my_cards[0] == my_cards[1]));
    bool can_double = (my_cards.length() == 2);

    int dealer_points = value_fn(dealer_card);
    bool dealer_can_reduce = (dealer_card == 'A');

    cout << fixed << setprecision(3) << best_strategy(my_points, can_reduce, can_split, can_double, dealer_points, dealer_can_reduce, true) << "\n";

    return 0;
}
