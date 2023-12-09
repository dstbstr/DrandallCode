#include "2023/d7_CamelCards.h"

SOLUTION(2023, 7) {
	enum struct Card : u32 { Ace, King, Queen, Jack, Ten, Nine, Eight, Seven, Six, Five, Four, Three, Two };
	enum struct Hand : u32 { Five, Four, Full, Three, Two, One, High };

	constexpr Hand FindHand(const std::vector<Card>&cards) {
		std::array<u32, 13> counts{};
		for (auto c : cards) {
			counts[static_cast<u32>(c)]++;
		}

		auto max = *std::max_element(counts.begin(), counts.end());
		if (max == 5) return Hand::Five;
		if (max == 4) return Hand::Four;
		if (max == 1) return Hand::High;

		auto pairs = std::count(counts.begin(), counts.end(), 2u);
		auto threes = std::count(counts.begin(), counts.end(), 3u);

		if (threes == 1 && pairs == 1) return Hand::Full;
		if (threes == 1) return Hand::Three;
		if (pairs == 2) return Hand::Two;
		if (pairs == 1) return Hand::One;

		throw "Wat?";
	}

	constexpr Hand FindHandJokers(const std::vector<Card>&cards) {
		std::array<u32, 13> counts{};
		for (auto c : cards) {
			counts[static_cast<u32>(c)]++;
		}

		auto jokers = counts[static_cast<u32>(Card::Jack)];
		counts[static_cast<u32>(Card::Jack)] = 0;

		auto max = *std::max_element(counts.begin(), counts.end());
		max += jokers;

		if (max == 5) return Hand::Five;
		if (max == 4) return Hand::Four;
		if (max == 1) return Hand::High;

		auto pairs = std::count(counts.begin(), counts.end(), 2u);
		auto threes = std::count(counts.begin(), counts.end(), 3u);

		if (jokers == 0) {
			if (threes == 1 && pairs == 1) return Hand::Full;
			if (threes == 1) return Hand::Three;
			if (pairs == 2) return Hand::Two;
			if (pairs == 1) return Hand::One;
		}
		else if (jokers == 1) {
			if (pairs == 2) return Hand::Full;
			if (pairs == 1) return Hand::Three;
			return Hand::One;
		}
		else if (jokers == 2) {
			if (pairs == 1) return Hand::Four;
			return Hand::Three;
		}

		throw "Wat?";
	}

	constexpr u32 NormalScore(Card card) {
		return static_cast<u32>(card);
	}

	constexpr u32 JokerScore(Card card) {
		if (card == Card::Jack) return 20;
		return NormalScore(card);
	}

	constexpr Card ToCard(char c) {
		switch (c) {
		case 'A': return Card::Ace;
		case 'K': return Card::King;
		case 'Q': return Card::Queen;
		case 'J': return Card::Jack;
		case 'T': return Card::Ten;
		case '9': return Card::Nine;
		case '8': return Card::Eight;
		case '7': return Card::Seven;
		case '6': return Card::Six;
		case '5': return Card::Five;
		case '4': return Card::Four;
		case '3': return Card::Three;
		case '2': return Card::Two;
		}
		throw "Wat?";
	}

	constexpr void SortHands(std::vector<std::pair<std::vector<Card>, u32>>&hands, auto HandFunc, auto ScoreFunc) {
		std::sort(hands.begin(), hands.end(), [&](const std::pair<std::vector<Card>, u32>& lhs, const std::pair<std::vector<Card>, u32>& rhs) {
			auto leftHand = HandFunc(lhs.first);
			auto rightHand = HandFunc(rhs.first);
			if (leftHand != rightHand) return static_cast<u32>(leftHand) < static_cast<u32>(rightHand);
			for (auto i = 0; i < lhs.first.size(); i++) {
				auto left = ScoreFunc(lhs.first[i]);
				auto right = ScoreFunc(rhs.first[i]);
				if (left != right) return left < right;
			}
			throw "Wat?";
			});
	}

	constexpr std::vector<std::pair<std::vector<Card>, u32>> ParseInput(const std::vector<std::string>&lines) {
		std::vector<std::pair<std::vector<Card>, u32>> result;
		for (const auto& line : lines) {
			auto s = Constexpr::Split(line, " ");
			std::vector<Card> cards;
			for (auto c : s[0]) {
				cards.push_back(ToCard(c));
			}

			u32 bid;
			Constexpr::ParseNumber(s[1], bid);
			result.emplace_back(cards, bid);
		}

		return result;
	}

	constexpr u32 Solve(const std::vector<std::string>&lines, auto HandFunc, auto ScoreFunc) {
		auto hands = ParseInput(lines);
		SortHands(hands, HandFunc, ScoreFunc);
		std::reverse(hands.begin(), hands.end());

		u32 result = 0;
		auto multiplier = 1;
		for (const auto& hand : hands) {
			result += (hand.second * multiplier++);
		}
		return result;
	}

	PART_ONE() {
		return Constexpr::ToString(Solve(lines, FindHand, NormalScore));
	}

	PART_TWO() {
		return Constexpr::ToString(Solve(lines, FindHandJokers, JokerScore));
	}

	TESTS() {
		std::vector<std::string> example = {
			"32T3K 765",
			"T55J5 684",
			"KK677 28",
			"KTJJT 220",
			"QQQJA 483"
		};

		if (PartOne(example) != "6440") return false;
		if (PartTwo(example) != "5905") return false;

		return true;
	}
}