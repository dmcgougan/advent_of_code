/*
 * Problem 24, Advent of Code 2018
 * Danjel McGougan
 */

#include <format>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <source_location>
#include <unordered_set>
#include <vector>

using namespace std;

template <typename T>
static inline void CHECK(T&& ok, source_location loc = source_location::current())
{
    if (!static_cast<bool>(ok)) {
        cerr << std::format("CHECK failed at {}:{}\n", loc.file_name(), loc.line());
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    // Get input stream
    ifstream file;
    istream& in = (argc > 1 ? (file.open(argv[1]), file) : cin);
    CHECK(in);

    // Parse input
    struct group_t
    {
        int units;
        int hit_points;
        int attack_points;
        int initiative;
        string attack_type;
        unordered_set<string> weak;
        unordered_set<string> immune;
        int defender;
    };
    vector<vector<group_t>> initial_armies(2);
    {
        regex head_re(R"(^(\d+) units each with (\d+) hit points(?: \(([^)]*)\))? )"
                      R"(with an attack that does (\d+) (\w+) damage at initiative (\d+)$)");
        regex mod_re(R"((\w+) to ([^;]+))");
        regex split_re(R"([^\s,]+)");

        int g = 0;
        for (string line; getline(in, line);) {
            if (line.empty()) continue;
            if (line == "Immune System:") {
                g = 0;
                continue;
            }
            if (line == "Infection:") {
                g = 1;
                continue;
            }

            smatch m;
            CHECK(std::regex_match(line, m, head_re));

            group_t& group = initial_armies[g].emplace_back();
            group.units = std::stoi(m[1]);
            group.hit_points = std::stoi(m[2]);
            group.attack_points = std::stoi(m[4]);
            group.attack_type = m[5];
            group.initiative = std::stoi(m[6]);

            // Optional weak/immune block
            if (m[3].matched) {
                const string& mods = m[3];
                for (auto it = std::sregex_iterator(mods.begin(), mods.end(), mod_re);
                     it != std::sregex_iterator();
                     ++it)
                {
                    const string& kind = (*it)[1];
                    const string& list = (*it)[2];
                    auto& target = (kind == "weak" ? group.weak : group.immune);
                    for (auto jt = std::sregex_iterator(list.begin(), list.end(), split_re);
                         jt != std::sregex_iterator();
                         ++jt)
                    {
                        target.insert(jt->str());
                    }
                }
            }
        }
    }

    // Common
    auto combat = [&](int boost = 0) -> pair<int, bool> {
        vector<vector<group_t>> armies = initial_armies;
        for (auto& g : armies[0]) g.attack_points += boost;
        auto calc_damage = [](const group_t& attacker, const group_t& defender) -> int {
            if (defender.immune.contains(attacker.attack_type)) return 0;
            int result = attacker.units * attacker.attack_points;
            if (defender.weak.contains(attacker.attack_type)) result *= 2;
            return result;
        };
        for (;;) {
            // Target selection phase
            for (int a = 0; a < 2; ++a) {
                auto& army = armies[a];
                const auto& enemy = armies[1 - a];
                vector<bool> attacker_selected(army.size());
                vector<bool> defender_selected(enemy.size());
                for (;;) {
                    // Choose attacker
                    pair<int, int> max_ep{};
                    int attacker = 0;
                    for (size_t i = 0; i < army.size(); ++i) {
                        const auto& g = army[i];
                        if (!g.units || attacker_selected[i]) continue;
                        auto ep = make_pair(g.units * g.attack_points, g.initiative);
                        if (ep > max_ep) {
                            max_ep = ep;
                            attacker = i;
                        }
                    }
                    if (max_ep == make_pair(0, 0)) break;
                    attacker_selected[attacker] = true;
                    // Choose defender
                    tuple<int, int, int> max_damage{};
                    int defender = -1;
                    for (size_t i = 0; i < enemy.size(); ++i) {
                        const auto& g = enemy[i];
                        if (!g.units || defender_selected[i]) continue;
                        int damage = calc_damage(army[attacker], g);
                        if (!damage) continue;
                        auto damage_tuple =
                            make_tuple(damage, g.units * g.attack_points, g.initiative);
                        if (damage_tuple > max_damage) {
                            max_damage = damage_tuple;
                            defender = i;
                        }
                    }
                    if (defender >= 0) defender_selected[defender] = true;
                    army[attacker].defender = defender;
                }
            }

            // Attacking phase
            set<pair<int, int>> attacker_selected;
            int total_kills = 0;
            for (;;) {
                int max_initiative = -1;
                int selected_army = 0;
                int selected_group = 0;
                for (size_t a = 0; a < 2; ++a) {
                    const auto& army = armies[a];
                    for (size_t i = 0; i < army.size(); ++i) {
                        const auto& g = army[i];
                        if (!g.units || g.defender < 0 || attacker_selected.contains({a, i}))
                            continue;
                        if (g.initiative > max_initiative) {
                            max_initiative = g.initiative;
                            selected_army = a;
                            selected_group = i;
                        }
                    }
                }
                if (max_initiative < 0) break;
                attacker_selected.insert({selected_army, selected_group});

                // Calculate kills
                const auto& attacker = armies[selected_army][selected_group];
                CHECK(attacker.defender >= 0);
                auto& defender = armies[1 - selected_army][attacker.defender];
                int damage = calc_damage(attacker, defender);
                int kills = min(defender.units, damage / defender.hit_points);
                defender.units -= kills;
                total_kills += kills;
            }

            // Check if there were no kills during this round
            if (!total_kills) break;
        }

        // Get total remaining units; also check if all infections are gone
        int units = 0;
        for (size_t i = 0; i < armies[0].size(); ++i) {
            units += armies[0][i].units;
        }
        bool infection_gone = true;
        for (size_t i = 0; i < armies[1].size(); ++i) {
            units += armies[1][i].units;
            if (armies[1][i].units) {
                infection_gone = false;
            }
        }

        return make_pair(units, infection_gone);
    };

    // Solve part 1
    int part1 = combat().first;

    // Solve part 2
    int part2 = 0;
    for (int boost = 1;; ++boost) {
        auto [units, won] = combat(boost);
        if (won) {
            part2 = units;
            break;
        }
    }

    // Output answers
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    return 0;
}
