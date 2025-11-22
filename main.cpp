#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <utility>
#include <algorithm>
void put_data(int &states, int &init_state, std::vector<int> &final_state,
              std::multimap<std::pair<int, std::string>, int> &dfa_nfa_pair)
{
  std::fstream file("./check.txt");
  if (file.is_open())
  {
    for (int i = 0; i < 3; i++)
    {
      if (i == 2)
      {
        std::string hold;
        std::getline(file, hold);
        std::getline(file, hold);
        std::stringstream ss(hold);
        while (!ss.eof())
        {
          std::string number;
          std::getline(ss, number, ' ');
          final_state.push_back(std::stoi(number));
        }
      }
      else if (i == 0)
      {
        file >> states;
      }
      else
      {
        file >> init_state;
      }
    }
    std::string sub_hold;

    while (!file.eof() && std::getline(file, sub_hold))
    {
      int start;
      std::string transaction;
      int next;
      std::stringstream sub_ss(sub_hold);
      sub_ss >> start;
      sub_ss >> transaction;
      sub_ss >> next;
      dfa_nfa_pair.insert({{start, transaction}, next});
    }
    file.close();
  }
  else
  {
    std::cout << "there is an error to open file" << std::endl;
  }
}

bool check_dfa_nfa(std::string user_input, int init_state, std::vector<int> &final_state, std::multimap<std::pair<int, std::string>, int> dfa_nfa_pair)
{
  if (user_input.size() == 1)
  {
    int hold = dfa_nfa_pair.find({init_state, user_input})->second;
    if (std::find(final_state.begin(), final_state.end(), hold) != final_state.end())
      return true;
    else
      return false;
  }
  else if (dfa_nfa_pair.find({init_state, std::string() + user_input[0]}) != dfa_nfa_pair.end())
  {
    auto range = dfa_nfa_pair.equal_range({init_state, std::string() + user_input[0]});
    auto count = range.first;
    while (count != range.second)
    {
      auto check = count;
      check++;
      if (check != range.second && check->first.first == count->first.first && check->first.second==count->first.second )
      {
        count++;
        continue;
      }

      int next_state = count->second;
      std::string hold_string = user_input.substr(1, user_input.size() - 1);
      if (check_dfa_nfa(hold_string, next_state, final_state, dfa_nfa_pair))
      {
        return true;
      }
      count++;
    }
  }
    return false;
};

int main()
{
  int states = 0;
  int init_state = 0;
  std::vector<int> final_state;
  std::multimap<std::pair<int, std::string>, int> dfa_nfa_pair;
  put_data(states, init_state, final_state, dfa_nfa_pair);

  std::cout << "Total Number of States : " << states << std::endl;
  std::cout << "Initial State : q" << init_state << std::endl;
  std::cout << "Final States : " << std::endl;
  for (int i = 0; i < final_state.size(); i++)
  {
    std::cout << "q" << final_state[i] << std::endl;
  }
  std::cout << "Transactions" << std::endl;
  for (auto it = dfa_nfa_pair.begin(); it != dfa_nfa_pair.end(); it++)
  {
    std::cout << "q" << it->first.first << " -> " << it->first.second << " = q" << it->second << std::endl;
  }

  std::string user_input;
  std::cout << std::endl
            << "Enter String" << std::endl;
  while (std::cin >> user_input && user_input != "exit")
  {
    if (check_dfa_nfa(user_input, init_state, final_state, dfa_nfa_pair))

      std::cout << "String is Accepted" << std::endl;
    else
      std::cout << "String is rejected" << std::endl;

    std::cout << "Enter String" << std::endl;
  }
  return 0;
}