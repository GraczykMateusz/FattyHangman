#include <iostream>
#include <random>
#include <boost/property_tree/json_parser.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "Hangman.h"
#include "Game.h"

boost::property_tree::ptree Hangman::pt;

Hangman::Hangman() {
  try {
    this->pt = Game::getPt();
    boost::property_tree::read_json(Game::getQuestionsPath(), pt);
  } catch(const std::exception& e) {
    std::cout << "Make sure the location of the questions file is there:\n"
              << Game::getQuestionsPath
              << "\n";
  }
}

bool Hangman::startGame() {
  try {
    boost::property_tree::read_json(Game::getQuestionsPath(), pt);
  } catch(const std::exception& e) {
    std::cout << "Make sure the location of the questions file is there:\n"
              << Game::getQuestionsPath
              << "\n";
    back();
    return false;
  }
  try {
    if(pt.empty())
      throw "Cannot start! Empty questions file!\n";
  } catch(const char* err) {
    std::cout << err;
    back();
    return false;
  }

  int questIterNum;
  auto randQuest = [&questIterNum]() {
    std::random_device rd;
    std::mt19937 gen(rd());

    int i = pt.size();
    std::uniform_int_distribution<> disIterNum(0, --i);

    questIterNum = disIterNum(gen);
  };
  randQuest();
  
  boost::property_tree::ptree::iterator it = pt.begin();

  for(int i = 0; i < questIterNum; ++i) 
    ++it;

  id = it->first;
  std::cout << "Id = "
            << id
            << "\n";

  auto it2 = it->second.begin();
  
  question = it2->second.get_value < std::string > ();
  std::cout << it2->first
            << " = "
            << question 
            << "\n";

  ++it2;

  answer = it2->second.get_value < std::string > ();
  std::cout << it2->first
            << " = "
            << answer
            << "\n";
  
  std::cout << answer.length();

  back();
  return true;
}
