
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <chrono>

int BLACKJACK = 21;
int DEALER_STAY = 17;
int ACE = 1;

class Card {
	public:
    char suit;
    int value;
	Card(char x, int y) {
		suit = x;
		value = y;
	}
};

class Deck {
	public:
	std::vector<Card> deckCards =  {Card('H',2),Card('H',3),Card('H',4),Card('H',5),Card('H',6),Card('H',7),Card('H',8),Card('H',9),Card('H',10),
	Card('H',10),Card('H',10),Card('H',10),Card('H',1), Card('D',2),Card('D',3),Card('D',4),Card('D',5),Card('D',6),Card('D',7),Card('D',8),Card('D',9),Card('D',10),
	Card('D',10),Card('D',10),Card('D',10),Card('D',1), Card('S',2),Card('S',3),Card('S',4),Card('S',5),Card('S',6),Card('S',7),Card('S',8),Card('S',9),Card('S',10),
	Card('S',10),Card('S',10),Card('S',10),Card('S',1),Card('C',2),Card('C',3),Card('C',4),Card('C',5),Card('C',6),Card('C',7),Card('C',8),Card('C',9),Card('C',10),
	Card('C',10),Card('C',10),Card('C',10),Card('C',1)};
	
	void Shuffle(std::vector<Card> &deck){
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
    }	
};

class Player {
    
    public: 
    int money = 100;
    std::vector<Card> playerCards;
    int sum_player_cards = 0;
    int sum_high_ace_count = 0;
    bool hasAce = false;
	bool usePlayerHighAceCount = false;
};

class Dealer { 
    public:
    std::vector<Card> dealerCards;
    int sum_dealer_cards = 0;
    int sum_high_ace_count = 0;
    bool hasAce = false;
	bool dealerBust = false;
	bool useDealerHighAceCount = false;
};

void InitialDeal(Player &player, Dealer &dealer, Deck &deck){
	player.playerCards.push_back(deck.deckCards.back());
 	deck.deckCards.pop_back();
 	dealer.dealerCards.push_back(deck.deckCards.back());
 	deck.deckCards.pop_back();
 	player.playerCards.push_back(deck.deckCards.back());
 	deck.deckCards.pop_back();
 	dealer.dealerCards.push_back(deck.deckCards.back());
 	deck.deckCards.pop_back();
}

void EvaluateScore(Player &player, Dealer &dealer, int wager){
	if (player.usePlayerHighAceCount && dealer.useDealerHighAceCount){	
		if (player.sum_high_ace_count > dealer.sum_high_ace_count){
			player.money = player.money + wager;
			std::cout << "You win " << wager << ". Your total is now " << player.money << std::endl;
		}
		else if (player.sum_high_ace_count == dealer.sum_high_ace_count){
			std::cout << "You push. Your total remains " << player.money << std::endl;
		}
		else {
			player.money = player.money - wager;
			std::cout << "You lose " << wager << ". Your total is now " << player.money << std::endl;
		}	
	}
	else if (player.usePlayerHighAceCount && !dealer.useDealerHighAceCount){
		if (player.sum_high_ace_count > dealer.sum_dealer_cards){
			player.money = player.money + wager;
			std::cout << "You win " << wager << ". Your total is now " << player.money << std::endl;
		}
		else if (player.sum_high_ace_count == dealer.sum_dealer_cards){
			std::cout << "You push. Your total remains " << player.money << std::endl;
		}
		else {
			player.money = player.money - wager;
			std::cout << "You lose " << wager << ". Your total is now " << player.money << std::endl;
		}	
	}
	else if (!player.usePlayerHighAceCount && dealer.useDealerHighAceCount){
		if (player.sum_player_cards > dealer.sum_high_ace_count){
			player.money = player.money + wager;
			std::cout << "You win " << wager << ". Your total is now " << player.money << std::endl;
		}
		else if (player.sum_player_cards == dealer.sum_high_ace_count){
			std::cout << "You push. Your total remains " << player.money << std::endl;
		}
		else {
			player.money = player.money - wager;
			std::cout << "You lose " << wager << ". Your total is now " << player.money << std::endl;
		}	
	}
	else {	
		if (player.sum_player_cards > dealer.sum_dealer_cards){
			player.money = player.money + wager;
			std::cout << "You win " << wager << ". Your total is now " << player.money << std::endl;
		}
		else if (player.sum_player_cards == dealer.sum_dealer_cards){
			std::cout << "You push. Your total remains " << player.money << std::endl;
		}
		else {
			player.money = player.money - wager;
			std::cout << "You lose " << wager << ". Your total is now " << player.money << std::endl;
		}		
	}	
}
 
int main()
{
	Player player;
	Dealer dealer;
	int wager;
    std::cout<<"Let's play Blackjack! You have " << player.money << " dollars, enter your wager" << std::endl;
	std::cin >> wager;
	
	while(std::cin.fail() || wager < 1){	
	
		std::cout<< "Error, please enter an integer greater than 1 and less than your remaining balance" << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin >> wager;		
	}
	
	if (wager >= player.money){
		wager = player.money;
		std::cout << "You wagered " << player.money << " you are all in!" << std::endl;
	}
	else{
		std::cout << "You wagered " << wager << std::endl;
	}
	
	// shuffle, initial deal
	
    Deck deck;
	deck.Shuffle(deck.deckCards);	
 	InitialDeal(player, dealer, deck);
 	
 	for (Card c: player.playerCards){
  	      player.sum_player_cards += c.value; 		  
  	}
	player.sum_high_ace_count = player.sum_player_cards + 10;
 	
 	if (player.playerCards.at(0).value == ACE || player.playerCards.at(1).value == ACE){
 	    player.hasAce = true;
 	}
 	
    if (player.hasAce){
        std::cout << "You have a total of " << player.sum_player_cards << " or " << player.sum_high_ace_count << std::endl;
    }

  	else {
  	    std::cout << "You have a " << player.playerCards.at(0).value << " and a " << player.playerCards.at(1).value << " totaling " <<
 	    player.sum_player_cards << std::endl;
  	}
  	
  	std::cout << "The dealer is showing a " << dealer.dealerCards.at(0).value << std::endl;
  	
  	// player input
  	
  	while (player.sum_player_cards <= BLACKJACK){
  	    char c;
      	std::cout << "Press h to hit or s to stay" << std::endl;
      	std::cin >> c;
      	while(std::cin.fail() && c != 'h' && c != 's'){	
    		std::cout<< "Error, please enter h or s" << std::endl;
    		std::cin.clear();
    		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    		std::cin >> c;		
    	}
    	
    	if (c == 'h'){
    	    player.playerCards.push_back(deck.deckCards.back());
			deck.deckCards.pop_back();
			
			if (player.playerCards.back().value == ACE && !player.hasAce){
				player.hasAce = true;
			}
    	    
    	    player.sum_player_cards += player.playerCards.back().value;
			player.sum_high_ace_count += player.playerCards.back().value;
			
			if (player.hasAce && player.sum_high_ace_count <= BLACKJACK){
				std::cout << "Hit! You got a " << player.playerCards.back().value << " totaling " << player.sum_player_cards << " or " << player.sum_high_ace_count << std::endl;
			}
			else {
				std::cout << "Hit! You got a " << player.playerCards.back().value << " totaling " << player.sum_player_cards << std::endl;
			}
			
	    }
	    
	    else if (c =='s') {
			
			if (player.hasAce && player.sum_high_ace_count <= BLACKJACK){
				std::cout << "Stay! Your total is " <<  player.sum_high_ace_count << std::endl;		
				player.usePlayerHighAceCount = true;
			}
			else {
				std::cout << "Stay! Your total is " <<  player.sum_player_cards << std::endl;
			}        
	        break;
	    }
  	}
  	
  	if (player.sum_player_cards > BLACKJACK){
  	    std::cout << "Bust! Better luck next time" << std::endl;
  	    player.money = player.money - wager;
  	    std::cout << "You lose " << wager << ". Your total is now " << player.money << std::endl;
		return 0;
  	}
  	
  	// show dealer hand, deal to dealer
  	
  	else{
  	    
  	    for (Card c: dealer.dealerCards){
  	      dealer.sum_dealer_cards += c.value;  
  	    }
		dealer.sum_high_ace_count = dealer.sum_dealer_cards + 10;
		
		if (dealer.dealerCards.at(0).value == ACE || dealer.dealerCards.at(1).value == ACE){
			dealer.hasAce = true;
		}
		
		if (dealer.hasAce){
			std::cout << "The Dealer has a total of " << dealer.sum_dealer_cards << " or " << dealer.sum_high_ace_count << std::endl;
		}

		else {
			std::cout << "The Dealer has a " << dealer.dealerCards.at(0).value << " and a " << dealer.dealerCards.at(1).value << " totaling " <<
			dealer.sum_dealer_cards << std::endl;
		}
			
		while (true)
		{		
			if (dealer.sum_dealer_cards > BLACKJACK) {
				std::cout << "Dealer busts with  " << dealer.sum_dealer_cards << std::endl;
				dealer.dealerBust = true;
				break;		
			}
			
			if (dealer.sum_dealer_cards >= DEALER_STAY){			
				std::cout << "Dealer stays with " << dealer.sum_dealer_cards << std::endl;
				break;
			}
			
			else if (dealer.hasAce && dealer.sum_high_ace_count >= DEALER_STAY && dealer.sum_high_ace_count <= BLACKJACK){
				std::cout << "Dealer stays with " << dealer.sum_high_ace_count << std::endl;
				dealer.useDealerHighAceCount = true;
				break;
			}
			
			else {
				dealer.dealerCards.push_back(deck.deckCards.back());
				deck.deckCards.pop_back();
				dealer.sum_dealer_cards += dealer.dealerCards.back().value;
				dealer.sum_high_ace_count += dealer.dealerCards.back().value;
				std::cout << "Dealer hits, got a " << dealer.dealerCards.back().value << std::endl;
			}
		}	
  	}
	
	// if you made it here, evaluate player vs dealer cards
	
	if (dealer.dealerBust){
		std::cout << "Player wins because of dealer bust" << std::endl;
		player.money = player.money + wager;
  	    std::cout << "You win " << wager << ". Your total is now " << player.money << std::endl;
	}

	else {
		EvaluateScore(player, dealer, wager);
	}

    return 0;
}

